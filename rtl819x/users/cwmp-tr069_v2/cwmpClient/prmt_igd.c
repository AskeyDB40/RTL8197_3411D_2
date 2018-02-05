#include "prmt_igd.h"
#include "prmt_deviceinfo.h"
#include "prmt_mngmtserver.h"
#include "prmt_layer3fw.h"
#include "prmt_landevice.h"
#include "prmt_ippingdiag.h"
#include "prmt_wandevice.h"
#include "prmt_layer2bridge.h"

#ifdef TIME_ZONE
#include "prmt_time.h"
#endif
#if defined( _PRMT_SERVICES_)
#include "prmt_services.h"
#endif
#ifdef _PRMT_USERINTERFACE_
#include "prmt_userif.h"
#endif 
#ifdef _PRMT_X_STD_QOS_
#include "prmt_ctcom_queuemnt.h"
#elif IP_QOS
#include "prmt_queuemnt.h"
#elif _STD_QOS_
#include "prmt_queuemnt.h"
#endif
#ifdef _PRMT_TR143_
#include "prmt_tr143.h"
#endif //_PRMT_TR143_
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
#include "prmt_captiveportal.h"
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
#include "prmt_traceroute.h"
#endif //_SUPPORT_TRACEROUTE_PROFILE_

#define LANDEVICE_NUM	1	/* one instance of LANDevice*/
#define WANDEVICE_NUM	1	/* one instance of WANDevice*/

/******DEVICESUMMARY*****************************************************************************/
//baseline profile
#ifdef _PRMT_WT107_
#define DS_PREFIX	"InternetGatewayDevice:1.4[](Baseline:2"
#else
#define DS_PREFIX	"InternetGatewayDevice:1.1[](Baseline:1"
#endif //_PRMT_WT107_

#define DS_ETH_WAN		",EthernetWAN:1"

//ethernetlan profile
#ifdef _PRMT_WT107_
#define DS_ETH		",EthernetLAN:2"
#else
#define DS_ETH		",EthernetLAN:1"
#endif //_PRMT_WT107_

//usblan profile
#ifdef _PRMT_USB_ETH_
#ifdef _PRMT_WT107_
#define DS_USB		",USBLAN:2"
#else
#define DS_USB		",USBLAN:1"
#endif //_PRMT_WT107_
#else
#define DS_USB		""
#endif //_PRMT_USB_ETH_

//wifilan profile
#ifdef WLAN_SUPPORT
#ifdef _PRMT_WT107_
#define DS_WIFI		",WiFiLAN:2"
#else
#define DS_WIFI		",WiFiLAN:1"
#endif //_PRMT_WT107_
#else
#define DS_WIFI		""
#endif //WLAN_SUPPORT

//wifiwmm profile
#ifdef _SUPPORT_WIFIWMM_PROFILE_
#define DS_WIFIWMM		",WiFiWMM:1"
#else
#define DS_WIFIWMM		""
#endif //_SUPPORT_WIFIWMM_PROFILE_

//wifiwps profile
#ifdef _SUPPORT_WIFIWPS_PROFILE_
#define DS_WIFIWPS		",WiFiWPS:1"
#else
#define DS_WIFIWPS		""
#endif //_SUPPORT_WIFIWPS_PROFILE_

//qos profile
#ifdef IP_QOS
#ifdef _PRMT_WT107_
#define DS_QOS		",QoS:2"
#else
#define DS_QOS		",QoS:1"
#endif //_PRMT_WT107_
#else
#define DS_QOS		""
#endif //IP_QOS

//time profile
#ifdef TIME_ZONE
#ifdef _PRMT_WT107_
#define DS_TIME		",Time:2"
#else
#define DS_TIME		",Time:1"
#endif //_PRMT_WT107_
#else
#define DS_TIME		""
#endif

//captiveportal profile
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
#define DS_CAPTIVEPORTAL	",CaptivePortal:1"
#else
#define DS_CAPTIVEPORTAL	""
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_

//ipping profile
#define DS_PING		",IPPing:1"

//traceroute profile
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
#define DS_TRACEROUTE	",TraceRoute:1"
#else
#define DS_TRACEROUTE	""
#endif //_SUPPORT_TRACEROUTE_PROFILE_

//download,downloadtcp,upload,uploadtcp,udpecho,udpechoplus profiles
#ifdef _PRMT_TR143_
#define DS_TR143	",Download:1,DownloadTCP:1,Upload:1,UploadTCP:1,UDPEcho:1,UDPEchoPlus:1"
#else
#define DS_TR143	""
#endif //_PRMT_TR143_
 
//deviceassociation profile
#ifdef TR069_ANNEX_G
#ifdef _PRMT_WT107_
#define DS_TR111	",DeviceAssociation:2"
#else
#define DS_TR111	",DeviceAssociation:1"
#endif //_PRMT_WT107_
#else
#define DS_TR111	""
#endif //TR069_ANNEX_G

//dhcpcondserving profile
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
#define DS_DHCPCONDSERV	",DHCPCondServing:1"
#else
#define DS_DHCPCONDSERV	""
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_

//dhcpoption profile
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
#define DS_DHCPOPTION	",DHCPOption:1"
#else
#define DS_DHCPOPTION	""
#endif //_PRMT_X_TELEFONICA_ES_DHCPOPTION_

#define DS_END		")"
#define DEVICESUMMARY	DS_PREFIX DS_ETH_WAN DS_ETH DS_USB DS_WIFI DS_WIFIWMM DS_WIFIWPS \
			DS_QOS DS_TIME DS_CAPTIVEPORTAL \
			DS_PING DS_TRACEROUTE DS_TR143  \
			DS_TR111 DS_DHCPCONDSERV DS_DHCPOPTION DS_END
/*******end DEVICESUMMARY****************************************************************************/


/******LANConfigSecurity***************************************************************************/
struct CWMP_OP tLANConfigSecurityLeafOP = { getLANConfSec,	setLANConfSec };
struct CWMP_PRMT tLANConfigSecurityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"ConfigPassword",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLANConfigSecurityLeafOP },
};
enum eLANConfigSecurityLeaf
{
	eConfigPassword,
};
struct CWMP_LEAF tLANConfigSecurityLeaf[] =
{
{ &tLANConfigSecurityLeafInfo[eConfigPassword] },
{ NULL	}
};

#ifdef _PRMT_CAPABILITIES_
/******Capabilities**********************************************************************************/
struct CWMP_PRMT tCapabilitiesObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
#ifdef _PRMT_TR143_
{"PerformanceDiagnostic",	eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif //_PRMT_TR143_
};
enum eCapabilitiesObject
{
#ifdef _PRMT_TR143_
	ePerformanceDiagnostic,
#endif //_PRMT_TR143_
	eCapabilities_END /*the last one*/
};
struct CWMP_NODE tCapabilitiesObject[] =
{
/*info,  						leaf,				node)*/
#ifdef _PRMT_TR143_
{&tCapabilitiesObjectInfo[ePerformanceDiagnostic],	tPerformanceDiagnosticLeaf,	NULL},
#endif //_PRMT_TR143_
{NULL,							NULL,				NULL}
};
#endif //_PRMT_CAPABILITIES_

/******IGD*****************************************************************************************/
struct CWMP_OP tIGDLeafOP = { getIGD, NULL };
struct CWMP_PRMT tIGDLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"LANDeviceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tIGDLeafOP},
{"WANDeviceNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tIGDLeafOP},
{"DeviceSummary",		eCWMP_tSTRING,	CWMP_READ,		&tIGDLeafOP},
};
enum eIGDLeaf
{
	eLANDeviceNumberOfEntries,
	eWANDeviceNumberOfEntries,
	eDeviceSummary
};
struct CWMP_LEAF tIGDLeaf[] =
{
{ &tIGDLeafInfo[eLANDeviceNumberOfEntries]  },
{ &tIGDLeafInfo[eWANDeviceNumberOfEntries]  },
{ &tIGDLeafInfo[eDeviceSummary]  },
{ NULL	}
};
struct CWMP_PRMT tIGDObjectInfo[] =
{
/*(name,			type,		flag,		op)*/
{"DeviceInfo",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
#ifdef _PRMT_DEVICECONFIG_
{"DeviceConfig",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_PRMT_DEVICECONFIG_
{"ManagementServer",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#ifdef TIME_ZONE
{"Time",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif /*TIME_ZONE*/
#ifdef _PRMT_USERINTERFACE_
{"UserInterface",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif /*_PRMT_USERINTERFACE_*/
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
{"CaptivePortal",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
{"Layer3Forwarding",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#ifdef _PRMT_X_STD_QOS_
{"QueueManagement",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#elif IP_QOS
{"QueueManagement",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#elif _STD_QOS_
{"QueueManagement",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif
{"LANConfigSecurity",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"IPPingDiagnostics",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
{"TraceRouteDiagnostics",	eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_SUPPORT_TRACEROUTE_PROFILE_
{"LANDevice",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
{"LANInterfaces",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif
{"Layer2Bridging",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
/*ping_zhang:20081217 END*/
{"WANDevice",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
#ifdef _PRMT_SERVICES_
{"Services",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_PRMT_SERVICES_
#ifdef _PRMT_CAPABILITIES_
{"Capabilities",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_PRMT_CAPABILITIES_
#ifdef _PRMT_TR143_
{"DownloadDiagnostics",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"UploadDiagnostics",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"UDPEchoConfig",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif //_PRMT_TR143_
};
enum eIGDObject
{
	eDeviceInfo,
#ifdef _PRMT_DEVICECONFIG_
	eDeviceConfig,
#endif //_PRMT_DEVICECONFIG_
	eManagementServer,
#ifdef TIME_ZONE
	eTime,
#endif //TIME_ZONE
#ifdef _PRMT_USERINTERFACE_
	eUserInterface,
#endif /*_PRMT_USERINTERFACE_*/
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
	eCaptivePortal,
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
	eLayer3Forwarding,
#ifdef _PRMT_X_STD_QOS_
	eQueueManagement,
#elif IP_QOS
	eQueueManagement,
#elif _STD_QOS_
	eQueueManagement,
#endif //IP_QOS
	eLANConfigSecurity,
	eIPPingDiagnostics,
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
	eTraceRouteDiagnostics,
#endif //_SUPPORT_TRACEROUTE_PROFILE_
	eLANDevice,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
	eLANInterfaces,
#endif
	eLayer2Bridging,
/*ping_zhang:20081217 END*/
	eWANDevice,
#ifdef _PRMT_SERVICES_
	eServices,
#endif //_PRMT_SERVICES_
#ifdef _PRMT_CAPABILITIES_
	eCapabilities,
#endif //_PRMT_CAPABILITIES_
#ifdef _PRMT_TR143_
	eDownloadDiagnostics,
	eUploadDiagnostics,
	eUDPEchoConfig,
#endif //_PRMT_TR143_
};
struct CWMP_NODE tIGDObject[] =
{
/*info,  				leaf,			next)*/
{&tIGDObjectInfo[eDeviceInfo],		tDeviceInfoLeaf,	tDeviceInfoObject},
#ifdef _PRMT_DEVICECONFIG_
{&tIGDObjectInfo[eDeviceConfig],	tDeviceConfigLeaf,	NULL},
#endif //_PRMT_DEVICECONFIG_
{&tIGDObjectInfo[eManagementServer],	tManagementServerLeaf,	tManagementServerObject},
#ifdef TIME_ZONE
{&tIGDObjectInfo[eTime],		tTimeLeaf,		NULL},
#endif //TIME_ZONE
#ifdef _PRMT_USERINTERFACE_
{&tIGDObjectInfo[eUserInterface],	tUserIFLeaf,		NULL},
#endif /*_PRMT_USERINTERFACE_*/
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
{&tIGDObjectInfo[eCaptivePortal],	tCaptivePortalLeaf,	NULL},
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
{&tIGDObjectInfo[eLayer3Forwarding],	tLayer3ForwardingLeaf,	tLayer3ForwardingObject},
#ifdef _PRMT_X_STD_QOS_
{&tIGDObjectInfo[eQueueManagement],	tQueueMntLeaf,		tQueueMntObject},
#elif IP_QOS
{&tIGDObjectInfo[eQueueManagement],	tQueueMntLeaf,		tQueueMntObject},
#elif _STD_QOS_
{&tIGDObjectInfo[eQueueManagement],	tQueueMntLeaf,		tQueueMntObject},
#endif //IP_QOS
{&tIGDObjectInfo[eLANConfigSecurity],	tLANConfigSecurityLeaf,	NULL},
{&tIGDObjectInfo[eIPPingDiagnostics],	tIPPingDiagnosticsLeaf,	NULL},
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
{&tIGDObjectInfo[eTraceRouteDiagnostics],tTraceRouteDiagLeaf,	tTraceRouteDiagObject},
#endif //_SUPPORT_TRACEROUTE_PROFILE_
{&tIGDObjectInfo[eLANDevice],		NULL,			tLANDeviceObject},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
{&tIGDObjectInfo[eLANInterfaces],	tLANInterfacesLeaf,			NULL},
#endif
{&tIGDObjectInfo[eLayer2Bridging],		tLayer2BridgingEntityLeaf,		tLayer2BridgingObject},
/*ping_zhang:20081217 END*/
{&tIGDObjectInfo[eWANDevice],		NULL,			tWANDeviceObject},
#ifdef _PRMT_SERVICES_
{&tIGDObjectInfo[eServices],		NULL,			tServicesObject},
#endif //_PRMT_SERVICES_
#ifdef _PRMT_CAPABILITIES_
{&tIGDObjectInfo[eCapabilities],	NULL,			tCapabilitiesObject},
#endif //_PRMT_CAPABILITIES_
#ifdef _PRMT_TR143_
{&tIGDObjectInfo[eDownloadDiagnostics],	tDownloadDiagnosticsLeaf,NULL},
{&tIGDObjectInfo[eUploadDiagnostics],	tUploadDiagnosticsLeaf,	NULL},
{&tIGDObjectInfo[eUDPEchoConfig],	tUDPEchoConfigLeaf,	NULL},
#endif //_PRMT_TR143_
{NULL,					NULL,			NULL}
};

/******Root*****************************************************************************************/
struct CWMP_PRMT tROOTObjectInfo[] =
{
/*(name,			type,		flag,		op)*/
{"InternetGatewayDevice",	eCWMP_tOBJECT,	CWMP_READ,	NULL}
};
enum eROOTObject
{
	eInternetGatewayDevice
};
struct CWMP_NODE tROOT[] =
{
/*info, 	 				leaf,			next*/
{&tROOTObjectInfo[eInternetGatewayDevice],	tIGDLeaf,		tIGDObject	},
{NULL,						NULL,			NULL		}
};

/***********************************************************************************************/

int getIGD(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "LANDeviceNumberOfEntries" )==0 )
	{
		*data = uintdup( LANDEVICE_NUM ); 
	}else if( strcmp( lastname, "WANDeviceNumberOfEntries" )==0 )
	{
		*data = uintdup( WANDEVICE_NUM ); 
	}else if( strcmp( lastname, "DeviceSummary" )==0 )
	{
		*data = strdup( DEVICESUMMARY );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int getLANConfSec(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "ConfigPassword" )==0 )
	{
#if 1
		*data = strdup( "" ); /*return an empty string*/
#else
		char buf[65];
	#ifdef TELEFONICA_DEFAULT_CFG
		mib_get(MIB_SUSER_PASSWORD, (void *)buf);
	#else
		mib_get(CWMP_LAN_CONFIGPASSWD, (void *)buf);
	#endif //TELEFONICA_DEFAULT_CFG
		*data = strdup( buf );
#endif
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setLANConfSec(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "ConfigPassword" )==0 )
	{
#ifdef TELEFONICA_DEFAULT_CFG
		char *buf=data;
		if(buf==NULL) return ERR_9007;
		if(strlen(buf)==0) return ERR_9001;
		if(strlen(buf)>=MAX_NAME_LEN) return ERR_9001;
		mib_set(MIB_SUSER_PASSWORD, (void *)buf);
	#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UserAccount, CWMP_RESTART, 0, NULL, 0 );
		return 0;
	#else
		return 1;
	#endif //_CWMP_APPLY_
#else
		char *buf=data;
		int  len=0;
		if( buf ) len = strlen( buf );
		if( len == 0 ){
			mib_set( MIB_USER_PASSWORD, (void *)"");
			mib_set(MIB_USER_NAME, (void *)"");
		}	
		else if( len < MAX_NAME_LEN ){
			mib_set( MIB_USER_PASSWORD, (void *)buf);
			mib_set(MIB_USER_NAME, (void *)buf);
		}
		else
			return ERR_9007;
		
		return 1;
#endif //TELEFONICA_DEFAULT_CFG
	}else{
		return ERR_9005;
	}
	
	return 0;

}
