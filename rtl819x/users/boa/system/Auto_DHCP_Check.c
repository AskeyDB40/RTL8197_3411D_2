#include "rtk_eventd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>    
#include <sys/stat.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/socket.h>
//#include <net/if.h>
#include <sys/times.h>
#include <sys/select.h>
#include <linux/wireless.h>

#include "apmib.h"
#include "mibtbl.h"
#include <sysconf.h>
#include "sys_utility.h"
#include <time.h> 


#define RTL8651_IOCTL_GETWANLINKSTATUS 2000
#define DHCPD_CONF_FILE "/var/udhcpd.conf"
#define Rtk_Event_Netlink_Group  (1<<0)

static struct sockaddr_nl src_addr, dest_addr;
static struct nlmsghdr *nlh = NULL;
static struct iovec iov;
static int sock_fd=0;
static struct msghdr msg;
/*
*
*Data structures
*
*/
#define SIOCGIWIND      			0x89ff  // RTL8192CD_IOCTL_USER_DAEMON_REQUEST
typedef struct _DOT11_DISCONNECT_REQ{
        unsigned char   EventId;
        unsigned char   IsMoreEvent;
        unsigned short  Reason;
        char            MACAddr[6];
}DOT11_DISCONNECT_REQ;
typedef enum{
    DOT11_EVENT_NO_EVENT = 1,
    DOT11_EVENT_REQUEST = 2,
    DOT11_EVENT_ASSOCIATION_IND = 3,
    DOT11_EVENT_ASSOCIATION_RSP = 4,
    DOT11_EVENT_AUTHENTICATION_IND = 5,
    DOT11_EVENT_REAUTHENTICATION_IND = 6,
    DOT11_EVENT_DEAUTHENTICATION_IND = 7,
    DOT11_EVENT_DISASSOCIATION_IND = 8,
    DOT11_EVENT_DISCONNECT_REQ = 9,
    DOT11_EVENT_SET_802DOT11 = 10,
    DOT11_EVENT_SET_KEY = 11,
    DOT11_EVENT_SET_PORT = 12,
    DOT11_EVENT_DELETE_KEY = 13,
    DOT11_EVENT_SET_RSNIE = 14,
    DOT11_EVENT_GKEY_TSC = 15,
    DOT11_EVENT_MIC_FAILURE = 16,
    DOT11_EVENT_ASSOCIATION_INFO = 17,
    DOT11_EVENT_INIT_QUEUE = 18,
    DOT11_EVENT_EAPOLSTART = 19,

    DOT11_EVENT_ACC_SET_EXPIREDTIME = 31,
    DOT11_EVENT_ACC_QUERY_STATS = 32,
    DOT11_EVENT_ACC_QUERY_STATS_ALL = 33,
    DOT11_EVENT_REASSOCIATION_IND = 34,
    DOT11_EVENT_REASSOCIATION_RSP = 35,
    DOT11_EVENT_STA_QUERY_BSSID = 36,
    DOT11_EVENT_STA_QUERY_SSID = 37,
    DOT11_EVENT_EAP_PACKET = 41,

#ifdef RTL_WPA2_PREAUTH
    DOT11_EVENT_EAPOLSTART_PREAUTH = 45,
    DOT11_EVENT_EAP_PACKET_PREAUTH = 46,
#endif

#ifdef RTL_WPA2_CLIENT
DOT11_EVENT_WPA2_MULTICAST_CIPHER = 47,
#endif

DOT11_EVENT_WPA_MULTICAST_CIPHER = 48,

#ifdef AUTO_CONFIG
	DOT11_EVENT_AUTOCONF_ASSOCIATION_IND = 50,
	DOT11_EVENT_AUTOCONF_ASSOCIATION_CONFIRM = 51,
	DOT11_EVENT_AUTOCONF_PACKET = 52,
	DOT11_EVENT_AUTOCONF_LINK_IND = 53,
#endif

#ifdef WIFI_SIMPLE_CONFIG
	DOT11_EVENT_WSC_SET_IE = 55,
	DOT11_EVENT_WSC_PROBE_REQ_IND = 56,
	DOT11_EVENT_WSC_PIN_IND = 57,
	DOT11_EVENT_WSC_ASSOC_REQ_IE_IND = 58,
DOT11_EVENT_WSC_START_IND = 70,
DOT11_EVENT_WSC_MODE_IND = 71,
DOT11_EVENT_WSC_STATUS_IND = 72,
DOT11_EVENT_WSC_METHOD_IND = 73,
DOT11_EVENT_WSC_STEP_IND = 74,
DOT11_EVENT_WSC_OOB_IND = 75,
#endif
DOT11_EVENT_WSC_PBC_IND = 76,
// for WPS2DOTX
DOT11_EVENT_WSC_SWITCH_MODE = 100,	// for P2P P2P_SUPPORT
DOT11_EVENT_WSC_STOP = 101	,
DOT11_EVENT_WSC_SET_MY_PIN = 102,		// for WPS2DOTX
DOT11_EVENT_WSC_SPEC_SSID = 103,
DOT11_EVENT_WSC_SPEC_MAC_IND = 104,
DOT11_EVENT_WSC_CHANGE_MODE = 105,	
DOT11_EVENT_WSC_RM_PBC_STA = 106,
DOT11_EVENT_WSC_CHANGE_MAC_IND=107,	
DOT11_EVENT_WSC_SWITCH_WLAN_MODE=108	
} DOT11_EVENT;

/*
*
*Data structures end
*
*/

int pidfile_acquire(char *pidfile)
{
	int pid_fd;
	if (pidfile == NULL) 
	{
		return -1;
	}
	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0) 
	{
		printf("Unable to open pidfile %s\n",pidfile);
	}
	else 
	{
		lockf(pid_fd, F_LOCK, 0);
	}
	return pid_fd;
}

void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if (pid_fd < 0) 
		return;

	if ((out = fdopen(pid_fd, "w")) != NULL) 
	{
		fprintf(out, "%d\n", getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}
int IssueDisconnect(unsigned char *pucMacAddr, char* wlan_if,unsigned short reason)
{
    int skfd;
    int retVal = 0;
    struct iwreq wrq;
    DOT11_DISCONNECT_REQ Disconnect_Req;
	if(pucMacAddr == NULL || wlan_if == NULL)
	{
		return -1;
	}
    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (skfd < 0) {
        printf("socket() error!\n");
        return -1;
    }


    Disconnect_Req.EventId = DOT11_EVENT_DISCONNECT_REQ;

    Disconnect_Req.IsMoreEvent = 0;
    Disconnect_Req.Reason = reason;
    memcpy(Disconnect_Req.MACAddr,  pucMacAddr, 6);

    strcpy(wrq.ifr_name, wlan_if);	


    wrq.u.data.pointer = (caddr_t)&Disconnect_Req;
    wrq.u.data.length = sizeof(DOT11_DISCONNECT_REQ);

    if(ioctl(skfd, SIOCGIWIND, &wrq) < 0)
    {
        printf("Issues disassociation : ioctl error!\n");
        retVal = -1;
    }

    close(skfd);
    return retVal;

}
int SetWlan_idx(char * wlan_iface_name)
{
	int idx;

	idx = atoi(&wlan_iface_name[4]);
	if (idx >= NUM_WLAN_INTERFACE) {
			printf("invalid wlan interface index number!\n");
			return 0;
	}
	wlan_idx = idx;
	vwlan_idx = 0;

#ifdef MBSSID		
	
	if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
			wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
			idx = atoi(&wlan_iface_name[8]);
			if (idx >= NUM_VWLAN_INTERFACE) {
				printf("invalid virtual wlan interface index number!\n");
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
static inline int iw_get_ext(int skfd, char *ifname, int request, struct iwreq *pwrq)
{
	/* Set device name */
	strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
	/* Do the request */
	return(ioctl(skfd, request, pwrq));
}
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
int deauth_wlan_clients(void)
{
	char *buff;
	WLAN_STA_INFO_Tp pInfo;
	int i;
	int wlan0_disabled;
	int wlan0_mode;
#if defined(FOR_DUAL_BAND)
	int wlan1_disabled;
	int wlan1_mode;
#endif
	buff = calloc(1, sizeof(WLAN_STA_INFO_T) * (MAX_STA_NUM+1));
	if (buff == 0) 
	{
		printf("Allocate buffer failed!\n");
		return 0;
	}
	SetWlan_idx("wlan0");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan0_disabled);
	apmib_get(MIB_WLAN_MODE,(void*)&wlan0_mode);
#if defined(FOR_DUAL_BAND)
	SetWlan_idx("wlan1");
	apmib_get(MIB_WLAN_WLAN_DISABLED,(void*)&wlan1_disabled);
	apmib_get(MIB_WLAN_MODE,(void*)&wlan1_mode);
#endif
	if (wlan0_disabled == 0 && wlan0_mode == AP_MODE && getWlStaInfo("wlan0",(WLAN_STA_INFO_Tp)buff) == 0) 
	{
		for (i=1; i<=MAX_STA_NUM; i++) 
		{
			pInfo = (WLAN_STA_INFO_Tp)&buff[i*sizeof(WLAN_STA_INFO_T)];
			if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)) {
//				printf("Issue disconnect to :\n");
//				printf("%02X:%02X:%02X:%02X:%02X:%02X\n",pInfo->addr[0],
//						pInfo->addr[1],pInfo->addr[2],pInfo->addr[3],pInfo->addr[4],pInfo->addr[5]);
				IssueDisconnect(pInfo->addr,"wlan0",1);
			}
		}
	}
#if defined(FOR_DUAL_BAND)
	if (wlan1_disabled == 0 && wlan1_mode == AP_MODE && getWlStaInfo("wlan1",(WLAN_STA_INFO_Tp)buff) == 0) 
	{
		for (i=1; i<=MAX_STA_NUM; i++) 
		{
			pInfo = (WLAN_STA_INFO_Tp)&buff[i*sizeof(WLAN_STA_INFO_T)];
			if (pInfo->aid && (pInfo->flag & STA_INFO_FLAG_ASOC)) {
//				printf("Issue disconnect to :\n");
//				printf("%02X:%02X:%02X:%02X:%02X:%02X\n",pInfo->addr[0],
//						pInfo->addr[1],pInfo->addr[2],pInfo->addr[3],pInfo->addr[4],pInfo->addr[5]);
				IssueDisconnect(pInfo->addr,"wlan1",1);
			}
		}
	}
#endif
	free(buff);
	buff = NULL;
}
void set_lan_dhcpc(char *iface)
{
	char script_file[100], deconfig_script[100], pid_file[100];
	char *strtmp=NULL;
	char tmp[32], Ip[32], Mask[32], Gateway[32];
	char cmdBuff[200];
#ifdef  HOME_GATEWAY
	int intValue=0;
#endif
	sprintf(script_file, "/usr/share/udhcpc/%s.sh", iface); /*script path*/
	sprintf(deconfig_script, "/usr/share/udhcpc/%s.deconfig", iface);/*deconfig script path*/
	sprintf(pid_file, "/etc/udhcpc/udhcpc-%s.pid", iface); /*pid path*/
	apmib_get( MIB_IP_ADDR,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Ip, "%s",strtmp);

	apmib_get( MIB_SUBNET_MASK,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Mask, "%s",strtmp);

	apmib_get( MIB_DEFAULT_GATEWAY,  (void *)tmp);
	strtmp= inet_ntoa(*((struct in_addr *)tmp));
	sprintf(Gateway, "%s",strtmp);

	Create_script(deconfig_script, iface, LAN_NETWORK, Ip, Mask, Gateway);

	sprintf(cmdBuff, "udhcpc -i %s -p %s -s %s -a 15 &", iface, pid_file, script_file);
	system(cmdBuff);

#if defined(CONFIG_APP_SIMPLE_CONFIG)
	system("echo 0 > /var/sc_ip_status");
#endif
}
static int switch_dhcpc_dhcpd(void)
{
	int pid;
	char cmdBuf[32]={0};
	struct in_addr router_ip, netmask;
	char router_ip_buf[16]={0}, netmask_buf[16]={0};
	char *p_router_ip, *p_netmask;
	int tempStatus = discovery_dhcp("br0");
	if(tempStatus == 1) //if front-end router's DHCP is enabled ,this router's DHCPD should be disabled
	{
		if(find_pid_by_name("udhcpd") > 0)/*dhcpd is running*/
		{
			RunSystemCmd(NULL_FILE, "killall", "-9", "udhcpd", NULL_STR); 
		}
		//if(find_pid_by_name("udhcpc")==0)
		if(!isFileExist("/etc/udhcpc/udhcpc-br0.pid"))
		{
			set_lan_dhcpc("br0");
			deauth_wlan_clients();
			RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "down", NULL_STR); 
			sleep(1);
			RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "up", NULL_STR); 
		}
		else/*dhcpc already running*/
		{
			/*do nothing*/
		}
	}
	else			   //if front-end router's DHCP is disabled and this router's DHCP is not running,this router's DHCP should be enabled
	{
#if 0
		//will kill wan udhcpc
		if(find_pid_by_name("udhcpc") > 0)/*dhcpc is running*/
		{
			RunSystemCmd(NULL_FILE, "killall", "-9", "udhcpc", NULL_STR);
		}
#else
		if(isFileExist("/etc/udhcpc/udhcpc-br0.pid"))
		{
			pid=getPid_fromFile("/etc/udhcpc/udhcpc-br0.pid");
			if(pid>0)
			{
				snprintf(cmdBuf, sizeof(cmdBuf), "kill -9 %d", pid);
				system(cmdBuf);
				
				unlink("/etc/udhcpc/udhcpc-br0.pid");
			}
		}
#endif
		if(find_pid_by_name("udhcpd")==0)
		{
			apmib_get(MIB_IP_ADDR,  (void *)&router_ip);			
			p_router_ip=inet_ntoa(router_ip);
			if(p_router_ip!=NULL)
			{
				strncpy(router_ip_buf, p_router_ip, sizeof(router_ip_buf));			
				//printf("\n\nrouter_ip_buf=%s\n",router_ip_buf);
			}
			
			apmib_get(MIB_SUBNET_MASK,  (void *)&netmask);
			p_netmask=inet_ntoa(netmask);
			if(p_netmask!=NULL)
			{
				strncpy(netmask_buf, p_netmask, sizeof(netmask_buf));			
				//printf("\n\nnetmask_buf=%s\n",netmask_buf);
			}

			if(router_ip_buf[0] && netmask_buf[0])
				RunSystemCmd(NULL_FILE, "ifconfig", "br0", router_ip_buf, "netmask", netmask_buf, NULL_STR);
			
			RunSystemCmd(NULL_FILE, "udhcpd", DHCPD_CONF_FILE, NULL_STR); 
			deauth_wlan_clients();
			RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "down", NULL_STR); 
			sleep(1);			
			RunSystemCmd(NULL_FILE, "ifconfig", "eth0", "up", NULL_STR); 
		}
		else/*dhcpd already running*/
		{
			/*do nothing*/
		}
	}
}
int main() 
{
	if (daemon(0, 1) == -1)
	{
		perror("rtk_eventd fork error");
		goto ERROR_EXIT;
	}
	
	int pid_fd;
	pid_fd = pidfile_acquire(RTK_EVENTD_PID_FILE);
	pidfile_write_release(pid_fd);
	
	char msgBuf[256]={0};
	rtkEventHdr *pEventdHdr=msgBuf;

	pEventdHdr->eventID=0;
	strcpy(pEventdHdr->name, "br0");
	strcpy(pEventdHdr->data, "RTK EVENTD START!");	
	
	sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_RTK_EVENTD);
	if(sock_fd<0)
	{
		printf("%s:%d ##create netlink socket fail!\n",__FUNCTION__,__LINE__);
		goto ERROR_EXIT;
	}
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	//src_addr.nl_pid = getpid(); // self pid 
	src_addr.nl_pid = 0;
	//src_addr.nl_groups = 0;     // not in mcast groups
	src_addr.nl_groups = Rtk_Event_Netlink_Group; 
	bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;    // For Linux Kernel 
	dest_addr.nl_groups = 0; // unicast 
	
	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));	
	if(nlh==NULL)
		goto ERROR_EXIT;
	
	// fill the netlink message header 
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid(); // self pid
	nlh->nlmsg_flags = 0;	
	
	memcpy(NLMSG_DATA(nlh), msgBuf, strlen(pEventdHdr->data)+RTK_EVENTD_HDR_LEN);

	memset(&msg, 0, sizeof(msg));
	memset(&iov, 0, sizeof(iov));
	
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	sendmsg(sock_fd, &msg, 0);


	apmib_init();
	int opmode;              //router's mode
	int dhcp_type;           //DHCP status:client,server or disabled.if disabled,nothing will be done
	int pid;
	char cmdBuf[32]={0};
    //avoid 60s waiting time when start up
    switch_dhcpc_dhcpd();
	fd_set fds;              //file descriptor set
	struct timeval timeout={60,0};//60s time out
	while(1)                 //read message from kernel
	{
		FD_ZERO(&fds);       //clean up file descriptor set,else can't detect the change of file descriptor 
		FD_SET(sock_fd,&fds);//add file descriptor
		//if(timeout.tv_sec == 0 && timeout.tv_usec== 0)
		{
			timeout.tv_sec = 60;
			timeout.tv_usec = 0;
		}
		switch(select(sock_fd+1,&fds,NULL,NULL,&timeout))
		{ 
			case -1: //select() function error  
				printf("select() function error!\n");
				break;
			case 0:  //time out  
			{
				switch_dhcpc_dhcpd();
				break;
			}
			default: //capture wire plug-in event
			{
				memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
				recvmsg(sock_fd, &msg, 0);
				pEventdHdr=NLMSG_DATA(nlh);	
			
				if(pEventdHdr->eventID == WIRE_PLUG_ON)
				{
					printf("from Auto_DHCP_Check: WIRE_PLUG_ON: port:%s\n",pEventdHdr->data);
					switch_dhcpc_dhcpd();
				}
				else if(pEventdHdr->eventID == WIFI_CONNECT_SUCCESS)
				{
					printf("Wlan connect success!\n");
					switch_dhcpc_dhcpd();
					sleep(2);
				#if 0
					if(find_pid_by_name("udhcpc") > 0)/*dhcpc is running*/
					{
						kill(find_pid_by_name("udhcpc"),SIGUSR1);/*renew IP*/
					}
				#else					
					if(isFileExist("/etc/udhcpc/udhcpc-br0.pid"))
					{
						pid=getPid_fromFile("/etc/udhcpc/udhcpc-br0.pid");
						if(pid>0)
							kill(pid, SIGUSR1);/*renew IP*/						
					}
				#endif
				}
				break;
			}
		}
	}
	
ERROR_EXIT:
	if(sock_fd>0)       // close netlink socket 
		close(sock_fd);	
	if(nlh)
		free(nlh);
	return 0;
}

