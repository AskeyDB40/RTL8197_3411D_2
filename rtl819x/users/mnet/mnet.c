#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/types.h>
#include <linux/netlink.h>

#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>

#include "built_time"

//DONE: H21
//TODO: apmib should be shared library
//DONE: libusb should be shared library
//TODO: get usb info via libusb
//DONE: tune ppp interface MTU, refer set_wan.c
		//RunSystemCmd(NULL_FILE, "ifconfig", "ppp0", "mtu", tmp_args, "txqueuelen", "25",NULL_STR);
//DONE: special parameter in usb3g.chat & enable.pin, enable.userpass
//DONE: not execute, while web apply reboot
//DONE: remove mac entry in the web -> wan -> status

//--------------------------------------------------------------
#define DEMON		1
#define OUTPUT		0

//--------------------------------------------------------------
#define FAILURE		0
#define SUCCESS		1

#define DEV_NONE    0
#define DEV_ADD		1
#define DEV_REMOVE	2
#define DEV_ADD_DONT_DIAL 3

#define VENDOR		"Vendor"
#define PRODID		"ProdID"
#define LINUX_VID	"1d6b"

#define PPP_OPTION	"/var/usb3g.option"
#define PPP_CHAT	"/var/usb3g.chat"

#define MAX_RETRY   30
#define OFFHUB_THD  3

#define BUSYWAIT(X)	{while (X) { ; }}

enum functions {
	FUNC_NONE = 0,
	FUNC_SCSI,
	FUNC_TTY,
	FUNC_MAX
};

//int connect_type = 0;
int g_pwroff     = 0;

//--------------------------------------------------------------
#define DEF_BH_SHOWFLOW 0

#if DEF_BH_SHOWFLOW
#define BH_SHOWFLOW printf
#else
#define BH_SHOWFLOW(format, args...)
#endif

#define THIS_FILE "mnet.c"

//#define BH_DBG printf
#define BH_DBG(format, args...)
//BH_DBG("[bruce:%s:%d]\n", __FUNCTION__, __LINE__);


#define WORKAROUND_FOR_MPS 1
//--------------------------------------------------------------
struct usb3g_info_s {
	char pin[5];
	char apn[20];
	char dialnum[12];
	char user[32];
	char pass[32];
	char conntype[2];
	char idle[6];
	char mtu[6];
};

int get_device_info_from_proc(char *vender, char *prodid);
//--------------------------------------------------------------
#if defined(CONFIG_4G_LTE_SUPPORT)
struct dont_dial_s{
	char *devinfo;
};
static struct dont_dial_s dont_dial_list[] = {
	{ "12d1:1f01" },
	{ "12d1:14db" },
	{ "1ab7:1761" },
};
const int dont_dial_num = sizeof(dont_dial_list)/sizeof(struct dont_dial_s);

#define WEB_PID_FILENAME			("/var/run/webs.pid")
#define AT_BUFFER_LEN	(256)

/*-1 meas error or not found*/
int get_value_from_flash(char *mib)
{
	char buffer[AT_BUFFER_LEN], *ptr, fmt[32];
	int fd;
	int value;

	snprintf(buffer,sizeof(buffer),"flash get %s > /var/flash_get.txt",mib);
	system(buffer);

	fd = open("/var/flash_get.txt",O_RDWR);
	if(fd < 0)
		return -1;
	
	read(fd,buffer,AT_BUFFER_LEN-1);

	/*like xxx=a*/
	if(ptr=strstr(buffer,mib) != NULL){
		snprintf(fmt,sizeof(fmt),"%s=%s",mib,"%d");	 
		sscanf(buffer,fmt,&value); 
		return value;
	}
	close(fd);
	return -1;
}
int is_lte_wan()
{
	int lte4g;
	int wan_dhcp;
	lte4g=get_value_from_flash("LTE4G");
	wan_dhcp=get_value_from_flash("WAN_DHCP");
	if((lte4g == 1) && (wan_dhcp ==1))
		return 1;
	else
		return 0;
}

int is_usb3g_wan()
{
	int usb3g;
	usb3g=get_value_from_flash("WAN_DHCP");
	if(usb3g == 16)
		return 1;
	else
		return 0;
}

int changto_lte_wan()
{
	system("flash set WAN_DHCP 1");
	system("flash set LTE4G 1");
}
int changto_usb3g_wan()
{
	system("flash set WAN_DHCP 16");
	system("flash set LTE4G 0");
}

int reinit_system()
{
#if 0
	system("init.sh gw all");
#else
    FILE *fp;
    char line[100];
    pid_t pid;
    char pidcase[30];
#ifdef REINIT_VIA_RELOAD_DAEMON
    strcpy(pidcase,"reload daemon");
    if ((fp = fopen("/var/run/rc.pid", "r")) != NULL)
#else
    strcpy(pidcase,"web server daemon");
    if ((fp = fopen(WEB_PID_FILENAME, "r")) != NULL)
#endif
    {
        fgets(line, sizeof(line), fp);
        if ( sscanf(line, "%d", &pid) ) {
            if (pid > 1) {
                printf("Start Reinit via(%s)(pid=%d)\n",pidcase , pid );
                kill(pid, SIGUSR1);
            }
        }
        fclose(fp);
    }
    else{
        printf("open pid(%s) fail\n",pidcase );
    }
#endif
}
		
#ifdef CONFIG_LEADCORE_4G_SUPPORT
int is_leadcore=0;
int leadcore_check_cops(void)
{	
	char buffer[AT_BUFFER_LEN];
	int fd;

	unlink("/var/leadcore_log.txt");
	system("main_lc5761 /dev/ttyUSB0 \"AT+COPS?\" > /var/leadcore_log.txt");

	fd = open("/var/leadcore_log.txt",O_RDWR);
	if(fd < 0)
		return 0;
	read(fd,buffer,AT_BUFFER_LEN-1);

	if(strstr(buffer,"CHINA MOBILE") != NULL){
		printf("COPS OK:%s:%d\n",__func__,__LINE__);
		close(fd);
		return 1;
	}
	
	close(fd);
	return 0;
	
}

/* check 23g status */
int leadcore_check_CREG()
{
	int state,n;
	char buffer[AT_BUFFER_LEN];
	int fd;
	char *ptr  = NULL;

	unlink("/var/leadcore_log.txt");
	system("main_lc5761 /dev/ttyUSB0 \"AT+CREG?\" > /var/leadcore_log.txt");

	fd = open("/var/leadcore_log.txt",O_RDWR);
	if(fd < 0)
		return 0;
	read(fd,buffer,AT_BUFFER_LEN-1);

	ptr = strstr(buffer,"+CREG:");
	if(ptr != NULL){
		sscanf(buffer,"+CREG: %d,%d",&n,&state);
		if(state == 1|| state == 5){
			close(fd);
			return 1;
		}
		
	}
	
	close(fd);
	return 0;

}

/* check 4g status */
int leadcore_check_CEREG()
{
	int state,n;
	char buffer[AT_BUFFER_LEN];
	int fd;
	char *ptr  = NULL;

	unlink("/var/leadcore_log.txt");
	system("main_lc5761 /dev/ttyUSB0 \"AT+CEREG?\" > /var/leadcore_log.txt");

	fd = open("/var/leadcore_log.txt",O_RDWR);
	if(fd < 0)
		return 0;
	read(fd,buffer,AT_BUFFER_LEN-1);

	ptr = strstr(buffer,"+CEREG:");
	if(ptr != NULL){
		sscanf(buffer,"+CEREG: %d,%d",&n,&state);
		if(state == 1|| state == 5){
			close(fd);
			return 1;
		}
		
	}
	
	close(fd);
	return 0;
	
}

int ld_connected(char *fname)
{
    FILE *fp;
    char buffer[128];
    char *ptr  = NULL;

    fp = fopen(fname, "r");
    if (fp==NULL)
        return FAILURE;

    while (NULL != fgets(buffer, sizeof(buffer),fp))
    {
        ptr = strstr(buffer,"DPPPI");
        if (ptr) {
    		fclose(fp);
            return SUCCESS;
        }
    }
    fclose(fp);

    return FAILURE;

}


int leadcore_check_rssi(void)
{
	FILE *fp;
	int rssi,ber;
	char* ptr;
	char buffer[AT_BUFFER_LEN];

	unlink("/var/leadcore_log.txt");
	system("main_lc5761 /dev/ttyUSB0 \"AT+CSQ\" > /var/leadcore_log.txt");

	fp = fopen("/var/leadcore_log.txt", "r");
    if (fp==NULL)
        return FAILURE;

	while(NULL != fgets(buffer, sizeof(buffer),fp)){
		ptr = strstr(buffer,"+CSQ:");
		
		if(ptr != NULL){
			sscanf(buffer,"+CSQ: %d,%d",&rssi,&ber);

			if(rssi < 200){
				/* 23g */
				fclose(fp);
				return 1;
			}else{
				/* 4g */
				fclose(fp);
				return 2;
			}
		}
	}
	
	fclose(fp);
	return FAILURE;	
}

static int leadcore_23g_cid_active(void)
{
	int state,cid;
	char buffer[AT_BUFFER_LEN];
	FILE *fp;
	char *ptr  = NULL;

	unlink("/var/leadcore_log.txt");
	system("main_lc5761 /dev/ttyUSB0 \"AT+CGACT?\" > /var/leadcore_log.txt");


	fp = fopen("/var/leadcore_log.txt", "r");
    if (fp==NULL)
        return FAILURE;

	while(NULL != fgets(buffer, sizeof(buffer),fp)){
		ptr = strstr(buffer,"+CGACT:");
		
		if(ptr != NULL){
			sscanf(buffer,"+CGACT: %d,%d",&cid,&state);

			if(cid == 1 && state == 1){
				fclose(fp);
				return 1;
			}else{
				fclose(fp);
				return 0;
			}
		}
	}
	
	fclose(fp);
	return 0;
}


int connect_23g()
{
	int count=0;
	
	if(is_leadcore)
	{
		if(leadcore_23g_cid_active() == 0){
			/* inactive */
			system("main_lc5761 /dev/ttyUSB0  \"AT+CGDCONT=1,\\\"IP\\\",\\\"cmnet\\\",,0,0\""); 
			//sleep(1);
			system("main_lc5761 /dev/ttyUSB0  \"AT+CGEQREQ=1,2,64,64,0,0,0,1500,\\\"0E0\\\",\\\"0E0\\\",,0,0\"");
			//sleep(1);
			system("main_lc5761 /dev/ttyUSB0  \"AT+CGACT=1,1\""); 
		}

		unlink("/var/ldconnect.txt");

#if 1
		while(!ld_connected("/var/ldconnect.txt") && (count < 20))
		{
			//printf("%s %d count %d\n",__FUNCTION__,__LINE__,count);
			count++;	
			if(count % 2 == 0)
				system("main_lc5761 /dev/ttyUSB0 \"AT+CGDATA=\\\"M-0000\\\",1\" >> /var/ldconnect.txt");
			else
				system("main_lc5761 /dev/ttyUSB2 \"AT+CGDATA=\\\"M-0000\\\",1\" >> /var/ldconnect.txt");
			sleep(2);
		}
		if(count==20)
		{
			/*reset ldcore*/
			printf("%s %d count %d\n",__FUNCTION__,__LINE__,count);
			//system("main_lc5761 /dev/ttyUSB0 \"AT^DSRST\"");
			return FAILURE;
		}
#endif
	}

	return SUCCESS;
}

int connect_4g()
{
	if(is_leadcore)
	{
		int count=0;
		unlink("/var/ldconnect.txt");
		unlink("/var/4g_dial_log.txt");
		while(!ld_connected("/var/ldconnect.txt") && (count < 20))
		{
			printf("%s %d count %d\n",__FUNCTION__,__LINE__,count);
			count++;
			
			if(count % 2 == 0){
				system("main_lc5761 /dev/ttyUSB0 \"AT+CGDATA=\\\"M-0000\\\",1\" > /var/ldconnect.txt");
			}
			else
				system("main_lc5761 /dev/ttyUSB2 \"AT+CGDATA=\\\"M-0000\\\",1\" > /var/ldconnect.txt");

			system("cat  /var/ldconnect.txt >> /var/4g_dial_log.txt");
			sleep(2);
		}
		if(count==20)
		{
			/*reset ldcore*/
			printf("%s %d count %d\n",__FUNCTION__,__LINE__,count);
			//system("main_lc5761 /dev/ttyUSB0 \"AT^DSRST\"");
			return FAILURE;
		}
	}

	return SUCCESS;
}
#endif //CONFIG_LEADCORE_4G_SUPPORT

int is_lte_4g_exist(char *devinfo)
{
	FILE *fp;
	int match_count;
	char *ptr,buffer[256],ID_str[16];
	
	unlink("/vart/usbdevices.txt");
	system("busybox lsusb > /var/usbdevices.txt");
    fp = fopen("/var/usbdevices.txt", "r");
    if (fp==NULL)
        return FAILURE;
	
	match_count=0;

    while (NULL != fgets(buffer, sizeof(buffer),fp))
    {
    	memset(ID_str,0x0,sizeof(ID_str));
        ptr = strstr(buffer,"ID ");
        if (ptr) {
			int count=0;
			/*skip "ID "*/
			ptr+=3;
			memset(ID_str,0,sizeof(ID_str));
			while((*ptr != ' ') && (*ptr != '\r') && (*ptr != '\n') && (count< sizeof(ID_str)))
			{
				ID_str[count++]=*(ptr++);
			}
			for(count=0;count<dont_dial_num;count++)
			{
				if(!strcmp(ID_str,dont_dial_list[count].devinfo))
				{
					strncpy(devinfo,ID_str,sizeof(ID_str));
					match_count++;
				}
			}
        }
    }
	
    fclose(fp);
	return match_count;
}

void lte_start_dhcpc()
{
	struct stat fst;
	/*start udhcpc if not exist*/
	if ((stat("/etc/udhcpc/udhcpc-usb0.pid", &fst) < 0)) {
		system("udhcpc -i usb0 -p/etc/udhcpc/udhcpc-usb0.pid -s /usr/share/udhcpc/usb0.sh -h Realtek-a30");
	}
	return;
}
	
#endif /* #if defined(CONFIG_4G_LTE_SUPPORT) */

int isFileExisted(char *filename)
{
	struct stat fst;
	if ( stat(filename, &fst) < 0)
		return FAILURE; //not exist
	return SUCCESS;
}

int isPPPRunning(void)
{
	FILE   *fp;
	char buffer[8];
	/* check ppp pid */
	fp = fopen("/var/run/ppp0.pid", "r");
	if (fp ==NULL)
		return FAILURE;
		
	if (fgets(buffer, sizeof(buffer),fp))
		if (-1 != getpgid(atoi(buffer)))
			return FAILURE;

	return SUCCESS;
}

int onoff_usbhub(void)
{
	int sys_ret = 0;

	BH_SHOWFLOW("[%s:%s:%d] turn off & on the power of hub\n", THIS_FILE, __FUNCTION__, __LINE__);
#if 1
    sys_ret = system("hub-ctrl -b 1 -d 2 -P 1 -p");
	sleep(1);
    sys_ret = system("hub-ctrl -b 1 -d 2 -P 1 -p 1");
	sleep(1);
#elif 0
	sys_ret = system("hub-ctrl -P 1");
	sleep(1);
	sys_ret = system("hub-ctrl -P 1 -p 1");
	sleep(1);
#else
	system("iwpriv wlan0 write_mem dw,b8021054,1,100000");
#endif
	return SUCCESS;
}

int need_a_poweroff(void)
{
	BH_SHOWFLOW("[%s:%s:%d]\n", THIS_FILE, __FUNCTION__, __LINE__);
	g_pwroff = 0;
	onoff_usbhub();
	return 0;
}

#define USB3G_CONN   1
#define USB3G_DISC   2
#define USB3G_INIT   3
#define USB3G_DIAL   4
#define USB3G_REMOVE 5

void export_status(int stat)
{
	switch (stat) {
	case USB3G_CONN:
		system("echo \"conn\" > /var/usb3g.stat");
		break;
	case USB3G_DISC:
		system("echo \"disc\" > /var/usb3g.stat");
		break;
	case USB3G_INIT:
		system("echo \"init\" > /var/usb3g.stat");
		break;
	case USB3G_DIAL:
		system("echo \"dial\" > /var/usb3g.stat");
		break;
	case USB3G_REMOVE:
		system("echo \"remove\" > /var/usb3g.stat");
		break;
	}
}

void kill_ppp_inet(void)
{
	system("killall ppp_inet >/dev/null 2>&1");
	system("killall pppd >/dev/null 2>&1");
	system("rm /etc/ppp/connectfile >/dev/null 2>&1");
}

int get_tty(char *ttyitf)
{
    FILE   *fp;
    char   buffer[64];
	int    sys_ret = 0;

	sys_ret = system("ls /sys/bus/usb-serial/devices > /var/tty-devices");
	fp = fopen("/var/tty-devices", "r");
	if (fp !=NULL && NULL != fgets(buffer, sizeof(buffer),fp)) {
		char *ptr = strstr(buffer, " ");
		if (ptr != NULL)
			return FAILURE;

		ptr = '\0';
		strcpy(ttyitf, buffer);
		BH_SHOWFLOW("[%s:%s:%d] first one is /dev/%s\n", THIS_FILE, __FUNCTION__, __LINE__, ttyitf);
		sys_ret = system("rm /var/tty-devices >/dev/null 2>&1");
		fclose(fp);

		return SUCCESS;
	}

	sys_ret = system("rm /var/tty-devices >/dev/null 2>&1");
	fclose(fp);
	return FAILURE;
}

int switch_to_modem(char *ttyitf)
{
    FILE   *fp;
    char   buffer[128];
	int    sys_ret = 0, idx = 0;

	if (FAILURE == isFileExisted("/var/usb_modeswitch.conf"))
		return FAILURE;

	export_status(USB3G_INIT);
	sys_ret = system("usb_modeswitch -c /var/usb_modeswitch.conf");

	if (get_tty(ttyitf)) {
		fp = fopen("/var/usb3g.tty", "r");
		if (fp==NULL)
			return FAILURE;

		fgets(buffer, sizeof(buffer),fp);
		sscanf(ttyitf, "ttyUSB%d", &idx);
		sprintf(ttyitf,"ttyUSB%d", idx+atoi(buffer));
		BH_SHOWFLOW("usb_modeswitch done! and ttyitf is [%s], idx[%d]\n", ttyitf, idx);
		sys_ret = system("rm /var/usb_modeswitch.conf >/dev/null 2>&1");
		fclose(fp);
		return SUCCESS;
	}

	return FAILURE;
}

/* TODO: apmib should be shared library */
int get_mib(char *val, char *mib)
{
	FILE *fp;
 	char buf[32];

	sprintf(buf, "flash get %s", mib);
    fp = popen(buf, "r");
	if (fp==NULL)
		return FAILURE;

	if (NULL == fgets(buf, sizeof(buf),fp)) {
		pclose(fp);
		return FAILURE;
	}

	strcpy(val, strstr(buf, "\"")+1);
	val[strlen(val)-2] = '\0';
	pclose(fp);
	return SUCCESS;
}

int get_mib_para(struct usb3g_info_s *info)
{
	get_mib(info->pin,		"USB3G_PIN");
	get_mib(info->apn,		"USB3G_APN");
	get_mib(info->dialnum,	"USB3G_DIALNUM");
	get_mib(info->user,		"USB3G_USER");
	get_mib(info->pass,		"USB3G_PASS");
	get_mib(info->conntype, "USB3G_CONN_TYPE");
	get_mib(info->idle,		"USB3G_IDLE_TIME");
	get_mib(info->mtu,		"USB3G_MTU_SIZE");
	
	return SUCCESS;
}

void gen_ppp_option(char *ttyitf)
{
    FILE *fp;
	struct usb3g_info_s info;
    char buf[128];
	char commentAPN[4];
#if WORKAROUND_FOR_MPS
	char commentCIMI[4];
	char vendorID[6];
	char productID[6];
#endif

	get_mib_para(&info);
	BH_SHOWFLOW("[%s:%s:%d] PPP parameter: apn[%s], dialnum[%s]\n", THIS_FILE, __FUNCTION__, __LINE__, info.apn, info.dialnum);

#if WORKAROUND_FOR_MPS
	get_device_info_from_proc(vendorID, productID);
#endif
	
    buf[0] = '\0';
	//if (!strcmp(info.dialnum, "#777"))
#if WORKAROUND_FOR_MPS
	if (strcmp(vendorID, "feed")) /* for mobilepeak */
#endif
		sprintf(buf+strlen(buf), "'OK' \"ATQ0 V1 E1  S0=0 &C0 &D2\"\n");
    if (strlen(info.pin) != 0)
        sprintf(buf+strlen(buf), "'OK' 'AT+CPIN=%s'\n", info.pin);
#if WORKAROUND_FOR_MPS
    if (!strcmp(vendorID, "feed")) /* for mobilepeak */
        sprintf(buf+strlen(buf), "'OK' 'AT%%B'\n");//TODO
#endif
    if (buf == NULL)
        sprintf(buf, "\n");

	commentAPN[0] = '\0';
	if (!strcmp(info.dialnum, "#777"))
		sprintf(commentAPN, "#");

#if WORKAROUND_FOR_MPS
	commentCIMI[0] = '\0';
	if (!strcmp(vendorID, "feed")) /* for mobilepeak */
		sprintf(commentCIMI, "#");
#endif

	fp = fopen(PPP_CHAT, "w");
	fprintf(fp,
			"ABORT 'NO DIAL TONE'\n"
			"ABORT 'NO ANSWER'\n"
			"ABORT 'NO CARRIER'\n"
			"ABORT DELAYED\n"
			"ABORT 'COMMAND NOT SUPPORT'\n"
			"\n"
			"'' ''\n"
			"'' 'ATZ'\n"
            "%s" //buf: PIN
			"\n"
			"SAY 'show device infomation...\\n'\n"
			"'OK' 'ATI'\n"
			"\n"
			"SAY 'show SIM CIMI...\\n'\n"
	#if WORKAROUND_FOR_MPS
			"%s" //commentCIMI, mobilepeak doesn't have cimi yet
	#endif
			"'OK' 'AT+CIMI'\n"
			"\n"
			"SAY 'set APN...\\n'\n"
			"%s" //commentAPN: enable APN or not
			"'OK' 'AT+CGDCONT=1,\"IP\",\"%s\"'\n"
			"\n"
			"SAY 'dial...\\n\\n'\n"
			"'OK' 'ATD%s'\n"
			"'CONNECT' ''\n",
            buf,
	#if WORKAROUND_FOR_MPS
			commentCIMI,
	#endif
			commentAPN, info.apn, info.dialnum);

	fclose(fp);

    buf[0] = '\0';
	if (atoi(info.conntype) == 1) {
        sprintf(buf+strlen(buf), "demand\n");
        sprintf(buf+strlen(buf), "idle %s\n", info.idle);
    }
    if (strlen(info.user) != 0)
        sprintf(buf+strlen(buf), "user %s\n", info.user);
    if (strlen(info.pass) != 0)
        sprintf(buf+strlen(buf), "password %s\n", info.pass);
    if (strlen(info.mtu) != 0) {
        sprintf(buf+strlen(buf), "mtu %s\n", info.mtu);
		sprintf(buf+strlen(buf), "mru %s\n", info.mtu);
	}
    if (buf == NULL)
        sprintf(buf, "\n");

	fp = fopen(PPP_OPTION, "w");
	fprintf(fp,
			//"-detach\n"
			"hide-password\n"
			"persist\n"
			"nodetach\n"
			"lcp-echo-interval 20\n"
			"lcp-echo-failure 3\n"
			"holdoff 2\n"
			"connect-delay 100\n"
			"noauth\n"
			"/dev/%s\n"
			"115200\n"
			"debug\n"
			"defaultroute\n"
			"ipcp-accept-local\n"
			"ipcp-accept-remote\n"
			"usepeerdns\n"
			"crtscts\n"
			"lock\n"
			"noccp\n"
			"noipdefault\n"
            "%s" //buf: ondemand, user, password, mtu
			"connect '/bin/chat -v -t10 -f %s'\n",
			ttyitf, buf, PPP_CHAT);

    fclose(fp);
}

int dial_ppp(void)
{
    char cmd[32], buf[4];
	int  sys_ret = 0, recheck = 5, connect_type = -1;
	
	get_mib(buf, "USB3G_CONN_TYPE");
	connect_type = atoi(buf);

	//sleep(1);
	
    if (connect_type == 2)
        return SUCCESS;

	export_status(USB3G_DIAL);
	//TODO: check ppp_inet
	kill_ppp_inet();
	sprintf(cmd, "ppp_inet -t 16 -c %d -x", connect_type);
printf("---> %s\n", cmd);
	sys_ret = system(cmd);

	/* check connect */
	while (recheck--)
	{
		if (SUCCESS == isFileExisted("/etc/ppp/link"))
			break;
		else
			sleep(1);
	}

	if (recheck<=0)
		export_status(USB3G_DISC);

	return SUCCESS;
}

int get_device_info_from_proc(char *vender, char *prodid)
{
	FILE *fp;
	char buffer[128];
    char *ptr  = NULL;

#ifdef KERNEL_3_10
	fp = popen("cat /sys/kernel/debug/usb/devices", "r");
#else
    fp = popen("cat /proc/bus/usb/devices", "r");
#endif
	if (fp==NULL)
		return FAILURE;

    while (NULL != fgets(buffer, sizeof(buffer),fp))
    {
        ptr = strstr(buffer, VENDOR);
        if (ptr) {
            if (strstr(buffer, LINUX_VID))
                /* if vender == LINUX, get next */
                continue;
            else {
                sscanf(ptr, "Vendor=%s ProdID=%s Rev*", vender, prodid);
                return SUCCESS;
            }
        }
    }
    pclose(fp);

	return FAILURE;
}

int exec_action_for_tty(int action, char *ttyitf)
{
	char buffer[64];
    char vender[6];
    char prodid[6];
	FILE *fp;
	int idx =0;
	int retry_count = 0;
	int leadcore_rssi;

	if (action == DEV_ADD
	#if defined(CONFIG_4G_LTE_SUPPORT)
		 || action == DEV_ADD_DONT_DIAL
	#endif
	) {

		if (get_tty(ttyitf)) {
	#if defined(CONFIG_4G_LTE_SUPPORT)
			if(action == DEV_ADD)
	#endif	
			{
				if (FAILURE == isFileExisted("/var/usb3g.tty"))
				{
					get_device_info_from_proc(vender, prodid);
					sprintf(buffer, "usb_modeswitch -f %s:%s -c /etc/usb_modeswitch.d/tty%s:%s", vender, prodid, vender, prodid);
					system(buffer);
					if (SUCCESS == isFileExisted("/var/usb3g.tty")) {
						fp = fopen("/var/usb3g.tty", "r");
						fgets(buffer, sizeof(buffer),fp);
						sscanf(ttyitf, "ttyUSB%d", &idx);
						sprintf(ttyitf,"ttyUSB%d", idx+atoi(buffer));
						fclose(fp);
					}
				}
				else {
					fp = fopen("/var/usb3g.tty", "r");
					fgets(buffer, sizeof(buffer),fp);
					sscanf(ttyitf, "ttyUSB%d", &idx);
					sprintf(ttyitf,"ttyUSB%d", idx+atoi(buffer));		
					fclose(fp);
				}
			}
		}

		system("mdev -s");//
		sleep(2);//

		BH_SHOWFLOW("ttyitf[%s], idx[%d]\n", ttyitf, idx);
#if defined(CONFIG_4G_LTE_SUPPORT)
		if (action == DEV_ADD) {
			gen_ppp_option(ttyitf);
			dial_ppp();
		} else if(action == DEV_ADD_DONT_DIAL)
		{
#if defined(CONFIG_LEADCORE_4G_SUPPORT)	
			if(is_leadcore) {
#if 1
			/* test code start*/
			/* force into 3g mode */
			/*
			system("main_lc5761 /dev/ttyUSB0 \"AT+CFUN=4\""); 
			sleep(1);
			*/
			//system("main_lc5761 /dev/ttyUSB0 \"AT^DSTMEX=1,2\"");	//if want use 3g, please open this code
			//sleep(1);
			/* test code end */
#endif
			//printf("DSTMEX OK,%s:%d\n",__func__,__LINE__);
			system("main_lc5761 /dev/ttyUSB0  \"AT+CMEE=1\""); // AT debug function,report  error info 
		
			system("main_lc5761 /dev/ttyUSB0 \"AT+CFUN=1\"");
			//printf("CFUN OK,%s:%d\n",__func__,__LINE__);

			/* check cops */
			#if 1
			for(retry_count = 0;retry_count < 10;retry_count++){
				if(leadcore_check_cops() == 1)
					break;
				else
					sleep(1);
			}
			#endif

			leadcore_rssi = leadcore_check_rssi();

			if(leadcore_rssi == 1){
				/* check CREG */
				for(retry_count = 0;retry_count < 10;retry_count++){
					if(leadcore_check_CREG() == 1)
						break;
					else
						sleep(1);
				}
				/* 3g */
				if(connect_23g()) {
					system("ifconfig usb0 up");
				}
			}else if(leadcore_rssi == 2){
				/* check CEREG */
				for(retry_count = 0;retry_count < 10;retry_count++){
					if(leadcore_check_CEREG() == 1)
						break;
					else
						sleep(1);
				}
			
				/* 4g */
				if(connect_4g()) {
					system("ifconfig usb0 up");
				}
			}
		}
#else
		/*other 4g dongle 4g on command*/	
			system("ifconfig usb0 up");
#endif
			/*assume all 4g dongle using dhcpc to get ip*/
			lte_start_dhcpc();
		}
#else
		gen_ppp_option(ttyitf);
		dial_ppp();
#endif
		return SUCCESS;
	}
    else if (action == DEV_REMOVE) {
        BH_SHOWFLOW("[%s:%s:%d] do nothing\n", THIS_FILE, __FUNCTION__, __LINE__);
		kill_ppp_inet();
		export_status(USB3G_REMOVE);
        return SUCCESS;
    }

	return FAILURE;
}

int exec_action(int action, int retry, char *ttyitf)
{
	/* MUST BE storage device !!! */
	if (action == DEV_ADD
	#if defined(CONFIG_4G_LTE_SUPPORT)
		 || action == DEV_ADD_DONT_DIAL
	#endif
	) {
		BH_SHOWFLOW("[%s:%s:%d] action == DEV_ADD\n", THIS_FILE, __FUNCTION__, __LINE__);
		if (switch_to_modem(ttyitf)) {
		#if defined(CONFIG_4G_LTE_SUPPORT)
			if (action == DEV_ADD) {
				gen_ppp_option(ttyitf);
				return dial_ppp();
			}
			else {
				return SUCCESS;
			}
		#else
			gen_ppp_option(ttyitf);
			return dial_ppp();
		#endif
		}
	}
	else if (action == DEV_REMOVE) {
		BH_SHOWFLOW("[%s:%s:%d] do nothing\n", THIS_FILE, __FUNCTION__, __LINE__);
		export_status(USB3G_REMOVE);
		return SUCCESS;
	}

	return FAILURE;
}

int find_profile(char *profile)
{
    FILE *fp;
    char buffer[128];
	int sys_ret = 0;

    fp = popen("ls -a /etc/usb_modeswitch.d", "r");
    while (NULL != fgets(buffer, sizeof(buffer),fp))
    {
        if (strstr(buffer, profile))
        {
            sprintf(buffer, "cp /etc/usb_modeswitch.d/%s /var/usb_modeswitch.conf", profile);
            sys_ret = system(buffer);
            pclose(fp);
			return SUCCESS;
        }
    }
    pclose(fp);
    return FAILURE;
}

#ifdef DEMON
char *id_pend_zero(char *id, char *buf)
{
	char *ptr = NULL;
	int pend_zero = 0, i;

	ptr = strstr(buf, "/");
	if (ptr) {
		ptr[0] = '\0';	

		pend_zero = 4-strlen(buf);
		strcpy(id+pend_zero, buf);
		if (pend_zero>0) {
			for (i = 0; i<pend_zero; i++)
				id[i] = '0';
		}
	}
	return ptr;
}

int parse_hotplug_info(char *buf, int len, char *devinfo, int *action)
{
    char *ptr;
	int i = 0, subsystem = 0, devtype = 0, product = 0;
	
	*action   = DEV_NONE;
	devtype   = 0;
	product   = 0;
	subsystem = 0;

	while (i < len) {
		if (*action == DEV_NONE) {
            if (strstr(buf+i,"ACTION=add"))
                *action = DEV_ADD;
            else if (strstr(buf+i,"ACTION=remove"))
                *action = DEV_REMOVE;
        }
		else if (subsystem == 0) {
            if (strstr(buf+i,"SUBSYSTEM=usb"))
                subsystem = 1;
		}
        else if (devtype == 0) {
			if (strstr(buf+i, "DEVTYPE=usb_device"))
				devtype = 1;
        }
        else if (*action != DEV_NONE && devtype == 1) {
            ptr = strstr(buf+i, "PRODUCT=");
            if (ptr) {
				char vid[5], pid[5];
				memset(vid, 0, sizeof(vid));
				memset(pid, 0, sizeof(pid));
				ptr = ptr+sizeof("PRODUCT=")-1;
				strncpy(devinfo, ptr, strlen(ptr));
				ptr = id_pend_zero(vid, devinfo);
				ptr = id_pend_zero(pid, ptr+1);		
				memset(devinfo, 0, sizeof(devinfo));
				sprintf(devinfo, "%s:%s", vid, pid);
				return find_profile(devinfo);
            }
        }

		i += strlen(buf+i) + 1;
	}

	return FAILURE;
}

int parse_hotplug_tty_info(char *buf, int len, char *devinfo, int *action)
{
	int i = 0, subsystem = 0, major = 0;

	*action   = DEV_NONE;
	while (i < len) {
		if (*action == DEV_NONE) {
            if (strstr(buf+i,"ACTION=add"))
                *action = DEV_ADD;
            else if (strstr(buf+i,"ACTION=remove"))
                *action = DEV_REMOVE;
        }
		else if (subsystem == 0) {
            if (strstr(buf+i,"SUBSYSTEM=tty"))
                subsystem = 1;
		}
        else if (major == 0) {
			if (strstr(buf+i, "MAJOR=188"))
				return SUCCESS;
        }

		i += strlen(buf+i) + 1;
	}

	return FAILURE;
}

int run_fork(int func, int action, char *devinfo)
{
	int retry = MAX_RETRY;
	pid_t pid = 0;
	char ttyitf[8];

	BH_SHOWFLOW("[%s:%s:%d]\n", THIS_FILE, __FUNCTION__, __LINE__);
	
	if ((pid = fork())< 0)
		printf("process[%d] fail to fork child\n", getpid());
	else if (pid == 0) {
		if ((pid = fork())< 0)
			printf("process[%d] fail to fork child\n", getpid());
		else if (pid > 0)
			exit(0);
		else {
            if (SUCCESS == isFileExisted("/var/mnet_fork")) {
				BH_SHOWFLOW("[%s:%s:%d] /var/mnet_fork is existed, exit(0)\n", THIS_FILE, __FUNCTION__, __LINE__);
				exit(0);
            }
            else
				system("echo \"\" > /var/mnet_fork >/dev/null 2>&1");
			
			if (func == FUNC_SCSI)
				exec_action(action, retry, ttyitf);
			else if (func == FUNC_TTY)
				exec_action_for_tty(action, ttyitf);

			system("rm /var/mnet_fork >/dev/null 2>&1");
			exit(0);
		}
	}
	waitpid(pid, NULL, 0);
	return SUCCESS;
}

int run_fork_modeswitch(void)
{
	pid_t pid = 0;
	char vender[6];
	char prodid[6];
	char buffer[32];

	if ((pid = fork())< 0)
		printf("process[%d] fail to fork child\n", getpid());
	else if (pid == 0) {
		if ((pid = fork())< 0)
			printf("process[%d] fail to fork child\n", getpid());
		else if (pid > 0)
			exit(0);
		else {
			get_device_info_from_proc(vender, prodid);
			sprintf(buffer, "usb_modeswitch -c /etc/usb_modeswitch.d/%s:%s  >/dev/null 2>&1", vender, prodid);
			system(buffer);
			exit(0);
		}
	}
	waitpid(pid, NULL, 0);
	return SUCCESS;
}

int demon(void)
{
    struct sockaddr_nl nls;
    struct pollfd pfd;
    char buf[512], devinfo[20];
	int action = DEV_NONE;
	int func   = 0;
	int len    = 0;
	int count=0,need_to_run=0;
	memset(&nls, 0, sizeof(nls));
	nls.nl_family = AF_NETLINK;
	nls.nl_pid = getpid();
	nls.nl_groups = -1;

	pfd.events = POLLIN;
	pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (pfd.fd == -1) {
		printf("Not root\n");
		exit(1);
	}

	if (bind(pfd.fd, (void*)&nls, sizeof(nls))) {
		printf("bind failed\n");
		exit(1);
	}

	if (g_pwroff == 1) {
		run_fork_modeswitch();
		//need_a_poweroff();
	}
	
	while (-1 != poll(&pfd, 1, -1)) {
		func = FUNC_NONE;
		len  = recv(pfd.fd, buf, sizeof(buf), MSG_DONTWAIT);
		if (len == -1) {
			printf("recv failed\n");
			//exit(1);
		}

		/* get device info */
		if (parse_hotplug_info(buf, len, devinfo, &action)) {
			BH_SHOWFLOW("\t*** Device %s[%d] ***\n", devinfo, action);
			func = FUNC_SCSI;
		}
		else if (parse_hotplug_tty_info(buf, len, devinfo, &action))
			func = FUNC_TTY;
	#if defined(CONFIG_4G_LTE_SUPPORT)
	{
		int i;
		for (i = 0; i < dont_dial_num; i++) {
			if (!strcmp(devinfo, dont_dial_list[i].devinfo)) {
				if(action == DEV_ADD)
					action = DEV_ADD_DONT_DIAL;
	#ifdef CONFIG_LEADCORE_4G_SUPPORT
				if(!strcmp(devinfo,"1ab7:1761") && (action == DEV_ADD_DONT_DIAL) && (func==FUNC_TTY))
				{
					is_leadcore=1;
					count++;
					if(count >= 3) {
						need_to_run=1;
						count=0;
					}
				}
	#endif		
				/*first time plug in*/
				if(!is_lte_wan() && is_usb3g_wan()){
					changto_lte_wan();
					reinit_system();
					return;
				}
				break;
			}
		}
	}

	if(action == DEV_ADD) /*3g dongle first plug in*/
	{
		if(is_lte_wan())
		{
			changto_usb3g_wan();
			reinit_system();
			return;
		}
	}
	#endif /* #if defined(CONFIG_4G_LTE_SUPPORT) */

		if (func==FUNC_SCSI || func==FUNC_TTY) {

	#ifdef CONFIG_LEADCORE_4G_SUPPORT
			if(action == DEV_ADD_DONT_DIAL) {
				if(is_leadcore && need_to_run) {
					run_fork(func, action, devinfo);
					need_to_run=0;
				}
			}
			else
	#endif			
				run_fork(func, action, devinfo);
		}
	#if defined(CONFIG_LEADCORE_4G_SUPPORT)
		is_leadcore=0;
	#endif
	}
	printf("mnet demon end...\n");

	return SUCCESS;
}
#endif /* #ifdef DEMON */

#ifdef OUTPUT
int output(void)
{
    struct sockaddr_nl nls;
    struct pollfd pfd;
    char buf[512];

	memset(&nls, 0, sizeof(nls));
	nls.nl_family = AF_NETLINK;
	nls.nl_pid = getpid();
	nls.nl_groups = -1;

	pfd.events = POLLIN;
	pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (pfd.fd == -1) {
		printf("Not root\n");
		exit(1);
	}

	if (bind(pfd.fd, (void*)&nls, sizeof(nls))) {
		printf("bind failed\n");
		exit(1);
	}
	while (-1 != poll(&pfd, 1, -1)) {
		int i, len = recv(pfd.fd, buf, sizeof(buf), MSG_DONTWAIT);
		if (len == -1) {
			printf("recv failed\n");
			//exit(1);
		}
		i = 0;
		while (i < len) {
			printf("%s\n", buf + i);
			i += strlen(buf+i) + 1;
		}
	}
	printf("mnet output end...\n");

	return SUCCESS;
}
#endif /* #ifdef OUTPUT */

int main(int argc, char *argv[])
{
	printf("\nmnet built on %s\n\n", BUILT_TIME);
	char devinfo[20];

#ifdef DEMON
	if (argc > 1 && !strcmp(argv[1], "-d")) {
		char ttyitf[16];
		//char buf[4];

        system("mdev -s");
        sleep(2);
#ifdef CONFIG_4G_LTE_SUPPORT
		if(is_lte_4g_exist(&devinfo)) {			
			if (SUCCESS == get_tty(ttyitf)) {
				if(!is_lte_wan() && is_usb3g_wan()){
					changto_lte_wan();
					reinit_system();
					return;
				}
#ifdef CONFIG_LEADCORE_4G_SUPPORT
				if(!strcmp(devinfo,"1ab7:1761")) {
					is_leadcore=1;
				}	
#endif			
				
				run_fork(FUNC_TTY, DEV_ADD_DONT_DIAL,"main()");	
#ifdef CONFIG_LEADCORE_4G_SUPPORT	
				is_leadcore=0;
#endif
			}
		} else 
#endif		
		{
			/* if usb was learnt as tty device at bootup */
			if (SUCCESS == get_tty(ttyitf)) {
#ifdef CONFIG_4G_LTE_SUPPORT
				if(is_lte_wan())
				{
					changto_usb3g_wan();
					reinit_system();
					return;
				}
#endif			
				
				run_fork(FUNC_TTY, DEV_ADD, "main()");
				sleep(1);
				BUSYWAIT(FAILURE == isFileExisted("/var/mnet_fork")); // wait here, till mnet_fork does not exist
	  		}
			if (FAILURE == isFileExisted("/var/run/ppp0.pid")) {
				g_pwroff=1;
			}
		}
		demon();
	}
#endif

#ifdef OUTPUT
	if (argc > 1 && !strcmp(argv[1], "-o"))
		output();
#endif

	if (argc > 1 && !strcmp(argv[1], "-p"))
		onoff_usbhub();

	if (argc > 1 && !strcmp(argv[1], "-r")) {
		if (isPPPRunning()) {
			//system("killall pppd >/dev/null 2>&1");
			kill_ppp_inet();
			sleep(3);
		}
		system("iwpriv wlan0 write_mem dw,b8021054,1,100000");
	}

	return 0;
}
