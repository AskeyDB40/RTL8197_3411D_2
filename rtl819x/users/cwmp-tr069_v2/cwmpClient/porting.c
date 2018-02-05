#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "stdsoap2.h"
#include "prmt_igd.h"
#include "prmt_ippingdiag.h"
#include "prmt_wancondevice.h"
#include "prmt_deviceinfo.h"
#include "prmt_utility.h"
#ifdef _PRMT_TR143_
#include "prmt_tr143.h"
#endif //_PRMT_TR143_
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
#include "prmt_traceroute.h"
#endif //_SUPPORT_TRACEROUTE_PROFILE_
#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
#include "prmt_captiveportal.h"
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
#ifdef SUPPORT_DHCP_RESERVED_IPADDR
#include "prmt_landevice.h"
#endif //SUPPORT_DHCP_RESERVED_IPADDR
#ifdef CONFIG_APP_TR104
#include "cwmp_main_tr104.h"
#endif
#include "cwmp_base.h"
#include "cwmp_core.h"
#include "cwmp_download.h"
#include "cwmp_notify.h"

#define CWMP_HTTP_REALM					"realtek.com.tw"
#ifdef CONFIG_RTL_JFFS2_FILE_PARTITION
#define CONFIG_DIR						"/jffs2/cwmp_config"
#define TRANSFER_FILE					"/jffs2/cwmp_config/transfer.conf"
#define NOTIFICATION_FILE				"/jffs2/cwmp_config/notification.conf"
#else
#define CONFIG_DIR						"/var/cwmp_config"
#define CWMP_CACERT_MTD_NAME			"cwmp cacert"
#endif
/*certificates*/
#define CA_FNAME						CONFIG_DIR"/cacert.pem"
#define CERT_FNAME						CONFIG_DIR"/client.pem"
#define DEF_CA_FN						"/etc/cacert.pem"
#define DEF_CERT_FN						"/etc/client.pem"
/*
 * download/upload
 * MTD data format:
 * |    8 bytes    | 4 bytes | 4 bytes | length bytes |
 *	   Magic Num       Tag	    length     payload
 */
#define CWMP_MTD_TRANSFER_MAGIC_NUM		"deadbeef"
#define CWMP_MTD_NOTIFICATION_MAGIC_NUM "halacwmp"
#define CWMP_MTD_TAG_PLAIN				BIT(1)
#define CWMP_MTD_TAG_ENCRPT				BIT(2)	// reserved
#define CWMP_MTD_OFFSET_MAGIC_NUM		0
#define	CWMP_MTD_SIZE_MAGIC_NUM			8
#define CWMP_MTD_OFFSET_TAG				(CWMP_MTD_OFFSET_MAGIC_NUM + CWMP_MTD_SIZE_MAGIC_NUM)
#define CWMP_MTD_SIZE_TAG				(sizeof(int))
#define CWMP_MTD_OFFSET_LENGTH			(CWMP_MTD_OFFSET_TAG + CWMP_MTD_SIZE_TAG)
#define CWMP_MTD_SIZE_LENGTH			(sizeof(int))
#define CWMP_MTD_OFFSET_PAYLOAD			(CWMP_MTD_OFFSET_LENGTH + CWMP_MTD_SIZE_LENGTH)
#ifdef MTD_NAME_MAPPING
#define CWMP_MTD_TYPE_TRANSFER			(1)
#define CWMP_MTD_TYPE_NOTIFICATION		(2)
#define CWMP_TRANSFER_MTD_NAME			"cwmp transfer"
#define CWMP_NOTIFY_MTD_NAME			"cwmp notification"
extern int rtl_name_to_mtdchar(char* mtd_name,char* buf);
#endif
/*
 * Data model spec forced notification
 * Used in port_notify_init_default(), cwmp__SetParameterAttribtues()
 */
#if defined(CONFIG_USER_CWMP_WITH_TR181)
unsigned char DATAMODLE_PREFIX[] = "Device";
struct cwmp_notify_t DEFAULT_NOTIFY_LIST[] = 
{
{
	"Device.DeviceInfo.HardwareVersion",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
},
{
	"Device.DeviceInfo.SoftwareVersion",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"Device.DeviceInfo.ProvisioningCode",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"Device.ManagementServer.ConnectionRequestURL",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"Device.ManagementServer.ParameterKey",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
}
};
#elif defined(CONFIG_USER_CWMP_WITH_TR098)
unsigned char DATAMODLE_PREFIX[] = "InternetGatewayDevice";
struct cwmp_notify_t DEFAULT_NOTIFY_LIST[] = 
{
{
	"InternetGatewayDevice.DeviceSummary",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.DeviceInfo.SpecVersion",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.DeviceInfo.HardwareVersion",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.DeviceInfo.SoftwareVersion",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.DeviceInfo.ProvisioningCode",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.ManagementServer.ConnectionRequestURL",
	CWMP_NTF_FORCED|CWMP_NTF_ACT, 
	CWMP_ACS_MASK
},
{
	"InternetGatewayDevice.ManagementServer.ParameterKey",
	CWMP_NTF_FORCED|CWMP_NTF_PAS, 
	CWMP_ACS_MASK
}
};
#else
unsigned char DATAMODLE_PREFIX[] = "";
struct cwmp_notify_t *DEFAULT_NOTIFY_LIST = NULL;
#endif


/*********************************************************************/
/* utility */
/*********************************************************************/
int upgradeConfig( char *fwFilename )
{
	unsigned int filelen;
	unsigned int reboot_Wait=0;
	char cmdBuf[100];
	int ret = -1;
	char buff_msg[200];
	int type=0;
	if(fwFilename == NULL)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>Image file not exist\n", __FUNCTION__, __LINE__ ) );
		return -1;
	}
	
	memcpy(&filelen,fwFilename,4); //grap fw length
	fwFilename += 4;

	if (filelen <= 0)
	{		
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>Image file not exist\n", __FUNCTION__, __LINE__ ) );
		return -1;
	}	



	reboot_Wait = 50;

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s:%d>Ready to upgrade the new Config. Config length=%u, reboot_Wait=%u \n", __FUNCTION__, __LINE__,filelen,reboot_Wait  ) );

	
	doUpdateConfigIntoFlash(fwFilename, filelen, &type,&ret);
	
	if (ret == 0 || type == 0) { // checksum error
		ret=-1;
	}else{
		sleep(2);
	}
	return ret;
}

int upgradeFirmware( char *fwFilename )
{
	long filelen;
	int reboot_Wait=0;
	char cmdBuf[100];
	int ret = -1;
	char buff_msg[200];
	
	if(fwFilename == NULL)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>Image file not exist\n", __FUNCTION__, __LINE__ ) );
		return -1;
	}
	
	memcpy(&filelen,fwFilename,4); //grap fw length
	fwFilename += 4;

	if (filelen <= 0)
	{		
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>Image file not exist\n", __FUNCTION__, __LINE__ ) );
		return -1;
	}	

#ifdef CHECK_SETTING
	unlink("/var/log/messages");
	va_cmd("/bin/CreatexmlConfig", 0, 1);
	va_cmd("/bin/cp", 2, 1,  CONFIG_XMLFILE, OLD_SETTING_FILE);
#endif

	reboot_Wait = (filelen/30000);

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s:%d>Ready to upgrade the new firmware. firmware length=%u, reboot_Wait=%u \n", __FUNCTION__, __LINE__,filelen,reboot_Wait  ) );

	//pre-set watch dog time to reboot device whether F/W success or not.	

	if(doFirmwareUpgrade(fwFilename, filelen, 0, buff_msg) == 1) // 1:ok
		ret = 0;

	return ret;
}


/*********************************************************************/
/* porting functions */
/*********************************************************************/
void port_setuppid(void)
{
    FILE *f;
    int tr069_pid;
    char *pidfile = "/var/run/tr069.pid";

    tr069_pid = (int)getpid();  
    if((f = fopen(pidfile, "w")) == NULL)
        return;
    fprintf(f, "%d\n", tr069_pid);
    fclose(f);

}

/*
 * port_init_soap:
 *		init cpe_soap for a session
 * @Return Value
 *		0: failure
 *		1: succeed
 */
int port_init_soap(struct cpe_machine *m)
{
	if(m==NULL)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "m(%p) is NULL!!\n", m));
		return 0;
	}
	
	struct cwmp_userdata *ud = m->cpe_user;
#ifdef CWMP_DYNAMIC_SERV_AUTH
	struct stat file_stat;
#endif

	if(ud==NULL)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "ud(%p) is NULL!!\n", ud));
		return 0;
	}
	
#if defined(_WITH_MATRIXSSL_)
	soap_ssl_init();
#elif defined(WITH_OPENSSL)
	soap_ssl_init();
	if (CRYPTO_thread_setup())
	{ 
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "Cannot setup thread mutex\n"));
		return 0;
	}
#endif

	soap_init2( &m->cpe_soap, SOAP_IO_KEEPALIVE|SOAP_C_UTFSTRING, SOAP_IO_KEEPALIVE|SOAP_XML_TREE|SOAP_C_UTFSTRING );
	//m->cpe_soap.socket_flags = MSG_DONTWAIT | MSG_NOSIGNAL;
	m->cpe_soap.connect_timeout = 3;
	m->cpe_soap.send_timeout = 30;
	//m->cpe_soap.recv_timeout = 15;
	m->cpe_soap.user = m->cpe_user;
	
	if(gWanIfSpecified)
	{
		struct sockaddr_in local_addr_in;
		local_addr_in.sin_family = AF_INET;
		while(!getInAddr(tr069WanIfName, IP_ADDR, (void *)&(local_addr_in.sin_addr))){
			sleep(1);
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d] didn't get IP for %s\n", __FUNCTION__, __LINE__, tr069WanIfName));
		}
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] local address: %s\n", __FUNCTION__, __LINE__, inet_ntoa(local_addr_in.sin_addr)));
		soap_set_local_addr(&cpe_client.cpe_soap, &local_addr_in, tr069WanIfName);
	}
	
#if defined(_WITH_MATRIXSSL_) || defined(WITH_OPENSSL)
#ifdef CWMP_DYNAMIC_SERV_AUTH
	if( stat( CA_FNAME, &file_stat )<0 )
		gNeedSSLAuth = 0;
	else
		gNeedSSLAuth = 1;
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gNeedSSLAuth=%d\n", __FUNCTION__, __LINE__, gNeedSSLAuth));

	//update certificate location
	{
		struct stat file_stat;

		if( stat( CERT_FNAME, &file_stat )<0 )
			ud->cert_path = strdup(DEF_CERT_FN);
		else
			ud->cert_path = strdup(CERT_FNAME);

		if( stat( CA_FNAME, &file_stat )<0 )
			ud->ca_cert = strdup(DEF_CA_FN);
		else
			ud->ca_cert = strdup(CA_FNAME);
	}
#endif
	if( certificate_setup( &m->cpe_soap, gNeedSSLAuth )<0 && strncmp(ud->url, "https:", 6)==0 ){
		return 0;
	}
#endif /*#ifdef CWMP_ENABLE_SSL*/

#ifdef WITH_COOKIES
	m->cpe_soap.cookie_max = 10;
#endif
	if (soap_register_plugin(&m->cpe_soap, http_da))
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "register plugin error\n"));
		soap_print_fault(&m->cpe_soap, stderr); // failed to register
		return 0;
	}
	
	return 1;
}

/************************************************************************************************
 **********************  F l a s h  O p e r a t i o n  U t i l i t i e s  ***********************
 ************************************************************************************************/
/*
 * Basic flash I/O function
 * Return value: 0: failure, 1: succeed
 */
int flash_read(char *buf, char *path, int offset, int len)
{
	int fh;
	int ok=1;

	fh = open(path, O_RDWR);
	if ( fh == -1 )
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("open file error\n"));
		return 0;
	}
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s %d] opened %s for read\n", __FUNCTION__, __LINE__, path));

	if(lseek(fh, offset, SEEK_SET) < 0){	
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("lseek  error,%s\n",strerror(errno)));
		close(fh);
		return 0;
	}
	if (read(fh, buf, len) != len){	
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("read file error,%s\n",strerror(errno)));
		ok = 0;
	}

	close(fh);

	return ok;
}

int flash_write(char *buf, char *path, int offset, int len)
{
	int fh;
	int ok=1;

	fh = open(path, O_RDWR);
	if ( fh == -1 )
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("open file error\n"));
		return 0;
	}
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s %d] opened %s for write %d bytes from offset %d\n", __FUNCTION__, __LINE__, path, len, offset));

	lseek(fh, offset, SEEK_SET);
	
	if ( write(fh, buf, len) != len)
		ok = 0;

	close(fh);
	sync();

	return ok;
}

void flash_dump(char *path, int offset, int len)
{
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]---------dump cwmp flash------------\n", __FUNCTION__, __LINE__));
	int i=0;
	char *buf = (char *) malloc(len);
	if(buf == NULL)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("malloc %d bytes fail\n", len));
	}else{
		if(flash_read(buf,path,offset,len))
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("Hex dump %d bytes of %s from offset %d.\n", len, path, offset));
			for(i=0; i<len; i++)
			{
				if(i!=0 && i%4==0)
					printf(" ");
				if(i!=0 && i%16==0)
					printf("\n");
				printf("%02x", buf[i]);
			}
			printf("\n");
		}else{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("flash read failure\n"));
		}
	}
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]---------end dump flash-------------\n", __FUNCTION__, __LINE__));
}
#ifdef CONFIG_RTL_JFFS2_FILE_PARTITION
/*
 * Get flash data in jffs2 partition
 * @payload:
 *		Ptr to buffer that used to store mtd payload.
 *		(NOTE:) Callee must remember to free(*payload) !!
 * @type:
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * Return 0: failure, mtd read size: succeed
 */

static int load_cwmp_mtd(char **payload, int type)
{
	int mtd_data_len = 0;
	char *mtd_data = NULL;
	FILE * fp = NULL;
	char *filepath;
	
	if(payload==NULL) return 0;
	*payload = NULL;	// initial as NULL
	
	if( CWMP_MTD_TYPE_TRANSFER == type)
		filepath = TRANSFER_FILE;
	else if( CWMP_MTD_TYPE_NOTIFICATION == type)
		filepath = NOTIFICATION_FILE;
	else{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d] Error: type %d invalid!!\n", __FUNCTION__, __LINE__, type));
		return 0;
	}
	fp = fopen(filepath,"r");
	if(fp == NULL){
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fopen %s failed,%s!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		return 0;
	}
	if(fread(&mtd_data_len, 1, sizeof(int), fp) != sizeof(int)){		
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fread %s failed,%s!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		fclose(fp);
		return 0;
	}	
	if((mtd_data = malloc(mtd_data_len))==NULL){
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]malloc %d failed!\n", __FUNCTION__, __LINE__,mtd_data_len));
		fclose(fp);
		return 0;
	}	
	if(fread(mtd_data, 1, mtd_data_len, fp) != mtd_data_len){		
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fread %s failed,%s!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		fclose(fp);
		return 0;
	}
	*payload = mtd_data;
	fclose(fp);
	return mtd_data_len;
}
/*
 * store_cwmp_mtd:
 *		write buf into jffs2 partition.
 * @buf:  data to store into partition
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * @Return Value:
 *		0: failure, 1: succeed
 */
static int store_cwmp_mtd(char *buf, int size, int type)
{
	FILE * fp = NULL;
	char *filepath;
	
	if( buf==NULL || size<=0 )
		return 0;
	
	if( CWMP_MTD_TYPE_TRANSFER == type)
		filepath = TRANSFER_FILE;
	else if( CWMP_MTD_TYPE_NOTIFICATION == type)
		filepath = NOTIFICATION_FILE;
	else{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d] Error: type %d invalid!!\n", __FUNCTION__, __LINE__, type));
		return 0;
	}
	
	fp = fopen(filepath,"w");
	if(fp == NULL){
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fopen %s failed,%s!!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		return 0;
	}
	if(fwrite(&size, 1, sizeof(int), fp) != sizeof(int)){
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fwrite %s failed,%s!!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		fclose(fp);
		return 0;
	}
	if(fwrite(buf, 1, size, fp) != size){
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]fwrite %s failed,%s!!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		fclose(fp);
		return 0;
	}
	fclose(fp);
	return 1;
}

/*
 * flush_cwmp_mtd:
 * @type:
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * @Return Value:
 *		0: failure, 1: succeed
 */
static int flush_cwmp_mtd(int type)
{
	char *filepath;
	
	if( CWMP_MTD_TYPE_TRANSFER == type)
		filepath = TRANSFER_FILE;
	else if( CWMP_MTD_TYPE_NOTIFICATION == type)
		filepath = NOTIFICATION_FILE;
	else{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d] Error: type %d invalid!!\n", __FUNCTION__, __LINE__, type));
		return 0;
	}
	
	if(unlink(filepath)){		
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]unlink %s failed,%s!!\n", __FUNCTION__, __LINE__,filepath,strerror(errno)));
		return 0;
	}
	
	return 1;
}
#else
/*
 * Get mtd partition for cwmp
 * Return 0: failure, 1: succeed
 */
static int get_cwmp_mtdname(char *mtdname, int type)
{
    if(mtdname==NULL)
        return 0; 

	if( CWMP_MTD_TYPE_TRANSFER == type )
	{
#ifdef MTD_NAME_MAPPING
    	rtl_name_to_mtdblock(CWMP_TRANSFER_MTD_NAME, mtdname);
#else
		sprintf(mtdname, "/dev/mtdblock2");
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_WARNING, ("[%s:%d]Warning: TR-069 uses default mtd partition %s, which may don't exist!!\n",__FUNCTION__, __LINE__, mtdname));
#endif
	}else if( CWMP_MTD_TYPE_NOTIFICATION == type ){
#ifdef MTD_NAME_MAPPING
    	rtl_name_to_mtdblock(CWMP_NOTIFY_MTD_NAME, mtdname);
#else
		sprintf(mtdname, "/dev/mtdblock3");
		CWMPDBG_FUNC(MODULE_CORE|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]Warning: TR-069 uses default mtd partition %s, which may don't exist!!\n",__FUNCTION__, __LINE__, mtdname));
#endif
	}else{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d] Error: type %d invalid!!\n", __FUNCTION__, __LINE__, type));
		return 0;
	}
    CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] %s uses mtd partition:%s\n", __FUNCTION__, __LINE__, (type==CWMP_MTD_TYPE_TRANSFER)?CWMP_TRANSFER_MTD_NAME:CWMP_NOTIFY_MTD_NAME, mtdname));
    return 1;
}

/*
 * Get flash data in cwmp mtd partition
 * @payload:
 *		Ptr to buffer that used to store mtd payload.
 *		(NOTE:) Callee must remember to free(*payload) !!
 * @type:
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * Return 0: failure, mtd read size: succeed
 */
static int load_cwmp_mtd(char **payload, int type)
{
	char mtd_name[32] = {0};
	char mgmt_data[CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH] = {0};
	char *payload_tmp = NULL;
	if(payload==NULL)
		return 0;
	*payload = NULL;	// initial as NULL
	
	if(get_cwmp_mtdname(mtd_name, type)==1)
	{
		// validation and get payload length
		if(flash_read(mgmt_data, mtd_name, CWMP_MTD_OFFSET_MAGIC_NUM, 
				CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH))
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]read mtd partition %s succeed, begin to validate data...\n", __FUNCTION__, __LINE__, mtd_name));
			if( memcmp((type==CWMP_MTD_TYPE_TRANSFER)?CWMP_MTD_TRANSFER_MAGIC_NUM:CWMP_MTD_NOTIFICATION_MAGIC_NUM, mgmt_data, CWMP_MTD_SIZE_MAGIC_NUM)==0 )
			{
				int payload_tag = -1, payload_length = -1;
				char payload_tag_buf[CWMP_MTD_SIZE_TAG + 1] = {0}, payload_length_buf[CWMP_MTD_SIZE_LENGTH + 1] = {0};
				memcpy(&payload_tag, mgmt_data+CWMP_MTD_OFFSET_TAG, sizeof(int));
				if(payload_tag<=0)
				{
					// error
					CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: tag area %02x:%02x:%02x:%02x invalid!!\n", __FUNCTION__, __LINE__,
						payload_tag_buf[0], payload_tag_buf[1], payload_tag_buf[2], payload_tag_buf[3]));
					return 0;
				}else
					CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]tag:%x\n", __FUNCTION__, __LINE__, payload_tag));
				switch (payload_tag)
				{
				case CWMP_MTD_TAG_PLAIN:
					memcpy(&payload_length, mgmt_data+CWMP_MTD_OFFSET_LENGTH, CWMP_MTD_SIZE_LENGTH);
					if(payload_length<=0)
					{
						// error
						CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: payload length area %02x:%02x:%02x:%02x invalid!!\n", __FUNCTION__, __LINE__,
							payload_length_buf[0], payload_length_buf[1], payload_length_buf[2], payload_length_buf[3]));
						return 0;
					}else{
						CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]mtd data is valid, total %d bytes payload, begin to read payload...\n", __FUNCTION__, __LINE__, payload_length));
					}
					break;
				default:
					// error
					CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: tag value %d not supported\n", __FUNCTION__, __LINE__, payload_tag));
					return 0;
				}
				payload_tmp = (char *)malloc(payload_length);
				if(payload_tmp == NULL)
				{
					// error
					CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: malloc %d bytes failed\n", __FUNCTION__, __LINE__, payload_length));
					return 0;
				}else{
					// read mtd payload to buffer
					if(flash_read(payload_tmp, mtd_name, CWMP_MTD_OFFSET_PAYLOAD, payload_length)==1)
					{
						CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]read mtd payload successfully!\n", __FUNCTION__, __LINE__));
						*payload = payload_tmp;
						return payload_length;
					}else{
						CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: read mtd payload failed!!\n", __FUNCTION__, __LINE__));
						free(payload_tmp);
						payload_tmp = NULL;
						return 0;
					}
				}
			}else{
				mgmt_data[CWMP_MTD_SIZE_MAGIC_NUM] = '\0';
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: magic number %02x%02x%02x%02x invalid\n", __FUNCTION__, __LINE__, 
					mgmt_data[0], mgmt_data[1], mgmt_data[2], mgmt_data[3]));
				return 0;
			}
		}else{
			// error
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: get cwmp mtd management data error!!\n", __FUNCTION__, __LINE__));
			return 0;
		}
	}else{
		// error
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: get cwmp mtd partition name error!!\n", __FUNCTION__, __LINE__));
		return 0;
	}
}

/*
 * store_cwmp_mtd:
 *		write buf into cwmp mtd partition.
 * @buf:  data to store into cwmp mtd partition, including format buf with cwmp mtd partition data format.
 * @size: data total size.
 * @type:
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * @Return Value:
 *		0: failure, 1: succeed
 */
static int store_cwmp_mtd(char *buf, int size, int type)
{
	if( buf==NULL || size<=0 )
		return 0;
	
	char mtd_name[32] = {0};
	int mgmt_tag = CWMP_MTD_TAG_PLAIN;
	if(get_cwmp_mtdname(mtd_name, type)==0)
	{
		// error
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Error: get cwmp mtd partition name error!!\n", __FUNCTION__, __LINE__));
		return 0;
	}
	
	int mtd_mgmt_size = CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH ;
	char mtd_mgmt[CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH] = {0};

	// validation
	if( type==CWMP_MTD_TYPE_NOTIFICATION )
	{
		if((size+mtd_mgmt_size)>CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE){
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] store type %d oversize(%d+%d>%d)\n", __FUNCTION__, __LINE__, type,
				size, mtd_mgmt_size, CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE));
			return 0;
		}
	}else if( type==CWMP_MTD_TYPE_TRANSFER ){
		if((size+mtd_mgmt_size)>CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE){
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] store type %d oversize(%d+%d>%d)\n", __FUNCTION__, __LINE__, type,
				size, mtd_mgmt_size, CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE));
			return 0;
		}
	}
	
	// format mgmt data and write to flash
	//CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]store cwmp mtd management data...tag:%s;length:%s\n", __FUNCTION__, __LINE__,tmp1, tmp2));
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_MAGIC_NUM, (type==CWMP_MTD_TYPE_TRANSFER)?CWMP_MTD_TRANSFER_MAGIC_NUM:CWMP_MTD_NOTIFICATION_MAGIC_NUM, CWMP_MTD_SIZE_MAGIC_NUM);
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_TAG, &mgmt_tag, sizeof(int));
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_LENGTH, &size, sizeof(int));
		
	if(flash_write(mtd_mgmt, mtd_name, CWMP_MTD_OFFSET_MAGIC_NUM, mtd_mgmt_size)==1)
	{
		//write payload to flash
		if(flash_write(buf, mtd_name, CWMP_MTD_OFFSET_PAYLOAD, size)==1)
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]write data to mtd %s succeed!\n", __FUNCTION__, __LINE__, mtd_name));
			return 1;
		}else{
			//error
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Error: write payload data %d bytes to mtd %s fail!!\n", __FUNCTION__, __LINE__, size, mtd_name));
			return 0;
		}
	}else{
		//error
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Error: write management data %d bytes to mtd %s fail!!\n", __FUNCTION__, __LINE__, mtd_mgmt_size, mtd_name));
		return 0;
	}
}

/*
 * flush_cwmp_mtd:
 *		clear cwmp mtd partition payload, but keep magic number/tag/length(value:0).
 * @type:
 *		CWMP_MTD_TYPE_TRANSFER / CWMP_MTD_TYPE_NOTIFICATION
 * @Return Value:
 *		0: failure, 1: succeed
 */
static int flush_cwmp_mtd(int type)
{
	char mtd_name[32] = {0};
	int mtd_tag = CWMP_MTD_TAG_PLAIN, mtd_mgmt_size = CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH ;
	int new_size = 0;
	char mtd_mgmt[CWMP_MTD_SIZE_MAGIC_NUM + CWMP_MTD_SIZE_TAG + CWMP_MTD_SIZE_LENGTH] = {0};
	if(get_cwmp_mtdname(mtd_name, type)==0)
	{
		// error
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: get cwmp mtd partition name error!!\n", __FUNCTION__, __LINE__));
		return 0;
	}

	// format mgmt data and write to flash
	//CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]store cwmp mtd management data...tag:%s;length:%s\n", __FUNCTION__, __LINE__,tmp1, tmp2));
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_MAGIC_NUM, (type==CWMP_MTD_TYPE_TRANSFER)?CWMP_MTD_TRANSFER_MAGIC_NUM:CWMP_MTD_NOTIFICATION_MAGIC_NUM, CWMP_MTD_SIZE_MAGIC_NUM);
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_TAG, &mtd_tag, CWMP_MTD_SIZE_TAG);
	memcpy(mtd_mgmt+CWMP_MTD_OFFSET_LENGTH, &new_size, CWMP_MTD_SIZE_LENGTH);

	if(flash_write(mtd_mgmt, mtd_name, CWMP_MTD_OFFSET_MAGIC_NUM, mtd_mgmt_size)==1)
	{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] flush cwmp mtd %s succeed.\n", __FUNCTION__, __LINE__, mtd_name));
		return 1;
	}else{
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: flush cwmp mtd %s failed!!\n", __FUNCTION__, __LINE__, mtd_name));
		return 0;
	}
}

#endif
int cwmp_flush_partition(char * partition)
{
	if(strcmp(partition, "transfer" ) == 0)
		flush_cwmp_mtd(CWMP_MTD_TYPE_TRANSFER);
	else if(strcmp(partition, "notification" ) == 0)
		flush_cwmp_mtd(CWMP_MTD_TYPE_NOTIFICATION);
	else
		return -1;

	return 0;
}
#if defined(MTD_NAME_MAPPING) && defined(CONFIG_RTL_CWMP_CACERT_PARTITION)
static int restore_cacert_from_partition(void)
{
	char mtdname[32] = {0};
	int offset = 2*sizeof(int);
	int head[2] = {0};
	char *payload;
	FILE	* stream;
	
	rtl_name_to_mtdblock(CWMP_CACERT_MTD_NAME, mtdname);

	if(flash_read((char *)head, mtdname, 0, sizeof(head)) == 0)
		return -1;
	
	if(head[0] != 0xabcddcba){	
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: Cacert Tag not find!!\n", __FUNCTION__, __LINE__));
		return -1;
	}

	if(head[1] == 0){	
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]Error: len is 0!!\n", __FUNCTION__, __LINE__));
		return -1;
	}
	
	if((payload = calloc(head[1]+1,sizeof(char))) == NULL){
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: calloc failed!!\n", __FUNCTION__, __LINE__));
		return -1;
	}
		
	if(flash_read(payload, mtdname, offset,head[1]) == 0){		
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: flash_read failed!!\n", __FUNCTION__, __LINE__));
		return -1;
	}
	
	if((stream = fopen(CA_FNAME,"w")) == NULL){
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]Error: fopen %s failed,%s!!\n", __FUNCTION__, __LINE__, CA_FNAME, strerror(errno)));
		return -1;
	}
	
	fwrite(payload, head[1], 1, stream);
	
	fclose(stream);
	free(payload);
	return 0;
}
#endif

/************************************************************************************************
 *********************  E n d  F l a s h  O p e r a t i o n  U t i l i t i e s  *****************
 ************************************************************************************************/
 
/************************************************************************************************
 ************************** F i l e  T r a n s f e r  U t i l i t i e s  ************************
 ************************************************************************************************/
/*
 * init_transfer_queue: init cwmp_userdata->TransferQueue by read cwmp mtd partition data
 * @root:
 *		&cwmp_userdata->TransferQueue
 * @Return Value:
 * 		0: failure, 1: succeed
 */
static int init_transfer_queue(struct node **root)
{
	struct cwmp_transfer_t *transfer = NULL;
	char *mtd_data = NULL;
	int mtd_data_len = -1, total, i, j;

	*root = NULL;
	if((mtd_data_len=load_cwmp_mtd(&mtd_data, CWMP_MTD_TYPE_TRANSFER))==0 || mtd_data==NULL )
	{
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Error: no exsit transfer queue!!\n", __FUNCTION__, __LINE__));
		return 0;
	}
	if(mtd_data_len%sizeof(struct cwmp_transfer_t)!=0)
	{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ("[%s:%d]Error: transfer queue raw data length %d(%d) invalid!!\n", __FUNCTION__, __LINE__, mtd_data_len, sizeof(struct cwmp_transfer_t)));
		return 0;
	}

	total = mtd_data_len/sizeof(struct cwmp_transfer_t);
	for(i = 0 ; i < total ; i++)
	{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] init transfer node %d( Total %d ):\n", __FUNCTION__, __LINE__, i, total));
		/*
		 * node data transfer will be free in port_free_transfer_queue(), or when remove_node().
		 */
		transfer = (struct cwmp_transfer_t *)malloc(sizeof(struct cwmp_transfer_t));
		if(transfer==NULL)
		{
			CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Error: malloc transfer queue node fail!!\n", __FUNCTION__, __LINE__));
			return 0;
		}
		memset(transfer, 0x0, sizeof(struct cwmp_transfer_t));
		memcpy(transfer, (struct cwmp_transfer_t *)mtd_data + i, sizeof(struct cwmp_transfer_t));
		
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TransferState:%d\n", transfer->TransferState));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TransferType:%d\n", transfer->TransferType));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->CommandKey:%s\n",transfer->CommandKey));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FileType:%s\n", transfer->FileType));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->URL:%s\n", transfer->URL));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->Username:%s\n", transfer->Username));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->Password:%s\n", transfer->Password));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FileSize:%d\n", transfer->FileSize));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TargetFileName:%s\n", transfer->TargetFileName));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->DelaySeconds:%d\n", transfer->DelaySeconds));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->SuccessURL:%s\n", transfer->SuccessURL));
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FailureURL:%s\n", transfer->FailureURL));

		push_node_data(root, transfer);
	}

	if(NULL!=mtd_data)
		free(mtd_data);
	mtd_data = NULL;
	return 1;
}

/*
 * dump_transfer_queue:
 *		Debug function, will dump all transfer queue in cwmp_userdata->TransferQueue
 */
static void dump_transfer_queue(struct node *root)
{
	int total = 0, i;
	struct cwmp_transfer_t *transfer = NULL;

	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]-------dump transfer queue-------\n", __FUNCTION__, __LINE__));
	total = get_node_count(root);
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("Total: %d\n", total));
	if(total)
	{
		for(i=0; i<total; i++)
		{
			transfer = get_node_data(root, i);
			
			if(transfer){
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TransferState:%d\n", transfer->TransferState));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TransferType:%s\n", transfer->TransferType==CWMP_TRANSFER_TYPE_UPLOAD?"Upload":"Download"));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->CommandKey:%s\n",transfer->CommandKey));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FileType:%s\n", transfer->FileType));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->URL:%s\n", transfer->URL));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->Username:%s\n", transfer->Username));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->Password:%s\n", transfer->Password));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FileSize:%d\n", transfer->FileSize));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->TargetFileName:%s\n", transfer->TargetFileName));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->DelaySeconds:%d\n", transfer->DelaySeconds));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->SuccessURL:%s\n", transfer->SuccessURL));
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer->FailureURL:%s\n", transfer->FailureURL));
			}else
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("transfer at %d is NULL\n", i));
		}
	}
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]---------------end---------------\n", __FUNCTION__, __LINE__));
}

/*
 * port_save_transfer_queue:
 * 		save cwmp_userdata->TransferQueue into mtd partition.
 *		(allow be called directyly to store transfer queue.)
 * @Return Value:
 *		0: failure; 1: succeed.
 */
int port_save_transfer_queue(struct node *root)
{
	struct cwmp_transfer_t *transfer = NULL;
	char *buf = NULL;
	int i, total, bufsize=0;

	// if root==NULL, total will be 0.
	total = get_node_count(root);
	if(total==0)
	{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Warning: no transfer queue node any more, flush flash partition...\n", __FUNCTION__, __LINE__));
		return flush_cwmp_mtd(CWMP_MTD_TYPE_TRANSFER);
	}
	
	bufsize = total*sizeof(struct cwmp_transfer_t);
	buf = (char *)malloc(bufsize);
	if(NULL==buf)
	{
		//error
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ("[%s:%d]Error: malloc %d*%d bytes fail!!\n", __FUNCTION__, __LINE__, total, sizeof(struct cwmp_transfer_t)));
		return 0;
	}
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Save transfer queue into flash...\n", __FUNCTION__, __LINE__));
	memset(buf, 0x0, bufsize);
	
	for(i = 0 ; i < total ; i++)
	{
		transfer = (struct cwmp_transfer_t *)get_node_data(root, i);
		if(transfer == NULL)
			continue;

		memcpy(buf+i*sizeof(struct cwmp_transfer_t), transfer, sizeof(struct cwmp_transfer_t));
	}

	// dump out for debug
	dump_transfer_queue(root);
	
	if(store_cwmp_mtd(buf, bufsize, CWMP_MTD_TYPE_TRANSFER)==0)
	{
		//error
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ("[%s:%d]Error: store transfer queue into flash fail!!\n", __FUNCTION__, __LINE__));
		if(buf!=NULL)
		{
			free(buf);
			buf = NULL;
		}
		return 0;
	}else{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]store transfer queue into flash succeed\n", __FUNCTION__, __LINE__));
		if(buf!=NULL)
		{
			free(buf);
			buf = NULL;
		}
		return 1;
	}
}

/*
 * free_transfer_queue:
 *		free cwmp_userdata->TransferQueue
 */
void port_free_transfer_queue( struct cwmp_userdata *ud )
{
	if(NULL==ud->TransferQueue)
		return;

	struct cwmp_transfer_t *transfer = NULL;
	struct node *queue = ud->TransferQueue;
	while(transfer = pop_node_data(&queue))
	{
		if(transfer!=NULL)
		{
			free(transfer);
			transfer = NULL;
		}
	}
}

/*
 * port_dump_transfer_queue
 *		dump transfer queue in ud->TransferQueue
 */
void port_dump_transfer_queue( struct cwmp_userdata *ud )
{
	if(ud!=NULL && ud->TransferQueue!=NULL)
		dump_transfer_queue(ud->TransferQueue);
	else
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_WARNING, ("[%s:%d]transfer queue hasn't been initialized!!\n", __FUNCTION__, __LINE__)); 
	return ;
}

/*
 * port_correct_transfer_queue:
 *		correct delayseconds in transfer queue, minus @sec for each node
 */
void port_correct_transfer_queue( struct cwmp_userdata *ud, unsigned int sec )
{
	int i, total=0;
	if(ud && ud->TransferQueue)
	{
		total = get_node_count(ud->TransferQueue);
		if(total)
		{
			struct cwmp_transfer_t *transfer = NULL;
			for(i=0; i<total; i++)
			{
				transfer = get_node_data(ud->TransferQueue, i);
				if(transfer && transfer->TransferState==CWMP_DLQ_STATE_NOT_YET_START)
				{
					CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] correct transfer node %d delay seconds down %d seconds\n", __FUNCTION__, __LINE__,
						i, sec));
					transfer->DelaySeconds -= sec;
				}
			}
		}
	}
}

/************************************************************************************************
 ********************  E n d  F i l e  T r a n s f e r  U t i l i t i e s  **********************
 ************************************************************************************************/
/************************************************************************************************
 *************************  N o t i f i c a t i o n  U t i l i t i e s  *************************
 ************************************************************************************************/
/*
 * init_mtd_notification_data:
 * 		read notification data from mtd flash partition. ( store with format of n*(struct cwmp_notify_t) in mtd )
 *		add them to pCWMPNotifyRoot link table.
 * @Return Value:
 *		0: failure, 1: succeed
 */
static int init_mtd_notification_data()
{
	char *mtddata = NULL;
	int mtd_len = 0, total = 0, return_val = -1;

	mtd_len = load_cwmp_mtd(&mtddata, CWMP_MTD_TYPE_NOTIFICATION);

	if( 0==mtd_len || mtddata==NULL )
	{
		CWMPDBG_FUNC(MODULE_CORE|MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d]Error: read mtd partition data error, return %d!!\n", __FUNCTION__, __LINE__, mtd_len));
		return_val = 0;
	}else if( mtd_len%sizeof(struct cwmp_notify_t) != 0 ){
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_ERROR, ("[%s:%d]Error: mtd partition length %d error, should be n*sizeof(struct cwmp_notify_t)(%d)!!\n", __FUNCTION__, __LINE__, mtd_len, sizeof(struct cwmp_notify_t)));
		return_val = 0;
	}else{
		int i = 0;
		char *ptr = NULL;
		char delim[] = ".";
		struct cwmp_notify_t tmp = {0};
		total = mtd_len/sizeof(struct cwmp_notify_t);
		/*
		 * check whether data in mtd partition matches current data model selection (e.g: current using tr-181 but mtd partitions store tr-098's notificaion)
		 * If mismatch, flush mtd partition.
		 */
		strncpy(tmp.name, mtddata, sizeof(tmp.name));
		if((ptr=strtok(tmp.name, delim))==NULL)
		{
			CWMPDBG_FUNC(MODULE_CORE|MODULE_NOTIFY, LEVEL_ERROR, ("[%s:%d]Error: data model name in mtd is incorrect(%s)!!\n", __FUNCTION__, __LINE__, tmp.name));
			return_val = 0;
		}else{
			if(strcmp(ptr, DATAMODLE_PREFIX)!=0)
			{
				// flush mtd partition
				CWMPDBG_FUNC(MODULE_CORE|MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]data model name(%s) in mtd mismatches current selection(%s), flush mtd parittion 'cwmp notification'!!\n", __FUNCTION__, __LINE__, tmp.name, DATAMODLE_PREFIX));
				flush_cwmp_mtd(CWMP_MTD_TYPE_NOTIFICATION);
				return_val = 1;
			}else
			{
				/*
				 * load mtd notification data into cwmpClient.
				 */
				memset(&tmp, 0x0, sizeof(struct cwmp_notify_t));
				for(i=0; i<total; i++)
				{
					strncpy(tmp.name, mtddata+i*sizeof(struct cwmp_notify_t), sizeof(tmp.name));
					memcpy(&tmp.notifx, mtddata+i*sizeof(struct cwmp_notify_t)+sizeof(tmp.name), sizeof(tmp.notifx));
					memcpy(&tmp.accesslist, mtddata+i*sizeof(struct cwmp_notify_t)+sizeof(tmp.name)+sizeof(tmp.notifx), sizeof(tmp.accesslist));
					CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d]load %d(total %d) notification node from mtd to pCWMPNotifyRoot\n", __FUNCTION__, __LINE__, i, total));
					CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("name:%s\n", tmp.name));
					CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("notifx:%x\n", tmp.notifx));
					CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("accesslist:%x\n", tmp.accesslist));
					if(tmp.notifx==0)
						CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("(Ignored because of notifx is 0)\n"));
					else
						notify_set_attributes( tmp.name, tmp.notifx, tmp.accesslist );

					memset(&tmp, 0x0, sizeof(struct cwmp_notify_t));
				}
				return_val = 1;
			}
		}	
	}

	if(mtddata!=NULL)
		free(mtddata);
	return return_val;
}

static int save_mtd_notification_data()
{
	extern struct CWMP_NOTIFY *pCWMPNotifyRoot;
	struct CWMP_NOTIFY *raw = NULL;
	struct cwmp_notify_t tmp;
	int total = 0, i, retVal = -1;
	char *buf = NULL;

	if(pCWMPNotifyRoot == NULL)
	{
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]Warning: no notification data, flush flash partition...\n", __FUNCTION__, __LINE__));
		return flush_cwmp_mtd(CWMP_MTD_TYPE_NOTIFICATION);
	}
	
	total = notify_count(&pCWMPNotifyRoot);
	buf = (char *) malloc( total*sizeof(struct cwmp_notify_t) );
	if(buf==NULL)
	{
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_ERROR, ("[%s:%d] Error: malloc %d*%d bytes error!!\n", __FUNCTION__, __LINE__, total, sizeof(struct cwmp_notify_t)));
		retVal = 0;
	}else{
		raw = pCWMPNotifyRoot;
		for(i=0; i<total; i++)
		{
			strncpy(tmp.name, raw->name, strlen(raw->name)<sizeof(tmp.name)?strlen(raw->name):sizeof(tmp.name));
			tmp.notifx = raw->notifx;
			tmp.accesslist = raw->accesslist;
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d] store %d(total %d) notification node from pCWMPNotifyRoot to mtd\n", __FUNCTION__, __LINE__, i, total));
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("name:%s\n", tmp.name));
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("notifx:%x\n", tmp.notifx));
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("accesslist:%x\n", tmp.accesslist));
			memcpy(buf+i*sizeof(struct cwmp_notify_t), &tmp, sizeof(struct cwmp_notify_t));
			
			memset(&tmp, 0x0, sizeof(struct cwmp_notify_t));
			raw = raw->next;
		}

		if(store_cwmp_mtd(buf, total*sizeof(struct cwmp_notify_t), CWMP_MTD_TYPE_NOTIFICATION)==0)
		{
			// error
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d]Error: store notification to flash error!!\n", __FUNCTION__, __LINE__));
			retVal = 0;
		}else{
			CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d] store notification to flash succeed!!\n", __FUNCTION__, __LINE__));
			retVal = 1;
		}
	}

	if(buf!=NULL)
		free(buf);

	return retVal;
}

static void dump_mtd_notification_data()
{
	extern struct CWMP_NOTIFY *pCWMPNotifyRoot;
	struct CWMP_NOTIFY *raw = NULL;
	int i=0, total=0;
	
	if(pCWMPNotifyRoot == NULL)
	{
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_WARNING, ("[%s:%d]no notification data\n", __FUNCTION__, __LINE__));
		return ;
	}
	
	total = notify_count(&pCWMPNotifyRoot);
	raw = pCWMPNotifyRoot;
	for(i=0; i<total; i++)
	{
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("[%s:%d]notification node %d( total %d )\n", __FUNCTION__, __LINE__, i, total));
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("name:%s\n", raw->name));
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("notifx:%x\n", raw->notifx));
		CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ("accesslist:%x\n", raw->accesslist));
		
		raw = raw->next;
		
		if(raw==NULL)
			break;
	}
	return;

}

/*
 * port_notify_load_userdef:
 *		wrapper of init_mtd_notification_data.
 *		Load used defined notification data to pCWMPNotifyRoot.
 * @Return Value
 *		0: failed, 1: succeed
 */
int port_notify_load_userdef()
{
	int retval = init_mtd_notification_data();

	return retval;
}

/*
 * port_notify_save:
 *		Save all CWMP_NOTIFY in gCWMPNotifyRoot to flash.
 * @Return Value:
 *		0: failure, 1: succeed
 */
int port_notify_save(  )
{
	int retVal = 0;

	retVal = save_mtd_notification_data();
	
	return retVal;
}

/*
 * port_notify_dump
 *		dump notification list pCWMPNotifyRoot.
 *		(not raw data in mtd partition. There may be some different between them: data model with notifx 0 won't be added to notification list)
 */
void port_notify_dump()
{
	dump_mtd_notification_data();

	return;
}

int port_notify_load_default(int flag)
{
#if defined(CONFIG_USER_CWMP_WITH_TR181)
	struct CWMP_NOTIFY *pNotify;
	int i = 0, total = (DEFAULT_NOTIFY_LIST==NULL)?0:(sizeof(DEFAULT_NOTIFY_LIST)/sizeof(struct cwmp_notify_t));

	//forced inform parameters
	for(i=0; i<total; i++)
		notify_set_attributes( DEFAULT_NOTIFY_LIST[i].name, DEFAULT_NOTIFY_LIST[i].notifx, DEFAULT_NOTIFY_LIST[i].accesslist );

	pNotify = notify_find(&pCWMPNotifyRoot, "Device.ManagementServer.ConnectionRequestURL");
	if(pNotify) pNotify->id = CWMP_NTF_CNT_REQ_URL;

	pNotify = notify_find(&pCWMPNotifyRoot, "Device.ManagementServer.ParameterKey");
	if(pNotify) pNotify->id = CWMP_NTF_PARAM_KEY;

	return 0;
#else
	char *name=NULL;
	int add_default_wan_error=1;
	struct CWMP_NOTIFY *pNotify;
	int i = 0, total = (DEFAULT_NOTIFY_LIST==NULL)?0:(sizeof(DEFAULT_NOTIFY_LIST)/sizeof(struct cwmp_notify_t));

	//forced inform parameters
	for(i=0; i<total; i++)
		notify_set_attributes( DEFAULT_NOTIFY_LIST[i].name, DEFAULT_NOTIFY_LIST[i].notifx, DEFAULT_NOTIFY_LIST[i].accesslist );

	pNotify = notify_find(&pCWMPNotifyRoot, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
	if(pNotify) pNotify->id = CWMP_NTF_CNT_REQ_URL;

	pNotify = notify_find(&pCWMPNotifyRoot, "InternetGatewayDevice.ManagementServer.ParameterKey");
	if(pNotify) pNotify->id = CWMP_NTF_PARAM_KEY;

	if(gBringLanMacAddrInInform)
	{
		char sLanName[90];
		snprintf( sLanName, sizeof(sLanName)-1, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.%d.MACAddress", gBringLanMacAddrInInform );
		sLanName[sizeof(sLanName)-1]=0;
		notify_set_attributes( sLanName, CWMP_NTF_FORCED|CWMP_NTF_PAS, CWMP_ACS_MASK  );
	}

	notify_clean_wan_with_forced_attribute();
	//InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{j}.WAN{***}Connection.{k}.ExternalIPAddress	
	{	//add the default wan channel
		char *pName="InternetGatewayDevice.Layer3Forwarding.DefaultConnectionService";
		int  type=eCWMP_tNONE;
		void *data=NULL;

		if(get_ParameterValue( pName, &type, &data )==0)
		{
			char *pwanchannel=data;
			char buf[256+1]="";
			int len;
			
			len=strlen(pwanchannel);
			if( len>0 && len<256 )
			{
				snprintf( buf, 256, "%s.ExternalIPAddress", pwanchannel );
				buf[256]=0;
				CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ( "notify_load_default: add(default) %s\n", buf ) );
		  #ifdef CONFIG_BOA_WEB_E8B_CH
				if(isTR069(name)>0)
		  #endif
				if( notify_set_attributes( buf, CWMP_NTF_FORCED|CWMP_NTF_ACT, CWMP_ACS_MASK )==0 ) //success
					add_default_wan_error=0;
				
				if(flag!=0)
				{
					if( strstr(buf,".WANPPPConnection.") )
					{
						snprintf( buf, 256, "%s.Username", pwanchannel );
						buf[256]=0;
						CWMPDBG_FUNC(MODULE_NOTIFY, LEVEL_INFO, ( "notify_load_default: add(default) %s\n", buf ) );
				  #ifdef CONFIG_BOA_WEB_E8B_CH
						if(isINTERNET(name)>0)
				  #endif
						notify_set_attributes( name, CWMP_NTF_FORCED|CWMP_NTF_PAS, CWMP_ACS_MASK ); 					
					}
				}				
			}
			if(data) get_ParameterValueFree(type, data); //free(data);
		}
	}
	
	//bring all wan ip in the inform 
	if(gNotifyBringAllWanIPinInform || add_default_wan_error)
	{
		if( get_ParameterName( "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.", 0, &name )==0 )
		{
			do{				
				if( strstr( name, ".ExternalIPAddress" ) )
				{
					//CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "add %s\n", name ));
			  #ifdef CONFIG_BOA_WEB_E8B_CH
					if(isTR069(name)>0)
			  #endif
					notify_set_attributes( name, CWMP_NTF_FORCED|CWMP_NTF_ACT, CWMP_ACS_MASK );
				}else if(flag!=0)
				{		//for china-telecom, need to report wanpppconnection.{i}.username to acs
					if( strstr(name,".WANPPPConnection.") && strstr(name,".Username") )
					{
						//CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "add %s\n", name ));
				  #ifdef CONFIG_BOA_WEB_E8B_CH
						if(isINTERNET(name)>0)
				  #endif
						notify_set_attributes( name, CWMP_NTF_FORCED|CWMP_NTF_PAS, CWMP_ACS_MASK ); 		
					}
				}
			}while( get_ParameterName( NULL, 0, &name )==0 );
		}//if
	}

	return 0;

#endif
}

/************************************************************************************************
 **********************  E n d  N o t i f i c a t i o n  U t i l i t i e s	*********************
 ************************************************************************************************/
/*
 * port_cwmp_startup_check:
 *		check TR-069 startup environment
 *		must be called after apmib_init()
 * @Return Value
 *		see enum CWMP_ENV_STATUS in prmt_utility.h
 */
int port_cwmp_startup_check()
{
	unsigned char buf[256+1] = {0}, mtdname[32] = {0};
	unsigned char ip[16] = {0}, netmask[16] = {0}, defgw[16] = {0}, wanHWAddr[32] = {0};
	
	/* ACS url validation */
	apmib_get(MIB_CWMP_ACS_URL, (void *)buf);
	if( strncmp(buf, "https://", strlen("https://"))==0 )
	{
#ifndef CWMP_ENABLE_SSL
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("[%s:%d]error: please enable cwmp ssl support.\n", __FUNCTION__, __LINE__));
		return CWMP_ENV_ACS_URL_ERROR;
#endif
	}
	
	/* wan status check */
#ifdef MULTI_WAN_SUPPORT
	if(gWanIfSpecified && strlen(tr069WanIfName)!=0)
	{
		int count_down = 3;
		struct sockaddr_in local_addr_in;
		
		local_addr_in.sin_family = AF_INET;
		while(!getInAddr(tr069WanIfName, IP_ADDR, (void *)&(local_addr_in.sin_addr))){
			count_down--;
			sleep(1);
			if(count_down<0)
				break;
		}
		if(count_down<0)
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]please check wan connection, no ip address for %s\n", __FUNCTION__, __LINE__, tr069WanIfName));
			return CWMP_ENV_WAN_DISCONN_ERROR;
		}else
			goto CWMP_STARTUP_CHECK_END;
	}else
		return CWMP_ENV_WAN_INTERFACE_ERROR;
#else
	{
		int count_down = 3;
		while(0!=getWanInfo(ip, netmask, defgw, wanHWAddr))
		{
			count_down--;
			sleep(1);
			if(count_down<0)
				break;
		}
		if(count_down>=0){
			// succeed
			if(strcmp(ip, "0.0.0.0")==0)
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]error: please check wan connection, no ip address.\n", __FUNCTION__, __LINE__));
				return CWMP_ENV_WAN_DISCONN_ERROR;
			}else
				goto CWMP_STARTUP_CHECK_END;
		}else{
			// failed
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("[%s:%d]error: please check wan connection.\n", __FUNCTION__, __LINE__));
			return CWMP_ENV_WAN_DISCONN_ERROR;
		}
	}
#endif

	/* end startup check */
CWMP_STARTUP_CHECK_END:
	return CWMP_ENV_OK;
}

int port_init_userdata( struct cwmp_userdata *data )
{
	char buf[256 + 1];
	unsigned int ch;
	unsigned int uVal;

	if (data) {
		memset( data, 0, sizeof( struct cwmp_userdata ) );
		
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]****init cwmp userdata****\n", __FUNCTION__, __LINE__));
		//relative to SOAP header
		data->ID = 1;
		data->HoldRequests = 0;
		data->NoMoreRequests = 0;
		data->CPE_MaxEnvelopes = 1;
		data->ACS_MaxEnvelopes = 1;
		
		//cwmp:fault
		data->FaultCode=0;

		//download/upload
		data->DownloadState = DOWNLD_NONE;
		data->DownloadWay = DLWAY_NONE;
		data->DLCommandKey = mib_get(MIB_CWMP_DL_COMMANDKEY,(void *)buf) ? strdup(buf) : NULL;
		data->DLStartTime = mib_get(MIB_CWMP_DL_STARTTIME, (void *)&uVal) ? uVal : 0;
		data->DLCompleteTime = mib_get(MIB_CWMP_DL_COMPLETETIME, (void *)&uVal) ? uVal : 0;
		data->DLFaultCode = mib_get(MIB_CWMP_DL_FAULTCODE, (void *)&uVal) ? uVal : 0;
		data->CurrentQueueIdx = mib_get(MIB_CWMP_DL_QUEUE_IDX, (void *)&uVal) ? uVal : 0;
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLCommandKey:%s\n", __FUNCTION__, __LINE__, data->DLCommandKey));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLStartTime:%lu\n", __FUNCTION__, __LINE__, data->DLStartTime));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLCompleteTime:%lu\n", __FUNCTION__, __LINE__, data->DLCompleteTime));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLFaultCode:%d\n", __FUNCTION__, __LINE__, data->DLFaultCode));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]CurrentQueueIdx:%d\n", __FUNCTION__, __LINE__, data->CurrentQueueIdx));
		data->TransferQueue  = NULL;
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Init transfer queue...\n", __FUNCTION__, __LINE__));
		if(init_transfer_queue(&data->TransferQueue)==0)
			CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Init transfer queue fail!!\n", __FUNCTION__, __LINE__));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]Init transfer queue done.\n", __FUNCTION__, __LINE__));
#if defined(MTD_NAME_MAPPING) && defined(CONFIG_RTL_CWMP_CACERT_PARTITION)
		restore_cacert_from_partition();	
#endif
		//inform
		data->InformInterval = mib_get(MIB_CWMP_INFORM_INTERVAL, (void *)&uVal) ? uVal : 60;
		data->InformTime = mib_get(MIB_CWMP_INFORM_TIME, (void *)&uVal) ? uVal : 0;
		data->PeriodicInform = mib_get(MIB_CWMP_INFORM_ENABLE, (void *)&ch) ? ch : 1;
		data->EventCode= mib_get(MIB_CWMP_INFORM_EVENTCODE, (void *)&uVal) ? uVal : 0;
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Periodic inform enabled:%d\n", __FUNCTION__, __LINE__, data->PeriodicInform));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Periodic inform interval:%d\n", __FUNCTION__, __LINE__, data->InformInterval));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Periodic inform start time:%ld\n", __FUNCTION__, __LINE__, data->InformTime));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]EventCode:0x%08x\n", __FUNCTION__, __LINE__, data->EventCode));
		data->NotifyParameter=NULL;
		data->InformIntervalCnt = 0;
		//ScheduleInform
		data->ScheduleInformCnt = mib_get(MIB_CWMP_SI_DELAYTIME, (void *)&uVal) ? uVal : 0;
		if(data->ScheduleInformCnt)
			data->ScheduleInformEnabled = 1;
		mib_get(MIB_CWMP_SI_COMMANDKEY, (void *)buf);
		if(strcmp(buf, "")==0)
			data->SI_CommandKey = NULL;
		else
			data->SI_CommandKey = strdup(buf);
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Schedule inform delaytime:%d\n", __FUNCTION__, __LINE__, data->ScheduleInformCnt));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Schedule inform commandkey:%s\n", __FUNCTION__, __LINE__, data->SI_CommandKey));

		//Reboot
		data->RB_CommandKey = mib_get(MIB_CWMP_RB_COMMANDKEY, (void *)buf) ? strdup(buf) : NULL;
		data->Reboot = 0;
		data->HardReboot = 0;
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Reboot commandkey:%s\n", __FUNCTION__, __LINE__, data->RB_CommandKey));

		//FactoryReset
		data->FactoryReset = 0;

		// andrew
		data->url = mib_get(MIB_CWMP_ACS_URL, (void *)buf) ? strdup(buf) : strdup("");

		data->username = mib_get(MIB_CWMP_ACS_USERNAME, (void *)buf) ? strdup(buf) : NULL;
		data->password = mib_get(MIB_CWMP_ACS_PASSWORD, (void *)buf) ? strdup(buf) : NULL;
		//use the wan ip address as realm??
		data->conreq_username = mib_get(MIB_CWMP_CONREQ_USERNAME, (void *)buf) ? strdup(buf) : NULL;
		data->conreq_password = mib_get(MIB_CWMP_CONREQ_PASSWORD, (void *)buf) ? strdup(buf) : NULL;
		data->realm = CWMP_HTTP_REALM;
		data->server_port = mib_get(MIB_CWMP_CONREQ_PORT, (void *)&uVal) ? uVal : 7547;
		if (mib_get(MIB_CWMP_CONREQ_PATH, (void *)&buf[1])) {
			if (buf[1] != '/') {
				buf[0]='/';
				data->server_path = strdup( buf );
			}else
				data->server_path = strdup( &buf[1] );
		}else{
			data->server_path =  strdup("/");
		}
		/*data->machine = &cpe_client;*/
		data->redirect_url = NULL;
		data->redirect_count = 0;
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]ACS URL:%s\n", __FUNCTION__, __LINE__, data->url));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]ACS username:%s\n", __FUNCTION__, __LINE__, data->username));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]ACS password:%s\n", __FUNCTION__, __LINE__, data->password));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Connection username:%s\n", __FUNCTION__, __LINE__, data->conreq_username));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Connection password:%s\n", __FUNCTION__, __LINE__, data->conreq_password));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Port:%d\n", __FUNCTION__, __LINE__, data->server_port));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]Path:%s\n", __FUNCTION__, __LINE__, data->server_path));

		//certificate
#if defined(WITH_OPENSSL)
		data->ssl_type = 1;
#elif defined(_WITH_MATRIXSSL_)
		data->ssl_type = 2;
#else
		data->ssl_type = 0;
#endif
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]SSL type:%d\n", __FUNCTION__, __LINE__, data->ssl_type));
		data->cert_passwd = mib_get( MIB_CWMP_CERT_PASSWORD, (void *)buf ) ? strdup(buf) : NULL;
		{
			struct stat file_stat;
			
			if( stat( CERT_FNAME, &file_stat )<0 )
				data->cert_path = strdup(DEF_CERT_FN);
			else
				data->cert_path = strdup(CERT_FNAME);

			if( stat( CA_FNAME, &file_stat )<0 )
				data->ca_cert = strdup(DEF_CA_FN);
			else
				data->ca_cert = strdup(CA_FNAME);
		}
		
		data->url_changed = 0;
		
		data->inform_ct_ext = 0;

		if (mib_get(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL, (void *)&uVal)) {
			if (uVal == 0) {
				uVal = 5;
				mib_set(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL, (void *)&uVal);
			}
			data->retryMinWaitInterval = uVal;
		}
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]RetryMinWaitInterval:%d\n", __FUNCTION__, __LINE__, data->retryMinWaitInterval));

		if (mib_get(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER, (void *)&uVal)) {
			if (uVal == 0) {
				uVal = 2000;
				mib_set(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER, (void *)&uVal);
			}
			data->retryIntervalMutiplier = uVal;
		}
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]RetryIntervalMutiplier:%d\n", __FUNCTION__, __LINE__, data->retryIntervalMutiplier));

		// clear update bitmap
		data->update_bitmap = 0;
	}
	
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]****init cwmp userdata done!****\n", __FUNCTION__, __LINE__));
	return 0;
}

/*
 * update latest MIB into cwmp_userdata 
 * @is_discon: 
 * 		1 - cpe_state is disconencted with ACS
 *		0 - cpe_state is connecting with ACS
 * @mibId:
 *		MIB to be updated.
 */ 
int port_update_userdata( struct cwmp_userdata *data, int is_discon, int mibId )
{
	char buf[256 + 1];
	unsigned char ch, *pnew, *pold, vchar;
	unsigned int vUint=0, vint1, vint2, vint3;

	if(is_discon)
	{
		/* CPE disconnect with ACS, can safely update cwmp_userdata */
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]update mib %d to cwmp_userdata\n", __FUNCTION__, __LINE__, mibId));
		switch (mibId)
		{
		case MIB_CWMP_ACS_URL:
			if( mib_get(MIB_CWMP_ACS_URL, buf)!=0 )
			{
				if( (data->url==NULL) || (strcmp(data->url,buf)!=0) )
				{
					pnew = strdup(buf);
					pold = data->url;
					data->url = pnew;
					if(pold) free(pold);
					data->url_changed = 1;	/* will trigger EC_BOOTSTRAP in cwmp_process loop */
				}
			}
			break;
		case MIB_CWMP_ACS_USERNAME:
			if( mib_get(MIB_CWMP_ACS_USERNAME, buf)!=0 )
			{
				if( (data->username==NULL) || (strcmp(data->username,buf)!=0) )
				{
					pnew = strdup(buf);
					pold = data->username;
					data->username = pnew;
					if(pold) free(pold);
				}
			}
			break;
		case MIB_CWMP_ACS_PASSWORD:
			if( mib_get(MIB_CWMP_ACS_PASSWORD, buf)!=0 )
			{
				if( (data->password==NULL) || (strcmp(data->password,buf)!=0) )
				{
					pnew = strdup(buf);
					pold = data->password;
					data->password = pnew;
					if(pold) free(pold);
				}
			}
			break;
		case MIB_CWMP_INFORM_ENABLE:
		case MIB_CWMP_INFORM_INTERVAL:
		case MIB_CWMP_INFORM_TIME:
			mib_get(MIB_CWMP_INFORM_ENABLE, &vint3);
			mib_get(MIB_CWMP_INFORM_INTERVAL,&vint1);
			mib_get(MIB_CWMP_INFORM_TIME,&vint2);
			if( data->PeriodicInform!=vint3 ||
			    data->InformInterval!=vint1 ||
			    data->InformTime!=vint2 )
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("update the informinterval!\n" ));
				data->PeriodicInform = vint3;
				data->InformInterval = vint1;
				data->InformTime = vint2;
				cwmpMgmtSrvInformInterval();
			}
			break;
		case MIB_CWMP_CONREQ_USERNAME:
			if( mib_get(MIB_CWMP_CONREQ_USERNAME, buf)!=0 )
			{
				if( (data->conreq_username==NULL) || (strcmp(data->conreq_username,buf)!=0) )
				{
					pnew = strdup(buf);
					pold = data->conreq_username;
					data->conreq_username = pnew;
					if(pold) free(pold);
				}
			}
			break;
		case MIB_CWMP_CONREQ_PASSWORD:
			if( mib_get(MIB_CWMP_CONREQ_PASSWORD, buf)!=0 )
			{
				if( (data->conreq_password==NULL) || (strcmp(data->conreq_password,buf)!=0) )
				{
					pnew = strdup(buf);
					pold = data->conreq_password;
					data->conreq_password = pnew;
					if(pold) free(pold);
				}
			}
			break;
		case MIB_CWMP_RETRY_MIN_WAIT_INTERVAL:
			if( mib_get(MIB_CWMP_RETRY_MIN_WAIT_INTERVAL, &vUint)!=0 )
			{
				if (data->retryMinWaitInterval != vUint)
				{
					data->retryMinWaitInterval = vUint;
					cwmpRetryWaitTblInit(data);
				}
			}
			break;
		case MIB_CWMP_RETRY_INTERVAL_MUTIPLIER:
			if( mib_get(MIB_CWMP_RETRY_INTERVAL_MUTIPLIER, &vUint)!=0 )
			{
				if (data->retryIntervalMutiplier != vUint)
				{
					data->retryIntervalMutiplier = vUint;
					cwmpRetryWaitTblInit(data);
				}
			}
			break;
		default:
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("port update userdata: mibid %d doesn't need update\n", mibId));
			break;
		}
	}else{
		/* CPE is connected with ACS, can't update cwmp_userdata immediately */
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] can't update mib %d to cwmp_userdata in connection, record it in bitmap and update them in CPE disconnect\n", __FUNCTION__, __LINE__, mibId));
		switch (mibId)
		{
		case MIB_CWMP_ACS_URL:
			data->update_bitmap |= BIT(USERDATA_BITMAP_ACSURL) ;
			break;
		case MIB_CWMP_ACS_USERNAME:
			data->update_bitmap |= BIT(USERDATA_BITMAP_ACSUSERNAME) ;
			break;
		case MIB_CWMP_ACS_PASSWORD:
			data->update_bitmap |= BIT(USERDATA_BITMAP_ACSPASSWORD) ;
			break;
		case MIB_CWMP_INFORM_ENABLE:
		case MIB_CWMP_INFORM_INTERVAL:
		case MIB_CWMP_INFORM_TIME:
			data->update_bitmap |= BIT(USERDATA_BITMAP_PERIODIC_INFORM) ;
			break;
		case MIB_CWMP_CONREQ_USERNAME:
			data->update_bitmap |= BIT(USERDATA_BITMAP_CONREQ_USERNAME) ;
			break;
		case MIB_CWMP_CONREQ_PASSWORD:
			data->update_bitmap |= BIT(USERDATA_BITMAP_CONREQ_PASSWORD) ;
			break;
		case MIB_CWMP_RETRY_MIN_WAIT_INTERVAL:
			data->update_bitmap |= BIT(USERDATA_BITMAP_RETRY_MIN) ;
			break;
		case MIB_CWMP_RETRY_INTERVAL_MUTIPLIER:
			data->update_bitmap |= BIT(USERDATA_BITMAP_RETRY_MUTIPLIER) ;
			break;
		default:
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("port update userdata: mibid %d doesn't need update\n", mibId));
			break;
		}
	}

	return 0;
}

/*
 * port_update_userdata_by_bitmap
 *		Sync mib value to dirty userdata
 *		Must ONLY used in cpe_state CPE_ST_DISCONNECTED
 * @data: 
 *		If data->update_bitmap bit is set, update corresponding mib to cwmp_userdata
 *		The relationship between bit and mib is defined in libcwmp.h
 */
void port_update_userdata_by_bitmap( struct cwmp_userdata *data )
{
	if(data!=NULL && data->update_bitmap!=0)
	{
		/* dirty bitmap is set */
		int bitmap = data->update_bitmap;
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] update dirty userdata from MIB, dirty bitmap: %x\n", __FUNCTION__, __LINE__, bitmap));
		// ACS URL
		if(TESTBIT(bitmap, USERDATA_BITMAP_ACSURL))
		{
			port_update_userdata(data, 1, MIB_CWMP_ACS_URL);
		}
		// ACS username
		if(TESTBIT(bitmap, USERDATA_BITMAP_ACSUSERNAME))
		{
			port_update_userdata(data, 1, MIB_CWMP_ACS_USERNAME);
		}
		// ACS password
		if(TESTBIT(bitmap, USERDATA_BITMAP_ACSPASSWORD))
		{
			port_update_userdata(data, 1, MIB_CWMP_ACS_PASSWORD);
		}
		// periodic inform related
		if(TESTBIT(bitmap, USERDATA_BITMAP_PERIODIC_INFORM))
		{
			// all three of them are updated together
			port_update_userdata(data, 1, MIB_CWMP_INFORM_ENABLE);
		}
		// connection request username
		if(TESTBIT(bitmap, USERDATA_BITMAP_CONREQ_USERNAME))
		{
			port_update_userdata(data, 1, MIB_CWMP_CONREQ_USERNAME);
		}
		// connection request password
		if(TESTBIT(bitmap, USERDATA_BITMAP_CONREQ_PASSWORD))
		{
			port_update_userdata(data, 1, MIB_CWMP_CONREQ_PASSWORD);
		}
		// retry minimal interval
		if(TESTBIT(bitmap, USERDATA_BITMAP_RETRY_MIN))
		{
			port_update_userdata(data, 1, MIB_CWMP_RETRY_MIN_WAIT_INTERVAL);
		}
		// retry multiplier
		if(TESTBIT(bitmap, USERDATA_BITMAP_RETRY_MUTIPLIER))
		{
			port_update_userdata(data, 1, MIB_CWMP_RETRY_INTERVAL_MUTIPLIER);
		}

		data->update_bitmap = 0;
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] dirty bitmap(%x) is clear.\n", __FUNCTION__, __LINE__, data->update_bitmap));
	}
}

void port_save_reboot( struct cwmp_userdata *user, int reboot_flag )
{
	char acs_url[256 + 1];

	if( user )
	{
		//reboot commandkey
		if(user->RB_CommandKey)
			mib_set( MIB_CWMP_RB_COMMANDKEY, user->RB_CommandKey );
		else
			mib_set( MIB_CWMP_RB_COMMANDKEY, "" );
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]RB_CommandKey:%s\n", __FUNCTION__, __LINE__, user->RB_CommandKey));

		//scheduleinform delaytime and commandkey
		mib_set( MIB_CWMP_SI_DELAYTIME, (void *)&user->ScheduleInformCnt);
		
		if(user->SI_CommandKey)
			mib_set( MIB_CWMP_SI_COMMANDKEY, user->SI_CommandKey );
		else
			mib_set( MIB_CWMP_SI_COMMANDKEY, "" );
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]SI_DelayTime:%d\n", __FUNCTION__, __LINE__, user->ScheduleInformCnt));
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d]SI_CommandKey:%s\n", __FUNCTION__, __LINE__, user->SI_CommandKey));
		
		//transfer information
		if(user->DLCommandKey)
			mib_set( MIB_CWMP_DL_COMMANDKEY, user->DLCommandKey );
		else
			mib_set( MIB_CWMP_DL_COMMANDKEY, "" );	
		mib_set( MIB_CWMP_DL_STARTTIME, &user->DLStartTime );
		mib_set( MIB_CWMP_DL_COMPLETETIME, &user->DLCompleteTime );
		mib_set( MIB_CWMP_DL_FAULTCODE, &user->DLFaultCode );
		mib_set( MIB_CWMP_DL_QUEUE_IDX, &user->CurrentQueueIdx);
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLCommandKey:%s\n", __FUNCTION__, __LINE__, user->DLCommandKey));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLStartTime:%lu\n", __FUNCTION__, __LINE__, user->DLStartTime));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLCompleteTime:%lu\n", __FUNCTION__, __LINE__, user->DLCompleteTime));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]DLFaultCode:%d\n", __FUNCTION__, __LINE__, user->DLFaultCode));
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]CurrentQueueIdx:%d\n", __FUNCTION__, __LINE__, user->CurrentQueueIdx));
		/*
		 * port_save_transfer_queue is done in each necessary location. Don't do it for every cwmp_SaveReboot().
		 */ 

		//inform event code
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] set Eventcode 0x%08x to mib\n", __FUNCTION__, __LINE__, user->EventCode));
		mib_set( MIB_CWMP_INFORM_EVENTCODE, &user->EventCode );
		
	}


	//CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>\n", __FUNCTION__, __LINE__ );fflush(stderr));
	apmib_update(CURRENT_SETTING);
	//CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s:%d>\n", __FUNCTION__, __LINE__ );fflush(stderr));

	if(reboot_flag)
	{
#ifdef MULTI_WAN_SUPPORT
        if(user->HardReboot)
        {
            printf("***Device Reboot By ACS***\n");
            system("reboot");
            exit(0);
        }else {
        	printf(" --> cmd_reboot [%s %d]\n", __FUNCTION__,__LINE__);
            cmd_reboot();
        }
#else
        {
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s:%d>The system is restarting ...\n", __FUNCTION__, __LINE__ ) );
            cmd_reboot();
            exit(0);
        }
#endif
	}

}

void port_factoryreset_reboot(void)
{
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_DETAILED, ( "<%s:%d>\n", __FUNCTION__, __LINE__ ) ); 	
	va_cmd("/bin/flash", 1, 1, "default-sw");

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s:%d>The system is restarting ...\n", __FUNCTION__, __LINE__ ) );
#ifdef CONFIG_BOA_WEB_E8B_CH
	sleep(5); //wait finishing setdefault configuration for 16M flash
#endif
}

int port_before_download( int file_type, char *target )
{
	CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_ERROR, ( "<%s:%d> file type:%d, target:%s\n", __FUNCTION__, __LINE__, file_type, target?target:"" ));

	if(target==NULL) return -1;

	switch( file_type )
	{
	case DLTYPE_IMAGE:
		strcpy( target, "/tmp/vm.img" );
		break;
//	case DLTYPE_WEB: //not support right now
//		strcpy( target, "/tmp/web.bin" );
//		break;
	case DLTYPE_CONFIG:	
		strcpy( target, "/tmp/config.xml" );
		break;
	}
	
	return 0;
}

int port_after_download( int file_type, char *target )
{

	switch(file_type)
	{
		CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>file_type = %d\n", __FUNCTION__, __LINE__, file_type ) );
	case DLTYPE_IMAGE:
		//update firmware
			if( upgradeFirmware( target ) ) //return 0: success
			{
				CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>Image Checksum Failed!\n", __FUNCTION__, __LINE__ ) );
				return ERR_9010; 
			}
		break;
	case DLTYPE_CONFIG: 	
		if(upgradeConfig(target) < 0 ){
			CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>Config Checksum Failed!\n", __FUNCTION__, __LINE__ ) );
			return ERR_9010; 
		}		

		break;
	}
	return 0;
}


int port_before_upload( int file_type, char *target )
{
	if( target==NULL ) return -1;

	switch(file_type)
	{
	case DLTYPE_CONFIG:
		strcpy( target, "/web/config.dat" );
		break;
	
	case DLTYPE_LOG:
		strcpy( target, "/var/log/messages" );
		break;
	}

	CWMPDBG_FUNC(MODULE_CORE|MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d> file type:%d, target:%s\n", __FUNCTION__, __LINE__, file_type, target?target:"" ));

	return 0;
}


int port_after_upload( int file_type, char *target )
{
	if( target==NULL ) return -1;

	//remove the target file
	switch(file_type)
	{
	case DLTYPE_CONFIG:
 		break;
	case DLTYPE_LOG:
		//not have to remove the log file
		break;
	}

	return 0;	
}

/*
 * Add DeviceID field in Inform packet
 */
void port_add_default_inform_device(struct soap *soap, struct cwmp__Inform *req)
{
	int vtype;
	void *vdata;
	unsigned char name[256+1] = {0};
	
	sprintf(name, "%s.DeviceInfo.Manufacturer", DATA_MODEL_PREFIX);
	get_ParameterValue( name, &vtype, &vdata );
	req->DeviceId.Manufacturer = cwmp_valuedup( soap, vtype, vdata);
	if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	memset(name, 0x0, sizeof(name));

	sprintf(name, "%s.DeviceInfo.ManufacturerOUI", DATA_MODEL_PREFIX);
	get_ParameterValue( name, &vtype, &vdata );
	req->DeviceId.OUI = cwmp_valuedup( soap, vtype, vdata);
	if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	memset(name, 0x0, sizeof(name));

	/*Works system ACS needs this filed*/
	sprintf(name, "%s.DeviceInfo.ProductClass", DATA_MODEL_PREFIX);
	get_ParameterValue( name, &vtype, &vdata );
	req->DeviceId.ProductClass = cwmp_valuedup( soap, vtype, vdata);//this is an optional parameter
	if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	memset(name, 0x0, sizeof(name));

	sprintf(name, "%s.DeviceInfo.SerialNumber", DATA_MODEL_PREFIX);
	get_ParameterValue( name, &vtype, &vdata );
	req->DeviceId.SerialNumber = cwmp_valuedup( soap, vtype, vdata);
	if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	memset(name, 0x0, sizeof(name));
}

void port_MgmtSrvInformInterval_init(struct cwmp_userdata *pUserData)
{
	int vtype;
	void *vdata;
	unsigned char name[256+1] = {0};
	
	if(pUserData == NULL)
		return;
	
	sprintf(name, "%s.ManagementServer.PeriodicInformEnable", DATA_MODEL_PREFIX);
	if( get_ParameterValue( name, &vtype, &vdata )==0 )
	{
		pUserData->PeriodicInform = *(int*)vdata;
		if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	}else
		pUserData->PeriodicInform = 1;
	memset(name, 0x0, sizeof(name));
	
	sprintf(name, "%s.ManagementServer.PeriodicInformInterval", DATA_MODEL_PREFIX);
	if( get_ParameterValue( name, &vtype, &vdata )==0 )
	{
		pUserData->InformInterval = *(unsigned int*)vdata;
		if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	}else
		pUserData->InformInterval = 300;
	memset(name, 0x0, sizeof(name));

	sprintf(name, "%s.ManagementServer.PeriodicInformTime", DATA_MODEL_PREFIX);
	if( get_ParameterValue( name, &vtype, &vdata )==0 )
	{
		pUserData->InformTime = *(time_t*)vdata;
		if(vdata) get_ParameterValueFree(vtype, vdata); //free( vdata );
	}else
		pUserData->InformTime = 0;
}

/* Add user defined Inform RPC parameters */
int port_add_user_inform_prmt(struct node **prmt, unsigned int event, unsigned int user_event)
{
	char *name = NULL;

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("event=0x%x, user_event=0x%x\n", event, user_event));

	if(prmt == NULL)
		return -1;

	/*
	 * Following code is an example
	 * Call push_node_data(prmt, name) to push user defined Inform parameter list member to prmt
	 */
#ifdef CONFIG_E8B
	char *nameGet = NULL, *cmp = NULL;
	int t;
	void *d;
	if (get_ParameterName("InternetGatewayDevice.WANDevice.", 0, &nameGet) == 0) {
		do {
			if (user_event & EC_X_CT_COM_BIND_1) {
				if (strstr(nameGet, ".WANPPPConnection.") && strstr(nameGet, ".Username")) {
					if (get_ParameterValue(nameGet, &t, &d) == 0) {
						CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s:%d> Add [%s]\n", __FUNCTION__, __LINE__, nameGet));
						name = strdup(nameGet);
						if (name) push_node_data(prmt, name);
						if (d) get_ParameterValueFree(t, d);
					}
				}
			}
		} while (get_ParameterName(NULL, 0, &nameGet) == 0);
	}
#endif

	return 0;
}

/*
 * startup misc thread, such as STUN, udpEchoServer.
 */
void port_misc_thread_startup()
{
    // STUN thread
#ifdef TR069_ANNEX_G
	{
		unsigned int vInt = 0;
		apmib_get(MIB_CWMP_STUN_EN, (void *)&vInt);
		printf("[%s:%d] stun_en=%d\n", __FUNCTION__, __LINE__, vInt);
		if (vInt)
			cwmpStartStun();
		else
			cwmpStopStun();
	}
#endif
}

/* Clear user defined Inform RPC parameters */
int port_clear_user_inform_prmt(struct node **prmt)
{
	struct node *node = *prmt;
	char *name = NULL;

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("<%s:%d>\n", __FUNCTION__, __LINE__));

	// nothing to be free
	if(prmt == NULL || *prmt == NULL)
		return 0;

	while(*prmt)
	{
		name = (char *)pop_node_data(prmt);
		if(name)
			free(name);
	}

	return 0;
}
int port_session_closed(struct cwmp_userdata *data)
{
	unsigned int delay=3;
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gStartPing:%d\n", __FUNCTION__, __LINE__, gStartPing));
	if (gStartPing)
	{
#ifdef __CLOSE_INTERFACE_BEFORE_WRITE_
		/*delay some seconds to ready*/
		{while(delay!=0) delay=sleep(delay);}
#endif /*__CLOSE_INTERFACE_BEFORE_WRITE_*/
		cwmpStartPingDiag();
		gStartPing = 0;
	}	

#ifdef _PRMT_TR143_
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gStartTR143DownloadDiag:%d, gStartTR143UploadDiag:%d\n", __FUNCTION__, __LINE__, gStartTR143DownloadDiag,gStartTR143UploadDiag));
	if(gStartTR143DownloadDiag)
	{
#ifdef __CLOSE_INTERFACE_BEFORE_WRITE_
		/*delay some seconds to ready*/
		{while(delay!=0) delay=sleep(delay);}
#endif /*__CLOSE_INTERFACE_BEFORE_WRITE_*/
		StartTR143DownloadDiag();
		gStartTR143DownloadDiag=0;
	}else if(gStartTR143UploadDiag)
	{
#ifdef __CLOSE_INTERFACE_BEFORE_WRITE_
		/*delay some seconds to ready*/
		{while(delay!=0) delay=sleep(delay);}
#endif /*__CLOSE_INTERFACE_BEFORE_WRITE_*/
		StartTR143UploadDiag();
		gStartTR143UploadDiag=0;
	}
#endif //_PRMT_TR143_

#ifdef TR069_ANNEX_G
    extern int gRestartStun;
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gRestartStun:%d\n", __FUNCTION__, __LINE__, gRestartStun));
    if(gRestartStun)
	{
		unsigned int vInt = 0;
		apmib_get(MIB_CWMP_STUN_EN, (void *)&vInt);
		printf("[%s:%d] stun_en=%d\n", __FUNCTION__, __LINE__, vInt);
		if (vInt)
			cwmpStartStun();
		else
			cwmpStopStun();
		
		gRestartStun=0;
	}
#endif

#ifdef _SUPPORT_TRACEROUTE_PROFILE_  
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gStartTraceRouteDiag:%d\n", __FUNCTION__, __LINE__, gStartTraceRouteDiag));
	if(gStartTraceRouteDiag)
	{
	#ifdef __CLOSE_INTERFACE_BEFORE_WRITE_
		/*delay some seconds to ready*/
		{while(delay!=0) delay=sleep(delay);}
	#endif /*__CLOSE_INTERFACE_BEFORE_WRITE_*/
		StartTraceRouteDiag();
		gStartTraceRouteDiag=0;
  }
#endif //_SUPPORT_TRACEROUTE_PROFILE_

#if 1 
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] gStartReset:%d\n", __FUNCTION__, __LINE__, gStartReset));
	if(gStartReset)
	{
	#ifdef __CLOSE_INTERFACE_BEFORE_WRITE_
		/*delay some seconds to ready*/
		{while(delay!=0) delay=sleep(delay);}
	#endif /*__CLOSE_INTERFACE_BEFORE_WRITE_*/
		cwmpStartReset();
		gStartReset=0;
	}
#endif

#ifdef CONFIG_USE_XML
	unlink(CONFIG_FILE_NAME);
#endif	

#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
	unlink(FILE4CaptivePortal);
#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_
#ifdef SUPPORT_DHCP_RESERVED_IPADDR
	unlink(FILE4DHCPReservedIPAddr);
#endif //SUPPORT_DHCP_RESERVED_IPADDR
	
	//update the userdata
	
	return 0;
}

int port_backup_config( void )
{
	int ret=0;
	CWMPDBG( _SET_ATTR, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	
	ret = mib_backup(1);
	
	CWMPDBG( _SET_ATTR, ( stderr, "<%s:%d> ret=%d\n", __FUNCTION__, __LINE__,ret ) );
	
	return ret;
}

int port_restore_config(int restore)
{
	int ret=0;
	CWMPDBG( _SET_ATTR, ( stderr, "<%s:%d> \n", __FUNCTION__, __LINE__) );

	ret=mib_restore(restore);	
	CWMPDBG( _SET_ATTR, ( stderr, "<%s:%d> ret=%d\n", __FUNCTION__, __LINE__,ret ) );

	return ret;
}

/***********************************************************************
 ***************  H T T P  C l i e n t  U t i l i t i e s  *************
 ***********************************************************************/
static void save_download_upload_percent_status(char * file_name,int persent)
{  // 0 ~ completed,   1 ~ doing,   1-100 ~ download percent
	FILE * fp;
	char name[128];
	if(strstr(file_name,"vm.img"))
		sprintf(name,"/var/tr069_download_fw_status");
	else if (strstr(file_name,"/web/config.dat"))
		sprintf(name,"/var/tr069_upload_config_status");
	else if (strstr(file_name,"config"))
		sprintf(name,"/var/tr069_download_config_status");
	else	
		sprintf(name,"/var/tr069_downupload_status",file_name);
	//CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("****%s %d: name=%s persent=%d file_name=%s****\n",__FUNCTION__,__LINE__,name,persent,file_name));
							
	fp = fopen(name, "w");
    if (fp) {
        fprintf(fp, "%d", persent);
    }
	else{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("open download percent file fail!\n"));
	}
    fclose(fp);
}

static void http_client_end(struct soap *soap)
{
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if(soap)
	{
		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
#ifdef WITH_OPENSSL
		CRYPTO_thread_cleanup();
#endif
	}
}

static int http_client_init( struct soap *soap, void* data )
{
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>\n", __FUNCTION__, __LINE__) );
	
	if( soap==NULL ) return -1;

#ifdef CWMP_ENABLE_SSL
	soap_ssl_init();
#if defined(WITH_OPENSSL)
	if (CRYPTO_thread_setup())
	{ 
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ( "Cannot setup thread mutex\n"));
		return -1;
	}
#endif
#endif

	soap_init(soap);
	soap->connect_timeout=10;
	soap->recv_timeout=10;
	soap->user=data;	

	if (soap_register_plugin(soap, http_da))
		goto http_client_init_err;

#ifdef CWMP_ENABLE_SSL
	if( certificate_setup( soap, 0 )<0 ) //gNeedSSLAuth, looser for download
		goto http_client_init_err;
#endif /*#ifdef CWMP_ENABLE_SSL*/

	return 0;	

http_client_init_err:
	soap_print_fault(soap, stderr);
	http_client_end(soap);
	return -1;
}

static int http_send( struct soap *soap, char *url, char *target, int action )
{
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if( soap_begin_count(soap) || soap_end_count(soap) )
		return soap->error;	

	switch( action )
	{	
	case SOAP_PUT:
		{
		 struct stat st;
		 if( stat( target, &st)==0 ) soap->count = st.st_size;
		 soap->http_content = "text/plain";
		}
		break;

	case SOAP_GET:
		break;

	default:
		return -1;
	}

	if( soap_connect_command(soap, action, url, NULL) )
	{
		soap_print_fault(soap, stderr);
		return -1;
	}

	switch(action)
	{
	case SOAP_PUT:
		{
			FILE *fp=NULL;
			char rbuf[64];
			int  rsize=0;
			int sendfail=0;			

			fp = fopen( target, "r" );
			if(fp)
			{
				while( (rsize=fread( rbuf, 1, 64, fp )) > 0  )
				{					
					if(soap_send_raw( soap,rbuf,rsize ))
					{
						sendfail=1;
						break;
					}
				}
				fclose( fp );
			}
			
			if(sendfail) CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ( "SOAP_PUT FAILURE(errno=%d)!!!\n", soap->error ));

			//save download status in file
			save_download_upload_percent_status(target,1);//save upload status

			if( soap_end_send(soap) )
			{
				soap_print_fault(soap, stderr);
				if(sendfail) 
					return 0;//give an chance to receive the http response
				else
					return -1;
			}
			//save download status in file
			save_download_upload_percent_status(target,0);//save upload status
		}
		break;

	case SOAP_GET:
		break;
	}

/* //using get result in memory fault, why???
	if( soap_end_send(soap) )
	{
		soap_print_fault(soap, stderr);
		return -1;
	}
*/
	return 0;

}

static int http_recv( struct soap *soap, char *target, int action )
{
	extern char *fwDownloadPtr;
	int	ret=-1;
	/*ql:20080729 START: check image key according to IC version*/
	unsigned int key;
	/*ql:20080729 END*/
	struct stat statbuf;
	int download_len = 0;
	int download_per = 0;

	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if( soap_begin_recv(soap) )
		return soap->error;

	switch(action)
	{
	case SOAP_GET:
		{
			unsigned int fileSize=0;
			unsigned int allocSize=0;
			unsigned int nbytes = 256;
			int resLenBytes = 4;
							
			fileSize = soap->length;
			if(fwDownloadPtr != NULL)
			{
				free(fwDownloadPtr);
			}
			
			allocSize = (fileSize + nbytes + 1 + resLenBytes) * sizeof(char);
			fwDownloadPtr = realloc( fwDownloadPtr, allocSize);

			if(fwDownloadPtr != NULL)
			{
				#define BATCHSIZE 4096
				unsigned char buf[BATCHSIZE];
				int c,buf_index=0;
				char *filePtr;

				memset(fwDownloadPtr,0x00,allocSize);
				memcpy(fwDownloadPtr,&fileSize,resLenBytes); //reserve first [resLenBytes] bytes to record the firmware length

				filePtr = fwDownloadPtr+resLenBytes;

				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ( "content-length:%d <SOAP_BUFLEN:%d>\n", soap->length,SOAP_BUFLEN ));
				memset(buf,0x00,BATCHSIZE);

				while( (c=soap_get1(soap))!=EOF )
				{
					buf[buf_index]=c;

					buf_index++;
					if(buf_index==BATCHSIZE)
					{
						memcpy(filePtr,buf,buf_index);
						filePtr+=buf_index;
						
						download_len += buf_index;
						if(download_len * 100 > (download_per + 5)* fileSize){//very 5% save status
							download_per = (download_len*100)/fileSize;
							//save download status in file
							save_download_upload_percent_status(target,download_per);										
						}
						
						buf_index=0;
						memset(buf,0x00,BATCHSIZE);
					}
				}	
				//save download status in file	
				save_download_upload_percent_status(target,0);//download completed ,save 0

				if(buf_index)
				{
					memcpy(filePtr,buf,buf_index);
					filePtr+=buf_index;
				}
				ret=0;
			}
			else
			{
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ("\r\n realloc size [%u] fail!!,  __[%s-%u]",allocSize,__FILE__,__LINE__));
				ret=-1;
			}
		}
		break;
	case SOAP_PUT:
		{
			ret=0;
		}
		break;
	}

	soap_end_recv(soap);		

	return ret;
}

/*
 * http_client:
 *		HTTP file transfer client.
 *		Support None/Basic/Digest authentication to HTTP file server.
 */
int http_client(char *url, char *name, char *passwd, char *filename, int action, void *data)
{
	struct soap c_soap;
	int auth_type = CPE_AUTH_NONE;
	int ret = -1;

	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("<%s:%d>start,<name:%s>,<passwd:%s><filename:%s>\n",
		 __FUNCTION__, __LINE__, name, passwd, filename));

	//init
	if (http_client_init(&c_soap, data))
		return ERR_9002;

	soap_begin(&c_soap);

	// no auth at first time
	c_soap.userid = NULL;
	c_soap.passwd = NULL;

	if (http_send(&c_soap, url, filename, action) == 0) //success
	{
		if (http_recv(&c_soap, filename, action))
		{
			if (c_soap.error == 401)
			{
				// try to authentication
				// this code path support both basic & digest auth.
				struct http_da_info info = {0};

				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] retry using basic/digest auth., <name:%s>,<passwd:%s><realm:%s>\n",
					 __FUNCTION__, __LINE__, name, passwd, c_soap.authrealm));

				http_da_save(&c_soap, &info, c_soap.authrealm, name, passwd);
				if (http_send(&c_soap, url, filename, action) == 0)
				{
					if (http_recv(&c_soap, filename, action))
					{
						ret = ERR_9012;
					}
					else
					{
						ret = 0; //OK
					}
				}
				http_da_release(&c_soap, &info);
			}
			else
				soap_print_fault(&c_soap, stderr);

		}
		else
		{
			ret = 0;	//OK
		}
	}

	/*clear the connection */
	c_soap.keep_alive = 0;
	soap_closesock(&c_soap);
	soap_destroy(&c_soap);
	soap_end(&c_soap);

#ifdef WITH_OPENSSL
	if (c_soap.session) {
		SSL_SESSION_free(c_soap.session);
		c_soap.session = NULL;
	}

	//char cmd[32] = {0};
	//sprintf(cmd, "echo 1 > /proc/rg/hwnat");
	//system(cmd);
#endif

	http_client_end(&c_soap);
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ( "<%s:%d> done. ret=%d\n", __FUNCTION__, __LINE__, ret));

	return ret;
}

/***********************************************************************
 ***********  E N D  H T T P  C l i e n t  U t i l i t i e s  **********
 ***********************************************************************/

/***********************************************************************
 ****************  F T P  C l i e n t  U t i l i t i e s  **************
 ***********************************************************************/
/*
 * ftp_client:
 * Return value:
 *		-1: failure, 0: succeed.
 */
int ftp_client(char *url, char *name, char *passwd, char *filename, int action, void *data)
{
	/* arguments validation */
	if(!url || strncmp(url, "ftp://", strlen("ftp://"))!=0 || (action!=FTP_GET && action!=FTP_PUT) )
	{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] arguments invalid: url=%s\nusername=%s\npassword=%s\nfilename=%s\naction=%d\n", __FUNCTION__, __LINE__,
			url, name, passwd, filename, action));
		return -1;
	}
	const char *FTP_STATUE_FILE = "/var/cwmp_ftp_transfer_status.txt";
	const char *FTP_COMMAND_FILE = "/var/cwmp_ftp_command.sh";
	char ftp_cmd[ 32 ] = {0} ; //strlen(FTP_COMMAND_FILE) + 4
	FILE *fd = NULL;
	char ftp_transfer_status = '\0';

	/* 
	 * parse url into ftpget/ftpput arguments 
	 * example URL: ftp://172.29.36.84:21/tmp/fw.bin
	 */
	char *pHost=NULL, *pPort=NULL, *pPath=NULL, *pFile=NULL;
	pHost=url+6;
	pPath=strchr(pHost, '/');
	if(pPath==NULL) 
		return -1;
	pPath[0]=0;
	pPath++;
	pFile=strrchr( pPath, '/');
	if(pFile==NULL)
	{
		pFile=pPath;
		pPath=NULL;
	}else{
		pFile[0]=0;
		pFile++;
	}
	pPort=strchr( pHost, ':');
	if(pPort)
	{
		pPort[0]=0;
		pPort++;
		if( strlen(pPort)==0 ) 
			pPort=NULL;
	}
	CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_ERROR, ("[%s:%d] url=%s\npHost=%s pPort=%s pPath=%s pFile=%s \n",__FUNCTION__, __LINE__, url,pHost,pPort,pPath,pFile));
	if( pHost==NULL || strlen(pHost)==0 ||
		pFile==NULL || strlen(pFile)==0 )
		return -1;

	/* generate FTP transfer command */
	fd = fopen(FTP_COMMAND_FILE, "w");
	if( fd!=NULL )
	{
		switch (action)
		{
		case FTP_GET:
			/* download */
			/* ftpget will store file with same name as filename in URL in /tmp/ */
			if(pPath){
                if(pPort==NULL)
                    fprintf( fd, "/bin/ftpget -u %s -p %s %s /tmp/%s %s/%s; \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pHost,pFile,pPath,pFile, FTP_STATUE_FILE, FTP_STATUE_FILE);
                else
                    fprintf( fd, "/bin/ftpget -u %s -p %s -P %s %s /tmp/%s %s/%s; \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pPort,pHost,pFile,pPath,pFile, FTP_STATUE_FILE, FTP_STATUE_FILE);
            }
			else{
                if(pPort==NULL)
                    fprintf( fd, "/bin/ftpget -u %s -p %s %s /tmp/%s %s; \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pHost,pFile,pFile, FTP_STATUE_FILE, FTP_STATUE_FILE);
                else
                    fprintf( fd, "/bin/ftpget -u %s -p %s -P %s %s /tmp/%s %s; \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pPort,pHost,pFile,pFile, FTP_STATUE_FILE, FTP_STATUE_FILE);
            }
			break;
			
		case FTP_PUT:
			/* upload */
			if(pPath){
                if(pPort==NULL)
                    fprintf( fd, "/bin/ftpput -u %s -p %s %s %s/%s %s \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pHost,pPath,pFile,filename, FTP_STATUE_FILE, FTP_STATUE_FILE);
                else
                    fprintf( fd, "/bin/ftpput -u %s -p %s -P %s %s %s/%s %s \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pPort,pHost,pPath,pFile,filename, FTP_STATUE_FILE, FTP_STATUE_FILE);
            }
			else{
                if(pPort==NULL)
                    fprintf( fd, "/bin/ftpput -u %s -p %s %s %s %s \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pHost,pFile,filename, FTP_STATUE_FILE, FTP_STATUE_FILE);
                else
                    fprintf( fd, "/bin/ftpput -u %s -p %s -P %s %s %s %s \n\
                                if [ $? == 0 ]; \n\
                                then echo 1 >%s \n\
                                else \n\
                                echo 0 >%s\n\
                                fi\n",name,passwd,pPort,pHost,pFile,filename, FTP_STATUE_FILE, FTP_STATUE_FILE);
            }
			break;
			
		default:
			CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Abnormal situation: action %d is invalid!!\n", __FUNCTION__, __LINE__, action));
			break;
		}
		fclose(fd);
	}else{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] open FTP command file %s fail!!\n", __FUNCTION__, __LINE__, FTP_COMMAND_FILE));
		return -1;
	}

	/* do FTP transfer */
	sprintf(ftp_cmd, ". %s", FTP_COMMAND_FILE);
    CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d]%s\n", __FUNCTION__, __LINE__, ftp_cmd));
    system(ftp_cmd);

	/* pending for FTP transfer complete */
	while((fd=fopen(FTP_STATUE_FILE, "r"))==NULL){
		/* transfer isn't complete. FTP_STATUE_FILE doesn't exist! */
		sleep(1);
	}
	if(fd!=NULL){
        fread(&ftp_transfer_status, 1, 1, fd);
        fclose(fd);
	}else{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Abnormal situation: fd shouldnot be NULL here!!\n", __FUNCTION__, __LINE__));
		return -1;
	}

	/* handle transfer result */
	if(ftp_transfer_status=='0')
	{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] FTP Transfer fail!!\n", __FUNCTION__, __LINE__));
		return -1;
	}else if(ftp_transfer_status=='1'){
		int rsize,fsize,lsize;
		char *target_filename = NULL;
		extern char *fwDownloadPtr;
		
		/* transfer succeed */
		switch (action)
		{
		case FTP_GET:
			if(fwDownloadPtr != NULL){
				free(fwDownloadPtr);
				fwDownloadPtr = NULL;
			}
			CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] done FTP download.\n", __FUNCTION__, __LINE__));
			target_filename = (char *)malloc(strlen(pFile) + strlen("/tmp/") + 1);
			if(target_filename)
			{
				sprintf(target_filename,"/tmp/%s",pFile);
				fd = fopen(target_filename, "rb" );
				if(fd){
					fseek (fd, 0, SEEK_END);
		       		fsize = ftell (fd); 
					fseek(fd,0,SEEK_SET);   
					lsize = 4;
					fwDownloadPtr = malloc(fsize + lsize + 256  + 1);
					if(fwDownloadPtr == NULL) 
					{
						CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] malloc %d bytes for fwDownloadPtr fail!!\n", __FUNCTION__, __LINE__, (fsize + lsize + 256  + 1)));
						return -1;
					}
					memset(fwDownloadPtr,0,fsize);
					memcpy(fwDownloadPtr,&fsize,4); //store firmware length at the head of fwDownloadPtr
					rsize = fread(fwDownloadPtr + lsize, 1, fsize, fd );
					CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] done fread FTP download file read to fwDownloadPtr, filesize=%d, readsize=%d\n",__FUNCTION__,__LINE__,fsize,rsize));
					fclose( fd );

					return 0;
				}else{
					CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Error: ftpget result file %s open fail!!\n", __FUNCTION__, __LINE__, target_filename));
					return -1;
				}
			}else{
				CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Error: malloc target filename fail, pFile=%s, malloc size:%d\n", __FUNCTION__, __LINE__, pFile, (strlen(pFile) + strlen("/tmp/") + 1)));
				return -1;
			}
			break;

		case FTP_PUT:
			CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] done FTP upload.\n", __FUNCTION__, __LINE__));
			return 0;

		default:
			CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Abnormal situation: action %d is invalid!!\n", __FUNCTION__, __LINE__, action));
			return -1;
		}
	}else{
		CWMPDBG_FUNC(MODULE_FILE_TRANS, LEVEL_INFO, ("[%s:%d] Abnormal situation: ftp transfer status 0x%x invalid!!\n", __FUNCTION__, __LINE__, ftp_transfer_status));
		return -1;
	}
}

/***********************************************************************
 *************  E N D  F T P  C l i e n t  U t i l i t i e s  **********
 ***********************************************************************/


#ifdef CONFIG_BOA_WEB_E8B_CH
int isTR069(char *name)
{
	unsigned int devnum,ipnum,pppnum;
	WANIFACE_T Entry;
	unsigned int total;
	int i;

	devnum = getWANConDevInstNum( name );
	ipnum  = getWANIPConInstNum( name );
	pppnum = getWANPPPConInstNum( name );

	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);
	for(i=1;i<=total;i++){
		
		*((char *)&Entry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)&Entry))
			continue;
		
		if(Entry.ConDevInstNum==devnum 
			&& Entry.ConIPInstNum==ipnum 
			&& Entry.ConPPPInstNum==pppnum
			&& (Entry.applicationtype==APPTYPE_TR069_INTERNET || Entry.applicationtype==APPTYPE_TR069)){
			return 1;
		}	
	}

	return 0;

}
int isINTERNET(char *name)
{
	unsigned int devnum,ipnum,pppnum;
	WANIFACE_T Entry;
	unsigned int total;
	int i;

	devnum = getWANConDevInstNum( name );
	ipnum  = getWANIPConInstNum( name );
	pppnum = getWANPPPConInstNum( name );

	mib_get(MIB_WANIFACE_TBL_NUM, (void *)&total);

	for(i=1;i<=total;i++){
		*((char *)&Entry) = (char)i;
		if(!mib_get(MIB_WANIFACE_TBL, (void *)&Entry))
			continue;
		
		if(Entry.ConDevInstNum==devnum 
			&& Entry.ConIPInstNum==ipnum 
			&& Entry.ConPPPInstNum==pppnum
			&& (Entry.applicationtype==APPTYPE_TR069_INTERNET || Entry.applicationtype==APPTYPE_INTERNET)){
			return 1;
		}	
	}

	return 0;


}
#endif

