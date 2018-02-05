#include "rtk_btconfig_api.h"
#include "./common/rtk_wlan.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string.h>
#include <time.h>
#include <linux/wireless.h>
#include <linux/if.h>
#include <linux/if_ether.h> 
#include <linux/if_pppox.h> 
#include <linux/if_packet.h> 
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdarg.h>
#include <ieee802_mib.h>
#include <apmib.h>
//#include <dirent.h>
#define PRESENT_TIME_LENGTH 30
extern int seq_number;

int ba2str(const bdaddr_t *ba, char *str)
{
	return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
		ba->b[5], ba->b[4], ba->b[3], ba->b[2], ba->b[1], ba->b[0]);
}
int bachk(const char *str)
{
	if (!str)
		return -1;

	if (strlen(str) != 17)
		return -1;

	while (*str) {
		if (!isxdigit(*str++))
			return -1;

		if (!isxdigit(*str++))
			return -1;

		if (*str == 0)
			break;

		if (*str++ != ':')
			return -1;
	}

	return 0;
}
int str2ba(const char *str, bdaddr_t *ba)
{
	int i;

	if (bachk(str) < 0) {
		memset(ba, 0, sizeof(*ba));
		return -1;
	}

	for (i = 5; i >= 0; i--, str += 3)
		ba->b[i] = strtol(str, NULL, 16);

	return 0;
}

int SetWlan_idx(char * wlan_iface_name)
{
	int idx;
	idx = atoi(&wlan_iface_name[4]);
	if (idx >= NUM_WLAN_INTERFACE) {
		RTK_BTCONFIG_ERR("idx:%d,NUM_WLAN_INTERFACE:%d\n",idx,NUM_WLAN_INTERFACE);
		RTK_BTCONFIG_ERR("invalid wlan interface index number!\n");
		return 0;
	}
	wlan_idx = idx;
	vwlan_idx = 0;

#ifdef MBSSID		
	if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
		wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
		idx = atoi(&wlan_iface_name[8]);
		if (idx >= NUM_VWLAN_INTERFACE) {
			printf("invalid virtual wlan interface index number!\n");
			return 0;
		}
		
		vwlan_idx = idx+1;
		idx = atoi(&wlan_iface_name[4]);
		wlan_idx = idx;
	}
#endif		

#ifdef UNIVERSAL_REPEATER
		if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
				!memcmp(&wlan_iface_name[6], "vxd", 3)) {
			vwlan_idx = NUM_VWLAN_INTERFACE;
			idx = atoi(&wlan_iface_name[4]);
			wlan_idx = idx;
		}
#endif				
	return 1;		
}





int char_to_hex(char *string,int len)
{
	char c;
	int i;
	char tmp[8];
	char output[32] = {0};
	if(string == NULL || len <= 0)
	{
		return -1;
	}
	for(i=0;i<len;i++)
	{
		c = string[i];
		sprintf(tmp,"%x",c);
		if(strlen(output) == 0)
		{
			strcpy(output,tmp);
		}
		else
		{
			strcat(output,tmp);
		}
	}
	strcpy(string,output);
}

static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

int string_to_hex(char *string, unsigned char *key, int len)
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



static int check_wlan_args(char* wlan_interface,BssDscr *pBss,struct rtk_connect_AP_info *ap_info,struct rtk_wlan_config_info* pconfig_info)
{
	int ret = -1;
	int key_len;
	if(wlan_interface == NULL || pBss == NULL || ap_info == NULL || pconfig_info == NULL)
	{
		RTK_BTCONFIG_ERR("Invalid argument!\n");
		goto err;
	}
	if(strcmp(wlan_interface,"wlan0-vxd")
#if defined(FOR_DUAL_BAND)
		&& strcmp(wlan_interface,"wlan1-vxd")
#endif
	  )
	{
		RTK_BTCONFIG_ERR("Invalid interface name!\n");
		goto err;
	}
	key_len = strlen(ap_info->password);
	if((pBss->bdCap & cPrivacy) == 0)/*no*/
	{
		if(key_len != 0)
		{
			RTK_BTCONFIG_ERR("Auth disabled!\n");
			goto err;
		}
	}
	else if (pBss->bdTstamp[0] == 0)/*wep*/
	{
		if(key_len != 5 && key_len != 10 && key_len != 13 && key_len != 26)
		{
			RTK_BTCONFIG_ERR("Invalid wep key length!\n");
			goto err;
		}
	}
	else/*WPA*/
	{
		if(key_len > (MAX_PSK_LEN-1)|| key_len < MIN_PSK_LEN )
		{
			RTK_BTCONFIG_ERR("Invalid PSK length!\n");
			goto err;
		}
	}
	ret = 0;
err:
	return ret;
}
int rtk_btconfig_get_wlan_config(char* wlan_interface,BssDscr *pBss,struct rtk_connect_AP_info *ap_info,struct rtk_wlan_config_info* pconfig_info)
{
	int ret  = -1;
	if(check_wlan_args(wlan_interface,pBss,ap_info,pconfig_info) == -1)
	{
		RTK_BTCONFIG_ERR("Invalid argument!!\n");
		goto err;
	}
	memset(pconfig_info,0,sizeof(struct rtk_wlan_config_info));
	strcpy(pconfig_info->wlan_interface,wlan_interface);
	strcpy(pconfig_info->ssid,pBss->bdSsIdBuf);
	if((pBss->bdCap & cPrivacy) == 0)/*no encrypt*/
	{
		pconfig_info->auth_type = AUTH_OPEN;
		pconfig_info->enc_mode = ENCRYPT_DISABLED;
	}
	else
	{
		if (pBss->bdTstamp[0] == 0)/*wep*/
		{
			pconfig_info->auth_type = AUTH_BOTH;
			pconfig_info->enc_mode = ENCRYPT_WEP;
			strcpy(pconfig_info->wepkey,ap_info->password);
		}
		else/*WPA mixed*/
		{
			pconfig_info->auth_type = AUTH_BOTH;
			pconfig_info->enc_mode = ENCRYPT_WPA2_MIXED;
			strcpy(pconfig_info->psk,ap_info->password);
		}
	}
	ret = 0;
err:
	return ret;
}
int apmib_update_web(int type)
{
	int ret = apmib_update(type);

	if (ret == 0)
		return 0;

	if (type & CURRENT_SETTING) {
		save_cs_to_file();
	}
	return ret;
}

static unsigned char conn_status = 0;
void set_connect_status(int status)
{
	conn_status = status;
}
int get_connect_status(void)
{
	return conn_status;
}
int rtk_btconfig_get_current_status(struct rtk_btconfig_status *pStatus)
{
	int rptEnabled1 = 0;
	unsigned char res;
#if defined(FOR_DUAL_BAND)
	int rptEnabled2 = 0;
#endif
	char ssid[33];
	int reason_code;
	bss_info bss;
	int band = 0;
	char wlan_if[32] = {0};
	ENCRYPT_T encrypt;
	int retry_cnt = 0;
	if(pStatus == NULL)
	{
		return -1;
	}
	apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled1);
#if defined(FOR_DUAL_BAND)
	apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled2);
#endif
	if(rptEnabled1)
	{
		strcpy(wlan_if,"wlan0-vxd");
#if defined(FOR_DUAL_BAND)
#if defined(CONFIG_BAND_5G_ON_WLAN0)
		pStatus->band = 1;
#else
		pStatus->band = 0;
#endif
#else
		pStatus->band = 0;
#endif
	}
#if defined(FOR_DUAL_BAND)
	else if(rptEnabled2)
	{
		strcpy(wlan_if,"wlan1-vxd");
#if defined(CONFIG_BAND_5G_ON_WLAN0)
		pStatus->band = 0;
#else
		pStatus->band = 1;
#endif
	}
#endif
	else
	{
		RTK_BTCONFIG_DEBUG("Repeater not enabled!!\n");
		return -1;
	}
	getWlBssInfo(wlan_if, &bss);
	strcpy(pStatus->ssid,bss.ssid);
	apmib_save_wlanIdx();
	SetWlan_idx(wlan_if);
	if(strlen(pStatus->ssid) == 0)
	{
		apmib_get(MIB_WLAN_SSID,(void*)pStatus->ssid);
	}
	pStatus->status = bss.state;
	memcpy(pStatus->bssid,bss.bssid,6);
	pStatus->rssi = bss.rssi;
	
	apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt);
	if(encrypt == ENCRYPT_DISABLED)
	{
		pStatus->encrypt = 0;
	}
	else if(encrypt == ENCRYPT_WPA || encrypt == ENCRYPT_WPA2 || encrypt == ENCRYPT_WPA2_MIXED)
	{
		pStatus->encrypt= 1;
	}
	else if(encrypt == ENCRYPT_WEP)
	{
		pStatus->encrypt = 2;
	}
	apmib_recov_wlanIdx();
	if(bss.state != STATE_CONNECTED)
	{
		RTK_BTCONFIG_DEBUG("Not connected!!!!\n");
		if(get_connect_status() == 0xf)/*password error*/
		{
			pStatus->status = 0xf;
			RTK_BTCONFIG_DEBUG("password error!\n");
			return 0;
		}
		if (getWlJoinResult(wlan_if, &res) < 0) {
			RTK_BTCONFIG_DEBUG("Join request failed!\n");
			return 0;
		}
		if(res==STATE_Bss  || res==STATE_Ibss_Idle || res==STATE_Ibss_Active) { // completed 
			RTK_BTCONFIG_DEBUG("Completed..\n");
			return 0;
		}
		else
		{
			if(encrypt != ENCRYPT_DISABLED)
			{
				retry:		
				getWlAuthResult(wlan_if, &reason_code);
				RTK_BTCONFIG_DEBUG("reason_code:%d\n",reason_code);
				if(reason_code == 15 && ++ retry_cnt < 3)/*reasoncode 15 means timeout*/
				{
					RTK_BTCONFIG_DEBUG("retry_cnt:%d\n",retry_cnt);
					sleep(1);
					goto retry;
				}
				if(is_password_error(reason_code))
				{
					RTK_BTCONFIG_DEBUG("password error!\n");
					set_connect_status(0xf);
					pStatus->status = 0xf;
				}
			}
		}
	}
	set_connect_status(0);
#if 0//def RTK_DEBUG
	RTK_BTCONFIG_DEBUG("band:%d\n",pStatus->band);
	RTK_BTCONFIG_DEBUG("status:%d\n",pStatus->status);
	RTK_BTCONFIG_DEBUG("encrypt:%d\n",pStatus->encrypt);
	RTK_BTCONFIG_DEBUG("ssid:%s\n",pStatus->ssid);
	RTK_BTCONFIG_MAC_PRINT(pStatus->bssid);
#endif	
	return 0;
}
int rtk_btconfig_update_flash_mib(struct rtk_wlan_config_info pconfig_info)
{
	int ret = -1;
	char wepkey[32] = {0};
	char cmd[128] = {0};
	int rptEnabled;
	int authType;
	ENCRYPT_T encrypt;
	int val;
	int key_len;
	int wep_key_type = 0;
	int opmode;
	int is_vxd = 0;
	RTK_BTCONFIG_DEBUG("pconfig_info.wlan_interface:%s\n",pconfig_info.wlan_interface);
	if(strcmp(pconfig_info.wlan_interface,"wlan0")
		&&strcmp(pconfig_info.wlan_interface,"wlan0-vxd")
#if defined(FOR_DUAL_BAND)
		&& strcmp(pconfig_info.wlan_interface,"wlan1")
    	&& strcmp(pconfig_info.wlan_interface,"wlan1-vxd")
#endif
	  )
	{
		RTK_BTCONFIG_ERR("Invalid interface name!\n");
		return -1;
	}
	if(strstr(pconfig_info.wlan_interface,"-vxd"))
	{
		is_vxd = 1;
	}
	rptEnabled = 0;
#if defined(FOR_DUAL_BAND)
	val = 1;
	if(!strcmp(pconfig_info.wlan_interface,"wlan0-vxd"))
	{
		apmib_set(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
		apmib_save_wlanIdx();
		SetWlan_idx("wlan1-vxd");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&val);
		apmib_recov_wlanIdx();
	}
	else if(!strcmp(pconfig_info.wlan_interface,"wlan1-vxd"))
	{
		apmib_set(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
		apmib_save_wlanIdx();
		SetWlan_idx("wlan0-vxd");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&val);
		apmib_recov_wlanIdx();
	}
#endif
	apmib_save_wlanIdx();
	RTK_BTCONFIG_DEBUG("pconfig_info.wlan_interface:%s\n",pconfig_info.wlan_interface);
	SetWlan_idx(pconfig_info.wlan_interface);
	apmib_set(MIB_WLAN_SSID, (void *)pconfig_info.ssid);
	val = 0;
	apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&val);
	/*set opmode*/
	if(is_vxd)
	{
		opmode = CLIENT_MODE;
	}
	else
	{
		opmode = AP_MODE;
	}
	apmib_set(MIB_WLAN_MODE, (void *)&opmode);	

	val = 0;
	apmib_set(MIB_WLAN_ENABLE_1X, (void *)&val);/*currently does not support 802.1x*/
	rptEnabled = 1;
	if(!strcmp(pconfig_info.wlan_interface,"wlan0-vxd"))
	{
		apmib_set(MIB_REPEATER_SSID1, (void *)pconfig_info.ssid);
		apmib_set(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	}
#if defined(FOR_DUAL_BAND)
	else if(!strcmp(pconfig_info.wlan_interface,"wlan1-vxd"))
	{
		apmib_set(MIB_REPEATER_SSID2, (void *)pconfig_info.ssid);
		if(!apmib_set(MIB_REPEATER_ENABLED2, (void *)&rptEnabled))
		{
			RTK_BTCONFIG_ERR("apmib_set MIB_REPEATER_ENABLED2 failed!\n");
		}
	}
#endif
	if(pconfig_info.enc_mode == ENCRYPT_DISABLED)/*no encrypt*/
	{
		RTK_BTCONFIG_DEBUG("Open system\n");
		authType = AUTH_OPEN;
		apmib_set(MIB_WLAN_AUTH_TYPE,(void *)&authType);
		encrypt = ENCRYPT_DISABLED;
		apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);
	}
	else
	{
		if (pconfig_info.enc_mode == ENCRYPT_WEP)/*wep*/
		{
			RTK_BTCONFIG_DEBUG("WEP\n");
			authType = AUTH_BOTH;
			apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType);
			encrypt = ENCRYPT_WEP;
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);
			int wep = WEP_DISABLED;
			key_len = strlen(pconfig_info.wepkey);
			if(key_len == 5 || key_len == 10)
			{
				RTK_BTCONFIG_DEBUG("WEP64\n");
				wep = WEP64;
			}
			else if(key_len == 13 || key_len == 26)
			{
				RTK_BTCONFIG_DEBUG("WEP128\n");
				wep = WEP128;
			}
			else
			{
				RTK_BTCONFIG_ERR("Invalid wep keylen!!");
			}
			if(key_len == 5 || key_len == 13)/*ASCII*/
			{
				RTK_BTCONFIG_DEBUG("ASCII\n");
				val = 0;
				apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&val);
			}
			else if(key_len == 10 || key_len == 26)/*hex*/
			{
				RTK_BTCONFIG_DEBUG("hex\n");
				val = 1;
				apmib_set(MIB_WLAN_WEP_KEY_TYPE, (void *)&val);
			}
			apmib_set(MIB_WLAN_WEP, (void *)&wep);
			RTK_BTCONFIG_DEBUG("pconfig_info.wepkey:%s\n",pconfig_info.wepkey);
			if(key_len == 10 || key_len == 26)/*ascii hex format*/
			{
				string_to_hex(pconfig_info.wepkey,wepkey,key_len);
			}
			else
			{
				strcpy(wepkey,pconfig_info.wepkey);
			}
			if(wep == WEP64)
			{
				RTK_BTCONFIG_DEBUG("set WEPKEY64\n");
				apmib_set(MIB_WLAN_WEP64_KEY1, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP64_KEY2, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP64_KEY3, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP64_KEY4, (void *)wepkey);
			}
			else if(wep == WEP128)
			{
				RTK_BTCONFIG_DEBUG("set WEPKEY128\n");
				apmib_set(MIB_WLAN_WEP128_KEY1, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP128_KEY2, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP128_KEY3, (void *)wepkey);
				apmib_set(MIB_WLAN_WEP128_KEY4, (void *)wepkey);
			}
			val = 0;
			apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&val);
		}
		else if(pconfig_info.enc_mode == ENCRYPT_WPA || pconfig_info.enc_mode == ENCRYPT_WPA2 || pconfig_info.enc_mode == ENCRYPT_WPA2_MIXED)
		{
			RTK_BTCONFIG_DEBUG("WPA/WPA2/WPA mixed\n");
			authType = AUTH_BOTH;
			apmib_set(MIB_WLAN_AUTH_TYPE,(void *)&authType);
			encrypt = ENCRYPT_WPA | ENCRYPT_WPA2;
			apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt);
			val = WPA_CIPHER_MIXED;
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&val);
			apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&val);
			apmib_set(MIB_WLAN_WPA_PSK, (void *)pconfig_info.psk);
		}
	}
	apmib_recov_wlanIdx();
	apmib_update_web(CURRENT_SETTING);
	return 0;
}

