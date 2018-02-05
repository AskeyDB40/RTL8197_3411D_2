#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>


#ifdef SAMBA_WEB_SUPPORT
#ifndef HOME_GATEWAY
#define HOME_GATEWAY
#endif

#include "apmib.h"
#include "mibtbl.h"
#endif

#define CHECK_TIME 2 //s
#define USB_STORAGE_MAP_FILE "/tmp/usb/mnt_map"
#define SAMBA_CONF_FILE "/var/samba/smb.conf"
#define USB_STORAGE_DIR "/var/tmp/usb"

typedef struct UsbDevListNode_
{
	struct UsbDevListNode_ * next;
	char name[64];
} UsbDevListNode,*pUsbDevListNode;

static UsbDevListNode usbDevListHeader={0};

#if defined(SAMBA_WEB_SUPPORT)
#include <unistd.h>

#define SAMBA_SECURITY "security"
#define SAMBA_SECURITY_SHARE "share"
#define SAMBA_SECURITY_USER "user"

#define SAMBA_MANAGEMENT_ACCOUNT "root"
#define SAMBA_SHAREINFO_TBL "STORAGE_SHAREINFO_TBL"
#define SAMBA_SHAREINFO_READONLY 1
#define SAMBA_SHAREINFO_READWRITE 2

#define SAMBA_ANONYMOUS_ACCESS_DIR "public"
#define SAMBA_ANONYMOUS_DEFAULT_DISK "sda1"


extern char *optarg;
extern int optind;
static int fd = 0;
static FILE *fp = NULL;
static int anonymous = 0;
static unsigned char manage_account[32] = {0};
typedef struct _share_folder_info
{
	unsigned char foldername[32];
	unsigned char path[48];
	unsigned char account[32]; 
	unsigned char permission;
}share_folder_info;

static void terminalHandler(int signo)
{
	pUsbDevListNode listPtr = NULL;
	pUsbDevListNode tmpPtr = NULL;
	//printf("%s %d catch signal %d\n", __FUNCTION__, __LINE__, signo);

	//free fd/fp and memory, then exit
	if (fd > 0)
	{
		close(fd);
		fd = -1;
	}

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	
	listPtr=usbDevListHeader.next;
	//printf("\n free usbdev list\n");
	while(listPtr!=NULL)
	{
		//printf("%s->",listPtr->name);
		tmpPtr = listPtr;
		listPtr=listPtr->next;
		free(tmpPtr);
	}
	//printf("null\n");
	
    exit(0);
}

void storageUpdateSambaConf(void)
{
	static char linebuffer[512] = {0};
	static char buffer1[512] = {0};
	
	int anon_access = 0, change_flag = 0;
	int line_len = 0;
	int len = 0;
	int res;
	char *p = NULL;

	fp = fopen(SAMBA_CONF_FILE, "r+");
	if(fp == NULL)
	{
		printf("open smb.conf error\n");
		return ;
	}
	
	//apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anon_access);
	//printf("%s %d anon_access=%d\n", __FUNCTION__, __LINE__, anon_access);
	while(fgets(linebuffer, 512, fp))
	{
		line_len = strlen(linebuffer);
		len += line_len;
		/* skip comment line */
		if ((linebuffer[0] == '#') || (linebuffer[0] == ';'))
			continue;
		
		/* find security */
		if (strstr(linebuffer, SAMBA_SECURITY))
		{
			p = linebuffer + strlen(SAMBA_SECURITY);
			/* enable Anonymous access and security = share or 
				disable Anonymous access and security = user
				just continue
			 */
			if ((anonymous && strstr(p, SAMBA_SECURITY_SHARE))||
				(!anonymous && strstr(p, SAMBA_SECURITY_USER)))
			{
				//printf("%s %d anonymous=%d p=%s\n", __FUNCTION__, __LINE__, anonymous, p);
				continue;
			}
			/* roll back to line header */
			len -= strlen(linebuffer);
			res = fseek(fp, len, SEEK_SET);
			if(res < 0)
			{
				printf("%s %d fseek\n", __FUNCTION__, __LINE__);
				return ;
			}
			sprintf(buffer1, "\t%s = %s  \n", SAMBA_SECURITY, anonymous?SAMBA_SECURITY_SHARE:SAMBA_SECURITY_USER);
			//printf("%s %d buffer1=%s\n", __FUNCTION__, __LINE__, buffer1);
			/*
			* write to file......
			*/
			fprintf(fp, "%s", buffer1);
			//fclose(fp);
			change_flag = 1;
			/* modify smb.conf end , just break */
			break;
		}
	}
	#if 0
	if (change_flag)
	{
		/* restart samba */
		printf("%s %d restart samba\n", __FUNCTION__, __LINE__);
		
		system("killall smbd 2> /dev/null");
		system("killall nmbd 2> /dev/null");
		
		system("smbd -D");
		system("nmbd -D");
	}
	#endif
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	
	return;
}

int addAnonShareFolder(int fd, unsigned char *devname)
{
	unsigned char buff[256] = {0}, tmpbuf[128] = {0};
	
	if (fd < 0 || !devname)
		return -1;

	//make public dir first
	bzero(tmpbuf,sizeof(tmpbuf));
	sprintf(tmpbuf, "mkdir /tmp/usb/%s/%s", devname, SAMBA_ANONYMOUS_ACCESS_DIR);
	system(tmpbuf);

	//modify config file
	bzero(buff,sizeof(buff));
	sprintf(buff,"\n[%s]\n comment = Share Folder %s \n path = /tmp/usb/%s/%s\n",
		SAMBA_ANONYMOUS_ACCESS_DIR,SAMBA_ANONYMOUS_ACCESS_DIR,devname, SAMBA_ANONYMOUS_ACCESS_DIR);
	if(write(fd,buff,strlen(buff))<0)
	{
		return -1;
	}
	
	bzero(buff,sizeof(buff));
	sprintf(buff," read only = yes\n public = yes\n oplocks = no\n kernel oplocks = no\n create mask = 0777\n browseable = yes\n guest ok = yes\n directory mask = 0777\n");
	//printf("%s %d buff=%s \n", __FUNCTION__, __LINE__, buff);
	if(write(fd,buff,strlen(buff))<0)
	{
		return -1;
	}

	return 0;
	
}


int addShareFolder(int fd)
{
	unsigned char buff[512] = {0}, tmpbuf[128] = {0};
	STORAGE_SHAREINFO_T shareinfo_entry;
	int k = 0;
	int entryNum;	
	
	if (fd < 0)
		return -1;	
	
	if(!apmib_get(MIB_STORAGE_SHAREINFO_TBL_NUM,(void *)&entryNum)){
		printf("Get storage share info num error!");
		return -1;
	}

	for (k = 1; k <= entryNum; k++)
	{		
		memset((void *)&shareinfo_entry, '\0', sizeof(STORAGE_SHAREINFO_T));
		*((char *)&shareinfo_entry) = (char)k;
		if ( !apmib_get(MIB_STORAGE_SHAREINFO_TBL, (void *)&shareinfo_entry)) {
			printf("Get storage share info table entry error!");
			return -1;
		}

		if (shareinfo_entry.storage_sharefolder_name[0] == '\0' ||
			shareinfo_entry.storage_sharefolder_path[0] == '\0' ||
			shareinfo_entry.storage_account[0] == '\0')
			{	
				continue;
			}
		
		bzero(buff,sizeof(buff));
		sprintf(buff,"\n[%s]\n comment = Share Folder %s \n path = %s/%s\n",
			shareinfo_entry.storage_sharefolder_name,shareinfo_entry.storage_sharefolder_name,
			shareinfo_entry.storage_sharefolder_path, shareinfo_entry.storage_sharefolder_name);
		
		if(write(fd,buff,strlen(buff))<0)
		{
			return -1;
		}
		
		bzero(buff,sizeof(buff));
		sprintf(buff," read only = no\n writeable = yes\n public = yes\n oplocks = no\n kernel oplocks = no\n create mask = 0777\n browseable = yes\n guest ok = yes\n directory mask = 0777\n");
		if(write(fd,buff,strlen(buff))<0)
		{
			return -1;
		}
		
		bzero(buff,sizeof(buff));
		if (shareinfo_entry.storage_permission == SAMBA_SHAREINFO_READWRITE)
			sprintf(buff, " admin users = %s,%s\n valid users = %s,%s\n", manage_account, shareinfo_entry.storage_account, manage_account, shareinfo_entry.storage_account);
		else
			sprintf(buff, " admin users = %s\n valid users = %s,%s\n", manage_account, manage_account, shareinfo_entry.storage_account);
		
		if (shareinfo_entry.storage_permission == SAMBA_SHAREINFO_READWRITE)
			sprintf(tmpbuf, " read list = %s,%s\n write list = %s,%s\n", manage_account, shareinfo_entry.storage_account, manage_account, shareinfo_entry.storage_account);
		else
			sprintf(tmpbuf, " read list = %s,%s\n write list = %s\n", manage_account, shareinfo_entry.storage_account, manage_account);
		strcat(buff, tmpbuf);
		//printf("%s %d buff=%s \n", __FUNCTION__, __LINE__, buff);
		if(write(fd,buff,strlen(buff))<0)
		{
			return -1;
		}
		memset(tmpbuf, '\0', sizeof(tmpbuf));
	
	}	
	
	return 0;
}


/* usage:  usbStorageAppController -a 1
  *            -a means anonymous
  */
static void usage(const char *programname)
{
	fprintf(stderr,"Usage: %s [-a 1/0]\n",programname);
	fprintf(stderr,"       -a means anonymous access, 1:enable anonymous access, 0:disable anonymous access\n");

}
static void parseCommand(int argc,char*argv[])
{
	int c;						/* command line arg */

	while ((c = getopt(argc, argv, "a:")) != -1) {
		switch (c) {
			case 'a':
				anonymous = atoi(optarg);
				//printf("%s %d anonymous=%d optarg=%s\n", __FUNCTION__, __LINE__, anonymous, optarg);
				break;
			default:
				usage(argv[0]);
				exit(1);
		}
	}

	return;
}

#endif

void dump_usbDevList()
{
	pUsbDevListNode listPtr=usbDevListHeader.next;
	printf("\ndump usbdev list\n");
	while(listPtr!=NULL)
	{
		printf("%s->",listPtr->name);
		listPtr=listPtr->next;
	}
	printf("null\n");
}

//static pUsbDevListNode usbDevlistHeader_new;
/****************************************
	return 0: not change,not update
	return 1: changed and updated
****************************************/
static int updateUsbDevList()
{
	DIR   *dir=NULL;   
	struct   dirent   *ptr=NULL;   
	int   retVal=0;
	pUsbDevListNode listPtr=&usbDevListHeader;
	char fullPath[128]={0};
	struct stat statbuf={0};
	//printf("%s:%d\n",__FUNCTION__,__LINE__);
	//dump_usbDevList();

	dir   =opendir(USB_STORAGE_DIR);

	while((ptr=readdir(dir))!=NULL)
	{
		//printf("%s:%d read name=%s\n",__FUNCTION__,__LINE__,ptr->d_name);
		  if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
		  	continue;
		  bzero(fullPath,sizeof(fullPath));
		  sprintf(fullPath,"%s/%s",USB_STORAGE_DIR,ptr->d_name);
		  if(lstat(fullPath,&statbuf)<0)
		  	continue;
		//  printf("%s:%d read name=%s\n",__FUNCTION__,__LINE__,ptr->d_name);

		  if(S_ISDIR(statbuf.st_mode)==0)
		  	continue;
		  if(strcmp(ptr->d_name,"sda")==0)
		  	continue;
		  //printf("%s:%d\n",__FUNCTION__,__LINE__);
		  if(listPtr->next==NULL)
		  {//have new usbDev
		  	retVal=1;
			if((listPtr->next=(pUsbDevListNode)malloc(sizeof(UsbDevListNode)))==NULL)
			{
				printf("malloc fail!\n");
				return -1;
			}
			
			bzero(listPtr->next,sizeof(UsbDevListNode));
			strcpy(listPtr->next->name,ptr->d_name);
			listPtr=listPtr->next;
			//printf("%s:%d add name %s\n",__FUNCTION__,__LINE__,ptr->d_name);
			//dump_usbDevList();
		  }
		  else
		  {
		  	if(strcmp(ptr->d_name,listPtr->next->name)==0)
		  	{//the same dir
				//printf("%s:%d name=%s\n",__FUNCTION__,__LINE__,listPtr->next->name);
		  		listPtr=listPtr->next;
		  	}
			else
			{//not the same,changed
				//printf("%s:%d\n",__FUNCTION__,__LINE__);
				retVal=1;
				bzero(listPtr->next->name,sizeof(listPtr->next->name));
				strcpy(listPtr->next->name,ptr->d_name);
		  		listPtr=listPtr->next;
			}
		  }
	}
	closedir(dir);
	if(listPtr->next!=NULL)
	{
		pUsbDevListNode listPtrTmp=listPtr->next;
		listPtr->next=NULL;
		listPtr=listPtrTmp;
		retVal=1;
		//printf("%s:%d\n",__FUNCTION__,__LINE__);
		while(listPtrTmp!=NULL)
		{
			listPtrTmp=listPtrTmp->next;
			//printf("%s:%d free name=%s\n",__FUNCTION__,__LINE__,listPtr->name);
			free(listPtr);
			listPtr=listPtrTmp;
		}
	}
	//if(retVal)
	//dump_usbDevList();
	return retVal;
}
static int reStartUsbStorageApps()
{	
	char buff[256]={0};
	#if !defined(SAMBA_WEB_SUPPORT)
	int fd=0;
	#endif
	#if defined(SAMBA_WEB_SUPPORT)
	unsigned tmp_buff[256] = {0};
	#endif
	pUsbDevListNode listPtr=usbDevListHeader.next;

	//dump_usbDevList();

	if(listPtr==NULL) return -1;
	system("killall smbd 2> /dev/null");
	system("killall nmbd 2> /dev/null");
	system("rm /var/samba/smb.conf");
	system("cp /etc/samba/smb.conf /var/samba/smb.conf");
	#if defined(SAMBA_WEB_SUPPORT)
	storageUpdateSambaConf();
	#endif
	fd=open(SAMBA_CONF_FILE,O_WRONLY|O_APPEND);
	if(fd<0)
	{
		printf("can't open %s\n",SAMBA_CONF_FILE);
		return -1;
	}
	//dump_usbDevList();

	while(listPtr!=NULL)
	{
		bzero(buff,sizeof(buff));
		sprintf(buff,"\n[%s]\n comment = Temporary file space %s \n path = /tmp/usb/%s\n",listPtr->name,listPtr->name,listPtr->name);
		if(write(fd,buff,strlen(buff))<0)
		{
			goto WRITE_FAIL;
		}
		bzero(buff,sizeof(buff));
		sprintf(buff," read only = no\n writeable = yes\n public = yes\n oplocks = no\n kernel oplocks = no\n create mask = 0777\n browseable = yes\n guest ok = yes\n directory mask = 0777\n");
		#if defined(SAMBA_WEB_SUPPORT)
		sprintf(tmp_buff, " admin users = %s\n valid users = %s\n read list = %s\n write list = %s\n", manage_account, manage_account, manage_account, manage_account);
		strcat(buff, tmp_buff);
		//fprintf(stderr,"#ifdef SAMBA_WEB_SUPPORT: %s, %d\n",__FUNCTION__,__LINE__);
		#else
		//fprintf(stderr,"#ifndef SAMBA_WEB_SUPPORT: %s, %d\n",__FUNCTION__,__LINE__);
		#endif
		if(write(fd,buff,strlen(buff))<0)
		{
			goto WRITE_FAIL;
		}
		#if defined(SAMBA_WEB_SUPPORT)
		if ((anonymous)&&(!strncmp(listPtr->name, SAMBA_ANONYMOUS_DEFAULT_DISK, strlen(SAMBA_ANONYMOUS_DEFAULT_DISK))))
		{
			addAnonShareFolder(fd, listPtr->name);
		}
		#endif
		listPtr=listPtr->next;
	}
	#if defined(SAMBA_WEB_SUPPORT)
		addShareFolder(fd);
	#endif
	close(fd);
	fd = -1;
	//printf("%s:%d\n",__FUNCTION__,__LINE__);

	//dump_usbDevList();
	sleep(1);
	system("smbd -D");
	//fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
	system("nmbd -D");
	//fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
	system("/bin/up_usbStorage_apps");
	return 0;
WRITE_FAIL:
	printf("can't write %s\n",SAMBA_CONF_FILE);
	close(fd);
	fd = -1;
	return -1;
	
}
static int killUsbStorageApps()
{
	system("killall smbd 2> /dev/null");
	system("killall nmbd 2> /dev/null");
	system("/bin/kill_usbStorage_apps");
}
static void timelyCheck(int signo)
{
	if(updateUsbDevList()<=0)//fail or not change
		return;
	if(usbDevListHeader.next==NULL)//have no usbDev
	{
		killUsbStorageApps();
	}
	else
	{//have new udbDev
		reStartUsbStorageApps();
	}	 
}
int main(int argc,char*argv[])
{
	signal(SIGALRM, timelyCheck);
	#if defined(SAMBA_WEB_SUPPORT)

	memcpy(manage_account, SAMBA_MANAGEMENT_ACCOUNT, strlen(SAMBA_MANAGEMENT_ACCOUNT));
	signal(SIGTERM, terminalHandler);
	parseCommand(argc, argv);

	if(!apmib_init()){
		printf("apmib init fail\n");
		return -1;
	}
	
	#endif
	while(1){		
		alarm(CHECK_TIME);
		pause();
	}
	return 0;
}
