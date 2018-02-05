#include <stdint.h>
#include <linux/atm.h>
#include <linux/atmdev.h>
#ifdef KERNEL_2630
#include <sys/sysinfo.h>
#endif

#include "prmt_wandevice.h"
#include "prmt_wancondevice.h"

#include "prmt_utility.h"
#include <linux/wireless.h>
#include <linux/netdevice.h>

#if 1//defined CONFIG_ETHWAN
struct CWMP_OP tWANEthInfCfgLeafOP = {getWANEthInfCfg, setWANEthInfCfg};
struct CWMP_PRMT tWANEthInfCfgLeafInfo[] =
{
	/*(name,			type,		flag,			op)*/
	{"Enable", eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANEthInfCfgLeafOP},
	{"Status", eCWMP_tSTRING,	CWMP_READ,		&tWANEthInfCfgLeafOP},
	{"MACAddress", eCWMP_tSTRING,	CWMP_READ,		&tWANEthInfCfgLeafOP},
	{"MaxBitRate", eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANEthInfCfgLeafOP},
	{"DuplexMode", eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANEthInfCfgLeafOP}
};

enum eWANEthInfCfgLeaf
{
	eETHWAN_Enable,
	eETHWAN_Status,
	eETHWAN_MACAddress,
	eETHWAN_MaxBitRate,
	eETHWAN_DuplexMode
};

struct  CWMP_LEAF tWANEthInfCfgLeaf[] =
{
	{ &tWANEthInfCfgLeafInfo[eETHWAN_Enable] },
	{ &tWANEthInfCfgLeafInfo[eETHWAN_Status] },
	{ &tWANEthInfCfgLeafInfo[eETHWAN_MACAddress] },
	{ &tWANEthInfCfgLeafInfo[eETHWAN_MaxBitRate] },
	{ &tWANEthInfCfgLeafInfo[eETHWAN_DuplexMode] },
	{ NULL }
};

struct CWMP_PRMT tWANEthInfCfgObjectInfo[] =
{
	/*(name,			type,		flag,			op)*/
	{"Stats", eCWMP_tOBJECT, CWMP_READ, NULL},
};

enum eWANEthInfCfgObject
{
	eWANEthInfCfg_Stats,
};

struct CWMP_OP tWANEthInfCfgStatsLeafOP = {getWANEthStatsLeaf, NULL};
struct CWMP_PRMT tWANEthInfCfgStatsLeafInfo[] =
{
	/*(name,			type,		flag,			op)*/
	{"BytesSent", eCWMP_tUINT, CWMP_READ, &tWANEthInfCfgStatsLeafOP},
	{"BytesReceived", eCWMP_tUINT, CWMP_READ, &tWANEthInfCfgStatsLeafOP},
	{"PacketsSent", eCWMP_tUINT,	CWMP_READ, &tWANEthInfCfgStatsLeafOP},
	{"PacketsReceived", eCWMP_tUINT, CWMP_READ, &tWANEthInfCfgStatsLeafOP}
};

enum eWANEthInfCfgStatsLeaf
{
	eETHWANStats_BytesSent,
	eETHWANStats_BytesReceived,
	eETHWANStats_PacketsSent,
	eETHWANStats_PacketsReceived
};

struct  CWMP_LEAF tWANEthInfCfgStatsLeaf[] =
{
	{ &tWANEthInfCfgStatsLeafInfo[eETHWANStats_BytesSent] },
	{ &tWANEthInfCfgStatsLeafInfo[eETHWANStats_BytesReceived] },
	{ &tWANEthInfCfgStatsLeafInfo[eETHWANStats_PacketsSent] },
	{ &tWANEthInfCfgStatsLeafInfo[eETHWANStats_PacketsReceived] },
	{ NULL }
};

struct CWMP_NODE tWANEthInfCfgObject[] =
{
 /*info,  					leaf,			node)*/
 {&tWANEthInfCfgObjectInfo[eWANEthInfCfg_Stats], tWANEthInfCfgStatsLeaf, NULL},
 {NULL, NULL, NULL}
};
#endif // #if defined CONFIG_ETHWAN

struct CWMP_OP tWANCmnIfCfgLeafOP = { getWANCmnIfCfg, setWANCmnIfCfg };
struct CWMP_PRMT tWANCmnIfCfgLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"EnabledForInternet",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANCmnIfCfgLeafOP},
{"WANAccessType",		eCWMP_tSTRING,	CWMP_READ,		&tWANCmnIfCfgLeafOP},
{"Layer1UpstreamMaxBitRate",	eCWMP_tUINT,	CWMP_READ,		&tWANCmnIfCfgLeafOP},
{"Layer1DownstreamMaxBitRate",	eCWMP_tUINT,	CWMP_READ,		&tWANCmnIfCfgLeafOP},
{"PhysicalLinkStatus",		eCWMP_tSTRING,	CWMP_READ,		&tWANCmnIfCfgLeafOP},
#if 0 //keith	remove
{"WANAccessProvider",		eCWMP_tSTRING,	CWMP_READ,		&tWANCmnIfCfgLeafOP},
#endif 
{"TotalBytesSent",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCmnIfCfgLeafOP},
{"TotalBytesReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCmnIfCfgLeafOP},
{"TotalPacketsSent",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCmnIfCfgLeafOP},
{"TotalPacketsReceived",	eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCmnIfCfgLeafOP}
/*MaximumActiveConnections*/
/*NumberOfActiveConnections*/
};
enum eWANCmnIfCfgLeaf
{
	eEnabledForInternet,
	eWANAccessType,
	eLayer1UpstreamMaxBitRate,
	eLayer1DownstreamMaxBitRate,
	ePhysicalLinkStatus,
#if 0 //keith	remove	
	eWANAccessProvider,
#endif	
	eTotalBytesSent,
	eTotalBytesReceived,
	eTotalPacketsSent,
	eTotalPacketsReceived
};
struct CWMP_LEAF tWANCmnIfCfgLeaf[] =
{
{ &tWANCmnIfCfgLeafInfo[eEnabledForInternet] },
{ &tWANCmnIfCfgLeafInfo[eWANAccessType] },
{ &tWANCmnIfCfgLeafInfo[eLayer1UpstreamMaxBitRate] },
{ &tWANCmnIfCfgLeafInfo[eLayer1DownstreamMaxBitRate] },
{ &tWANCmnIfCfgLeafInfo[ePhysicalLinkStatus] },
#if 0 //keith	remove
{ &tWANCmnIfCfgLeafInfo[eWANAccessProvider] },
#endif
{ &tWANCmnIfCfgLeafInfo[eTotalBytesSent] },
{ &tWANCmnIfCfgLeafInfo[eTotalBytesReceived] },
{ &tWANCmnIfCfgLeafInfo[eTotalPacketsSent] },
{ &tWANCmnIfCfgLeafInfo[eTotalPacketsReceived] },
{ NULL }
};

struct CWMP_OP tWANDevEntityLeafOP = { getWANDevEntity, NULL };
struct CWMP_PRMT tWANDevEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"WANConnectionNumberOfEntries",eCWMP_tUINT,	CWMP_READ,		&tWANDevEntityLeafOP}
};
enum eWANDevEntityLeaf
{
	eWANConnectionNumberOfEntries
};
struct CWMP_LEAF tWANDevEntityLeaf[] =
{
{ &tWANDevEntityLeafInfo[eWANConnectionNumberOfEntries] },
{ NULL }
};

struct CWMP_OP tWAN_WANConnectionDevice_OP = { NULL, objConDev };
struct CWMP_PRMT tWANDevEntityObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"WANCommonInterfaceConfig",	eCWMP_tOBJECT,	CWMP_READ,		NULL},
#if 1//defined CONFIG_ETHWAN
	{"WANEthernetInterfaceConfig",	eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
{"WANConnectionDevice",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_WANConnectionDevice_OP}
};

enum eWANDevEntityObject
{
	eWANCommonInterfaceConfig,
#if 1//defined CONFIG_ETHWAN
	eWANEthernetInterfaceConfig,
#endif
 	eWANConnectionDevice
};

struct CWMP_NODE tWANDevEntityObject[] =
{
/*info,  						leaf,			node)*/
{ &tWANDevEntityObjectInfo[eWANCommonInterfaceConfig],	tWANCmnIfCfgLeaf,	NULL},
#if 1//defined CONFIG_ETHWAN
	{ &tWANDevEntityObjectInfo[eWANEthernetInterfaceConfig],	tWANEthInfCfgLeaf,	tWANEthInfCfgObject},
#endif
{ &tWANDevEntityObjectInfo[eWANConnectionDevice],	NULL,			NULL},
{ NULL,							NULL,			NULL }
};

/*
 * According to TR-098 spec, each instance contains all objects associated with a particular physical WAN interface.
 * We only have one physical WAN interface, so hard code WANDevice.{i}. to WANDevice.1. here.
 */
struct CWMP_PRMT tWANDeviceObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"1",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
};
enum eWANDeviceObject
{
	eWANDevice1
};
struct CWMP_NODE tWANDeviceObject[] =
{
/*info,  				leaf,			node)*/
{&tWANDeviceObjectInfo[eWANDevice1],	tWANDevEntityLeaf,	tWANDevEntityObject},
{NULL,					NULL,			NULL}
};
void rtl_get_eth_status_upstream(unsigned int *portmask,char *port_status,unsigned int type)
{
	FILE *fp;
	char *file_name="/proc/rtl865x/tr069_eth_if";
	char buff[40];
	char		*strptr; 
	char		*tokptr;
	memset(buff,0,sizeof(buff));
	fp = fopen( file_name, "r" );
	if(fp){
		memset(buff,0,sizeof(buff));
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{	strptr=buff;
			tokptr = strsep(&strptr," ");
			if( tokptr!=NULL ){
				if(type==1)   //wan type
				{
					if(strncmp(tokptr,"wan",3)==0){
						tokptr = strsep(&strptr," ");
						if( tokptr!=NULL ){
							*portmask=atoi(tokptr);
							tokptr = strsep(&strptr," ");
							if( tokptr!=NULL ){
								strcpy(port_status,tokptr);
								fclose(fp);
								return ;
							}
						}
				
					}
				}
				else if(type==2){  //lan type
					if(strncmp(tokptr,"lan",3)==0){
						tokptr = strsep(&strptr," ");
						if( tokptr!=NULL ){
							*portmask=atoi(tokptr);
							tokptr = strsep(&strptr," ");
							if( tokptr!=NULL ){
								strcpy(port_status,tokptr);
								fclose(fp);
								return ;
							}
						}
				
					}
				}
			}

		}
		fclose(fp);
	}
}
int rtl_get_eth_MaxBitRate_and_DuplexMode(unsigned int wan_portmask,unsigned int get_type)
{
	FILE *fp;
	char *file_name="/proc/rtl865x/port_status";
	char buff[120];
	int i=0;
	char port_number[10];
	int check_port_status=0;
	char		*strptr; 
	char		*tokptr;
	memset(buff,0,sizeof(buff));
	memset(port_number,0,sizeof(port_number));
	fp = fopen( file_name, "r" );
	if(fp){
		for(i=0;i<5;i++)
			if(wan_portmask&(1<<i))
				break;
		
		sprintf(port_number, "Port%d",i);
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{
			if(!check_port_status){
				//sscanf(buff, "%s",port_number_get);
				strptr=buff;
				tokptr = strsep(&strptr," ");
				if(strcmp(tokptr,port_number)==0)
					check_port_status=1;

			}
			if(check_port_status){
				strptr=buff;
				tokptr = strsep(&strptr," ");
				if( tokptr!=NULL )
					if((strcmp(tokptr,port_number)!=0)&&(strncmp(tokptr,"Port",4)==0)){
						check_port_status=0;
						break;
					}
				while(tokptr!=NULL)
				{
					if(get_type==1)   //MaxBitRate
					{
						if(strncmp(tokptr,"Speed",5)==0){
							tokptr = strsep(&strptr," ");
							if(tokptr!=NULL){
								tokptr = strsep(&tokptr,"M");
								if(tokptr!=NULL){
									fclose(fp);
									return atoi(tokptr);
								}
								
							}
							
						}
					}
					else if(get_type == 2){         //DuplexMode
						if(strncmp(tokptr,"NWay",4)==0){
							tokptr = strsep(&strptr," ");
							if(tokptr!=NULL){
								if(strncmp(tokptr,"Mode",4)==0){
									tokptr = strsep(&strptr," ");
									if(tokptr!=NULL){
										if(strncmp(tokptr,"Enabled",7)==0){
											fclose(fp);
											return 2;
	
										}
									}
									
								}
								
							}
							
						}
						if(strncmp(tokptr,"Duplex",6)==0){
							tokptr = strsep(&strptr," ");
							if(tokptr!=NULL){
								if(strncmp(tokptr,"Enabled",7)==0){
									fclose(fp);
									return 1;
								}
								else{
									fclose(fp);
									return 0;
					
								}
								
							}
							
						}

					}
					tokptr = strsep(&strptr," ");

				}
	
			}
				
			
		}
	fclose(fp);
	}
	return -1;
}

#if 1//defined CONFIG_ETHWAN //#ifdef CONFIG_DEV_xDSL
int getWANEthInfCfg(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned int bs=0,br=0,ps=0,pr=0;
	char	buf[256]="";
	struct net_link_info netlink_info;
	unsigned int portnum;
	char port_status[20];
	int ret;
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp(lastname, "Enable")==0 )
	{
	#if 0
		int flags;
		getInFlags("eth1", &flags);
		*data = booldup(flags & IFF_UP);
	#endif	
	*data = booldup(1);
	}
	else if( strcmp(lastname, "Status")==0 )
	{
	#if 0
		int link_status = get_net_link_status("nas0");

		switch(link_status)
		{
			case -1:
				strcpy(buf, "Error");
				break;
			case 0:
				strcpy(buf, "NoLink");
				break;
			case 1:
				strcpy(buf, "Up");
				break;
			default:
				return ERR_9002;
		}
		*data = strdup(buf);
#endif
		*data = strdup("Up");
		
	}
	else if( strcmp(lastname, "MACAddress")==0 )
	{
		//struct in_addr inAddr;
		unsigned char *pMacAddr;
		struct sockaddr hwaddr;
		if(!getInAddr("eth1", HW_ADDR, (void *)&hwaddr))
			return ERR_9002;
		
		pMacAddr = (unsigned char *)hwaddr.sa_data;
		sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
	

		*data = strdup(buf);
#if 0
		unsigned char macadd[MAC_ADDR_LEN];
		MIB_CE_ATM_VC_T *pEntry, tmpentry;

		pEntry = &tmpentry;
		if(!mib_chain_get( MIB_ATM_VC_TBL, 0, (void*)pEntry ))
			return ERR_9002;

#ifdef CONFIG_USER_IPV6READYLOGO_ROUTER
		char wanif[IFNAMSIZ];
		int num;
		MEDIA_TYPE_T mType = MEDIA_INDEX(pEntry->ifIndex);

		ifGetName(PHY_INTF(pEntry->ifIndex), wanif, sizeof(wanif));
	#ifdef CONFIG_RTL_ALIASNAME
		TOKEN_NUM(wanif,&num);
	#else
		if (mType == MEDIA_ETH) //assured that the Wan device is nas0 not vc0
			sscanf(wanif, "nas%u", &num);
		else 	if (mType == MEDIA_ATM) //assured that the Wan device is nas0 not vc0
			sscanf(wanif, "vc%u", &num);
	#endif
#endif

		mib_get(MIB_ELAN_MAC_ADDR, (void *)macadd);
		sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", macadd[0], macadd[1], macadd[2],
#ifdef CONFIG_USER_IPV6READYLOGO_ROUTER
			macadd[3], macadd[4], macadd[5]+(num+1));
#else
			macadd[3], macadd[4], macadd[5]);
#endif
		*data = strdup(buf);
#endif
	}
	else if( strcmp(lastname, "MaxBitRate")==0 )
	{
		rtl_get_eth_status_upstream(&portnum,port_status,1);
		ret=rtl_get_eth_MaxBitRate_and_DuplexMode(portnum,1);
		if(ret < 0)
			return  ERR_9001;
		else{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d",ret);
			*data = strdup(buf);
		}
	}
	else if( strcmp(lastname, "DuplexMode")==0 )
	{
		rtl_get_eth_status_upstream(&portnum,port_status,1);
		ret=rtl_get_eth_MaxBitRate_and_DuplexMode(portnum,2);
		printf("  11ret=%d \n",ret);
		if(ret < 0)
			return  ERR_9001;
		else if(ret == 0)
			*data = strdup("Half");
		else if(ret == 1)
			*data = strdup("Full");
		else if(ret == 2)
			*data = strdup("Auto");
	
		
	}
	else
		return ERR_9005;

	return 0;
}

int setWANEthInfCfg(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	unsigned int portnum;
	char port_status[20];
	char buff[40];
	char *buf_get = data;
	int ret;
	int i;
	if( (name==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "Enable" )==0 )
	{
		int entrynum;
		int *i = data;

		if( i == NULL ) return ERR_9007;

		if( *i == 1 )
			system("ifconfig eth1 up");
		else if( *i == 0 )
			system("ifconfig eth1 down");
		else
			return ERR_9003;
	}
	else if( strcmp(lastname, "MaxBitRate")==0 )
	{
		int i;
		if( data == NULL ) return ERR_9007;
		i=atoi(data);
		rtl_get_eth_status_upstream(&portnum,port_status,1);
		ret=rtl_get_eth_MaxBitRate_and_DuplexMode(portnum,2);
		memset(buff,0,sizeof(buff));
		if(!((i==10)||(i==100)||(i==1000)||(i==10000)))
			return ERR_9005;
		if(ret<0)
			return ERR_9001;
		else if(ret == 0){
			sprintf(buff,"echo port 0x%x %d_half > /proc/rtl865x/port_status\n",portnum,i);
			system(buff);
		}
		else if(ret == 1){
			sprintf(buff,"echo port 0x%x %d_full > /proc/rtl865x/port_status\n",portnum,i);
			system(buff);
		}
		else if(ret == 2){
			sprintf(buff,"echo port 0x%x an_%dm > /proc/rtl865x/port_status\n",portnum,i);
			system(buff);
		}
	}
	else if( strcmp(lastname, "DuplexMode")==0 )
		
	{
		if (buf_get == NULL) return ERR_9007;
		rtl_get_eth_status_upstream(&portnum,port_status,1);
		ret=rtl_get_eth_MaxBitRate_and_DuplexMode(portnum,1);
		memset(buff,0,sizeof(buff));
		if(ret<0)
			return ERR_9001;
		else{
			if(strcmp(buf_get, "Half") == 0){
				sprintf(buff,"echo port 0x%x %d_half > /proc/rtl865x/port_status\n",portnum,ret);
				system(buff);
			}
			else if(strcmp(buf_get, "Full") == 0){
				sprintf(buff,"echo port 0x%x %d_full > /proc/rtl865x/port_status\n",portnum,ret);
				system(buff);

			}
			else if(strcmp(buf_get, "Auto") == 0){
				sprintf(buff,"echo port 0x%x an_%dm > /proc/rtl865x/port_status\n",portnum,ret);
				system(buff);
			}
			else
				return ERR_9005;
		}	
	}
	else
	{
		return ERR_9005;
	}
	return 0;
}

int getWANEthStatsLeaf(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned int bs=0,br=0,ps=0,pr=0;
	struct user_net_device_stats nds;

	//if(getStats((char (* const)[IFNAMSIZ])"eth1", &nds) < 0)
	//if(get_net_device_stats((char (* const)[IFNAMSIZ])"nas0", 1, &nds) < 0)
	if(getStats("eth1", &nds) < 0)
		return ERR_9002;


	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp(lastname, "BytesSent")==0 )
	{
		*data = uintdup(nds.tx_bytes);
	}
	else if( strcmp(lastname, "BytesReceived")==0 )
	{
		*data = uintdup(nds.rx_bytes);
	}
	else if( strcmp(lastname, "PacketsSent")==0 )
	{
		*data = uintdup(nds.tx_packets);
	}
	else if( strcmp(lastname, "PacketsReceived")==0 )
	{
		*data = uintdup(nds.rx_packets);
	}
	else
		return ERR_9005;

	return 0;
}
#endif // #elif defined CONFIG_ETHWAN

int getWANCmnIfCfg(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	buf[256]="";
#if 1//defined CONFIG_ETHWAN
	struct user_net_device_stats nds;
	struct net_link_info netlink_info;

	if(getStats((char *)"eth1", &nds) < 0)
	//if(get_net_device_stats((char (* const)[IFNAMSIZ])"nas0", 1, &nds) < 0)
		return ERR_9002;
#endif
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "EnabledForInternet" )==0 )
	{
		*data = booldup( 1 );
	}else if( strcmp( lastname, "WANAccessType" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = strdup( "Ethernet" );
#endif
	}else if( strcmp( lastname, "Layer1UpstreamMaxBitRate" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
//BRAD DEBUG
	//	if(get_net_link_info("eth1", &netlink_info))
		//	return ERR_9002;
		
		//*data = uintdup(netlink_info.speed*1000000);
#if defined(CONFIG_RTL_8367R_SUPPORT)||defined(CONFIG_RTL_8198C)
		*data = uintdup(1000000000);
#else
		*data = uintdup(100000000);
#endif
#endif
	}else if( strcmp( lastname, "Layer1DownstreamMaxBitRate" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
//BRAD DEBUG
		//if(get_net_link_info("nas0", &netlink_info))
		//	return ERR_9002;
		
		//*data = uintdup(netlink_info.speed*1000000);
#if defined(CONFIG_RTL_8367R_SUPPORT)||defined(CONFIG_RTL_8198C)
		*data = uintdup(1000000000);
#else
		*data = uintdup(100000000);
#endif
#endif
	}else if( strcmp( lastname, "PhysicalLinkStatus" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
			*data = strdup( "Up" );
#endif
	}else if( strcmp( lastname, "WANAccessProvider" )==0 )
	{
			*data = strdup( "" );
	}else if( strcmp( lastname, "TotalBytesSent" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(nds.tx_bytes);
#endif
	}else if( strcmp( lastname, "TotalBytesReceived" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(nds.rx_bytes);
#endif
	}else if( strcmp( lastname, "TotalPacketsSent" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(nds.tx_packets);
#endif
	}else if( strcmp( lastname, "TotalPacketsReceived" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(nds.rx_packets);
#endif
	}else{
		return ERR_9005;
	}
	
	return 0;
}


int setWANCmnIfCfg(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "EnabledForInternet" )==0 )
	{
		int *i = data;

		if( i==NULL ) return ERR_9007;
		if( *i!= 1 ) return ERR_9001;
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int getWANDevEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "WANConnectionNumberOfEntries" )==0 )
	{
		int instnum;
        /*
         * According to TR-098 spec of InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.: 
         *    In the case of Ethernet (interface or link), only one WANConnectionDevice instance is supported.
         */
		instnum = 1;
		*data = uintdup( instnum ); 
	}else{
		return ERR_9005;
	}
	
	return 0;
}




