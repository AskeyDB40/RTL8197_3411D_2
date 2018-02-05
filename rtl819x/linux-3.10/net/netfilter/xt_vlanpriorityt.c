/*
 *	xt_vlanpriorityt - Netfilter module to modify the vlanpriority field of an skb
 *
 *	anita hu <anita_hu@realsil.com.cn>
 
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_vlanpriorityt.h>
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/pkt_cls.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl865x_outputQueue.h>
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("anita hu <anita_hu@realsil.com.cn>");
MODULE_DESCRIPTION("Xtables: packet vlanpriority modification");
MODULE_ALIAS("ipt_vlanpriorityt");
MODULE_ALIAS("ip6t_vlanpriorityt");

static unsigned int
vlanpri_tg_v0(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlanpriority_target_info *vlanpriinfo = par->targinfo;
	
#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
	skb->srcVlanPriority= vlanpriinfo->vlanpri;
#endif	

	return XT_CONTINUE;
}

static unsigned int
vlanpri_tg_v1(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlanpriority_target_info_v1 *vlanpriinfo = par->targinfo;
	int vlanpri = 0;

	switch (vlanpriinfo->mode) {
	case XT_VLANPRI_SET:
		vlanpri  = vlanpriinfo->vlanpri ;
		break;

#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
	case XT_VLANPRI_AND:
		vlanpri  = skb->srcVlanPriority & vlanpriinfo->vlanpri ;
		break;

	case XT_VLANPRI_OR:
		vlanpri  = skb->srcVlanPriority | vlanpriinfo->vlanpri ;
		break;
#endif
	}

#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
	skb->srcVlanPriority = vlanpri;
#endif

	return XT_CONTINUE;
}

static unsigned int
vlanpri_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_vlanpriority_tginfo2 *info = par->targinfo;	

#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
	skb->srcVlanPriority = (skb->srcVlanPriority & ~info->mask) ^ info->vlanpri;
#endif

	return XT_CONTINUE;
}

static bool vlanpri_tg_check_v0(const struct xt_tgchk_param *par)
{
	const struct xt_vlanpriority_target_info *vlanpriinfo = par->targinfo;

	if (vlanpriinfo->vlanpri > 0x7) {
		printk(KERN_WARNING "vlanpriority: Only supports 0~7 mark\n");
		return false;
	}
	return true;
}

static bool vlanpri_tg_check_v1(const struct xt_tgchk_param *par)
{
	const struct xt_vlanpriority_target_info_v1 *vlanpriinfo = par->targinfo;

	if (vlanpriinfo->mode != XT_VLANPRI_SET
	    && vlanpriinfo->mode != XT_VLANPRI_AND
	    && vlanpriinfo->mode != XT_VLANPRI_OR) {
		printk(KERN_WARNING "VLANPRI: unknown mode %u\n",
		       vlanpriinfo->mode);
		return false;
	}
	if (vlanpriinfo->vlanpri > 0x7) {
		printk(KERN_WARNING "vlanpriority: Only supports 0~7 mark\n");
		return false;
	}
	return true;
}

#ifdef CONFIG_COMPAT
struct compat_xt_vlanpri_target_info {
	compat_ulong_t	mark;
};

static void mark_tg_compat_from_user_v0(void *dst, void *src)
{
	const struct compat_xt_mark_target_info *cm = src;
	struct xt_mark_target_info m = {
		.mark	= cm->mark,
	};
	memcpy(dst, &m, sizeof(m));
}

static int mark_tg_compat_to_user_v0(void __user *dst, void *src)
{
	const struct xt_mark_target_info *m = src;
	struct compat_xt_mark_target_info cm = {
		.mark	= m->mark,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}

struct compat_xt_mark_target_info_v1 {
	compat_ulong_t	mark;
	u_int8_t	mode;
	u_int8_t	__pad1;
	u_int16_t	__pad2;
};

static void mark_tg_compat_from_user_v1(void *dst, void *src)
{
	const struct compat_xt_mark_target_info_v1 *cm = src;
	struct xt_mark_target_info_v1 m = {
		.mark	= cm->mark,
		.mode	= cm->mode,
	};
	memcpy(dst, &m, sizeof(m));
}

static int mark_tg_compat_to_user_v1(void __user *dst, void *src)
{
	const struct xt_mark_target_info_v1 *m = src;
	struct compat_xt_mark_target_info_v1 cm = {
		.mark	= m->mark,
		.mode	= m->mode,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */

#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
int xt_target2acl_v0(const char *tablename,
			  const void *entry,
			  const struct xt_target *target,
			  void *targinfo,
			  rtl865x_AclRule_t *rule,
			  unsigned int hook_mask, 
			  void **data)
{
	struct net_device *dev, *matchDev;
	struct ipt_entry *e;
	const struct xt_mark_target_info *markinfo = targinfo;
	rtl865x_qos_rule_t qosRule;


	e = (struct ipt_entry *)entry;

	{
		dev = __dev_get_by_name(&init_net, e->ip.outiface);
		memcpy(qosRule.inIfname, e->ip.iniface, IFNAMSIZ);
		memcpy(qosRule.outIfname, e->ip.outiface, IFNAMSIZ);
		
		qosRule.mark = markinfo->mark;

		////////////////////////
		//Patch for hardware QoS
		//To seperate uplink and downlink according to mark from iptables rule
		if(qosRule.mark<53)
		{
			rule->upDown_=0;//Uplink
		}
		else
		{
			rule->upDown_=1;//Downlink
		}
		//////////////////////////
		
		qosRule.rule = rule;
		
		tc_getHandleByKey(markinfo->mark, &qosRule.handle, dev, &matchDev);
		rtl865x_qosAddMarkRule(&qosRule);
	}

	return RTL865X_SKIP_THIS_RULE;
}
int xt_target2acl_v1(const char *tablename,
			  const void *entry,
			  const struct xt_target *target,
			  void *targinfo,
			  rtl865x_AclRule_t *rule,
			  unsigned int hook_mask, 
			  void **data)
{
	struct net_device *dev, *matchDev;
	struct ipt_entry *e;
	const struct xt_mark_target_info_v1 *markinfo = targinfo;
	rtl865x_qos_rule_t qosRule;


	e = (struct ipt_entry *)entry;

	{
		dev = __dev_get_by_name(&init_net, e->ip.outiface);
		memcpy(qosRule.inIfname, e->ip.iniface, IFNAMSIZ);
		memcpy(qosRule.outIfname, e->ip.outiface, IFNAMSIZ);
		
		qosRule.mark = markinfo->mark;

		////////////////////////
		//Patch for hardware QoS
		//To seperate uplink and downlink according to mark from iptables rule
		if(qosRule.mark<53)
		{
			rule->upDown_=0;//Uplink
		}
		else
		{
			rule->upDown_=1;//Downlink
		}
		//////////////////////////
		
		qosRule.rule = rule;
		
		tc_getHandleByKey(markinfo->mark, &qosRule.handle, dev, &matchDev);
		rtl865x_qosAddMarkRule(&qosRule);
	}

	return RTL865X_SKIP_THIS_RULE;
}
int xt_target2acl_v2(const char *tablename,
			  const void *entry,
			  const struct xt_target *target,
			  void *targinfo,
			  rtl865x_AclRule_t *rule,
			  unsigned int hook_mask, 
			  void **data)
{
	struct net_device *dev, *matchDev;
	struct ipt_entry *e;
	const struct xt_mark_tginfo2 *markinfo = targinfo;
	rtl865x_qos_rule_t qosRule;


	e = (struct ipt_entry *)entry;

	{
		dev = __dev_get_by_name(&init_net, e->ip.outiface);
		memcpy(qosRule.inIfname, e->ip.iniface, IFNAMSIZ);
		memcpy(qosRule.outIfname, e->ip.outiface, IFNAMSIZ);
		
		qosRule.mark = markinfo->mark;

		////////////////////////
		//Patch for hardware QoS
		//To seperate uplink and downlink according to mark from iptables rule
		if(qosRule.mark<53)
		{
			rule->upDown_=0;//Uplink
		}
		else
		{
			rule->upDown_=1;//Downlink
		}
		//////////////////////////
		
		qosRule.rule = rule;
		
		tc_getHandleByKey(markinfo->mark, &qosRule.handle, dev, &matchDev);
		rtl865x_qosAddMarkRule(&qosRule);
	}

	return RTL865X_SKIP_THIS_RULE;
}
#endif

static struct xt_target vlanpri_tg_reg[] __read_mostly = {
	{
		.name		= "VLANPRIORITY",
		.family		= NFPROTO_UNSPEC,
		.revision	= 0,
		.checkentry	= vlanpri_tg_check_v0,
		.target		= vlanpri_tg_v0,
		.targetsize	= sizeof(struct xt_vlanpriority_target_info),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_mark_target_info),
		.compat_from_user = mark_tg_compat_from_user_v0,
		.compat_to_user	= mark_tg_compat_to_user_v0,
#endif
		.table		= "mangle",
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.target2acl	= xt_target2acl_v0,
#endif
		.me		= THIS_MODULE,
	},
	{
		.name		= "VLANPRIORITY",
		.family		= NFPROTO_UNSPEC,
		.revision	= 1,
		.checkentry	= vlanpri_tg_check_v1,
		.target		= vlanpri_tg_v1,
		.targetsize	= sizeof(struct xt_vlanpriority_target_info_v1),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_mark_target_info_v1),
		.compat_from_user = mark_tg_compat_from_user_v1,
		.compat_to_user	= mark_tg_compat_to_user_v1,
#endif
		.table		= "mangle",
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.target2acl	= xt_target2acl_v1,
#endif
		.me		= THIS_MODULE,
	},
	{
		.name           = "VLANPRIORITY",
		.revision       = 2,
		.family         = NFPROTO_UNSPEC,
		.target         = vlanpri_tg,
		.targetsize     = sizeof(struct xt_vlanpriority_tginfo2),
#if	defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
		.target2acl	= xt_target2acl_v2,
#endif
		.me             = THIS_MODULE,
	},
};

static int __init vlanpri_tg_init(void)
{
	return xt_register_targets(vlanpri_tg_reg, ARRAY_SIZE(vlanpri_tg_reg));
}

static void __exit vlanpri_tg_exit(void)
{
	xt_unregister_targets(vlanpri_tg_reg, ARRAY_SIZE(vlanpri_tg_reg));
}

module_init(vlanpri_tg_init);
module_exit(vlanpri_tg_exit);
