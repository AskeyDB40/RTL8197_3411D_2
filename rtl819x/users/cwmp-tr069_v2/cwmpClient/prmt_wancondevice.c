#include <stdint.h>
#include <signal.h>
#include <linux/atm.h>
#include <linux/atmdev.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>


#include "prmt_wancondevice.h"

//#undef _PRMT_X_TELEFONICA_ES_DHCPOPTION_

#include "mibtbl.h" //keith add.
#include "prmt_utility.h" //keith add.
#include <sys/stat.h> //keith add.
#include <linux/wireless.h>
		extern char WLAN_IF[];
		extern char WAN_IF[];
#if defined(_PRMT_X_CT_COM_DDNS_)
#include "prmt_ddns.h"
#endif
#ifdef WLAN_SUPPORT
#include "prmt_landevice_wlan.h"
#endif

/*old: all the same rip version, new: different rip version for different WAN*/
#define _USE_NEW_RIP_

extern int getInterfaceStat(
	char *ifname,
	unsigned long *bs,
	unsigned long *br,
	unsigned long *ps,
	unsigned long *pr );
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
extern int getInterfaceStat1(
	char *ifname,
	unsigned long *es, unsigned long *er,
	unsigned long *ups, unsigned long *upr, 
	unsigned long *dps, unsigned long *dpr, 
	unsigned long *mps, unsigned long *mpr, 
	unsigned long *bps, unsigned long *bpr, 
	unsigned long *uppr);
//#endif
/*ping_zhang:20081217 END*/
extern int getChainID( struct CWMP_LINKNODE *ctable, int num );

//int getObjectID( struct sCWMP_ENTITY *ctable, int chainid );
unsigned char getNewIfIndex(int cmode);
unsigned int getWANConDevInstNum( char *name );
unsigned int getWANPPPConInstNum( char *name );
unsigned int getWANIPConInstNum( char *name );
unsigned int getWANPortMapInstNum( char *name );
unsigned int getInstNum( char *name, char *objname );

/*****port mapping api****/
unsigned int getPortMappingMaxInstNum(int wanIf_idx);
int getPortMappingCount(int *portEntityCount);
#ifdef MULTI_WAN_SUPPORT
unsigned int getPortMappingTotalNum(int wan_num);
#endif

#ifdef VIRTUAL_SERVER_SUPPORT
int getPortMappingByID( unsigned char ifindex, int id, vtlsvr_entryx *c, unsigned int *chainID );
int getPortMappingByInstNum( unsigned char ifindex, unsigned int instnum, vtlsvr_entryx *c, unsigned int *chainID );
#else
int getPortMappingByID( unsigned char ifindex, int id, PORTFW_T *c, unsigned int *chainID );
int getPortMappingByInstNum( unsigned char ifindex, unsigned int instnum, PORTFW_T *c, unsigned int *chainID );
#endif
/*****endi port mapping api****/

/*ppp utilities*/
int getPPPConStatus( char *pppname, char *status ); 
int getPPPEchoRetry( char *pppname, int ppptype, unsigned int *retry ); 

#if 1//defined(CONFIG_ETHWAN)
int getWANETHLINKCONF(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setWANETHLINKCONF(char *name, struct CWMP_LEAF *entity, int type, void *data);
#endif 


struct CWMP_OP tWANCONSTATSLeafOP = { getWANCONSTATS, NULL };
struct CWMP_PRMT tWANCONSTATSLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"EthernetBytesSent",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetBytesReceived",	eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetPacketsSent",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetPacketsReceived",	eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
{"EthernetErrorsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetErrorsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetUnicastPacketsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetUnicastPacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetDiscardPacketsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetDiscardPacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetMulticastPacketsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetMulticastPacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetBroadcastPacketsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetBroadcastPacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP},
{"EthernetUnknownProtoPacketsReceived",	eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANCONSTATSLeafOP}
//#endif
/*ping_zhang:20081217 END*/
};
enum eWANCONSTATSLeaf
{
	eEthernetBytesSent,
	eEthernetBytesReceived,
	eEthernetPacketsSent,
	eEthernetPacketsReceived,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
	eEthernetErrorsSent,
	eEthernetErrorsReceived,
	eEthernetUnicastPacketsSent,
	eEthernetUnicastPacketsReceived,
	eEthernetDiscardPacketsSent,
	eEthernetDiscardPacketsReceived,
	eEthernetMulticastPacketsSent,
	eEthernetMulticastPacketsReceived,
	eEthernetBroadcastPacketsSent,
	eEthernetBroadcastPacketsReceived,
	eEthernetUnknownProtoPacketsReceived
//#endif
/*ping_zhang:20081217 END*/
};
struct CWMP_LEAF tWANCONSTATSLeaf[] =
{
{ &tWANCONSTATSLeafInfo[eEthernetBytesSent] },
{ &tWANCONSTATSLeafInfo[eEthernetBytesReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetPacketsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetPacketsReceived] },
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
{ &tWANCONSTATSLeafInfo[eEthernetErrorsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetErrorsReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetUnicastPacketsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetUnicastPacketsReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetDiscardPacketsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetDiscardPacketsReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetMulticastPacketsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetMulticastPacketsReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetBroadcastPacketsSent] },
{ &tWANCONSTATSLeafInfo[eEthernetBroadcastPacketsReceived] },
{ &tWANCONSTATSLeafInfo[eEthernetUnknownProtoPacketsReceived] },
//#endif
/*ping_zhang:20081217 END*/
{ NULL }
};

/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#if 0//def _PRMT_X_TELEFONICA_ES_DHCPOPTION_
/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.DHCPClient.SentDHCPOption.{i}.*/
struct CWMP_OP tDHCPClientSentENTITYLeafOP = { getDHCPClientOptENTITY, setDHCPClientOptENTITY };
struct CWMP_PRMT tDHCPCLientSentENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tDHCPClientSentENTITYLeafOP},
{"Tag",			eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tDHCPClientSentENTITYLeafOP},
{"Value",			eCWMP_tBASE64,	CWMP_WRITE|CWMP_READ,	&tDHCPClientSentENTITYLeafOP}
};
enum eDHCPCLientSentENTITYLeaf
{
	eSentEnable,
	eSentTag,
	eSentValue
};
struct CWMP_LEAF tDHCPCLientSentENTITYLeaf[] =
{
{ &tDHCPCLientSentENTITYLeafInfo[eSentEnable] },
{ &tDHCPCLientSentENTITYLeafInfo[eSentTag] },
{ &tDHCPCLientSentENTITYLeafInfo[eSentValue] },
{ NULL }
};

struct CWMP_PRMT tDHCPClientSentObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eDHCPClientSentObjectInfo
{
	eDHCPCLientSent0
};
struct CWMP_LINKNODE tDHCPClientSentObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tDHCPClientSentObjectInfo[eDHCPCLientSent0],	tDHCPCLientSentENTITYLeaf,	NULL,		NULL,			0},
};

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.DHCPClient.ReqDHCPOption.{i}.*/
struct CWMP_OP tDHCPClientReqENTITYLeafOP = { getDHCPClientOptENTITY, setDHCPClientOptENTITY };
struct CWMP_PRMT tDHCPCLientReqENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tDHCPClientReqENTITYLeafOP},
{"Order",			eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tDHCPClientReqENTITYLeafOP},
{"Tag",			eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tDHCPClientReqENTITYLeafOP},
{"Value",			eCWMP_tBASE64,	CWMP_READ,				&tDHCPClientReqENTITYLeafOP}
};
enum eDHCPCLientReqENTITYLeaf
{
	eReqEnable,
	eReqOrder,
	eReqTag,
	eReqValue
};
struct CWMP_LEAF tDHCPCLientReqENTITYLeaf[] =
{
{ &tDHCPCLientReqENTITYLeafInfo[eReqEnable] },
{ &tDHCPCLientReqENTITYLeafInfo[eReqOrder] },
{ &tDHCPCLientReqENTITYLeafInfo[eReqTag] },
{ &tDHCPCLientReqENTITYLeafInfo[eReqValue] },
{ NULL }
};

struct CWMP_PRMT tDHCPClientReqObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eDHCPClientReqObjectInfo
{
	eDHCPCLientReq0
};
struct CWMP_LINKNODE tDHCPClientReqObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tDHCPClientReqObjectInfo[eDHCPCLientReq0],	tDHCPCLientReqENTITYLeaf,	NULL,		NULL,			0},
};
#endif
/*ping_zhang:20080919 END*/

struct CWMP_OP tPORTMAPENTITYLeafOP = { getPORMAPTENTITY, setPORMAPTENTITY };
struct CWMP_PRMT tPORTMAPENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"PortMappingEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"PortMappingLeaseDuration",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ|CWMP_DENY_ACT,&tPORTMAPENTITYLeafOP}, 
{"RemoteHost",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP}, 
{"ExternalPort",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"ExternalPortEndRange",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"InternalPort",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"PortMappingProtocol",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"InternalClient",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP},
{"PortMappingDescription",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPORTMAPENTITYLeafOP}
};
enum ePORTMAPENTITYLeaf
{
	ePortMappingEnabled,
	ePortMappingLeaseDuration,
	eRemoteHost,
	eExternalPort,
	eExternalEndPort,
	eInternalPort,
	ePortMappingProtocol,
	eInternalClient,
	ePortMappingDescription
};
struct CWMP_LEAF tPORTMAPENTITYLeaf[] =
{
{ &tPORTMAPENTITYLeafInfo[ePortMappingEnabled] },
{ &tPORTMAPENTITYLeafInfo[ePortMappingLeaseDuration] },
{ &tPORTMAPENTITYLeafInfo[eRemoteHost] },
{ &tPORTMAPENTITYLeafInfo[eExternalPort] },
{ &tPORTMAPENTITYLeafInfo[eExternalEndPort] },
{ &tPORTMAPENTITYLeafInfo[eInternalPort] },
{ &tPORTMAPENTITYLeafInfo[ePortMappingProtocol] },
{ &tPORTMAPENTITYLeafInfo[eInternalClient] },
{ &tPORTMAPENTITYLeafInfo[ePortMappingDescription] },
{ NULL }
};


struct CWMP_PRMT tWANPORTMAPObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eWANPORTMAPObject
{
	eWANPORTMAP0
};
struct CWMP_LINKNODE tWANPORTMAPObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tWANPORTMAPObjectInfo[eWANPORTMAP0],	tPORTMAPENTITYLeaf,	NULL,		NULL,			0},
};


struct CWMP_OP tWANPPPCONENTITYLeafOP = { getWANPPPCONENTITY, setWANPPPCONENTITY };
struct CWMP_PRMT tWANPPPCONENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{"Reset",				eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
#endif
/*ping_zhang:20081217 END*/
{"ConnectionStatus",		eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"PossibleConnectionTypes",	eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"ConnectionType",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{"PPPoESessionID",		eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"DefaultGateway",		eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
#endif
/*ping_zhang:20081217 END*/
{"Name",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"Uptime",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWANPPPCONENTITYLeafOP},
{"LastConnectionError",		eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 
{"AutoDisconnectTime",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP}, 
{"IdleDisconnectTime",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"WarnDisconnectDelay",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP}, 
{"RSIPAvailable",		eCWMP_tBOOLEAN,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"NATEnabled",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"Username",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"Password",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"PPPEncryptionProtocol",	eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"PPPCompressionProtocol",	eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"PPPAuthenticationProtocol",	eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 
{"ExternalIPAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"RemoteIPAddress",		eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 	
{"MaxMRUSize",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP}, 	
{"CurrentMRUSize",		eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 

{"DNSEnabled",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"DNSOverrideAllowed",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"DNSServers",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,		&tWANPPPCONENTITYLeafOP},

{"MACAddress",			eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"MACAddressOverride",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,		&tWANPPPCONENTITYLeafOP},

{"TransportType",		eCWMP_tSTRING,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"PPPoEACName",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP}, 
{"PPPoEServiceName",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"ConnectionTrigger",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"RouteProtocolRx",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
{"PPPLCPEcho",			eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 
{"PPPLCPEchoRetry",		eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}, 
#if defined(CONFIG_ETHWAN)
{"X_CT-COM_MulticastVlan", eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tWANPPPCONENTITYLeafOP},
#endif
{"ShapingBurstSize",	eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP},
{"PortMappingNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tWANPPPCONENTITYLeafOP}
};
enum eWANPPPCONENTITYLeaf
{
	ePPP_Enable,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	ePPP_Reset,
#endif
/*ping_zhang:20081217 END*/
	ePPP_ConnectionStatus,
	ePPP_PossibleConnectionTypes,
	ePPP_ConnectionType,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	ePPP_PPPoESessionID,
	ePPP_DefaultGateway,
#endif
/*ping_zhang:20081217 END*/
	ePPP_Name,
	ePPP_Uptime,
	ePPP_LastConnectionError,
	ePPP_AutoDisconnectTime,
	ePPP_IdleDisconnectTime,
	ePPP_WarnDisconnectDelay, 
	ePPP_RSIPAvailable,
	ePPP_NATEnabled,
	ePPP_Username,
	ePPP_Password,
	ePPP_PPPEncryptionProtocol,
	ePPP_PPPCompressionProtocol,
	ePPP_PPPAuthenticationProtocol, 
	ePPP_ExternalIPAddress,
	ePPP_RemoteIPAddress, 
	ePPP_MaxMRUSize, 	
	ePPP_CurrentMRUSize, 
	ePPP_DNSEnabled,
	ePPP_DNSOverrideAllowed,
	ePPP_DNSServers,
	ePPP_MACAddress,
	ePPP_MACAddressOverride,
	ePPP_TransportType,
	ePPP_PPPoEACName, 
	ePPP_PPPoEServiceName,
	ePPP_ConnectionTrigger,
	ePPP_RouteProtocolRx,
	ePPP_PPPLCPEcho, 
	ePPP_PPPLCPEchoRetry, 
#if defined(CONFIG_ETHWAN)
	ePPP_X_CTCOM_MulticastVlan,
#endif
	ePPP_ShapingBurstSize,
	ePPP_PortMappingNumberOfEntries
};
struct CWMP_LEAF tWANPPPCONENTITYLeaf[] =
{
{ &tWANPPPCONENTITYLeafInfo[ePPP_Enable] },
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{ &tWANPPPCONENTITYLeafInfo[ePPP_Reset] },
#endif
/*ping_zhang:20081217 END*/
{ &tWANPPPCONENTITYLeafInfo[ePPP_ConnectionStatus] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PossibleConnectionTypes] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_ConnectionType] },
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPoESessionID] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_DefaultGateway] },
#endif
/*ping_zhang:20081217 END*/
{ &tWANPPPCONENTITYLeafInfo[ePPP_Name] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_Uptime] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_LastConnectionError] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_AutoDisconnectTime] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_IdleDisconnectTime] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_WarnDisconnectDelay] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_RSIPAvailable] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_NATEnabled] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_Username] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_Password] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPEncryptionProtocol] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPCompressionProtocol] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPAuthenticationProtocol] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_ExternalIPAddress] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_RemoteIPAddress] }, 	
{ &tWANPPPCONENTITYLeafInfo[ePPP_MaxMRUSize] }, 	
{ &tWANPPPCONENTITYLeafInfo[ePPP_CurrentMRUSize] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_DNSEnabled] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_DNSOverrideAllowed] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_DNSServers] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_MACAddress] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_MACAddressOverride] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_TransportType] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPoEACName] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPoEServiceName] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_ConnectionTrigger] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_RouteProtocolRx] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPLCPEcho] }, 
{ &tWANPPPCONENTITYLeafInfo[ePPP_PPPLCPEchoRetry] }, 
#if defined(CONFIG_ETHWAN)
{ &tWANPPPCONENTITYLeafInfo[ePPP_X_CTCOM_MulticastVlan] },
#endif
{ &tWANPPPCONENTITYLeafInfo[ePPP_ShapingBurstSize] },
{ &tWANPPPCONENTITYLeafInfo[ePPP_PortMappingNumberOfEntries] },
{ NULL }
};

struct CWMP_OP tWAN_PortMapping_OP = { NULL, objWANPORTMAPPING };
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
struct CWMP_OP tWAN_DDNSConf_OP = { NULL, objDDNS };
#endif

struct CWMP_PRMT tWANPPPCONENTITYObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"PortMapping",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_PortMapping_OP},
{"Stats",			eCWMP_tOBJECT,	CWMP_READ,		NULL},
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
{"X_CT-COM_DDNSConfiguration",	eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_DDNSConf_OP},
#endif
};
enum eWANPPPCONENTITYObject
{
	ePPP_PortMapping,
	ePPP_Stats,
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
	ePPP_X_CTCOM_DDNSConfiguration
#endif
};
struct CWMP_NODE tWANPPPCONENTITYObject[] =
{
/*info,  							leaf,			node)*/
{&tWANPPPCONENTITYObjectInfo[ePPP_PortMapping],			NULL,			NULL},
{&tWANPPPCONENTITYObjectInfo[ePPP_Stats],			tWANCONSTATSLeaf,	NULL},
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
{&tWANPPPCONENTITYObjectInfo[ePPP_X_CTCOM_DDNSConfiguration],	NULL,			NULL},
#endif
{NULL,								NULL,			NULL}
};


struct CWMP_PRMT tWANPPPCONObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eWANPPPCONObject
{
	WANPPPCON0
};
struct CWMP_LINKNODE tWANPPPCONObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tWANPPPCONObjectInfo[WANPPPCON0],	tWANPPPCONENTITYLeaf,	tWANPPPCONENTITYObject,		NULL,			0}
};


/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
struct CWMP_OP tWANIPConDHCPClientLeafOP = { getWANIPConDHCPClientENTITY, NULL };
struct CWMP_PRMT tWANIPConDHCPClientLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"SentDHCPOptionNumberOfEntries",		eCWMP_tUINT,		CWMP_READ,	&tWANIPConDHCPClientLeafOP},
{"ReqDHCPOptionNumberOfEntries",		eCWMP_tUINT,		CWMP_READ,	&tWANIPConDHCPClientLeafOP}
};
enum eWANIPConDHCPClientLeaf
{
	eSentDHCPOptionNumberOfEntries,
	eReqDHCPOptionNumberOfEntries
};
struct CWMP_LEAF tWANIPConDHCPClientLeaf[] =
{
{ &tWANIPConDHCPClientLeafInfo[eSentDHCPOptionNumberOfEntries] },
{ &tWANIPConDHCPClientLeafInfo[eReqDHCPOptionNumberOfEntries] },
{ NULL }
};

struct CWMP_OP tDHCPClientSentOpt_OP = { NULL, objDHCPClientSentOpt};
struct CWMP_OP tDHCPClientReqOpt_OP = { NULL, objDHCPClientReqOpt};
struct CWMP_PRMT tWANIPConDHCPClientObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"SentDHCPOption",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPClientSentOpt_OP},
{"ReqDHCPOption",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,		&tDHCPClientReqOpt_OP},
};
enum eWANIPConDHCPClientObjectInfo
{
	eSentDHCPOption,
	eReqDHCPOption
};
struct CWMP_NODE tWANIPConDHCPClientObject[] =
{
/*info,  							leaf,			node)*/
{&tWANIPConDHCPClientObjectInfo[eSentDHCPOption],	NULL,	NULL},
{&tWANIPConDHCPClientObjectInfo[eReqDHCPOption],	NULL,	NULL},
{NULL,								NULL,			NULL}
};
#endif
/*ping_zhang:20080919 END*/

struct CWMP_OP tWANIPCONENTITYLeafOP = { getWANIPCONENTITY, setWANIPCONENTITY };
struct CWMP_PRMT tWANIPCONENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"Reset",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"ConnectionStatus",		eCWMP_tSTRING,	CWMP_READ,		&tWANIPCONENTITYLeafOP},
{"PossibleConnectionTypes",	eCWMP_tSTRING,	CWMP_READ,		&tWANIPCONENTITYLeafOP},
{"ConnectionType",		eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"Name",				eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP}, 
{"Uptime",				eCWMP_tUINT,		CWMP_READ|CWMP_DENY_ACT,&tWANIPCONENTITYLeafOP},
{"LastConnectionError",	eCWMP_tSTRING,		CWMP_READ,				&tWANIPCONENTITYLeafOP}, 
{"AutoDisconnectTime", 	eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP}, 
{"IdleDisconnectTime", 	eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP}, 
{"WarnDisconnectDelay", eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP}, 
{"RSIPAvailable",		eCWMP_tBOOLEAN,		CWMP_READ,				&tWANIPCONENTITYLeafOP},
{"NATEnabled",			eCWMP_tBOOLEAN,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"AddressingType",		eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"ExternalIPAddress",	eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"SubnetMask",			eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"DefaultGateway",		eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},

{"DNSEnabled",			eCWMP_tBOOLEAN,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"DNSOverrideAllowed",	eCWMP_tBOOLEAN,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"DNSServers",			eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},

{"MaxMTUSize",			eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"MACAddress",			eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"MACAddressOverride",	eCWMP_tBOOLEAN,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
#if defined(MULTI_WAN_SUPPORT)
{"VlanEnabled",			eCWMP_tBOOLEAN,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"VlanID",				eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"VlanPriority",		eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
#endif
{"ConnectionTrigger",	eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"RouteProtocolRx",		eCWMP_tSTRING,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"ShapingRate", 		eCWMP_tINT,			CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
{"ShapingBurstSize",	eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
#if defined(CONFIG_ETHWAN)
{"X_CT-COM_MulticastVlan",	eCWMP_tINT, 	CWMP_WRITE|CWMP_READ,	&tWANIPCONENTITYLeafOP},
#endif
{"PortMappingNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,			&tWANIPCONENTITYLeafOP}
};
enum eWANIPCONENTITYLeaf
{
	eIP_Enable,
	eIP_Reset,
	eIP_ConnectionStatus,
	eIP_PossibleConnectionTypes,
	eIP_ConnectionType,
	eIP_Name, 
	eIP_Uptime,
	eIP_LastConnectionError,
	eIP_AutoDisconnectTime,
	eIP_IdleDisconnectTime,
	eIP_WarnDisconnectDelay,
	eIP_RSIPAvailable,
	eIP_NATEnabled,
	eIP_AddressingType,
	eIP_ExternalIPAddress,
	eIP_SubnetMask,
	eIP_DefaultGateway,
	eIP_DNSEnabled,
	eIP_DNSOverrideAllowed,
	eIP_DNSServers,
	eIP_MaxMTUSize,
	eIP_MACAddress,
	eIP_MACAddressOverride,
#if defined(MULTI_WAN_SUPPORT)
	eIP_VlanEnabled,
	eIP_VlanID,
	eIP_VlanPriority,
#endif
	eIP_ConnectionTrigger,
	eIP_RouteProtocolRx,
	eIP_ShapingRate,
	eIP_ShapingBurstSize,
#if defined(CONFIG_ETHWAN)
	eIP_X_CTCOM_MulticastVlan,
#endif

	eIP_PortMappingNumberOfEntries
};
struct CWMP_LEAF tWANIPCONENTITYLeaf[] =
{
{ &tWANIPCONENTITYLeafInfo[eIP_Enable] },
{ &tWANIPCONENTITYLeafInfo[eIP_Reset] },
{ &tWANIPCONENTITYLeafInfo[eIP_ConnectionStatus] },
{ &tWANIPCONENTITYLeafInfo[eIP_PossibleConnectionTypes] },
{ &tWANIPCONENTITYLeafInfo[eIP_ConnectionType] },
{ &tWANIPCONENTITYLeafInfo[eIP_Name] }, 
{ &tWANIPCONENTITYLeafInfo[eIP_Uptime] },
{ &tWANIPCONENTITYLeafInfo[eIP_LastConnectionError] }, 
{ &tWANIPCONENTITYLeafInfo[eIP_AutoDisconnectTime] }, 
{ &tWANIPCONENTITYLeafInfo[eIP_IdleDisconnectTime] }, 
{ &tWANIPCONENTITYLeafInfo[eIP_WarnDisconnectDelay] }, 
{ &tWANIPCONENTITYLeafInfo[eIP_RSIPAvailable] },
{ &tWANIPCONENTITYLeafInfo[eIP_NATEnabled] },
{ &tWANIPCONENTITYLeafInfo[eIP_AddressingType] },
{ &tWANIPCONENTITYLeafInfo[eIP_ExternalIPAddress] },
{ &tWANIPCONENTITYLeafInfo[eIP_SubnetMask] },
{ &tWANIPCONENTITYLeafInfo[eIP_DefaultGateway] },
{ &tWANIPCONENTITYLeafInfo[eIP_DNSEnabled] },
{ &tWANIPCONENTITYLeafInfo[eIP_DNSOverrideAllowed] },
{ &tWANIPCONENTITYLeafInfo[eIP_DNSServers] },
{ &tWANIPCONENTITYLeafInfo[eIP_MaxMTUSize] },
{ &tWANIPCONENTITYLeafInfo[eIP_MACAddress] },
{ &tWANIPCONENTITYLeafInfo[eIP_MACAddressOverride] },
#if defined(MULTI_WAN_SUPPORT)
{ &tWANIPCONENTITYLeafInfo[eIP_VlanEnabled] },
{ &tWANIPCONENTITYLeafInfo[eIP_VlanID] },
{ &tWANIPCONENTITYLeafInfo[eIP_VlanPriority] },
#endif
{ &tWANIPCONENTITYLeafInfo[eIP_ConnectionTrigger] },
{ &tWANIPCONENTITYLeafInfo[eIP_RouteProtocolRx] },
{ &tWANIPCONENTITYLeafInfo[eIP_ShapingRate] },
{ &tWANIPCONENTITYLeafInfo[eIP_ShapingBurstSize] },
#if defined(CONFIG_ETHWAN)
{ &tWANIPCONENTITYLeafInfo[eIP_X_CTCOM_MulticastVlan] },
#endif
{ &tWANIPCONENTITYLeafInfo[eIP_PortMappingNumberOfEntries] },
{ NULL }
};

struct CWMP_PRMT tWANIPCONENTITYObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{"DHCPClient",			eCWMP_tOBJECT,	CWMP_READ,	NULL},
#endif
/*ping_zhang:20080919 END*/
{"PortMapping",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_PortMapping_OP},
{"Stats",			eCWMP_tOBJECT,	CWMP_READ,		NULL},
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
{"X_CT-COM_DDNSConfiguration",	eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_DDNSConf_OP}
#endif
};
enum eWANIPCONENTITYObject
{
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
	eIP_DHCPClient,
#endif
/*ping_zhang:20080919 END*/
	eIP_PortMapping,
	eIP_Stats,
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
	eIP_X_CTCOM_DDNSConfiguration
#endif
};
struct CWMP_NODE tWANIPCONENTITYObject[] =
{
/*info,  							leaf,			node)*/
/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
{&tWANIPCONENTITYObjectInfo[eIP_DHCPClient],			tWANIPConDHCPClientLeaf,			tWANIPConDHCPClientObject},
#endif
/*ping_zhang:20080919 END*/
{&tWANIPCONENTITYObjectInfo[eIP_PortMapping],			NULL,			NULL},
{&tWANIPCONENTITYObjectInfo[eIP_Stats],				tWANCONSTATSLeaf,	NULL},
//#if defined(_PRMT_X_CT_COM_DDNS_) && defined(CONFIG_USER_DDNS)
#if defined(_PRMT_X_CT_COM_DDNS_)
{&tWANIPCONENTITYObjectInfo[eIP_X_CTCOM_DDNSConfiguration],	NULL,			NULL},
#endif
{NULL,								NULL,			NULL}
};


struct CWMP_PRMT tWANIPCONObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eWANIPCONObject
{
	eWANIPCON0
};
struct CWMP_LINKNODE tWANIPCONObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tWANIPCONObjectInfo[eWANIPCON0],	tWANIPCONENTITYLeaf,	tWANIPCONENTITYObject,		NULL,			0}
};

#if 1//defined CONFIG_ETHWAN // #ifdef CONFIG_DEV_xDSL

struct CWMP_OP tWANETHLINKFLeafOP = { getWANETHLINKCONF, setWANETHLINKCONF };
struct CWMP_PRMT tWANETHLINKLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"EthernetLinkStatus",			eCWMP_tSTRING,	CWMP_READ,	&tWANETHLINKFLeafOP}
};
enum eWANETHLINKLeaf
{
	eEthernetLinkStatus
};
struct CWMP_LEAF tWANETHLINKLeaf[] =
{
{ &tWANETHLINKLeafInfo[eEthernetLinkStatus] },
{ NULL }
};
#endif

struct CWMP_OP tWANCONDEVENTITYLeafOP = { getWANCONDEVENTITY, NULL };
struct CWMP_PRMT tWANCONDEVENTITYLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"WANIPConnectionNumberOfEntries",eCWMP_tUINT,	CWMP_READ,		&tWANCONDEVENTITYLeafOP},
{"WANPPPConnectionNumberOfEntries",eCWMP_tUINT,	CWMP_READ,		&tWANCONDEVENTITYLeafOP}
};
enum eWANCONDEVENTITYLeaf
{
	eWANIPConnectionNumberOfEntries,
	eWANPPPConnectionNumberOfEntries
};
struct CWMP_LEAF tWANCONDEVENTITYLeaf[] =
{
{ &tWANCONDEVENTITYLeafInfo[eWANIPConnectionNumberOfEntries] },
{ &tWANCONDEVENTITYLeafInfo[eWANPPPConnectionNumberOfEntries] },
{ NULL }
};



struct CWMP_OP tWAN_WANIPConnection_OP = { NULL, objWANIPConn };
struct CWMP_OP tWAN_WANPPPConnection_OP = { NULL, objWANPPPConn };
struct CWMP_PRMT tWANCONDEVENTITYObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
#if 1//defined CONFIG_ETHWAN
{"WANEthernetLinkConfig",	eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
{"WANIPConnection",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_WANIPConnection_OP},
{"WANPPPConnection",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tWAN_WANPPPConnection_OP}
};
enum eWANCONDEVENTITYObject
{
#if 1//defined CONFIG_ETHWAN
	eWANEthernetLinkConfig,
#endif
 	eWANIPConnection,
	eWANPPPConnection
};
struct CWMP_NODE tWANCONDEVENTITYObject[] =
{
	/*info,  							leaf,			node)*/
#if 1//defined CONFIG_ETHWAN
{&tWANCONDEVENTITYObjectInfo[eWANEthernetLinkConfig],	tWANETHLINKLeaf,	NULL},
#endif
{&tWANCONDEVENTITYObjectInfo[eWANIPConnection],			NULL,			NULL},
{&tWANCONDEVENTITYObjectInfo[eWANPPPConnection],		NULL,			NULL},
{NULL,								NULL,			NULL}
};


struct CWMP_PRMT tWANCONDEVObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eWANCONDEVObject
{
	eWANCONDEV0
};
struct CWMP_LINKNODE tWANCONDEVObject[] =
{
/*info,  				leaf,			next,			sibling,		instnum)*/
{&tWANCONDEVObjectInfo[eWANCONDEV0],	tWANCONDEVENTITYLeaf,	tWANCONDEVENTITYObject,	NULL,			0},
};

#if defined(MULTI_WAN_SUPPORT)
static int transfer2IfNameWithVlan( char *name, char *ifname )
{
	struct CWMP_LEAF *e=NULL;
	WANIFACE_T *pEntry,wan_entity;
	pEntry = &wan_entity;
	
	if( (name==NULL) || ( ifname==NULL ) ) return -1;
	if( get_ParameterEntity( name, &e ) < 0 ) return -1;
	ifname[0]=0;

	if( strstr( name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.")!=NULL )
	{
		/* IPConnection */
		int ipnum;
		//printf("[%s:%d]name=%s\n", __FUNCTION__, __LINE__, name);
		ipnum = getWANIPConInstNum(name);
#if 0
		/*
		 *  1     2      3     4
		 * eth1, eth8, eth9, eth10
		 */
   		*((char *)&wan_entity) = (char)ipnum;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
			return -1;
		if(pEntry->vlan)
			sprintf(ifname, "eth%d.%d", (ipnum==1)?1:(ipnum+6), pEntry->vlanid);
		else
			sprintf(ifname, "eth%d", (ipnum==1)?1:(ipnum+6));
#else
		getWanIfName(ipnum,ifname,16);
#endif
		//printf("[%s:%d]ifname=%s\n", __FUNCTION__, __LINE__, ifname);
	}else if( strstr( name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.")!=NULL )
	{
		/* PPPConnection */
		int pppnum;
		//printf("[%s:%d]name=%s\n", __FUNCTION__, __LINE__, name);
		pppnum = getWANPPPConInstNum( name );
#if 0
		/*
		 *  1     2      3     4
		 * ppp1, ppp8, ppp9, ppp10
		 */
		sprintf(ifname, "ppp%d", (pppnum==1)?1:(pppnum+6));
#else
		getPPPIfName(pppnum,ifname,16);
#endif
		//printf("[%s:%d]ifname=%s\n", __FUNCTION__, __LINE__, ifname);
	}

	return 0;
}

static int parseDNSForMultiWan(int index, char *dns)
{
	if(!dns || index<=0 || index >WANIFACE_NUM)
		return -1;

	char filename[16] = {0};
	char buff[64] = {0};
	char nameserver[32]={0};
    char nameserver_ip[32]={0};
	int i;
	FILE *dns_fp = NULL;
#define RESOLV_CONF "/var/resolv.conf"

	for(i=0;i<APPTYPE_END;i++)
	{
		sprintf(filename,"%s_%d_%d",RESOLV_CONF,index,i);
		if(isFileExist(filename) == 0)
			continue;

		dns_fp = fopen(filename, "r");
		if(dns_fp)
		{
			bzero(dns, sizeof(dns));
			while(fgets(buff,sizeof(buff),dns_fp)!=NULL){
				
				sscanf(buff,"%s %s",nameserver,nameserver_ip);
				if(strcmp(nameserver,"nameserver")==0){
                	strcat(dns,nameserver_ip);	
					strcat(dns,",");	
                }
			}
			
			/* omit the last comma */
			dns[strlen(dns)-1] = 0x0;
			fclose(dns_fp);
			dns_fp = NULL;
			break; 
		}else
			return -1;
	}
#undef RESOLV_CONF 

	if(i==APPTYPE_END)
		return -1;
	else
		return 0;
}
#else

static int parseDNSForAutoMode(char *dns, int len)
{

	char filename[16] = {0};
	char buff[64] = {0};
	char nameserver[32]={0};
    char nameserver_ip[32]={0};
	int i;
	FILE *dns_fp = NULL;
#define RESOLV_CONF "/var/resolv.conf"
	
	if( !dns || isFileExist(RESOLV_CONF)==0)
		return -1;
	bzero(dns, len);
	
	dns_fp = fopen(RESOLV_CONF, "r");
	if(dns_fp){
		while(fgets(buff,sizeof(buff),dns_fp)!=NULL){				
			sscanf(buff,"%s %s",nameserver,nameserver_ip);
			if(strcmp(nameserver,"nameserver")==0){
				if(strlen(dns) == 0){
					snprintf(dns, len, "%s", nameserver_ip);
				}else{
					strcat(dns,",");
					strcat(dns,nameserver_ip);					
				}	    		
		    }
		}
	}else{
	#undef RESOLV_CONF 
	return -1;
	}
		
	#undef RESOLV_CONF 	
	fclose(dns_fp);
	dns_fp = NULL;
	return 1;
}

#endif

int getDNSList( char *buf )
{
	unsigned char tmp[64];
	char *zeroip="0.0.0.0";

	if( buf==NULL ) return -1;

	buf[0]=0;
	tmp[0]=0;
	getMIB2Str(MIB_DNS1, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
		strcat( buf, tmp );
	
	tmp[0]=0;
	getMIB2Str(MIB_DNS2, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}
	
	tmp[0]=0;
	getMIB2Str(MIB_DNS3, tmp);
	if( (strlen(tmp)>0) && (strcmp(tmp, zeroip)!=0) )
	{
		if( strlen(buf) > 0 )
			strcat( buf, ",");
		strcat( buf, tmp );
	}
	
	return 0;
}

int setDNSList( char *buf)
{
	char *tok1, *tok2, *tok3;
	int  ret=0;
	struct in_addr in1,in2,in3,emp_in;

	if( buf==NULL ) return -1;
	tok1 = NULL;
	tok2 = NULL;
	tok3 = NULL;

	tok1=strtok( buf, "," );
	tok2=strtok( NULL, "," );
	tok3=strtok( NULL, "," );
	if( (tok1==NULL)&&(tok2==NULL)&&(tok3==NULL) )
		return -1;
	
	if(tok1) 
		if(  inet_aton( tok1, &in1 )==0  ) ret=-1;
	if(tok2) 
		if(  inet_aton( tok2, &in2 )==0  ) ret=-1;
	if(tok3) 
		if(  inet_aton( tok3, &in3 )==0  ) ret=-1;

	memset( &emp_in, 0, sizeof(struct in_addr) );
	if(ret==0)
	{
		if( tok1!=NULL )
			mib_set(MIB_DNS1, (void *)&in1);
		else
			mib_set(MIB_DNS1, (void *)&emp_in);
	
		if(tok2!=NULL)
			mib_set(MIB_DNS2, (void *)&in2);
		else
			mib_set(MIB_DNS2, (void *)&emp_in);
	
		if(tok3!=NULL)
			mib_set(MIB_DNS3, (void *)&in3);
		else
			mib_set(MIB_DNS3, (void *)&emp_in);
	}
	return ret;
}

/* WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.DHCPClient. LEAF */
int getWANIPConDHCPClientENTITY(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	int num=0;
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "SentDHCPOptionNumberOfEntries" )==0 )
	{
		mib_get(MIB_DHCP_SERVER_OPTION_TBL_NUM, (void *)&num);
		*data = uintdup(num);
	}else if( strcmp( lastname, "ReqDHCPOptionNumberOfEntries" )==0 )
	{
		*data = uintdup(0);
	}

	return 0;
}

/* WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.DHCPClient.SentDHCPOption.{i}. */
int objDHCPClientSentOpt(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	return 0;
}

/* WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.{i}.DHCPClient.ReqDHCPOption.{i}. */
/* Currently not supported */
int objDHCPClientReqOpt(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	return 0;
}

int getPORMAPTENTITY(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned int 	devnum, mapnum, pppnum, ipnum;
	PORTFW_T port_entity;
	int num=0;
	int		port_chainid=0;
	unsigned int wanif_index = 0;
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d> \n", __FUNCTION__, __LINE__ ) );
	
	devnum  = getWANConDevInstNum( name );
	ipnum   = getWANIPConInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	mapnum  = getWANPortMapInstNum( name );
	if( (mapnum==0) || (devnum==0) || ((ipnum==0)&&(pppnum==0)) ) return ERR_9005;

	getPortMappingCount( &num );
	memset( &port_entity, 0, sizeof( PORTFW_T ) );

#ifdef MULTI_WAN_SUPPORT
	wanif_index = (ipnum!=0)?ipnum:pppnum;
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d> wan_idx = %d \n", __FUNCTION__, __LINE__, wanif_index ) );
#else
	wanif_index = 0;
#endif
	if( getPortMappingByInstNum( wanif_index, mapnum, &port_entity, &port_chainid ) )
		return ERR_9002;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "PortMappingEnabled" )==0 )
	{
		int vUint;
		mib_get( MIB_PORTFW_ENABLED, (void *)&vUint);
		*data = booldup( vUint&(port_entity.enabled) );
	}
	else if( strcmp( lastname, "PortMappingLeaseDuration" )==0 )
	{	
		*data = booldup(0);
	}else if( strcmp( lastname, "RemoteHost" )==0 )
	{	
        unsigned char remote_host[32] = "";
        sprintf(remote_host, "%d.%d.%d.%d", port_entity.rmtipAddr[0], port_entity.rmtipAddr[1],port_entity.rmtipAddr[2],port_entity.rmtipAddr[3]);
        *data = strdup(remote_host);
	}

	else if( strcmp( lastname, "ExternalPort" )==0 )
	{	
		*data=uintdup( port_entity.externelFromPort );
	}	
	else if( strcmp( lastname, "ExternalPortEndRange" )==0 )
	{	
		*data=uintdup( port_entity.externelToPort );
	}	
	else if( strcmp( lastname, "InternalPort" )==0 )
	{	
		*data=uintdup( port_entity.fromPort );
	}
	else if( strcmp( lastname, "PortMappingProtocol" )==0 )
	{	
		if( port_entity.protoType==PROTO_TCP )
			*data = strdup( "TCP" );
		else if( port_entity.protoType==PROTO_UDP )
			*data = strdup( "UDP" );
		else if (port_entity.protoType==PROTO_BOTH )
			*data = strdup("TCPandUDP");
		else /*PROTO_NONE or PROTO_ICMP*/
			*data = strdup( "" );
	}else if( strcmp( lastname, "InternalClient" )==0 )
	{	
		char *tmp;		
		tmp = inet_ntoa(*((struct in_addr *)&(port_entity.ipAddr)));
		if(tmp)
			*data = strdup( tmp ); 
		else
			*data = strdup( "" );
	}else if( strcmp( lastname, "PortMappingDescription" )==0 )
	{	
		*data = strdup( port_entity.comment );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int check_value_isValid(unsigned int wanif_idx, PORTFW_T set_entity, int set_entry_num ){
	unsigned int i, entry_num;
	PORTFW_T entry;
	apmib_get(MIB_PORTFW_TBL_NUM, (void *)&entry_num);
	
	for(i = 1; i <= entry_num; i++){
		*((unsigned int *)&entry) = i;
		apmib_get(MIB_PORTFW_TBL, (void *)&entry);
		
#ifdef MULTI_WAN_SUPPORT
		if( entry.WANIfIndex != wanif_idx)
			continue;
#endif
		if(i == set_entry_num)//skip the set_entry self
			continue;
		
		if( ((set_entity.fromPort <= entry.fromPort && set_entity.toPort >= entry.fromPort) ||
			(set_entity.fromPort >= entry.fromPort && set_entity.fromPort <= entry.toPort))&&
			(set_entity.protoType & entry.protoType)){
			return -1;
		}			       
	}

	return 1;
}


int setPORMAPTENTITY(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	unsigned int 	devnum, mapnum, pppnum, ipnum;
	char		*buf=data;
	int		port_chainid=0;
	PORTFW_T target[2], *port_ent=NULL;
	PORTFW_T port_entity;
	unsigned int wanif_index = 0;
	
	int entry_num=0;
	int ret = 0;
	
	if( (name==NULL) || (entity==NULL)) return -1;
	if( data==NULL ) return ERR_9007;

	if( entity->info->type!=type ) return ERR_9006;


	devnum  = getWANConDevInstNum( name );
	ipnum   = getWANIPConInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	mapnum  = getWANPortMapInstNum( name );
	if( (mapnum==0) || (devnum==0) || ((ipnum==0)&&(pppnum==0)) ) return ERR_9005;

	getPortMappingCount( &entry_num );
	memset( &target[0], 0, sizeof( PORTFW_T ) );
	memset( &target[1], 0, sizeof( PORTFW_T ) );
#ifdef MULTI_WAN_SUPPORT
	wanif_index = (ipnum!=0)?ipnum:pppnum;
#endif

	if( getPortMappingByInstNum( wanif_index, mapnum, &port_entity, &port_chainid ) )
		return ERR_9002;

	memcpy(&target[0], &port_entity, sizeof(PORTFW_T));

	if( strcmp( lastname, "PortMappingEnabled" )==0 )
	{
		int *vUint = data;	
		int entry_Num = 0, value = 1, j = 0;
		PORTFW_T tmp_entry;
		
		port_entity.enabled = (*vUint==0)? 0:1;

		ret = 1;
	}
	else if( strcmp( lastname, "PortMappingLeaseDuration" )==0 )
	{	
		unsigned int *i = data;
		//only support value 0
		if( *i!=0 ) return ERR_9001;
		ret = 1;
	}
	else if( strcmp( lastname, "RemoteHost" )==0 )
	{	
        if(data==NULL) return ERR_9007;

        if(!strcmp(data, ""))
            inet_aton("0.0.0.0", (struct in_addr *)&(port_entity.rmtipAddr));
        else{
			if(!inet_aton(data, (struct in_addr *)&(port_entity.rmtipAddr)))
				return ERR_9007;

		}
		ret = 1;
	}
	else if( strcmp( lastname, "ExternalPort" )==0 )
	{	

		unsigned int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i> 65535) return ERR_9007;

		port_entity.externelFromPort= *i;

		if(port_entity.externelFromPort > port_entity.externelToPort) return ERR_9007;
		
		ret = 1;
	}	
	else if( strcmp( lastname, "ExternalPortEndRange" )==0 )
	{	
		unsigned int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i> 65535) return ERR_9007;
		
		port_entity.externelToPort= *i;

		if(port_entity.externelFromPort > port_entity.externelToPort) return ERR_9007;
		
		ret = 1;
	}	
	else if( strcmp( lastname, "InternalPort" )==0 )
	{	
		unsigned int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i> 65535) return ERR_9007;

		port_entity.fromPort= *i;
		port_entity.toPort = *i;

		if(check_value_isValid(wanif_index, port_entity, port_chainid)< 0)
			return ERR_9007;
		
		ret = 1;
	}
	else if( strcmp( lastname, "PortMappingProtocol" )==0 )
	{	
		if( strlen(buf)==0 ) return ERR_9007;

		if( strcmp( buf, "TCP" )==0 )
			port_entity.protoType = PROTO_TCP;
		else if( strcmp( buf, "UDP" )==0 )
			port_entity.protoType = PROTO_UDP;
		else if ( strcmp( buf, "TCPandUDP" )==0 )
			port_entity.protoType = PROTO_BOTH;
		else
			return ERR_9007;
		
		if(check_value_isValid(wanif_index, port_entity, port_chainid)< 0)
			return ERR_9007;	
		
		ret = 1;
	}else if( strcmp( lastname, "InternalClient" )==0 )
	{	
		if( strlen(buf)==0 ) return ERR_9007; //can't be empty
			
		if ( !inet_aton(buf, (struct in_addr *)&(port_entity.ipAddr)) )
			return ERR_9007;

		ret = 1;

	}else if( strcmp( lastname, "PortMappingDescription" )==0 )
	{
		strncpy( port_entity.comment, buf, COMMENT_LEN-1 );
		port_entity.comment[COMMENT_LEN-1]=0;

		ret = 0;
	}
	else
	{
		return ERR_9005;
	}
	
	memcpy(&target[1], &port_entity, sizeof(PORTFW_T));
	if ( !mib_set(MIB_PORTFW_MOD, (void *)target)) 
		return ERR_9005;

	return ret;
}

int objWANPORTMAPPING(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	unsigned int devnum,ipnum,pppnum;
	unsigned int chainid;
	PORTFW_T entry;
	unsigned int maxInsNum = 0, wanif_index = 0;

	if( (name==NULL) || (entity==NULL) ) return -1;

	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d>(type = %d)\n", __FUNCTION__, __LINE__, type ) );

	devnum  = getWANConDevInstNum( name );
	ipnum   = getWANIPConInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	if( (devnum==0) || ( (ipnum==0) && (pppnum==0) ) ) return ERR_9005;

#ifdef MULTI_WAN_SUPPORT
	wanif_index = (ipnum != 0?ipnum:pppnum);	
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d> (wanif_index = %d)\n", __FUNCTION__, __LINE__, wanif_index ) );
#endif

	maxInsNum = getPortMappingMaxInstNum(wanif_index);
//	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d> (maxInsNum = %d)\n", __FUNCTION__, __LINE__, maxInsNum ) );

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		int num=0,i,portfw_enabled=0;
		struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
		PORTFW_T *p, port_entity;
		unsigned int port_chainID=0;//, MaxInstNum=0
		PORTFW_T target[2];
		unsigned int instance_num;
		/* if port forwarding of DUT is disabled, return immediately */
		apmib_get(MIB_PORTFW_ENABLED, (void *)&portfw_enabled);
		if(portfw_enabled==0)
			return 0;
		
		memset( &target[0], 0, sizeof( PORTFW_T ) );
		memset( &target[1], 0, sizeof( PORTFW_T ) );
		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

		getPortMappingCount( &num );
		
		for( i=1; i<=num;i++ )
		{
			*((char *)&port_entity) = (char)i;
			mib_get(MIB_PORTFW_TBL, (void *)&port_entity);
			memcpy(&target[0], &port_entity, sizeof(PORTFW_T));
#ifdef MULTI_WAN_SUPPORT
			if( port_entity.WANIfIndex != wanif_index)
				continue;
#endif
			if( port_entity.InstanceNum== 0 ) //maybe createn by web or cli
			{
				maxInsNum++;
				port_entity.InstanceNum = maxInsNum;
				memcpy(&target[1], &port_entity, sizeof(PORTFW_T));
				mib_set(MIB_PORTFW_MOD, (void *)&target);
				
			//	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "<%s:%d> (maxInsNum = %d)\n", __FUNCTION__, __LINE__, maxInsNum ) );
			}
			instance_num = port_entity.InstanceNum;

		if( create_Object( c, tWANPORTMAPObject, sizeof(tWANPORTMAPObject), 1, instance_num ) < 0 )				
				return -1;

		}
		add_objectNum( name, maxInsNum );
		return 0;
	     }
	case eCWMP_tADDOBJ:
	     {
	    int ret;
		int num,portfw_enabled=0;

		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

		num = get_ParameterNameCount(name,1);
		if(num >= MAX_FILTER_NUM)
	  		return ERR_9004;
	  		
		ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPORTMAPObject, sizeof(tWANPORTMAPObject), data );
		if( ret >= 0 )
		{
			PORTFW_T port_entity;
			char lan_ip[32];
			
			memset(lan_ip,0x00,sizeof(lan_ip));
			memset(&port_entity, 0, sizeof( PORTFW_T ) );

			sprintf(lan_ip,"%d.%d.%d.%d",0, 0, 0, 0 ); //110: temp ip.
			inet_aton(lan_ip, (struct in_addr *)&port_entity.ipAddr);
			port_entity.fromPort = 0;
			port_entity.toPort = 0;
#ifdef MULTI_WAN_SUPPORT
			port_entity.WANIfIndex = wanif_index;
#endif
			port_entity.InstanceNum= *(int*)data;

			port_entity.protoType = PROTO_BOTH;
			sprintf(port_entity.comment,"%s[%u]", "Created by TR069",port_entity.InstanceNum);

			if ( mib_set(MIB_PORTFW_ADD, (void *)&port_entity) == 0) 
			{
				fprintf(stderr,"\r\n Add PW table entry error!");
				return -1;
			}

			/* if port forwarding is not enabled, enable it now */			
			apmib_get(MIB_PORTFW_ENABLED, (void *)&portfw_enabled);
			if(portfw_enabled==0)
			{
				portfw_enabled = 1;
				mib_set(MIB_PORTFW_ENABLED, (void *)&portfw_enabled);
			}
		}

#ifndef _CWMP_APPLY_
		if(ret==0) ret=1;
#endif
		return ret;
	     }
	case eCWMP_tDELOBJ:
	     {
	    int ret;
		PORTFW_T port_entity;
		unsigned int port_chainID=0;
		unsigned int wan_num = 0;
	    if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
		 
#ifdef MULTI_WAN_SUPPORT
		 wan_num = wanif_index;
#endif

		if( getPortMappingByInstNum( wan_num, *(unsigned int*)data, &port_entity, &port_chainID ) )
			return -1;
		if ( !mib_set(MIB_PORTFW_DEL, (void *)&port_entity)) 
		{
			fprintf(stderr,"\r\n Del PW table entry error!");
			return -1;
		}

		ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );

#ifndef _CWMP_APPLY_
		if(ret==0) ret=1;
#endif

		return ret;
	     	break;
	     }
	case eCWMP_tUPDATEOBJ:		
	     {
	     	int num=0,i,portfw_enabled=0;
	     	struct CWMP_LINKNODE *old_table;
	     	unsigned int port_chainID=0;
	     	PORTFW_T target[2];
	     	
	     	/* if port forwarding of DUT is disabled, return immediately */
			apmib_get(MIB_PORTFW_ENABLED, (void *)&portfw_enabled);
			if(portfw_enabled==0)
				return 0;

			getPortMappingCount( &num );
	     	old_table = (struct CWMP_LINKNODE *)entity->next;
	     	entity->next = NULL;
			memset( &target[0], 0, sizeof( PORTFW_T ) );
			memset( &target[1], 0, sizeof( PORTFW_T ) );
	     	for( i=1; i<=num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;
				PORTFW_T *p, port_entity;
				p = &port_entity;

				*((char *)&port_entity) = (char)i;
				mib_get(MIB_PORTFW_TBL, (void *)&port_entity);
				memcpy(&target[0], &port_entity, sizeof(PORTFW_T));
#ifdef MULTI_WAN_SUPPORT
				if( port_entity.WANIfIndex != wanif_index )
					continue;
#endif		
				remove_entity = remove_SiblingEntity( &old_table,  port_entity.InstanceNum);
		
				if( remove_entity!=NULL )
				{				
					add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
				}else{ 
					unsigned int MaxInstNum;
					MaxInstNum = port_entity.InstanceNum;

					add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPORTMAPObject, sizeof(tWANPORTMAPObject), &MaxInstNum );

					if(MaxInstNum!=port_entity.InstanceNum)
					{				
						port_entity.InstanceNum = MaxInstNum;
						//printf("%s:%d:port_entity.InstanceNum=%d\n", __FUNCTION__, __LINE__, port_entity.InstanceNum);
						memcpy(&target[1], &port_entity, sizeof(PORTFW_T));
						if ( !mib_set(MIB_PORTFW_MOD, (void *)&target)) 
	     				{
							return -1;
						}
					}
				}	
	     	}

			//add_objectNum(name, num); //update ParameterTable max num
	     	
	     	if( old_table )
	     		{
	     		destroy_ParameterTable( (struct CWMP_NODE *)old_table );
	     		}
	     	return 0;
	     }
	
	}

	return -1;
}

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIP/PPPConnection.Stats*/
int getWANCONSTATS(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	*tok;
	unsigned int	chainid;
#if defined(MULTI_WAN_SUPPORT)
	WANIFACE_T *pEntry,wan_entity;
#endif 	
	char	ifname[16];
	char	buf[256];
#if 1//defined CONFIG_ETHWAN
		struct user_net_device_stats stats;
#endif

/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1//def _PRMT_WT107_
	unsigned long es=0,er=0, ups=0,upr=0, dps=0,dpr=0, mps=0,mpr=0, bps=0, bpr=0, uppr=0;
#endif
/*ping_zhang:20081217 END*/
	unsigned int devnum,ipnum,pppnum;
	OPMODE_T opmode=-1;
	char *iface=NULL;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	devnum  = getWANConDevInstNum( name );
	ipnum   = getWANIPConInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	if( (devnum==0) || ( (ipnum==0) && (pppnum==0) ) ) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)
	if(transfer2IfNameWithVlan(name, ifname)<0)
		return ERR_9002;
	if(getStats(ifname, &stats)< 0)
		bzero(&stats,sizeof(stats));
#else

	mib_get( MIB_OP_MODE, (void *)&opmode);
	if(opmode == WISP_MODE)
		iface = "wlan0";
	else if(pppnum != 0)
		iface = "ppp0";
	else
		iface = "eth1";
	
	if ( getStats(iface, &stats) < 0)
		stats.tx_packets = 0;
#endif

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "EthernetBytesSent" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup( stats.tx_bytes );
#endif
	}
	else if( strcmp( lastname, "EthernetBytesReceived" )==0 )
	{	
			
#if 1//defined CONFIG_ETHWAN
			*data = uintdup( stats.rx_bytes );
#endif
			
	}
	else if( strcmp( lastname, "EthernetPacketsSent" )==0 )
	{	
			
#if 1//defined CONFIG_ETHWAN
			*data = uintdup( stats.tx_packets );
#endif
			
	}
	else if( strcmp( lastname, "EthernetPacketsReceived" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
			*data = uintdup( stats.rx_packets );
#endif
	}
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	else if( strcmp( lastname, "EthernetErrorsSent" )==0 )
	{	
		
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(stats.tx_errors);
#endif
		
	}else if( strcmp( lastname, "EthernetErrorsReceived" )==0 )
	{	
		
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(stats.rx_errors);
#endif
		
	}
	else if( strcmp( lastname, "EthernetUnicastPacketsSent" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(stats.tx_packets);
#endif
	}
	else if( strcmp( lastname, "EthernetUnicastPacketsReceived" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(stats.rx_packets - stats.rx_multicast);
#endif
	}

	else if( strcmp( lastname, "EthernetDiscardPacketsSent" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup( stats.tx_dropped );
#endif
	}else if( strcmp( lastname, "EthernetDiscardPacketsReceived" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup( stats.rx_dropped );
#endif
	}
	else if( strcmp( lastname, "EthernetMulticastPacketsSent" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(0);
#endif
	}
	else if( strcmp( lastname, "EthernetMulticastPacketsReceived" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(stats.rx_multicast);
#endif
	}
	else if( strcmp( lastname, "EthernetBroadcastPacketsSent" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(0);
#endif
	}
	else if( strcmp( lastname, "EthernetBroadcastPacketsReceived" )==0 )
	{	
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(0);
#endif

	}
	else if( strcmp( lastname, "EthernetUnknownProtoPacketsReceived" )==0 )
	{
#if 1//defined CONFIG_ETHWAN
		*data = uintdup(0);
#endif		
	}
#endif //#ifdef _PRMT_WT107_
/*ping_zhang:20081217 END*/
	else{
		return ERR_9005;
	}
	
	return 0;
}


/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANPPPConnection.*/
int getWANPPPCONENTITY(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	*tok;
	unsigned int	chainid;
#if defined(MULTI_WAN_SUPPORT)
	WANIFACE_T *pEntry,wan_entity;
#endif 	
	char	ifname[16];
	char	buf[512]="";
	unsigned int devnum,pppnum;
	unsigned int vChar=0;
	int wan_dhcp;
	mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;


	devnum = getWANConDevInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	if( (devnum==0) || (pppnum==0) ) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)
	pEntry = &wan_entity;
   	*((char *)&wan_entity) = (char)pppnum;
	if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
		return ERR_9002;
	if( transfer2IfNameWithVlan(name, ifname)<0)
		return ERR_9002;
#endif

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->enable)
			*data = booldup(1);
		else
			*data = booldup(0);
#else
		if(wan_dhcp != PPPOE)
			*data = booldup(0);
		else
			*data = booldup(1);
		
		
#endif
	}
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
	else if( strcmp( lastname, "Reset" )==0 )
	{
		/*while read, this parameter always returns False.*/
		*data = booldup(0);
	}
//#endif
/*ping_zhang:20081217 END*/
	else if( strcmp( lastname, "ConnectionStatus" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		char pppstatus[32]={0};
#ifdef PPPOE_PASSTHROUGH
		int flags;
		
		if(pEntry->cmode==IP_BRIDGE)
		{	
			if(pEntry->enable==0)
				*data = strdup( "Disconnected" );
			else if(getInFlags( ifname, &flags) == 1)
			{
				if (flags & IFF_UP)
					*data = strdup( "Connected" );
				else
					*data = strdup( "Disconnected" );
			}else
				*data = strdup( "Disconnected" );
		}else
			*data = strdup( "Connected" );
#endif
#else //#if defined(MULTI_WAN_SUPPORT)
		int isWanPhy_Link=0;
		OPMODE_T opmode=-1;
		DHCP_T dhcp;
		mib_get(MIB_WAN_DHCP, (void *)&dhcp);
		
		mib_get( MIB_OP_MODE, (void *)&opmode);
		
		if(opmode != WISP_MODE)
		{	
 			isWanPhy_Link=getWanLink("eth1"); 
 		}
		
		if ( dhcp ==  PPPOE )
		{
			if ( isConnectPPP())
			{
				if(isWanPhy_Link < 0)
						*data = strdup( "Disconnected" );
					else
						*data = strdup("Connected");
			}
			else			
				*data = strdup( "Disconnected" );
		}
		else
		{
			*data = strdup( "Disconnected" );
		}
			
#endif //#if defined(MULTI_WAN_SUPPORT)
#if 0  //NOT Support YET
		if( getPPPConStatus( ifname, pppstatus )==0 )
			{
				/*pppstatus is defined in if_spppsubr.c*/
			if( strcmp( pppstatus, "Dead" )==0 )
						*data = strdup( "Disconnected" );
			else if( strcmp( pppstatus, "Establish" )==0 )
				*data = strdup( "Connecting" );
			else if( strcmp( pppstatus, "Terminate" )==0 )
				*data = strdup( "Disconnecting" );
			else if( strcmp( pppstatus, "Authenticate" )==0 )
				*data = strdup( "Authenticating" );
			else if( strcmp( pppstatus, "Network" )==0 )
						*data = strdup("Connected");
			else			
				*data = strdup( "Disconnected" );
		}else
				*data = strdup( "Disconnected" );
#endif		
	}	
	else if( strcmp( lastname, "PossibleConnectionTypes" )==0 )
	{	
#ifdef PPPOE_PASSTHROUGH
		*data = strdup( "IP_Routed,PPPoE,Bridged" );
#else
		*data = strdup( "IP_Routed,PPPOE" );
#endif
	}
	else if( strcmp( lastname, "ConnectionType" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)	
		switch(pEntry->AddressType)
		{
			case PPPOE:
				*data = strdup( "PPPoE" );
				break;
			case BRIDGE:
				*data = strdup( "Bridged" );
				break;
			default:
				*data = strdup( "IP_Routed" );
				break;
		}			
#else
		*data = strdup( "PPPoE" );
#endif //#if defined(MULTI_WAN_SUPPORT)			
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
	}
#if 1 //def _PRMT_WT107_
	else if( strcmp( lastname, "PPPoESessionID" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		*data = intdup(pEntry->pppSessionNum);
#else
//todo: add multi-wan
		unsigned int sessionId = 0;

		mib_get(MIB_PPP_SESSION_NUM, (void *)&sessionId);
		*data = intdup(sessionId);
#endif
	}else if( strcmp( lastname, "DefaultGateway" )==0 )
	{
#ifdef MULTI_WAN_SUPPORT
		char *temp=NULL;
		struct in_addr inAddr;
		if(getDefaultRoute(ifname, &inAddr) )
				temp = inet_ntoa(inAddr);
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");
#else
	//todo: add multi-wan

		char strWanIP[16];
		char strWanMask[16];
		char strWanDefIP[16];
		char strWanHWAddr[18];
		getWanInfo(strWanIP,strWanMask,strWanDefIP,strWanHWAddr);
			
		*data=strdup(strWanDefIP);
#endif
	}
#endif
/*ping_zhang:20081217 END*/	
	else if( strcmp( lastname, "Name" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->WanName&&pEntry->WanName[0])
			*data = strdup( pEntry->WanName);
		else
			*data = strdup( "" );
#else //#if defined(MULTI_WAN_SUPPORT)
		{
			char wanName[MAX_NAME_LEN+1]={0};
			mib_get(MIB_CWMP_WANNAME,(void*)&wanName);
			*data = strdup(wanName);
		}
#endif //#if defined(MULTI_WAN_SUPPORT)
	}
	else if( strcmp( lastname, "Uptime" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		unsigned int uptime;
#ifdef PPPOE_PASSTHROUGH
		if(pEntry->cmode==IP_BRIDGE)
		{
		struct sysinfo info;
		sysinfo(&info);
		*data = uintdup( info.uptime );
			return 0;
		}else
		if(pEntry->cmode==IP_PPP)
		{
			FILE *fp;
			unsigned int startTime = 0;
			struct timeval nowTime;
			char fileName[32]={0};

			sprintf(fileName,"/var/ppp/startTime%d",pppnum);
			fp = fopen( fileName, "r" );
			
			while (fp && fgets( buf,160,fp ))
			{
				char *p;
				
				p = strtok(buf, " \n\r");
				if (p)
				{
					startTime = atol(p);
				}
			}
			
			if (fp) fclose(fp);

			if (startTime == 0)
				*data = uintdup(0);
			else
			{
				if(gettimeofday(&nowTime, NULL)<0)
					return ERR_9002;
//				printf("%s:%d startTime=%u  now=%u uptime=%u\n",__FUNCTION__,__LINE__,startTime,nowTime.tv_sec,nowTime.tv_sec - startTime);
				*data = uintdup(nowTime.tv_sec - startTime);
				return 0;
			}
		}
#endif
		//NOT Support YET
//		if( getPPPUptime( ifname, pEntry->cmode, &uptime )==0 )
//			*data = uintdup( uptime );
//		else
			*data = uintdup( 0 );
#else //
#if 1
		FILE *fp;
		unsigned int startTime = 0;
		struct timeval nowTime;

		fp = fopen( "/var/ppp/startTime", "r" );
		
		while (fp && fgets( buf,160,fp ))
		{
			char *p;
			
			p = strtok(buf, " \n\r");
			if (p)
			{
				startTime = atol(p);
			}
		}
		
		if (fp) fclose(fp);

		if (startTime == 0)
			*data = uintdup(0);
		else
		{
			if(gettimeofday(&nowTime, NULL)<0)
				return ERR_9002;
			printf("%s:%d startTime=%u  now=%u\n",__FUNCTION__,__LINE__,startTime,nowTime.tv_sec);
			*data = uintdup(nowTime.tv_sec - startTime);
		}

#else
		struct sysinfo info;
		DHCP_T dhcp;
		mib_get(MIB_WAN_DHCP, (void *)&dhcp);
		int isWanPhy_Link;
		
		isWanPhy_Link=getWanLink("eth1"); 
		
		if ( dhcp ==  PPPOE )
		{
			if ( isConnectPPP())
			{
				if(isWanPhy_Link < 0)
					*data = uintdup( 0 );
				else
				{
					sysinfo(&info);
					*data = uintdup( info.uptime );
				}
			}
			else			
				*data = uintdup( 0 );
		}
		else
			*data = uintdup( 0 );
#endif
#endif //#if defined(MULTI_WAN_SUPPORT)
	}
	else if( strcmp( lastname, "LastConnectionError" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)	
#ifdef PPPOE_PASSTHROUGH
		if(pEntry->cmode==IP_BRIDGE)
			*data = strdup( "ERROR_NONE" );
		else
#endif
		//NOT Support YET
//		*data = strdup(getLastConnectionError(pEntry->ifIndex));	// Jenny
#endif //#if defined(MULTI_WAN_SUPPORT)
		*data = strdup( "ERROR_UNKNOWN" );
	}
	else if( strcmp( lastname, "AutoDisconnectTime" )==0 )
	{	
		*data = uintdup( 0 ); //only 0:the connection is not to be shut down automatically
//		*data = uintdup( pEntry->autoDisTime );		// Jenny
	}
	else if( strcmp( lastname, "IdleDisconnectTime" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)		
		*data = uintdup( pEntry->pppIdleTime*60 );
#else
		unsigned int idleTime=0;

		mib_get(MIB_PPP_IDLE_TIME, (void *)&idleTime);
		*data = uintdup(idleTime);
#endif
	}

	else if( strcmp( lastname, "WarnDisconnectDelay" )==0 )	// Jenny
	{	
#if defined(MULTI_WAN_SUPPORT)
		*data = uintdup( pEntry->warnDisDelay );
#else
		*data = uintdup( 0 );
#endif
	}

	else if( strcmp( lastname, "RSIPAvailable" )==0 )
	{	
		*data = booldup( 0 );
	}
	else if( strcmp( lastname, "NATEnabled" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
#if 1
	//	printf("%s:%d NATDisabled=%d\n",__FUNCTION__,__LINE__,pEntry->nat_disable);
		if(pEntry->nat_disable ==0)
			*data = booldup( 1 );
		else
			*data = booldup( 0 );
#else
		if (pEntry->cmode == IP_BRIDGE)
			*data = booldup( 0 );
		else if (pEntry->cmode == IP_ROUTE||pEntry->cmode==IP_PPP)
			*data = booldup( 1 );		
		else
			return ERR_9002;
#endif
#else
		*data = booldup( 1 );
#endif//#if defined(MULTI_WAN_SUPPORT)
	}
	else if( strcmp( lastname, "Username" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->pppUsername[0])
			*data = strdup( pEntry->pppUsername );
		else
			*data = strdup( "" );
#else
		mib_get(MIB_PPP_USER_NAME, (void *)buf);
		*data = strdup(buf);
#endif
	}else if( strcmp( lastname, "Password" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		*data = strdup( "" ); /*return an empty string*/
#else
#if DEBUG
		mib_get(MIB_PPP_PASSWORD, (void*)buf);
		*data = strdup(buf);
#else
		*data = strdup( "" ); /*return an empty string*/
#endif
#endif //#if defined(MULTI_WAN_SUPPORT)
	}
	else if( strcmp( lastname, "PPPEncryptionProtocol" )==0 )	// Jenny
	{
		*data = strdup( "None" );
	}
	else if( strcmp( lastname, "PPPCompressionProtocol" )==0 )	// Jenny
	{
		*data = strdup( "None" );
	}

	else if( strcmp( lastname, "PPPAuthenticationProtocol" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->pppAuth==PPP_AUTH_PAP)
			*data = strdup( "PAP" );
		else if(pEntry->pppAuth==PPP_AUTH_CHAP)
			*data = strdup( "CHAP" );
		else if(pEntry->pppAuth==PPP_AUTH_AUTO)
			*data = strdup( "PAPandCHAP" );
		else
			*data = strdup( "PAP, CHAP, PAPandCHAP" );
#else
        *data = strdup( "PAP, CHAP, PAPandCHAP" );
#endif
	}

	else if( strcmp( lastname, "ExternalIPAddress" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		char *temp=NULL;
		struct in_addr inAddr;
			
		if (getInAddr( ifname, IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("");
#else
		struct in_addr inAddr;
		char *temp=NULL;

		
		if (getInAddr( "ppp0", IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");
#endif
	}

	else if( strcmp( lastname, "RemoteIPAddress" )==0 )
	{	
		char *temp=NULL;	
		struct in_addr inAddr;

#if defined(MULTI_WAN_SUPPORT)
		if (getInAddr( ifname, DST_IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
#else
		if (getInAddr( "ppp0", DST_IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
#endif
		
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");
	}
	else if( strcmp( lastname, "MaxMRUSize" )==0 )	// Jenny
	{	
#if defined(MULTI_WAN_SUPPORT)	
		*data = uintdup( pEntry->pppoeMtu );
#else
int mtu_size;
mib_get( MIB_PPP_MTU_SIZE, (void *)&mtu_size);
*data = uintdup( mtu_size );
#endif
	}
	else if( strcmp( lastname, "CurrentMRUSize" )==0 )	// Jenny
	{	
#if defined(MULTI_WAN_SUPPORT)
	//NOT Support YET
	//	unsigned int cmru;
	//	if( getPPPCurrentMRU( ifname, pEntry->cmode, &cmru )==0 )
	//		*data = uintdup( cmru );
	//	else
			*data = uintdup( pEntry->pppoeMtu );
#else
#if 1 //should use ioctl SIOCGIFMTU get current mtu
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "ppp0");
	if(do_ioctl(SIOCGIFMTU, &ifr)<0){
		return ERR_9002;
	}
	printf("%s:%d current MTU:%d\n",__FUNCTION__,__LINE__,ifr.ifr_mtu);
	*data=intdup(ifr.ifr_mtu);
#else
int mtu_size;
mib_get( MIB_PPP_MTU_SIZE, (void *)&mtu_size);
*data = uintdup( mtu_size );
#endif
#endif //#if defined(MULTI_WAN_SUPPORT)
	}

	else if( strcmp( lastname, "DNSEnabled" )==0 )
	{	
		int dns_disable;
#if defined(MULTI_WAN_SUPPORT)	
		dns_disable=pEntry->dnsDisable;
		if(dns_disable)
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
#else
		mib_get( MIB_CWMP_DNS_DISABLE, (void *)&dns_disable);
		if(dns_disable == 0) 
			*data = booldup( 1 );
		else
			*data = booldup( 0 );
		
#endif
	}
	else if( strcmp( lastname, "DNSOverrideAllowed" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		vChar=pEntry->dnsAuto;
		if( vChar==1 ) //automatically attain DNS
			*data = booldup( 1 );
		else
		*data = booldup( 0 );
#else
		int dns;
		mib_get( MIB_DNS_MODE, (void *)&dns);
		if(dns == 1) //Manual set DNS
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
		
#endif	
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		char dns[64] = {0};
		char dns1[16] = {0};
		char dns2[16] = {0};
		char dns3[16] = {0};

		vChar=pEntry->dnsAuto;
		if( vChar==1 ) //automatically attain DNS
		{
			if(parseDNSForMultiWan(pppnum, dns)<0)
				*data = strdup("0.0.0.0");
			else
				*data = strdup( dns );
		}
		else
		{
			unsigned char *dns_tmp = pEntry->wanIfDns1;
			if(dns_tmp)
				sprintf(dns1, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns1, "0.0.0.0");

			dns_tmp = pEntry->wanIfDns2;
			if(dns_tmp)
				sprintf(dns2, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns2, "0.0.0.0");
			
			dns_tmp = pEntry->wanIfDns3;
				
			if(dns_tmp)
				sprintf(dns3, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns3, "0.0.0.0");

			sprintf(dns, "%s,%s,%s", dns1, dns2, dns3);

			*data = strdup( dns );
		}
#else
		int vInt;
		mib_get( MIB_DNS_MODE, (void *)&vInt);
		if( vInt==0 ) //automatically attain DNS
		{
			FILE *fp=NULL;
			char buff[128]={0};
			char outputBuf[256]={0};
			char nameserver[32]={0};
			char nameserver_ip[80]={0};

			if((fp=fopen("/etc/ppp/resolv.conf", "r"))==NULL){
				printf("Invalid file, /etc/ppp/resolv.conf not exist!\n");
            }
            
            if((fp==NULL) && ((fp=fopen("/etc/resolv.conf", "r"))==NULL) )
            {
				printf("Invalid file, /etc/resolv.conf not exist!\n");
            }

            if(fp==NULL)
            {
                sprintf(outputBuf, "0.0.0.0");
                *data=strdup(outputBuf);
            }else{

                while(fgets(buff,sizeof(buff),fp)!=NULL){
                    //printf("%s:%d DNSServers buff=%s\n",__FUNCTION__,__LINE__,buff);
                    bzero(nameserver_ip,sizeof(nameserver_ip));
                    sscanf(buff,"%s %s",nameserver,nameserver_ip);
                    if(strcmp(nameserver,"nameserver")==0){
                        strcat(outputBuf,nameserver_ip);
                        strcat(outputBuf,",");
                    }

                }
                //printf("%s:%d DNSServers outputBuf=%s\n",__FUNCTION__,__LINE__,outputBuf);
                fclose(fp);
                outputBuf[strlen(outputBuf)-1]=0;
                *data=strdup(outputBuf);
            }
        }
        else
        {
            getDNSList(buf);			
            *data = strdup( buf );
		}
#endif
	}
	else if( strcmp( lastname, "MACAddress" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		char wanif[IFNAMSIZ];
		unsigned char *pMacAddr;
		struct sockaddr hwaddr;
		//ifGetName(PHY_INTF(pEntry->ifIndex), wanif, sizeof(wanif));
		getWanIfName(pppnum, wanif,IFNAMSIZ);
		if(!getInAddr(wanif, HW_ADDR, (void *)&hwaddr))
			sprintf(buf,"00:00:00:00:00:00");
		else{
			pMacAddr = (unsigned char *)hwaddr.sa_data;
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		*data = strdup(buf);
#else
#if 1
		struct ifreq ifr;
		unsigned char hwaddr[6]={0};

		strcpy(ifr.ifr_name, "ppp0");
		if(do_ioctl(SIOCGIFHWADDR, &ifr)<0){
			return ERR_9002;
		}
		memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, 6);
		sprintf(buf,"%02x%02x%02x%02x%02x%02x\n",hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
		*data = strdup(buf);
#else
		struct sockaddr hwaddr;
		unsigned char *pMacAddr;

		if(!getInAddr("ppp0", HW_ADDR, (void *)&hwaddr))
			*data = strdup("00:00:00:00:00:00");
		else
		{
			pMacAddr = (unsigned char *)hwaddr.sa_data;
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
	
			*data = strdup(buf);
		}
#endif
#endif //#if defined(MULTI_WAN_SUPPORT)
	}
	else if( strcmp( lastname, "MACAddressOverride" )==0 )
	{
		*data = booldup(0);
	}
	else if( strcmp( lastname, "TransportType" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if( pEntry->cmode == IP_PPP )
			*data = strdup("PPPoE");
		else
			*data = strdup("");
#else
		int vInt;
		
		mib_get(MIB_WAN_DHCP, (void *)&vInt);
		if(vInt == PPPOE)
			*data = strdup("PPPoE");
		else
			*data = strdup("");
#endif
	}

	else if( strcmp( lastname, "PPPoEACName" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		if( pEntry->cmode==IP_PPP && pEntry->pppACName[0])
			*data = strdup( pEntry->pppACName );
		else
			*data = strdup( "" );
#else
char ser_name[200]={0};
mib_get( MIB_PPP_SERVICE_NAME,  (void *)ser_name);
*data = strdup( ser_name );
#endif
	}

	else if( strcmp( lastname, "PPPoEServiceName" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)	
		if( pEntry->cmode==IP_PPP && pEntry->pppServiceName[0] )	// Jenny
			*data = strdup( pEntry->pppServiceName );
		else
			*data = strdup( "" );
#else
char ser_name[200]={0};
mib_get( MIB_PPP_SERVICE_NAME,  (void *)ser_name);
*data = strdup( ser_name );
#endif
	}
	else if( strcmp( lastname, "ConnectionTrigger" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)	
#ifdef PPPOE_PASSTHROUGH
		if( pEntry->cmode==IP_BRIDGE )
			*data = strdup( "AlwaysOn" );
		else
#endif
		if( pEntry->pppCtype==CONTINUOUS )
			*data = strdup( "AlwaysOn" );
		else if( pEntry->pppCtype==CONNECT_ON_DEMAND )
			*data = strdup( "OnDemand" );
		else
			*data = strdup( "Manual" );
#else
		PPP_CONNECT_TYPE_T type;
		mib_get(MIB_PPP_CONNECT_TYPE, (void *)&type);
		*data=strdup(buf);
		
		if( type==CONTINUOUS )
			*data = strdup( "AlwaysOn" );
		else if( type==CONNECT_ON_DEMAND )
			*data = strdup( "OnDemand" );
		else
			*data = strdup( "Manual" );
#endif
	}	
	else if( strcmp( lastname, "RouteProtocolRx" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
			unsigned char ripver = pEntry->rip;
			if(ripver == DISABLE_MODE)
				*data = strdup("Off");
			else if(ripver == RIP1_MODE)
				*data = strdup("RIPv1");
			else if(ripver == RIP2_MODE)
				*data = strdup("RIPv2");
			else
				*data = strdup("Off");
#else

			*data = strdup( "Off" );
#endif
	}	
	else if( strcmp( lastname, "PPPLCPEcho" )==0 )	// Jenny
	{	
#if defined(MULTI_WAN_SUPPORT)
		//NOT Support YET
		//unsigned int echo;
		
		//if( getPPPLCPEcho( ifname, pEntry->cmode, &echo )==0 )
		//	*data = uintdup( echo );
		//else
#endif
			*data = uintdup( 20 );

	}


	else if( strcmp( lastname, "PPPLCPEchoRetry" )==0 )	// Jenny
	{	
#if defined(MULTI_WAN_SUPPORT)	
		//NOT Support YET
		//unsigned int retry;
		//if( getPPPEchoRetry( ifname, pEntry->cmode, &retry )==0 )
		//	*data = uintdup( retry );
		//else
#endif
			*data = uintdup( 3 );

	}

#if defined (CONFIG_ETHWAN)
	else if( strcmp( lastname, "X_CT-COM_MulticastVlan" )==0 )
	{
			*data = intdup(pEntry->multicastVlan);
	}
#endif
	else if( strcmp( lastname, "ShapingBurstSize" )==0 )
	{//todo
		*data = uintdup( 1492 );
	}
	else if( strcmp( lastname, "PortMappingNumberOfEntries" )==0 )
	{	
		int portEntityCount = 0;
#ifdef MULTI_WAN_SUPPORT
		portEntityCount = getPortMappingTotalNum(pppnum);
		*data = uintdup(portEntityCount);
#else
		if(getPortMappingCount(&portEntityCount) == 0) //o:OK
			*data = uintdup( portEntityCount );
		else
			*data = uintdup( 0 );
#endif
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
int gStartReset=0;
static int resetThread=0;
static int resetChainID=-1;
const char DHCPC_SCRIPT[] = "/etc/scripts/udhcpc.sh";
const char DHCPC_SCRIPT_NAME[] = "/var/udhcpc/udhcpc";
const char DHCPC_PID[] = "/var/run/udhcpc.pid";

// calculate the 15-0(bits) Cell Rate register value (PCR or SCR)
// return its corresponding register value
static int cr2reg(int pcr)
{
	int k, e, m, pow2, reg;
	
	k = pcr;
	e=0;
	
	while (k>1) {
		k = k/2;
		e++;
	}
	
	//printf("pcr=%d, e=%d\n", pcr,e);
	pow2 = 1;
	for (k = 1; k <= e; k++)
		pow2*=2;
	
	//printf("pow2=%d\n", pow2);
	//m = ((pcr/pow2)-1)*512;
	k = 0;
	while (pcr >= pow2) {
		pcr -= pow2;
		k++;
	}
	m = (k-1)*512 + pcr*512/pow2;
	//printf("m=%d\n", m);
	reg = (e<<9 | m );
	//printf("reg=%d\n", reg);
	return reg;
}

static void *reset_thread(void *arg) {	
	va_cmd("/bin/ifconfig",2,1,"ppp0","down");
	va_cmd("/bin/ifconfig",2,1,"ppp0","up");

END:
	resetThread=0;
}

void cwmpStartReset() {
	pthread_t reset_pid;

	//printf("%s:%d resetThread=%d,resetChainID=%d\n",__FUNCTION__,__LINE__,resetThread,resetChainID);
	if (resetThread) {
		//printf("reset in progress, try again later=\n");
		return;
	}

	resetThread = 1;
	if( pthread_create( &reset_pid, NULL, reset_thread, 0 ) != 0 )
	{
		resetThread = 0;
		return;
	}
	pthread_detach(reset_pid);
	
}
#endif
#ifdef MULTI_WAN_SUPPORT
int setMultiWanDNSList(char *buf,WANIFACE_T *pEntry)
{
	char *tok1, *tok2, *tok3;
	int  ret=0;
	struct in_addr in1,in2,in3,emp_in;

	if( buf==NULL || pEntry==NULL) return -1;
	tok1 = NULL;
	tok2 = NULL;
	tok3 = NULL;

	tok1=strtok( buf, "," );
	tok2=strtok( NULL, "," );
	tok3=strtok( NULL, "," );
	if( (tok1==NULL)&&(tok2==NULL)&&(tok3==NULL) )
		return -1;
	
	if(tok1) 
		if(  inet_aton( tok1, &in1 )==0  ) ret=-1;
	if(tok2) 
		if(  inet_aton( tok2, &in2 )==0  ) ret=-1;
	if(tok3) 
		if(  inet_aton( tok3, &in3 )==0  ) ret=-1;

	memset( &emp_in, 0, sizeof(struct in_addr) );
	if(ret==0)
	{
		bzero(pEntry->wanIfDns1,sizeof(pEntry->wanIfDns1));
		bzero(pEntry->wanIfDns2,sizeof(pEntry->wanIfDns2));
		bzero(pEntry->wanIfDns3,sizeof(pEntry->wanIfDns3));
		if( tok1!=NULL )
			memcpy(pEntry->wanIfDns1,&in1,sizeof(in1));	
		if( tok2!=NULL )
			memcpy(pEntry->wanIfDns2,&in2,sizeof(in2));
		if( tok3!=NULL )
			memcpy(pEntry->wanIfDns3,&in3,sizeof(in3));
	}
	return ret;
}
#endif
/*ping_zhang:20081217 END*/
int setWANPPPCONENTITY(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	char	*tok;
	char ifname[16]={0};
	unsigned int	chainid;
#if defined(MULTI_WAN_SUPPORT)	
	WANIFACE_T *pEntry=NULL;// wan_entity;
	WANIFACE_T target[2]={0};
#endif
	unsigned int devnum,pppnum;
	
	if( (name==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	devnum = getWANConDevInstNum( name );
	pppnum  = getWANPPPConInstNum( name );
	if( (devnum==0) || (pppnum==0) ) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)	
	bzero(target,sizeof(target));
	*((char *)target) = (char)pppnum;
	if(!mib_get(MIB_WANIFACE_TBL, (void *)target))
		return ERR_9002;
	if( transfer2IfNameWithVlan(name, ifname)<0)
		return ERR_9002;
	/* set current handling WAN index */
	apmib_set(MIB_WANIFACE_CURRENT_IDX, (void *)&pppnum);
	memcpy(&target[1], &target[0], sizeof(WANIFACE_T));
	pEntry=&target[1];
#endif
	if( strcmp( lastname, "Enable" )==0 )
	{

		int *i=data;
		unsigned int vInt=0;
		if(i==NULL) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		pEntry->enable=*i;
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		if(*i==1)
			vInt = PPPOE;
		else
			vInt = DHCP_CLIENT;
		
		mib_set(MIB_WAN_DHCP, (void *)&vInt);
#endif
		return 1;
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1 //def _PRMT_WT107_
	}else if( strcmp( lastname, "Reset" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(*i==1) 
		{
		#ifdef MULTI_WAN_SUPPORT
			if(pEntry->AddressType==PPPOE){
				char cmdbuf[64]={0};
				sprintf(cmdbuf,"sysconf disc option_%s",ifname);
				//printf("%s:%d ifname=%s\n",__FUNCTION__,__LINE__,ifname);
				//fprintf(stderr,"%s:%d cmdbuf=%s\n",__FUNCTION__,__LINE__,cmdbuf);
				//system("echo \"tttttttttttttt\">/dev/console");
				system(cmdbuf);
			}
			return 0;
		#else
			//resetChainID = chainid;
			gStartReset = 1;
		#endif
			return 1;
		}
		return 0;
#endif
/*ping_zhang:20081217 END*/
	}
	else if( strcmp( lastname, "ConnectionType" )==0 )
	{
		unsigned int vInt=0;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9001;		

#if defined(MULTI_WAN_SUPPORT)
		if(strstr(buf,"Bridged")){
			pEntry->AddressType=BRIDGE;
			pEntry->cmode=IP_BRIDGE;
			pEntry->brmode=BRIDGE_ETHERNET;
		}
		else if(strstr(buf,"PPPoE")){
			pEntry->AddressType=PPPOE;
			pEntry->cmode=IP_PPP;
			pEntry->brmode=BRIDGE_DISABLE;
		}
		else if(strstr(buf,"IP_Routed")){
			pEntry->AddressType=DHCP_CLIENT;
			pEntry->cmode=IP_ROUTE;
			pEntry->brmode=BRIDGE_DISABLE;
		}
		mib_set(MIB_WANIFACE_MOD,(void*)target);
		return 1;
#else
		if(strstr(buf,"PPPoE") != NULL) //IP_Routed == PPPoE ??????
			vInt = PPPOE;
		else if(strstr(buf,"IP_Routed") != NULL)
			vInt=DHCP_CLIENT;
		else
			return ERR_9005;
			
		mib_set(MIB_WAN_DHCP, (void *)&vInt);
		return 1;
#endif
	}
	else if( strcmp( lastname, "Name" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9001;
		//printf("%s:%d buf=%s\n",__FUNCTION__,__LINE__,buf);
		strcpy(pEntry->WanName,buf);
		//printf("%s:%d WanName=%s\n",__FUNCTION__,__LINE__,pEntry->WanName);
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		{
			char wanName[MAX_NAME_LEN+1]={0};
			if( buf==NULL ) return ERR_9007;
			if( strlen(buf)==0 ) return ERR_9001;
			snprintf(wanName,sizeof(wanName),"%s",buf);
			mib_set(MIB_CWMP_WANNAME,(void*)&wanName);
		}
#endif

		return 0;
	}	

	else if( strcmp( lastname, "AutoDisconnectTime" )==0 )
	{
		unsigned int *idletime=data;

		if(idletime==NULL) return ERR_9007;
		if(*idletime > 0 && *idletime < 1001)
#if defined(MULTI_WAN_SUPPORT)
		{
			pEntry->pppIdleTime=*idletime;
			mib_set(MIB_WANIFACE_MOD,(void*)target);
		}
#else
			mib_set(MIB_PPP_IDLE_TIME, (void *)idletime);

#endif
		return 1;
	}

	else if( strcmp( lastname, "IdleDisconnectTime" )==0 )
	{
		unsigned int *idletime=data;

		if(idletime==NULL) return ERR_9007;
		if(*idletime > 0 && *idletime < 1001)
#if defined(MULTI_WAN_SUPPORT)
		{
			pEntry->pppIdleTime=*idletime;
			mib_set(MIB_WANIFACE_MOD,(void*)target);
		}
#else
			mib_set(MIB_PPP_IDLE_TIME, (void *)idletime);
#endif
		return 1;
	}

	else if( strcmp( lastname, "WarnDisconnectDelay" )==0 )	// Jenny
	{
		return 0;
	}
	else if( strcmp( lastname, "NATEnabled" )==0 )
	{
		int *i=data;
		if(i==NULL) return ERR_9007;
	//	printf("%s:%d NATEnabled=%d\n",__FUNCTION__,__LINE__,*i);
#if defined(MULTI_WAN_SUPPORT)
#if 1
		if(*i==0)
			pEntry->nat_disable=1;
		else
			pEntry->nat_disable=0;
		mib_set(MIB_WANIFACE_MOD,(void*)target);
		return 1;
#else
		switch(pEntry->AddressType){
			case BRIDGE:
				if(*i==1){
					pEntry->AddressType=PPPOE;
					pEntry->cmode=IP_PPP;
					pEntry->brmode=BRIDGE_DISABLE;
					mib_set(MIB_WANIFACE_MOD,(void*)target);
				}
				break;
			default:
				if(*i==0){
					pEntry->AddressType=BRIDGE;
					pEntry->cmode=IP_BRIDGE;
					pEntry->brmode=BRIDGE_ETHERNET;
					mib_set(MIB_WANIFACE_MOD,(void*)target);
			//		printf("%s:%d NATEnabled=%d\n",__FUNCTION__,__LINE__,*i);
				}
				break;
		}
		return 0;
#endif
#else
		return 0;
#endif//#if defined(MULTI_WAN_SUPPORT)	
	}
	else if( strcmp( lastname, "Username" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		strcpy(pEntry->pppUsername,buf);
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set(MIB_PPP_USER_NAME, (void*)buf);
#endif
		return 1;
	}else if( strcmp( lastname, "Password" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		strcpy(pEntry->pppPassword,buf);
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set(MIB_PPP_PASSWORD, (void*)buf);
#endif
		return 1;
	}
	else if( strcmp( lastname, "ExternalIPAddress" )==0 )
	{
		if( buf==NULL ) 
			return ERR_9007;
		if( strlen(buf)==0 ) 
			return ERR_9007;
		struct in_addr in;
		
		if(  inet_aton( buf, &in )==0  ) 
			return ERR_9007;
#ifdef MULTI_WAN_SUPPORT
		if( pEntry->AddressType!=DHCP_DISABLED)
			/* This parameter is configurable only if the AddressingType is Static. */
			return ERR_9007;
		else
		{
			memcpy(pEntry->ipAddr, &in, sizeof(in));

			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 0;
		}
#else
		{
			int wan_dhcp=0;
			mib_get(MIB_WAN_DHCP, (void *)&wan_dhcp);
			if(wan_dhcp!=DHCP_DISABLED)
				return ERR_9007;

			mib_set(MIB_WAN_IP_ADDR, (void *)&in);	
			return 1;
		}
#endif // MULTI_WAN_SUPPORT
	}
	else if( strcmp( lastname, "MaxMRUSize" )==0 )	// Jenny
	{
		unsigned int *mru = data;
		unsigned mtu_size = 0;
		
		if (mru==NULL) return ERR_9007;
		if (*mru<1)	*mru = 1;
		else if (*mru>1540)	*mru = 1540;

		mtu_size = *mru;
#if defined(MULTI_WAN_SUPPORT)
		pEntry->pppoeMtu=mtu_size;
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set( MIB_PPP_MTU_SIZE, (void *)&mtu_size);
#endif
		return 1;
	}
	else if( strcmp( lastname, "DNSEnabled" )==0 )
	{
		int *i=data;
		int dns_disable;
		printf("%s:%d\n",__FUNCTION__,__LINE__);
		if(i==NULL) return ERR_9007;
		
		if(*i ==0)
			dns_disable=1; 
		else if(*i ==1)
			dns_disable=0; 
		//printf("%s:%d dns_disable=%d\n",__FUNCTION__,__LINE__,dns_disable);
#if defined(MULTI_WAN_SUPPORT)
		mib_set( MIB_WANIFACE_DNS_DISABLE, (void *)&dns_disable);
#else

		mib_set( MIB_CWMP_DNS_DISABLE, (void *)&dns_disable);
#endif
		return 1;	
	}
	else if( strcmp( lastname, "DNSOverrideAllowed" )==0 )
	{	
		int *i=data;
		int dns;
			
		if(i==NULL) return ERR_9007;
		

#if defined(MULTI_WAN_SUPPORT)
		{
			int dns_auto=(*i);		
			mib_set(MIB_WANIFACE_DNS_AUTO,(void*)dns_auto);
		}
#else
		if(*i ==0)
			dns=1; //manual mode
		else if(*i ==1)
			dns=0; //auto mode
		mib_set( MIB_DNS_MODE, (void *)&dns);
#endif
		return 1;	
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{	
		int vInt = 0;
		if( (buf==NULL) || (strlen(buf)==0) )
		{	//automatically attain DNS
			vInt = 0; //automatically
		}
		else
		{
#if defined(MULTI_WAN_SUPPORT)
			if(setMultiWanDNSList(buf,pEntry)==0)
				vInt=1;
#else
			if( setDNSList( buf ) == 0 )
			{
				vInt = 1;

			}
#endif
		}
#if defined(MULTI_WAN_SUPPORT)
		pEntry->dnsAuto=vInt;
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set( MIB_DNS_MODE, (void *)&vInt);
#endif			
		return 1;			
	}
	else if( strcmp( lastname, "MACAddressOverride" )==0 )
	{
		int *i=data;
		
		if(i==NULL) return ERR_9007;
		if(*i==1) return ERR_9008;
		return 0;
	}
	else if( strcmp( lastname, "PPPoEACName" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		strcpy(pEntry->pppACName,buf);
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set( MIB_PPP_SERVICE_NAME,  (void *)buf);
#endif
		return 1;

	}
	else if( strcmp( lastname, "PPPoEServiceName" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		strcpy(pEntry->pppServiceName,buf);
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else
		mib_set( MIB_PPP_SERVICE_NAME,  (void *)buf);
#endif
		return 1;
	}else if( strcmp( lastname, "ConnectionTrigger" )==0 )
	{
		PPP_CONNECT_TYPE_T type;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
			
		if( strcmp( buf, "AlwaysOn" )==0 )
			type = CONTINUOUS;
		else if( strcmp( buf, "OnDemand" )==0 )
			type = CONNECT_ON_DEMAND;
		else if( strcmp( buf, "Manual" )==0 )
			type = MANUAL;
		else
			return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		pEntry->pppCtype=type;
		mib_set(MIB_WANIFACE_MOD,(void*)target);
#else			
		mib_set(MIB_PPP_CONNECT_TYPE, (void *)&type);
#endif
		return 1;
	}
	else if( strcmp( lastname, "RouteProtocolRx" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#ifdef MULTI_WAN_SUPPORT
		if( strstr( buf, "Off" ) ){
			pEntry->rip=DISABLE_MODE;
			return 1;
		}else if( strstr( buf, "RIPv1" ) ){
			pEntry->rip=RIP1_MODE;
		}
		else if( strstr( buf, "RIPv2" ) ){
			pEntry->rip=RIP2_MODE;
		}
		mib_set(MIB_WANIFACE_MOD,(void*)target);
		return 1;
#else
		if( strstr( buf, "Off" ) ){
			return 0;
		}else
			return ERR_9007;
#endif
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int objWANPPPConn(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	char	*tok;	
#if defined(MULTI_WAN_SUPPORT)
	WANIFACE_T *pEntry, wan_entity;
	WANIFACE_T target[2];
#endif
	unsigned int devnum;
	unsigned int num=0,i,maxnum=0,chainid=0;

	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);fflush(NULL);
	CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d)\n", __FUNCTION__, __LINE__, name,type ) );
	if( (name==NULL) || (entity==NULL) ) return -1;

	devnum = getWANConDevInstNum( name );
	if(devnum==0) {
		CWMPDBG( 2, ( stderr, "<%s:%d>devnum:%d\n", __FUNCTION__, __LINE__, devnum) );
		return ERR_9005;
		}

	switch( type )
	{		
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **ptable = (struct CWMP_LINKNODE **)data;
			
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			maxnum = num;
			CWMPDBG( 2, ( stderr, "<%s:%d>eCWMP_tINITOBJ:maxnum:%d,num=%d\n", __FUNCTION__, __LINE__, maxnum,num) );
			
			for( i=1; i<=num;i++ )
			{
				pEntry = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				if( (pEntry->connDisable==0)  && (pEntry->enable==1))
				{
					if( (pEntry->cmode == IP_PPP)
	#ifdef PPPOE_PASSTHROUGH
					|| ( (pEntry->cmode==IP_BRIDGE)&&(pEntry->brmode==BRIDGE_PPPOE) ) 
	#endif
					)
					{
						/*
						 * Instance number of this WANPPPConnection is the same as wan device index.
						 */
						if( create_Object( ptable, tWANPPPCONObject, sizeof(tWANPPPCONObject), 1, pEntry->ConDevInstNum ) < 0 )
							return ERR_9002;
						printf("[%s:%d]add conppp:%d\n", __FUNCTION__, __LINE__, pEntry->ConDevInstNum);
					}//if
				}//if
			}//for

			add_objectNum( name, maxnum );
#else //#if defined(MULTI_WAN_SUPPORT)
			int wan_dhcp;
			int cwmp_pppconn_instnum;
			int cwmp_pppconn_created;
			
			mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
			mib_get( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
			mib_get( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

			if(wan_dhcp == PPPOE)
			{
				if(cwmp_pppconn_instnum == 0)
					cwmp_pppconn_instnum = 1;

				if( create_Object( ptable, tWANPPPCONObject, sizeof(tWANPPPCONObject), 1, cwmp_pppconn_instnum ) < 0 )
					return -1;

				maxnum = 1; //only support 1 ppp connection
			}
			add_objectNum( name, maxnum );
			mib_set( MIB_CWMP_PPPCON_INSTNUM, (void *)&maxnum);

#endif //#if defined(MULTI_WAN_SUPPORT)			
			
			return 0;
		}
		case eCWMP_tADDOBJ:	     
		{
			int ret;

#if defined(MULTI_WAN_SUPPORT)		
			int cnt=0,Found=0;

			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return ERR_9005;

			memset( &target[0], 0, sizeof( WANIFACE_T ) );
			memset( &target[1], 0, sizeof( WANIFACE_T ) );
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			CWMPDBG( 2, ( stderr, "<%s:%d>eCWMP_tADDOBJ:num=%d\n", __FUNCTION__, __LINE__, num) );
			for( i=1; i<=num;i++ )
			{
				pEntry = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				
				if(pEntry->enable==0)
				{
					Found = 1;
					apmib_set(MIB_WANIFACE_CURRENT_IDX, (void *)&i);
					memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
					break;
				}
			}
			if(Found==0) 
				return ERR_9005;

			*(unsigned int *)data = i;
			ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPPPCONObject, sizeof(tWANPPPCONObject), data );
			if( ret >= 0 )
			{
				pEntry->enable=1;
				pEntry->connDisable = 0;
				pEntry->cmode = IP_PPP;
				pEntry->brmode = BRIDGE_DISABLE;
				pEntry->AddressType = PPPOE;
				pEntry->mtu = 1492;

				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T)); ///wan_entry has been updated
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				{
					ret=-1;
				}
			}
			notify_set_wan_changed();

			if(ret>=0) ret=1;

			return ret;
#else
			int wan_dhcp;
			int cwmp_pppconn_instnum;
			int num=0;
			int cwmp_pppconn_created;
			
			mib_get( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
			mib_get( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

			num = get_ParameterNameCount(name,1);
			
			if(cwmp_pppconn_created == 1) // ppp connection max is 1
	  			return ERR_9004;

			cwmp_pppconn_created = 1;
			
			CWMPDBG( 2, ( stderr, "<%s:%d>addobj,\n", __FUNCTION__, __LINE__) );

			if(cwmp_pppconn_instnum == 0)
				cwmp_pppconn_instnum = 1;

			*(unsigned int*)data = cwmp_pppconn_instnum;
			ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPPPCONObject, sizeof(tWANPPPCONObject), data );
		
			if(ret == 0)
			{
				wan_dhcp = PPPOE;
				mib_set( MIB_WAN_DHCP, (void *)&wan_dhcp);

				mib_set( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
				cwmp_pppconn_instnum = *(unsigned int*)data;
				mib_set( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

				notify_set_wan_changed();
			}
			return 1;
#endif //#if defined(MULTI_WAN_SUPPORT)
		}	     
		case eCWMP_tDELOBJ:	     		
		{
			int ret;
#if defined(MULTI_WAN_SUPPORT)
			int Found=0;
			char s_appname[32];
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

			memset( &target[0], 0, sizeof( WANIFACE_T ) );
			memset( &target[1], 0, sizeof( WANIFACE_T ) );
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			for( i=1; i<=num;i++ )
			{
				
				pEntry = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				
				if((pEntry->ConDevInstNum==*(unsigned int*)data) && (pEntry->cmode==IP_PPP))
				{
					Found = 1;
					memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
					break;
				}
			}//for

			if(Found==0) 
				return ERR_9005;

			ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			if( ret==0 )
			{
				/* DeleteObject succeed */
				pEntry->enable = 0;
				
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T)); ///wan_entry has been updated

				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					ret=ERR_9002;
				else
					ret=1;
			}
			return ret;
#else //#if defined(MULTI_WAN_SUPPORT)
			int whichOne=*(unsigned int*)data;
			int wan_dhcp;
			int cwmp_pppconn_instnum;
			int cwmp_pppconn_created;

			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
			
			mib_get( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

			if(whichOne != cwmp_pppconn_instnum) return ERR_9007; 

			ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			if(ret == 0)
			{
			wan_dhcp = DHCP_CLIENT;
			mib_set( MIB_WAN_DHCP, (void *)&wan_dhcp);

			
			/* change cwmp_pppconn_created to 0, after reboot the ppp connection will be not created*/
			cwmp_pppconn_created = 0;
			mib_set( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);

			}

			CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d) eCWMP_tDELOBJ\n", __FUNCTION__, __LINE__, name,type ) );
			return ret;
#endif //#if defined(MULTI_WAN_SUPPORT)
		}
		case eCWMP_tUPDATEOBJ:
		{
#if defined(MULTI_WAN_SUPPORT)
			int has_new=0;
			struct CWMP_LINKNODE *old_table;

			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			CWMPDBG( 2, ( stderr, "<%s:%d>eCWMP_tUPDATEOBJ:num=%d\n", __FUNCTION__, __LINE__, num) );
			old_table = (struct CWMP_LINKNODE *)entity->next;
			entity->next = NULL;
			for( i=1; i<=num;i++ )
			{
				struct CWMP_LINKNODE *remove_entity=NULL;
				WANIFACE_T *p,wan_entity;

				p = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				if((p->connDisable==1) || (p->enable==0)){
					CWMPDBG( 2, ( stderr, "<%s:%d>devnum:%d, connDisableD\n", __FUNCTION__, __LINE__, devnum) );
					continue;
				}

				if( p->cmode == IP_PPP
#ifdef PPPOE_PASSTHROUGH
					|| ( (p->cmode==IP_BRIDGE)&&(p->brmode==BRIDGE_PPPOE) ) 
#endif
				) 
				{
					remove_entity = remove_SiblingEntity( &old_table, p->ConDevInstNum );
					if( remove_entity!=NULL )
					{
						add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
					}else{
						unsigned int MaxInstNum=p->ConDevInstNum;
						add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPPPCONObject, sizeof(tWANPPPCONObject), &MaxInstNum );
						has_new=1;
					}
				}
			}

			if( old_table )
				destroy_ParameterTable( (struct CWMP_NODE *)old_table );
			return 0;
#else

			int has_new=0;
			struct CWMP_LINKNODE *old_table;
			int wan_dhcp;
			int num=0;
			int cwmp_pppconn_created, cwmp_pppconn_instnum;
			int ret = 0;


			mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
			mib_get( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
			mib_get( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

			if( wan_dhcp == PPPOE) //only support 1 ppp connection
			{
				struct CWMP_LINKNODE *remove_entity=NULL;

				old_table = (struct CWMP_LINKNODE *)entity->next;	     	
				entity->next = NULL;
				
				remove_entity = remove_SiblingEntity( &old_table, cwmp_pppconn_instnum );
				if( remove_entity!=NULL )
				{					
					add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
				}else{
				unsigned int MaxInstNum=cwmp_pppconn_instnum;
					ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANPPPCONObject, sizeof(tWANPPPCONObject), &MaxInstNum );
					cwmp_pppconn_instnum = MaxInstNum;				
				}

				if(ret == 0)
				{
					has_new=1;
					mib_set( MIB_CWMP_PPPCON_INSTNUM, (void *)&cwmp_pppconn_instnum);

				cwmp_pppconn_created = 1;
				mib_set( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
			}
				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );
			}
			else
			{
				cwmp_pppconn_created = 0;				
				mib_set( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);
				CWMPDBG( 2, ( stderr, "<%s:%d>devnum:%d, ConPPPInstNum=0\n", __FUNCTION__, __LINE__, devnum) );
			}				

			return 0;
				
#endif //#if defined(MULTI_WAN_SUPPORT)
		}
	}

	return -1;
}


/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANIPConnection.*/
int getWANIPCONENTITY(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	int	chainid;
#if defined(MULTI_WAN_SUPPORT)
	WANIFACE_T *pEntry,wan_entity;
#endif
	char	ifname[16];
	char	buf[512]="";
	unsigned int devnum,ipnum;
	int vChar=0;
	int wan_dhcp;
	mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);


	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;
	
	devnum = getWANConDevInstNum( name );
	ipnum  = getWANIPConInstNum( name );
	if( (devnum==0) || (ipnum==0) ) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)
   	pEntry = &wan_entity;
   	*((char *)&wan_entity) = (char)ipnum;
	if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
		return ERR_9002;

	if( transfer2IfNameWithVlan(name, ifname)<0)
		return ERR_9002;
#endif

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)	
		if(pEntry->enable)
			*data = booldup(1);
		else
			*data = booldup(0);
#else
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED)
#endif
		*data = booldup(1);
		else
			*data = booldup(0);
#endif //#if defined(MULTI_WAN_SUPPORT)
	}	
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_
	else if( strcmp( lastname, "Reset" )==0 )
	{
		/*while read, this parameter always returns False.*/
		*data = booldup(0);
	}	
//#endif
/*ping_zhang:20081217 END*/
	else if( strcmp( lastname, "ConnectionStatus" )==0 )
	{
		char *temp = NULL;
#if defined(MULTI_WAN_SUPPORT)	
		int flags;
		
		if(pEntry->enable==0)
			*data = strdup( "Unconfigured" );
		else
		{
			struct in_addr inAddr;

			if (getInAddr( ifname, IP_ADDR, (void *)&inAddr) == 1)
			{
				temp = inet_ntoa(inAddr);

				if(temp && strcmp(temp,"0.0.0.0")!=0)
					*data = strdup( "Connected" );
				else
					*data = strdup( "Connecting" );
			}else 
				*data = strdup( "Connecting" );
		}
#else		

#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED)
#endif
		{
			struct in_addr inAddr;
			if (getInAddr( "eth1", IP_ADDR, (void *)&inAddr) == 1)
				temp = inet_ntoa(inAddr);

			if(temp && strcmp(temp,"0.0.0.0")!=0)
		*data = strdup( "Connected" );
			else
				*data = strdup( "Disconnected" );

		}
		else
			*data = strdup( "Disconnected" );			
		
#endif //#if defined(MULTI_WAN_SUPPORT)
		
	}
	else if( strcmp( lastname, "PossibleConnectionTypes" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)	
		*data = strdup( "IP_Routed,IP_Bridged" );
#else
		*data = strdup( "IP_Routed" );
#endif
	}
	else if( strcmp( lastname, "ConnectionType" )==0 )
	{	

#if defined(MULTI_WAN_SUPPORT)
		if (pEntry->cmode == IP_BRIDGE)
			*data = strdup("IP_Bridged");
		else if (pEntry->cmode == IP_ROUTE)
			*data = strdup("IP_Routed");		
		else
			return ERR_9002;
#else
		*data = strdup("IP_Routed");	
#endif
	}

	else if( strcmp( lastname, "Name" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)	
		if(pEntry->WanName&&pEntry->WanName[0])
			*data = strdup( pEntry->WanName);
		else
			*data = strdup( "" );
#else
		{
			char wanName[MAX_NAME_LEN+1]={0};
			mib_get(MIB_CWMP_WANNAME,(void*)&wanName);
			*data = strdup(wanName);
		}
#endif
	}
	
	else if( strcmp( lastname, "Uptime" )==0 )
	{	
		struct sysinfo info;

		sysinfo(&info);
		*data = uintdup( info.uptime );
	}
	
	else if( strcmp( lastname, "LastConnectionError" )==0 )
	{	
		*data = strdup( "ERROR_NONE" );
	}
	else if( strcmp( lastname, "AutoDisconnectTime" )==0 )
	{	
		*data = uintdup( 0 );
	}
	else if( strcmp( lastname, "IdleDisconnectTime" )==0 )
	{	
		*data = uintdup( 0 );
	}
	else if( strcmp( lastname, "WarnDisconnectDelay" )==0 )
	{	
		*data = uintdup( 0 );
	}

	else if( strcmp( lastname, "RSIPAvailable" )==0 )
	{	
		*data = booldup( 0 );
	}
	else if( strcmp( lastname, "NATEnabled" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
#if 1
//		printf("%s:%d NATDisabled=%d\n",__FUNCTION__,__LINE__,pEntry->nat_disable);
		if(pEntry->nat_disable==0)
			*data = booldup( 1 );
		else
			*data = booldup( 0 );
#else

		if (pEntry->cmode == IP_BRIDGE)
			*data = booldup( 0 );
		else if (pEntry->cmode == IP_ROUTE)
			*data = booldup( 1 );		
		else
			return ERR_9002;
#endif
#else
		*data = booldup( 1 );
#endif
	}
	else if( strcmp( lastname, "AddressingType" )==0 )
	{
		
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->AddressType == (char)DHCP_DISABLED)
			*data = strdup( "Static" );
		else
			*data = strdup( "DHCP" );
#else
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT)
#endif
			*data = strdup( "DHCP" );
		else if(wan_dhcp == DHCP_DISABLED)
			*data = strdup( "Static" );
		else
			*data = strdup( "" );
#endif
	}
	else if( strcmp( lastname, "ExternalIPAddress" )==0 )
	{
		char *temp=NULL;		
		struct in_addr inAddr;
#if defined(MULTI_WAN_SUPPORT)		
		if (getInAddr( ifname, IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
#else
		if (getInAddr( "eth1", IP_ADDR, (void *)&inAddr) == 1)
			temp = inet_ntoa(inAddr);
#endif

		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");	
	}
	else if( strcmp( lastname, "SubnetMask" )==0 )
	{	
		char *temp=NULL;
		struct in_addr inAddr;

#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->AddressType == (char)DHCP_DISABLED)
			temp = inet_ntoa(*((struct in_addr *)pEntry->netMask));
		else
			if (getInAddr( ifname, SUBNET_MASK, (void *)&inAddr) == 1)
				temp = inet_ntoa(inAddr);
#else

#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED)
#endif
		{
			if (getInAddr( "eth1", SUBNET_MASK, (void *)&inAddr) == 1)
				temp = inet_ntoa(inAddr);
		}
#endif
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");
	}
	else if( strcmp( lastname, "DefaultGateway" )==0 )
	{	
		char *temp=NULL;
		struct in_addr inAddr;
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->AddressType == (char)DHCP_DISABLED)
			temp = inet_ntoa(*((struct in_addr *)pEntry->remoteIpAddr));
		else
			if(getDefaultRoute(ifname, &inAddr) )
				temp = inet_ntoa(inAddr);
#else
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED)
#endif
		{
			if(getDefaultRoute("eth1", &inAddr) )
				temp = inet_ntoa(inAddr);
		}
#endif
		if(temp)
			*data=strdup(temp);
		else
			*data=strdup("0.0.0.0");
	}
	else if( strcmp( lastname, "DNSEnabled" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		vChar=pEntry->dnsDisable;
		if( vChar==0 ) //Manual set DNS
			*data = booldup( 1 );
		else
			*data = booldup(0);
#else
		int dns_disable;
		mib_get( MIB_CWMP_DNS_DISABLE, (void *)&dns_disable);
		if(dns_disable == 0) 
			*data = booldup( 1 );
		else
			*data = booldup( 0 );
		
#endif
	}
	else if( strcmp( lastname, "DNSOverrideAllowed" )==0 )
	{	
		/* TR098A2:Whether or not a manually set, non-empty DNS address can be overridden by a DNS entry received from the WAN. */
#if defined(MULTI_WAN_SUPPORT)
		vChar=pEntry->dnsAuto;
		if( vChar==0 ) //Manual set DNS
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
#else
		int dns;
		mib_get( MIB_DNS_MODE, (void *)&dns);
		if(dns == 1) //Manual set DNS
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
		
#endif		
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)
		vChar=pEntry->dnsAuto;
		char dns[64] = {0};
		char dns1[16] = {0};
		char dns2[16] = {0};
		char dns3[16] = {0};

		if( vChar==0 ) //manually set DNS
		{
			unsigned char *dns_tmp = pEntry->wanIfDns1;
			if(dns_tmp)
				sprintf(dns1, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns1, "0.0.0.0");

			dns_tmp = pEntry->wanIfDns2;
			if(dns_tmp)
				sprintf(dns2, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns2, "0.0.0.0");
			
			dns_tmp = pEntry->wanIfDns3;
				
			if(dns_tmp)
				sprintf(dns3, "%d.%d.%d.%d", dns_tmp[0], dns_tmp[1],dns_tmp[2],dns_tmp[3]);
			else
				sprintf(dns3, "0.0.0.0");

			sprintf(dns, "%s,%s,%s", dns1, dns2, dns3);

			*data = strdup( dns );
		}
		else
		{
			if(parseDNSForMultiWan(ipnum, dns)<0)
				*data = strdup("0.0.0.0");
			else
				*data = strdup( dns );
		}
#else
		int dns;
		mib_get( MIB_DNS_MODE, (void *)&dns);
		if(dns == 0) //Auto get DNS
		{
			parseDNSForAutoMode(buf, sizeof(buf));
		}
		else
		{
			getDNSList(buf);			
		}
		*data = strdup( buf );
#endif
	}
	else if( strcmp( lastname, "MaxMTUSize" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(pEntry->AddressType ==DHCP_CLIENT)
			*data = uintdup(pEntry->dhcpMtu);
		if(pEntry->AddressType ==DHCP_DISABLED)
			*data = uintdup(pEntry->staticIpMtu);
#else
		int mtu_size;
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
		if(wan_dhcp == DHCP_CLIENT || pppoeWithDhcpEnabled)
#else
		if(wan_dhcp == DHCP_CLIENT)
#endif
			mib_get( MIB_DHCP_MTU_SIZE, (void *)&mtu_size);
		else if(wan_dhcp == DHCP_DISABLED)
			mib_get( MIB_FIXED_IP_MTU_SIZE, (void *)&mtu_size);
		else
			mtu_size = 0;
		*data=intdup(mtu_size);
#endif		
	}
	else if( strcmp( lastname, "MACAddress" )==0 )
	{	
#if defined(MULTI_WAN_SUPPORT)	
		char wanif[IFNAMSIZ]={0};
		unsigned char *pMacAddr=NULL;
		struct sockaddr hwaddr={0};
		//ifGetName(PHY_INTF(pEntry->ifIndex), wanif, sizeof(wanif));
		getWanIfName(ipnum, wanif,IFNAMSIZ);
		if(!getInAddr(wanif, HW_ADDR, (void *)&hwaddr))
			sprintf(buf,"00:00:00:00:00:00");
		else{
			pMacAddr = (unsigned char *)hwaddr.sa_data;
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		}
		*data = strdup(buf);
#else
		char wanif[IFNAMSIZ];
		unsigned char *pMacAddr;
		struct sockaddr hwaddr;

		sprintf(wanif,"%s","eth1");

		if(!getInAddr(wanif, HW_ADDR, (void *)&hwaddr))
			*data = strdup("00:00:00:00:00:00");
		else
		{
			pMacAddr = (unsigned char *)hwaddr.sa_data;
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
				
			*data = strdup(buf);
		}
#endif	
	}
	else if(strcmp(lastname,"MACAddressOverride")==0)
	{
		*data=booldup(1);
	}
#if defined(MULTI_WAN_SUPPORT)
	else if(strcmp(lastname,"VlanEnabled")==0)
	{
		*data=booldup(pEntry->vlan);
	}
	else if(strcmp(lastname,"VlanID")==0)
	{
		*data=uintdup(pEntry->vlanid);
	}
	else if(strcmp(lastname,"VlanPriority")==0)
	{
		*data=uintdup(pEntry->vlanpriority);
	}
#endif
	else if( strcmp( lastname, "ConnectionTrigger" )==0 )
	{	
		*data = strdup( "AlwaysOn" );
	}
	else if( strcmp( lastname, "RouteProtocolRx" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		unsigned char ripver = pEntry->rip;
		if(ripver == DISABLE_MODE)
			*data = strdup("Off");
		else if(ripver == RIP1_MODE)
			*data = strdup("RIPv1");
		else if(ripver == RIP2_MODE)
			*data = strdup("RIPv2");
		else
			*data = strdup("Off");
#else
		*data=strdup("Off");
#endif
	}
	else if( strcmp( lastname, "ShapingRate" )==0 )
	{
		// TODO
		*data = intdup(-1);
	}
	else if( strcmp( lastname, "ShapingBurstSize" )==0 )
	{
		// TODO
		*data = uintdup(1500);
	}
#ifdef _PRMT_X_CT_COM_WANEXT_
	else if( strcmp( lastname, "X_CT-COM_LanInterface" )==0 )
	{
		char tmp[1024];
		getLanInterface(chainid, tmp);
		*data = strdup( tmp );

	}
	else if( strcmp( lastname, "X_CT-COM_ServiceList" )==0 )
	{
		convertFlag2ServiceList( pEntry->ServiceList, buf );
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "X_CT-COM_IPMode" ) == 0 )
	{
#ifdef CONFIG_IPV6
			switch(pEntry->IpProtocol)
			{
				case 0:
					*data = uintdup(2);
					break;
				case 1:
					*data = uintdup(0);
					break;
				case 2:
					*data = uintdup(1);
					break;
				default:
					return ERR_9003;
					break;
			}
#else
			*data = uintdup(0);
#endif
	}
	else if( strcmp( lastname, "X_CT-COM_IPv6ConnectionStatus" ) == 0 )
	{
		*data = strdup("");
	}
	else if( strcmp( lastname, "X_CT-COM_LanInterface-DHCPEnable" )==0 )
	{
			if(!pEntry->enableLanDhcp)
				*data=booldup(0);
			else
				*data=booldup(1);
	}
#endif //_PRMT_X_CT_COM_WANEXT_
	else if( strcmp( lastname, "PortMappingNumberOfEntries" )==0 )
	{
		int portEntityCount;
#ifdef MULTI_WAN_SUPPORT
		portEntityCount = getPortMappingTotalNum(ipnum);
		*data = uintdup(portEntityCount);
#else
		if(getPortMappingCount(&portEntityCount) == 0){ //o:OK
			*data = uintdup( portEntityCount );
		}else
			*data = uintdup( 0 );
#endif
	}
	else{
		return ERR_9005;
	}
	return 0;
}

int setWANIPCONENTITY(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	if( (name==NULL) || (entity==NULL)) return -1;

	char	*lastname = entity->info->name;
	char	*buf=data;
	int	chainid;
#if defined(MULTI_WAN_SUPPORT)	
	WANIFACE_T *pEntry, wan_entity;
	WANIFACE_T target[2]={0};
#endif
	char	ifname[16];
	unsigned int devnum,ipnum;
	int wan_dhcp;
	
	if( entity->info->type!=type ) return ERR_9006;

	devnum = getWANConDevInstNum( name );
	ipnum  = getWANIPConInstNum( name );
	if( (devnum==0) || (ipnum==0) ) return ERR_9005;
		
#if defined(MULTI_WAN_SUPPORT)
	pEntry = &wan_entity;
	*((char *)&wan_entity) = (char)ipnum;
	if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
		return ERR_9002;
		
	if( transfer2IfNameWithVlan(name, ifname)<0)
		return ERR_9002;

	/* set current handling WAN index */
	apmib_set(MIB_WANIFACE_CURRENT_IDX, (void *)&ipnum);
	memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
#else
	mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
#endif

	if( strcmp( lastname, "Enable" )==0 )
	{
#ifdef MULTI_WAN_SUPPORT
		if( data==NULL) return ERR_9007;
		
		int *i = data;
		if(pEntry->enable != *i)
		{
			/* going to change WAN status */
			pEntry->enable = *i;
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}else
			/* nothing changed */
			return 0;
#else
		int *i = data;
		unsigned int vChar=0;
		
		if(i==NULL) 
			return ERR_9007;

		vChar = DHCP_CLIENT;
		mib_set(MIB_WAN_DHCP, (void *)&vChar);
#endif // _ALPHA_DUAL_WAN_SUPPORT_
	
		return 1;
	}
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
//#ifdef _PRMT_WT107_	
	else if( strcmp( lastname, "Reset" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(data==NULL) return ERR_9007;
		int *i = data;
		if(*i==1 && pEntry->AddressType==DHCP_CLIENT)
		{
			/* renew if WAN is DHCPC */
			/* TODO: The device MUST initiate the reset after completion of the current CWMP session. */
			char tmpBuf[100];
    		int pid=0; 
    		memset(tmpBuf, '\0', 100);
    		sprintf(tmpBuf, "%s/wan_%d.pid", "/var/run", ipnum);
    		pid = getPid(tmpBuf);
			printf("[%s:%d] udhcpc pid:%d\n", __FUNCTION__, __LINE__, pid);
    		if ( pid > 0) {
        		kill(pid, SIGUSR2);
    			usleep(1000);
				kill(pid, SIGUSR1);
				return 0;
			}else
				return ERR_9002;
		}else
			/* do nothing */
			return 0;
#else
		int *i=data;
		if(i==NULL) return ERR_9007;
		if(*i==1) 
		{
			//resetChainID = chainid;
			printf("%s:%d\n",__FUNCTION__,__LINE__);
			gStartReset = 1;
			return 1;
		}
		return 0;
#endif
	}
//#endif
/*ping_zhang:20081217 END*/	
	else if( strcmp( lastname, "ConnectionType" )==0 )
	{		
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		int current_cmode = pEntry->cmode;
		/* IP_BRIDGE=0, IP_ROUTE=1, IP_PPP=2 */
		
		if( strcmp( buf, "IP_Routed" )==0 )
		{
			if(current_cmode == IP_ROUTE)
				/* do nothing, remain the same */
				return 0;
			else
			{
				/* change WAN from IP bridge to DHCP */
				pEntry->connDisable = 0;
				pEntry->cmode = IP_ROUTE;
				pEntry->brmode = BRIDGE_DISABLE;
				pEntry->AddressType = DHCP_CLIENT;
				pEntry->mtu = 1500;

				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
				else
					return 1;
			}
		}
		else if( strcmp( buf, "IP_Bridged" )==0 )
		{
			if(current_cmode == IP_BRIDGE)
				return 0; /* do nothing, remain the same */
			else
			{
				/* change WAN from DHCP to IP bridge */
				pEntry->connDisable = 0;
				pEntry->cmode = IP_BRIDGE;
				pEntry->AddressType = BRIDGE;
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
				else
					return 1;
			}
		}else if( strcmp( buf, "Unconfigured" )==0 ){
			/* Unconfigured */
			if(pEntry->connDisable==0)
			{
				pEntry->connDisable = 1;
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
				else
					return 1;
			}else
				/* do nothing, remain the same */
				return 0;
		}else
			return ERR_9007;
#else
		if( strcmp( buf, "IP_Routed" )==0 )
		{

			return 1;
                }
                else
		         return ERR_9007;
#endif
	}
	else if( strcmp( lastname, "Name" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9001;
		//printf("%s:%d buf=%s\n",__FUNCTION__,__LINE__,buf);
		strcpy(pEntry->WanName,buf);
		//printf("%s:%d WanName=%s\n",__FUNCTION__,__LINE__,pEntry->WanName);
		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else{
			//printf("%s:%d WanName=%s\n",__FUNCTION__,__LINE__,target[0].WanName);
			return 0;
			}
#else
		{
			char wanName[MAX_NAME_LEN+1]={0};
			if( buf==NULL ) return ERR_9007;
			if( strlen(buf)==0 ) return ERR_9001;
			snprintf(wanName,sizeof(wanName),"%s",buf);
			mib_set(MIB_CWMP_WANNAME,(void*)&wanName);			
		}
#endif
		return 0;
	}
	else if( strcmp( lastname, "NATEnabled" )==0 )
		{
			int *i=data;
			if(i==NULL) return ERR_9007;
	//		printf("%s:%d NATEnabled=%d\n",__FUNCTION__,__LINE__,*i);
#if defined(MULTI_WAN_SUPPORT)
			if(*i==0)
				pEntry->nat_disable=1;
			else
				pEntry->nat_disable=0;
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
			else
				return 1;
#else
		//not support now
#endif
			return 0;
		}

	else if( strcmp( lastname, "AddressingType" )==0 )
	{
		if( buf==NULL ) 
			return ERR_9007;
		if( strlen(buf)==0 ) 
			return ERR_9007;
		
#ifdef MULTI_WAN_SUPPORT
		int current_type = pEntry->AddressType;
		/* DHCP_DISABLED=0, DHCP_CLIENT=1 */
		if( strcmp(buf, "DHCP")==0 )
		{
			if(current_type==DHCP_CLIENT)
				/* do nothing */
				return 0;
			else
			{
				pEntry->AddressType = DHCP_CLIENT;
				
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
				else
					return 1;
			}
		}else if( strcmp(buf, "Static")==0 )
		{
			if(current_type==DHCP_DISABLED)
				/* do nothing */
				return 0;
			else
			{
				pEntry->AddressType = DHCP_DISABLED;
				
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					return ERR_9002;
				else
					return 1;
			}
		}else
			return ERR_9007;
#else
		unsigned int vChar=0;
		
		if( strcmp( buf, "DHCP" )==0 )
			vChar = DHCP_CLIENT;
		else if( strcmp( buf, "Static" )==0 )
			vChar =  DHCP_DISABLED;
		else
			return ERR_9007;
		mib_set(MIB_WAN_DHCP, (void*)&vChar);
		return 1;	
#endif // MULTI_WAN_SUPPORT
	}
	else if( strcmp( lastname, "ExternalIPAddress" )==0 )
	{
		if( buf==NULL ) 
			return ERR_9007;
		if( strlen(buf)==0 ) 
			return ERR_9007;
		struct in_addr in;
		
		if(  inet_aton( buf, &in )==0  ) 
			return ERR_9007;
#ifdef MULTI_WAN_SUPPORT
		if( pEntry->AddressType!=DHCP_DISABLED)
			/* This parameter is configurable only if the AddressingType is Static. */
			return ERR_9007;
		else
		{
			memcpy(pEntry->ipAddr, &in, sizeof(in));
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}
#else
		if(wan_dhcp!=DHCP_DISABLED)
			return ERR_9007;

		mib_set(MIB_WAN_IP_ADDR, (void *)&in);	
		return 1;		
#endif // MULTI_WAN_SUPPORT
	}
	else if( strcmp( lastname, "SubnetMask" )==0 )
	{
		struct in_addr in;
		
		if( buf==NULL ) 
			return ERR_9007;
		if( strlen(buf)==0 ) 
			return ERR_9007;
		if(  inet_aton( buf, &in )==0  ) 
			return ERR_9007;
		
#ifdef MULTI_WAN_SUPPORT
		if( pEntry->AddressType!=DHCP_DISABLED)
			/* This parameter is configurable only if the AddressingType is Static. */
			return ERR_9007;
		else
		{
			memcpy(pEntry->netMask, &in, sizeof(in));
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}
#else
		if(wan_dhcp!=DHCP_DISABLED)
			return ERR_9007;

		mib_set(MIB_WAN_SUBNET_MASK, (void *)&in);	

		return 1;	
#endif // MULTI_WAN_SUPPORT
	}
	else if( strcmp( lastname, "DefaultGateway" )==0 )
	{
		struct in_addr in;
		if( buf==NULL ) 
			return ERR_9007;
		if( strlen(buf)==0 ) 
			return ERR_9007;
		if(  inet_aton( buf, &in )==0  ) 
			return ERR_9007;
		
#ifdef MULTI_WAN_SUPPORT
		if( pEntry->AddressType!=DHCP_DISABLED)
			/* This parameter is configurable only if the AddressingType is Static. */
			return ERR_9007;
		else
		{
			memcpy(pEntry->remoteIpAddr, &in, sizeof(in));
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}
#else
		if(wan_dhcp!=DHCP_DISABLED)
			return ERR_9007;
		mib_set(MIB_WAN_DEFAULT_GATEWAY, (void *)&in);	

		return 1;
#endif // MULTI_WAN_SUPPORT
	}
	else if( strcmp( lastname, "MaxMTUSize" )==0 )
	{
		unsigned int *newmtu=(unsigned int *)data;
#if defined(MULTI_WAN_SUPPORT)
		if(newmtu==NULL || *newmtu ==0 || *newmtu >1540)
			/* unsignedInt-[1:1540] */
			return ERR_9007;

		/* NOTE: check MTU value legality is guaranteed by operator */
		if(pEntry->AddressType==DHCP_DISABLED)
		{
			pEntry->staticIpMtu = *newmtu;
		}else if(pEntry->AddressType==DHCP_CLIENT)
		{
			pEntry->dhcpMtu = *newmtu;
		}else
			return ERR_9007;
		
		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
#else
		if(wan_dhcp== DHCP_CLIENT){
		if(*newmtu > 1399 && *newmtu < 1501)
		{
			mib_set(MIB_DHCP_MTU_SIZE, (void *)newmtu);
			}else
			return ERR_9007;
		}else
		if(wan_dhcp== DHCP_DISABLED){
			if(*newmtu > 1399 && *newmtu < 1501)
			{
				mib_set(MIB_FIXED_IP_MTU_SIZE, (void *)newmtu);
			}else
			return ERR_9007;
		}else
			return ERR_9007;
		
		return 1;
#endif
	}
	else if( strcmp( lastname, "DNSEnabled" )==0 )
	{
		int *i=data;
		int dns_disable;
		
		if(i==NULL) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		/* DNSEnabled: 
			Whether or not the device SHOULD attempt to query a DNS server across this connection.
		 */
		if(pEntry->AddressType==DHCP_DISABLED)
			/* Can't set DNS to auto when static IP */
			return ERR_9007;
		
		pEntry->dnsDisable = (*i==0)?1:0;

		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
#else
		if(*i ==0)
			dns_disable=1; 
		else if(*i ==1)
			dns_disable=0; 

		mib_set( MIB_CWMP_DNS_DISABLE, (void *)&dns_disable);
		return 1;
#endif
	}
	else if( strcmp( lastname, "DNSOverrideAllowed" )==0 )
	{	
		int *i=data;
		int dns;
		
		if(i==NULL) return ERR_9007;
#if defined(MULTI_WAN_SUPPORT)
		/* Whether or not a manually set, non-empty DNS address can be overridden by a DNS entry received from the WAN. */
		/* ONLY when dnsAuto is off can DNSOverrideAllowed is 1 */

		pEntry->dnsAuto = (*i);

		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
#else
		if(*i ==0)
			dns=1; //manual mode
		else if(*i ==1)
			dns=0; //auto mode

		mib_set( MIB_DNS_MODE, (void *)&dns);
		return 1;
#endif
	}
	else if( strcmp( lastname, "DNSServers" )==0 )
	{
#if defined(MULTI_WAN_SUPPORT)
		if(buf == NULL || strlen(buf)==0)
			return ERR_9007;
		if(pEntry->dnsAuto==1)
			return ERR_9007;

		struct in_addr dns;
		
		/* Comma-separated list */
        char *p;
		int i = 2;

        p = strtok(buf ,",");
        if( inet_aton( p, &dns )==0 ) 
            return ERR_9007;
        memcpy(pEntry->wanIfDns1, &dns, sizeof(dns));
		
        while((p=strtok(NULL, ","))!=NULL)
        {
        	if(i>4)
        		/* Support for more than three DNS Servers is OPTIONAL */
				break;
            if(p)
            {
                if( inet_aton( p, &dns )==0 ) 
                    return ERR_9007;
                switch(i)
                {
                    case 2:
                        memcpy(pEntry->wanIfDns2, &dns, sizeof(dns));
						i++;
                        break;
					case 3:
						memcpy(pEntry->wanIfDns3, &dns, sizeof(dns));
						i++;
						break;
					default:
						break;
				}
			}
			else
				break;
		}
		
		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
#else
		int vInt = 0;
		if( (buf==NULL) || (strlen(buf)==0) )
		{	//automatically attain DNS
			vInt = 0; //automatically
			}
		else
		{
			if( setDNSList( buf ) == 0 )
			{
				vInt = 1;
		}
		}
		mib_set( MIB_DNS_MODE, (void *)&vInt);
		return 1;	
#endif
	}
	else if(strcmp(lastname,"MACAddress")==0)
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		
		char macBuf[64]={0};
		if(!mac_string_to_hex(buf, macBuf, 17))
			return ERR_9007;
		
#if defined(MULTI_WAN_SUPPORT)
		memcpy(pEntry->wanMacAddr,macBuf,sizeof(pEntry->wanMacAddr));

		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
#else
		apmib_set(MIB_WAN_MAC_ADDR,(void*)macBuf);
		return 1;
#endif
	}
#if defined(MULTI_WAN_SUPPORT)
	else if(strcmp(lastname,"VlanEnabled")==0)
	{
		int *i = data;
		if(i==NULL)
			return ERR_9007;

		pEntry->vlan = (*i==0)?0:1;
		
		memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
		if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
			return ERR_9002;
		else
			return 1;
	}
	else if(strcmp(lastname,"VlanID")==0)
	{
		unsigned int *i = data;
		if(i==NULL)
			return ERR_9007;
		if(*i>=1 && *i<=4094)
		{
			pEntry->vlanid = *i;
			
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}else
			return ERR_9007;
	}
	else if(strcmp(lastname,"VlanPriority")==0)
	{
		unsigned int *i = data;
		if(i==NULL)
			return ERR_9007;
		if(*i>=0 && *i<=7)
		{
			pEntry->vlanpriority = *i;
			
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				return ERR_9002;
			else
				return 1;
		}else
			return ERR_9007;
	}
#endif
	else if( strcmp( lastname, "ConnectionTrigger" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "AlwaysOn" )!=0 ) return ERR_9007;
		}
	else if( strcmp( lastname, "RouteProtocolRx" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "Off" )==0 ){
			
		return 0;
		}else
			return ERR_9007;
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int objWANIPConn(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
#if defined(MULTI_WAN_SUPPORT)
	WANIFACE_T *pEntry, wan_entity;
	WANIFACE_T target[2];
#endif
	unsigned int devnum;
	unsigned int num=0,i,maxnum=0;
	int Found=0;
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>name:%s(action:%d)\n", __FUNCTION__, __LINE__, name,type ) );
	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);fflush(NULL);
	if( (name==NULL) || (entity==NULL) ) return -1;

	devnum = getWANConDevInstNum( name );
	if(devnum==0) return ERR_9005;

	switch( type )
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **ptable = (struct CWMP_LINKNODE **)data;
			
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

#if defined(MULTI_WAN_SUPPORT)
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			maxnum = num;
			for( i=1; i<=num;i++ )
			{
				pEntry = &wan_entity;

				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				
				if( (pEntry->connDisable==0) && (pEntry->enable==1))
				{
					if( (pEntry->cmode == IP_ROUTE) 
#ifdef PPPOE_PASSTHROUGH
							    || ( (pEntry->cmode==IP_BRIDGE)&&(pEntry->brmode!=BRIDGE_PPPOE) )  
#endif
						    )
					{
						/*
						 * Instance number of this WANIPConnection is the same as wan device index.
						 */
						if( create_Object( ptable, tWANIPCONObject, sizeof(tWANIPCONObject), 1, pEntry->ConDevInstNum ) < 0 )
							return -1;
						CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ( "[%s:%d]add condev:%u ipcon:%u\n", __FUNCTION__, __LINE__, pEntry->ConDevInstNum, pEntry->ConIPInstNum));
					}
				}
			}//for
			add_objectNum( name, maxnum );
#else //#if defined(MULTI_WAN_SUPPORT)
			int wan_dhcp;
			int cwmp_ipconn_instnum;
			int cwmp_ipconn_created;
			
			mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
			mib_get( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
			mib_get( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);

			if( (wan_dhcp == DHCP_CLIENT) || (wan_dhcp == DHCP_DISABLED))
			{
				if(cwmp_ipconn_instnum == 0)
					cwmp_ipconn_instnum = 1;

				if( create_Object( ptable, tWANIPCONObject, sizeof(tWANIPCONObject), 1, cwmp_ipconn_instnum ) < 0 )
					return -1;

				maxnum = 1; //only support 1 ip connection
			}
			add_objectNum( name, maxnum );
			mib_set( MIB_CWMP_IPCON_INSTNUM, (void *)&maxnum);
#endif //#if defined(MULTI_WAN_SUPPORT)
			
			return 0;
		}
		case eCWMP_tADDOBJ:	     
		{
			int ret;
#if defined(MULTI_WAN_SUPPORT)	
#if 1
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return ERR_9005;
			Found = 0;
			
			/* check possibility of adding WAN */
			for( i=1; i<=WANIFACE_NUM;i++ )
			{
				pEntry = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				if(pEntry->enable==0)
				{
					Found = 1;
					apmib_set(MIB_WANIFACE_CURRENT_IDX, (void *)&i);
					memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
					break;
				}
			}

			if(Found==0) 
				return ERR_9005;/*do not find entry for update*/

			*(unsigned int*)data = i;
			ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANIPCONObject, sizeof(tWANIPCONObject), data );

			/* only do necessary WAN mib initial here for reuse */
			if( ret >= 0 )
			{
				pEntry->enable=1;
				pEntry->connDisable = 0;
				pEntry->cmode = IP_ROUTE;
				pEntry->brmode = BRIDGE_DISABLE;
				pEntry->AddressType = DHCP_CLIENT;
				pEntry->mtu = 1500;

				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T)); ///wan_entry has been updated
				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
				{
					ret=-1;
				}
			}
			notify_set_wan_changed();
			if( ret >= 0 )
				ret=1;
			return ret;
#endif // #end if 1

#else
			int wan_dhcp;
			int cwmp_ipconn_instnum;
			int num=0;
			int cwmp_ipconn_created;
			
			mib_get( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
			mib_get( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);

			num = get_ParameterNameCount(name,1);
			
			if(cwmp_ipconn_created == 1) // ip connection max is 1
	  			return ERR_9004;

			cwmp_ipconn_created = 1;
			
			CWMPDBG( 2, ( stderr, "<%s:%d>addobj,\n", __FUNCTION__, __LINE__) );

			if(cwmp_ipconn_instnum == 0)
				cwmp_ipconn_instnum = 1;

			*(unsigned int*)data = cwmp_ipconn_instnum;
			ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANIPCONObject, sizeof(tWANIPCONObject), data );		
			if(ret == 0)
			{
				wan_dhcp = DHCP_CLIENT;
				mib_set( MIB_WAN_DHCP, (void *)&wan_dhcp);				
                mib_set( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
                cwmp_ipconn_instnum = *(unsigned int*)data;
                mib_set( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);

				notify_set_wan_changed();
			}
			return 1;

#endif //#if defined(MULTI_WAN_SUPPORT)
		}	     
		case eCWMP_tDELOBJ:	     		
		{
			int ret;
			
#if defined(MULTI_WAN_SUPPORT)
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
			
			Found=0;
			memset( &target[0], 0, sizeof( WANIFACE_T ) );
			memset( &target[1], 0, sizeof( WANIFACE_T ) );
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			for( i=1; i<=num;i++ )
			{
				pEntry = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;
				
				if( (pEntry->ConDevInstNum==*(unsigned int*)data) 
					&& (pEntry->cmode==DHCP_CLIENT || pEntry->cmode==DHCP_DISABLED)){
				    Found=1;
					memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
					break;
				}
			}
			if(Found==0) 
				return ERR_9005;

			ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			if( ret==0 )
			{
				/* DeleteObject succeed */
				pEntry->enable = 0;
				
				memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T)); ///wan_entry has been updated

				if ( !mib_set(MIB_WANIFACE_MOD, (void *)&target)) 
					ret=ERR_9002;
				else
					ret=1;
			}
			return ret;
#else //#if defined(MULTI_WAN_SUPPORT)
			int whichOne=*(unsigned int*)data;
			int wan_dhcp;
			int cwmp_ipconn_instnum;
			int cwmp_ipconn_created;
			
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
			
			mib_get( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);
			
			if(whichOne != cwmp_ipconn_instnum) return ERR_9007; 

			ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
			if(ret == 0)
			{
			wan_dhcp = PPPOE;
			mib_set( MIB_WAN_DHCP, (void *)&wan_dhcp);

			
				/* change cwmp_pppconn_created to 0, after reboot the ppp connection will be not created*/
			cwmp_ipconn_created = 0;

				mib_set( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_ipconn_created);

			}

			CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d) eCWMP_tDELOBJ\n", __FUNCTION__, __LINE__, name,type ) );
			return ret;
#endif //#if defined(MULTI_WAN_SUPPORT)
		}
		case eCWMP_tUPDATEOBJ:
		{
#if defined(MULTI_WAN_SUPPORT)
			struct CWMP_LINKNODE *old_table;
			int has_new=0;

			//CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
			mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
			old_table = (struct CWMP_LINKNODE *)entity->next;
			entity->next = NULL;
			for( i=1; i<=num;i++ )
			{
				struct CWMP_LINKNODE *remove_entity=NULL;
				WANIFACE_T *p,wan_entity;

				p = &wan_entity;
				*((char *)&wan_entity) = (char)i;
				if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
					continue;

				if( p->connDisable==1 ) continue;

				if( (p->enable==1) && (p->cmode == IP_ROUTE
#ifdef PPPOE_PASSTHROUGH
					|| ( (p->cmode==IP_BRIDGE)&&(p->brmode!=BRIDGE_PPPOE) )  
#endif
				)) 
				{
					CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d]ConDevInstNum = %d\n", __FUNCTION__, __LINE__, p->ConDevInstNum));
					remove_entity = remove_SiblingEntity( &old_table, p->ConDevInstNum );
					if( remove_entity!=NULL )
					{
						add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
					}
					else
					{
						add_Object( name, (struct CWMP_LINKNODE **)&entity->next, tWANIPCONObject, sizeof(tWANIPCONObject), &(p->ConDevInstNum) );
						has_new=1;
					}
				}
			}

			if( old_table )
				destroy_ParameterTable( (struct CWMP_NODE *)old_table );

			return 0;
#else


			int has_new=0;
			struct CWMP_LINKNODE *old_table;
			int wan_dhcp;
			int num=0;
			int cwmp_ipconn_created, cwmp_ipconn_instnum;
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
			int pppoeWithDhcpEnabled = 0;
#endif


			num = get_ParameterNameCount(name,1);
			mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
			mib_get( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
#ifdef _ALPHA_DUAL_WAN_SUPPORT_
			mib_get(MIB_PPPOE_DHCP_ENABLED, (void *)&pppoeWithDhcpEnabled);
			if( wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED || pppoeWithDhcpEnabled)
#else
			if( wan_dhcp == DHCP_CLIENT || wan_dhcp == DHCP_DISABLED)
#endif
			{
				struct CWMP_LINKNODE *remove_entity=NULL;
				int ret = 0;

				old_table = (struct CWMP_LINKNODE *)entity->next;	     	
				entity->next = NULL;
			
				mib_get( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);

                remove_entity = remove_SiblingEntity( &old_table, cwmp_ipconn_instnum );
				if( remove_entity!=NULL )
                {
                    add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
				}else{
					unsigned int MaxInstNum=cwmp_ipconn_instnum;
					ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWANIPCONObject, sizeof(tWANIPCONObject), &MaxInstNum );

					cwmp_ipconn_instnum = MaxInstNum;									
				}
				
				if(ret == 0)
				{
					has_new=1;
                    mib_set( MIB_CWMP_IPCON_INSTNUM, (void *)&cwmp_ipconn_instnum);

				cwmp_ipconn_created = 1;
					mib_set( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
				}
				
				if( old_table )
					destroy_ParameterTable( (struct CWMP_NODE *)old_table );
				
			}
			else
			{
				cwmp_ipconn_created = 0;
				mib_set( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);
				CWMPDBG( 2, ( stderr, "<%s:%d>devnum:%d, ConPPPInstNum=0\n", __FUNCTION__, __LINE__, devnum) );
			}

			return 0;
				
#endif //#if defined(MULTI_WAN_SUPPORT)
	}
	}
	return -1;
}

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.WANEthernetLinkConfig.*/
int getWANETHLINKCONF(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	buf[256];
	int isFound=0;
	unsigned int devnum,num,i;
#if defined(MULTI_WAN_SUPPORT)	
	WANIFACE_T *pEntry,wan_entity;
#endif

	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	devnum = getWANConDevInstNum( name );
	if(devnum==0) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)	
	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
	for( i=1; i<=num;i++ )
	{
		pEntry = &wan_entity;
		*((char *)&wan_entity) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
			continue;
		if( pEntry->ConDevInstNum==devnum ){
			isFound=1;
			break;
		}
	}
	if(isFound==0) return ERR_9005;
#endif

	*type = entity->info->type;
	*data = NULL;

	if( strcmp( lastname, "EthernetLinkStatus" )==0 )
	{
#if 0
		*data = strdup("Unavailable");
#else
        FILE *fp_link = fopen("/proc/eth1/link_status", "r");
        if(fp_link==NULL)
            return ERR_9005;
        char buf[16]={0};
        fread(buf, 16, 1, fp_link);
        if(buf[0] == '0')
            *data = strdup("Down");
        else if(buf[0] == '1')
            *data = strdup("Up");
        else 
            *data = strdup("Unavailable");
        fclose(fp_link);
#endif
	}
	else
		return ERR_9005;

	return 0;
}

int setWANETHLINKCONF(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	return ERR_9005; // No value can be set for WANEthernetLinkConfig of standard TR-069.
}

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.*/
int getWANCONDEVENTITY(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
#if defined(MULTI_WAN_SUPPORT)	
	WANIFACE_T *p,wan_entity;
#endif
	unsigned int instnum, i,num, ipcnt=0,pppcnt=0;

	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	instnum = getWANConDevInstNum( name );
	if(instnum==0) return ERR_9005;

#if defined(MULTI_WAN_SUPPORT)
	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&num);
	for( i=1; i<=num;i++ )
	{
		p = &wan_entity;
		
		*((char *)&wan_entity) = (char)i;
		
		if(!mib_get(MIB_WANIFACE_TBL, (void *)&wan_entity))
			continue;	
			
		//if( p->ConDevInstNum==instnum )
		{
			switch( p->cmode )
			{
			case IP_BRIDGE:
#ifdef PPPOE_PASSTHROUGH
				if( p->connDisable==0 && p->enable==1)
				{
					if(p->brmode==BRIDGE_PPPOE)
						pppcnt++;
					else
						ipcnt++;
				}
				break;
#endif
			case IP_ROUTE:
				if(p->AddressType == DHCP_CLIENT || p->AddressType ==DHCP_DISABLED){
				if( p->connDisable==0 && p->enable==1) ipcnt++;
				}
				if(p->AddressType == PPPOE){
				if( p->connDisable==0 && p->enable==1) pppcnt++;
				}
				break;
			
			case IP_PPP:
				if(p->AddressType == PPPOE){
					if( p->connDisable==0 && p->enable==1) pppcnt++;
				}
				break;
			}
		}
	}

#else //#if defined(MULTI_WAN_SUPPORT)
		int wan_dhcp;

		int cwmp_ipconn_created;
		int cwmp_pppconn_created;

		mib_get( MIB_CWMP_IPCON_CREATED, (void *)&cwmp_ipconn_created);			
		mib_get( MIB_CWMP_PPPCON_CREATED, (void *)&cwmp_pppconn_created);

		ipcnt = cwmp_ipconn_created;
		pppcnt = cwmp_pppconn_created;
#endif //#if defined(MULTI_WAN_SUPPORT)		
	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "WANIPConnectionNumberOfEntries" )==0 )
	{
			*data = uintdup( ipcnt );
	}
	else if( strcmp( lastname, "WANPPPConnectionNumberOfEntries" )==0 )
	{	
			*data = uintdup( pppcnt );
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int objConDev(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	
	CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d)\n", __FUNCTION__, __LINE__, name,type ) );

	switch( type )
	{
		case eCWMP_tINITOBJ:
		{
			int num, MaxInstNum, i;
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;		
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) 
                return ERR_9002;

            /*
             * According to TR-098 spec of InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}.: 
             *    In the case of Ethernet (interface or link), only one WANConnectionDevice instance is supported.
             */
			{	
				MaxInstNum = 1;
				if( create_Object( c, tWANCONDEVObject, sizeof(tWANCONDEVObject), 1, 1 ) < 0 )
                    return ERR_9002;

				add_objectNum( name, MaxInstNum );
				CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d) eCWMP_tINITOBJ\n", __FUNCTION__, __LINE__, name,type ) );
				return 0;
			}
			break;
				
		}
		case eCWMP_tADDOBJ:
            return ERR_9001;
		case eCWMP_tDELOBJ:	     		
			return ERR_9001;
		case eCWMP_tUPDATEOBJ:
		{
            return 0;
        }
    }
	CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d) return -1\n", __FUNCTION__, __LINE__, name,type ) );

	return 0;
}

/**************************************************************************************/
/* utility functions*/
/**************************************************************************************/
#ifdef HOME_GATEWAY
#define _DHCPC_PID_PATH		"/etc/udhcpc"
#define _DHCPC_PROG_NAME	"udhcpc"

int isDhcpClientExist(char *name)
{
	char tmpBuf[100];
	struct in_addr intaddr;

	if ( getInAddr(name, IP_ADDR, (void *)&intaddr ) ) {
		snprintf(tmpBuf, 100, "%s/%s-%s.pid", _DHCPC_PID_PATH, _DHCPC_PROG_NAME, name);
		if ( getPid(tmpBuf) > 0)
			return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
int isConnectPPP()
{
	struct stat status;

	if ( stat("/etc/ppp/link", &status) < 0)
		return 0;

	return 1;
}
#endif

/*functions for get an object's number*/
unsigned int getInstNum( char *name, char *objname )
{
	unsigned int num=0;
	
	if( (objname!=NULL)  && (name!=NULL) )
	{
		char buf[256],*tok;
		sprintf( buf, ".%s.", objname );
		tok = strstr( name, buf );
		if(tok)
		{
			tok = tok + strlen(buf);
			sscanf( tok, "%u.%*s", &num );
		}
	}
	
	return num;
}

unsigned int getWANConDevInstNum( char *name )
{
	return getInstNum( name, "WANConnectionDevice" );
}

unsigned int getWANPPPConInstNum( char *name )
{
	return getInstNum( name, "WANPPPConnection" );
}

unsigned int getWANIPConInstNum( char *name )
{
	return getInstNum( name, "WANIPConnection" );
}


unsigned int getWANPortMapInstNum( char *name )
{
	return getInstNum( name, "PortMapping" );
}

/*ping_zhang:20080919 START:add for new telefonica tr069 request: dhcp option*/
#ifdef _PRMT_X_TELEFONICA_ES_DHCPOPTION_
unsigned int getSentDHCPOptInstNum( char *name )
{
	return getInstNum( name, "SentDHCPOption" );
}

unsigned int getReqDHCPOptInstNum( char *name )
{
	return getInstNum( name, "ReqDHCPOption" );
}
#endif
/*ping_zhang:20080919 END*/


int getPortMappingCount(int *portEntityCount)
{
	int count = 0;	

	mib_get(MIB_PORTFW_TBL_NUM, (void *)&count);
	*portEntityCount = count;

	return 0;
}

unsigned int getPortMappingMaxInstNum(int wanIf_idx)
{
	unsigned int ret = 0;	
	PORTFW_T port_entity;
	unsigned int total,i;

	mib_get(MIB_PORTFW_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{

		*((char *)&port_entity) = (char)i;
		if( mib_get( MIB_PORTFW_TBL, (void*)&port_entity ) )
		{
			if( (port_entity.InstanceNum > ret)
#ifdef MULTI_WAN_SUPPORT
				&& (port_entity.WANIfIndex == wanIf_idx)
#endif
			) 
				ret = port_entity.InstanceNum;
		}
	}
	return ret;
}

#ifdef MULTI_WAN_SUPPORT
unsigned int getPortMappingTotalNum(int wanIf_idx)
{
	unsigned int ret = 0;	
	PORTFW_T port_entity;
	unsigned int total,i;
	
	mib_get(MIB_PORTFW_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{

		*((unsigned char *)&port_entity) = (unsigned char)i;
		if( mib_get( MIB_PORTFW_TBL, (void*)&port_entity ) )
		{
			if(port_entity.WANIfIndex != wanIf_idx)
				continue;
			ret++;
		}
	}
	return ret;
}
#endif

int getPortMappingByInstNum( unsigned char wanif_idx, unsigned int instnum, PORTFW_T *c, unsigned int *chainID )
{
	unsigned int total,i;

	if( (instnum == 0) || (c==NULL) || (chainID==NULL) ) return -1;

	mib_get(MIB_PORTFW_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{
		*((char *)c) = (char)i;
		 mib_get( MIB_PORTFW_TBL, (void*)c);	 
#ifdef MULTI_WAN_SUPPORT
		 if( c->WANIfIndex != wanif_idx)
			continue;
#endif
		if(c->InstanceNum == instnum)
		{
			*chainID = i;
     		return 0;
     	}
	}
	
	return -1;
}

/*for DefaultConnectionService, set/get default route */
int getDefaultRouteIfaceName( char *name )
{
#if defined(MULTI_WAN_SUPPORT)
	int total,i;
	WANIFACE_T *pEntry,wan_entity;

	if( name==NULL ) return -1;
	name[0]=0;

#ifdef DEFAULT_GATEWAY_V2
	{
		unsigned int dgw;
		if (mib_get(MIB_ADSL_WAN_DGW_ITF, (void *)&dgw) != 0)
		{
			transfer2PathName( dgw, name );
		}
	}
#else	
	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);

	for( i=1; i<=total; i++ )
	{
		pEntry = &wan_entity;
		
		*((char *)pEntry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)pEntry))
			continue;
		
		if(pEntry->dgw==1)
		{
			char strfmt[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d"; //wt-121v8-3.33, no trailing dot
			char ipstr[]="WANIPConnection";
			char pppstr[]="WANPPPConnection";
			char *pconn=NULL;
			unsigned int instnum=0;

			if( (pEntry->cmode==IP_PPP) 
#ifdef PPPOE_PASSTHROUGH
			   || ((pEntry->cmode==IP_BRIDGE)&&(pEntry->brmode==BRIDGE_PPPOE))
#endif
			    )
			{
				pconn = pppstr;
				instnum = pEntry->ConPPPInstNum;
			}else{
				pconn = ipstr;
				instnum = pEntry->ConIPInstNum;
			}

			if( pEntry->connDisable==0 )
			{
				sprintf( name, strfmt, pEntry->ConDevInstNum , pconn, instnum );
				break;
			}else
				return -1;

		}
	}
#endif
	//name = InternetGatewayDevice.WANDevice.1.WANConnection.2.WANPPPConnection.1.

#else
	int wan_dhcp;
	char strfmt[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.%s.1"; //wt-121v8-3.33, no trailing dot
	char ipstr[]="WANIPConnection";
	char pppstr[]="WANPPPConnection";

	mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
	if(wan_dhcp== PPPOE)
		sprintf( name, strfmt, pppstr );
	else
		sprintf( name, strfmt, ipstr );
#endif //#if defined(MULTI_WAN_SUPPORT)
	return 0;
}

int setDefaultRouteIfaceName( char *name )
{
#if defined(MULTI_WAN_SUPPORT)
	//name = InternetGatewayDevice.WANDevice.1.WANConnection.2.WANPPPConnection.1.
	WANIFACE_T *pEntry, wan_entity;
	WANIFACE_T target[2];
	struct CWMP_LEAF *e=NULL;
	unsigned int devnum,ipnum, pppnum;
	int total,i;
	
	if(name==NULL) return -1;
	if( get_ParameterEntity( name, &e ) < 0 ) return -1;


#ifdef DEFAULT_GATEWAY_V2
	{
		unsigned int dgw;
		dgw = transfer2IfIndex( name );
		if( dgw==DUMMY_IFINDEX ) return -1;
		mib_set(MIB_ADSL_WAN_DGW_ITF, (void *)&dgw);
	}
#else
	devnum = getWANConDevInstNum( name );
	ipnum  = getWANIPConInstNum( name );
	pppnum = getWANPPPConInstNum( name );
	if( (devnum==0) || ((ipnum==0)&&(pppnum==0)) ) return -1;

	//delete old/update new default route
	
	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);
#ifdef CONFIG_BOA_WEB_E8B_CH
	for( i=1; i<=total; i++ )
	{
		pEntry = &wan_entity;
		*((char *)pEntry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)pEntry))
			continue;


		if( (pEntry->ConDevInstNum==devnum) &&
		    (pEntry->ConIPInstNum==ipnum) &&
		    (pEntry->ConPPPInstNum==pppnum) ) //new default route
		{
			if(!(pEntry->applicationtype&X_CT_SRV_INTERNET)) // Only INTERNET connection can be default connection
				return -1;
		}
	}
#endif
	for( i=1; i<=total; i++ )
	{
		pEntry = &wan_entity;
		memset( &target[0], 0, sizeof( WANIFACE_T ) );
		memset( &target[1], 0, sizeof( WANIFACE_T ) );
		*((char *)pEntry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)pEntry))
			continue;
		
		memcpy(&target[0], &wan_entity, sizeof(WANIFACE_T));
		if( (pEntry->ConDevInstNum==devnum) &&
		    (pEntry->ConIPInstNum==ipnum) &&
		    (pEntry->ConPPPInstNum==pppnum) ) //new default route
		{
			pEntry->dgw=1;
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			mib_set(MIB_WANIFACE_MOD, (void *)&target);
		}else if(pEntry->dgw==1) //old default route
		{
			pEntry->dgw=0;
			memcpy(&target[1], &wan_entity, sizeof(WANIFACE_T));
			mib_set(MIB_WANIFACE_MOD, (void *)&target);
		}
	}
#endif

#else

#endif //#if defined(MULTI_WAN_SUPPORT)
	return 0;
}

int transfer2IfName( char *name, char *ifname )
{
	struct CWMP_LEAF *e=NULL;
	
	if( (name==NULL) || ( ifname==NULL ) ) return -1;
	if( get_ParameterEntity( name, &e ) < 0 ) return -1;
	ifname[0]=0;
	
	if( strcmp( name, "InternetGatewayDevice.LANDevice.1" )==0 )
		strcpy( ifname, "br0" );
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1" )==0 )
		strcpy( ifname, "br0" );
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1" )==0 )
		strcpy( ifname, "eth0" );
#ifdef WLAN_SUPPORT
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1" )==0 )
	{
		CWMP_WLANCONF_T wlanConf;
		int rootIdx=0,vwlanIdx=0;
		char wlan_ifname[10]={0};
		
		getWLANIdxFromInstNum(1, &wlanConf, &rootIdx, &vwlanIdx);

		if(vwlanIdx == 0)
			sprintf(wlan_ifname,"wlan%d",rootIdx);
		else
			sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx);
		
		strcpy( ifname, wlan_ifname );
	}
#ifdef WLAN_MBSSID
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2" )==0 )
	{
		CWMP_WLANCONF_T wlanConf;
		int rootIdx=0,vwlanIdx=0;
		char wlan_ifname[10]={0};
		
		getWLANIdxFromInstNum(2, &wlanConf, &rootIdx, &vwlanIdx);

		if(vwlanIdx == 0)
			sprintf(wlan_ifname,"wlan%d",rootIdx);
		else
			sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx);
		
		strcpy( ifname, wlan_ifname );
	}
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3" )==0 )
	{
		CWMP_WLANCONF_T wlanConf;
		int rootIdx=0,vwlanIdx=0;
		char wlan_ifname[10]={0};
		
		getWLANIdxFromInstNum(3, &wlanConf, &rootIdx, &vwlanIdx);

		if(vwlanIdx == 0)
			sprintf(wlan_ifname,"wlan%d",rootIdx);
		else
			sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx);
		
		strcpy( ifname, wlan_ifname );
	}
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4" )==0 )
	{
		CWMP_WLANCONF_T wlanConf;
		int rootIdx=0,vwlanIdx=0;
		char wlan_ifname[10]={0};
		
		getWLANIdxFromInstNum(4, &wlanConf, &rootIdx, &vwlanIdx);

		if(vwlanIdx == 0)
			sprintf(wlan_ifname,"wlan%d",rootIdx);
		else
			sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx);
		
		strcpy( ifname, wlan_ifname );
	}
	else if( strcmp( name, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.5" )==0 )
	{
		CWMP_WLANCONF_T wlanConf;
		int rootIdx=0,vwlanIdx=0;
		char wlan_ifname[10]={0};
		
		getWLANIdxFromInstNum(5, &wlanConf, &rootIdx, &vwlanIdx);

		if(vwlanIdx == 0)
			sprintf(wlan_ifname,"wlan%d",rootIdx);
		else
			sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx);
		
		strcpy( ifname, wlan_ifname );
	}
#endif //WLAN_MBSSID
#endif //WLAN_SUPPORT
	else //wan interface
	{
#if defined(MULTI_WAN_SUPPORT)
		if( strstr( name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.")!=NULL )
		{
			/* IPConnection */
			int ipnum;
			printf("[%s:%d]name=%s\n", __FUNCTION__, __LINE__, name);
			ipnum = getWANIPConInstNum(name);
			/*
			 *  1     2      3     4
			 * eth1, eth8, eth9, eth10
			 */
			sprintf(ifname, "eth%d", (ipnum==1)?1:(ipnum+6));
			printf("[%s:%d]ifname=%s\n", __FUNCTION__, __LINE__, ifname);
		}else if( strstr( name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.")!=NULL )
		{
			/* PPPConnection */
			int pppnum;
			printf("[%s:%d]name=%s\n", __FUNCTION__, __LINE__, name);
			pppnum = getWANPPPConInstNum( name );
			/*
			 *  1     2      3     4
			 * ppp1, ppp8, ppp9, ppp10
			 */
			sprintf(ifname, "ppp%d", (pppnum==1)?1:(pppnum+6));
			printf("[%s:%d]ifname=%s\n", __FUNCTION__, __LINE__, ifname);
		}
#else
		int wan_dhcp;
		mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);

		if (wan_dhcp == PPPOE)
			strcpy( ifname, "ppp0" );
		else
			strcpy( ifname, "eth1" );
#endif
	}
	return 0;	
}

int transfer2PathNamefromItf( char *ifname, char *pathname )
{
	struct CWMP_LEAF *e=NULL;
	
	if( (ifname==NULL) || ( pathname==NULL ) ) return -1;
	pathname[0]=0;
	
	//if( strcmp( ifname, "br0" )==0 )
	//	strcpy( pathname, "InternetGatewayDevice.LANDevice.1" );
	//else 
	if( strcmp( ifname, "br0" )==0 )
		strcpy( pathname, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1" );
	else if( strcmp( ifname, "eth0" )==0 )
		strcpy( pathname, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1" );
	else if( strncmp( ifname, "eth", strlen("eth") )==0 ){
#ifdef MULTI_WAN_SUPPORT
		unsigned int wan_idx = getWanIdx(ifname);
		sprintf(pathname, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.%d", wan_idx);
#else
		strcpy(pathname, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1");
#endif
	}else if( strcmp( ifname, "ppp" )==0 ){
#ifdef MULTI_WAN_SUPPORT
		unsigned int wan_idx = getWanIdx(ifname);
		sprintf(pathname, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.%d", wan_idx);
#else
		strcpy(pathname, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1");
#endif
	}

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] pathname:%s\n", __FUNCTION__, __LINE__, pathname));
	return 0;	
}

unsigned int transfer2IfIndex( char *name )
{
#if defined(MULTI_WAN_SUPPORT)
	struct CWMP_LEAF *e=NULL;
	unsigned int ret=DUMMY_IFINDEX;
	
	if( name==NULL ) return ret;
	if( get_ParameterEntity( name, &e ) < 0 ) return ret;
	
	{
		unsigned int devnum,ipnum,pppnum;
		WANIFACE_T *pEntry,wan_entity;
		int total,i;
		
		devnum = getWANConDevInstNum( name );
		ipnum  = getWANIPConInstNum( name );
		pppnum = getWANPPPConInstNum( name );
		if( (devnum==0) || ((ipnum==0)&&(pppnum==0)) ) return -1;

		mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);
		for( i=1; i<=total; i++ )
		{
			pEntry = &wan_entity;

			*((char *)pEntry) = (char)i;
			if(!mib_get(MIB_WANIFACE_TBL, (void *)pEntry))
				continue;
			
			if( (pEntry->ConDevInstNum==devnum) &&
			    (pEntry->ConIPInstNum==ipnum) &&
			    (pEntry->ConPPPInstNum==pppnum) ) 
			{
				ret = pEntry->ifIndex;
				break;
			}
		}		
	}
	return ret;
#else
	return 1;
#endif
}

int transfer2PathName( unsigned int ifindex, char *name )
{
#if defined(MULTI_WAN_SUPPORT)
	int total,i;
	WANIFACE_T *pEntry, wan_entity;

	if( ifindex==DUMMY_IFINDEX ) return -1;
	if( name==NULL ) return -1;
	name[0]=0;
	
	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);
	for( i=1; i<=total; i++ )
	{
		pEntry = &wan_entity;
		
		*((char *)pEntry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)pEntry))
			continue;
		
		if(pEntry->ifIndex==ifindex)
		{
			char strfmt[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d"; //wt-121v8-3.33, no trailing dot
			char ipstr[]="WANIPConnection";
			char pppstr[]="WANPPPConnection";
			char *pconn=NULL;
			unsigned int instnum=0;

			if( (pEntry->cmode==IP_PPP) 
#ifdef PPPOE_PASSTHROUGH
			    || ((pEntry->cmode==IP_BRIDGE)&&(pEntry->brmode==BRIDGE_PPPOE))
#endif
			    )
			{
				pconn = pppstr;
				instnum = pEntry->ConPPPInstNum;
			}else{
				pconn = ipstr;
				instnum = pEntry->ConIPInstNum;
			}

			if( pEntry->connDisable==0 )
			{
				sprintf( name, strfmt, pEntry->ConDevInstNum , pconn, instnum );
				break;
			}else
				return -1;
		}
	}
	//name = InternetGatewayDevice.WANDevice.1.WANConnection.2.WANPPPConnection.1.
#else

	int wan_dhcp;
	char strfmt[]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.%s.1"; //wt-121v8-3.33, no trailing dot
	char ipstr[]="WANIPConnection";
	char pppstr[]="WANPPPConnection";

	mib_get( MIB_WAN_DHCP, (void *)&wan_dhcp);
	if(wan_dhcp== PPPOE)
		sprintf( name, strfmt, pppstr );
	else
		sprintf( name, strfmt, ipstr );
#endif
	return 0;
		
}

/*ppp utilities*/
/*refer to fmstatus.c & utility.c*/
char *PPP_CONF = "/var/ppp/ppp.conf";
char *PPPOE_CONF = "/var/ppp/pppoe.conf";
char *PPPOA_CONF = "/var/ppp/pppoa.conf";
char *PPP_PID = "/var/run/spppd.pid";
int getPPPConStatus( char *pppname, char *status )
{
	char buff[256];
	FILE *fp;
	char strif[6],tmpst[32];
	int  ret=-1;

	if( (pppname==NULL) || (status==NULL) ) return -1;
	
	status[0]=0;
	if (!(fp=fopen(PPP_CONF, "r")))
	{
		fclose(fp);
		fprintf( stderr, "%s not exists.\n", PPP_CONF);
		return -1;
	}else{
		fgets(buff, sizeof(buff), fp);
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{
			//if   dev   dev_v   gw phase          username                password
			if(sscanf(buff, "%s %*s %*s %*s %s", strif, tmpst)!=2) 
			{
				fprintf( stderr, "Unsuported ppp configuration format\n");
				break;
			}
			if ( !strcmp(pppname, strif) )
			{
				strcpy( status, tmpst );
				ret=0;
				break;
			}
		}
		fclose(fp);
	}
	return ret;
}

