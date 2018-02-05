#include "ipv6.h"
#include "dc_types_util.h"

static UINT get_ipv6_addr_sum(const UCHAR *ip)
{
    UINT i, sum;
	
    for (i = 0, sum = 0; i < IPV6_ADDR_LEN; i+=2)
        sum += dc_hton16(*((USHORT *)(ip + i)));
	
    return sum;
}

USHORT ipv6_packet_checksum(ipv6_header_t *ip, UINT length)
{
    UINT i, sum = 0;
    UCHAR *payload_base = ((UCHAR *)ip) + sizeof(ipv6_header_t);
    UINT payload_length = length - sizeof(ipv6_header_t);
	
    /* Calculate checksum - First sum the header + payload. */
    for (i = 0; i < payload_length - 1; i += sizeof(USHORT))
        sum += dc_hton16(*(USHORT *)(payload_base + i));
	
		/* If the number of bytes in the payload is odd - take the last byte, treat
	* it as SHORT and sum it as well. */
    if (i < length)
        sum += dc_hton16((USHORT)payload_base[i]);
	
    /* Now add the pseudo header to the sum */
    sum += get_ipv6_addr_sum(ip->src_ip_addr);
    sum += get_ipv6_addr_sum(ip->dst_ip_addr);
    sum += payload_length;
    sum += ip->next_header;
	
    while (sum > 0xFFFF)
        sum = (sum & 0xFFFF) + (sum >> 16);
	
    /* one's compliment */
    sum = (~sum) & 0xffff;
	
    /* Zero checksum is not valid */
    if (!sum)
        sum = 0xFFFF;
    return dc_hton16((USHORT)sum);
}


void init_ipv6_reply(packet_info_t *pi, 
					 eth_header_t *eth, 
					 ipv6_header_t *ip,
					 UCHAR *client_ip_addr, 
					 UCHAR hop_limit, 
					 UCHAR next_header)
{
    /* Ethernet header */
    ETH_COPY_NETWORK_ADDRESS(eth->dst_addr, pi->s->emul_cmn_info.client_mac_addr);
    ETH_COPY_NETWORK_ADDRESS(eth->src_addr, pi->s->emul_cmn_info.gw_mac_addr);
    *(USHORT *)eth->eth_type = ETHER_TYPE_IPV6;
	
    /* IPv6 header */
    ((UCHAR *)ip->version_traffic_flow)[0] = IP_VERSION_IPV6 << 4;
    ip->hop_limit = hop_limit;
    ip->next_header = next_header;
    NdisMoveMemory(ip->src_ip_addr, pi->s->emul_v6_info.gw_ip_addr, IPV6_ADDR_LEN);
    NdisMoveMemory(ip->dst_ip_addr, client_ip_addr, IPV6_ADDR_LEN);
}

void init_ipv6_reply_ra(packet_info_t *pi, 
						eth_header_t *eth, 
						ipv6_header_t *ip,
						UCHAR *client_ip_addr, 
						UCHAR hop_limit, 
						UCHAR next_header)
{
	ua8_t dst_mac_temp[ETH_ADDR_LENGTH]={0x33,0x33,0x0,0x0,0x0,0x1};
	ua8_t ra_src_ip[16]={0xfe,0x80,0x0,0x0,0x0,0x0,0x0,0x0
		,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
	int i=0;
	
	for(i=8;i<16;i++)
	{
		ra_src_ip[i]=pi->s->emul_v6_info.client_ip_addr[i];
	}
	
	/* Ethernet header */
	//    ETH_COPY_NETWORK_ADDRESS(eth->dst_addr, pi->s->emul_cmn_info.client_mac_addr);
	ETH_COPY_NETWORK_ADDRESS(eth->src_addr, pi->s->emul_cmn_info.gw_mac_addr);
	ETH_COPY_NETWORK_ADDRESS(eth->dst_addr, dst_mac_temp);
	NdisMoveMemory(ip->src_ip_addr, ra_src_ip, IPV6_ADDR_LEN);
	NdisMoveMemory(ip->dst_ip_addr, client_ip_addr, IPV6_ADDR_LEN);
	
	*(USHORT *)eth->eth_type = ETHER_TYPE_IPV6;
	/* IPv6 header */
	((UCHAR *)ip->version_traffic_flow)[0] = IP_VERSION_IPV6 << 4;
	ip->hop_limit = hop_limit;
	ip->next_header = next_header;
}


