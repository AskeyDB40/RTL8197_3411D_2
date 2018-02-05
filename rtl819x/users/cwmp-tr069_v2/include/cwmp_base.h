/*
 * cwmp_base.h
 * The basic definition used by libcwmp
 */
#ifndef _CWMP_BASE_H_
#define _CWMP_BASE_H_

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

extern int gDebugLevel;
extern int gDebugModule;
extern int gNeedSendGetRPC;
extern int gNeedSSLAuth;
extern int gSkipMReboot;
extern int gDelayStart;
extern int gSeflReboot;
extern int gDisConReqAuth;
extern int gBringLanMacAddrInInform;
extern int gTR069_SSLAllowSelfSignedCert;
extern int gTR069_SSLRejectErrCert;
extern int gDisplaypassword;
extern int gRegSpecialParameter;
extern int gReturn503;
extern int gWanIfSpecified;
extern char tr069WanIfName[];

extern char	*pNotifyFileName;
extern int	gNotifyBringAllWanIPinInform;

/***************************************************************************
 ***	debug macro
 ***************************************************************************/
extern char *CWMP_DBG_MODULE_NAMELIST[];
extern char *CWMP_DBG_LEVEL_STRING[];
/*
 * cwmp debug modules
 *		Debug message with modules selected will be print out.
 */
enum{
	MODULE_CORE			=(1<<1),		//2
	MODULE_SERVER		=(1<<2),		//4
	MODULE_RPC			=(1<<3),		//8
	MODULE_FILE_TRANS	=(1<<4),		//16
	MODULE_NOTIFY		=(1<<5),		//32
	MODULE_DATA_MODEL	=(1<<6),		//64
	MODULE_TR_143		=(1<<7),		//128
	MODULE_TR_111		=(1<<8),		//256
}CWMP_DBG_MODULE;
	
/*
 * cwmp debug level
 *		Debug message with level below gDebugLevel will be print out.
 *		By default gDebugLevel is 0, so only errors will be print out.
 */
enum{
	LEVEL_ERROR=0,
	LEVEL_WARNING,
	LEVEL_INFO,
	LEVEL_DETAILED,
}CWMP_DBG_LEVEL;

/*
 * cwmp debug function
 *		Use stdout as output pipe
 *		time[strlen(time)-1] is to remove asctime() last '\n'.
 */
#define CWMPDBG_FUNC(mod, level, X)	 \
do{ \
	if( (mod)>0 && ((mod)&gDebugModule)!=0 && ((level)<=gDebugLevel) ) \
	{ \
		unsigned char *dbg_func_time_buf;\
		time_t dbg_func_curTime; \
		int dbg_func_i = -1, dbg_func_val = (mod); \
		dbg_func_curTime = time(NULL); \
		dbg_func_time_buf = asctime(localtime(&dbg_func_curTime)); \
		dbg_func_time_buf[strlen(dbg_func_time_buf)-1] = '\0'; \
		while(dbg_func_val!=1) {dbg_func_i++; dbg_func_val=((dbg_func_val)>>1);} \
		printf("[%s][%s][%s] ", dbg_func_time_buf, CWMP_DBG_MODULE_NAMELIST[dbg_func_i], CWMP_DBG_LEVEL_STRING[level]); \
		printf X; \
	}\
}while(0)


/***************************************************************************
 ***	cwmp core data structure
 ***************************************************************************/
#endif /* _CWMP_BASE_H_ */
