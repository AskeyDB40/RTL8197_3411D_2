#include "auth.h"

#ifdef SUPER_NAME_SUPPORT

//common user' permission to access the web
auth_html_t auth_html[] = {
		{"8021q_vlan.htm",            	  { 1,1 } },
		{"administration.htm",            { 1,1 } },
		{"airtime.htm",                   { 1,1 } },
		{"bt.htm ",                       { 1,1 } }, 
		{"btclient.htm",                  { 1,1 } },
		{"btnewtorrent.htm",              { 1,1 } },
		{"code.htm",                      { 1,1 } },
		{"cpuShow.htm",                   { 1,1 } },  
		{"ddns.htm",                      { 1,1 } },   
		{"dhcp6s.htm",                    { 1,1 } }, 
		{"dhcptbl.htm",                   { 1,1 } },
		{"diagnostic.htm",                { 1,1 } },
		{"disk.htm",                      { 1,1 } },
		{"dmz.htm",                       { 1,1 } },    
		{"dnsv6.htm",                     { 1,1 } },
		{"dos.htm",                       { 1,1 } },
		{"eth_dot1x.htm",                 { 1,1 } },
		{"ethdot1xCertInstall.htm",       { 1,1 } },
		{"home.htm",                      { 1,1 } },  
		{"home_old.htm",                  { 1,1 } },  
		{"http_files.htm",                { 1,1 } },
		{"http_files_dir.htm",            { 1,1 } },		
		{"index.htm",                     { 1,1 } },
		{"ip_qos.htm",                    { 1,1 } }, 
		{"ip6_qos.htm",                   { 1,1 } },  
		{"ip6filter.htm",                 { 1,1 } },  
		{"ipfilter.htm",                  { 1,1 } },		 
		{"ipv6_basic.htm",                { 1,1 } },
		{"ipv6_wan.htm",                  { 1,1 } },		
		{"logout.htm",                    { 1,1 } },  
		{"macfilter.htm",                 { 1,1 } }, 
		{"map_e.html",                    { 1,1 } },
		{"menu.htm",                      { 1,1 } },
		{"menu_empty.html",               { 1,1 } },
		{"ntp.htm",                       { 1,1 } }, 
		{"opmode.htm",                    { 1,1 } },
		{"password.htm",                  { 1,1 } },  
		{"pocket_sitesurvey.htm",         { 1,1 } },
		{"portfilter.htm",                { 1,1 } }, 
		{"portfilter6.htm",               { 1,1 } },		
		{"portfw.htm",                    { 1,1 } },
		{"powerConsumption.htm",          { 1,1 } },
		{"powerConsumption_data.htm",     { 1,1 } },	
		{"powerConsumption_proc.sxsl",    { 1,1 } },		
		{"radvd.htm",                     { 1,1 } },  
		{"reload.htm",                    { 1,1 } }, 
		{"route.htm",                     { 1,1 } }, 
		{"routetbl.htm",                  { 1,1 } },
		{"rsCertInstall.htm",             { 1,1 } }, 
		{"saveconf.htm",                  { 1,0 } }, // "save/reload setting" widget and saveconf.htm  invisible
		{"snmp.htm",                      { 1,1 } }, 
		{"stats.htm",                     { 1,1 } },
		{"status.htm",                    { 1,1 } }, 
		{"storage_creategroup.htm",       { 1,1 } },
		{"storage_createsharefolder.htm", { 1,1 } },
		{"storage_createuser.htm",        { 1,1 } },  
		{"storage_editgroup.htm",         { 1,1 } },
		{"storage_edituser.htm",          { 1,1 } },
		{"super_password.htm",            { 1,0 } }, //only superUser can access
		{"syscmd.htm",                    { 1,1 } },
		{"syslog.htm",                    { 1,1 } },
		{"tcpip_staticdhcp.htm",          { 1,1 } },	
		{"tcpiplan.htm",                  { 1,1 } }, 
		{"tcpipwan.htm",                  { 1,1 } },		
		{"title.htm",                     { 1,1 } },
		{"tr069config.htm",               { 1,1 } }, 
		{"tunnel6.htm",                   { 1,1 } },
		{"ulinker_opmode.htm",            { 1,1 } },
		{"upload.htm",                    { 1,0 } },  //"upgrade firmware" widget and upload.htm invisible
		{"upload_st.htm",                 { 1,1 } },
		{"urlfilter.htm",                 { 1,1 } },
		{"usb_conninfo.htm",              { 1,1 } },
		{"usb_fileinfo.htm",              { 1,1 } },
		{"vlan.htm",                      { 1,1 } },
		{"vlan_wan.htm",                  { 1,1 } }, 
		{"wlactrl.htm",                   { 1,1 } }, 
		{"wladvanced.htm",                { 1,1 } }, 
		{"wlan_schedule.htm",             { 1,1 } },
		{"wlbandmode.htm",                { 1,1 } }, 
		{"wlbasic.htm",                   { 1,1 } }, 
		{"wlft.htm",                      { 1,1 } },
		{"wlmesh.htm",                    { 1,1 } },
		{"wlmeshACL.htm",                 { 1,1 } },		
		{"wlmeshinfo.htm",                { 1,1 } },
		{"wlmeshproxy.htm",               { 1,1 } },
		{"wlmultipleap.htm",              { 1,1 } },
		{"wlp2p.htm",                     { 1,1 } },
		{"wlp2pscanlist.htm",             { 1,1 } },
		{"wlp2pstate.htm",                { 1,1 } },
		{"wlp2pwscconnect.htm",           { 1,1 } },
		{"wlp2pwscsel.htm",               { 1,1 } }, 
		{"wlsch.htm",                     { 1,1 } },
		{"wlsecurity.htm",                { 1,1 } },
		{"wlsecurity_all.htm",            { 1,1 } },		
		{"wlstatbl.htm",                  { 1,1 } }, 
		{"wlstatbl_vap.htm",              { 1,1 } },
		{"wlsurvey.htm",                  { 1,1 } },
		{"wlwapiCertManagement.htm",      { 1,1 } },
		{"wlwapiDistribute.htm",          { 1,1 } },
		{"wlwapiinstallcert.htm",         { 1,1 } },
		{"wlwapiRekey.htm",               { 1,1 } },
		{"wlwds.htm",                     { 1,1 } },
		{"wlwdsenp.htm",                  { 1,1 } },		
		{"wlwdstbl.htm",                  { 1,1 } },
		{"wlwps.htm",                     { 1,1 } },
		{"wlp2p.htm",						{1,1}},
//new Dir
		{"countDownPage.htm",             { 1,1 } },
		{"sub_menu.htm",                  { 1,1 } },
		{"sub_menu_firewall.htm",         { 1,1 } },		
		{"sub_menu_ipv6.htm",             { 1,1 } },
		{"sub_menu_mng.htm",              { 1,1 } },//test for topmenu
		{"sub_menu_tcpip.htm",            { 1,1 } },
		{"sub_menu_voip.htm",             { 1,1 } },
		{"sub_menu_wlan.htm",             { 1,1 } },
		{"title.html",                    { 1,1 } },
		{"top_menu.htm",                  { 1,1 } },		
		{NULL,                             { NULL,NULL } }
};

//common user' permission to modify the settings
auth_form_t auth_form[] = {
			{"formWlanSetup",             { 1,1 } },
			{"formWlanRedirect",          { 1,1 } },
#if 0
			{"formWep64", { 1,1 } },
			{"formWep128", { 1,1 } },
#endif
			{"formWep", { 1,1 } },
#ifdef MBSSID
			{"formWlanMultipleAP", { 1,1 } },
#endif
#ifdef CONFIG_RTL_AIRTIME
			{"formAirtime", { 1,1 } },
#endif
#ifdef CONFIG_RTK_MESH
			{"formMeshSetup", { 1,1 } },
			{"formMeshProxy", { 1,1 } },
			//{"formMeshProxyTbl", { 1,1 } },
			{"formMeshStatus", { 1,1 } },
#ifdef _MESH_ACL_ENABLE_
			{"formMeshACLSetup", { 1,1 } },
#endif
#endif
			{"formTcpipSetup", { 1,1 } },
			{"formPasswordSetup", { 1,1 } },
			{"formLogout", { 1,1 } },
			{"formUpload", { 1,1 } },
#if defined(CONFIG_USBDISK_UPDATE_IMAGE)
			{"formUploadFromUsb", { 1,1 } },
#endif
#ifdef CONFIG_RTL_WAPI_SUPPORT
			{"formWapiReKey", { 1,1 } },
			{"formUploadWapiCert", { 1,1 } },
			{"formUploadWapiCertAS0", { 1,1 } },
			{"formUploadWapiCertAS1", { 1,1 } },
			{"formWapiCertManagement", { 1,1 } },
			{"formWapiCertDistribute", { 1,1 } },
#endif
#ifdef CONFIG_RTL_802_1X_CLIENT_SUPPORT
			{"formUpload8021xUserCert", { 1,1 } },
#endif
#ifdef CONFIG_RTL_ETH_802DOT1X_CLIENT_MODE_SUPPORT
			{"formUploadEth8021xUserCert",{ 1,1 } },
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
			{"formEthDot1x", { 1,1 } },
#endif
#ifdef TLS_CLIENT	
			{"formCertUpload", { 1,1 } },
#endif
			{"formWlAc", { 1,1 } },
			{"formAdvanceSetup", { 1,1 } },
			{"formReflashClientTbl", { 1,1 } },
			{"formWlEncrypt", { 1,1 } },
			{"formStaticDHCP", { 1,1 } },
#if defined(CONFIG_RTL_92D_SUPPORT) || defined(CONFIG_RTL_8881A_SELECTIVE)
				{"formWlanBand2G5G", { 1,1 } },
#endif
#ifdef FAST_BSS_TRANSITION	
			{"formFt",{ 1,1 } },
#endif
		
#if defined(VLAN_CONFIG_SUPPORTED)
			{"formVlan", { 1,1 } },
#endif
#ifdef HOME_GATEWAY
#if defined(CONFIG_RTK_VLAN_WAN_TAG_SUPPORT)
			{"formVlanWAN", { 1,1 } },
#endif
			{"formWanTcpipSetup", { 1,0 } }, //test form secondUser post action
#ifdef ROUTE_SUPPORT
			{"formRoute", { 1,1 } },
#endif
			{"formPortFw", { 1,1 } },
			{"formFilter", { 1,1 } },
			//{"formTriggerPort", { 1,1 } },
			{"formDMZ", { 1,1 } },
			{"formDdns", { 1,1 } },
			{"formOpMode", { 1,1 } },
#if defined(CONFIG_RTL_ULINKER)
			{"formUlkOpMode", { 1,1 } },
#endif	
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)	
			{"formDualFirmware", { 1,1 } },
#endif
#if defined(GW_QOS_ENGINE)
			{"formQoS", { 1,1 } },
#elif defined(QOS_BY_BANDWIDTH)	
			{"formIpQoS", { 1,1 } },
#endif
#ifdef CONFIG_RTL_BT_CLIENT
			{"formBTBasicSetting",{ 1,1 } },
			{"formBTClientSetting",{ 1,1 } },
			{"formBTFileSetting",{ 1,1 } },
			{"formBTNewTorrent",{ 1,1 } },
#endif
#ifdef CONFIG_RTL_TRANSMISSION
			{"formTransmissionBT", { 1,1 } },
#endif
#ifdef DOS_SUPPORT
			{"formDosCfg", { 1,1 } },
#endif
#ifdef CONFIG_IPV6
			{"formRadvd", { 1,1 } },
			{"formDnsv6", { 1,1 } },
			{"formDhcpv6s", { 1,1 } },
			{"formIPv6Addr", { 1,1 } },
			{"formIpv6Setup", { 1,1 } },
			{"formTunnel6", { 1,1 } },
#ifdef CONFIG_MAP_E_SUPPORT
			{"formMapE", { 1,1 } },
#endif
#endif
#else
			{"formSetTime", { 1,1 } },
#endif //HOME_GATEWAY
			// by sc_yang
			{"formNtp", { 1,1 } },
			{"formWizard", { 1,1 } },
			{"formPocketWizard", { 1,1 } },
#ifdef REBOOT_CHECK	
			{"formRebootCheck", { 1,1 } },
#if defined(WLAN_PROFILE)	
			{"formSiteSurveyProfile", { 1,1 } },
#endif //#if defined(WLAN_PROFILE)		
#endif	
			{"formSysCmd", { 1,1 } },
			{"formSysLog", { 1,1 } },
#ifdef SYS_DIAGNOSTIC
			{"formDiagnostic", { 1,1 } },
#endif
#ifdef CONFIG_APP_SMTP_CLIENT
			{"formSmtpClient", { 1,1 } },
#endif
#if defined(CONFIG_SNMP)
			{"formSetSNMP", { 1,1 } },
#endif
			{"formSaveConfig", { 1,1 } },
			{"formUploadConfig", { 1,1 } },
			{"formSchedule", { 1,1 } },
#if defined(NEW_SCHEDULE_SUPPORT)	
			{"formNewSchedule", { 1,1 } },
#endif 
#if defined(CONFIG_RTL_P2P_SUPPORT)
			{"formWiFiDirect", { 1,1 } },
			{"formWlP2PScan", { 1,1 } },
#endif
			{"formWirelessTbl", { 1,1 } },
			{"formStats", { 1,1 } },
			{"formWlSiteSurvey", { 1,1 } },
			{"formWlWds", { 1,1 } },
			{"formWdsEncrypt", { 1,1 } },
#ifdef WLAN_EASY_CONFIG
			{"formAutoCfg", { 1,1 } },
#endif
#ifdef WIFI_SIMPLE_CONFIG
			{"formWsc", { 1,1 } },
#endif
#ifdef LOGIN_URL
			{"formLogin", { 1,1 } },
#endif
#ifdef CONFIG_APP_TR069
			{"formTR069Config", { 1,1 } },
#ifdef _CWMP_WITH_SSL_
		
			{"formTR069CPECert", { 1,1 } },
			{"formTR069CACert", { 1,1 } },
#endif
#endif
#ifdef HTTP_FILE_SERVER_SUPPORTED
			{"formusbdisk_uploadfile", { 1,1 } },
#endif
#ifdef VOIP_SUPPORT
			{"voip_general_set", { 1,1 } },
			{"voip_dialplan_set", { 1,1 } },
			{"voip_tone_set", { 1,1 } },
			{"voip_ring_set", { 1,1 } },
			{"voip_other_set", { 1,1 } },
			{"voip_config_set", { 1,1 } },
			{"voip_fw_set", { 1,1 } },
			{"voip_net_set", { 1,1 } },
#ifdef CONFIG_RTK_VOIP_IVR
			{"voip_ivrreq_set", { 1,1 } },
#endif
#ifdef CONFIG_RTK_VOIP_SIP_TLS
			{voip_TLSCertUpload, { 1,1 } },
#endif
#endif
		
#ifdef SAMBA_WEB_SUPPORT
			{"formDiskCfg",{ 1,1 } },
			
			{"formDiskManagementAnon",{ 1,1 } },
			{"formDiskManagementUser",{ 1,1 } },
			{"formDiskManagementGroup",{ 1,1 } },
			
			{"formDiskCreateUser",{ 1,1 } },
			{"formDiskCreateGroup",{ 1,1 } },
			{"formDiskEditUser",{ 1,1 } },
			{"formDiskEditGroup",{ 1,1 } },
		
			{"formDiskCreateShare",{ 1,1 } },
			{"formDiskCreateFolder",{ 1,1 } },
#endif
		
#ifdef CONFIG_CPU_UTILIZATION
			{"formCpuUtilization",{ 1,1 } },
#endif

	{NULL,                        { NULL,NULL } }
};


int aspUserFlag(request *req){
	char superBuf[50];
	if(req->userName == NULL) //superUser = "", no_login has all authorities
	{
		sprintf(superBuf,"%d",NO_LOGIN);
	}else{
		if ( !apmib_get(MIB_SUPER_NAME, ((void *)superBuf))) 
			return -1;
		
		if( !strcmp(req->userName, superBuf) )
			sprintf(superBuf,"%d",SUPER_FLAG);
		
		else{
			if ( !apmib_get(MIB_USER_NAME, ((void *)superBuf))) 
				return -1;
			if( !strcmp(req->userName, superBuf) )//second-degree User
				sprintf(superBuf,"%d",SECOND_FLAG);
		}
		
	}
	req_format_write(req,superBuf);
	return 0;
}



int getHtmlAuth(char *htmlName, int flag){// html file can be read or not
	int i;
	auth_html_t *now_html;
	if( !htmlName || flag > USERS_NUMS){
		return -1;
	}
	for(i = 0; auth_html[i].name != NULL; i++){
		now_html = &auth_html[i];
		if( !strcmp(now_html->name, htmlName) )
		{
			return now_html->read[flag - 1];

		}
			
	}

	return 1;//default can be read

}

int getFormAuth(char *formName, int flag){// postform can be  actioned or not
	int i;
	auth_form_t *now_form;
	if( !formName || flag > USERS_NUMS){
		return -1;
	}
	for(i = 0; auth_form[i].name != NULL; i++){
		now_form = &auth_form[i];
		if( !strcmp(now_form->name, formName) ){
			return now_form->write[flag - 1];
		}
			
	}

	return 1;//default can be post
}



#endif

