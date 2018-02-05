#include "prmt_deviceinfo.h"
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include "prmt_utility.h" //keith add.

#define MANUFACTURER_STR	DEF_MANUFACTURER_STR //"REALTEK SEMICONDUCTOR CORP."
#define MANUFACTUREROUI_STR	DEF_MANUFACTUREROUI_STR //"00E04C"
#define SPECVERSION_STR		"1.0"
#define HWVERSION_STR		"81xx"

#define ADDITIONAL_HW_VER_STR	"AdditionalHardwareVersion"
#define ADDITIONAL_SW_VER_STR	"AdditionalSoftwareVersion"

extern char *fwVersion;	// defined in version.c

extern int icmp_test(char *intf, char *host, unsigned int count, unsigned int timeout, unsigned int datasize, unsigned char tos,
	unsigned int *cntOK, unsigned int *cntFail, unsigned int *timeAvg, unsigned int *timeMin, unsigned int *timeMax, unsigned int needWaitRsp);

/*ping_zhang:20100525 START:CT 1PW Extension*/

struct CWMP_OP tVendorCfgEntityLeafOP = { getVendorCfgEntity, NULL };
struct CWMP_PRMT tVendorCfgEntityLeafInfo[] =
{
/*(name,	type,		flag,		op)*/
{"Name",	eCWMP_tSTRING,	CWMP_READ,	&tVendorCfgEntityLeafOP},
{"Version",	eCWMP_tSTRING,	CWMP_READ,	&tVendorCfgEntityLeafOP},
{"Date",	eCWMP_tDATETIME,CWMP_READ,	&tVendorCfgEntityLeafOP},
{"Description",	eCWMP_tSTRING,	CWMP_READ,	&tVendorCfgEntityLeafOP},
};
enum eVendorCfgEntityLeaf
{
	eVCName,
	eVCVersion,
	eVCDate,
	eDescription
};
struct CWMP_LEAF tVendorCfgEntityLeaf[] =
{
{ &tVendorCfgEntityLeafInfo[eVCName] },
{ &tVendorCfgEntityLeafInfo[eVCVersion] },
{ &tVendorCfgEntityLeafInfo[eVCDate] },
{ &tVendorCfgEntityLeafInfo[eDescription] },
{ NULL }
};


struct CWMP_PRMT tVendorConfigObjectInfo[] =
{
/*(name,	type,		flag,		op)*/
{"1",		eCWMP_tOBJECT,	CWMP_READ,	NULL}
};
enum eVendorConfigObject
{
	eVC1
};
struct CWMP_NODE tVendorConfigObject[] =
{
/*info,  				leaf,			next)*/
{&tVendorConfigObjectInfo[eVC1],	tVendorCfgEntityLeaf, 	NULL},
{NULL, 					NULL, 			NULL}
};


struct CWMP_OP tDeviceInfoLeafOP = { getDeviceInfo, setDeviceInfo };
struct CWMP_PRMT tDeviceInfoLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Manufacturer",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"ManufacturerOUI",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"ModelName",			eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"Description",			eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"ProductClass",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"SerialNumber",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"HardwareVersion",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"SoftwareVersion",		eCWMP_tSTRING,	CWMP_READ|CWMP_FORCE_ACT,		&tDeviceInfoLeafOP},
{"EnabledOptions",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"AdditionalHardwareVersion",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"AdditionalSoftwareVersion",		eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"SpecVersion",			eCWMP_tSTRING,	CWMP_READ,		&tDeviceInfoLeafOP},
{"ProvisioningCode",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ|CWMP_FORCE_ACT,	&tDeviceInfoLeafOP},
{"UpTime",			eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tDeviceInfoLeafOP},
{"FirstUseDate",		eCWMP_tDATETIME,CWMP_READ,		&tDeviceInfoLeafOP},
{"DeviceLog",			eCWMP_tSTRING,	CWMP_READ|CWMP_DENY_ACT,&tDeviceInfoLeafOP},
{"VendorConfigFileNumberOfEntries",eCWMP_tUINT,	CWMP_READ,		&tDeviceInfoLeafOP}
};
enum eDeviceInfoLeaf
{
	eDIManufacturer,
	eDIManufacturerOUI,
	eDIModelName,
	eDIDescription,
	eDIProductClass,
	eDISerialNumber,
	eDIHardwareVersion,
	eDISoftwareVersion,
 	eDIEnabledOptions,
	eDIAdditionalHardwareVersion,
	eDIAdditionalSoftwareVersion,
	eDISpecVersion,
	eDIProvisioningCode,
	eDIUpTime,
	eDIFirstUseDate,
	eDIDeviceLog,
	eDIVendorConfigFileNumberOfEntries
};
struct CWMP_LEAF tDeviceInfoLeaf[] =
{
{ &tDeviceInfoLeafInfo[eDIManufacturer] },
{ &tDeviceInfoLeafInfo[eDIManufacturerOUI] },
{ &tDeviceInfoLeafInfo[eDIModelName] },
{ &tDeviceInfoLeafInfo[eDIDescription] },
{ &tDeviceInfoLeafInfo[eDIProductClass] },
{ &tDeviceInfoLeafInfo[eDISerialNumber] },
{ &tDeviceInfoLeafInfo[eDIHardwareVersion] },
{ &tDeviceInfoLeafInfo[eDISoftwareVersion] },
{ &tDeviceInfoLeafInfo[eDIEnabledOptions] },
{ &tDeviceInfoLeafInfo[eDIAdditionalHardwareVersion] },
{ &tDeviceInfoLeafInfo[eDIAdditionalSoftwareVersion] },
{ &tDeviceInfoLeafInfo[eDISpecVersion] },
{ &tDeviceInfoLeafInfo[eDIProvisioningCode] },
{ &tDeviceInfoLeafInfo[eDIUpTime] },
{ &tDeviceInfoLeafInfo[eDIFirstUseDate] },
{ &tDeviceInfoLeafInfo[eDIDeviceLog] },
{ &tDeviceInfoLeafInfo[eDIVendorConfigFileNumberOfEntries] },
{ NULL	}
};
struct CWMP_PRMT tDeviceInfoObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"VendorConfigFile",		eCWMP_tOBJECT,	CWMP_READ,		NULL},
};
enum eDeviceInfoObject
{
	eDIVendorConfigFile,
		/*ping_zhang:20100525 START:CT 1PW Extension*/
};
struct CWMP_NODE tDeviceInfoObject[] =
{
/*info,  						leaf,		next)*/
{ &tDeviceInfoObjectInfo[eDIVendorConfigFile],		NULL,		tVendorConfigObject },
{ NULL,							NULL,		NULL }
};


#ifdef _PRMT_DEVICECONFIG_
struct CWMP_OP tDeviceConfigLeafOP = { getDeviceConfig, setDeviceConfig };
struct CWMP_PRMT tDeviceConfigLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"PersistentData",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tDeviceConfigLeafOP},
{"ConfigFile",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tDeviceConfigLeafOP}
};
enum eDeviceConfigLeaf
{
	eDCPersistentData,
	eDCConfigFile
};
struct CWMP_LEAF tDeviceConfigLeaf[] =
{
{ &tDeviceConfigLeafInfo[eDCPersistentData] },
{ &tDeviceConfigLeafInfo[eDCConfigFile] },
{ NULL	}
};
#endif //_PRMT_DEVICECONFIG_

int getVendorCfgEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned char buf[256]={0};
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Name" )==0 )
	{
		*data = strdup( "config.dat" );
	}else if( strcmp( lastname, "Version" )==0 )
	{
		*data = strdup( fwVersion );
	}else if( strcmp( lastname, "Date" )==0 )
	{
		*data = timedup( 0 );//unknown time
	}else if( strcmp( lastname, "Description" )==0 )
	{
		*data = strdup( "" ); 
	}else{
		return ERR_9005;
	}
	
	return 0;
}


int getDeviceInfo(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned char buf[256]={0};
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Manufacturer" )==0 )
	{
		*data = strdup( MANUFACTURER_STR );
	}else if( strcmp( lastname, "ManufacturerOUI" )==0 )
	{
		*data = strdup( MANUFACTUREROUI_STR );
	}else if( strcmp( lastname, "ModelName" )==0 )
	{
		//mib_get( MIB_SNMP_SYS_NAME, (void *)buf);
		*data = strdup( "ModelName" );
	}else if( strcmp( lastname, "Description" )==0 )
	{
		//mib_get( MIB_SNMP_SYS_DESCR, (void *)buf);
		*data = strdup( "Description" );
	}else if( strcmp( lastname, "ProductClass" )==0 )
	{
		*data = strdup( DEF_PRODUCTCLASS_STR ); //"IGD"
	}else if( strcmp( lastname, "SerialNumber" )==0 )
	{
		char tmpBuff[512];
#ifdef CONFIG_BOA_WEB_E8B_CH
		mib_get( MIB_CWMP_SERIALNUMBER, (void *)tmpBuff);
		strcpy(buf,tmpBuff);
		if(!buf[0])
#endif
{
		mib_get(MIB_HW_NIC1_ADDR,  (void *)tmpBuff);

		sprintf(buf, "%02x%02x%02x%02x%02x%02x", (unsigned char)tmpBuff[0], (unsigned char)tmpBuff[1], 
			(unsigned char)tmpBuff[2], (unsigned char)tmpBuff[3], (unsigned char)tmpBuff[4], (unsigned char)tmpBuff[5]);
}
		
		*data = strdup( buf );
	}else if( strcmp( lastname, "HardwareVersion" )==0 )
	{
		*data = strdup( HWVERSION_STR );
	}else if( strcmp( lastname, "SoftwareVersion" )==0 )
	{
		*data = strdup( fwVersion );
	}
 	else if( strcmp( lastname, "EnabledOptions" )==0 )
	{
		*data = strdup( "" );
	}else if( strcmp( lastname, "AdditionalHardwareVersion" )==0 )
	{
		*data = strdup( ADDITIONAL_HW_VER_STR );
	}else if( strcmp( lastname, "AdditionalSoftwareVersion" )==0 )
	{
		*data = strdup( ADDITIONAL_SW_VER_STR );
	}else if( strcmp( lastname, "SpecVersion" )==0 )
	{
		*data = strdup( SPECVERSION_STR );
	}else if( strcmp( lastname, "ProvisioningCode" )==0 )
	{
		mib_get( MIB_CWMP_PROVISIONINGCODE, (void *)buf);
		*data = strdup( buf );	
	}else if( strcmp( lastname, "UpTime" )==0 )
	{
		struct sysinfo info;
		sysinfo(&info);
		*data = uintdup( info.uptime );
	}else if( strcmp( lastname, "FirstUseDate" )==0 )
	{
		*data = timedup( 0 );
	}else if( strcmp( lastname, "DeviceLog" )==0 )
	{
#if defined(CONFIG_USER_BUSYBOX_SYSLOGD) || defined(SYSLOG_SUPPORT)
		*type = eCWMP_tFILE; /*special case*/
#ifdef CONFIG_BOA_WEB_E8B_CH //NOT Support Yet 20120104
		*data = strdup( "/var/config/syslogd.txt" );
#else
		*data = strdup( "/var/log/messages" );
#endif
#else
		*data = strdup( "" );
#endif //#ifdef CONFIG_USER_BUSYBOX_SYSLOGD
	}else if( strcmp( lastname, "VendorConfigFileNumberOfEntries" )==0 )
	{
		*data = uintdup( 1 );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setDeviceInfo(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char 	*buf=data;
	int  	len=0;	
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;
	
	if( strcmp( lastname, "ProvisioningCode" )==0 )
	{
		if( buf ) len = strlen( buf );
		if( len ==0 )
			mib_set( MIB_CWMP_PROVISIONINGCODE, (void *)"");
		else if( len < 64 )
			mib_set( MIB_CWMP_PROVISIONINGCODE, (void *)buf);
		else
			return ERR_9007;
			
		return 0;
	}else
		return ERR_9005; 
	return 0;
}

#ifdef _PRMT_DEVICECONFIG_
int getDeviceConfig(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char persis_data[256];
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "PersistentData" )==0 )
	{
		if(!mib_get(MIB_CWMP_PERSISTENT_DATA,  (void *)persis_data))
			return ERR_9002;

		*data = strdup(persis_data);
	}else if( strcmp( lastname, "ConfigFile" )==0 )
	{
#ifdef CONFIG_USE_XML
		if( va_cmd("/bin/saveconfig",0,1) )
		{
			fprintf( stderr, "<%s:%d>exec /bin/saveconfig error!\n", __FUNCTION__, __LINE__  );
			return ERR_9002;
		}
		// rename
		rename("/tmp/config.xml", CONFIG_FILE_NAME);
		*type = eCWMP_tFILE; /*special case*/
		*data = strdup(CONFIG_FILE_NAME);
#else
#define CONFIG_FILE "/web/config.dat"
        FILE *fd = fopen(CONFIG_FILE, "r");
        unsigned long filesize;
        struct stat fstat;
        unsigned char *raw;
        unsigned char *result;
        
        if(result==NULL){
            *data = strdup("0");
            return 0;
        }

        stat(CONFIG_FILE, &fstat);
        filesize = (unsigned long)fstat.st_size;
		if(filesize >= 24*1024){//length of base64 is 4/3*filesize (max size of base64 is 32K)	
			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_ERROR, ( "<%s:%d> size of config.dat is larger than 24K!\n", __FUNCTION__, __LINE__ ) );
			*data = strdup("0");
			return;
		}
        raw = malloc(filesize);
        if(raw==NULL){
			CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_ERROR, ( "<%s:%d> malloc failed!\n", __FUNCTION__, __LINE__ ) );
            *data = strdup("0");
            return 0;
        }
		result = malloc(32*1024); // max allowed in spec
		 if(result==NULL){
		 	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_ERROR, ( "<%s:%d> malloc failed!\n", __FUNCTION__, __LINE__ ) );
            *data = strdup("0");
			free(raw);
            return 0;
        }
        memset(raw, 0x0, filesize);
        memset(result, 0x0, 32*1024);
        fread(raw, sizeof(char), filesize, fd);
		 
        base64encode(raw, result, filesize);
        *data = strdup(result);
        free(result);
        free(raw);
        
        if(*data==NULL)
        {
            *data = strdup("0");
            return 0;
        }
#endif	
	}else{
		return ERR_9005;
	}
	
	return 0;
}

extern void parse_xml_escaped_str(char *out, char *in);

int setDeviceConfig(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char 	tmp[256+1]={0};
	
	if( (name==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "PersistentData" )==0 )
	{
		if(strlen(data) >= 256)
			return ERR_9007;
#ifdef CONFIG_USE_XML
		parse_xml_escaped_str(tmp, data);
#else
		snprintf(tmp, sizeof(tmp), "%s", data);
#endif
		if(!mib_set(MIB_CWMP_PERSISTENT_DATA,  (void *)tmp))
			return ERR_9001;
	}else if( strcmp( lastname, "ConfigFile" )==0 )
	{
		char *buf=data;
		int  buflen=0;
		FILE *fp=NULL;
		
		if( buf==NULL ) return ERR_9007;
		buflen = strlen(buf);
		if( buflen==0 ) return ERR_9007;		

        unsigned char *config_data = malloc(32*1024); // max allowed in spec
        int config_len = 0, config_type, config_ret;

        memset(config_data, 0x0, 32*1024);
        config_len = base64decode(config_data, buf, buflen, 0);
        if(config_len<=0)
            return ERR_9007;
        doUpdateConfigIntoFlash(config_data, config_len, &config_type, &config_ret);
		  
        if(config_ret==0 || config_type==0)
            return ERR_9007;
        else
            sleep(2);
        
        return 1; // to take effect		

	}else{
		return ERR_9005;
	}

	return 0;

}
#endif //_PRMT_DEVICECONFIG_
/*ping_zhang:20100525 START:CT 1PW Extension*/
