#include "kstype.h"
#include "filter.h"
#include "dc_types_util.h"
#include "ipv6.h"

typedef enum 
{
    DHCPV6_MSG_SOLICIT =               1,
    DHCPV6_MSG_REQUEST =               3,
    DHCPV6_MSG_CONFIRM =               4,
    DHCPV6_MSG_RENEW =                 5,
    DHCPV6_MSG_REBIND =                6,
    DHCPV6_MSG_RELEASE =               8,
    DHCPV6_MSG_DECLINE =               9,
    DHCPV6_MSG_INFORMATION_REQUEST =   11
} dhcpv6_client_msg_t;

typedef enum 
{
    DHCPV6_MSG_ADVERTISE =             2,
    DHCPV6_MSG_REPLY =                 7,
    DHCPV6_MSG_RECONFIGURE =          10,
} dhcpv6_server_msg_t;

#define DHCPV6_MESSAGE_TYPE_SIZE 1
#define DHCPV6_XID_SIZE 3
#define DHCP_HOP_LIMIT 0x40

/* Option types as defined in RFC3315 */
#define DHCPV6_OPTION_CLIENTID      1
#define DHCPV6_OPTION_SERVERID      2
#define DHCPV6_OPTION_IA_NA         3
#define DHCPV6_OPTION_IA_ADDRESS    5
#define DHCPV6_OPTION_DNS_SERVERS   23


BOOLEAN dc_net_is_dhcpv6_packet(packet_info_t *pi)
{
    udp_header_t udphdr;
    UINT udp_offset;
    UCHAR bytes[DHCPV6_MESSAGE_TYPE_SIZE + DHCPV6_XID_SIZE];
    dc_status_t err;

    /* Calculate the UDP datagram offset and read the UDP header */
    udp_offset = pi->eth_datagram_offset + IPV6_IP_HEADER_SIZE;
    if (dc_net_get_packet_bytes(pi->packet, udp_offset, sizeof(udphdr),
        &udphdr))
    {
        printk(KERN_INFO DRV_NAME " %s: dc_net_get_packet_bytes failed\n", __func__);
        return FALSE;
    }

    /* Test if this is a dhcpv6 frame (UDP with src/dst ports 546/547) */
    if (*(USHORT *)udphdr.src_port != UDP_IPV6_CLIENT_PORT ||
        *(USHORT *)udphdr.dst_port != UDP_IPV6_SERVER_PORT)
    {
        return FALSE;
    }
    pi->udp_datagram_offset = udp_offset + sizeof(udphdr);

    /* We drop all dhcp packets */
    pi->drop = TRUE;

    /* Read the message type and transaction id */
    err = dc_net_get_packet_bytes(pi->packet, pi->udp_datagram_offset,
        sizeof(bytes), bytes);
    if (err)
    {
        printk(KERN_INFO DRV_NAME " %s: dc_net_get_packet_bytes failed\n", __func__);
        return FALSE;
    }

    pi->dhcp_req_type = bytes[0];
    /* ipv6 xid is 3 bytes: assuming little endianness - shift right the bits to
     * clear the req type byte */
    pi->dhcp_xid = (*(UINT *)(bytes)) >> 8;
    return TRUE;
}

/* init_dhcpv6_reply - see comment for dhcp_pkt_ctx_t */
static void init_dhcpv6_reply(packet_info_t *pi, dhcpv6_pkt_ctx_t *ctx,
    uint8_t *client_ip_addr)
{
    dhcpv6_packet_t *p = &ctx->pkt;

    NdisZeroMemory(ctx, sizeof(*ctx));

    init_ipv6_reply(pi, &p->eth, &p->ip, client_ip_addr, DHCP_HOP_LIMIT, IP_PROTOCOL_UDP);

    /* UDP header */
    *(USHORT *)p->udp.src_port = UDP_IPV6_SERVER_PORT;
    *(USHORT *)p->udp.dst_port = UDP_IPV6_CLIENT_PORT;

    /* DHCPv6 section */
    *(UINT *)p->msg_type_xid = pi->dhcp_xid << 8;
}

/**
 * Function name:  finalize_dhcpv6_reply
 * Description:    Set lengths and checksums fields in the networking headers.
 * Parameters:
 *     @ctx:       DHCPv6 packet context
 *
 * Return value:   None
 * Scope:          Local
 **/
static void finalize_dhcpv6_reply(dhcpv6_pkt_ctx_t *ctx)
{
    dhcpv6_packet_t *p = &ctx->pkt;
    USHORT udp_length;

    ctx->pkt_length = DC_OFFSET_OF(dhcpv6_packet_t, dhcp_options) + ctx->options_size;
    udp_length = (USHORT)(ctx->pkt_length - DC_OFFSET_OF(dhcpv6_packet_t, udp));
    *(USHORT *)p->udp.length = dc_hton16(udp_length);

    *(USHORT *)p->ip.payload_len = dc_hton16(udp_length);
    *(USHORT *)p->udp.checksum = ipv6_packet_checksum(&p->ip, ctx->pkt_length - sizeof(eth_header_t));
}

/**
 * Function name:  locate_client_option
 * Description:    Runs over the client's option list, searching for a requested
 *                 option.
 *                 if 'retrieve' is TRUE - the option will be copied to the
 *                 'option_buff' array and the 'option_buff_len' varialbe will
 *                 be updated.
 *                 Note that only the 'Value' section of the option will be
 *                 copied, not including the Type or Length sections.
 * Parameters:
 *     @pi:              Holds the packet, emulation information and headers
 *                       offsets
 *     @option:          The searched option code
 *     @Option_buff:     buffer to copy the option to, if 'retrieve' it True
 *     @Option_buff_len: option Length to be updated, if 'retrieve' it True
 *     @retrieve:        determines if we want to get the option data back or
 *                       just verify that its there.
 *
 * Return value:   False if option was not found or failed reading next option,
 *                 True otherwise.
 * Scope:          Local
 **/
static BOOLEAN locate_client_option(packet_info_t *pi, 
									USHORT option,
									UCHAR *option_buff, 
									USHORT *option_buff_len, 
									BOOLEAN retrieve)
{
    dc_status_t err;
    /* Offset of the fist option - DHCPv6 section after the message type and
     * the xid */
    uint32_t offset =  pi->udp_datagram_offset + 4;
    struct {
        USHORT type;
        USHORT length;
    } option_type_len;

    err = dc_net_get_packet_bytes(pi->packet, offset, sizeof(option_type_len), &option_type_len);
    if (err)
    {
        printk(KERN_INFO DRV_NAME " %s: Failed reading the value from the requested option\n", __func__);
        return FALSE;
    }

    do {
        if (dc_ntoh16(option_type_len.type) == option)
        {
            if (retrieve)
            {
                USHORT len = dc_ntoh16(option_type_len.length);

                if (len > (*option_buff_len))
                {
                    printk(KERN_INFO DRV_NAME " %s: destination buffer is not big enough for the "
                        "requested option\n", __func__);
                    return FALSE;
                }
                *option_buff_len = len;
                err = dc_net_get_packet_bytes(pi->packet, 
                    offset + sizeof(option_type_len),
                    *option_buff_len, option_buff);
                if (err)
                {
                    printk(KERN_INFO DRV_NAME " %s: Failed reading the value from the requested "
                        "option\n", __func__);
                    return FALSE;
                }
            }

            printk(KERN_INFO DRV_NAME " %s: Found client option %u", __func__, option);
            return TRUE;
        }

        /* Not the option we were looking for - read next option */
        offset += sizeof(option_type_len) + dc_ntoh16(option_type_len.length);
        err = dc_net_get_packet_bytes(pi->packet, offset,
            sizeof(option_type_len), &option_type_len);
    } while (!err);

    printk(KERN_INFO DRV_NAME " %s: Failed to find the requested option in client's packet\n",
        __func__);
    return FALSE;
}

/**
 * Function name:  dhcpv6_add_option
 * Description:    Adds a TLV structured options to the option array of the
 *                 DHCPv6 response packet
 * Parameters:
 *     @ctx:            DHCPv6 packet context
 *     @op_type:        Type parameter of the TLV structure
 *     @op_data_length: Length parameter of the TLV structure
 *     @op_data:        Value parameter of the TLV structure
 *
 * Return value:   dc_status_t
 * Scope:          Local
 **/
static dc_status_t dhcpv6_add_option(dhcpv6_pkt_ctx_t *ctx, 
									 USHORT op_type,
									 USHORT op_data_length, 
									 void const *op_data)
{
    dhcpv6_packet_t *p = &ctx->pkt;
    UCHAR *option_base = p->dhcp_options + ctx->options_size;
    /* op_type + data_len + op_data_length */
    USHORT option_size = 2 * sizeof(USHORT) + op_data_length;

    printk(KERN_INFO DRV_NAME " %s: Entered, adding option %x\n", __func__, op_type);
    if (ctx->options_size + option_size > sizeof(p->dhcp_options))
        goto Error;

    ((USHORT *)option_base)[0] = dc_hton16(op_type);
    ((USHORT *)option_base)[1] = dc_hton16(op_data_length);

    NdisMoveMemory(option_base + 4, op_data, op_data_length);
    ctx->options_size += option_size;
    return DC_STATUS_SUCCESS;

Error:
    printk(KERN_INFO DRV_NAME " %s: no room for DHCP option\n", __func__);
    return DC_STATUS_BUFFER_TOO_SMALL;
}

/**
 * Function name:  ai_na_generate_and_add_option
 * Description:   Handle the 'Identity Association for Non-temporary Addresses'
 *                option and its sub-option (DHCPV6_OPTION_IA_ADDRESS)
 * Parameters:
 *     @pi:       Holds the packet, emulation information and headers offsets
 *     @ctx:      DHCPv6 packet context
 *
 * Return value:   dc_status_t
 * Scope:          Local
 **/
static dc_status_t ai_na_generate_and_add_option(packet_info_t *pi,
												 dhcpv6_pkt_ctx_t *ctx)
{
#define INFINITE 0xffffffff
    struct {
        ua32_t iaid;
        UINT t1;
        UINT t2;
        USHORT ia_addr_option;
        USHORT len;
        UINT ip_addr[4];
        UINT prefered_lifetime;
        UINT valid_lifetime;
    } buff;

    UCHAR client_ia_buff[40];
    USHORT client_ia_buff_len = 40;

    if (!locate_client_option(pi, 
		                      DHCPV6_OPTION_IA_NA, 
		                      client_ia_buff,
							  &client_ia_buff_len, 
							  TRUE))
    {
        printk(KERN_INFO DRV_NAME " %s: Failed retreving the Client's IA_NA option\n",
            __func__);
        return DC_STATUS_OPERATION_FAILED;
    }

    /* copy the client's IAID to our reply */
    NdisMoveMemory(buff.iaid, client_ia_buff, 4);

    /* We should check & research the implications of an infinite timeout for
     * the given ip address: need to make sure that the dhcp client will verify
     * the validity of its address every time the NIC goes up, in case our
     * device has switched ip */
    buff.t1 = INFINITE;
    buff.t2 = INFINITE;

    buff.ia_addr_option = dc_hton16(DHCPV6_OPTION_IA_ADDRESS);
    /* fix length for this encapsulated option in our reply - 24 bytes */
    buff.len = dc_hton16(24);

    NdisMoveMemory(buff.ip_addr, pi->s->emul_v6_info.client_ip_addr, IPV6_ADDR_LEN);
    buff.prefered_lifetime = INFINITE;
    buff.valid_lifetime = INFINITE;

    /* add the IA_NA option to the options list */
    return dhcpv6_add_option(ctx, DHCPV6_OPTION_IA_NA, 40, &buff);
}

static UCHAR zero_addr[IPV6_ADDR_LEN] = {0};
#define IS_ZERO_IP_ADDR(addr) RtlEqualMemory(addr, zero_addr, IPV6_ADDR_LEN)

/**
 * Function name: dns_servers_generate_and_add_option
 * Description:   Handle the 'OPTION_DNS_SERVERS' option 
 * Parameters:
 *     @pi:       Holds the packet, emulation information and headers offsets
 *     @ctx:      DHCPv6 packet context
 *
 * Return value:   dc_status_t
 * Scope:          Local
 **/
static dc_status_t dns_servers_generate_and_add_option(packet_info_t *pi,
													   dhcpv6_pkt_ctx_t *ctx)
{
    UCHAR buff[2 * IPV6_ADDR_LEN];
    USHORT dns_num;

    if (IS_ZERO_IP_ADDR(pi->s->emul_v6_info.dns_ip_addr_1))
    {
        printk(KERN_INFO DRV_NAME " %s: No DNS server addresses were supplied by the client\n",__func__);
        return DC_STATUS_SUCCESS;
    }

    if (IS_ZERO_IP_ADDR(pi->s->emul_v6_info.dns_ip_addr_2))
        dns_num = 1;
    else
        dns_num = 2;

    NdisMoveMemory(buff, pi->s->emul_v6_info.dns_ip_addr_1, IPV6_ADDR_LEN);
    if (dns_num == 2)
    {
        NdisMoveMemory(buff + IPV6_ADDR_LEN, pi->s->emul_v6_info.dns_ip_addr_2,
            IPV6_ADDR_LEN); 
    }

    return dhcpv6_add_option(ctx, DHCPV6_OPTION_DNS_SERVERS, 
        dns_num * IPV6_ADDR_LEN, buff);
}

static BOOLEAN handle_advertise_reply_message(packet_info_t *pi,
											  dhcpv6_pkt_ctx_t *ctx, 
											  UCHAR msg)
{
    UCHAR client_duid[32] = {0};
    USHORT client_duid_len = sizeof(client_duid);

    ((UCHAR *)ctx->pkt.msg_type_xid)[0] = msg;
    if (!locate_client_option(pi, 
		                      DHCPV6_OPTION_CLIENTID, 
		                      client_duid,
							  &client_duid_len, 
							  TRUE))
    {
        printk(KERN_INFO DRV_NAME " %s: Client's SOLICIT message does not contain the "
            "CLIENTID option.\n", __func__);
        return FALSE;
    }

    if (dhcpv6_add_option(ctx, DHCPV6_OPTION_CLIENTID, client_duid_len, client_duid))
    {
        return FALSE;
    }

    if (dhcpv6_add_option(ctx, DHCPV6_OPTION_SERVERID, pi->s->emul_v6_info.gw_duid_len, pi->s->emul_v6_info.gw_duid))
    {
        return FALSE;
    }

    if (ai_na_generate_and_add_option(pi, ctx))
    {
        printk(KERN_INFO DRV_NAME " %s: Failed creating/adding AI_NA option.\n", __func__);
        return FALSE;
    }
    
    /* if the device supplied at least one DNS server option - add it to the
     * reply packets */ 
    if (dns_servers_generate_and_add_option(pi, ctx))
    {
        printk(KERN_INFO DRV_NAME " %s: Failed creating/adding DNS_SERVERS option.\n",
            __func__);
        return FALSE;
    }

    return TRUE;
}

/**
 * Function name:  dc_net_process_dhcpv6_packet
 * Description:    process the received dhcpv6 messages, generates a result
 *                 packet and send it back to NDIS
 * Parameters:
 *     @pi: holds the packet, emulation information and headers offsets
 *     @iphdr: holds the IP header
 *
 * Return value:   None
 * Scope:          Global
 **/
void dc_net_process_dhcpv6_packet(packet_info_t *pi, ipv6_header_t *iphdr)
{
    dhcpv6_pkt_ctx_t ctx;
    BOOLEAN res;

	//printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);

    if (!pi->s->emul_v6_info.is_valid)
    {
        printk(KERN_INFO DRV_NAME " %s: DHCPv6 emulation info is not valid\n", __func__);
		//printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
        return;
    }

	memset(&ctx, 0, sizeof(ctx));

    switch (pi->dhcp_req_type)
    {
    case DHCPV6_MSG_REBIND:
        {
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_REBIND\n", __func__);

            init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
            res = handle_advertise_reply_message(pi, &ctx, DHCPV6_MSG_REPLY);
            if (!res)
			{
				printk(KERN_ERR DRV_NAME " %s: handle_advertise_reply_message failed.\n", __func__);
                goto Error;
			}
        }
		break;
    case DHCPV6_MSG_SOLICIT:
		{
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_SOLICIT\n", __func__);

            init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
            res = handle_advertise_reply_message(pi, &ctx, DHCPV6_MSG_ADVERTISE);
            if (!res)
			{
				printk(KERN_ERR DRV_NAME " %s: handle_advertise_reply_message failed.\n", __func__);
                goto Error;
			}
		}
		break;
    case DHCPV6_MSG_REQUEST:
		{
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_REQUEST\n", __func__);

            init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
            res = handle_advertise_reply_message(pi, &ctx, DHCPV6_MSG_REPLY);
            if (!res)
			{
				printk(KERN_ERR DRV_NAME " %s: handle_advertise_reply_message failed.\n", __func__);
                goto Error;
			}
		}
		break;
    case DHCPV6_MSG_RENEW:
        {
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_RENEW\n", __func__);

            init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
            res = handle_advertise_reply_message(pi, &ctx, DHCPV6_MSG_REPLY);
            if (!res)
			{
				printk(KERN_ERR DRV_NAME " %s: handle_advertise_reply_message failed.\n", __func__);
                goto Error;
			}
        }
		break;
    case DHCPV6_MSG_RELEASE:
		{
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_RELEASE\n", __func__);

	        init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
	        ((UCHAR *)ctx.pkt.msg_type_xid)[0] = DHCPV6_MSG_REPLY;
		}
		break;
    case DHCPV6_MSG_DECLINE:
		{
			printk(KERN_INFO DRV_NAME " %s: DHCPV6_MSG_DECLINE\n", __func__);

	        init_dhcpv6_reply(pi, &ctx, iphdr->src_ip_addr);
	        ((UCHAR *)ctx.pkt.msg_type_xid)[0] = DHCPV6_MSG_REPLY;
		}
        break;

     default:
        goto Error;
    }

    finalize_dhcpv6_reply(&ctx);
	
#if 0
	printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);
	printk(KERN_INFO DRV_NAME " %s: DHCPV6_REPLAY\n", __func__);
	dump_packets((UCHAR*)&ctx.pkt, ctx.pkt_length);
	printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
#endif

    dc_net_self_receive(pi->dev, (UCHAR*)&ctx.pkt, ctx.pkt_length);

	//printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
    return;

Error:
    printk(KERN_INFO DRV_NAME " %s: failed to construct DHCPv6 response\n", __func__);
	//printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
}


