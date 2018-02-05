#include "rtk_btconfig_api.h"
#include "./common/rtk_wlan.h"
#include "./common/rtk_btconfig_common.h"
#include <apmib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

rtk_btconfig_API btconfig_api_table[]=
{
	{TAG_BT_CMD_SCAN,rtk_btconfig_process_scan},
	{TAG_BT_CMD_CONNECT,rtk_btconfig_process_connect},
	{TAG_BT_CMD_GET_STATUS,rtk_btconfig_get_status},
	{TAG_BT_CMD_GET_WLAN_BAND,rtk_btconfig_get_band_info},
	{TAG_BT_CMD_GET_STA_INFO,rtk_btconfig_get_sta_info},
	{TAG_BT_CMD_SYS_CMD,rtk_btconfig_syscmd},
	/*Add API below*/
	/*Add API end*/
	{TAG_BT_CMD_INVALID,NULL},
};
int rtk_btconfig_send_msg(int sk,unsigned char* pBuf,int len)
{
	int sent_len = 0;
	unsigned char* tmp = pBuf;
	int total_len = len;
	int tmp_len;
	if(sk < 0 || pBuf == NULL || len <= 0)
	{
		return -1;
	}
	if(len > MAX_SEND_BUF_SIZE)
	{
		while(len > MAX_SEND_BUF_SIZE)
		{
			tmp_len = write(sk,tmp,MAX_SEND_BUF_SIZE);
			if(tmp_len < MAX_SEND_BUF_SIZE)
			{
				return -1;
			}
			sent_len += tmp_len;
			tmp += MAX_SEND_BUF_SIZE;
			len -= MAX_SEND_BUF_SIZE;
		}
		if(len > 0)
		{
			tmp_len  = write(sk,tmp,len);
			if(tmp_len < len)
			{
				return -1;
			}
			sent_len += tmp_len;
		}
	}
	else
	{
		sent_len = write(sk,pBuf,len);
	}
	if(sent_len < total_len)
	{
		RTK_BTCONFIG_ERR("send failed!\n");
	}
	return sent_len;
}

void rtk_btconfig_check_repeater_status(void)
{
	int wlan0_disabled;
	int wlan0_vxd_disabled;
	int repeaterEnabled1;
	int btconfig_configured;
#if defined(FOR_DUAL_BAND)
	int repeaterEnabled2;
	int wlan1_disabled;
	int wlan1_vxd_disabled;
#endif
	int config_changed = 0;
	RTK_BTCONFIG_DEBUG("check repeater status....\n");
	apmib_get(MIB_BT_CONFIG_CONFIGURED,(void*)&btconfig_configured);
	apmib_save_wlanIdx();
	SetWlan_idx("wlan0");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan0_disabled);
	apmib_recov_wlanIdx();

	apmib_save_wlanIdx();
	SetWlan_idx("wlan0-vxd");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan0_vxd_disabled);
	apmib_recov_wlanIdx();
	
	apmib_get(MIB_REPEATER_ENABLED1,(void*)&repeaterEnabled1);
#if defined(FOR_DUAL_BAND)
	apmib_save_wlanIdx();
	SetWlan_idx("wlan1");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan1_disabled);
	apmib_recov_wlanIdx();

	apmib_save_wlanIdx();
	SetWlan_idx("wlan1-vxd");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan1_vxd_disabled);
	apmib_recov_wlanIdx();

	apmib_get(MIB_REPEATER_ENABLED2,(void*)&repeaterEnabled2);
#endif
	if(wlan0_disabled)
	{
		wlan0_disabled = 0;
		apmib_save_wlanIdx();
		SetWlan_idx("wlan0");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&wlan0_disabled);
		apmib_recov_wlanIdx();
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan0", "up", NULL_STR);
		sleep(1);
		config_changed = 1;
	}
#if defined(FOR_DUAL_BAND)
	if(wlan1_disabled)
	{
		wlan1_disabled = 0;
		apmib_save_wlanIdx();
		SetWlan_idx("wlan1");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&wlan1_disabled);
		apmib_recov_wlanIdx();
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan1", "up", NULL_STR);
		sleep(1);
		config_changed = 1;
	}
#endif

#if !defined(FOR_DUAL_BAND)
	if(wlan0_vxd_disabled)
	{
		wlan0_vxd_disabled = 0;
		apmib_save_wlanIdx();
		SetWlan_idx("wlan0-vxd");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&wlan0_vxd_disabled);
		apmib_recov_wlanIdx();
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan0-vxd", "up", NULL_STR);
		config_changed = 1;
	}
	if(!repeaterEnabled1)
	{
		repeaterEnabled1 = 1;
		apmib_set(MIB_REPEATER_ENABLED1,(void*)&repeaterEnabled1);
		config_changed = 1;
	}
#else
	if(wlan0_vxd_disabled && wlan1_vxd_disabled)
	{
		wlan1_vxd_disabled = 0;
		apmib_save_wlanIdx();
		SetWlan_idx("wlan1-vxd");
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void*)&wlan1_vxd_disabled);
		apmib_recov_wlanIdx();
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan1-vxd", "up", NULL_STR);
		config_changed = 1;
	}
	if(!repeaterEnabled1 && !repeaterEnabled2)
	{
		repeaterEnabled2 = 1;
		apmib_set(MIB_REPEATER_ENABLED2,(void*)&repeaterEnabled2);
		config_changed = 1;
	}
#endif
	if(config_changed)
	{
		apmib_update_web(CURRENT_SETTING);
	}
}


int rtk_btconfig_cmd_reply(int sk,unsigned short tag,unsigned char result)
{
	char send_buf[16];
	char *pBuf;
	int sent_len,total_len;
	memset(send_buf,0,strlen(send_buf));
	pBuf = send_buf;
	pBuf = rtk_btconfig_add_tlv(pBuf, tag,1, (void *)&result);
	total_len = RTK_BTCONFIG_MSG_HDR_LEN+1;
	sent_len = rtk_btconfig_send_msg(sk,send_buf,total_len);
	RTK_BTCONFIG_DEBUG("sent_len:%d\n",sent_len);
	if(sent_len < total_len)
	{
		RTK_BTCONFIG_ERR("Failed to send replyfail!\n");
		close(sk);
		exit(0);
	}
}

int rtk_btconfig_ba2str(const bdaddr_t *ba, char *str)
{
	return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
		ba->b[5], ba->b[4], ba->b[3], ba->b[2], ba->b[1], ba->b[0]);
}
int rtk_btconfig_bachk(const char *str)
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
int rtk_btconfig_str2ba(const char *str, bdaddr_t *ba)
{
	int i;

	if (rtk_btconfig_bachk(str) < 0) {
		memset(ba, 0, sizeof(*ba));
		return -1;
	}

	for (i = 5; i >= 0; i--, str += 3)
		ba->b[i] = strtol(str, NULL, 16);

	return 0;
}




int rtk_btconfig_process_scan(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	unsigned char* pBuf;
	int tag_len;
	int i;
	struct rtk_btconfig_ss_result ss_result;
	int ret = RTK_BTCONFIG_RET_FAIL;
	int total_len,sent_len;
	unsigned char value = 0;
	char wlan_interface[32] = {0};
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		goto err;
	}
	pBuf = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_SCAN,reqLen,&tag_len);
	if(pBuf == NULL)
	{
		RTK_BTCONFIG_DEBUG("Cannot find scan request tag!\n");
		goto err;
	}
	value = pBuf[0];
	RTK_BTCONFIG_DEBUG("value:%d\n",value);
	if(value !=0 && value != 1)
	{
		RTK_BTCONFIG_ERR("Invalid band!\n");
		goto err;
	}
	if(value == 0)/*2.4G*/
	{
		rtk_btconfig_get_2g_interface(wlan_interface);
	}
#if defined(FOR_DUAL_BAND)
	else if(value == 1)/*5G*/
	{
		rtk_btconfig_get_5g_interface(wlan_interface);
	}
#endif
	else
	{
		RTK_BTCONFIG_ERR("Invalid band!!\n");
		return ret;
	}
	RTK_BTCONFIG_DEBUG("wlan_interface:%s\n",wlan_interface);
	memset(&ss_result,0,sizeof(ss_result));
	ss_result.band = value;
	if(rtk_btconfig_wlan_scan(wlan_interface,&ss_result) != 0)
	{
		RTK_BTCONFIG_ERR("failed to scan on %s\n",wlan_interface);
		goto err;
	}
#if defined(RTK_DEBUG)
	RTK_BTCONFIG_DEBUG("ss_result.number:%d\n",ss_result.number);
	for(i=0;i<ss_result.number;i++)
	{
		RTK_BTCONFIG_DEBUG("%s\n",ss_result.bss_info[i].bdSsIdBuf);
	}
#endif
	/*send ss result*/
	ss_result.number = htonl(ss_result.number);
	pBuf = retBuf;
	pBuf = rtk_btconfig_add_tlv(pBuf, TAG_BT_CMD_REPLY_SCAN, sizeof(struct rtk_btconfig_ss_result), (void *)&ss_result);
	*retBufLen = RTK_BTCONFIG_MSG_HDR_LEN+sizeof(struct rtk_btconfig_ss_result);
	ret = RTK_BTCONFIG_RET_SUCCESS;
err:
	return ret;
}
int rtk_btconfig_process_connect(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	unsigned char* pBuf;
	int tag_len;
	int ret = RTK_BTCONFIG_RET_FAIL;
	int i;
    unsigned char* tmp;
	int found = 0;
	char cmd[128] = {0};
	int interface_changed = 0;
	unsigned char mac[6] = {0};
	int opmode;
	int rptEnabled1,rptEnabled2;
	struct rtk_connect_AP_info *ap_info;
	struct rtk_wlan_config_info pconfig_info;
	int auth_type,encmode;
	char interf[16] = {0};
	char interf_vxd[16] = {0};
	int btconfig_configured = 0;
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		goto err;
	}
	pBuf = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_CONNECT,reqLen,&tag_len);
	if(pBuf == NULL)
	{
		RTK_BTCONFIG_DEBUG("Cannot find connect request tag!\n");
		goto err;
	}
	ap_info = (struct rtk_connect_AP_info*)pBuf;
#if defined(RTK_DEBUG)
	RTK_BTCONFIG_DEBUG("band:%d\n",ap_info->band);
	RTK_BTCONFIG_DEBUG("ssid:%s\n",ap_info->bdSSID);
	RTK_BTCONFIG_MAC_PRINT(ap_info->bdMAC);
	RTK_BTCONFIG_DEBUG("psk:%s\n",ap_info->password);
#endif
#if !defined(FOR_DUAL_BAND)
	if(ap_info->band == 1)
	{
		RTK_BTCONFIG_ERR("Invalid band!\n");
		goto err;
	}
#endif

	if(ap_info->band == 0)/*2.4G*/
	{
		rtk_btconfig_get_2g_interface(interf);
	}
#if defined(FOR_DUAL_BAND)
	else if(ap_info->band == 1)/*5G*/
	{
		rtk_btconfig_get_5g_interface(interf);
	}
#endif
	else
	{
		RTK_BTCONFIG_ERR("Invalid band info!\n");
		goto err;
	}

	strcpy(interf_vxd,interf);
	strcat(interf_vxd,"-vxd");
	strcpy(pconfig_info.wlan_interface,interf_vxd);
	strcpy(pconfig_info.ssid,ap_info->bdSSID);
	if(ap_info->encrypt == 0)
	{
		RTK_BTCONFIG_DEBUG("OPEN SYSTEM!!!!!\n");
		pconfig_info.auth_type = AUTH_OPEN;
		pconfig_info.enc_mode = ENCRYPT_DISABLED;
	}
	else if(ap_info->encrypt == 1)
	{
		RTK_BTCONFIG_DEBUG("WPA/WPA2/WPA Mixed!!!!!\n");
		pconfig_info.auth_type = AUTH_BOTH;
		pconfig_info.enc_mode = ENCRYPT_WPA2_MIXED;
		strcpy(pconfig_info.psk,ap_info->password);
	}
	else if(ap_info->encrypt == 2)
	{
		RTK_BTCONFIG_DEBUG("WEP!!\n");
		pconfig_info.auth_type = AUTH_BOTH;
		pconfig_info.enc_mode = ENCRYPT_WEP;
		i = strlen(ap_info->password);
		if(i != 5 && i != 10 && i != 13 && i != 26)
		{
			RTK_BTCONFIG_ERR("Invalid WEP key length!!\n");
			goto err;
		}
		strcpy(pconfig_info.wepkey,ap_info->password);
		RTK_BTCONFIG_DEBUG("WEP Key:%s\n",pconfig_info.wepkey);
	}
	RTK_BTCONFIG_DEBUG("set conn_status to 0\n");
	set_connect_status(0);

	RTK_BTCONFIG_DEBUG("interf:%s\n",interf);
	RTK_BTCONFIG_DEBUG("interf_vxd:%s\n",interf_vxd);
	apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled1);
#if defined(FOR_DUAL_BAND)
	apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled2);
#endif
	rtk_btconfig_update_flash_mib(pconfig_info);
	if(rptEnabled1 && !strcmp(interf_vxd,"wlan1-vxd"))
	{
		RTK_BTCONFIG_DEBUG("interface changed!!!!\n");
		interface_changed = 1;
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan0-vxd", "down", NULL_STR);
		rtk_RunSystemCmd(NULL_FILE, "flash", "set_mib", "wlan1-vxd", NULL_STR);
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan1-vxd", "up", NULL_STR);
	}
#if defined(FOR_DUAL_BAND)
	else if(rptEnabled2 && !strcmp(interf_vxd,"wlan0-vxd"))
	{
		RTK_BTCONFIG_DEBUG("interface changed!!!!\n");
		interface_changed = 1;
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan1-vxd", "down", NULL_STR);
		rtk_RunSystemCmd(NULL_FILE, "flash", "set_mib", "wlan0-vxd", NULL_STR);
		rtk_RunSystemCmd(NULL_FILE, "ifconfig", "wlan0-vxd", "up", NULL_STR);
	}
#endif
	if(interface_changed == 1)
	{
		sleep(1);
	}
	if(rtk_btconfig_update_wlan_mib(pconfig_info) == -1)
	{
		RTK_BTCONFIG_ERR("update wlan mib failed...\n");
		ret = RTK_BTCONFIG_RET_FAIL;
		goto err;
	}
	if(interface_changed == 1)
	{
		rtk_btconfig_update_bridge_interfaces(interf_vxd);
	}
	/*set root interface, if dualband, set both root interfaces*/
	strcpy(pconfig_info.wlan_interface,interf);
	rtk_btconfig_update_flash_mib(pconfig_info);
	if(rtk_btconfig_update_wlan_mib(pconfig_info) == -1)
	{
		ret = RTK_BTCONFIG_RET_FAIL;
		goto err;
	}
#if defined(FOR_DUAL_BAND)/*set the other interface*/
	if(!strcmp(interf,"wlan0"))
	{
		strcpy(pconfig_info.wlan_interface,"wlan1");
	}
	else if(!strcmp(interf,"wlan1"))
	{
		strcpy(pconfig_info.wlan_interface,"wlan0");
	}
	rtk_btconfig_update_flash_mib(pconfig_info);
	if(rtk_btconfig_update_wlan_mib(pconfig_info) == -1)
	{
		ret = RTK_BTCONFIG_RET_FAIL;
		goto err;
	}
#endif
	apmib_get(MIB_OP_MODE,(void*)&opmode);
	RTK_BTCONFIG_DEBUG("interf_vxd:%s\n",interf_vxd);	
	if(opmode!=BRIDGE_MODE)
	{
		opmode = BRIDGE_MODE;
		apmib_set(MIB_OP_MODE,(void*)&opmode);
		apmib_update_web(CURRENT_SETTING);
		system("sysconf init gw all");
	}
	
	apmib_get(MIB_BT_CONFIG_CONFIGURED,(void*)&btconfig_configured);
	if(0 == btconfig_configured)
	{
		btconfig_configured = 1;
		apmib_set(MIB_BT_CONFIG_CONFIGURED,(void*)&btconfig_configured);
		apmib_update_web(CURRENT_SETTING);
	}
	ret = RTK_BTCONFIG_RET_SUCCESS;
	*retBufLen = 0;
err:
	return ret;
}

int rtk_btconfig_get_status(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	int ret = RTK_BTCONFIG_RET_FAIL;
	unsigned char* pData;
	int buf_len, tag_len;
	struct rtk_btconfig_status pStatus;
	unsigned char *pBuf;
	int btconfig_configured;
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		return RTK_BTCONFIG_RET_FAIL;
	}
	pData = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_GET_STATUS,reqLen,&tag_len);
	if(pData == NULL)
	{
		RTK_BTCONFIG_ERR("NOT GET_STATUS command!\n");
		return RTK_BTCONFIG_RET_FAIL;
	}
	memset(&pStatus,0,sizeof(struct rtk_btconfig_status));
	if(rtk_btconfig_get_current_status(&pStatus)!=0)
	{
		return RTK_BTCONFIG_RET_FAIL;
	}
	apmib_get(MIB_BT_CONFIG_CONFIGURED,(void*)&btconfig_configured);
	pStatus.config_status = btconfig_configured;
	RTK_BTCONFIG_DEBUG("btconfig configured:%d\n",btconfig_configured);
	pBuf = retBuf;
	pBuf = rtk_btconfig_add_tlv(pBuf, TAG_BT_CMD_REPLY_GET_STATUS, sizeof(struct rtk_btconfig_status), (void *)&pStatus);
	*retBufLen = RTK_BTCONFIG_MSG_HDR_LEN+sizeof(struct rtk_btconfig_status);
	return RTK_BTCONFIG_RET_SUCCESS;
}

int rtk_btconfig_get_band_info(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	int ret = RTK_BTCONFIG_RET_FAIL;
	unsigned char* pData;
	int tag_len;
	unsigned char *pBuf;
	struct rtk_btconfig_band_info pBandInfo;
	pBandInfo.support_2_4g = 0;
	pBandInfo.support_5g = 0;
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		return RTK_BTCONFIG_RET_FAIL;
	}
	pData = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_GET_WLAN_BAND,reqLen,&tag_len);
	if(pData == NULL)
	{
		RTK_BTCONFIG_ERR("NOT GET BAND command!\n");
		return RTK_BTCONFIG_RET_FAIL;
	}
	pBandInfo.support_2_4g = 1;
#if defined(FOR_DUAL_BAND)
	pBandInfo.support_5g = 1;
#endif
	pBuf = retBuf;
	pBuf = rtk_btconfig_add_tlv(pBuf, TAG_BT_CMD_REPLY_WLAN_BAND, sizeof(struct rtk_btconfig_band_info), (void *)&pBandInfo);
	*retBufLen = RTK_BTCONFIG_MSG_HDR_LEN+sizeof(struct rtk_btconfig_band_info);
	return RTK_BTCONFIG_RET_SUCCESS;
}
int rtk_btconfig_get_sta_info(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	unsigned char* pData;
	int tag_len;
	unsigned char value;
	char interf[16] = {0};
	int i;
	RTK_WLAN_STA_INFO_Tp pInfo;
	char *buff = NULL;
	struct rtk_btconfig_sta_info rtk_staInfo;
	unsigned char *pBuf;
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		goto err;
	}
	pData = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_GET_STA_INFO,reqLen,&tag_len);
	if(pData == NULL)
	{
		RTK_BTCONFIG_ERR("NOT GET BAND command!\n");
		return 0;
	}
	value = pData[0];
	RTK_BTCONFIG_DEBUG("value:%d\n",value);
	if(value == 0)/*2.4G*/
	{
		rtk_btconfig_get_2g_interface(interf);
	}
#if defined(FOR_DUAL_BAND)
	else if(value == 1)/*5G*/
	{
		rtk_btconfig_get_5g_interface(interf);
	}
#endif
	else
	{
		RTK_BTCONFIG_ERR("Invalid band!\n");
		goto err;
	}
	buff = calloc(1, sizeof(RTK_WLAN_STA_INFO_T) * (RTK_BTCONFIG_MAX_BSS_NUM+1));
	if (buff == 0) 
	{
		printf("Allocate buffer failed!\n");
		return 0;
	}
	memset(&rtk_staInfo,0,sizeof(struct rtk_btconfig_sta_info));
	rtk_staInfo.band = value;
	RTK_BTCONFIG_DEBUG("interface:%s\n",interf);
	if (rtk_getWlStaInfo(interf,(RTK_WLAN_STA_INFO_Tp)buff ) < 0 ) 
	{
		RTK_BTCONFIG_ERR("Failed to get sta_info!\b");
		free(buff);
		buff = NULL;
		goto err;
	}
	rtk_staInfo.number = 0;
	for(i=1;i<RTK_BTCONFIG_MAX_BSS_NUM;i++)
	{
		pInfo = (RTK_WLAN_STA_INFO_Tp)&buff[i*sizeof(RTK_WLAN_STA_INFO_T)];
		if(pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC))
		{
			rtk_staInfo.number ++;
			memcpy(rtk_staInfo.sta_info[i-1].bssid,pInfo->addr,6);
			rtk_staInfo.sta_info[i-1].rssi = pInfo->rssi;
		}
	}
#ifdef RTK_DEBUG
	RTK_BTCONFIG_DEBUG("total Sta_info for band %d:%d\n",value,rtk_staInfo.number);
	for(i=0;i<rtk_staInfo.number;i++)
	{
		RTK_BTCONFIG_MAC_PRINT(rtk_staInfo.sta_info[i].bssid);
	}
#endif
	rtk_staInfo.number = htonl(rtk_staInfo.number);
	pBuf = retBuf;
	pBuf = rtk_btconfig_add_tlv(pBuf, TAG_BT_CMD_REPLY_STA_INFO, sizeof(struct rtk_btconfig_sta_info), (void *)&rtk_staInfo);
	*retBufLen = RTK_BTCONFIG_MSG_HDR_LEN+sizeof(struct rtk_btconfig_sta_info);
	if(buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
err:
	return RTK_BTCONFIG_RET_FAIL;
}
int rtk_btconfig_syscmd(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
	unsigned char* pData;
	unsigned short val;
	char line[256] = {0};
	//char *cmd_args[MAX_CMD_ARGS]={"ifconfig",NULL};
	char cmd[512] = {0};
	int buf_len,tag_len;
	FILE* fp;
	unsigned char value;
	char interf[16] = {0};
	int i;
	RTK_WLAN_STA_INFO_Tp pInfo;
	char *buff = NULL;
	struct rtk_btconfig_sta_info rtk_staInfo;
	unsigned char *pBuf;
	int sent_len;
	if(reqCmd == NULL || reqLen <= 0 || retBuf == NULL || retBufLen == NULL)
	{
		return RTK_BTCONFIG_RET_FAIL;
	}
	pData = rtk_btconfig_search_tag(reqCmd,TAG_BT_CMD_SYS_CMD,reqLen,&tag_len);
	if(pData == NULL)
	{
		RTK_BTCONFIG_ERR("NOT syscmd command!\n");
		return RTK_BTCONFIG_RET_FAIL;
	}
	RTK_BTCONFIG_DEBUG("pData:%s\n",pData);
	strcpy(cmd,pData);
	strcat(cmd,">"RTK_SYS_CMD_LOG);
	RTK_BTCONFIG_DEBUG("cmd:%s\n",cmd);
	system(cmd);
	fp = fopen(RTK_SYS_CMD_LOG,"rb");
	if(fp != NULL)
	{
		fseek(fp,0,SEEK_END);
		buf_len = ftell(fp);
		RTK_BTCONFIG_DEBUG("buf_len:%d\n",buf_len);
		fclose(fp);
	}
	if(buf_len > 0)
	{
		pBuf = retBuf;
		*retBufLen = RTK_BTCONFIG_MSG_HDR_LEN + buf_len;
		val = htons(TAG_BT_CMD_REPLY_SYS_CMD);
		memcpy(pBuf,&val,2);//T
		pBuf += 2;
		val = htons(buf_len);
		memcpy(pBuf,&val,2);					//L
		pBuf += 2;
		fp = fopen(RTK_SYS_CMD_LOG,"r");
		if(fp != NULL)									//V
		{
			while(fgets(line, 200, fp))
			{
				int len=strlen(line);
				//RTK_BTCONFIG_DEBUG("line len:%d\n",len);
				memcpy(pBuf,line,len);
				pBuf += len;
			}
			fclose(fp);
		}
	}
	return RTK_BTCONFIG_RET_SUCCESS;
}

#ifdef RTK_DEBUG
void rtk_btconfig_dump_buf(unsigned char* pBuf, int len)
{
	int i;
	if(pBuf == NULL || len < 0)
	{
		return;	
	}
	for(i=0;i<len;i++)
	{
		printf("0x%02x ",pBuf[i]);
		if(((i+1)%16)==0)
			printf("\n");
	}
	printf("\n");
}
#endif


