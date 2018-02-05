#include <linux/proc_fs.h>
#include <linux/string.h>
#include <net/ip.h>
#include <net/protocol.h>
#include <linux/icmp.h>
#include <net/tcp.h>//brad
#include <net/rtl/rtl_types.h>
#include <net/rtl/fastpath/fastpath_core.h>
#include <net/rtl/rtl_nic.h>

typedef int (*dos_filter_handler)(void *skb);
extern dos_filter_handler filter_dos_ipv4_handler;
extern dos_filter_handler filter_dos_ipv6_handler;
typedef void (*dos_handler_func)(void);
extern dos_handler_func dos_timer_ipv4_func;
extern dos_handler_func dos_timer_ipv6_func;
extern void (*dos_read_proc_func)(struct seq_file *s, void *v);

extern void dos_read_proc_handler(struct seq_file *s, void *v);
extern void dos_timer_ipv6_handler(void);
extern void dos_timer_ipv4_handler(void);
extern int dos_filter_ipv4__action(void *skb);
extern int dos_filter_ipv6__action(void *skb);
extern void clear_attack_address_ipv6(void);
extern void clear_attack_address_ipv4(void);
extern void dos_ipv4_init(void);

/*add by cl*/
void clear_attack_address(void)
{
	clear_attack_address_ipv4();
	
#ifdef CONFIG_IPV6
	clear_attack_address_ipv6();
#endif
	return;
}

#if defined(CONFIG_RTL_FAST_FILTER)
#else 
#ifdef FASTPATH_FILTER
extern struct tcphdr * rtl_get_tcpv6_header(void *skb);

struct tcphdr *rtl_get_tcp_header(void *skb)
{
	struct iphdr *iph;
 	struct tcphdr *tcph;
#ifdef CONFIG_IPV6
	struct ipv6hdr *ip6h;
	ip6h = ipv6_hdr(skb);
#endif

	iph=rtl_ip_hdr(skb);
	
	if(iph->version==4&&iph->protocol == IPPROTO_TCP)
		tcph = (struct tcphdr *)((void *)iph + iph->ihl*4);
#ifdef CONFIG_IPV6
	else if(ip6h->version==6)
	{
		tcph = rtl_get_tcpv6_header(skb);
		if(!tcph) return 0;
	}
#endif
	else
		return 0;
	return tcph;
}

int get_payload_len(void *skb)
{
	struct iphdr *iph;
	int datalen = 0;
#ifdef CONFIG_IPV6
	struct tcphdr *tcph;
	struct ipv6hdr *ip6h;
	ip6h = ipv6_hdr(skb);
#endif

	iph=rtl_ip_hdr(skb);
		
	if(iph->version==4)
		datalen= ntohs(iph->tot_len) -(iph->ihl*8);
#ifdef CONFIG_IPV6
	else if(ip6h->version==6)
	{
		tcph = rtl_get_tcpv6_header(skb);
		if(!tcph) return 0;
		
		datalen= ntohs(ip6h->payload_len) - tcph->doff*4;
	}
#endif
	else
		return 0;

	return datalen;
}
#endif
#endif

static void *wan_dev,*wan_ppp_dev;
int filter_wan_check(void *skb)
{
	
#ifndef CONFIG_RTL_ISP_MULTI_WAN_SUPPORT
	// david
		wan_ppp_dev = (void*)rtl_get_dev_by_name(RTL_PS_PPP0_DEV_NAME); //brad modify
#if defined(CONFIG_RTL_PUBLIC_SSID)
		wan_dev = (void*)rtl_get_dev_by_name(RTL_GW_WAN_DEVICE_NAME);
#else
		wan_dev=(void*)rtl_get_dev_by_name(RTL_PS_WAN0_DEV_NAME);		  //brad modify
#endif
		if(rtl_get_skb_dev(skb) && (rtl_get_skb_dev(skb) == wan_dev || rtl_get_skb_dev(skb) == wan_ppp_dev)) //brad modify
		//if (skb->dev && !memcmp(skb->dev->name, "eth1", 4))
#else		
		//if(rtl_get_skb_dev(skb) && (!memcmp(rtl_get_skb_dev(skb)->name,"eth1",4)||!memcmp(rtl_get_skb_dev(skb)->name,"ppp",3)))
		if(((struct sk_buff *)skb)->from_dev && 
			(((struct sk_buff *)skb)->from_dev->priv_flags & IFF_DOMAIN_WAN))
#endif
			return 1;
		else
			return 0;
}

struct net_device *rtl_get_wan_dev(void)
{
	struct net_device *wan_dev = NULL;

	#if defined(CONFIG_RTL_PUBLIC_SSID)
	wan_dev = __dev_get_by_name(&init_net,RTL_GW_WAN_DEVICE_NAME);
	#else
	wan_dev = __dev_get_by_name(&init_net, RTL_PS_WAN0_DEV_NAME);
	#endif		
	
	return wan_dev;
}

int rtl_strcmp_wan_dev_name(const char *dst)
{
	int ret = -1;

	#if defined(CONFIG_RTL_PUBLIC_SSID)
	ret = strcmp(RTL_GW_WAN_DEVICE_NAME, dst);
	#else	
	ret = strcmp(RTL_PS_WAN0_DEV_NAME, dst);
	#endif	

	return ret;
}

void dos_func_init(void)
{
	filter_dos_ipv4_handler = &dos_filter_ipv4__action;
	dos_timer_ipv4_func = &dos_timer_ipv4_handler;
	dos_read_proc_func = &dos_read_proc_handler;// no ipv6 func
#ifdef CONFIG_IPV6
	filter_dos_ipv6_handler = &dos_filter_ipv6__action;
	dos_timer_ipv6_func = &dos_timer_ipv6_handler;
#endif
	dos_ipv4_init();
}

