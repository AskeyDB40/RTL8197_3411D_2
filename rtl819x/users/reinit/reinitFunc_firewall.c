#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "apmib.h"
#include "reinit_utility.h"
#include "reinitFunc_wan.h"
#include "reinitHandle.h"


char Iptables[]="iptables";
#if defined(CONFIG_APP_EBTABLES)&&defined(CONFIG_EBTABLES_KERNEL_SUPPORT)
char Ebtables[]="ebtables";
#endif
char Ip6tables[]="ip6tables";
char INPUT[]= "INPUT";
char OUTPUT[]= "OUTPUT";
char FORWARD[]= "FORWARD";
char PREROUTING[]="PREROUTING";
char POSTROUTING[]= "POSTROUTING";
char ACCEPT[]= "ACCEPT";
char DROP[]= "DROP";
char RET[]= "RETURN";
char LOG[]= "LOG";
char MASQUERADE[]="MASQUERADE";
char REDIRECT[]="REDIRECT";
char MARK[]="MARK";
// iptables operations, manupilations, matches, options etc.
char ADD[]= "-A";
char DEL[]= "-D";
char FLUSH[]= "-F";
char INSERT[]="-I";
char NEW[]= "-N";
char POLICY[]= "-P";
char X[]= "-X";
char Z[]= "-Z";
char _dest[]= "-d";
char in[]= "-i";
char jump[]= "-j";
char match[]= "-m";
char out[]= "-o";
char _protocol[]= "-p";
char _src[]= "-s";
char _table[]= "-t";
char nat_table[]= "nat";
char mangle_table[]= "mangle";
char NOT[]= "!";
char _mac[]= "mac";
char mac_src[]= "--mac-source";
char mac_dst[]= "--mac-destination";
char dport[]= "--dport";
char sport[]= "--sport";
char syn[]= "--syn";
char ALL[]= "ALL";
char DNAT[]= "DNAT";
char icmp_type[]="--icmp-type";
char echo_request[]="echo-request";
char echo_reply[]="echo-reply";
char mstate[]="state";
char state[]="--state";
char _udp[]="udp";
char _tcp[]="tcp";
char _icmp[]="icmp";
char RELATED_ESTABLISHED[]= "RELATED,ESTABLISHED";
char INVALID[]="INVALID";
char tcp_flags[]="--tcp-flags";
char MSS_FLAG1[]="SYN,RST";
char MSS_FLAG2[]="SYN";
char clamp[]="--clamp-mss-to-pmtu";
char TCPMSS[]="TCPMSS";
char ip_range[]="iprange";
char src_rnage[]="--src-range";
char dst_rnage[]="--dst-range";
char set_mark[]="--set-mark";

static const char _tc[] = "tc";
static const char _qdisc[] = "qdisc";
static const char _add[] = "add";
static const char _dev[] = "dev";
static const char _root[] = "root";
static const char _handle[] = "handle";
static const char _htb[] = "htb";
static const char _default[] = "default";
static const char _classid[] = "classid";
static const char _rate[] = "rate";
static const char _ceil[] = "ceil";
static const char _sfq[] = "sfq";
static const char _perturb[] = "perturb";
static const char _class[] = "class";
static const char _filter[] = "filter";
static const char _protocol2[] = "protocol";
static const char _ip[] = "ip";
static const char _prio[] = "prio";
static const char _fw[] = "fw";
static const char _parent[] = "parent";
static const char _quantum[] = "quantum";
static const char _r2q[] = "r2q";


/*user-defined chain ***************************/
static const char INPUT_HEAD[]="INPUT_HEAD";
static const char INPUT_TAIL[]="INPUT_TAIL";
static const char FORWARD_HEAD[]="FORWARD_HEAD";
static const char FORWARD_TAIL[]="FORWARD_TAIL";
static const char OUTPUT_HEAD[]="OUTPUT_HEAD";
static const char OUTPUT_TAIL[]="OUTPUT_TAIL";

static const char INPUT_HEAD_START[]="INPUT_HEAD_START";
static const char INPUT_HEAD_END[]="INPUT_HEAD_END";
static const char INPUT_TAIL_START[]="INPUT_TAIL_START";
static const char INPUT_TAIL_END[]="INPUT_TAIL_END";

static const char FORWARD_HEAD_START[]="FORWARD_HEAD_START";
static const char FORWARD_HEAD_END[]="FORWARD_HEAD_END";
static const char FORWARD_TAIL_START[]="FORWARD_TAIL_START";
static const char FORWARD_TAIL_END[]="FORWARD_TAIL_END";


#define MAX_WAN_NUM			5	

static char MAC_FILTER_NAME[2][32]={
	"INPUT_MACFILTER",
	"FORWARD_MACFILTER"
};

static char PORT_FILTER_NAME[2][32]={
	"INPUT_PORTFILTER",
	"FORWARD_PORTFILTER"
};

static char IP_FILTER_NAME[2][32]={
	"INPUT_IPFILTER",
	"FORWARD_IPFILTER"
};


static char PORTFORWARD_FORWARD_NAME[MAX_WAN_NUM][32]={
	"PORTFORWARD_FORWARD",
	"PORTFORWARD_FORWARD_WAN1",
	"PORTFORWARD_FORWARD_WAN2",
	"PORTFORWARD_FORWARD_WAN3",
	"PORTFORWARD_FORWARD_WAN4"
};
static char PORTFORWARD_PREROUTING_NAME[MAX_WAN_NUM][32]={
	"PORTFORWARD_PREROUTING",
	"PORTFORWARD_PREROUTING_WAN1",
	"PORTFORWARD_PREROUTING_WAN2",
	"PORTFORWARD_PREROUTING_WAN3",
	"PORTFORWARD_PREROUTING_WAN4"
};


static char DMZ_POSTROUTING_NAME[MAX_WAN_NUM][32]={
	"DMZ_POSTROUTING",
	"DMZ_POSTROUTING_WAN1",
	"DMZ_POSTROUTING_WAN2",
	"DMZ_POSTROUTING_WAN3",
	"DMZ_POSTROUTING_WAN4"
};

static char DMZ_PREROUTING_NAME[MAX_WAN_NUM][32]={
	"DMZ_PREROUTING",
	"DMZ_PREROUTING_WAN1",
	"DMZ_PREROUTING_WAN2",
	"DMZ_PREROUTING_WAN3",
	"DMZ_PREROUTING_WAN4"
};

static char DMZ_FORWARD_NAME[MAX_WAN_NUM][32]={
	"DMZ_FORWARD",
	"DMZ_FORWARD_WAN1",
	"DMZ_FORWARD_WAN2",
	"DMZ_FORWARD_WAN3",
	"DMZ_FORWARD_WAN4"
};
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
static char IGMP_FORWARD_NAME[MAX_WAN_NUM][32]={
	"IGMP_FORWARD",
	"IGMP_FORWARD_WAN1",
	"IGMP_FORWARD_WAN2",
	"IGMP_FORWARD_WAN3",
	"IGMP_FORWARD_WAN4"
};

static char IGMP_INPUT_NAME[MAX_WAN_NUM][32]={
	"IGMP_INPUT",
	"IGMP_INPUT_WAN1",
	"IGMP_INPUT_WAN2",
	"IGMP_INPUT_WAN3",
	"IGMP_INPUT_WAN4"
};
#endif

static char COMMON_CHAIN_NAME[MAX_WAN_NUM][32]={
	"COMMON_",
	"INPUT_COMMON",
	"FORWARD_COMMON",
	"OUTPUT_COMMON",
	"PREROUTEING_COMMON"
};	

static char INPUT_CHAIN_PORTFORWARD[MAX_WAN_NUM][32]={
	"INPUT_WAN",
	"INPUT_PORTFORWARD_WAN1",
	"INPUT_PORTFORWARD_WAN2",
	"INPUT_PORTFORWARD_WAN3",
	"INPUT_PORTFORWARD_WAN4"
};	

static char INPUT_CHAIN_NAME[MAX_WAN_NUM][32]={
	"INPUT_WAN",
	"INPUT_WAN1",
	"INPUT_WAN2",
	"INPUT_WAN3",
	"INPUT_WAN4"
};	
static char OUTPUT_CHAIN_NAME[MAX_WAN_NUM][32]={
	"OUTPUT_WAN",
	"OUTPUT_WAN1",
	"OUTPUT_WAN2",
	"OUTPUT_WAN3",
	"OUTPUT_WAN4"
};
static char FORWARD_CHAIN_NAME[MAX_WAN_NUM][32]={
	"FORWARD_WAN",
	"FORWARD_WAN1",
	"FORWARD_WAN2",
	"FORWARD_WAN3",
	"FORWARD_WAN4"
};
static char POSTROUTING_CHAIN_NAME[MAX_WAN_NUM][32]={
	"POSTROUTING_WAN",
	"POSTROUTING_WAN1",
	"POSTROUTING_WAN2",
	"POSTROUTING_WAN3",
	"POSTROUTING_WAN4"
};	
static char PREROUTING_CHAIN_NAME[MAX_WAN_NUM][32]={
	"PREROUTING_WAN",
	"PREROUTING_WAN1",
	"PREROUTING_WAN2",
	"PREROUTING_WAN3",
	"PREROUTING_WAN4"
};

static char PING_INPUT_NAME[MAX_WAN_NUM][32]={
	"PING_INPUT",
	"PING_INPUT_WAN1",
	"PING_INPUT_WAN2",
	"PING_INPUT_WAN3",
	"PING_INPUT_WAN4"
};

static char WEB_INPUT_NAME[MAX_WAN_NUM][32]={
	"WEB_INPUT",
	"WEB_INPUT_WAN1",
	"WEB_INPUT_WAN2",
	"WEB_INPUT_WAN3",
	"WEB_INPUT_WAN4"
};

#if defined(CONFIG_APP_TR069)
static char TR069_INPUT_NAME[MAX_WAN_NUM][32]={
	"TR069_INPUT",
	"TR069_INPUT_WAN1",
	"TR069_INPUT_WAN2",
	"TR069_INPUT_WAN3",
	"TR069_INPUT_WAN4"
};
#endif
#if defined(CONFIG_RTL_IP_POLICY_ROUTING_SUPPORT)
#define WAN_DEV_NUM_MAX	8
#define LAN_DEV_NUM_MAX	14	//lan(4) + wlan0(5) + wlan1(5)

#define ALIASNAME_PPP  					"ppp"
#define ALIASNAME_ETH_WAN 				"eth1."
#define ALIASNAME_WLAN0_VA  			"wlan0-va"
#define ALIASNAME_WLAN1_VA 			"wlan1-va"

#define RTL_ELAN1_DEV_NAME 			"eth0"
#define RTL_ELAN2_DEV_NAME 			"eth2"
#define RTL_ELAN3_DEV_NAME 			"eth3" 
#define RTL_ELAN4_DEV_NAME 			"eth4"
#define RTL_WLAN0_DEV_NAME 			"wlan0"
#define RTL_WLAN1_DEV_NAME 			"wlan1"

#define RTL_ETH_LAN0_BIND_MASK		(0x1<<0)
#define RTL_ETH_LAN1_BIND_MASK		(0x1<<1)
#define RTL_ETH_LAN2_BIND_MASK		(0x1<<2)
#define RTL_ETH_LAN3_BIND_MASK		(0x1<<3)
#define RTL_WLAN0_BIND_MASK			(0x1<<4)
#define RTL_WLAN0_VA0_BIND_MASK		(0x1<<5)
#define RTL_WLAN0_VA1_BIND_MASK		(0x1<<6)
#define RTL_WLAN0_VA2_BIND_MASK		(0x1<<7)
#define RTL_WLAN0_VA3_BIND_MASK		(0x1<<8)
#define RTL_WLAN1_BIND_MASK			(0x1<<9)
#define RTL_WLAN1_VA0_BIND_MASK		(0x1<<10)
#define RTL_WLAN1_VA1_BIND_MASK		(0x1<<11)
#define RTL_WLAN1_VA2_BIND_MASK		(0x1<<12)
#define RTL_WLAN1_VA3_BIND_MASK		(0x1<<13)

#define RTL_DEV_NAME_NUM(name,num)	name#num //fix compile error when tro09 enabled

#define RTL_WLAN0_VA0_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN0_VA, 0)
#define RTL_WLAN0_VA1_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN0_VA, 1)
#define RTL_WLAN0_VA2_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN0_VA, 2)
#define RTL_WLAN0_VA3_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN0_VA, 3)
#define RTL_WLAN1_VA0_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN1_VA, 0)
#define RTL_WLAN1_VA1_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN1_VA, 1)
#define RTL_WLAN1_VA2_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN1_VA, 2)
#define RTL_WLAN1_VA3_DEV_NAME 	RTL_DEV_NAME_NUM(ALIASNAME_WLAN1_VA, 3)

#define RTL_ETHWAN1_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,1)//"eth1.1"
#define RTL_ETHWAN2_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,2)//"eth1.2"
#define RTL_ETHWAN3_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,3)//"eth1.3"
#define RTL_ETHWAN4_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,4)//"eth1.4"
#define RTL_ETHWAN5_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,5)//"eth1.5"
#define RTL_ETHWAN6_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,6)//"eth1.6"
#define RTL_ETHWAN7_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,7)//"eth1.7"
#define RTL_ETHWAN8_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_ETH_WAN,8)//"eth1.8"

#define RTL_PPP1_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,1)//"ppp1"
#define RTL_PPP2_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,2)//"ppp2"
#define RTL_PPP3_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,3)//"ppp3"
#define RTL_PPP4_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,4)//"ppp4"
#define RTL_PPP5_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,5)//"ppp5"
#define RTL_PPP6_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,6)//"ppp6"
#define RTL_PPP7_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,7)//"ppp7"
#define RTL_PPP8_DEV_NAME RTL_DEV_NAME_NUM(ALIASNAME_PPP,8)//"ppp8"

struct lan_dev_bind_mask {
	char 	ifname[16];
	int 		bind_mask;
	int 		mapping_mark_bak;	//just for delete iptables for mark
};

struct wan_dev_mapping_router_tables {
	char 	ifname[16];
	int 		router_table_id;
	int 		mapping_mark;
};

static struct lan_dev_bind_mask lan_dev_bind_mask_mapping[LAN_DEV_NUM_MAX] = {
	{RTL_ELAN1_DEV_NAME, 			RTL_ETH_LAN0_BIND_MASK, 	0},
	{RTL_ELAN2_DEV_NAME, 			RTL_ETH_LAN1_BIND_MASK, 	0},
	{RTL_ELAN3_DEV_NAME, 			RTL_ETH_LAN2_BIND_MASK, 	0},
	{RTL_ELAN4_DEV_NAME, 			RTL_ETH_LAN3_BIND_MASK, 	0},
	{RTL_WLAN0_DEV_NAME, 			RTL_WLAN0_BIND_MASK, 		0},
	{RTL_WLAN0_VA0_DEV_NAME, 		RTL_WLAN0_VA0_BIND_MASK, 	0},
	{RTL_WLAN0_VA1_DEV_NAME, 		RTL_WLAN0_VA1_BIND_MASK, 	0},
	{RTL_WLAN0_VA2_DEV_NAME, 		RTL_WLAN0_VA2_BIND_MASK, 	0},
	{RTL_WLAN0_VA3_DEV_NAME, 		RTL_WLAN0_VA3_BIND_MASK, 	0},
	{RTL_WLAN1_DEV_NAME, 			RTL_WLAN1_BIND_MASK, 		0},
	{RTL_WLAN1_VA0_DEV_NAME, 		RTL_WLAN1_VA0_BIND_MASK, 	0},
	{RTL_WLAN1_VA1_DEV_NAME, 		RTL_WLAN1_VA1_BIND_MASK, 	0},
	{RTL_WLAN1_VA2_DEV_NAME, 		RTL_WLAN1_VA2_BIND_MASK, 	0},
	{RTL_WLAN1_VA3_DEV_NAME, 		RTL_WLAN1_VA3_BIND_MASK, 	0},
};

static struct wan_dev_mapping_router_tables wan_dev_router_tables_mapping[WAN_DEV_NUM_MAX*2] = {
	//{"eth1", 	25, 8000},//for test
	{RTL_ETHWAN1_DEV_NAME, 	25, 8000},
	{RTL_ETHWAN2_DEV_NAME, 	24, 7000},
	{RTL_ETHWAN3_DEV_NAME, 	23, 6000},
	{RTL_ETHWAN4_DEV_NAME, 	22, 5000},
	{RTL_ETHWAN5_DEV_NAME, 	21, 4000},
	{RTL_ETHWAN6_DEV_NAME, 	20, 3000},
	{RTL_ETHWAN7_DEV_NAME, 	19, 2000},
	{RTL_ETHWAN8_DEV_NAME, 	18, 1000},
	{RTL_PPP1_DEV_NAME, 		25, 8000},
	{RTL_PPP2_DEV_NAME, 		24, 7000},
	{RTL_PPP3_DEV_NAME, 		23, 6000},
	{RTL_PPP4_DEV_NAME, 		22, 5000},
	{RTL_PPP5_DEV_NAME, 		21, 4000},
	{RTL_PPP6_DEV_NAME, 		20, 3000},
	{RTL_PPP7_DEV_NAME, 		19, 2000},
	{RTL_PPP8_DEV_NAME, 		18, 1000},
};

static int rtl_get_bind_port_by_wan_interface(BASE_DATA_Tp pdata)
{
	WanIntfacesType wanBindingLanPorts[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};
	WAN_DATA_Tp wan_p ;
	int bind_port_mask =0;
	int index,wan_index;
	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;
	printf("%s.%d.wan_index(%d)\n",__FUNCTION__,__LINE__,wan_index);
	
	if(!apmib_get(MIB_WANIFACE_BINDING_LAN_PORTS,(void*)wanBindingLanPorts))
	{
		fprintf(stderr,"%s:%d get MIB_WANIFACE_BINDING_LAN_PORTS fail!\n",__FUNCTION__,__LINE__);
		return 0;
	}
	for(index=0;index<WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM;index++)
	{			
	 	if(checkLanPortInvalid(index)) continue;
		if(wan_index == wanBindingLanPorts[index])
			bind_port_mask |= (0x1<<index);
	}
	
	printf("%s.%d.bind_port_mask(%x)\n",__FUNCTION__,__LINE__,bind_port_mask);
	return bind_port_mask;
}

int clean_policy_rules(BASE_DATA_Tp pdata)
{
	FILE *fp;
	WAN_DATA_Tp wan_p ;
	unsigned char policy_file[64],buf[128];
	wan_p = (BASE_DATA_Tp)pdata;
	sprintf(policy_file,"/var/policy_rule_wan_%d",wan_p->base.wan_idx);
	if ((fp = fopen(policy_file, "r")) != NULL) 
	{
		while(fgets(buf,sizeof(buf), fp))
		{		  
		   system(buf);
		}
		fclose(fp);
		unlink(policy_file);
	}
	
}
int rtl_set_policy_router_by_wan_interface(BASE_DATA_Tp pdata)
{
	int i,wan_index;
	char router_table_id_str[8];
	char wan_ip_str[32];
	char gw_ip_str[32];
	
	//char wan_mask_str[32];
	char	default_gateway_str[32];
	char	mapping_mark_str[32];
	char	mapping_mark_bak_str[32];
	int router_table_id = 0;
	int bind_port_mask = 0;
	int mapping_mark_value = 0;	
	WAN_DATA_Tp wan_p ;
	unsigned char dev_name[32],cmd_buff[128],file_name[64];
	struct in_addr wan_addr = {0}, wan_mask = {0}, default_gateway = {0};
	char *br0info[50];
	char *bInterface = "br0";
	int get_br0ip =0;
	int get_br0mask =0;	
	char Br0NetSectAddr[30];
	char * strbr0Ip ,* strbr0Mask ;
	struct in_addr br0addr,br0mask;
	unsigned int numofone ;	
	char NumStr[10];
	
	
//	printf("%s.%d.set policy rule\n",__FUNCTION__,__LINE__);
	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;
	/*get wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(dev_name,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(dev_name,wan_p->wan_data.wan_iface);

	memset(gw_ip_str,0,sizeof(gw_ip_str));
	strcpy(gw_ip_str,wan_p->wan_data.gw_addr);

	printf("%s.%d.dev_name(%s)\n",__FUNCTION__,__LINE__,dev_name);
	/*
		jwj: todo get wan mib str by interface name, if proto is bridge, 
		no need to add policy router for this interface.
	*/
#if 1
	/*Get wan ip, mask and default gateway*/	
	getInAddr(dev_name, IP_ADDR_T, (void *)&wan_addr);
//	getInAddr(dev_name, NET_MASK_T, (void *)&wan_mask);
//	wan_addr.s_addr = wan_addr.s_addr & wan_mask.s_addr;
	sprintf(wan_ip_str, "%s", inet_ntoa(wan_addr));
	//sprintf(wan_mask_str, "%s", inet_ntoa(wan_mask));
#endif
#if 0	
	if (strncmp(dev_name, "eth", 3) == 0) {
		/*jwj: todo every static ip/dhcp wan should have its own default gateway mib*/
		apmib_get(MIB_WAN_DEFAULT_GATEWAY,  (void *)&default_gateway);

		if (default_gateway.s_addr == 0)
			memset(default_gateway_str, 0x00, 32);
		else
			sprintf(default_gateway_str, "%s", inet_ntoa(default_gateway));
	} else if (strncmp(dev_name, "ppp", 3) == 0){
		if (getInAddr(dev_name, 0, (void *)&default_gateway) > 0)
			sprintf(default_gateway_str, "%s", inet_ntoa(default_gateway));
	}
#endif

	/*get router table id*/
	for (i=0; i<WAN_DEV_NUM_MAX*2; i++)
	{
		if (strncmp(wan_dev_router_tables_mapping[i].ifname, dev_name, 16) == 0) {
			mapping_mark_value = wan_dev_router_tables_mapping[i].mapping_mark;
			router_table_id = wan_dev_router_tables_mapping[i].router_table_id;
			sprintf(router_table_id_str, "%d", router_table_id);
			sprintf(mapping_mark_str, "%d", mapping_mark_value);
			break;
		}
	}
	printf("%s.%d.router_table_id(%d),wan_addr(%u),",__FUNCTION__,__LINE__,router_table_id,wan_addr.s_addr);
	if ((router_table_id==0) ||(wan_addr.s_addr==0))
		return -1;


#if 1
	get_br0ip = getInAddr(bInterface, IP_ADDR_T,(void *)&br0addr);
	if(get_br0ip ==0 ){
		printf("br0 NO ip!\n");
	}	
	get_br0mask = getInAddr(bInterface, NET_MASK_T,(void *)&br0mask);
	if(get_br0mask ==0){
		printf("br0 No MASK!\n");		
	}
	br0addr.s_addr &= br0mask.s_addr ;
	for(numofone =0;br0mask.s_addr;++numofone)
		br0mask.s_addr &= br0mask.s_addr-1;
	sprintf (NumStr, "%d", numofone);
	strcpy(Br0NetSectAddr,inet_ntoa(br0addr));
	strcat(Br0NetSectAddr,"/");
	strcat(Br0NetSectAddr,NumStr);
#endif

//sprintf(command,"ip route add %s dev br0 table %d",Br0NetSectAddr,wanIfaceIndex +10);
			
	/*Add route table *************** */
	RunSystemCmd(NULL_FILE, "ip", "route", "flush", "table", router_table_id_str, NULL_STR);
	RunSystemCmd(NULL_FILE, "ip", "route", "add",Br0NetSectAddr,"dev",bInterface,"table",router_table_id_str,NULL_STR);
	RunSystemCmd(NULL_FILE, "ip", "route", "add", "default", "via", gw_ip_str,"dev", dev_name, "table", router_table_id_str, NULL_STR);


	/*Add ip rule for policy route ****************/
	RunSystemCmd(NULL_FILE, "ip", "rule", "del", "fwmark", mapping_mark_str, NULL_STR);
	RunSystemCmd(NULL_FILE, "ip", "rule", "add", "fwmark", mapping_mark_str, "lookup", router_table_id_str, NULL_STR);

	/* clean last configure wan policy rule*/
	clean_policy_rules(wan_p);
	
	/*Add ip rule*/
	bind_port_mask = rtl_get_bind_port_by_wan_interface(wan_p);
	printf("%s.%d.bind_port_mask(%d)\n",__FUNCTION__,__LINE__,bind_port_mask);
	for (i=0; i<LAN_DEV_NUM_MAX; i++)
	{
		if (lan_dev_bind_mask_mapping[i].bind_mask & bind_port_mask) {
			lan_dev_bind_mask_mapping[i].mapping_mark_bak = mapping_mark_value;			
//			RunSystemCmd(NULL_FILE, "iptables", "-t", "mangle", "-A", "PREROUTING", "-i", lan_dev_bind_mask_mapping[i].ifname, "-j", "MARK", "--set-mark", mapping_mark_str, NULL_STR);
			RunSystemCmd(NULL_FILE, "iptables", "-t", "mangle", "-A",PREROUTING_CHAIN_NAME[wan_index], "-i", lan_dev_bind_mask_mapping[i].ifname, "-j", "MARK", "--set-mark", mapping_mark_str, NULL_STR);
			
			/*backup policy rule to file*/
			sprintf(file_name,"/var/policy_rule_wan_%d",wan_p->base.wan_idx);
			sprintf(cmd_buff,"iptables -t mangle -D PREROUTING -i %s -j MARK --set-mark %s\n",lan_dev_bind_mask_mapping[i].ifname,mapping_mark_str);		
			write_line_to_file2(file_name,cmd_buff);
			printf("%s.%d.set policy rule done!!\n",__FUNCTION__,__LINE__);
		}
	}

	return 0;
}
#endif

#ifdef CONFIG_RTL_HW_NAPT
int update_hwnat_setting()
{
	int opmode=0;
	//fprintf(stderr,"--------------%s:%d -------------------------\n",__FUNCTION__,__LINE__);
	if(!apmib_get(MIB_OP_MODE, (void *)&opmode))
		goto error;
//for wisp and bridge(?) mode
	{
		if(opmode==BRIDGE_MODE)
		{
			RunSystemCmd("/proc/hw_nat", "echo", "2", NULL_STR);
			return 0;
		}
		else if(opmode==WISP_MODE)
		{
			RunSystemCmd("/proc/hw_nat", "echo", "3", NULL_STR);
			return 0;
		}
	}
//for subMask
	{
		int ivalue=0;
		if(!apmib_get(MIB_SUBNET_MASK,(void*)&ivalue))
			goto error;
		if((ivalue&HW_NAT_LIMIT_NETMASK)!=HW_NAT_LIMIT_NETMASK)
		{
				RunSystemCmd("/proc/hw_nat", "echo", "-1", NULL_STR);
				return 0;
		}		
	}
#ifdef CONFIG_RTK_VOIP
	//for voip
	{
		voipCfgParam_t  voipCfgParam ={0};
		if(!apmib_get(MIB_VOIP_CFG, (void*)&voipCfgParam))
			goto error;
		
		if(!voipCfgParam.hwnat_enable)
		{
			RunSystemCmd(HW_NAT_FILE, "echo", "0", NULL_STR);
			return 0;
		}
	}
#endif

//for url filter
	{
		int urlfilter_enable=0,urlfilter_num=0;
		if(!apmib_get(MIB_URLFILTER_ENABLED,  (void *)&urlfilter_enable))
			goto error;
		if(!apmib_get(MIB_URLFILTER_TBL_NUM,  (void *)&urlfilter_num))
			goto error;
		if(opmode == GATEWAY_MODE && urlfilter_enable!=0 && urlfilter_num>0)
		{
			RunSystemCmd("/proc/hw_nat", "echo", "0", NULL_STR);
			return 0;
		}
	}
//for dos
	{
		int dos_enabled=0;
		if(!apmib_get(MIB_DOS_ENABLED, (void *)&dos_enabled))
			goto error;
		if(dos_enabled>0)
		{
			RunSystemCmd("/proc/hw_nat", "echo", "0", NULL_STR);
			return 0;
		}
	}
//for l2tp,pptp
	{
		#if 1
		int curr_wan_idx = 1, wan_type = 0, num = 0;
		for(curr_wan_idx = 1; curr_wan_idx <= WANIFACE_NUM; curr_wan_idx++)
		{
			apmib_set(MIB_WANIFACE_CURRENT_IDX,(void*)(&curr_wan_idx));
			apmib_get(MIB_WANIFACE_ADDRESSTYPE,(void*)(&wan_type));
			if (wan_type == L2TP || wan_type == PPTP)
				num++;			
		}
		//if all wan type as l2tp or pptp, then disable hw nat.
		if (num == WANIFACE_NUM){
			RunSystemCmd("/proc/hw_nat", "echo", "0", NULL_STR);
        	return 0;
		}
		#else
    	int wan_dhcp;

        apmib_get(MIB_WAN_DHCP, (void *)&wan_dhcp);
        if ((wan_dhcp==L2TP) || (wan_dhcp==PPTP)) {
			RunSystemCmd("/proc/hw_nat", "echo", "0", NULL_STR);
        	return 0;
		}
		#endif
    }
	//printf("%s:%d\n",__FUNCTION__,__LINE__);
	RunSystemCmd(HW_NAT_FILE, "echo", "1", NULL_STR);
	return 0;
error:
	printf("update hardware nat error!\n");
	RunSystemCmd(HW_NAT_FILE, "echo", "-1", NULL_STR);
	return -1;
}
#endif

int rtl_set_DMZ(BASE_DATA_Tp pdata)
{	
	int i;
	int get_wanip=0,mapping_mark_value;
	int dmz_enable,wan_index;
	char IpAddrBuf[30],IpAddr[30], *strIp;		
	char WanIpAddr[30],mapping_mark_str[32],*strWanIp;
	unsigned char dev_name[32];	
	struct in_addr wanaddr;
	WAN_DATA_Tp wan_p ;
	
	apmib_get( MIB_DMZ_ENABLED, (void *)&dmz_enable);
	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;

	/*start to flush dmz firewall chain **************/
	RunSystemCmd(NULL_FILE,Iptables,FLUSH,DMZ_FORWARD_NAME[wan_index], NULL_STR);				
	RunSystemCmd(NULL_FILE,Iptables,_table,nat_table,FLUSH, DMZ_PREROUTING_NAME[wan_index],NULL_STR);
	RunSystemCmd(NULL_FILE,Iptables,_table,nat_table,FLUSH, DMZ_POSTROUTING_NAME[wan_index],NULL_STR);
		
	if(!dmz_enable)
		return;

	apmib_get( MIB_DMZ_HOST,  (void *)IpAddrBuf);	
	
	/*get wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(dev_name,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(dev_name,wan_p->wan_data.wan_iface);	

	get_wanip = getInAddr(dev_name, IP_ADDR_T, (void *)&wanaddr);
	if( get_wanip ==0){   //get wan ip fail
		//printf("No wan ip currently!\n");
		return 0;
	}else{
		strWanIp = inet_ntoa(wanaddr);
		strcpy(WanIpAddr, strWanIp);
	}
	
#if defined(CONFIG_RTL_IP_POLICY_ROUTING_SUPPORT)	
	/*get lan<---> binding value*/
	for (i=0; i<WAN_DEV_NUM_MAX*2; i++)
	{
		if (strncmp(wan_dev_router_tables_mapping[i].ifname, dev_name, 16) == 0) {
			mapping_mark_value = wan_dev_router_tables_mapping[i].mapping_mark;
			sprintf(mapping_mark_str, "%d", mapping_mark_value);
			break;
		}
	}	
#endif	
	strIp = inet_ntoa(*((struct in_addr *)IpAddrBuf));
	if(strcmp(strIp, "0.0.0.0"))
	{
		unsigned char command[128];
		strcpy(IpAddr, strIp);
		RunSystemCmd(NULL_FILE, Iptables,ADD,DMZ_FORWARD_NAME[wan_index], in, dev_name, _dest, IpAddr, jump, ACCEPT, NULL_STR);		
		RunSystemCmd(NULL_FILE, Iptables,_table, nat_table, ADD, DMZ_PREROUTING_NAME[wan_index], _dest, WanIpAddr, jump, DNAT, "--to", IpAddr, NULL_STR);
	//	RunSystemCmd(NULL_FILE, Iptables, DEL, INPUT, _protocol, _tcp, dport, "80:80", in, pInterface, _dest, WanIpAddr, jump, DROP, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables,_table, nat_table, ADD, DMZ_POSTROUTING_NAME[wan_index],"-m","mark","--mark",mapping_mark_str,_dest,IpAddr,jump,"SNAT","--to",WanIpAddr,NULL_STR);

		sprintf(command,"iptables -t nat -A %s -m mark --mark %d -d %s -j SNAT --to %s",
				DMZ_POSTROUTING_NAME[wan_index],mapping_mark_value,IpAddr,WanIpAddr);
		//system(command);
//		RunSystemCmd(NULL_FILE, Iptables,_table, nat_table,ADD, DMZ_POSTROUTING_NAME[wan_index],match,MARK,"--mark",mapping_mark_str,_dest,IpAddr,jump,"SNAT","--to",WanIpAddr,NULL_STR);
		printf("%s.%d.set dmz (%s)\n",__FUNCTION__,__LINE__,command);
		//-m mark --mark 100 -d 192.168.1.99 -j SNAT --to 192.168.2.200
	}
}

void reinit_fun_firewall_dmz()
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;

	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{
		#if 0
			/*start to flush dmz firewall chain **************/
			RunSystemCmd(NULL_FILE,Iptables,FLUSH,DMZ_FORWARD_NAME[wan_index], NULL_STR);				
			RunSystemCmd(NULL_FILE,Iptables,_table,nat_table,FLUSH, DMZ_PREROUTING_NAME[wan_index],NULL_STR);
			RunSystemCmd(NULL_FILE,Iptables,_table,nat_table,FLUSH, DMZ_POSTROUTING_NAME[wan_index],NULL_STR);
		#endif	
			/*start to set dmz firewall rules ****************/
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_DMZ(&wan_info);
		}
	}
	
}

#if defined(CONFIG_APP_TR069)
void reinit_fun_firewall_tr069()
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;

	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_tr069_firewall_rule(&wan_info);
		}
	}
	
}

void rtl_set_tr069_firewall_rule(BASE_DATA_Tp pdata)
{
	int cwmp_flag = 0, wan_index = 0, get_wanip = 0;
	int conReqPort = 0;
	char acsUrl[CWMP_ACS_URL_LEN+1] = {0};
	char acsUrlRange[2*(CWMP_ACS_URL_LEN+1)] = {0};
	char conReqPortRange[2*(5+1)] = {0};
	WAN_DATA_Tp wan_p ;
	WANIFACE_T WanIfaceEntry;
	char pIfaceWan[32] = {0}, pIpaddrWan[30] = {0};
	struct in_addr wanaddr;	
	char *strWanIp = NULL;
	
	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;

	//get current wan info.
	memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
	if(!getWanIfaceEntry(wan_index,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return 0;
	}
	
	//tr069 related firewall rule only set for wan application type as APPTYPE_TR069
	//if (WanIfaceEntry.applicationtype != APPTYPE_TR069)
		//return 0;
	
	/*get wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(pIfaceWan,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(pIfaceWan,wan_p->wan_data.wan_iface);
	
	//get wan ip address
	get_wanip = getInAddr(pIfaceWan, IP_ADDR_T, (void *)&wanaddr);
	if( get_wanip ==0){   //get wan ip fail
		//printf("No wan ip currently!\n");
		return 0;
	}else{
		strWanIp = inet_ntoa(wanaddr);
		strcpy(pIpaddrWan, strWanIp);
	}

	//set tr069 related firewall rules
	apmib_get( MIB_CWMP_FLAG, (void *)&cwmp_flag );
	if(cwmp_flag & CWMP_FLAG_AUTORUN)
	{
		apmib_get( MIB_CWMP_CONREQ_PORT, (void *)&conReqPort);
		if(conReqPort >0 && conReqPort<65535)
		{
			
			apmib_get( MIB_CWMP_ACS_URL, (void *)acsUrl);
			if((strstr(acsUrl,"https://") != 0 || strstr(acsUrl,"http://") != 0) && strlen(acsUrl) != 0)
			{
				char *lineptr = acsUrl;
				char *str=NULL;

				
				str = strsep(&lineptr,"/");
				str = strsep(&lineptr,"/");
				str = strsep(&lineptr,"/");

				if(str != NULL && strlen(str) != 0)
				{
					sprintf(acsUrlRange,"%s-%s",str,str);
					
					sprintf(conReqPortRange,"%d:%d",conReqPort,conReqPort);
					
					/* flush the chain first **************/
					RunSystemCmd(NULL_FILE,Iptables,FLUSH,TR069_INPUT_NAME[wan_index], NULL_STR);				
					////then add .....
					RunSystemCmd(NULL_FILE, Iptables,ADD,TR069_INPUT_NAME[wan_index], _protocol, _tcp, dport, conReqPortRange, in, pIfaceWan, _dest, pIpaddrWan, jump, ACCEPT, NULL_STR); 	
				}
			}			
						
		}				
		
	}	
	
}
#endif

void rtl_set_static_route(BASE_DATA_Tp pdata)
{
	unsigned long v1, v2, v3;	
	unsigned long ipAddr, curIpAddr, curSubnet;
	int check_ip1=0;
	int check_ip2=0;

	int intValue=0, wan_index,i;
	char line_buffer[64]={0};
	char tmp_args[16]={0};
	char	ip[32], netmask[32], gateway[32], *tmpStr=NULL; 
	unsigned char interface[32];
	char wan_ip_str[32];
	struct in_addr wan_addr = {0};
	WAN_DATA_Tp wan_p ;
	int entry_Num=0;
	STATICROUTE_T entry;
	
	wan_p = (WAN_DATA_Tp)pdata;	
	wan_index = wan_p->base.wan_idx;
	getWanIface(pdata, interface);
	
	apmib_get(MIB_STATICROUTE_ENABLED, (void *)&intValue);
	apmib_get(MIB_STATICROUTE_TBL_NUM, (void *)&entry_Num);
	if(intValue > 0 && entry_Num > 0)
	{
		for (i=1; i<= entry_Num; i++) 
		{
			*((char *)&entry) = (char)i;
			apmib_get(MIB_STATICROUTE_TBL, (void *)&entry);
			
			if(entry.interface >=1 && (wan_index != entry.interface))
				continue;
			
			if(entry.metric < 0)
				continue;
			
			tmpStr = inet_ntoa(*((struct in_addr *)entry.dstAddr));
			sprintf(ip, "%s", tmpStr);
			tmpStr = inet_ntoa(*((struct in_addr *)entry.netmask));
			sprintf(netmask, "%s", tmpStr);
			tmpStr = inet_ntoa(*((struct in_addr *)entry.gateway));
			sprintf(gateway, "%s", tmpStr);
			sprintf(tmp_args, "%d", entry.metric);

#if 1
			/*check weather valid for gateway & interface *******/
			/*just check lan/ethernet wan *****************/
			if(strncmp(interface, "ppp",3) || entry.interface==0)
			{
				memcpy((void *)&ipAddr, (void *)entry.gateway, 4);
				check_ip1= getInAddr(interface, IP_ADDR, (void *)&curIpAddr);
				check_ip2= getInAddr(interface, SUBNET_MASK, (void *)&curSubnet);
				v1 = ipAddr;
				v2 = curIpAddr;
				v3 = curSubnet;
				if (v1 && check_ip1 && check_ip2) 
				{
					if ( (v1 & v3) != (v2 & v3) ) 
					{
						printf("%s.%d.invalid Gateway address\n");
						continue;
					}
				}
			}
#endif			
			if(!strncmp(interface, "ppp",3))
			{
				if(entry.interface >=1)
				{
					RunSystemCmd(NULL_FILE, "route", "add", "-net", ip, "netmask", netmask, "metric", tmp_args, "dev", interface,  NULL_STR);
				}
				else
				{
					RunSystemCmd(NULL_FILE, "route", "add", "-net", ip, "netmask", netmask, "gw",  gateway, "metric", tmp_args, "dev", "br0",  NULL_STR);
				}
			}
			else
			{
				if(entry.interface >=1)
				{
					RunSystemCmd(NULL_FILE, "route", "add", "-net", ip, "netmask", netmask, "gw",  gateway, "metric", tmp_args, "dev", interface,  NULL_STR);
				}
				else if(entry.interface==0)
				{
					RunSystemCmd(NULL_FILE, "route", "add", "-net", ip, "netmask", netmask, "gw",  gateway, "metric", tmp_args, "dev", "br0",  NULL_STR);
				}
			}
			printf("%s.%d.static route set done!!\n",__FUNCTION__,__LINE__);
		}
	}	
	
}


void reinit_fun_static_route()
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;
	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_static_route(&wan_info);
		}
	}	
}

void rtl_set_port_forward(BASE_DATA_Tp pdata)
{
	char PortRange[60],ip[30],pIfaceWan[32],pIpaddrWan[30];
	char *tmpStr,*strWanIp;;
	int entryNum=0,index,wan_index,get_wanip,portforward_enable;
	PORTFW_T entry;
	WAN_DATA_Tp wan_p ;		
	struct in_addr wanaddr;	

	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;
	
	apmib_get(MIB_PORTFW_ENABLED,  (void *)&portforward_enable);
	apmib_get(MIB_PORTFW_TBL_NUM, (void *)&entryNum);

	RunSystemCmd(NULL_FILE, "iptables", "-F", PORTFORWARD_PREROUTING_NAME[wan_index],"-t","nat",NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORTFORWARD_FORWARD_NAME[wan_index], NULL_STR);

	if(portforward_enable == 0 || entryNum==0)
		return; 

	/*get wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(pIfaceWan,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(pIfaceWan,wan_p->wan_data.wan_iface);	

	get_wanip = getInAddr(pIfaceWan, IP_ADDR_T, (void *)&wanaddr);
	if( get_wanip ==0){   //get wan ip fail
		//printf("No wan ip currently!\n");
		return 0;
	}else{
		strWanIp = inet_ntoa(wanaddr);
		strcpy(pIpaddrWan, strWanIp);
	}	
	for (index=1; index <= entryNum; index++) 
	{
		memset(&entry, '\0', sizeof(entry));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_PORTFW_TBL, (void *)&entry);

		tmpStr = inet_ntoa(*((struct in_addr *)entry.ipAddr));
		sprintf(ip, "%s", tmpStr);

		sprintf(PortRange, "%d:%d", entry.fromPort, entry.toPort);
		
		if(entry.protoType ==PROTO_TCP)
		{
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_PREROUTING_NAME[wan_index], _table, nat_table, _protocol, _tcp, dport, PortRange,_dest, pIpaddrWan, jump, DNAT, "--to", ip, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_FORWARD_NAME[wan_index], in, pIfaceWan, _dest, ip, _protocol, _tcp, dport, PortRange, jump , ACCEPT, NULL_STR);
		}
		if(entry.protoType ==PROTO_UDP)
		{
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_PREROUTING_NAME[wan_index], _table, nat_table, _protocol, _udp, dport, PortRange,_dest, pIpaddrWan, jump, DNAT, "--to", ip, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_FORWARD_NAME[wan_index], in, pIfaceWan, _dest, ip, _protocol, _udp, dport, PortRange, jump , ACCEPT, NULL_STR);
		}
		if(entry.protoType ==PROTO_BOTH)
		{
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_PREROUTING_NAME[wan_index], _table, nat_table, _protocol, _tcp, dport, PortRange,_dest, pIpaddrWan, jump, DNAT, "--to", ip, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_FORWARD_NAME[wan_index], in, pIfaceWan, _dest, ip, _protocol, _tcp, dport, PortRange, jump , ACCEPT, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_PREROUTING_NAME[wan_index], _table, nat_table, _protocol, _udp, dport, PortRange,_dest, pIpaddrWan, jump, DNAT, "--to", ip, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORTFORWARD_FORWARD_NAME[wan_index], in, pIfaceWan, _dest, ip, _protocol, _udp, dport, PortRange, jump , ACCEPT, NULL_STR);
		}
	}
	printf("%s.%d.set portforward rules\n",__FUNCTION__,__LINE__);
	return 0;
}


void reinit_fun_firewall_portforward()
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;

	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{	
			/*start to set dmz firewall rules ****************/
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_port_forward(&wan_info);
		}
	}
}


void reinit_fun_firewall_urlfilter(void)
{
	char keywords[500];
	char cmdBuffer[500];
	char macAddr[30];
	char tmp1[64]={0};
	URLFILTER_T entry;
	int entryNum=0, index;
	int mode,i=0;
	int urlEnabled,intVal_num;
	char c = 22;	//unseen char to distinguish

	//url filter setting
	apmib_get(MIB_URLFILTER_ENABLED,  (void *)&urlEnabled);
	apmib_get(MIB_URLFILTER_TBL_NUM,  (void *)&intVal_num);

	RunSystemCmd("/proc/filter_table", "echo", "flush", NULL_STR);
	RunSystemCmd("/proc/filter_table", "echo", "init", "3",  NULL_STR);

	/*if url fiter disable or no url rules *********/
	if(urlEnabled ==0 || intVal_num ==0)
	{
		return ;
	}
	
	printf("set urlfilter\n");
	/*add URL filter Mode 0:Black list 1:White list*/
	apmib_get(MIB_URLFILTER_MODE,  (void *)&mode);
	apmib_get(MIB_URLFILTER_TBL_NUM, (void *)&entryNum);
	//sprintf(keywords, "%d ", entryNum);
	bzero(keywords,sizeof(keywords));
	for (index=1; index<= entryNum; index++) 
	{
		memset(&entry, '\0', sizeof(entry));
		bzero(tmp1,sizeof(tmp1));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_URLFILTER_TBL, (void *)&entry);
		if(mode!=entry.ruleMode)
			continue;
		strcpy(tmp1,entry.urlAddr);
		if(!strncmp(tmp1,"http://",7))
			for(i=7;i<sizeof(tmp1);i++)
				tmp1[i-7]=tmp1[i];
	//printf("%s:%d tmp1=%s\n",__FUNCTION__,__LINE__,tmp1);
		if(!strncmp(tmp1,"www.",4))
			for(i=4;i<sizeof(tmp1);i++)
				tmp1[i-4]=tmp1[i];
	
	//printf("%s:%d entryNum=%d\n",__FUNCTION__,__LINE__,entryNum);
		if(changeDividerToESC(tmp1,sizeof(tmp1)," #:\\")<0)
			return -1;
		//printf("%s:%d tmp1=%s\n",__FUNCTION__,__LINE__,tmp1);
		sprintf(tmp1, "%s ;", tmp1);
		//printf("%s:%d tmp1=%s\n",__FUNCTION__,__LINE__,tmp1);
		strcat(keywords, tmp1);
	}
		
	if(mode)
		RunSystemCmd("/proc/filter_table", "echo", "white", NULL_STR);
	else
		RunSystemCmd("/proc/filter_table", "echo", "black", NULL_STR);
	//sprintf(cmdBuffer, "%s", keywords);
	//RunSystemCmd("/proc/url_filter", "echo", cmdBuffer, NULL_STR);//disable h/w nat when url filter enabled
	sprintf(cmdBuffer, "add:0#3 3 %s",keywords);
	printf("%s:%d cmdBuffer=%s\n",__FUNCTION__,__LINE__,cmdBuffer);
	//sleep(1);
	RunSystemCmd("/proc/filter_table", "echo", cmdBuffer, NULL_STR);
	return 0;
}


void  rtl_set_url_filter(void)
{
	reinit_fun_firewall_urlfilter();
}
void reinit_fun_firewall_macfilter(void)
{
	int macfilterEnable,entryNum,index;
	char macEntry[30],cmdBuffer[80];
	MACFILTER_T entry;

	apmib_get(MIB_MACFILTER_ENABLED,  (void *)&macfilterEnable);
	apmib_get(MIB_MACFILTER_TBL_NUM, (void *)&entryNum);

	RunSystemCmd(NULL_FILE, "iptables", "-F", MAC_FILTER_NAME[0], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-F", MAC_FILTER_NAME[1], NULL_STR);
	
	if(macfilterEnable ==0 || entryNum ==0)
		return ;
	
	for (index=1; index <= entryNum; index++) 
	{
		memset(&entry, '\0', sizeof(entry));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_MACFILTER_TBL, (void *)&entry);
		sprintf(macEntry,"%02X:%02X:%02X:%02X:%02X:%02X", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		RunSystemCmd(NULL_FILE, Iptables, ADD, MAC_FILTER_NAME[0], match, "mac" ,mac_src, macEntry, jump, DROP, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, ADD, MAC_FILTER_NAME[1], match, "mac" ,mac_src, macEntry, jump, DROP, NULL_STR);
		memset(cmdBuffer, 0, sizeof(cmdBuffer));
		sprintf(cmdBuffer, "rtk_cmd igmp_delete %02X:%02X:%02X:%02X:%02X:%02X", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2], entry.macAddr[3], entry.macAddr[4], entry.macAddr[5]);
		system(cmdBuffer);
	}
	printf("%s.%d.set mac filter rules\n",__FUNCTION__,__LINE__);
	return 0;	
}
void rtl_set_mac_filter()
{
	reinit_fun_firewall_macfilter();
}


void reinit_fun_firewall_ipfilter(void)
{
	int ipfilterEnable,entryNum,index;
	IPFILTER_T entry;
	char ipAddr[30];
	char *tmpStr;
	apmib_get(MIB_IPFILTER_ENABLED,  (void *)&ipfilterEnable);
	apmib_get(MIB_IPFILTER_TBL_NUM, (void *)&entryNum);

	RunSystemCmd(NULL_FILE, "iptables", "-F", IP_FILTER_NAME[0], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-F", IP_FILTER_NAME[1], NULL_STR);

	if(ipfilterEnable ==0 || entryNum ==0)
		return ;

	for(index=1; index <= entryNum ; index++) 
	{
		memset(&entry, '\0', sizeof(entry));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_IPFILTER_TBL, (void *)&entry);

		tmpStr = inet_ntoa(*((struct in_addr *)entry.ipAddr));
		sprintf(ipAddr, "%s", tmpStr);

		if(entry.protoType==PROTO_TCP){
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[1], _protocol, _tcp, _src, ipAddr, jump, DROP, NULL_STR);
		}
		if(entry.protoType==PROTO_UDP){
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[1], _protocol, _udp, _src, ipAddr, jump, DROP, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[0], _protocol, _udp, dport, "53:53", _src, ipAddr, jump, DROP, NULL_STR);
		}
		if(entry.protoType==PROTO_BOTH)	{
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[1], _protocol, _tcp, _src, ipAddr, jump, DROP, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[1], _protocol, _udp, _src, ipAddr, jump, DROP, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, IP_FILTER_NAME[0], _protocol, _udp, dport, "53:53", _src, ipAddr, jump, DROP, NULL_STR);
		}
	}
	printf("%s.%d.set ip filter rules\n",__FUNCTION__,__LINE__);
}


void rtl_set_ip_filter()
{
	reinit_fun_firewall_ipfilter();
}

void  reinit_fun_firewall_portfilter(void)
{
	char PortRange[30];
	int portfilter_enable,entryNum,index;
	PORTFILTER_T entry;
	
	apmib_get(MIB_PORTFILTER_ENABLED,  (void *)&portfilter_enable);
	apmib_get(MIB_PORTFILTER_TBL_NUM, (void *)&entryNum);

	RunSystemCmd(NULL_FILE, "iptables", "-F", PORT_FILTER_NAME[0], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORT_FILTER_NAME[1], NULL_STR);

	if(portfilter_enable ==0 || entryNum ==0)
		return ;
	
	for (index=1; index <= entryNum; index++) 
	{
		memset(&entry, '\0', sizeof(entry));
		*((char *)&entry) = (char)index;
		apmib_get(MIB_PORTFILTER_TBL, (void *)&entry);
		sprintf(PortRange, "%d:%d", entry.fromPort, entry.toPort);
		
		if(entry.protoType==PROTO_TCP)
		{
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[1], _protocol, _tcp, dport, PortRange, jump, DROP, NULL_STR);
		}
		
		if(entry.protoType==PROTO_UDP)
		{
			if(entry.fromPort<53 && entry.toPort >= 53)
			{
				RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[0], _protocol, _udp, dport, "53:53", jump, DROP, NULL_STR);
			}
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[1], _protocol, _udp, dport, PortRange, jump, DROP, NULL_STR);
		}
		
		if(entry.protoType==PROTO_BOTH)	
		{
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[1], _protocol, _tcp, dport, PortRange, jump, DROP, NULL_STR);
			RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[1], _protocol, _udp, dport, PortRange, jump, DROP, NULL_STR);
			if(entry.fromPort<53 && entry.toPort >= 53)
			{
				RunSystemCmd(NULL_FILE, Iptables, ADD, PORT_FILTER_NAME[0], _protocol, _udp, dport, "53:53", jump, DROP, NULL_STR);
			}
		}
	}

	printf("%s.%d. port filter rules\n",__FUNCTION__,__LINE__);
	return 0;
}

void rtl_set_port_filter()
{
	reinit_fun_firewall_portfilter();
}

void rtl_set_ping_access(BASE_DATA_Tp pdata)
{
	int index,wan_index;
	int pingEnable;
	unsigned char dev_name[32];
	char wan_ip_str[32];
	struct in_addr wan_addr = {0};
	WAN_DATA_Tp wan_p ;
	
	wan_p = (WAN_DATA_Tp)pdata;	
	wan_index = wan_p->base.wan_idx;
	getWanIface(pdata, dev_name);
	/*Get wan ip *******/	
	getInAddr(dev_name, IP_ADDR_T, (void *)&wan_addr);
	sprintf(wan_ip_str, "%s", inet_ntoa(wan_addr));
	
	#if 0
	/*flush firewall for wan ping access*/
	RunSystemCmd(NULL_FILE, Iptables, DEL, INPUT_CHAIN_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	RunSystemCmd(NULL_FILE, Iptables, DEL, INPUT_CHAIN_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR); 	
	
	apmib_get( MIB_PING_WAN_ACCESS_ENABLED, (void *)&pingEnable);
	if(pingEnable==1)
		RunSystemCmd(NULL_FILE, Iptables, ADD, INPUT_CHAIN_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	else
		RunSystemCmd(NULL_FILE, Iptables, ADD, INPUT_CHAIN_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR); 
	#endif
	//flush 
	RunSystemCmd(NULL_FILE, "iptables", "-F", PING_INPUT_NAME[wan_index], NULL_STR);	
	//then add...
	apmib_get( MIB_PING_WAN_ACCESS_ENABLED, (void *)&pingEnable);
	if(pingEnable==1)
		RunSystemCmd(NULL_FILE, Iptables, ADD, PING_INPUT_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	else
		RunSystemCmd(NULL_FILE, Iptables, ADD, PING_INPUT_NAME[wan_index], _protocol, _icmp, icmp_type, echo_request,	in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR); 

	printf("%s.%d.ping access\n",__FUNCTION__,__LINE__);
}

void  reinit_fun_firewall_pingAccess(void)
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;

	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{
			/*start to set dmz firewall rules ****************/
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_ping_access(&wan_info);
		}
	}
	printf("%s.%d. ping access rules\n",__FUNCTION__,__LINE__);
	return 0;	
}


void rtl_set_web_access(BASE_DATA_Tp pdata)
{
	int index,wan_index;
	int accessEnable;
	unsigned char dev_name[32];
	char wan_ip_str[32];
	struct in_addr wan_addr = {0};
	WAN_DATA_Tp wan_p ;
	
	wan_p = (WAN_DATA_Tp)pdata;	
	wan_index = wan_p->base.wan_idx;
	getWanIface(pdata, dev_name);
	/*Get wan ip *******/	
	getInAddr(dev_name, IP_ADDR_T, (void *)&wan_addr);
	sprintf(wan_ip_str, "%s", inet_ntoa(wan_addr));

	#if 0
	/*flush firewall for wan ping access*/
	RunSystemCmd(NULL_FILE, Iptables, DEL, INPUT_CHAIN_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	RunSystemCmd(NULL_FILE, Iptables, DEL, INPUT_CHAIN_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR);

	apmib_get( MIB_WEB_WAN_ACCESS_ENABLED, (void *)&accessEnable);
	if(accessEnable==1)
		RunSystemCmd(NULL_FILE, Iptables, ADD, INPUT_CHAIN_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	else
		RunSystemCmd(NULL_FILE, Iptables, ADD, INPUT_CHAIN_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR);
	#endif
	//flush 
	RunSystemCmd(NULL_FILE, "iptables", "-F", WEB_INPUT_NAME[wan_index], NULL_STR);	
	//then add...
	apmib_get( MIB_WEB_WAN_ACCESS_ENABLED, (void *)&accessEnable);
	if(accessEnable==1)
		RunSystemCmd(NULL_FILE, Iptables, ADD, WEB_INPUT_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, ACCEPT, NULL_STR);
	else
		RunSystemCmd(NULL_FILE, Iptables, ADD, WEB_INPUT_NAME[wan_index], _protocol, _tcp,  dport, "80:80", in, dev_name, _dest, wan_ip_str, jump, DROP, NULL_STR);

	printf("%s.%d.web access\n",__FUNCTION__,__LINE__);
}

void  reinit_fun_firewall_web_access(void)
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T wan_info;
	BASE_DATA_T base;

	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.enable)
		{
			/*start to set dmz firewall rules ****************/
			base.wan_idx = wan_index;
			if(!format_wan_data(&wan_info,&base))
				return 0;
			rtl_set_web_access(&wan_info);
		}
	}
	printf("%s.%d. web access rules\n",__FUNCTION__,__LINE__);
	return 0;	
}

static int rtl_set_wan_rules(BASE_DATA_Tp pdata)
{
	int wan_index;
	WAN_DATA_Tp wan_p ;
	unsigned char dev_name[32];
	char wan_ip_str[32];
	struct in_addr wan_addr = {0};
	

	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;
	#if 0
	/*get wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(dev_name,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(dev_name,wan_p->wan_data.wan_iface);	
	#endif	
	/*get wan interface**/
	getWanIface(pdata, dev_name);


	RunSystemCmd(NULL_FILE, Iptables, ADD, FORWARD_CHAIN_NAME[wan_index], in, dev_name, match, mstate, state,RELATED_ESTABLISHED,jump,ACCEPT,NULL_STR);		
	printf("%s.%d.set wan rules\n",__FUNCTION__,__LINE__);
}
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
static int rtl_set_igmpproxy_rules(BASE_DATA_Tp pdata)
{
	int wan_index;
	WAN_DATA_Tp wan_p;

	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;
	char str_bridge[32] = {0};
	sprintf(str_bridge, "br%d", wan_index);

	//printf("wan_index = %d, [%s:%d]\n", wan_index, __FUNCTION__, __LINE__);

	if(wan_p->wan_data.wan_type == PPPOE)
	{
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_FORWARD_NAME[wan_index], _protocol, "udp", in, wan_p->wan_data.wan_iface, _dest, "224.0.0.0/4", jump,ACCEPT,NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_FORWARD_NAME[wan_index], _protocol, "udp", in, wan_p->wan_data.virtual_wan_iface, _dest, "224.0.0.0/4", jump,ACCEPT,NULL_STR); 	
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_INPUT_NAME[wan_index], _protocol, "2", in, wan_p->wan_data.wan_iface, jump,ACCEPT,NULL_STR); 	
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_INPUT_NAME[wan_index], _protocol, "2", in, wan_p->wan_data.virtual_wan_iface, jump,ACCEPT,NULL_STR); 	
	}
	else if(wan_p->wan_data.wan_type == BRIDGE)
	{
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_INPUT_NAME[wan_index], _protocol, "2", in, str_bridge, jump,ACCEPT,NULL_STR); 
	}
	else
	{
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_FORWARD_NAME[wan_index], _protocol, "udp", in, wan_p->wan_data.wan_iface, _dest, "224.0.0.0/4", jump,ACCEPT,NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, ADD, IGMP_INPUT_NAME[wan_index], _protocol, "2", in, wan_p->wan_data.wan_iface, jump,ACCEPT,NULL_STR); 
	}
	
	//RunSystemCmd(NULL_FILE, "iptables -nvL", NULL_STR);
		
}
#endif
static int rtl_set_common_rules()
{
	if(isFileExist("/var/system/common_chain"))
		return 0;

	/*set common firewall rules.users defined chain ---
		INPUT_COMMON,FORWARD_COMMON,OUTPUT_COMMON*/

	RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_HEAD_START,match, mstate, state,RELATED_ESTABLISHED,jump,ACCEPT,NULL_STR);	
	
	/*input end position chain ****************/
	RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_TAIL_START,"-i","br0","-j","ACCEPT",NULL_STR);		
	RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_TAIL_START,"-i","lo","-j","ACCEPT",NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_TAIL_START,"-i","br0","-j","ACCEPT",NULL_STR);
	/*create file to prevent recreate common chain **********/
	RunSystemCmd("/var/system/common_chain", "echo", "1", NULL_STR);	
	return 1;
}

static rtl_set_nat_rules(BASE_DATA_Tp pdata)
{
	WAN_DATA_Tp wan_p ;
	int wan_index;	
	unsigned char wan_iface[32];
	
	wan_p = (WAN_DATA_Tp)pdata;
	wan_index = wan_p->base.wan_idx;

	getWanIface(pdata,wan_iface);
	
	/*set SNAT rules for each wan interface for nat mode*/
	RunSystemCmd(NULL_FILE, Iptables, _table, nat_table, ADD, POSTROUTING_CHAIN_NAME[wan_index], out, wan_iface, jump, MASQUERADE, NULL_STR);	
}

static void rtl_set_redirect_acl_rules_and_port_mapping(BASE_DATA_Tp pdata)
{
	 WAN_DATA_Tp wan_p ;
	 int wan_index;	
	 unsigned char wan_iface[32];
	 int i, bind_port_mask = 0;
	 unsigned char cmd_buf[128];
	 WanIntfacesType wanBindingLanPorts[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};
	 
	 wan_p = (WAN_DATA_Tp)pdata;
	 wan_index = wan_p->base.wan_idx;

	 getWanIface(pdata, wan_iface);
	 if (!apmib_get(MIB_WANIFACE_BINDING_LAN_PORTS, (void *)&wanBindingLanPorts)) {
 		reinitSer_printf(LOG_ERR,"%s:%d get MIB_WANIFACE_BINDING_LAN_PORTS fail!\n",__FUNCTION__,__LINE__);
		return;
	 }

	 for (i=0; i<WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM; i++)
	 {
	 	if(checkLanPortInvalid(i)) continue;
		if ((wanBindingLanPorts[i]!=NO_WAN) && (wanBindingLanPorts[i]==wan_p->base.wan_idx)) {
			bind_port_mask |= 1<<i;
		}
	 }

	if (bind_port_mask != 0) {
#if defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
		sprintf(cmd_buf,"echo \" add %d %s %s %s\" > /proc/adv_rt ", bind_port_mask, wan_p->wan_data.ip_addr, wan_p->wan_data.gw_addr, wan_iface);
		system(cmd_buf);
		memset(cmd_buf, 0, 128);
#endif
		/*Add port mapping for wan dev*/
		sprintf(cmd_buf,"vconfig port_mapping %s %x 1", wan_p->wan_data.wan_iface, bind_port_mask);				
		system(cmd_buf);

#if defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
		if(wan_p->wan_data.wan_type == PPPOE)
			sprintf(cmd_buf, "vconfig port_mapping %s %x 1", wan_p->wan_data.virtual_wan_iface, bind_port_mask);
			system(cmd_buf);
#endif
	}
}

/* non-leaf chain maybe include jump rule for user defined chain, 
    so please donot flush non-leaf chain in this function!!!!  
 */
static int flushFirewallChain(BASE_DATA_Tp pdata)
{	
	WAN_DATA_Tp wan_p ;	
	int wanIdx;
	
	wan_p = (WAN_DATA_Tp)pdata;	
	wanIdx = wan_p->base.wan_idx;

	#if 0
	RunSystemCmd(NULL_FILE, "iptables", "-t","filter","-F", INPUT_CHAIN_NAME[wanIdx], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-t","filter","-F", OUTPUT_CHAIN_NAME[wanIdx], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-t","filter","-F", FORWARD_CHAIN_NAME[wanIdx], NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-t","nat",   "-F", PREROUTING_CHAIN_NAME[wanIdx],NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-t","nat",   "-F", POSTROUTING_CHAIN_NAME[wanIdx],  NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-t","mangle","-F", PREROUTING_CHAIN_NAME[wanIdx], NULL_STR);
	#endif
	

	/*flush leaf node firewall rule for each wan by wan index ***********************/
	/*flush dmz rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", DMZ_FORWARD_NAME[wanIdx], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", DMZ_PREROUTING_NAME[wanIdx],"-t","nat",NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", DMZ_POSTROUTING_NAME[wanIdx],"-t","nat",NULL_STR);

	/*flush portforward rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORTFORWARD_FORWARD_NAME[wanIdx], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORTFORWARD_PREROUTING_NAME[wanIdx],"-t","nat",NULL_STR);		

	/*flush mac filter  rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", MAC_FILTER_NAME[0], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", MAC_FILTER_NAME[1], NULL_STR);

	/*flush ip filter  rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", IP_FILTER_NAME[0], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", IP_FILTER_NAME[1], NULL_STR);

	/*flush port filter  rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORT_FILTER_NAME[0], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", PORT_FILTER_NAME[1], NULL_STR);

	/*flush policy  rules******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", PREROUTING_CHAIN_NAME[wanIdx],"-t", "mangle",NULL_STR);


	//jump rule for user defined chain will be cleared, so please donot flush INPUT_WANX chain here!!
	//RunSystemCmd(NULL_FILE, "iptables", "-F", INPUT_CHAIN_NAME[wanIdx],NULL_STR);


	/*flush post routing rules(NAT)*****************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", POSTROUTING_CHAIN_NAME[wanIdx],"-t", "nat",NULL_STR);


//	RunSystemCmd(NULL_FILE, "iptables", "-F", FORWARD_CHAIN_NAME[wanIdx],NULL_STR);
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	/*flush igmpproxy firewall rules*/
	RunSystemCmd(NULL_FILE, "iptables", "-F", IGMP_FORWARD_NAME[wanIdx],NULL_STR);
	RunSystemCmd(NULL_FILE, "iptables", "-F", IGMP_INPUT_NAME[wanIdx],NULL_STR);
#endif
	
//	RunSystemCmd(NULL_FILE, "iptables", "-F", FORWARD_CHAIN_NAME[wanIdx],NULL_STR);	

	//flush ping and access web from wan side rules for each wan
	RunSystemCmd(NULL_FILE, "iptables", "-F", PING_INPUT_NAME[wanIdx], NULL_STR);	
	RunSystemCmd(NULL_FILE, "iptables", "-F", WEB_INPUT_NAME[wanIdx], NULL_STR);	

	#if defined(CONFIG_APP_TR069)
	/* flush tr069 rules of each wan  ******************/
	RunSystemCmd(NULL_FILE, "iptables", "-F", TR069_INPUT_NAME[wanIdx], NULL_STR);	
	#endif

	printf("%s.%d.flush firewall for each wan\n",__FUNCTION__,__LINE__);	
}

static int initFirewallChain(BASE_DATA_Tp pdata)
{
	WAN_DATA_Tp wan_p ;
	char cmdBuf[256],fileName[64];
	int wanIdx;
	wan_p = (WAN_DATA_Tp)pdata;
	wanIdx = wan_p->base.wan_idx;
	
	printf("%s.%d.init firewall chain\n",__FUNCTION__,__LINE__);
	
	/*flush all build-in chain,just first time************************/
	if(!isFileExist("/var/system/buildin_chain"))
	{
		RunSystemCmd("/var/system/buildin_chain", "echo", "1", NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables,_table, nat_table, FLUSH, POSTROUTING, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables,_table, nat_table, FLUSH, PREROUTING, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, FLUSH, _table, mangle_table, NULL_STR);		
		RunSystemCmd(NULL_FILE, Iptables, FLUSH, INPUT, NULL_STR);		
		RunSystemCmd(NULL_FILE, Iptables, FLUSH, OUTPUT, NULL_STR);	
		RunSystemCmd(NULL_FILE, Iptables, FLUSH, FORWARD, NULL_STR);
	
		RunSystemCmd(NULL_FILE, Iptables, FLUSH, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, POLICY, INPUT, DROP, NULL_STR);
		RunSystemCmd(NULL_FILE, Iptables, POLICY, FORWARD, DROP, NULL_STR);		
		RunSystemCmd(NULL_FILE, Iptables, POLICY, OUTPUT, ACCEPT, NULL_STR);
	}

	/*Add common chain for common rules **************/
	if(!isFileExist("/var/system/common_chain"))
	{
		/*create user-defined  common rule chain  ************************/
		/*create input chain *****************/
		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_HEAD, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_TAIL, NULL_STR);

		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_HEAD_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_HEAD_END, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_TAIL_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",INPUT_TAIL_END, NULL_STR);

		/*create forward chain *****************/
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_HEAD, NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_TAIL, NULL_STR);
		
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_HEAD_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_HEAD_END, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_TAIL_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",FORWARD_TAIL_END, NULL_STR);
		
		/*create output chain *****************/
		RunSystemCmd(NULL_FILE, "iptables", "-N",OUTPUT_HEAD, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N",OUTPUT_TAIL, NULL_STR);		

		/*create rule to connect user-defined chain for input chain***************/
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT,"-j",INPUT_HEAD, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT,"-j",INPUT_TAIL, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_HEAD,"-j",INPUT_HEAD_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_HEAD,"-j",INPUT_HEAD_END, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_TAIL,"-j",INPUT_TAIL_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",INPUT_TAIL,"-j",INPUT_TAIL_END, NULL_STR);

		/*create rule to connect user-defined chain for forward chain***************/
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD,"-j",FORWARD_HEAD, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD,"-j",FORWARD_TAIL, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_HEAD,"-j",FORWARD_HEAD_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_HEAD,"-j",FORWARD_HEAD_END, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_TAIL,"-j",FORWARD_TAIL_START, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_TAIL,"-j",FORWARD_TAIL_END, NULL_STR);
		
		/*create rule to connect user-defined chain for output chain***************/
		RunSystemCmd(NULL_FILE, "iptables", "-A", OUTPUT, "-j",OUTPUT_HEAD, NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A", OUTPUT, "-j",OUTPUT_TAIL, NULL_STR);		
	}	


	sprintf(fileName, "/var/system/multichain%d", wanIdx);
	if(!isFileExist(fileName))
	{
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", INPUT_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N", OUTPUT_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N", FORWARD_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N", PREROUTING_CHAIN_NAME[wanIdx], "-t", "nat", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N", POSTROUTING_CHAIN_NAME[wanIdx], "-t", "nat", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-N", PREROUTING_CHAIN_NAME[wanIdx], "-t", "mangle", NULL_STR);				

		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-I", INPUT,"2","-j", INPUT_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", OUTPUT,"2","-j", OUTPUT_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", FORWARD,"2","-j", FORWARD_CHAIN_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", PREROUTING,"-j", PREROUTING_CHAIN_NAME[wanIdx], "-t", "nat", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", POSTROUTING,"-j", POSTROUTING_CHAIN_NAME[wanIdx], "-t", "nat", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", PREROUTING,"-j", PREROUTING_CHAIN_NAME[wanIdx], "-t", "mangle", NULL_STR);
		
		RunSystemCmd("/tmp/firewall_igd", "echo", "1", NULL_STR);
	}

	sprintf(fileName, "/var/system/dmzRule%d", wanIdx);
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", DMZ_FORWARD_NAME[wanIdx], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", DMZ_PREROUTING_NAME[wanIdx],"-t","nat",NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", DMZ_POSTROUTING_NAME[wanIdx],"-t","nat",NULL_STR);
		
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_CHAIN_NAME[wanIdx],"-j",DMZ_FORWARD_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-t","nat","-A",PREROUTING_CHAIN_NAME[wanIdx],"-j",DMZ_PREROUTING_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-t","nat","-A",POSTROUTING_CHAIN_NAME[wanIdx],"-j",DMZ_POSTROUTING_NAME[wanIdx], NULL_STR);		
	}
	
	sprintf(fileName, "/var/system/portforward%d", wanIdx);
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", PORTFORWARD_FORWARD_NAME[wanIdx], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", PORTFORWARD_PREROUTING_NAME[wanIdx],"-t","nat",NULL_STR);		
		
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A",FORWARD_CHAIN_NAME[wanIdx],"-j",PORTFORWARD_FORWARD_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-t","nat","-A",PREROUTING_CHAIN_NAME[wanIdx],"-j",PORTFORWARD_PREROUTING_NAME[wanIdx], NULL_STR);
	}

	sprintf(fileName,"%s","/var/system/macfilter");
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", MAC_FILTER_NAME[0], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", MAC_FILTER_NAME[1], NULL_STR);

		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-I", INPUT,"2","-j", MAC_FILTER_NAME[0], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", FORWARD,"2","-j", MAC_FILTER_NAME[1], NULL_STR);	
	}

	sprintf(fileName,"%s","/var/system/ipfilter");
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", IP_FILTER_NAME[0], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", IP_FILTER_NAME[1], NULL_STR);

		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-I", INPUT,"2","-j", IP_FILTER_NAME[0], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", FORWARD,"2","-j", IP_FILTER_NAME[1], NULL_STR);	
	}

	sprintf(fileName,"%s","/var/system/portfilter");
	if(!isFileExist(fileName))
	{
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", PORT_FILTER_NAME[0], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", PORT_FILTER_NAME[1], NULL_STR);

		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-I", INPUT,"2","-j", PORT_FILTER_NAME[0], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-I", FORWARD,"2","-j", PORT_FILTER_NAME[1], NULL_STR);	
	}
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	sprintf(fileName, "/var/system/igmpproxyRule%d", wanIdx);
	if(!isFileExist(fileName))
	{
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", IGMP_INPUT_NAME[wanIdx], NULL_STR);	
		RunSystemCmd(NULL_FILE, "iptables", "-N", IGMP_FORWARD_NAME[wanIdx], NULL_STR);
	
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A", FORWARD_CHAIN_NAME[wanIdx],"-j",IGMP_FORWARD_NAME[wanIdx], NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A", INPUT_CHAIN_NAME[wanIdx],"-j",IGMP_INPUT_NAME[wanIdx], NULL_STR);
	}
#endif	
	/*flush user-defined chain for use.*/

	//for ping from wan side
	sprintf(fileName, "/var/system/pingRule%d", wanIdx);
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", PING_INPUT_NAME[wanIdx], NULL_STR);	
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A", INPUT_CHAIN_NAME[wanIdx], "-j", PING_INPUT_NAME[wanIdx], NULL_STR);
	}

	//for access web from wan side.
	sprintf(fileName, "/var/system/webRule%d", wanIdx);
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", WEB_INPUT_NAME[wanIdx], NULL_STR);	
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A", INPUT_CHAIN_NAME[wanIdx], "-j", WEB_INPUT_NAME[wanIdx], NULL_STR);
	}

	#if defined(CONFIG_APP_TR069)
	sprintf(fileName, "/var/system/tr069Rule%d", wanIdx);
	if(!isFileExist(fileName))
	{			
		RunSystemCmd(fileName, "echo", "1", NULL_STR);
		
		/*create user-defined chain  ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-N", TR069_INPUT_NAME[wanIdx], NULL_STR);	
		/*jump rule for user-defined chain ************************/
		RunSystemCmd(NULL_FILE, "iptables", "-A", INPUT_CHAIN_NAME[wanIdx], "-j", TR069_INPUT_NAME[wanIdx], NULL_STR);
	}
	#endif
	
	/*flush user-defined chain for use. please donot flush non-leaf chain in this function */
	flushFirewallChain(pdata);

}


void clean_firewall_rules(BASE_DATA_Tp data)
{
	WAN_DATA_T wan_info;
	unsigned char wan_iface[32];

	/*format wan private data */
	if(!format_wan_data(&wan_info,data))
		return 0;
	
	getWanIface(&wan_info,wan_iface);
	flushFirewallChain(&wan_info);	
}

int func_set_firewall_rules(BASE_DATA_Tp data)
{
	WAN_DATA_Tp wan_info;

	BASE_DATA_T base;
	base.wan_idx = data->wan_idx;
	/*format wan private data */
	if(!format_wan_data(data,&base))
		return 0;

	wan_info = (WAN_DATA_Tp)data;
	
	/*init firewall chain ************************/
	initFirewallChain(wan_info);

	/*set user-defined common rules *********/
	rtl_set_common_rules();

	/*set dmz rule*/
	rtl_set_DMZ(wan_info);
	
	/*set port forward  rule*****/
	rtl_set_port_forward(wan_info);

	/*set url filter  rule*****/
	rtl_set_url_filter();

	/*set mac filter  rule*****/
	rtl_set_mac_filter();

	/*set ip filter  rule*****/
	rtl_set_ip_filter();

	/*set port filter  rule*****/
	rtl_set_port_filter();

	/*set ping access on WAN rule*****/
	rtl_set_ping_access(wan_info);

	/*set web access from wan *****/
	rtl_set_web_access(wan_info);

	/*set policy rule for lan<-->wan binding  ****************/
#if defined(CONFIG_RTL_IP_POLICY_ROUTING_SUPPORT)
	rtl_set_policy_router_by_wan_interface(wan_info);
#endif

	/*set each static route rules ***************************/
	rtl_set_static_route(wan_info);

	/*set each wan firewall rules ***************************/
	rtl_set_wan_rules(wan_info);

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	/*set igmpproxy rules***************************/
	rtl_set_igmpproxy_rules(wan_info);
#endif
	
	/*set nat rule for nat mode ***************************/
	rtl_set_nat_rules(wan_info);
	
	/*set redirect acl rule and port_mapping for nat mode ***************************/
	rtl_set_redirect_acl_rules_and_port_mapping(wan_info);
	
#ifdef CONFIG_RTL_HW_NAPT
	update_hwnat_setting();
#endif

#if defined(CONFIG_APP_TR069)
	/*set tr069 related firewall rules ***************************/
	rtl_set_tr069_firewall_rule(wan_info);
#endif

	/*enable ipv4 forward ********************************/
	system("echo 1 > /proc/sys/net/ipv4/ip_forward");
	printf("%s.%d. set firewall done!!!\n",__FUNCTION__,__LINE__);	
}

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
int func_set_firewall_rules_for_igmpproxy(BASE_DATA_Tp data)
{
	WAN_DATA_Tp wan_info;

	BASE_DATA_T base;
	base.wan_idx = data->wan_idx;
	/*format wan private data */
	if(!format_wan_data(data,&base))
		return 0;

	wan_info = (WAN_DATA_Tp)data;
	
	/*init firewall chain ************************/
	initFirewallChain(wan_info);

	rtl_set_common_rules();

	rtl_set_igmpproxy_rules(wan_info);

	rtl_set_redirect_acl_rules_and_port_mapping(wan_info);
}
#endif
