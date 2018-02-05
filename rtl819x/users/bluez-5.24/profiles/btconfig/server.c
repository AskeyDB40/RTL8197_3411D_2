/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2011  Nokia Corporation
 *  Copyright (C) 2011  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>


#include "src/adapter.h"
#include "src/device.h"
#include "src/profile.h"
#include "src/plugin.h"

#include "lib/uuid.h"
#include "attrib/gattrib.h"
#include "attrib/att.h"
#include "attrib/gatt.h"
#include "attrib/att-database.h"
#include "src/shared/util.h"
#include "src/attrib-server.h"
#include "attrib/gatt-service.h"
#include "src/log.h"

#define RECV_TIMEOUT 60
#define MAX_BUF_LEN  4096

#define BTCONF_FETCH_SVC_UUID         0xFF01
#define BTCONF_CHR_BTCFG_UUID         0x2A0D//0x2A0D

#define DEFAULT_UNIX_SOCKET_NAME "/tmp/bleconfig_cmd_listener"

#define unit 3
#define max_unit 64


//#define DEBUG_MSG


typedef struct {
    struct attribute *m_attribute;
	struct btd_device *m_btd_device;
	gpointer user_data;
} gatt_info_data;

typedef struct _SOCKETQ socketq_t;
struct _SOCKETQ {
	unsigned int req;
	unsigned int count;
    unsigned char bssid[6];
    char ssid[33];
    char pwd[65];
    char pin[64];
};

pthread_mutex_t GlobalData=PTHREAD_MUTEX_INITIALIZER;
uint8_t value[MAX_BUF_LEN]={0};
unsigned int valueLen = 0;

uint8_t sendBuf[4096]={0};
unsigned int sendBufLen = 0;
unsigned short ss_sectionNum = 0;


int connectToListener(char *listener_name)
{
	int ret = -1;
    int s, len;
    struct sockaddr_un remote;
    fd_set fdset;
    struct timeval tv;
    char socketName[256] = {0};
int max_retry=0;
    if (listener_name)
        strncpy(socketName, listener_name, sizeof(socketName));
    else
        strncpy(socketName, DEFAULT_UNIX_SOCKET_NAME, sizeof(socketName));

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        printf("socket: %s", strerror(errno));
        return (s);
    }

    //printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, socketName);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    
    int optval = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
    
    long mode = fcntl(s, F_GETFL, NULL);
    fcntl(s, F_SETFL, mode | O_NONBLOCK);
ReTry:	
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        if (errno != EINPROGRESS) {
            //printf("%s connect: %d (%s)\n", __FUNCTION__, errno, strerror(errno));
	if(errno  == ECONNREFUSED && max_retry < 10){
		max_retry++;
		printf("%s connect: %d (%s), ReTry.......\n", __FUNCTION__, errno, strerror(errno));
		sleep(1);
		goto ReTry;
	}else{
            close(s);
            s = -1;
		}
        } else {
            FD_ZERO(&fdset);
            FD_SET(s, &fdset);
            tv.tv_sec = 15;
            tv.tv_usec = 0;
            ret = select(s+1, NULL, &fdset, NULL, &tv);
            if (ret && FD_ISSET(s, &fdset)) {
                int so_error;
                socklen_t len = sizeof(so_error);
                getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &len);
                if (so_error != 0) {
                    fprintf(stderr, "%s - error in connection with %s\n", __func__, socketName);
                    close(s);
                    s = -1;
                }
            }
            else {
                fprintf(stderr, "%s - connect timeout with %s\n", __func__, socketName);
                close(s);
                s = -1;
            }
        }
    }
    


    return (s);
}

int send_wifi_profile_cmd(const int sockfd, socketq_t *req)
{
    int n;
    if ((n = send(sockfd, (char*)req, sizeof(socketq_t), 0)) == -1) {
        fprintf(stderr, "send failed: %d(%s)\n", errno, strerror(errno));
        return -1;
    }
    return n;
}

int send_req_cmd(const int sockfd, unsigned char *cmd,unsigned int cmdLen)
{
    int n = 0;
    if ((n = send(sockfd, (char*)cmd, cmdLen, 0)) == -1) {
        fprintf(stderr, "send_req_cmd failed: %d(%s)\n", errno, strerror(errno));
        return -1;
    }
    return n;
}

int recv_req_cmd(const int sockfd, unsigned char *buf)
{
    unsigned int bufLen = 0;

    //long mode = fcntl(sockfd, F_GETFL, NULL);
    //mode &= (~O_NONBLOCK);
    //fcntl(sockfd, F_SETFL, mode);
    if( bufLen = recv(sockfd,buf,MAX_BUF_LEN,0) < 0 ){
        printf("recv_req_cmd recv FAIL\n");
        return -1;
    }
    return bufLen;
}

void disconnectListener(int Id)
{
    if (Id > 0)
        close(Id);
}

int  _ServiceRegisterconnectToListener(void)
{
    int s, len;
    struct sockaddr_un remote;
    char socketName[128] = {0};

    strncpy(socketName, DEFAULT_UNIX_SOCKET_NAME, sizeof(socketName));

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        printf("%s socket FAIL: %s", __FUNCTION__, strerror(errno));
        return (s);
    }

    //printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, socketName);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        printf("%s connect FAIL: %s\n", __FUNCTION__, strerror(errno));
        close(s);
        s = -1;
        return (s);
    }

    //printf("Connected. (id = %d)\n", s);
    return (s);
}

void _ServiceRegisterdisconnectListener(int Id)
{
    if (Id > 0)
        close(Id);
}

int _ServiceSendTo(int sockfd, void *register_str, int str_len)
{
    int ret = send(sockfd, (char*)register_str, str_len, 0);
    if (ret <= 0) {
        fprintf(stderr, "%s - send failure %d (%s)\n", __func__, errno, strerror(errno));
    }
    return ret;
}

void _ServiceRecv(int sockfd, void *register_str, int *str_len, int timeout)
{
    int ret = -1;
    int fd_max = sockfd;
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    FD_SET(0,&rfds);

    tv.tv_sec = timeout;
    tv.tv_usec = 5;

    ret = select(fd_max+1, &rfds, NULL, NULL, &tv);
    if(ret==-1){
        fprintf(stderr, "%s - select failed: %d (%s)\n", __func__, errno, strerror(errno));
        return;
    }else if(ret == 0){
        fprintf(stderr, "%s - select timeout\n", __func__);
        return;
    }

    *str_len = recv(sockfd, register_str, MAX_BUF_LEN, 0);
    //printf("register_str:%s\n",(char*)register_str);
    if (*str_len < 0) {
        fprintf(stderr, "%s - recv failure %d (%s)\n", __func__, errno, strerror(errno));
    }
}


static int handle_request_btcfg_cmd(unsigned char *cmd,unsigned int cmdLen,unsigned char *resultBuf,unsigned int *resultBufLen)
{
    int ret = -1;
    int bufLen = 0;
    int socketId;

    socketId = _ServiceRegisterconnectToListener();
    if (socketId == -1) {
        printf("%s connect failed: %d(%s)\n", __FUNCTION__, errno, strerror(errno));
        return -1;
    }
    if(_ServiceSendTo(socketId, cmd, cmdLen) > 0){
        _ServiceRecv(socketId, resultBuf, &bufLen, RECV_TIMEOUT);
        if(bufLen>0){
            ret = 1;
            *resultBufLen = bufLen;
        }
    }


    _ServiceRegisterdisconnectListener(socketId);

    return ret;
}

static int notify_bleconfig(char *bssid,char *ssid, char *pwd, char *pin)
{
    int ret = 1;
    int socketId;
    int str_index = 0;
    char* pToken = NULL;
    char* pSave = NULL;
    char* pDelimiter = ":";
    socketq_t req;

    memset(&req,0,sizeof(req));

    //creat unix socket 
    socketId =  connectToListener(DEFAULT_UNIX_SOCKET_NAME);
    if (socketId == -1) {
        printf("%s connect failed: %d(%s)\n", __FUNCTION__, errno, strerror(errno));
        return -1;
    }

    req.req = 11;
    pToken = strtok_r(bssid, pDelimiter, &pSave);
    while(pToken){

        sscanf(pToken,"%02X",&req.bssid[str_index]);
        str_index++;
        pToken = strtok_r(NULL, pDelimiter, &pSave);
    }

    sprintf(req.ssid,"%s",ssid);
    sprintf(req.pwd,"%s",pwd);
    sprintf(req.pin,"%s",pin);

    if ((ret = send_wifi_profile_cmd(socketId, &req)) < 0){
        return -2;
    }

    disconnectListener(socketId);
    
    return 1;
}

void reset_sendBuf()
{
    pthread_mutex_lock(&GlobalData);
    memset(sendBuf,0,sizeof(sendBuf));
    //sendBufLen = 1;
    //sendBuf[0] = 0xff;//TAG_BTCMD_REPLY_FAIL
    sendBufLen = 0;
    pthread_mutex_unlock(&GlobalData);
}

static uint8_t btconf_read_handle_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{

    int max_section = 0;
    
    struct btd_adapter *adapter = user_data;
    uint8_t m_sendBuf[MAX_BUF_LEN]={0};
    unsigned int m_sendBufLen = 0;


    pthread_mutex_lock(&GlobalData);
    if(sendBuf[0]==0X00 && sendBuf[1]==0X02 ){
        //length is 2633 bytes
        //site survey cmd due to received max length(628bytes) with iOS APP, so there are 8 section (  337bytes/1 section) by 0x2A0D
        //But it send 337 byte, bluez can send three time, so we modify 20 section, 132byte/1section
        if(ss_sectionNum==0){
            ss_sectionNum = 1;
        }else{
            ss_sectionNum++;
        }
        max_section = max_unit/unit;
        if(max_unit%unit!=0)max_section++;


        m_sendBufLen = 2+2+1+4+(41*unit);
        memcpy(m_sendBuf,sendBuf,5);

        m_sendBuf[6] = ss_sectionNum*unit;
        memcpy(m_sendBuf+7,sendBuf+7,2);

        if(ss_sectionNum==max_section){//reset
            memcpy(m_sendBuf+9,sendBuf+9+((ss_sectionNum-1)*41*unit),(41*(max_unit%unit)));
            ss_sectionNum = 0;
        }else{
            memcpy(m_sendBuf+9,sendBuf+9+((ss_sectionNum-1)*41*unit),(41*unit));
        }

        
#if DEBUG_MSG
        printf("####### ss_sectionNum(%d)\n",ss_sectionNum);
#endif        
        
    }else{
        m_sendBufLen = sendBufLen;
        memcpy(m_sendBuf,sendBuf,m_sendBufLen);
    }
    pthread_mutex_unlock(&GlobalData);

#if DEBUG_MSG   
    int i=0;
                        printf("GATT_btconf_read_band_capability_cmd=====>(%d)\n",m_sendBufLen);

                        if(m_sendBufLen>64){
                            m_sendBufLen = 132;
                        //if(m_sendBufLen<64){
                        for(i=0;i<m_sendBufLen;i++){
                            if(i>0 && i%10==0){
                                printf("\n");
                            }
                            printf("%02X ",m_sendBuf[i]);
                        }
                        printf("\n");
                        }
#endif

    attrib_db_update(adapter, a->handle, NULL, m_sendBuf, m_sendBufLen, NULL);

    return 0;
}

static uint8_t btconf_write_handle_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
  
    
    int i=0;
    int ret = 0;
    unsigned char data[128] = {0};
    unsigned char resultBuf[4096] = {0};
    unsigned int cmdLen = 0;
    unsigned int resultBufLen = 0;
    reset_sendBuf();
    
    if(a->len>0){
        memcpy(data,a->data,a->len);
#if DEBUG_MSG          
        printf("GATT_write_band_capability_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",a->len);
        for(i=0;i<a->len;i++){
            if(i>0 && i%10==0)printf("\n");
            printf("%02X ",data[i]);
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
#endif

        cmdLen = a->len;
        ret = handle_request_btcfg_cmd(data,cmdLen,resultBuf,&resultBufLen);
        if(ret>0){
#if DEBUG_MSG            
                        printf("handle_request_btcfg_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",resultBufLen);
                        if(resultBufLen<128){
                        for(i=0;i<resultBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%X ",resultBuf[i]);
                        }
                        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                        }
#endif                        
        }
        if(resultBufLen>0){
            pthread_mutex_lock(&GlobalData);
            memset(sendBuf,0,sizeof(sendBuf));
            sendBufLen = resultBufLen;
            memcpy(sendBuf,resultBuf,resultBufLen);
            pthread_mutex_unlock(&GlobalData);
        }
    }

    return 0;
}

#if 0
static uint8_t btconf_read_sitesurvey_2_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    struct btd_adapter *adapter = user_data;
    uint8_t m_sendBuf[MAX_BUF_LEN]={0};
    unsigned int m_sendBufLen = 0;

    pthread_mutex_lock(&GlobalData);
    m_sendBufLen = sendBufLen;
    memcpy(m_sendBuf,sendBuf,m_sendBufLen);
    pthread_mutex_unlock(&GlobalData);

    int i=0;
    printf("btconf_read_sitesurvey_2_cmd=====>\n");
                        for(i=0;i<m_sendBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%X ",m_sendBuf[i]);
                        }
                        printf("\n");
    m_sendBufLen = 9 + 10*410;
    attrib_db_update(adapter, a->handle, NULL, m_sendBuf, m_sendBufLen, NULL);

    //if(m_sendBufLen>1){
    //    reset_sendBuf();
    //}
    
    return 0;
}

static uint8_t btconf_write_sitesurvey_2_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    int i=0;
    int ret = 0;
    unsigned char data[32] = {0};
    unsigned char resultBuf[MAX_BUF_LEN] = {0};
    unsigned int cmdLen = 0;
    unsigned int resultBufLen = 0;
    reset_sendBuf();

    
    if(a->len>0){
        memcpy(data,a->data,a->len);
        printf("btconf_write_band_capability_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",a->len);
        for(i=0;i<a->len;i++){
            if(i>0 && i%10==0)printf("\n");
            else printf("%X ",data[i]);
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


        cmdLen = a->len;
        ret = handle_request_btcfg_cmd(data,cmdLen,resultBuf,&resultBufLen);
        if(ret>0){
            printf("handle_request_btcfg_cmd ok\n");
                        printf("handle_request_btcfg_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",resultBufLen);
                        //for(i=0;i<resultBufLen;i++){
                        //    if(i>0 && i%10==0)printf("\n");
                        //    else printf("%X ",resultBuf[i]);
                        //}
                        //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        }
        if(resultBufLen>0){
            pthread_mutex_lock(&GlobalData);
            memset(sendBuf,0,sizeof(sendBuf));
            memcpy(sendBuf,resultBuf,resultBufLen);
            sendBufLen = resultBufLen;
            pthread_mutex_unlock(&GlobalData);
        }
    }

    return 0;
}

static uint8_t btconf_read_sitesurvey_5_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    struct btd_adapter *adapter = user_data;
    uint8_t m_sendBuf[MAX_BUF_LEN]={0};
    unsigned int m_sendBufLen = 0;

    pthread_mutex_lock(&GlobalData);
    m_sendBufLen = sendBufLen;
    memcpy(m_sendBuf,sendBuf,m_sendBufLen);
    pthread_mutex_unlock(&GlobalData);

    int i=0;
    printf("btconf_read_sitesurvey_5_cmd=====>\n");
                        for(i=0;i<m_sendBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%X ",m_sendBuf[i]);
                        }
                        printf("\n");
    
    attrib_db_update(adapter, a->handle, NULL, m_sendBuf, m_sendBufLen, NULL);

    if(m_sendBufLen>1){
        reset_sendBuf();
    }
    
    return 0;
}

static uint8_t btconf_write_sitesurvey_5_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    int i=0;
    int ret = 0;
    unsigned char data[32] = {0};
    unsigned char resultBuf[512] = {0};
    unsigned int cmdLen = 0;
    unsigned int resultBufLen = 0;
    reset_sendBuf();

    if(a->len>0){
        memcpy(data,a->data,a->len);
        printf("btconf_write_band_capability_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",a->len);
        for(i=0;i<a->len;i++){
            if(i>0 && i%10==0)printf("\n");
            else printf("%X ",data[i]);
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


        cmdLen = a->len;
        ret = handle_request_btcfg_cmd(data,cmdLen,resultBuf,&resultBufLen);
        if(ret>0){
            printf("handle_request_btcfg_cmd ok\n");
                        printf("handle_request_btcfg_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",resultBufLen);
                        for(i=0;i<resultBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%02X ",resultBuf[i]);
                        }
                        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        }
        if(resultBufLen>0){
            pthread_mutex_lock(&GlobalData);
            memset(sendBuf,0,sizeof(sendBuf));
            memcpy(sendBuf,resultBuf,resultBufLen);
            sendBufLen = resultBufLen;
            pthread_mutex_unlock(&GlobalData);
        }
    }

    return 0;
}


static uint8_t btconf_read_connect_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    struct btd_adapter *adapter = user_data;
    uint8_t m_sendBuf[MAX_BUF_LEN]={0};
    unsigned int m_sendBufLen = 0;

    pthread_mutex_lock(&GlobalData);
    if(sendBuf[0]==0X00 && sendBuf[1]==0X06 ){//site survey cmd due to received max length(628bytes) with iOS APP, so there are 8 section (  337bytes/1section)
        if(ss_sectionNum==0){
            ss_sectionNum = 1;
        }else{
            ss_sectionNum++;
        }

        m_sendBufLen = 2+2+1+4+(41*8);
        memcpy(m_sendBuf,sendBuf,5);
        m_sendBuf[6] = ss_sectionNum*8;
        memcpy(m_sendBuf+7,sendBuf+7,2);
        memcpy(m_sendBuf+9,sendBuf+9+((ss_sectionNum-1)*41*8),(41*8));

        if(ss_sectionNum==8){//reset
            ss_sectionNum = 0;
        }
    }else{
        m_sendBufLen = sendBufLen;
        memcpy(m_sendBuf,sendBuf,m_sendBufLen);
    }
    pthread_mutex_unlock(&GlobalData);

    int i=0;
    printf("btconf_read_connect_cmd=====>(%d)\n",m_sendBufLen);
                        for(i=0;i<m_sendBufLen;i++){
                            if(i>0 && i%10==0)printf("\n"); 
                            printf("%02X ",m_sendBuf[i]);
                        }
                        printf("\n");
    
    attrib_db_update(adapter, a->handle, NULL, m_sendBuf, m_sendBufLen, NULL);
    
    return 0;
}

static uint8_t btconf_write_connect_cmd(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    int i=0;
    int ret = 0;
    unsigned char data[32] = {0};
    unsigned char resultBuf[MAX_BUF_LEN] = {0};
    unsigned int cmdLen = 0;
    unsigned int resultBufLen = 0;
    reset_sendBuf();

    if(a->len>0){
        memcpy(data,a->data,a->len);
        printf("btconf_write_connect_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",a->len);
        for(i=0;i<a->len;i++){
            if(i>0 && i%10==0)printf("\n");
            else printf("%02X ",data[i]);
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


        cmdLen = a->len;
        ret = handle_request_btcfg_cmd(data,cmdLen,resultBuf,&resultBufLen);
        if(ret>0){
            /*
            printf("btconf_write_connect_cmd ok\n");
                        printf("handle_request_btcfg_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",resultBufLen);
                        for(i=0;i<resultBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%X ",resultBuf[i]);
                        }
                        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                        */
        }
        if(resultBufLen>0){
            pthread_mutex_lock(&GlobalData);
            memset(sendBuf,0,sizeof(sendBuf));
            memcpy(sendBuf,resultBuf,resultBufLen);
            sendBufLen = resultBufLen;
            pthread_mutex_unlock(&GlobalData);
        }
    }

    return 0;
}

static uint8_t btconf_read_status(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    struct btd_adapter *adapter = user_data;
    uint8_t m_sendBuf[512]={0};
    unsigned int m_sendBufLen = 0;

    pthread_mutex_lock(&GlobalData);
    m_sendBufLen = sendBufLen;
    memcpy(m_sendBuf,sendBuf,m_sendBufLen);
    pthread_mutex_unlock(&GlobalData);

    int i=0;
    printf("btconf_read_connect_cmd=====>(%d)\n",m_sendBufLen);
                        for(i=0;i<m_sendBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%02X ",m_sendBuf[i]);
                        }
                        printf("\n");
    
    attrib_db_update(adapter, a->handle, NULL, m_sendBuf, m_sendBufLen, NULL);
    
    return 0;
}

static uint8_t btconf_write_status(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    printf("<<##%s(%d)##>>\n",__FUNCTION__,__LINE__);
    int i=0;
    int ret = 0;
    unsigned char data[32] = {0};
    unsigned char resultBuf[512] = {0};
    unsigned int cmdLen = 0;
    unsigned int resultBufLen = 0;
    reset_sendBuf();

    if(a->len>0){
        memcpy(data,a->data,a->len);
        printf("btconf_write_status recv:(%d)>>>>>>>>>>>>>>>>>>\n",a->len);
        for(i=0;i<a->len;i++){
            if(i>0 && i%10==0)printf("\n");
            else printf("%02X ",data[i]);
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");


        cmdLen = a->len;
        ret = handle_request_btcfg_cmd(data,cmdLen,resultBuf,&resultBufLen);
        if(ret>0){
            /*
            printf("handle_request_btcfg_cmd ok\n");
                        printf("handle_request_btcfg_cmd recv:(%d)>>>>>>>>>>>>>>>>>>\n",resultBufLen);
                        for(i=0;i<resultBufLen;i++){
                            if(i>0 && i%10==0)printf("\n");
                            else printf("%X ",resultBuf[i]);
                        }
                        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                        */
        }
        if(resultBufLen>0){
            pthread_mutex_lock(&GlobalData);
            memset(sendBuf,0,sizeof(sendBuf));
            memcpy(sendBuf,resultBuf,resultBufLen);
            sendBufLen = resultBufLen;
            pthread_mutex_unlock(&GlobalData);
        }
    }
    printf("<<##%s(%d)##>> Done\n",__FUNCTION__,__LINE__);
    return 0;
}


#endif

static uint8_t btconf_ssid_psk_read(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
	struct btd_adapter *adapter = user_data;

    if(!strcmp(value,"")){
        sprintf(value,"%s","rtk_btconfig_ssid_psw");
        valueLen = 21;
    }else{
        value[valueLen] = '1';
        valueLen++;
    }

    //printf("w:%s\n",value);
	attrib_db_update(adapter, a->handle, NULL, value, sizeof(value), NULL);

	return 0;
}


static uint8_t btconf_ssid_psk_update(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    int ret = -1;
    int str_index = 0;
    char* pToken = NULL;
    char* pSave = NULL;
    char* pDelimiter = ";";
    char bssid[32] = {0};
    char ssid[33] = {0};
    char pwd[65] = {0};
    char pin[65] = {0};
    char tmp[128] = {0};
    char wifi_profile[256] = {0};

    

    
    if(a->len<=255){
        memcpy(wifi_profile,a->data,a->len);
        //DBG("handle 0x%04x, wifi_profile: %s (%d)", a->handle, wifi_profile,a->len);

        pToken = strtok_r(wifi_profile, pDelimiter, &pSave);
        while(pToken){
            //printf("   pToken[%s]; pSave[%s]\n",pToken,pSave);
            if(str_index==0){
                 sprintf(bssid,"%s",pToken);
                 bssid[strlen(bssid)] = '\0';
            }else if(str_index==1){
                sprintf(ssid, "%s",pToken);
                ssid[strlen(ssid)] = '\0';
            }else if(str_index==2){
                sprintf(pwd, "%s",pToken); 
                pwd[strlen(pwd)] = '\0';
                if( strcmp(pwd,"________")==0 )memset(pwd,0,sizeof(pwd));
            }else if(str_index==3){
                sprintf(pin, "%s",pToken);
                pwd[strlen(pwd)] = '\0';
            }
            str_index++;
            pToken = strtok_r(NULL, pDelimiter, &pSave);
        }

        //printf("\n===============================================\n");
        //printf("   BSSID:%s\n",bssid);
        //printf("    SSID:%s\n",ssid);
        //printf("SECURITY:%s\n",pwd);
        //printf("     PIN:%s\n",pin);
        //printf("===============================================\n\n");

        ret = notify_bleconfig(bssid,ssid,pwd,pin);
        if(ret<0){
            printf("< notify_bleconfig fail >\n");
        }

        
    }else{
        DBG("password is over max length! (128)");
    }
    

    //printf("### %s:%d] Done\n", __FUNCTION__, __LINE__);
	return 0;
}

//_fromCentral
static uint8_t request_wifiState(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
    return 0;
}

static uint8_t response_wifiState(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{

    int i=0,maxRetry=10;
    int ret=-1;
	struct btd_adapter *adapter = user_data;
	uint8_t value[32]="";

    sprintf(value,"response_wifiState:%d\n",i);
    attrib_db_update(adapter, a->handle, NULL, value, sizeof(value), NULL);
    return 0;
}

static gboolean register_btconf_service(struct btd_adapter *adapter)
{
	bt_uuid_t uuid;

	bt_uuid16_create(&uuid, BTCONF_FETCH_SVC_UUID);

	/* Current Time service */
	return gatt_service_add(adapter, GATT_PRIM_SVC_UUID, &uuid,

                GATT_OPT_CHR_UUID16, BTCONF_CHR_BTCFG_UUID,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,
#if 0
    GATT_OPT_CHR_UUID16, 0x2A00,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 0x2A01,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 0x2A08,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 	0x2A09,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 	0x2A0A,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 0x2A0C,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 	0x2A14,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 0x2A24,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,

    GATT_OPT_CHR_UUID16, 0x2A25,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ | GATT_CHR_PROP_WRITE,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						btconf_read_handle_cmd, adapter,
                GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
                        btconf_write_handle_cmd, adapter,
#endif
				GATT_OPT_INVALID);
}

static int btconf_param_probe(struct btd_service *service)
{
    return 0;
}

static int btconf_param_remove(struct btd_service *service)
{
    return 0;
}

static int btconf_server_init(struct btd_profile *p, struct btd_adapter *adapter)
{
	const char *path = adapter_get_path(adapter);

	//DBG("path %s", path);

	if (!register_btconf_service(adapter)) {
		error("Btconfig Service could not be registered");
		return -EIO;
	}

	return 0;
}

static void btconf_server_exit(struct btd_profile *p,
						struct btd_adapter *adapter)
{
    int ret=-1;
	const char *path = adapter_get_path(adapter);

	//DBG("path %s", path);

    /*if(g_gatt_info_data){
        free(g_gatt_info_data);
        g_gatt_info_data = NULL;
    }

    ret = pthread_join(g_controlThread, NULL);
	if(ret!=0){
		printf("controlThread join failed!!\n");
		return -12;
	}*/
}

struct btd_profile btconf_profile = {
	.name		= "gatt-btconfig-server",
	.adapter_probe	= btconf_server_init,
	.adapter_remove	= btconf_server_exit,
};

static int btconf_init(void)
{
	return btd_profile_register(&btconf_profile);
}

static void btconf_exit(void)
{
	btd_profile_unregister(&btconf_profile);
}

BLUETOOTH_PLUGIN_DEFINE(btconfig, VERSION,
			BLUETOOTH_PLUGIN_PRIORITY_DEFAULT,
			btconf_init, btconf_exit)
