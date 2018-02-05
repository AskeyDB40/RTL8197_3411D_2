#ifndef _RTK_BTCONFIG_COMMON_H__
#define _RTK_BTCONFIG_COMMON_H__
#include "./rtk_wlan.h"

unsigned char 	*rtk_btconfig_search_tag(unsigned char *data, unsigned short id, int len, int *out_len);
unsigned char 	*rtk_btconfig_add_tlv(unsigned char *data, unsigned short id, int len, void *val);
unsigned short 	rtk_btconfig_get_cmd_tag(char* msg, int msg_len);
int 			rtk_RunSystemCmd(char *filepath, ...);
int 			getWlBssInfo(char *interface, RTK_BSS_INFOp pInfo);
int 			rtk_wlan_scan_request(char* interf,int* pStatus);
int 			rtk_getWlStaInfo(char *interface,  RTK_WLAN_STA_INFO_Tp pInfo);
int 			rtk_wlan_scan_result(char* interf,SS_STATUS_Tp pStatus);
int 			rtk_btconfig_wlan_scan(char* interf,struct rtk_btconfig_ss_result *ss_result);
int 			is_password_error(int reason_code);
int 			getWlAuthResult(char *interface, int *pStatus);
int 			getWlJoinResult(char *interface, unsigned char *res);
int 			rtk_btconfig_update_wlan_mib(struct rtk_wlan_config_info pconfig_info);
int 			rtk_btconfig_get_2g_interface(char* interface_2g);
#if defined(FOR_DUAL_BAND)
int 			rtk_btconfig_get_5g_interface(char* interface_5g);
#endif
int 			rtk_btconfig_update_bridge_interfaces(char* vxd_interface);
#endif/*_RTK_BTCONFIG_COMMON_H__*/
