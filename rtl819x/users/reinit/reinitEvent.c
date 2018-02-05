#include "reinitSer.h"
#include "reinitEvent.h"
#include "reinitHandle.h"
#include "reinitWan_confliction.h"

int reinit_event_process(int eventId,char*data,int dataLen,char*msg)
{
    int i=0;
    int retVal=0;

    if(eventId<REINIT_EVENT_BEGIN||eventId>REINIT_EVENT_END||!data)
    {
        reinitSer_printf(LOG_EMERG,"%s:%d Invalid input!\n",__FUNCTION__,__LINE__);
        return -1;
            
    }
    while(eventHandleFuncSet[i].eventId)
    {
    	if(eventId==eventHandleFuncSet[i].eventId)
    	{
        	retVal=eventHandleFuncSet[i].event_handle(data,dataLen,msg);
        	if(retVal<0)
            	return -1;
    	}
		i++;
    }
    sprintf(msg,"%s","OK");
    reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
    return 0;
}

int reinit_event_wan_power_on(char * data,int dataLen,char *errmsg)
{
	//printf("%s.%d.data(%s).datalen(%d)\n",__FUNCTOIN__,__LINE__,data,dataLen);
	wan_power_on();	
}

int format_wan_event_para(BASE_DATA_Tp pbase,char *data,int dataLen)
{
	char *token=NULL;
	char *savestr=NULL;
	WAN_DATA_Tp	wan_p = pbase;

	token=strtok_r(data,",",&savestr);
	while(token != NULL)
	{
		printf("%s:%d ###%s\n",__FUNCTION__,__LINE__,token);
		if(strncmp("wan_index=",token,strlen("wan_index="))==0)
		{
			wan_p->base.wan_idx = strtol(token+strlen("wan_index="),NULL,16);
		}
		if(strncmp("ip=",token,strlen("ip="))==0)
		{
			strcpy(wan_p->wan_data.ip_addr,(token+strlen("ip=")));
		}
		if(strncmp("subnet=",token,strlen("subnet="))==0)
		{
			strcpy(wan_p->wan_data.sub_net,(token+strlen("subnet=")));
		}
		if(strncmp("router=",token,strlen("router="))==0)
		{
			strcpy(wan_p->wan_data.gw_addr,(token+strlen("router=")));
		}	
		if(strncmp("dns=",token,strlen("dns="))==0)
		{
			strcpy(wan_p->wan_data.dns_addr,(token+strlen("dns=")));
		}
		token=strtok_r(NULL,",",&savestr);
	}
}

int format_lan_event_para(BASE_DATA_Tp pbase,char *data,int dataLen)
{
	char *token=NULL;
	char *savestr=NULL;
	LAN_DATA_Tp	lan_p = pbase;

	token=strtok_r(data,",",&savestr);
	while(token != NULL)
	{
		if(strncmp("lan_index=",token,strlen("lan_index="))==0)
		{
			lan_p->base.lan_idx = strtol(token+strlen("lan_index="),NULL,16);
		}
		if(strncmp("ip=",token,strlen("ip="))==0)
		{
			strcpy(lan_p->lan_data.ip_addr,(token+strlen("ip=")));
		}
		if(strncmp("subnet=",token,strlen("subnet="))==0)
		{
			strcpy(lan_p->lan_data.sub_net,(token+strlen("subnet=")));
		}
		if(strncmp("router=",token,strlen("router="))==0)
		{
			strcpy(lan_p->lan_data.gw_addr,(token+strlen("router=")));
		}	
		if(strncmp("dns=",token,strlen("dns="))==0)
		{
			strcpy(lan_p->lan_data.dns_addr,(token+strlen("dns=")));
		}
		token=strtok_r(NULL,",",&savestr);
	}

	return 0;
}

int reinit_event_wan_connect(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;
	printf("%s.%d. wan_connect #######\n",__FUNCTION__,__LINE__);
	printf("data(%s), len(%d)######\n",data,dataLen);
	format_wan_event_para(&wan_info,data,dataLen);
//	printf("###wan_index(%d)\n",wan_info.base.wan_idx);
//	printf("###ip_addr(%s)\n",wan_info.wan_data.ip_addr);	
//	printf("###sub_net(%s)\n",wan_info.wan_data.sub_net);	
//	printf("###gw_addr(%s)\n",wan_info.wan_data.gw_addr);	
//	printf("###dns_addr(%s)\n",wan_info.wan_data.dns_addr);	

//check lan and wan ip confliction
	check_wan_confliction(wan_info.wan_data.ip_addr,wan_info.wan_data.sub_net);

	func_connect_wan(&wan_info);
}

int reinit_event_lan_connect(char * data,int dataLen,char *errmsg)
{
	LAN_DATA_T	lan_info;
	printf("%s.%d. lan_connect #######\n",__FUNCTION__,__LINE__);
	printf("data(%s), len(%d)######\n",data,dataLen);
	format_lan_event_para(&lan_info,data,dataLen);
	func_connect_lan(&lan_info);
}
int reinit_event_dhcp_connect(char * data,int dataLen,char *errmsg)
{
	LAN_DATA_T	lan_info;
	printf("data(%s), len(%d)######\n",data,dataLen);
	format_lan_event_para(&lan_info,data,dataLen);
	func_lan_dhcp_connect(&lan_info);	
}

int reinit_event_wan_disconnect(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. wan_disconnect #######\n",__FUNCTION__,__LINE__);
	format_wan_event_para(&wan_info,data,dataLen);	
	func_disconnect_wan(&wan_info);
}
int reinit_event_wan_firewall(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);		
	printf("data(%s), len(%d)######\n",data,dataLen);
	format_wan_event_para(&wan_info,data,dataLen);
	func_set_firewall_rules(&wan_info);
}
int reinit_event_pppoe_start(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_pppoe_start(&wan_info);
}

int reinit_event_pppoe_exit(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_pppoe_exit(&wan_info);
}

int reinit_event_pptp_start(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_pptp_start(&wan_info);
}

int reinit_event_pptp_exit(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_pptp_exit(&wan_info);
}
int reinit_event_l2tp_start(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_l2tp_start(&wan_info);
}

int reinit_event_l2tp_exit(char * data,int dataLen,char *errmsg)
{
	WAN_DATA_T	wan_info;	
	printf("%s.%d. set firewall #######\n",__FUNCTION__,__LINE__);	
	format_wan_event_para(&wan_info,data,dataLen);
	func_l2tp_exit(&wan_info);
}


EventHandleFuncItem_t eventHandleFuncSet[]=
{
    {REINIT_EVENT_MIBCHANGE,reinit_event_mibChange},
    {REINIT_EVENT_APPLYCAHNGES,reinit_event_applyChanges},
  	{REINIT_EVENT_WAN_POWER_ON,reinit_event_wan_power_on},
	{REINIT_EVENT_WAN_CONNECT,reinit_event_wan_connect},
	{REINIT_EVENT_WAN_DISCONNECT,reinit_event_wan_disconnect},
	{REINIT_EVENT_FIREWALL,reinit_event_wan_firewall}, 
    {REINIT_EVENT_PPPOE_START,reinit_event_pppoe_start},
	{REINIT_EVENT_PPPOE_EXIT,reinit_event_pppoe_exit},
    {REINIT_EVENT_LAN_CONNECT,reinit_event_lan_connect},
    {REINIT_EVENT_LAN_DHCP_CONNECT,reinit_event_dhcp_connect},
    {REINIT_EVENT_PPTP_START,reinit_event_pptp_start},
	{REINIT_EVENT_PPTP_EXIT,reinit_event_pptp_exit},
    {REINIT_EVENT_L2TP_START,reinit_event_l2tp_start},
	{REINIT_EVENT_L2TP_EXIT,reinit_event_l2tp_exit},	
    {0}
};
