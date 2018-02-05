#ifndef ipv6_h__
#define ipv6_h__

#include "kstype.h"
#include "filter.h"

USHORT ipv6_packet_checksum(ipv6_header_t *ip, UINT length);
void init_ipv6_reply(packet_info_t *pi, eth_header_t *eth, ipv6_header_t *ip,
					 UCHAR *client_ip_addr, UCHAR hop_limit, UCHAR next_header);
void init_ipv6_reply_ra(packet_info_t *pi, eth_header_t *eth, ipv6_header_t *ip,
						UCHAR *client_ip_addr, UCHAR hop_limit, UCHAR next_header);

#endif // ipv6_h__