#include "rtk_bleconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#include <apmib.h>
#include <rtk_btconfig_api.h>
#include <common/rtk_wlan.h>

#define DEFAULT_UNIX_SOCKET_NAME "/tmp/bleconfig_cmd_listener"
#define DEFAULT_PROFILE_FILE     "/var/bleconfig.cfg"

#define UDP_DISCOVERY_PORT 61214


pthread_t g_bleconfig_cmd_thread;

extern rtk_btconfig_API btconfig_api_table[];

struct bleconfig_userdata {
    int blecfg_cmd_thread_go;       //0:off     1:on
    int blecfg_cmd_is_exist;        //0:leave 1:exist  
    struct rtk_wlan_config_info configData;
    struct rtk_connect_AP_info connecData;
};

/*
typedef struct _SOCKETQ socketq_t;
struct _SOCKETQ {
	unsigned int req;
	unsigned int count;
    unsigned char bssid[6];
    char ssid[33];
    char pwd[65];
    char pin[64];
};
*/

int g_clientSocketId = -1;


void rtk_bleonfig_cmd_reply(unsigned short tag,unsigned char result)
{
	char send_buf[16];
	char *pBuf;
	int sent_len,total_len;
	memset(send_buf,0,sizeof(send_buf));
	pBuf = send_buf;
	pBuf = rtk_btconfig_add_tlv(pBuf, tag,1, (void *)&result);
	total_len = RTK_BTCONFIG_MSG_HDR_LEN+1;
	sent_len = send(g_clientSocketId, send_buf, total_len, 0);
	RTK_BTCONFIG_DEBUG("sent_len:%d\n",sent_len);
}

int create_unix_socket_server(const char *socketName)
{
    int s, len;
    struct sockaddr_un local;
    int status;
    struct stat st;
    char strSocketName[128] = {0};
    if (NULL == socketName)
        strncpy(strSocketName, DEFAULT_UNIX_SOCKET_NAME, sizeof(strSocketName));
    else
        strncpy(strSocketName, socketName, sizeof(strSocketName));

    //printf("create socket server: %s\n", strSocketName);
    // make sure clients have permission to connect to
    // the socket node.
    mode_t oldMask;
    oldMask = umask(000);

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        printf("socket: %s\n", strerror(errno));
        return -1;
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, strSocketName);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    status = stat(strSocketName, &st);
    if (0 == status) {
        /**
         * A file already exists. Check if this file is a socket node.
         * If yes, unlink it.
         * If no, treat it as an error condition.
         */
        if ((st.st_mode & S_IFMT) == S_IFSOCK) {
            status = unlink(strSocketName);
            if (status != 0) {
                fprintf(stderr, "Error unlinking the socket node: %s\n", strSocketName);
                return -2;
            }
            //printf("unlink existing socket node: %s\n", strSocketName);
        }
        else {
            fprintf(stderr, "The path %s already exists and is not a socket node.\n", strSocketName);
            return -2;
        }
    }
    else {
        if (ENOENT == errno) {
            /* Do nothing */
            //printf("No exist socket node %s. It is safe to open the socket node.\n", strSocketName);
        }
        else {
            fprintf(stderr, "Error stating the socket node path: %s\n", strSocketName);
            return -2;
        }
    }

    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        printf("bind: %d(%s)\n", errno, strerror(errno));
        return -3;
    }

    if (listen(s, 5) == -1) {
        printf("listen: %s\n", strerror(errno));
        return -4;
    }
    umask(oldMask);
    // change access permission for socket node to 0666
    chmod(strSocketName, S_IRGRP|S_IROTH|S_IRUSR|S_IWGRP|S_IWOTH|S_IWUSR);
    return s;
}

int blecfg_receive_request(int socketId, void *req, int *reqLen, long timeout_sec)
{
    struct timeval tv;
	fd_set rfds;
	int retval=0;
	struct sockaddr_un remote;
	size_t t;
	FD_ZERO(&rfds);
	FD_SET(socketId, &rfds);
	
	if (timeout_sec != -1) {
		tv.tv_sec = timeout_sec;
		tv.tv_usec = 0; //timeoutMS*1000;
	}
	retval = select(socketId+1, &rfds, NULL, NULL, (timeout_sec==-1)?NULL:&tv);
	if (retval && FD_ISSET(socketId, &rfds))	{
		if (retval == -1) {
			printf("select failed: %d(%s)\n", errno, strerror(errno));
			return retval;
		} 

	
		t = sizeof(remote);
		if ((g_clientSocketId = accept(socketId, (struct sockaddr *)&remote, &t)) == -1) {
			printf("accept failed: %d(%s)\n", errno, strerror(errno));
			return -1;
		}

		retval = recv(g_clientSocketId, (char*)req, *reqLen, 0);
		*reqLen = retval;
		
	}else
		retval=0;
    return retval;
}



int rtk_bleconfig_reply_fail(unsigned char *reqCmd, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
    unsigned char* tmp;
	char result[] = "fail";
	char *pBuf;
	int sent_len,total_len;

    memset(retBuf,0,RTK_BTCONFIG_MSG_HDR_LEN+strlen(result));
    tmp = retBuf;
	rtk_btconfig_add_tlv(tmp, TAG_BT_CMD_REPLY_FAIL,strlen(result), (void *)result);
	total_len = RTK_BTCONFIG_MSG_HDR_LEN+strlen(result);
    *retBufLen = total_len;

    return 1;
}


int blecfg_handleRequest(struct bleconfig_userdata *bc_u, unsigned char *req, unsigned int reqLen, unsigned char *retBuf, unsigned int *retBufLen)
{
    int i = 0;
    int ret = -1;
    unsigned short type = 0;
    unsigned char* pData;

    memcpy(&type, req, 2);
    type = ntohs(type);
#if DEBUG_MSG 
    printf("<%d>bleconfig recv:(%d)\n",type,reqLen);
        for(i=0;i<reqLen;i++){
            if(i>0 && i%10==0)printf("\n"); 
                printf("%02X ",req[i]);
        }
        printf("--------------------------------\n");
#endif
	unsigned short tag = rtk_btconfig_get_cmd_tag(req,reqLen);
	while(btconfig_api_table[i].cmd_id != TAG_BT_CMD_INVALID)
	{
		if(btconfig_api_table[i].cmd_id == tag)
		{
			if(TAG_BT_CMD_CONNECT == tag)
			{
				RTK_BTCONFIG_DEBUG("reply connect...........\n");
				rtk_bleonfig_cmd_reply(TAG_BT_CMD_REPLY_CONNECT,1);
			}
			ret = btconfig_api_table[i].cmd_function(req,reqLen,retBuf,retBufLen);
			if(ret == RTK_BTCONFIG_RET_SUCCESS)
			{
				ret = 1;
			}
			break;
		}
		i ++;
	}
    return ret;
}

void *bleconfig_cmd_thread(void *arg)
{
    int i=0;
    int recvN = 0;
	int socketId=-1;
    int clientSocketId=-1;
    int recvBufLen = 0;
    unsigned int resultBufLen = 0;
    unsigned char cmd[256] = {0};
    unsigned char resultBuf[4096] = {0};
    struct sockaddr_un from;
    socklen_t from_len = 0;
    struct bleconfig_userdata *bleconfigData = (struct bleconfig_userdata *)arg;

    unlink(DEFAULT_UNIX_SOCKET_NAME);
	socketId = create_unix_socket_server(DEFAULT_UNIX_SOCKET_NAME);
    if (socketId < 0) {
	    printf("create bleconfig unix socket server failed: %d (%s)\n", errno, strerror(errno));
	    return NULL;
	}

    bleconfigData->blecfg_cmd_thread_go = 1;
    bleconfigData->blecfg_cmd_is_exist = 1;

    

    while(bleconfigData->blecfg_cmd_thread_go) {

        memset(cmd,0,sizeof(cmd));
        memset(resultBuf,0,sizeof(resultBuf));
        recvBufLen = sizeof(cmd);
        //recvN = blecfg_receive_request(socketId, &clientSocketId, cmd, 500,&from); // noblocking

        recvN = blecfg_receive_request(socketId, cmd, &recvBufLen,5);
        if (recvN <= 0) {
            if (recvN < 0){
            	printf("mr-cmd recv failed: %d(%s)\n",  errno, strerror(errno));
            }
            continue;
        } else {
            if (blecfg_handleRequest(bleconfigData, cmd, recvN,resultBuf,&resultBufLen) > 0){
/*
                printf("blecfg_handleRequest-----------------%d\n",resultBufLen);
            	for(i=0;i<resultBufLen;i++){
                    if(i>0 && i%10==0)printf("\n");
                    printf("%02X ",resultBuf[i]);
                }
                printf("\n");
*/                
                
                from_len = sizeof(struct sockaddr_un);
                //recvN = sendto(socketId,resultBuf,resultBufLen,0,(struct sockaddr *)&from, sizeof(from));
                //recvN = send(clientSocketId,(char*)resultBuf,resultBufLen,0);
				if(resultBuf != NULL && resultBufLen > 0)
				{
                	recvN = send(g_clientSocketId, resultBuf, resultBufLen, 0);
					if(recvN>0){
						//printf("blecfg_handleRequest ok\n");
					}else printf("blecfg_handleRequest fail:%d(%s)\n",	errno, strerror(errno));
					
				}
            }else{
                
                printf("Not hand socket request\n");
            }
            
            close(g_clientSocketId);
        }

    }


    if(socketId>0) close(socketId);
    printf("quit bleconfig_cmd thread\n");
    
    return NULL;
}


int start_cmd_listener_thread(void *userdata)
{
    struct bleconfig_userdata *bleconfigData;

    if (userdata == NULL) {
		printf("%s - userdata is null\n", __func__);
 		return 0;
	}

    bleconfigData = (struct bleconfig_userdata *)userdata;
    
    if (pthread_create(&g_bleconfig_cmd_thread, NULL, bleconfig_cmd_thread, bleconfigData) < 0) {
	    return 0;
	}
	
	return 1;
}

int stop_cmd_listener_thread(void *userdata)
{
    struct bleconfig_userdata *bleconfigData;
    int maxWaitCount = 0;
    if (userdata == NULL) {
		printf("%s - userdata is null\n", __func__);
 		return 0;
	}

    bleconfigData = (struct bleconfig_userdata *)userdata;
    
    bleconfigData->blecfg_cmd_thread_go = 0;

    while(maxWaitCount++<100){
        usleep(100000);
        if(bleconfigData->blecfg_cmd_is_exist==0)
            break;
    }
    
	if (pthread_join (g_bleconfig_cmd_thread, NULL)<0) {
		return 0;
	}

    printf("<BLE_CONFIG_CMD_THREAD LEAVE>\n");
    
	return 1;
}

int main (int argc,char **argv)
{
	int sk,nsk,result,retry=0;
    int CheckWiFiConnection1,CheckWiFiConnection2;
	int status;
    char connect_ap_info[100] = {0};
	char buf[128] = {0};
	char recv_buf[512] = {0};
	int pid = -1;
	int bytes_read = 0;
    unsigned int isSaveProfile=0;
    struct sigaction sa;
    struct bleconfig_userdata bleconfigData;
    struct rtk_btconfig_status pStatus;

    //fprintf(stderr,"========== init BT Setting start =============\n");
	system("add_le_adv.sh");
	//fprintf(stderr,"========== init BT Setting End =============\n");
	sleep(1);
    
    memset (&bleconfigData, 0, sizeof(struct bleconfig_userdata));
    
	//RTK_BTCONFIG_DEBUG("Init apmib...\n");
	apmib_init();
	//RTK_BTCONFIG_DEBUG("Creating unix socket...\n");

    set_connect_status(0);
    apmib_get(MIB_BT_CONFIG_CONFIGURED,(void *)&isSaveProfile);
    if(isSaveProfile==1){
        set_connect_status(2);
    }

    start_cmd_listener_thread(&bleconfigData);
    
	RTK_BTCONFIG_DEBUG("Waiting for connection...\n");
	while(1)
	{
        //check wifi state
        sleep(1);
#if 0 

        CheckWiFiConnection1 = Check_Wlan_isConnected(CLIENT_MODE, connect_ap_info,"wlan0-vxd");//5G
        CheckWiFiConnection2 = Check_Wlan_isConnected(CLIENT_MODE, connect_ap_info,"wlan1-vxd");//2.4G
        if( CheckWiFiConnection1 || CheckWiFiConnection2){ 

            printf("==== connect_ap_info : %s \n",connect_ap_info);
          
            if( rtk_btconfig_get_current_status(&pStatus)!=1 ){
                //printf("rtk_btconfig_get_current_status FAIL\n");
            }

            if( get_connect_status() == 1){/*0:disconnected,1:connected,2:BT_CONFIG_CONFIGURED*/
                printf("\n\n########### Connected #############\n\n");

                //send cmd to client
                retry=0;
                do{
                    result = response_scuccess_ack();
                    if(result==0){
                        if(retry++>=30)
                        break;
                    }
                }while(result!=1);

                set_connect_status(2);

                apmib_get(MIB_BT_CONFIG_CONFIGURED,(void *)&isSaveProfile);
                if(isSaveProfile!=1){
                    isSaveProfile = 1;
        			apmib_set(MIB_BT_CONFIG_CONFIGURED,(void *)&isSaveProfile);
                    printf("#### SAVE_PROFILE ####\n");
                }
                
            }
            
        }
#endif 


#if 0
        if(signals_inited == 0){
            RTK_BTCONFIG_DEBUG("Init signals.....\n");
            signal(SIGCHLD,sig_child_handler);			
            signal(SIGUSR1, sig_user_handler);
#if defined(FOR_DUAL_BAND)
            signal(SIGUSR2, sig_user_handler);
#endif
            signals_inited = 1;
        }
#endif        
	}

    stop_cmd_listener_thread(&bleconfigData);

    return 1;
}

