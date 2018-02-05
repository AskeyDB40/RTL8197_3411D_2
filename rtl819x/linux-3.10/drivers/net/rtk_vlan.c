/*
 *      Realtek VLAN handler
 *
 *      $Id: rtk_vlan.c,v 1.5 2009/06/09 12:58:30 davidhsu Exp $
 */
#include <linux/version.h>
	 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#include <linux/kconfig.h>
#else
#include <linux/config.h>
#endif
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <asm/string.h>
#include <net/rtl/rtk_vlan.h>
#include <net/rtl/rtl_nic.h>


//---------------------------------------------------------------------------

#if 0
#define DEBUG_ERR(format, args...) panic_printk("%s [%s]: "format, __FUNCTION__, dev->name, ## args)
#else
#define DEBUG_ERR(format, args...)
#endif


#if 0
#define DEBUG_TRACE(format, args...) panic_printk("%s [%s]: "format, __FUNCTION__, dev->name, ## args)
#else
#define DEBUG_TRACE(format, args...)
#endif

#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
extern int RTK_MC_VLANID;//from rtl_nic.c
extern __DRAM_FWD int rtl_hw_vlan_enable;

extern int is_rtl_nat_vlan(struct sk_buff *skb);  
extern int is_rtl_manage_vlan_tagged(struct sk_buff *skb, uint16 *vid);
extern int is_rtl_mc_vlan_tagged(uint16 *vid);
extern int is_rtl_mc_vlan_tagged2(struct sk_buff *skb, uint16 *vid);
extern int is_rtl_wan_mac(struct sk_buff *skb);
extern struct net_device *rtl_get_dev_by_vid(uint16 vid);
extern struct vlan_info *rtl_get_vinfo_by_vid(uint16 vid);
extern int is_rtl_multi_nat_vlan (struct sk_buff *skb, uint16 *vid); 

extern struct net_device *rtl_get_man_dev(void);
extern struct vlan_info *rtl_get_man_vlaninfo(void);
extern int rtl_is_lan_mac(uint8 mac_addr[]);


static uint16 get_skb_vid( struct sk_buff *skb)
{
     uint16 vid=0; //untag or NoVlan group	 
     
     if( skb->tag.f.tpid == htons(ETH_P_8021Q) )
		vid = ntohs(skb->tag.f.pci & 0xfff);	

    return vid;
}

static int  eth_insert_vlan_tag(struct sk_buff *skb , uint16 vid) 
{
	struct vlan_tag tag,*adding_tag;
	adding_tag = &skb->tag;
       adding_tag->f.tpid = htons(ETH_P_8021Q); 
	adding_tag->f.pci = (0xf000 & adding_tag->f.pci) | (0xfff & vid) ; //reset tag info and insert		
	
        memcpy(&tag, skb->data+ETH_ALEN*2, VLAN_HLEN);	
	 if (tag.f.tpid !=  htons(ETH_P_8021Q)) { // tag not existed, insert tag	
		if (skb_headroom(skb) < VLAN_HLEN && skb_cow(skb, VLAN_HLEN) !=0 ) {
				printk("%s-%d: error! (skb_headroom(skb) == %d < 4). Enlarge it!\n",
				__FUNCTION__, __LINE__, skb_headroom(skb));
				while (1) ;
		}
			skb_push(skb, VLAN_HLEN);
			memmove(skb->data, skb->data+VLAN_HLEN, ETH_ALEN*2);
	    		
			memcpy(skb->data+ETH_ALEN*2, adding_tag, VLAN_HLEN);     
	}	
	return 0;
}

#endif

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
#define MAX_IFACE_VLAN_CONFIG 18
#else
#define MAX_IFACE_VLAN_CONFIG 17
#endif
#define WAN_IFACE_INDEX MAX_IFACE_VLAN_CONFIG-1
#define VIRTUAL_IFACE_INDEX MAX_IFACE_VLAN_CONFIG-2

#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
#define PASSTHRU_IFACE_INDEX (MAX_IFACE_VLAN_CONFIG-3)
#define MAX_IFACE_INDEX		PASSTHRU_IFACE_INDEX
#else
#define MAX_IFACE_INDEX		VIRTUAL_IFACE_INDEX
#endif

static struct vlan_info_item vlan_info_items[MAX_IFACE_VLAN_CONFIG];
static unsigned char wan_macaddr[6] = {0};
unsigned char lan_macaddr[6] = {0};

unsigned char BRCST_MAC[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
//static unsigned short eth_arp = 0x0806;

int is_management_packets(struct sk_buff *skb)
{
	/*add code to check whether skb is management pacekts*/
	return 0;
}


struct net_device *rtl_get_wan_from_vlan_info(void)
{
	return vlan_info_items[WAN_IFACE_INDEX].dev;
}

struct net_device* rtl_get_virtual_dev_from_vlan_info(void)
{
	return vlan_info_items[VIRTUAL_IFACE_INDEX].dev;
}

#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)	
struct net_device* rtl_get_passthru_dev_from_vlan_info(void)
{
	return vlan_info_items[PASSTHRU_IFACE_INDEX].dev;
}

#endif
struct net_device *rtl_get_dev_bridged_with_wan(void)
{
	int index;

	for(index=0 ;index<MAX_IFACE_INDEX ; index++) {
		if( vlan_info_items[index].info.forwarding_rule == 1 )		/*forwarding_rule = 1 means dev bridged with wan dev*/
			return vlan_info_items[index].dev;
	}

	return NULL;
}


struct vlan_info_item *rtl_get_vlan_info_item_by_vid(int vid)
{
	struct vlan_info_item *item = NULL;
	int index;
	
	for(index=0; index<MAX_IFACE_INDEX; index++) {
		item = &vlan_info_items[index];
		if( item->dev && item->info.id == vid )
			return item;
	}

	return NULL;
}

struct vlan_info_item *rtl_get_vlan_info_item_by_dev(struct net_device *dev)
{
	struct vlan_info_item *item = NULL;
	int index;

	for(index=0; index<MAX_IFACE_VLAN_CONFIG; index++) {
		item = &vlan_info_items[index];
		if( item->dev && (memcmp(dev->name, item->dev->name,16) == 0) ) {	// IFNAMSIZ = 16
			DEBUG_TRACE("found in list id:%d, is LAN:%d, enable:%d\n",item->info.id,item->info.is_lan,item->info.vlan);
			return item;
		}
	}

	return NULL;
}


int rtl_add_vlan_info(struct vlan_info *info, struct net_device *dev)
{
	struct vlan_info_item *item = NULL;
	int index;

	if( memcmp(dev->name,"eth0",4) == 0 ){
		memcpy(lan_macaddr, dev->dev_addr, 6);
	}


	if( memcmp(dev->name,"eth1",4) == 0 ){
		item = &vlan_info_items[WAN_IFACE_INDEX];

		memset(item, 0, sizeof(struct vlan_info_item));
		memcpy(&item->info, info, sizeof(struct vlan_info));
		item->dev = dev;
		memcpy(wan_macaddr, dev->dev_addr, 6);

		DEBUG_TRACE("WAN port vlan id:%d, is LAN:%d, fowarding:%d\n",item->info.id,item->info.is_lan,item->info.forwarding_rule);
		return 0;
	}

	if( memcmp(dev->name,"eth7",4) == 0 ){
		item = &vlan_info_items[VIRTUAL_IFACE_INDEX];

		memset(item, 0, sizeof(struct vlan_info_item));
		#if 0
		for(index=0; index<VIRTUAL_IFACE_INDEX; index++) {
			if( vlan_info_items[index].info.forwarding_rule == 1 ) {
				memcpy(info,&vlan_info_items[index].info,sizeof(*info));
				info = &vlan_info_items[index].info;
				break;
			}
		}
		#endif
		memcpy(&item->info, info, sizeof(struct vlan_info));
		item->dev = dev;
		DEBUG_TRACE("Virtual port vlan id:%d, is LAN:%d, fowarding:%d\n",item->info.id,item->info.is_lan,item->info.forwarding_rule);
		return 0;
	}
	
#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)	
	if( memcmp(dev->name,"peth",4) == 0 ){
		item = &vlan_info_items[PASSTHRU_IFACE_INDEX];

		memset(item, 0, sizeof(struct vlan_info_item));
		memcpy(&item->info, info, sizeof(struct vlan_info));
		item->dev = dev;
		DEBUG_TRACE("passthru dev vlan id:%d, is LAN:%d, fowarding:%d\n",item->info.id,item->info.is_lan,item->info.forwarding_rule);
		return 0;
	}
#endif	
	if((item = rtl_get_vlan_info_item_by_dev(dev)) != NULL){
                memcpy(&item->info, info, sizeof(struct vlan_info));
                return 0;
        }

	for(index=0; index<MAX_IFACE_VLAN_CONFIG; index++) {
		if( vlan_info_items[index].dev == NULL ) {
			item = &vlan_info_items[index];

			memset(item, 0, sizeof(*item));
			memcpy(&item->info, info, sizeof(*info));
			item->dev = dev;

			DEBUG_TRACE("insert vlan id:%d, is LAN:%d, enable:%d\n",item->info.id,item->info.is_lan,item->info.vlan);
			return 0;
		}
	}

	if( item == NULL ) {
		DEBUG_ERR("VLAN info. list is FULL\n");
		return -1;
	}
}

int rtl_delete_vlan_info(struct net_device *dev)
{
	struct vlan_info_item *item = NULL;

	if((item = rtl_get_vlan_info_item_by_dev(dev)) != NULL){
		memset(item, 0, sizeof(struct vlan_info_item));
		return 0;
	} else {
		DEBUG_ERR("Delete dev from vlan_info_items failed\n");
		return -1;
	}
}

#endif


//---------------------------------------------------------------------------

#ifndef CONFIG_PPPOE_VLANTAG
#define COPY_TAG(tag, info) { \
	tag.f.tpid =  htons(ETH_P_8021Q); \
	tag.f.pci = (unsigned short) (((((unsigned char)info->pri)&0x7) << 13) | \
					((((unsigned char)info->cfi)&0x1) << 12) |((unsigned short)info->id&0xfff)); \
	tag.f.pci =  htons(tag.f.pci);	\
}


#define STRIP_TAG(skb) { \
	memmove(skb->data+VLAN_HLEN, skb->data, ETH_ALEN*2); \
	skb_pull(skb, VLAN_HLEN); \
}

#endif

//---------------------------------------------------------------------------

#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
extern int rtk_vlan_support_enable;
#endif

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT) || defined(CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT)
__MIPS16
__IRAM_FWD
int  rx_vlan_process(struct net_device *dev, struct vlan_info *info_ori, struct sk_buff *skb, struct sk_buff **new_skb)
#else
int  rx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb)
#endif
{
	struct vlan_tag tag;
	unsigned short vid;

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
	struct vlan_info_item *item = NULL;
	struct vlan_info *info, info_backup;
	memcpy(&info_backup, info_ori, sizeof(struct vlan_info)); //because may need to modify info,so create a info_backup
	info= &info_backup;
	if(new_skb)
		*new_skb = NULL;
#endif
#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
	struct vlan_info *info =info_ori ;
	if(new_skb)
		*new_skb = NULL;
	if(!rtl_hw_vlan_enable)
		return 0;
#endif

	DEBUG_TRACE("==> Process Rx packet\n");

	if (!info->global_vlan) {
		DEBUG_TRACE("<== Return w/o change due to gvlan not enabled\n");
		return 0;
	}

	memcpy(&tag, skb->data+ETH_ALEN*2, VLAN_HLEN);

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
	if (info->is_lan) {
		skb->src_info = info_ori;
	}
#endif

	// When port-vlan is disabled, discard tag packet
	if (!info->vlan) {
		if (tag.f.tpid == htons(ETH_P_8021Q)) {
#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
			STRIP_TAG(skb); 
#else
			DEBUG_ERR("<Drop> due to packet w/ tag!\n");
			return 1;
#endif			
		}
		DEBUG_TRACE("<== Return w/o change, and indicate not from vlan port enabled\n");
		skb->tag.f.tpid = 1; // indicate this packet come from the port w/o vlan enabled
		return 0;
	}

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
	if (!info->is_lan && (info->tag&0x1))   //wan port do not need to select tag
	{
		info->tag = 1;
	}
	else
		info->tag = 0;
#endif

	// Drop all no-tag packet if port-tag is enabled
#if !(defined(CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT) || defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)) 
	if (info->tag && tag.f.tpid != htons(ETH_P_8021Q)) {
		DEBUG_ERR("<Drop> due to packet w/o tag but port-tag is enabled!\n");
		return 1;
	}
#endif

	if (tag.f.tpid == htons(ETH_P_8021Q)) { // tag existed in incoming packet
		if (info->is_lan) {
			// Drop all tag packets if VID is not matched
			vid = ntohs(tag.f.pci & 0xfff);
			DEBUG_TRACE("rx from lan!\n");
			if (vid != (unsigned short)info->id) {
				DEBUG_ERR("<Drop> due to VID not matched!\n");
				return 1;
			}
		}
#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
		else {	
			
			STRIP_TAG(skb); //strip here is more safe in case you need to skb_clone !!!
			//from wan interface(p4)
			vid = ntohs(tag.f.pci & 0xfff);	
			// MC vlan process
			if((skb->data[0] & 0x01) && (skb->data[0] != 0xFF ) && (RTK_MC_VLANID !=0) && (vid == ntohs(RTK_MC_VLANID))) //MC_VLAN_ID= 50 defuat support 
			{				
				struct net_device *mc_dev=rtl_get_dev_by_vid(vid);
				//it come from WAN port , but also belong to MC vlan (if some port belong to MC vlan)								
				if ( (mc_dev)  && (new_skb)) { //MC vlan group exist 
				   //  clone a  newl skb-> dev=et1  and pass up .
						*new_skb = skb_clone(skb, GFP_ATOMIC);
						if (*new_skb == NULL) {
							DEBUG_ERR("skb_clone() failed!\n");
						}
						else {							
							(*new_skb)->dev = skb->dev; //dev to eth1
							COPY_TAG((*new_skb)->tag, info);	//copy eth1 iffo															
							skb->dev = mc_dev; //change skb->dev to MC vlan
						}
				}
				else  // if mc vlan not exist , just chage vlan tag to eth1 WAN nat tag
				{
					tag.f.pci= ntohs(info->vlan & 0xfff);
				}		
			}			
#if 0  //mark_hwv , FIXME future
			else if(is_rtl_wan_mac(skb)) //if UC to wan MAC, just tag with wan NAT tag
				tag.f.pci= ntohs(info->vlan & 0xfff);
#endif			
		memcpy(&skb->tag, &tag, sizeof(struct vlan_tag));
		#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
		skb->srcVlanPriority = ntohs(tag.f.pci>>13)&0x7;
		#endif

		return 0;			
		}
#endif
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
		else {	//wan interface
				vid = ntohs(tag.f.pci & 0xfff);
				item = rtl_get_vlan_info_item_by_vid(vid);

				if( item ) {
					if( item->info.forwarding_rule == 1 ){
						skb->dev = rtl_get_virtual_dev_from_vlan_info(); //return virtual interface
						DEBUG_TRACE("vid is bridge vid\n");
						if(skb->dev == NULL)
							return 1;
					} else if ( item->info.forwarding_rule == 0 ) {
						DEBUG_TRACE("<Drop> due to VLAN is disabled\n");
						return 1;
					} else {  //forwarding_rule is nat
						DEBUG_TRACE("Recv from WAN normally\n");
					}
				}
			}
#endif
		memcpy(&skb->tag, &tag, sizeof(struct vlan_tag));
		STRIP_TAG(skb);
		#if	defined(CONFIG_RTL_QOS_8021P_SUPPORT)
		skb->srcVlanPriority = ntohs(tag.f.pci>>13)&0x7;
		#endif
			DEBUG_TRACE("<==%s(%d)	 Tag [vid=%d] existed in Rx packet, strip it and pass up\n", __FUNCTION__,__LINE__,
				(int)ntohs(tag.f.pci&0xfff));
		}
	else	 {
		if( info->is_lan ) {
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
			if(is_management_packets(skb)){
				COPY_TAG(skb->tag, (&management_vlan));
				DEBUG_TRACE("<== Management packet from lan, carry port tag [vid=%d] and pass up\n",
					(int)ntohs(skb->tag.f.pci&0xfff));
			}else
#endif
			{
				COPY_TAG(skb->tag, info);
				DEBUG_TRACE("<== No tag existed, carry port tag [vid=%d] and pass up\n",
					(int)ntohs(skb->tag.f.pci&0xfff));
			}
		} else {
#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
			//untag from wan (p4) 
			//MC vlan , BC not incude (will be forwad with HW layer2)
			if((skb->data[0] & 0x01) &&(skb->data[0] != 0xFF )&& (RTK_MC_VLANID !=0) ) //MC_VLAN_ID= 50 defuat support 
			{
				struct net_device *mc_dev=rtl_get_dev_by_vid(RTK_MC_VLANID);
				struct vlan_info *mc_info = rtl_get_vinfo_by_vid(RTK_MC_VLANID);
				//it come from WAN port , but also belong to MC vlan (if some port belong to MC vlan)								
				if ( (mc_dev) && new_skb &&(mc_info)) 
				{ //MC vlan group exist  and it's untag group
				     if(mc_info->tag == 0) {
				   //  clone a  newl skb-> dev=et1  and pass up .
						*new_skb = skb_clone(skb, GFP_ATOMIC);
						if (*new_skb == NULL) {
							DEBUG_ERR("skb_clone() failed!\n");
						}
						else {							
							(*new_skb)->dev = skb->dev; //dev to eth1
							COPY_TAG((*new_skb)->tag, info);	//copy eth1 iffo															
							skb->dev = mc_dev; //change skb->dev to MC vlan											
							info = mc_info; // copy mc info to skb tag	
						}					
				      }	
				}				
			}
			else 
			{ //mark_manv
				#if 0
				struct net_device *man_dev=rtl_get_man_dev();
				struct vlan_info *man_info = rtl_get_man_vlaninfo();
				if ( (man_dev) && new_skb &&(man_info)) 
				{
					// if manag vlan enable and untag , need to duplicate to "eth7"
				     if(man_info->tag == 0) {
				   //  clone a  newl skb-> dev=et1  and pass up .
						*new_skb = skb_clone(skb, GFP_ATOMIC);
						if (*new_skb == NULL) {
							DEBUG_ERR("skb_clone() failed!\n");
						}
						else {							
							(*new_skb)->dev = skb->dev; //dev to eth1
							COPY_TAG((*new_skb)->tag, info);	//copy eth1 iffo															
							skb->dev = man_dev; 									
							info = man_info; // copy mana vlan info to skb tag							
						}					
				      }	
				}
				#endif
			
			}			
			// normal path
			COPY_TAG(skb->tag, info);
			DEBUG_TRACE("<== No tag existed, carry port tag [vid=%d] and pass up\n",
			(int)ntohs(skb->tag.f.pci&0xfff));	
			return 0;
#endif
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
				if (!memcmp(wan_macaddr, skb->data, 6)){  /*unicast for nat*/
					 if(is_management_packets(skb)){
						COPY_TAG(skb->tag, (&management_vlan));
						DEBUG_TRACE("<== Management packet from wan to nat, carry port tag [vid=%d] and pass up\n",
							(int)ntohs(skb->tag.f.pci&0xfff));
					 }
				}else if (skb->data[0] & 0x01){  /*multicast*/
					skb->src_info = info_ori;
					if (new_skb) {
						*new_skb = skb_clone(skb, GFP_ATOMIC);
						if (*new_skb == NULL) {
							DEBUG_ERR("skb_clone() failed!\n");
						}
						else {
							struct vlan_info *new_info =  &vlan_info_items[VIRTUAL_IFACE_INDEX].info;
							(*new_skb)->dev = rtl_get_virtual_dev_from_vlan_info();
							if((*new_skb)->dev == NULL)
								return 2;
							//COPY_TAG((*new_skb)->tag, new_info);
							(*new_skb)->src_info = new_info;
							(*new_skb)->src_info->index=1;
						}
					}
				}else if(memcmp(BRCST_MAC, skb->data, 6)){	 /*unicast for bridge*/
					/*management packest are unicast!!!!*/
					 if(is_management_packets(skb)){
							COPY_TAG(skb->tag, (&management_vlan));
							DEBUG_TRACE("<== Management packet from wan to bridge, carry port tag [vid=%d] and pass up\n",
								(int)ntohs(skb->tag.f.pci&0xfff));
							}
					skb->dev = rtl_get_virtual_dev_from_vlan_info();
					if(skb->dev == NULL)
						return 1;
				}
#endif
			}

	}

	return 0;
}

EXPORT_SYMBOL(rx_vlan_process);
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
int  tx_vlan_process(struct net_device *dev, struct vlan_info *info_ori, struct sk_buff *skb, int wlan_pri)
#else
__MIPS16
__IRAM_FWD
int  tx_vlan_process(struct net_device *dev, struct vlan_info *info, struct sk_buff *skb, int wlan_pri)
#endif
{
	struct vlan_tag tag, *adding_tag;
#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
       uint16 vid = 0;
	if(!rtl_hw_vlan_enable)
		return 0;
#endif

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
	struct vlan_info *info, info_backup;

	if (skb->src_info != NULL && !info_ori->is_lan) {
		memcpy(&info_backup, skb->src_info, sizeof(struct vlan_info));    //use lan port vlan info
		info_backup.is_lan = info_ori->is_lan;		//info is wan port
	}
	else{
		memcpy(&info_backup, info_ori, sizeof(struct vlan_info));
	}
	info= &info_backup;
#endif

	DEBUG_TRACE("==> Process Tx packet\n");

	//printk("---------%s(%d), dev(%s),skb->tag.f.tpid(0x%x)\n",__FUNCTION__,__LINE__,dev->name,skb->tag.f.tpid);
	if (wlan_pri)
		skb->cb[0] = '\0';		// for WMM priority

	if (!info->global_vlan) {
		DEBUG_TRACE("<== Return w/o change due to gvlan not enabled\n");
		return 0;
	}

#ifdef CONFIG_RTL_HW_VLAN_SUPPORT_HW_NAT
	if(info->is_lan)  // only lan port do drop process
	{
	    vid = get_skb_vid(skb);
		if(info->forwarding_rule == 1) //Bridge out
		{
			
			extern int RTK_QUERYFORBRIDGEPORT;
			if (RTK_QUERYFORBRIDGEPORT)
			{
				if ((skb->tag.f.tpid == 0) && (skb->data[0]&0x01) && (skb->data[0] != 0xff) && (vid == 0))
				{
					return 0;//bypass igmp query /multicast packets from protocol, if needed.
				}
			}
		   //drop vid mismatch packet 
		   	if(vid != (unsigned short)info->id)
			   {
				   	DEBUG_ERR("<Drop> due to VID is not matched!\n");
					return 1;		   	
			    }
			
				if(rtl_is_lan_mac(&skb->data[6]) && skb->data[37]==68)
				{
					return 1;
				}
				
		    //Eth LAN , Bridge spiecal case, UC always need to add tag in packet for HW l2 loopup.		    
			  if((!(skb->data[0] & 0x01)) && (!memcmp(dev->name, "eth", 3)))
			  {
				eth_insert_vlan_tag(skb,vid);
		   	  }
		     	  
		}
		else if (info->forwarding_rule == 2) //NAT lan
		{
			//only drop packet not NAT vlan group
			//if(!is_rtl_nat_vlan(skb))  //mark_wvlan
			if(vid ==0 ) 
				return 0; //bypass packet from protocol
			//drop vid mismatch packet 
		   	if(vid != (unsigned short)info->id)
			{			
	 			DEBUG_ERR("<Drop> due to NAT VID is not matched!\n");
				return 1;		   	    	
			}	
			// nat packet for lan no more tag
		}
	}
	else  // wan process
	{
#ifndef CONFIG_RTL_8367R_SUPPORT //mark_8367, 8367 need to use autoTag to sync vlan info , so d't insert tag here
	    //if(((skb->data[0] & 0x01) &&(skb->data[0] != 0xFF )) && is_rtl_mc_vlan_tagged(&vid))
	    if(((skb->data[0] & 0x01) &&(skb->data[0] != 0xFF )) && is_rtl_mc_vlan_tagged2(skb,&vid))
  	  	   eth_insert_vlan_tag(skb,vid);	//if it is MC vlan packet and need tagout ?	    	
	    else if( is_rtl_manage_vlan_tagged(skb,&vid)) 	    //if it is manamgat vlan packet from CPU ?	
		   eth_insert_vlan_tag(skb,vid);	
	    else if( is_rtl_multi_nat_vlan(skb,&vid)) 	 
		   eth_insert_vlan_tag(skb,vid);		
#endif		
	    // other nat packet tagga or not will control in hw (autoadd portmask) 
	}
	//need add wifi pri ?
	return 0;	
	// ignore legcy vlan process ....
#endif

	if (!info->vlan) {
		// When port-vlan is disabled, discard packet if packet come from source port w/ vlan enabled
		if (skb->tag.f.tpid == htons(ETH_P_8021Q)) {
			DEBUG_ERR("<Drop> due to port-vlan is disabled but Tx packet w/o vlan enabled!\n");
			return 1;
		}
		DEBUG_TRACE("<== Return w/o change because both Tx port and source vlan not enabled\n");
		return 0;
	 }

	// Discard packet if packet come from source port w/o vlan enabled except from protocol stack
	if (skb->tag.f.tpid != 0) {
		if (skb->tag.f.tpid != htons(ETH_P_8021Q)) {
			DEBUG_ERR("<Drop> due to port-vlan is enabled but not from vlan enabled port!\n");
			return 1;
		}

		// Discard packet if its vid not matched, except it come from protocol stack or lan
		if (info->is_lan && ((ntohs(skb->tag.f.pci&0xfff) != ((unsigned short)info->id)) && (ntohs(skb->tag.f.pci&0xfff) != 0xd1))) {
			DEBUG_ERR("<Drop> due to VID is not matched!\n");
			return 1;
		}
	}


#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
	 if (!info->is_lan)
	{
		/*
		  lan->wan: if the lan port receive this packet is tagged or this packet is original tagged,
		  it will go out of wan port as tagged
		 */
		if (info->tag&0x1)
			info->tag = 1;
	}
	else {
		/*wan->lan: all packets are untag to lan, no matter lan port is tagged or not*/
		info->tag = 0;
	}
	if((!memcmp(lan_macaddr,skb->data+6, 6)) && (info->forwarding_rule==1) && (skb->data[37] == 68))
	{
		return 1;
	}
#endif
#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
	if(rtk_vlan_support_enable == 1)
#endif
			if (!info->tag)
			{
				//printk("[%s][%d]-skb->dev[%s],proto(0x%x)\n", __FUNCTION__, __LINE__, skb->dev->name,skb->protocol);
				DEBUG_TRACE("<== Return w/o tagging\n");
				if (wlan_pri) {
					if (!info->is_lan &&  skb->tag.f.tpid == htons(ETH_P_8021Q))
						skb->cb[0] = (unsigned char)((ntohs(skb->tag.f.pci)>>13)&0x7);
					else
						skb->cb[0] = (unsigned char)info->pri;
				}

				return 0;
			}

		// Add tagging

	//	if (!info->is_lan && skb->tag && skb->tag.f.tpid != 0) { // WAN port and not from local, add source tag
		if (skb->tag.f.tpid != 0) { // WAN port and not from local, add source tag
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
			struct vlan_info_item *item = rtl_get_vlan_info_item_by_vid(skb->tag.f.pci & 0xfff);
			if( item && item->info.forwarding_rule == 0 ) { //check if packet form VLAN that disable traffic to/from  WAN
				DEBUG_ERR("<Drop> due to VLAN is disabled\n");
				return 1;
			} else
#endif
			{
				adding_tag = &skb->tag;
				DEBUG_TRACE("---%s(%d) source port tagging [vid=%d]\n",__FUNCTION__,__LINE__, (int)ntohs(skb->tag.f.pci&0xfff));
			}
		}
		else {
			adding_tag = NULL;
			DEBUG_TRACE("---%s(%d)	 Return w/ port tagging [vid=%d]\n", __FUNCTION__,__LINE__,info->id);
		}

#if defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
		if(rtk_vlan_support_enable == 2 && adding_tag == NULL)
			return 0;
#endif

		memcpy(&tag, skb->data+ETH_ALEN*2, VLAN_HLEN);
		if (tag.f.tpid !=  htons(ETH_P_8021Q)) { // tag not existed, insert tag
			if (skb_headroom(skb) < VLAN_HLEN && skb_cow(skb, VLAN_HLEN) !=0 ) {
				printk("%s-%d: error! (skb_headroom(skb) == %d < 4). Enlarge it!\n",
				__FUNCTION__, __LINE__, skb_headroom(skb));
				while (1) ;
			}
			skb_push(skb, VLAN_HLEN);
			memmove(skb->data, skb->data+VLAN_HLEN, ETH_ALEN*2);
		}

		if (!adding_tag)	{ // add self-tag
			COPY_TAG(tag, info);
			adding_tag = &tag;
		}

		memcpy(skb->data+ETH_ALEN*2, adding_tag, VLAN_HLEN);

		if (wlan_pri)
			skb->cb[0] = (unsigned char)((ntohs(adding_tag->f.pci)>>13)&0x7);
		return 0;
}

EXPORT_SYMBOL(tx_vlan_process);

