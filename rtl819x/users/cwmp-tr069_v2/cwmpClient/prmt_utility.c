/*
 *      Utiltiy function to communicate with TCPIP stuffs
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: utility.c,v 1.18 2009/09/04 06:02:14 keith_huang Exp $
 *
 */

/*-- System inlcude files --*/
#include <stdio.h>
#include <stdarg.h> /* Keith add for tr069 --start */
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/ioctl.h>
//#include <net/if.h>
#include <linux/wireless.h>
#include <dirent.h>
#include <time.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
/*-- Local include files --*/
#include "apmib.h"
#include "prmt_utility.h"
#include "cwmp_core.h"
    
/*-- Local constant definition --*/
#define _PATH_PROCNET_ROUTE	"/proc/net/route"
#define _PATH_PROCNET_DEV	"/proc/net/dev"
#define _PATH_RESOLV_CONF	"/etc/resolv.conf"

/* -- Below define MUST same as /linux2.4.18/drivers/net/rtl865x/eth865x.c */
#define RTL8651_IOCTL_GETWANLINKSTATUS 2000
#define RTL8651_IOCTL_GETLANLINKSTATUS 2102
#define RTL8651_IOCTL_GET_ETHER_EEE_STATE 2105
#define RTL8651_IOCTL_GET_ETHER_BYTES_COUNT 2106

/* Keep this in sync with /usr/src/linux/include/linux/route.h */
#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */

#define READ_BUF_SIZE	50
const char STR_DISABLE[] = "Disabled";
const char STR_ENABLE[] = "Enabled";
const char STR_AUTO[] = "Auto";
const char STR_MANUAL[] = "Manual";
const char STR_UNNUMBERED[] = "unnumbered";
const char STR_ERR[] = "err";
const char STR_NULL[] = "null";
const char EMPTY_MAC[MAC_ADDR_LEN] = {0};

static char table64[]=
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*-- Local routine declaration --*/
static int get_dev_fields(int type, char *bp, struct user_net_device_stats *pStats);
static char *get_name(char *name, char *p);

#ifdef TR069_ANNEX_G
#include "udp.h"
#include "stun.h"

int gRestartStun=0;
int stunThread = 0;
int stunStop = 0;
int stunState = STUN_BINDING_REQ;
int OldStunState = STUN_BINDING_REQ;

pthread_cond_t cond;
pthread_mutex_t mutex;

char udpConnReqAddr[CWMP_UDP_CONN_REQ_ADDR_LEN+1];
char stunServerAddr[CWMP_STUN_SERVER_ADDR_LEN+1];
char OldStunServerAddrAndPort[CWMP_STUN_SERVER_ADDR_LEN+1];
unsigned int stunServerPort;
char stunUsername[CWMP_STUN_USERNAME_LEN+1];
char stunPassword[CWMP_STUN_PASSWORD_LEN+1];
int stunMaxPeriod;
unsigned int stunMinPeriod;
unsigned int natDetected;
unsigned int msgIntegrityDetected;

char acsUrl[CWMP_ACS_URL_LEN+1];

time_t g_ts = 0;
unsigned int g_id = 0;
#endif

#if defined(CONFIG_APP_TR069)
#if defined(_PRMT_TR143_)

char *strItf[]=
{
	"",		//ITF_ALL
	ALIASNAME_ETH1,		//ITF_WAN
    ALIASNAME_BR0,		//ITF_LAN

	ALIASNAME_ETH0,		//ITF_ETH0
	ALIASNAME_ETH0_SW0,	//ITF_ETH0_SW0
	ALIASNAME_ETH0_SW1,	//ITF_ETH0_SW1
	ALIASNAME_ETH0_SW2,	//ITF_ETH0_SW2
	ALIASNAME_ETH0_SW3,	//ITF_ETH0_SW3

	ALIASNAME_WLAN0,	//ITF_WLAN0
	ALIASNAME_WLAN0_VAP0,	//ITF_WLAN0_VAP0
	ALIASNAME_WLAN0_VAP1,	//ITF_WLAN0_VAP1
	ALIASNAME_WLAN0_VAP2,	//ITF_WLAN0_VAP2
	ALIASNAME_WLAN0_VAP3,	//ITF_WLAN0_VAP3

	ALIASNAME_WLAN1,	//ITF_WLAN0
	ALIASNAME_WLAN1_VAP0,	//ITF_WLAN0_VAP0
	ALIASNAME_WLAN1_VAP1,	//ITF_WLAN0_VAP1
	ALIASNAME_WLAN1_VAP2,	//ITF_WLAN0_VAP2
	ALIASNAME_WLAN1_VAP3,	//ITF_WLAN0_VAP3

/*
	"br0",		//ITF_LAN

	"eth0",		//ITF_ETH0
	"eth0_sw0",	//ITF_ETH0_SW0
	"eth0_sw1",	//ITF_ETH0_SW1
	"eth0_sw2",	//ITF_ETH0_SW2
	"eth0_sw3",	//ITF_ETH0_SW3

	"wlan0",	//ITF_WLAN0
	"wlan0-vap0",	//ITF_WLAN0_VAP0
	"wlan0-vap1",	//ITF_WLAN0_VAP1
	"wlan0-vap2",	//ITF_WLAN0_VAP2
	"wlan0-vap3",	//ITF_WLAN0_VAP3

	"wlan1",	//ITF_WLAN0
	"wlan1-vap0",	//ITF_WLAN0_VAP0
	"wlan1-vap1",	//ITF_WLAN0_VAP1
	"wlan1-vap2",	//ITF_WLAN0_VAP2
	"wlan1-vap3",	//ITF_WLAN0_VAP3
*/

	"usb0",		//ITF_USB0

	""		//ITF_END
};
#endif
#endif

/*
 *   Parse XML string with escaped characters
 *   parameters:
 *   in:   pointer to the string to be parsed
 *   out:  buffer to store the parsed string, make sure the size nust be large enough
 */
void parse_xml_escaped_str(char *out, char *in)
{
	char pos;
	char *endptr;
	int  len;
	int  num;
	
	if(!out || !in)
		return;

	pos = 0;
	len = strlen(in);

	while(pos < len) {
		if(*in != '&') {
		//if(strncmp(in, "&", 1) != 0) {
			*out++ = *in++;
			pos++;
			continue;
		}
		
		if(strncmp(in, "&lt;", 4) == 0) {
			*out++ = '<';
			in += 4;
			pos += 4;
		} else if (strncmp(in, "&gt;", 4) == 0) {
			*out++ = '>';
			in += 4;
			pos += 4;
		} else if (strncmp(in, "&amp;", 5) == 0) {
			*out++ = '&';
			in += 5;
			pos += 5;
		} else if (strncmp(in, "&apos;", 6) == 0) {
			*out++ = '\'';
			in += 6;
			pos += 6;
		} else if (strncmp(in, "&quot;", 6) == 0) {
			*out++ = '\"';
			in += 6;
			pos += 6;
		} else if (strncmp(in, "&#", 2) == 0) {
			if(strncmp(in+2, "x", 1) == 0) { //hex
				if(strncmp(in+4, ";", 1) == 0) {
					num = *(in+3);
					if( (num>='0' && num<='9') // 0~9
						|| (num>='a' && num<='f') // a~f
						|| (num>='A' && num<='F') // A~F
						) { // hexadecimal
						*out++ = num;
						in += 5;
						pos += 5;
					} else {
						memcpy(out, in, 5);
						in += 5;
						out += 5;
						pos += 5;
					}
				} else if(strncmp(in+5, ";", 1) == 0) {
					num = strtol(in+3, &endptr, 16);
					if(num>0 && endptr==(in+5)) { // char A~F, a~f
						*out++ = num;
						in += 6;
						pos += 6;
					} else {
						memcpy(out, in, 6);
						in += 6;
						out += 6;
						pos += 6;
					}
				} else {
					memcpy(out, in, 3);
					in += 3;
					out += 3;
					pos += 3;
				}
			} else { //dec
				if(strncmp(in+3, ";", 1) == 0) {
					if( *(in+2)>'0' && *(in+2)<'9') { // decimal 0~9
						*out++ = *(in+2);
						in += 4;
						pos += 4;
					} else {
						memcpy(out, in, 4);
						in += 4;
						out += 4;
						pos += 4;
					}
				} else if(strncmp(in+4, ";", 1) == 0) {
					num = strtol(in+2, &endptr, 10);
					if(num>0 && endptr==(in+4)) { // char A~F, a~f
						*out++ = num;
						in += 5;
						pos += 5;
					} else {
						memcpy(out, in, 5);
						in += 5;
						out += 5;
						pos += 5;
					}
				} else {
					memcpy(out, in, 2);
					in += 2;
					out += 2;
					pos += 2;
				}
			}
		} else {
			*out++ = *in++;
			pos++;
		}
	}
	
	*out = '\0';
}

/*------------------------------------------------------------------*/
/*
 * Wrapper to extract some Wireless Parameter out of the driver
 */
static inline int
iw_get_ext(int                  skfd,           /* Socket to the kernel */
           char *               ifname,         /* Device name */
           int                  request,        /* WE ID */
           struct iwreq *       pwrq)           /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}


/////////////////////////////////////////////////////////////////////////////
int getWlStaNum( char *interface, int *num )
{
#ifndef NO_ACTION
    int skfd=0;
    unsigned short staNum;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
      return -1;
	}
    wrq.u.data.pointer = (caddr_t)&staNum;
    wrq.u.data.length = sizeof(staNum);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTANUM, &wrq) < 0){
    	 close( skfd );
	return -1;
	}
    *num  = (int)staNum;

    close( skfd );
#else
    *num = 0 ;
#endif

    return 0;
}
#ifdef CONFIG_RTL_WAPI_SUPPORT
/*
*   parameters:
*   certsInfo (output): to store user cert information get from from CERTS_DATABASE
*   count (input): number of entries in CERTS_DATABASE
*
*   return 0: success; return -1: failed
*/
#define SUCCESS 0
#define FAILED -1
int getCertsDb(CERTS_DB_ENTRY_Tp certsInfo, int count, int *realcount)
{
        FILE *fp;
        time_t  now, expired_tm;
        struct tm *tnow;
        struct tm tm_time;
 
        struct stat status;
        int readSize;
        int ret, toRet;
        int i,intVal;
        long longVal;
        char *p1, *p2, *ptr;
 
        char buffer[100];
        char tmpBuf[100];//Added for test
        char tmpBuf2[3];//Added for test
 
        if ( stat(CERTS_DATABASE, &status) < 0)
        {
                printf("%s(%d): %s not exist.\n",__FUNCTION__,__LINE__, CERTS_DATABASE);//Added for test
                toRet=FAILED;
                goto err;
        }
//        printf("%s(%d)\n",__FUNCTION__,__LINE__);//Added for tes
        fp = fopen(CERTS_DATABASE, "r");
        if (!fp) {
                printf("open %s error.\n", CERTS_DATABASE);//Added for test
                toRet=FAILED;
                goto err;
        }
 
        p1=NULL;
        p2=NULL;
        for(i=0;i<count;i++)
        {
        	   *realcount=i;
                if(!fgets(buffer, sizeof(buffer), fp))
                {
//			printf("%s(%d): file end.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=SUCCESS;
                        goto err;
                }
//                printf("%s(%d),i=%d,buffer=%s, len=%d\n",__FUNCTION__,__LINE__,i,buffer, strlen(buffer));//Added for test
 
                //To set cert type, 0: X.509 (only at preset)
                certsInfo[i].certType=0;
 
                //dumpHex(buffer, strlen(buffer)+1);
                if(buffer[0]=='E')
                {
                        //Expired
                        certsInfo[i].certStatus=1;
                }
                else if(buffer[0]=='R')
                {
                        //Revoked
                        certsInfo[i].certStatus=2;
                }
                else
                {
                        //Valid
                        certsInfo[i].certStatus=0;
                }
                //printf("%s(%d): certsInfo[i].certStatus=%d.\n",__FUNCTION__,__LINE__, certsInfo[i].certStatus);//Added for test
 
                //To parse exipred time
                p1=strchr(buffer,'\t');
                if(p1==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                p1++;
                p2=strchr(p1,'\t');
                if(p2==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                if(p2>p1)
                {
                        memset(tmpBuf, 0, sizeof(tmpBuf));
                        //memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf, p1, p2-p1);
                        //printf("%s(%d): tmpBuf=%s.\n",__FUNCTION__,__LINE__, tmpBuf);//Added for test
                        memset(&tm_time, 0 , sizeof(tm_time));
 
                        //?????
                        //tm_time.tm_isdst=-1;

                        //To get year value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, tmpBuf, 2);
                        intVal=atoi(tmpBuf2);
                        if(intVal>=70)
                        {
                                //year: 1970 ~ 1999
                                //year - 1900
                                tm_time.tm_year=(intVal+1900)-1900;
                        }
                        else
                        {
                                //year: 2000 ~ 2069
                                //year - 1900
                                tm_time.tm_year=(intVal+2000)-1900;
                        }
 
                        //To get month value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, &tmpBuf[2], 2);
                        tm_time.tm_mon=atoi(tmpBuf2)-1;
 
                        //To get day value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, &tmpBuf[4], 2);
                        tm_time.tm_mday=atoi(tmpBuf2);
 
                        //To get hour value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, &tmpBuf[6], 2);
                        tm_time.tm_hour=atoi(tmpBuf2);
 
                        //To get minute value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, &tmpBuf[8], 2);
                        tm_time.tm_min=atoi(tmpBuf2);
 
                        //To get second value
                        memset(tmpBuf2, 0, sizeof(tmpBuf2));
                        strncpy(tmpBuf2, &tmpBuf[10], 2);
                        tm_time.tm_sec=atoi(tmpBuf2);
 
                //      printf("(0): %d %d %d %d %d %d, tm_isdst=%d\n", 1900+tm_time.tm_year,tm_time.tm_mon+1,tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec, tm_time.tm_isdst);//Added for test
 
                        expired_tm = mktime(&tm_time);
                        if(expired_tm < 0){
                                printf("Error:set Time Error for tm!\n");//Added for test
                                toRet=FAILED;
                                goto err;
                        }
                //      printf("%s(%d): expired_tm=%ld.\n",__FUNCTION__,__LINE__,expired_tm);//Added for test
 
 
                        now=time(0);
                        tnow=localtime(&now);
                //      printf("now=%ld, %d %d %d %d %d %d, tm_isdst=%d\n",now, 1900+tnow->tm_year,tnow->tm_mon+1,tnow->tm_mday,tnow->tm_hour,tnow->tm_min,tnow->tm_sec, tnow->tm_isdst);//Added for test
 
                        longVal=difftime(expired_tm,now);
//                        printf("The difference is: %ld seconds\n",longVal);
                        if(longVal<=0)
                                certsInfo[i].validDaysLeft=0;
                        else
                                certsInfo[i].validDaysLeft=(unsigned short)(longVal/ONE_DAY_SECONDS)+1;
 
                //      printf("%s(%d): certsInfo[%d].validDaysLeft=%d.\n",__FUNCTION__,__LINE__,i, certsInfo[i].validDaysLeft);//Added for test
 
//                      printf("%s(%d): tmpBuf2=%s.\n",__FUNCTION__,__LINE__, tmpBuf2);//Added for test
                }
 
                //To parse revoked time(Not used now)
                p1=p2;
                p1++;
                p2=strchr(p1,'\t');
                if(p2==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                if(p2>p1)
                {
                        memset(tmpBuf, 0, sizeof(tmpBuf));
                        strncpy(tmpBuf, p1, p2-p1);
//                     printf("%s(%d): tmpBuf=%s.\n",__FUNCTION__,__LINE__, tmpBuf);//Added for test
                }
 
                //To parse serial
                p1=p2;
                p1++;
                p2=strchr(p1,'\t');
                if(p2==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                if(p2>p1)
                {
                        memset(tmpBuf, 0, sizeof(tmpBuf));
                        strncpy(tmpBuf, p1, p2-p1);
//                     printf("%s(%d):serial tmpBuf=%s.\n",__FUNCTION__,__LINE__, tmpBuf);//Added for test
			    certsInfo[i].serial=strtol(tmpBuf, (char **)NULL,16);
//                     ret=str2hex(tmpBuf, &certsInfo[i].serial);
//                     printf("%s(%d), ret=%d, certsInfo[%d].serial=0x%x\n",__FUNCTION__,__LINE__, ret, i,certsInfo[i].serial);//Added for test
#if 0
                        if(ret==FAILED)
                        {
                                printf("%s(%d), str2hex failed.\n",__FUNCTION__,__LINE__);//Added for test
                                toRet=FAILED;
                                goto err;
                        }
#endif						
                }
                //To parse total valid days
                p1=p2;
                p1++;
                p2=strchr(p1,'\t');
                if(p2==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                if(p2>p1)
                {
                        memset(tmpBuf, 0, sizeof(tmpBuf));
                        strncpy(tmpBuf, p1, p2-p1);
                        //printf("%s(%d):total valid days tmpBuf=%s.\n",__FUNCTION__,__LINE__, tmpBuf);//Added for test
                        certsInfo[i].validDays=(unsigned short)atoi(tmpBuf);
                        //printf("%s(%d):certsInfo[%d].validDays=%d.\n",__FUNCTION__,__LINE__, i, certsInfo[i].validDays);//Added for test
                }

		if((certsInfo[i].validDaysLeft>certsInfo[i].validDays)
			||((certsInfo[i].certStatus==1)&&certsInfo[i].validDaysLeft>0))
		{
			//printf("%s(%d), warning: system time setting is not correct.\n",__FUNCTION__,__LINE__);//Added for test
			//To indicate our system hasn't sync time yet
			sprintf(tmpBuf,"echo \"1\" > %s", SYS_TIME_NOT_SYNC_CA);
			system(tmpBuf);
			//End indication
			certsInfo[i].validDaysLeft=0;
		}
 
                //To parse user name
                p1=p2;
                p1++;
                p2=strchr(p1,'\n');
                if(p2==NULL)
                {
                        printf("%s(%d): strchr failed.\n",__FUNCTION__,__LINE__);//Added for test
                        toRet=FAILED;
                        goto err;
                }
                if(p2>p1)
                {
                        memset(tmpBuf, 0, sizeof(tmpBuf));
                        strncpy(tmpBuf, p1, p2-p1);
                        //printf("%s(%d):user name tmpBuf=%s.\n",__FUNCTION__,__LINE__, tmpBuf);//Added for test
                        ptr=NULL;
                        ptr=strstr(p1, "CN=");
                        if(p2==NULL)
                        {
                                printf("%s(%d): strstr failed.\n",__FUNCTION__,__LINE__);//Added for test
                                toRet=FAILED;
                                goto err;
                        }
                        ptr+=3;//Point to user name
                        memset(certsInfo[i].userName, 0, sizeof(certsInfo[i].userName));
                        strncpy(certsInfo[i].userName, ptr, p2-ptr);
                        //printf("%s(%d):certsInfo[%d].userName=%s.\n",__FUNCTION__,__LINE__, i, certsInfo[i].userName);//Added for test
                }
 
//              p1=buffer;
//              p2=strstr(p1,"\t");
//              printf("%s(%d),i=%d,p2=%s, len=%d\n",__FUNCTION__,__LINE__,i,buffer, strlen(p2));//Added for test
        }
 
        toRet=SUCCESS;
 
err:
        if(fp!=NULL)
                fclose(fp);
//        printf("%s(%d), toRet=%d\n",__FUNCTION__,__LINE__,toRet);//Added for tes
        return toRet;
}
static int searchCertStatus(CERTS_DB_ENTRY_Tp all, CERTS_DB_ENTRY_Tp cert,int status, int count)
{
	int i=0;
	int cnt=0;
	for(i=0;i<count;i++)
	{
		if(all[i].certStatus == status)
		{
			memcpy(cert+cnt,all+i,sizeof(CERTS_DB_ENTRY_T));
			cnt++;
		}
	}
	return cnt;
}

static int searchCertName(CERTS_DB_ENTRY_Tp all, CERTS_DB_ENTRY_Tp cert,char *buffer, int count)
{
	int i=0;
	int cnt=0;
	for(i=0;i<count;i++)
	{
		if(!strcmp(all[i].userName, buffer))
		{
			memcpy(cert+cnt,all+i,sizeof(CERTS_DB_ENTRY_T));
			cnt++;
		}
	}
	return cnt;
}
static int searchCertSerial(CERTS_DB_ENTRY_Tp  all, CERTS_DB_ENTRY_Tp cert, unsigned long serial, int count)
{
	int i=0;
	int cnt=0;
	for(i=0;i<count;i++)
	{
		if(all[i].serial==serial)
		{
			memcpy(cert+cnt,all+i,sizeof(CERTS_DB_ENTRY_T));
			cnt++;
		}
	}
	return cnt;
}

int searchWapiCert(CERTS_DB_ENTRY_Tp cert, int index, char *buffer)
{
	int status=0;
	int all=0;
	int count=0,searchCnt=0;
	int retVal;
	unsigned long serial=0;
	CERTS_DB_ENTRY_Tp allCert=(CERTS_DB_ENTRY_Tp)malloc(sizeof(CERTS_DB_ENTRY_T)*128);
	retVal=getCertsDb(allCert,128,&count);
	if(retVal < 0)
	{
		return 0;
	}
	if(count == 0)
	{
		free(allCert);
		return count;
	}
	switch (index)
	{
		case 5:
			/*0 actived, 2 revoked*/
			status=buffer[0]-'0';
			searchCnt=searchCertStatus(allCert,cert,status,count);
			break;
		case 4:
			/*now only support x.509*/
			all=1;
			break;
		case 3:
			/*username is in buffer*/
			searchCnt=searchCertName(allCert,cert,buffer,count);
			break;
		case 2:
			/*serail  in buffer in ASCII*/
			serial=strtol(buffer,(char **)NULL,16);
			searchCnt=searchCertSerial(allCert,cert,serial,count);
			break;
		case 1:		
			/*All*/
			all=1;
			break;
		default:
			/*all*/
			all=1;
			break;
	}
	if(all)
	{
		memcpy(cert,allCert,count*sizeof(CERTS_DB_ENTRY_T));
		searchCnt=count;
	}
	free(allCert);
	return searchCnt;
}
#endif
/////////////////////////////////////////////////////////////////////////////
int getWlStaInfo( char *interface,  WLAN_STA_INFO_Tp pInfo )
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
	}
    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1);
    memset(pInfo, 0, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1));

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSTAINFO, &wrq) < 0){
    	close( skfd );
		return -1;
	}
    close( skfd );
#else
    return -1;
#endif
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus )
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
	}
    wrq.u.data.pointer = (caddr_t)pStatus;

    if ( pStatus->number == 0 )
    	wrq.u.data.length = sizeof(SS_STATUS_T);
    else
        wrq.u.data.length = sizeof(pStatus->number);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSDB, &wrq) < 0){
    	close( skfd );
	return -1;
	}
    close( skfd );
#else
	return -1 ;
#endif

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;
	unsigned char *tmp = NULL;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pBss;
    wrq.u.data.length = sizeof(BssDscr);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLJOINREQ, &wrq) < 0)
	return -1;

    close( skfd );

	tmp = (unsigned char *)wrq.u.data.pointer;
    *res = *(unsigned char *)&tmp[0];
#else
    return -1;
#endif

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getWlJoinResult(char *interface, unsigned char *res)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
	}
    wrq.u.data.pointer = (caddr_t)res;
    wrq.u.data.length = 1;

    if (iw_get_ext(skfd, interface, SIOCGIWRTLJOINREQSTATUS, &wrq) < 0){
    	close( skfd );
	return -1;
	}
    close( skfd );

    return 0;
}



/////////////////////////////////////////////////////////////////////////////
int getWlSiteSurveyRequest(char *interface, int *pStatus)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;
    unsigned char result;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
      /* If no wireless name : no wireless extensions */
      close( skfd );
        return -1;
	}
    wrq.u.data.pointer = (caddr_t)&result;
    wrq.u.data.length = sizeof(result);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSCANREQ, &wrq) < 0){
    	close( skfd );
	return -1;
	}
    close( skfd );

    if ( result == 0xff )
    	*pStatus = -1;
    else
	*pStatus = (int) result;
#else
	*pStatus = -1;
#endif
#ifdef CONFIG_RTK_MESH 
	// ==== modified by GANTOE for site survey 2008/12/26 ==== 
	return (int)*(char*)wrq.u.data.pointer; 
#else
	return 0;
#endif
}

/////////////////////////////////////////////////////////////////////////////
int getWlBssInfo(char *interface, bss_info *pInfo)
{
#ifndef NO_ACTION
    int skfd=0;
    struct iwreq wrq;



    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
#ifdef VOIP_SUPPORT 
	{
		// rock: avoid status page error if no wlan interface
		memset(pInfo, 0, sizeof(bss_info));
        return 0;
	}
#else
      /* If no wireless name : no wireless extensions */
      {
      	 close( skfd );
        return -1;
      }
#endif

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = sizeof(bss_info);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSINFO, &wrq) < 0){
    	 close( skfd );
	return -1;
	}
    close( skfd );
#else
    memset(pInfo, 0, sizeof(bss_info)); 
#endif

    return 0;
}



/////////////////////////////////////////////////////////////////////////////
int getInAddr( char *interface, ADDR_T type, void *pAddr )
{
    struct ifreq ifr;
    int skfd=0, found=0;
    struct sockaddr_in *addr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return 0;
		
    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
    	close( skfd );
		return (0);
	}
    if (type == HW_ADDR) {
    	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
		memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
		found = 1;
	}
    }
    else if (type == IP_ADDR) {
	if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    }
    else if (type == DST_IP_ADDR) {
		if (ioctl(skfd,SIOCGIFDSTADDR, &ifr) == 0) {
			addr = ((struct sockaddr_in *)&ifr.ifr_addr);
			*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
			found = 1;
		}
	}
    else if (type == SUBNET_MASK) {
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    }
    close( skfd );
    return found;

}

/////////////////////////////////////////////////////////////////////////////
extern pid_t find_pid_by_name( char* pidName)
{
	DIR *dir;
	struct dirent *next;
	pid_t pid;
	
	if ( strcmp(pidName, "init")==0)
		return 1;

	dir = opendir("/proc");
	if (!dir) {
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("Cannot open /proc"));
		return 0;
	}

	while ((next = readdir(dir)) != NULL) {
		FILE *status;
		char filename[READ_BUF_SIZE];
		char buffer[READ_BUF_SIZE];
		char name[READ_BUF_SIZE];

		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;

		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;

		sprintf(filename, "/proc/%s/status", next->d_name);
		if (! (status = fopen(filename, "r")) ) {
			continue;
		}
		if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) {
			fclose(status);
			continue;
		}
		fclose(status);

		/* Buffer should contain a string like "Name:   binary_name" */
		sscanf(buffer, "%*s %s", name);
		if (strcmp(name, pidName) == 0) {
		//	pidList=xrealloc( pidList, sizeof(pid_t) * (i+2));
			pid=(pid_t)strtol(next->d_name, NULL, 0);
			closedir(dir);
			return pid;
		}
	}	
	closedir(dir);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#if defined(MULTI_WAN_SUPPORT)
int getDefaultRoute(char *interface, struct in_addr *route)
{
	int wan_idx;
	char wan_info_file[64]={0},  gateway[16]={0};

	wan_idx=getWanIdx(interface);
	snprintf(wan_info_file, sizeof(wan_info_file), "/var/wan_info_%d", wan_idx);
	
	FILE *fp = fopen(wan_info_file, "r");
	if(fp==NULL)
		return 0;

	fscanf(fp, "%s", gateway);
	inet_aton(gateway, route);

    	fclose(fp);
    	return 1;
}

#else

int getDefaultRoute(char *interface, struct in_addr *route)
{
	char buff[1024], iface[16];
	char gate_addr[128], net_addr[128], mask_addr[128];
	int num, iflags, metric, refcnt, use, mss, window, irtt;
	FILE *fp = fopen(_PATH_PROCNET_ROUTE, "r");
	char *fmt;
	int found=0;
	unsigned long addr;

	if (!fp) {
       	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("Open %s file error.\n", _PATH_PROCNET_ROUTE));
		return 0;
    }

	fmt = "%16s %128s %128s %X %d %d %d %128s %d %d %d";

	while (fgets(buff, 1023, fp)) {
		num = sscanf(buff, fmt, iface, net_addr, gate_addr,
		     		&iflags, &refcnt, &use, &metric, mask_addr, &mss, &window, &irtt);
		if (num < 10 || !(iflags & RTF_UP) || !(iflags & RTF_GATEWAY) || strcmp(iface, interface))
	    		continue;
		sscanf(gate_addr, "%lx", &addr );
		*route = *((struct in_addr *)&addr);

		found = 1;
		break;
	}

    	fclose(fp);
    	return found;
}
#endif

bool _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

int mac_string_to_hex(char *string, unsigned char *key, int len)
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
		if(string[idx+2]==':') idx++;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
int getStats(char *interface, struct user_net_device_stats *pStats)
{
 	FILE *fh;
  	char buf[512];
	int type;

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("Warning: cannot open %s\n",_PATH_PROCNET_DEV));
		return -1;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);

  	if (strstr(buf, "compressed"))
		type = 3;
	else if (strstr(buf, "bytes"))
		type = 2;
	else
		type = 1;

	while (fgets(buf, sizeof buf, fh)) {
		char *s, name[40];
		s = get_name(name, buf);
		if ( strcmp(interface, name))
			continue;
		get_dev_fields(type, s, pStats);
		fclose(fh);
		return 0;
    	}
	fclose(fh);
	return -1;
}

void convert_bin_to_str(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		sprintf(tmpbuf, "%02x", bin[i]);
		strcat(out, tmpbuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
static char *get_name(char *name, char *p)
{
    while (isspace(*p))
	p++;
    while (*p) {
	if (isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return NULL;
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

////////////////////////////////////////////////////////////////////////////////
static int get_dev_fields(int type, char *bp, struct user_net_device_stats *pStats)
{
    switch (type) {
    case 3:
	sscanf(bp,
	"%Lu %Lu %lu %lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,
	       &pStats->rx_compressed,
	       &pStats->rx_multicast,

	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors,
	       &pStats->tx_compressed);
	break;

    case 2:
	sscanf(bp, "%Lu %Lu %lu %lu %lu %lu %Lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_bytes,
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,

	       &pStats->tx_bytes,
	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_multicast = 0;
	break;

    case 1:
	sscanf(bp, "%Lu %lu %lu %lu %lu %Lu %lu %lu %lu %lu %lu",
	       &pStats->rx_packets,
	       &pStats->rx_errors,
	       &pStats->rx_dropped,
	       &pStats->rx_fifo_errors,
	       &pStats->rx_frame_errors,

	       &pStats->tx_packets,
	       &pStats->tx_errors,
	       &pStats->tx_dropped,
	       &pStats->tx_fifo_errors,
	       &pStats->collisions,
	       &pStats->tx_carrier_errors);
	pStats->rx_bytes = 0;
	pStats->tx_bytes = 0;
	pStats->rx_multicast = 0;
	break;
    }
    return 0;
}


/////////////////////////////////////////////////////////////////////////////
int getWdsInfo(char *interface, char *pInfo)
{

#ifndef NO_ACTION
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pInfo;
    wrq.u.data.length = MAX_WDS_NUM*sizeof(WDS_INFO_T);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETWDSINFO, &wrq) < 0)
	return -1;

    close( skfd );
#else
    memset(pInfo, 0, MAX_WDS_NUM*sizeof(WDS_INFO_T)); 
#endif

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
int getMiscData(char *interface, struct _misc_data_ *pData)
{

#ifndef NO_ACTION
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pData;
    wrq.u.data.length = sizeof(struct _misc_data_);

    if (iw_get_ext(skfd, interface, SIOCGMISCDATA, &wrq) < 0)
		return -1;

    close(skfd);
#else
    memset(pData, 0, sizeof(struct _misc_data_)); 
#endif

    return 0;
}

/*      IOCTL system call */
static int re865xIoctl(char *name, unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
  unsigned int args[4];
  struct ifreq ifr;
  int sockfd;

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      perror("fatal error socket\n");
      return -3;
    }
  
  strcpy((char*)&ifr.ifr_name, name);
  ((unsigned int *)(&ifr.ifr_data))[0] = (unsigned int)args;

  if (ioctl(sockfd, SIOCDEVPRIVATE, &ifr)<0)
    {
      perror("device ioctl:");
      close(sockfd);
      return -1;
    }
  close(sockfd);
  return 0;
} /* end re865xIoctl */

#ifdef HOME_GATEWAY
int getWanInfo(char *pWanIP, char *pWanMask, char *pWanDefIP, char *pWanHWAddr)
{
	DHCP_T dhcp;
	OPMODE_T opmode=-1;
	unsigned int wispWanId=0;
	char *iface=NULL;
	struct in_addr	intaddr;
	struct sockaddr hwaddr;
	unsigned char *pMacAddr;
	
	if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
		return -1;
  
  if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
		return -1;

	if( !apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId))
		return -1;
	
	if ( dhcp == PPPOE || dhcp == PPTP || dhcp == L2TP || dhcp == USB3G ) { /* # keith: add l2tp support. 20080515 */
#ifdef MULTI_PPPOE
	if(dhcp == PPPOE){
		extern char  ppp_iface[32];
		iface = ppp_iface;
	}
#else
	iface = "ppp0";
#endif
		
		if ( !isConnectPPP() )
			iface = NULL;
	}
	else if (opmode == WISP_MODE){
		if(0 == wispWanId)
			iface = "wlan0";
		else if(1 == wispWanId)
			iface = "wlan1";
	}
	else
		iface = "eth1";
	
	if(opmode != WISP_MODE)
	{
		if(iface){
			if(getWanLink("eth1") < 0){
				sprintf(pWanIP,"%s","0.0.0.0");
			}
		}	
	}
	
	if ( iface && getInAddr(iface, IP_ADDR, (void *)&intaddr ) )
		sprintf(pWanIP,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanIP,"%s","0.0.0.0");

	if ( iface && getInAddr(iface, SUBNET_MASK, (void *)&intaddr ) )
		sprintf(pWanMask,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanMask,"%s","0.0.0.0");
	
	if ( iface && getDefaultRoute(iface, &intaddr) )
		sprintf(pWanDefIP,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanDefIP,"%s","0.0.0.0");	

	//To get wan hw addr
	if(opmode == WISP_MODE) {
		if(0 == wispWanId)
			iface = "wlan0";
		else if(1 == wispWanId)
			iface = "wlan1";
	}	
	else
		iface = "eth1";
	
	if ( getInAddr(iface, HW_ADDR, (void *)&hwaddr ) ) 
	{
		pMacAddr = (unsigned char *)hwaddr.sa_data;
		sprintf(pWanHWAddr,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
	}
	else
		sprintf(pWanHWAddr,"%s","00:00:00:00:00:00");

	return 0;
}
#endif

/* ethernet port link status */
int getEth0PortLink(unsigned int port_index)
{
    int    ret=-1;
    unsigned int    args[0];
        
    ret = port_index;
    re865xIoctl("eth0", RTL8651_IOCTL_GETLANLINKSTATUS, (unsigned int)(args), 0, (unsigned int)&ret);
    if(ret ==0){
    	return 1;
    }else if(ret < 0){
    	return 0;
    }
    return 0;
}

#if defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_8198) || defined(CONFIG_RTL_819XD)//only support in 8196c or RTL8198
/* RTL8651_IOCTL_GET_ETHER_EEE_STATE */
int getEthernetEeeState(unsigned int port_index)
{
      unsigned int    ret=0;
      unsigned int    args[0];
 			
 			re865xIoctl("eth0", RTL8651_IOCTL_GET_ETHER_EEE_STATE, (unsigned int)(args), 0, (unsigned int)&ret);      
    	
			//if(ret & (1<<(3+port_index*4)))
			if(ret & (1<<(1+port_index*4)))
      	return 1;
      else
      	return 0;
}
#endif //#if defined(CONFIG_RTL_8196C)

/* RTL8651_IOCTL_GET_ETHER_BYTES_COUNT */
unsigned int getEthernetBytesCount(unsigned int port_index)
{
      unsigned int    ret=0;
      unsigned int    args[0];
 			
 			ret = port_index;
 			re865xIoctl("eth0", RTL8651_IOCTL_GET_ETHER_BYTES_COUNT, (unsigned int)(args), 0, (unsigned int)&ret);      
    	
      return ret;
}


#define RTL8651_IOCTL_GETWANLINKSTATUS 2000
/* Wan link status detect */
int getWanLink(char *interface)
{
        unsigned int    ret;
        unsigned int    args[0];

        re865xIoctl(interface, RTL8651_IOCTL_GETWANLINKSTATUS, (unsigned int)(args), 0, (unsigned int)&ret) ;
        return ret;
}

#ifdef UNIVERSAL_REPEATER
/////////////////////////////////////////////////////////////////////////////
int isVxdInterfaceExist(char *interface)
{
#ifndef NO_ACTION	
	int skfd, ret;  
	struct ifreq ifr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(ifr.ifr_name, interface);
	if ((ret = ioctl(skfd, SIOCGIFFLAGS, &ifr)) < 0)
		ret = 0;
	else {
		if (!(ifr.ifr_flags & IFF_UP))
			ret = 0;	
		else
			ret = 1;
     }
	close(skfd);
	
	return ret;	
#else	

	return 0;
#endif	
	
}
#endif // UNIVERSAL_REPEATER

int fwChecksumOk(char *data, int len)
{
	unsigned short sum=0;
	int i;

	for (i=0; i<len; i+=2) {
#ifdef _LITTLE_ENDIAN_
		sum += WORD_SWAP( *((unsigned short *)&data[i]) );
#else
		sum += *((unsigned short *)&data[i]);
#endif

	}
	return( (sum==0) ? 1 : 0);
}

///////////////////////////////////////////////////////////////////////////////
int do_cmd(const char *filename, char *argv [], int dowait)
{
	pid_t pid, wpid;
	int stat=0, st;
	
	if((pid = vfork()) == 0) {
		/* the child */
		char *env[3];
		
		signal(SIGINT, SIG_IGN);
		argv[0] = (char *)filename;
		env[0] = "PATH=/bin:/usr/bin:/etc:/sbin:/usr/sbin";
		env[1] = NULL;

		execve(filename, argv, env);

		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("exec %s failed\n", filename));
		_exit(2);
	} else if(pid > 0) {
		if (!dowait)
			stat = 0;
		else {
			/* parent, wait till rc process dies before spawning */
			while ((wpid = wait(&st)) != pid)
				if (wpid == -1 && errno == ECHILD) { /* see wait(2) manpage */
					stat = 0;
					break;
				}
		}
	} else if(pid < 0) {
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("fork of %s failed\n", filename));
		stat = -1;
	}
	return st;
}

int va_cmd(const char *cmd, int num, int dowait, ...)
{
	va_list ap;
	int k;
	char *s;
	char *argv[24];
	int status;
	
	va_start(ap, dowait);
	
	for (k=0; k<num; k++)
	{
		s = va_arg(ap, char *);
		argv[k+1] = s;
	}
	
	argv[k+1] = NULL;
	status = do_cmd(cmd, argv, dowait);
	va_end(ap);
	
	return status;
}



void kill_processes(void)
{

#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
    return ;
#else
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("upgrade: killing tasks...\n"));
	
	kill(1, SIGTSTP);		/* Stop init from reforking tasks */
	kill(1, SIGSTOP);		
	kill(2, SIGSTOP);		
	kill(3, SIGSTOP);		
	kill(4, SIGSTOP);		
	kill(5, SIGSTOP);		
	kill(6, SIGSTOP);		
	kill(7, SIGSTOP);		
	//atexit(restartinit);		/* If exit prematurely, restart init */
	sync();

	signal(SIGTERM,SIG_IGN);	/* Don't kill ourselves... */
	setpgrp(); 			/* Don't let our parent kill us */
	sleep(1);
	signal(SIGHUP, SIG_IGN);	/* Don't die if our parent dies due to
					 * a closed controlling terminal */
#endif
}


static int daemonKilled = 0;
void killDaemon(int wait)
{
    if (daemonKilled)
    	return;

    daemonKilled = 1;

	system("echo 1,0 > /proc/br_mCastFastFwd ");
#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
    // do nothing, they should have enough memory
    return ;
#else
	va_cmd( "/bin/killall", 2, 1, "-9","sleep");
	va_cmd( "/bin/killall", 2, 1, "-9","routed");
	va_cmd("/bin/killall", 2, 1, "-9","pppoe");
	va_cmd("/bin/killall", 2, 1, "-9","pppd ");
	va_cmd("/bin/killall", 2, 1, "-9","pptp");
	va_cmd("/bin/killall", 2, 1, "-9","dnrd");
	va_cmd("/bin/killall", 2, 1, "-9","ntpclient");
	va_cmd("/bin/killall", 2, 1, "-9","miniigd");
	va_cmd("/bin/killall", 2, 1, "-9","lld2d");
	va_cmd("/bin/killall", 2, 1, "-9","l2tpd");	
	va_cmd("/bin/killall", 2, 1, "-9","udhcpc");	
	va_cmd("/bin/killall", 2, 1, "-9","udhcpd");	
	va_cmd("/bin/killall", 2, 1, "-9","reload");		
	va_cmd("/bin/killall", 2, 1, "-9","iapp");	
	va_cmd("/bin/killall", 2, 1, "-9","wscd");
	va_cmd("/bin/killall", 2, 1, "-9","mini_upnpd");
	va_cmd("/bin/killall", 2, 1, "-9","iwcontrol");
	va_cmd("/bin/killall", 2, 1, "-9","auth");
	va_cmd("/bin/killall", 2, 1, "-9","disc_server");
	va_cmd("/bin/killall", 2, 1, "-9","igmpproxy");
	va_cmd("/bin/killall", 2, 1, "-9","syslogd");
	va_cmd("/bin/killall", 2, 1, "-9","klogd");
	va_cmd("/bin/killall", 2, 1, "-9","ntfs-3g");
	va_cmd("/bin/killall", 2, 1, "-9","smbd");
	va_cmd("/bin/killall", 2, 1, "-9","boa");
	va_cmd("/bin/killall", 2, 1, "-9","timelycheck");
	
#ifdef VOIP_SUPPORT
	va_cmd("/bin/killall", 2, 1, "-9","snmpd");
	va_cmd("/bin/killall", 2, 1, "-9","solar_monitor");
	va_cmd("/bin/killall", 2, 1, "-9","solar");
	va_cmd("/bin/killall", 2, 1, "-9","dns_task");
	va_cmd("/bin/killall", 2, 1, "-9","ivrserver");
	va_cmd("/bin/killall", 2, 1, "-9","fwupdate");
	va_cmd("/bin/killall", 2, 1, "-9","netlink");
#endif	
#ifdef CONFIG_IPV6
	va_cmd("/bin/killall", 2, 1, "-9","mldproxy");
	va_cmd("/bin/killall", 2, 1, "-9","dhcp6c");
	va_cmd("/bin/killall", 2, 1, "-9","dhcp6s");
	va_cmd("/bin/killall", 2, 1, "-9","radvd");
	va_cmd("/bin/killall", 2, 1, "-9","ecmh");
	va_cmd("/bin/killall", 2, 1, "-9","ripng");
	va_cmd("/bin/killall", 2, 1, "-9","zebra");	
	va_cmd("/bin/killall", 2, 1, "-9","dnsmasq");
#endif	
#ifdef CONFIG_SNMP	
	va_cmd("/bin/killall", 2, 1, "-9","snmpd");
#endif
#endif

}

int cmd_reboot()
{
    char cmd[32] = {0};
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("***%s:%s:%d***\n",__FILE__,__FUNCTION__,__LINE__));
#ifdef MULTI_WAN_SUPPORT
    sprintf(cmd, "reinitCli -e %d &", REINIT_EVENT_APPLYCAHNGES);
	printf("%s\n", cmd);
    system(cmd);
#else
	system("reboot");
	exit(0);
#endif
	return 0;
}

const char *dhcp_mode[] = {
	"None", "DHCP Relay", "DHCP Server"
};
const char *wlan_band[] = {
	0, "2.4 GHz (B)", "2.4 GHz (G)", "2.4 GHz (B+G)", 0
	, 0, 0, 0, "2.4 GHz (N)", 0, "2.4 GHz (G+N)", "2.4 GHz (B+G+N)", 0
};

const char *wlan_mode[] = {
	//"AP", "Client", "AP+WDS"
	"AP", "Client", "WDS", "AP+WDS"
};

const char *wlan_rate[] = {
	"1M", "2M", "5.5M", "11M", "6M", "9M", "12M", "18M", "24M", "36M", "48M", "54M"
	, "MCS0", "MCS1", "MCS2", "MCS3", "MCS4", "MCS5", "MCS6", "MCS7", "MCS8", "MCS9", "MCS10", "MCS11", "MCS12", "MCS13", "MCS14", "MCS15"
};

const char *wlan_auth[] = {
	"Open", "Shared", "Auto"
};

//modified  by xl_yue

const char *wlan_preamble[] = {
	"Long", "Short"
};


const char *wlan_encrypt[] = {
	"None",
	"WEP",
	"WPA(TKIP)",
	"WPA(AES)",
	"WPA2(AES)",
	"WPA2(TKIP)",
	"WPA2 Mixed",
	"",
};

const char *wlan_pskfmt[] = {
	"Passphrase", "Hex"
};

const char *wlan_wepkeylen[] = {
	"Disable", "64-bit", "128-bit"
};

const char *wlan_wepkeyfmt[] = {
	"ASCII", "Hex"
};

const char *wlan_Cipher[] = {
	"TKIP", "AES", "Both"
};

int getWlanMib(int wlanRootIndex, int wlanValIndex, int id, void *value)
{
	int root_old, val_old;

	root_old = wlan_idx;
	val_old = vwlan_idx;

	wlan_idx = wlanRootIndex;
	vwlan_idx = wlanValIndex;
	
	apmib_get(id, (void *)value);
	
	wlan_idx = root_old;
	vwlan_idx = val_old;
	
	return 0;
}

int setWlanMib(int wlanRootIndex, int wlanValIndex, int id, void *value)
{
	int root_old, val_old;

	root_old = wlan_idx;
	val_old = vwlan_idx;

	wlan_idx = wlanRootIndex;
	vwlan_idx = wlanValIndex;
	
	apmib_set(id, (void *)value);
	
	wlan_idx = root_old;
	vwlan_idx = val_old;
	
	return 0;
}

char wlan_ifname[20]={0};
int getWlanBssInfo(int wlanRootIndex, int wlanValIndex, void *bss)
{
	int root_old, val_old;
	
	root_old = wlan_idx;
	val_old = vwlan_idx;
	
	wlan_idx = wlanRootIndex;
	vwlan_idx = wlanValIndex;
	
	if (vwlan_idx == 0)
	{
		sprintf(wlan_ifname, "wlan%d", wlan_idx);
	}
	else if (vwlan_idx > 0 && vwlan_idx < 5) //repeater is 5; wds is 6
	{
		sprintf(wlan_ifname, "wlan%d-va%d", wlan_idx, vwlan_idx-1);
	}
	else if (vwlan_idx == 5) //repeater is 5; wds is 6
	{
		sprintf(wlan_ifname, "wlan%d-vxd", wlan_idx);
	}
	else if (vwlan_idx == 6) //repeater is 5; wds is 6
	{
		sprintf(wlan_ifname, "wlan%d-wds", wlan_idx);
	}

	if( getWlBssInfo(wlan_ifname, bss) !=0 )
	{
		wlan_idx = root_old;
		vwlan_idx = val_old;
		return -1;		
	}
	
	wlan_idx = root_old;
	vwlan_idx = val_old;
		
	return 0;
}

static int getWlanStatsFileValue(char *line, unsigned int *value)
{
	char string[64];
	unsigned char *p;
	sscanf(line, "%*[^:]:%[^?]",string);
	p=string;
	while(*p == ' ')
		p++;
	*value = atoi(p);

	return 0;
}

int getWlanStatsInfo(char *ifname, wlan_stats *stats_info)
{
	char filename[32], line[100], string[64];
	FILE *fp=NULL;
	unsigned int tmp = 0;	
	
	sprintf(filename, "proc/%s/stats",ifname);	
	//printf("filename(%s), ifname(%s)\n",filename, ifname);
	fp=fopen( filename, "r" );
	if(fp)
	{
		memset(stats_info,0,sizeof(wlan_stats));
		fgets(line, sizeof(line), fp);
		while(!feof(fp))
		{		
			if( strstr(line,"tx_packets") ) 			// tx packets
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalPacketsSent = tmp;				
				
			}else if( strstr(line,"tx_bytes") )			// tx bytes
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalBytesSent = tmp;				
				
			}else if( strstr(line,"tx_ucast_pkts") )		// tx ucast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->unicastPacketsSent = tmp;
				
			}else if( strstr(line,"tx_mcast_pkts") )		// tx mcast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->multicastPacketsSent = tmp;
				
			}else if( strstr(line,"tx_bcast_pkts") )		// tx bcast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->broadcastPacketsSent = tmp;
				
			}else if( strstr(line,"tx_fails") )				// tx errors
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->errorsSent = tmp;
				
			}else if( strstr(line,"tx_drops") )				// tx discard
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->discardPacketsSent = tmp;
				
			}else if( strstr(line,"rx_packets") )			// rx packets
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalPacketsReceived = tmp;
				
			}else if( strstr(line,"rx_bytes") )				// rx bytes
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalBytesReceived = tmp;
				
			}else if( strstr(line,"rx_ucast_pkts") )		// rx ucast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->unicastPacketsReceived = tmp;
				
			}else if( strstr(line,"rx_mcast_pkts") )		// rx mcast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->multicastPacketsReceived = tmp;
				
			}else if( strstr(line,"rx_bcast_pkts") )		// rx bcast
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->broadcastPacketsReceived = tmp;
				
			}else if( strstr(line,"rx_errors") )			// rx errors
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->errorsReceived = tmp;
				
			}else if( strstr(line,"rx_data_drops") )		// rx discard
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->discardPacketsReceived = tmp;
				
			}else if( strstr(line,"unknown_pro_pkts") ) 	// unknown
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->unknownProtoPacketsReceived = tmp;
				
			}else if( strstr(line,"total_psk_fail") )		// total psk fail
			{	
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalPSKFailures = tmp;

			}else if( strstr(line,"total_mic_fail") )		// total mic check fail
			{
				getWlanStatsFileValue(line, &tmp);
				stats_info->totalIntegrityFailures = tmp;
			}
			fgets(line, sizeof(line), fp);			
		}	
		fclose(fp);
		return 0;
	}
	return -1;
}

int IS_MCAST(unsigned char *da)
{
	if ((*da) & 0x01)
		return 1;
	else
		return 0;
}

static int updateConfigIntoFlash(unsigned char *data, int total_len, int *pType, int *pStatus)
{
	int len=0, status=1, type=0, ver, force;
#ifdef HEADER_LEN_INT
		HW_PARAM_HEADER_Tp phwHeader;
		int isHdware=0;
#endif
	PARAM_HEADER_Tp pHeader;
#ifdef COMPRESS_MIB_SETTING
	COMPRESS_MIB_HEADER_Tp pCompHeader;
	unsigned char *expFile=NULL;
	unsigned int expandLen=0;
	int complen=0;
#endif
	char *ptr;

	do {
#ifdef COMPRESS_MIB_SETTING
		pCompHeader =(COMPRESS_MIB_HEADER_Tp)&data[complen];
#ifdef _LITTLE_ENDIAN_
		pCompHeader->compRate = WORD_SWAP(pCompHeader->compRate);
		pCompHeader->compLen = DWORD_SWAP(pCompHeader->compLen);
#endif
		/*decompress and get the tag*/
		expFile=malloc(pCompHeader->compLen*pCompHeader->compRate);
		if(NULL==expFile)
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("malloc for expFile error!!\n"));
			return 0;
		}
		expandLen = Decode(data+complen+sizeof(COMPRESS_MIB_HEADER_T), pCompHeader->compLen, expFile);
		pHeader = (PARAM_HEADER_Tp)expFile;
#ifdef HEADER_LEN_INT

		if (memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN)==0 ||
			memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN)==0 ||
			memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN)==0 )
			{
				isHdware=1;
				phwHeader=(HW_PARAM_HEADER_Tp)expFile;
			}			
#endif
#else
#ifdef HEADER_LEN_INT
		if(isHdware)
			phwHeader=(HW_PARAM_HEADER_Tp)&data[len];
		else
#endif
		pHeader = (PARAM_HEADER_Tp)&data[len];
#endif
		
#ifdef _LITTLE_ENDIAN_
#ifdef HEADER_LEN_INT
		if(isHdware)
			phwHeader->len = WORD_SWAP(phwHeader->len);
		else
#endif
		pHeader->len = HEADER_SWAP(pHeader->len);
#endif
#ifdef HEADER_LEN_INT
		if(isHdware)
			len += sizeof(HW_PARAM_HEADER_T);
		else
#endif
		len += sizeof(PARAM_HEADER_T);

		if ( sscanf(&pHeader->signature[TAG_LEN], "%02d", &ver) != 1)
			ver = -1;
			
		force = -1;
		if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1; // update
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN))
			force = 2; // force
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN))
			force = 0; // upgrade

		if ( force >= 0 ) {
#if 0
			if ( !force && (ver < CURRENT_SETTING_VER || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
			
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}

//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
		#ifndef VOIP_SUPPORT_TLV_CFG
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMib->voipCfgParam);
#endif
#endif

#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(CURRENT_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(CURRENT_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(CURRENT_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif
			type |= CURRENT_SETTING;
			continue;
		}


		if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1;	// update
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
			force = 2;	// force
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) )
			force = 0;	// upgrade

		if ( force >= 0 ) {
#if 0
			if ( (ver < DEFAULT_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}

//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
		#ifndef VOIP_SUPPORT_TLV_CFG
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMibDef->voipCfgParam);
#endif
#endif

#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(DEFAULT_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(DEFAULT_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(DEFAULT_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}	
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif
			type |= DEFAULT_SETTING;
			continue;
		}

		if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN) )
			force = 1;	// update
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) )
			force = 2;	// force
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) )
			force = 0;	// upgrade

		if ( force >= 0 ) {
#if 0
			if ( (ver < HW_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(HW_SETTING_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif
#ifdef COMPRESS_MIB_SETTING
#ifdef HEADER_LEN_INT
			if(isHdware)
				ptr = expFile+sizeof(HW_PARAM_HEADER_T);
			else
#endif
			ptr = expFile+sizeof(PARAM_HEADER_T);
#else
			ptr = &data[len];
#endif
			

#ifdef COMPRESS_MIB_SETTING
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				DECODE_DATA(ptr, phwHeader->len);
			else
#endif
			DECODE_DATA(ptr, pHeader->len);
#endif
#ifdef HEADER_LEN_INT
			if(isHdware)
			{
				if ( !CHECKSUM_OK(ptr, phwHeader->len)) {
				status = 0;
				break;
				}
			}
			else
#endif
			if ( !CHECKSUM_OK(ptr, pHeader->len)) {
				status = 0;
				break;
			}
#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(HW_SETTING, &data[complen], pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				apmib_updateFlash(HW_SETTING, ptr, phwHeader->len-1, force, ver);
			else
#endif
			apmib_updateFlash(HW_SETTING, ptr, pHeader->len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			complen += pCompHeader->compLen+sizeof(COMPRESS_MIB_HEADER_T);
			if(expFile)
			{
				free(expFile);
				expFile=NULL;
			}
#else
#ifdef HEADER_LEN_INT
			if(isHdware)
				len += phwHeader->len;
			else
#endif
			len += pHeader->len;
#endif

			type |= HW_SETTING;
			continue;
		}
	}
#ifdef COMPRESS_MIB_SETTING	
	while (complen < total_len);
#else
	while (len < total_len);
#endif
	if(expFile)
	{
		free(expFile);
		expFile=NULL;
	}

	*pType = type;
	*pStatus = status;
#ifdef COMPRESS_MIB_SETTING	
	return complen;
#else
	return len;
#endif
}

int doUpdateConfigIntoFlash(unsigned char *data, int total_len, int *pType, int *pStatus)
{
	int len=0, status=1, type=0, ver, force;
	PARAM_HEADER_T param_head={0};
	PARAM_HEADER_Tp pHeader=&param_head;
#ifdef COMPRESS_MIB_SETTING
	COMPRESS_MIB_HEADER_Tp pCompHeader;
	unsigned char *expFile=NULL;
	unsigned int expandLen=0;
	/*Always use BE in apmib,so we save parameteres translated below.*/
	int currLen = 0;
	int Comp_Rate = 0,Comp_Len = 0,Header_Len = 0;
#endif
	char *ptr;
	unsigned char isValidfw = 0;
//printf("total_len=%d\n",total_len);
	do {
		if (
#ifdef COMPRESS_MIB_SETTING
			memcmp(&data[currLen], COMP_HS_SIGNATURE, COMP_SIGNATURE_LEN) &&
			memcmp(&data[currLen], COMP_DS_SIGNATURE, COMP_SIGNATURE_LEN) &&
			memcmp(&data[currLen], COMP_CS_SIGNATURE, COMP_SIGNATURE_LEN)
#else
			memcmp(&data[len], CURRENT_SETTING_HEADER_TAG, TAG_LEN) &&
			memcmp(&data[len], CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN) &&
			memcmp(&data[len], CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) &&
			memcmp(&data[len], DEFAULT_SETTING_HEADER_TAG, TAG_LEN) &&
			memcmp(&data[len], DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) &&
			memcmp(&data[len], DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) &&
			memcmp(&data[len], HW_SETTING_HEADER_TAG, TAG_LEN) &&
			memcmp(&data[len], HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) &&
			memcmp(&data[len], HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) 
#endif
		) {
			if (isValidfw == 1){
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Tag is invalid!\n",__FUNCTION__,__LINE__));
				break;
			}
		}
		
#ifdef COMPRESS_MIB_SETTING
		pCompHeader =(COMPRESS_MIB_HEADER_Tp)&data[currLen];
#ifdef _LITTLE_ENDIAN_
		Comp_Rate = WORD_SWAP(pCompHeader->compRate);
		Comp_Len = DWORD_SWAP(pCompHeader->compLen);
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Comp_Rate=%d,Comp_Len=%d!\n",__FUNCTION__,__LINE__,Comp_Rate,Comp_Len));
#else
		Comp_Rate = pCompHeader->compRate;
		Comp_Len = pCompHeader->compLen;
#endif
		/*decompress and get the tag*/
		if(expFile) free(expFile);
		expFile=malloc(Comp_Rate*Comp_Len);
		
		if (NULL==expFile) {
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("malloc for expFile error!!\n"));
			return 0;
		}
		expandLen = Decode(data+currLen+sizeof(COMPRESS_MIB_HEADER_T), Comp_Len, expFile);
		
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>expandLen=%d!\n",__FUNCTION__,__LINE__,expandLen));
		memcpy(pHeader,expFile,sizeof(PARAM_HEADER_T));
#else
		memcpy(pHeader,&data[len],sizeof(PARAM_HEADER_T));
		//pHeader = (PARAM_HEADER_Tp)&data[len];
#endif
		
#ifdef _LITTLE_ENDIAN_
		pHeader->len = HEADER_SWAP(pHeader->len);
#endif
		len += sizeof(PARAM_HEADER_T);

		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Header_Len=%d,len=%d,pHeader->len=%x!\n",__FUNCTION__,__LINE__,Header_Len,len,pHeader->len));
		if ( sscanf((char *)&pHeader->signature[TAG_LEN], "%02d", &ver) != 1)
			ver = -1;
			
		force = -1;
		if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 1; // update
		}
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN)) {
			isValidfw = 1;
			force = 2; // force
		}
		else if ( !memcmp(pHeader->signature, CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN)) {
			isValidfw = 1;
			force = 0; // upgrade
		}

		if ( force >= 0 ) {
#if 0
			if ( !force && (ver < CURRENT_SETTING_VER || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
		ptr = (char *)(expFile+sizeof(PARAM_HEADER_T));
#else
		ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
		DECODE_DATA(ptr, Header_Len);
#endif
		if (!CHECKSUM_OK((unsigned char *)ptr, Header_Len)) {
			printf("pHeader->len=%d\n",Header_Len);
			status = 0;
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Check Sum failed!\n",__FUNCTION__,__LINE__));
			break;
		}
//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMib->voipCfgParam);
#endif

#ifdef COMPRESS_MIB_SETTING
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("%s:call  apmib_updateFlash CURRENT_SETTING\n", __FUNCTION__));
			apmib_updateFlash(CURRENT_SETTING, (char *)&data[currLen], Comp_Len+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
			apmib_updateFlash(CURRENT_SETTING, ptr, Header_Len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
						currLen += Comp_Len+sizeof(COMPRESS_MIB_HEADER_T);
						if (expFile) {
							free(expFile);
							expFile=NULL;
						}
#else
						len += Header_Len;
#endif
			type |= CURRENT_SETTING;
			continue;
		}


		if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 1;	// update
		}
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 2;	// force
		}
		else if ( !memcmp(pHeader->signature, DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 0;	// upgrade
		}

		if ( force >= 0 ) {
#if 0
			if ( (ver < DEFAULT_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(APMIB_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif

#ifdef COMPRESS_MIB_SETTING
			ptr = (char *)(expFile+sizeof(PARAM_HEADER_T));
#else
			ptr = &data[len];
#endif

#ifdef COMPRESS_MIB_SETTING
#else
				DECODE_DATA(ptr, Header_Len);
#endif
				if ( !CHECKSUM_OK((unsigned char *)ptr, Header_Len)) {
				printf("pHeader->len=%d\n",Header_Len);
				status = 0;				
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Check Sum failed!\n",__FUNCTION__,__LINE__));
				break;
			}

//#ifdef _LITTLE_ENDIAN_
//			swap_mib_word_value((APMIB_Tp)ptr);
//#endif

// added by rock /////////////////////////////////////////
#ifdef VOIP_SUPPORT
			flash_voip_import_fix(&((APMIB_Tp)ptr)->voipCfgParam, &pMibDef->voipCfgParam);
#endif

#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(DEFAULT_SETTING, (char *)&data[currLen],Comp_Len+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
			apmib_updateFlash(DEFAULT_SETTING, ptr, Header_Len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			currLen += Comp_Len+sizeof(COMPRESS_MIB_HEADER_T);
			if (expFile) {
				free(expFile);
				expFile=NULL;
			}	
#else
			len += Header_Len;
#endif
			type |= DEFAULT_SETTING;
			continue;
		}

		if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 1;	// update
		}
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 2;	// force
		}
		else if ( !memcmp(pHeader->signature, HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ) {
			isValidfw = 1;
			force = 0;	// upgrade
		}

		if ( force >= 0 ) {
#if 0
			if ( (ver < HW_SETTING_VER) || // version is less than current
				(pHeader->len < (sizeof(HW_SETTING_T)+1)) ) { // length is less than current
				status = 0;
				break;
			}
#endif
#ifdef COMPRESS_MIB_SETTING
			ptr = (char *)(expFile+sizeof(PARAM_HEADER_T));
#else
			ptr = &data[len];
#endif
			

#ifdef COMPRESS_MIB_SETTING
#else
			DECODE_DATA(ptr, Header_Len);
#endif
			if ( !CHECKSUM_OK((unsigned char *)ptr, Header_Len)) {
				printf("pHeader->len=%d\n",pHeader->len);
				status = 0;
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("<%s:%d>Check Sum failed!\n",__FUNCTION__,__LINE__));
				break;
			}
#ifdef COMPRESS_MIB_SETTING
			apmib_updateFlash(HW_SETTING, (char *)&data[currLen], Comp_Len+sizeof(COMPRESS_MIB_HEADER_T), force, ver);
#else
			apmib_updateFlash(HW_SETTING, ptr, Header_Len-1, force, ver);
#endif

#ifdef COMPRESS_MIB_SETTING
			currLen += Comp_Len+sizeof(COMPRESS_MIB_HEADER_T);
			if (expFile) {
				free(expFile);
				expFile=NULL;
			}
#else
			len += Header_Len;
#endif

			type |= HW_SETTING;
			continue;
		}
	}
#ifdef COMPRESS_MIB_SETTING	
	while (currLen < total_len); // end do-while

	if (expFile) {
		free(expFile);
		expFile=NULL;
	}
#else
	while (len < total_len);
#endif

	*pType = type;
	*pStatus = status;
	if(type != 0 && status != 0){
		apmib_reinit();
	}
#ifdef COMPRESS_MIB_SETTING	
	return currLen;
#else
	return len;
#endif
}

char *get_ifname(char *name, char *p)
{
	while (isspace(*p))
		p++;
	while (*p) {
		if (isspace(*p))
			break;
		if (*p == ':') {	/* could be an alias */
			char *dot = p, *dotname = name;

			*name++ = *p++;
			while (isdigit(*p))
				*name++ = *p++;
			if (*p != ':') {	/* it wasn't, backup */
				p = dot;
				name = dotname;
			}
			if (*p == '\0')
				return NULL;
			p++;
			break;
		}
		*name++ = *p++;
	}
	*name++ = '\0';
	return p;
}
#define _PATH_PROCNET_DEV "/proc/net/dev"
int shutdown_netdev(void)
{
	FILE *fh;
	char buf[512];
	char *s, name[16], tmp_str[16];
	int iface_num=0;
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		return 0;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);
	while (fgets(buf, sizeof buf, fh)) {
		s = get_ifname(name, buf);
		if(strstr(name, "eth") || strstr(name, "wlan") || strstr(name, "ppp")|| strstr(name, "sit")){
			iface_num++;
			va_cmd( "/bin/ifconfig", 2, 1, name,"down");
		}
	}
	
	fclose(fh);
	return iface_num;
}
int doFirmwareUpgrade(char *upload_data, int upload_len, int is_root, char *buffer)
{
int head_offset=0 ;
int isIncludeRoot=0;
 int		 len;
    int          locWrite;
    int          numLeft;
    int          numWrite;
    IMG_HEADER_Tp pHeader;
	int flag=0, startAddr=-1, startAddrWeb=-1;
	int update_fw=0, update_cfg=0;
#ifdef __mips__
    int fh;
#else
    FILE *       fp;
    char_t *     bn = NULL;
#endif
	unsigned char cmdBuf[30];
#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
    // do nothing
#else
	shutdown_netdev();
	system("ifconfig br0 down 2> /dev/null");
#endif
	killDaemon(1);

while(head_offset <   upload_len) {
    locWrite = 0;
    pHeader = (IMG_HEADER_Tp) &upload_data[head_offset];
    len = pHeader->len;
#ifdef _LITTLE_ENDIAN_
    len  = DWORD_SWAP(len);
#endif    
    numLeft = len + sizeof(IMG_HEADER_T) ;
    
    // check header and checksum
    if (!memcmp(&upload_data[head_offset], FW_HEADER, SIGNATURE_LEN) || !memcmp(&upload_data[head_offset], FW_HEADER_WITH_ROOT, SIGNATURE_LEN))
    	flag = 1;
    else if (!memcmp(&upload_data[head_offset], WEB_HEADER, SIGNATURE_LEN))
    	flag = 2;
    else if (!memcmp(&upload_data[head_offset], ROOT_HEADER, SIGNATURE_LEN))
    {
    	flag = 3;
    	isIncludeRoot = 1;
    }
    else if (
#ifdef COMPRESS_MIB_SETTING
				!memcmp(&upload_data[head_offset], COMP_HS_SIGNATURE, COMP_SIGNATURE_LEN) ||
				!memcmp(&upload_data[head_offset], COMP_DS_SIGNATURE, COMP_SIGNATURE_LEN) ||
				!memcmp(&upload_data[head_offset], COMP_CS_SIGNATURE, COMP_SIGNATURE_LEN)
#else	
    	 	!memcmp(&upload_data[head_offset], CURRENT_SETTING_HEADER_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], CURRENT_SETTING_HEADER_FORCE_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], CURRENT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], DEFAULT_SETTING_HEADER_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], DEFAULT_SETTING_HEADER_FORCE_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], DEFAULT_SETTING_HEADER_UPGRADE_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], HW_SETTING_HEADER_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], HW_SETTING_HEADER_FORCE_TAG, TAG_LEN) ||
				!memcmp(&upload_data[head_offset], HW_SETTING_HEADER_UPGRADE_TAG, TAG_LEN)
#endif				
		)
    {
		int type, status, cfg_len;
			
		cfg_len = updateConfigIntoFlash(&upload_data[head_offset], 0, &type, &status);
		
		if (status == 0 || type == 0) { // checksum error
			strcpy(buffer, "Invalid configuration file!");
			goto ret_upload;
		}
		else { // upload success
			strcpy(buffer, "Update successfully!");
			head_offset += cfg_len;
			update_cfg = 1;
		}    	
		continue;
    }
    else {
       	sprintf(buffer, "Invalid file format [%s-%d]!",__FILE__,__LINE__);
		goto ret_upload;
    }


    if(len > 0x700000){ //len check by sc_yang
      		sprintf(buffer, "Image len exceed max size 0x700000 ! len=0x%x</b><br>", len);
		goto ret_upload;
    }
    if ( (flag == 1) || (flag == 3)) {
    	if ( !fwChecksumOk(&upload_data[sizeof(IMG_HEADER_T)+head_offset], len)) 
    	{
      		sprintf(buffer, "Image checksum mismatched! len=0x%x, checksum=0x%x</b><br>", len,
			*((unsigned short *)&upload_data[len-2]) );
		goto ret_upload;
	}
    }
    else {
    	char *ptr = &upload_data[sizeof(IMG_HEADER_T)+head_offset];
    	if ( !CHECKSUM_OK(ptr, len) ) 
    	{
     		sprintf(buffer, "Image checksum mismatched! len=0x%x</b><br>", len);
		goto ret_upload;
	}
    }

#ifdef __mips__
    if(flag == 3)
    	fh = open(FLASH_DEVICE_NAME1, O_RDWR);
    else
       fh = open(FLASH_DEVICE_NAME, O_RDWR);

    if ( fh == -1 ) {
#else
    if (flag == 1)
    	bn = "apcode.bin";
    else if (flag == 3)
    	bn = "root.bin" ;
    else
    	bn = "web.gz.up";

    if ((fp = fopen((bn == NULL ? "upldForm.bin" : bn), "w+b")) == NULL) {
#endif
       	strcpy(buffer, "File open failed!");
	goto ret_upload;
    } else {


#ifdef __mips__
	if (flag == 1) {
		if ( startAddr == -1){
			//startAddr = CODE_IMAGE_OFFSET;
			startAddr = pHeader->burnAddr ;
			#ifdef _LITTLE_ENDIAN_
    				startAddr = DWORD_SWAP(startAddr);
    			#endif
		}

	}
	else if (flag == 3) {
		if ( startAddr == -1){
			startAddr = 0; // always start from offset 0 for 2nd FLASH partition
		}
	}
	else {
		if ( startAddrWeb == -1){
			//startAddr = WEB_PAGE_OFFSET;
			startAddr = pHeader->burnAddr ;
			#ifdef _LITTLE_ENDIAN_
    				startAddr = DWORD_SWAP(startAddr);
    			#endif
		}
		else
			startAddr = startAddrWeb;
	}
	lseek(fh, startAddr, SEEK_SET);
	if(flag == 3){
		locWrite += sizeof(IMG_HEADER_T); // remove header
		numLeft -=  sizeof(IMG_HEADER_T);
		
		kill_processes();
		sleep(2);
	}
	
	numWrite = write(fh, &(upload_data[locWrite+head_offset]), numLeft);
#else
	numWrite = fwrite(&(upload_data[locWrite+head_offset]), sizeof(*(upload_data)), numLeft, fp);
#endif
	if (numWrite < numLeft) {
#ifdef __mips__
		sprintf(buffer, "File write failed. locWrite=%d numLeft=%d numWrite=%d Size=%d bytes.", locWrite, numLeft, numWrite, upload_len);

#else
                sprintf(buffer, "File write failed. ferror=%d locWrite=%d numLeft=%d numWrite=%d Size=%d bytes.", ferror(fp), locWrite, numLeft, numWrite, upload_len);
#endif
	goto ret_upload;
	}
	locWrite += numWrite;
 	numLeft -= numWrite;
	sync();
#ifdef KERNEL_3_10
#define BLKFLSBUF  _IO(0x12,97) /* flush buffer cache */
    if(ioctl(fh,BLKFLSBUF,NULL) < 0){ 
        CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("flush mtd system cache error\n"));
    }   
#endif
#ifdef __mips__
	//if(flag != 3)
		close(fh);
#else
	fclose(fp);
#endif
	head_offset += len + sizeof(IMG_HEADER_T) ;
	startAddr = -1 ; //by sc_yang to reset the startAddr for next image
	update_fw = 1;
    }
} //while //sc_yang   
#ifndef NO_ACTION

//		alarm(2);
		//system("reboot");
		//for(;;);

#else
#ifdef VOIP_SUPPORT
	// rock: for x86 simulation
	if (update_cfg && !update_fw) {
		if (apmib_reinit()) {
			reset_user_profile();  // re-initialize user password
		}
		if(FW_Data)
			free(FW_Data);
	}
#endif
#endif
  return 1;
  ret_upload:	
  	fprintf(stderr, "%s\n", buffer);	
	return 0;
}

int cwmpSettingChange(int mibId)
{
	int cwmp_msgid;
	struct cwmp_message cwmpmsg = {0};

	if ((cwmp_msgid = msgget((key_t)MSG_KEY, 0)) >= 0)
	{
		cwmpmsg.msg_type = MSG_USERDATA_CHANGE;
		cwmpmsg.msg_datatype = mibId;
		CWMPDBG_FUNC(MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d] mibid:%d\n", __FUNCTION__, __LINE__, mibId));
		msgsnd(cwmp_msgid, (void *)&cwmpmsg, MSG_SIZE, 0);

		return 0;
	}
	return -1;
}

#define BACKUP_RAW_CSCONF

#if defined(BACKUP_RAW_CSCONF)
static unsigned char *compFile = NULL;
#endif

int mib_backup(int backup_target)
{
	int ret = 1;

	//fprintf(stderr,"\r\n __[%s-%u]",__FILE__,__LINE__);
	
#if defined(BACKUP_RAW_CSCONF)
	ret = flash_read_raw_mib(&compFile); /* only backup current setting, don't care backup_target*/
#endif

	return ret;
}

int mib_restore(int restore)
{
	int ret = 1;

	//fprintf(stderr,"\r\nrestore=[%d], __[%s-%u]",restore,__FILE__,__LINE__);
	
#if defined(BACKUP_RAW_CSCONF)
	if (restore) {
		ret = flash_write_raw_mib(&compFile);
		if (ret == 1) {
			if (1!= apmib_reinit())
				ret = -1;
}
	}
#endif
	return ret;
}

#if defined(MOD_FOR_TR098_LANDEVICE)
int getInFlags(char *interface, int *flags)
{
	int skfd, ret = 0;
	struct ifreq ifr;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(ifr.ifr_name, interface);
	if ((ret = ioctl(skfd, SIOCGIFFLAGS, &ifr)) < 0)
		ret = 0;
	else {
 		*flags = ifr.ifr_flags;
		ret = 1;
    }

	close(skfd);
	return ret;	
}
#endif

#define	RTL8651_IOCTL_GETLANLINKUP			2300
#define 	RTL8651_IOCTL_GETWANLINKUP			2301
#define   RTL8651_IOCTL_GETWANSPEED 			2306


int getLanLinkUpFlag(char *interface, int *flags)
{
	int flags_tmp;
	int ret = -1;
	ret = re865xIoctl(interface, RTL8651_IOCTL_GETLANLINKUP, 0, 0, (unsigned int)&flags_tmp);
	*flags = flags_tmp;
	return ret;
}

int getWanLinkUpFlag(char *interface, int *flags)
{
	int flags_tmp;
	int ret = -1;
	ret = re865xIoctl(interface, RTL8651_IOCTL_GETWANLINKUP, 0, 0, (unsigned int)&flags_tmp);
	*flags = flags_tmp;
	return ret;
}

int getWanSpeed(char *interface, unsigned int *speed)
{
	unsigned int speed_tmp;
	int ret = -1;
	ret = re865xIoctl(interface, RTL8651_IOCTL_GETWANSPEED, 0, 0, (unsigned int)&speed_tmp);
	*speed = speed_tmp;
	return ret;
}


int getWanStats(char *interface, unsigned int type, unsigned long *stats)
{
	unsigned long stats_tmp = 0;
	int ret = -1;
	ret = re865xIoctl(interface, type, 0, 0, (unsigned int)&stats_tmp);
	*stats = stats_tmp;
	return ret;
}

unsigned char *gettoken(const unsigned char *str,unsigned int index,unsigned char symbol)
{
	static char tmp[50];
	unsigned char tk[50]; //save symbol index
	char *ptmp;
	int i,j,cnt=1,start,end;
	//scan symbol and save index
	
	memset(tmp, 0x00, sizeof(tmp));
	
	for (i=0;i<strlen((char *)str);i++)
	{          
		if (str[i]==symbol)
		{
			tk[cnt]=i;
			cnt++;
		}
	}
	
	if (index>cnt-1)
	{
		return NULL;
	}
			
	tk[0]=0;
	tk[cnt]=strlen((char *)str);
	
	if (index==0)
		start=0;
	else
		start=tk[index]+1;

	end=tk[index+1];
	
	j=0;
	for(i=start;i<end;i++)
	{
		tmp[j]=str[i];
		j++;
	}
		
	return (unsigned char *)tmp;
}

unsigned int getWLAN_ChipVersion()
{
	FILE *stream;
	char buffer[128];
	typedef enum { CHIP_UNKNOWN=0, CHIP_RTL8188C=1, CHIP_RTL8192C=2, CHIP_RTL8192D=3} CHIP_VERSION_T;
	CHIP_VERSION_T chipVersion = CHIP_UNKNOWN;	

	sprintf(buffer,"/proc/wlan%d/mib_rf",wlan_idx);
	stream = fopen (buffer, "r" );
	if ( stream != NULL )
	{		
		char *strtmp;
		char line[100];
								 
		while (fgets(line, sizeof(line), stream))
		{
			
			strtmp = line;
			while(*strtmp == ' ')
			{
				strtmp++;
			}
			

			if(strstr(strtmp,"RTL8192SE") != 0)
			{
				chipVersion = CHIP_UNKNOWN;
			}
			else if(strstr(strtmp,"RTL8188C") != 0)
			{
				chipVersion = CHIP_RTL8188C;
			}
			else if(strstr(strtmp,"RTL8192C") != 0)
			{
				chipVersion = CHIP_RTL8192C;
			}
			else if(strstr(strtmp,"RTL8192D") !=0)
			{
				chipVersion = CHIP_RTL8192D;
			}
		}			
		fclose ( stream );
	}

	return chipVersion;


}

int isFileExist(char *file_name)
{
	struct stat status;

	if ( stat(file_name, &status) < 0)
		return 0;

	return 1;
}

int SetWlan_idx(char * wlan_iface_name)
{
	int idx;
	
		idx = atoi(&wlan_iface_name[4]);
		if (idx >= NUM_WLAN_INTERFACE) {
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("invalid wlan interface index number %d!\n", idx));
				return 0;
		}
		wlan_idx = idx;
		vwlan_idx = 0;
	
#ifdef MBSSID		
		
		if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
				wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
				idx = atoi(&wlan_iface_name[8]);
				if (idx >= NUM_VWLAN_INTERFACE) {
					CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("invalid virtual wlan interface index number %d!\n", idx));
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

//printf("\r\n wlan_iface_name=[%s],wlan_idx=[%u],vwlan_idx=[%u],__[%s-%u]\r\n",wlan_iface_name,wlan_idx,vwlan_idx,__FILE__,__LINE__);

return 1;		
}

short whichWlanIfIs(PHYBAND_TYPE_T phyBand)
{
	int i;
	int ori_wlan_idx=wlan_idx;
	int ret=-1;
	
	for(i=0 ; i<NUM_WLAN_INTERFACE ; i++)
	{
		unsigned char wlanif[10];
		memset(wlanif,0x00,sizeof(wlanif));
		sprintf((char *)wlanif, "wlan%d",i);
		if(SetWlan_idx((char *)wlanif))
		{
			int phyBandSelect;
			apmib_get(MIB_WLAN_PHY_BAND_SELECT, (void *)&phyBandSelect);
			if(phyBandSelect == phyBand)
			{
				ret = i;
				break;			
			}
		}						
	}
	
	wlan_idx=ori_wlan_idx;
	return ret;		
}
int getOUTfromMAC(char *ouiname)
{
	char oui[10];
	int i;
	unsigned char tmpBuff[32]={0};
	
	mib_get(MIB_ELAN_MAC_ADDR,  (void *)tmpBuff);
	
	if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6)){
		mib_get(MIB_HW_NIC0_ADDR,  (void *)tmpBuff);
		if(!memcmp(tmpBuff, "\x00\x00\x00\x00\x00\x00", 6))
			sprintf(oui,"%s", "00E04C");
		else{
			sprintf(oui,"%02x%02x%02x",tmpBuff[0],tmpBuff[1],tmpBuff[2]);
		}	
	}else{
			sprintf(oui,"%02x%02x%02x",tmpBuff[0],tmpBuff[1],tmpBuff[2]);
	}	
	for(i=0;i<strlen(oui);i++)
	{
		ouiname[i]=toupper(oui[i]);
	}
	ouiname[i]=0;
	
}

#if defined(CONFIG_REPEATER_WPS_SUPPORT) || defined(POWER_CONSUMPTION_SUPPORT)
WLAN_STATE_T lastWanState = WLAN_OFF;
WLAN_STATE_T updateWlanifState(char *wlanif_name)
{
	FILE *stream;
	int debug_check = 0;
	WLAN_STATE_T wlanState = WLAN_NO_LINK;
	unsigned char tmpStr[100];
	
	memset(tmpStr,0x00,sizeof(tmpStr));
	sprintf((char *)tmpStr,"/proc/%s/sta_info",wlanif_name);

	stream = fopen ((char *)tmpStr, "r" );
	if ( stream != NULL )
	{		
		char *strtmp;
		char line[100];
		while (fgets(line, sizeof(line), stream))
		{
			unsigned char *p;
			strtmp = line;
			
			while(*strtmp == ' ')
				strtmp++;
			
			if(strstr(strtmp,"active") != 0)
			{
				unsigned char str1[10], str2[10];
				
				//-- STA info table -- (active: 1)
				if(debug_check)
					fprintf(stderr,"\r\n [%s]",strtmp);
				
				sscanf(strtmp, "%*[^:]:%[^)]",str1);
				
				p = str1;
				while(*p == ' ')
					p++;										
				
				if(strcmp((char *)p,"0") == 0)
				{
					wlanState = WLAN_NO_LINK;
				}
				else
				{
					wlanState = WLAN_LINK;						
				}										
				
				break;
			}
				
		}
		fclose(stream );
		
	}
	
	if(wlanState == WLAN_LINK)
	{
		bss_info bss;

		getWlBssInfo(wlanif_name, &bss);

		if(bss.state != STATE_CONNECTED && bss.state != STATE_STARTED)
			wlanState = WLAN_NO_LINK;
		
	}
	
	memset(tmpStr,0x00,sizeof(tmpStr));
	if(lastWanState != wlanState)
	{
		lastWanState = wlanState;
		if(wlanState == WLAN_LINK)
			sprintf((char *)tmpStr,"echo \"%s LINK\" > /var/wlan_state",wlanif_name);
		else
			sprintf((char *)tmpStr,"echo \"%s NO_LINK\" > /var/wlan_state",wlanif_name);
		
		system((char *)tmpStr);
	}
	
	return wlanState;
		
}
#endif //#if defined(CONFIG_REPEATER_WPS_SUPPORT) || defined(POWER_CONSUMPTION_SUPPORT)

#if defined(CONFIG_RTL_P2P_SUPPORT)

int getWlP2PScanResult(char *interface, SS_STATUS_Tp pStatus )
{
#ifndef NO_ACTION
  int skfd=0;
  struct iwreq wrq;

  skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
  if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
    /* If no wireless name : no wireless extensions */
    close( skfd );
      return -1;
	}
   
  wrq.u.data.pointer = (caddr_t)pStatus;

  if ( pStatus->number == 0 )
  	wrq.u.data.length = sizeof(SS_STATUS_T);
  else
    wrq.u.data.length = sizeof(pStatus->number);

  if (iw_get_ext(skfd, interface, SIOCP2PGETRESULT, &wrq) < 0){
  	close( skfd );
		return -1;
	}
    close( skfd );
#else
	return -1 ;
#endif

    return 0;
}

int getWlP2PScanRequest(char *interface, int *pStatus)
{
#ifndef NO_ACTION
  int skfd=0;
  struct iwreq wrq;
  unsigned char result;

//printf("\r\n __[%s-%u]\r\n",__FILE__,__LINE__);

  skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;

//printf("\r\n __[%s-%u]\r\n",__FILE__,__LINE__);

    /* Get wireless name */
  if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
    /* If no wireless name : no wireless extensions */
    close( skfd );
    return -1;
	}
  wrq.u.data.pointer = (caddr_t)&result;
  wrq.u.data.length = sizeof(result);

//printf("\r\n __[%s-%u]\r\n",__FILE__,__LINE__);

  if (iw_get_ext(skfd, interface, SIOCP2PSCANREQ, &wrq) < 0)
  {
  	close( skfd );
		return -1;
	}
  close( skfd );


  if ( result == 0xff )
  	*pStatus = -1;
  else
		*pStatus = (int) result;
#else
	*pStatus = -1;
#endif // #ifndef NO_ACTION

	return 0;

}

int getWlP2PStateEvent( char *interface, P2P_SS_STATUS_Tp pP2PStatus)
{
#ifndef NO_ACTION
  int skfd=0;
  struct iwreq wrq;

  skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
  if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
    /* If no wireless name : no wireless extensions */
    close( skfd );
    return -1;
	}
  wrq.u.data.pointer = (caddr_t)pP2PStatus;
  wrq.u.data.length = sizeof(P2P_SS_STATUS_T);

  if (iw_get_ext(skfd, interface, SIOCP2PPGETEVNIND, &wrq) < 0){
  	 close( skfd );
  	 
		return -1;
	}     


	close( skfd );
#else
	*num = 0 ;
#endif

  return 0;
}


int getClientConnectState(void)
{

	static struct __p2p_state_event P2PStatus_t;
	memset(&P2PStatus_t , 0 ,sizeof(struct __p2p_state_event));

	int skfd=0;
	struct iwreq wrq;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return 0;
	
	/* Get wireless name */
	if ( iw_get_ext(skfd, "wlan0", SIOCGIWNAME, &wrq) < 0){
	    /* If no wireless name : no wireless extensions */
	    close( skfd );
	    return 0;
	}

	wrq.u.data.pointer = (caddr_t)&P2PStatus_t;
	wrq.u.data.length = sizeof(struct __p2p_state_event);

	if (iw_get_ext(skfd, "wlan0", SIOCP2P_REPORT_CLIENT_STATE, &wrq) < 0){
  		close( skfd );  	 
		return 0;
	}     
	close( skfd );

	return P2PStatus_t.p2p_status;
	
}


int sendP2PProvisionCommInfo( char *interface, P2P_PROVISION_COMM_Tp pP2PProvisionComm)
{
#ifndef NO_ACTION
  int skfd=0;
  struct iwreq wrq;

  skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
  if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
    /* If no wireless name : no wireless extensions */
    close( skfd );
    return -1;
	}
  wrq.u.data.pointer = (caddr_t)pP2PProvisionComm;
  wrq.u.data.length = sizeof(P2P_PROVISION_COMM_T);

  if (iw_get_ext(skfd, interface, SIOCP2PPROVREQ, &wrq) < 0){
  	 close( skfd );
  	 
		return -1;
	}     


	close( skfd );
#else
	*num = 0 ;
#endif

  return 0;
}


int sendP2PWscConfirm( char *interface, P2P_WSC_CONFIRM_Tp pP2PWscConfirm)
{
#ifndef NO_ACTION
  int skfd=0;
  struct iwreq wrq;

  skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
    /* Get wireless name */
  if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0){
    /* If no wireless name : no wireless extensions */
    close( skfd );
    return -1;
	}
  wrq.u.data.pointer = (caddr_t)pP2PWscConfirm;
  wrq.u.data.length = sizeof(P2P_WSC_CONFIRM_T);

  if (iw_get_ext(skfd, interface, SIOCP2WSCMETHODCONF, &wrq) < 0){
  	 close( skfd );
  	 
		return -1;
	}     


	close( skfd );
#else
	*num = 0 ;
#endif

  return 0;
}
#endif

#ifdef CONFIG_APP_TR069
const char IPTABLES[] = "/bin/iptables";
const char ARG_T[] = "-t";
const char FW_DEL[] = "-D";
const char FW_PREROUTING[] = "PREROUTING";
const char ARG_I[] = "-i";
const char LANIF[] = "br0";
const char ARG_TCP[] = "TCP";
const char ARG_UDP[] = "UDP";
const char FW_DPORT[] = "--dport";
const char RMACC_MARK[] = "0x1000";
const char FW_ADD[] = "-A";

#ifdef _PRMT_TR143_
const char gUDPEchoServerName[]="/bin/udpechoserver";
const char gUDPEchoServerPid[] = "/var/run/udpechoserver.pid";

int read_pid(char *filename)
{
	int fh;
	FILE *in;
	int pid;

	fh = open(filename, O_RDWR);
	if ( fh == -1 ) return -1;
	if ((in = fdopen(fh, "r")) == NULL) return -1;
	fscanf(in, "%d", &pid);
	fclose(in);
	close(fh);

	return pid;
}

void UDPEchoConfigSave(struct TR143_UDPEchoConfig *p)
{
	if(p)
	{
		unsigned int vChar;
		unsigned char intfName[8] = {0};
		mib_get( MIB_TR143_UDPECHO_ENABLE, (void *)&vChar );
		p->Enable = vChar;
		mib_get( MIB_TR143_UDPECHO_SRCIP, (void *)p->SourceIPAddress );
		mib_get( MIB_TR143_UDPECHO_PORT, (void *)&vChar );
		p->UDPPort = vChar;
		mib_get( MIB_TR143_UDPECHO_PLUS, (void *)&vChar );
		p->EchoPlusEnabled = vChar;

		mib_get( MIB_TR143_UDPECHO_ITFTYPE, (void *)intfName );
		
		strcpy( p->Interface, intfName );
		LANDEVNAME2BR0(p->Interface);
	}
	return;
}

int UDPEchoConfigStart( struct TR143_UDPEchoConfig *p )
{
    char cmd[64]={0};
#if defined(MULTI_WAN_SUPPORT)
    extern char tr069WanIfName[];
#endif
	if(!p) return -1;

	if( p->Enable )
	{
		char strPort[16], strAddr[32];
		char *argv[10];
		int  i;

		if(p->UDPPort==0)
		{
			CWMPDBG_FUNC(MODULE_TR_143, LEVEL_ERROR, ("UDPEchoConfigStart> error p->UDPPort=0\n" ));
			return -1;
		}
		sprintf( strPort, "%u", p->UDPPort );
		va_cmd(IPTABLES, 15, 1, ARG_T, "mangle", FW_ADD, (char *)FW_PREROUTING,
			(char *)ARG_I, "!", (char *)LANIF, "-p", (char *)ARG_UDP,
			(char *)FW_DPORT, strPort, "-j", (char *)"MARK", "--set-mark", RMACC_MARK);

		i=0;
		argv[i]=(char *)gUDPEchoServerName;
		i++;
		argv[i]="-port";
		i++;
		argv[i]=strPort;
		i++;
		if( strlen(p->Interface) > 0 )
		{
			argv[i]="-i";
			i++;
			argv[i]=p->Interface;
			i++;
		}
		if( p->SourceIPAddress[0]!=0 ||
			p->SourceIPAddress[1]!=0 ||
			p->SourceIPAddress[2]!=0 ||
			p->SourceIPAddress[3]!=0  )
		{
			struct in_addr *pSIP = (struct in_addr *)p->SourceIPAddress;
			argv[i]="-addr";
			i++;
			sprintf( strAddr, "%s", inet_ntoa( *pSIP ) );
			argv[i]=strAddr;
			i++;
		}
		if( p->EchoPlusEnabled )
		{
			argv[i]="-plus";
			i++;
		}

		argv[i]=NULL;
#if defined(MULTI_WAN_SUPPORT)
        sprintf(cmd, "iptables -A INPUT -i %s -p udp --dport %d -j ACCEPT", tr069WanIfName, p->UDPPort);
#else
        sprintf(cmd, "iptables -A INPUT -i eth1 -p udp --dport %d -j ACCEPT", p->UDPPort);
#endif
        CWMPDBG_FUNC(MODULE_TR_143, LEVEL_INFO, ("[%s:%d] iptables command: %s\n", __FUNCTION__, __LINE__, cmd));
        system(cmd);
		do_cmd( gUDPEchoServerName, argv, 0 );

	}else{
        CWMPDBG_FUNC(MODULE_TR_143, LEVEL_INFO, ("[%s:%d] UDPEcho is not enabled\n", __FUNCTION__, __LINE__));
	}

	return 0;
}

int UDPEchoConfigStop( struct TR143_UDPEchoConfig *p )
{
	char strPort[16];
	int pid;
	int status;
    char cmd[64] = {0};

	if(!p) return -1;

	sprintf( strPort, "%u", p->UDPPort );
	va_cmd(IPTABLES, 15, 1, ARG_T, "mangle", FW_DEL, (char *)FW_PREROUTING,
		(char *)ARG_I, "!", (char *)LANIF, "-p", (char *)ARG_UDP,
		(char *)FW_DPORT, strPort, "-j", (char *)"MARK", "--set-mark", RMACC_MARK);


	pid = read_pid((char *)gUDPEchoServerPid);
	if (pid >= 1)
	{
        sprintf(cmd, "iptables -D INPUT -i eth1 -p udp --dport %d -j ACCEPT", p->UDPPort);
        CWMPDBG_FUNC(MODULE_TR_143, LEVEL_INFO, ("[%s:%d] cmd: %s\n", __FUNCTION__, __LINE__, cmd));
        system(cmd);
		status = kill(pid, SIGTERM);
		if (status != 0)
		{
			CWMPDBG_FUNC(MODULE_TR_143, LEVEL_ERROR, ("Could not kill UDPEchoServer's pid '%d'\n", pid));
			return -1;
		}
	}

	return 0;
}

int apply_UDPEchoConfig( int action_type, int id, void *olddata )
{
	struct TR143_UDPEchoConfig newdata;
	int ret=0;

	UDPEchoConfigSave(&newdata);
	switch( action_type )
	{
	case CWMP_RESTART:
		if(olddata) UDPEchoConfigStop(olddata);
	case CWMP_START:
		UDPEchoConfigStart(&newdata);
		break;
	case CWMP_STOP:
		if(olddata) UDPEchoConfigStop(olddata);
		break;
	default:
		ret=-1;
	}

	return ret;
}
int alias_name_are_eq(char *orig_name,char *alias1,char *alias2)
{


int rtnValue=1;
rtnValue=strncmp(orig_name,alias1,strlen(alias1));

if(rtnValue)
    rtnValue&=strncmp(orig_name,alias2,strlen(alias2));

#if 0//def CONFIG_RTL_ALIASNAME_DEBUG
    if(rtnValue!=strncmp(orig_name,cmp_name,strlen(cmp_name)))
    {
        printk("====CONFIG_RTL_ALIASNAME_DEBUG====\n\ncmp_name : %s\norig_name :%s\n",cmp_name,orig_name);
                printk("rtnValue %d\n",rtnValue);
    }
#endif
    return !rtnValue;

}
int IfName2ItfId( char *s )
{
	int i;
	if( !s || s[0]==0 ) return ITF_ALL;

//	if( (strncmp(s, "ppp", 3)==0) || (strncmp(s, "vc", 2)==0) )
    if(alias_name_are_eq(s,ALIASNAME_PPP,ALIASNAME_VC))
		return ITF_WAN;

	for( i=0;i<ITF_END;i++ )
	{
		if( strcmp( strItf[i],s )==0 ) return i;
	}

	return -1;
}
#endif //_PRMT_TR143_

static int procnetdev_version(char *buf)
{
	if (strstr(buf, "compressed"))
		return 2;
	if (strstr(buf, "bytes"))
		return 1;
	return 0;
}

/**
 * list_net_device_with_flags - list network devices with the specified flags
 * @flags: input argument, the network device flags
 * @nr_names: input argument, number of elements in @names
 * @names: output argument, constant pointer to the array of network device names
 *
 * Returns the number of resulted elements in @names for success
 * or negative errno values for failure.
 */
int list_net_device_with_flags(short flags, int nr_names,
				char (* const names)[IFNAMSIZ])
{
	FILE *fh;
	char buf[512];
	struct ifreq ifr;
	int nr_result, skfd;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0)
		goto out;

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh)
		goto out_close_skfd;
	fgets(buf, sizeof(buf), fh);	/* eat line */
	fgets(buf, sizeof(buf), fh);

	nr_result = 0;
	while (fgets(buf, sizeof(buf), fh) && nr_result < nr_names) {
		char name[128];

		get_name(name, buf);

		strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));
		if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
			goto out_close_fh;

		if (ifr.ifr_flags & flags) {
			strncpy(names[nr_result++], name, ARRAY_SIZE(names[0]));
		}
	}

	if (ferror(fh))
		goto out_close_fh;

	fclose(fh);
	close(skfd);

	return nr_result;

out_close_fh:
	fclose(fh);
out_close_skfd:
	close(skfd);
out:
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("%s():%d", __FUNCTION__, __LINE__));

	return -errno;
}
// Kaohj
/*
 *	Get the link status about device.
 *	Return:
 *		-1 on error
 *		0 on link down
 *		1 on link up
 */
int get_net_link_status(const char *ifname)
{
	
	
	int ret=0;
#if 0	
	struct ethtool_value edata;
	struct ifreq ifr;
#ifdef CONFIG_DEV_xDSL
	Modem_LinkSpeed vLs;
#endif

#ifdef CONFIG_DEV_xDSL
	if (!strcmp(ifname, ALIASNAME_DSL0)) {
       		if ( adsl_drv_get(RLCM_GET_LINK_SPEED, (void *)&vLs,
			RLCM_GET_LINK_SPEED_SIZE) && vLs.upstreamRate != 0)
			ret = 1;
		else
			ret = 0;
		return ret;
	}
#endif
	strcpy(ifr.ifr_name, ifname);
	edata.cmd = ETHTOOL_GLINK;
	ifr.ifr_data = (caddr_t)&edata;

	ret = do_ioctl(SIOCETHTOOL, &ifr);
	if (ret == 0)
		ret = edata.data;
#endif		
	return ret;
}
int do_ioctl(unsigned int cmd, struct ifreq *ifr)
{
	int skfd, ret;

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return (-1);
	}

	ret = ioctl(skfd, cmd, ifr);
	close(skfd);
	return ret;
}
/*
 *	Get the link information about device.
 *	Return:
 *		-1 on error
 *		0 on success
 */
int get_net_link_info(const char *ifname, struct net_link_info *info)
{
	struct ifreq ifr;
	struct ethtool_cmd ecmd;
	int ret=-1;

	memset(info, 0, sizeof(struct net_link_info));
	strcpy(ifr.ifr_name, ifname);
	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_data = (caddr_t)&ecmd;

	ret = do_ioctl(SIOCETHTOOL, &ifr);
	if (ret == 0) {
		info->supported = ecmd.supported; // ports, link modes, auto-negotiation
		info->advertising = ecmd.advertising; // link modes, pause frame use, auto-negotiation
		info->speed = ecmd.speed; // 10Mb, 100Mb, gigabit
		info->duplex = ecmd.duplex; // Half, Full, Unknown
		info->phy_address = ecmd.phy_address;
		info->transceiver = ecmd.transceiver;
		info->autoneg = ecmd.autoneg;
	}
	return ret;
}

/*
* Convert ifIndex to system interface name, e.g. eth0,vc0...
*/
char *ifGetName(int ifindex, char *buffer, unsigned int len)
{
	MEDIA_TYPE_T mType;

	if ( ifindex == DUMMY_IFINDEX )
		return 0;
	if (PPP_INDEX(ifindex) == DUMMY_PPP_INDEX)
	{
		mType = MEDIA_INDEX(ifindex);
		if (mType == MEDIA_ATM)
			snprintf( buffer, len,  "%s%u",ALIASNAME_VC, VC_INDEX(ifindex) );
		else if (mType == MEDIA_ETH)
#ifdef CONFIG_RTL_MULTI_ETH_WAN
			snprintf( buffer, len, "%s%d",ALIASNAME_MWNAS, ETH_INDEX(ifindex));
#else
			snprintf( buffer, len,  "%s%u",ALIASNAME_ETH, ETH_INDEX(ifindex) );
#endif
		else
			return 0;
	}
	else if (ETH_INDEX(ifindex)){
		#ifdef CONFIG_RTL_MULTI_ETH_WAN
		snprintf( buffer, len, "%s%d",ALIASNAME_MWNAS, ETH_INDEX(ifindex));
		#else
		snprintf( buffer, len,	"%s%u",ALIASNAME_ETH, ETH_INDEX(ifindex) );
		#endif
	}
	else{
		snprintf( buffer, len,  "%s%u",ALIASNAME_PPP, PPP_INDEX(ifindex) );
	}
	return buffer;
}

void base64encode(unsigned char *from, char *to, int len)
{
	int total_len = len;
	while (len) {
		unsigned char in[3];
		int i, num4Base64;
		memset(in, 0x00, 3);
		num4Base64 = (len < 3) ? len : 3;	
		
		for(i = 0; i < num4Base64; i++)
			in[i] = from[total_len-len+i];

		*to++ = (unsigned char) table64[ (int)(in[0] >> 2) ];
    	*to++ = (unsigned char) table64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    	*to++ = (unsigned char) (num4Base64 > 1 ? table64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
   		*to++ = (unsigned char) (num4Base64 > 2 ? table64[ (int)(in[2] & 0x3f) ] : '=');
		
		len -= num4Base64;
	}
}

static char GetBase64Value(char ch){
    if ((ch >= 'A') && (ch <= 'Z'))  // A ~ Z
    return ch - 'A';
    if ((ch >= 'a') && (ch <= 'z'))  // a ~ z
    return ch - 'a' + 26;
    if ((ch >= '0') && (ch <= '9'))  // 0 ~ 9
    return ch - '0' + 52;
    switch (ch)
    {
    case '+':
        return 62;
    case '/':
        return 63;
    case '=': //padding char
        return 0;
    default:
        return 0;
    }
}

int base64decode(unsigned char *to, unsigned char *from, int fromLen, int force)
{
    if( fromLen%4 && !force ) // for !force mode and fromLen%4 --> wrong length
    {
        to[0] = '\0';
        return -1;
    }
    unsigned char Base64Encode[4];
    int toLen=0;

    while( fromLen > 2) // ignore when length is less than 3 bytes
    {
        Base64Encode[0] = GetBase64Value(from[0]);
        Base64Encode[1] = GetBase64Value(from[1]);
        Base64Encode[2] = GetBase64Value(from[2]);
        Base64Encode[3] = GetBase64Value(from[3]);

        *to ++ = (Base64Encode[0] << 2) | (Base64Encode[1] >> 4);
        *to ++ = (Base64Encode[1] << 4) | (Base64Encode[2] >> 2);
        *to ++ = (Base64Encode[2] << 6) | (Base64Encode[3]);

        from += 4;
        fromLen -= 4;
        toLen += 3;
    }

    return toLen;
}

int getMIB2Str(unsigned int id, char *strbuf)
{
	unsigned char buffer[64];

	if (!strbuf)
		return -1;

	switch (id) {
		// INET address
		case MIB_IP_ADDR:
		case MIB_SUBNET_MASK:
		case MIB_DMZ_HOST:
		case MIB_DHCP_CLIENT_START:
		case MIB_DHCP_CLIENT_END:
		case MIB_DNS1:
		case MIB_DNS2:
		case MIB_DNS3:
#if defined(_PRMT_X_TELEFONICA_ES_DHCPOPTION_)
		case MIB_LAN_DNS1:
       	case MIB_LAN_DNS2:
       	case MIB_LAN_DNS3:
#endif
		case MIB_DEFAULT_GATEWAY:

#ifdef _PRMT_TR143_
		case MIB_TR143_UDPECHO_SRCIP:
#endif //_PRMT_TR143_

			if(!mib_get( id, (void *)buffer))
				return -1;
			// Mason Yu
			if ( ((struct in_addr *)buffer)->s_addr == INADDR_NONE ) {
				sprintf(strbuf, "%s", "");
			} else {
				sprintf(strbuf, "%s", inet_ntoa(*((struct in_addr *)buffer)));
			}
			break;
		// Ethernet address
		case MIB_ELAN_MAC_ADDR:
		case MIB_WLAN_WLAN_MAC_ADDR:
			if(!mib_get( id,  (void *)buffer))
				return -1;
			
			sprintf(strbuf, "%02x%02x%02x%02x%02x%02x", buffer[0], buffer[1],
				buffer[2], buffer[3], buffer[4], buffer[5]);
			
			break;
		case MIB_WLAN_CHANNEL:
		case MIB_WLAN_WLAN_DISABLED:
		case MIB_WLAN_ENABLE_1X:
		case MIB_WLAN_ENCRYPT:
		case MIB_WLAN_WPA_AUTH:
		case MIB_WLAN_NETWORK_TYPE:
			if(!mib_get( id,  (void *)buffer))
				return -1;
	   		sprintf(strbuf, "%u", *(unsigned char *)buffer);
	   		break;
#ifdef WLAN_SUPPORT
	   	case MIB_WLAN_FRAG_THRESHOLD:
	   	case MIB_WLAN_RTS_THRESHOLD:
	   	case MIB_WLAN_BEACON_INTERVAL:
#endif
			if(!mib_get( id,  (void *)buffer))
				return -1;
			sprintf(strbuf, "%u", *(unsigned short *)buffer);
			break;

		case MIB_DHCP_LEASE_TIME:
			if(!mib_get( id,  (void *)buffer))
				return -1;
			// if MIB_ADSL_LAN_DHCP_LEASE=0xffffffff, it indicate an infinate lease
			if ( *(unsigned long *)buffer == 0xffffffff )
				sprintf(strbuf, "-1");
			else
				sprintf(strbuf, "%u", *(unsigned int *)buffer);
			break;

		case MIB_CWMP_CONREQ_PORT:
		case MIB_CWMP_INFORM_INTERVAL:

			if(!mib_get( id,  (void *)buffer))
				return -1;
			sprintf(strbuf, "%u", *(unsigned int *)buffer);
			break;
#ifdef WLAN_SUPPORT
		case MIB_WLAN_SSID:
#endif
		case MIB_CWMP_PROVISIONINGCODE:
		case MIB_CWMP_ACS_URL:
		case MIB_CWMP_ACS_USERNAME:
		case MIB_CWMP_ACS_PASSWORD:
		case MIB_CWMP_CONREQ_USERNAME:
		case MIB_CWMP_CONREQ_PASSWORD:
		case MIB_CWMP_CONREQ_PATH:
		case MIB_CWMP_LAN_CONFIGPASSWD:
		case MIB_CWMP_SERIALNUMBER:
		case MIB_CWMP_DL_COMMANDKEY:
		case MIB_CWMP_RB_COMMANDKEY:
		case MIB_CWMP_ACS_PARAMETERKEY:
		case MIB_CWMP_CERT_PASSWORD:
#ifdef _PRMT_USERINTERFACE_
		case MIB_UIF_AUTOUPDATESERVER:
		case MIB_UIF_USERUPDATESERVER:
#endif
		case MIB_CWMP_SI_COMMANDKEY:
		case MIB_CWMP_ACS_KICKURL:
		case MIB_CWMP_ACS_DOWNLOADURL:
#ifdef SUPER_NAME_SUPPORT
		case MIB_SUPER_NAME:
#endif
		case MIB_USER_NAME:
			if(!mib_get( id,  (void *)strbuf)){
				return -1;
			}
			break;


		default:
			return -1;
	}

	return 0;
}

int getSYS2Str(SYSID_T id, char *strbuf)
{
	unsigned char buffer[128], vChar;
	struct sysinfo info;
	int updays, uphours, upminutes, len, i;
	time_t tm;
	struct tm tm_time, *ptm_time;
	FILE *fp;
	unsigned char tmpBuf[64], *pStr;
	unsigned short vUShort;
	unsigned int vUInt;

	if (!strbuf)
		return -1;

	strbuf[0] = '\0';

	switch (id) {
		case SYS_UPTIME:
			sysinfo(&info);
			updays = (int) info.uptime / (60*60*24);
			if (updays)
				sprintf(strbuf, "%d day%s, ", updays, (updays != 1) ? "s" : "");
			len = strlen(strbuf);
			upminutes = (int) info.uptime / 60;
			uphours = (upminutes / 60) % 24;
			upminutes %= 60;
			if(uphours)
				sprintf(&strbuf[len], "%2d:%02d", uphours, upminutes);
			else
				sprintf(&strbuf[len], "%d min", upminutes);
			break;
		case SYS_DATE:
	 		time(&tm);
			memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
			strftime(strbuf, 200, "%a %b %e %H:%M:%S %Z %Y", &tm_time);
			break;
		case SYS_YEAR:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_year+ 1900));
			break;
		case SYS_MONTH:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_mon+ 1));
			break;
		case SYS_DAY:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_mday));
			break;
		case SYS_HOUR:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_hour));
			break;
		case SYS_MINUTE:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_min));
			break;
		case SYS_SECOND:
	 		time(&tm);
			ptm_time = localtime(&tm);
			snprintf(strbuf, 64, "%d", (ptm_time->tm_sec));
			break;
		case SYS_LAN_DHCP:
			if ( !mib_get( MIB_DHCP, (void *)&vUInt) )
				return -1;
			if (DHCP_SERVER == vUInt)
				strcpy(strbuf, STR_ENABLE);
			else
				strcpy(strbuf, STR_DISABLE);
			break;
		case SYS_DHCP_LAN_IP:
			getMIB2Str(MIB_IP_ADDR, strbuf);
			break;
		case SYS_DHCP_LAN_SUBNET:
			getMIB2Str(MIB_SUBNET_MASK, strbuf);
			break;
#ifdef WLAN_SUPPORT
		case SYS_WLAN:
			if ( !mib_get( MIB_WLAN_WLAN_DISABLED, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_ENABLE);
			else
				strcpy(strbuf, STR_DISABLE);
			break;
		case SYS_WLAN_BCASTSSID:
			if ( !mib_get( MIB_WLAN_HIDDEN_SSID, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_ENABLE);
			else
				strcpy(strbuf, STR_DISABLE);
			break;
		case SYS_WLAN_BAND:
			if ( !mib_get( MIB_WLAN_BAND, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_band[vUInt]);
			break;
		case SYS_WLAN_AUTH:
			if ( !mib_get( MIB_WLAN_AUTH_TYPE, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_auth[vUInt]);
			break;
		case SYS_WLAN_PREAMBLE:
			if ( !mib_get( MIB_WLAN_PREAMBLE_TYPE, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_preamble[vUInt]);
			break;
		case SYS_WLAN_ENCRYPT:
			if ( !mib_get( MIB_WLAN_ENCRYPT, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_encrypt[vUInt]);
			break;
		case SYS_WLAN_WPA_CIPHER:
			if ( !mib_get( MIB_WLAN_WPA_CIPHER_SUITE,  (void *)&vUInt) )
				return -1;
			if ( vUInt == 0 )
				strcpy(strbuf, "");
			else
				strcpy(strbuf, wlan_Cipher[vUInt-1]);
			break;
		case SYS_WLAN_WPA2_CIPHER:
			if ( !mib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&vUInt) )
				return -1;
			if ( vUInt == 0 )
				strcpy(strbuf, "");
			else
				strcpy(strbuf, wlan_Cipher[vUInt-1]);
			break;
		case SYS_WLAN_PSKFMT:
			if ( !mib_get( MIB_WLAN_PSK_FORMAT, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_pskfmt[vUInt]);
			break;
		case SYS_WLAN_PSKVAL:
			if ( !mib_get( MIB_WLAN_WPA_PSK, (void *)buffer) )
				return -1;
			for (len=0; len<strlen(buffer); len++)
				strbuf[len]='*';
			strbuf[len]='\0';
			break;
		case SYS_WLAN_WEP_KEYLEN:
			if ( !mib_get( MIB_WLAN_WEP, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_wepkeylen[vUInt]);
			break;
		case SYS_WLAN_WEP_KEYFMT:
			if ( !mib_get( MIB_WLAN_WEP_KEY_TYPE, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_wepkeyfmt[vUInt]);
			break;
		case SYS_WLAN_WPA_MODE:
			if (!mib_get(MIB_WLAN_WPA_AUTH, (void *)&vUInt))
				return -1;
			if (vUInt == WPA_AUTH_AUTO)
				strcpy(strbuf, "Enterprise (RADIUS)");
			else if (vUInt == WPA_AUTH_PSK)
				strcpy(strbuf, "Personal (Pre-Shared Key)");
			break;
		case SYS_WLAN_RSPASSWD:
			if (!mib_get(MIB_WLAN_RS_PASSWORD, (void *)buffer))
				return -1;
			for (len=0; len<strlen(buffer); len++)
				strbuf[len]='*';
			strbuf[len]='\0';
			break;
		case SYS_WLAN_MODE:
			if ( !mib_get( MIB_WLAN_MODE, (void *)&vUInt) )
				return -1;
			strcpy(strbuf, wlan_mode[vUInt]);
			break;
		case SYS_WLAN_TXRATE:
			if ( !mib_get( MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&vUInt) )
				return -1;
			if (0 == vUInt){
				if ( !mib_get( MIB_WLAN_FIX_RATE, (void *)&vUShort) )
					return -1;
				for (i=0; i<12; i++)
					if (1<<i == vUShort)
						strcpy(strbuf, wlan_rate[i]);
			}
			else if (1 == vUInt)
				strcpy(strbuf, STR_AUTO);
			break;
		case SYS_WLAN_BLOCKRELAY:
			if ( !mib_get( MIB_WLAN_BLOCK_RELAY, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_DISABLE);
			else
				strcpy(strbuf, STR_ENABLE);
			break;
		case SYS_WLAN_AC_ENABLED:
			if ( !mib_get( MIB_WLAN_MACAC_ENABLED, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_DISABLE);
			else if(1 == vUInt)
				strcpy(strbuf, "Allow Listed");
			else if(2 == vUInt)
				strcpy(strbuf, "Deny Listed");
			else
				strcpy(strbuf, STR_ERR);
			break;
		case SYS_WLAN_WDS_ENABLED:
			if ( !mib_get( MIB_WLAN_WDS_ENABLED, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_DISABLE);
			else
				strcpy(strbuf, STR_ENABLE);
			break;
#endif
		case SYS_DNS_MODE:
			if ( !mib_get( MIB_DNS_MODE, (void *)&vUInt) )
				return -1;
			if (0 == vUInt)
				strcpy(strbuf, STR_AUTO);
			else
				strcpy(strbuf, STR_MANUAL);
			break;

		case SYS_DHCP_MODE:
			if(!mib_get( MIB_DHCP, (void *)&vUInt) )
				return -1;

			if(DHCP_DISABLED== vUInt)
				strcpy(strbuf, dhcp_mode[0]);
			else if(DHCP_SERVER == vUInt)
				strcpy(strbuf, dhcp_mode[2]);
			else if(DHCP_RELAY == vUInt)
				strcpy(strbuf, dhcp_mode[1]);

			break;

		default:
			return -1;
	}

	return 0;
}


#endif

#ifdef MULTI_WAN_SUPPORT
int getWanIfaceEntry(int index,WANIFACE_T* pEntry)
{
	memset(pEntry, '\0', sizeof(*pEntry));	
	*((char *)pEntry) = (char)index;
	if(!apmib_get(MIB_WANIFACE_TBL,(void *)pEntry)){
		printf("get wanIface mib error\n");
		return 0;
	}
}

int getPPPIfName(int index,char* ifName,int len)
{
	if(!ifName||len<6) return -1;
	if(index<1||index>WANIFACE_NUM){
		printf("Invalid wan index %d!\n",index);
		return -1;
	}
	if(index==1) snprintf(ifName,len,"ppp1");
	else snprintf(ifName,len,"ppp%d",index+6);
	return 0;
}

int updateWanIfName(void)
{
	WANIFACE_T WanIfaceEntry;
	int idx;
	char cmdbuf[128], tmpbuf[16];
	int vlanId = 0;
	char wanIfName[IFACE_NAME_MAX] ={0};


	memset((void *)cmdbuf, 0, sizeof(cmdbuf));
	
	for(idx=1; idx<=WANIFACE_NUM; idx++)
	{
		if(!getWanIfaceEntry(idx, &WanIfaceEntry))
		{
			printf("%s.%d.get waniface entry fail\n", __FUNCTION__, __LINE__);
			return -1;
		}
		if(getEthWanIfName(idx,wanIfName,sizeof(wanIfName))<0){
			printf("%s.%d.get getEthWanIfName fail idx=%d wanIfName=%s\n", __FUNCTION__, __LINE__,idx,wanIfName);
			return -1;
		}

		vlanId = WanIfaceEntry.vlan ? WanIfaceEntry.vlanid : -1;
		if(WanIfaceEntry.enable && vlanId > 0)
			sprintf(tmpbuf, "%s.%d:", wanIfName, vlanId);
		else
			sprintf(tmpbuf, "%s:", wanIfName);

		strcat(cmdbuf, tmpbuf);
	}

	RunSystemCmd(MULTI_WAN_IFNAME_FILE, "echo", cmdbuf, NULL_STR);

	return 0;
}

int getWanIfName(int wan_idx, char *name)
{
	FILE *fp = NULL;
	char buf[128];
	char *token = NULL, *saveStr = NULL;
	int idx = 0;

	if(! name) 
		goto err_return;
	
	if(wan_idx < 1 || wan_idx > WANIFACE_NUM) 
		goto err_return;

	if(!isFileExist(MULTI_WAN_IFNAME_FILE))
	{
		updateWanIfName();

		if(!isFileExist(MULTI_WAN_IFNAME_FILE))
			goto err_return;
	}

	fp = fopen(MULTI_WAN_IFNAME_FILE, "r");
	if(!fp)
		goto err_return;

	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	fclose(fp);
	token = strtok_r(buf, ":", &saveStr);
	do{
		if(!token)
			break;

		idx++;
		if(idx == wan_idx)
			break;
		
		token = strtok_r(NULL, ":", &saveStr);
	}
	while(token);

	if(token == NULL || idx != wan_idx || memcmp(token, "eth", 3) != 0)
		goto err_return;
	
	sprintf(name, "%s", token);

	return 0;

err_return:
	
	printf("[%s %d] getWanIfName failed!\n", __FUNCTION__, __LINE__);
	return -1;
}

int getRealWanIfNameByIdx(int wan_index,char* wanIfname,int wanIfNameLen)
{
	WANIFACE_T WanIfaceEntry={0};
	if(wan_index<=0||wan_index>WANIFACE_NUM){
		fprintf(stderr,"Invalid wan_index %d!\n",wan_index);
		return -1;
	}
	if(!getWanIfaceEntry(wan_index,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return -1;
	}
	switch(WanIfaceEntry.AddressType){
	case PPPOE:
	case PPTP:
	case L2TP:
		if(getPPPIfName(wan_index,wanIfname,wanIfNameLen)<0){
			printf("%s.%d.getPPPIfName fail\n",__FUNCTION__,__LINE__);
			return -1;
		}		
		return 0;		
	default:
		break;
	}
	if(getWanIfName(wan_index,wanIfname)<0){
		fprintf(stderr,"getWanIfName fail!\n");
		return -1;
	}
	return 0;
}

int GET_REAL_IFNAME(char *ifName)	
{	
	int stridx;

	if(ifName == NULL)
		return 0;
	
	for(stridx=0; stridx<32; stridx++)	
		if(ifName[stridx] == '.')
		{
			ifName[stridx] = '\0';
			return 1;
		}

	return 0;
}

int getWanIfIdx(const char *name)
{
	int idx = 0;
	char realName[32] = {0};
	

	if(! name) 
		return -1;

	memcpy(realName, name, 32);
	GET_REAL_IFNAME(realName);
	
	idx=atoi(realName+3);//ppp* eth*
	if(idx>1) idx-=6;
	if(idx<1||idx>WANIFACE_NUM)
		return -1;
	return idx;
}
#endif

/*******************************************************************************
 * TR-111 Utility Function
 *******************************************************************************/
#ifdef TR069_ANNEX_G
static void procUdpConnReq(char *buf, unsigned int bufLen)
{
	int i;
	char *p;
	time_t ts = 0; // timestamp
	unsigned int id = 0;
	char un[CWMP_STUN_USERNAME_LEN+1]; // username
	char cn[CWMP_STUN_CNONCE_LEN+1]; // Cnonce
	char sig[CWMP_STUN_SIGNATURE_LEN+1]; // Signature

	char sigStr[CWMP_STUN_SIGNATURE_STRING_LEN];
	char hash[20];
	char hashStr[CWMP_STUN_SIGNATURE_LEN+1];
	
	char connReqUN[CWMP_CONREQ_USERNAME_LEN+1];
	char connReqPW[CWMP_CONREQ_PASSWD_LEN+1];

	tr181_printf("UDP Conn Req: %s", buf);
	tr181_printf("Len (%d)", bufLen);

#if 1  // validation and authentication

	if (buf == NULL)
	{
		tr181_printf("buf NULL error!");
		return;
	}

	if (strncmp(buf, "GET", 3) != 0)
	{
		tr181_printf("not a GET msg!");
		return;
	}

	if (strstr(buf, "HTTP/1.1") == NULL)
	{
		tr181_printf("not a HTTP/1.1 msg!");
		return;
	}

	p = strtok(buf, "?=& ");

	while (p != NULL)
	{
		tr181_printf("strtok (%s)", p);

		if (strcmp(p, "ts") == 0)
		{
			p = strtok(NULL, "?=& ");

			if (p != NULL)
			{
				ts = (time_t)atoi(p);
				tr181_printf("ts (%d)", ts);
			}
		}
		else if (strcmp(p, "id") == 0)
		{
			p = strtok(NULL, "?=& ");

			if (p != NULL)
			{
				id = atol(p);
				tr181_printf("id (%ld)", id);
			}
		}
		else if (strcmp(p, "un") == 0)
		{
			p = strtok(NULL, "?=& ");

			if (p != NULL)
			{
				strcpy(un, p);
				tr181_printf("un (%s)", un);
			}
		}
		else if (strcmp(p, "cn") == 0)
		{
			p = strtok(NULL, "?=& ");

			if (p != NULL)
			{
				strcpy(cn, p);
				tr181_printf("cn (%s)", cn);
			}
		}
		else if (strcmp(p, "sig") == 0)
		{
			p = strtok(NULL, "?=& ");

			if (p != NULL)
			{
				strcpy(sig, p);
				tr181_printf("sig (%s)", sig);
			}
		}

		p = strtok(NULL, "?=& ");
	}

	if (ts <= g_ts)
	{
		tr181_printf("ts(%d) <= g_ts(%d) error!", ts, g_ts);
		return;
	}
	else
	{
		g_ts = ts;
	}

	if (id == g_id)
	{
		tr181_printf("id(%ld) == g_id(%ld) error!", id, g_id);
		return;
	}
	else
	{
		g_id = id;
	}

	mib_get(MIB_CWMP_CONREQ_USERNAME, connReqUN);

	if (strcmp(un, connReqUN) != 0)
	{
		tr181_printf("username (un) mismatch error!");
		return;
	}

	mib_get(MIB_CWMP_CONREQ_PASSWORD, connReqPW);

	sprintf(sigStr, "%d%ld%s%s", ts, id, un, cn);

	tr181_printf("sigStr (%s)", sigStr);

    /* use sigStr with connReqPW to generate Hmac for comparison. */
#ifdef HAS_MESSAGE_INTEGRITY
	computeHmac(hash, sigStr, strlen(sigStr), connReqPW, strlen(connReqPW));
#else
#error "please enable cwmp with OpenSSL, TR-111(cwmp Annex G needs it)!"
#endif

#endif

	cwmpSendUdpConnReq();
}

static void stun_getHost(char *host, char *url)
{ 
	const char *s;
	int i, n;

	if (!url || !*url)
		return;

	s = strchr(url, ':');
	if (s && s[1] == '/' && s[2] == '/')
		s += 3;
	else
		s = url;

	tr181_printf("s (%s)", s);

	n = strlen(s);

	for (i = 0; i < n; i++)
	{ 
		host[i] = s[i];
		if (s[i] == '/' || s[i] == ':')
			break; 
	}

	host[i] = '\0';
	return;
}

static int stun_getHostByName(char *addr, struct in_addr *inaddr)
{ 
	in_addr_t iadd = -1;
	struct hostent hostent, *host = &hostent;

	/* iadd: inet_addr() returns network order */
	iadd = inet_addr(addr);

	if (iadd != -1)
	{ 
		memcpy(inaddr, &iadd, sizeof(iadd));
		CWMPDBG_FUNC(MODULE_TR_111, LEVEL_DETAILED, ("[%s:%d] iadd:%x\n", __FUNCTION__, __LINE__, iadd));
		return 0;
	}

	host = gethostbyname(addr);

	if (!host)
	{ 
		tr181_printf("Host name not found");
		return -1;
	}

	memcpy(inaddr, host->h_addr, host->h_length);

	return 0;
}

static void *stun_thread(void *arg)
{
	StunAddress4 stunServer, stunMappedAddr;
	StunMessage stunMsg;
	Socket priFd = -1, secFd = -1, s = -1;
	struct in_addr in;
	char checkAddr[CWMP_UDP_CONN_REQ_ADDR_LEN+1];
	char checkStunServerAddrAndPort[CWMP_UDP_CONN_REQ_ADDR_LEN+1];
	char host[CWMP_ACS_URL_LEN+1];
	unsigned int checkNat;
	unsigned int stunPeriod;
	struct timeval now;
	struct timespec outtime;
	time_t now_time_t, out_time_t;

	tr181_printf("stun started!!!");
	//sleep(1); //wait for set value to mib

	pthread_mutex_lock(&mutex);

stun_reread:
	stunState = STUN_BINDING_REQ;

	mib_get(MIB_CWMP_STUN_SERVER_ADDR, stunServerAddr);
	mib_get(MIB_CWMP_STUN_SERVER_PORT, &stunServerPort);
	mib_get(MIB_CWMP_STUN_USERNAME, stunUsername);
	mib_get(MIB_CWMP_STUN_PASSWORD, stunPassword);
	mib_get(MIB_CWMP_STUN_MAX_KEEP_ALIVE_PERIOD, &stunMaxPeriod);
	mib_get(MIB_CWMP_STUN_MIN_KEEP_ALIVE_PERIOD, &stunMinPeriod);

	mib_get(MIB_CWMP_ACS_URL, acsUrl);

	tr181_printf("stunServerAddr (%s)", stunServerAddr);
	tr181_printf("stunServerPort (%d)", stunServerPort);
	tr181_printf("stunUsername (%s)", stunUsername);
	tr181_printf("stunPassword (%s)", stunPassword);
	tr181_printf("stunMaxPeriod (%d)", stunMaxPeriod);
	tr181_printf("stunMinPeriod (%d)", stunMinPeriod);
	tr181_printf("acsUrl (%s)", acsUrl);

	memset(&stunServer, 0, sizeof(StunAddress4));

	if (strlen(stunServerAddr) == 0)
	{
		stun_getHost(host, acsUrl);
	}
	else
	{
		stun_getHost(host, stunServerAddr);
	}

	tr181_printf("host %s", host);

	while(!stunStop)
	{
		if (stun_getHostByName(host, &in) == -1)
			sleep(5);
		else
			break;
		//goto stun_err;
	}

	/* stunServer.addr uses host order */
	stunServer.addr = ntohl((UInt32)in.s_addr);

	/* stunServer.port uses host order */
	if (stunServerPort == 0)
	{
		stunServer.port = STUN_PORT; // use default stun port number
	}
	else
	{
		stunServer.port = stunServerPort;
	}

	tr181_printf("stun server: %s:%d", inet_ntoa(in), stunServer.port);
	sprintf(OldStunServerAddrAndPort, "%s:%d", inet_ntoa(in), stunServer.port);
	CWMPDBG_FUNC(MODULE_TR_111, LEVEL_INFO, ("[%s:%d]OldStunServerAddrAndPort:%s\n", __FUNCTION__, __LINE__, OldStunServerAddrAndPort));
	CWMPDBG_FUNC(MODULE_TR_111, LEVEL_INFO, ("[%s:%d]stunServer.addr:%x\n", __FUNCTION__, __LINE__, stunServer.addr));
	
	while (!stunStop)
	{
		tr181_printf("stunState (%d)", stunState);

		memset(&stunMsg, 0, sizeof(StunMessage));
		
		switch (stunState)
		{
			case STUN_BINDING_REQ:
				tr181_trace();

				priFd = openPort(0, 0);

				stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 0, 0, NULL, stunUsername, NULL, procUdpConnReq);
				if (stunMsg.hasErrorCode && stunMsg.errorCode.errorClass == 4 &&
							stunMsg.errorCode.number == 1) // Unauthorized
				{
					msgIntegrityDetected = 1;
					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);

					if (stunMsg.hasErrorCode)
					{
						tr181_printf("Unauthorized Username/Password");
						close(priFd);
						priFd = -1;
						break;
					}
				}
				else
				{
					msgIntegrityDetected = 0;
				}

				if (stunMsg.hasMappedAddress)
				{
					s = openPort(11000, stunMsg.mappedAddress.ipv4.addr);

					if (s != INVALID_SOCKET) // not behind NAT
					{
						close(s);
						s = -1;
						natDetected = 0;
					}
					else
					{
						natDetected = 1;
					}

					stunMappedAddr.addr = stunMsg.mappedAddress.ipv4.addr;
					stunMappedAddr.port = stunMsg.mappedAddress.ipv4.port;

					in.s_addr = htonl(stunMsg.mappedAddress.ipv4.addr);
					sprintf(udpConnReqAddr, "%s:%d", inet_ntoa(in),
						stunMsg.mappedAddress.ipv4.port);

					mib_set(MIB_CWMP_UDP_CONN_REQ_ADDR, udpConnReqAddr);
					mib_set(MIB_CWMP_NAT_DETECTED, &natDetected);

					tr181_printf("isNAT: %d, UDPConnReqAddr: %s", natDetected, udpConnReqAddr);

					if (natDetected)
					{
						/* Binding Req with BindingChange attr. */
						if(msgIntegrityDetected == 1)
							stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);
						else
							stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, NULL, procUdpConnReq);

#if 0//def _CWMP_WITH_SSL_
						if (stunMsg.hasErrorCode && stunMsg.errorCode.errorClass == 4 &&
							stunMsg.errorCode.number == 1) // Unauthorized
						{
							stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);

							if (stunMsg.hasErrorCode)
							{
								tr181_printf("Unauthorized Username/Password");
								close(priFd);
								break;
							}
						}
#endif

						if (stunMaxPeriod == stunMinPeriod)
						{
							stunPeriod = stunMinPeriod;
							stunState = STUN_BINDING_MAINTAIN;
						}
						else
						{
							stunPeriod = (stunMinPeriod + stunMaxPeriod) / 2;
							if(stunPeriod == stunMinPeriod || stunPeriod == stunMaxPeriod)
								stunState = STUN_BINDING_MAINTAIN;
							else
								stunState = STUN_BINDING_TIMEOUT_DISCOVERY;
						}

						tr181_printf("stunState (%d) stunPeriod (%d) "
							"stunMinPeriod (%d) stunMaxPeriod (%d)",
							stunState, stunPeriod, stunMinPeriod, stunMaxPeriod);
					}
					else
					{
						tr181_trace();
						close(priFd);
						priFd = -1;
						gettimeofday(&now, NULL);
						outtime.tv_sec = now.tv_sec + stunMaxPeriod;
						outtime.tv_nsec = now.tv_usec * 1000;
						pthread_cond_timedwait(&cond, &mutex, &outtime);
						//sleep(stunMaxPeriod);
					}
				}
				else
				{
					tr181_trace();
					close(priFd);
					priFd = -1;
					gettimeofday(&now, NULL);
					//outtime.tv_sec = now.tv_sec + stunMaxPeriod;
					outtime.tv_sec = now.tv_sec + 10;
					outtime.tv_nsec = now.tv_usec * 1000;
					pthread_cond_timedwait(&cond, &mutex, &outtime);
					goto stun_reread;
					//sleep(stunMaxPeriod);
				}
				break;

			case STUN_BINDING_MAINTAIN:
				pthread_mutex_unlock(&mutex);
				now_time_t = time(NULL);
				out_time_t = now_time_t + stunPeriod;
				while(now_time_t < out_time_t)
				{
					stunTest_tr111_recv(priFd, 1, &stunMsg, procUdpConnReq);
					if(stunState != STUN_BINDING_MAINTAIN)
					{
						pthread_mutex_lock(&mutex);
						goto stun_binding_maintain_break;
					}
					now_time_t = time(NULL);
				}
				pthread_mutex_lock(&mutex);
				//gettimeofday(&now, NULL);
				//outtime.tv_sec = now.tv_sec + stunPeriod;
				//outtime.tv_nsec = now.tv_usec * 1000;
				//pthread_cond_timedwait(&cond, &mutex, &outtime);
				//sleep(stunPeriod);
				
				/* jump to STUN_BINDING_REQ if local IP changed */

				if(msgIntegrityDetected == 1)
					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 0, 0, NULL, stunUsername, stunPassword, procUdpConnReq);
				else
					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 0, 0, NULL, stunUsername, NULL, procUdpConnReq);

				if (stunMsg.hasMappedAddress)
				{
					s = openPort(11000, stunMsg.mappedAddress.ipv4.addr);

					if (s != INVALID_SOCKET) // not behind NAT
					{
						close(s);
						s = -1;
						checkNat = 0;
					}
					else
					{
						checkNat = 1;
					}

					in.s_addr = htonl(stunMsg.mappedAddress.ipv4.addr);
					sprintf(checkAddr, "%s:%d", inet_ntoa(in),
						stunMsg.mappedAddress.ipv4.port);

					tr181_printf("isNAT: %d, UDPConnReqAddr: %s", checkNat, checkAddr);

					if (checkNat != natDetected)
					{
						tr181_printf("natDetected changed!!!");
						mib_set(MIB_CWMP_NAT_DETECTED, &checkNat);
						natDetected = checkNat;
					}

					if (strcmp(checkAddr, udpConnReqAddr) != 0)
					{
						tr181_printf("UDPConnReqAddr changed!!!");
						mib_set(MIB_CWMP_UDP_CONN_REQ_ADDR, checkAddr);
						strcpy(udpConnReqAddr, checkAddr);

						/* do sometheing according to G.2.1.3 */
						if (natDetected)
						{
							/* Binding Req with BindingChange attr. */
							if(msgIntegrityDetected == 1)
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);
							else
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, NULL, procUdpConnReq);

#if 0//def _CWMP_WITH_SSL_
							if (stunMsg.hasErrorCode && stunMsg.errorCode.errorClass == 4 &&
								stunMsg.errorCode.number == 1) // Unauthorized
							{
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);

								if (stunMsg.hasErrorCode)
								{
									tr181_printf("Unauthorized Username/Password");
									stunState = STUN_BINDING_REQ;
									close(priFd);
									break;
								}
							}
#endif
						}
					}
				}
				else
				{
					tr181_trace();
					CWMPDBG_FUNC(MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d] got UDP connection response, instead of Bind Response\n", __FUNCTION__, __LINE__));
                    /*
                     * do not reset socket after receiving UDP connection request
                     * It causes each UDP connection request from ACS uses different dport, which is not expected by some ISP.
                     */
					//stunState = STUN_BINDING_REQ;
					//close(priFd);
					//priFd = -1;
					//gettimeofday(&now, NULL);
					//outtime.tv_sec = now.tv_sec + stunMaxPeriod;
					//outtime.tv_nsec = now.tv_usec * 1000;
					//pthread_cond_timedwait(&cond, &mutex, &outtime);
					//sleep(stunMaxPeriod);
				}
stun_binding_maintain_break:
				break;

			case STUN_BINDING_TIMEOUT_DISCOVERY:
				gettimeofday(&now, NULL);
				outtime.tv_sec = now.tv_sec + stunPeriod;
				outtime.tv_nsec = now.tv_usec * 1000;
				pthread_cond_timedwait(&cond, &mutex, &outtime);
				//sleep(stunPeriod);

				secFd = openPort(stunMappedAddr.port+1, 0);

				if(msgIntegrityDetected == 1)
					stunTest_tr111_send(secFd, &stunServer, 1, 1, 0, 0, 1, &stunMappedAddr, stunUsername, stunPassword);
				else
					stunTest_tr111_send(secFd, &stunServer, 1, 1, 0, 0, 1, &stunMappedAddr, stunUsername, NULL);

				stunTest_tr111_recv(priFd, 1, &stunMsg, procUdpConnReq);

				if (stunMsg.hasMappedAddress) // not timeout yet
				{
					tr181_printf("not timeout yet, stunPeriod (%d)", stunPeriod);
					//stunPeriod *= 2;
					stunMinPeriod = stunPeriod;
					stunPeriod = (stunMinPeriod + stunMaxPeriod)/2;
					if(stunPeriod == stunMinPeriod || stunPeriod == stunMaxPeriod)
						stunState = STUN_BINDING_MAINTAIN;
					tr181_printf("stunPeriod = (%d)", stunPeriod);
				}
				else // already timeout
				{
					tr181_printf("already timeout, stunPeriod (%d)", stunPeriod);
					//stunPeriod /= 2;
					stunMaxPeriod = stunPeriod;
					stunPeriod = (stunMinPeriod + stunMaxPeriod)/2;
					tr181_printf("stunPeriod = (%d)", stunPeriod);
					stunState = STUN_BINDING_TIMEOUT_MONITORING;
				}

				close(secFd);
				secFd = -1;
				break;

			case STUN_BINDING_TIMEOUT_MONITORING:
				if(msgIntegrityDetected == 1)
					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 0, 0, NULL, stunUsername, stunPassword, procUdpConnReq);
				else
					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 0, 0, NULL, stunUsername, NULL, procUdpConnReq);

				if (stunMsg.hasMappedAddress)
				{
					s = openPort(11000, stunMsg.mappedAddress.ipv4.addr);

					if (s != INVALID_SOCKET) // not behind NAT
					{
						close(s);
						s = -1;
						checkNat = 0;
					}
					else
					{
						checkNat = 1;
					}

					stunMappedAddr.addr = stunMsg.mappedAddress.ipv4.addr;
					stunMappedAddr.port = stunMsg.mappedAddress.ipv4.port;

					in.s_addr = htonl(stunMsg.mappedAddress.ipv4.addr);
					sprintf(checkAddr, "%s:%d", inet_ntoa(in),
						stunMsg.mappedAddress.ipv4.port);

					tr181_printf("isNAT: %d, UDPConnReqAddr: %s", checkNat, checkAddr);

					if (checkNat != natDetected)
					{
						tr181_printf("natDetected changed!!!");
						mib_set(MIB_CWMP_NAT_DETECTED, &checkNat);
						natDetected = checkNat;
					}

					if (strcmp(checkAddr, udpConnReqAddr) != 0)
					{
						tr181_printf("UDPConnReqAddr changed!!!");
						mib_set(MIB_CWMP_UDP_CONN_REQ_ADDR, checkAddr);
						strcpy(udpConnReqAddr, checkAddr);

						/* do sometheing according to G.2.1.3 */
						if (natDetected)
						{
							/* Binding Req with BindingChange attr. */
							if(msgIntegrityDetected == 1)
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);
							else
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, NULL, procUdpConnReq);

#if 0//def _CWMP_WITH_SSL_
							if (stunMsg.hasErrorCode && stunMsg.errorCode.errorClass == 4 &&
								stunMsg.errorCode.number == 1) // Unauthorized
							{
								stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);

								if (stunMsg.hasErrorCode)
								{
									tr181_printf("Unauthorized Username/Password (%d-%d)",
										         stunMsg.errorCode.errorClass,
										         stunMsg.errorCode.number);
									
									stunState = STUN_BINDING_REQ;
									close(priFd);
									break;
								}
							}
#endif
						}
					}


					if (!natDetected)
					{
						tr181_trace();
						gettimeofday(&now, NULL);
						outtime.tv_sec = now.tv_sec + stunPeriod;
						outtime.tv_nsec = now.tv_usec * 1000;
						pthread_cond_timedwait(&cond, &mutex, &outtime);
						//sleep(stunPeriod);
						break;
					}
				}
				else
				{
					tr181_trace();
					gettimeofday(&now, NULL);
					outtime.tv_sec = now.tv_sec + stunPeriod;
					outtime.tv_nsec = now.tv_usec * 1000;
					pthread_cond_timedwait(&cond, &mutex, &outtime);
					//sleep(stunPeriod);
					break;
				}

				tr181_printf("stunPeriod (%d)", stunPeriod);

				gettimeofday(&now, NULL);
				outtime.tv_sec = now.tv_sec + stunPeriod;
				outtime.tv_nsec = now.tv_usec * 1000;
				pthread_cond_timedwait(&cond, &mutex, &outtime);
				//sleep(stunPeriod);

				memset(&stunMsg, 0, sizeof(StunMessage));

				secFd = openPort(stunMappedAddr.port+1, 0);

				if(msgIntegrityDetected == 1)
					stunTest_tr111_send(secFd, &stunServer, 1, 1, 0, 0, 1, &stunMappedAddr, stunUsername, stunPassword);
				else
					stunTest_tr111_send(secFd, &stunServer, 1, 1, 0, 0, 1, &stunMappedAddr, stunUsername, NULL);

				stunTest_tr111_recv(priFd, 1, &stunMsg, procUdpConnReq);

				close(secFd);
				secFd = -1;

				if (stunMsg.hasMappedAddress) // not timeout yet
				{
					tr181_printf("not timeout yet");

					stunState = STUN_BINDING_MAINTAIN;
				}
				else // already timeout
				{
					tr181_printf("already timeout");

					close(priFd);
					priFd = -1;
					
					stunMaxPeriod = stunPeriod;
					stunState = STUN_BINDING_REQ;
				}
				break;

			case STUN_BINDING_CHANGE:
				mib_get(MIB_CWMP_STUN_SERVER_ADDR, stunServerAddr);
				mib_get(MIB_CWMP_STUN_SERVER_PORT, &stunServerPort);

				tr181_printf("stunServerAddr (%s)", stunServerAddr);
				tr181_printf("stunServerPort (%d)", stunServerPort);

				memset(&stunServer, 0, sizeof(StunAddress4));

				if (strlen(stunServerAddr) == 0)
				{
					stun_getHost(host, acsUrl);
				}
				else
				{
					stun_getHost(host, stunServerAddr);
				}

				tr181_printf("host %s", host);
					
				if (stun_getHostByName(host, &in) == -1)
					goto stun_err;
				
				stunServer.addr = ntohl((UInt32)in.s_addr);

				if (stunServerPort == 0)
				{
					stunServer.port = STUN_PORT; // use default stun port number
				}
				else
				{
					stunServer.port = stunServerPort;
				}

				tr181_printf("stun server: %s:%d", inet_ntoa(in), stunServer.port);
				sprintf(checkStunServerAddrAndPort, "%s:%d", inet_ntoa(in), stunServer.port);

				if (strcmp(checkStunServerAddrAndPort, OldStunServerAddrAndPort) != 0)
				{
					close(priFd);
					priFd = -1;
					tr181_printf("Stun Server Address or port changed!!!");
					strcpy(OldStunServerAddrAndPort, checkStunServerAddrAndPort);

					priFd = openPort(0, 0);

					stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, NULL, procUdpConnReq);
					if (stunMsg.hasErrorCode && stunMsg.errorCode.errorClass == 4 &&
								stunMsg.errorCode.number == 1) // Unauthorized
					{
						msgIntegrityDetected = 1;
						stunTest_tr111(priFd, &stunServer, 1, 1, &stunMsg, 1, 1, 0, NULL, stunUsername, stunPassword, procUdpConnReq);

						if (stunMsg.hasErrorCode)
						{
							tr181_printf("Unauthorized Username/Password");
							close(priFd);
							priFd = -1;
							break;
						}
					}
					else
					{
						msgIntegrityDetected = 0;
					}

					if (stunMsg.hasMappedAddress)
					{
						s = openPort(11000, stunMsg.mappedAddress.ipv4.addr);

						if (s != INVALID_SOCKET) // not behind NAT
						{
							close(s);
							s = -1;
							natDetected = 0;
						}
						else
						{
							natDetected = 1;
						}

						stunMappedAddr.addr = stunMsg.mappedAddress.ipv4.addr;
						stunMappedAddr.port = stunMsg.mappedAddress.ipv4.port;

						in.s_addr = htonl(stunMsg.mappedAddress.ipv4.addr);
						sprintf(udpConnReqAddr, "%s:%d", inet_ntoa(in),
							stunMsg.mappedAddress.ipv4.port);

						mib_set(MIB_CWMP_UDP_CONN_REQ_ADDR, udpConnReqAddr);
						mib_set(MIB_CWMP_NAT_DETECTED, &natDetected);

						tr181_printf("isNAT: %d, UDPConnReqAddr: %s", natDetected, udpConnReqAddr);

						if (natDetected)
						{
							if (stunMaxPeriod == stunMinPeriod)
							{
								stunPeriod = stunMinPeriod;
								stunState = STUN_BINDING_MAINTAIN;
							}
							else
							{
								stunPeriod = (stunMinPeriod + stunMaxPeriod) / 2;
								if(stunPeriod == stunMinPeriod || stunPeriod == stunMaxPeriod)
									stunState = STUN_BINDING_MAINTAIN;
								else
									stunState = STUN_BINDING_TIMEOUT_DISCOVERY;
							}

							tr181_printf("stunState (%d) stunPeriod (%d) "
								"stunMinPeriod (%d) stunMaxPeriod (%d)",
								stunState, stunPeriod, stunMinPeriod, stunMaxPeriod);
						}
						else
						{
							tr181_trace();
							close(priFd);
							priFd = -1;
							stunState = STUN_BINDING_REQ;
							gettimeofday(&now, NULL);
							outtime.tv_sec = now.tv_sec + stunMaxPeriod;
							outtime.tv_nsec = now.tv_usec * 1000;
							pthread_cond_timedwait(&cond, &mutex, &outtime);
							//sleep(stunMaxPeriod);
						}
					}
					else
					{
						tr181_trace();
						close(priFd);
						priFd = -1;
						stunState = STUN_BINDING_REQ;
						gettimeofday(&now, NULL);
						outtime.tv_sec = now.tv_sec + stunMaxPeriod;
						outtime.tv_nsec = now.tv_usec * 1000;
						pthread_cond_timedwait(&cond, &mutex, &outtime);
						//sleep(stunMaxPeriod);
					}
				}
				else
				{
					tr181_trace();
					stunState = OldStunState;
				}
				break;

			case STUN_CHANGE_OTHER_PARAM:
				goto stun_reread;

			default:
				break;
		}
	}

stun_err:

	if(priFd != -1)
		close(priFd);
	if(secFd != -1)
		close(secFd);
	if(s != -1)
		close(s);

	tr181_printf("stun stopped!!!");

	stunThread = 0;
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void cwmpStartStun()
{
	pthread_t stun_pid;
	
	tr181_trace();
	
	while (stunThread)
	{
		tr181_printf("stun thread exist, stop current stun thread.");
		cwmpStopStun();
		sleep(1);
	}

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	stunThread = 1;
	stunStop = 0;
	tr181_printf("create stun thread.");
	if( pthread_create( &stun_pid, NULL, stun_thread, 0 ) != 0 )
	{
		tr181_printf("stun thread create fail!!!");
		stunThread = 0;
		return;
	}
	
	pthread_detach(stun_pid);
}

void cwmpStopStun()
{
	tr181_trace();

	stunStop = 1;	
}
#endif


#ifdef _CWMP_APPLY_
/*******************************************************************************/
/**** APIs for applying the new values****/
/*******************************************************************************/
struct CWMP_APPLY *pApplyList=NULL;

struct CWMP_APPLY *apply_malloc(void)
{
	struct CWMP_APPLY *p=NULL;	
	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	p = (struct CWMP_APPLY *)malloc( sizeof(struct CWMP_APPLY) );
	if(p) memset( p, 0, sizeof(struct CWMP_APPLY) );
	return p;
}

void apply_dealloc(struct CWMP_APPLY *p)
{
	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	if(p)
	{
		if( p->olddata ) free( p->olddata );
		free( p );
	}	
	return;
} 

int apply_add2list(struct CWMP_APPLY **root, struct CWMP_APPLY *p)
{
	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	if( (root==NULL) || (p==NULL) ) return -1;

	CWMPDBG( 1, ( stderr, "<%s:%d>add to list: cb_fun:0x%x, action:%d, id=%d\n", __FUNCTION__, __LINE__, p->cb_fun, p->action, p->id ) );
	if( *root )
	{
		struct CWMP_APPLY *c, *pre=NULL;

		//found if the callback function had been put in the list.
		c = *root;
		while( c )
		{
			if( (c->cb_fun==p->cb_fun) && (c->action==p->action) && (c->id==p->id) )
			{
				CWMPDBG( 1, ( stderr, "<%s:%d>found the cb_fun had been put in the list\n", __FUNCTION__, __LINE__ ) );
				//dealloc
				apply_dealloc( p );
				return 0;
			}
			c=c->next;
		}

		//sort by priority
		c = *root;
		pre = NULL;
		while( c )
		{
			if( p->priority < c->priority )
			{
				if(pre)
					pre->next = p;
				else
					*root = p; //the first one
				p->next = c;
				break;
			}else if(c->next==NULL) //the last one
			{
				c->next = p;
				break;
			}
			
			pre = c;
			c=c->next;
		}
	}else
		*root = p;
		
	return 0;
}

int apply_add(
	int			priority,
	void			*cb_fun,
	int			action,
        int	 		id,
        void 			*olddata,
        int			size
)
{
	struct CWMP_APPLY *p;
	
	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	if( cb_fun==NULL ) return -1;

	p=apply_malloc();
	if(p)
	{
		p->priority = priority;
		p->cb_fun = cb_fun;
		p->action = action;
		p->id = id;
		if( (size>0) && (olddata!=NULL) )
		{
			p->olddata = malloc( size );
			if(p->olddata)
			{
				memcpy(p->olddata, olddata, size );
			}else{
				free(p);
				return -1;
			}
		}
		apply_add2list( &pApplyList, p );
		return 0;
	}
	return -1;
}

int apply_destroy( void )
{
	struct CWMP_APPLY *c;

	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	
	c = pApplyList;
	while( c )
	{
		struct CWMP_APPLY *n = c->next;
		
		CWMPDBG( 1, ( stderr, "<%s:%d>free cb_fun:0x%x, action:%d, id=%d\n", __FUNCTION__, __LINE__, c->cb_fun, c->action, c->id ) );
		apply_dealloc( c );
		
		c = n;
	}
	pApplyList = NULL;
	return 0;
}


int apply_takeaction( void )
{
	struct CWMP_APPLY *c;

	CWMPDBG( 3, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__ ) );
	
	c = pApplyList;
	while( c )
	{
		CWMPDBG( 1, ( stderr, "<%s:%d>cb_fun:0x%x, action:%d, id=%d\n", __FUNCTION__, __LINE__, c->cb_fun, c->action, c->id ) );
		if(c->cb_fun)
		{
			c->cb_fun( c->action, c->id, c->olddata );
		}		
		c = c->next;
	}
	
	apply_destroy();
	return 0;
}
/*******************************************************************************/
/**** End APIs for applying the new values****/
/*******************************************************************************/
#else
/*
 * define fake functions here for compiler happy
 */
int apply_add(
	int 		priority,
	void			*cb_fun,
	int 		action,
		int 		id,
		void			*olddata,
		int 		size
)
{
	return 0;
}
int apply_destroy( void ) {return 0;}
int apply_takeaction( void ) {return 0;}
#endif
