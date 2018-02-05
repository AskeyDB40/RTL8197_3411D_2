#ifndef _PRMT_TR143_H_
#define _PRMT_TR143_H_

#include <sys/time.h>
#include "prmt_igd.h"
#include "cwmp_download.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifdef _PRMT_TR143_

/* TR143 HTTP Download/Upload Diagnostics*/
#define SHM_PROJ_ID	'S'
#define MAXLINE 4096
#define ECHOTOK "/bin/udpechoserver"

struct netdevstate
{
	unsigned long bytes_sent;
	unsigned long bytes_recv;
	unsigned long packets_sent;
	unsigned long packets_recv;
};

enum
{
	eTR143_None=0,
	eTR143_Requested,
	eTR143_Completed,
	eTR143_Error_InitConnectionFailed,
	eTR143_Error_NoResponse,
	eTR143_Error_PasswordRequestFailed,
	eTR143_Error_LoginFailed,
	eTR143_Error_NoTransferMode,
	eTR143_Error_NoPASV,
	//download
	eTR143_Error_TransferFailed,
	eTR143_Error_IncorrectSize,
	eTR143_Error_Timeout,
	//upload
	eTR143_Error_NoCWD,
	eTR143_Error_NoSTOR,
	eTR143_Error_NoTransferComplete,

	eTR143_End /*last one*/
};

struct TR143_Diagnostics
{
	int		Way;
	
	int		DiagnosticsState;
	char		*pInterface;
	char		IfName[32];
	char		*pURL;
	unsigned int	DSCP;
	unsigned int	EthernetPriority;
	unsigned int	TestFileLength;
	struct timeval	ROMTime;
	struct timeval	BOMTime;
	struct timeval	EOMTime;
	unsigned int	TestBytesReceived;
	unsigned int	TotalBytesReceived;
	unsigned int	TotalBytesSent;
	struct timeval	TCPOpenRequestTime;
	struct timeval	TCPOpenResponseTime;
	
	unsigned long int	http_pid;
	unsigned long int	ftp_pid;
};

struct ECHOPLUS
{
	unsigned int	TestGenSN;
	unsigned int	TestRespSN;
	unsigned int	TestRespRecvTimeStamp;
	unsigned int	TestRespReplyTimeStamp;
	unsigned int	TestRespReplyFailureCount;
};

struct ECHORESULT
{
	unsigned int	TestRespSN;
	unsigned int	TestRespReplyFailureCount;
	unsigned int	PacketsReceived;
	unsigned int	PacketsResponded;
	unsigned int	BytesReceived;
	unsigned int	BytesResponded;
	struct timeval	TimeFirstPacketReceived;
	struct timeval	TimeLastPacketReceived;
};


int TR143StartHttpDiag(struct TR143_Diagnostics *p);
int TR143StopHttpDiag(struct TR143_Diagnostics *p);
/* End TR143 HTTP Download/Upload Diagnostics*/

extern struct CWMP_LEAF tPerformanceDiagnosticLeaf[];
int getPerformanceDiagnostic(char *name, struct CWMP_LEAF *entity, int *type, void **data);

extern struct CWMP_LEAF tDownloadDiagnosticsLeaf[];
int getDownloadDiagnostics(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDownloadDiagnostics(char *name, struct CWMP_LEAF *entity, int type, void *data);

extern struct CWMP_LEAF tUploadDiagnosticsLeaf[];
int getUploadDiagnostics(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setUploadDiagnostics(char *name, struct CWMP_LEAF *entity, int type, void *data);

extern struct CWMP_LEAF tUDPEchoConfigLeaf[];
int getUDPEchoConfig(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setUDPEchoConfig(char *name, struct CWMP_LEAF *entity, int type, void *data);


#ifdef CONFIG_USER_FTP_FTP_FTP
void checkPidforFTPDiag( pid_t  pid );
#endif //CONFIG_USER_FTP_FTP_FTP

extern int gStartTR143DownloadDiag;
extern int gStartTR143UploadDiag;
void StopTR143DownloadDiag(void);
void StopTR143UploadDiag(void);
void StartTR143DownloadDiag(void);
void StartTR143UploadDiag(void);

#endif /*_PRMT_TR143_*/	
#ifdef __cplusplus
}
#endif
#endif
