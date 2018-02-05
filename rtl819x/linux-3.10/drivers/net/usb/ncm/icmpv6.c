#include "kstype.h"
#include "dc_types_util.h"
#include "filter.h"
#include "ipv6.h"

BOOLEAN is_ndp_ipv6_packet(packet_info_t *pi, UCHAR *msg_type)
{
    UINT icmpv6_offset;

    /* Calculate the ICMPv6 datagram offset and read the ICMPv6 message type */
    icmpv6_offset = pi->eth_datagram_offset + IPV6_IP_HEADER_SIZE;
    if (dc_net_get_packet_bytes(pi->packet, icmpv6_offset, sizeof(*msg_type),
        msg_type))
    {
        printk(KERN_ERR DRV_NAME " %s: dc_net_get_packet_bytes for msg_type failed\n",
            __func__);
        return FALSE;
    }

    /* The below 5 requests are the only messages out of the ICMPv6 protocol
     * that we want to filter and drop during the NDP session handling */
    switch (*msg_type)
    {
    case ICMPV6_MSG_TYPE_RS:
    case ICMPV6_MSG_TYPE_RA:
    case ICMPV6_MSG_TYPE_NS:
    case ICMPV6_MSG_TYPE_NA:
    case ICMPV6_MSG_TYPE_REDIRECT:
        return TRUE;
    default:
        return FALSE;
    }
}

static void init_icmpv6_reply(packet_info_t *pi, icmpv6_pkt_ctx_t *ctx,
    UCHAR *client_ip_addr)
{
    icmpv6_packet_t *p = &ctx->pkt;

    NdisZeroMemory(ctx, sizeof(*ctx));

    init_ipv6_reply(pi, &p->eth, &p->ip, client_ip_addr, ICMPV6_HOP_LIMIT,
        IP_PROTOCOL_ICMPV6);
}

static void init_icmpv6_reply_ra(packet_info_t *pi, icmpv6_pkt_ctx_t *ctx,
    UCHAR *client_ip_addr)
{
    icmpv6_packet_t *p = &ctx->pkt;

    NdisZeroMemory(ctx, sizeof(*ctx));

    init_ipv6_reply_ra(pi, &p->eth, &p->ip, client_ip_addr, ICMPV6_HOP_LIMIT,
        IP_PROTOCOL_ICMPV6);
}

/**
 * Function name:  handle_ra_message_reply
 * Description:    Fill router advertisement header fields to perform stateful
 *                 autoconfiguration.
 * Parameters:
 *     @ctx:       ICMPv6 packet context
 *
 * Return value:   None
 * Scope:          Local
 **/
static void handle_ra_message_reply(icmpv6_pkt_ctx_t *ctx)
{
    icmpv6_ra_header_t *p = &ctx->pkt.icmp.ra;

    ctx->pkt.icmpv6_hdr.type = ICMPV6_MSG_TYPE_RA;

    p->cur_hop_limit = ICMPV6_HOP_LIMIT;
    p->flags = ICMPV6_RA_FLAG_MANAGED | ICMPV6_RA_FLAG_OTHER;
    *(USHORT *)p->router_lifetime = ICMPV6_ROUTER_LIFTIME;
}

/**
 * Function name:  icmpv6_add_option
 * Description:    Adds a TLV structured options to the option array of the
 *                 ICMPv6 response packet
 * Parameters:
 *     @ctx:            ICMPv6 packet context
 *     @op_type:        Type parameter of the TLV structure
 *     @op_data_length: Length parameter of the TLV structure
 *     @op_data:        Value parameter of the TLV structure
 *
 * Return value:   dc_status_t
 * Scope:          Local
 **/
dc_status_t icmpv6_add_option(icmpv6_pkt_ctx_t *ctx, UCHAR op_type, UCHAR op_data_length, void const *op_data)
{
    icmpv6_packet_t *p = &ctx->pkt;
    UCHAR *option_base = ((UCHAR *)p) + ctx->pkt_length;
    /* op_type + data_len + op_data_length */
    USHORT option_size = 2 * sizeof(UCHAR) + op_data_length;

    printk(KERN_INFO DRV_NAME " %s: Entered, adding option %x\n", __func__, op_type);
    if (ctx->pkt_length + option_size > sizeof(icmpv6_packet_t))
    {
        printk(KERN_ERR DRV_NAME " %s: no room for ICMP option\n", __func__);
        return DC_STATUS_BUFFER_TOO_SMALL;
    }

    ((UCHAR *)option_base)[0] = op_type;

    /* The length of the option (including the type and length fields) in units
     * of 8 octets. See RFC 4861, section 4.6 */
    //ASSERT(option_size%8 == 0);
    ((UCHAR *)option_base)[1] = (UCHAR)(option_size/8);

    NdisMoveMemory(option_base + 2, op_data, op_data_length);
    ctx->pkt_length += option_size;

    return DC_STATUS_SUCCESS;
}

/**
 * Function name:  handle_na_message_reply
 * Description:    Read requested target address and fill a neighbor
 *                 advertisement response.
 * Parameters:
 *     @pi:        holds the packet, emulation information and headers offsets
 *     @ctx:       ICMPv6 packet context
 *
 * Return value:   TRUE if NA packet was build successfully, FALSE if we don't
 *                 need to send NA packet or failed to construct one.
 * Scope:          Local
 **/
static BOOLEAN handle_na_message_reply(packet_info_t *pi, icmpv6_pkt_ctx_t *ctx)
{
    icmpv6_ns_header_t ns_msg;
    UINT ns_offset;
    icmpv6_na_header_t *p = &ctx->pkt.icmp.na;

    ns_offset = pi->eth_datagram_offset + IPV6_IP_HEADER_SIZE + sizeof(icmpv6_header_t);
    if (dc_net_get_packet_bytes(pi->packet, ns_offset, sizeof(icmpv6_ns_header_t), &ns_msg))
    {
        printk(KERN_ERR DRV_NAME " %s: dc_net_get_packet_bytes for neighbor solicitation message failed\n", __func__);
        return FALSE;
    }

    /* Check for Duplicated Address Detection */
	DBG_I_IPV6_ADDR("target_addr   ", ns_msg.target_addr);
	DBG_I_IPV6_ADDR("client_ip_addr", pi->s->emul_v6_info.client_ip_addr);

    if (RtlEqualMemory(ns_msg.target_addr, pi->s->emul_v6_info.client_ip_addr, sizeof(ns_msg.target_addr)))
    {
        printk(KERN_ERR DRV_NAME " %s: Duplicated Address Detection request. Should not reply\n", __func__);
        return FALSE;
    }

    ctx->pkt.icmpv6_hdr.type = ICMPV6_MSG_TYPE_NA;
    *(UINT *)p->flags = dc_hton32(ICMPV6_NA_FLAG_SOLICITED | ICMPV6_NA_FLAG_OVERRIDE);

    /* Router option should be set if the target address is the same as the
     * gateway address */
    if (RtlEqualMemory(ns_msg.target_addr, pi->s->emul_v6_info.gw_ip_addr,
        sizeof(ns_msg.target_addr)))
    {
        *(UINT *)p->flags |= dc_hton32(ICMPV6_NA_FLAG_ROUTER);
    }

    NdisMoveMemory(p->target_addr, ns_msg.target_addr, IPV6_ADDR_LEN);

    if (icmpv6_add_option(ctx, ICMPV6_OPTION_TARGET_ADDRESS, ETH_ADDR_LENGTH,
        pi->s->emul_cmn_info.gw_mac_addr))
    {
        printk(KERN_ERR DRV_NAME " %s: icmpv6_add_option failed\n", __func__);
        return FALSE;
    }

    return TRUE;
}

/**
 * Function name:  finalize_icmpv6_reply
 * Description:    Set lengths and checksums fields in the networking headers.
 * Parameters:
 *     @ctx:       ICMPv6 packet context
 *
 * Return value:   None
 * Scope:          Local
 **/
static void finalize_icmpv6_reply(icmpv6_pkt_ctx_t *ctx)
{
    icmpv6_packet_t *p = &ctx->pkt;
    USHORT icmp_length;

    icmp_length = (USHORT)(ctx->pkt_length -
        DC_OFFSET_OF(icmpv6_packet_t, icmpv6_hdr));

    *(USHORT *)p->ip.payload_len = dc_hton16(icmp_length);

    *(USHORT *)ctx->pkt.icmpv6_hdr.checksum = ipv6_packet_checksum(&p->ip,
        ctx->pkt_length - sizeof(eth_header_t));
}

/**
 * Function name:  dc_net_process_icmpv6_packet
 * Description:    process the received icmpv6 messages
 * Parameters:
 *     @pi: holds the packet, emulation information and headers offsets
 *     @iphdr: holds the IP header
 *     @msg_type
 *
 * Return value:   None
 * Scope:          Global
 **/
void dc_net_process_icmpv6_packet(packet_info_t *pi, ipv6_header_t *iphdr, UCHAR msg_type)
{
    icmpv6_pkt_ctx_t ctx;
    BOOLEAN res;
    icmpv6_prefix_option_t prefix_option;
    ua8_t duobo_dst_ip_addr[16]={0xff,0x02,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1};
	
    //printk(KERN_ERR DRV_NAME " %s: Entered, msg_type %d\n", __func__, msg_type);

    /* We drop ICMPv6 packets */
    pi->drop = TRUE;

    if (!pi->s->emul_v6_info.is_valid)
    {
        printk(KERN_ERR DRV_NAME " %s: emulation info is not valid\n", __func__);
        return;
    }

	memset(&ctx, 0, sizeof(ctx));

    switch (msg_type)
    {
    case ICMPV6_MSG_TYPE_RS:
		{
			printk(KERN_ERR DRV_NAME " %s: ICMPV6_MSG_TYPE_RS\n", __func__);

			init_icmpv6_reply_ra(pi, &ctx, duobo_dst_ip_addr);
			ctx.pkt_length = DC_OFFSET_OF(icmpv6_packet_t, icmp) + sizeof(icmpv6_ra_header_t);
			handle_ra_message_reply(&ctx);

			memset(&prefix_option, 0, sizeof(prefix_option));
			prefix_option.type=0x03;
			prefix_option.len=0x4;
			prefix_option.prefix_len=0;
			prefix_option.flags=0x80;
			prefix_option.valid_lifetime=0x8d2700;
			prefix_option.preferred_lifetime=0x803a0900;
			prefix_option.reserved=0x0;
			prefix_option.Prefix[0]=0x0;
			prefix_option.Prefix[1]=0x0;
			prefix_option.Prefix[2]=0x0;
			prefix_option.Prefix[3]=0x0;
			icmpv6_add_option(&ctx,prefix_option.type,sizeof(icmpv6_prefix_option_t)-2,(UINT8*)(&prefix_option)+2);
		}
        break;
    case ICMPV6_MSG_TYPE_NS:
		{
			printk(KERN_ERR DRV_NAME " %s: ICMPV6_MSG_TYPE_NS\n", __func__);

			init_icmpv6_reply(pi, &ctx, iphdr->src_ip_addr);
			ctx.pkt_length = DC_OFFSET_OF(icmpv6_packet_t, icmp) + sizeof(icmpv6_na_header_t);
			res = handle_na_message_reply(pi, &ctx);
			if (!res)
			{
				printk(KERN_ERR DRV_NAME " %s: failed to handle_na_message_reply\n", __func__);
				return;
			}
		}
        break;
    default:
		{
			printk(KERN_ERR DRV_NAME " %s: Unsupported message %d\n", __func__, msg_type);
			goto Error;
		}
    }

    finalize_icmpv6_reply(&ctx);

#if 0
	printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);
	printk(KERN_INFO DRV_NAME " %s: ICMPV6_REPLAY\n", __func__);
	dump_packets((UCHAR*)&ctx.pkt, ctx.pkt_length);
	printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
#endif

    dc_net_self_receive(pi->dev, (UCHAR*)&ctx.pkt, ctx.pkt_length);
	
    return;

Error:
    printk(KERN_ERR DRV_NAME " %s: failed to construct ICMPv6 response\n", __func__);
}

