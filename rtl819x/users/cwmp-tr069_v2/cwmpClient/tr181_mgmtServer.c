#include "cwmp_notify.h"
#include "cwmp_rpc.h"
#include "prmt_utility.h"
#include "tr181_mgmtServer.h"
#include "tr181_mgableDev.h"

#ifdef TR069_ANNEX_G
extern int stunState;
extern int gRestartStun;
extern int OldStunState;
extern pthread_cond_t cond;
extern pthread_mutex_t mutex;
#endif

/*******************************************************************************
DEVICE.ManagementServer Parameters
*******************************************************************************/
struct CWMP_OP tMgmtServerLeafOP = { getMgmtServer,setMgmtServer };
struct CWMP_PRMT tMgmtServerLeafInfo[] =
{
	/*(name,				type,		flag,			op)*/
	{"EnableCWMP",                        eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP}, // factory default true
	{"URL",                               eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"Username",                          eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"Password",                          eCWMP_tSTRING,	CWMP_PASSWORD|CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"PeriodicInformEnable",              eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"PeriodicInformInterval",            eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"PeriodicInformTime",                eCWMP_tDATETIME,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"ParameterKey",                      eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,	&tMgmtServerLeafOP},
	{"ConnectionRequestURL",              eCWMP_tSTRING,	CWMP_READ|CWMP_FORCE_ACT,	&tMgmtServerLeafOP},
	{"ConnectionRequestUsername",         eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"ConnectionRequestPassword",         eCWMP_tSTRING,	CWMP_PASSWORD|CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"UpgradesManaged",                   eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP}, // set problem
	{"KickURL",                           eCWMP_tSTRING,	CWMP_READ,	&tMgmtServerLeafOP},
	{"DownloadProgressURL",               eCWMP_tSTRING,	CWMP_READ,	&tMgmtServerLeafOP},
//	{"DefaultActiveNotificationThrottle", eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"CWMPRetryMinimumWaitInterval",      eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"CWMPRetryIntervalMultiplier",       eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
#ifdef TR069_ANNEX_G
	{"UDPConnectionRequestAddress",       eCWMP_tSTRING,	CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNEnable",                        eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNServerAddress",                 eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNServerPort",                    eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNUsername",                      eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNPassword",                      eCWMP_tSTRING,	CWMP_PASSWORD|CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNMaximumKeepAlivePeriod",        eCWMP_tINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"STUNMinimumKeepAlivePeriod",        eCWMP_tUINT,		CWMP_WRITE|CWMP_READ,	&tMgmtServerLeafOP},
	{"NATDetected",                       eCWMP_tBOOLEAN,	CWMP_READ,	&tMgmtServerLeafOP},
#endif
	{"ManageableDeviceNumberOfEntries",   eCWMP_tUINT,		CWMP_READ,	&tMgmtServerLeafOP},
};

enum eMgmtServerLeaf
{
	eMS_EnableCWMP,
	eMS_URL,
	eMS_Username,
	eMS_Password,
	eMS_PeriodicInformEnable,
	eMS_PeriodicInformInterval,
	eMS_PeriodicInformTime,
	eMS_ParameterKey,
	eMS_ConnectionRequestURL,
	eMS_ConnectionRequestUsername,
	eMS_ConnectionRequestPassword,
	eMS_UpgradesManaged,
	eMS_KickURL,
	eMS_DownloadProgressURL,
//	eMS_DefaultActiveNotificationThrottle,
	eMS_CWMPRetryMinimumWaitInterval,
	eMS_CWMPRetryIntervalMultiplier,
#ifdef TR069_ANNEX_G
	eMS_UDPConnectionRequestAddress,
	eMS_STUNEnable,
	eMS_STUNServerAddress,
	eMS_STUNServerPort,
	eMS_STUNUsername,
	eMS_STUNPassword,
	eMS_STUNMaximumKeepAlivePeriod,
	eMS_STUNMinimumKeepAlivePeriod,
	eMS_NATDetected,
#endif
	eMS_ManageableDeviceNumberOfEntries
};

struct CWMP_LEAF tMgmtServerLeaf[] =
{
	{ &tMgmtServerLeafInfo[eMS_EnableCWMP] },
	{ &tMgmtServerLeafInfo[eMS_URL] },
	{ &tMgmtServerLeafInfo[eMS_Username] },
	{ &tMgmtServerLeafInfo[eMS_Password] },
	{ &tMgmtServerLeafInfo[eMS_PeriodicInformEnable] },
	{ &tMgmtServerLeafInfo[eMS_PeriodicInformInterval] },
	{ &tMgmtServerLeafInfo[eMS_PeriodicInformTime] },
	{ &tMgmtServerLeafInfo[eMS_ParameterKey] },
	{ &tMgmtServerLeafInfo[eMS_ConnectionRequestURL] },
	{ &tMgmtServerLeafInfo[eMS_ConnectionRequestUsername] },
	{ &tMgmtServerLeafInfo[eMS_ConnectionRequestPassword] },
	{ &tMgmtServerLeafInfo[eMS_UpgradesManaged] },
	{ &tMgmtServerLeafInfo[eMS_KickURL] },
	{ &tMgmtServerLeafInfo[eMS_DownloadProgressURL] },
//	{ &tMgmtServerLeafInfo[eMS_DefaultActiveNotificationThrottle] },
	{ &tMgmtServerLeafInfo[eMS_CWMPRetryMinimumWaitInterval] },
	{ &tMgmtServerLeafInfo[eMS_CWMPRetryIntervalMultiplier] },
#ifdef TR069_ANNEX_G
	{ &tMgmtServerLeafInfo[eMS_UDPConnectionRequestAddress] },
	{ &tMgmtServerLeafInfo[eMS_STUNEnable] },
	{ &tMgmtServerLeafInfo[eMS_STUNServerAddress] },
	{ &tMgmtServerLeafInfo[eMS_STUNServerPort] },
	{ &tMgmtServerLeafInfo[eMS_STUNUsername] },
	{ &tMgmtServerLeafInfo[eMS_STUNPassword] },
	{ &tMgmtServerLeafInfo[eMS_STUNMaximumKeepAlivePeriod] },
	{ &tMgmtServerLeafInfo[eMS_STUNMinimumKeepAlivePeriod] },
	{ &tMgmtServerLeafInfo[eMS_NATDetected] },
#endif
	{ &tMgmtServerLeafInfo[eMS_ManageableDeviceNumberOfEntries] },
	{ NULL	}
};


int getMgmtServer(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	unsigned char buf[256+1]={0};
	unsigned char ch=0;
	unsigned int  in=0;
	int           in1=0;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	//notify_set_attributes( "Device.GatewayInfo.ManufacturerOUI", CWMP_NTF_FORCED|CWMP_NTF_ACT, CWMP_ACS_MASK );
	//notify_set_attributes( "Device.GatewayInfo.ProductClass", CWMP_NTF_FORCED|CWMP_NTF_ACT, CWMP_ACS_MASK );
	//notify_set_attributes( "Device.GatewayInfo.SerialNumber", CWMP_NTF_FORCED|CWMP_NTF_ACT, CWMP_ACS_MASK );

	*type = entity->info->type;
	*data = NULL;
	switch(getIndexOf(tMgmtServerLeaf, entity->info->name)) {
	case eMS_URL:
		CONFIG_GET(MIB_CWMP_ACS_URL, buf);
		*data = strdup( buf );
		break;
	case eMS_Username:
		CONFIG_GET(MIB_CWMP_ACS_USERNAME, buf);
		*data = strdup(buf);
		break;
	case eMS_Password:
#if DEBUG
		CONFIG_GET(MIB_CWMP_ACS_PASSWORD, buf);
		*data = strdup(buf);
#else
		*data = strdup("");
#endif
		break;
	case eMS_PeriodicInformEnable:
		CONFIG_GET(MIB_CWMP_INFORM_ENABLE, &in);
		*data = booldup(in);
		break;	
	case eMS_PeriodicInformInterval:
		CONFIG_GET(MIB_CWMP_INFORM_INTERVAL, &in);
		*data = uintdup(in);
		break;	
	case eMS_PeriodicInformTime:
		CONFIG_GET(MIB_CWMP_INFORM_TIME, &in);
		*data = timedup(in);
		break;	
	case eMS_ParameterKey: // ParameterKey
		{
			unsigned char gParameterKey[32+1];
			CONFIG_GET(MIB_CWMP_PARAMETERKEY,gParameterKey);
			*data = strdup(gParameterKey);
		break;	
		}
	case eMS_ConnectionRequestURL:
		if (MgmtSrvGetConReqURL(buf, 256))			
			*data = strdup(buf);
		else
			*data = strdup("");
		break;	
	case eMS_ConnectionRequestUsername:
		CONFIG_GET(MIB_CWMP_CONREQ_USERNAME, buf);
		*data = strdup(buf);
		break;	
	case eMS_ConnectionRequestPassword:
	#if DEBUG
		CONFIG_GET(MIB_CWMP_CONREQ_PASSWORD, buf);
		*data = strdup(buf);
	#else
		*data = strdup("");
	#endif
		break;	
	case eMS_UpgradesManaged:
		CONFIG_GET(MIB_CWMP_ACS_UPGRADESMANAGED, &ch);
		*data = booldup(ch);
		break;
	case eMS_KickURL:
		CONFIG_GET(MIB_CWMP_ACS_KICKURL, buf);
		*data = strdup(buf);
		break;	
	case eMS_DownloadProgressURL:
		CONFIG_GET(MIB_CWMP_ACS_DOWNLOADURL, buf);
		*data = strdup(buf);
		break;				

	case eMS_CWMPRetryMinimumWaitInterval:
		CONFIG_GET(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL, &in);
		*data = uintdup(in);
		break;

	case eMS_CWMPRetryIntervalMultiplier:
		CONFIG_GET(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER, &in);
		*data = uintdup(in);
		break;

#ifdef TR069_ANNEX_G
	case eMS_UDPConnectionRequestAddress:
		CONFIG_GET(MIB_CWMP_UDP_CONN_REQ_ADDR, buf);
		*data = strdup(buf);
		break;
		
	case eMS_STUNEnable:
		CONFIG_GET(MIB_CWMP_STUN_EN, &in);
		*data = booldup(in);
		break;
		
	case eMS_STUNServerAddress:
		CONFIG_GET(MIB_CWMP_STUN_SERVER_ADDR, buf);
		*data = strdup(buf);
		break;
		
	case eMS_STUNServerPort:
		CONFIG_GET(MIB_CWMP_STUN_SERVER_PORT, &in);
		*data = uintdup(in);
		break;
		
	case eMS_STUNUsername:
		CONFIG_GET(MIB_CWMP_STUN_USERNAME, buf);
		*data = strdup(buf);
		break;
		
	case eMS_STUNPassword:
		CONFIG_GET(MIB_CWMP_STUN_PASSWORD, buf);
		*data = strdup(buf);
		break;
		
	case eMS_STUNMaximumKeepAlivePeriod:
		CONFIG_GET(MIB_CWMP_STUN_MAX_KEEP_ALIVE_PERIOD, &in1);
		*data = intdup(in1);
		break;
		
	case eMS_STUNMinimumKeepAlivePeriod:
		CONFIG_GET(MIB_CWMP_STUN_MIN_KEEP_ALIVE_PERIOD, &in);
		*data = uintdup(in);
		break;
		
	case eMS_NATDetected:
		CONFIG_GET(MIB_CWMP_NAT_DETECTED, &in);
		*data = uintdup(in);
		break;
#endif

	case eMS_ManageableDeviceNumberOfEntries:
	{
		FILE *fp;
		int count=0;
		fp=fopen( TR069_ANNEX_F_DEVICE_FILE, "r" );
		
		while( fp && fgets( buf,160,fp ) )
		{
			char *p;
			
			p = strtok( buf, " \n\r" );
			if( p && atoi(p)>0 )
			{
				count++;
			}
		}
		if(fp) fclose(fp);
		gMgableDevNum = count;
		*data = uintdup(gMgableDevNum);
	}
		break;

	default:
		return ERR_9005;
				
	}

	return 0;

}

extern void parse_xml_escaped_str(char *out, char *in);

int setMgmtServer(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char 	*buf=data;
	char 	tmp[256+1]={0};
	int  	len=0;	
	unsigned int *pNum;
	unsigned char byte;
	unsigned int iVal;
	int          *pInt;
	
	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	switch(getIndexOf(tMgmtServerLeaf, entity->info->name)) {
	case eMS_URL:
		CHECK_PARAM_STR(buf, 0, 256+1);
		CONFIG_SET(MIB_CWMP_ACS_URL, buf);
		cwmpSettingChange(MIB_CWMP_ACS_URL);
		break;
	case eMS_Username:
		CHECK_PARAM_STR(buf, 0, 256+1);
		parse_xml_escaped_str(tmp, buf);
		CONFIG_SET(MIB_CWMP_ACS_USERNAME, tmp);
		cwmpSettingChange(MIB_CWMP_ACS_USERNAME);
		//break;
#ifdef MULTI_WAN_SUPPORT
		return 0;
#else
		return 1;
#endif
 // reboot to take effect
	case eMS_Password:
		CHECK_PARAM_STR(buf, 0, 256+1);
		parse_xml_escaped_str(tmp, buf);
		CONFIG_SET(MIB_CWMP_ACS_PASSWORD, tmp);
		cwmpSettingChange(MIB_CWMP_ACS_PASSWORD);
		//break;
#ifdef MULTI_WAN_SUPPORT
		return 0;
#else
		return 1;
#endif
 // reboot to take effect
	case eMS_PeriodicInformEnable:
		pNum = (unsigned int *)data;
		CHECK_PARAM_NUM(*pNum, 0, 1);
		iVal = (*pNum == 0) ? 0 : 1;
		CONFIG_SET(MIB_CWMP_INFORM_ENABLE, &iVal);	
		cwmpSettingChange(MIB_CWMP_INFORM_ENABLE);
		break;	
	case eMS_PeriodicInformInterval:
		pNum = (unsigned int *)data;
		if (*pNum < 1) return ERR_9007;		
		CONFIG_SET(MIB_CWMP_INFORM_INTERVAL, pNum);
		cwmpSettingChange(MIB_CWMP_INFORM_INTERVAL);
		break;	
	case eMS_PeriodicInformTime:
		pNum = (unsigned int *)buf;
		CONFIG_SET(MIB_CWMP_INFORM_TIME, buf);
		cwmpSettingChange(MIB_CWMP_INFORM_TIME);
		break;	

	case eMS_ConnectionRequestUsername:
		CHECK_PARAM_STR(buf, 0, 256+1);
		parse_xml_escaped_str(tmp, buf);
		CONFIG_SET(MIB_CWMP_CONREQ_USERNAME, tmp);
		cwmpSettingChange(MIB_CWMP_CONREQ_USERNAME);
		break;	
	case eMS_ConnectionRequestPassword:
		CHECK_PARAM_STR(buf, 0, 256+1);
		parse_xml_escaped_str(tmp, buf);
		CONFIG_SET(MIB_CWMP_CONREQ_PASSWORD, tmp);
		cwmpSettingChange(MIB_CWMP_CONREQ_PASSWORD);
		break;	
	case eMS_UpgradesManaged:
		pNum = (unsigned int *)data;
		CHECK_PARAM_NUM(*pNum, 0, 1);
		byte = (*pNum == 0) ? 0 : 1;
		CONFIG_SET(MIB_CWMP_ACS_UPGRADESMANAGED, &byte);	
		break;
	case eMS_CWMPRetryMinimumWaitInterval:
		pNum = (unsigned int *)data;
		if (*pNum < 1) return ERR_9007;		
		CONFIG_SET(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL, pNum);
		cwmpSettingChange(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL);
		break;	
	case eMS_CWMPRetryIntervalMultiplier:
		pNum = (unsigned int *)data;
		if (*pNum < 1000) return ERR_9007;		
		CONFIG_SET(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER, pNum);
		cwmpSettingChange(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER);
		break;	

#ifdef TR069_ANNEX_G
	case eMS_STUNEnable:
		pNum = (unsigned int *)data;
		CHECK_PARAM_NUM(*pNum, 0, 1);
		iVal = (*pNum == 0) ? 0 : 1;
		CONFIG_SET(MIB_CWMP_STUN_EN, &iVal);	
		gRestartStun = 1;
		break;
		
	case eMS_STUNServerAddress:
		if(stunState != STUN_BINDING_CHANGE)
		{
			CHECK_PARAM_STR(buf, 0, CWMP_STUN_SERVER_ADDR_LEN+1);
			CONFIG_SET(MIB_CWMP_STUN_SERVER_ADDR, buf);
			gRestartStun = 1;
		}
		break;
		
	case eMS_STUNServerPort:
		if(stunState != STUN_BINDING_CHANGE)
		{
			pNum = (unsigned int *)data;
			if ((*pNum < 0) || (*pNum > 65535)) return ERR_9007;		
			CONFIG_SET(MIB_CWMP_STUN_SERVER_PORT, pNum);
			gRestartStun = 1;
		}
		break;
		
	case eMS_STUNUsername:
		CHECK_PARAM_STR(buf, 0, CWMP_STUN_USERNAME_LEN+1);
		CONFIG_SET(MIB_CWMP_STUN_USERNAME, buf);
		pthread_mutex_lock(&mutex);
		stunState = STUN_CHANGE_OTHER_PARAM;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
		
	case eMS_STUNPassword:
		CHECK_PARAM_STR(buf, 0, CWMP_STUN_PASSWORD_LEN+1);
		CONFIG_SET(MIB_CWMP_STUN_PASSWORD, buf);
		pthread_mutex_lock(&mutex);
		stunState = STUN_CHANGE_OTHER_PARAM;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
		
	case eMS_STUNMaximumKeepAlivePeriod:
		pInt = (int *)data;
		if (*pInt < -1) return ERR_9007;	
		CONFIG_SET(MIB_CWMP_STUN_MAX_KEEP_ALIVE_PERIOD, pInt);
		pthread_mutex_lock(&mutex);
		stunState = STUN_CHANGE_OTHER_PARAM;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
		
	case eMS_STUNMinimumKeepAlivePeriod:
		pNum = (unsigned int *)data;
		CONFIG_SET(MIB_CWMP_STUN_MIN_KEEP_ALIVE_PERIOD, pNum);
		pthread_mutex_lock(&mutex);
		stunState = STUN_CHANGE_OTHER_PARAM;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		break;
#endif

	default:
		return ERR_9005;
				
	}

	return 0;


}