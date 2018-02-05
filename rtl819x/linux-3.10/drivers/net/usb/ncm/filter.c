#include "filter.h"
#include "lte-ncm.h"
#include "dhcp.h"
#include "arp.h"
#include "kstype.h"
#include "dc_types_util.h"


/* multicast addresses in IPV6 are recognized by its 'ff' prefix */
#define is_ipv6_multicast_packet(iphdr) \
	iphdr->dst_ip_addr[0] == IPV6_MCAST_ADDR_PREFIX


void dump_packets(UCHAR * frame, UINT frame_size)
{
	eth_header_t * eth_header;
	USHORT eth_type;
	UCHAR * ip_packet;
	UINT ip_packet_size;

	printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);
	
	if (frame_size < ETH_HEADER_SIZE)
	{
		printk(KERN_ERR DRV_NAME " %s: invalid frame size\n", __func__);
		printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
		return;
	}

	eth_header = (eth_header_t *)frame;
	DBG_I_MAC_ADDR("src_addr", eth_header->src_addr);
	DBG_I_MAC_ADDR("dst_addr", eth_header->dst_addr);
	
	eth_type = *(USHORT *)eth_header->eth_type;
	ip_packet = frame + ETH_HEADER_SIZE;
	ip_packet_size = frame_size - ETH_HEADER_SIZE;
	
	switch(eth_type)
	{
	case ETHER_TYPE_ARP:
		{
			arp_eth_ipv4_t * arp_eth_ipv4 = (arp_eth_ipv4_t *)ip_packet;
			printk(KERN_INFO DRV_NAME " %s: ETHER_TYPE_ARP\n", __func__);
			printk(KERN_INFO DRV_NAME " %s: arp_eth_ipv4:\n", __func__);
			printk(KERN_INFO DRV_NAME " %s: hw_type       = 0x%x\n", __func__, *(USHORT*)arp_eth_ipv4->hw_type);
			printk(KERN_INFO DRV_NAME " %s: prot_type     = 0x%x\n", __func__, *(USHORT*)arp_eth_ipv4->prot_type);
			printk(KERN_INFO DRV_NAME " %s: hw_addr_len   = 0x%x\n", __func__, arp_eth_ipv4->hw_addr_len);
			printk(KERN_INFO DRV_NAME " %s: prot_addr_len = 0x%x\n", __func__, arp_eth_ipv4->prot_addr_len);
			printk(KERN_INFO DRV_NAME " %s: opcode        = 0x%x\n", __func__, *(USHORT*)arp_eth_ipv4->opcode);
			DBG_I_MAC_ADDR("src_mac", arp_eth_ipv4->src_mac);
			DBG_I_MAC_ADDR("dst_mac", arp_eth_ipv4->dst_mac);
			DBG_I_IPV4_ADDR("src_ip", arp_eth_ipv4->src_ip);
			DBG_I_IPV4_ADDR("dst_ip", arp_eth_ipv4->dst_ip);
		}
		break;
		
	case ETHER_TYPE_IPV4:
		{
			ipv4_header_t * ipv4_header;
			UINT ip_h_size;
			
			printk(KERN_INFO DRV_NAME " %s: ETHER_TYPE_IPV4\n", __func__);

			if (ip_packet_size < sizeof(ipv4_header_t))
			{
				printk(KERN_INFO DRV_NAME " %s: invalid ipv4 length.\n", __func__);
				break;
			}

			ipv4_header = (ipv4_header_t *)ip_packet;			
			ip_h_size = 4 * (ipv4_header->version_and_length & 0x0F);

			if (ip_packet_size < ip_h_size)
			{
				printk(KERN_INFO DRV_NAME " %s: invalid ipv4 length2.\n", __func__);
				break;
			}

			DBG_I_IPV4_ADDR("src_ip_addr", ipv4_header->src_ip_addr);
			DBG_I_IPV4_ADDR("dst_ip_addr", ipv4_header->dst_ip_addr);

			switch(ipv4_header->protocol)
			{
			case IP_PROTOCOL_TCP:
				{
					printk(KERN_INFO DRV_NAME " %s: IP_PROTOCOL_TCP\n", __func__);
				
				}
				break;
				
			case IP_PROTOCOL_UDP:
				{
					udp_header_t * udp_header;
					UINT udp_length;
					
					printk(KERN_INFO DRV_NAME " %s: IP_PROTOCOL_UDP\n", __func__);

					if (ip_packet_size < ip_h_size + sizeof(udp_header_t))
					{
						printk(KERN_INFO DRV_NAME " %s: invalid udp_header_t length.\n", __func__);
						break;
					}		

					udp_header = (udp_header_t *)(ip_packet + ip_h_size);					
					udp_length = dc_hton16(*(USHORT *)udp_header->length);

					printk(KERN_INFO DRV_NAME " %s: udp length = %u\n", __func__, udp_length);
					printk(KERN_INFO DRV_NAME " %s: src_port   = %d\n", __func__, dc_hton16(*(USHORT*)udp_header->src_port));
					printk(KERN_INFO DRV_NAME " %s: dst_port   = %d\n", __func__, dc_hton16(*(USHORT*)udp_header->dst_port));

					// dhcp
					if (udp_length >= (sizeof(udp_header_t) + sizeof(bootp_header_t) + sizeof(ua32_t) + 3))
					{
						dhcp_packet_ex_t * dhcp_packet;
						dhcp_packet = (dhcp_packet_ex_t *)ip_packet;

						switch(dhcp_packet->bootp.op)
						{
						case BOOTP_OP_BOOTREQUEST:
							{
								printk(KERN_INFO DRV_NAME " %s: BOOTP_OP_BOOTREQUEST\n", __func__);								
								printk(KERN_INFO DRV_NAME " %s: MessageType = %02X %02X %02X\n", __func__, 
									dhcp_packet->dhcp_options[0], 
									dhcp_packet->dhcp_options[1], 
									dhcp_packet->dhcp_options[2]);
								
								if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPDISCOVER)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPDISCOVER\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPOFFER)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPOFFER\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPREQUEST)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPREQUEST\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPDECLINE)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPDECLINE\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPACK)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPACK\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPNAK)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPNAK\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPRELEASE)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPRELEASE\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPINFORM)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPINFORM\n", __func__);
								}
								else
								{
									printk(KERN_INFO DRV_NAME " %s: unknown 0x%02X\n", __func__, dhcp_packet->dhcp_options[2]);
								}
							}
							break;
						case BOOTP_OP_BOOTREPLY:
							{
								printk(KERN_INFO DRV_NAME " %s: BOOTP_OP_BOOTREPLY\n", __func__);								
								printk(KERN_INFO DRV_NAME " %s: MessageType = %02X %02X %02X\n", __func__, 
									dhcp_packet->dhcp_options[0], 
									dhcp_packet->dhcp_options[1], 
									dhcp_packet->dhcp_options[2]);
								
								if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPDISCOVER)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPDISCOVER\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPOFFER)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPOFFER\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPREQUEST)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPREQUEST\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPDECLINE)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPDECLINE\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPACK)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPACK\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPNAK)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPNAK\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPRELEASE)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPRELEASE\n", __func__);
								}
								else if (dhcp_packet->dhcp_options[2] == DHCP_MSG_DHCPINFORM)
								{
									printk(KERN_INFO DRV_NAME " %s: DHCP_MSG_DHCPINFORM\n", __func__);
								}
								else
								{
									printk(KERN_INFO DRV_NAME " %s: unknown 0x%02X\n", __func__, dhcp_packet->dhcp_options[2]);
								}						
							}
							break;
						default:
							{
								printk(KERN_INFO DRV_NAME " %s: op = %d\n", __func__, dhcp_packet->bootp.op);
							}
							break;
						}						
					}	
				}
				break;

			case IP_PROTOCOL_ICMPV4:
				{
					printk(KERN_INFO DRV_NAME " %s: IP_PROTOCOL_ICMPV4\n", __func__);
					
				}
				break;
				
			default:
				{
				}
				break;
			}			
		}
		break;
		
	case ETHER_TYPE_IPV6:
		{
			ipv6_header_t * ipv6_header = (ipv6_header_t*)ip_packet;
			
			printk(KERN_INFO DRV_NAME " %s: ETHER_TYPE_IPV6\n", __func__);

		    if ((((UCHAR *)ipv6_header->version_traffic_flow)[0] >> 4) != IP_VERSION_IPV6)
		    {
		        printk(KERN_INFO DRV_NAME " %s: IP version is not IPV6: %x\n", __func__, ((UCHAR *)ipv6_header->version_traffic_flow)[0] >> 4);
		        break;
		    }

			DBG_I_IPV6_ADDR("src_ip_addr", ipv6_header->src_ip_addr);
			DBG_I_IPV6_ADDR("dst_ip_addr", ipv6_header->dst_ip_addr);
			
			switch(ipv6_header->next_header)
			{
			case IP_PROTOCOL_ICMPV6:
				{
					icmpv6_header_t * icmpv6_hdr = (icmpv6_header_t*)(ip_packet + sizeof(ipv6_header_t));

					printk(KERN_INFO DRV_NAME " %s: IP_PROTOCOL_ICMPV6\n", __func__);

					switch(icmpv6_hdr->type)
					{
					case ICMPV6_MSG_TYPE_RS:
						{
							printk(KERN_INFO DRV_NAME " %s: ICMPV6_MSG_TYPE_RS\n", __func__);
						}
						break;
					case ICMPV6_MSG_TYPE_RA:
						{
							icmpv6_ra_header_t *p = (icmpv6_ra_header_t*)(ip_packet + sizeof(ipv6_header_t) + sizeof(icmpv6_header_t));

							printk(KERN_INFO DRV_NAME " %s: ICMPV6_MSG_TYPE_RA\n", __func__);
							printk(KERN_INFO DRV_NAME " %s:   cur_hop_limit   = %02X\n", __func__, p->cur_hop_limit);
							printk(KERN_INFO DRV_NAME " %s:   flags           = %02X\n", __func__, p->flags);
							printk(KERN_INFO DRV_NAME " %s:   router_lifetime = %04X\n", __func__, *(USHORT*)p->router_lifetime);
							printk(KERN_INFO DRV_NAME " %s:   reachable_time  = %08X\n", __func__, *(UINT*)p->reachable_time);
							printk(KERN_INFO DRV_NAME " %s:   retrans_timer   = %08X\n", __func__, *(UINT*)p->retrans_timer);
						}
						break;
					case ICMPV6_MSG_TYPE_NS:
						{
							icmpv6_ns_header_t * p = (icmpv6_ns_header_t*)(ip_packet + sizeof(ipv6_header_t) + sizeof(icmpv6_header_t));

							printk(KERN_INFO DRV_NAME " %s: ICMPV6_MSG_TYPE_NS\n", __func__);
							printk(KERN_INFO DRV_NAME " %s:   reserved    = 0x%08X\n", __func__, *(UINT*)p->reserved);
							DBG_I_IPV6_ADDR("  target_addr", p->target_addr);
						}
						break;
					case ICMPV6_MSG_TYPE_NA:
						{
							icmpv6_na_header_t *p = (icmpv6_na_header_t*)(ip_packet + sizeof(ipv6_header_t) + sizeof(icmpv6_header_t));
							printk(KERN_INFO DRV_NAME " %s: ICMPV6_MSG_TYPE_NA\n", __func__);
							printk(KERN_INFO DRV_NAME " %s:   flag        = 0x%08X\n", __func__, *(UINT*)p->flags);
							DBG_I_IPV6_ADDR("  target_addr", p->target_addr);
						}
						break;
					case ICMPV6_MSG_TYPE_REDIRECT:
						{
							printk(KERN_INFO DRV_NAME " %s: ICMPV6_MSG_TYPE_REDIRECT\n", __func__);
						}
						break;
					default:
						break;
					}
				}
				break;
			case IP_PROTOCOL_UDP:
				{
					printk(KERN_INFO DRV_NAME " %s: IP_PROTOCOL_UDPV6\n", __func__);

				}
				break;
			default:
				break;
			}
		}
		break;	
	default:
		printk(KERN_INFO DRV_NAME " %s: unknown, 0x%x.\n", __func__, eth_type);
		break;
	}	

	printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
}


//******************************************************************************
// Function:	dc_net_get_packet_bytes
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-06-05
//******************************************************************************
dc_status_t dc_net_get_packet_bytes(packet_h packet_p, uint32_t offset, uint32_t size, void *buff_ptr)
{
	UCHAR * p = (UCHAR*)packet_p;
	UCHAR * d = (UCHAR*)buff_ptr;
	
	p += offset;
	while (size)
	{
		*d = *p;
		d++;
		p++;
		size--;
	}

	return DC_STATUS_SUCCESS;
}

//******************************************************************************
// Function:	is_ipv6_packet_ex
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-05-20
//******************************************************************************
BOOLEAN is_ipv6_packet_ex( unsigned char * frame, unsigned int frame_size )
{
	ipv6_header_t * iphdr;
	
	if (frame_size < (ETH_HEADER_SIZE + sizeof(ipv6_header_t)))
	{
		return FALSE;
	}
	
	iphdr = (ipv6_header_t *)(frame + ETH_HEADER_SIZE);
	if ((((UCHAR *)iphdr->version_traffic_flow)[0] >> 4) != IP_VERSION_IPV6)
	{
		return FALSE;
	}

	return TRUE;
}

//******************************************************************************
// Function:	is_ipv6_packet
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-06-05
//******************************************************************************
BOOLEAN is_ipv6_packet(packet_info_t *pi, ipv6_header_t *iphdr)
{
    /* Read the entire IP header */
    if (dc_net_get_packet_bytes(pi->packet, pi->eth_datagram_offset,
        sizeof(*iphdr), iphdr))
    {
        printk(KERN_ERR DRV_NAME " %s: dc_net_get_packet_bytes failed\n", __func__);
        return FALSE;
    }
	
    /* Test the IP packet version */
    if ((((UCHAR *)iphdr->version_traffic_flow)[0] >> 4) != IP_VERSION_IPV6)
    {
        return FALSE;
    }
	
    if (!is_ipv6_multicast_packet(iphdr))
    {
        return FALSE;
    }
	
    return TRUE;
}


//******************************************************************************
// Function:	dc_net_init_filter_ctx
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-06-05
//******************************************************************************
int dc_net_init_filter_ctx(void *ctx)
{
    dc_net_filter_ctx_t *s;
    emulation_common_info_t *emul;
	dc_net_ctx_t *dc_net_ctx = (dc_net_ctx_t*)ctx;
    
	s = kmalloc(sizeof(*s), GFP_KERNEL);
	if (s == NULL)
	{
		printk(KERN_ERR DRV_NAME" %s: kmalloc failed\n", __func__);
		return 0;;
	}
	
    NdisZeroMemory(s, sizeof(*s));
    emul = &s->emul_cmn_info;
    s->dc_net_ctx = (dc_net_ctx_t*)dc_net_ctx;
	
    ETH_COPY_NETWORK_ADDRESS(emul->client_mac_addr, dc_net_ctx->phy_nic_mac_address);
	
    emul->emulate_arp = TRUE;
    emul->emulate_dhcp = TRUE;
	
    dc_net_ctx->filter_ctx = s;
	
    return 1;
}

//******************************************************************************
// Function:	dc_net_uninit_filter_ctx
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-06-05
//******************************************************************************
void dc_net_uninit_filter_ctx(void *ctx)
{
	dc_net_ctx_t *dc_net_ctx = (dc_net_ctx_t*)ctx;

    if (!(dc_net_ctx_t*)dc_net_ctx->filter_ctx)
        return;
	
	kfree((dc_net_ctx_t*)dc_net_ctx->filter_ctx);
    dc_net_ctx->filter_ctx = NULL;
}

void dc_net_filter_invalidate_emulation_info(void *ctx)
{
	dc_net_ctx_t *dc_net_ctx = (dc_net_ctx_t *)ctx;
    dc_net_filter_ctx_t *s = (dc_net_filter_ctx_t *)dc_net_ctx->filter_ctx;
	
    if (s)
    {
        s->emul_info.is_valid = FALSE;
        s->emul_v6_info.is_valid = FALSE;
    }
}

int dc_net_self_receive(struct usbnet* dev, UCHAR * data, UINT data_size)
{
	struct sk_buff* skb = NULL;	
	skb = dev_alloc_skb(data_size);
	if (!skb)
	{
		printk(KERN_ERR DRV_NAME" %s: dev_alloc_skb failed.\n", __func__);
		return 0;
	}
	skb_put(skb, data_size);
	memcpy(skb->data, data, data_size);
	usbnet_skb_return(dev, skb);
	return 1;
}

static void ipv6_dhcp_packet_handle(packet_info_t *pi)
{
    ipv6_header_t iphdr;
	
    if (!is_ipv6_packet(pi, &iphdr))
        return;
	
    if (iphdr.next_header == IP_PROTOCOL_ICMPV6)
    {
        UCHAR msg_type;		
        if (is_ndp_ipv6_packet(pi, &msg_type))
		{
            dc_net_process_icmpv6_packet(pi, &iphdr, msg_type);
		}
        return;
    }

    if (iphdr.next_header != IP_PROTOCOL_UDP)
    {
        printk(KERN_INFO DRV_NAME " %s: IP protocol is not UDP or ICMPv6: %u\n", __func__,
            (unsigned)iphdr.next_header);
        return;
    }
	
    if (!dc_net_is_dhcpv6_packet(pi))
	{
        return;
	}

	printk(KERN_INFO DRV_NAME" %s: DHCPv6 Packet.\n", __func__);
	
    dc_net_process_dhcpv6_packet(pi, &iphdr);
}

/* This function tests that the packet has a valid Ethernet header with our
* MAC address as a source and that it carries an IP/ARP payload and redirect
* it to further handling based on the specific protocol. */
void filter_handle_packet(packet_info_t *pi)
{
    dc_status_t err;
    eth_header_t eth;
	
    if (pi->size < ETH_HEADER_SIZE)
    {
        printk(KERN_INFO DRV_NAME " %s: size=%u is less than ETH_HEADER_SIZE\n", __func__, pi->size);
        return;
    }
	
    err = dc_net_get_packet_bytes(pi->packet, 0, sizeof(eth), &eth);
    if (err)
    {
        printk(KERN_INFO DRV_NAME " %s: dc_net_get_packet_bytes failed\n", __func__);
        return;
    }
	
    /* Here we assume that the ethernet header always has the "standard" size
	* (14) if we ever encounter cases where it isn't (like some virtual LANs)
	* we will only need to fix this location.
	* This value is needed for ARP processing so don't move it below. */
    pi->eth_datagram_offset = sizeof(eth);
	
#ifdef CONFIG_DC_NET_ARP_EMULATION
	if (pi->s->emul_cmn_info.emulate_arp)
	{
		if (*(USHORT *)eth.eth_type == ETHER_TYPE_ARP)
		{
			//dc_net_process_arp_packet(pi);
			return;
		}
	}
#endif
	
    if (!pi->s->emul_cmn_info.emulate_dhcp)
	{
        return;
	}
	
    /* Test if the source MAC address in the Ethernet header matches owr own
	* MAC address. We only filter packets that arrived from our MAC address */
    if (!RtlEqualMemory(eth.src_addr, pi->s->emul_cmn_info.client_mac_addr, sizeof(eth.src_addr)))
    {
        printk(KERN_INFO DRV_NAME " %s: not our client MAC address\n", __func__);
		DBG_I_MAC_ADDR( "    src_addr        ", eth.src_addr);
		DBG_I_MAC_ADDR( "    client_mac_addr ", pi->s->emul_cmn_info.client_mac_addr);
        return;
    }
	
#ifdef CONFIG_DC_NET_DHCP_EMULATION
    if (*(USHORT *)eth.eth_type == ETHER_TYPE_IPV4)
	{
        //ipv4_dhcp_packet_handle(pi);
	}
#endif
	
#ifdef CONFIG_DC_NET_DHCPV6_EMULATION
    if (*(USHORT *)eth.eth_type == ETHER_TYPE_IPV6)
	{
        ipv6_dhcp_packet_handle(pi);
	}
#endif
}


//******************************************************************************
// Function:	dhcpv6_state_machine
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	TRUE/FALSE
// Note    :	N/A
//				jiwenxiang
//				2013-06-06
//******************************************************************************
int dhcpv6_state_machine(struct usbnet* dev, struct cdc_ncm_ctx* ctx, unsigned char* buf, unsigned int len)
{
	packet_info_t pi;
	pi.dev = dev;
    pi.s = (dc_net_filter_ctx_t*)ctx->filter_ctx;
    pi.packet = buf;
    pi.size = len;
    pi.drop = 0;
	
    filter_handle_packet(&pi);
	if (pi.drop)
	{
		return 1;
	}
	
	return 0;
}


//******************************************************************************
// Function:	filter_tx_packet
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	1 : drop
//				0 : pass
// Note    :	N/A
//				jiwenxiang
//				2013-05-20
//******************************************************************************
int filter_tx_packet(struct usbnet *dev, unsigned char * frame, unsigned int frame_size)
{
	int ret = 0;
	struct cdc_ncm_ctx * ctx;
	dc_net_filter_ctx_t *filter_ctx;

	//printk(KERN_INFO DRV_NAME" %s : ++++++++\n", __func__);

	ctx = (struct cdc_ncm_ctx *)dev->data[0];
	filter_ctx = (dc_net_filter_ctx_t *)ctx->filter_ctx;

	do 
	{
		if (!filter_ctx)
		{
			break;
		}

		if (!filter_ctx->emul_info.is_valid && !filter_ctx->emul_v6_info.is_valid)
		{
			break;
		}

		if (is_ipv6_packet_ex(frame, frame_size))
		{
			if (dhcpv6_state_machine(dev, ctx, frame, frame_size))
			{
				ret = 1;
				break;
			}
		}
		else
		{
			if (dhcp_state_machine(dev, ctx, frame, frame_size))
			{
				ret = 1;
				break;
			}
			
			if (arp_state_machine(dev, ctx, frame, frame_size))
			{
				ret = 1;
				break;
			}
		}
	} while (0);

	//printk(KERN_INFO DRV_NAME" %s : --------\n", __func__);

	return ret;
}


//******************************************************************************
// Function:	filter_rx_packet
// Purpose :	
// Params  :	Type  &			Name			In/Out	Description  
//				--------------------------		------	------------- 
//				
// Return  :	1 : drop
//				0 : pass
// Note    :	N/A
//				jiwenxiang
//				2013-05-20
//******************************************************************************
int filter_rx_packet(struct usbnet *dev, unsigned char * frame, unsigned int frame_size)
{
	//printk(KERN_INFO DRV_NAME" %s : ++++++++\n", __func__);
	
#if 0
	if (frame && frame_size)
	{
		printk(KERN_INFO DRV_NAME" %s : RX_PACKET\n", __func__);
		dump_packets(frame, frame_size);
	}
#endif

	//printk(KERN_INFO DRV_NAME" %s : --------\n", __func__);

	return 0;
}

