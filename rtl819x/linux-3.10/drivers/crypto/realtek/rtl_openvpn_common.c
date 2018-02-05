#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <net/dst.h>
#include <net/rtl/rtl_types.h>

unsigned char *rtl_vpn_get_skb_data(struct sk_buff* skb)
{
	return skb->data;
}

struct iphdr *rtl_vpn_ip_hdr(struct sk_buff *skb)
{
	return ip_hdr(skb);
}

unsigned int rtl_vpn_get_skb_len(struct sk_buff *skb)
{
	return skb->len;
}

struct net_device *rtl_vpn_get_skb_dev(struct sk_buff* skb)
{
	return skb->dev;
}

void rtl_vpn_set_skb_tail_direct(struct sk_buff *skb, int offset)
{
	skb->tail=skb->data+offset;
}

void rtl_vpn_set_skb_len(struct sk_buff *skb, int len)
{
	skb->len=len;
}

void rtl_vpn_set_skb_protocol(struct sk_buff *skb,__be16 protocol)
{
	skb->protocol=protocol;
}

struct net_device * rtl_vpn_get_dev_by_name(char *name)
{
	return __dev_get_by_name(&init_net, name);
}

void rtl_vpn_set_skb_dev(struct sk_buff *skb, struct net_device *dev)
{	
	skb->dev = dev;
	return;
}

unsigned char *rtl_vpn_skb_pull(struct sk_buff *skb, unsigned int len)
{
	return skb_pull(skb,len);
}

unsigned char *rtl_vpn_skb_put(struct sk_buff *skb, unsigned int len)
{
	return skb_put(skb,len);
}

unsigned int rtl_vpn_skb_headroom(struct sk_buff *skb)
{
	return skb_headroom(skb);
}

int rtl_vpn_skb_cloned(struct sk_buff *skb)
{
	return skb_cloned(skb);
}

int rtl_vpn_skb_shared(const struct sk_buff *skb)
{
	return skb_shared(skb);
}

unsigned char *rtl_vpn_skb_push(struct sk_buff *skb, unsigned int len)
{
	return skb_push(skb,len);
}

int rtl_vpn_skb_linearize(struct sk_buff *skb)
{
	return skb_linearize(skb);
}

int rtl_vpn_call_skb_ndo_start_xmit(struct sk_buff *skb)
{
	return skb->dev->netdev_ops->ndo_start_xmit(skb,skb->dev);
}


int rtl_vpn_netif_rx(struct sk_buff *skb)
{
#ifdef CONFIG_RTL_8197F
	return netif_rx(skb);
#else
	return netif_receive_skb(skb);
#endif
}

#if 0
inline void *UNCACHED_MALLOC(int size)
{
	return ((void *)(((unsigned int)kmalloc(size, GFP_ATOMIC)) | UNCACHE_MASK));
}
#endif

#if 0
int32 rtl_fast_ipsecEngine(uint32 modeCrypto, uint32 modeAuth, 
	uint32 cntScatter, rtl_ipsecScatter_t *scatter, void *pCryptResult,
	uint32 lenCryptoKey, void* pCryptoKey, 
	uint32 lenAuthKey, void* pAuthKey, 
	void* pIv, void* pPad, void* pDigest,
	uint32 a2eo, uint32 enl)
{
	return rtl_ipsecEngine(modeCrypto, modeAuth, 
	cntScatter, scatter, pCryptResult,
	lenCryptoKey, pCryptoKey, 
	lenAuthKey, pAuthKey, 				
	pIv, pPad, pDigest,
	a2eo, enl);
}
#endif

















