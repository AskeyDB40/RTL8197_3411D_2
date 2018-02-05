#include "prmt_landevice_wlan.h"
#include "prmt_landevice.h"
#include <linux/wireless.h>
//#include "mibtbl.h"

//#include <rtk/utility.h>
#ifdef WLAN_SUPPORT

#ifdef TELEFONICA_DEFAULT_CFG
#define _SHOW_WLAN_KEY_WHEN_GET_
#endif //TELEFONICA_DEFAULT_CFG

#define GETPSKINDEX(a,b) ( (unsigned char)(((a&0xf)<<4)|(b&0xf)) )
#define	WLANUPDATETIME	90

//CONFIG_WLAN_SETTING_T cwmpWlan, *pcwmpWlan;

char		gLocationDescription[4096]={0};
char		gWLANExternalRegs[ sizeof(WSC_ER_T) * MAX_EXTERNAL_REGISTRAR_NUM];
char		gWLANAssociations[ sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1) ];
char		gWLANKeyInfo[64*(MAX_STA_NUM+1)]={0};
time_t		gWLANAssUpdateTime=0;
unsigned int	gWLANTotalClients=0;
unsigned int	gWLANTotalERNumbers=0;
int		gWLANIDForAssInfo=-1; /*-1:no info, 0:wlan0, 1:wlan0-vap0, 2:wlan-vap1.....*/
#ifdef MBSSID
int MaxWLANIface=NUM_WLAN_INTERFACE*(NUM_VWLAN+2);
#else
int MaxWLANIface=NUM_WLAN_INTERFACE;
#endif

extern int clone_wlaninfo_set(CONFIG_WLAN_SETTING_T *wlanptr, int rootwlan_idx,int vwlan_idx,int Newrootwlan_idx,int Newvwlan_idx, int ChangeRFBand );
static int _is_hex(char c);
static int string_to_hex(char *string, unsigned char *key, int len);
int getWLANIdxFromInstNum(int instnum, CWMP_WLANCONF_T *pwlanConf, int *rootIdx, int *vwlan_idx);
int getNewWLANIdxFromReq(int *rootIdx, int *vwlan_idx, int ReqBand);
int updateWLANAssociations(void);
int loadWLANAssInfoByInstNum( unsigned int instnum );
int getWLANSTAINFO(int id, WLAN_STA_INFO_T *info);
int getRateStr( unsigned short rate, char *buf );
int setRateStr( char *buf, unsigned short *rate );
int setRateStr_Basic( char *buf, unsigned short *rate );
int getIPbyMAC( char *mac, char *ip );

unsigned int getWLANConfInstNum( char *name );
unsigned int getWEPInstNum( char *name );
unsigned int getAssDevInstNum( char *name );
unsigned int getPreSharedKeyInstNum( char *name );
unsigned int getWPSRegistrarInstNum( char *name );
int setWPSCfgMethod( char *buf, unsigned short *method );
void set_11ac_txrate(WLAN_STA_INFO_Tp pInfo,char* txrate);
int tranRate(WLAN_STA_INFO_T *pInfo, unsigned char rate);

#ifdef E8B_NEW_DIAGNOSE
void writeSSIDFile(char *msg, int index)
{
	FILE *fp;
	char buf[32];

	fp = fopen(NEW_SETTING, "r+");
	if (fp) {
		fgets(buf, sizeof(buf), fp);
		if (strcmp(buf, SSID_FILE)) {
			clearssidfile();
		} else {
			rewind(fp);
			ftruncate(fileno(fp), 0);
			fprintf(fp, "%s", SSID_FILE);
		}
		fclose(fp);
	}

	sprintf(buf, "%s%d", SSID_FILE, index);
	fp = fopen(buf, "w");
	if (fp) {
		fprintf(fp, msg);
		fclose(fp);

		fp = fopen(REMOTE_SETSAVE_FILE, "r");
		if (fp) {
			fclose(fp);
			unlink(REMOTE_SETSAVE_FILE);
			va_cmd("/bin/flatfsd", 1, 1, "-s");
		}
	}
}
#endif

/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WPS.xxx
*******************************************************************************/
#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
struct CWMP_OP tWPSEntityLeafOP = { getWPSEntity, setWPSEntity };
struct CWMP_PRMT tWPSEntityLeafInfo[] =
{
/*(name,						type,			flag,						op)*/
{"Enable",						eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWPSEntityLeafOP},
{"DeviceName",					eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"DevicePassword",				eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWPSEntityLeafOP},
{"UUID",						eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"Version",						eCWMP_tUINT,	CWMP_READ,				&tWPSEntityLeafOP},
{"ConfigMethodsSupported",		eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"ConfigMethodsEnabled",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWPSEntityLeafOP}, 
{"SetupLockedState",			eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"SetupLock",					eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWPSEntityLeafOP},
{"ConfigurationState",			eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"LastConfigurationError",		eCWMP_tSTRING,	CWMP_READ,				&tWPSEntityLeafOP},
{"RegistrarNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,				&tWPSEntityLeafOP},
{"RegistrarEstablished",		eCWMP_tBOOLEAN,	CWMP_READ,				&tWPSEntityLeafOP},
};
enum eWPSEntityLeaf
{
	eEnable,
	eDeviceName,
	eDevicePassword,
	eUUID,
	eVersion,
	eConfigMethodsSupported,
	eConfigMethodsEnabled,
	eSetupLockedState,
	eSetupLock,
	eConfigurationState,
	eLastConfigurationError,
	eRegistrarNumberOfEntries,
	eRegistrarEstablished	
};
struct CWMP_LEAF tWPSEntityLeaf[] =
{
{ &tWPSEntityLeafInfo[eEnable] },
{ &tWPSEntityLeafInfo[eDeviceName] },
{ &tWPSEntityLeafInfo[eDevicePassword] },
{ &tWPSEntityLeafInfo[eUUID] },
{ &tWPSEntityLeafInfo[eVersion] },
{ &tWPSEntityLeafInfo[eConfigMethodsSupported] },
{ &tWPSEntityLeafInfo[eConfigMethodsEnabled] },
{ &tWPSEntityLeafInfo[eSetupLockedState] },
{ &tWPSEntityLeafInfo[eSetupLock] },
{ &tWPSEntityLeafInfo[eConfigurationState] },
{ &tWPSEntityLeafInfo[eLastConfigurationError] },
{ &tWPSEntityLeafInfo[eRegistrarNumberOfEntries] },
{ &tWPSEntityLeafInfo[eRegistrarEstablished] },
{ NULL }
};

struct CWMP_OP tWLAN_WPSRegistrar_OP = { NULL, objWPSRegistrar };
struct CWMP_PRMT tWPSObjectInfo[] =
{
/*(name,			type,			flag,				op)*/
{"Registrar",		eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE,	&tWLAN_WPSRegistrar_OP},
};
enum eWPSObject
{
	Registrar0
};
struct CWMP_NODE tWPSObject[] =
{
/*info,  							leaf,							next)*/
{&tWPSObjectInfo[Registrar0],	NULL,						NULL},
{NULL,							NULL,						NULL}
};

/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WPS.Registrar.{i}.
*******************************************************************************/
struct CWMP_OP tWPSRegistrarEntityLeafOP = { getWPSRegistrarEntity, setWPSRegistrarEntity };
struct CWMP_PRMT tWPSRegistrarEntityLeafInfo[] =
{
/*(name,						type,			flag,						op)*/
{"Enable",						eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWPSRegistrarEntityLeafOP},
{"UUID",						eCWMP_tSTRING,	CWMP_READ,				&tWPSRegistrarEntityLeafOP},
{"DeviceName",					eCWMP_tSTRING,	CWMP_READ,				&tWPSRegistrarEntityLeafOP}
};
enum eWPSRegistrarEntityLeaf
{
	eEnable1,
	eUUID1,	
	eDeviceName1
};
struct CWMP_LEAF tWPSRegistrarEntityLeaf[] =
{
{ &tWPSRegistrarEntityLeafInfo[eEnable1] },
{ &tWPSRegistrarEntityLeafInfo[eUUID1] },
{ &tWPSRegistrarEntityLeafInfo[eDeviceName1] },
{ NULL }
};

struct CWMP_PRMT tWPSRegistrarEntityObjectInfo[] =
{
/*(name,			type,			flag,						op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_LNKLIST,	NULL}
};
enum eWPSRegistrarEntityObject
{
	eWPSRegistrar0	
};
struct CWMP_LINKNODE tWPSRegistarObject[] =
{
/*info,  												leaf,							next) 	sibling,		instnum)*/
{&tWPSRegistrarEntityObjectInfo[eWPSRegistrar0],	tWPSRegistrarEntityLeaf,	NULL,		NULL,	0}
};
#endif	// WIFI_SIMPLE_CONFIG_SUPPORT

/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.Stats.
*******************************************************************************/
struct CWMP_OP tStatsOP = { getStatsEntity, NULL };
struct CWMP_PRMT tStatsEntityLeafInfo[] =
{
/*(name,							type,			flag,				op)*/
{"ErrorsSent",						eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"ErrorsReceived",					eCWMP_tUINT,	CWMP_READ,		&tStatsOP},
{"UnicastPacketsSent",				eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"UnicastPacketsReceived",			eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"DiscardPacketsSent",				eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"DiscardPacketsReceived",			eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"MulticastPacketsSent",			eCWMP_tUINT,	CWMP_READ,		&tStatsOP},
{"MulticastPacketsReceived",		eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"BroadcastPacketsSent",			eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"BroadcastPacketsReceived",		eCWMP_tUINT, 	CWMP_READ,		&tStatsOP},
{"UnknownProtoPacketsReceived",		eCWMP_tUINT, 	CWMP_READ,		&tStatsOP}
};
enum eStatsEntityLeaf
{
	eErrorsSent,
	eErrorsReceived,
	eUnicastPacketsSent,
	eUnicastPacketsReceived,
	eDiscardPacketsSent,
	eDiscardPacketsReceived,
	eMulticastPacketsSent,
	eMulticastPacketsReceived,
	eBroadcastPacketsSent,
	eBroadcastPacketsReceived,
	eUnknownProtoPacketsReceived	
};
struct CWMP_LEAF tStatsEntityLeaf[] =
{
{ &tStatsEntityLeafInfo[eErrorsSent] },
{ &tStatsEntityLeafInfo[eErrorsReceived] },
{ &tStatsEntityLeafInfo[eUnicastPacketsSent] },
{ &tStatsEntityLeafInfo[eUnicastPacketsReceived] },
{ &tStatsEntityLeafInfo[eDiscardPacketsSent] },
{ &tStatsEntityLeafInfo[eDiscardPacketsReceived] },
{ &tStatsEntityLeafInfo[eMulticastPacketsSent] },
{ &tStatsEntityLeafInfo[eMulticastPacketsReceived] },
{ &tStatsEntityLeafInfo[eBroadcastPacketsSent] },
{ &tStatsEntityLeafInfo[eBroadcastPacketsReceived] },
{ &tStatsEntityLeafInfo[eUnknownProtoPacketsReceived] }, 
{ NULL }
};


/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.PreSharedKey.
*******************************************************************************/
struct CWMP_OP tPreSharedKeyEntityLeafOP = { getPreSharedKeyEntity, setPreSharedKeyEntity };
struct CWMP_PRMT tPreSharedKeyEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"PreSharedKey",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPreSharedKeyEntityLeafOP},
{"KeyPassphrase",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPreSharedKeyEntityLeafOP},
{"AssociatedDeviceMACAddress",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPreSharedKeyEntityLeafOP}
};
enum ePreSharedKeyEntityLeaf
{
	ePreSharedKey,
	eKeyPassphrase,
	ePreAssociatedDeviceMACAddress
};
struct CWMP_LEAF tPreSharedKeyEntityLeaf[] =
{
{ &tPreSharedKeyEntityLeafInfo[ePreSharedKey] },
{ &tPreSharedKeyEntityLeafInfo[eKeyPassphrase] },
{ &tPreSharedKeyEntityLeafInfo[ePreAssociatedDeviceMACAddress] },
{ NULL }
};


struct CWMP_PRMT tPreSharedKeyObjectInfo[] =
{
/*(name,	type,		flag,		op)*/
{"1",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"2",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"3",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"4",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"5",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"6",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"7",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"8",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"9",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"10",		eCWMP_tOBJECT,	CWMP_READ,	NULL}
};
enum ePreSharedKeyObject
{
	ePreSharedKey1,
	ePreSharedKey2,
	ePreSharedKey3,
	ePreSharedKey4,
	ePreSharedKey5,
	ePreSharedKey6,
	ePreSharedKey7,
	ePreSharedKey8,
	ePreSharedKey9,
	ePreSharedKey10
};
struct CWMP_NODE tPreSharedKeyObject[] =
{
/*info,  					leaf,				next)*/
{&tPreSharedKeyObjectInfo[ePreSharedKey1],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey2],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey3],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey4],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey5],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey6],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey7],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey8],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey9],	tPreSharedKeyEntityLeaf,	NULL},
{&tPreSharedKeyObjectInfo[ePreSharedKey10],	tPreSharedKeyEntityLeaf,	NULL},
{NULL,						NULL,				NULL}
};

/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.WEPKey.
*******************************************************************************/
struct CWMP_OP tWEPKeyEntityLeafOP = { getWEPKeyEntity, setWEPKeyEntity };
struct CWMP_PRMT tWEPKeyEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"WEPKey",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWEPKeyEntityLeafOP}
};
enum eWEPKeyEntityLeaf
{
	eWEPKey
};
struct CWMP_LEAF tWEPKeyEntityLeaf[] =
{
{ &tWEPKeyEntityLeafInfo[eWEPKey] },
{ NULL }
};


struct CWMP_PRMT tWEPKeyObjectInfo[] =
{
/*(name,	type,		flag,		op)*/
{"1",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"2",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"3",		eCWMP_tOBJECT,	CWMP_READ,	NULL},
{"4",		eCWMP_tOBJECT,	CWMP_READ,	NULL}
};
enum eWEPKeyObject
{
	eWEP1,
	eWEP2,
	eWEP3,
	eWEP4
};
struct CWMP_NODE tWEPKeyObject[] =
{
/*info,  			leaf,			next)*/
{&tWEPKeyObjectInfo[eWEP1],	tWEPKeyEntityLeaf,	NULL},
{&tWEPKeyObjectInfo[eWEP2],	tWEPKeyEntityLeaf,	NULL},
{&tWEPKeyObjectInfo[eWEP3],	tWEPKeyEntityLeaf,	NULL},
{&tWEPKeyObjectInfo[eWEP4],	tWEPKeyEntityLeaf,	NULL},
{NULL,				NULL,			NULL}
};

/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.AssociatedDevice.{i}.
*******************************************************************************/
struct CWMP_OP tAscDeviceEntityLeafOP = { getAscDeviceEntity,NULL };
struct CWMP_PRMT tAscDeviceEntityLeafInfo[] =
{
/*(name,				type,		flag,				op)*/
{"AssociatedDeviceMACAddress",		eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"AssociatedDeviceIPAddress",		eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"AssociatedDeviceAuthenticationState",	eCWMP_tBOOLEAN,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"LastRequestedUnicastCipher",		eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"LastRequestedMulticastCipher",	eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"LastPMKId",				eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP},
{"LastDataTransmitRate",	eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tAscDeviceEntityLeafOP}
};
enum eAscDeviceEntityLeaf
{
	eAssociatedDeviceMACAddress,
	eAssociatedDeviceIPAddress,
	eAssociatedDeviceAuthenticationState,
	eLastRequestedUnicastCipher,
	eLastRequestedMulticastCipher,
	eLastPMKId,
	eLastDataTransmitRate
};
struct CWMP_LEAF tAscDeviceEntityLeaf[] =
{
{ &tAscDeviceEntityLeafInfo[eAssociatedDeviceMACAddress] },
{ &tAscDeviceEntityLeafInfo[eAssociatedDeviceIPAddress] },
{ &tAscDeviceEntityLeafInfo[eAssociatedDeviceAuthenticationState] },
{ &tAscDeviceEntityLeafInfo[eLastRequestedUnicastCipher] },
{ &tAscDeviceEntityLeafInfo[eLastRequestedMulticastCipher] },
{ &tAscDeviceEntityLeafInfo[eLastPMKId] },
{ &tAscDeviceEntityLeafInfo[eLastDataTransmitRate] },
{ NULL }
};


struct CWMP_PRMT tAscDeviceObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_LNKLIST,	NULL}
};
enum eAscDeviceObject
{
	eAscDevice0
};
struct CWMP_LINKNODE tAscDeviceObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tAscDeviceObjectInfo[eAscDevice0],	tAscDeviceEntityLeaf,	NULL,		NULL,			0}
};


/*******************************************************************************
InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}.
*******************************************************************************/
struct CWMP_OP tWLANConfEntityLeafOP = { getWLANConf, setWLANConf };
struct CWMP_PRMT tWLANConfEntityLeafInfo[] =
{
/*(name,				type,		flag,			op)*/
{"Enable",				eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"Status",				eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"Name",				eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"BSSID",				eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"MaxBitRate",				eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"Channel",				eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"AutoChannelEnable",	eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"SSID",				eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"BeaconType",				eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
#ifdef MAC_FILTER
{"MACAddressControlEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
#endif /*MAC_FILTER*/
#ifdef _PRMT_X_CT_COM_WLAN_
{"Standard",					eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,		&tWLANConfEntityLeafOP},
#else
{"Standard",				eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
#endif
{"WEPKeyIndex",				eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"KeyPassphrase",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
#ifdef _PRMT_X_CT_COM_WLAN_	//cathy
{"WEPEncryptionLevel",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,		&tWLANConfEntityLeafOP},
#else
{"WEPEncryptionLevel",			eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
#endif
{"BasicEncryptionModes",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"BasicAuthenticationMode",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"WPAEncryptionModes",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"WPAAuthenticationMode",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"IEEE11iEncryptionModes",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"IEEE11iAuthenticationMode",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"PossibleChannels",			eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"BasicDataTransmitRates",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"OperationalDataTransmitRates",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"PossibleDataTransmitRates",		eCWMP_tSTRING,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"InsecureOOBAccessEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"BeaconAdvertisementEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"SSIDAdvertisementEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP}, /*version 1.4*/
{"RadioEnabled",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"TransmitPowerSupported", 		eCWMP_tSTRING,  CWMP_READ,	&tWLANConfEntityLeafOP},
{"TransmitPower",				eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"AutoRateFallBackEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"LocationDescription",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"RegulatoryDomain", 			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"TotalPSKFailures",			eCWMP_tUINT,	CWMP_READ,	&tWLANConfEntityLeafOP},
{"TotalIntegrityFailures",		eCWMP_tUINT,	CWMP_READ,	&tWLANConfEntityLeafOP},
{"ChannelsInUse",			eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP},
{"DeviceOperationMode",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"DistanceFromRoot", 			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"PeerBSSID", 					eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"AuthenticationServiceMode",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"WMMSupported",				eCWMP_tBOOLEAN,	CWMP_READ, &tWLANConfEntityLeafOP},
{"UAPSDSupported",				eCWMP_tBOOLEAN,	CWMP_READ, &tWLANConfEntityLeafOP},
{"WMMEnable",					eCWMP_tBOOLEAN, CWMP_WRITE|CWMP_READ, &tWLANConfEntityLeafOP},
{"UAPSDEnable",					eCWMP_tBOOLEAN, CWMP_WRITE|CWMP_READ, &tWLANConfEntityLeafOP},
{"TotalBytesSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP},
{"TotalBytesReceived",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP},
{"TotalPacketsSent",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP},
{"TotalPacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP},
#ifdef _PRMT_X_CT_COM_WLAN_
{"X_CT-COM_SSIDHide",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_RFBand", 			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_ChannelWidth",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_GuardInterval",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_RetryTimeout",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_Powerlevel",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
{"X_CT-COM_PowerValue",			eCWMP_tUINT,	CWMP_READ,		&tWLANConfEntityLeafOP},
{"X_CT-COM_APModuleEnable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,		&tWLANConfEntityLeafOP},
{"X_CT-COM_WPSKeyWord", 		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tWLANConfEntityLeafOP},
#endif //_PRMT_X_CT_COM_WLAN_
{"TotalAssociations",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tWLANConfEntityLeafOP}
};
enum eWLANConfEntityLeafInfo
{
	eWL_Enable,
	eWL_Status,
	eWL_Name,
	eWL_BSSID,
	eWL_MaxBitRate,
	eWL_Channel,
	eWL_AutoChannelEnable,
	eWL_SSID,
	eWL_BeaconType,
#ifdef MAC_FILTER
	eWL_MACAddressControlEnabled,
#endif
	eWL_Standard,
	eWL_WEPKeyIndex,
	eWL_KeyPassphrase,
	eWL_WEPEncryptionLevel,
	eWL_BasicEncryptionModes,
	eWL_BasicAuthenticationMode,
	eWL_WPAEncryptionModes,
	eWL_WPAAuthenticationMode,
	eWL_IEEE11iEncryptionModes,
	eWL_IEEE11iAuthenticationMode,
	eWL_PossibleChannels,
	eWL_BasicDataTransmitRates,
	eWL_OperationalDataTransmitRates,
	eWL_PossibleDataTransmitRates,
	eWL_InsecureOOBAccessEnabled,
	eWL_BeaconAdvertisementEnabled,
	eWL_SSIDAdvertisementEnabled,
	eWL_RadioEnabled,
	eWL_TransmitPowerSupported,
	eWL_TransmitPower,
	eWL_AutoRateFallBackEnabled,
	eWL_LocationDescription,
	eWL_RegulatoryDomain,
	eWL_TotalPSKFailures,
	eWL_TotalIntegrityFailures,
	eWL_ChannelsInUse,
	eWL_DeviceOperationMode,
	eWL_DistanceFromRoot,
	eWL_PeerBSSID,
	eWL_AuthenticationServiceMode,
	eWL_WMMSupported,
	eWL_UAPSDSupported,
	eWL_WMMEnable,
	eWL_UAPSDEnable,
	eWL_TotalBytesSent,
	eWL_TotalBytesReceived,
	eWL_TotalPacketsSent,
	eWL_TotalPacketsReceived,
#ifdef _PRMT_X_CT_COM_WLAN_
	eWL_X_CTCOM_SSIDHide,
	eWL_X_CTCOM_RFBand,
	eWL_X_CTCOM_ChannelWidth,
	eWL_X_CTCOM_GuardInterval,
	eWL_X_CTCOM_RetryTimeout,
	eWL_X_CTCOM_Powerlevel,
	eWL_X_CTCOM_PowerValue,
	eWL_X_CTCOM_APModuleEnable,
	eWL_X_CTCOM_WPSKeyWord,
#endif //_PRMT_X_CT_COM_WLAN_
	eWL_TotalAssociations	
};
struct CWMP_LEAF tWLANConfEntityLeaf[] =
{
{ &tWLANConfEntityLeafInfo[eWL_Enable] },
{ &tWLANConfEntityLeafInfo[eWL_Status] },
{ &tWLANConfEntityLeafInfo[eWL_Name] },
{ &tWLANConfEntityLeafInfo[eWL_BSSID] },
{ &tWLANConfEntityLeafInfo[eWL_MaxBitRate] },
{ &tWLANConfEntityLeafInfo[eWL_Channel] },
{ &tWLANConfEntityLeafInfo[eWL_AutoChannelEnable] },
{ &tWLANConfEntityLeafInfo[eWL_SSID] },
{ &tWLANConfEntityLeafInfo[eWL_BeaconType] },
#ifdef MAC_FILTER
{ &tWLANConfEntityLeafInfo[eWL_MACAddressControlEnabled] },
#endif
{ &tWLANConfEntityLeafInfo[eWL_Standard] },
{ &tWLANConfEntityLeafInfo[eWL_WEPKeyIndex] },
{ &tWLANConfEntityLeafInfo[eWL_KeyPassphrase] },
{ &tWLANConfEntityLeafInfo[eWL_WEPEncryptionLevel] },
{ &tWLANConfEntityLeafInfo[eWL_BasicEncryptionModes] },
{ &tWLANConfEntityLeafInfo[eWL_BasicAuthenticationMode] },
{ &tWLANConfEntityLeafInfo[eWL_WPAEncryptionModes] },
{ &tWLANConfEntityLeafInfo[eWL_WPAAuthenticationMode] },
{ &tWLANConfEntityLeafInfo[eWL_IEEE11iEncryptionModes] },
{ &tWLANConfEntityLeafInfo[eWL_IEEE11iAuthenticationMode] },
{ &tWLANConfEntityLeafInfo[eWL_PossibleChannels] },
{ &tWLANConfEntityLeafInfo[eWL_BasicDataTransmitRates] },
{ &tWLANConfEntityLeafInfo[eWL_OperationalDataTransmitRates] },
{ &tWLANConfEntityLeafInfo[eWL_PossibleDataTransmitRates] },
{ &tWLANConfEntityLeafInfo[eWL_InsecureOOBAccessEnabled] },
{ &tWLANConfEntityLeafInfo[eWL_BeaconAdvertisementEnabled] },
{ &tWLANConfEntityLeafInfo[eWL_SSIDAdvertisementEnabled] },
{ &tWLANConfEntityLeafInfo[eWL_RadioEnabled] },
{ &tWLANConfEntityLeafInfo[eWL_TransmitPowerSupported] },
{ &tWLANConfEntityLeafInfo[eWL_TransmitPower] },
{ &tWLANConfEntityLeafInfo[eWL_AutoRateFallBackEnabled] },
{ &tWLANConfEntityLeafInfo[eWL_LocationDescription] },
{ &tWLANConfEntityLeafInfo[eWL_RegulatoryDomain] },
{ &tWLANConfEntityLeafInfo[eWL_TotalPSKFailures] },
{ &tWLANConfEntityLeafInfo[eWL_TotalIntegrityFailures] },
{ &tWLANConfEntityLeafInfo[eWL_ChannelsInUse] },
{ &tWLANConfEntityLeafInfo[eWL_DeviceOperationMode] },
{ &tWLANConfEntityLeafInfo[eWL_DistanceFromRoot] },
{ &tWLANConfEntityLeafInfo[eWL_PeerBSSID] },
{ &tWLANConfEntityLeafInfo[eWL_AuthenticationServiceMode] },
{ &tWLANConfEntityLeafInfo[eWL_WMMSupported] },
{ &tWLANConfEntityLeafInfo[eWL_UAPSDSupported] },
{ &tWLANConfEntityLeafInfo[eWL_WMMEnable] },
{ &tWLANConfEntityLeafInfo[eWL_UAPSDEnable] },
{ &tWLANConfEntityLeafInfo[eWL_TotalBytesSent] },
{ &tWLANConfEntityLeafInfo[eWL_TotalBytesReceived] },
{ &tWLANConfEntityLeafInfo[eWL_TotalPacketsSent] },
{ &tWLANConfEntityLeafInfo[eWL_TotalPacketsReceived] },
#ifdef _PRMT_X_CT_COM_WLAN_

{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_SSIDHide] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_RFBand] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_ChannelWidth] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_GuardInterval] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_RetryTimeout] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_Powerlevel] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_PowerValue] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_APModuleEnable] },
{ &tWLANConfEntityLeafInfo[eWL_X_CTCOM_WPSKeyWord] },

#endif //_PRMT_X_CT_COM_WLAN_
{ &tWLANConfEntityLeafInfo[eWL_TotalAssociations] },	
{ NULL }
};

struct CWMP_OP tWLAN_AssociatedDevice_OP = { NULL, objAscDevice };
struct CWMP_PRMT tWLANConfEntityObjectInfo[] =
{
/*(name,				type,		flag,			op)*/
{"Stats",					eCWMP_tOBJECT,	CWMP_READ,		NULL},
#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
{"WPS",						eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
{"AssociatedDevice",			eCWMP_tOBJECT,	CWMP_READ,		&tWLAN_AssociatedDevice_OP},
{"WEPKey",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"PreSharedKey",			eCWMP_tOBJECT,	CWMP_READ,		NULL}
};
enum eWLANConfEntityObject
{
	eWLAN_Stats,
#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
	eWLAN_WPS,
#endif
	eWLAN_AssociatedDevice,
	eWLAN_WEPKey,
	eWLAN_PreSharedKey
};
struct CWMP_NODE tWLANConfEntityObject[] =
{
/*info,  						leaf,	next)*/
{&tWLANConfEntityObjectInfo[eWLAN_Stats], 				tStatsEntityLeaf, 	NULL},  
#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
{&tWLANConfEntityObjectInfo[eWLAN_WPS],					tWPSEntityLeaf,		tWPSObject}, 
#endif
{&tWLANConfEntityObjectInfo[eWLAN_AssociatedDevice],	NULL,	NULL},
{&tWLANConfEntityObjectInfo[eWLAN_WEPKey],		NULL,	tWEPKeyObject},
{&tWLANConfEntityObjectInfo[eWLAN_PreSharedKey],	NULL,	tPreSharedKeyObject},
{NULL,							NULL,	NULL}
};



struct CWMP_PRMT tWLANConfigObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
#if defined(FOR_DUAL_BAND)
{"1",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"2",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#ifdef WLAN_MBSSID
{"3",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"4",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"5",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"6",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"7",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"8",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"9",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"10",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"11",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"12",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
#else
{"1",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#ifdef WLAN_MBSSID
{"2",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"3",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"4",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"5",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
{"6",				eCWMP_tOBJECT,	CWMP_READ,		NULL},
#endif
#endif
};
enum eWLANConfigObject
{
#if defined(FOR_DUAL_BAND)
	eWLAN1,
	eWLAN2,
#ifdef WLAN_MBSSID	
	eWLAN3,
	eWLAN4,
	eWLAN5,
	eWLAN6,
	eWLAN7,
	eWLAN8,
	eWLAN9,
	eWLAN10,
	eWLAN11,
	eWLAN12,	
#endif
#else
	eWLAN1,
#ifdef WLAN_MBSSID
	eWLAN2,
	eWLAN3,
	eWLAN4,
	eWLAN5,
	eWLAN6,	
#endif
#endif
};
struct CWMP_NODE tWLANConfigObject[] =
{
/*info,  			leaf,			node)*/
#if defined(FOR_DUAL_BAND)
{ &tWLANConfigObjectInfo[eWLAN1],tWLANConfEntityLeaf,	tWLANConfEntityObject},
{ &tWLANConfigObjectInfo[eWLAN2],tWLANConfEntityLeaf,	tWLANConfEntityObject},
#ifdef WLAN_MBSSID
{ &tWLANConfigObjectInfo[eWLAN3],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN4],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN5],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN6],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN7],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN8],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN9],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN10],tWLANConfEntityLeaf,	tWLANConfEntityObject},
{ &tWLANConfigObjectInfo[eWLAN11],tWLANConfEntityLeaf,	tWLANConfEntityObject},
{ &tWLANConfigObjectInfo[eWLAN12],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
#endif
#else
{ &tWLANConfigObjectInfo[eWLAN1],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
#ifdef WLAN_MBSSID
{ &tWLANConfigObjectInfo[eWLAN2],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN3],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN4],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN5],tWLANConfEntityLeaf,	tWLANConfEntityObject},	
{ &tWLANConfigObjectInfo[eWLAN6],tWLANConfEntityLeaf,	tWLANConfEntityObject},
#endif
#endif
{ NULL,				NULL,			NULL}
};

#if 1//def CTCOM_WLAN_REQ
struct CWMP_PRMT tWLANObjectInfo[] =
{
/*(name,				type,		flag,			op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eWLANObject
{
	eWLAN0
};
struct CWMP_LINKNODE tWLANObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tWLANObjectInfo[eWLAN0],	tWLANConfEntityLeaf,	tWLANConfEntityObject,		NULL,			0}
};
#endif

int getPreSharedKeyEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char		*lastname = entity->info->name;
	int		id=0,chain_id=0, wlaninst=0;
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	unsigned char	buf[128]="";
	int rootIdx=0,vwlanIdx=0;
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	wlaninst = getWLANConfInstNum( name );

	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	id = getPreSharedKeyInstNum( name );
	if( (id<1) || (id>10) ) return ERR_9007;


	*type = entity->info->type;
	*data = NULL;
	

	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	
	if( strcmp( lastname, "PreSharedKey" )==0 )
	{
		if( id==1 )
		{
			unsigned int pskfmt;//0:Passphrase,   1:hex
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_PSK_FORMAT, (void *)&pskfmt);
			if(pskfmt==1)
			{
				getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
				//*data = strdup( buf );
				*data = strdup( "" );
			}else
				*data = strdup( "" );
		}
		return 0;
	}
	else if( strcmp( lastname, "KeyPassphrase" )==0 )
	{
		if( id==1 )
		{
			unsigned int pskfmt;//0:Passphrase,   1:hex
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_PSK_FORMAT, (void *)&pskfmt);
			if(pskfmt==0)
	{
				getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
				//*data = strdup( buf );
				*data = strdup( "" );
			}else
				*data = strdup( "" );
		}
		return 0;
	}
	else if( strcmp( lastname, "AssociatedDeviceMACAddress" )==0 )
	{
		*data = strdup( "" );
	}else{
		return ERR_9005;
	}
	return 0;
}

int setPreSharedKeyEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char		*lastname = entity->info->name;
	int	id=0, chain_id=0, wlaninst=0;
	char		*tok;
	char		*buf=data;
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	int rootIdx=0,vwlanIdx=0;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	id = getPreSharedKeyInstNum( name );
	if( (id<1) || (id>10) ) return ERR_9007;

	
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	
	if( strcmp( lastname, "PreSharedKey" )==0 )
	{
		int i,len;
		if( buf==NULL ) return ERR_9007;
		
		len = strlen(buf);	
		if( len!=64 ) // hex len  must be 64
			return ERR_9007;
	
		for( i=0; i<len; i++ )
			if( _is_hex(buf[i])==0 ) return ERR_9007;
		
		if( id==1 ) //also update MIB_WLAN_WPA_PSK
		{
			unsigned char pskfmt;
			
			pskfmt = 1;//0:Passphrase,   1:hex
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_PSK_FORMAT, (void *)&pskfmt);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
			
#ifdef _CWMP_APPLY_
			apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
			return 0;
#else
			return 1;
#endif
		}

		return 0;
	}
	else if( strcmp( lastname, "KeyPassphrase" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( (strlen(buf)<8) || (strlen(buf)>63) ) return ERR_9007;

		
		if( id==1 ) //also update MIB_WLAN_WPA_PSK
		{
			unsigned char pskfmt;
			pskfmt = 0; //0:Passphrase,   1:hex
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_PSK_FORMAT, (void *)&pskfmt);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
#ifdef _CWMP_APPLY_
			apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
			return 1;
#endif
		}
		
		return 0;
	}
	else if( strcmp( lastname, "AssociatedDeviceMACAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)!=0 ) return ERR_9001;
	}else{
		return ERR_9005;
	}
	
	return 0;
}

#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
int getWPSEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;	
	unsigned char buf[256]="";
	unsigned int vChar=0;
	unsigned int vUint=0;
	unsigned int wlaninst=0;
	int rootIdx=0,vwlanIdx=0;	
	unsigned int GetValue=0;
	unsigned short uShort;
	unsigned int wpa_cipher=0, wpa2_cipher=0;
	char wlan_ifname[10]={0};	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	int dual_wps_icons = 0;
	char wlan_ssid[100], device_name[100], wsc_pin[100];
	char filename[32], line[100], string[64];
	FILE *fp=NULL;
	unsigned char tmp[256] = "";
	char tmp2[100];
	int wlan0_mode=0, wsc_method;
	 
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
			return ERR_9007;

	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else if(vwlanIdx<NUM_VWLAN+1)
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);
	else
		sprintf(wlan_ifname,"wlan%d-vxd",rootIdx);	
		
	*type = entity->info->type;
	*data = NULL;

	getWlanMib(rootIdx, 0, MIB_DUAL_WPS_ICONS,&dual_wps_icons);
	if( strcmp( lastname, "Enable" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_DISABLE, (void *)&vChar);
		if(vChar==1)
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
	}
	else if ( strcmp( lastname, "DeviceName" )==0 ) 
	{		
		getWlanMib(rootIdx, vwlanIdx, MIB_DEVICE_NAME, (void *)&device_name);
		if(dual_wps_icons)
		{		
			getWlanMib(rootIdx, 0, MIB_WLAN_PHY_BAND_SELECT, (void *)&vChar);
			if(vChar==PHYBAND_5G) //(!strncmp(wlan_ifname,"wlan0",5)) 
			{
				strcat(device_name,"_5G");
			}
			else if(vChar==PHYBAND_2G) //(!strncmp(wlan_ifname,"wlan1",5))
			{
				strcat(device_name,"_2.4G");
			}
		}		
		*data = strdup(device_name); 		
	}
	else if ( strcmp( lastname, "DevicePassword" )==0 ) // return 0 regardless of actual value
	{
		*data = uintdup( 0 ); 		 
	}
	else if ( strcmp( lastname, "UUID" )==0 ) 
	{	
		sprintf(filename, WSC_ROMFS_CFG_CILE);	
		//printf("filename(%s) \n",filename );
		fp=fopen( filename, "r" );
		if(fp)
		{
			fgets(line, sizeof(line), fp);			
			while(!feof(fp))
			{		
				if( strstr(line,"uuid") ) 			
				{					
					unsigned char *p;	
					int i;	
					p = line;	
					for(i=0;i<sizeof(line);++i)		
						if(*p == '=')			
							break;			
						else		
							p++;	
						p++;		
						while(*p == ' ')		
							p++;
					memcpy(tmp, p, strlen(p));						
					break;					
				}
				fgets(line, sizeof(line), fp);					
			}			
			fclose(fp);
		}
		else
			return ERR_9002;		
		
		getWlanMib(rootIdx, 0, MIB_WLAN_WLAN_MAC_ADDR, (void *)&buf);
		if (!memcmp(buf, "\x00\x00\x00\x00\x00\x00", 6)) {
			getWlanMib(rootIdx, vwlanIdx, MIB_HW_NIC0_ADDR, (void *)&buf);
		}
		
		if(dual_wps_icons)
		{
			if(!strncmp(wlan_ifname,"wlan1",5))
			{
				(*(char *)(buf+5)) += 0x10;   
			}
		}
		convert_bin_to_str((unsigned char *)buf, 6, tmp2);		
		memcpy(tmp+20, tmp2, 12);		
		*data = strdup(tmp); 		
	}
	else if ( strcmp( lastname, "Version" )==0 ) // wps 2.0 ... ???
	{
		*data = intdup(2); 	
	}
	else if ( strcmp( lastname, "ConfigMethodsSupported" )==0 )
	{
		*data = strdup("PushButton,Label,Keypad");	// Label: pin
	}
	else if ( strcmp( lastname, "ConfigMethodsEnabled" )==0 ) 
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_METHOD, (void *)&vChar); // what about p2p ???
		if(vChar==1)	// pin
			*data = strdup("Label,Keypad");
		else if(vChar==2)	// pbc
			*data = strdup("PushButton,Keypad");
		else if(vChar==3)	// pin + pbc
			*data = strdup("PushButton,Label,Keypad");
	}
	else if ( strcmp( lastname, "SetupLockedState" )==0 )
	{
		unsigned int pinReached = 0, lock = 0;		
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_AUTO_LOCK_DOWN, (void *)&lock);	// lockdown
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_LOCKDOWN_PIN_REACHED, (void *)&pinReached); // pin reach
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_ER_NUM, (void *)&vChar);	// ER number

		/*   if locked,  already established registrars will continue to be able to add new 
			enrollees, (the SetupLockedState becomes LockedByRemoteManagement).  */
		if(lock){			
			if(vChar)
				*data = strdup("LockedByRemoteManagement");	
			else if(pinReached)
				*data = strdup("PINRetryLimitReached"); 
			else
				*data = strdup("LockedByLocalManagement"); // maybe changed
		}else if( !lock )
			*data = strdup("Unlocked");	
		else
			*data = strdup("");		
	}
	else if ( strcmp( lastname, "SetupLock" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_AUTO_LOCK_DOWN, (void *)&GetValue);
		*data = booldup(GetValue);
	}
	else if ( strcmp( lastname, "ConfigurationState" )==0 )
	{
		getWlanMib( rootIdx, vwlanIdx, MIB_WLAN_WSC_CONFIGURED, (void *)&vChar);
		if( vChar==1 )
			*data = strdup("Configured");
		else if( vChar==0 )
			*data = strdup("Not configured");
	}
	else if ( strcmp( lastname, "LastConfigurationError" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_LAST_CONFIG_ERR, (void *)&vChar);
		switch(vChar)
		{
			case CONFIG_ERR_NO_ERR:
				*data = strdup("NoError");
				break;
			case CONFIG_ERR_DECRYPTION_CRC_ERR:
				*data = strdup("DecryptionCRCFailure"); //
				break;
			case CONFIG_ERR_SIGNAL_TOO_WEAK:
				*data = strdup("SignalTooWeak"); //
				break;	
			case CONFIG_ERR_CANNOT_CONNECT_TO_REG:
				*data = strdup("CouldntConnectToRegistrar");
				break;
			case CONFIG_ERR_ROGUE_ACT_SUSPECTED:
				*data = strdup("RogueActivitySuspected");//
				break;
			case CONFIG_ERR_DEV_BUSY:
				*data = strdup("DeviceBusy");
				break;
			case CONFIG_ERR_SETUP_LOCKED:
				*data = strdup("SetupLocked");
				break;
			case CONFIG_ERR_MESSAGE_TIMEOUT:
				*data = strdup("MessageTimeout");//
				break;
			case CONFIG_ERR_REG_SESSION_TIMEOUT:
				*data = strdup("RegistrationSessionTimeout");//
				break;
			case CONFIG_ERR_DEV_PASS_AUTH_FAIL:
				*data = strdup("DevicePasswordAuthFailure");
				break;
			default:
				*data = strdup("");
				break;
		}		
	}
	else if ( strcmp( lastname, "RegistrarNumberOfEntries" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_ER_NUM, (void *)&vChar);		
		*data = uintdup(vChar);
	}
	else if ( strcmp( lastname, "RegistrarEstablished" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_ER_NUM, (void *)&vChar);	
		if(vChar)
			*data = booldup(1);
		else
			*data = booldup(0);
	}
	else
	{
		return ERR_9005;
	}	
	return 0;
}

int setWPSEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int	vChar=0;
	unsigned int wlaninst=0;	
	int rootIdx=0,vwlanIdx=0;
	int NewRootIdx=0, NewvwlanIdx=0;
	int wpa_cipher=0, wpa2_cipher=0;
	unsigned int SetValue=0;
	unsigned int GetValue=0;
	unsigned short uShort;
	int len = 0;	
	int isWLANMIBUpdated=1;
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	int wlanBand2G5GSelect=0;
	int maxWlanIface=NUM_WLAN_INTERFACE; // just for root ap
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	pwlanConf = &wlanConf;
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);	
	
	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i = data;
		if( i==NULL ) return ERR_9007;
		vChar = (*i==0)?1:0;		
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_DISABLE, (void *)&vChar);
		isWLANMIBUpdated=1;
	}else if( strcmp( lastname, "DevicePassword" )==0 )
	{
		int *intvalue = data;
		if( intvalue==NULL ) return ERR_9007;
		unsigned char buftmp[9] = {0};	
		sprintf(buftmp, "%d", *intvalue);		
		
		int i;
		if( strlen(buftmp)!=8 ){
			printf("===> len not 8, len(%d), buf(%s)\n", strlen(buftmp), buftmp);
			return ERR_9007;
		}
		
		for ( i=0;i<strlen(buftmp);++i){			
			if( !(buftmp[i]>='0' && buftmp[i]<='9') ){			
				printf("===> not dec\n");
				return ERR_9007;
			}
		}
		apmib_set(MIB_HW_WSC_PIN, (void *)buftmp);    // HW mib cannot be set via apmib_set		
		apmib_update(HW_SETTING);   				  // .......
		isWLANMIBUpdated = 1;			
	}
	else if( strcmp( lastname, "ConfigMethodsEnabled" )==0 )    	// data model type is bool, but spec is string ...
	{
		printf("==== [%s-%d]buf(%s)\n",__FUNCTION__,__LINE__,buf);
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
	#if 0	
		if( setWPSCfgMethod( buf, &uShort )<0 ) return ERR_9007;

		SetValue=uShort;	
		printf("=== set cfg method (%d)\n",SetValue);
		setWlanMib(rootIdx, 0, MIB_WLAN_WSC_METHOD, (void *)&SetValue);  
		isWLANMIBUpdated = 1;	
	#endif	
	}	
	else if( strcmp( lastname, "SetupLock" )==0 )
	{
		int *i = data;
		if( i==NULL ) return ERR_9007;
		
		SetValue = (*i==1)?1:0;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_AUTO_LOCK_DOWN,(void *)&SetValue);
#ifdef FOR_DUAL_BAND
		if(rootIdx==0){
			setWlanMib(1, vwlanIdx, MIB_WLAN_WSC_AUTO_LOCK_DOWN,(void *)&SetValue);
		}
		else if(rootIdx==1){
			setWlanMib(0, vwlanIdx, MIB_WLAN_WSC_AUTO_LOCK_DOWN,(void *)&SetValue);
		}
#endif
		/* always not pin reach because of reinit/reboot */
		SetValue = 0;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_LOCKDOWN_PIN_REACHED,(void *)&SetValue); 
#ifdef FOR_DUAL_BAND
		if(rootIdx==1)
			setWlanMib(0, vwlanIdx, MIB_WLAN_WSC_LOCKDOWN_PIN_REACHED,(void *)&SetValue);
		else
			setWlanMib(1, vwlanIdx, MIB_WLAN_WSC_LOCKDOWN_PIN_REACHED,(void *)&SetValue);
#endif
		isWLANMIBUpdated = 1;		
	}
	else {
		return ERR_9005;
	}

#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
	return 0;
#else
		if(isWLANMIBUpdated)
			return 1;
		else
			return 0;
#endif
}
#endif

int getStatsEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	unsigned long es=0,er=0, ups=0,upr=0, dps=0,dpr=0, mps=0,mpr=0, bps=0, bpr=0, uppr=0;
	char	*lastname = entity->info->name;
	unsigned int wlaninst;
	char wlan_ifname[10];
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	int rootIdx=0,vwlanIdx=0;			
	wlan_stats stats_info;

	if ((name == NULL) || (entity == NULL) || (type == NULL) || (data == NULL))
		return -1;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);

	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else if(vwlanIdx<NUM_VWLAN+1)
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);
	else
		sprintf(wlan_ifname,"wlan%d-vxd",rootIdx);	
		
	*type = entity->info->type;
	*data = NULL;	
	if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
		return -1;
	if (strcmp(lastname, "ErrorsSent") == 0) {		
		
		*data = uintdup( stats_info.errorsSent );
		
	} else if (strcmp(lastname, "ErrorsReceived") == 0) {	
	
		*data = uintdup( stats_info.errorsReceived );	
		
	} else if (strcmp(lastname, "UnicastPacketsSent") == 0) {	
	
		*data = uintdup( stats_info.unicastPacketsSent );
		
	} else if (strcmp(lastname, "UnicastPacketsReceived") == 0) {	
	
		*data = uintdup( stats_info.unicastPacketsReceived );
		
	} else if (strcmp(lastname, "DiscardPacketsSent") == 0) {	
	
		*data = uintdup( stats_info.discardPacketsSent );
		
	} else if (strcmp(lastname, "DiscardPacketsReceived") == 0) {	
	
		*data = uintdup( stats_info.discardPacketsReceived );
		
	} else if (strcmp(lastname, "MulticastPacketsSent") == 0) {		
	
		*data = uintdup( stats_info.multicastPacketsSent );
		
	} else if (strcmp(lastname, "MulticastPacketsReceived") == 0) {	
	
		*data = uintdup( stats_info.multicastPacketsReceived );
		
	} else if (strcmp(lastname, "BroadcastPacketsSent") == 0) {	
	
		*data = uintdup( stats_info.broadcastPacketsSent );
		
	} else if (strcmp(lastname, "BroadcastPacketsReceived") == 0) {	
	
		*data = uintdup( stats_info.broadcastPacketsReceived );	
		
	} else if (strcmp(lastname, "UnknownProtoPacketsReceived") == 0) {
	
		*data = uintdup( stats_info.unknownProtoPacketsReceived );
	}	
	else {
		return ERR_9005;
	}	
	return 0;
}

int getWEPKeyEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	int	keyid=0;
	char	*tok;
	unsigned int wlaninst=0,wepinst=0;;
	unsigned char	hex_key[32], ascii_key[32];
	unsigned int keyfmt = WEP64; 	//0:disable, 1:64, 2:128
	int rootIdx=0,vwlanIdx=0;
	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	wepinst = getWEPInstNum( name );
	if( wepinst<1 || wepinst>4 )	return ERR_9007;


	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "WEPKey" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&keyfmt);
		keyid = wepinst;
		switch( keyid )
		{
			case 1:
				if(keyfmt == WEP64)
				{
					getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY1, (void *)ascii_key);
				}
				else
				{
					getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY1, (void *)ascii_key);
				}
				break;
			case 2:
				if(keyfmt == WEP64)
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep64Key2, sizeof(EntryMbssid.wep64Key2));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY2, (void *)ascii_key);
				}
				else
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep128Key2, sizeof(EntryMbssid.wep128Key2));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID

						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY2, (void *)ascii_key);
				}
				break;
		case 3:
				if(keyfmt == WEP64)
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep64Key3, sizeof(EntryMbssid.wep64Key3));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY3, (void *)ascii_key);
				}
				else
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep128Key3, sizeof(EntryMbssid.wep128Key3));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY3, (void *)ascii_key);
				}
				break;
			case 4:
				if(keyfmt == WEP64)
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep64Key4, sizeof(EntryMbssid.wep64Key4));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY4, (void *)ascii_key);
				}
				else
				{
#ifdef WLAN_MBSSID
					if( wlaninst!=1 )
						// Magician: No support multi-wep-keys for MBSSID, return 0.
						//memcpy(ascii_key, EntryMbssid.wep128Key4, sizeof(EntryMbssid.wep128Key4));
						bzero(ascii_key, sizeof(ascii_key));
					else
#endif //WLAN_MBSSID
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY4, (void *)ascii_key);
				}
				break;
			default:
				return ERR_9005;
		}
		if( keyfmt!=WEP128 )
			sprintf( hex_key, "%02x%02x%02x%02x%02x",
				ascii_key[0], ascii_key[1], ascii_key[2], ascii_key[3], ascii_key[4] );
		else
			sprintf( hex_key, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
				ascii_key[0], ascii_key[1], ascii_key[2], ascii_key[3], ascii_key[4],
				ascii_key[5], ascii_key[6], ascii_key[7], ascii_key[8], ascii_key[9],
				ascii_key[10], ascii_key[11], ascii_key[12] );

		//*data = strdup( hex_key );
		*data = strdup( "" );
	}else{
		return ERR_9005;
	}
	return 0;
}

int setWEPKeyEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int wlaninst=0,wepinst=0;
	int rootIdx=0,vwlanIdx=0;
	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	wepinst = getWEPInstNum( name );
	if( wepinst<1 || wepinst>4 )	return ERR_9007;

	
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);

	if( strcmp( lastname, "WEPKey" )==0 )
	{
		unsigned char ascii_key[32];
		int len=0, keyid;
		unsigned int keyfmt,key_type;

		if( buf==NULL ) return ERR_9007;
		len = strlen(buf);
		if( (len!=10) && (len!=26) ) return ERR_9007;
		memset( ascii_key, 0, sizeof(ascii_key) );
		if(!string_to_hex(buf, ascii_key, len)) return ERR_9007;

		keyfmt = (len==10)?WEP64:WEP128; //key format==>0:disable, 1:64, 2:128
		key_type = KEY_HEX; //key type==>KEY_ASCII:ascii, KEY_HEX:hex, tr-069 always uses the hex format.
		
		
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&keyfmt);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP_KEY_TYPE, (void *)&key_type);
		keyid = wepinst;
		printf("\n~~ascii_key=%s,keyfmt=%d,keyid=%d\n\n", ascii_key, keyfmt, keyid);
				switch( keyid )
		{			
					case 1:
					if(keyfmt==WEP64)
					{
						setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY1, (void *)ascii_key);
		}
		else
		{
						setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY1, (void *)ascii_key);
		}
					break;
					case 2:
					if(keyfmt==WEP64)
					{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep64Key2, ascii_key, sizeof(EntryMbssid.wep64Key2) );
							return ERR_9000;
						else
		#endif
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY2, (void *)ascii_key);
					}
		else
		{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep128Key2, ascii_key, sizeof(EntryMbssid.wep128Key2) );
							return ERR_9000;
						else
		#endif
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY2, (void *)ascii_key);
		}
					break;
					case 3:
					if(keyfmt==WEP64)
		{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep64Key3, ascii_key, sizeof(EntryMbssid.wep64Key3) );
							return ERR_9000;
						else
				#endif
						setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY3, (void *)ascii_key);
			
					}
					else
			{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep128Key3, ascii_key, sizeof(EntryMbssid.wep128Key3) );
							return ERR_9000;
						else
						#endif
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY3, (void *)ascii_key);
					}
							break;
					case 4:
					if(keyfmt==WEP64)
					{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep64Key4, ascii_key, sizeof(EntryMbssid.wep64Key4) );
							return ERR_9000;
						else
						#endif
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY4, (void *)ascii_key);
			}
			else
			{
				#ifdef WLAN_MBSSID
						if(wlaninst != 1)
							// Magician: No support multi-wep-keys for MBSSID, return ERR_9000.
							//memcpy( EntryMbssid.wep128Key4, ascii_key, sizeof(EntryMbssid.wep128Key4) );
							return ERR_9000;
						else
						#endif
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY4, (void *)ascii_key);
					}
							break;			
					default:
							return ERR_9005;		
				}
				
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

//changes in following table should be synced to VHT_MCS_DATA_RATE[] in 8812_vht_gen.c
const unsigned short VHT_MCS_DATA_RATE[3][2][30] = 
	{	{	{ 13, 26, 39, 52, 78, 104, 117, 130, 156, 156,
			  26, 52, 78, 104, 156, 208, 234, 260, 312, 312,
			  39, 78, 117, 156, 234, 312, 351, 390, 468, 520 },			// Long GI, 20MHz
			 
			{14, 29, 43, 58, 87, 116, 130, 144, 173, 173,
			  29, 58, 87, 116, 173, 231, 260, 289, 347, 347,
			  43, 86, 130, 173, 260, 347, 390, 433, 520, 578 }	},		// Short GI, 20MHz
			
		{	{27, 54, 81, 108, 162, 216, 243, 270, 324, 360, 
			 54, 108, 162, 216, 324, 432, 486, 540, 648, 720,
			 81, 162, 243, 342, 486, 648, 729, 810, 972, 1080}, 		// Long GI, 40MHz
			 
			{30, 60, 90, 120, 180, 240, 270, 300,360, 400, 
			 60, 120, 180, 240, 360, 480, 540, 600, 720, 800,
			 90, 180, 270, 360, 540, 720, 810, 900, 1080, 1200} },		// Short GI, 40MHz
			 
		{	{59, 117,  176, 234, 351, 468, 527, 585, 702, 780,
			 117, 234, 351, 468, 702, 936, 1053, 1170, 1404, 1560,
			  176, 351, 527, 702, 1053, 1408, 1408, 1745, 2106, 2340 }, // Long GI, 80MHz
			
			{65, 130, 195, 260, 390, 520, 585, 650, 780, 867, 
			 130, 260, 390, 520, 780, 1040, 1170, 1300, 1560,1733,
			 195, 390, 585, 780, 1170, 1560, 1560, 1950, 2340, 2600	} }	// Short GI, 80MHz
	};

//changes in following table should be synced to MCS_DATA_RATEStr[] in 8190n_proc.c
WLAN_RATE_T rate_11n_table_20M_LONG[]={
	{MCS0, 	"6.5"},
	{MCS1, 	"13"},
	{MCS2, 	"19.5"},
	{MCS3, 	"26"},
	{MCS4, 	"39"},
	{MCS5, 	"52"},
	{MCS6, 	"58.5"},
	{MCS7, 	"65"},
	{MCS8, 	"13"},
	{MCS9, 	"26"},
	{MCS10, 	"39"},
	{MCS11, 	"52"},
	{MCS12, 	"78"},
	{MCS13, 	"104"},
	{MCS14, 	"117"},
	{MCS15, 	"130"},
	{MCS16, 	"19.5"},
	{MCS17, 	"39"},
	{MCS18, 	"58.5"},
	{MCS19, 	"78"},
	{MCS20, 	"117"},
	{MCS21, 	"156"},
	{MCS22, 	"175.5"},
	{MCS23, 	"195"},
	{0}
};
WLAN_RATE_T rate_11n_table_20M_SHORT[]={
	{MCS0, 	"7.2"},
	{MCS1, 	"14.4"},
	{MCS2, 	"21.7"},
	{MCS3, 	"28.9"},
	{MCS4, 	"43.3"},
	{MCS5, 	"57.8"},
	{MCS6, 	"65"},
	{MCS7, 	"72.2"},
	{MCS8, 	"14.4"},
	{MCS9, 	"28.9"},
	{MCS10, 	"43.3"},
	{MCS11, 	"57.8"},
	{MCS12, 	"86.7"},
	{MCS13, 	"115.6"},
	{MCS14, 	"130"},
	{MCS15, 	"144.5"},
	{MCS16, 	"21.7"},
	{MCS17, 	"43.3"},
	{MCS18, 	"65"},
	{MCS19, 	"86.7"},
	{MCS20, 	"130"},
	{MCS21, 	"173.3"},
	{MCS22, 	"195"},
	{MCS23, 	"216.7"},
	{0}
};
WLAN_RATE_T rate_11n_table_40M_LONG[]={
	{MCS0, 	"13.5"},
	{MCS1, 	"27"},
	{MCS2, 	"40.5"},
	{MCS3, 	"54"},
	{MCS4, 	"81"},
	{MCS5, 	"108"},
	{MCS6, 	"121.5"},
	{MCS7, 	"135"},
	{MCS8, 	"27"},
	{MCS9, 	"54"},
	{MCS10, 	"81"},
	{MCS11, 	"108"},
	{MCS12, 	"162"},
	{MCS13, 	"216"},
	{MCS14, 	"243"},
	{MCS15, 	"270"},
	{MCS16, 	"40.5"},
	{MCS17, 	"81"},
	{MCS18, 	"121.5"},
	{MCS19, 	"162"},
	{MCS20, 	"243"},
	{MCS21, 	"324"},
	{MCS22, 	"364.5"},
	{MCS23, 	"405"},
	{0}
};
WLAN_RATE_T rate_11n_table_40M_SHORT[]={
	{MCS0, 	"15"},
	{MCS1, 	"30"},
	{MCS2, 	"45"},
	{MCS3, 	"60"},
	{MCS4, 	"90"},
	{MCS5, 	"120"},
	{MCS6, 	"135"},
	{MCS7, 	"150"},
	{MCS8, 	"30"},
	{MCS9, 	"60"},
	{MCS10, 	"90"},
	{MCS11, 	"120"},
	{MCS12, 	"180"},
	{MCS13, 	"240"},
	{MCS14, 	"270"},
	{MCS15, 	"300"},
	{MCS16, 	"45"},
	{MCS17, 	"90"},
	{MCS18, 	"135"},
	{MCS19, 	"180"},
	{MCS20, 	"270"},
	{MCS21, 	"360"},
	{MCS22, 	"405"},
	{MCS23, 	"450"},
	{0}
};

void set_11ac_txrate(WLAN_STA_INFO_Tp pInfo,char* txrate)
{
	char channelWidth=0;//20M 0,40M 1,80M 2
	char shortGi=0;
	char rate_idx=pInfo->txOperaRates-0xA0; //  A0 : AC RATE
	if(!txrate)return;
/*
	TX_USE_40M_MODE		= BIT(0),
	TX_USE_SHORT_GI		= BIT(1),
	TX_USE_80M_MODE		= BIT(2)
*/
	if(pInfo->ht_info & 0x4)
		channelWidth=2;
	else if(pInfo->ht_info & 0x1)
		channelWidth=1;
	else
		channelWidth=0;
	if(pInfo->ht_info & 0x2)
		shortGi=1;

	sprintf(txrate, "%d", VHT_MCS_DATA_RATE[channelWidth][shortGi][rate_idx]>>1);
}

int tranRate(WLAN_STA_INFO_T *pInfo, unsigned char rate)
{
	char txrate[20];
	int rateid=0;
	
	if(rate >= 0xA0) { 
		//sprintf(txrate, "%d", pInfo->acTxOperaRate); 
		set_11ac_txrate(pInfo, txrate);
	} else if((rate & 0x80) != 0x80){	
		if(rate%2){
			sprintf(txrate, "%d%s",rate/2, ".5"); 
		}else{
			sprintf(txrate, "%d",rate/2); 
		}
	}else{
		if((pInfo->ht_info & 0x1)==0){ //20M
			if((pInfo->ht_info & 0x2)==0){//long
				for(rateid=0; rateid<24;rateid++){
					if(rate_11n_table_20M_LONG[rateid].id == rate){
						sprintf(txrate, "%s", rate_11n_table_20M_LONG[rateid].rate);
						break;
					}
				}
			}else if((pInfo->ht_info & 0x2)==0x2){//short
				for(rateid=0; rateid<24;rateid++){
					if(rate_11n_table_20M_SHORT[rateid].id == rate){
						sprintf(txrate, "%s", rate_11n_table_20M_SHORT[rateid].rate);
						break;
					}
				}
			}
		}else if((pInfo->ht_info & 0x1)==0x1){//40M
			if((pInfo->ht_info & 0x2)==0){//long
				
				for(rateid=0; rateid<24;rateid++){
					if(rate_11n_table_40M_LONG[rateid].id == rate){
						sprintf(txrate, "%s", rate_11n_table_40M_LONG[rateid].rate);
						break;
					}
				}
			}else if((pInfo->ht_info & 0x2)==0x2){//short
				for(rateid=0; rateid<24;rateid++){
					if(rate_11n_table_40M_SHORT[rateid].id == rate){
						sprintf(txrate, "%s", rate_11n_table_40M_SHORT[rateid].rate);
						break;
					}
				}
			}
		}
		
	}

	//tr181_printf("txrate %s", txrate);
	return atoi(txrate);
}

int getAscDeviceEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char		*lastname = entity->info->name;
	unsigned int	device_id=0;
	WLAN_STA_INFO_T info;
	char		*tok=NULL;
	unsigned int wlaninst=0;

	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;

	if( loadWLANAssInfoByInstNum(wlaninst)<0 ) return ERR_9002;
	
	device_id = getAssDevInstNum(name);
	
	if( device_id<1 || device_id>gWLANTotalClients ) return ERR_9005;
	
	if( getWLANSTAINFO( device_id-1, &info )<0 ) return ERR_9002;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "AssociatedDeviceMACAddress" )==0 )
	{
		char buf[32];
		sprintf( buf, "%02x:%02x:%02x:%02x:%02x:%02x",
				info.addr[0],info.addr[1],info.addr[2],
				info.addr[3],info.addr[4],info.addr[5] );
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "AssociatedDeviceIPAddress" )==0 )
	{
		char aip[32]="",amac[32]="";
		sprintf( amac, "%02x:%02x:%02x:%02x:%02x:%02x",
				info.addr[0],info.addr[1],info.addr[2],
				info.addr[3],info.addr[4],info.addr[5] );
		if( getIPbyMAC( amac, aip ) < 0 )
			*data = strdup( "" );
		else
			*data = strdup( aip );	
	}
	else if( strcmp( lastname, "AssociatedDeviceAuthenticationState" )==0 )
	{
		int i = ((info.flag & STA_INFO_FLAG_ASOC)==STA_INFO_FLAG_ASOC);
		*data = intdup( i );
	}
	else if( strcmp( lastname, "LastRequestedUnicastCipher" )==0 )
	{
		char ucipher[256];
		if(!getUnicastCipher( device_id, ucipher ))
			*data = strdup( ucipher );
		else
		*data = strdup( "" );
	}
	else if( strcmp( lastname, "LastRequestedMulticastCipher" )==0 )
	{
		char mcipher[256];
		if(!getMulticastCipher( device_id, mcipher ))
			*data = strdup( mcipher );
		else
		*data = strdup( "" );
	}
	else if( strcmp( lastname, "LastPMKId" )==0 )
	{
		char key[256];
		if(!getKeyInfo( device_id, key ))
			*data = strdup( key );
		else
		*data = strdup( "" );
	}
	else if( strcmp( lastname, "LastDataTransmitRate" )==0 )
	{
		char sTranRate[5];		
		sprintf(sTranRate, "%d", tranRate(&info, info.RxOperaRate) );
		*data = strdup(sTranRate);
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int getUnicastCipher(int id, char* ucipher)
{
	if( id<1 || id>gWLANTotalClients )
		return -1;
	
	if(*((int*)&gWLANKeyInfo[64*(id-1)])==0)
		return -1;

	int type = *((int*)&gWLANKeyInfo[64*(id-1)+16]);
	if(type==1||type==5)
		strcpy(ucipher, "wep");
	else if(type==2)
		strcpy(ucipher, "tkip");
	else if(type==4)
		strcpy(ucipher, "aes");
	else
		strcpy(ucipher, "none");

	return 0;
}
int getMulticastCipher(int id, char* mcipher)
{
	if( id<1 || id>gWLANTotalClients )
		return -1;
	
	if(*((int*)&gWLANKeyInfo[64*(id-1)])==0)
		return -1;

	int type = *((int*)&gWLANKeyInfo[64*(id-1)+20]);
	if(type==1||type==5)
		strcpy(mcipher, "wep");
	else if(type==2)
		strcpy(mcipher, "tkip");
	else if(type==4)
		strcpy(mcipher, "aes");
	else
		strcpy(mcipher, "none");

	return 0;

}
int getKeyInfo(int id, char* key)
{
	if( id<1 || id>gWLANTotalClients )
		return -1;
	
	if(*((int*)&gWLANKeyInfo[64*(id-1)])==0)
		return -1;

	memcpy(key, &gWLANKeyInfo[64*(id-1)+24], 32);
	key[32] = 0;
	
	return 0;
}


int objAscDevice(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);fflush(NULL);
	unsigned int wlaninst=0;
		
	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface ) return ERR_9007;

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
		
		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
		
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tINITOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );
		*c=NULL;

		loadWLANAssInfoByInstNum(wlaninst);
		if(gWLANTotalClients>0)
			return create_Object( c, tAscDeviceObject, sizeof(tAscDeviceObject), gWLANTotalClients, 1 );
		
		return 0;
		break;
	     }
	case eCWMP_tUPDATEOBJ:
	     {
	     	unsigned int num,i;
	     	struct CWMP_LINKNODE *old_table;
			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tUPDATEOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );

		loadWLANAssInfoByInstNum(wlaninst);
		num = gWLANTotalClients;
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;
	     	for( i=0; i<num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;

			remove_entity = remove_SiblingEntity( &old_table, i+1 );
			if( remove_entity!=NULL )
			{
				add_SiblingEntity( (struct CWMP_LINKNODE**)&entity->next, remove_entity );
			}else{ 
				unsigned int InstNum=i+1;
				add_Object( name, (struct CWMP_LINKNODE**)&entity->next,  tAscDeviceObject, sizeof(tAscDeviceObject), &InstNum );
			}
	     	}
	     	if( old_table )
	     		destroy_ParameterTable( (struct CWMP_NODE*)old_table );
		return 0;
	     	break;
	     }
	}

	return -1;
}

#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
int objWPSRegistrar(char *name, struct CWMP_LEAF *e, int type, void *data)
{	
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;	
	unsigned int wlaninst=0;		
	int ret=0;
	int maxWlanIface=NUM_WLAN_INTERFACE;
	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface ) 
		return ERR_9007;

	switch( type )
	{
		case eCWMP_tINITOBJ:
		{			
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;			
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) 
				return -1;
			*c=NULL;
			
			loadWLANExternalRegByInstNum(wlaninst);			
			if( gWLANTotalERNumbers>0 )
				ret = create_Object( c, tWPSRegistarObject, sizeof(tWPSRegistarObject), gWLANTotalERNumbers, 1 );
			return 0;
		}
		case eCWMP_tUPDATEOBJ:
		{			
	     	unsigned int num,i;
	     	struct CWMP_LINKNODE *old_table;
			
			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tUPDATEOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );
			loadWLANExternalRegByInstNum(wlaninst);	
			num = gWLANTotalERNumbers;			
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
			
	     	entity->next = NULL;
	     	for( i=0; i<num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;
				remove_entity = remove_SiblingEntity( &old_table, i+1 );
				if( remove_entity!=NULL )
				{
					add_SiblingEntity( (struct CWMP_LINKNODE**)&entity->next, remove_entity );					
				}else{ 
					unsigned int InstNum=i+1;					
					ret = add_Object( name, (struct CWMP_LINKNODE**)&entity->next,  tWPSRegistarObject, sizeof(tWPSRegistarObject), &InstNum );
					
				}
	     	}
	     	if( old_table )
	     		destroy_ParameterTable( (struct CWMP_NODE*)old_table );
			return 0;
	     }		
		default:
			ret = -1;
			break;			
	}

	return -1;
}
#endif

#if 1//def CTCOM_WLAN_REQ

int getCwmpWlanConById(int w_idx, int vw_idx, CWMP_WLANCONF_T *pwlanConf)
{
	unsigned int ret = 0;	
	CWMP_WLANCONF_T wlanCon_entity;
	unsigned int total,i;
	
	mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{

		*((char *)&wlanCon_entity) = (char)i;
		if( mib_get( MIB_CWMP_WLANCONF_TBL, (void*)&wlanCon_entity ) )
		{
			if(wlanCon_entity.RootIdx == w_idx &&  wlanCon_entity.VWlanIdx == vw_idx)
			{
				memcpy(pwlanConf, &wlanCon_entity, sizeof(CWMP_WLANCONF_T));
				ret = 1;
				break;
			}
		}
	}

	return ret;
}

int getCwmpWlanConMaxInstNum()
{
	unsigned int ret = 0;	
	CWMP_WLANCONF_T wlanCon_entity;
	unsigned int total,i;
	

	mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&total);
	for( i=1;i<=total;i++ )
	{

		*((char *)&wlanCon_entity) = (char)i;
		if( mib_get( MIB_CWMP_WLANCONF_TBL, (void*)&wlanCon_entity ) )
		{
			if(wlanCon_entity.InstanceNum > ret) 
				ret = wlanCon_entity.InstanceNum;
		}
	}
	return ret;
}

int objWLANConfiguration(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	int maxWLAN=NUM_WLAN_INTERFACE;
	int maxVWLAN=(NUM_VWLAN_INTERFACE*NUM_WLAN_INTERFACE);
	int i, j, instnum=0;
	int rootwlanIdx=0, vwlanIdx=0;
	int vInt=0;
	int num;
	int vWlanCount=0;
	CWMP_WLANCONF_T *pwlanConf, wlanconf_entity;
	CWMP_WLANCONF_T target[2];
	unsigned int instNum = 0;
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>name:%s(action:%d)\n", __FUNCTION__, __LINE__, name,type ) );
	
	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		unsigned int MaxInstNum = 0;
		struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

		
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tINITOBJ name:%s(MaxInstNum:%d)\n", __FUNCTION__, __LINE__, name,MaxInstNum ) );
		//mbssid:

		mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);
		pwlanConf = &wlanconf_entity;
		vWlanCount=0;
		for( i=1; i<=num;i++ )
		{
			*((char *)pwlanConf) = (char)i;
			if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
				continue;


			if(pwlanConf->IsConfigured && pwlanConf->InstanceNum)
			{
				vInt = pwlanConf->InstanceNum;
				MaxInstNum ++;
				if( create_Object( c, tWLANObject, sizeof(tWLANObject), 1, vInt ) < 0 )
					return -1;
			}
		}	


		add_objectNum( name, MaxInstNum );
		return 0;
		}
	case eCWMP_tADDOBJ:
		{
	     	int ret;
		
		int *datap = (int *)data;

	     	
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;		
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tADDOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );
#if defined(MBSSID)

		mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);
		pwlanConf = &wlanconf_entity;
		vWlanCount=0;
		for( i=1; i<=num;i++ )
		{
			*((char *)pwlanConf) = (char)i;
			if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
				continue;

			if(i < (maxWLAN+1) ) //skip root interface
				continue;
			
			if(vWlanCount > maxVWLAN)
				break;
			
			if( pwlanConf->IsConfigured==0 )
			{
				memcpy(&target[0], pwlanConf, sizeof(CWMP_WLANCONF_T));
				break;
			}
			vWlanCount++;
		}
		if( vWlanCount>maxVWLAN )
			return ERR_9004;
		
		/* depend on spec, when add / delete one dynamic object,  the added / deleted  obj can be got 
		    when the current session stops and restart a new session.
		 */
		#if 0 
		instNum = pwlanConf->InstanceNum;		
		if( pwlanConf->InstanceNum==0 ) // first time add
		ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWLANObject, sizeof(tWLANObject), data );
		else
			ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWLANObject, sizeof(tWLANObject), &instNum );
		#endif
		
		if(1)//( ret >= 0 )
		{
			if( pwlanConf->InstanceNum==0 ){
				//pwlanConf->InstanceNum = *(int*)data;
				if (get_objectNextNum(name, &instNum) < 0)
					return -1;
				pwlanConf->InstanceNum = instNum;
			}				
			pwlanConf->IsConfigured = 1;
			//getWlanMib(pwlanConf->RootIdx, pwlanConf->VWlanIdx, MIB_WLAN_PHY_BAND_SELECT, (void *)&vInt);
			//pwlanConf->RfBand =vInt;
			memcpy(&target[1], pwlanConf, sizeof(CWMP_WLANCONF_T));
			mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);
			ret=1;
		}
		return ret;
#else
		return ERR_9004;		
		
#endif
		}
	case eCWMP_tDELOBJ:
	     {
	     	int ret;	     	
		unsigned int *pUint=data;
		
		if(*pUint==1)
			return ERR_9001;		
		CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tDELOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );
#if defined(MBSSID)
		
		mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);
		pwlanConf = &wlanconf_entity;
		vWlanCount=0;
		for( i=1; i<=num;i++ )
		{
			*((char *)pwlanConf) = (char)i;
			if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
				continue;

			if(i < (maxWLAN+1) ) //skip root interface
				continue;
			
			if(vWlanCount > maxVWLAN)
				break;
			
			if( pwlanConf->InstanceNum==*pUint )
			{
				int w_idx, vw_idx;
				CWMP_WLANCONF_T wlanConf;
					
				memcpy(&target[0], pwlanConf, sizeof(CWMP_WLANCONF_T));
				//pwlanConf->InstanceNum = 0;
				pwlanConf->IsConfigured = 0;
				memcpy(&target[1], pwlanConf, sizeof(CWMP_WLANCONF_T));
				mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);

				/* disable wlan interface */
				if(getWLANIdxFromInstNum(pwlanConf->InstanceNum, &wlanConf, &w_idx, &vw_idx) == 0)
				{
					char wlan_ifname[10] = {0};
					if(vw_idx == 0)
						sprintf(wlan_ifname, "wlan%d",w_idx);
					else if(vw_idx<NUM_VWLAN+1)
						sprintf(wlan_ifname, "wlan%d-va%d",w_idx,vw_idx-1);
					else
						sprintf(wlan_ifname, "wlan%d-vxd",w_idx);


					apmib_save_wlanIdx();
					if(SetWlan_idx(wlan_ifname))
					{
						int wlan_disabled = 1;
						mib_set( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);
					}
					apmib_recov_wlanIdx();

				}
				break;
			}
			vWlanCount++;
		}
		if(vWlanCount>maxVWLAN)
			return ERR_9004;
		
		
		//ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
		ret = 0;
		if(ret==0)
		{
			ret=1;
		}
		return ret;
#else
		return ERR_9005;
#endif
		
		}
	case eCWMP_tUPDATEOBJ:
		{
			struct CWMP_LINKNODE *old_table;
			int w_idx, vw_idx;

			old_table = (struct CWMP_LINKNODE *)entity->next;
		     	entity->next = NULL;


			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("<%s:%d>eCWMP_tUPDATEOBJ name:%s\n", __FUNCTION__, __LINE__, name ) );
				
			for(w_idx=0;w_idx<NUM_WLAN_INTERFACE;w_idx++)
			{

				for(vw_idx=0;vw_idx<NUM_VWLAN_INTERFACE;vw_idx++)
				{
					char wlan_ifname[12] = {0};
					if(vw_idx == 0)
						sprintf(wlan_ifname, "wlan%d",w_idx);
					else if(vw_idx<NUM_VWLAN+1)
						sprintf(wlan_ifname, "wlan%d-va%d",w_idx,vw_idx-1);
					else
						sprintf(wlan_ifname, "wlan%d-vxd",w_idx);

					apmib_save_wlanIdx();
					if(SetWlan_idx(wlan_ifname))
					{
						int wlan_disabled = 1;
						
						mib_get( MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled);

 						memset( &wlanconf_entity, 0, sizeof( CWMP_WLANCONF_T ) );

						if( getCwmpWlanConById( w_idx, vw_idx, &wlanconf_entity ) )
						{
							
							struct CWMP_LINKNODE *remove_entity=NULL;
							memset( &target[0], 0, sizeof( CWMP_WLANCONF_T ) );
							memset( &target[1], 0, sizeof( CWMP_WLANCONF_T ) );

							remove_entity = remove_SiblingEntity( &old_table, wlanconf_entity.InstanceNum );
							memcpy(&target[0], &wlanconf_entity, sizeof(CWMP_WLANCONF_T));

							if(wlan_disabled== 0 || wlanconf_entity.IsConfigured == 1)
							{
								if( remove_entity!=NULL )
								{
									add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
								}
								else
								{							
									unsigned int MaxInstNum=getCwmpWlanConMaxInstNum();

									if(wlanconf_entity.InstanceNum == 0)
										MaxInstNum++;
									else
										MaxInstNum = wlanconf_entity.InstanceNum;

									/* set MIB wlanconf first, to avoid tr069 daemon die. */
									wlanconf_entity.IsConfigured = 1;
									wlanconf_entity.InstanceNum = MaxInstNum;
									memcpy(&target[1], &wlanconf_entity, sizeof(CWMP_WLANCONF_T));
									mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);
									add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tWLANObject, sizeof(tWLANObject), &MaxInstNum );
									//wlanconf_entity.IsConfigured = 1;
									//wlanconf_entity.InstanceNum = MaxInstNum;
									//memcpy(&target[1], &wlanconf_entity, sizeof(CWMP_WLANCONF_T));
									//mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);
								}
							}							
						}
					}

					apmib_recov_wlanIdx();
				}
			}

			if( old_table )
	     		{
	     		
	     			destroy_ParameterTable( (struct CWMP_NODE *)old_table );
	     		}


			
			return 0;
		}
	}
		
	return -1;
}
#endif
		
/*InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.* */		
int getWLANConf(char *name, struct CWMP_LEAF *entity, int *type, void **data)
		{
	char	*lastname = entity->info->name;
	unsigned long bs=0,br=0,ps=0,pr=0;
	unsigned char buf[256]="";
	unsigned int vChar=0;
	unsigned int vUint=0;
	unsigned int wlaninst=0;
	int rootIdx=0,vwlanIdx=0;
	bss_info bss;
	unsigned int GetValue=0;
	unsigned short uShort;
	unsigned int wpa_cipher=0, wpa2_cipher=0;
	char wlan_ifname[10]={0};
	wlan_stats stats_info;
	unsigned char mac[6];
	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;
					
	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )
		return ERR_9007;	
				
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);

	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else if(vwlanIdx<NUM_VWLAN+1)
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);
	else
		sprintf(wlan_ifname,"wlan%d-vxd",rootIdx);
	
	//CWMPDBG( 1, ( stderr, "<%s:%d>target:<%d,%d>\n", __FUNCTION__, __LINE__, rootIdx,vwlanIdx ) );
		
	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
		int func_off=0;
		
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WLAN_DISABLED, (void *)&vChar);
		//getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_FUNC_OFF, (void *)&func_off);

		if(vChar==1 || func_off == 1)
			*data = booldup( 0 );
		else
			*data = booldup( 1 );
	}
	else if( strcmp( lastname, "Status" )==0 )
	{
		int flags=0;
		if( getInFlags(wlan_ifname, &flags)==1 )
		{
			if (flags & IFF_UP)
				*data = strdup( "Up" );
		else
				*data = strdup( "Disabled" );
		}else
			*data = strdup( "Error" );
		}
	else if( strcmp( lastname, "Name" )==0 )
	{
		*data = strdup(wlan_ifname);
	}
	else if( strcmp( lastname, "BSSID" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WLAN_DISABLED,(void *)&vUint);
		if(vUint == 0){
			getWlanBssInfo(rootIdx, vwlanIdx, (void*)&bss);
			sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x", bss.bssid[0], bss.bssid[1], bss.bssid[2], bss.bssid[3], bss.bssid[4], bss.bssid[5]);	
		}
		else
		{
			strcpy(buf,"00:00:00:00:00:00");
		}		

		*data=strdup(buf);
	}
	else if( strcmp( lastname, "MaxBitRate" )==0 )
	{	
		unsigned short value;
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES, (void *)&value);			
		if(value == 0xfff)
			*data=strdup( "Auto" ); // all cck + ofdm rates permitted
		else if(value & TX_RATE_54M)
			*data = strdup( "54" );
		else if(value & TX_RATE_48M)
			*data = strdup( "48" );
		else if(value & TX_RATE_36M)
			*data = strdup( "36" );
		else if(value & TX_RATE_24M)
			*data = strdup( "24" );
		else if(value & TX_RATE_18M)
			*data = strdup( "18" );
		else if(value & TX_RATE_12M)
			*data = strdup( "12" );		
		else if(value & TX_RATE_9M)
			*data = strdup( "9" );
		else if(value & TX_RATE_6M)
			*data = strdup( "6" );	
		else if(value & TX_RATE_11M)
			*data = strdup( "11" );
		else if(value & TX_RATE_5M)
			*data = strdup( "5.5" );
		else if(value & TX_RATE_2M)
			*data = strdup( "2" );
		else if(value & TX_RATE_1M)
			*data = strdup( "1" );
		else			
			*data = strdup( "" );//return ERR_9002;		
	}
else if( strcmp( lastname, "Channel" )==0 )
		{

		
		getWlanMib(rootIdx, 0, MIB_WLAN_CHANNEL, (void *)&vChar);

		if (vChar == 0) // AutoChannelEnable == 1
		{
			char WlanIf[32];

			sprintf(WlanIf, "wlan%d", rootIdx);
			if (getWlBssInfo(WlanIf, &bss) < 0)
			{
				return -1;
			}

			vChar = bss.channel;
		}
		
		*data = uintdup( (unsigned int)vChar );
	}
	else if( strcmp( lastname, "AutoChannelEnable" )==0 )
	{
		getWlanMib(rootIdx, 0, MIB_WLAN_CHANNEL, (void *)&vChar);
		
		if ((unsigned int)vChar == 0)
		{
			*data = booldup( 1 );
		}
		else
		{
			*data = booldup( 0 );
		}
	}
	else if( strcmp( lastname, "SSID" )==0 )
	{
		
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SSID, (void *)buf);
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "BeaconType" )==0 )
	{
#if 1//def CTCOM_WLAN_REQ	//cathy
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
		if(vChar==ENCRYPT_DISABLED)
				*data = strdup( "None" );
		else if(vChar==ENCRYPT_WEP)
			*data = strdup( "Basic" );
#else
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WLAN_DISABLED, (void *)&vChar);
		if(vChar==1) //disabled, so no beacon type
			{
				*data = strdup( "None" );
				return 0;
			}

		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
		getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_CIPHER_SUITE,(void *)&wpa_cipher);
		getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&wpa2_cipher);
		
		if( vChar >= ENCRYPT_WPA && vChar < ENCRYPT_WAPI ){
			if(vChar == ENCRYPT_WPA && wpa_cipher ==WPA_CIPHER_TKIP)
				vChar = ENCRYPT_WPA_TKIP;
			else if (vChar == ENCRYPT_WPA && wpa_cipher ==WPA_CIPHER_AES)
				vChar = ENCRYPT_WPA_AES;
			else if (vChar == ENCRYPT_WPA2 && wpa2_cipher ==WPA_CIPHER_TKIP)
				vChar = ENCRYPT_WPA2_TKIP;
			else if (vChar == ENCRYPT_WPA2 && wpa2_cipher ==WPA_CIPHER_AES)
				vChar = ENCRYPT_WPA2_AES;
		}
		if( (vChar==ENCRYPT_WEP) || (vChar==ENCRYPT_DISABLED) )
			*data = strdup( "Basic" );
#endif
		else if( vChar==ENCRYPT_WPA_TKIP|| vChar==ENCRYPT_WPA_AES)
			*data = strdup( "WPA" );
		else if( vChar==ENCRYPT_WPA2_AES /*IEEE 802.11i*/
			|| vChar==ENCRYPT_WPA2_TKIP
				)
			*data = strdup( "11i" );
		else if( vChar==ENCRYPT_WPA2_MIXED ) /*WPA & WPA2*/
			*data = strdup( "WPAand11i" );
		else
			return ERR_9002;
	}
	
#ifdef _CWMP_MAC_FILTER_	
	else if( strcmp( lastname, "MACAddressControlEnabled" )==0 )
	{
#if 1
		getWlanMib(rootIdx, 0, MIB_WLAN_MACAC_ENABLED, (void *)&vUint);
		*data = booldup( vUint!=0 );
#else
		mib_get(MIB_CWMP_MACFILTER_WLAN_MAC_CTRL, (void *)&vChar);
		*data = booldup( vChar!=0 );
#endif
	}
#endif /*_CWMP_MAC_FILTER_*/	
	else if( strcmp( lastname, "Standard" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_BAND, (void *)&vChar);
		//CWMPDBG( 1, ( stderr, "<%s:%d>target:<%d,%d>, WiFi Standard=%d\n", __FUNCTION__, __LINE__, rootIdx, vwlanIdx, vChar ) );
		if( vChar==BAND_11B )
			*data = strdup( "b" );
		else if( vChar==BAND_11G )
			*data = strdup( "g" );
		else if( vChar==BAND_11BG )
			*data = strdup( "b,g" );
		else if( vChar==BAND_11A)
			*data = strdup( "a" );
		else if( vChar==BAND_11N )
			*data = strdup( "n" );
		else if( vChar==(BAND_11B|BAND_11G|BAND_11N))
			*data = strdup( "b,g,n" );		
		else if( vChar==(BAND_11G|BAND_11N))
			*data = strdup( "g,n" );
		else if( vChar==(BAND_11A | BAND_11N))
			*data = strdup( "a,n" );
		else if( vChar==(BAND_11A | BAND_11N | BAND_5G_11AC))
			*data = strdup( "a,n,ac" );
		else if( vChar==(BAND_11N | BAND_5G_11AC))
			*data = strdup( "n,ac" );
		else if( vChar==(BAND_11A | BAND_5G_11AC))
			*data = strdup( "a,ac" );
		else if( vChar==BAND_5G_11AC)
			*data = strdup( "ac" );
		else 
			*data = strdup( "" ); 
	}
	else if( strcmp( lastname, "WEPKeyIndex" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP_DEFAULT_KEY, (void *)&vChar);
		vChar = vChar + 1;//mib's wepid is from 0 to 3
		*data = uintdup( (unsigned int)vChar );
	}
	else if( strcmp( lastname, "KeyPassphrase" )==0 )
	{
/*		
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
		*data = strdup( buf );
*/		
		*data = strdup( "" ); //always empty
	}
	else if( strcmp( lastname, "WEPEncryptionLevel" )==0 )
	{
#if 1//def CTCOM_WLAN_REQ	//cathy
			//0:disable, 1:64, 2:128
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&vChar);

		if(vChar == 0)
				*data = strdup("Disabled");
		else if (vChar == 1)
				*data = strdup("40-bit");
		else
				*data = strdup("104-bit");
#else
		*data = strdup( "Disabled,40-bit,104-bit" );
#endif
	}
	else if( strcmp( lastname, "BasicEncryptionModes" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
		if( vChar==ENCRYPT_WEP )
			*data = strdup( "WEPEncryption" );
		else if( vChar==ENCRYPT_DISABLED )
			*data = strdup( "None" );
		else
		{			
			*data = strdup( "None" );
		}
		}
	else if( strcmp( lastname, "BasicAuthenticationMode" )==0 )
	{			
#if 1//def CTCOM_WLAN_REQ	//cathy
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		if(vChar)
			*data = strdup( "EAPAuthentication" );
		else 
		{
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
			if(vChar==0)
				*data = strdup( "None" );
			else if(vChar==1)
				*data = strdup( "SharedAuthentication" );
			else
				*data = strdup( "both" );
		}				
#else
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		if(vChar)
			*data = strdup( "EAPAuthentication" );
		else
			*data = strdup( "None" );
#endif
	}
	else if( strcmp( lastname, "WPAEncryptionModes" )==0 )
		{
		getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_ENCRYPT,(void *)&vUint);
			if( vUint >= ENCRYPT_WPA && vUint < ENCRYPT_WAPI )
			{
				unsigned int cipher=0,cipher2=0;
				getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_CIPHER_SUITE,(void *)&cipher);
				getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&cipher2);
				if(vUint == ENCRYPT_WPA)
					vUint = cipher;
				else if(vUint == ENCRYPT_WPA2)
					vUint = cipher2;
				else if(vUint == ENCRYPT_WPA2_MIXED)
					vUint = (cipher | cipher2);
				else
 					vUint = 0;

				if( vUint==WPA_CIPHER_TKIP )
					*data = strdup( "TKIPEncryption" );
				else if( vUint==WPA_CIPHER_AES )
					*data = strdup( "AESEncryption" );
				else if( vUint==WPA_CIPHER_MIXED )
					*data = strdup( "TKIPandAESEncryption" );
				else
					return ERR_9002;
			}
			else if (vUint == ENCRYPT_WEP)
			{
				*data = strdup( "WEPEncryption" );
			}
			else
			{
				*data = strdup( "None" );
			}
			
		}
	else if( strcmp( lastname, "WPAAuthenticationMode" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		if(vChar)
			*data = strdup( "EAPAuthentication" );	
		else 
			*data = strdup( "PSKAuthentication" );		
	}
	else if( strcmp( lastname, "IEEE11iEncryptionModes" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_ENCRYPT,(void *)&vUint);
		if( vUint >= ENCRYPT_WPA && vUint < ENCRYPT_WAPI )
		{
			unsigned int cipher=0,cipher2=0;
			getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_CIPHER_SUITE,(void *)&cipher);
			getWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&cipher2);
			if(vUint == ENCRYPT_WPA)
				vUint = cipher;
			else if(vUint == ENCRYPT_WPA2)
				vUint = cipher2;
			else if(vUint == ENCRYPT_WPA2_MIXED)
				vUint = (cipher | cipher2);
			else
				vUint = 0;
			if( vUint==WPA_CIPHER_TKIP )
				*data = strdup( "TKIPEncryption" );
			else if( vUint==WPA_CIPHER_AES )
				*data = strdup( "AESEncryption" );
			else if( vUint==WPA_CIPHER_MIXED )
				*data = strdup( "TKIPandAESEncryption" );
			else
				return ERR_9002;
		}
		else if (vUint == ENCRYPT_WEP)
		{
			*data = strdup( "WEPEncryption" );
		}
		else
		{
			*data = strdup( "None" );
		}
	}
	else if( strcmp( lastname, "IEEE11iAuthenticationMode" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		if(vChar)
			*data = strdup( "EAPAuthentication" );	
		else 
			*data = strdup( "PSKAuthentication" );		
	}
	else if( strcmp( lastname, "PossibleChannels" )==0 )
	{		
		getWlanMib(rootIdx, 0, MIB_WLAN_PHY_BAND_SELECT, (void *)&vChar);				
		if(vChar==PHYBAND_5G){			
			*data = strdup("36-165");	//mib_all  AVAIL_CH
		}
		else if(vChar==PHYBAND_2G){				
			*data = strdup("1-11");					
		}
		else
			return ERR_9002;			
	}
	else if( strcmp( lastname, "BasicDataTransmitRates" )==0 )
	{
		/* root / vap do not share basic rates */
		if(rootIdx==0)
			*data = strdup( "" );	
		else
		{
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_BASIC_RATES,(void *)&GetValue); 
			uShort = (unsigned short)GetValue;
			getRateStr( uShort, buf );
			*data = strdup( buf );
		}				
	}
	else if( strcmp( lastname, "OperationalDataTransmitRates" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES,(void *)&GetValue);
		uShort = (unsigned short)GetValue;
		getRateStr( uShort, buf );
		*data = strdup( buf );		
	}
	else if( strcmp( lastname, "PossibleDataTransmitRates" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES,(void *)&GetValue);  
		uShort = (unsigned short)GetValue;
		getRateStr( uShort, buf );
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "InsecureOOBAccessEnabled" )==0 ) // vai UI or flash
	{		
		//getWlanMib(rootIdx, 0, MIB_WEB_WAN_ACCESS_ENABLED, (void *)&GetValue);
		*data = booldup( 1 );
	}
	else if( strcmp( lastname, "BeaconAdvertisementEnabled" )==0 )
	{		
		// always beacons.
		*data = booldup( 1 ); // maybe need change

	}
	else if( strcmp( lastname, "SSIDAdvertisementEnabled" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_HIDDEN_SSID,(void *)&GetValue); 
		*data = booldup( (GetValue==0) );
	}
	else if( strcmp( lastname, "RadioEnabled" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_FUNC_OFF, (void *)&vChar);
		*data = booldup( (vChar==0) );
	}
	else if( strcmp( lastname, "TransmitPowerSupported")==0 ) 
	{
		sprintf( buf, "%s", "100,70,50,35,15");	
		*data = strdup( buf );		
	}
	else if( strcmp( lastname, "TransmitPower")==0 )
	{
		getWlanMib(rootIdx, 0, MIB_WLAN_RFPOWER_SCALE, &vUint);		
		if( vUint==0 )
			*data = intdup( 100 );			
		else if( vUint==1 )
			*data = intdup( 70 );
		else if( vUint==2 )
			*data = intdup( 50 );
		else if( vUint==3 )
			*data = intdup( 35 );
		else if( vUint==4 )
			*data = intdup( 15 );	
		else
			return ERR_9002;
	}
	else if( strcmp( lastname, "AutoRateFallBackEnabled" )==0 )
		{
		getWlanMib(rootIdx, vwlanIdx , MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&vChar); 
		*data = booldup( (vChar==1) );
	}
	else if( strcmp( lastname, "LocationDescription" )==0 )
	{
		*data = strdup( gLocationDescription );
	}
	else if( strcmp( lastname, "RegulatoryDomain")==0 ) // spec: 3 chars of country code
	{		
		getWlanMib(rootIdx, 0, MIB_WLAN_COUNTRY_STRING, (void *)buf);
		*data = strdup(buf);
	}
	else if( strcmp( lastname, "TotalPSKFailures")==0 ) // refer to jimmylin 2015/1/6
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalPSKFailures );
	}
	else if( strcmp( lastname, "TotalIntegrityFailures")==0 )  // ???
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalIntegrityFailures );
	}
	else if( strcmp( lastname, "ChannelsInUse" )==0 )
	{
		char WlanIf[32];

		sprintf(WlanIf, "wlan%d", rootIdx);
		if ( getWlBssInfo(WlanIf, &bss) < 0)
			return -1;

		if (bss.channel)
			sprintf( buf, "%u", bss.channel );
		else
			sprintf( buf, "%s", "Auto");
				
		*data = strdup( buf );
		}
	else if( strcmp( lastname, "DeviceOperationMode" )==0 )
	{		
		getWlanMib(rootIdx, 0, MIB_WLAN_MODE,(void *)&vChar);
		if(wlaninst==1) //5G
			getWlanMib(rootIdx, 0, MIB_REPEATER_ENABLED1,(void *)&vUint);
		else if(wlaninst==2) //2G
			getWlanMib(rootIdx, 0, MIB_REPEATER_ENABLED2,(void *)&vUint);
		
		if(vChar==AP_MODE && vUint==0)
			*data = strdup( "InfrastructureAccessPoint" );
		else if(vChar==CLIENT_MODE)
			*data = strdup( "WirelessStation" );
		else if(vChar==WDS_MODE)
			*data = strdup( "WirelessBridge" );
		else if(vChar==AP_MODE && vUint==1)
			*data = strdup( "WirelessRepeater" );
		else
			return ERR_9002;
	}
	else if( strcmp( lastname, "DistanceFromRoot" )==0 )
	{
		*data = uintdup(0);
	}
	else if( strcmp( lastname, "PeerBSSID" )==0 )
	{
		unsigned int rptDisable = 2;
		getWlanMib(rootIdx, 0, MIB_WLAN_MODE, (void *)&vChar); 						// root
		getWlanMib(rootIdx, 5, MIB_WLAN_WLAN_DISABLED, (void *)&rptDisable); 		// vxd			
		
		if( vChar == AP_MODE  && rptDisable==0 )						 			// repeater mode
		{	
			getWlanBssInfo(rootIdx, 5, (void*)&bss);
			sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x", bss.bssid[0], bss.bssid[1], bss.bssid[2], bss.bssid[3], bss.bssid[4], bss.bssid[5]);			
			*data = strdup(mac);				
		}
		else if( vChar==WDS_MODE )	// WDS mode
		{
			getWlanMib(rootIdx, 0, MIB_WLAN_WDS_ENABLED, (void *)&vChar); 		// root
			if( vChar==1 )
			{			
				unsigned int entryNum;
				getWlanMib(rootIdx, 0, MIB_WLAN_WDS_NUM, (void *)&entryNum);
				if( entryNum==0 )
				{
					strcpy(buf,"");	
					
				}else
				{
					int j,k;					
					WDS_Tp pwds_EntryUI;
					unsigned char buf1[1024], buf2[20];	
					memset(buf, 0, sizeof(buf));

					for(j=0;j<entryNum;j++)
					{
						buf1[0] = j+1;    // index
						getWlanMib(rootIdx, 0, MIB_WLAN_WDS, (void *)buf1);						
						pwds_EntryUI = (WDS_Tp)buf1;						
						
						sprintf(buf2,"%02x:%02x:%02x:%02x:%02x:%02x", pwds_EntryUI->macAddr[0],pwds_EntryUI->macAddr[1],
							pwds_EntryUI->macAddr[2],pwds_EntryUI->macAddr[3],pwds_EntryUI->macAddr[4],pwds_EntryUI->macAddr[5]);						
						strncat(buf,buf2,17);					
						if(j<entryNum-1)
							strcat(buf,",");
					}										
				}
			}
			else
				strcpy(buf,"");			
			*data = strdup(buf);			
		}
		else
			return ERR_9001;		
	}
	else if( strcmp( lastname, "AuthenticationServiceMode" )==0 )
	{
		*data = strdup( "None" );
	}
	else if( strcmp( lastname, "WMMSupported" )==0 )
	{		
		*data = booldup(1);			// always supported
	}
	else if( strcmp( lastname, "UAPSDSupported" )==0 )
	{
		*data = booldup(1);			// always supported
	}
	else if( strcmp( lastname, "WMMEnable" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WMM_ENABLED,(void *)&vChar);
		if( vChar==1 )		
			*data = booldup(1);
		else
			*data = booldup(0);					
	}
	else if( strcmp( lastname, "UAPSDEnable" )==0 )
	{
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WMM_ENABLED,(void *)&vChar);
		if( vChar==1 ) 				// uapsd support implies that wmm support
		{
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_UAPSD_ENABLED,(void *)&vChar);
			*data = booldup(vChar);
		}
		else
			*data = booldup(0);			
	}
	else if( strcmp( lastname, "TotalBytesSent" )==0 )
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalBytesSent );
	}
	else if( strcmp( lastname, "TotalBytesReceived" )==0 )
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalBytesReceived );
	}
	else if( strcmp( lastname, "TotalPacketsSent" )==0 )
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalPacketsSent );
	}
	else if( strcmp( lastname, "TotalPacketsReceived" )==0 )
	{
		if( getWlanStatsInfo( wlan_ifname, &stats_info ) < 0 )
			return -1;
		*data = uintdup( stats_info.totalPacketsReceived );
	}		
	else if( strcmp( lastname, "TotalAssociations" )==0 )
	{
		if( loadWLANAssInfoByInstNum(wlaninst)< 0 )
			*data = uintdup( 0 );
		else
			*data = uintdup( gWLANTotalClients );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

/*InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.* */
int setWLANConf(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int	vChar=0;
	unsigned int wlaninst=0;
	int rootIdx=0,vwlanIdx=0;
	int NewRootIdx=0, NewvwlanIdx=0;
	int wpa_cipher=0, wpa2_cipher=0;
	unsigned int SetValue=0;
	unsigned int GetValue=0;
	unsigned short uShort;	
	int isWLANMIBUpdated=1;
#if 0	
	unsigned char origRFBand;
	unsigned char NewRFBand;
	unsigned char origWlanDisabled;
	unsigned char NewWlanDisabled;
	unsigned char origChannelWidth;
	unsigned char NewChannelWidth;
	int isRFBandChanged=0;
	CWMP_WLANCONF_T target[2];
#endif	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;
	int wlanBand2G5GSelect=0;
		
	if( (name==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	if( data==NULL ) return ERR_9007;


	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )
		return ERR_9007;

	pwlanConf = &wlanConf;
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);

#if 0
	memcpy(&target[0], &wlanConf, sizeof(CWMP_WLANCONF_T));

	pcwmpWlan = &cwmpWlan;
	memset(pcwmpWlan, 0x00, sizeof(CONFIG_WLAN_SETTING_T));

	clone_wlaninfo_get(pcwmpWlan, rootIdx,vwlanIdx);

	origRFBand=pcwmpWlan->phyBandSelect;
	origWlanDisabled = pcwmpWlan->wlanDisabled;
	origChannelWidth = pcwmpWlan->channelbonding;

	//memcpy(pcwmpWlan, &pMib->wlan[rootIdx][vwlanIdx], sizeof(CONFIG_WLAN_SETTING_T));
	//CWMPDBG( 1, ( stderr, "<%s:%d>pMib:%p\n", __FUNCTION__, __LINE__, pMib) );
	//CWMPDBG( 1, ( stderr, "<%s:%d>orig ssid:%s\n", __FUNCTION__, __LINE__, pcwmpWlan->ssid ) );
	//CWMPDBG( 1, ( stderr, "<%s:%d>orig channel:%d\n", __FUNCTION__, __LINE__, pcwmpWlan->channel ) );
#endif //#if 0


	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i = data;

		if( i==NULL ) return ERR_9007;
		vChar = (*i==0)?1:0;
		//pcwmpWlan->func_off=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WLAN_DISABLED, (void *)&vChar);
		//setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_FUNC_OFF, (void *)&vChar);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "MaxBitRate" )==0 )
	{		
		if( strlen(buf)==0 ) 
			return ERR_9007;	
		unsigned short value, setvalue;
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES, (void *)&value);		
		if(strcmp(buf,"Auto")==0) // all rates support
			setvalue = 0xfff;			
		else if(strcmp(buf,"54")==0)
			setvalue = (TX_RATE_54M | value);		
		else if(strcmp(buf,"48")==0)		
			setvalue = (TX_RATE_48M | value) & 0x7ff;	
		else if(strcmp(buf,"36")==0)
			setvalue = (TX_RATE_36M | value) & 0x3ff;
		else if(strcmp(buf,"24")==0)
			setvalue = (TX_RATE_24M | value) & 0x1ff;
		else if(strcmp(buf,"18")==0)
			setvalue = (TX_RATE_18M | value) & 0xff;
		else if(strcmp(buf,"12")==0)
			setvalue = (TX_RATE_12M | value) & 0x7f;
		else if(strcmp(buf,"11")==0)
			setvalue = (TX_RATE_11M | value) & 0xf;
		else if(strcmp(buf,"9")==0)
			setvalue = (TX_RATE_9M | value) & 0x3f;  // mask 11M
		else if(strcmp(buf,"6")==0)
			setvalue = (TX_RATE_6M | value) & 0x1f;	// mask 11M		
		else if(strcmp(buf,"5.5")==0)
			setvalue = (TX_RATE_5M | value) & 0x7;
		else if(strcmp(buf,"2")==0)
			setvalue = (TX_RATE_2M | value) & 0x3;
		else if(strcmp(buf,"1")==0)
			setvalue = (TX_RATE_1M | value) & 0x1;
		else				
			return ERR_9007;		
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES, (void *)&setvalue);				
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "Channel" )==0 )
	{
		unsigned int *i = data;

		if( i==NULL ) return ERR_9007;
		
		if(*i!=0) //0:auto
		{
#if 0//defined(CONFIG_RTL_92D_SUPPORT) || defined(CONFIG_RTL8881A_RTL8192E)
			int wlanChannel = *i;

			if(vwlanIdx == 0) // root wlan can not modify RFband
			{
				if( (wlanChannel>=1 && wlanChannel<=14 && pwlanConf->RfBandAvailable == PHYBAND_5G)					
				||(wlanChannel>=34 && wlanChannel<=165 && pwlanConf->RfBandAvailable == PHYBAND_2G)
				)
				{
					return ERR_9007;

				}

			}

			
			if( (*i>=1) && (*i<=14))
			{
				if(pwlanConf->RfBandAvailable == PHYBAND_5G)
				{
					// find free wlan
					if(getNewWLANIdxFromReq(&NewRootIdx, &NewvwlanIdx,PHYBAND_2G)<0)
					{
						return ERR_9007; //no free wlan.
					}
					else
					{
						CWMP_WLANCONF_T target[2], newTarget[2];
						CONFIG_WLAN_SETTING_T origWlanCfg, newWlanCfg;

						clone_wlaninfo_get(&origWlanCfg, rootIdx, vwlanIdx);
						clone_wlaninfo_get(&newWlanCfg, NewRootIdx, NewvwlanIdx);
						
						memset( &target[0], 0, sizeof( CWMP_WLANCONF_T ) );
						memset( &target[1], 0, sizeof( CWMP_WLANCONF_T ) );

						memset( &newTarget[0], 0, sizeof( CWMP_WLANCONF_T ) );
						memset( &newTarget[1], 0, sizeof( CWMP_WLANCONF_T ) );

						// copy 5g data to 2g
						memcpy(&newWlanCfg, &origWlanCfg, sizeof(CONFIG_WLAN_SETTING_T));

						newWlanCfg.phyBandSelect = PHYBAND_2G; // restore original phyBandSelect

						/* reinit original 5g wlan setting */
						origWlanCfg.wlanDisabled = 1;
						origWlanCfg.func_off = 0;

						/* update cwmp_wlan date */
						if(getCwmpWlanConById(rootIdx,vwlanIdx, &target[0])
							&& getCwmpWlanConById(NewRootIdx,NewvwlanIdx, &newTarget[0])
						)
						{
							int tmpInstanceNum;
							
							memcpy(&target[1], &target[0], sizeof( CWMP_WLANCONF_T ));
							memcpy(&newTarget[1], &newTarget[0], sizeof( CWMP_WLANCONF_T ));

							/* swap instance number */
							tmpInstanceNum = target[1].InstanceNum;
							target[1].InstanceNum = newTarget[1].InstanceNum;
							newTarget[1].InstanceNum = tmpInstanceNum;

							/* set  IsConfigured value*/
							target[1].IsConfigured = 0;
							newTarget[1].IsConfigured = 1;

							mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);
							mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&newTarget);

#if 0 // The channel of vrtual wlan is follow root wlan, here just to change rfband
							if(newWlanCfg.channelbonding == 1) // 40 MHz
							{
								
								if(wlanChannel <6){
									newWlanCfg.controlsideband=1; //lower
								}else{
									newWlanCfg.controlsideband=0; //upper
								}

							}
#endif
							clone_wlaninfo_set(&origWlanCfg, 0, 0, rootIdx, vwlanIdx, 0);
							clone_wlaninfo_set(&newWlanCfg, 0, 0, NewRootIdx, NewvwlanIdx, 0);


							rootIdx = NewRootIdx;
							vwlanIdx = NewvwlanIdx;
						}
						else
						{
							return ERR_9007; //no free wlan.
						}

					}
				}
				else
				{

				}
				valid = 1;
			}
			else if( (*i>=34) && (*i<=165) ) 
			{
				if(pwlanConf->RfBandAvailable == PHYBAND_2G)
				{
					// find free wlan
					if(getNewWLANIdxFromReq(&NewRootIdx, &NewvwlanIdx,PHYBAND_5G)<0)
					{
						return ERR_9007; //no free wlan.
					}
					else
					{
						CWMP_WLANCONF_T target[2], newTarget[2];
						CONFIG_WLAN_SETTING_T origWlanCfg, newWlanCfg;

						clone_wlaninfo_get(&origWlanCfg, rootIdx, vwlanIdx);
						clone_wlaninfo_get(&newWlanCfg, NewRootIdx, NewvwlanIdx);
						
						memset( &target[0], 0, sizeof( CWMP_WLANCONF_T ) );
						memset( &target[1], 0, sizeof( CWMP_WLANCONF_T ) );

						memset( &newTarget[0], 0, sizeof( CWMP_WLANCONF_T ) );
						memset( &newTarget[1], 0, sizeof( CWMP_WLANCONF_T ) );

						// copy 2g data to 5g
						memcpy(&newWlanCfg, &origWlanCfg, sizeof(CONFIG_WLAN_SETTING_T));

						newWlanCfg.phyBandSelect = PHYBAND_5G; // restore original phyBandSelect

						/* reinit original 2g wlan setting */
						origWlanCfg.wlanDisabled = 1;
						origWlanCfg.func_off = 0;

						/* update cwmp_wlan date */
						if(getCwmpWlanConById(rootIdx,vwlanIdx, &target[0])
							&& getCwmpWlanConById(NewRootIdx,NewvwlanIdx, &newTarget[0])
						)
						{
							int tmpInstanceNum;
							
							memcpy(&target[1], &target[0], sizeof( CWMP_WLANCONF_T ));
							memcpy(&newTarget[1], &newTarget[0], sizeof( CWMP_WLANCONF_T ));

							/* swap instance number */
							tmpInstanceNum = target[1].InstanceNum;
							target[1].InstanceNum = newTarget[1].InstanceNum;
							newTarget[1].InstanceNum = tmpInstanceNum;

							/* set  IsConfigured value*/
							target[1].IsConfigured = 0;
							newTarget[1].IsConfigured = 1;

							mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&target);
							mib_set(MIB_CWMP_WLANCONF_MOD, (void *)&newTarget);

#if 0 // vrtual wlan's channel is follow root wlan, here just to change rfband
							if(newWlanCfg.channelbonding == 1) // 40 MHz
							{
								if(wlanChannel > 0 && (wlanChannel == 36 
									|| wlanChannel==44 || wlanChannel==149
									|| wlanChannel == 157 ||wlanChannel==165)){
									newWlanCfg.controlsideband=1; //lower
								}else if(wlanChannel > 0 && (wlanChannel == 40 
									|| wlanChannel==48 || wlanChannel==153
									|| wlanChannel == 161)){
									newWlanCfg.controlsideband=0; //upper
								}

							}
#endif

							clone_wlaninfo_set(&origWlanCfg, 0, 0, rootIdx, vwlanIdx, 0);
							clone_wlaninfo_set(&newWlanCfg, 0, 0, NewRootIdx, NewvwlanIdx, 0);


							rootIdx = NewRootIdx;
							vwlanIdx = NewvwlanIdx;
						}
						else
						{
							return ERR_9007; //no free wlan.
						}

					}
				}
				else
				{

				}
				valid = 1;
			}
			else
				valid =0;
#else
			int wlanChannel = *i;	

			if( (wlanChannel>=1 && wlanChannel<=14 && pwlanConf->RfBandAvailable == PHYBAND_5G)					
				||(wlanChannel>=34 && wlanChannel<=165 && pwlanConf->RfBandAvailable == PHYBAND_2G))
			{
				return ERR_9007;
			}	
#endif						
		}
		vChar = *i;

#if 0
		memset(pcwmpWlan, 0x00, sizeof(CONFIG_WLAN_SETTING_T));

		clone_wlaninfo_get(pcwmpWlan, rootIdx,vwlanIdx);
	
		pcwmpWlan->channel = vChar;
#endif
		setWlanMib(rootIdx, 0, MIB_WLAN_CHANNEL, (void *)&vChar);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "AutoChannelEnable" )==0 )
	{
		int *i = data;
		if (i == NULL) 
			return ERR_9007;
		if (*i == 1) {
			vChar = 0;
			setWlanMib(rootIdx, 0, MIB_WLAN_CHANNEL, (void *)&vChar);  
			isWLANMIBUpdated = 1;
		}
		else {
			bss_info bss;
			char WlanIf[32];

			sprintf(WlanIf, "wlan%d", rootIdx);
			if ( getWlBssInfo(WlanIf, &bss) < 0)
				return -1;

			vChar = bss.channel;
			setWlanMib(rootIdx, 0, MIB_WLAN_CHANNEL, (void *)&vChar);
			isWLANMIBUpdated = 1;

		}
	}
	else if( strcmp( lastname, "SSID" )==0 )
	{						
		//MIB_WLAN_SSID
		if( buf==NULL ) return ERR_9007;
		if( (strlen(buf)==0) || (strlen(buf)>=MAX_SSID_LEN) ) return ERR_9007;

		//sprintf(pcwmpWlan->ssid, "%s", buf);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SSID, (void *)buf);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "BeaconType" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "None" )==0 )
		{
#if 1//def CTCOM_WLAN_REQ	//cathy
			vChar = ENCRYPT_DISABLED;
			//pcwmpWlan->encrypt=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			//pcwmpWlan->authType=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);

#else
			vChar = 1;
			pcwmpWlan->wlanDisabled=vChar;
			//setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WLAN_DISABLED, (void *)&vChar);

#endif
			isWLANMIBUpdated=1;
		}
		else if( strcmp( buf, "Basic" )==0 )
		{
#if 1//def CTCOM_WLAN_REQ	//cathy
			vChar = ENCRYPT_WEP;
			//pcwmpWlan->encrypt=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			
			//0:disable, 1:64, 2:128
			//vChar=pcwmpWlan->wep;
			getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&vChar);
			if(vChar==0)
			{
				vChar=WEP64;
				//pcwmpWlan->wep=vChar;
				setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&vChar);
			}
			vChar = 2;	//both open and shared mode
			//pcwmpWlan->authType=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
#else

			vChar = ENCRYPT_WEP;
			pcwmpWlan->encrypt=vChar;
			//setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);


#endif
			isWLANMIBUpdated=1;
			}
		else if( strcmp( buf, "WPA" )==0 )
		{
			vChar = ENCRYPT_WPA;
			wpa_cipher = WPA_CIPHER_TKIP;
			//pcwmpWlan->encrypt=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			//pcwmpWlan->wpaCipher=wpa_cipher;
			setWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_CIPHER_SUITE,(void *)&wpa_cipher);
			vChar = 2;	// PSK
			//pcwmpWlan->authType=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);
			/* if not wep, force open system */
			vChar = 0; 
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
			isWLANMIBUpdated=1;
		}
		else if( strcmp( buf, "11i" )==0 )
		{
								
			vChar = ENCRYPT_WPA2;
			wpa2_cipher = WPA_CIPHER_AES;
			//pcwmpWlan->encrypt=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			//pcwmpWlan->wpa2Cipher = wpa2_cipher;
			setWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&wpa2_cipher);
			vChar = 0;
			//pcwmpWlan->authType=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);
			/* if not wep, force open system */
			vChar = 0; 
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
			isWLANMIBUpdated=1;
		}
		else if( strcmp( buf, "WPAand11i" )==0 )
		{
			vChar = ENCRYPT_WPA2_MIXED;
			wpa_cipher = WPA_CIPHER_MIXED;
			wpa2_cipher = WPA_CIPHER_MIXED;
			//pcwmpWlan->encrypt=vChar;
			//pcwmpWlan->wpaCipher=wpa_cipher;
			//pcwmpWlan->wpa2Cipher = wpa2_cipher;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			setWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_CIPHER_SUITE,(void *)&wpa_cipher);
			setWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA2_CIPHER_SUITE,(void *)&wpa2_cipher);
		
			vChar = 2;
			//pcwmpWlan->authType=vChar;				
			setWlanMib(rootIdx, vwlanIdx,MIB_WLAN_WPA_AUTH, (void *)&vChar);
			/* if not wep, force open system */
			vChar = 0; 
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
			isWLANMIBUpdated=1;
		}else
			return ERR_9007;
	}
#ifdef _CWMP_MAC_FILTER_
	else if( strcmp( lastname, "MACAddressControlEnabled" )==0 )
	{
		int *i = data;
		unsigned int uInt;
		
		if( i==NULL ) return ERR_9007;
#if 1
		/* 0: disable, 1: allow list, 2: deny list */
		if(*i!=0 && *i!=1)
			return ERR_9007;
		
		uInt = (*i==1)?1:0;
		setWlanMib(rootIdx, 0, MIB_WLAN_MACAC_ENABLED, (void *)&uInt);
#else
		vChar = (*i==1)?1:0;
		mib_set( MIB_CWMP_MACFILTER_WLAN_MAC_CTRL, (void *)&vChar);
		{
			unsigned char eth_mac_ctrl=0,mac_out_dft=1;
			mib_get(MIB_CWMP_MACFILTER_ETH_MAC_CTRL, (void *)&eth_mac_ctrl);
			if( vChar==1 || eth_mac_ctrl==1 )
				mac_out_dft=0;//0:deny, 1:allow
			mib_set(MIB_CWMP_MACFILTER_OUT_ACTION, (void *)&mac_out_dft);
		}
#endif
		isWLANMIBUpdated=1;
	}
#endif /*_CWMP_MAC_FILTER_*/
	else if( strcmp( lastname, "WEPKeyIndex" )==0 )
	{
		unsigned int *i = data;

		if( i==NULL ) return ERR_9007;
		if( (*i<1) || (*i>4) ) return ERR_9007;
		vChar = (unsigned char)*i;
		vChar = vChar - 1; //mib's wepid is from 0 to 3
		//pcwmpWlan->wepDefaultKey=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP_DEFAULT_KEY, (void *)&vChar);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "KeyPassphrase" )==0 ) // for wep key, has nothing with wpa_psk??
	{
		unsigned int pskfmt, keyfmt, wep, authType, enc;		
		if( buf==NULL ) return ERR_9007;
#if 0 // ori
		if( (strlen(buf)<8) || (strlen(buf)>63) ) return ERR_9007;
		pskfmt = 0; //0:Passphrase,   1:hex

		{
			//pcwmpWlan->wpaPSKFormat=pskfmt;
			//sprintf(pcwmpWlan->wpaPSK,"%s",buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_PSK_FORMAT, (void *)&pskfmt);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_PSK, (void *)buf);
		}
#endif
		keyfmt = KEY_ASCII; //0:passphrase 	1:hex	
		enc = ENCRYPT_WEP;
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&authType);
		//printf("#### [%s %d]keyfmt:%d, enc:%d, authType:%d\n", __FUNCTION__,__LINE__,keyfmt, enc, authType);
		
		if( authType!=AUTH_OPEN && authType!=AUTH_SHARED )
			authType = AUTH_BOTH;		
		if(strlen(buf)==5)	 	// 64bit 			
			wep = WEP64;			
		else if(strlen(buf)==13) // 128 bit		
			wep = WEP128;			
		else
			return ERR_9007;	
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&enc);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP_KEY_TYPE, (void *)&keyfmt);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&wep);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&authType);

		if (wep == WEP64){
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY1, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY2, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY3, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP64_KEY4, (void *)buf);			
		}else if(wep == WEP128){
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY1, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY2, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY3, (void *)buf);
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP128_KEY4, (void *)buf);				
		}		
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "BasicEncryptionModes" )==0 )
	{
		unsigned int c_mode;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
	
		
		//c_mode = pcwmpWlan->encrypt;
		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&c_mode);
		//printf("##### [%s %d]c_mode:%d\n", __FUNCTION__,__LINE__,c_mode);
		if( strcmp( buf, "WEPEncryption" )==0 )
		{
			if( c_mode==ENCRYPT_DISABLED )
			{
				vChar = ENCRYPT_WEP;
				{
					//pcwmpWlan->encrypt = vChar;
					setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
					{
						unsigned char c_key=0;
						//c_key = pcwmpWlan->wep;
						getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&c_key);
						if( c_key==WEP_DISABLED )
						{
							c_key=WEP64;
							//pcwmpWlan->wep = c_key;
							setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WEP, (void *)&c_key);
						}
					}
				}

			}
		}
		else if( strcmp( buf, "None" )==0 )
		{
			if( c_mode==ENCRYPT_WEP )
			{
				vChar = ENCRYPT_DISABLED;
				//pcwmpWlan->encrypt = vChar;
				setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT, (void *)&vChar);
			}
			
		}else
			return ERR_9007;

		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "BasicAuthenticationMode" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
#if 1//def CTCOM_WLAN_REQ	//cathy
		if( strcmp( buf, "EAPAuthentication")==0 ){
			vChar = 1;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);			
		}else{
			if( strcmp( buf, "None")==0 || strcmp( buf, "Open authentication")==0 ) 
				vChar=0;
			else if( strcmp( buf, "SharedAuthentication")==0 )
				vChar=1;
			else if( strcmp( buf, "Both")==0 )
				vChar=2;
			else
				return ERR_9007;
			//pcwmpWlan->authType=vChar;	
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_AUTH_TYPE, (void *)&vChar);
			isWLANMIBUpdated=1;
		}			
#else
		if( strcmp( buf, "None")==0 )
			vChar=0;
		else if( strcmp( buf, "EAPAuthentication")==0 )
			vChar=1;
		else		
#endif	
#if 0//ndef CTCOM_WLAN_REQ	//cathy
		pcwmpWlan->enable1X = vChar;
		//setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		vChar=AUTH_BOTH;
#endif
	}
	else if( strcmp( lastname, "WPAEncryptionModes" )==0 )
	{
		unsigned int cipher=0;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;

		vChar = ENCRYPT_WPA;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT,(void *)&vChar);
		if(strcmp( buf, "TKIPEncryption" )==0)
			cipher = WPA_CIPHER_TKIP;
		else if(strcmp( buf, "AESEncryption" )==0)
			cipher = WPA_CIPHER_AES;
		else if(strcmp( buf, "TKIPandAESEncryption" )==0){		
			cipher = WPA_CIPHER_MIXED;			
		}
		else
			return ERR_9001;		
		//pcwmpWlan->wpaCipher=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_CIPHER_SUITE, (void *)&cipher);		
		isWLANMIBUpdated=1;
						}
	else if( strcmp( lastname, "WPAAuthenticationMode" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "PSKAuthentication")==0 ){
			vChar=WPA_AUTH_PSK;
			//pcwmpWlan->wpaAuth=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);	
			vChar=0;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		}
		else if( strcmp( buf, "EAPAuthentication")==0 ){
			vChar=1;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
			vChar=WPA_AUTH_AUTO;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);
		}
		else
			return ERR_9001;
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "IEEE11iEncryptionModes" )==0 )
	{
		unsigned int cipher2=0;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;

		vChar = ENCRYPT_WPA2;		
		if(strcmp( buf, "TKIPEncryption" )==0)
			cipher2 = WPA_CIPHER_TKIP;
		else if(strcmp( buf, "AESEncryption" )==0)
			cipher2 = WPA_CIPHER_AES;
		else if(strcmp( buf, "TKIPandAESEncryption" )==0){	
			vChar = ENCRYPT_WPA2_MIXED;
			cipher2 = WPA_CIPHER_MIXED;	
		}
		else
			return ERR_9001;		
		//pcwmpWlan->wpaCipher=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENCRYPT,(void *)&vChar);
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher2);		
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "IEEE11iAuthenticationMode" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( strcmp( buf, "PSKAuthentication")==0 ){
			vChar=WPA_AUTH_PSK;
			//pcwmpWlan->wpaAuth=vChar;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);	
			vChar=0;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
		}
		else if( strcmp( buf, "EAPAuthentication")==0 ){
			vChar=1;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_ENABLE_1X, (void *)&vChar);
			vChar=WPA_AUTH_AUTO;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WPA_AUTH, (void *)&vChar);
		}
		else
			return ERR_9001;
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "BasicDataTransmitRates" )==0 )
	{
		
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( rootIdx==0 ) // 5g has no basic rates		
			return ERR_9007;		
		
		if( setRateStr_Basic( buf, &uShort )<0 ) return ERR_9007;			
		//pcwmpWlan->basicRates=uShort;
		SetValue=uShort;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_BASIC_RATES, (void *)&SetValue);				
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "OperationalDataTransmitRates" )==0 )
	{

		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( setRateStr( buf, &uShort )<0 ) return ERR_9007;
			
		//pcwmpWlan->supportedRates=uShort;
		SetValue=uShort;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_SUPPORTED_RATES, (void *)&SetValue);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "InsecureOOBAccessEnabled" )==0 )
	{	
		int *i = data;
		if( i==NULL ) 
			return ERR_9007; 
#if 0
		vChar = (*i==1)?1:0;
		setWlanMib(rootIdx, 0, MIB_WEB_WAN_ACCESS_ENABLED, (void *)&vChar);
		vChar = 8080;
		setWlanMib(rootIdx, 0, MIB_WEB_WAN_ACCESS_PORT, (void *)&vChar);
#endif
		if(*i !=1)
			return ERR_9001; 
		
		isWLANMIBUpdated=0;
	}
	else if( strcmp( lastname, "BeaconAdvertisementEnabled" )==0 )
	{
		int *i = data;		
		
		if( i==NULL ) return ERR_9007;
		vChar = (*i==0)?0:1;

		isWLANMIBUpdated=0;
	}
	else if( strcmp( lastname, "SSIDAdvertisementEnabled" )==0 )
	{
		int *i = data;
		
		if( i==NULL ) return ERR_9007;
		vChar = (*i==0)?1:0;
		//pcwmpWlan->hiddenSSID=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_HIDDEN_SSID, (void *)&vChar);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "RadioEnabled" )==0 )
	{
		int *i = data;
	
		if( i==NULL ) return ERR_9007;
		vChar = (*i==0)?1:0;
		//pcwmpWlan->func_off=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_FUNC_OFF, (void *)&vChar); 
		isWLANMIBUpdated=1;			
	}
	else if( strcmp( lastname, "TransmitPower")==0 ) 
	{
		int *value = data;
		if( value==NULL ) 
			return ERR_9007;	
		
		if( *value<0 || *value>100 ) // check value range(0-100)
			return ERR_9007;
			
		// if the board is not k-ed, then cannot set power scale
		//getWlanMib(rootIdx, 0, MIB_HW_11N_THER, (void *)&GetValue);		
		//if(GetValue==0)
		//	return ERR_9001;
		
		if( *value==100 )
			SetValue = 0;
		else if( *value==70 )
			SetValue = 1;
		else if( *value==50 )
			SetValue = 2;
		else if( *value==35 )
			SetValue = 3;
		else if( *value==15 )
			SetValue = 4;
		else
			return ERR_9007;
		setWlanMib(rootIdx, 0, MIB_WLAN_RFPOWER_SCALE, (void *)&SetValue);
		isWLANMIBUpdated=1;		
	}
	else if( strcmp( lastname, "AutoRateFallBackEnabled" )==0 )
	{
		int *i = data;
		if( i==NULL ) return ERR_9007;
		vChar = (*i==1)?1:0;
		//pcwmpWlan->rateAdaptiveEnabled=vChar;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&vChar);
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "LocationDescription" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 )
			strcpy( gLocationDescription, "" );
		else if( strlen(buf)<4096 )
			strcpy( gLocationDescription, buf );
		else 
			return ERR_9007;
		
		isWLANMIBUpdated=0;
	}
	else if( strcmp( lastname, "RegulatoryDomain")==0 )
	{
		if (buf == NULL) return ERR_9007;	
		if (strlen(buf) != 3 && strlen(buf) != 2) return ERR_9007; 
		if (buf[0] < 'A' || buf[0] > 'Z')  return ERR_9007; 
		if (buf[1] < 'A' || buf[1] > 'Z')  return ERR_9007; 
		if(strlen(buf) == 3)		
			if (buf[2] != ' ' && buf[2] != 'I' && buf[2] != 'O')				
				return ERR_9007;		
		setWlanMib(rootIdx, 0, MIB_WLAN_COUNTRY_STRING, (void *)buf);
		isWLANMIBUpdated = 1;
	}
	else if( strcmp( lastname, "DeviceOperationMode" )==0 )
	{
		unsigned int rpt_id, vxd_set_val = 2;  
		if( buf==NULL ) 
			return ERR_9007;
		if(wlaninst==1)
			rpt_id = MIB_REPEATER_ENABLED1;
		else if(wlaninst==2)
			rpt_id = MIB_REPEATER_ENABLED2;
		
		if( strcmp(buf, "InfrastructureAccessPoint" )==0 )
		{
			vChar=AP_MODE;			
			vxd_set_val = 0;
		}			
		else if( strcmp(buf, "WirelessStation" )==0 )
#ifdef CONFIG_WLAN_CLIENT_MODE
			vChar=CLIENT_MODE;
#else
			return ERR_9001;
#endif
		else if( strcmp(buf, "WirelessBridge" )==0 )
			vChar=WDS_MODE;
		else if( strcmp(buf, "WirelessRepeater" )==0 )
		{
			vChar=AP_MODE;
			vxd_set_val = 1;			
		}else			
			return ERR_9007;			
		setWlanMib(rootIdx, 0, MIB_WLAN_MODE, (void *)&vChar);	
		if( vxd_set_val==0 || vxd_set_val==1 ){
			setWlanMib(rootIdx, 0, rpt_id, (void *)&vxd_set_val);
			vxd_set_val = (vxd_set_val?0:1);
			setWlanMib(rootIdx, 5, MIB_WLAN_WLAN_DISABLED, (void *)&vxd_set_val);
		}			
		isWLANMIBUpdated=1;
	}
	else if( strcmp( lastname, "DistanceFromRoot" )==0 )
	{
		isWLANMIBUpdated=0;
	}
	else if( strcmp( lastname, "PeerBSSID" )==0 )
	{
		unsigned int rptDisable = 2;
		WDS_T macEntry, macEntrytmp;		
		struct wifi_mib *pmib;
		unsigned char mac_addr[6];
		
		if( strlen(buf)!=12 || !string_to_hex(buf, mac_addr, 12) || (mac_addr[0] & 0x01) )
			return ERR_9007;
		
		getWlanMib(rootIdx, 0, MIB_WLAN_MODE, (void *)&vChar); 						// root
		getWlanMib(rootIdx, 5, MIB_WLAN_WLAN_DISABLED, (void *)&rptDisable); 		// vxd	
		if( vChar == AP_MODE  && rptDisable==0 )						 			// repeater mode	
		{		
			setWlanMib(rootIdx, 5, MIB_WLAN_PEER_BSSID, (void *)mac_addr);
			isWLANMIBUpdated=1;			
		}
		else if( vChar==WDS_MODE )
		{
			getWlanMib(rootIdx, 0, MIB_WLAN_WDS_ENABLED, (void *)&vChar); 			// root
			if( vChar==1 )
			{
				unsigned int entryNum;				
				int j;				
				getWlanMib(rootIdx, 0, MIB_WLAN_WDS_NUM, (void *)&entryNum);
				if( entryNum+1 > MAX_WDS_NUM )
					return ERR_9001;			
			
				for(j=1; j<=entryNum; j++)
				{					
					memset(&macEntrytmp, 0x00, sizeof(macEntrytmp));
					*((char *)&macEntrytmp) = (char)j;		
					getWlanMib(rootIdx, 0, MIB_WLAN_WDS, (void *)&macEntrytmp);
					if (!memcmp(macEntrytmp.macAddr, mac_addr, 6)) // mac already exist
					{
						printf("<--- %s %d> MAC already exist!!!\n",__FUNCTION__,__LINE__);
						return ERR_9001;	
					}					
				}				
				memcpy(macEntry.macAddr, mac_addr, 6);
				macEntry.comment[0] = '\0'; // default
				macEntry.fixedTxRate =0;	// default			
				
				// set to MIB. try to delete it first to avoid duplicate case
				setWlanMib(rootIdx, 0, MIB_WLAN_WDS_DEL, (void *)&macEntry);
				setWlanMib(rootIdx, 0, MIB_WLAN_WDS_ADD, (void *)&macEntry);				
				isWLANMIBUpdated=1;
			
			}			
			else
				return ERR_9001;				
		}
		else
			return ERR_9001;	
	}
		
	else if( strcmp( lastname, "AuthenticationServiceMode" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strcmp(buf, "None" )!=0 ) return ERR_9001;
		isWLANMIBUpdated=0;
	}
        else if( strcmp( lastname, "WMMEnable" )==0 )
	{
		unsigned int *i = data;
		if( i==NULL ) 
			return ERR_9007;	
			
		if( *i!=0 && *i!=1 ) 
			return ERR_9007;
		
		if( *i==1 )
			vChar = 1;
		else
			vChar = 0;
		setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WMM_ENABLED, (void *)&vChar);
		isWLANMIBUpdated=1;		
	}
	else if( strcmp( lastname, "UAPSDEnable" )==0 )
	{
		int *i = data;
		if( i==NULL ) 
			return ERR_9007;	
		if( *i!=0 && *i!=1 ) 
			return ERR_9007;		

		getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WMM_ENABLED,(void *)&vChar);
		if( vChar==1 ){
			if( *i==1 )
				vChar = 1;
			else
				vChar = 0;
			setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_UAPSD_ENABLED, (void *)&vChar);
		}else{
			if( *i==1 )
				return ERR_9007;	
			else{
				vChar = 0;
				setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_UAPSD_ENABLED, (void *)&vChar);
			}			
		}		
		isWLANMIBUpdated=1;
	}
	else{
		return ERR_9005;
	}

#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
	return 0;
#else
		if(isWLANMIBUpdated)
			return 1;
		else
			return 0;
#endif
	
}

/**************************************************************************************/
/* utility functions*/
/**************************************************************************************/
/*InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}. 
SingleBand Mapping:
InstanceNum:
1,		2,			3,			4,			5,			6,
wlan iface:
wlan0	wlan0-va0	wlan0-va1	wlan0-va2	wlan0-va3	wlan0-vxd

mib index:<rootIdx, vwlan_idx>	
<0,0>	<0,1>		<0,2>		<0,3>		<0,4>		<0,5>

DualBand Mapping:
InstanceNum:
1,		2,			3,			4,			5,			6,			7,			8,			9,			10,			11,			12

wlan iface:
wlan0	wlan0-va0	wlan0-va1	wlan0-va2	wlan0-va3	wlan0-vxd	wlan1		wlan1-va0	wlan1-va1	wlan1-va2	wlan1-va3	wlan1-vxd

mib index:<rootIdx, vwlan_idx>	
<0,0>	<0,1>		<0,2>		<0,3>		<0,4>		<0,5>		<1,0>		<1,1>		<1,2>		<1,3>		<1,4>		<1,5>

*/
int getWLANIdxFromInstNum(int instnum, CWMP_WLANCONF_T *pwlanConf, int *rootIdx, int *vwlan_idx)
{
	
	int ret=-1;
	unsigned int i,num;
		
	mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);

	for( i=1; i<=num;i++ )
	{
		*((char *)pwlanConf) = (char)i;
		if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
			continue;
		if( (pwlanConf->InstanceNum==instnum))
		{
			*rootIdx = pwlanConf->RootIdx;
			*vwlan_idx = pwlanConf->VWlanIdx;
			ret = 0;
			break;
		}
	}	
	return ret; 

}		

int getInstNumFromWLANIdx(int rootIdx, int vwlan_idx,CWMP_WLANCONF_T *pwlanConf, int *instnum)
{

	int ret=-1;
	unsigned int i,num;
		
	mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);

	for( i=1; i<=num;i++ )
	{
		*((char *)pwlanConf) = (char)i;
		if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
			continue;
		if( (pwlanConf->RootIdx==rootIdx) && (pwlanConf->VWlanIdx==vwlan_idx))
		{
			*instnum = pwlanConf->InstanceNum;
			ret = 0;
			break;
		}
	}	
	return ret; 

	return 0;
}

int getNewWLANIdxFromReq(int *rootIdx, int *vwlan_idx, int ReqBand)
{
	
	int ret=-1;
	unsigned int i,num;
	int maxWLAN=NUM_WLAN_INTERFACE;
	CWMP_WLANCONF_T *pwlanConf, wlanConf;	

	pwlanConf = &wlanConf;
	mib_get(MIB_CWMP_WLANCONF_TBL_NUM, (void *)&num);

	for( i=1; i<=num;i++ )
	{
		*((char *)pwlanConf) = (char)i;
		if(!mib_get(MIB_CWMP_WLANCONF_TBL, (void *)pwlanConf))
			continue;	
		
		if( pwlanConf->IsConfigured==0 && pwlanConf->RfBandAvailable ==ReqBand)
		{
			*rootIdx = pwlanConf->RootIdx;
			*vwlan_idx = pwlanConf->VWlanIdx;
			ret = 0;
			break;
		}
	}	
	return ret; 

}		

extern unsigned int getInstNum( char *name, char *objname );
unsigned int getWLANConfInstNum( char *name )
{
	return getInstNum( name, "WLANConfiguration" );
}

unsigned int getWEPInstNum( char *name )
{
	return getInstNum( name, "WEPKey" );
}

unsigned int getWPSRegistrarInstNum( char *name )
{
	return getInstNum( name, "Registrar" );
}

unsigned int getAssDevInstNum( char *name )
{
	return getInstNum( name, "AssociatedDevice" );
}

unsigned int getPreSharedKeyInstNum( char *name )
{
	return getInstNum( name, "PreSharedKey" );
}


char WLANASSFILE[] = "/tmp/stainfo";
int updateWLANAssociations( void )
{
	int i;
	time_t c_time=0;
	int w_idx, vw_idx;
	
	c_time = time(NULL);
	if( c_time >= gWLANAssUpdateTime+WLANUPDATETIME )
	{

		for(w_idx=0;w_idx<NUM_WLAN_INTERFACE;w_idx++)
		{

			for(vw_idx=0;vw_idx<NUM_VWLAN_INTERFACE;vw_idx++)
			{
				char filename[32];
				FILE *fp=NULL;
				int  has_info;
				char wlan_ifname[10] = {0};

				if(vw_idx == 0)
					sprintf(wlan_ifname, "wlan%d",w_idx);
				else
					sprintf(wlan_ifname, "wlan%d-va%d",w_idx,vw_idx-1);					

				has_info=1;
				memset( gWLANAssociations, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1) );	
				if ( getWlStaInfo( wlan_ifname,  (WLAN_STA_INFO_T *)gWLANAssociations ) < 0 )
				{
					
					CWMPDBG( 2, ( stderr, "<%s:%d>wlanIface=%s getSTAInfo error\n", __FUNCTION__, __LINE__,wlan_ifname ) );
					memset( gWLANAssociations, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1) );
					has_info=0;
				}

			
				sprintf( filename, "%s.%s", WLANASSFILE, wlan_ifname );
				fp=fopen( filename, "wb" );
				if(fp)
				{
					if(has_info)
					{
						//printf("#### [%s %d]after getWlStaInfo \n", __FUNCTION__,__LINE__);
						//dumpInfo(1, gWLANAssociations,  sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1) );
					
						FILE *fp2=NULL;
						char buf[100]; // ... the max length of a line is more than 70
						int keyg;						
						sprintf( filename, "/proc/%s/mib_gkeytbl", wlan_ifname );
						fp2 = fopen(filename, "rb");
						fwrite( gWLANAssociations, 1, sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1), fp );
						if(fp2){
							fscanf(fp2, "  %s", buf);
							fscanf(fp2, "    dot11Privacy: %d", &keyg);
							fclose(fp2);
						CWMPDBG_FUNC(MODULE_DATA_MODEL, LEVEL_DETAILED, ("<%s:%d>wlanIface=%s keyg=%d\n", __FUNCTION__, __LINE__,wlan_ifname,keyg ) );
						}
						sprintf( filename, "/proc/%s/sta_keyinfo", wlan_ifname );
						fp2 = fopen(filename, "r");
						if(fp2){
							int id,key;
							int count;
							char mac[18];
							fgets(buf, sizeof(buf), fp2);
							while(!feof(fp2)){
								if(fscanf(fp2, "%d: %s", &id, buf)<=0)
									break;
								fscanf(fp2, "    hwaddr: %s", mac);
								fscanf(fp2, "    keyInCam: %s", buf);
								fscanf(fp2, "    dot11Privacy: %d", &key);
								fscanf(fp2, "    dot11EncryptKey.dot11TTKeyLen: %d", buf);
								fscanf(fp2, "    dot11EncryptKey.dot11TMicKeyLen: %d", buf);
								fscanf(fp2, "    dot11EncryptKey.dot11TTKey.skey: %s", buf);
								fwrite( &id, 1, sizeof(int), fp );
								fwrite( &mac, 1, 12, fp );
								fwrite( &key, 1, sizeof(int), fp );
								fwrite( &keyg, 1, sizeof(int), fp );
								fwrite( &buf, 1, 40, fp );
								//printf("id:%d, mac:%02x%02x%02x%02x%02x%02x, key:%d, keyg:%d\n",
								//	id, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5], key, keyg);
								
								fgets(buf, sizeof(buf), fp2); // skip blank line							
								fgets(buf, sizeof(buf), fp2);
								fgets(buf, sizeof(buf), fp2);
								fgets(buf, sizeof(buf), fp2);
								fgets(buf, sizeof(buf), fp2);
							}
							fclose(fp2);
						}						
					}
					fclose(fp);
				}	
			}
		}

		
		gWLANAssUpdateTime = c_time;
	}
	return 0;
}

#if defined(WIFI_SIMPLE_CONFIG_SUPPORT)
int getWPSRegistrarEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{	
	char	*lastname = entity->info->name;		
	unsigned int wlaninst=0;
	int rootIdx=0, vwlanIdx=0, reginst=0;		
	char wlan_ifname[10]={0};	
	CWMP_WLANCONF_T *pwlanConf, wlanConf;	
	int maxWlanIface=NUM_WLAN_INTERFACE;
	WSC_ER_T extReg;
	unsigned int vChar = 0;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else if(vwlanIdx<NUM_VWLAN+1)
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);
	else
		sprintf(wlan_ifname,"wlan%d-vxd",rootIdx);		

	loadWLANExternalRegByInstNum(wlaninst);
	reginst = getWPSRegistrarInstNum( name );
	
	memset(&extReg, 0, sizeof(WSC_ER_T));	
	if(getWLAN_WSC_ER_INFO(reginst-1, &extReg)<0){
		printf("[%s %d]getWLAN_WSC_ER_INFO fail !!!\n", __FUNCTION__,__LINE__);
		return ERR_9007;
	}
//	printf("[%s %d]info:%s\n", __FUNCTION__,__LINE__, (char *)&extReg);
			
	*type = entity->info->type;
	*data = NULL;	
	if( strcmp( lastname, "Enable" )==0 )
	{
		vChar = (extReg.wscERDisable?0:1);		
		*data = booldup( vChar );	
	}	
	else if ( strcmp( lastname, "UUID" )==0 )
	{		
		*data = strdup(extReg.wscERUUID);
	}	
	else if ( strcmp( lastname, "DeviceName" )==0 )
	{
		*data = strdup(extReg.wscERDeviceName);
	}
	else
	{
		return ERR_9005;
	}	
	return 0;
}

int setWPSRegistrarEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastname = entity->info->name;
	char *buf=data;	
	unsigned int wlaninst=0;
	int rootIdx=0,vwlanIdx=0, reginst=0;	
	int isWLANMIBUpdated=1;
	CWMP_WLANCONF_T *pwlanConf, wlanConf;	
	int maxWlanIface=NUM_WLAN_INTERFACE;	
	char wlan_ifname[10] = {0};
	WSC_ER_T extReg;
	int setvalue = 0;

	if( (name==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;
	if( entity->info->type!=type ) 
		return ERR_9006;

	wlaninst = getWLANConfInstNum( name );
	if( wlaninst<1 || wlaninst>MaxWLANIface )	
		return ERR_9007;
		
	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else if(vwlanIdx<NUM_VWLAN+1)
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);
	else
		sprintf(wlan_ifname,"wlan%d-vxd",rootIdx);	

	reginst = getWPSRegistrarInstNum( name );	
//	printf("[%s %d]wlaninst:%d, reginst:%d, lastname:%s\n", __FUNCTION__,__LINE__,wlaninst, reginst, lastname);	
	loadWLANExternalRegByInstNum(wlaninst);	
	
	if( strcmp( lastname, "Enable" )==0 ) 
	{	
		if(getWLAN_WSC_ER_INFO(reginst-1, &extReg)<0){		
			isWLANMIBUpdated=0;
			printf("the certain external registara not exist...\n");
			return ERR_9007; 
		}else {
			int *i = data; 
			if(i==NULL)
				return ERR_9007; 

			setvalue = (*i?0:1);
			if(setWLAN_WSC_ER_INFO(wlaninst, reginst, setvalue)<0){
				isWLANMIBUpdated = 0;
				printf("change the certain entry error...\n");
				return ERR_9007;
			}else{
				isWLANMIBUpdated=1;
				printf("change ER enable status success !\n");
			}			
		}		
	}
	else {
		return ERR_9005;
	}
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_WLAN, CWMP_RESTART, 0, NULL, 0 );
	return 0;
#else
		if(isWLANMIBUpdated)
			return 1;
		else
			return 0;
#endif	
}
#endif

int loadWLANAssInfoByInstNum( unsigned int instnum )
{
	char filename[32];
	FILE *fp=NULL;
	int  found=0;
	CWMP_WLANCONF_T  wlanConf;
	int rootIdx, vwlanIdx;
	char wlan_ifname[10]={0};
	
	if( instnum==0 || instnum>MaxWLANIface ) 
		return -1;

	if( updateWLANAssociations()< 0 )
	{
		gWLANIDForAssInfo = -1;
		memset( gWLANAssociations, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1) );
		gWLANTotalClients=0;
		return -1;
	}
#ifdef CONFIG_USER_CWMP_WITH_TR181
	extern int _getWLANIdxFromInstNum(int instnum, int *rootIdx, int *vwlan_idx);
	unsigned int instnum1;
	if( instnum<1 || instnum>10 )	return ERR_9007;

	if (instnum > 5) {
		instnum1 = instnum - 5;
		_getWLANIdxFromInstNum(instnum1, &rootIdx, &vwlanIdx);
	}
	else {
		_getWLANIdxFromInstNum(instnum, &rootIdx, &vwlanIdx);
	}

	if (instnum > 5)	rootIdx = 1;

	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);

	//CWMPDBG( 1, ( stderr, "<%s:%d>wlanIface=%s %d %d instnum %d\n", __FUNCTION__, __LINE__,wlan_ifname,rootIdx,vwlanIdx,instnum ) );
#else
	getWLANIdxFromInstNum(instnum, &wlanConf, &rootIdx, &vwlanIdx);

	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);   
#endif
	gWLANIDForAssInfo = -1;
	memset( gWLANAssociations, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1) );
	memset( gWLANKeyInfo, 0, 64 * (MAX_STA_NUM+1) );	
	gWLANTotalClients=0;

	
	sprintf( filename, "%s.%s", WLANASSFILE, wlan_ifname );
	fp=fopen( filename, "rb" );
	if(fp)
	{
		int i;
		WLAN_STA_INFO_T *pInfo;
		int count=0;
		char buf[64];
		
		fread( gWLANAssociations,  1, sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1), fp );
		//dumpInfo(0, gWLANAssociations,  sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM+1) );
		
		while(!feof(fp)){
			if(fread( buf,  1, 64, fp )==64){
				memcpy(&gWLANKeyInfo[64*count], buf, 64);
				count++;
			//	printf("[%s %d]cnt:%d\n", __FUNCTION__,__LINE__,count);
			}
		}
		fclose(fp);
		
		for (i=1; i<=MAX_STA_NUM; i++)
		{
			pInfo = (WLAN_STA_INFO_T*)&gWLANAssociations[i*sizeof(WLAN_STA_INFO_T)];
			//if(pInfo->aid)
			//	printf("[%s %d]aid:%d, flag:0x%x\n", __FUNCTION__,__LINE__,pInfo->aid, pInfo->flag);
			if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)){
				found++;
			//	printf("[%s %d]found:%d\n", __FUNCTION__,__LINE__,found);
			}
		}
		
		gWLANTotalClients = found;		
	}
	
	return 0;
}

int getWLANSTAINFO(int id, WLAN_STA_INFO_T *info)
{
	WLAN_STA_INFO_T* pInfo;
	int found=-1, i;
	//id starts from 0,1,2...
	if( (id<0) || (id>=gWLANTotalClients) || (info==NULL) ) return -1;
	
	for (i=1; i<=MAX_STA_NUM; i++)
	{
		pInfo = (WLAN_STA_INFO_T*)&gWLANAssociations[i*sizeof(WLAN_STA_INFO_T)];
		if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC))
		{
			found++;
			if(found==id) break;
		}
	}
	if( i>MAX_STA_NUM ) return -1;
	
	memcpy( info, pInfo, sizeof(WLAN_STA_INFO_T) );
	return 0;
}

int loadWLANExternalRegByInstNum( unsigned int instnum)
{
	CWMP_WLANCONF_T  wlanConf;
	int rootIdx, vwlanIdx, i;
	char wlan_ifname[10]={0};
	unsigned int entryNum = 0;
	WSC_ER_Tp pinfo = NULL;	
	WSC_ER_T extReg;
	
	if( instnum==0 || instnum>MaxWLANIface ) 
		return -1;
	
#ifdef CONFIG_USER_CWMP_WITH_TR181
	extern int _getWLANIdxFromInstNum(int instnum, int *rootIdx, int *vwlan_idx);
	unsigned int instnum1;
	if( instnum<1 || instnum>10 )	return ERR_9007;

	if (instnum > 5) {
		instnum1 = instnum - 5;
		_getWLANIdxFromInstNum(instnum1, &rootIdx, &vwlanIdx);
	}
	else {
		_getWLANIdxFromInstNum(instnum, &rootIdx, &vwlanIdx);
	}

	if (instnum > 5)	rootIdx = 1;

	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);	
#else
	getWLANIdxFromInstNum(instnum, &wlanConf, &rootIdx, &vwlanIdx);
	if(vwlanIdx == 0)
		sprintf(wlan_ifname,"wlan%d",rootIdx);
	else
		sprintf(wlan_ifname,"wlan%d-va%d",rootIdx,vwlanIdx-1);   
#endif

	memset(gWLANExternalRegs, 0, sizeof(WSC_ER_T) * MAX_EXTERNAL_REGISTRAR_NUM);	
	getWlanMib(rootIdx, 0, MIB_WLAN_WSC_ER_NUM, (void *)&entryNum);
	gWLANTotalERNumbers = entryNum;
	//printf("[%s %d]entryNum[%d], sizeof(WSC_ER_T):%d\n", __FUNCTION__, __LINE__, entryNum, sizeof(WSC_ER_T));
	
	for(i=1;i<=entryNum;i++)
	{
		memset(&extReg, 0, sizeof(extReg));
		*((char *)&extReg) = (char)i;
		getWlanMib(rootIdx, 0, MIB_WLAN_WSC_ER_TBL, (void *)&extReg);
		//printf("[%s %d]extReg[%d]:%s\n", __FUNCTION__, __LINE__, i-1, (char *)&extReg);
		memcpy(&gWLANExternalRegs[(i-1)*sizeof(WSC_ER_T)], &extReg, sizeof(WSC_ER_T));	
		//printf("[%s %d]gWLANExternalRegs:%s\n", __FUNCTION__, __LINE__, gWLANExternalRegs);		
	}	
	return 0;
}


/* find the certain external registrar by 'id' */
/* id starts from 0,1,2... */
int getWLAN_WSC_ER_INFO(int id, WSC_ER_Tp info)
{
	//printf("[%s %d]reg id:%d\n", __FUNCTION__,__LINE__,id);
	WSC_ER_Tp pInfo;
	int count=-1, i;	
	int found = 0;
	
	if( (id<0) || (id>=gWLANTotalERNumbers) || (info==NULL) ) {
		printf("[%s %d]return !!!!\n", __FUNCTION__,__LINE__);
		return -1;
	}

	//printf("[%s %d]gWLANExternalRegs:%s\n", __FUNCTION__, __LINE__, gWLANExternalRegs);
	for (i=0; i<MAX_EXTERNAL_REGISTRAR_NUM; i++)
	{
		pInfo = (WSC_ER_T *)&gWLANExternalRegs[i*sizeof(WSC_ER_T)];
		//printf("[%s %d]id:%d, disable:%d\n", __FUNCTION__,__LINE__,i, pInfo->wscERDisable);
		//printf("[%s %d]id:%d, uuid:%s\n", __FUNCTION__,__LINE__,i,pInfo->wscERUUID);
		//printf("[%s %d]id:%d, devname:%s\n", __FUNCTION__,__LINE__,i,pInfo->wscERDeviceName);
		if(strcmp(pInfo->wscERUUID, "00000000000000000000000000000000")!=0)
		{
			count++;
			if(count==id){
				found = 1;
				break;		
			}
		}	
	}
	
	if( i>MAX_EXTERNAL_REGISTRAR_NUM ) {
		return -1;
	}
	
	if(!found){
		return -1;
	}
	
	memcpy( info, pInfo, sizeof(WSC_ER_T) );
	//printf("[%s %d]found:%d, info:%s\n", __FUNCTION__,__LINE__,found, (char *)info);
	return 0;
}

/* reginst: 1,2,3... */
int setWLAN_WSC_ER_INFO(int wlaninst, int reginst, int setvalue)
{	
	WSC_ER_T extReg;
	int found=-1, i;	
	CWMP_WLANCONF_T  wlanConf;
	int rootIdx, vwlanIdx;
	
	if( wlaninst==0 || wlaninst>MaxWLANIface ) 
		return -1;
	
	if( (reginst<=0) || (reginst>gWLANTotalERNumbers) ) 
		return -1;

	getWLANIdxFromInstNum(wlaninst, &wlanConf, &rootIdx, &vwlanIdx);
	
	//find the certain entry	
	memset(&extReg, 0x00, sizeof(extReg));
	*((char *)&extReg) = (char)reginst; // ...
	getWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_ER_TBL, (void *)&extReg);	
	//printf("[%s %d]reg id:%d, extReg:%s\n", __FUNCTION__,__LINE__,reginst, (char *)&extReg);
	//printf("[%s %d]reg id:%d, disable:%d\n", __FUNCTION__,__LINE__,reginst, extReg.wscERDisable);
	//printf("[%s %d]reg id:%d, uuid:%s\n", __FUNCTION__,__LINE__,reginst, extReg.wscERUUID);
	//printf("[%s %d]reg id:%d, devname:%s\n", __FUNCTION__,__LINE__,reginst, extReg.wscERDeviceName);
	
	setWlanMib(rootIdx, vwlanIdx, MIB_WLAN_WSC_ER_DEL, (void *)&extReg); // 5g for dual band, 2g for single band
#ifdef FOR_DUAL_BAND
	if(rootIdx==0)
		setWlanMib(1, vwlanIdx, MIB_WLAN_WSC_ER_DEL, (void *)&extReg); // 2g for dual band
	else
		setWlanMib(0, vwlanIdx, MIB_WLAN_WSC_ER_DEL, (void *)&extReg); // 2g for dual band
#endif

	extReg.wscERDisable = setvalue;	
	setWlanMib(rootIdx, vwlanIdx,  MIB_WLAN_WSC_ER_ADD, (void *)&extReg);	
#ifdef FOR_DUAL_BAND
	if(rootIdx==0)
		setWlanMib(1, vwlanIdx, MIB_WLAN_WSC_ER_ADD, (void *)&extReg);
	else
		setWlanMib(0, vwlanIdx, MIB_WLAN_WSC_ER_ADD, (void *)&extReg);
#endif

	return 0;
}

/*copy from mib.c, because it defines with "static" */
static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

static int string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}

int getRateStr( unsigned short rate, char *buf )
{
	int len;
	if( buf==NULL ) return -1;

	buf[0]=0;
	if( rate & TX_RATE_1M )
		strcat( buf, "1," );
	if( rate & TX_RATE_2M )
		strcat( buf, "2," );
	if( rate & TX_RATE_5M )
		strcat( buf, "5.5," );
	if( rate & TX_RATE_11M )
		strcat( buf, "11," );
	if( rate & TX_RATE_6M )
		strcat( buf, "6," );
	if( rate & TX_RATE_9M )
		strcat( buf, "9," );
	if( rate & TX_RATE_12M )
		strcat( buf, "12," );
	if( rate & TX_RATE_18M )
		strcat( buf, "18," );
	if( rate & TX_RATE_24M )
		strcat( buf, "24," );
	if( rate & TX_RATE_36M )
		strcat( buf, "36," );
	if( rate & TX_RATE_48M )
		strcat( buf, "48," );
	if( rate & TX_RATE_54M )
		strcat( buf, "54," );
		
	len = strlen(buf);
	if( len>1 )
		buf[len-1]=0;
	return 0;
}

int setRateStr( char *buf, unsigned short *rate )
{
	if( (rate!=NULL) && (buf!=NULL) )
	{
		char *tok;
		
		*rate=0;
		tok = strtok(buf,", \n\r");
		while(tok)
		{
			if( strcmp( tok, "1" )==0 )
				*rate = *rate | TX_RATE_1M;
			else if( strcmp( tok, "2" )==0 )
				*rate = *rate | TX_RATE_2M;
			else if( strcmp( tok, "5.5" )==0 )
				*rate = *rate | TX_RATE_5M;
			else if( strcmp( tok, "11" )==0 )
				*rate = *rate | TX_RATE_11M;
			else if( strcmp( tok, "6" )==0 )
				*rate = *rate | TX_RATE_6M;
			else if( strcmp( tok, "9" )==0 )
				*rate = *rate | TX_RATE_9M;
			else if( strcmp( tok, "12" )==0 )
				*rate = *rate | TX_RATE_12M;
			else if( strcmp( tok, "18" )==0 )
				*rate = *rate | TX_RATE_18M;
			else if( strcmp( tok, "24" )==0 )
				*rate = *rate | TX_RATE_24M;
			else if( strcmp( tok, "36" )==0 )
				*rate = *rate | TX_RATE_36M;
			else if( strcmp( tok, "48" )==0 )
				*rate = *rate | TX_RATE_48M;
			else if( strcmp( tok, "54" )==0 )
				*rate = *rate | TX_RATE_54M;
			else{
				*rate=0;
				return -1;
			}
							
			tok = strtok(NULL,", \n\r");
		}		
		return 0;
	}
	
	return -1;
}

int setRateStr_Basic( char *buf, unsigned short *rate )
{
	if( (rate!=NULL) && (buf!=NULL) )
	{
		char *tok;
		
		*rate=0;
		tok = strtok(buf,", \n\r");
		while(tok)
		{
			if( strcmp( tok, "1" )==0 )
				*rate = *rate | TX_RATE_1M;
			else if( strcmp( tok, "2" )==0 )
				*rate = *rate | TX_RATE_2M;
			else if( strcmp( tok, "5.5" )==0 )
				*rate = *rate | TX_RATE_5M;
			else if( strcmp( tok, "11" )==0 )
				*rate = *rate | TX_RATE_11M;			
			else{
				*rate=0;
				return -1;
			}							
			tok = strtok(NULL,", \n\r");
		}		
		return 0;
	}	
	return -1;
}

int setWPSCfgMethod( char *buf, unsigned short *method )
{
	if( (method!=NULL) && (buf!=NULL) )
	{
		char *tok;		
		*method=0;
		tok = strtok(buf,", \n\r");
		printf("==== [%s-%d]tok(%s)\n",__FUNCTION__,__LINE__,tok);
		while(tok)
		{
			if( strcmp( tok, "Label" )==0 ) // pin
				*method = *method | 0x1;
			else if( strcmp( tok, "PushButton" )==0 )	// pbc
				*method = *method | 0x2;			
			else {
				*method=0;
				return -1;
			}							
			tok = strtok(NULL,", \n\r");
			printf("==== [%s-%d]tok(%s)\n",__FUNCTION__,__LINE__,tok);
		}		
		return 0;
	}	
	return -1;
}

int getIPbyMAC( char *mac, char *ip )
{
	int	ret=-1;
	FILE 	*fh;
	char 	buf[128];

	if( (mac==NULL) || (ip==NULL) )	return ret;
	ip[0]=0;
	
	fh = fopen("/proc/net/arp", "r");
	if (!fh) return ret;

	fgets(buf, sizeof buf, fh);	/* eat line */
	//fprintf( stderr, "%s\n", buf );
	while (fgets(buf, sizeof buf, fh))
	{
		char cip[32],cmac[32];
		
		//fprintf( stderr, "%s\n", buf );
		//format: IP address       HW type     Flags       HW address            Mask     Device
		if( sscanf(buf,"%s %*s %*s %s %*s %*s", cip,cmac)!=2 )
			continue;

		if( strcasecmp( mac, cmac )==0 )
		{
			strcpy( ip, cip );
			ret=0;
			break;
		}
	}
	fclose(fh);
	return ret;
}
#endif /*#ifdef WLAN_SUPPORT*/
