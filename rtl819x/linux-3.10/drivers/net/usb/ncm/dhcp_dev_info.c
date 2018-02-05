#include "dhcp_dev_info.h"
#include "dc_types_util.h"
#include "kstype.h"
#include "filter.h"
#include "lte-ncm.h"


static void hton_cpy(UCHAR *dst, UCHAR *src, uint32_t len)
{
    uint32_t i;
	
    for (i = 0; i < len; i++)
        dst[i] = src[len -1 - i];
}


/* Pass NULL dhcp_info to invalidate emulation info */
void update_dhcpv4_emulation_info(dc_net_ctx_t *dc_net_ctx, dhcp_info_t *dhcp_info)
{
    emulation_info_t *emul;
    UINT client_ip, gateway_ip, dns_1, dns_2, subnet;
    emulation_common_info_t *emul_cmn;

	printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);

    if (!dc_net_ctx->filter_ctx)
	{
		printk(KERN_INFO DRV_NAME " %s: dc_net_ctx->filter_ctx = NULL\n", __func__);
		printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
        return;
	}

    emul = &((dc_net_filter_ctx_t *)dc_net_ctx->filter_ctx)->emul_info;
    emul_cmn = &((dc_net_filter_ctx_t *)dc_net_ctx->filter_ctx)->emul_cmn_info;

    if (!dhcp_info)
    {
		printk(KERN_INFO DRV_NAME " %s: dhcp_info = NULL\n", __func__);
        NdisZeroMemory(emul, sizeof(*emul));
		printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
        return;
    }

    printk(KERN_INFO DRV_NAME " %s: Updating DHCPv4 info\n", __func__);
	DBG_I_IPV4_ADDR("    client_ip  ", dhcp_info->client_ip);
	DBG_I_IPV4_ADDR("    gateway_ip ", dhcp_info->gateway_ip);
	DBG_I_IPV4_ADDR("    subnet     ", dhcp_info->subnet);
	DBG_I_IPV4_ADDR("    dns_1      ", dhcp_info->dns_1);
	DBG_I_IPV4_ADDR("    dns_2      ", dhcp_info->dns_2);
	DBG_I_MAC_ADDR( "    gateway_mac", dhcp_info->gateway_mac);

    /* Get little endian values from the device */
    client_ip = dc_letoh32(*(UINT *)dhcp_info->client_ip);
    gateway_ip = dc_letoh32(*(UINT *)dhcp_info->gateway_ip);
	subnet = dc_letoh32(*(UINT *)dhcp_info->subnet);
	dns_1 = dc_letoh32(*(UINT *)dhcp_info->dns_1);
	dns_2 = dc_letoh32(*(UINT *)dhcp_info->dns_2);

    /* Write big endian values for the filter */
#if 1
    emul->client_ip_addr = dc_hton32(client_ip);
    emul->gw_ip_addr = dc_hton32(gateway_ip);
    emul->dns_ip_addr_1 = dc_hton32(dns_1);
    emul->dns_ip_addr_2 = dc_hton32(dns_2);
    emul->subnet = dc_hton32(subnet);
#else
    emul->client_ip_addr = client_ip;
    emul->gw_ip_addr = gateway_ip;
    emul->dns_ip_addr_1 = dns_1;
    emul->dns_ip_addr_2 = dns_2;
    emul->subnet = subnet;
#endif

    ETH_COPY_NETWORK_ADDRESS(emul_cmn->gw_mac_addr, dhcp_info->gateway_mac);
    emul->is_valid = TRUE;
	printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
}

/* Pass NULL dhcp_info to invalidate emulation info */
void update_dhcpv6_emulation_info(dc_net_ctx_t *dc_net_ctx, dhcpv6_info_t *dhcpv6_info)
{
    emulation_v6_info_t *emul;
    emulation_common_info_t *emul_cmn;
//    UCHAR  * mac_offset;

	printk(KERN_INFO DRV_NAME " %s: ++++++++\n", __func__);

    if (!dc_net_ctx->filter_ctx)
	{
		printk(KERN_INFO DRV_NAME " %s: dc_net_ctx->filter_ctx = NULL\n", __func__);
		printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
        return;
	}

    emul = &((dc_net_filter_ctx_t *)dc_net_ctx->filter_ctx)->emul_v6_info;
    emul_cmn = &((dc_net_filter_ctx_t *)dc_net_ctx->filter_ctx)->emul_cmn_info;

    if (!dhcpv6_info)
    {
		printk(KERN_INFO DRV_NAME " %s: dhcpv6_info = NULL\n", __func__);
        NdisZeroMemory(emul, sizeof(*emul));
		printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
        return;
    }

    printk(KERN_INFO DRV_NAME " %s: Updating DHCPv6 info\n", __func__);
 	DBG_I_IPV6_ADDR("    client_ip_addr ", dhcpv6_info->client_ip);
 	DBG_I_IPV6_ADDR("    gw_ip_addr     ", dhcpv6_info->gateway_ip);
 	DBG_I_IPV6_ADDR("    dns_ip_addr_1  ", dhcpv6_info->dns_1);
 	DBG_I_IPV6_ADDR("    dns_ip_addr_2  ", dhcpv6_info->dns_2);

    hton_cpy(emul->client_ip_addr, dhcpv6_info->client_ip, IPV6_ADDR_LEN);
    hton_cpy(emul->gw_ip_addr, dhcpv6_info->gateway_ip, IPV6_ADDR_LEN);
    hton_cpy(emul->dns_ip_addr_1, dhcpv6_info->dns_1, IPV6_ADDR_LEN);
    hton_cpy(emul->dns_ip_addr_2, dhcpv6_info->dns_2, IPV6_ADDR_LEN);

    emul->gw_duid_len = dhcpv6_info->gateway_duid_len;
    hton_cpy(emul->gw_duid, dhcpv6_info->gateway_duid, emul->gw_duid_len);

	DBG_I_DUID_H("    gw_duid_H      ",emul->gw_duid);
	DBG_I_DUID_L("    gw_duid_L      ",emul->gw_duid);

#if 0
    /* gateway's mac is always the last 6 bytes of its DUID */
    mac_offset = &emul->gw_duid[emul->gw_duid_len - ETH_ADDR_LENGTH - 1];
    ETH_COPY_NETWORK_ADDRESS(emul_cmn->gw_mac_addr, mac_offset);
	DBG_I_MAC_ADDR( "    gateway_mac    ", emul_cmn->gw_mac_addr);
#endif
    emul->is_valid = TRUE;

	printk(KERN_INFO DRV_NAME " %s: --------\n", __func__);
}


void dc_net_complete_link_up(dc_net_ctx_t *dc_net_ctx)
{
	printk(KERN_ERR DRV_NAME" %s: network linkup!!!\n", __func__);
    netif_carrier_on(dc_net_ctx->dev->net);
}

void dc_net_link_up(dc_net_ctx_t *dc_net_ctx)
{
#if defined(CONFIG_DC_NET_DHCP_EMULATION) || defined(CONFIG_DC_NET_DHCPV6_EMULATION)
    dc_vendor_get_dhcp_info_and_link_up(dc_net_ctx->dev, dc_net_ctx);
#else
    dc_net_complete_link_up(dc_net_ctx);
#endif
}

void dc_net_link_down(dc_net_ctx_t *dc_net_ctx)
{
	printk(KERN_ERR DRV_NAME" %s: network linkdown!!!\n", __func__);

	netif_carrier_off(dc_net_ctx->dev->net);
	dc_net_ctx->tx_speed = dc_net_ctx->rx_speed = 0;
	
#if defined(CONFIG_DC_NET_DHCP_EMULATION) || defined(CONFIG_DC_NET_DHCPV6_EMULATION)
    dc_net_filter_invalidate_emulation_info(dc_net_ctx);
#endif
}

/* Complete Link Up if we have at least one type of DHCP info */
void vendor_complete_link_up(dc_net_ctx_t *dc_net_ctx)
{
    dc_net_filter_ctx_t *filter_ctx = dc_net_ctx->filter_ctx;
	
    if (!filter_ctx)
        return;
	
    if (filter_ctx->emul_info.is_valid || filter_ctx->emul_v6_info.is_valid)
	{
		dc_net_complete_link_up(dc_net_ctx);
	}
}
