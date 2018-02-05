/*
 *
 */

/* System include files */
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/wait.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <net/if.h>
#include <stddef.h>		/* offsetof */
#include <net/if_arp.h>
#include <linux/if_ether.h>

#include <dirent.h>
#include "reinit_utility.h"
#include "apmib.h"


#define ifreq_offsetof(x)  offsetof(struct ifreq, x)

///////////////////////////////////////////////////////////////////////////////////////

void get_br_interface_name(char* br_interface_name)
{
	if(!br_interface_name)
	{		
		printf("%s:%d invalid br_interface_name!\n",__FUNCTION__,__LINE__);
		return;
	}
	sprintf(br_interface_name, "%s", "br0");
}



int DoCmd(char *const argv[], char *file)
{    
	pid_t pid;
	int status;
	int fd;
	char _msg[30];
	switch (pid = fork()) 
	{
			case -1:	/* error */
				perror("fork");
				return errno;
			case 0:	/* child */
				
				signal(SIGINT, SIG_IGN);
				if(file){
					if((fd = open(file, O_RDWR | O_CREAT))==-1){ /*open the file */
						sprintf(_msg, "open %s", file); 
  						perror(_msg);
  						exit(errno);
					}
					dup2(fd,STDOUT_FILENO); /*copy the file descriptor fd into standard output*/
					dup2(fd,STDERR_FILENO); /* same, for the standard error */
					close(fd); /* close the file descriptor as we don't need it more  */
				}else{
				#ifndef SYS_DEBUG		
					close(2); //do not output error messages
				#endif	
				}
				setenv("PATH", "/sbin:/bin:/usr/sbin:/usr/bin", 1);
				execvp(argv[0], argv);
				perror(argv[0]);
				exit(errno);
			default:	/* parent */
				{
					
					waitpid(pid, &status, 0);
					#ifdef SYS_DEBUG	
					if(status != 0)
						printf("parent got child's status:%d, cmd=%s %s %s\n", status, argv[0], argv[1], argv[2]);
					#endif		
					if (WIFEXITED(status)){
					#ifdef SYS_DEBUG	
						printf("parent will return :%d\n", WEXITSTATUS(status));
					#endif		
						return WEXITSTATUS(status);
					}else{
						
						return status;
					}
				}
	}
}
int RunSystemCmd(char *filepath, ...)
{
	va_list argp;
	char *argv[24]={0};
	int status;
	char *para;
	int argno = 0;
	va_start(argp, filepath);
    #ifdef DISPLAY_CMD
	printf("\n"); 
    #endif
	while (1){ 
		para = va_arg( argp, char*);
		if ( strcmp(para, "") == 0 )
			break;
		argv[argno] = para;
        #ifdef DISPLAY_CMD
		printf(" %s ", para); 
        #endif
		argno++;
	} 
    #ifdef DISPLAY_CMD    
	printf("\n");     
    #endif
	argv[argno+1] = NULL;
	status = DoCmd(argv, filepath);
	va_end(argp);
	return status;
}

int isFileExist(char *file_name)
{
	struct stat status;

	if ( stat(file_name, &status) < 0)
		return 0;

	return 1;
}

/*add line buffer to file ,modify by dzh*/
int write_line_to_file2(char *filename,char *line_data)
{
	unsigned char tmpbuf[512];
	int fh=0;
	int mode;
	
	/*if file not exist ************/
	if(access(filename,0)!=0)
		mode = 1;
	else
		mode = 2;
	
	if(mode == 1) {/* write line datato file */
		
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC);
		
	}else if(mode == 2){/*append line data to file*/
		
		fh = open(filename, O_RDWR|O_APPEND);	
	}
	
	
	if (fh < 0) {
		fprintf(stderr, "Create %s error!\n", filename);
		return 0;
	}


	sprintf((char *)tmpbuf, "%s", line_data);
	write(fh, tmpbuf, strlen((char *)tmpbuf));



	close(fh);
	return 1;
}


int write_line_to_file(char *filename, int mode, char *line_data)
{
	unsigned char tmpbuf[512];
	int fh=0;

	
	if(mode == 1) {/* write line datato file */
		
		fh = open(filename, O_RDWR|O_CREAT|O_TRUNC);
		
	}else if(mode == 2){/*append line data to file*/
		
		fh = open(filename, O_RDWR|O_APPEND);	
	}
	
	
	if (fh < 0) {
		fprintf(stderr, "Create %s error!\n", filename);
		return 0;
	}


	sprintf((char *)tmpbuf, "%s", line_data);
	write(fh, tmpbuf, strlen((char *)tmpbuf));



	close(fh);
	return 1;
}


/*
devide string into char*
example:
devide buff="eth0:wlan0:wlan1:" and divider=':' into
item[0]="eth0"
item[1]="wlan0"
item[2]="wlan1"
item[3]=""
item[4]=NULL
...
item[count_max-1]=NULL

item should be char* item[count_max] type
*/
int devideStringIntoItems(char * buff,int count_max,char divider, char *item[])
{
	int i=0,count=0;
	int buffLen=strlen(buff);
	if(!buff||!item||!divider)
	{
		fprintf(stderr,"invalid input!%d\n",__LINE__);
		exit(0);
	}

	for(count=0;count<count_max;count++)
	{
		item[count]=NULL;
	}

	count=0;
	item[count]=buff;
	for(i=0;i<buffLen;i++)
	{
		if(buff[i]==divider)
		{
			buff[i]='\0';
			count++;
			item[count]=buff+i+1;
		}
	}
#if 0
	for(i=0;item[i] && item[i][0];i++)
	{		
		fprintf(stderr,"iface=%s  %s:%d\n",item[i],__FUNCTION__,__LINE__);
	}
#endif
	return 0;
}
pid_t find_pid_by_name( char* pidName)
{
	DIR *dir;
	struct dirent *next;
	pid_t pid;
	
	if ( strcmp(pidName, "init")==0)
		return 1;
	
	dir = opendir("/proc");
	if (!dir) {
		printf("Cannot open /proc");
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

int getPid_fromFile(char *file_name)
{
	FILE *fp;
	char *pidfile = file_name;
	int result = -1;
	
	fp= fopen(pidfile, "r");
	if (!fp) {
        	printf("can not open:%s\n", file_name);
		return -1;
   	}
	fscanf(fp,"%d",&result);
	fclose(fp);
	
	return result;
}

int killDaemonByPidFile(char *pidFile)
{
	char strPID[10];
	int pid=-1;
	
	if(!pidFile)
	{
		printf("%s : input file name is null!\n",__FUNCTION__);
		return -1;
	}
	if(isFileExist(pidFile))
	{
		pid=getPid_fromFile(pidFile);
		if(pid != 0){
			sprintf(strPID, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", strPID, NULL_STR);
		}
		unlink(pidFile);
		return 0;
	}else
	{
		//printf("%s : %s file is not exist!\n",__FUNCTION__,pidFile);
		return -1;
	}
	return 0;
}

int setInAddr( char *interface, char *Ipaddr, char *Netmask, char *HwMac, int type)
{
    struct ifreq ifr;
    int skfd=0;
    struct in_addr in_addr, in_netmask, in_broadaddr;
    struct sockaddr sa;
    int default_type= (IFACE_FLAG_T) | (IP_ADDR_T) | (NET_MASK_T) | (HW_ADDR_T);
    int request_action=0;
    request_action =type & default_type;
	
    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return -1;
		
    strcpy(ifr.ifr_name, interface);
   
	    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
	    		close( skfd );
			return (-1);
		}
	 if(request_action ==IFACE_FLAG_T){
	 	return 0;
		}
	ifr.ifr_flags = IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST;
	if(request_action & IP_ADDR_T ){
		if (Ipaddr) {
			inet_aton(Ipaddr, &in_addr);
			(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr).s_addr= in_addr.s_addr;
			ifr.ifr_addr.sa_family = AF_INET;
			if (ioctl(skfd, SIOCSIFADDR, &ifr) < 0)
				goto set_err;
		}
	}
	if(request_action & NET_MASK_T){
		if (Ipaddr && Netmask) {
			inet_aton(Netmask, &in_netmask);
			(((struct sockaddr_in *)(&ifr.ifr_netmask))->sin_addr).s_addr= in_netmask.s_addr;
			ifr.ifr_netmask.sa_family = AF_INET;
			if (ioctl(skfd, SIOCSIFNETMASK, &ifr) < 0)
				goto set_err;
	
			in_broadaddr.s_addr = (in_addr.s_addr & in_netmask.s_addr) | ~in_netmask.s_addr;
			(((struct sockaddr_in *)(&ifr.ifr_broadaddr))->sin_addr).s_addr= in_broadaddr.s_addr;
			ifr.ifr_broadaddr.sa_family = AF_INET;
			if (ioctl(skfd, SIOCSIFBRDADDR, &ifr) < 0)
				goto set_err;
		}
	}
	if(request_action & HW_ADDR_T){
		if(HwMac){
			sa.sa_family = ARPHRD_ETHER;
			memcpy(&sa.sa_data,HwMac,6); 
			memcpy((((char *) (&ifr)) +ifreq_offsetof(ifr_hwaddr)), &sa, sizeof(struct sockaddr));
			if (ioctl(skfd, SIOCSIFHWADDR, &ifr) < 0)
				goto set_err;
			
		}
	}
    close( skfd );
    return 0;
    
set_err:
	close(skfd);
	perror(interface);
	return -1;
}

char *get_name(char *name, char *p)
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

int if_readlist_proc(char *target, char *key, char *exclude)
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
		s = get_name(name, buf);
		if(strstr(name, key)){
			iface_num++;
			if(target[0]==0x0){
				sprintf(target, "%s", name);
			}else{
				sprintf(tmp_str, " %s", name);
				strcat(target, tmp_str);
			}
		}
		//printf("iface name=%s, key=%s\n", name, key);
	}
	
	fclose(fh);
	return iface_num;
}




int daemon_is_running(const char * pid_file)
{
	int pidFd;
	char pidBuf[16]={0};

	struct flock lock;

	pidFd=open(pid_file,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
	if(pidFd<0)
	{
		fprintf(stderr, "could not create pid file: %s \n", pid_file);
		exit(1);
	}
	
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_whence = SEEK_SET;
	if(fcntl(pidFd,F_GETLK,&lock)<0)
	{
		
		fprintf(stderr, "fcntl error! \n");
		exit(1);
	}
	close(pidFd);
	if(lock.l_type==F_UNLCK)
		return 0;
	return lock.l_pid;
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

#define RTL8651_IOCTL_GETWANLINKSTATUS 2000
/* Wan link status detect */
int getWanLink(char *interface)
{
        unsigned int    ret;
        unsigned int    args[0];

        re865xIoctl(interface, RTL8651_IOCTL_GETWANLINKSTATUS, (unsigned int)(args), 0, (unsigned int)&ret) ;
        return ret;
}

int getInAddr( char *interface, int type, void *pAddr )
{
    struct ifreq ifr;
    int skfd, found=0;
	struct sockaddr_in *addr;
    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
    		close( skfd );
		return (0);
	}
    if (type == HW_ADDR_T) {
    	if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
		memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
		found = 1;
	}
    }
    else if (type == IP_ADDR_T) {
		if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0) {
			addr = ((struct sockaddr_in *)&ifr.ifr_addr);
			*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
			found = 1;
		}
    }
    else if (type == NET_MASK_T) {
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    }
	else {
    	
    	if (ioctl(skfd, SIOCGIFDSTADDR, &ifr) >= 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    	
    }
    close( skfd );
    return found;

}

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
       		printf("Open %s file error.\n", _PATH_PROCNET_ROUTE);
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


int SetWlan_idx(char * wlan_iface_name)
{
	int idx;
	
		idx = atoi(&wlan_iface_name[4]);
		if (idx >= NUM_WLAN_INTERFACE) {
				printf("invalid wlan interface index number!\n");
				return 0;
		}
		apmib_rtkReinit_setIdx(idx,0,-1);
	
	
#ifdef MBSSID		
		
		if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
				wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
				idx = atoi(&wlan_iface_name[8]);
				if (idx >= NUM_VWLAN_INTERFACE) {
					printf("invalid virtual wlan interface index number!\n");
					return 0;
				}
				apmib_rtkReinit_setIdx(-1,idx+1,-1);
				idx = atoi(&wlan_iface_name[4]);
				apmib_rtkReinit_setIdx(idx,-1,-1);
		}
#endif		

#ifdef UNIVERSAL_REPEATER
				if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
						!memcmp(&wlan_iface_name[6], "vxd", 3)) {
					idx = atoi(&wlan_iface_name[4]);
					apmib_rtkReinit_setIdx(idx,NUM_VWLAN_INTERFACE,-1);
				}
#endif				

//printf("\r\n wlan_iface_name=[%s],wlan_idx=[%u],vwlan_idx=[%u],__[%s-%u]\r\n",wlan_iface_name,wlan_idx,vwlan_idx,__FILE__,__LINE__);

return 1;		
}

#ifdef CONFIG_SMART_REPEATER
int getWispRptIfaceName(char*pIface,int wlanId)
{
	int rptEnabled=0,wlanMode=0,opMode=0;
	char wlan_wanIfName[16]={0};
	if(wlanId == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else if(1 == wlanId)
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
	else return -1;
	apmib_get(MIB_OP_MODE,(void *)&opMode);
	if(opMode!=WISP_MODE)
		return -1;
	apmib_save_wlanIdx();

	sprintf(wlan_wanIfName,"wlan%d",wlanId);
	SetWlan_idx(wlan_wanIfName);
	//for wisp rpt mode,only care root ap
	apmib_get(MIB_WLAN_MODE, (void *)&wlanMode);
	if((AP_MODE==wlanMode || AP_WDS_MODE==wlanMode || AP_MESH_MODE==wlanMode || MESH_MODE==wlanMode ) && rptEnabled)
	{//root AP mode and rpt enabled, use -vxd as wanIf
		if(!strstr(pIface,"-vxd"))
		{
			strcat(pIface,"-vxd");
		}
	}else
	{
		char * ptmp = strstr(pIface,"-vxd");
		if(ptmp)
			memset(ptmp,0,sizeof(char)*strlen("-vxd"));
	}
	apmib_recov_wlanIdx();
	return 0;
}
#endif

#ifdef CONFIG_SMART_REPEATER
int getWispRptIface(char**pIface,int wlanId)
{
	int rptEnabled=0,wlanMode=0,opMode=0;
	char wlan_wanIfName[16]={0};
	if(wlanId == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else if(1 == wlanId)
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
	else return -1;
	apmib_get(MIB_OP_MODE,(void *)&opMode);
	if(opMode!=WISP_MODE)
		return -1;
	apmib_save_wlanIdx();
	
	sprintf(wlan_wanIfName,"wlan%d",wlanId);
	SetWlan_idx(wlan_wanIfName);
	//for wisp rpt mode,only care root ap
	apmib_get(MIB_WLAN_MODE, (void *)&wlanMode);
	if((AP_MODE==wlanMode || AP_MESH_MODE==wlanMode || MESH_MODE==wlanMode || AP_WDS_MODE == wlanMode ) && rptEnabled)
	{
		if(wlanId == 0)
			*pIface = "wlan0-vxd";
		else if(1 == wlanId)
			*pIface = "wlan1-vxd";
		else return -1;
	}else
	{
		char * ptmp = strstr(*pIface,"-vxd");
		if(ptmp)
			memset(ptmp,0,sizeof(char)*strlen("-vxd"));
	}
	apmib_recov_wlanIdx();
	return 0;
}
#endif

int isConnectPPP()
{
	struct stat status;
	if ( stat("/etc/ppp/link", &status) < 0)
		return 0;

	return 1;
}

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
	int isWanPhyLink = 0;	
	if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
		return -1;
  
  if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
		return -1;

	if( !apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId))
		return -1;
	
	if ( dhcp == PPPOE || dhcp == PPTP || dhcp == L2TP || dhcp == USB3G ) { /* # keith: add l2tp support. 20080515 */

	iface = "ppp0";
		if ( !isConnectPPP() )
			iface = NULL;
	}
	else if (opmode == WISP_MODE){
		if(0 == wispWanId)
			iface = "wlan0";
		else if(1 == wispWanId)
			iface = "wlan1";
#ifdef CONFIG_SMART_REPEATER
		if(getWispRptIface(&iface,wispWanId)<0)
					return -1;
#endif			
	}
	else
		iface = "eth1";
	
	if(opmode != WISP_MODE)
	{
		if(iface){
			if((isWanPhyLink = getWanLink("eth1")) < 0){
				sprintf(pWanIP,"%s","0.0.0.0");
			}
		}	
	}
	
	if ( iface && getInAddr(iface, IP_ADDR, (void *)&intaddr ) && ((isWanPhyLink >= 0)) )
		sprintf(pWanIP,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanIP,"%s","0.0.0.0");

	if ( iface && getInAddr(iface, SUBNET_MASK, (void *)&intaddr ) && ((isWanPhyLink >= 0) ))
		sprintf(pWanMask,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanMask,"%s","0.0.0.0");
	
	if ( iface && getDefaultRoute(iface, &intaddr) && ((isWanPhyLink >= 0) ))
		sprintf(pWanDefIP,"%s",inet_ntoa(intaddr));
	else
		sprintf(pWanDefIP,"%s","0.0.0.0");	

#ifdef _ALPHA_DUAL_WAN_SUPPORT_
	if (dhcp == PPPOE)
	{
		if (getInAddr("eth1", IP_ADDR, (void *)&intaddr ) && ((isWanPhyLink >= 0)) ) {
			strcat(pWanIP, ", ");
			strcat(pWanIP, inet_ntoa(intaddr));
		}
		else
			strcat(pWanIP, ", 0.0.0.0");

		if (getInAddr("eth1", SUBNET_MASK, (void *)&intaddr ) && ((isWanPhyLink >= 0) )) {
			strcat(pWanMask, ", ");
			strcat(pWanMask, inet_ntoa(intaddr));
		}
		else
			strcat(pWanMask, ", 0.0.0.0");
		
		if (getDefaultRoute("eth1", &intaddr) && ((isWanPhyLink >= 0) )) {
			strcat(pWanDefIP, ", ");
			strcat(pWanDefIP, inet_ntoa(intaddr));
		}
		else
			strcat(pWanDefIP, ", 0.0.0.0");
	}
#endif

	//To get wan hw addr
	if(opmode == WISP_MODE) {
		if(0 == wispWanId)
			iface = "wlan0";
		else if(1 == wispWanId)
			iface = "wlan1";
#ifdef CONFIG_SMART_REPEATER
		if(getWispRptIface(&iface,wispWanId)<0)
					return -1;
#endif			
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

int get_wan_iface(char * ifname)
{
	DHCP_T dhcp;
	OPMODE_T opmode=-1;
	unsigned int wispWanId=0;
	char *iface=NULL;
	

		
	if ( !apmib_get( MIB_WAN_DHCP, (void *)&dhcp) )
		return -1;
  
 	if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
		return -1;

	if( !apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId))
		return -1;
	
	if ( dhcp == PPPOE || dhcp == PPTP || dhcp == L2TP || dhcp == USB3G ) { /* # keith: add l2tp support. 20080515 */

		iface = "ppp0";
		if ( !isConnectPPP() )
			iface = NULL;
	}
	else if (opmode == WISP_MODE){
		if(0 == wispWanId)
			iface = "wlan0";
		else if(1 == wispWanId)
			iface = "wlan1";
#ifdef CONFIG_SMART_REPEATER
		if(getWispRptIface(&iface,wispWanId)<0)
					return -1;
#endif			
	}
	else
		iface = "eth1";

	if(iface)
		strcpy(ifname,iface);
	else
		ifname[0]=0;
	return 0;
}
int is_wan_connected()
{
	char strWanIP[16];
	char wanIfce[16]={0};
	int isWanPhyLink = 0;
	OPMODE_T opmode=-1;
	struct in_addr	intaddr;

	if(get_wan_iface(wanIfce))
		return -1;
	
	if ( !apmib_get( MIB_OP_MODE, (void *)&opmode) )
		return -1;
	if(opmode != WISP_MODE)
	{
		if(wanIfce[0]){
			if((isWanPhyLink = getWanLink("eth1")) < 0){
				return 0;
			}
		}	
	}

	if ( wanIfce[0] && getInAddr(wanIfce, IP_ADDR, (void *)&intaddr ) && ((isWanPhyLink >= 0)) )
	{
		sprintf(strWanIP,"%s",inet_ntoa(intaddr));
		if(strcmp(strWanIP,"0.0.0.0")==0)
			return 0;
		return 1;
	}
	return 0;
}

/*create deconfig script for dhcp client*/
void Create_script(char *script_path, char *iface, int network, char *ipaddr, char *mask, char *gateway)
{	
	unsigned char tmpbuf[100];
	int fh;
	
	fh = open(script_path, O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
	if (fh < 0) {
		fprintf(stderr, "Create %s file error!\n", script_path);
		return;
	}
	if(network==LAN_NETWORK){
		sprintf((char *)tmpbuf, "%s", "#!/bin/sh\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "ifconfig %s %s netmask %s\n", iface, ipaddr, mask);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "while route del default dev %s\n", iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "%s\n", "do :");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "%s\n", "done");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "route add -net default gw %s dev %s\n", gateway, iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		sprintf((char *)tmpbuf, "%s\n", "init.sh ap wlan_app");
		write(fh, tmpbuf, strlen((char *)tmpbuf));
	}
	if(network==WAN_NETWORK){
		sprintf((char *)tmpbuf, "%s", "#!/bin/sh\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));

#if 0 //def CONFIG_POCKET_ROUTER_SUPPORT //it needn't do this
		sprintf((char *)tmpbuf, "sysconf disc dhcpc\n");
		write(fh, tmpbuf, strlen((char *)tmpbuf));			
#endif	

		sprintf((char *)tmpbuf, "ifconfig %s 0.0.0.0\n", iface);
		write(fh, tmpbuf, strlen((char *)tmpbuf));
		
	}
	close(fh);
}

// input:process name output: 1--exist 0---not exist
int IsExistProcess(unsigned char * proName)
{
	unsigned char command[128];
	unsigned char buf[32];
	FILE *fp;
	int prog_cnt;
	sprintf(command,"ps -ef|grep \"%s\" | grep -v grep | wc -l",proName);
	fp = popen(command,"r");
	if(NULL == fp)
		return -1;
	
	if (NULL == fgets(buf, sizeof(buf),fp)) {
		pclose(fp);
		return -1;
	}
	pclose(fp);
	sscanf(buf,"%d",&prog_cnt);
	return prog_cnt;
}

#endif
#if defined(MULTI_WAN_SUPPORT)
int getWanIfaceEntry(int index,WANIFACE_T* pEntry)
{	
	memset(pEntry, '\0', sizeof(*pEntry));	
	*((char *)pEntry) = (char)index;
	if(!apmib_get(MIB_WANIFACE_TBL,(void *)pEntry)){
		printf("get wanIface mib error\n");
		return FALSE;
	}
}

/*format wan info */
int format_wan_data(BASE_DATA_Tp pdata,BASE_DATA_Tp orig_data)
{
	WANIFACE_T WanIfaceEntry;
	WAN_DATA_Tp WANp ;
#ifdef 	SINGLE_WAN_SUPPORT
	OPMODE_T opmode;
	int wan_wifi_idx;
#endif
	if(!getWanIfaceEntry(orig_data->wan_idx,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return 0;
	}

	WANp = (WAN_DATA_Tp)(pdata);	
	WANp->base.wan_idx = orig_data->wan_idx;
	WANp->wan_data.wan_type = WanIfaceEntry.AddressType;

	//if more private data need to be added ,just modify wan_data struct and add value in here.
#ifdef SINGLE_WAN_SUPPORT
	apmib_get(MIB_OP_MODE,&opmode);
	if(opmode == GATEWAY_MODE)
		sprintf(WANp->wan_data.wan_iface,"eth1.%d",WANp->base.wan_idx);
	else if(opmode == WISP_MODE)
	{
		apmib_get(MIB_WISP_WAN_ID,&wan_wifi_idx);
		sprintf(WANp->wan_data.wan_iface,"wlan%d-vxd",wan_wifi_idx);	
	}
#else
	sprintf(WANp->wan_data.wan_iface,"eth1.%d",WANp->base.wan_idx);
#endif

	if(	WANp->wan_data.wan_type == PPPOE
#if defined(SINGLE_WAN_SUPPORT)
		|| WANp->wan_data.wan_type == PPTP
		|| WANp->wan_data.wan_type == L2TP
#endif
		)
		sprintf(WANp->wan_data.virtual_wan_iface,"ppp%d",WANp->base.wan_idx);
	return 1;
}


int isValidWan(BASE_DATA_Tp pdata)
{

	WANIFACE_T WanIfaceEntry;
	if(!getWanIfaceEntry(pdata->wan_idx,&WanIfaceEntry))
	{
		printf("%s.%d.get waniface entry fail\n",__FUNCTION__,__LINE__);
		return 0;
	}
	
	if(!WanIfaceEntry.enable)
		return 0;
	
	return 1;
}
void getWanIface(BASE_DATA_Tp pdata ,unsigned char *wan_iface)
{	
	WAN_DATA_Tp wan_p ;
	wan_p = (WAN_DATA_Tp)pdata;
	
	/*set wan iface *************/
	if(wan_p->wan_data.wan_type == PPPOE
#if defined(SINGLE_WAN_SUPPORT)
	|| wan_p->wan_data.wan_type == PPTP
	|| wan_p->wan_data.wan_type == L2TP
#endif
	)
		strcpy(wan_iface,wan_p->wan_data.virtual_wan_iface);
	else
		strcpy(wan_iface,wan_p->wan_data.wan_iface);
	
}


//wanBindingLanPorts index to ifname
int getLanPortIfName(char* name,int index)
{
	if(!name) return -1;
	
	switch(index)
	{
		case 0:
			sprintf(name,"eth0");
			break;
		case 1:
		case 2:
		case 3:
			sprintf(name,"eth%d",index+1);
			break;
		case WAN_INTERFACE_LAN_PORT_NUM:
			sprintf(name,"wlan0");
			break;
		case WAN_INTERFACE_LAN_PORT_NUM+1:
		case WAN_INTERFACE_LAN_PORT_NUM+2:
		case WAN_INTERFACE_LAN_PORT_NUM+3:
		case WAN_INTERFACE_LAN_PORT_NUM+4:
			sprintf(name,"wlan0-va%d",index-WAN_INTERFACE_LAN_PORT_NUM-1);
			break;
		case WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM/2:
			sprintf(name,"wlan1");
			break;
		case WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM/2+1:
		case WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM/2+2:
		case WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM/2+3:
		case WAN_INTERFACE_LAN_PORT_NUM+WAN_INTERFACE_WLAN_PORT_NUM/2+4:
			sprintf(name,"wlan1-va%d",index-WAN_INTERFACE_LAN_PORT_NUM-WAN_INTERFACE_WLAN_PORT_NUM/2-1);
			break;
		default:
			return 0;
			
	}
	
	return 1;
}
int checkLanPortInvalid(int index)
{
	char ifName[IFACE_NAME_MAX]={0};
	if(getLanPortIfName(ifName,index)!=1)
		return 1;
	return 0;
}

int getIfNameByIdx(int index,char *name)
{
	if(!name) return -1;
	if(index<0||index>=IF_IDX_END) return -1;

	switch(index)
	{
		case IF_ETH0_IDX:
			sprintf(name,"eth0");
			return 0;
		case IF_ETH2_IDX:			
			sprintf(name,"eth2");
			return 0;
		case IF_ETH3_IDX:
			sprintf(name,"eth3");
			return 0;
		case IF_ETH4_IDX:
			sprintf(name,"eth4");
			return 0;
		case IF_ETH1_IDX:
			sprintf(name,"eth1");
			return 0;
		default:
			break;
	}
	if(index>=IF_ETH1_1_IDX && index <=IF_ETH1_1_IDX+WANIFACE_NUM-1)
	{
		sprintf(name,"eth1.%d",index-IF_ETH1_1_IDX+1);
	}
	else if(index==IF_WLAN0_IDX)
	{
		sprintf(name,"wlan0");
	}
	else if(index>IF_WLAN0_IDX && index<=IF_WLAN0_IDX+NUM_VWLAN)
	{
		sprintf(name,"wlan0-va%d",index-IF_WLAN0_IDX-1);
	}
	else if(index==IF_WLAN0_VXD_IDX)
	{
		sprintf(name,"wlan0-vxd");
	}
	else if(index==IF_WLAN0_VXD_IDX+1)
	{
		sprintf(name,"wlan0-msh");
	}
	else if(index>IF_WLAN0_VXD_IDX+1 && index <=IF_WLAN0_VXD_IDX+1+MAX_WDS_NUM)
	{
		sprintf(name,"wlan0-wds%d",index-IF_WLAN0_VXD_IDX-1);
	}
	else if(index==IF_WLAN1_IDX)
	{
		sprintf(name,"wlan1");
	}
	else if(index>IF_WLAN1_IDX && index<=IF_WLAN1_IDX+NUM_VWLAN)
	{
		sprintf(name,"wlan1-va%d",index-IF_WLAN1_IDX-1);
	}
	else if(index==IF_WLAN1_VXD_IDX)
	{
		sprintf(name,"wlan1-vxd");
	}
	else if(index==IF_WLAN1_VXD_IDX+1)
	{
		sprintf(name,"wlan1-msh");
	}
	else if(index>IF_WLAN1_VXD_IDX+1 && index <=IF_WLAN1_VXD_IDX+1+MAX_WDS_NUM)
	{
		sprintf(name,"wlan1-wds%d",index-IF_WLAN1_VXD_IDX-1);
	}
	return 0;
}

int checkDevExist(char *devName)
{
	char target[512]={0};
	return if_readlist_proc(target, devName, 0);	
}

int checkWlanType(int index,int isRootAp,int isVxd)
{
	int opmode=0;
	int wispWanId=0;
	int wlan_disable=0;
	apmib_save_wlanIdx();
	wlan_idx=index;
	if(isRootAp)
	{
		vwlan_idx=0;
	}
	if(isVxd)
		vwlan_idx=NUM_VWLAN_INTERFACE+1;

	apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disable);
	apmib_recov_wlanIdx();
	if(wlan_disable)
		return IF_TYPE_DISABLED;
	
	if(!apmib_get( MIB_OP_MODE, (void *)&opmode))
			return -1;
	if(!apmib_get(MIB_WISP_WAN_ID, (void *)&wispWanId))
		return -1;
	if(opmode==WISP_MODE && wispWanId==index)
	{		
#ifdef UNIVERSAL_REPEATER
		if(isVxd)
			return IF_TYPE_WAN;
#else
		if(isRootAp)
			return IF_TYPE_WAN;
#endif
	}
	
	return IF_TYPE_LAN;
}

int getIfType(char *name)
{
	if(!name) return -1;

	if(!checkDevExist(name))
		return IF_TYPE_NOTSUPPORT;
	
	if(strcmp(name,"eth0")==0 ||
		strcmp(name,"eth2")==0 ||
		strcmp(name,"eth3")==0 ||
		strcmp(name,"eth4")==0)
	{
		return IF_TYPE_LAN;
	}
	else if(strcmp(name,"eth1")==0)
	{
		int opmode=0;
		if(!apmib_get( MIB_OP_MODE, (void *)&opmode))
			return -1;
		if(opmode==GATEWAY_MODE)
			return IF_TYPE_WAN;
		else
			return IF_TYPE_LAN;
		
	}
	else if(strncmp(name,"eth1.",5)==0)
	{
		return IF_TYPE_WAN;
	}
	else if(strcmp(name,"wlan0")==0 || strcmp(name,"wlan1")==0)
	{
		return checkWlanType(name[4]-'0',1,0);
	}
	else if(strncmp(name,"wlan0-va",8)==0 
		|| strncmp(name,"wlan1-va",8)==0)
	{
		int wlan_disable=0;
		apmib_save_wlanIdx();
		wlan_idx=name[4]-'0';
		vwlan_idx=name[8]-'0'+1;
		apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disable);
		apmib_recov_wlanIdx();
		if(wlan_disable)
			return IF_TYPE_DISABLED;
		return IF_TYPE_LAN;
	}
	else if(strcmp(name,"wlan-msh")==0
		|| strcmp(name,"wlan0-msh")==0
		|| strcmp(name,"wlan1-msh")==0)
	{
		int wlan_mode=0,wlan_mesh_enabled=0;	
		if(isFileExist(MESH_PATHSEL)){
	        apmib_save_wlanIdx();
			wlan_idx=name[4]-'0';
	        apmib_get(MIB_WLAN_MODE, (void *)&wlan_mode);
	        apmib_get(MIB_WLAN_MESH_ENABLE,(void *)&wlan_mesh_enabled); 
			apmib_recov_wlanIdx();
	        if(wlan_mode != AP_MESH_MODE && wlan_mode != MESH_MODE || !wlan_mesh_enabled) {
	            return IF_TYPE_DISABLED;
	        }        
		}
		return IF_TYPE_LAN;
	}
	else if(strncmp(name,"wlan0-wds",9)==0
		|| strncmp(name,"wlan1-wds",9)==0
		)
	{
		int wlan_wds_enabled=0,wlan_wds_num=0,wlan_mode=0;
		apmib_save_wlanIdx();
		wlan_idx=name[4]-'0';
		apmib_get( MIB_WLAN_WDS_ENABLED, (void *)&wlan_wds_enabled);
		apmib_get( MIB_WLAN_WDS_NUM, (void *)&wlan_wds_num);
		apmib_get( MIB_WLAN_MODE, (void *)&wlan_mode); //get wlan if mode
		apmib_recov_wlanIdx();
		if(!wlan_wds_enabled || 
		(wlan_mode!=WDS_MODE && wlan_mode!=AP_WDS_MODE)||
		 name[9]-'0'>=wlan_wds_num)
		{
			return IF_TYPE_DISABLED;
		}
		return IF_TYPE_LAN;
	}
	else if(strcmp(name,"wlan0-vxd")==0 || strcmp(name,"wlan1-vxd")==0)
	{		
		return checkWlanType(name[4]-'0',0,1);
	}
	return IF_TYPE_LAN;
}

char* getifNameFormBuf(char *buf)
{
    char* retVal=NULL;
    if(retVal=strstr(buf,"eth")) return retVal;
    else if(retVal=strstr(buf,"wlan")) return retVal;
    return retVal;
}
int getBrList(char* brName,char ifNames[][IFACE_NAME_MAX])
{
    char tmpBuf[128]={0};
    char find=0;
    int index=0;
    char *ifNameTmp=NULL;
    FILE * fp=popen("brctl show","r");
    if(!fp) return -1;
    while(fgets(tmpBuf,sizeof(tmpBuf),fp)!=NULL)
    {
        if(find && strstr(tmpBuf,"8000"))
            break;
        if(!find && strstr(tmpBuf,brName))
            find = 1;
            
        if(find && (ifNameTmp=getifNameFormBuf(tmpBuf)))
        {
            strcpy(ifNames[index],ifNameTmp);
            ifNames[index][strlen(ifNames[index])-1]='\0';
            index++;
        }
    }
    pclose(fp);
    return index;
}

int charInString(char c,const char*str)
{
	int i=0,len=strlen(str);
	for(i=0;i<len;i++)
		if(c==str[i])
		{			
			return 1;
		}
	return 0;
}

int changeDividerToESC(char *src,unsigned int size,const char*dividerChars)
{
	int srclen=0,i=0,j=0;
	if(!src||!dividerChars)
	{
		printf("%s : input value is null!\n",__FUNCTION__);
		return -1;
	}
	srclen=strlen(src);
	if(srclen>=size)
	{
		printf("%s : invalid input value!\n",__FUNCTION__);
		return -1;
	}
	for(i=srclen-1;i>=0;i--)
	{
		if(charInString(src[i],dividerChars))
		{
			srclen++;
			if(srclen>=size)
			{
				printf("%s : over size!\n",__FUNCTION__);
				return -1;
			}
			for(j=srclen-1;j>i;j--)
			{
				src[j]=src[j-1];
			}
			//assert(j==i);
			src[j]='\\';			
		}
	}
	return 0;
}

#endif
