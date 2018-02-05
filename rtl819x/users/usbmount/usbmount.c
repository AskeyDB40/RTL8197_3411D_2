/* 
USB Mount Handler
andrew 2008/02/21

handles linux 2.6 hotplug event
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
//#include <linux/config.h>
//#include <config/autoconf.h>
#include <sys/mount.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "include/fs.h"
#include "include/debug.h"
//#define  CONFIG_CT_USBMOUNT_DIRECTORY
/*

Environment Variables:

ARGS 1
DEVPATH
ACTION

Partition Numbering..
sda		0
sda1		1
.....
sda15	15
sdb		16
sdb1		17
.....
sdb15	31


*/

#define NTFS_3G 1

static char CMD_UMOUNT_FMT[] = "/bin/umount %s";
static char CMD_MOUNT_FMT[] = "/bin/mount -t %s /dev/%s %s";
#ifdef CONFIG_EXFAT_FUSE
static char CMD_MOUNT_FMT_EXFAT_FUSE[] = "/bin/mount.exfat-fuse /dev/%s %s";
#endif
#if NTFS_3G
//static char  CMD_MOUNT_FMT_NTFS[] = "/bin/ntfs-3g /dev/%s %s -o silent,umask=0000";
static char  CMD_MOUNT_FMT_NTFS[] = "/bin/ntfs-3g /dev/%s %s -o force";
static char CMD_MOUNT_FMT_NTFS_KILL[] = "/etc/killntfs %s/%s%d";
#endif
static char CMD_MKDIR[]="mkdir %s";
// configure this!!
//static char MNT_ROOT[] = "/tmp/usb"; // directory where sda,sda1.. directory will be created for mounting.
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
#define MNT_ROOT "/mnt"
static char MNT_FILE[] = "/tmp/usb/mnt_map"; // file contains 32bit INT representing mounted paritions.
#else
#define MNT_ROOT "/var/tmp/usb"
static char MNT_FILE[] = MNT_ROOT"/mnt_map"; // file contains 32bit INT representing mounted paritions.
#define MNT_ROOT_SD "/var/tmp/mmc"
static char MNT_FILE_SD[] = MNT_ROOT_SD"/mnt_map"; 
#endif

struct action_table {
	char *action_str;
	int	(*action_func)(int, char **, char *);
};

char *fs_names[] = {
	"",
	"",
	"MBR",
	"EXT2",
	"EXT3",
	"FAT",
	"HFSPLUS",
	"",
	"NTFS",
	"",
	"EXT4",
	"EXFAT"
};

unsigned int file_read_number(const char *filename) 
{
	FILE *fp;
	char buffer[10];
	unsigned int mask = 0;
	
	fp = fopen(filename, "r");
	
	if (!fp) {		
		return 0;
	}

	fgets(buffer, sizeof(buffer), fp);
	if (1 != sscanf(buffer, "%u", &mask)) {
		goto out;
	}
	fclose(fp);
	return mask;
	
out:
	fclose(fp);
	return 0;
	
}




static const char *basename(const char *path) {	
	const char *s = path, *tmp;
	DEBUG("%s(1)\n", __FUNCTION__);
	
	#if 0//devel_debug
		snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
			"echo \"BASE:%s \" >> /tmp/usbmount\.log", 
			 __FUNCTION__);
		system(cmd_bufferx);
                #endif
	
	
	while ((tmp = strchr(s, '/'))!= 0) {
		DEBUG("%s(2 %s)\n", __FUNCTION__, tmp);
		
		#if 0//devel_debug
		snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
			"echo \"2 BASE:%s \" >> /tmp/usbmount\.log", 
			 tmp);
		system(cmd_bufferx);
                #endif
		
		
		s = &tmp[1];
	}
	DEBUG("%s(3 %s)\n", __FUNCTION__, s);
	if (strlen(s))
		return s;
	return 0;
}

static int _get_partition_id(const char *dev, const char *root) {
	char p;
	if (strncmp(dev, root, strlen(root)) == 0) {
		p = dev[3];
		if (p)
			return (p - '0');
		else
			return 0;
	}
	return -1;
}
void put_debug_message(char *message)
{
	char cmdBuff[512]={0};
	
sprintf(cmdBuff, "echo \"%s\" >> /var/usbmount_debug", message);
system(cmdBuff);

}
static unsigned int get_partition_id(const char *dev) {
	int tmp;
	int rv=0; 
	unsigned char char_start=0;
	unsigned char dev_str[4]={'s','d',0,0};
	unsigned char sd_dev_str[10]={'m','m','c','b','l','k','0','p',0, 0};
	unsigned int index=0;
#if 1
	if(!strncmp(dev, sd_dev_str, 6)){
		for(char_start='1';char_start<='4';char_start++){
			sd_dev_str[8]=char_start;
			if (!strncmp(dev, sd_dev_str, strlen(sd_dev_str))){				
				rv = sscanf(dev, "mmcblk0p%u", &tmp);
				if (rv> 0 ) {
					return 1 << tmp;
				}
			}
		}
	}
	else{		
		for(char_start='a';char_start<='z';char_start++){
			dev_str[2]=char_start;
			index=index+1;
			if(index % 2 ==0){
				if (strcmp(dev_str, dev)==0){
						return 1 << 16;
				}	
				if(!strncmp(dev, dev_str, strlen(dev_str))){
					rv = sscanf(dev+strlen(dev_str), "%u", &tmp);
					if (rv> 0 ) {
						return 1 << (tmp+16);
					}	
				}
				
			}else{
				if (strcmp(dev_str, dev)==0){
						return 1;
				}	
				if(!strncmp(dev, dev_str, strlen(dev_str))){
					rv = sscanf(dev+strlen(dev_str), "%u", &tmp);
					if (rv> 0 ) {
						return 1 << tmp;
					}	
				}
			}
		}
	}//!MMC
#else
	if (strcmp("sda", dev)==0)
		return 1;

	rv = sscanf(dev, "sda%u", &tmp);
	if (rv==1) {
		return 1 << tmp;
	}
	
	if (strcmp("sdb", dev)==0)
		return 1 << 16;
	
	rv = sscanf(dev, "sdb%u", &tmp);
	if (rv==1) {
		return 1 << (tmp + 16);
	}
#endif
	/*
	tmp = _get_partition_id(dev, "sda");
	ASSERT(tmp >= 0);
	if (tmp >= 0) {
		return 1 << tmp;
	}

	
	tmp = _get_partition_id(dev, "sdb");
	ASSERT(tmp >= 0);
	if (tmp >= 0) {
		return 1 << (tmp + 16);
	}
	*/

	return 0;
}

static unsigned int get_umount_partition_id(const char *dev) {
	FILE * fp;
	char buffer[10];
	unsigned int mask = 0;
	int ret=0;
	
	/*
	fp = fopen(MNT_FILE, "r");
	if (!fp) {		
		return ret;
	}

	fgets(buffer, sizeof(buffer), fp);
	sscanf(buffer, "%u", &mask);
	fclose(fp);
	*/
	mask = file_read_number(MNT_FILE);
	

	if(!strcmp(dev, "sda")){
		ret = mask&0xffff;
	}else if(!strcmp(dev, "sdb")){
		ret = mask&0xffff0000;
	} else {
		ret = mask;
	}
	
	return ret;
}

static int try_mount(const char *devnode, const char *mnt) {
	char cmd_buffer[220];
	int rv = -1;
	char source[128], block_name[4];

	if(strlen(devnode) <= 3)
		return rv;

	snprintf(source, sizeof(source), "/dev/%s", devnode);
	int fs = detect_fs(source);

#if devel_debug
	snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
		"echo \"%s: devnode = %s, source = %s, mnt = %s, fs_names[fs] = %s \" >> /tmp/usbmount\.log", 
		__FUNCTION__, devnode, source, mnt, fs_names[fs]);
	system(cmd_bufferx);
#endif

	if(fs == NONE)
		return rv;

	int ufsd_mod = 0;
	FILE *fp;
	fp = popen("lsmod | grep ufsd 2>&1", "r");
	if(fp) {
		while (fgets(cmd_buffer, sizeof(cmd_buffer)-1, fp) != NULL)
			++ufsd_mod;
		pclose(fp);
	}

	switch(fs) {
	case EXT2:
		if((rv = mount(source, mnt, "ext2"
			, MS_NODIRATIME | MS_NOATIME, 0)) < 0) {
			snprintf(cmd_buffer, sizeof(cmd_buffer), "e2fsck -a %s", source);

			if(system(cmd_buffer) < 0) {
				fprintf(stderr, "[%s][%d] %s failed!\n"
					, __FUNCTION__, __LINE__, cmd_buffer);
				break;
			}
#if devel_debug
			snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
				"echo \"%s: fsck ext2 finished!\" >> /tmp/usbmount\.log", 
				__FUNCTION__);
			system(cmd_bufferx);
#endif
			rv = mount(source, mnt, "ext2",
				MS_NODIRATIME | MS_NOATIME, 0);
		}
		break;
	case EXT3:
		if((rv = mount(source, mnt, "ext3"
			, MS_NODIRATIME | MS_NOATIME, 0)) < 0) {
			snprintf(cmd_buffer, sizeof(cmd_buffer), "e2fsck -a %s", source);

			if(system(cmd_buffer) < 0) {
				fprintf(stderr, "[%s][%d] %s failed!\n"
					, __FUNCTION__, __LINE__, cmd_buffer);
				break;
			}
#if devel_debug
			snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
				"echo \"%s: fsck ext3 finished!\" >> /tmp/usbmount\.log", 
				__FUNCTION__);
			system(cmd_bufferx);
#endif
			rv = mount(source, mnt, "ext3",
				MS_NODIRATIME | MS_NOATIME, 0);
		}
		break;
	case EXT4:
		if((rv = mount(source, mnt, "ext4"
			, MS_NODIRATIME | MS_NOATIME, 0)) < 0) {
			snprintf(cmd_buffer, sizeof(cmd_buffer), "e2fsck -a %s", source);

			if(system(cmd_buffer) < 0) {
				fprintf(stderr, "[%s][%d] %s failed!\n"
					, __FUNCTION__, __LINE__, cmd_buffer);
				break;
			}
#if devel_debug
			snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
				"echo \"%s: fsck ext4 finished!\" >> /tmp/usbmount\.log", 
				__FUNCTION__);
			system(cmd_bufferx);
#endif
			rv = mount(source, mnt, "ext4",
				MS_NODIRATIME | MS_NOATIME, 0);
		}
		break;
	case FAT:
#ifdef NLS_UTF8_SUPPORT
		rv = mount(source, mnt, "vfat", 
			MS_NODIRATIME | MS_NOATIME, "iocharset=utf8");
#else
		rv = mount(source, mnt, "vfat", 
			MS_NODIRATIME | MS_NOATIME,0);
#endif
		break;
	case HFSPLUS:
		if(ufsd_mod > 0) {
			if((rv = mount(source, mnt, "ufsd", 
				MS_NODIRATIME | MS_NOATIME, "nls=utf8")) < 0) {
				snprintf(cmd_buffer, sizeof(cmd_buffer), "chkhfs -f -a %s", source);
				
				if(system(cmd_buffer) < 0) {
					fprintf(stderr, "[%s][%d] %s failed!\n"
						, __FUNCTION__, __LINE__, cmd_buffer);
					break;
				}
#if devel_debug
				snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
					"echo \"%s: chkhfs finished!\" >> /tmp/usbmount\.log", 
					__FUNCTION__);
				system(cmd_bufferx);
#endif
				rv = mount(source, mnt, "ufsd"
					, MS_NODIRATIME | MS_NOATIME, "nls=utf8");
			}
		}
		break;
	case NTFS:
		if(ufsd_mod > 0) {
			if((rv = mount(source, mnt, "ufsd"
				, MS_NODIRATIME | MS_NOATIME, "nls=utf8")) < 0) {
				snprintf(cmd_buffer, sizeof(cmd_buffer), "chkntfs -f -a %s", source);
				
				if(system(cmd_buffer) < 0) {
					fprintf(stderr, "[%s][%d] %s failed!\n"
						, __FUNCTION__, __LINE__, cmd_buffer);
					break;
				}
#if devel_debug
				snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
					"echo \"%s: chkntfs finished!\" >> /tmp/usbmount\.log", 
					__FUNCTION__);
				system(cmd_bufferx);
#endif
				rv = mount(source, mnt, "ufsd"
					, MS_NODIRATIME | MS_NOATIME, "nls=utf8");
			}
		}
#if NTFS_3G
		else {
			snprintf(cmd_buffer, sizeof(cmd_buffer), CMD_MOUNT_FMT_NTFS,devnode,mnt);
			DEBUG("CMD: %s\n\n", cmd_buffer);
			rv = system(cmd_buffer);
			if(rv < 0) {
				fprintf(stderr, "[%s][%d] %s failed!\n"
					, __FUNCTION__, __LINE__, cmd_buffer);
				break;
			}
			rv = WIFEXITED(rv);
		}
#endif
		break;
	case EXFAT:
#ifndef CONFIG_EXFAT_FUSE
		if((rv = mount(source, mnt, "exfat"
			, MS_NODIRATIME | MS_NOATIME, "iocharset=utf8")) < 0) {
			snprintf(cmd_buffer, sizeof(cmd_buffer), "chkexfat -f -a %s", source);

			if(system(cmd_buffer) < 0) {
				fprintf(stderr, "[%s][%d] %s failed!\n"
					, __FUNCTION__, __LINE__, cmd_buffer);
				break;
			}
#if devel_debug
			snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
				"echo \"%s: chkexfat finished!\" >> /tmp/usbmount\.log", 
				__FUNCTION__);
			system(cmd_bufferx);
#endif
			rv = mount(source, mnt, "exfat"
				, MS_NODIRATIME | MS_NOATIME, "iocharset=utf8");
		}
#else
		snprintf(cmd_buffer, sizeof(cmd_buffer), CMD_MOUNT_FMT_EXFAT_FUSE, devnode, mnt);
		DEBUG("CMD: %s\n", cmd_buffer);
		rv = system(cmd_buffer);
		if(rv < 0) {
			fprintf(stderr, "[%s][%d] %s failed!\n"
				, __FUNCTION__, __LINE__, cmd_buffer);
				break;
		}
		rv = WIFEXITED(rv);
#endif
		break;
	}
	
	if(rv == 0) {
		strncpy(block_name, devnode, 3);
		block_name[3] = '\0';
		
		snprintf(cmd_buffer, sizeof(cmd_buffer), "echo 4096 > /sys/block/%s/queue/read_ahead_kb", block_name, block_name);
		DEBUG("CMD: %s\n\n", cmd_buffer);
		system(cmd_buffer);
	}
#if devel_debug
	snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
		"echo \"%s: mount %s return %d read_ahead_kb %s\" >> /tmp/usbmount.log", 
		__FUNCTION__, source, rv, block_name);
	system(cmd_bufferx);
#endif
	return rv;
}

static unsigned int _mount(unsigned int parition_map, const char *base, const char *mnt_path) {
	char dev_buffer[10];
	char mnt_buffer[30];
	int tmp;
	unsigned int mnt_map = 0;
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
	char usbnum = (!strcmp(base,"sda")) ? 1 : 2;
#endif

	if (0==parition_map)
		return 0;
	
	strcpy(dev_buffer, base);
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
	snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/usb%d_1", mnt_path, usbnum);
#else
	snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/%s", mnt_path, base);
#endif

	for (tmp = 0; tmp < 16; tmp++) {
		if (!(parition_map & (1 << tmp)))
			continue;
		if (tmp) {
			snprintf(dev_buffer, sizeof(dev_buffer), "%s%d", base, tmp);
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
			snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/usb%d_%d", mnt_path, usbnum, tmp);
#else
			snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/%s%d", mnt_path, base, tmp);
#endif
		
		mkdir(mnt_buffer, 0755);
		if (try_mount(dev_buffer, mnt_buffer)==0) {
			mnt_map |= (1 << tmp);
		} else {
			rmdir(mnt_buffer); 
		}
		}else{
			if(tmp==0){
				snprintf(dev_buffer, sizeof(dev_buffer), "%s", base);
				snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/%s", mnt_path, base);
				mkdir(mnt_buffer, 0755);
				if (try_mount(dev_buffer, mnt_buffer)==0) {
					mnt_map |= (1 << tmp);
				}else{
					rmdir(mnt_buffer); 
				}
			}
		}
	}

	return mnt_map;
}


static unsigned int fs_mount(unsigned int parition_id,const char *dev) {
	unsigned int mnt_map = 0;
	unsigned char char_start=0;
	unsigned int index=0;
	unsigned char dev_str[4]={'s','d',0,0};
	unsigned char sd_dev_str[10]={'m','m','c','b','l','k','0','p',0,0};
	//parition_id &= ~10001;
#if 0
	mnt_map = _mount(parition_id & 0xffff, "sda", MNT_ROOT);
	mnt_map |= _mount(parition_id >> 16, "sdb", MNT_ROOT) << 16;
#else
	if(!strncmp(dev, sd_dev_str, 6)){
		for(char_start='1';char_start<='4';char_start++){
			//sd_dev_str[8]=char_start;
			index=index+1;
			if(index % 2 ==0){
				if (strncmp(dev, sd_dev_str, strlen(sd_dev_str))==0){
					mnt_map |= _mount(parition_id >> 16, sd_dev_str, MNT_ROOT_SD) << 16;
					break;
				}
			}else{
				if (strncmp(dev, sd_dev_str, strlen(sd_dev_str))==0){
					mnt_map = _mount(parition_id & 0xffff, sd_dev_str, MNT_ROOT_SD);
					break;
				}
			}
		}
		
	}
	else{
		for(char_start='a';char_start<='z';char_start++){
			dev_str[2]=char_start;
			index=index+1;
			if(index % 2 ==0){
				if (strncmp(dev, dev_str, strlen(dev_str))==0){
					mnt_map |= _mount(parition_id >> 16, dev_str, MNT_ROOT) << 16;
					break;
				}
			}else{
				if (strncmp(dev, dev_str, strlen(dev_str))==0){
					mnt_map = _mount(parition_id & 0xffff, dev_str, MNT_ROOT);
					break;
				}
			}
		}
	}//!MMC
#endif
	return mnt_map;	
}
static int try_umount(const char *mnt_buffer)
{
	struct timeval expiry={0}, now={0};
	int rv=0;
	gettimeofday(&expiry, 0);		
	expiry.tv_sec += 1; // expire in 1 sec..
	srand(expiry.tv_usec);
	do
	{
		fsync();
		//printf("%s:%d umount %s\n",__FUNCTION__,__LINE__,mnt_buffer);
		rv = umount(mnt_buffer);
		if(rv==0)
		{
			//printf("%s:%d rmdir %s\n",__FUNCTION__,__LINE__,mnt_buffer);
			rmdir(mnt_buffer);
			
#if devel_debug
			snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
				"echo \"(%u) E umount %s success ret %d \" >> /tmp/usbmount\.log", 
				++cmd_counter, mnt_buffer,	rv);
			system(cmd_bufferx); 
#endif
			return 0;
		}
		//printf("%s:%d umount %s fail\n",__FUNCTION__,__LINE__,mnt_buffer);
		usleep(rand() % 500000);
		gettimeofday(&now, 0);
	} while(timercmp(&expiry, &now, >));
#if devel_debug
	snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
		"echo \"(%u) E umount %s fail ret %d \" >> /tmp/usbmount\.log", 
		++cmd_counter, mnt_buffer,	rv);
	system(cmd_bufferx); 
#endif

	return rv;
}

static unsigned int _umount(unsigned int parition_map, const char *base, const char *mnt_path) {
	//char dev_buffer[10];
	char mnt_buffer[30];
	char cmd_buffer[100];
	int tmp, rv;
	unsigned int mnt_map = 0;
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
	char usbnum = (!strcmp(base,"sda")) ? 1 : 2;
#endif

#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
	snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/usb%d_1", mnt_path, usbnum);
	//snprintf(cmd_buffer, sizeof(cmd_buffer), "/bin/umount %s/usb%d_0", mnt_path, usbnum);
#else
	snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/%s", mnt_path, base);
	//snprintf(cmd_buffer, sizeof(cmd_buffer), "/bin/umount %s/%s", mnt_path, base);
#endif

	for (tmp = 0; tmp < 16; tmp++) {
		if (!(parition_map & (1 << tmp)))
			continue;
		
		if (tmp) {
#ifdef CONFIG_CT_USBMOUNT_DIRECTORY
			snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/usb%d_%d", mnt_path, usbnum, tmp);
			//snprintf(cmd_buffer, sizeof(cmd_buffer), "/bin/umount %s/usb%d_%d", mnt_path, usbnum, tmp);
#else
			snprintf(mnt_buffer, sizeof(mnt_buffer), "%s/%s%d", mnt_path, base, tmp);
			//snprintf(cmd_buffer, sizeof(cmd_buffer), "/bin/umount %s/%s%d", mnt_path, base, tmp);
#endif
			rv=try_umount(mnt_buffer);
			if(rv==0)
			{
				mnt_map |= (1 << tmp);			
			}	
		
		}else{
			if(tmp==0){
				try_umount(mnt_buffer);
			}
		}
		/*
		if (system(cmd_buffer) == 0) {			
			mnt_map |= (1 << tmp);
			rmdir(mnt_buffer);
		}
		*/
	}

	return mnt_map;
}


static int file_update(unsigned int *new_mask, int adding) {
	FILE *fp;
	char buffer[10];
	unsigned int mask = 0;
	
	/*
	fp = fopen(MNT_FILE, "r");
	
	if (!fp) {		
		goto write_it;
	}

	fgets(buffer, sizeof(buffer), fp);
	sscanf(buffer, "%u", &mask);
	fclose(fp);
	*/
	mask = file_read_number(MNT_FILE);

write_it:
	if (adding)
		mask |= *new_mask;
	else
		mask &= ~(*new_mask);

	fp = fopen(MNT_FILE, "w");
	if (!fp) {
		fprintf(stderr, "fail to create %s\n", MNT_FILE);
		return -1;
	}
	fprintf(fp, "%u", mask);
	fclose(fp);

	*new_mask = mask;
	
	return 0;		
}

static unsigned int fs_umount(unsigned int parition_id,const char *dev) {
	unsigned int mnt_map = 0;
	unsigned char char_start=0;
	unsigned int index=0;
	unsigned char dev_str[4]={'s','d',0,0};
	unsigned char sd_dev_str[10]={'m','m','c','b','l','k','0','p',0,0};
	DEBUG("%s(1, 0x%x)\n", __FUNCTION__, parition_id);
#if 0	
	mnt_map = _umount(parition_id & 0xffff, "sda", MNT_ROOT);
	mnt_map |= _umount(parition_id >> 16, "sdb", MNT_ROOT) << 16;

#else
	if(!strncmp(dev, sd_dev_str, 6)){
		for(char_start='1';char_start<='4';char_start++){
			//sd_dev_str[8]=char_start;
			index=index+1;
			if(index % 2 ==0){
				if (strncmp(dev, sd_dev_str, strlen(sd_dev_str))==0){
					mnt_map |= _umount(parition_id >> 16, sd_dev_str, MNT_ROOT_SD) << 16;
					break;
				}
			}else{
				if (strncmp(dev, sd_dev_str, strlen(sd_dev_str))==0){
					mnt_map = _umount(parition_id & 0xffff, sd_dev_str, MNT_ROOT_SD);
					break;
				}
			}
		}
	}
	else{
		for(char_start='a';char_start<='z';char_start++){
			dev_str[2]=char_start;
			index=index+1;
			if(index % 2 ==0){
				if (strncmp(dev, dev_str, strlen(dev_str))==0){
					mnt_map |= _umount(parition_id >> 16, dev_str, MNT_ROOT) << 16;
					break;
				}
			}else{
				if (strncmp(dev, dev_str, strlen(dev_str))==0){
					mnt_map = _umount(parition_id & 0xffff, dev_str, MNT_ROOT);
					break;
				}
			}
		}
	}//!MMC
#endif
	return mnt_map;
}


static int action_add(int argc, char **argv, char *devpath) {
	unsigned int id, mnt_map;
#ifdef CONFIG_BOA_WEB_E8B_CH
#ifdef CONFIG_USER_BFTPD_BFTPD
    char *myprog = "/bin/ftp_manage";
    char *myargs[2];
    char *myenv[2];
#endif
#endif	
	id = get_partition_id(basename(devpath));
	
	if (id) {
		mnt_map = fs_mount(id,basename(devpath));

		if (mnt_map) {
			char cmd_buffer[30];

			//snprintf(cmd_buffer, sizeof(cmd_buffer), "echo %d > %s", mnt_map, MNT_FILE);
			//system(cmd_buffer);
			file_update(&mnt_map, 1);
		}
	}

#ifdef CONFIG_BOA_WEB_E8B_CH
#ifdef CONFIG_USER_BFTPD_BFTPD

    myargs[0] = myprog;
    myargs[1] = NULL;

    myenv[0] = "PATH=/bin:/usr/bin:/etc:/sbin:/usr/sbin";
    myenv[1] = NULL;
    if(fork() == 0){
    	execve(myprog, myargs, myenv);
	exit(0);
    }
#endif
#endif

	return 0;
}

static int action_remove(int argc, char **argv, char *devpath) {

	unsigned int id, mnt_map;
#ifdef CONFIG_BOA_WEB_E8B_CH
#ifdef CONFIG_USER_BFTPD_BFTPD

    char *myprog = "/bin/ftp_manage";
    char *myargs[2];
    char *myenv[2];
#endif
#endif
	id = get_partition_id(basename(devpath));
	//id = get_umount_partition_id(basename(devpath));
	printf("\nID=%x\n");
	if (id) {
		mnt_map = fs_umount(id, basename(devpath));
		if (mnt_map) {
			file_update(&mnt_map, 0);
			if (mnt_map==0)
				unlink(MNT_FILE);
		}

	}

#ifdef CONFIG_BOA_WEB_E8B_CH
#ifdef CONFIG_USER_BFTPD_BFTPD

    myargs[0] = myprog;
    myargs[1] = NULL;

    myenv[0] = "PATH=/bin:/usr/bin:/etc:/sbin:/usr/sbin";
    myenv[1] = NULL;
    if(fork() == 0){
    	execve(myprog, myargs, myenv);
	exit(0);
    }
#endif
#endif
}


static const struct action_table hotplug_table[] = {
	{ "mount", action_add },
	{ "add", action_add },
	{ "remove", action_remove },	
	{ 0 },
};



int main(int argc, char **argv) {
	
	char *devpath;
	char *action;
	const struct action_table *p;
	
	int fd = open("/dev/null", O_RDWR);
	if (fd < 0) {
		/* NB: we can be called from init
		 * , and there /dev/null may legitimately not (yet) exist!
		 */
		fd = open("/", O_RDONLY, 0666); /* don't believe this can fail */
	}
	
	while ((unsigned)fd < 2)
		fd = dup(fd); /* have 0,1,2 open at least to /dev/null */
	
	if(fd > 2)
		close(fd);
	
	ASSERT(argc > 1);
	if (argc<= 1) {
		DEBUG("not enough arg\n");
		return -1;
	}
	
	devpath = getenv("DEVPATH");
	ASSERT(devpath != 0);
	
	action = getenv("ACTION");
	ASSERT(action != 0);

	if (strcmp(argv[1], "block")) {
		DEBUG("will not handle %s\n", argv[1]);
		return -1;
	}

	if ((action == 0) || (devpath == 0)){
		printf("required env var missing\n");
		return -1;
	}

#if devel_debug
	snprintf(cmd_bufferx, sizeof(cmd_bufferx), 
			"echo \"D=(%s) A=(%s) A1=%s \" >> /tmp/usbmount\.log", 
			devpath, action, argv[1]);
		system(cmd_bufferx);
#endif

	for (p = &hotplug_table[0]; p->action_str; p++) {
		if (strcmp(action, p->action_str)==0) {
			return (p->action_func(argc-1, &argv[1], devpath));
		}
	}
	return 0;
}



