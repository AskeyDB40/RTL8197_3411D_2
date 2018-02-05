#ifndef dhcp_dev_info_h__
#define dhcp_dev_info_h__

#include "kstype.h"
#include "filter.h"

void update_dhcpv4_emulation_info(dc_net_ctx_t *dc_net_ctx, dhcp_info_t *dhcp_info);
void update_dhcpv6_emulation_info(dc_net_ctx_t *dc_net_ctx, dhcpv6_info_t *dhcpv6_info);

void dc_net_complete_link_up(dc_net_ctx_t *dc_net_ctx);
void dc_net_link_up(dc_net_ctx_t *dc_net_ctx);
void dc_net_link_down(dc_net_ctx_t *dc_net_ctx);
void vendor_complete_link_up(dc_net_ctx_t *dc_net_ctx);

#endif // dhcp_dev_info_h__
