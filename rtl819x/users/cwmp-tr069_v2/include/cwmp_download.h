#ifndef _CWMP_DOWNLOAD_H_
#define _CWMP_DOWNLOAD_H_

#include "soapH.h"

#ifdef __cplusplus
extern "C" {
#endif


/*download way*/
#define DLWAY_NONE			0
#define DLWAY_DOWN			1
#define DLWAY_UP			2
/*ftp action*/
#define FTP_GET				0
#define FTP_PUT				1
/*download type*/
#define DLTYPE_NONE			0
#define DLTYPE_IMAGE		1
#define DLTYPE_WEB			2
#define DLTYPE_CONFIG		3
#define DLTYPE_LOG			4
/*download status*/
#define DOWNLD_NONE			0
#define DOWNLD_READY		1
#define DOWNLD_START		2
#define DOWNLD_FINISH		3
#define DOWNLD_ERROR		4
/*transfer message key*/
#define TRANSFER_MSG_KEY				(5678)
#define TRANSFER_QUEUE_UPDATE_FREQUENCY (60)	// update transfer queue in flash every 60 seconds.
#define MAX_TRANSFER_QUEUE_NUM 			(0xFFFF)
extern int cwmp_dl_msgid;
enum CWMP_TRANSFER_MESSAGE
{
	CWMP_DLCMD_TIMER = 100,
};

enum cwmp_transfer_state
{
	CWMP_DLQ_STATE_NOT_YET_START = 1,
	CWMP_DLQ_STATE_IN_PROGRESS,
	CWMP_DLQ_STATE_COMPLETED,
};
enum cwmp_transfer_type
{
	CWMP_TRANSFER_TYPE_UPLOAD = 0,
	CWMP_TRANSFER_TYPE_DOWNLOAD = 1,
};
	
/*
 * cwmp_transfer_t: actual member of cwmp_userdata->TransferQueue->data
 */
struct cwmp_transfer_t
{
	int 	TransferState;      	/* 1:not yet start, 2: in progress, 3: completed*/
	int 	TransferType; 			/* 0: Upload, 1: Download */
	char	CommandKey[32+4];		/* additional 4 bytes for '\0' and alignment */
	char	FileType[64+4];
	char	URL[256+4];
	char	Username[256+4];
	char	Password[256+4];
	int		FileSize;
	char	TargetFileName[256+4];
	int		DelaySeconds;
	char	SuccessURL[256+4];
	char	FailureURL[256+4];
};

void *cwmp_download_thread( void *data);
void *cwmp_http_pthread(void *data);

#ifdef __cplusplus
}
#endif

#endif /*_CWMP_DOWNLOAD_H_*/
