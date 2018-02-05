#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
#include <signal.h>
#endif

#include "apmib.h"
#include "reinit_utility.h"
#include "reinitFunc_wan.h"
#include "reinitHandle.h"

#define RESOLV_CONF "/var/resolv.conf"
#define PROC_FASTPPPOE_FILE "/proc/fast_pppoe"
#define PPTP_PEERS_FILE "/etc/ppp/peers/rpptp"
#define L2TPCONF "/etc/ppp/l2tpd.conf"

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
#define PROC_BR_IGMPPROXY "/proc/br_igmpProxy"
#define PROC_BR_IGMPSNOOP "/proc/br_igmpsnoop"
#define PROC_BR_IGMPVERSION "/proc/br_igmpVersion"
#define PROC_BR_IGMPQUERY "/proc/br_igmpquery"
#define PROC_BR_MCASTFASTFWD "/proc/br_mCastFastFwd"
#define PROC_BR_MLDSNOOP "/proc/br_mldsnoop"
#define PROC_BR_MLDQUERY "/proc/br_mldquery"
#define PROC_BR_IGMPDB "/proc/br_igmpDb"
#define PROC_IGMP_MAX_MEMBERS "/proc/sys/net/ipv4/igmp_max_memberships"

#define IGMPPROXY_PID_FILE "var/run/igmp_pid"
#endif

#define RM(file)	if(stat(file, &status)==0)	\
						unlink(file);	\


int clean_wan_app_ntp()
{	
	RunSystemCmd(NULL_FILE, "rm", NTPTMP_FILE,"2>/dev/null", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "ntp_inet", "2>/dev/null", NULL_STR);
	return 0;
}
int start_wan_app_ntp()
{
	unsigned int ntp_onoff=0;
	unsigned char buffer[16]={0};

	unsigned int ntp_server_id;
	char	ntp_server[16]={0};

	
	apmib_get(MIB_NTP_ENABLED, (void *)&ntp_onoff);
	
	if(ntp_onoff == 1)
	{
		RunSystemCmd(NULL_FILE, "echo", "Start NTP daemon", NULL_STR);
		/* prepare requested info for ntp daemon */
		apmib_get( MIB_NTP_SERVER_ID,  (void *)&ntp_server_id);

		if(ntp_server_id == 0)
			apmib_get( MIB_NTP_SERVER_IP1,  (void *)buffer);
		else if(ntp_server_id == 1)
			apmib_get( MIB_NTP_SERVER_IP2,  (void *)buffer);
		else if(ntp_server_id == 2)
			apmib_get( MIB_NTP_SERVER_IP3,  (void *)buffer);
		else if(ntp_server_id == 3)
			apmib_get( MIB_NTP_SERVER_IP4,  (void *)buffer);
		else if(ntp_server_id == 4)
			apmib_get( MIB_NTP_SERVER_IP5,  (void *)buffer);
		else if(ntp_server_id == 5)
			apmib_get( MIB_NTP_SERVER_IP6,  (void *)buffer);

		sprintf(ntp_server, "%s", inet_ntoa(*((struct in_addr *)buffer)));
		RunSystemCmd(NULL_FILE, "ntp_inet", "-x", ntp_server,NULL_STR);
	}
	return 0;
}
int reinit_func_wan_app_ntp(BASE_DATA_Tp data)
{
	clean_wan_app_ntp();
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	start_wan_app_ntp();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}
#ifdef DOS_SUPPORT
int clean_wan_dos()
{
	
}
int start_wan_dos()
{
	char cmdBuffer[512];
	unsigned long dos_enabled=0;
	unsigned int *dst, *mask;
	unsigned int synsynflood=0;
	unsigned int sysfinflood=0;
	unsigned int sysudpflood=0;
	unsigned int sysicmpflood=0;
	unsigned int pipsynflood=0;
	unsigned int pipfinflood=0;
	unsigned int pipudpflood=0;
	unsigned int pipicmpflood=0;
	unsigned int blockTime=0;
	struct in_addr curIpAddr={0}, curSubnet={0};
	char br_interface_name[16]={0};
	apmib_get(MIB_DOS_ENABLED, (void *)&dos_enabled);
	apmib_get(MIB_DOS_SYSSYN_FLOOD, (void *)&synsynflood);
	apmib_get(MIB_DOS_SYSFIN_FLOOD, (void *)&sysfinflood);
	apmib_get(MIB_DOS_SYSUDP_FLOOD, (void *)&sysudpflood);
	apmib_get(MIB_DOS_SYSICMP_FLOOD, (void *)&sysicmpflood);
	apmib_get(MIB_DOS_PIPSYN_FLOOD, (void *)&pipsynflood);
	apmib_get(MIB_DOS_PIPFIN_FLOOD, (void *)&pipfinflood);
	apmib_get(MIB_DOS_PIPUDP_FLOOD, (void *)&pipudpflood);
	apmib_get(MIB_DOS_PIPICMP_FLOOD, (void *)&pipicmpflood);
	apmib_get(MIB_DOS_BLOCK_TIME, (void *)&blockTime);

	get_br_interface_name(br_interface_name);
	getInAddr(br_interface_name, IP_ADDR_T, (void *)&curIpAddr);
    getInAddr(br_interface_name, NET_MASK_T, (void *)&curSubnet);
  	//apmib_get(MIB_IP_ADDR,  (void *)ipbuf);
  	dst = (unsigned int *)&curIpAddr;
  	//apmib_get( MIB_SUBNET_MASK,  (void *)maskbuf);
  	mask = (unsigned int *)&curSubnet;
  	
	sprintf(cmdBuffer, "echo \" 0 %X %X %ld %d %d %d %d %d %d %d %d %d\" >  /proc/enable_dos", (*dst & 0xFFFFFF00), *mask, dos_enabled, synsynflood, sysfinflood, sysudpflood, sysicmpflood, pipsynflood, pipfinflood, pipudpflood, pipicmpflood, blockTime);
	//printf("%s:%d cmdBuffer=%s \n",__FUNCTION__,__LINE__,cmdBuffer);
	system(cmdBuffer);
	
return 0;
}
int reinit_func_wan_dos(BASE_DATA_Tp data)
{
	clean_wan_dos();
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	start_wan_dos();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}
#endif
/**************************WAN  ***********************************/

int clean_wan_interface(BASE_DATA_Tp data)
{	
	FILE * fp;
	unsigned char stop_file[32];
	unsigned char del_cmd[64];
	unsigned char command[64];

	/*down wan interface *********************/
	sprintf(command,"ifconfig eth1.%d down",data->wan_idx);
	system(command);

	/*del wan interface *********************/
	sprintf(stop_file,"/var/stop_wan_%d",data->wan_idx);
	if((fp = fopen(stop_file,"r+"))!=NULL)
	{	
		fgets(del_cmd,sizeof(del_cmd),fp);
		fclose(fp);
		system(del_cmd);		
		printf("cmd(%s)\n",del_cmd);		
	}
	return 1;
}

		

void clean_wan_config_file(BASE_DATA_Tp data)
{
	unsigned char link_file[32],chap_file[32],pap_file[32],option_file[32];
	unsigned char del_wan[32],clean_wan[32];	
	struct stat status;

	sprintf(chap_file,"/etc/ppp/chap-secrets%d",data->wan_idx);	
	sprintf(pap_file,"/etc/ppp/pap-secrets%d",data->wan_idx);	
	sprintf(option_file,"etc/ppp/options%d",data->wan_idx);	
	sprintf(link_file,"/etc/ppp/link%d",data->wan_idx);
	sprintf(del_wan,"/var/stop_wan_%d",data->wan_idx);
	sprintf(clean_wan,"/var/run/wan_%d.pid",data->wan_idx);

	/*remove this wan last configure **************/
	RM(chap_file);
	RM(pap_file);
	RM(option_file);
	RM(link_file);
	RM(del_wan);
	RM(clean_wan);
	
}

int create_wan_interface(BASE_DATA_Tp data)
{
	unsigned char all_command[128];
	unsigned char start_command[64],del_command[64];
	unsigned char start_file[32],stop_file[32];
	unsigned char vlan_buff[16];
	WANIFACE_T WanIfaceEntry;
#ifdef CONFIG_ISP_IGMPPROXY_MULTIWAN
	unsigned char igmpproxy_buff[16];
#endif
	
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	
	if(!getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return FALSE;
	}

	//vconfig addsmux pppoe eth1 eth1.1 napt vlan 100
	//vconfig remsmux ipoe eth1 eth1.1
	printf("%s.%d.AddressType(%d)\n",__FUNCTION__,__LINE__,WanIfaceEntry.AddressType);
	switch(WanIfaceEntry.AddressType)
	{
		case DHCP_DISABLED:
		case DHCP_CLIENT:
			sprintf(start_command,"vconfig addsmux ipoe eth1 eth1.%d napt",
				wan_p->base.wan_idx);		
			sprintf(del_command,"vconfig remsmux ipoe eth1 eth1.%d napt",
				wan_p->base.wan_idx);	
			break;
		case BRIDGE:
			sprintf(start_command,"vconfig addsmux bridge eth1 eth1.%d",
				wan_p->base.wan_idx);		
			sprintf(del_command,"vconfig remsmux bridge eth1 eth1.%d",
				wan_p->base.wan_idx);
			break;
		case PPPOE:
#if defined(SINGLE_WAN_SUPPORT)			
		case L2TP:
		case PPTP:
#endif
			sprintf(start_command,"vconfig addsmux pppoe eth1 eth1.%d napt",
				wan_p->base.wan_idx);
			sprintf(del_command,"vconfig remsmux pppoe eth1 eth1.%d napt",
				wan_p->base.wan_idx);				
			break;
		default:			
			printf("Error wan type!!!\n");
			break;
	}
	
	memset(vlan_buff,0,sizeof(vlan_buff));

	/*enable vlan setting */
	if(WanIfaceEntry.vlan)	
	{
		sprintf(vlan_buff," vlan %d",WanIfaceEntry.vlanid);
	}
	
	strcat(start_command,vlan_buff);

#ifdef CONFIG_ISP_IGMPPROXY_MULTIWAN
	memset(igmpproxy_buff, 0, sizeof(igmpproxy_buff));
	sprintf(igmpproxy_buff, " igmpproxy %d %d", WanIfaceEntry.enableIGMP, WanIfaceEntry.onPhyIf);
	strcat(start_command, igmpproxy_buff);
	//printf("%s.%d.start command(%s)#########\n",__FUNCTION__,__LINE__,start_command);
#endif
	system(start_command);

	printf("%s.%d.start command(%s)#########\n",__FUNCTION__,__LINE__,start_command);
	
	sprintf(stop_file,"/var/stop_wan_%d",wan_p->base.wan_idx);

	/*save stop wan command to file */
	sprintf(all_command,"echo %s > %s",del_command,stop_file);
	printf("%s.%d. command(%s)\n",__FUNCTION__,__LINE__,all_command);
	system(all_command);
	
	//sprintf(wan_p->wan_data.wan_iface,"eth1.%d",wan_p->base.wan_idx);
	/*save  wan interface to file */
	
}

int set_wan_interface(BASE_DATA_Tp data)
{	
	
	unsigned char command[64],cmdBuffer[100], tmpBuff[32], wlan_root_if[16];
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	memset(cmdBuffer,0,sizeof(cmdBuffer));
	
	if(!getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return 0;
	}

	/*clone mac address *********************/
	memcpy((void *)tmpBuff,WanIfaceEntry.wanMacAddr,6);
	sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", 
			(unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1],
			(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], 
			(unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);
	
	/*default hw mac address *********************/
	if(!strcmp(cmdBuffer,"000000000000"))
	{
		memset(tmpBuff,0,sizeof(tmpBuff));
		apmib_get(MIB_HW_NIC1_ADDR,  (void *)tmpBuff);
		sprintf(cmdBuffer,"%02x%02x%02x%02x%02x%02x", 
			(unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1],
			(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], 
			(unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);
	}
	
//	printf("start set interface(%s) mac(%s)\n",
//				wan_p->wan_data.wan_iface,cmdBuffer);

	/*interface down*/
	sprintf(command,"ifconfig %s down",wan_p->wan_data.wan_iface);
	system(command);

	/*is WISP mode */
	if(strncmp(wan_p->wan_data.wan_iface,"wlan",sizeof("wlan"))==0)
	{
		memset(wlan_root_if,0,sizeof(wlan_root_if));
		strncpy(wlan_root_if,wan_p->wan_data.wan_iface,sizeof("wlanX"));
		sprintf(command,"ifconfig %s down",wlan_root_if);
		system(command);
	}
	
	/*set wan mac address */
	#if 0
	sprintf(command,"ifconfig eth1.%d hw ether 00:E0:4C:81:96:C9",wan_p->base.wan_idx);
	system(command);
	#else
	if(strcmp(cmdBuffer,"000000000000"))
	{
		RunSystemCmd(NULL_FILE, "ifconfig", wan_p->wan_data.wan_iface, "hw", "ether", cmdBuffer, NULL_STR);
	}
	#endif
	/*interface up*/
	sprintf(command,"ifconfig %s up",wan_p->wan_data.wan_iface);
	system(command);

	if(strncmp(wan_p->wan_data.wan_iface,"wlan",sizeof("wlan"))==0)
	{
		sprintf(command,"ifconfig %s up",wlan_root_if);
		system(command);
	}
}
static void set_staticIP(BASE_DATA_Tp data)
{
	int intValue=0;
	unsigned char wan_iface[32];
	char tmpBuff[100];
	char tmp_args[16];
	char Ip[32], Mask[32], Gateway[32];
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_T	wan_info;
	
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry);
	strcpy(wan_iface,wan_p->wan_data.wan_iface);

	sprintf(Ip, "%s", inet_ntoa(*((struct in_addr *)(WanIfaceEntry.ipAddr))));
	sprintf(Mask, "%s", inet_ntoa(*((struct in_addr *)(WanIfaceEntry.netMask))));
	strcpy(tmpBuff,WanIfaceEntry.remoteIpAddr);
	if (!memcmp(tmpBuff, "\x0\x0\x0\x0", 4))
		memset(Gateway, 0x00, 32);
	else
		sprintf(Gateway, "%s", inet_ntoa(*((struct in_addr *)tmpBuff)));	

	RunSystemCmd(NULL_FILE, "ifconfig", wan_iface, Ip, "netmask", Mask, NULL_STR);
	intValue = WanIfaceEntry.staticIpMtu;
	sprintf(tmp_args, "%d", intValue);
	RunSystemCmd(NULL_FILE, "ifconfig", wan_iface, "mtu", tmp_args, NULL_STR);

	wan_info.base.wan_idx = wan_p->base.wan_idx;
	strcpy(wan_info.wan_data.ip_addr,Ip);	
	strcpy(wan_info.wan_data.sub_net,Mask);
	strcpy(wan_info.wan_data.gw_addr,Gateway);
	
	func_connect_wan(&wan_info);
	func_set_firewall_rules(&wan_info);
}


void set_dhcp_client(BASE_DATA_Tp data)
{
	char hostname[257];
	char cmdBuff[200];
	char script_file[100], deconfig_script[100], pid_file[100];
	unsigned char wan_iface[32];
	int intValue=0;
	char tmp_args[16];
	WANIFACE_T WanIfaceEntry;
	
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry);
	
	strcpy(wan_iface,wan_p->wan_data.wan_iface);
	intValue = WanIfaceEntry.dhcpMtu;
	sprintf(tmp_args, "%d", intValue);	
	
	RunSystemCmd(NULL_FILE, "ifconfig", wan_iface, "mtu", tmp_args, NULL_STR);
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", wan_iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", wan_iface);/*deconfig script path*/
	//sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", wan_iface); /*pid path*/
	sprintf(pid_file, "/var/run/wan_%d.pid", wan_p->base.wan_idx);
	Create_script(deconfig_script, wan_iface, WAN_NETWORK, 0, 0, 0);

	memset(hostname, 0x00, 100);
	if(WanIfaceEntry.dhcpHostName[0])
		strcpy(hostname, WanIfaceEntry.dhcpHostName);
	else
		hostname[0]='\0';

	if(hostname[0]){
		sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -h %s -a 30 &", wan_iface, pid_file, script_file, hostname);
	}else{
		sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -a 30 &", wan_iface, pid_file, script_file);
	}
	system(cmdBuff);
}


void set_pppoe(BASE_DATA_Tp data)
{
	unsigned char wan_iface[32];
	int intValue=0, cmdRet=-1;
	char line_buffer[100]={0};
	char tmp_args[64]={0};
	char tmp_args1[32]={0};
	int  connect_type=0, idle_time=0;
	char wanIndexStr[5];
	int  wan_index;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp wan_p;
	char* pppoe_file_list[4][3]={
		{"/etc/ppp/pap-secrets1","/etc/ppp/chap-secrets1","/etc/ppp/options1"},
		{"/etc/ppp/pap-secrets2","/etc/ppp/chap-secrets2","/etc/ppp/options2"},
		{"/etc/ppp/pap-secrets3","/etc/ppp/chap-secrets3","/etc/ppp/options3"},
		{"/etc/ppp/pap-secrets4","/etc/ppp/chap-secrets4","/etc/ppp/options4"}};

	wan_p = (WAN_DATA_Tp)data;
	wan_index = wan_p->base.wan_idx;
	getWanIfaceEntry(wan_index,&WanIfaceEntry);
	
	sprintf(wanIndexStr,"%d",wan_index);
	strcpy(wan_iface,wan_p->wan_data.wan_iface);

	wan_index = wan_index - 1;
	RunSystemCmd(NULL_FILE, "ifconfig",wan_iface, "0.0.0.0", NULL_STR);
	cmdRet = RunSystemCmd(NULL_FILE, "flash", "gen-pppoe",
				pppoe_file_list[wan_index][2], 
				pppoe_file_list[wan_index][0],
				pppoe_file_list[wan_index][1], 
				wanIndexStr, NULL_STR);
	
	if(cmdRet==0){
		sprintf(line_buffer,"%s\n", "noauth");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "nomppc");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "noipdefault");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "hide-password");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "defaultroute");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "persist");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "ipcp-accept-remote");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "ipcp-accept-local");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "nodetach");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "usepeerdns");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		
		intValue = WanIfaceEntry.pppoeMtu;
		sprintf(line_buffer,"mtu %d\n", intValue);
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"mru %d\n", intValue);
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "lcp-echo-interval 20");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "lcp-echo-failure 3");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "wantype 3");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"%s\n", "holdoff 10");
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
		sprintf(line_buffer,"unit %d\n",wan_index+1);
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);

		strcpy(tmp_args,WanIfaceEntry.pppServiceName);
		if(tmp_args[0]){		
			sprintf(line_buffer,"plugin /etc/ppp/plubins/libplugin.a rp_pppoe_service %s %s\n",tmp_args, wan_iface);
		}else{
			sprintf(line_buffer,"plugin /etc/ppp/plubins/libplugin.a %s\n", wan_iface);
		}
		write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);		
	}
	
	connect_type = WanIfaceEntry.pppCtype;
	switch(connect_type)
	{
		case CONNECT_ON_DEMAND:
			//apmib_get(MIB_PPP_IDLE_TIME, (void *)&idle_time);
			idle_time = WanIfaceEntry.pppIdleTime * 60;
			sprintf(line_buffer,"%s\n", "demand");
			write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);
			sprintf(line_buffer,"idle %d\n", idle_time);
			write_line_to_file(pppoe_file_list[wan_index][2],2, line_buffer);			
			break;
		case MANUAL:
			return;
		case CONTINUOUS:
			break;				
		default:
			break;
	}
	cmdRet = IsExistProcess("ppp_controler");
	printf("%s.%d.cmdRet(%d)\n",__FUNCTION__,__LINE__,cmdRet);
	if(cmdRet == 0)
	{
		RunSystemCmd(NULL_FILE, "ppp_controler", NULL_STR);	
	}
	else if(cmdRet == 1)
	{
		printf("ppp_inet already exist\n");
	}
	else
	{
		printf("get ppp_inet status fail\n");
	}
}
#if defined(SINGLE_WAN_SUPPORT)
void set_L2TP(BASE_DATA_Tp data)
{
#define PPP_OPTIONS_FILE1 "/etc/ppp/options1"
#define PPP_PAP_FILE1 	  "/etc/ppp/pap-secrets1"
#define PPP_CHAP_FILE1 	  "/etc/ppp/chap-secrets1"
	char Ip[32], Mask[32], ServerIp[32];
	char *strtmp=NULL;
	unsigned char wan_iface[32];
	int intValue=0, cmdRet=-1;
	char line_buffer[100]={0};
	char tmp_args[64]={0};
	char tmp_args1[32]={0};
	int  connect_type=0, idle_time=0;
	char wanIndexStr[5];
	int  wan_index;
	int pwd_len;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp wan_p;

	wan_p = (WAN_DATA_Tp)data;
	wan_index = wan_p->base.wan_idx;
	getWanIfaceEntry(wan_index,&WanIfaceEntry);
	
	sprintf(wanIndexStr,"%d",wan_index);
	strcpy(wan_iface,wan_p->wan_data.wan_iface);

#if 1	
	//	apmib_get(MIB_PPTP_SERVER_IP_ADDR,	(void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.l2tpServerIpAddr);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(ServerIp, "%s", strtmp);	
	
	//apmib_get(MIB_PPTP_IP_ADDR,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.l2tpIpAddr);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(Ip, "%s", strtmp);	

	//apmib_get(MIB_PPTP_SUBNET_MASK,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.l2tpSubnetMask);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(Mask, "%s", strtmp);

	RunSystemCmd(NULL_FILE, "ifconfig", wan_iface, Ip, "netmask", Mask, NULL_STR);
	RunSystemCmd(NULL_FILE, "route", "del", "default", "gw", "0.0.0.0", NULL_STR);
		
	strcpy(tmp_args,WanIfaceEntry.l2tpUserName);	
	strcpy(tmp_args1,WanIfaceEntry.l2tpPassword);
	//apmib_get( MIB_L2TP_USER_NAME,  (void *)tmp_args);
	//apmib_get( MIB_L2TP_PASSWORD,  (void *)tmp_args1);
	pwd_len = strlen(tmp_args1);
	/*options file*/
	sprintf(line_buffer,"user \"%s\"\n",tmp_args);
	write_line_to_file(PPP_OPTIONS_FILE1, 1, line_buffer);
	
	/*secrets files*/
	sprintf(line_buffer,"%s\n","#################################################");
	write_line_to_file(PPP_PAP_FILE1, 1, line_buffer);
	
	sprintf(line_buffer, "\"%s\"	*	\"%s\"\n",tmp_args, tmp_args1);
	write_line_to_file(PPP_PAP_FILE1, 2, line_buffer);
	
	sprintf(line_buffer,"%s\n","#################################################");
	write_line_to_file(PPP_CHAP_FILE1, 1, line_buffer);
	
	sprintf(line_buffer, "\"%s\"	*	\"%s\"\n",tmp_args, tmp_args1);
	write_line_to_file(PPP_CHAP_FILE1, 2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "lock");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	sprintf(line_buffer,"%s\n", "noauth");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	sprintf(line_buffer,"%s\n", "defaultroute");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	sprintf(line_buffer,"%s\n", "usepeerdns");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	sprintf(line_buffer,"%s\n", "lcp-echo-interval 0");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	sprintf(line_buffer,"%s\n", "wantype 6");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);

	sprintf(line_buffer,"unit %d\n",wan_index);
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);

	intValue = WanIfaceEntry.l2tpMtuSize;
	//apmib_get(MIB_L2TP_MTU_SIZE, (void *)&intValue);
	sprintf(line_buffer,"mtu %d\n", intValue);
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);

	
	strcpy(tmp_args,WanIfaceEntry.l2tpUserName);	
	//apmib_get( MIB_L2TP_USER_NAME,  (void *)tmp_args);
	sprintf(line_buffer,"name %s\n", tmp_args);
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
//	sprintf(line_buffer,"%s\n", "noauth");
//	write_line_to_file(PPP_OPTIONS_FILE,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "nodeflate");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "nobsdcomp");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "nodetach");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "novj");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "default-asyncmap");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "nopcomp");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "noaccomp");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "noccp");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "novj");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "refuse-eap");
	write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	
	if(pwd_len > 35){
		sprintf(line_buffer,"%s\n", "-mschap");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "-mschap-v2");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
	}
	
	sprintf(line_buffer,"%s\n", "[global]");
	write_line_to_file(L2TPCONF,1, line_buffer);
	
	sprintf(line_buffer,"%s\n", "port = 1701");
	write_line_to_file(L2TPCONF,2, line_buffer);
	
	sprintf(line_buffer,"auth file = %s\n", PPP_CHAP_FILE1);
	write_line_to_file(L2TPCONF,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "[lac client]");
	write_line_to_file(L2TPCONF,2, line_buffer);
	
	sprintf(line_buffer,"lns=%s\n", ServerIp);
	write_line_to_file(L2TPCONF,2, line_buffer);

	sprintf(line_buffer,"%s\n", "require chap = yes");
	write_line_to_file(L2TPCONF,2, line_buffer);

	
	//apmib_get( MIB_L2TP_USER_NAME,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.l2tpUserName);	
	sprintf(line_buffer,"name = %s\n", tmp_args);
	write_line_to_file(L2TPCONF,2, line_buffer);
	
	sprintf(line_buffer,"%s\n", "pppoptfile = /etc/ppp/options1");
	write_line_to_file(L2TPCONF, 2, line_buffer);

	printf("%s.%d. kill l2tpd#############\n",__FUNCTION__,__LINE__);
	RunSystemCmd(NULL_FILE, "kill","-9", "l2tpd", NULL_STR);
//	RunSystemCmd(NULL_FILE, "killall", "ppp_inet", NULL_STR);
	sleep(2);
	//RunSystemCmd(NULL_FILE, "l2tpd", NULL_STR);	
	system("l2tpd&");
	sleep(3);
	
	//apmib_get(MIB_L2TP_CONNECTION_TYPE, (void *)&connect_type);
	
	connect_type = WanIfaceEntry.l2tpConnectType;

	switch(connect_type)
	{
		case CONNECT_ON_DEMAND:
			sprintf(line_buffer,"%s\n", "connect /etc/ppp/true");
			write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
			sprintf(line_buffer,"%s\n", "demand");
			write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
			idle_time = WanIfaceEntry.l2tpIdleTime * 60;
			sprintf(line_buffer,"idle %d\n", idle_time);
			write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);					
			break;
		case MANUAL:
			return;
		case CONTINUOUS:
			break;				
		default:
			break;		
		
	}
#endif

	cmdRet = IsExistProcess("ppp_controler");
	if(cmdRet == 0)
	{
		RunSystemCmd(NULL_FILE, "ppp_controler", NULL_STR);	
	}
	else if(cmdRet == 1)
	{
		printf("ppp_inet already exist\n");
	}
	else
	{
		printf("get ppp_inet status fail\n");
	}

}
void set_PPTP(BASE_DATA_Tp data)
{
#define PPP_OPTIONS_FILE1 "/etc/ppp/options1"
#define PPP_PAP_FILE1 	  "/etc/ppp/pap-secrets1"
#define PPP_CHAP_FILE1 	  "/etc/ppp/chap-secrets1"


	unsigned char wan_iface[32];
	int intValue=0, intValue1=0, cmdRet=-1;
	char Ip[32], Mask[32], ServerIp[32];
	char line_buffer[100]={0};
	char tmp_args[64]={0};
	char tmp_args1[32]={0};	
	char *strtmp=NULL;
	int  connect_type=0, idle_time=0;
	char wanIndexStr[5];
	int  wan_index;
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp wan_p;

	wan_p = (WAN_DATA_Tp)data;
	wan_index = wan_p->base.wan_idx;
	getWanIfaceEntry(wan_index,&WanIfaceEntry);
	
	sprintf(wanIndexStr,"%d",wan_index);
	strcpy(wan_iface,wan_p->wan_data.wan_iface);

#if 1

//	apmib_get(MIB_PPTP_SERVER_IP_ADDR,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.pptpServerIpAddr);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(ServerIp, "%s", strtmp);	
	
	//apmib_get(MIB_PPTP_IP_ADDR,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.pptpIpAddr);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(Ip, "%s", strtmp);	

	//apmib_get(MIB_PPTP_SUBNET_MASK,  (void *)tmp_args);
	strcpy(tmp_args,WanIfaceEntry.pptpSubnetMask);
	strtmp= inet_ntoa(*((struct in_addr *)tmp_args));
	sprintf(Mask, "%s", strtmp);
	
	RunSystemCmd(NULL_FILE, "ifconfig", wan_iface, Ip, "netmask", Mask, NULL_STR);
	RunSystemCmd(NULL_FILE, "route", "del", "default", "gw", "0.0.0.0", NULL_STR);
				
	cmdRet = RunSystemCmd(NULL_FILE, "flash", "gen-pptp", 
								PPP_OPTIONS_FILE1, 
								PPP_PAP_FILE1, 
								PPP_CHAP_FILE1,
								wanIndexStr,NULL_STR);
	
	if(cmdRet==0)
	{
		sprintf(line_buffer,"%s\n", "lock");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "noauth");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "nobsdcomp");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "nodeflate");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "usepeerdns");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);

		sprintf(line_buffer,"%s\n", "lcp-echo-interval 20");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "lcp-echo-failure 3");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "wantype 4");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);


		sprintf(line_buffer,"unit %d\n",wan_index);
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		//apmib_get(MIB_PPTP_MTU_SIZE, (void *)&intValue);
		
		intValue = WanIfaceEntry.pptpMtuSize;		 
		sprintf(line_buffer,"mtu %d\n", intValue);
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "holdoff 2");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "refuse-eap");
		write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "remotename PPTP");
		write_line_to_file(PPTP_PEERS_FILE,1, line_buffer);
		
		sprintf(line_buffer,"%s\n", "linkname PPTP");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "ipparam PPTP");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(tmp_args, "pty \"pptp %s --nolaunchpppd\"", ServerIp);
		sprintf(line_buffer,"%s\n", tmp_args);
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		//apmib_get( MIB_PPTP_USER_NAME,  (void *)tmp_args);
		strcpy(tmp_args,WanIfaceEntry.pptpUserName);
		sprintf(line_buffer,"name %s\n", tmp_args);
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);

		#if 1
		//apmib_get( MIB_PPTP_SECURITY_ENABLED, (void *)&intValue);
		intValue = WanIfaceEntry.pptpSecurityEnabled;
		if(intValue==1)
		{
			sprintf(line_buffer,"%s\n", "+mppe required,stateless");
			write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
			
			//sprintf(line_buffer,"%s\n", "+mppe no128,stateless");/*disable 128bit encrypt*/
			//write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
			//sprintf(line_buffer,"%s\n", "+mppe no56,stateless");/*disable 56bit encrypt*/
			//write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
			
		}
		
		intValue1 = WanIfaceEntry.pptpMppcEnabled;
		//apmib_get( MIB_PPTP_MPPC_ENABLED, (void *)&intValue1);
		if(intValue1==1){
			sprintf(line_buffer,"%s\n", "mppc");
			write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
			sprintf(line_buffer,"%s\n", "stateless");
			write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		}else{
			sprintf(line_buffer,"%s\n", "nomppc");
			write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		}
		#endif
		if(intValue ==0 && intValue1==0){
			sprintf(line_buffer,"%s\n", "noccp");
			write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		}
		
		sprintf(line_buffer,"%s\n", "persist");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "noauth");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "file /etc/ppp/options1");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "nobsdcomp");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "nodetach");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		sprintf(line_buffer,"%s\n", "novj");
		write_line_to_file(PPTP_PEERS_FILE,2, line_buffer);
		
		
		//apmib_get(MIB_PPTP_CONNECTION_TYPE, (void *)&connect_type);
		connect_type = WanIfaceEntry.pptpConnectType;

		switch(connect_type)
		{
			case CONNECT_ON_DEMAND:
				RunSystemCmd(NULL_FILE, "route", "del", "default", NULL_STR);
				RunSystemCmd(NULL_FILE, "route", "add", "default", "gw", "10.112.112.112", wan_iface, NULL_STR);
				
				sprintf(line_buffer,"%s\n", "persist");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "nodetach");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "connect /etc/ppp/true");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "demand");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				//apmib_get(MIB_PPTP_IDLE_TIME, (void *)&idle_time);
				idle_time = WanIfaceEntry.pptpIdleTime * 60;
				sprintf(line_buffer,"idle %d\n", idle_time);
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "ktune");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "ipcp-accept-remote");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "ipcp-accept-local");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "noipdefault");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "hide-password");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				
				sprintf(line_buffer,"%s\n", "defaultroute");
				write_line_to_file(PPP_OPTIONS_FILE1,2, line_buffer);
				break;
			case MANUAL:
				return;
			case CONTINUOUS:
				break;				
			default:
				break;
		}
	}
#endif
	cmdRet = IsExistProcess("ppp_controler");
	printf("%s.%d.cmdRet(%d)\n",__FUNCTION__,__LINE__,cmdRet);
	if(cmdRet == 0)
	{
		RunSystemCmd(NULL_FILE, "ppp_controler", NULL_STR);	
	}
	else if(cmdRet == 1)
	{
		printf("ppp_inet already exist\n");
	}
	else
	{
		printf("get ppp_inet status fail\n");
	}
}
#endif

#if 0
int clean_origWanBindLanPort(WanIntfacesType wanIfIdx,char * lanPortIfName)
{
	BASE_DATA_T data={0};
	int wanType=0;
	char cmdBuff[COMMAND_BUFF_LEN]={0};
	reinitSer_printf(LOG_DEBUG,"%s:%d wanIfIdx=%d lanPortIfName=%s\n",__FUNCTION__,__LINE__,wanIfIdx,lanPortIfName);

	if(wanIfIdx==NO_WAN)
	{
		sprintf(cmdBuff,"brctl delif br0 %s",lanPortIfName);
		system(cmdBuff);
		reinitSer_printf(LOG_DEBUG,"%s:%d cmdBuff=%s\n",__FUNCTION__,__LINE__,cmdBuff);
		return 0;
	}
	
	data.wan_idx=wanIfIdx;

	if(reinitFunc_get_mib_orig_value(MIB_WANIFACE_ADDRESSTYPE,(void*)&wanType,&data)<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
		return -1;
	}
	if(wanType==BRIDGE)
	{
		sprintf(cmdBuff,"brctl delif "MULTI_BRNAME_FORMAT" %s",wanIfIdx,lanPortIfName);
		system(cmdBuff);
		reinitSer_printf(LOG_DEBUG,"%s:%d cmdBuff=%s\n",__FUNCTION__,__LINE__,cmdBuff);
	}else
	{
		sprintf(cmdBuff,"brctl delif "LAN_BRNAME_FORMAT" %s",wanIfIdx,lanPortIfName);
		system(cmdBuff);
		reinitSer_printf(LOG_DEBUG,"%s:%d cmdBuff=%s\n",__FUNCTION__,__LINE__,cmdBuff);
	}
}
#endif
int set_wan_bridge(BASE_DATA_Tp data)
{//only set wan iface bridge

#if 0
	char multiwanBrifName[IFACE_NUM_MAX]={0};
	char wanIfaceName[IFACE_NUM_MAX]={0};

	sprintf(multiwanBrifName,MULTI_BRNAME_FORMAT,data->wan_idx);
	sprintf(wanIfaceName,WAN_IFNAME_FORMAT,data->wan_idx);
	RunSystemCmd(NULL_FILE,"brctl","addif",multiwanBrifName,wanIfaceName,NULL_STR);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d brctl addif %s %s\n",__FUNCTION__,__LINE__,multiwanBrifName,wanIfaceName);
#else
	int i=0;
	char cmdBuff[COMMAND_BUFF_LEN]={0};
	char ifName[IFACE_NAME_MAX]={0};
	char wanBrifName[IFACE_NAME_MAX]={0};
	char wanIfaceName[IFACE_NAME_MAX]={0};
	int bind_lan_port_mask = 0;
	char bind_lan_port_mask_str[16]={0};
	//WanIntfacesType wanBindingLanPorts_orig[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};
	WanIntfacesType wanBindingLanPorts[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};

	#if defined(CONFIG_ISP_IGMPPROXY_MULTIWAN) 
	int intValue =0;
	#endif
	
	if(!apmib_rtkReinit_get(MIB_WANIFACE_BINDING_LAN_PORTS, (void *)&wanBindingLanPorts,data->wlan_idx,data->vwlan_idx,data->wan_idx))
	{
		reinitSer_printf(LOG_ERR,"%s:%d get MIB_WANIFACE_BINDING_LAN_PORTS fail!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	/*if(reinitFunc_get_mib_orig_value(MIB_WANIFACE_BINDING_LAN_PORTS,(void*)&wanBindingLanPorts_orig,data)<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
		return -1;
	}*/
	
	sprintf(wanBrifName,MULTI_BRNAME_FORMAT,data->wan_idx);
	sprintf(wanIfaceName,WAN_IFNAME_FORMAT,data->wan_idx);	

	RunSystemCmd(NULL_FILE,"brctl","addbr",wanBrifName,NULL_STR);
	
	//fprintf(stderr,"%s:%d brctl addbr %s\n",__FUNCTION__,__LINE__,wanBrifName);
	RunSystemCmd(NULL_FILE,"brctl","addif",wanBrifName,wanIfaceName,NULL_STR);
	
	for(i=0;i<WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM;i++)
	{
		if(wanBindingLanPorts[i]!=data->wan_idx) //not current wan bind, continue
			continue;
		bzero(ifName,sizeof(ifName));
		
		if(getLanPortIfName(ifName,i)!=1)
			continue;
		
		//in case other lan/wan bind to the port, delif form the lan/wan bridge first
		//if(wanBindingLanPorts_orig[i]!=data->wan_idx)//orig bind port not current bind port, need to clean
		//	clean_origWanBindLanPort(wanBindingLanPorts_orig[i],ifName);
		//else
		//fprintf(stderr,"brctl delif br0 %s\n",ifName);
		//fprintf(stderr,"brctl addif %s %s\n",wanBrifName,ifName);
		{//also need to add the port to current bridge again			
			RunSystemCmd(NULL_FILE,"brctl","delif","br0",ifName,NULL_STR);
			RunSystemCmd(NULL_FILE,"brctl","addif",wanBrifName,ifName,NULL_STR);
		}

		bind_lan_port_mask |= 1<<i;
	}	
	RunSystemCmd(NULL_FILE,"ifconfig", wanBrifName, "up" , NULL_STR);
	sprintf(bind_lan_port_mask_str,"%x", bind_lan_port_mask);
	RunSystemCmd(NULL_FILE,"vconfig","port_mapping", wanIfaceName, bind_lan_port_mask_str, "1", NULL_STR);

	#if defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
	apmib_get(MIB_IGMP_FAST_LEAVE_DISABLED, (void *)&intValue);
	if(intValue)
	{
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","0","2", NULL_STR);
	}
	else
	{	
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","1","0", NULL_STR);
	}
	#endif
	return 0;
#endif
}

void clean_wan_app(BASE_DATA_Tp data)
{	
	FILE *fp;
	unsigned char pidfile[48],line[40],del_cmd[64];
	int pid;

	/*clean last wan daemon (dhcp/pppoe)*/
	sprintf(pidfile,"/var/run/wan_%d.pid",data->wan_idx);
	if ((fp = fopen(pidfile, "r")) != NULL) 
	{
		fgets(line, sizeof(line), fp);
		if (sscanf(line, "%d", &pid)) 
		{
			if (pid > 1)
			{
				memset(del_cmd,0,sizeof(del_cmd));
				sprintf(del_cmd,"kill -15 %d",pid);
				system(del_cmd);
				printf("%s.%d.kill command(%s)\n",__FUNCTION__,__LINE__,del_cmd);
			}
		}
		fclose(fp);
	}
}

#if 1
int get_last_wan_type(BASE_DATA_Tp data,int *plast_wan_type)
{
	int last_wan_type=0,retVal=0;
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

	retVal=reinitFunc_get_mib_orig_value(MIB_WANIFACE_ADDRESSTYPE,(void*)&last_wan_type,data);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	if(retVal<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
		return -1;
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d last_wan_type=%d\n",__FUNCTION__,__LINE__,last_wan_type);
	*plast_wan_type=last_wan_type;
	return 0;
}
#endif
int clean_wan_bridge(BASE_DATA_Tp data)
{
#if 0
	int origWanType=0;
	char multiwanBrifName[IFACE_NUM_MAX]={0};
	char wanIfaceName[IFACE_NUM_MAX]={0};

	if(reinitFunc_get_mib_orig_value(MIB_WANIFACE_ADDRESSTYPE,(void*)&origWanType,data)<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
		return -1;
	}
	sprintf(multiwanBrifName,MULTI_BRNAME_FORMAT,data->wan_idx);
	if(origWanType==BRIDGE)
	{			
		sprintf(wanIfaceName,WAN_IFNAME_FORMAT,data->wan_idx);
		RunSystemCmd(NULL_FILE,"brctl","delif",multiwanBrifName,wanIfaceName,NULL_STR);
	}
#else
	int last_wan_type=0,i=0;
	if(get_last_wan_type(data,&last_wan_type)<0)
		return -1;
	
	reinitSer_printf(LOG_DEBUG,"%s:%d last_wan_type=%d\n",__FUNCTION__,__LINE__,last_wan_type);
	if(last_wan_type==BRIDGE)
	{
		char cmdbuff[COMMAND_BUFF_LEN]={0};
		char wanBrName[MAX_NAME_LEN]={0};
		char lanPortIfName[MAX_NAME_LEN]={0};
		WanIntfacesType wanBindingLanPorts_orig[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};

		sprintf(wanBrName,MULTI_BRNAME_FORMAT,data->wan_idx);
		
		RunSystemCmd(NULL_FILE,"ifconfig",wanBrName,"down",NULL_STR);
		
		
		if(reinitFunc_get_mib_orig_value(MIB_WANIFACE_BINDING_LAN_PORTS,(void*)&wanBindingLanPorts_orig,data)<0)
		{
			reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
			return -1;
		}

		for(i=0;i<WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM;i++)
		{
			if(wanBindingLanPorts_orig[i]==data->wan_idx)
			{//need to del the iface from the bridge
				bzero(lanPortIfName,sizeof(lanPortIfName));
				
				if(getLanPortIfName(lanPortIfName,i)!=1)
					continue;
				//fprintf(stderr,"brctl delif br0 %s\n",wanBrName);
		//fprintf(stderr,"brctl addif br0 %s\n",lanPortIfName);
				RunSystemCmd(NULL_FILE,"brctl","delif",wanBrName,lanPortIfName,NULL_STR);
				reinitSer_printf(LOG_DEBUG,"%s:%d brctl delif %s %s\n",__FUNCTION__,__LINE__,wanBrName,lanPortIfName);
			//add the iface to br0 incase it change to not bridge mode
				RunSystemCmd(NULL_FILE,"brctl","addif","br0",lanPortIfName,NULL_STR);
			}
		}
		RunSystemCmd(NULL_FILE,"brctl","delbr",wanBrName,NULL_STR);
		reinitSer_printf(LOG_DEBUG,"%s:%d brctl delbr %s\n",__FUNCTION__,__LINE__,wanBrName);
		//sprintf(cmdbuff,"brctl delbr "MULTI_BRNAME_FORMAT,data->wan_idx);
		//system(cmdbuff);
		//reinitSer_printf(LOG_DEBUG,"%s:%d cmdBuff=%s\n",__FUNCTION__,__LINE__,cmdbuff);
	}
#endif
	return 0;
}
void start_wanapp(BASE_DATA_Tp data)
{
	WAN_DATA_T wan_info;

	/*format wan private data */
	if(!format_wan_data(&wan_info,data))
		return FALSE;

	/*start wan */
}
clean_last_wan_info(BASE_DATA_Tp data)
{	
	int last_wan_type;
	
	if(get_last_wan_type(data,&last_wan_type)<0)
			return -1;

	if(last_wan_type ==PPTP)
	{
		RunSystemCmd(NULL_FILE, "killall", "-15", "pptp",NULL_STR);	
//		RunSystemCmd(NULL_FILE, "killall", "-15", "pppd", NULL_STR);
		
	}
	else if(last_wan_type ==L2TP)
	{
		system("echo\"d client\" > /var/run/l2tp-control &");
	//	printf("kill l2tp $$$$$$$$$$$$$$$$\n");	
		RunSystemCmd(NULL_FILE, "killall", "-15","l2tpd", NULL_STR);
//		RunSystemCmd(NULL_FILE, "killall", "-15", "pppd", NULL_STR);
	}
}

/* clean each wan connect info */
void clean_wan(BASE_DATA_Tp data)
{
	clean_wan_bridge(data);
	/* clean wan app for current wan connect */
	clean_wan_app(data);
	/* clean wan firewall for current wan connect */
	clean_firewall_rules(data);
	/*clean wan info*/
	clean_last_wan_info(data);
	/*delete wan  interface ,must the end position for clean wan API*******/	
	clean_wan_interface(data);	
	/*clean wan configure file *******/
	clean_wan_config_file(data);
}
void set_common_wan_info()
{
	int wan_index,flag;
	WANIFACE_T WanIfaceEntry;
	flag = 0;
	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
		getWanIfaceEntry(wan_index,&WanIfaceEntry);
		if(WanIfaceEntry.AddressType == PPPOE)
		{
			flag = 1;
			break;
		}
	}
	if(flag)
		RunSystemCmd(PROC_FASTPPPOE_FILE, "echo", "1", NULL_STR);
	else 
		RunSystemCmd(PROC_FASTPPPOE_FILE, "echo", "0", NULL_STR);
	printf("%s.%d.fast_pppoe(%d)\n",__FUNCTION__,__LINE__,flag);
}

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
int get_Igmpproxy_Pid()
{
	struct stat status;
	char buff[100];
	char filename[100];
	FILE *fp;
	strncpy(filename, IGMPPROXY_PID_FILE, 100);

	if ( stat(filename, &status) < 0)
		return -1;
	fp = fopen(filename, "r");
	if (!fp) {
        	fprintf(stderr, "Read pid file error!\n");
		return -1;
   	}
	fgets(buff, 100, fp);
	fclose(fp);

	return (atoi(buff));
}

int start_igmpproxy(BASE_DATA_Tp data, int startTimeFlag)
{
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	int pid = -1;
	char pidBuf[100] = {0};
	char WanIfName[MAX_NAME_LEN+1] = {0};
	int signo = MYSIG;
	union sigval mysigval;
	int WanIfFlag;
	char Command[256] = {0};
		
	if(!getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry))
	{
		//printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return FALSE;
	}

#if 0
	printf("wan type: %d, igmpproxy enable: %d, on phyIf : %d, [%s:%d]\n", 
		wan_p->wan_data.wan_type, WanIfaceEntry.enableIGMP, WanIfaceEntry.onPhyIf, __FUNCTION__, __LINE__);
#endif
	//printf("flush igmpDb! [%s:%d]\n", __FUNCTION__, __LINE__);
	//RunSystemCmd(PROC_BR_IGMPDB, "echo", "flush", NULL_STR);

	
	/*set firewall rules for igmpproxy*/
	func_set_firewall_rules_for_igmpproxy(wan_p);
	
	if(WanIfaceEntry.enableIGMP != 1)
	{
		//printf("igmp proxy disabled! [%s:%d]\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	
	/*if igmpproxy is on physical interface, handle igmpproxy issue in __start_wan()
	   else in func_connect_wan() */
	if((startTimeFlag == START_BEFORE_CONNECTION && WanIfaceEntry.onPhyIf == 0) ||
	   (startTimeFlag == START_AFTER_CONNECTION && WanIfaceEntry.onPhyIf == 1))
	{
		//printf("onPhyif flag is not matched! [%s:%d]\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	if(wan_p->wan_data.wan_type == PPPOE && WanIfaceEntry.onPhyIf == 0)
	{
		strcpy(WanIfName, wan_p->wan_data.virtual_wan_iface);
		WanIfFlag = WAN_VIF_1 << (wan_p->base.wan_idx-1);
	}
	else
	{
		strcpy(WanIfName, wan_p->wan_data.wan_iface);
		WanIfFlag = WAN_PIF_1 << (wan_p->base.wan_idx-1);
	}
	
	
	pid = get_Igmpproxy_Pid();
	if(pid > 0)
	{	
		/*igmpproxy already exsit, send a signal and modify igmpproxy wan dev*/
		//snprintf(pidBuf, 100, "%d", pid);
		//RunSystemCmd(NULL_FILE, "kill", "-SIGUSR1", pidBuf);
    	mysigval.sival_int = ADD_WAN_IF | WanIfFlag;  
  
    	if(sigqueue(pid, signo, mysigval)==-1)  
    	{  
        	printf("send signal fail!\n");  
        	return FALSE;  
    	}  
  
    	//printf("send signal successfully\n");
    	//sleep(2);  
	}
	else
	{
		/*start igmpproxy daemon */
		RunSystemCmd(NULL_FILE, "igmpproxy", WanIfName, "br0", NULL_STR);
		RunSystemCmd(PROC_BR_MCASTFASTFWD, "echo", "1,1", NULL_STR);
		RunSystemCmd(PROC_IGMP_MAX_MEMBERS, "echo", "128", NULL_STR);
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "1", NULL_STR);
		RunSystemCmd(PROC_BR_IGMPPROXY, "echo", "1", NULL_STR);
	}

}

int stop_igmpproxy(BASE_DATA_Tp data, int stopTimeFlag)
{
	WANIFACE_T WanIfaceEntry, WanIfaceEntry_tmp;
	WAN_DATA_Tp wan_p = (WAN_DATA_Tp)data;
	int pid = -1;
	int signo = MYSIG;
	union sigval mysigval;
	int WanIfFlag = 0, wan_index;
		
	if(!getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry))
	{
		//printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return FALSE;
	}

	//for static/dscp/bridge onphyIf is meaningless, it is always zeros
	//printf("onPhyIf:%d, enable igmpProxy:%d, [%s:%d]\n", WanIfaceEntry.onPhyIf, WanIfaceEntry.enableIGMP, __FUNCTION__, __LINE__);

	if(stopTimeFlag == STOP_AFTER_DISCONNECTION && WanIfaceEntry.onPhyIf)
	{
		//called in wan_disconnet, pppoe with onphyif==1
		//printf("stop Time Flag not matched! [%s:%d]\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	pid = get_Igmpproxy_Pid();
	if(pid <= 0)
	{
		//igmpproxy deamon not start yet
		//printf("igmpproxy deamon not started yet! [%s:%d]\n", __FUNCTION__, __LINE__);		
		return FALSE;
	}
	else
	{	
		//if no interface need igmpproxy, kill igmpproxy daemon
		
		for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
		{
	
			memset(&WanIfaceEntry_tmp,0,sizeof(WANIFACE_T));
			getWanIfaceEntry(wan_index,&WanIfaceEntry_tmp);
			if(WanIfaceEntry_tmp.enable && WanIfaceEntry_tmp.enableIGMP)
			{
				break;
			}
		}
		if(wan_index > WANIFACE_NUM)
		{
			//printf("no interface enable igmpproxy, kill it! [%s:%d]\n", __FUNCTION__, __LINE__);
			RunSystemCmd(NULL_FILE, "killall", "-9", "igmpproxy", NULL_STR);
			if(isFileExist(IGMPPROXY_PID_FILE))
			{
				unlink(IGMPPROXY_PID_FILE);
			}
			
			RunSystemCmd(PROC_BR_IGMPPROXY, "echo", "0", NULL_STR);
			RunSystemCmd(PROC_BR_MCASTFASTFWD, "echo", "0,0", NULL_STR);
			return TRUE;
		}

	}

	
	if((!WanIfaceEntry.enable) || (!WanIfaceEntry.enableIGMP))
	{
		//delete physical and virtual interface
		WanIfFlag = (WAN_PIF_1|WAN_VIF_1) << (wan_p->base.wan_idx-1);
		//printf("wanIfFlag = %x, [%s:%d]\n", WanIfFlag, __FUNCTION__, __LINE__);
 
	}
 	else
	{
		if(stopTimeFlag == STOP_BEFORE_DISCONNECTION)
		{	
			//called in __start_wan
			if(wan_p->wan_data.wan_type == PPPOE && WanIfaceEntry.onPhyIf == 0)
			{
				WanIfFlag = WAN_PIF_1 << (wan_p->base.wan_idx-1);
			}
			else if(wan_p->wan_data.wan_type == DHCP_CLIENT)
			{
				WanIfFlag = WAN_PIF_1 << (wan_p->base.wan_idx-1);
			}
			else
			{
				/*for static and pppoe+onphy, add signal will send soon, don't send del signal to advoid to much signal
				   pppx which possiblely should be deleted will be handled in igmpproxy daemon*/ 
			}
		
		}
		else if(stopTimeFlag == STOP_AFTER_DISCONNECTION)
		{
			//called in disconnect, delete vif for pppoe and delete pif for dhcp
			if(wan_p->wan_data.wan_type == PPPOE)
			{
				WanIfFlag = WAN_VIF_1 << (wan_p->base.wan_idx-1);
			}
			else if(wan_p->wan_data.wan_type == DHCP_CLIENT)
			{
				WanIfFlag = WAN_PIF_1 << (wan_p->base.wan_idx-1);
				WanIfFlag |= NEED_DEL_VIF;
			}
			else
			{
			 	//do nothing
			}
						
		}
		else 
		{
			//do nothing
		}
	}

	if(WanIfFlag)
	{
		//need to delete interface
		mysigval.sival_int = DEL_WAN_IF | WanIfFlag;
		//printf("pid = %d, parameter = %x [%s:%d]\n", pid, mysigval.sival_int, __FUNCTION__, __LINE__);
  
    	if(sigqueue(pid, signo, mysigval)==-1)  
    	{  
        	printf("send signal fail!\n");  
        	return FALSE;  
    	}  
      	//printf("send signal successfully\n"); 
	}
	
	return TRUE;

}
#endif

#ifdef SINGLE_WAN_SUPPORT
/*wan mac should be configured as lan mac when bridge or WISP mode. otherwise ping failed*/
void start_eth_wan_bridge_mac()
{
	char lan_mac[6]={0},hw_lan_mac[6]={0};
	char cmdBuffer[32]={0};
	
	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)lan_mac);
	if(!memcmp(lan_mac, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)lan_mac);
	sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)lan_mac[0], (unsigned char)lan_mac[1], 
		(unsigned char)lan_mac[2], (unsigned char)lan_mac[3], (unsigned char)lan_mac[4], (unsigned char)lan_mac[5]);

	RunSystemCmd(NULL_FILE, "ifconfig", "eth1.1", "down", NULL_STR);	
	RunSystemCmd(NULL_FILE, "ifconfig", "eth1.1", "hw", "ether",cmdBuffer, NULL_STR);
}
void set_eth_wan_as_bridge(BASE_DATA_Tp data)
{
	/*add in br0*/
	start_eth_wan_bridge_mac();
	RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,"eth1.1", NULL_STR);
	RunSystemCmd(NULL_FILE, "ifconfig", "eth1.1", "0.0.0.0", NULL_STR);	
	
}
#endif

void __start_wan(BASE_DATA_Tp data)
{
	WAN_DATA_T wan_info;
#ifdef SINGLE_WAN_SUPPORT
	OPMODE_T opmode;
#endif

	/*format wan private data */
	if(!format_wan_data(&wan_info,data))
		return 0;

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	stop_igmpproxy(&wan_info, STOP_BEFORE_DISCONNECTION);
#endif

	if(!isValidWan(&wan_info))
		return 0;
	
	/*create eth wan interface */
	create_wan_interface(&wan_info);
	
#ifdef SINGLE_WAN_SUPPORT
	apmib_get(MIB_OP_MODE,&opmode);
	if(opmode != GATEWAY_MODE) {
		set_eth_wan_as_bridge(data);
		if(opmode == BRIDGE_MODE)
			return;
	}
#endif

	/*set basic wan info */
	set_wan_interface(&wan_info);

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	/*start igmpproxy no matter wan interface is connected or not*/
	start_igmpproxy(&wan_info, START_BEFORE_CONNECTION);
#endif
	
	switch(wan_info.wan_data.wan_type)
	{
		case DHCP_DISABLED:
			set_staticIP(&wan_info);
			break;
		case DHCP_CLIENT:
			set_dhcp_client(&wan_info);
			break;
		case PPPOE:
			set_pppoe(&wan_info);
			break;
		case BRIDGE:
			set_wan_bridge(&wan_info);
		
#if defined(SINGLE_WAN_SUPPORT)
		case L2TP:
			set_L2TP(&wan_info);
				printf("%s.%d.start l2tp dail\n",__FUNCTION__,__LINE__);
			break;
		case PPTP:
			set_PPTP(&wan_info);
				printf("%s.%d.start PPTP dail\n",__FUNCTION__,__LINE__);			
			break;
#endif
		default:
			break;
	}
}

/* start each wan connect session */
void start_wan(BASE_DATA_Tp data)
{	
	/*set wan common info *****/
	set_common_wan_info();
	/*start  wan session **********/
	__start_wan(data);

//	start_wanapp(data);
	
}

int reinit_func_start_wan(BASE_DATA_Tp data)
{
	printf("%s.%d.reinit wan \n",__FUNCTION__,__LINE__);
	/*clean original wan session info*/
	clean_wan(data);

	printf("%s.%d.start wan \n",__FUNCTION__,__LINE__);

	/*start to create wan session. static/dhcp/pppoe*/
	start_wan(data);
	start_wan_upnp_igd();
	return 1;
}
int reinit_func_start_wan_power_on(BASE_DATA_Tp data)
{
	return reinit_func_start_wan(data);
}

int func_disconnect_wan(BASE_DATA_Tp data)
{	
	struct stat status;
	unsigned char link_file[32];
	
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	WAN_DATA_T wan_info;
	/*format wan private data */
	if(!format_wan_data(&wan_info,data))
		return 0;
#endif
	printf("%s.%d.start disconnect wan...\n",__FUNCTION__,__LINE__);
	sprintf(link_file,"/etc/ppp/link%d",data->wan_idx);	
	if(stat(link_file, &status)==0)	
		unlink(link_file);
#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	/*stop igmp proxy*/
	stop_igmpproxy(&wan_info, STOP_AFTER_DISCONNECTION);
#endif	

	/*clean wan app*/
	clean_wan_app(data);	

	
}
void create_resolv_file(WANIFACE_T *pdata)
{
	char tmpBuff1[32]={0}, tmpBuff2[32]={0}, tmpBuff3[32]={0};	
	char line_buffer[100]={0};
	char *strtmp=NULL;
	
	strcpy(tmpBuff1,pdata->wanIfDns1);
	strcpy(tmpBuff2,pdata->wanIfDns2);
	strcpy(tmpBuff3,pdata->wanIfDns3);

	if (memcmp(tmpBuff1, "\x0\x0\x0\x0", 4))
	{
		strtmp= inet_ntoa(*((struct in_addr *)tmpBuff1));
		sprintf(line_buffer,"nameserver %s\n",strtmp);		
		write_line_to_file2(RESOLV_CONF, line_buffer);
	}
	if (memcmp(tmpBuff2, "\x0\x0\x0\x0", 4))
	{
		strtmp= inet_ntoa(*((struct in_addr *)tmpBuff2));
		sprintf(line_buffer,"nameserver %s\n",strtmp);		
		write_line_to_file2(RESOLV_CONF,line_buffer);
	}
	if (memcmp(tmpBuff3, "\x0\x0\x0\x0", 4))
	{
		strtmp= inet_ntoa(*((struct in_addr *)tmpBuff3));
		sprintf(line_buffer,"nameserver %s\n",strtmp);		
		write_line_to_file2(RESOLV_CONF,line_buffer);
	}	
}

static void start_dnrd()
{	
	char line[128],nameserver[32], nameserver_ip[32];	
	char dns_server[5][32];	
	char *cmd_opt[16];
	int x,cmd_cnt = 0;
	FILE *fp1;
	
	system("killall -9 dnrd 2> /dev/null");
	
	fp1= fopen("/var/resolv.conf", "r");
	
	if(fp1 == NULL){
		printf("open file fail\n");
		return;
	}
	for (x=0;x<5;x++)
		memset(dns_server[x], '\0', 32);
	
	while (fgets(line, sizeof(line), fp1) != NULL) 
	{
		memset(nameserver_ip, '\0', 32);
		sscanf(line, "%s %s", nameserver, nameserver_ip);		
		{
			printf("nameserver_ip(%s)\n",nameserver_ip);
			for(x=0;x<5;x++)
			{
				if(dns_server[x][0] == '\0')
				{
					sprintf(dns_server[x], "%s", nameserver_ip);
					break;
				}
			}
		}
	}
	fclose(fp1);	

	cmd_opt[cmd_cnt++]="dnrd";
	cmd_opt[cmd_cnt++]="--cache=off";
	for(x=0;x<5;x++)
	{
		if(dns_server[x][0] != '\0')
		{
			cmd_opt[cmd_cnt++]="-s";
			cmd_opt[cmd_cnt++]=&dns_server[x][0];
		}
	}
	cmd_opt[cmd_cnt++] = 0;	
	DoCmd(cmd_opt, NULL_FILE);
	
	printf("\n\n");
	for(x = 0 ; x < cmd_cnt; ++x)
		printf("%s ",cmd_opt[x]);
	printf("\n\n");
	printf("%s.%d.start dnrd\n",__FUNCTION__,__LINE__);
}

void start_DNS(BASE_DATA_Tp data)
{
	char line_buffer[100]={0};

	WAN_DATA_Tp wan_p;	
	WANIFACE_T WanIfaceEntry;	
	wan_p = (WAN_DATA_Tp)data;
	if(!getWanIfaceEntry(wan_p->base.wan_idx,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return 0;
	}

	/*remove dns file******************/
	system("rm  -rf /var/resolv.conf");

	/*create dns resolv file ************/
	switch(WanIfaceEntry.dnsAuto)
	{
		case 0:	/*manual*/
			create_resolv_file(&WanIfaceEntry);
			break;
		case 1:/* auto */
			switch(WanIfaceEntry.AddressType)
			{
				case DHCP_CLIENT:
					sprintf(line_buffer,"nameserver %s\n",wan_p->wan_data.dns_addr);		
					write_line_to_file2(RESOLV_CONF,line_buffer);
					break;
				case PPPOE:
#if defined(SINGLE_WAN_SUPPORT)
				case PPTP:
				case L2TP:
#endif					
					system("cp -r /etc/ppp/resolv.conf /var/resolv.conf");
					break;
				default:
					break;
			}
			break;			
		default:
			break;
	}
		
	/*start dns resolv file ************/
	start_dnrd();
}

void start_wan_upnp_igd()
{
	char tmp1[16]={0};
	char tmp2[16]={0};		
	WANIFACE_T *pWan_entity,wan_entity;
	int i,total,val,found = 0;
	
	apmib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);
	for( i=1; i<=total; i++ )
	{
		pWan_entity = &wan_entity;
		*((char *)pWan_entity) = (char)i;
		if(!apmib_get(MIB_WANIFACE_TBL, (void *)pWan_entity))
			continue;
		if(pWan_entity->enable && pWan_entity->enableUPNP){
			found = 1;
			break;
		}
	}
		
	RunSystemCmd(NULL_FILE, "killall", "-15", "miniigd", NULL_STR); 
	if(found==1){
		RunSystemCmd(NULL_FILE, "route", "del", "-net", "239.255.255.250", "netmask", "255.255.255.255", "br0", NULL_STR); 
		RunSystemCmd(NULL_FILE, "route", "add", "-net", "239.255.255.250", "netmask", "255.255.255.255", "br0", NULL_STR); 
		sprintf(tmp1, "%d", pWan_entity->AddressType);
		sprintf(tmp2, "%d", i);
		RunSystemCmd(NULL_FILE, "miniigd", "-e", tmp1, "-i", "br0","-m",tmp2,NULL_STR);
	}
	
}

void add_wan_info(BASE_DATA_Tp data)
{
	struct stat status;
	char wan_info_file[64];
	WAN_DATA_Tp wan_p;
	int wan_index;	
	wan_p = (WAN_DATA_Tp)data;
	wan_index = wan_p->base.wan_idx;
	sprintf(wan_info_file,"/var/wan_info_%d",wan_index);


	/*remove this wan last configure **************/
	RM(wan_info_file);	
	write_line_to_file2(wan_info_file, wan_p->wan_data.gw_addr);
}

/*start wan app*/
int func_connect_wan(BASE_DATA_Tp data)
{	
	WAN_DATA_Tp wan_p;
	BASE_DATA_T base;
	unsigned char wan_iface[32];
	base.wan_idx = data->wan_idx;
	
	printf("%s.%d.start connect wan...\n",__FUNCTION__,__LINE__);	
	/*format wan private data */
	if(!format_wan_data(data,&base))
		return 0;
	
	wan_p = (WAN_DATA_Tp)data;
	
	printf("%s.%d.wan_type(%d)\n",__FUNCTION__,__LINE__,wan_p->wan_data.wan_type);	
	/*set dhcp ip address***********************/
	if(wan_p->wan_data.wan_type == DHCP_CLIENT)	
	{
		RunSystemCmd(NULL_FILE, "ifconfig", wan_p->wan_data.wan_iface, wan_p->wan_data.ip_addr,
			"netmask",wan_p->wan_data.sub_net, NULL_STR);
		
		/*set default gw */
		RunSystemCmd(NULL_FILE, "route", "add", "-net", "default", "gw", 
			wan_p->wan_data.gw_addr, "dev", wan_p->wan_data.wan_iface, NULL_STR);
	}

	#if defined(SINGLE_WAN_SUPPORT)
	/*set default gw */
	RunSystemCmd(NULL_FILE, "route", "add", "-net", "default", "gw", 
		wan_p->wan_data.gw_addr, NULL_STR);	
	#endif
	printf("%s.%d. set default gw........\n",__FUNCTION__,__LINE__);
	
	/*set wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE)
		strcpy(wan_iface,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(wan_iface,wan_p->wan_data.wan_iface);
	
	/*start dns *************/
	start_DNS(wan_p);

	/*add wan info to tmp file ************/
	add_wan_info(wan_p);

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
	/*send signal to igmpproxy*/
	start_igmpproxy(wan_p, START_AFTER_CONNECTION);
#endif
}


int wan_power_on()
{
	BASE_DATA_T	base;
	int wan_index ;
	printf("%s.%d. START WAN ....\n",__FUNCTION__,__LINE__);
	for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
	{
		memset(&base,0,sizeof(BASE_DATA_T));
		base.wan_idx = wan_index;
		reinit_func_start_wan_power_on(&base);
	}
	start_wan_upnp_igd();
}

int func_pppoe_start(BASE_DATA_Tp data)
{
	unsigned char command[64];
	sprintf(command,"pppd -%d &",data->wan_idx);
	system(command);	
}

int func_pppoe_exit(BASE_DATA_Tp data)
{
	system("killall -9 dnrd 2> /dev/null");
	func_disconnect_wan(data);	
}

int func_pptp_start(BASE_DATA_Tp data)
{	
	system("pppd call rpptp &");
}

int func_pptp_exit(BASE_DATA_Tp data)
{	
	RunSystemCmd(NULL_FILE, "killall", "-15", "pptp",NULL_STR); 
	RunSystemCmd(NULL_FILE, "killall", "-15", "pppd",NULL_STR); 
	system("killall -9 dnrd 2> /dev/null");	
	func_disconnect_wan(data);	
}
int func_l2tp_start(BASE_DATA_Tp data)
{
	system("echo \"c client\" > /var/run/l2tp-control &");
}

int func_l2tp_exit(BASE_DATA_Tp data)
{
	system("echo \"d client\" > /var/run/l2tp-control &");
	system("killall -9 dnrd 2> /dev/null");	
	func_disconnect_wan(data);
}


