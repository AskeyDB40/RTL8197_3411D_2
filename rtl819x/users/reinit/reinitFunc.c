
#include "reinitSer.h"
#include "reinit_utility.h"
#include "reinitFunc_wan.h"
#include "reinitFunc_lan.h"
#include "reinitFunc_wlan.h"
#include "decisonFunc.h"

int reinit_func_setSysTime(BASE_DATA_Tp data)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	system("flash settime");
	return 0;
}

int reinit_func_setSysPasswd(BASE_DATA_Tp data)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}

int clean_sys_app_log()
{
	RunSystemCmd(NULL_FILE, "killall", "-9", "syslogd", NULL_STR);
	RunSystemCmd(NULL_FILE, "killall", "-9", "klogd", NULL_STR);
#ifdef RINGLOG
	system("rm /var/log/log_split >/dev/null 2>&1");
#endif
	
	return 0;
}

int start_sys_app_log()
{
	int intValue=0,  intValue1=0;
	char tmpBuffer[32];
	char syslog_para[32];
	char localServer[32];
	
	apmib_get(MIB_SCRLOG_ENABLED, (void*)&intValue);
	if(intValue !=0 && intValue !=2 && intValue !=4 && intValue !=6 && intValue !=8 &&
		intValue !=10 && intValue !=12 && intValue !=14) 
	{
		apmib_get(MIB_REMOTELOG_ENABLED, (void*)&intValue1);
		if(intValue1 != 0){
			apmib_get(MIB_REMOTELOG_SERVER,  (void *)tmpBuffer);
			if (memcmp(tmpBuffer, "\x0\x0\x0\x0", 4))
			{
#ifdef RINGLOG /* ring log */
				sprintf(localServer, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
				sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
				system(tmpBuffer);
				sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
				sprintf(syslog_para, "%d", LOG_SPLIT);
				reinitSer_printf(LOG_DEBUG, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
					MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
				RunSystemCmd(NULL_FILE, "syslogd", "-L","-R", localServer, "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
				sprintf(syslog_para, "%s", inet_ntoa(*((struct in_addr *)tmpBuffer)));
				RunSystemCmd(NULL_FILE, "syslogd", "-L", "-R", syslog_para, NULL_STR);
#endif					
			}
		}
		else
		{
#ifdef RINGLOG /* ring log */
			sprintf(tmpBuffer, "echo %d > /var/log/log_split", LOG_SPLIT);
			system(tmpBuffer);
			sprintf(tmpBuffer, "%d", MAX_LOG_SIZE);
			sprintf(syslog_para, "%d", LOG_SPLIT);
			reinitSer_printf(LOG_DEBUG, "syslog will use %dKB for log(%d rotate, 1 original, %dKB for each)\n",
				MAX_LOG_SIZE * (LOG_SPLIT+1), LOG_SPLIT, MAX_LOG_SIZE);
			RunSystemCmd(NULL_FILE, "syslogd", "-L", "-s", tmpBuffer, "-b", syslog_para, NULL_STR);
#else
			RunSystemCmd(NULL_FILE, "syslogd", "-L", NULL_STR);
#endif
		}
		RunSystemCmd(NULL_FILE, "klogd", NULL_STR);
	}
	
	return 0;
}

int reinit_func_setSysLog(BASE_DATA_Tp data)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	clean_sys_app_log();
	start_sys_app_log();
	return 0;
}
#ifdef CONFIG_CPU_UTILIZATION
int reinit_func_setCpuUtilization(BASE_DATA_Tp data)
{	
	int enable_cpu=0;
	apmib_get(MIB_ENABLE_CPU_UTILIZATION, (void *)&enable_cpu);
	//printf("%s:%d\n",__FUNCTION__,__LINE__);
	if(enable_cpu)
	{
		//RunSystemCmd(NULL_FILE, "killall", "cpu", NULL_STR);
		//printf("%s:%d\n",__FUNCTION__,__LINE__);
		if(isFileExist(CPU_PID_FILE)==0)
		{
			//printf("%s:%d\n",__FUNCTION__,__LINE__);
			RunSystemCmd(NULL_FILE, "cpu", NULL_STR);		
		}
	}
	else
	{
		//printf("%s:%d\n",__FUNCTION__,__LINE__);
		if(isFileExist(CPU_PID_FILE)!=0)
		{
			//printf("%s:%d\n",__FUNCTION__,__LINE__);
			RunSystemCmd(NULL_FILE, "killall", "cpu", NULL_STR);
//			RunSystemCmd(NULL_FILE, "rm", "-f", "/web/*.cpudat", NULL_STR);
			system("rm -f  "CPU_PID_FILE" 2> /dev/null");
		}
	}	
	return 0;
}
#endif
MibChangeFuncItem_t mibDecisionFuncSet[]=
{
	{"",DECISION_NULL_FUNC_ID,NULL,0},
	
	{"decision_func_bridge_bind",DECISION_BRIDGE_BIND_FUNC_ID,decision_func_bridge_bind,0},
	{"decision_func_lan_ip",DECISION_LAN_IP_FUNC_ID,decision_func_lan_ip,0},
	{"decision_func_lan_mac",DECISION_LAN_MAC_FUNC_ID,decision_func_lan_mac,0},
	{"decision_func_ntp_enable",DECISION_NTP_ENABLE_FUNC_ID,decision_func_ntp_enable,0},
	{"decision_func_reinit_all",DECISION_REINIT_ALL_FUNC_ID,decision_func_reinit_all,0},
	{"decision_func_wisp_wan_id",DECISION_WISP_WAN_ID_FUNC_ID,decision_func_wisp_wan_id,0},
	{"decision_func_wlan_disable",DECISION_WLAN_DISABLE_FUNC_ID,decision_func_wlan_disable,0},
	{"decision_func_wlan_property",DECISION_WLAN_PROPERTY_FUNC_ID,decision_func_wlan_property,0},
	{"decision_func_wlan_repeater",DECISION_WLAN_REPEATER_FUNC_ID,decision_func_wlan_repeater,0},
	{"decision_func_wlan_schedule",DECISION_WLAN_SCHEDULE_FUNC_ID,decision_func_wlan_schedule,0},
	{"decision_func_wlan_app",DECISION_WLAN_APP_FUNC_ID,decision_func_wlan_app,0},
#ifdef WLAN_PROFILE
	{"decision_func_wlan_profile",DECISION_WLAN_PROFILE_FUNC_ID,decision_func_wlan_profile,0},
#endif
	{"decision_func_wlan_block_relay",DECISION_WLAN_BLOCK_RELAY_ID,decision_func_wlan_block_relay,0},
	//...
	{"",DECISION_END_FUNC_ID,NULL,0},
	{0}
};

MibChangeFuncItem_t mibReinitFuncSet[] =
{
	{"",REINIT_NULL_FUNC_ID,NULL,REINIT_FUNC_PRIORITY_MAX},
	//...
	{"reinit_func_setSysTime",REINIT_SYSTEM_TIME_FUNC_ID,reinit_func_setSysTime,REINIT_FUNC_PRIORITY_SYSTEM},
	{"reinit_func_setSysPasswd",REINIT_SYSTEM_PASSWD_FUNC_ID,reinit_func_setSysPasswd,REINIT_FUNC_PRIORITY_SYSTEM},
	{"reinit_func_setSysLog",REINIT_SYSTEM_LOG_FUNC_ID,reinit_func_setSysLog,REINIT_FUNC_PRIORITY_SYSTEM},
#ifdef CONFIG_CPU_UTILIZATION
	{"reinit_func_setCpuUtilization",REINIT_SYSTEM_CPU_UTILIZATION_FUNC_ID,reinit_func_setCpuUtilization,REINIT_FUNC_PRIORITY_SYSTEM},
#endif
	{"reinit_func_lan",REINIT_LAN_FUNC_ID,reinit_func_bridge,REINIT_FUNC_PRIORITY_LAN},
	{"reinit_func_lan_mac",REINIT_LAN_MAC_FUNC_ID,reinit_func_lan_mac,REINIT_FUNC_PRIORITY_LAN_MAC},
	{"reinit_func_lan_stp",REINIT_LAN_STP_FUNC_ID,reinit_func_lan_stp,REINIT_FUNC_PRIORITY_LAN_STP},
	{"reinit_func_lan_app_dhcp",REINIT_LAN_APP_DHCP_FUNC_ID,reinit_func_lan_app_dhcp,REINIT_FUNC_PRIORITY_LAN_APP_DHCP},
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
	{"reinit_func_lan_app_domain_query_dnrd",REINIT_LAN_APP_DOMAIN_QUERY_DNRD_FUNC_ID,reinit_func_lan_app_domain_query_dnrd,REINIT_FUNC_PRIORITY_LAN_APP_DOMAIN_QUERY_DNRD},
#endif
#ifndef STAND_ALONE_MINIUPNP
	{"reinit_func_lan_app_miniupnp",REINIT_LAN_APP_MINIUPNP_FUNC_ID,reinit_func_lan_app_miniupnp,REINIT_FUNC_PRIORITY_LAN_APP_MINIUPNP},
#endif
#ifdef CONFIG_AUTO_DHCP_CHECK
	{"reinit_func_lan_app_Auto_DHCP_Check",REINIT_LAN_APP_Auto_DHCP_CHECK_FUNC_ID,reinit_func_lan_app_Auto_DHCP_Check,REINIT_FUNC_PRIORITY_LAN_APP_Auto_DHCP_CHECK},
#endif
#ifdef SAMBA_WEB_SUPPORT
	{"reinit_func_lan_app_samba_web",REINIT_LAN_APP_SAMBA_WEB_FUNC_ID,reinit_func_lan_app_samba_web,REINIT_FUNC_PRIORITY_LAN_APP_SAMBA_WEB},
#endif
#if defined(CONFIG_APP_TR069)
	{"reinit_func_lan_app_tr069",REINIT_LAN_APP_TR069_FUNC_ID,reinit_func_lan_app_tr069,REINIT_FUNC_PRIORITY_LAN_APP_TR069},
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
	{"reinit_func_lan_app_ETH_802DOT1X",REINIT_LAN_APP_ETH_802DOT1X_FUNC_ID,reinit_func_lan_app_ETH_802DOT1X,REINIT_FUNC_PRIORITY_LAN_APP_ETH_802DOT1X},
#endif
#ifdef RTK_CAPWAP
	{"reinit_func_lan_app_capwap_app",REINIT_LAN_APP_CAPWAP_FUNC_ID,reinit_func_lan_app_capwap_app,REINIT_FUNC_PRIORITY_LAN_APP_CAPWAP},
#endif
	{"init_func_misc",INIT_MISC_FUNC_ID,init_func_misc,INIT_FUNC_PRIORITY_MISC},

	{"reinit_func_wan_app_ntp",REINIT_WANAPP_NTP_FUNC_ID,reinit_func_wan_app_ntp,REINIT_FUNC_PRIORITY_WAN},
	{"reinit_func_start_wan",REINIT_WAN_START_FUNC_ID,reinit_func_start_wan,REINIT_FUNC_PRIORITY_WAN},
#ifdef DOS_SUPPORT
	{"reinit_func_wan_dos",REINIT_WAN_DOS_FUNC_ID,reinit_func_wan_dos,REINIT_FUNC_PRIORITY_WAN_DOS},
#endif
	{"reinit_func_firewall_dmz",REINIT_FIREWALL_DMZ_ID,reinit_fun_firewall_dmz,REINIT_FUNC_PRIORITY_WAN},	
	{"reinit_func_firewall_portforward",REINIT_FIREWALL_PORTFORWARD_ID,reinit_fun_firewall_portforward,REINIT_FUNC_PRIORITY_WAN},		
	{"reinit_func_firewall_urlfilter",REINIT_FIREWALL_URLFILTER_ID,reinit_fun_firewall_urlfilter,REINIT_FUNC_PRIORITY_WAN},	
	{"reinit_func_firewall_macfilter",REINIT_FIREWALL_MACFILTER_ID,reinit_fun_firewall_macfilter,REINIT_FUNC_PRIORITY_WAN},			
	{"reinit_func_firewall_ipfilter",REINIT_FIREWALL_IPFILTER_ID,reinit_fun_firewall_ipfilter,REINIT_FUNC_PRIORITY_WAN},	
	{"reinit_func_firewall_portfilter",REINIT_FIREWALL_PORTFILTER_ID,reinit_fun_firewall_portfilter,REINIT_FUNC_PRIORITY_WAN},
	{"reinit_func_firewall_pingaccess",REINIT_FIREWALL_PINGACCESS_ID,reinit_fun_firewall_pingAccess,REINIT_FUNC_PRIORITY_WAN},	
	{"reinit_func_firewall_webaccess",REINIT_FIREWALL_WEBACCESS_ID,reinit_fun_firewall_web_access,REINIT_FUNC_PRIORITY_WAN},		
	{"reinit_func_static_route",REINIT_STAITC_ROUTE_ID,reinit_fun_static_route,REINIT_FUNC_PRIORITY_WAN},		
	{"reinit_func_wlan",REINIT_WLAN_FUNC_ID,reinit_func_wlan,REINIT_FUNC_PRIORITY_WLAN},
	{"reinit_func_wlan_block_relay",REINIT_WLAN_BLOCK_RELAY_FUNC_ID,reinit_func_wlan_block_relay,REINIT_FUNC_PRIORITY_WLAN_BLOCK_RELAY},
	{"reinit_func_wlan_schedule",REINIT_WLAN_SCHEDULE_FUNC_ID,reinit_func_wlan_schedule,REINIT_FUNC_PRIORITY_WLAN_SCHEDULE},
	{"reinit_func_wlan_app",REINIT_WLAN_APP_FUNC_ID,reinit_func_wlan_app,REINIT_FUNC_PRIORITY_WLAN_APP},
	//...
	{"",REINIT_END_FUNC_ID,NULL,REINIT_FUNC_PRIORITY_MAX},
	{0}
};




