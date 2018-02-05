#include "reinitFunc_lan.h"
#include "reinit_utility.h"
#include "reinitSer.h"




int clear_lan_dhcpc()
{
	char brIfName[32]={0};
	char pidFilePath[128]={0},deconfig_script[128]={0};

	get_br_interface_name(brIfName);
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", brIfName);
	sprintf(pidFilePath, "/etc/udhcpc/udhcpc-%s.pid", brIfName); /*pid path*/
	if(killDaemonByPidFile(pidFilePath)>=0)
	{
		reinitSer_printf(LOG_DEBUG,"kill lan dhcpc\n");
		unlink(deconfig_script);
		unlink(pidFilePath);
	}	
	return 0;
}
int clear_lan_dhcpd()
{
	int pid=0;
	char strPID[16]={0};
	if(isFileExist(DHCPD_PID_FILE))
	{
		pid=getPid_fromFile(DHCPD_PID_FILE);
		if(pid != 0){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-16", strPID, NULL_STR);/*inform dhcp server write lease table to file*/
			sleep(1);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
		}
		unlink(DHCPD_PID_FILE);
	}
}
int clean_lan_dhcp()
{
	clear_lan_dhcpc();
	clear_lan_dhcpd();
}

#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
void clean_lan_domain_query_dnrd()
{
	RunSystemCmd(NULL_FILE, "killall", "-9", "dnrd", NULL_STR);	
}
#endif

#ifndef STAND_ALONE_MINIUPNP
void clean_lan_miniupnp()
{	
	RunSystemCmd(NULL_FILE, "killall", "-9", "mini_upnpd", NULL_STR);	
}
#endif

#ifdef CONFIG_AUTO_DHCP_CHECK
void clean_lan_Auto_DHCP_Check()
{	
	RunSystemCmd(NULL_FILE, "killall", "-9", "Auto_DHCP_Check", NULL_STR);	
}
#endif
#ifdef SAMBA_WEB_SUPPORT
void clean_lan_samba_web()
{	
	RunSystemCmd(NULL_FILE, "killall", "-9", "smbd",NULL_STR);	
}
#endif
#if defined(CONFIG_APP_TR069)
void clean_lan_tr069()
{	
	char acsURLStr[CWMP_ACS_URL_LEN+1];
	int pid;
	char strPID[32]={0};
	/* Keep Tr069 alive unless 1.ACSURL changed 2.Turn off tr069 */
	if ((pid=find_pid_by_name("cwmpClient")) > 0)
	//if(isFileExist(TR069_PID_FILE))
	{									
		unsigned char acsUrltmp[CWMP_ACS_URL_LEN+1];
		int tr069Flag;
		
		apmib_get( MIB_CWMP_ACS_URL_OLD, (void *)acsUrltmp);
		apmib_get( MIB_CWMP_ACS_URL, (void *)acsURLStr);
		apmib_get( MIB_CWMP_FLAG, (void *)&tr069Flag);
		
		if(strcmp(acsUrltmp, acsURLStr) != 0 || ((tr069Flag & CWMP_FLAG_AUTORUN) == 0))
		{
		//pid=getPid_fromFile(TR069_PID_FILE);
		if(pid != -1){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-15", strPID, NULL_STR);
		}
		unlink(TR069_PID_FILE);
	}
	}	
}
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
void	clean_lan_ETH_802DOT1X()
{
	RunSystemCmd(NULL_FILE, "killall", "-9", "auth_eth",NULL_STR);
}
#endif

#ifdef RTK_CAPWAP
void	clean_lan_capwap_app()
{	
	RunSystemCmd(NULL_FILE, "killall", "-9", "WTP",NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "AC",NULL_STR);
}
#endif

void clean_lan_mac()
{
	//it seems nothing need to clean
}
void start_lan_mac()
{
	char lan_mac[6]={0},hw_lan_mac[6]={0};
	char cmdBuffer[BR_COMMAND_LEN]={0};
	char brIfName[IFACE_NAME_MAX]={0};
	char br_lan1_interface[IFACE_NAME_MAX]={0};
	char br_lan2_interface[IFACE_NAME_MAX]={0};
	int opmode=0;

	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)lan_mac);
	if(!memcmp(lan_mac, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)lan_mac);
	sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)lan_mac[0], (unsigned char)lan_mac[1], 
		(unsigned char)lan_mac[2], (unsigned char)lan_mac[3], (unsigned char)lan_mac[4], (unsigned char)lan_mac[5]);
	get_br_interface_name(brIfName);

//set br0
	RunSystemCmd(NULL_FILE, "ifconfig", brIfName, "hw", "ether",cmdBuffer, NULL_STR);

//set lan1(eth0)
	
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth2", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth3", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth4", "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
#else
	get_br_lan1_interface(br_lan1_interface);
	if(br_lan1_interface[0])
		RunSystemCmd(NULL_FILE, "ifconfig", br_lan1_interface, "hw", "ether", cmdBuffer, NULL_STR);/*set eth0 mac address*/
#endif
#ifdef MULTI_WAN_SUPPORT
	RunSystemCmd(NULL_FILE, "ifconfig", "eth2", "hw", "ether", cmdBuffer, NULL_STR);/*set eth2 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth3", "hw", "ether", cmdBuffer, NULL_STR);/*set eth3 mac address*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth4", "hw", "ether", cmdBuffer, NULL_STR);/*set eth4 mac address*/
#endif

//set lan2(eth1) when eth1 in lan bridge
	apmib_get(MIB_OP_MODE,(void*)&opmode);
	if(opmode == BRIDGE_MODE || opmode == WISP_MODE){
		apmib_get(MIB_ELAN_MAC_ADDR,  (void *)lan_mac);
		if(!memcmp(lan_mac, "\x00\x00\x00\x00\x00\x00", 6))
		{
#if defined(CONFIG_RTL_8198_AP_ROOT) || defined(CONFIG_RTL_8197D_AP)
			apmib_get(MIB_HW_NIC0_ADDR,  (void *)lan_mac);
#else
			apmib_get(MIB_HW_NIC1_ADDR,  (void *)lan_mac);
#endif			
		}
		sprintf(cmdBuffer, "%02x%02x%02x%02x%02x%02x", (unsigned char)lan_mac[0], (unsigned char)lan_mac[1], 
			(unsigned char)lan_mac[2], (unsigned char)lan_mac[3], (unsigned char)lan_mac[4], (unsigned char)lan_mac[5]);	
		getBr_lan2_interface(br_lan2_interface);
		if(br_lan2_interface[0])	
			RunSystemCmd(NULL_FILE, "ifconfig", br_lan2_interface, "hw", "ether", cmdBuffer, NULL_STR);/*set eth1 mac address when bridge mode*/
	}

}


#ifndef MULTI_WAN_SUPPORT
/*
*  BR_IFACE_FILE save br0 interfaces, format like eth0:wlan0:wlan1:
*/
int clean_bridge()
{
	FILE * fp=NULL;
	char bridge_ifaces[BR_IFACES_LEN], tmpBuff[BR_COMMAND_LEN], cmdBuffer[BR_COMMAND_LEN];
	char *ifaces[IFACE_NUM_MAX]={0};
	int i=0;
	char *token=NULL;
	
	if(isFileExist(BR_IFACE_FILE))
	{
		bzero(bridge_ifaces,sizeof(bridge_ifaces));
		fp=fopen(BR_IFACE_FILE, "r");
		if(!fp)
		{			
			reinitSer_printf(LOG_ERR,"%s:%d open file %s fail!\n",__FUNCTION__,__LINE__,BR_IFACE_FILE);
			exit(0);
		}
		fgets(bridge_ifaces,sizeof(bridge_ifaces),fp);
		bridge_ifaces[strlen(bridge_ifaces)-1]=0;// rm "\n"
		fclose(fp);
		unlink(BR_IFACE_FILE);
		reinitSer_printf(LOG_DEBUG,"bridge_ifaces=%s\n",bridge_ifaces);
		devideStringIntoItems(bridge_ifaces,IFACE_NUM_MAX,':',ifaces);
	}else
	{
		get_br_interfaces(bridge_ifaces);
		reinitSer_printf(LOG_DEBUG,"bridge_ifaces=%s\n",bridge_ifaces);
		devideStringIntoItems(bridge_ifaces,IFACE_NUM_MAX,' ',ifaces);
	}
		
	for(i=0;ifaces[i] && ifaces[i][0];i++)
	{
		//reinitSer_printf(LOG_DEBUG,"iface=%s  %s:%d\n",ifaces[i],__FUNCTION__,__LINE__);
		token=ifaces[i];
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0" ,token, NULL_STR);
		//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br1" ,token, NULL_STR);
		//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
		//RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
	}
	
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if(isFileExist(BR_IFACE_FILE2)){
		bzero(bridge_ifaces,sizeof(bridge_ifaces));
		fp=fopen(BR_IFACE_FILE2, "r");
		if(!fp)
		{			
			reinitSer_printf(LOG_ERR,"%s:%d open file %s fail!\n",__FUNCTION__,__LINE__,BR_IFACE_FILE2);
			exit(0);
		}
		fgets(bridge_ifaces,sizeof(bridge_ifaces),fp);
		bridge_ifaces[strlen(bridge_ifaces)-1]=0;// rm "\n"
		fclose(fp);
		unlink(BR_IFACE_FILE2);
		devideStringIntoItems(bridge_ifaces,IFACE_NUM_MAX,':',ifaces);
		if(strncmp(ifaces,"wlan",strlen("wlan"))!=0)
			RunSystemCmd(NULL_FILE, "ifconfig", ifaces, "down", NULL_STR);
		if( memcmp(ifaces, "br1", 3) != 0 ) 
			RunSystemCmd(NULL_FILE, "brctl", "delif", "br1" ,ifaces, NULL_STR);
	}
#endif
	

#if defined(VLAN_CONFIG_SUPPORTED) || defined(CONFIG_RTL_MULTI_LAN_DEV)
		RunSystemCmd(NULL_FILE, "ifconfig", "eth2", "down", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "eth3", "down", NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "eth4", "down", NULL_STR);
	
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", "eth2", NULL_STR);
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", "eth3", NULL_STR);
		RunSystemCmd(NULL_FILE, "brctl", "delif", "br0", "eth4", NULL_STR);
#endif
	if(setInAddr( "br0", 0,0,0, IFACE_FLAG_T)==0){
			RunSystemCmd(NULL_FILE,"brctl", "delif", "br0" ,"eth1", NULL_STR);
			RunSystemCmd(NULL_FILE,"brctl", "delif", "br0" ,"wlan0", NULL_STR);
		}

	//RunSystemCmd(NULL_FILE, "ifconfig", "br0", "down", NULL_STR);
	//RunSystemCmd(NULL_FILE, "ifconfig", "br1", "down", NULL_STR);	
	
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
	
}

#else
int clean_bridge(BASE_DATA_Tp data)
{
#if 1
	char brList[ALL_IF_NUM][IFACE_NAME_MAX]={0};
	int ifNum=0,i=0;
  //  printf("%s:%d\n",__FUNCTION__,__LINE__);

	ifNum=getBrList("br0",brList);
	for(i=0;i<ifNum;i++)
	{
		RunSystemCmd(NULL_FILE,"brctl","delif","br0",brList[i],NULL_STR);
	}
	RunSystemCmd(NULL_FILE,"brctl", "delbr", "br0" , NULL_STR);
#else
	int i=0;

	char ifName[IFACE_NUM_MAX]={0};

	BASE_DATA_T wanData={0};

	WanIntfacesType wanBindingLanPorts_orig[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};
	if(reinitFunc_get_mib_orig_value(MIB_WANIFACE_BINDING_LAN_PORTS,(void*)&wanBindingLanPorts_orig,data)<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinitFunc_get_mib_orig_value fail\n",__FUNCTION__,__LINE__);
		return -1;
	}
	for(i=0;i<ALL_IF_NUM;i++)
	{
		bzero(ifName,sizeof(ifName));
		getIfNameByIdx(i,ifName);
		if(getIfType(ifName)!=IF_TYPE_LAN)
			continue;
		if(wanBindingLanPorts_orig[i]==NO_WAN)
		{
			RunSystemCmd(NULL_FILE,"brctl","delif","br0",ifName,NULL_STR);
		}

	}
	RunSystemCmd(NULL_FILE,"brctl", "delbr", "br0" , NULL_STR);
	return 0;
#endif
}
#endif

void clean_br_stp()
{
	//seems nothing to do
}

void start_br_stp()
{
	int br_stp_enabled=0;
	apmib_get(MIB_STP_ENABLED,(void *)&br_stp_enabled);
	if(br_stp_enabled==1){
		RunSystemCmd(NULL_FILE,"brctl", "setfd", "br0", "4", NULL_STR);
		RunSystemCmd(NULL_FILE,"brctl", "stp", "br0", "1", NULL_STR);
	}else{
		RunSystemCmd(NULL_FILE,"brctl", "setfd", "br0", "0", NULL_STR);
		RunSystemCmd(NULL_FILE,"brctl", "stp", "br0", "0", NULL_STR);
	}
}


void getVlanInterface(char *vlan_interface)
{
	int intValue=0;
#if defined(VLAN_CONFIG_SUPPORTED)
	apmib_get(MIB_VLANCONFIG_ENABLED, (void *)&intValue);
	if(intValue !=0) {
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
#elif defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
		sprintf(vlan_interface, "%s %s %s %s", "eth2", "eth3", "eth4", "eth7");
#else
		sprintf(vlan_interface, "%s %s %s", "eth2", "eth3", "eth4");
#endif
	}
	else		
		memset(vlan_interface, 0x00, sizeof(vlan_interface));
	
#endif //#if defined(VLAN_CONFIG_SUPPORTED)	
}

void getWlan_interface(char * wlan_interface)
{
	int i=0;
	char tmp_args[16];
	for(i=0;i<NUM_WLAN_INTERFACE;i++)
	{
		if(wlan_interface[0]==0x00)
			sprintf(wlan_interface, "wlan%d", i);
		else{
			sprintf(tmp_args, " wlan%d", i);
			strcat(wlan_interface, tmp_args); 
		}
	}
}

void getBr_lan2_interface(char *br_lan2_interface)
{
	int opmode=-1;
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode ==BRIDGE_MODE || opmode == WISP_MODE) {
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
#else
			sprintf(br_lan2_interface, "%s", "eth1");
#endif
	}
}


void getWan_interface(char *wan_interface)
{
	int opmode=-1;
	int wisp_wan_id=0;

	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	if(opmode==WISP_MODE)
	{
		sprintf(wan_interface, "wlan%d", wisp_wan_id);
#if defined(CONFIG_SMART_REPEATER)	
		getWispRptIfaceName(wan_interface,wisp_wan_id);
#endif			
	}
	else
	{
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
		if(opmode == GATEWAY_MODE)
			sprintf(wan_interface, "%s", "eth1");
#else
		sprintf(wan_interface, "%s", "eth1");
#endif
	}
}
void get_wlan_vxd_interface(char* wlan_vxd_interface)
{
	char wan_interface[16]={0};
	int wlan_root_disabled=0;
#if defined(CONFIG_RTL_92D_SUPPORT)
	int wlan_root1_disabled=0;
#endif
	int repeater_enabled1=0,repeater_enabled2=0;
	apmib_get(MIB_REPEATER_ENABLED1,(void *)&repeater_enabled1);
	apmib_get(MIB_REPEATER_ENABLED2,(void *)&repeater_enabled2);

	getWan_interface(wan_interface);
	//if_readlist_proc(wlan_vxd_interface, "vxd", 0);
	apmib_save_wlanIdx();
	SetWlan_idx("wlan0");
	apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_root_disabled);
	if(repeater_enabled1 == 1 && !wlan_root_disabled){
#if defined(CONFIG_SMART_REPEATER)
			if(strcmp(wan_interface,"wlan0-vxd"))
#endif
			{
				if(strlen(wlan_vxd_interface) != 0)
					strcat(wlan_vxd_interface," "); 
				strcat(wlan_vxd_interface,"wlan0-vxd");
			}
		}
#if defined(CONFIG_RTL_92D_SUPPORT)
	SetWlan_idx("wlan1");
	apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_root1_disabled);
	if(repeater_enabled2 == 1 && !wlan_root1_disabled)
	{
#if defined(CONFIG_SMART_REPEATER)
		if(strcmp(wan_interface,"wlan1-vxd"))
#endif
		{

			if(strlen(wlan_vxd_interface) != 0)
				strcat(wlan_vxd_interface," "); 
			strcat(wlan_vxd_interface,"wlan1-vxd");
		}	
	}
#endif
	apmib_recov_wlanIdx();
}

void get_br_lan1_interface(char*br_lan1_interface)
{
	int opmode=-1,wan_dhcp_mode=-1;
	apmib_get(MIB_OP_MODE,(void *)&opmode);	
#ifdef   HOME_GATEWAY	
	apmib_get(MIB_WAN_DHCP,(void *)&wan_dhcp_mode);
#endif

#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
	//if(opmode != 0)
#ifdef RTK_USB3G
	if((wan_dhcp_mode == USB3G) || (opmode != 0))
		sprintf(br_lan1_interface, "%s" , "eth0");
	else
#endif
	if(opmode != GATEWAY_MODE)
		sprintf(br_lan1_interface, "%s" , "eth0");
		
#else
	sprintf(br_lan1_interface, "%s" , "eth0");
#endif
}

void get_br_interfaces(unsigned char *brif)
{

	int opmode=-1;
	int intValue=0,i=0,repeater_enabled1,repeater_enabled2,wan_dhcp_mode;
	char tmp_args[16];
	unsigned char tmpBuff[512]={0};
	char vlan_interface[32]={0};
	static char wlan_interface[16]={0};
	char wlan_virtual_interface[80]={0};
	char br_interface[16]={0};
	char br_lan1_interface[16]={0};
	char br_lan2_interface[16]={0};
	char wan_interface[16]={0};
	int wisp_wan_id=0;
	static char wlan_vxd_interface[32]={0};
#if defined(CONFIG_RTK_MESH)
	char wlan_mesh_interface[22]={0};
	int num_wlan_mesh_interface=0;
#endif



	
  	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
	

	getVlanInterface(vlan_interface);
	if(vlan_interface[0]){
		strcat(tmpBuff, vlan_interface);
		strcat(tmpBuff, " ");
	}

	
	getWlan_interface(wlan_interface);
	if(wlan_interface[0]){
		strcat(tmpBuff, wlan_interface);
		strcat(tmpBuff, " ");
	}
	
	if_readlist_proc(wlan_virtual_interface, "va", 0);
	if(wlan_virtual_interface[0]){
		strcat(tmpBuff, wlan_virtual_interface);
		strcat(tmpBuff, " ");
	}

	getBr_lan2_interface(br_lan2_interface);
#if defined(CONFIG_RTL_8198_AP_ROOT) || defined(CONFIG_RTL_8197D_AP)
	
	if(br_lan2_interface[0]){
		strcat(tmpBuff, br_lan2_interface);
		strcat(tmpBuff, " ");
	}
#endif
	
	get_wlan_vxd_interface(wlan_vxd_interface);
	reinitSer_printf(LOG_DEBUG,"%s:%d wlan_vxd_interface=%s\n",__FUNCTION__,__LINE__,wlan_vxd_interface);

	if(wlan_vxd_interface[0]){
		strcat(tmpBuff, wlan_vxd_interface);
		strcat(tmpBuff, " ");
	}
#if defined(CONFIG_RTK_MESH)
	if_readlist_proc(wlan_mesh_interface, "msh", 0);
	if(wlan_mesh_interface[0]){
		strcat(tmpBuff, wlan_mesh_interface);
		strcat(tmpBuff, " ");
	}
#endif

	get_br_interface_name(br_interface);
	strcat(tmpBuff, br_interface);
	strcat(tmpBuff, " ");
	
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	if(opmode == GATEWAY_MODE)
	{
		strcat(tmpBuff, "eth0 eth2 eth3 eth4");
	}
	else
	{
		strcat(tmpBuff, "eth0 eth1.1 eth2 eth3 eth4");
	}	
#else

	get_br_lan1_interface(br_lan1_interface);
	strcat(tmpBuff, br_lan1_interface);

#if defined(CONFIG_RTL_IVL_SUPPORT)
	/*add eth1 to br0 when in bridge&wisp mode*/
	if(opmode == BRIDGE_MODE || opmode == WISP_MODE) 
	{
		strcat(tmpBuff, " ");
		strcat(tmpBuff, br_lan2_interface);
	}
#endif
#if !defined(CONFIG_RTL_MULTI_LAN_DEV) && !defined(CONFIG_RTL_IVL_SUPPORT)
	/* rtk vlan enable and in bridge&wisp mode and disable ivl and disable multi lan, add eth1 to br0 */
	if (vlan_interface[0])
	{
		if(opmode == BRIDGE_MODE || opmode == WISP_MODE) 
		{
			strcat(tmpBuff, " ");
			strcat(tmpBuff, br_lan2_interface);
		}
	}
#endif
#endif

#if defined(CONFIG_RTL_ULINKER)
	strcat(tmpBuff, " ");
	strcat(tmpBuff, "usb0");
#endif
	memcpy(brif, tmpBuff, sizeof(tmpBuff));
	return;

}

#ifdef MULTI_WAN_SUPPORT
int set_bridge(char * brInterfaces)
{
	FILE *fp=NULL;
	int j;
	int opmode=-1;
	char bridge_iface[300], tmpBuff[200], cmdBuffer[100];
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	char bridge_iface2[300];
#endif
	char iface_name[16], tmp_iface[16];
	char *token=NULL, *savestr1=NULL;
	int intVal=0;
	int iface_index=0;
	int vlan_enabled=0, wlan_disabled=0;
	int wlan_mode=0, wisp_wan_id=0;
	int iswlan_va=0, wlan_wds_enabled=0;
	int wlan_wds_num=0, wlan0_mesh_enabled=0, wlan1_mesh_enabled=0;
	int br_stp_enabled=0, dhcp_mode=0;
	char lanIp[30], lanMask[30], lanGateway[30];
	int lan_addr,lan_mask;
	int vwlan_idx;
	char wlan_wan_iface[32]={0};
	int multiple_repeater=0;	/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND
	int multiple_repeater2=0;
#endif
#if defined(CONFIG_SMART_REPEATER)
	char vlan_netif[16]={0};
#endif

	printf("Init bridge interface...(%s)\n",brInterfaces);
	apmib_get(MIB_REPEATER_ENABLED1,(void *)&multiple_repeater);/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND
	apmib_get(MIB_REPEATER_ENABLED2,(void *)&multiple_repeater2);/*for support multiple repeater*/ 
#endif    

	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);


	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	if(isFileExist(BR_INIT_FILE)==0){//bridge init file is not exist

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br0", NULL_STR);
	}

	/*for support multiple repeater*/ 
	if(multiple_repeater==2){

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br1", NULL_STR);
	}
	/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND    
	if(multiple_repeater2==2){

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br1", NULL_STR);
	}	 
#endif
	


	memset(bridge_iface,0x00,sizeof(bridge_iface));
	token=NULL;
	savestr1=NULL;
	sprintf(tmpBuff, "%s", brInterfaces);
	
//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
	
	token = strtok_r(tmpBuff," ", &savestr1);
	do{
		if (token == NULL){/*check if the first arg is NULL*/
			break;
		}else{
			sprintf(iface_name,"%s", token);
			if(strncmp(iface_name, "eth", 3)==0){//ether iface
					intVal=1;
					if(intVal==1){
						RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,iface_name, NULL_STR);
						RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR);
						if(bridge_iface[0]){
							strcat(bridge_iface, iface_name);
							strcat(bridge_iface, ":");
						}else{
							sprintf(bridge_iface, "%s", iface_name);
							strcat(bridge_iface, ":");
						}
					}
			}
		}
		token = strtok_r(NULL, " ", &savestr1);
	}while(token !=NULL);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

	token=NULL;
	savestr1=NULL;
	sprintf(tmpBuff, "%s", brInterfaces);
	
//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
	token = strtok_r(tmpBuff," ", &savestr1);
	do{
		if(token == NULL){/*check if the first arg is NULL*/
			break;
		}else
		{
	    sprintf(iface_name,"%s", token);
	    
//printf("\r\n iface_name=[%s],__[%s-%u]\r\n",iface_name,__FILE__,__LINE__);

	    
			if(strncmp(iface_name, "wlan", 4)==0){//wlan iface
				if (strlen(iface_name) >= 9 && iface_name[5] == '-' &&
						iface_name[6] == 'v' && iface_name[7] == 'a')
				{
					char wlanRootName[16];
					memset(wlanRootName, 0x00, sizeof(wlanRootName));
					strncpy(wlanRootName,iface_name, 5);

					if(SetWlan_idx(wlanRootName)){
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get root if enable/disable
						apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get root if mode
						if(intVal==0)
						{
							if(wlan_mode != AP_MODE && wlan_mode != AP_WDS_MODE)
								wlan_disabled=1;//root if is disabled
							else
							{
								if(SetWlan_idx( iface_name))
								{
									apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get va if enable/disable
									if(intVal==0)
										wlan_disabled=0;
									else
										wlan_disabled=1;
								}else
										wlan_disabled=1;
							}
						}else
							wlan_disabled=1;
					}else
						wlan_disabled=1;//root if is disabled
				} 
                else if(strstr(iface_name, "msh")) { /*search for wlan-msh, wlan0-msh0 or wlan1-msh0*/
                    //for mesh interfaces
                    if(wlan0_mesh_enabled || wlan1_mesh_enabled)                    
                    {
                        wlan_disabled=0;
                    }
                    else {
                        wlan_disabled=1;
                    }

				} else {
					if(SetWlan_idx( iface_name)){
						vwlan_idx=atoi(&iface_name[8]);
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);
						wlan_disabled=intVal;
					}else
						wlan_disabled=1;
				}
				if(wlan_disabled==0){ //wlan if is enabled					
					sprintf(wlan_wan_iface,"wlan%d", wisp_wan_id);
#if defined(CONFIG_SMART_REPEATER)
					getWispRptIfaceName(wlan_wan_iface,wisp_wan_id);
					//strcat(wlan_wan_iface,"-vxd");
#endif					
					if(strlen(iface_name) >= 9 && iface_name[5] == '-' && iface_name[6] == 'v' && iface_name[7] == 'a')
						iswlan_va=1;

					if((iswlan_va==1) || (opmode != WISP_MODE) || (strcmp(wlan_wan_iface, iface_name) != 0) ){//do not add wlan wan  iface to br0
                        if(multiple_repeater==2 && (!strcmp(iface_name,"wlan0-va0") || !strcmp(iface_name,"wlan0-va2"))){
                            SDEBUG("    add if to br1 \n\n");                            
                            RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,iface_name, NULL_STR);     
                        }
                        #ifdef FOR_DUAL_BAND                            
                        if(multiple_repeater2==2 && (!strcmp(iface_name,"wlan1-va0") || !strcmp(iface_name,"wlan1-va2"))){
                            SDEBUG("    add if to br1 \n\n");                            
                            RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,iface_name, NULL_STR);     
                        }                        
                        #endif
                        else      
                        {
	    					RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,iface_name, NULL_STR);		
                        }
						RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR);
							if(bridge_iface[0]){
								strcat(bridge_iface, iface_name);
								strcat(bridge_iface, ":");
							}else{
								sprintf(bridge_iface, "%s", iface_name);
								strcat(bridge_iface, ":");
							}
					}
                    else{
						reinitSer_printf(LOG_ERR,"%s:%d ifconfig %s up!\n",__FUNCTION__,__LINE__,iface_name);
						//RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "up", NULL_STR);
					}

					if(SetWlan_idx( iface_name)){
						apmib_get( MIB_WLAN_WDS_ENABLED, (void *)&wlan_wds_enabled);
						apmib_get( MIB_WLAN_WDS_NUM, (void *)&wlan_wds_num);
						apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get wlan if mode
						if(wlan_wds_enabled !=0 && wlan_wds_num !=0 && (wlan_mode==WDS_MODE || wlan_mode==AP_WDS_MODE)){//add wds inface to br0
							for(j=0;j<wlan_wds_num;j++){
								sprintf(tmp_iface, "%s-wds%d", iface_name, j);
								RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,tmp_iface, NULL_STR);
								RunSystemCmd(NULL_FILE, "ifconfig", tmp_iface, "0.0.0.0", NULL_STR);
								if(bridge_iface[0]){
									strcat(bridge_iface, tmp_iface);
									strcat(bridge_iface, ":");
								}else{
									sprintf(bridge_iface, "%s", tmp_iface);
									strcat(bridge_iface, ":");
								}
							}
						}
					}
				}
				//printf("[%s] [%s] [%s]\n",iface_name,bridge_iface,bridge_iface2);
			}
		}
		token = strtok_r(NULL, " ", &savestr1);
	}while(token !=NULL);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

#if defined(CONFIG_RTL_ULINKER)
	if (opmode == 1) {
		int auto_wan;
		system("echo 1 > /proc/sw_nat");
		system("brctl addif br0 usb0 > /dev/null 2>&1");
		apmib_get(MIB_ULINKER_AUTO, (void *)&auto_wan);
#if 0
		if (auto_wan == 0)
			system("ifconfig usb0 0.0.0.0 > /dev/null 2>&1");
#endif
	}
	else {
		/* op_mode == 0, set usb0 in set_lan_dhcpd */
	}
#endif

//printf("\r\n bridge_iface=[%s],__[%s-%u]\r\n",bridge_iface,__FILE__,__LINE__);

	RunSystemCmd(BR_IFACE_FILE, "echo", bridge_iface, NULL_STR);

	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	//if(br_stp_enabled==0){/* Comment here because Enable Spanning tree and Clone Mac Address, then Apply changes, cannot access DUT .*/

	//}
	if(isFileExist(BR_INIT_FILE)==0){//bridge init file is not exist
		RunSystemCmd(NULL_FILE, "ifconfig", "br0", "0.0.0.0", NULL_STR);
		RunSystemCmd(BR_INIT_FILE, "echo", "1", NULL_STR);
	}

#if defined(CONFIG_RTL_WLAN_DOS_FILTER)
	RunSystemCmd("/proc/sw_nat", "echo", "9", NULL_STR);
#endif


	if(multiple_repeater==2 ){
		SDEBUG(" up br1 \n");
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "up", NULL_STR); 		
	}
#ifdef FOR_DUAL_BAND                            
	if(multiple_repeater2==2 ){
		SDEBUG(" up br1 \n");
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "up", NULL_STR); 		
	}
#endif
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

	return 0;
}

#else
int set_bridge(char * brInterfaces)
{
	FILE *fp=NULL;
	int j;
	int opmode=-1;
	char bridge_iface[300], tmpBuff[200], cmdBuffer[100];
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	char bridge_iface2[300];
#endif
	char iface_name[16], tmp_iface[16];
	char *token=NULL, *savestr1=NULL;
	int intVal=0;
	int iface_index=0;
	int vlan_enabled=0, wlan_disabled=0;
	int wlan_mode=0, wisp_wan_id=0;
	int iswlan_va=0, wlan_wds_enabled=0;
	int wlan_wds_num=0, wlan0_mesh_enabled=0, wlan1_mesh_enabled=0;
	int br_stp_enabled=0, dhcp_mode=0;
	char lanIp[30], lanMask[30], lanGateway[30];
	int lan_addr,lan_mask;
	int vwlan_idx;
	char wlan_wan_iface[32]={0};


#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
	int hw_bridge_num = 0;
		struct hw_vlan_setting{
			int vlan_enabled;
			int vlan_bridge;
			int vlan_tag;
			int vlan_vid;
		} rtl_hw_vlan_info[5];
		memset(rtl_hw_vlan_info, 0, 5*sizeof(struct hw_vlan_setting));
		//int rtl_hw_vlan_enable = 0;
#endif

#if defined(VLAN_CONFIG_SUPPORTED)
	VLAN_CONFIG_T vlan_entry;
	int entry_num=0;
	int i;
	int VlanisLan=0;
#endif
	int multiple_repeater=0;	/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND
	int multiple_repeater2=0;
#endif
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	int vlan_wan_enable = 0;
	int vlan_wan_tag = 0;
	int vlan_wan_bridge_enable = 0; 
	int vlan_wan_bridge_tag = 0; 
	int vlan_wan_bridge_port = 0;
	int vlan_wan_bridge_multicast_enable = 0;
	int vlan_wan_bridge_multicast_tag = 0;
	int vlan_wan_host_enable = 0;
	int vlan_wan_host_tag = 0;
	int vlan_wan_host_pri = 0;
	int vlan_wan_wifi_root_enable = 0;
	int vlan_wan_wifi_root_tag = 0;
	int vlan_wan_wifi_root_pri = 0;
	int vlan_wan_wifi_vap0_enable = 0;
	int vlan_wan_wifi_vap0_tag = 0;
	int vlan_wan_wifi_vap0_pri = 0;
	int vlan_wan_wifi_vap1_enable = 0; 
	int vlan_wan_wifi_vap1_tag = 0;
	int vlan_wan_wifi_vap1_pri = 0;
	int vlan_wan_wifi_vap2_enable = 0;
	int vlan_wan_wifi_vap2_tag = 0;
	int vlan_wan_wifi_vap2_pri = 0;
	int vlan_wan_wifi_vap3_enable = 0;
	int vlan_wan_wifi_vap3_tag = 0;
	int vlan_wan_wifi_vap3_pri = 0;

#endif
#if defined(CONFIG_SMART_REPEATER)
	char vlan_netif[16]={0};
#endif

	printf("Init bridge interface...(%s)\n",brInterfaces);
	apmib_get(MIB_REPEATER_ENABLED1,(void *)&multiple_repeater);/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND
	apmib_get(MIB_REPEATER_ENABLED2,(void *)&multiple_repeater2);/*for support multiple repeater*/ 
#endif    

	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);





	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	if(isFileExist(BR_INIT_FILE)==0){//bridge init file is not exist

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br0", NULL_STR);
	}

	/*for support multiple repeater*/ 
	if(multiple_repeater==2){

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br1", NULL_STR);
	}
	/*for support multiple repeater*/ 
#ifdef FOR_DUAL_BAND    
	if(multiple_repeater2==2){

		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br1", NULL_STR);
	}	 
#endif
	



#if defined(VLAN_CONFIG_SUPPORTED)
		apmib_get(MIB_VLANCONFIG_ENABLED,(void *)&vlan_enabled);

#if !defined(CONFIG_RTL_HW_VLAN_SUPPORT)
		if(vlan_enabled ==1 )
			RunSystemCmd("/proc/rtk_vlan_support", "echo", "1", NULL_STR);
		else
#endif
			RunSystemCmd("/proc/rtk_vlan_support", "echo", "0", NULL_STR);

		//apmib_get(MIB_VLANCONFIG_NUM,(void *)&entry_num);
		apmib_get(MIB_VLANCONFIG_TBL_NUM,(void *)&entry_num);
		//printf("*********************vlan tbl=%d,vlan_enable(%d)\n", entry_num,vlan_enabled);
		for (i=1; i<=entry_num; i++) {
			*((char *)&vlan_entry) = (char)i;
			apmib_get(MIB_VLANCONFIG_TBL, (void *)&vlan_entry);
			sprintf(tmpBuff, "/proc/%s/mib_vlan", vlan_entry.netIface);
			if(isFileExist(tmpBuff)){
#ifdef RTK_USB3G_PORT5_LAN
				DHCP_T wan_dhcp = -1;
		apmib_get( MIB_DHCP, (void *)&wan_dhcp);
#endif

				if(strncmp(vlan_entry.netIface,"eth1",strlen("eth1")) == 0){
#ifdef RTK_USB3G_PORT5_LAN
					if(opmode == WISP_MODE || opmode == BRIDGE_MODE || wan_dhcp == USB3G)
#else
					if(opmode == WISP_MODE || opmode == BRIDGE_MODE)
#endif
						VlanisLan=1;
					else
						VlanisLan=0;

#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
					sprintf(cmdBuffer,"echo \"1 %d %d %d %d %d %d 1\" > /proc/eth7/mib_vlan", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi);
					system(cmdBuffer);
					
#if !defined(CONFIG_RTL_HW_VLAN_SUPPORT)
//for pass thru
					if(vlan_enabled==1){			
						if(opmode == GATEWAY_MODE){
							sprintf(cmdBuffer,"echo \"1 %d %d %d %d %d %d %d\" > /proc/peth0/mib_vlan", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi,vlan_entry.forwarding_rule);
							system(cmdBuffer);
						}
						else if(opmode == WISP_MODE)
						{
							//to-do :for wisp mode bridge vlan
						}
					}
					else
#endif		
					{			
						if(opmode == GATEWAY_MODE){
							sprintf(cmdBuffer,"echo \"0 %d %d %d %d %d %d %d\" > /proc/peth0/mib_vlan", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi,vlan_entry.forwarding_rule);
							system(cmdBuffer);
						}
						else if(opmode == WISP_MODE)
						{
							//to-do :for wisp mode bridge vlan
						}
					}
#endif
			

				}
				else
				{
					sprintf(vlan_netif,"wlan%d",wisp_wan_id);
#if defined(CONFIG_SMART_REPEATER)
					getWispRptIfaceName(vlan_netif,wisp_wan_id);
#endif					
					if(strncmp(vlan_netif,vlan_entry.netIface, strlen(vlan_entry.netIface)) == 0)
					{
						if(opmode == WISP_MODE && (wisp_wan_id == 0 || wisp_wan_id==1))
							VlanisLan=0;
						else
							VlanisLan=1;
					}else
					{
						VlanisLan=1;
					}
				}
	#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
		#if !defined(CONFIG_RTL_HW_VLAN_SUPPORT)
				if(vlan_enabled==1){				//global_vlan, is_lan,vlan, tag, id, pri, cfi, forwarding_rule
					sprintf(cmdBuffer,"echo \"1 %d %d %d %d %d %d %d\" > %s", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi, vlan_entry.forwarding_rule ,tmpBuff);
					system(cmdBuffer);
				}
				else
		#endif
				{
					sprintf(cmdBuffer,"echo \"0 %d %d %d %d %d %d %d\" > %s", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi, vlan_entry.forwarding_rule, tmpBuff);
					system(cmdBuffer);
				}
	#else
		#if !defined(CONFIG_RTL_HW_VLAN_SUPPORT)
				if(vlan_enabled==1){				//global_vlan, is_lan,vlan, tag, id, pri, cfi
					sprintf(cmdBuffer,"echo \"1 %d %d %d %d %d %d\" > %s", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi, tmpBuff);
					system(cmdBuffer);
				}
				else
		#endif
				{
					sprintf(cmdBuffer,"echo \"0 %d %d %d %d %d %d\" > %s", VlanisLan,vlan_entry.enabled, vlan_entry.tagged, vlan_entry.vlanId, vlan_entry.priority, vlan_entry.cfi, tmpBuff);
					system(cmdBuffer);
				}
	#endif
			}

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
				if(i < 5){	   /*store lan port vlan info*/
					if(i != 1){   /*port1,2,3<-->eth2,3,4 on webpage*/
						rtl_hw_vlan_info[i].vlan_enabled = vlan_entry.enabled;
						rtl_hw_vlan_info[i].vlan_bridge = vlan_entry.forwarding_rule;
						rtl_hw_vlan_info[i].vlan_tag	  = vlan_entry.tagged;
						rtl_hw_vlan_info[i].vlan_vid	  = vlan_entry.vlanId;
					}else{	/*port0<-->eth0 on webpage*/
						rtl_hw_vlan_info[0].vlan_enabled = vlan_entry.enabled;
						rtl_hw_vlan_info[0].vlan_bridge = vlan_entry.forwarding_rule;
						rtl_hw_vlan_info[0].vlan_tag	  = vlan_entry.tagged;
						rtl_hw_vlan_info[0].vlan_vid	  = vlan_entry.vlanId;
					}
				}else if(strncmp(vlan_entry.netIface, "eth1", strlen("eth1")) == 0){ /*wan port*/
					rtl_hw_vlan_info[1].vlan_enabled = vlan_entry.enabled;
					rtl_hw_vlan_info[1].vlan_vid = vlan_entry.vlanId;
					rtl_hw_vlan_info[1].vlan_tag = vlan_entry.tagged;
				}
#endif

		}
#endif

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
	for(i=0; i<5; i++)
	{
		if((rtl_hw_vlan_info[i].vlan_enabled == 1)&&(rtl_hw_vlan_info[i].vlan_bridge == 1))
			hw_bridge_num ++;
	}
	//apmib_get(MIB_HW_VLANCONFIG_ENABLED, (void *)&rtl_hw_vlan_enable);
	sprintf(tmpBuff, "/proc/rtl_hw_vlan_support");
	sprintf(cmdBuffer,"echo \"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\" > %s", \
		vlan_enabled, rtl_hw_vlan_info[1].vlan_enabled, rtl_hw_vlan_info[1].vlan_tag, rtl_hw_vlan_info[1].vlan_vid,\
		rtl_hw_vlan_info[0].vlan_enabled, rtl_hw_vlan_info[0].vlan_bridge, rtl_hw_vlan_info[0].vlan_tag, rtl_hw_vlan_info[0].vlan_vid,\
		rtl_hw_vlan_info[2].vlan_enabled, rtl_hw_vlan_info[2].vlan_bridge, rtl_hw_vlan_info[2].vlan_tag, rtl_hw_vlan_info[2].vlan_vid,\
		rtl_hw_vlan_info[3].vlan_enabled, rtl_hw_vlan_info[3].vlan_bridge, rtl_hw_vlan_info[3].vlan_tag, rtl_hw_vlan_info[3].vlan_vid,\
		rtl_hw_vlan_info[4].vlan_enabled, rtl_hw_vlan_info[4].vlan_bridge, rtl_hw_vlan_info[4].vlan_tag, rtl_hw_vlan_info[4].vlan_vid,\
		tmpBuff);
	system(cmdBuffer);
#endif


#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT	
	if(opmode == GATEWAY_MODE)
	{
		apmib_get( MIB_VLAN_WAN_ENALE, (void *)&vlan_wan_enable);
		apmib_get( MIB_VLAN_WAN_TAG, (void *)&vlan_wan_tag);
		apmib_get( MIB_VLAN_WAN_BRIDGE_ENABLE, (void *)&vlan_wan_bridge_enable);
		apmib_get( MIB_VLAN_WAN_BRIDGE_TAG, (void *)&vlan_wan_bridge_tag);
		apmib_get( MIB_VLAN_WAN_BRIDGE_PORT, (void *)&vlan_wan_bridge_port);
		apmib_get( MIB_VLAN_WAN_BRIDGE_MULTICAST_ENABLE, (void *)&vlan_wan_bridge_multicast_enable);
		apmib_get( MIB_VLAN_WAN_BRIDGE_MULTICAST_TAG, (void *)&vlan_wan_bridge_multicast_tag);
		apmib_get( MIB_VLAN_WAN_HOST_ENABLE, (void *)&vlan_wan_host_enable);
		apmib_get( MIB_VLAN_WAN_HOST_TAG, (void *)&vlan_wan_host_tag);
		apmib_get( MIB_VLAN_WAN_HOST_PRI, (void *)&vlan_wan_host_pri);
		apmib_get( MIB_VLAN_WAN_WIFI_ROOT_ENABLE, (void *)&vlan_wan_wifi_root_enable);
		apmib_get( MIB_VLAN_WAN_WIFI_ROOT_TAG, (void *)&vlan_wan_wifi_root_tag);
		apmib_get( MIB_VLAN_WAN_WIFI_ROOT_PRI, (void *)&vlan_wan_wifi_root_pri);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP0_ENABLE, (void *)&vlan_wan_wifi_vap0_enable);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP0_TAG, (void *)&vlan_wan_wifi_vap0_tag);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP0_PRI, (void *)&vlan_wan_wifi_vap0_pri);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP1_ENABLE, (void *)&vlan_wan_wifi_vap1_enable);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP1_TAG, (void *)&vlan_wan_wifi_vap1_tag);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP1_PRI, (void *)&vlan_wan_wifi_vap1_pri);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP2_ENABLE, (void *)&vlan_wan_wifi_vap2_enable);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP2_TAG, (void *)&vlan_wan_wifi_vap2_tag);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP2_PRI, (void *)&vlan_wan_wifi_vap2_pri);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP3_ENABLE, (void *)&vlan_wan_wifi_vap3_enable);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP3_TAG, (void *)&vlan_wan_wifi_vap3_tag);
		apmib_get( MIB_VLAN_WAN_WIFI_VAP3_PRI, (void *)&vlan_wan_wifi_vap3_pri);	

	sprintf(cmdBuffer,"echo \"%d %d %d %d %d %d %d\" > /proc/rtk_vlan_wan_tag", vlan_wan_enable, vlan_wan_tag, 
		vlan_wan_bridge_enable, vlan_wan_bridge_tag, vlan_wan_bridge_port,vlan_wan_bridge_multicast_enable,vlan_wan_bridge_multicast_tag);

		system(cmdBuffer);

	  sprintf(cmdBuffer,"echo \"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\" > /proc/rtk_vlan_wan_tag_cpu", 
		vlan_wan_host_enable,	   vlan_wan_host_tag,	   vlan_wan_host_pri, 
	  vlan_wan_wifi_root_enable, vlan_wan_wifi_root_tag,	  vlan_wan_wifi_root_pri, 
	  vlan_wan_wifi_vap0_enable, vlan_wan_wifi_vap0_tag, vlan_wan_wifi_vap0_pri,
	  vlan_wan_wifi_vap1_enable, vlan_wan_wifi_vap1_tag, vlan_wan_wifi_vap1_pri,
	  vlan_wan_wifi_vap2_enable, vlan_wan_wifi_vap2_tag, vlan_wan_wifi_vap2_pri,
	  vlan_wan_wifi_vap3_enable, vlan_wan_wifi_vap3_tag, vlan_wan_wifi_vap3_pri);
		system(cmdBuffer);
	}
#endif

	memset(bridge_iface,0x00,sizeof(bridge_iface));
	token=NULL;
	savestr1=NULL;
	sprintf(tmpBuff, "%s", brInterfaces);
	
//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
	
	token = strtok_r(tmpBuff," ", &savestr1);
	do{
		if (token == NULL){/*check if the first arg is NULL*/
			break;
		}else{
			sprintf(iface_name,"%s", token);
			
//printf("\r\n iface_name=[%s],__[%s-%u]\r\n",iface_name,__FILE__,__LINE__);
			
			if(strncmp(iface_name, "eth", 3)==0){//ether iface
				intVal=1;
//				if(isFileExist(ETH_VLAN_SWITCH)){
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
#else
				if(
#if !defined(CONFIG_RTL_HW_VLAN_SUPPORT)
					(vlan_enabled==0)&&
#endif
					(iface_name[3]=='2' || iface_name[3]=='3' || iface_name[3]=='4'
#if defined(CONFIG_RTK_BRIDGE_VLAN_SUPPORT)
					|| iface_name[3]=='7'
#endif
					)
					)
					{
						intVal=0;
					}
#endif
//				}

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
					switch(hw_bridge_num)
					{
						case 1:
							if(iface_name[3]=='2')
								intVal = 1;
						break;
						case 2:
							if(iface_name[3]=='2' || iface_name[3]=='3')
								intVal = 1;
						break;
						case 3:
							if(iface_name[3]=='2' || iface_name[3]=='3' || iface_name[3]=='4')
								intVal = 1;
						break;
						default:
						break;
					}
#endif

					if(intVal==1){
//printf("add iface to br %s\n", iface_name);
						RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,iface_name, NULL_STR);
						RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR);
						if(bridge_iface[0]){
							strcat(bridge_iface, iface_name);
							strcat(bridge_iface, ":");
						}else{
							sprintf(bridge_iface, "%s", iface_name);
							strcat(bridge_iface, ":");
						}
					}
			}
		}
		token = strtok_r(NULL, " ", &savestr1);
	}while(token !=NULL);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	
	memset(bridge_iface2,0x00,sizeof(bridge_iface2));
  
  if(opmode == GATEWAY_MODE && vlan_wan_bridge_enable)
	{
		RunSystemCmd(NULL_FILE, "brctl", "addbr", "br1", NULL_STR);
		RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,"eth2", NULL_STR); 	
		RunSystemCmd(NULL_FILE, "ifconfig", "eth2", "0.0.0.0", NULL_STR); 
		sprintf(bridge_iface2, "%s", "eth2");
		strcat(bridge_iface2, ":");
	}
	else
	{
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "down", NULL_STR); 
		RunSystemCmd(NULL_FILE, "brctl", "delbr", "br1", NULL_STR);
	}
#endif	

	token=NULL;
	savestr1=NULL;
	sprintf(tmpBuff, "%s", brInterfaces);
	
//printf("\r\n tmpBuff=[%s],__[%s-%u]\r\n",tmpBuff,__FILE__,__LINE__);
	token = strtok_r(tmpBuff," ", &savestr1);
	do{
		if(token == NULL){/*check if the first arg is NULL*/
			break;
		}else
		{
	    sprintf(iface_name,"%s", token);
	    
//printf("\r\n iface_name=[%s],__[%s-%u]\r\n",iface_name,__FILE__,__LINE__);

	    
			if(strncmp(iface_name, "wlan", 4)==0){//wlan iface
				if (strlen(iface_name) >= 9 && iface_name[5] == '-' &&
						iface_name[6] == 'v' && iface_name[7] == 'a')
				{
					char wlanRootName[16];
					memset(wlanRootName, 0x00, sizeof(wlanRootName));
					strncpy(wlanRootName,iface_name, 5);

					if(SetWlan_idx(wlanRootName)){
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get root if enable/disable
						apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get root if mode
						if(intVal==0)
						{
							if(wlan_mode != AP_MODE && wlan_mode != AP_WDS_MODE)
								wlan_disabled=1;//root if is disabled
							else
							{
								if(SetWlan_idx( iface_name))
								{
									apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);//get va if enable/disable
									if(intVal==0)
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
									{
										wlan_disabled=0;
										if(opmode == GATEWAY_MODE && vlan_wan_bridge_enable)
										{
											wlan_disabled=0;
									
											if(vwlan_idx)//vap
											{
                      	if(vlan_wan_bridge_port&(1<<(6+vwlan_idx)))
													wlan_disabled=2;
											}
										}
									}
#else
										wlan_disabled=0;
#endif
									else
										wlan_disabled=1;
								}else
										wlan_disabled=1;
							}
						}else
							wlan_disabled=1;
					}else
						wlan_disabled=1;//root if is disabled
				} 
                else if(strstr(iface_name, "msh")) { /*search for wlan-msh, wlan0-msh0 or wlan1-msh0*/
                    //for mesh interfaces
                    if(wlan0_mesh_enabled || wlan1_mesh_enabled)                    
                    {
                        wlan_disabled=0;
                    }
                    else {
                        wlan_disabled=1;
                    }

				} else {
					if(SetWlan_idx( iface_name)){
						vwlan_idx=atoi(&iface_name[8]);
						apmib_get( MIB_WLAN_WLAN_DISABLED, (void *)&intVal);
						wlan_disabled=intVal;
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
            if(opmode == GATEWAY_MODE && vlan_wan_bridge_enable)
						{
							if(vlan_wan_bridge_port&(1<<(6+vwlan_idx)))
								wlan_disabled=2;
						}
#endif
					}else
						wlan_disabled=1;
				}
				if(wlan_disabled==0){ //wlan if is enabled					
					sprintf(wlan_wan_iface,"wlan%d", wisp_wan_id);
#if defined(CONFIG_SMART_REPEATER)
					getWispRptIfaceName(wlan_wan_iface,wisp_wan_id);
					//strcat(wlan_wan_iface,"-vxd");
#endif					
					if(strlen(iface_name) >= 9 && iface_name[5] == '-' && iface_name[6] == 'v' && iface_name[7] == 'a')
						iswlan_va=1;

					if((iswlan_va==1) || (opmode != WISP_MODE) || (strcmp(wlan_wan_iface, iface_name) != 0) ){//do not add wlan wan  iface to br0
                        if(multiple_repeater==2 && (!strcmp(iface_name,"wlan0-va0") || !strcmp(iface_name,"wlan0-va2"))){
                            SDEBUG("    add if to br1 \n\n");                            
                            RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,iface_name, NULL_STR);     
                        }
                        #ifdef FOR_DUAL_BAND                            
                        if(multiple_repeater2==2 && (!strcmp(iface_name,"wlan1-va0") || !strcmp(iface_name,"wlan1-va2"))){
                            SDEBUG("    add if to br1 \n\n");                            
                            RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,iface_name, NULL_STR);     
                        }                        
                        #endif
                        else      
                        {
	    					RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,iface_name, NULL_STR);		
                        }
						RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR);
							if(bridge_iface[0]){
								strcat(bridge_iface, iface_name);
								strcat(bridge_iface, ":");
							}else{
								sprintf(bridge_iface, "%s", iface_name);
								strcat(bridge_iface, ":");
							}
					}
                    else{
						reinitSer_printf(LOG_ERR,"%s:%d ifconfig %s up!\n",__FUNCTION__,__LINE__,iface_name);
						//RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "up", NULL_STR);
					}

					if(SetWlan_idx( iface_name)){
						apmib_get( MIB_WLAN_WDS_ENABLED, (void *)&wlan_wds_enabled);
						apmib_get( MIB_WLAN_WDS_NUM, (void *)&wlan_wds_num);
						apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get wlan if mode
						if(wlan_wds_enabled !=0 && wlan_wds_num !=0 && (wlan_mode==WDS_MODE || wlan_mode==AP_WDS_MODE)){//add wds inface to br0
							for(j=0;j<wlan_wds_num;j++){
								sprintf(tmp_iface, "%s-wds%d", iface_name, j);
								RunSystemCmd(NULL_FILE, "brctl", "addif", "br0" ,tmp_iface, NULL_STR);
								RunSystemCmd(NULL_FILE, "ifconfig", tmp_iface, "0.0.0.0", NULL_STR);
								if(bridge_iface[0]){
									strcat(bridge_iface, tmp_iface);
									strcat(bridge_iface, ":");
								}else{
									sprintf(bridge_iface, "%s", tmp_iface);
									strcat(bridge_iface, ":");
								}
							}
						}
					}
				}
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
				else if(wlan_disabled==2)//only in gateway mode
				{
					RunSystemCmd(NULL_FILE, "brctl", "addif", "br1" ,iface_name, NULL_STR);		
					RunSystemCmd(NULL_FILE, "ifconfig", iface_name, "0.0.0.0", NULL_STR); 
					if(bridge_iface[0])
					{
						strcat(bridge_iface2, iface_name);
						strcat(bridge_iface2, ":");
					}
					else
					{
							sprintf(bridge_iface2, "%s", iface_name);
							strcat(bridge_iface2, ":");
					}
				}
#endif
				//printf("[%s] [%s] [%s]\n",iface_name,bridge_iface,bridge_iface2);
			}
		}
		token = strtok_r(NULL, " ", &savestr1);
	}while(token !=NULL);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

#if defined(CONFIG_RTL_ULINKER)
	if (opmode == 1) {
		int auto_wan;
		system("echo 1 > /proc/sw_nat");
		system("brctl addif br0 usb0 > /dev/null 2>&1");
		apmib_get(MIB_ULINKER_AUTO, (void *)&auto_wan);
#if 0
		if (auto_wan == 0)
			system("ifconfig usb0 0.0.0.0 > /dev/null 2>&1");
#endif
	}
	else {
		/* op_mode == 0, set usb0 in set_lan_dhcpd */
	}
#endif

//printf("\r\n bridge_iface=[%s],__[%s-%u]\r\n",bridge_iface,__FILE__,__LINE__);

	RunSystemCmd(BR_IFACE_FILE, "echo", bridge_iface, NULL_STR);

#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
  if(opmode == GATEWAY_MODE && vlan_wan_bridge_enable)
	{
		RunSystemCmd(BR_IFACE_FILE2, "echo", bridge_iface2, NULL_STR);
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "up", NULL_STR); 
	}
#endif
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	//if(br_stp_enabled==0){/* Comment here because Enable Spanning tree and Clone Mac Address, then Apply changes, cannot access DUT .*/

	//}
	if(isFileExist(BR_INIT_FILE)==0){//bridge init file is not exist
		RunSystemCmd(NULL_FILE, "ifconfig", "br0", "0.0.0.0", NULL_STR);
		RunSystemCmd(BR_INIT_FILE, "echo", "1", NULL_STR);
	}

#if defined(CONFIG_RTL_WLAN_DOS_FILTER)
	RunSystemCmd("/proc/sw_nat", "echo", "9", NULL_STR);
#endif


	if(multiple_repeater==2 ){
		SDEBUG(" up br1 \n");
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "up", NULL_STR); 		
	}
#ifdef FOR_DUAL_BAND                            
	if(multiple_repeater2==2 ){
		SDEBUG(" up br1 \n");
		RunSystemCmd(NULL_FILE, "ifconfig", "br1", "up", NULL_STR); 		
	}
#endif
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

	return 0;
}
#endif

#ifndef MULTI_WAN_SUPPORT
int start_bridge()
{
#if 0
	start_multiple_repeater();
	if(isFileExist(BR_INIT_FILE)==0){//bridge init file is not exist
		RunSystemCmd(NULL_FILE,"brctl", "addbr", "br0", NULL_STR);
	}

	start_br_stp();
	start_vlan();
#endif
	char brInterfaces[512]={0};

	RunSystemCmd(PROC_BR_IGMPPROXY, "echo", "0", NULL_STR);

	get_br_interfaces(brInterfaces);
	reinitSer_printf(LOG_DEBUG,"%s:%d brInterfaces=%s\n",__FUNCTION__,__LINE__,brInterfaces);
	set_bridge(brInterfaces);
		
}
#else
int start_bridge()
{
	int i=0,wanEnabled=0,wanType=0;
	char ifName[IFACE_NAME_MAX]={0};
	BASE_DATA_T wanData={0};
	char brInterfaces[512]={0};
	WanIntfacesType wanBindingLanPorts[WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM]={0};

	if(!apmib_get(MIB_WANIFACE_BINDING_LAN_PORTS, (void *)&wanBindingLanPorts))
	{
		reinitSer_printf(LOG_ERR,"%s:%d get MIB_WANIFACE_BINDING_LAN_PORTS fail!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	RunSystemCmd(NULL_FILE,"brctl", "addbr", "br0" , NULL_STR);
	RunSystemCmd(NULL_FILE,"ifconfig", "eth0", "up" , NULL_STR);
	RunSystemCmd(NULL_FILE,"ifconfig", "eth1", "up" , NULL_STR);
	RunSystemCmd(NULL_FILE,"ifconfig", "eth2", "up" , NULL_STR);
	RunSystemCmd(NULL_FILE,"ifconfig", "eth3", "up" , NULL_STR);
	RunSystemCmd(NULL_FILE,"ifconfig", "eth4", "up" , NULL_STR);

	#if 1
	for(i=0;i<ALL_IF_NUM;i++)
	{
		bzero(ifName,sizeof(ifName));
		getIfNameByIdx(i,ifName);
		if(getIfType(ifName)!=IF_TYPE_LAN)
			continue;
		//try to add all iface to br0
			//for wan bridge init, init wan will add it to wanbr
			//for reinit, wan bridge already in wanbr and will not add success
	//	if(wanBindingLanPorts[i]==NO_WAN)
		{
			RunSystemCmd(NULL_FILE,"brctl","addif","br0",ifName,NULL_STR);
		}
	}
	
	#else
	get_br_interfaces(brInterfaces);
	set_bridge(brInterfaces);
	#endif

}
#endif
int set_static_lan()
{
	char br_interface[16]={0};
	char Ip[32], Mask[32], Gateway[32];
	int lan_addr=0,lan_mask=0,lan_gw=0;
	get_br_interface_name(br_interface);
	apmib_get( MIB_IP_ADDR,  (void *)&lan_addr);
	sprintf(Ip, "%s", inet_ntoa(*((struct in_addr *)&lan_addr)));
	apmib_get( MIB_SUBNET_MASK,  (void *)&lan_mask);
	sprintf(Mask, "%s", inet_ntoa(*((struct in_addr *)&lan_mask)));
	apmib_get(MIB_DEFAULT_GATEWAY,  (void *)&lan_gw);
	
	if (!memcmp(&lan_gw, "\x0\x0\x0\x0", 4))
		memset(Gateway, 0x00, sizeof(Gateway));
	else
		sprintf(Gateway, "%s", inet_ntoa(*((struct in_addr *)&lan_gw)));
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	RunSystemCmd(NULL_FILE, "ifconfig", br_interface, Ip, "netmask", Mask, NULL_STR);

	lan_addr &= lan_mask;
	sprintf(Ip,"%s",inet_ntoa(*((struct in_addr *)(&lan_addr))));
		RunSystemCmd(NULL_FILE, "route", "del", "-net",Ip, "netmask",Mask, NULL_STR);
		RunSystemCmd(NULL_FILE, "route", "add", "-net",Ip, "netmask",Mask, "dev",br_interface,NULL_STR);
		//end hyking added
	if(Gateway[0]){
		RunSystemCmd(NULL_FILE, "route", "del", "default", br_interface, NULL_STR);
		RunSystemCmd(NULL_FILE, "route", "add", "-net", "default", "gw", Gateway, "dev", br_interface, NULL_STR);
	}	
}
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
int create_dhcpd_configfile(char *interface)
{	
	DHCPS_SERVING_POOL_T entry;
	int entrynum, i;
	int k=1, count=0;
	char line_buffer[512]={0};

	int j, entry_Num, enabled;
	DHCPRSVDIP_T sdhcp_entry;
	
	if(!apmib_get(MIB_DHCPS_SERVING_POOL_TBL_NUM, (void *)&entrynum))
	{
		printf("%s:%d apmib_get MIB_DHCPS_SERVING_POOL_TBL_NUM fails!####\n",__FUNCTION__,__LINE__);
		return 0;
	}
	for (i=1; i<=entrynum; i++) 
	{
		*((char *)&entry) = (char)i;
		if(!apmib_get(MIB_DHCPS_SERVING_POOL_TBL, (void *)&entry))
			continue;		

		if(entry.enable==0)
			continue;

		if(!(entry.startaddr>0 && entry.endaddr>0 && entry.endaddr>entry.startaddr))
			continue;

		count++;
		
		if(strlen(entry.poolname)>0)
			sprintf(line_buffer,"poolname %s\n", entry.poolname);
		else
			sprintf(line_buffer,"poolname pool%d\n", count);
		write_line_to_file(DHCPD_CONF_FILE, k, line_buffer);

		if(entry.poolorder>0)
		{
			sprintf(line_buffer,"poolorder %d\n", entry.poolorder);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.sourceinterface>0)
		{
			sprintf(line_buffer,"sourceinterface %d\n", entry.sourceinterface);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.vendorclass)>0)
		{
			sprintf(line_buffer,"vendorclass %s\n", entry.vendorclass);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.vendorclassflag>0)
		{
			sprintf(line_buffer,"vendorclassflag %d\n", entry.vendorclassflag);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}		
		if(strlen(entry.vendorclassmode)>0)
		{
			sprintf(line_buffer,"vendorclassmode %s\n", entry.vendorclassmode);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.clientid)>0)
		{
			sprintf(line_buffer,"clientid %s\n", entry.clientid);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.clientidflag>0)
		{
			sprintf(line_buffer,"clientidflag %d\n", entry.clientidflag);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.userclass)>0)
		{
			sprintf(line_buffer,"userclass %s\n", entry.userclass);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.userclassflag>0)
		{
			sprintf(line_buffer,"userclassflag %d\n", entry.userclassflag);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.chaddr)>0)
		{
			sprintf(line_buffer,"chaddr %s\n", entry.chaddr);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.chaddrmask)>0)
		{
			sprintf(line_buffer,"chaddrmask %s\n", entry.chaddrmask);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.chaddrflag>0)
		{
			sprintf(line_buffer,"chaddrflag %d\n", entry.chaddrflag);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}

		sprintf(line_buffer,"interface %s\n", interface);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
		sprintf(line_buffer,"start %s\n", inet_ntoa(*((struct in_addr *)&entry.startaddr)));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
		sprintf(line_buffer,"end %s\n", inet_ntoa(*((struct in_addr *)&entry.endaddr)));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		if(entry.subnetmask>0)
		{
			sprintf(line_buffer,"opt subnet %s\n", inet_ntoa(*((struct in_addr *)&entry.subnetmask)));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.iprouter>0)
		{
			sprintf(line_buffer,"opt router %s\n", inet_ntoa(*((struct in_addr *)&entry.iprouter)));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.dnsserver1>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(*((struct in_addr *)&entry.dnsserver1)));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.dnsserver2>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(*((struct in_addr *)&entry.dnsserver2)));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.dnsserver3>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(*((struct in_addr *)&entry.dnsserver3)));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(strlen(entry.domainname)>0)
		{
			sprintf(line_buffer,"opt domain %s\n", entry.domainname);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		if(entry.leasetime>0)
		{
			sprintf(line_buffer,"opt lease %u\n", entry.leasetime*60);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}	

		/*static dhcp entry static_lease 000102030405 192.168.1.199*/
		apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&enabled);
		if(enabled==1)
		{
			apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
			if(entry_Num>0){
				for (j=1; j<=entry_Num; j++) 
				{
					*((char *)&sdhcp_entry) = (char)j;
					apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&sdhcp_entry);
					if(sdhcp_entry.dhcpRsvdIpEntryEnabled==0)
						continue;
					sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", sdhcp_entry.macAddr[0], sdhcp_entry.macAddr[1], sdhcp_entry.macAddr[2],
					sdhcp_entry.macAddr[3], sdhcp_entry.macAddr[4], sdhcp_entry.macAddr[5], inet_ntoa(*((struct in_addr*)sdhcp_entry.ipAddr)));
					write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
				}
			}
		}
		/*end static dhcp entry*/
		
		sprintf(line_buffer,"poolend \n");
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);	
		k=2;		
	}
	return count;
}

void create_dhcpd_default_configfile(char *interface, int mode, int pool_num)
{	
	unsigned int intValue;
	char line_buffer[128]={0};	
	struct in_addr addr;
	int dns_mode=0;
	char tmp_buff[64]={0};
	
	int i, entry_Num, write_mode;
	DHCPRSVDIP_T entry;

	write_mode=((pool_num>0) ? 2 : 1);
	
	sprintf(line_buffer,"poolname pool-default\n");
	write_line_to_file(DHCPD_CONF_FILE, write_mode, line_buffer);

	sprintf(line_buffer,"poolorder 0XFFFFFFFF\n");
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);		

	sprintf(line_buffer,"interface %s\n", interface);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	
	apmib_get(MIB_DHCP_CLIENT_START,  (void *)&addr);	
	sprintf(line_buffer,"start %s\n", inet_ntoa(addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_END,  (void *)&addr);
	sprintf(line_buffer,"end %s\n", inet_ntoa(addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_SUBNET_MASK,  (void *)&addr);
	sprintf(line_buffer,"opt subnet %s\n", inet_ntoa(addr));
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	
	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&intValue);
    	if((intValue==0) || (intValue<0) || (intValue>10080))
		intValue = 480; //8 hours
	intValue *= 60;
    	sprintf(line_buffer,"opt lease %ld\n",intValue);
    	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	if(mode==1)
	{//ap
		apmib_get(MIB_DEFAULT_GATEWAY,  (void *)&addr);
		if (addr.s_addr>0)
		{			
			sprintf(line_buffer,"opt router %s\n", inet_ntoa(addr));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		else
		{
			apmib_get(MIB_IP_ADDR, (void *)&addr);			
			sprintf(line_buffer,"opt router %s\n", inet_ntoa(addr));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
	}
	else
	{
		apmib_get(MIB_IP_ADDR, (void *)&addr);			
		sprintf(line_buffer,"opt router %s\n", inet_ntoa(addr));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}

	apmib_get( MIB_DNS_MODE, (void *)&dns_mode);
	if(dns_mode==0)
	{		
		sprintf(line_buffer,"opt dns %s\n", inet_ntoa(addr));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
	else
	{
		apmib_get(MIB_DNS1, (void *)&addr);
		if(addr.s_addr>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(addr));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		apmib_get(MIB_DNS2, (void *)&addr);
		if(addr.s_addr>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(addr));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		apmib_get(MIB_DNS3, (void *)&addr);
		if(addr.s_addr>0)
		{
			sprintf(line_buffer,"opt dns %s\n", inet_ntoa(addr));
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
	}
	
	apmib_get( MIB_DOMAIN_NAME, (void *)tmp_buff);
	if(tmp_buff[0])
	{
		sprintf(line_buffer,"opt domain %s\n",tmp_buff);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}	

	/*static dhcp entry static_lease 000102030405 192.168.1.199*/
	apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
	if(intValue==1){
		apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
		if(entry_Num>0){
			for (i=1; i<=entry_Num; i++) {
				*((char *)&entry) = (char)i;
				apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry);
				if(entry.dhcpRsvdIpEntryEnabled==0)
					continue;
				sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], inet_ntoa(*((struct in_addr*)entry.ipAddr)));
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			}
		}
	}
	/*end static dhcp entry*/
	
	sprintf(line_buffer,"poolend \n");
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);	
}
#endif

void set_lan_dhcpd()
{
	char tmpBuff1[32]={0}, tmpBuff2[32]={0};
	int intValue=0, dns_mode=0;
	char line_buffer[100]={0};
	char tmp1[64]={0};
	char tmp2[64]={0};
	char interface[32]={0};
	char *strtmp=NULL, *strtmp1=NULL;
	DHCPRSVDIP_T entry;
	int i, entry_Num=0, ret_val=0;

	unsigned int lan_ip, lan_netmask, reserve_ip;
	
#ifdef   HOME_GATEWAY
	char tmpBuff3[32]={0};
#endif

	get_br_interface_name(interface);
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	ret_val=create_dhcpd_configfile(interface);
	//dhcpd default config coexist with pool config, and place the default config at the end of dhcpd config file
	//if(ret_val<1)
	create_dhcpd_default_configfile(interface,2,ret_val);
#else	
	
	sprintf(line_buffer,"interface %s\n",interface);
	write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_START,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"start %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_CLIENT_END,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"end %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_SUBNET_MASK,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"opt subnet %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	apmib_get(MIB_DHCP_LEASE_TIME, (void *)&intValue);
    if( (intValue==0) || (intValue<0) || (intValue>10080))
    {
		intValue = 480; //8 hours
		
    }
	intValue *= 60;
    sprintf(line_buffer,"opt lease %ld\n",intValue);
    write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

	
	apmib_get(MIB_IP_ADDR,  (void *)tmp1);
	strtmp= inet_ntoa(*((struct in_addr *)tmp1));
	sprintf(line_buffer,"opt router %s\n",strtmp);
	write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#ifdef   HOME_GATEWAY
	apmib_get( MIB_DNS_MODE, (void *)&dns_mode);
	if(dns_mode==0){
		sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
#endif
	

	if(dns_mode==1)
	{
#if defined(HOME_GATEWAY) && !(defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_RTL_ULINKER))
		apmib_get( MIB_DNS1,  (void *)tmpBuff1);
		apmib_get( MIB_DNS2,  (void *)tmpBuff2);
		apmib_get( MIB_DNS3,  (void *)tmpBuff3);

		if (memcmp(tmpBuff1, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff1));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff2, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff2));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
		if (memcmp(tmpBuff3, "\x0\x0\x0\x0", 4)){
			strtmp= inet_ntoa(*((struct in_addr *)tmpBuff3));
			sprintf(line_buffer,"opt dns %s\n",strtmp);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			intValue++;
		}
#endif

#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT) || defined(CONFIG_RTL_ULINKER)
		apmib_get(MIB_IP_ADDR,  (void *)tmp1);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(line_buffer,"opt dns %s\n",strtmp); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
#endif //#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT

		if(intValue==0)
		{ /*no dns option for dhcp server, use default gatewayfor dns opt*/
			apmib_get( MIB_IP_ADDR,  (void *)tmp2);
			if (memcmp(tmp2, "\x0\x0\x0\x0", 4)){
				strtmp= inet_ntoa(*((struct in_addr *)tmp2));
				sprintf(line_buffer,"opt dns %s\n",strtmp);
				write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
			}			
		}
	}
	memset(tmp1, 0x00, 64);
	apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
	if(tmp1[0]){
		sprintf(line_buffer,"opt domain %s\n",tmp1);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	}
/*static dhcp entry static_lease 000102030405 192.168.1.199*/

	intValue=0;
    apmib_get(MIB_DHCPRSVDIP_ENABLED, (void *)&intValue);
    if(intValue==1){
        apmib_get(MIB_DHCPRSVDIP_TBL_NUM, (void *)&entry_Num);
        if(entry_Num>0){
            apmib_get(MIB_IP_ADDR, (void *)&lan_ip);
            apmib_get(MIB_SUBNET_MASK, (void *)&lan_netmask);
            for (i=1; i<=entry_Num; i++) {
                *((char *)&entry) = (char)i;
                apmib_get(MIB_DHCPRSVDIP_TBL, (void *)&entry);
                memcpy(&reserve_ip, entry.ipAddr, 4);
                if((reserve_ip & lan_netmask) != (lan_ip & lan_netmask))
                {
                    apmib_set(MIB_DHCPRSVDIP_DEL, (void *)&entry);
                    reserve_ip=((~lan_netmask) & reserve_ip) | (lan_ip & lan_netmask);
                    memcpy(entry.ipAddr, &reserve_ip, 4);
                    apmib_set(MIB_DHCPRSVDIP_ADD, (void *)&entry);
                }
                sprintf(line_buffer, "static_lease %02x%02x%02x%02x%02x%02x %s\n", entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
                entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], inet_ntoa(*((struct in_addr*)entry.ipAddr)));
                write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
            }
        }
    }
	
#endif
	
	/*start dhcp server*/
	#if defined(CONFIG_AUTO_DHCP_CHECK)
		int opmode;
		apmib_get(MIB_OP_MODE,(void *)&opmode);
		if(opmode != BRIDGE_MODE)
		{
			char tmpBuff4[100];
			sprintf(tmpBuff4,"udhcpd %s\n",DHCPD_CONF_FILE);
			system(tmpBuff4);
		}
	#else
		char tmpBuff4[100];
		sprintf(tmpBuff4,"udhcpd %s\n",DHCPD_CONF_FILE);
		system(tmpBuff4);
	#endif
	//RunSystemCmd(stdout, "udhcpd", DHCPD_CONF_FILE, NULL_STR);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);

#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("echo 1 > /var/sc_ip_status");
#endif

}
void set_lan_dhcpc(char *iface)
{
	char script_file[100], deconfig_script[100], pid_file[100];
	char *strtmp=NULL;
	char tmp[32], Ip[32], Mask[32], Gateway[32];
	char cmdBuff[200];
#ifdef  HOME_GATEWAY
	int intValue=0;
#endif
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", iface);/*deconfig script path*/
	sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", iface); /*pid path*/
	apmib_get( MIB_IP_ADDR,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Ip, "%s",strtmp);

	apmib_get( MIB_SUBNET_MASK,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Mask, "%s",strtmp);

	apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Gateway, "%s",strtmp);

	Create_script(deconfig_script, iface, LAN_NETWORK, Ip, Mask, Gateway);

	//RunSystemCmd(NULL_FILE, "udhcpc", "-i", iface, "-p", pid_file, "-s", script_file,  "-n", "-x", NULL_STR);
	//sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -n &", iface, pid_file, script_file);
	sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s &", iface, pid_file, script_file);
	system(cmdBuff);
#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("echo 0 > /var/sc_ip_status");
#endif
}

int start_lan_dhcp()
{
	int lan_dhcp=0;
	int op_mode=0;
	apmib_get(MIB_DHCP,(void*)&lan_dhcp);
	apmib_get(MIB_OP_MODE,(void*)&op_mode);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	switch(lan_dhcp)
	{
		case DHCP_DISABLED:
			set_static_lan();
			break;
		case DHCP_SERVER:
			set_static_lan();
			reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
			set_lan_dhcpd();
			
			//down phy to notify client to get ip again
			//printf("\n%s:%d Down Lan Port PhyPower!!\n",__FUNCTION__,__LINE__);
			system("echo 1 0 > /proc/phyPower");
			system("echo 2 0 > /proc/phyPower");
			system("echo 4 0 > /proc/phyPower");
			system("echo 8 0 > /proc/phyPower");

			if(op_mode!=WISP_MODE)
			{
				system("ifconfig wlan0 down");
				system("ifconfig wlan1 down");
			}

			sleep(3);
			
			//printf("\n%s:%d Up Lan Port PhyPower!!\n",__FUNCTION__,__LINE__);
			system("echo 1 1 > /proc/phyPower");
			system("echo 2 1 > /proc/phyPower");
			system("echo 4 1 > /proc/phyPower");
			system("echo 8 1 > /proc/phyPower");

			if(op_mode!=WISP_MODE)
			{
				system("ifconfig wlan0 up");
				system("ifconfig wlan1 up");
			}
			
			reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
			break;
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
		case DHCP_AUTO:
			set_static_lan();
			break;
#endif
		case DHCP_CLIENT:
			{
		#if 0
				int intVal=0;
				//		if(br_stp_enabled==1){		/* Comment here because br0 entering forwarding state always need some time especially booting up.*/
				printf("wait for bridge initialization...\n");
				intVal=10;
				do{
					intVal--;
					sleep(1);
				}while(intVal !=0);
		//		}
			//RunSystemCmd(NULL_FILE, "dhcpc.sh", "br0", "no", NULL_STR);
		#endif
				set_lan_dhcpc("br0");
				break;
			}
			
			
#if defined(CONFIG_RTL_ULINKER)
		case DHCP_AUTO_WAN:
			{
				disable_bridge_dhcp_filter();
				set_lan_dhcpc("br0");
				break;
			}			
#endif

		default:
			reinitSer_printf(LOG_ERR,"%s:%d invalid lan dhcp type %d!\n",__FUNCTION__,__LINE__,lan_dhcp);
			break;
	}
}
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT

int getFilter_Type(void)
{
	FILE *fp;
	char *filter_conf_file = "/proc/pocket/filter_conf";
	char ip_addr[10];
	char mac_addr[14];
	int filter_type=0;
	
	fp= fopen(filter_conf_file, "r");
	if (!fp) {
        	printf("can not open /proc/pocket/filter_conf\n");
		return -1;
   	}
	fscanf(fp,"%s %s %d",ip_addr,mac_addr, &filter_type);
	fclose(fp);
	
	return filter_type;
}

void	start_lan_domain_query_dnrd()
{
	int operation_mode, int wlan_mode, int lan_dhcp_mode, char lan_mac[32];
	char tmpBuff1[200];
	char lan_domain_name[	MAX_NAME_LEN]={0};

	unsigned char Ip[32], cmdBuffer[100], tmpBuff[200];
	unsigned char domanin_name[MAX_NAME_LEN]={0};
	
	
	system("echo 1 > /var/system/start_init");
	apmib_get(MIB_OP_MODE,(void *)&operation_mode);
	apmib_get( MIB_DOMAIN_NAME, (void *)lan_domain_name);
	apmib_get(MIB_DHCP,(void *)&lan_dhcp_mode);

	if(SetWlan_idx("wlan0"))
	{
		apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); 
	}
	apmib_get(MIB_ELAN_MAC_ADDR,  (void *)tmpBuff);
	if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6))
		apmib_get(MIB_HW_NIC0_ADDR,  (void *)tmpBuff);
	sprintf(lan_mac, "%02x%02x%02x%02x%02x%02x", (unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1], 
		(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], (unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);

	if(operation_mode==1 && ((wlan_mode==1 && lan_dhcp_mode==15) || (wlan_mode==0 && lan_dhcp_mode==15)) && lan_domain_name[0])
	{
		system("echo 1 > /proc/pocket/en_filter"); 
		sprintf(tmpBuff1,"echo \"00000000 %s 0\" > /proc/pocket/filter_conf",lan_mac); 
		system(tmpBuff1);
	}else{
		system("echo 0 > /proc/pocket/en_filter"); 
		system("echo \"00000000 000000000000 2\" > /proc/pocket/filter_conf");
	}

	apmib_get( MIB_IP_ADDR,  (void *)tmpBuff);
	sprintf(Ip, "%s", inet_ntoa(*((struct in_addr *)tmpBuff)));

	memset(domanin_name, 0x00, sizeof(domanin_name));	
	apmib_get( MIB_DOMAIN_NAME,  (void *)domanin_name);

	RunSystemCmd(NULL_FILE, "rm", "-f", "/etc/hosts", NULL_STR);
	memset(cmdBuffer, 0x00, sizeof(cmdBuffer));	
	//if(strlen(domanin_name) == 0)
	//{
		
	//	if(wlan_mode==0){//ap mode	
	//		sprintf(cmdBuffer,"%s\\%s\n", Ip, "RealTekAP");
	//	}else if(wlan_mode==1){//client mode
	//		sprintf(cmdBuffer,"%s\\%s\n", Ip, "RealTekCL");
	//	}
	//	RunSystemCmd("/etc/hosts", "echo", cmdBuffer,NULL_STR);

	//}
	if(domanin_name[0]){
		if(getFilter_Type() == 0) //1:client
		{
			sprintf(cmdBuffer,"%s\\%s\n", Ip, "AlwaysHost");
		}
		else
		{
			if(wlan_mode==0){//ap mode	
				sprintf(cmdBuffer,"%s\\%s%s%s%s\n", Ip, domanin_name, "AP.com|",domanin_name, "AP.net");
			}else if(wlan_mode==1){
				sprintf(cmdBuffer,"%s\\%s%s%s%s\n", Ip, domanin_name, "CL.com|",domanin_name, "CL.net");
			}
		}

		sprintf(tmpBuff, "echo \"%s\" > /etc/hosts", cmdBuffer);
		system(tmpBuff);
		//RunSystemCmd("/etc/hosts", "echo",cmdBuffer,NULL_STR);

		RunSystemCmd(NULL_FILE, "dnrd", "--cache=off", "-s", "168.95.1.1",NULL_STR);
	}
}
#endif
#ifndef STAND_ALONE_MINIUPNP
void	start_lan_miniupnp()
{
	
#ifdef   HOME_GATEWAY	
		int intValue=0,  intValue1=0,sys_op=0;
		if(SetWlan_idx("wlan0")){
			apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&intValue);
		}
		apmib_get(MIB_OP_MODE,(void *)&sys_op);
		if(sys_op !=1)
			apmib_get(MIB_UPNP_ENABLED, (void *)&intValue1);
		else 
			intValue1=0;
		if(intValue==0 && intValue1==0){//wps enabled and upnp igd is disabled
			RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-daemon", NULL_STR);
			//system("mini_upnpd -wsc /tmp/wscd_config &");
		}else if(intValue==1 && intValue1==1){//wps is disabled, and upnp igd is enabled
			RunSystemCmd(NULL_FILE, "mini_upnpd", "-igd", "/tmp/igd_config", "-daemon", NULL_STR);
			//system("mini_upnpd -igd /tmp/igd_config &");
		}else if(intValue==0 && intValue1==1){//both wps and upnp igd are enabled
			RunSystemCmd(NULL_FILE, "mini_upnpd", "-wsc", "/tmp/wscd_config", "-igd", "/tmp/igd_config","-daemon",	NULL_STR);
			//system("mini_upnpd -wsc /tmp/wscd_config -igd /tmp/igd_config &");
		}else if(intValue==1 && intValue1==0){//both wps and upnp igd are disabled
			/*do nothing*/
		}
#endif		
}
#endif

#ifdef CONFIG_AUTO_DHCP_CHECK
void	start_lan_Auto_DHCP_Check()
{
	int opmode=0,lan_dhcp_mode=0;
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_DHCP,(void *)&lan_dhcp_mode);
	
	if(opmode==BRIDGE_MODE && lan_dhcp_mode == DHCP_SERVER)
	{
		system("Auto_DHCP_Check &");
	}
}
#endif

#ifdef SAMBA_WEB_SUPPORT
void storage_UpdateSambaConf()
{
	char 				tmpBuff[1024];
	int					number,i;
	STORAGE_GROUP_T		group_info;
	int					anonAccess,anonSambaAccess;
	FILE				*fp;
	memset(tmpBuff,'\0',1024);
	fp = fopen("/etc/samba/smb.conf","w+");

	apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anonAccess);
	apmib_get(MIB_STORAGE_ANON_DISK_ENABLE,(void*)&anonSambaAccess);
	
	strcpy(tmpBuff,"[global]\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tserver string\t= RTCN 8190 Samba Server\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlog level\t= 0\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tmax xmit\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\taio write size\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\taio read size\t= 65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlarge readwrite =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tgetwd cache =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tread raw\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\twrite raw\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlpq cache\t= 30\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\toplocks =\tyes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\twinbind nested groups\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdomain master\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlocal master\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tpublic\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tinterfaces\t= br0\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tload printers\t= no\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tprinting\t= bsd\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tprintcap name\t= /dev/null\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdisable spoolss\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tlog file\t= /var/log/log.%m\n");
	if(anonAccess == 1 && anonSambaAccess == 1)
		strcpy(tmpBuff+strlen(tmpBuff),"\tsecurity\t= share\n");
	else
		strcpy(tmpBuff+strlen(tmpBuff),"\tsecurity\t= user\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tsocket options\t= IPTOS_LOWDELAY IPTOS_THROUGHPUT TCP_NODELAY SO_KEEPALIVE TCP_FASTACK SO_RCVBUF=65536 SO_SNDBUF=65536\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tgetwd cache\t= yes\n");
	strcpy(tmpBuff+strlen(tmpBuff),"\tdns proxy\t= no\n");

	//anonymous shared folder informatio
	if(anonAccess == 1 && anonSambaAccess == 1){
		strcpy(tmpBuff+strlen(tmpBuff),"[share]\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tpath\t=/tmp/usb\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tcomment\t= anonymous user's share folder\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tread only\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\twritable\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tpublic\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\toplocks\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tkernel oplocks\t= no\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tcreate mask\t= 0777\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tbrowseable\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tguest ok\t= yes\n");
		strcpy(tmpBuff+strlen(tmpBuff),"\tdirectory mask\t= 0777\n");
		goto setOk_SambaConfUpdate;
	}

	//shared folder information
	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		*((char*)&group_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&group_info);

		if(group_info.storage_group_sharefolder_flag == 1){
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"[%s]\n",group_info.storage_group_displayname);
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tpath\t=%s\n",group_info.storage_group_sharefolder);
			strcpy(tmpBuff+strlen(tmpBuff),"\tcomment\t= smbuser's share folder\n");
			snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tvalid users\t=@%s\n",group_info.storage_group_name);			
			
			if(!strcmp(group_info.storage_group_access,"r-w"))
				snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\twrite list\t=@%s\n",group_info.storage_group_name);
			else if(!strcmp(group_info.storage_group_access,"r"))
				snprintf(tmpBuff+strlen(tmpBuff),1024-strlen(tmpBuff),"\tread list\t=@%s\n",group_info.storage_group_name);
		}
	}
	
setOk_SambaConfUpdate:

	fwrite(tmpBuff,strlen(tmpBuff),1,fp);
	fclose(fp);

	system("smbd -D");
}


void	start_lan_samba_web()
{
	int i=0;
	STORAGE_USER_T 	user_info;
	STORAGE_GROUP_T	group_info;
	int 			number;
	char cmdBuffer[128]={0};
	system("cp /etc/passwd_orig /var/passwd");
	system("cp /etc/group_orig /var/group");
	system("cp /etc/samba/smbpasswd_orig /var/samba/smbpasswd");
	//system("cp /etc/samba/smb_orig.conf /var/samba/smb.conf");
	
	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		memset(&group_info,'\0',sizeof(STORAGE_GROUP_T));
		*((char*)&group_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&group_info);

		memset(cmdBuffer,'\0',sizeof(cmdBuffer));
		snprintf(cmdBuffer,sizeof(cmdBuffer),"addgroup %s",group_info.storage_group_name);
		system(cmdBuffer);
	}
	
	apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		memset(&user_info,'\0',sizeof(STORAGE_USER_T));
		*((char*)&user_info) = (char)(i+1);
		apmib_get(MIB_STORAGE_USER_TBL,(void*)&user_info);

		memset(cmdBuffer,'\0',sizeof(cmdBuffer));
		if(!strcmp(user_info.storage_user_group,"--")){
			snprintf(cmdBuffer,sizeof(cmdBuffer),"adduser %s",user_info.storage_user_name);
			system(cmdBuffer);
		}else{
			snprintf(cmdBuffer,sizeof(cmdBuffer),"adduser -G %s %s",user_info.storage_user_group,user_info.storage_user_name);
			system(cmdBuffer);
		}

		memset(cmdBuffer,'\0',sizeof(cmdBuffer));
		snprintf(cmdBuffer,sizeof(cmdBuffer),"smbpasswd %s %s",user_info.storage_user_name,user_info.storage_user_password);
		system(cmdBuffer);
	}
	storage_UpdateSambaConf();
}
#endif
#if defined(CONFIG_APP_TR069)
void	start_lan_tr069()
{
	if (find_pid_by_name("cwmpClient") == 0)
	{
		int lan_if = 0;
		int wan_if = 0;
		int port1, port2, port3, port4, port5;
		int bitRate1, bitRate2, bitRate3, bitRate4, bitRate5;
		char mode1[5]="", mode2[5]="", mode3[5]="", mode4[5]="", mode5[5]="";
		char cmd[512];
		int cwmp_flag = 0;

		// port1
		apmib_get( MIB_CWMP_SW_PORT1_DISABLE, (void *)&port1);
		if (port1 == 1) {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			system("echo set eth if0 Enable false > /proc/rtl865x/tr181_eth_set");
		}
		else {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			apmib_get( MIB_CWMP_SW_PORT1_MAXBITRATE, (void *)&bitRate1);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if0 MaxBitRate %d > /proc/rtl865x/tr181_eth_set", bitRate1);
			system(cmd);

			usleep(100000);

			apmib_get( MIB_CWMP_SW_PORT1_DUPLEXMODE, (void *)mode1);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if0 DuplexMode %s > /proc/rtl865x/tr181_eth_set", mode1);
			system(cmd);
		}

		usleep(100000);

		// port2
		apmib_get( MIB_CWMP_SW_PORT2_DISABLE, (void *)&port2);
		if (port2 == 1) {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			system("echo set eth if1 Enable false > /proc/rtl865x/tr181_eth_set");
		}
		else {

			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			apmib_get( MIB_CWMP_SW_PORT2_MAXBITRATE, (void *)&bitRate2);
			
			memset(cmd, 0, sizeof(cmd));
			//printf("%s-%s-%d: bitRate=%d\n", __FILE__, __FUNCTION__, __LINE__, bitRate2);
			sprintf(cmd, "echo set eth if1 MaxBitRate %d > /proc/rtl865x/tr181_eth_set", bitRate2);
			system(cmd);

			usleep(100000);

			apmib_get( MIB_CWMP_SW_PORT2_DUPLEXMODE, (void *)mode2);
			
			memset(cmd, 0, sizeof(cmd));
			//printf("%s-%s-%d: mode=%s\n", __FILE__, __FUNCTION__, __LINE__, mode2);
			sprintf(cmd, "echo set eth if1 DuplexMode %s > /proc/rtl865x/tr181_eth_set", mode2);
			system(cmd);
		}

		usleep(100000);

		// port3
		apmib_get( MIB_CWMP_SW_PORT3_DISABLE, (void *)&port3);
		if (port3 == 1) {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			system("echo set eth if2 Enable false > /proc/rtl865x/tr181_eth_set");
		}
		else {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			apmib_get( MIB_CWMP_SW_PORT3_MAXBITRATE, (void *)&bitRate3);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if2 MaxBitRate %d > /proc/rtl865x/tr181_eth_set", bitRate3);
			system(cmd);

			usleep(100000);

			apmib_get( MIB_CWMP_SW_PORT3_DUPLEXMODE, (void *)mode3);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if2 DuplexMode %s > /proc/rtl865x/tr181_eth_set", mode3);
			system(cmd);
		}

		usleep(100000);

		// port4
		apmib_get( MIB_CWMP_SW_PORT4_DISABLE, (void *)&port4);
		if (port4 == 1) {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			system("echo set eth if3 Enable false > /proc/rtl865x/tr181_eth_set");
		}
		else {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			apmib_get( MIB_CWMP_SW_PORT4_MAXBITRATE, (void *)&bitRate4);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if3 MaxBitRate %d > /proc/rtl865x/tr181_eth_set", bitRate4);
			system(cmd);

			usleep(100000);

			apmib_get( MIB_CWMP_SW_PORT4_DUPLEXMODE, (void *)mode4);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if3 DuplexMode %s > /proc/rtl865x/tr181_eth_set", mode4);
			system(cmd);
		}

		usleep(100000);

		// port5
		apmib_get( MIB_CWMP_SW_PORT5_DISABLE, (void *)&port5);
		if (port5 == 1) {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			system("echo set eth if4 Enable false > /proc/rtl865x/tr181_eth_set");
		}
		else {
			//printf("%s-%s-%d\n", __FILE__, __FUNCTION__, __LINE__);
			apmib_get( MIB_CWMP_SW_PORT5_MAXBITRATE, (void *)&bitRate5);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if4 MaxBitRate %d > /proc/rtl865x/tr181_eth_set", bitRate5);
			system(cmd);

			usleep(100000);

			apmib_get( MIB_CWMP_SW_PORT5_DUPLEXMODE, (void *)mode5);
			
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "echo set eth if4 DuplexMode %s > /proc/rtl865x/tr181_eth_set", mode5);
			system(cmd);
		}

		usleep(100000);

		apmib_get( MIB_CWMP_LAN_ETHIFDISABLE, (void *)&lan_if );
		if (lan_if == 1)
			system("ifconfig eth0 down");
		else
			system("ifconfig eth0 up");

		apmib_get( MIB_CWMP_WAN_ETHIFDISABLE, (void *)&wan_if );
		if (wan_if == 1)
			system("ifconfig eth1 down");
		else
			system("ifconfig eth1 up");

		apmib_get( MIB_CWMP_FLAG, (void *)&cwmp_flag );
		if((cwmp_flag & CWMP_FLAG_AUTORUN) && isFileExist("/bin/cwmpClient") )
		{
			system("flatfsd -r");
			system("/bin/cwmpClient &");			
		}	
	}
}
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
void	start_lan_ETH_802DOT1X()
{
	int sys_op;
	
	int val, dot1xenable, dot1xmode, proxy_port_mask = 0 , client_port_mask = 0, maxport = 0, i = 0;
	int type, unicast_enable, server_port;
	ETHDOT1X_T entry;
	unsigned char cmdBuffer[100];
	
	
	apmib_get(MIB_OP_MODE,(void*)&sys_op);
	apmib_get( MIB_ELAN_ENABLE_1X, (void *)&dot1xenable);
	apmib_get( MIB_ELAN_DOT1X_MODE, (void *)&dot1xmode);
	apmib_get( MIB_ELAN_DOT1X_PROXY_TYPE, (void *)&type);
	apmib_get( MIB_ELAN_EAPOL_UNICAST_ENABLED, (void *)&unicast_enable);
	apmib_get( MIB_ELAN_DOT1X_SERVER_PORT, (void *)&server_port);
	if (dot1xenable)
	{
		//client mode enable check in flash.c
		//if (dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)
		{
			maxport =  MAX_ELAN_DOT1X_PORTNUM - 1;
			if (sys_op ==BRIDGE_MODE || sys_op == WISP_MODE)
			{
			#if !defined(CONFIG_RTL_IVL_SUPPORT)
				maxport =  MAX_ELAN_DOT1X_PORTNUM;
			#endif
			}
			if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_SNOOPING_MODE_BIT))//snooping mode enable
			{
				sprintf(cmdBuffer,"echo \"1 1\" > /proc/802dot1x/enable");
				system(cmdBuffer);
				sprintf(cmdBuffer,"echo \"%d\" > /proc/802dot1x/server_port", server_port);
				system(cmdBuffer);
			}
			else if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))//proxy mode enable
			{
				
				sprintf(cmdBuffer,"echo \"1 %d\" > /proc/802dot1x/enable", unicast_enable);
				system(cmdBuffer);
			}			
			else if ((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT))//client mode enable
			{
				sprintf(cmdBuffer,"echo \"1 1\" > /proc/802dot1x/enable");
				system(cmdBuffer);
			}
			
			sprintf(cmdBuffer,"echo \"%d\" > /proc/802dot1x/type", type);
			system(cmdBuffer);
			
			for(i=1; i<=maxport ; i++)
			{
				memset(&entry, '\0', sizeof(entry));
				//	printf("--%s(%d)--i is %d\n", __FUNCTION__, __LINE__, i);

				*((char *)&entry) = (char)i;
				apmib_get(MIB_ELAN_DOT1X_TBL, (void *)&entry);
				if (!entry.enabled)
					continue;
				
				printf("%s %d entry.enabled=%d entry.portnum=%d\n", __FUNCTION__, __LINE__, entry.enabled, entry.portnum);
				proxy_port_mask |= (1<<entry.portnum);
				if (dot1xmode & ETH_DOT1X_SNOOPING_MODE_BIT)
				{
					sprintf(cmdBuffer,"echo \"1 %d 1 1\" > /proc/802dot1x/mode", entry.portnum);
				}
				else if (dot1xmode & ETH_DOT1X_PROXY_MODE_BIT)
				{
					if (unicast_enable)
						sprintf(cmdBuffer,"echo \"1 %d 2 1\" > /proc/802dot1x/mode", entry.portnum);
					else
						sprintf(cmdBuffer,"echo \"1 %d 2 0\" > /proc/802dot1x/mode", entry.portnum);
				}
				system(cmdBuffer);
				
			}
			if (((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))||
				((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
				(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT)))
				{
					val = 1;
					apmib_set( MIB_ELAN_MAC_AUTH_ENABLED, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_ACCOUNT_RS_MAXRETRY, (void *)&val);
					val = 0;
					apmib_set( MIB_ELAN_RS_REAUTH_TO, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_RS_MAXRETRY, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_RS_INTERVAL_TIME, (void *)&val);
					val = 3;
					apmib_set( MIB_ELAN_ACCOUNT_RS_MAXRETRY, (void *)&val);

					if ((dot1xenable & ETH_DOT1X_PROXY_SNOOPING_MODE_ENABLE_BIT)&&
						(dot1xmode & ETH_DOT1X_PROXY_MODE_BIT))
					{
						apmib_set( MIB_ELAN_DOT1X_PROXY_MODE_PORT_MASK, (void *)&proxy_port_mask);
					}
					
					if ((dot1xenable & ETH_DOT1X_CLIENT_MODE_ENABLE_BIT)&&
						(dot1xmode & ETH_DOT1X_CLIENT_MODE_BIT))
					{
						client_port_mask = 1<<ETH_DOT1X_CLIENT_PORT;//default .....
						apmib_set( MIB_ELAN_DOT1X_CLIENT_MODE_PORT_MASK, (void *)&client_port_mask);
						sprintf(cmdBuffer,"echo \"1 %d 3 0\" > /proc/802dot1x/mode", ETH_DOT1X_CLIENT_PORT);
						system(cmdBuffer);
						sprintf(cmdBuffer,"rsCert -rd");
						system(cmdBuffer);
					}
					sprintf(cmdBuffer,"flash ethdot1x /var/1x/eth_1x.conf");
					system(cmdBuffer);
					sprintf(cmdBuffer,"auth_eth eth0 br0 eth /var/1x/eth_1x.conf");
					system(cmdBuffer);
				}
			
		}
		
	}

	return 0;
	
}

#endif

#ifdef RTK_CAPWAP
void	start_lan_capwap_app()
{	
	int capwapMode;
	apmib_get(MIB_CAPWAP_MODE, &capwapMode);

	// for wtp
	printf("babylon test MIB_CAPWAP_MODE=%d, CAPWAP_WTP_ENABLE=%d, CAPWAP_AC_ENABLE=%d\n", capwapMode, CAPWAP_WTP_ENABLE, CAPWAP_AC_ENABLE);
	if (capwapMode & CAPWAP_WTP_ENABLE) {
		char ac_ip_str[16], wtp_id_str[8];		
		int restart_flag;
		unsigned char tmp_ip[4];
		int tmp_int_val;
		
		// get config from flash
		apmib_get(MIB_CAPWAP_AC_IP, tmp_ip);
		sprintf(ac_ip_str, "%d.%d.%d.%d", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]);
		apmib_get(MIB_CAPWAP_WTP_ID, &tmp_int_val);
		sprintf(wtp_id_str, "%d.%d.%d.%d", tmp_ip[0], tmp_ip[1], tmp_ip[2], tmp_ip[3]);

		// get original config & compare
		restart_flag = 0;
		if (capwap_config_changed("wtp_started", "wtp_status")) {			
			restart_flag = 1;
		}
		
		if (capwap_config_changed(ac_ip_str, "ac_ip")) {			
			restart_flag = 1;
		}

		if (capwap_config_changed(wtp_id_str, "wtp_id")) {			
			restart_flag = 1;
		}

		// restart wtp
		if (restart_flag) {
			
			capwap_set_config_to_file("wtp_started", "wtp_status");
			capwap_set_config_to_file(ac_ip_str, "ac_ip");
			capwap_set_config_to_file(wtp_id_str, "wtp_id");
			sleep(2);
			system("WTP "CAPWAP_APP_ETC_DIR);
		}
		
	} else {
		
		capwap_set_config_to_file("wtp_disabled", "wtp_status");
	}

	// for AC
	if (capwapMode & CAPWAP_AC_ENABLE) {		
		int restart_flag = 0;		
		if (capwap_config_changed("ac_started", "ac_status")) { 		
			restart_flag = 1;
		}
		if (restart_flag) {
						
			capwap_set_config_to_file("ac_started", "ac_status");
			sleep(2);
			system("AC "CAPWAP_APP_ETC_DIR);
		}
	} else {
		
		capwap_set_config_to_file("ac_disabled", "ac_status");
	}
	
}

#endif


int reinit_func_bridge(BASE_DATA_Tp data)
{
	clean_bridge(data);

	start_bridge();
}

int reinit_func_lan_mac(BASE_DATA_Tp data)
{
	clean_lan_mac();
	start_lan_mac();
}

int reinit_func_lan_stp(BASE_DATA_Tp data)
{
	clean_br_stp();
	start_br_stp();
}
int reinit_func_lan_app_dhcp(BASE_DATA_Tp data)
{
	clean_lan_dhcp();
	start_lan_dhcp();
}


int func_connect_lan(BASE_DATA_Tp data)
{
	/*called by sysconf init ap wlanapp when dhcp client connect*/
	/*To Down up interface*/
	RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "down", NULL_STR);
	RunSystemCmd(NULL_FILE, "ifconfig", "br0", "down", NULL_STR);
	RunSystemCmd(NULL_FILE, "ifconfig", "br0", "up", NULL_STR);

	/*restart wlan app. keep same with old style*/
	reinit_func_wlan_app(data);
}

int func_lan_dhcp_connect(BASE_DATA_Tp data)
{
	char wanip[32]={0}, mask[32]={0},remoteip[32]={0};
	unsigned char interface[32];
	
	LAN_DATA_Tp	plan;
	plan = (LAN_DATA_Tp)(data);
	sprintf(interface,"br%d",plan->base.lan_idx);
	memset(wanip,0,sizeof(wanip));
	strcpy(wanip,plan->lan_data.ip_addr);

	memset(mask,0,sizeof(mask));
	strcpy(mask,plan->lan_data.sub_net);
	RunSystemCmd(NULL_FILE, "ifconfig", interface, wanip, "netmask", mask, NULL_STR);
}

#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
int reinit_func_lan_app_domain_query_dnrd(BASE_DATA_Tp data)
{
	clean_lan_domain_query_dnrd();
	start_lan_domain_query_dnrd();
}

#endif
#ifndef STAND_ALONE_MINIUPNP
int reinit_func_lan_app_miniupnp(BASE_DATA_Tp data)
{
	clean_lan_miniupnp();
	start_lan_miniupnp();
}

#endif
#ifdef CONFIG_AUTO_DHCP_CHECK
int reinit_func_lan_app_Auto_DHCP_Check(BASE_DATA_Tp data)
{
	clean_lan_Auto_DHCP_Check();
	start_lan_Auto_DHCP_Check();
}

#endif
#ifdef SAMBA_WEB_SUPPORT
int reinit_func_lan_app_samba_web(BASE_DATA_Tp data)
{
	clean_lan_samba_web();
	start_lan_samba_web();
}

#endif
#if defined(CONFIG_APP_TR069)	
int reinit_func_lan_app_tr069(BASE_DATA_Tp data)
{
	clean_lan_tr069();
	start_lan_tr069();
}

#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
int reinit_func_lan_app_ETH_802DOT1X(BASE_DATA_Tp data)
{
	clean_lan_ETH_802DOT1X();
	start_lan_ETH_802DOT1X();
}

#endif
#ifdef RTK_CAPWAP	
int reinit_func_lan_app_capwap_app(BASE_DATA_Tp data)
{
	clean_lan_capwap_app();
	start_lan_capwap_app();
}

#endif
int get_blockDevPartition(char *str, char *partition)
{

	unsigned char tk[50];
	unsigned int i,j;
	unsigned int curCnt,preCnt;
	
	if(str==NULL)
	{
		return -1;
	}
	
	memset(tk,0, sizeof(tk));

	/*	partition table format:
		major minor  #blocks  name
	*/
	
	preCnt=0;
	curCnt=0;
	for (i=0;i<strlen(str);i++)
	{          
		if( (str[i]!=' ') && (str[i]!='\n') && (str[i]!='\r'))
		{
			if(preCnt==curCnt)
			{
				tk[curCnt]=i;
				curCnt++;
			}
		}
		else if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\r') )
		{
			preCnt=curCnt;
		}
	}
	
	/*to check device major number is 8*/
	
	if(!isdigit(str[tk[0]]))
	{
		return -1;
	}

	if(tk[1]==0)
	{
		return -1;
	}

	if(tk[1]<=tk[0])
	{
		return -1;
	}

	if((str[tk[0]]!='8') ||(str[tk[0]+1]!=' '))
	{
		return -1;
	}
	
	if(tk[3]==0)
	{
		return -1;
	}

	/*to get partition name*/
	j=0;
	for(i=tk[3]; i<strlen(str); i++)
	{
		
		if((str[i]==' ') || (str[i]=='\n') ||(str[i]=='\n'))
		{
			partition[j]='\0';
			return 0;
		}
		else
		{
			partition[j]=str[i];
			j++;
		}
			
	}
	return 0;
}
int Check_shouldMount(char *partition_name)
{
	DIR *dir=NULL;
	struct dirent *next;
	int found=0;
	dir = opendir("/tmp/usb");
	if (!dir) {
		printf("Cannot open %s", "/tmp/usb");
		return -1;
	}
	while ((next = readdir(dir)) != NULL) {
		//printf("Check_shouldMount:next->d_reclen=%d, next->d_name=%s\n",next->d_reclen, next->d_name);
			/* Must skip ".." */
			if (strcmp(next->d_name, "..") == 0)
				continue;
			if (strcmp(next->d_name, ".") == 0)
				continue;
			if (strcmp(next->d_name, "mnt_map") == 0)
				continue;
			if(!strcmp(next->d_name, partition_name)){
				found=1;
				break;
			}
	}
	closedir(dir);
	return found;
}

void autoMountOnBootUp(void)
{
	FILE *fp;
	
	int line=0;
	char buf[512];
	char partition[32];
	char usbMntCmd[64];
	int ret=-1;
	if(isFileExist(PARTITION_FILE)){
		fp= fopen(PARTITION_FILE, "r");
		if (!fp) {
	        	printf("can not  open /proc/partitions\n");
			return; 
	   	}

		while (fgets(buf, sizeof(buf), fp)) 
		{
			ret=get_blockDevPartition(buf, &partition);
			if(ret==0)
			{
				if(Check_shouldMount(partition)==0){
				sprintf(usbMntCmd, "DEVPATH=/sys/block/sda/%s ACTION=add usbmount block", partition);
				RunSystemCmd(NULL_FILE,  "echo", usbMntCmd, NULL_STR);
				system(usbMntCmd);
			}
			}
			
		}
	
		fclose(fp);
	}
	

}

void start_mount()
{
#if defined(HTTP_FILE_SERVER_SUPPORTED) || defined(RTL_USB_IP_HOST_SPEEDUP)
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "2048", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "1048576", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "4096 108544 4194304", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_moderate_rcvbuf", "echo", "0", NULL_STR);
#else	
	/*config linux parameter for improving samba performance*/
	RunSystemCmd("/proc/sys/vm/min_free_kbytes", "echo", "1024", NULL_STR);
	
	RunSystemCmd("/proc/sys/net/core/netdev_max_backlog", "echo", "8192", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/optmem_max", "echo", "131072", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/rmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_default", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/core/wmem_max", "echo", "524288", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_rmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_wmem", "echo", "131072 262144 393216", NULL_STR);
	RunSystemCmd("/proc/sys/net/ipv4/tcp_mem", "echo", "768 1024 1380", NULL_STR);
#endif
	/*config hot plug and auto-mount*/
	RunSystemCmd("/proc/sys/kernel/hotplug", "echo", "/usr/hotplug", NULL_STR);
	RunSystemCmd(NULL_FILE, "mkdir", "-p /tmp/usb/", NULL_STR);

	/*force kernel to write data to disk, don't cache in memory for a long time*/
	RunSystemCmd("/proc/sys/vm/vfs_cache_pressure", "echo", "10000", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_background_ratio", "echo", "5", NULL_STR);
	RunSystemCmd("/proc/sys/vm/dirty_writeback_centisecs", "echo", "100", NULL_STR);
	/*automatically mount partions listed in /proc/partitions*/
	autoMountOnBootUp();

	system("killall usbStorageAppController 2>/dev/null");
	system("usbStorageAppController&");
//	RunSystemCmd("/var/group",  "echo", " ",  NULL_STR);
//	RunSystemCmd(NULL_FILE,  "cp", "/etc/group", "/var/group",  NULL_STR);

}

#if defined(CONFIG_APP_SAMBA)
void start_samba()
{
	/*start samba*/
	//RunSystemCmd(NULL_FILE,  "echo", "start samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "mkdir", "/var/samba", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/samba/smb.conf", "/var/samba/smb.conf",  NULL_STR);
	RunSystemCmd("/var/group",  "echo", " ",  NULL_STR);
        RunSystemCmd(NULL_FILE,  "cp", "/etc/group", "/var/group",  NULL_STR);
	//RunSystemCmd(NULL_FILE,  "smbd", "-D", NULL_STR);
	//RunSystemCmd(NULL_FILE,  "nmbd", "-D", NULL_STR);
}
#endif

#if defined(CONFIG_APP_VSFTPD)
void start_vsftpd()
{
	RunSystemCmd(NULL_FILE, "echo","start vsftpd", NULL_STR);
	RunSystemCmd(NULL_FILE,  "cp", "/etc/vsftpd.conf", "/var/config/vsftpd.conf",  NULL_STR);
	system("vsftpd /var/config/vsftpd.conf &");
}
#endif

#if defined(CONFIG_RPS)
static void rtl_configRps(void)
{
	system("mount -t sysfs sysfs /sys");
	system("echo 2 > /sys/class/net/eth0/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth1/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth2/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth3/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/eth4/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/wlan0/queues/rx-0/rps_cpus");
	system("echo 2 > /sys/class/net/wlan1/queues/rx-0/rps_cpus");
	system("echo 4096 > /sys/class/net/eth0/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth1/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth2/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth3/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /sys/class/net/eth4/queues/rx-0/rps_flow_cnt");
	system("echo 4096 > /proc/sys/net/core/rps_sock_flow_entries");
}
#endif

void init_nat_proc()
{
	int opmode=-1,wan_dhcp_mode=0;
	
	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WAN_DHCP,(void*)&wan_dhcp_mode);
	
#ifdef   HOME_GATEWAY
#ifdef CONFIG_POCKET_AP_SUPPORT
#else
	if(isFileExist(HW_NAT_FILE)){/*hw nat supported*/
		/*cleanup hardware tables*/
		if(opmode==GATEWAY_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "1", NULL_STR);	/*gateway mode*/
		else if(opmode==BRIDGE_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "2", NULL_STR);	/*bridge mode*/
		else if(opmode==WISP_MODE)
			RunSystemCmd(HW_NAT_FILE, "echo", "3", NULL_STR);	/*wisp mode*/
		else if(opmode==3)
			RunSystemCmd(HW_NAT_FILE, "echo", "4", NULL_STR);	/*bridge mode with multiple vlan*/
		else
			RunSystemCmd(HW_NAT_FILE, "echo", "5", NULL_STR); /*wisp mode with multiple vlan*/
		
	}else{/*software nat supported*/ 
		if(opmode==GATEWAY_MODE)
		{
#ifdef RTK_USB3G //fix me???
			if(wan_dhcp_mode == USB3G)
      	RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
			else
#endif
				RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "0", NULL_STR);
		}
		else if(opmode==BRIDGE_MODE)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "1", NULL_STR);
		else if(opmode==WISP_MODE)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "2", NULL_STR);
		else if(opmode==3)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "3", NULL_STR);
		else if(opmode==4)
			RunSystemCmd(SOFTWARE_NAT_FILE, "echo", "4", NULL_STR);
		
	}
#endif	//CONFIG_POCKET_AP_SUPPORT
#endif	
}
int init_func_misc(BASE_DATA_Tp data)
{
		int pid=-1;
		char strPID[32]={0};
		char br_interface[32]={0};
		int intValue =0;
		get_br_interface_name(br_interface);
		RunSystemCmd(NULL_FILE, "iptables", "-F", NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-F", "-t", "nat",  NULL_STR);
		RunSystemCmd(NULL_FILE, "iptables", "-A", "INPUT", "-j", "ACCEPT", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/eth1_ip", NULL_STR);
		RunSystemCmd(NULL_FILE, "rm", "-f", "/var/ntp_run", NULL_STR);

		RunSystemCmd(PROC_FASTNAT_FILE, "echo", "1", NULL_STR);

		init_nat_proc();
		
		//set kthreadd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 2", NULL_STR);
		//set ksoftirqd high priority for performance
		RunSystemCmd(NULL_FILE, "renice -20 3", NULL_STR);
		//set boa high priority
		if(isFileExist(WEBS_PID_FILE)){
			pid=getPid_fromFile(WEBS_PID_FILE);
			if(pid != 0){
				sprintf(strPID, "%d", pid);
				RunSystemCmd(NULL_FILE, "renice", "-20", strPID, NULL_STR);
			}
		}

		if(isFileExist(LLTD_PROCESS_FILE)){
			RunSystemCmd(NULL_FILE, "lld2d", br_interface, NULL_STR);
		}
		if(isFileExist(SNMPD_PROCESS_FILE)){
		RunSystemCmd(NULL_FILE, "snmpd.sh", "restart", NULL_STR);
		//RunSystemCmd(NULL_FILE, "snmpd", "-c", SNMPD_CONF_FILE, "-p", SNMPD_PID_FILE,  NULL_STR);
		}
		if(isFileExist(NMSD_PROCESS_FILE)){
			RunSystemCmd(NULL_FILE, "nmsd", NULL_STR);
		}


		/*enable igmp snooping*/
	/*igmp snooping is independent with igmp proxy*/
#if defined (CONFIG_IGMPV3_SUPPORT)
	RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "3", NULL_STR);
#else
	RunSystemCmd(PROC_BR_IGMPVERSION, "echo", "2", NULL_STR);
#endif
	RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "1", NULL_STR);
	RunSystemCmd(PROC_BR_IGMPQUERY, "echo", "1", NULL_STR);
#if defined (CONFIG_RTL_MLD_SNOOPING)	
	RunSystemCmd(PROC_BR_MLDSNOOP, "echo", "1", NULL_STR);
	RunSystemCmd(PROC_BR_MLDQUERY, "echo", "1", NULL_STR);
#endif

	
	apmib_get(MIB_IGMP_FAST_LEAVE_DISABLED, (void *)&intValue);
	if(intValue)
	{
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","0","2", NULL_STR);
	}
	else
	{	
		RunSystemCmd(PROC_BR_IGMPSNOOP, "echo", "fastleave","1","0", NULL_STR);
	}
	
#ifdef SUPPORT_ZIONCOM_RUSSIA
	RunSystemCmd("/proc/sys/net/ipv4/conf/eth1/force_igmp_version", "echo", "2", NULL_STR);
#endif
	//RunSystemCmd("/proc/sys/net/ipv4/ip_conntrack_max", "echo", "2048", NULL_STR);

#if defined(CONFIG_APP_USBMOUNT)
printf("%s:%d####################################\n",__FUNCTION__,__LINE__);
	start_mount();
#if defined (CONFIG_APP_SAMBA)
	start_samba();
#endif
#if defined (CONFIG_APP_VSFTPD)
	start_vsftpd();
#endif	
#endif

#if defined(CONFIG_APP_DLNA_DMS)
	//printf("%s, %d\n", __FUNCTION__, __LINE__);
	system("ushare -f /etc/ushare.conf &");
#endif

#if defined(HTTP_FILE_SERVER_SUPPORTED)
		RunSystemCmd("/proc/http_file/getLanIp", "echo", "1", NULL_STR);
#endif

	#if defined(CONFIG_DOMAIN_NAME_QUERY_SUPPORT)
		system("rm -f  /var/system/start_init 2> /dev/null");
	#endif

#if defined(CONFIG_APP_FWD)
	//##For fwd
		//system("mount -t tmpfs none /sbin"); //put fwd to ram
		//system("cp /bin/fwd /sbin"); //put fwd to ram
		system("fwd &");
#endif

	//reply only if the target IP address is local address configured on the incoming interface
	RunSystemCmd("/proc/sys/net/ipv4/conf/eth1/arp_ignore", "echo", "1", NULL_STR);
	/*increase routing cache rebuild count from 4 to 2048*/
	RunSystemCmd(RT_CACHE_REBUILD_COUNT, "echo", "2048", NULL_STR);
	system("timelycheck &");
	
#if defined(CONFIG_RPS)
		rtl_configRps();
#endif
	return 0;
}
