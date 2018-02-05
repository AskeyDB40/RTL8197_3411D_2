#include "reinitSer.h"
#include "reinit_utility.h"

/*
* need to reinit all
*/
int reinit_add_reinit_func_all(BASE_DATA_Tp data)
{
	int i=REINIT_WLAN_FUNC_ID;
	while(i>=REINIT_WLAN_FUNC_ID && i<=REINIT_WLAN_END
		||i>=REINIT_SYSTEM_TIME_FUNC_ID && i<=REINIT_SYSTEM_END
		||i>=REINIT_LAN_FUNC_ID && i<=REINIT_LAN_END
		||i>REINIT_LAN_APP_BEGIN && i<=REINIT_LAN_APP_END
		||i>=REINIT_WANAPP_NTP_FUNC_ID && i<=REINIT_END_FUNC_ID
	)
	{
		if(i==REINIT_WLAN_END)
		{
			i=REINIT_SYSTEM_TIME_FUNC_ID;
			continue;
		}else
		if(i==REINIT_SYSTEM_END)
		{
			i=REINIT_LAN_FUNC_ID;
			continue;
		}else
		if(i==REINIT_LAN_END)
		{
			reinit_add_reinit_func(INIT_MISC_FUNC_ID,data);
			i=REINIT_LAN_APP_BEGIN+1;
			continue;
		}else
		if(i==REINIT_LAN_APP_END)
		{
			i=REINIT_WANAPP_NTP_FUNC_ID;
			continue;
		}
		
		if(i==REINIT_WLAN_FUNC_ID)
		{
			int j=0;
			for(j=0;j<NUM_WLAN_INTERFACE;j++)
			{
				data->wlan_idx=j;
				reinit_add_reinit_func(i,data);
			}
			data->wlan_idx=0;
			i++;
			continue;
		}
		if(i==REINIT_WAN_START_FUNC_ID)
		{
			int j=0;
			for(j=0;j<WANIFACE_NUM;j++)
			{				
				data->wan_idx=j+1;
				reinit_add_reinit_func(i,data);
			}
			data->wan_idx=0;
			i++;
			continue;
		}
		reinit_add_reinit_func(i,data);
		i++;
	}
}

/*
* need to reinit all lan app
*/
int reinit_add_reinit_func_lan_app_all(BASE_DATA_Tp data)
{
	int i=0;
	for(i=REINIT_LAN_APP_BEGIN+1;i<REINIT_LAN_APP_END;i++)
		reinit_add_reinit_func(i,data);
}

int decision_func_ntp_enable(BASE_DATA_Tp data)
{
	//MibChangeNode_tp pMibChangeNode=(MibChangeNode_tp)data;
	//if need, use this node
	
	int ntpEnable_new_value=0,ntpEnable_old_value=0;

	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	if(is_wan_connected()<=0)
	{
		reinitSer_printf(LOG_INFO,"Wan not connected, needn't reinit ntp!\n");
		return 0;
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
#if 1
	//reinit_add_reinit_func(REINIT_SYSTEM_TIME_FUNC_ID,data);
	reinit_add_reinit_func(REINIT_WANAPP_NTP_FUNC_ID,data);

#else
	apmib_get(pMibChangeNode->id,(void*)&ntpEnable_new_value);
	ntpEnable_old_value=*((int*)(pMibChangeNode->value));
	if(ntpEnable_old_value>0 && ntpEnable_new_value==0 && daemon_is_running(NTP_INRT_PID_FILE))
	{//enable to disable
		reinit_add_reinit_func(REINIT_WANAPP_NTP_FUNC_ID,pMibChangeNode);
	}
	if(ntpEnable_old_value=0 && ntpEnable_new_value>0)
	{//disable to enable
		reinit_add_reinit_func(REINIT_WANAPP_NTP_FUNC_ID,pMibChangeNode);
	}
#endif
	return 0;
}

int decision_func_reinit_all(BASE_DATA_Tp data)
{
#if 1
	reinit_add_reinit_func_all(data);
#else
	int op_mode_orig=0,op_mode=0;
	reinit_get_desionFuc_orig_mib_value(MIB_OP_MODE,(void*)&op_mode_orig,data);
	apmib_get(MIB_OP_MODE,(void*)&op_mode);

	if(op_mode==GATEWAY_MODE)
	{
		
	}
#endif
}

int decision_func_wisp_wan_id(BASE_DATA_Tp data)
{
	int op_mode=-1;
	apmib_get(MIB_OP_MODE,(void*)&op_mode);
	if(op_mode==WISP_MODE)
	{
		reinit_add_reinit_func_all(data);
	}
}

int decision_func_lan_ip(BASE_DATA_Tp data)
{
	reinit_add_reinit_func_lan_app_all(data);
}

int decision_func_bridge_bind(BASE_DATA_Tp data)
{
	reinit_add_reinit_func(REINIT_LAN_FUNC_ID,data);
	reinit_add_reinit_func(REINIT_WAN_START_FUNC_ID,data);
	
}
int decision_func_lan_mac(BASE_DATA_Tp data)
{
	reinit_add_reinit_func(REINIT_LAN_MAC_FUNC_ID,data);
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
	reinit_add_reinit_func(REINIT_LAN_APP_DOMAIN_QUERY_DNRD_FUNC_ID,data);
#endif
}
