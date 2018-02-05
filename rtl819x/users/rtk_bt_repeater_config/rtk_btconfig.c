//#include "rtk_btconfig.h"
#include <rtk_btconfig_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string.h>
#include <signal.h>
#include <apmib.h>

static int client_num = 0;
int expire_time = MAX_EXPIRE_TIME;
int clientsk = -1;
static int signals_inited = 0;

extern rtk_btconfig_API btconfig_api_table[];


static void sig_term_handler(int signo)
{
	RTK_BTCONFIG_DEBUG("Got SIGTERM !!!\n");
	if(clientsk > 0)
	{
		RTK_BTCONFIG_DEBUG("Child, close socket...\n");
		close(clientsk);
		clientsk = -1;
	}
}
static void sig_alarm_handler(int signo)
{
//	RTK_BTCONFIG_DEBUG("expire_time:%d\n",expire_time);
	if(expire_time > 0)
	{
		expire_time -= 1;
	}
	else
	{
		RTK_BTCONFIG_DEBUG("Client timeout!!\n");
		close(clientsk);
		exit(0);
	}
	alarm(1);
}

static void sig_child_handler(int sig_no)
{       
	pid_t pid; 
	int  stat; 
	while((pid = waitpid(-1, &stat, WNOHANG)) > 0){ 
	       RTK_BTCONFIG_DEBUG("child %d terminated\n", pid); 
		   client_num --;
		   RTK_BTCONFIG_DEBUG("current client num:%d\n",client_num);
	} 
	return;
}
int rtk_btconfig_msg_handler(int sk)
{
	unsigned char recv_buf[512] = {0};
	unsigned char result_buf[4096] = {0};
	int bytes_read;
	unsigned char* pData;
	int i,resultBufLen,ret;
	if(sk < 0)
	{
		return -1;
	}
	while(1)
	{
		memset(recv_buf,0,sizeof(recv_buf));
		bytes_read = read(sk,recv_buf,sizeof(recv_buf));
		if(bytes_read > 0)
		{
			if(expire_time < MAX_EXPIRE_TIME)
			{
				expire_time = MAX_EXPIRE_TIME;
			}
#ifdef RTK_DEBUG
			rtk_btconfig_dump_buf(recv_buf,bytes_read);
#endif
			unsigned short tag = rtk_btconfig_get_cmd_tag(recv_buf,bytes_read);
			RTK_BTCONFIG_DEBUG("cmd tag:%d\n",tag);
			i = 0;
			while(btconfig_api_table[i].cmd_id != TAG_BT_CMD_INVALID)
			{
				if(btconfig_api_table[i].cmd_id == tag)
				{
					if(TAG_BT_CMD_CONNECT == tag)
					{
						RTK_BTCONFIG_DEBUG("reply connect...........\n");
						rtk_btconfig_cmd_reply(sk,TAG_BT_CMD_REPLY_CONNECT,0);
					}
					memset(result_buf,0,sizeof(result_buf));
					resultBufLen = 0;
					ret = btconfig_api_table[i].cmd_function(recv_buf,bytes_read,result_buf,&resultBufLen);
					if(ret == RTK_BTCONFIG_RET_SUCCESS)
					{
						rtk_btconfig_send_msg(sk,result_buf,resultBufLen);
					}
					else
					{
						rtk_btconfig_cmd_reply(sk,tag+1,1);
					}
					break;
				}
				i ++;
			}
		}
		else
		{
			RTK_BTCONFIG_DEBUG("client closed!\n");
			close(sk);
			exit(0);
		}
	}
}

int main (int argc,char **argv)
{
	struct sockaddr_rc loc_addr ={0},rem_addr={0};
	int sk,nsk,result;
	int opt = sizeof(rem_addr);
	int status;
	char client_bdaddr[128] = {0};
	char recv_buf[512] = {0};
	int pid = -1;
	int bytes_read = 0;
    struct sigaction sa;
	
	RTK_BTCONFIG_DEBUG("Init apmib...\n");
	apmib_init();
	RTK_BTCONFIG_DEBUG("Creating socket...\n");
	sk =socket(PF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
	if(sk<0)
	{
		perror("create socket error");
	   	exit(1);
	}
	else
	{
		RTK_BTCONFIG_DEBUG("success!\n");
	}
	loc_addr.rc_family=AF_BLUETOOTH;
	loc_addr.rc_bdaddr=*BDADDR_ANY;
	loc_addr.rc_channel=(uint8_t)1;

	RTK_BTCONFIG_DEBUG("Binding socket...\n");
	result=bind(sk,(struct sockaddr *)&loc_addr, sizeof(loc_addr));
	if(result<0)
	{
		perror("bind socket error:");
		close(sk);
	  	exit(1);
	}

	RTK_BTCONFIG_DEBUG("Listen... \n");
	result=listen(sk,MAX_NUM_OF_BT_CLIENTS);
	if(result<0)
	{
	   RTK_BTCONFIG_DEBUG("listen error:%d\n:",result);
	   perror("listen error:");
	   close(sk);
	   exit(1);
	}
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_term_handler;
	sigaction(SIGTERM, &sa, NULL);
	RTK_BTCONFIG_DEBUG("Waiting for connection...\n");
	rtk_btconfig_check_repeater_status();
	while(1)
	{
		nsk = accept(sk,(struct sockaddr *)&rem_addr,&opt);
		if(nsk<0)
		{
			perror("accept error");
			close(sk);
			exit(1);
		}
		ba2str(&rem_addr.rc_bdaddr,client_bdaddr);
		fprintf(stderr,"accepted connection from %s \n",client_bdaddr);
		if(client_num+1 > MAX_NUM_OF_BT_CLIENTS)
		{
			RTK_BTCONFIG_ERR("Max number of clients exceeded,refuse!\n");
			close(nsk);
			continue;
		}
		pid = fork();
		if(pid == -1)
		{
			perror("fork!\n");
			close(nsk);
			close(sk);
			exit(1);
		}
		else if(pid == 0)/*child*/
		{
			close(sk);
			clientsk = nsk;
			RTK_BTCONFIG_DEBUG("Init child signals.....\n");
			signal(SIGALRM,sig_alarm_handler);
			alarm(1);
			set_connect_status(0);
			rtk_btconfig_msg_handler(nsk);
		}
		else/*parent*/
		{
			client_num ++;
			RTK_BTCONFIG_DEBUG("current client num:%d\n",client_num);
			if(signals_inited == 0)
			{
				RTK_BTCONFIG_DEBUG("Init signals.....\n");
				signal(SIGCHLD,sig_child_handler);			
				signals_inited = 1;
			}
			close(nsk);
		}
	}
error:
	close(sk);
	return 0 ;
}

