#ifndef _RTK_BTCONFIG_API_H__
#define _RTK_BTCONFIG_API_H__
/****************************************************************/

#define BTCONFIG_MAX_NONCE_LEN 64
#define BTCONFIG_MAX_RETRY_CNT 5
#define MAX_NUM_OF_BT_CLIENTS 5
#define MAX_EXPIRE_TIME			120
#define TEMP_DIGEST_LENGTH 16
#define FINAL_DIGEST_LENGTH 32
#define RTK_BTCONFIG_RET_FAIL 1
#define RTK_BTCONFIG_RET_SUCCESS 0


#define RTK_BTCONFIG_MSG_HDR_LEN		4


#define RTK_SYS_CMD_LOG "/tmp/syscmd_log"
/****************************************************************/

//#define RTK_DEBUG
#ifdef RTK_DEBUG
#define RTK_BTCONFIG_MAC_PRINT(fmt)\
do{\
	printf("%02X:%02X:%02X:%02X:%02X:%02X\n",\
	fmt[0],fmt[1],fmt[2],fmt[3],fmt[4],fmt[5]);\
}while(0)

#define RTK_BTCONFIG_DEBUG(fmt,args...)\
do{\
	printf("[rtk_btconfig][%s %d]:"fmt,__FUNCTION__ , __LINE__ , ## args);\
}while(0)
#else/*RTK_DEBUG*/
#define RTK_BTCONFIG_DEBUG(fmt,args...)
#endif/*RTK_DEBUG*/
#define RTK_BTCONFIG_ERR(fmt,args...)\
do{\
	printf("[rtk_btconfig][%s %d]:"fmt,__FUNCTION__ , __LINE__ , ## args);\
}while(0)

#define NULL_FILE 0
#define NULL_STR ""


#define MAX_NUM_OF_BT_CLIENTS 5
#define RTK_BTCONFIG_MSG_HDR_LEN		4
#define RTK_BTCONFIG_RET_FAIL 1
#define RTK_BTCONFIG_RET_SUCCESS 0
#define RTK_SYS_CMD_LOG "/tmp/syscmd_log"

/*common APIs*/
int 			rtk_btconfig_process_scan(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_process_connect(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_get_status(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_get_band_info(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_get_sta_info(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_syscmd(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
int 			rtk_btconfig_send_msg(int sk,unsigned char* pBuf,int len);

#ifdef RTK_DEBUG
void 			rtk_btconfig_dump_buf(unsigned char* pBuf, int len);
#endif


/* msg tag*/
#define TAG_BT_CMD_SCAN					0x01
#define TAG_BT_CMD_CONNECT				0x03
#define TAG_BT_CMD_GET_STATUS			0x05
#define TAG_BT_CMD_GET_WLAN_BAND		0x07
#define TAG_BT_CMD_GET_STA_INFO			0x09
#define TAG_BT_CMD_SYS_CMD				0x0b

/*msg reply*/
#define TAG_BT_CMD_REPLY_SCAN			0x02
#define TAG_BT_CMD_REPLY_CONNECT		0x04
#define TAG_BT_CMD_REPLY_GET_STATUS		0x06
#define TAG_BT_CMD_REPLY_WLAN_BAND		0x08
#define TAG_BT_CMD_REPLY_STA_INFO		0x0a
#define TAG_BT_CMD_REPLY_SYS_CMD		0x0c


#define TAG_BT_CMD_REPLY_FAIL			0xff
#define TAG_BT_CMD_INVALID				0xfe

#define RTK_BTCONFIG_MSG_HDR_LEN		4
#define MAX_SEND_BUF_SIZE 256

/*btconfig data structures*/

#define MAX_EXPIRE_TIME			120
typedef struct rtk_btconfig_api
{
	unsigned short cmd_id;
	int (*cmd_function) (unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen);
}rtk_btconfig_API;
#endif/*_RTK_BTCONFIG_API_H__*/

