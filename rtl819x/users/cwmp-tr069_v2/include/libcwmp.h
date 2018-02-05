#ifndef _LIBCWMP_H_
#define _LIBCWMP_H_

#include "apmib.h"
#include "parameter_api.h"
#include "cwmp_base.h"

/***************************************************************************
 ***	debug macro
 ***    #define CWMPDBGZ(X)		fprintf X 
 ***    or
 ***    #define CWMPDBGZ(X)		while(0){}
 ***	ex: CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
 ***************************************************************************/
#define CWMPDBG0(X)		fprintf X	//info
#define CWMPDBG1(X)		fprintf X	//debug 1
#define CWMPDBG2(X)		while(0){}	//debug 2
#define CWMPDBG3(X)		while(0){} 	//debug 3

#define CWMPDBG_SET_ATTR(X) if(0) fprintf X
#define CWMPDBG_GET_PRMT_NAME(X) if(0) fprintf X
#define CWMPDBG_SET_PRMT_VAL(X) if(0) fprintf X
#define CWMPDBG_NOTIFY_CHK(X) if(0) fprintf X
	
#define CWMPDBG_IPPING_DIAG(X) if(0) fprintf X
#define CWMPDBG_BOOTSTRAP(X) if(0) fprintf X
	
#define CWMPDBG_UPLOAD(X) if(0) fprintf X
	
#define CWMPDBG(level,X)	CWMPDBG##level(X)

#define LINE_(line) #line
#define LINE(line) LINE_(line)
#define CWMPDBP0(...) while(0){}
#define CWMPDBP1(...) fprintf(stderr, "<"__FILE__","LINE(__LINE__)">"__VA_ARGS__)
#define CWMPDBP2(...) fprintf(stderr, "<DEBUG:"__FILE__","LINE(__LINE__)">"__VA_ARGS__)
#define CWMPDBP(level, ...) CWMPDBP##level(__VA_ARGS__)


#define _USE_FILE_FOR_OUTPUT_
#ifdef _USE_FILE_FOR_OUTPUT_
#define OUTXMLFILENAME 		"/tmp/cwmp.xml"
#define OUTXMLFILENAME_BK	"/tmp/cwmp_bk.xml"
#endif

/* userdata update */
#define BIT(X)							(1<<(X))
#define TESTBIT(val, n)					(((val)&BIT(n))==0?0:1)
#define CLEARBIT(val, n)				((val)&~BIT(n))
#define USERDATA_BITMAP_ACSURL			0
#define USERDATA_BITMAP_ACSUSERNAME		1
#define USERDATA_BITMAP_ACSPASSWORD		2
#define USERDATA_BITMAP_PERIODIC_INFORM	3	//periodic inform related userdata share the same bit
#define USERDATA_BITMAP_CONREQ_USERNAME	4
#define USERDATA_BITMAP_CONREQ_PASSWORD 5
#define USERDATA_BITMAP_RETRY_MIN		6
#define USERDATA_BITMAP_RETRY_MUTIPLIER	7

/*download union*/
typedef union
{
struct cwmp__Download	Download;
struct cwmp__Upload	Upload;
}DownloadInfo_T;

struct cwmp_userdata
{
	//relative to SOAP header
	unsigned int	ID;
	unsigned int	HoldRequests;
	unsigned int	NoMoreRequests;
	unsigned int	CPE_MaxEnvelopes;
	unsigned int	ACS_MaxEnvelopes;
	
	//cwmp:fault
	int				FaultCode;
	
	//download/upload
	//current DL/UL state
	int			    DownloadState;
	int			    DownloadWay;
	//the following 4 members are included in TransferComplete, thus have corresponding mib.
	char			*DLCommandKey;
	time_t			DLStartTime;
	time_t			DLCompleteTime;
	unsigned int	DLFaultCode;
	DownloadInfo_T	DownloadInfo;
	//transfer queue
	int				CurrentQueueIdx;
	struct node 	*TransferQueue;
        	
	//inform
	unsigned int	InformInterval; //PeriodicInformInterval
	time_t			InformTime; //PeriodicInformTime
	int			    PeriodicInform;
	unsigned int	EventCode;
	struct node		*NotifyParameter;
	struct node 	*InformParameter;		//user customized inform parameters
	unsigned int	InformIntervalCnt; 
	
	//ScheduleInform
	unsigned int	ScheduleInformEnabled;
	unsigned int	ScheduleInformCnt; //for scheduleInform RPC Method, save the DelaySeconds
	char			*SI_CommandKey;

	//Reboot
	char			*RB_CommandKey;	//reboot's commandkey
	int			    Reboot; // reboot flag
	int				HardReboot; // Reboot RPC caused hard reboot
	
	//FactoryReset
	int			    FactoryReset;

	// andrew. 
	char 			*url;	// ACS URL
	char 			*username; // username used to auth us to ACS.
	char 			*password; // passwrd used to auth us to ACS.
	char 			*conreq_username;
	char 			*conreq_password;
	char 			*realm;
	int			    server_port;
	char			*server_path;
	void *			machine;
	
	char			*redirect_url;
	int			    redirect_count;
	
	//certificate
	int				ssl_type;			//0: no ssl; 1: openssl; 2: matrixssl
	char			*cert_passwd;
	char			*cert_path;
	char			*ca_cert;

	int			    url_changed;		//indicate ACS URL has been changed by ACS	
	int			    inform_ct_ext;

	unsigned int	retryMinWaitInterval;   // for TR181 CWMPRetryMinimumWaitInterval
	unsigned int	retryIntervalMutiplier; // for TR181 CWMPRetryIntervalMultiplier

	//userdata dirty bitmap
	int				update_bitmap;			// no more than 32 userdata mib need to be updated.
};


int cwmp_main( struct CWMP_NODE troot[] );

#endif /*#ifndef _LIBCWMP_H_*/
