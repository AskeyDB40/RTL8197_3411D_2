/* Kernel module to match Realsil extension functions. */

/* (C) 2009-2015 JWJ <wenjain_jai@realsil.com.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_vlanid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JWJ <wenjain_jai@realsil.com.cn>");
MODULE_DESCRIPTION("iptables realsil extensions matching module");
MODULE_ALIAS("ipt_vlanid");
MODULE_ALIAS("ip6t_vlanid");

static bool
match(const struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlanid_info *info = par->matchinfo;

	#if	defined(CONFIG_RTL_QOS_VLANID_SUPPORT)
	return (skb->srcVlanId==info->vid) ^ info->invert;
	#endif
}

static bool
checkentry(const struct xt_mtchk_param *par)
{
	const struct xt_vlanid_info *minfo = par->matchinfo;

	if ((minfo->vid<0) ||(minfo->vid>4095)) {
		printk(KERN_WARNING "vlan id: only supports 0~4095\n");
		return -EINVAL;
	}
	return 0;
}

#ifdef CONFIG_COMPAT
struct compat_xt_vlanid_info {
	u_int16_t	vid;
	u_int8_t	invert;
	u_int8_t	__pad2;
};

static void compat_from_user(void *dst, void *src)
{
	struct compat_xt_vlanid_info *cm = src;
	struct xt_vlanid_info m = {
		.vid		= cm->vid,
		.invert	= cm->invert,
	};
	memcpy(dst, &m, sizeof(m));
}

static int compat_to_user(void __user *dst, void *src)
{
	struct xt_vlanid_info *m = src;
	struct compat_xt_vlanid_info cm = {
		.vid		= m->vid,
		.invert	= m->invert,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */

static struct xt_match xt_vlanid_match[] = {
	{
		.name		= "vlanid",
		.family		= AF_INET,
		.checkentry	= checkentry,
		.match		= match,
		.matchsize	= sizeof(struct xt_vlanid_info),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_vlanid_info),
		.compat_from_user = compat_from_user,
		.compat_to_user	= compat_to_user,
#endif
		.me		= THIS_MODULE,
	},
	{
		.name		= "vlanid",
		.family		= AF_INET6,
		.checkentry	= checkentry,
		.match		= match,
		.matchsize	= sizeof(struct xt_vlanid_info),
		.me		= THIS_MODULE,
	},
};

static int __init xt_vlanid_init(void)
{
	return xt_register_matches(xt_vlanid_match, ARRAY_SIZE(xt_vlanid_match));
}

static void __exit xt_vlanid_fini(void)
{
	xt_unregister_matches(xt_vlanid_match, ARRAY_SIZE(xt_vlanid_match));
}

module_init(xt_vlanid_init);
module_exit(xt_vlanid_fini);

