/*
 *      Web server handler routines for storage
 *
 *
 */

/*-- System inlcude files --*/
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>

#include "boa.h"
#include "globals.h"
#include "apform.h"
#include "apmib.h"
#include "utility.h"
#include "asp_page.h"

#if defined(SAMBA_WEB_SUPPORT)

#define SAMBA_SHAREINFO_READONLY 1
#define SAMBA_SHAREINFO_READWRITE 2

#define SAMBA_SHAREINFO_READONLY_DISPLAY "read only"
#define SAMBA_SHAREINFO_READWRITE_DISPLAY "read write"

//format related
#define EXT_FORMAT_TOOL  "mke2fs"
#define FAT_FORMAT_TOOL  "mkfs.fat"
#define NTFS_FORMAT_TOOL "mkntfs"

#define EXT_SYSTYPE_PREFIX  "ext"
#define FAT_SYSTYPE_PREFIX  "fat"
#define NTFS_SYSTYPE_PREFIX "ntfs"

#define FAT16_SYSTYPE  "fat16"
#define FAT32_SYSTYPE  "fat32"

#define NTFS_COMMAND "ntfs-3g"

#define PARTED_PARTITION_TYPE_LOGICAL "logical"
#define PARTED_PARTITION_TYPE_EXTENDED "extended"

#define SAMBA_USER_NAME "samba_user"

typedef struct __disk_partition_info {
	//int total_num;
	unsigned char name[16];
	unsigned long long totalsize; //units blocks *1024 =bytes
	unsigned long long partedsize; 
	
}disk_partition_info;

typedef struct __disk_parted_print_info{
	//int total;
	unsigned char number[4];
	unsigned char start[16]; //units MB??
	unsigned char end[16]; //units MB??
	unsigned char size[16]; //units MB??
	unsigned char filesystem[16];
	unsigned char type[16];
	unsigned char flag[16];
}disk_parted_print_info;

typedef struct __add_partition_info{
	int number;
	unsigned long long size; //units MB
}add_partition_info;

#define MAX_ADD_PARTITION_NUMBER 8
#define MAX_LIST_PARTITION_NUMBER 16
#define MAX_DEVICE_NUMBER 8


int UserEditName(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0;
	int				index;
	STORAGE_USER_T	s_user;

	apmib_get(MIB_STORAGE_USER_EDIT_INDEX,(void*)&index);
	*((char*)&s_user) = (char)index;
	apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
		"<td width=\"20%%\"><font size=2><b>Name:</b></td>\n"
		"<td width=\"50%%\"><font size=2>%s</td></tr>\n"),
		s_user.storage_user_name);
	
	return nBytesSent;
}

int GroupEditName(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0;
	int				index;
	STORAGE_GROUP_T	s_group;
	
	apmib_get(MIB_STORAGE_GROUP_EDIT_INDEX,(void*)&index);
	*((char*)&s_group) = (char)index;
	apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
		"<td width=\"20%%\"><font size=2><b>Group Name</b></td>\n"
		"<td width=\"50%%\"><font size=2>%s</td></tr>\n"),
		s_group.storage_group_name);

	return nBytesSent;
}
//flag =1 only get partition name 'sdb1:sdb2:sdb3.....'
//flag=2  get disk name/size/.....
int partitionInfo(struct __disk_partition_info *info, unsigned char *command, unsigned char *tmpfile, 
					unsigned char *buff, int buff_len, int flag, int *total)
{
	unsigned char tmpbuff[512] = {0}, tmpbuff2[512] = {0};
	unsigned char name[16] = {0}, tmpname[16] = {0};
	unsigned char major[12] = {0}, minor[12] = {0}, block[32]={0}; 
	FILE *fp;
	unsigned char *ptr = NULL, *endptr = NULL;
	int ret = 0, i =  0;
	unsigned long long blocks, tmp_blocks; 
	

	
if ((flag == 2)&&(!info || !command || !tmpfile || !total))
		return -1;
	
if ((flag == 1)&&(!buff || !command || !tmpfile))
		return -1;
	
	sprintf(tmpbuff2, "%s > %s", command, tmpfile);
	system(tmpbuff2);
	sync();	
	fp = fopen(tmpfile,"r");
	if(fp == NULL)
	{
		//failed
		return -1;
	}
	memset(tmpbuff2, '\0', sizeof(tmpbuff2));
	memset(tmpbuff, '\0', sizeof(tmpbuff));
	memset(tmpname, '\0', sizeof(tmpname));
	tmp_blocks = 0;
	while (fgets(tmpbuff2, sizeof(tmpbuff2), fp))
	{
	
		memset(major, '\0', sizeof(major));
		memset(minor, '\0', sizeof(minor));
		memset(block, '\0', sizeof(block));
		memset(name, '\0', sizeof(name));
		
		ptr = strstr(tmpbuff2, "sd");
		if (ptr)
		{
			ret = sscanf(tmpbuff2, "%s %s %s %s", major, minor, block, name);
			//printf("%s %d: major=%s minor=%s blocks=%s name=%s\n", __FUNCTION__, __LINE__, major, minor, block, name);
			if (flag == 2)
			{
				if (strlen(name) == 3)//eg:sda/sdb.....
				{
					if (tmpname[0] && strncmp(name, tmpname, 3))//eg:sda ->sdb???
					{
						info[i].partedsize = tmp_blocks;
						
						tmp_blocks = 0;
						i++;//next
					}
					memcpy(tmpname, name, 3);
					//printf("%s %d: tmpname=%s name=%s\n", __FUNCTION__, __LINE__, tmpname, name);
					//blocks = atoll(block);
					blocks = strtoull(block, &endptr, 0);
					info[i].totalsize = blocks;
					memcpy(info[i].name, tmpname, 3);
					memset(tmpbuff2, '\0', sizeof(tmpbuff2));
					continue;
				}
				if (!strncmp(tmpname, name, 3))
				{
					//tmp_blocks += atoll(block);
					tmp_blocks += strtoull(block, &endptr, 0);
				}
			}
			if (flag == 1)
			{
				if (strlen(name) <= 3)//eg:skip sda/sdb.....
					continue;
				//if (atoll(block) < 100)//fix me ? extend partition
				if (strtoull(block, &endptr, 0) < 100)//fix me ? extend partition
					continue;
				strcat(tmpbuff, name);
				strcat(tmpbuff, ":");
			}
		}			
		memset(tmpbuff2, '\0', sizeof(tmpbuff2));
	}
	if (flag == 2)
	{
		info[i].partedsize = tmp_blocks;
		tmp_blocks = 0;
		i++;//next
		*total = i > MAX_DEVICE_NUMBER?MAX_DEVICE_NUMBER:i;
		//dump
		#if 0
		for(i = 0; i < *total; i++)
		{
			printf("%s %d: totolsize=%llu partedsize=%llu name=%s number=%d\n", __FUNCTION__, __LINE__, 
				info[i].totalsize, info[i].partedsize, info[i].name, *total);
		}
		#endif
	}
	if (flag == 1)
	{
		snprintf(buff, buff_len, "%s", tmpbuff);
		//printf("%s %d: buff=%s\n", __FUNCTION__, __LINE__, buff);
	}

	fclose(fp);
	sprintf(tmpbuff2, "rm %s", tmpfile);
	system(tmpbuff2);

	return 0;
	
}

//sprintf(bufStr, "token[%d] =\'%d|%s|%d|%d|%d|%d|%d|%d|%s|%d\';\n",idx,entry.enabled,IfaceName,entry.tagged,0,entry.priority,entry.cfi,0,entry.vlanId,wanLan, forwarding_rule);
//diskname|totalsize|alreadypartitionsize|unpartitionsize
int getDiskInfo(request *wp, int argc, char **argv)
{
	unsigned char tmpbuff[512] = {0}, total[64] = {0}, parted[64]= {0}, unparted[64] = {0};
	unsigned long long num1 = 0, num2 = 0, unpartedsize = 0;
	int number = 0, i = 0;
	struct __disk_partition_info partition_info[MAX_DEVICE_NUMBER];
	
	float total_float = 0.0;
	memset(partition_info, 0x00, sizeof(partition_info));
	partitionInfo(partition_info, "cat /proc/partitions", "/var/tmp/boa_parted.txt", tmpbuff, 512, 2, &number);
	//number = partition_info[0].total_num;
	for (i = 0; i < number; i++)
	{	
		total_float = (partition_info[i].totalsize*1024)/(1000*1000*1000.0);
		sprintf(total,"%0.3f",total_float);
		
		num1 = (partition_info[i].partedsize*1024)/(1000*1000*1000);
		num2 = ((partition_info[i].partedsize*1024)/(1000*1000))%1000;
		sprintf(parted,"%llu.%llu",num1,num2);

		unpartedsize = partition_info[i].totalsize - partition_info[i].partedsize;
		
		num1 = (unpartedsize*1024)/(1000*1000*1000);
		num2 = ((unpartedsize*1024)/(1000*1000))%1000;
		sprintf(unparted,"%llu.%llu",num1,num2);
		
		sprintf(tmpbuff, "token[%d] =\'%s|%s|%s|%s\';\n",i,partition_info[i].name,total,parted,unparted);
		req_format_write(wp, tmpbuff);
		//printf("%s %d: tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
	}

	return 0;
	
}
//case-insensitive strstr function
const char* strstri(const char* str, const char* subStr)
{
	int len = strlen(subStr);
	if(len == 0)
	{
		return NULL;
	}

	while(*str)
	{
		if(strncasecmp(str, subStr, len) == 0)
		{
			return str;
		}
		++str;
	}
	return NULL;
}

//default unit MB
int strtolonglong(unsigned char *str, unsigned long long *value)
{
	unsigned char *ptr, *endptr;
	
	if (!str || !value)
		return -1;
	
	*value = strtoull(str, &endptr, 0);
	ptr = strstri(str, "MB");
	if (ptr)
		return 0;
		
	ptr = strstri(str, "GB");
	if (ptr)
	{
		*value = (*value) * 1000;
		return 0;
	}
	
	ptr = strstri(str, "KB");
	if (ptr)
	{
		*value = (*value) / 1000;
		return 0;
	}
		
	return 0;
}
int pharsePartedPrint(disk_parted_print_info *info, unsigned char *cmd, unsigned char *dev, unsigned char *tmpfile, int *total)
{
	unsigned char tmpbuff[512] = {0};
	unsigned char number[4], start[16],end[16], size[16], filesys[16], type[16],flags[16];
	unsigned char *ptr;
	FILE *fp;
	int i, ret, flag;
	unsigned char extended_tmpbuff[512] = {0};
	if (!info || !cmd || !dev || !tmpfile)
		return -1;
	
	sprintf(tmpbuff, "%s %s p > %s", cmd, dev, tmpfile);
	system(tmpbuff);
	sync();	
	fp = fopen(tmpfile,"r");
	if(fp == NULL)
	{
		//failed
		return -1;
	}
	flag = 0;
	i = 0;
	memset(tmpbuff, '\0', sizeof(tmpbuff));
	memset(extended_tmpbuff, '\0', sizeof(extended_tmpbuff));
	while (fgets(tmpbuff, sizeof(tmpbuff), fp))
	{
		memset(number, '\0', sizeof(number));
		memset(start, '\0', sizeof(start));
		memset(end, '\0', sizeof(end));
		memset(size, '\0', sizeof(size));
		memset(filesys, '\0', sizeof(filesys));
		memset(type, '\0', sizeof(type));
		memset(flags, '\0', sizeof(flags));
		//printf("%s %d tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
		if (tmpbuff[0] == '\r' || tmpbuff[0] == '\n')
			continue;
		
		ptr = strstr(tmpbuff, "Number");
		if (!flag && !ptr)//skip start message
			continue;
		if (ptr)
		{
			flag = 1;
			continue;
		}		
		if (flag)//start to handle partition info
		{
			ret = sscanf(tmpbuff, "%s %s %s %s %s %s %s", number, start, end, size, type, filesys, flags);
			//printf("%s %d: number=%s start=%s end=%s size=%s filessys=%s name=%s flags=%s ret=%d\n", __FUNCTION__, __LINE__, 
				//number, start, end, size, filesys, type, flags, ret);
			#if 0
			if (!strncmp(type, "extended", strlen("extended")))
				continue;
			#endif

			//fprintf(stderr,"******<%s:%s:%d>**** type = %s\n",__FILE__,__FUNCTION__,__LINE__,type);
			if(!strcmp(type,"extended")){
				strcpy(extended_tmpbuff,tmpbuff);
			}else{
				memcpy(info[i].number, number, strlen(number));
				memcpy(info[i].start, start, strlen(start));
				memcpy(info[i].end, end, strlen(end));
				memcpy(info[i].size, size, strlen(size));
				memcpy(info[i].type, type, strlen(type));
				//printf("%s %d filesys[0]=0x%x\n", __FUNCTION__, __LINE__, filesys[0]);
				if (!filesys[0])
					memcpy(info[i].filesystem, "unformat", strlen("unformat"));
				else
					memcpy(info[i].filesystem, filesys, strlen(filesys));
				memcpy(info[i].flag, flags, strlen(flags));
				i++;
			}
			memset(tmpbuff, '\0', sizeof(tmpbuff));	
		}						
	}
	if (strlen(extended_tmpbuff) > 0)//start to handle partition info
	{
		ret = sscanf(extended_tmpbuff, "%s %s %s %s %s %s %s", number, start, end, size, type, filesys, flags);
		memcpy(info[i].number, number, strlen(number));
		memcpy(info[i].start, start, strlen(start));
		memcpy(info[i].end, end, strlen(end));
		memcpy(info[i].size, size, strlen(size));
		memcpy(info[i].type, type, strlen(type));
		//printf("%s %d filesys[0]=0x%x\n", __FUNCTION__, __LINE__, filesys[0]);
		if (!filesys[0])
			memcpy(info[i].filesystem, "unformat", strlen("unformat"));
		else
			memcpy(info[i].filesystem, filesys, strlen(filesys));
		memcpy(info[i].flag, flags, strlen(flags));
		i++;
		memset(extended_tmpbuff, '\0', sizeof(extended_tmpbuff));	
	}
	*total = i;
	
	#if 0
	//dump
	for(i = 0; i < *total; i++)
	{
		printf("%s %d: number=%s start=%s end=%s size=%s name=%s filesys=%s flag=%s *total=%d\n", __FUNCTION__, __LINE__, 
			info[i].number, info[i].start, info[i].end, info[i].size, info[i].type, info[i].filesystem, info[i].flag, *total);
	}
	#endif
	return 0;
}
int PartitionList(request *wp, int argc, char **argv)
{
	unsigned char tmpbuff[512] = {0}, partition_name[16] = {0}, filesystem[20] = {0};
	unsigned long long num1 = 0, num2 = 0, unpartedsize = 0;
	int number = 0, i = 0, j = 0, total = 0, nBytesSent = 0, partednum = 0, flag = 0;
	struct __disk_partition_info partition_info[MAX_DEVICE_NUMBER];
	disk_parted_print_info parted_print_info[MAX_LIST_PARTITION_NUMBER];
	
	memset(partition_info, 0x00, sizeof(partition_info));
	partitionInfo(partition_info, "cat /proc/partitions", "/var/tmp/boa_parted_tmp.txt", tmpbuff, 512, 2, &number);
	nBytesSent += req_format_write(wp, ("<input type=\"hidden\" value=\"%d\" name=\"total_device_num\">\n"), number);
	for (i = 0; i < number; i++)
	{
		total = 0;
		memset(tmpbuff, 0x00, sizeof(tmpbuff));
		memset(parted_print_info, 0x00, sizeof(parted_print_info));
		sprintf(tmpbuff, "/dev/%s", partition_info[i].name);
		pharsePartedPrint(parted_print_info, "parted", tmpbuff, "/var/tmp/boa_parted_tmp.txt", &total);
		nBytesSent += req_format_write(wp, ("<br>\n"));
		nBytesSent += req_format_write(wp, ("<font size=2><b>HardDisk%d Partition Table:</b></font>\n"), i+1);
		//nBytesSent += req_format_write(wp, ("<input type=\"hidden\" value=\"%d\" id = \"totalnum_sd%d\" name=\"totalnum_sd%d\">\n"), total, i+1, i+1);
		nBytesSent += req_format_write(wp, ("<table border=0 width=500>\n"));
		nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">\n"
			"<td align=center width=\"30%%\" ><font size=\"2\"><b>Number</b></font></td>\n"
			"<td align=center width=\"30%%\" ><font size=\"2\"><b>Size</b></font></td>\n"
			"<td align=center width=\"30%%\" ><font size=\"2\"><b>File system</b></font></td>\n"
  			"<td align=center width=\"10%%\" ><font size=\"2\"><input type=\"checkbox\" value=\"ON\" id = \"selall_sd%d\" name=\"selall_sd%d\" onClick=\"checkboxClick(%d)\"></font></td>\n"
			"</tr>\n"), i+1, i+1, i+1);
		flag = 0;
		int index = 0;
		for (j = 0; j < total; j++)
		{
			if (!strncmp(parted_print_info[j].type, PARTED_PARTITION_TYPE_EXTENDED, strlen(PARTED_PARTITION_TYPE_EXTENDED)))
			{
				flag = 1;
				continue; //donot display extended partition
			}
			memset(filesystem, '\0', sizeof(filesystem));
			memset(partition_name, '\0', sizeof(partition_name));
			//printf("%s %d j=%d filesystem=%s \n", __FUNCTION__, __LINE__, j, parted_print_info[j].filesystem);
			#if 0
			if (!strncmp(parted_print_info[j].filesystem, "unformat", strlen("unformat")))
			{
				if (!strncmp(parted_print_info[j].type, PARTED_PARTITION_TYPE_LOGICAL, strlen(PARTED_PARTITION_TYPE_LOGICAL)))
				{
					sprintf(partition_name, "/dev/%s%s", partition_info[i].name, parted_print_info[j].number);
					//printf("%s %d partition_name=%s \n", __FUNCTION__, __LINE__, partition_name);
					if(getDiskFileSystem(partition_name, filesystem, "/tmp/boa_par_mount_tmp.txt") != 0)
					{
						sprintf(filesystem, "%s", "unformat");
					}
				}
				else
					sprintf(filesystem, "%s", "unformat");
			}
			else
			{
				//formatted, get filesystem from mount
				sprintf(partition_name, "/dev/%s%s", partition_info[i].name, parted_print_info[j].number);
				//printf("%s %d partition_name=%s \n", __FUNCTION__, __LINE__, partition_name);
				if(getDiskFileSystem(partition_name, filesystem, "/tmp/boa_par_mount_tmp.txt") != 0)
				{
					sprintf(filesystem, "%s", "unformat");
				}
			}
			#else
			sprintf(partition_name, "/dev/%s%s", partition_info[i].name, parted_print_info[j].number);
			//printf("%s %d partition_name=%s \n", __FUNCTION__, __LINE__, partition_name);
			if(getDiskFileSystem(partition_name, filesystem, "/tmp/boa_par_mount_tmp.txt") != 0)
			{
				sprintf(filesystem, "%s", "unformat");
			}
			#endif
			index++;
			partednum = atoi(parted_print_info[j].number);
			nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">\n"
				"<td align=center width=\"30%%\" ><font size=\"2\">%d</font></td>\n"
				"<td align=center width=\"30%%\" ><font size=\"2\">%s</font></td>\n"
				"<td align=center width=\"30%%\" ><font size=\"2\">%s</font></td>\n"
				"<td align=center width=\"10%%\" ><font size=\"2\"><input type=\"checkbox\" value=\"ON\" id = \"sd%d_select%d\" name=\"sd%d_select%d\"></font></td>\n"
				"</tr>\n"),
				partednum, parted_print_info[j].size, filesystem, i+1, index, i+1, index);
		}
		nBytesSent += req_format_write(wp, ("</table>\n"));
		nBytesSent += req_format_write(wp, ("<input type=\"hidden\" value=\"%d\" id = \"totalnum_sd%d\" name=\"totalnum_sd%d\">\n"), (flag==1)?(total-1):total, i+1, i+1);
	}	
}

static int getPartition(unsigned char *command, unsigned char *filename, unsigned char *buff, int buff_len, int flag)
{
	unsigned char tmpbuff[512] = {0}, tmpbuff2[128] = {0};
	unsigned char partition[16] = {0}, id[16] = {0};
	unsigned char local[12] = {0}; 
	FILE *fp;
	unsigned char *ptr = NULL;
	int j = 0;
	
	//safety check
	if (!command || !filename || !buff)
		return -1;
	
	sprintf(tmpbuff2, "%s > %s", command, filename);
	system(tmpbuff2);
	sync();	
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		//failed
		return -1;
	}
	memset(tmpbuff2, '\0', sizeof(tmpbuff2));
	memset(tmpbuff, '\0', sizeof(tmpbuff));
	while (fgets(tmpbuff2, sizeof(tmpbuff2), fp)) 
	{
		if (tmpbuff2[0] != '/')
		{
			memset(tmpbuff2, '\0', sizeof(tmpbuff2));
			continue;
		}
		
		memset(partition, '\0', sizeof(partition));
		memset(id, '\0', sizeof(id));
		memset(local, '\0', sizeof(local));
		j = 0;
		ptr = strstr(tmpbuff2, "/dev/sd");
		if (ptr)
		{			
			local[j] =  ptr - tmpbuff2;
			while(j++ < 5)
			{
				ptr = strstr(ptr," ");
				while(*ptr == ' '){
					*ptr++ = '\0';
				}
				local[j] = ptr - tmpbuff2;
			}
			local[j] = ptr - tmpbuff2;
			/* flag = 1 , skip extend partition , cannnot format extend partition!! 
			*  5 Extended
			*  f Win95 Ext'd (LBA)
			*  85 Linux extended
			*/
			strcpy(id, tmpbuff2+local[4]);
			if (((!strcmp(id, "5")) || (!strcmp(id, "f")) || (!strcmp(id, "85"))) && flag)
			{
				memset(tmpbuff2, '\0', sizeof(tmpbuff2));
				continue;
			}
			
			//printf("%s %d tmpbuff2=%s partition=%s id=%s \n", __FUNCTION__, __LINE__, tmpbuff2, tmpbuff2+local[0]+strlen("/dev/"), id);
			strcpy(partition, tmpbuff2+local[0]+strlen("/dev/"));
			if (strlen(partition) > 3)
			{
				strcat(tmpbuff, partition);
				strcat(tmpbuff, ":");
			}
				
		}
		memset(tmpbuff2, '\0', sizeof(tmpbuff2));
	}
	
	snprintf(buff, buff_len, "%s", tmpbuff);
	//printf("%s %d buff=%s \n", __FUNCTION__, __LINE__, buff);
	
	fclose(fp);
	sprintf(tmpbuff2, "rm %s", filename);
	system(tmpbuff2);
	
	return 0;
}


int PartitionsList(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0,len = 0, ret = -1;
	int				number,i = 0;
	unsigned char tmpbuff[512] = {0};
	unsigned char *ptr = NULL;
	struct __disk_partition_info partition_info[MAX_DEVICE_NUMBER];
	
	memset(partition_info, 0x00, sizeof(partition_info));
	
	nBytesSent += req_format_write(wp, ("<select size=\"1\" name=\"partition\">"));
	//if (getPartition("fdisk -l | grep sd", "/tmp/boa_partition_tmp.txt", tmpbuff, 512, 1) == 0)
	ret = partitionInfo(partition_info, "cat /proc/partitions", "/var/tmp/boa_parted_get.txt", tmpbuff, 512, 1, &number);
	if (ret == 0)
	{
		//printf("%s %d tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
		ptr = strtok(tmpbuff, ":");
		i = 0;
		while (ptr != NULL)
		{
			if (i == 0)
				nBytesSent += req_format_write(wp, ("<option value=\"%s\" selected>%s</option>"), ptr, ptr);
			else
				nBytesSent += req_format_write(wp, ("<option value=\"%s\" >%s</option>"), ptr, ptr);
			//printf("%s %d ptr=%s \n", __FUNCTION__, __LINE__, ptr);
			i++;
			ptr = strtok(NULL, ":");
		}
	 }
	nBytesSent += req_format_write(wp, ("</select>"));


	return nBytesSent;
}

int ShareFolderList(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0,len = 0;
	int				number,i;
	STORAGE_SHAREINFO_T	sharefolder;
	unsigned char permission[16] = {0};
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
      	"<td align=center width=\"20%%\" ><font size=\"2\"><b>Folder Name</b></font></td>\n"
      	"<td align=center width=\"40%%\" ><font size=\"2\"><b>Folder Path</b></font></td>\n"
      	"<td align=center width=\"20%%\" ><font size=\"2\"><b>Owner</b></font></td>\n"
      	"<td align=center width=\"10%%\" ><font size=\"2\"><b>Permission</b></font></td>\n"
      	"<td align=center width=\"10%%\" ><font size=\"2\"><b>Select</b></font></td>"
		"</tr>\n"));

	apmib_get(MIB_STORAGE_SHAREINFO_TBL_NUM,(void*)&number);
	for(i = 0;i < number;i++)
	{
		memset(&sharefolder,'\0',sizeof(STORAGE_SHAREINFO_T));
		memset(permission, '\0', sizeof(permission));
		*((char*)&sharefolder) = (char)(i+1);
		apmib_get(MIB_STORAGE_SHAREINFO_TBL,(void*)&sharefolder);
		if (sharefolder.storage_permission == SAMBA_SHAREINFO_READWRITE)
		{
			strncpy(permission, SAMBA_SHAREINFO_READWRITE_DISPLAY, strlen(SAMBA_SHAREINFO_READWRITE_DISPLAY));
		}
		else
		{
			strncpy(permission, SAMBA_SHAREINFO_READONLY_DISPLAY, strlen(SAMBA_SHAREINFO_READONLY_DISPLAY));
		}
		nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">"
  			"<td align=center width=\"20%%\" ><font size=\"2\">%s</font></td>\n"
  			"<td align=center width=\"40%%\" ><font size=\"2\">%s</font></td>\n"
  			"<td align=center width=\"20%%\" ><font size=\"2\">%s</font></td>\n"
  			"<td align=center width=\"10%%\" ><font size=\"2\">%s</font></td>\n"
  			"<td align=center width=\"10%%\" ><font size=\"2\"><input type=\"checkbox\" value=\"ON\" name=\"select%d\"></font></td></tr>\n"),
  			sharefolder.storage_sharefolder_name,sharefolder.storage_sharefolder_path,sharefolder.storage_account,permission,
  			i+1);
	}

	return nBytesSent;
}

int Storage_GeDirRoot(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0;
	char*			dir_name;
	char			tmpBuff[30];
	char			tmpBuff2[30];

	memset(tmpBuff,'\0',30);
	memset(tmpBuff2,'\0',30);
	apmib_get(MIB_STORAGE_FOLDER_LOCAL,(void*)tmpBuff);

	dir_name = strstr(tmpBuff,"sd");
	strcpy(tmpBuff2,"/tmp/usb/");
	strcat(tmpBuff2,dir_name);
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
		"<td width=\"20%%\"><font size=2><b>Location</b></td>\n"
		"<td width=\"50%%\"><font size=2>%s</td></tr>\n"
		"<input type=\"hidden\" name=\"Location\" value=\"%s\">\n"),
		tmpBuff2,tmpBuff2);
	
	return nBytesSent;
}

int FolderList(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0,len;
	FILE 			*fp,*fp2;
	char			tmpBuff[100],tmpBuff2[100];
	char			strLocal[30],Location[30];
	char*			strRootDir;
	int				i = 0,index = 0,flag = 0,number;
	char			*p,*p2;
	STORAGE_GROUP_T	s_group;


	memset(tmpBuff,'\0',100);
	memset(tmpBuff2,'\0',100);
	memset(strLocal,'\0',30);
	
	apmib_get(MIB_STORAGE_FOLDER_LOCAL,(void*)strLocal);
	strRootDir = strstr(strLocal,"sd");
	snprintf(tmpBuff2,100,"ls /tmp/usb/%s >/tmp/tmp.txt",strRootDir);
	system(tmpBuff2);

	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Folder</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Group</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Select</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Delete</b></font></td></tr>\n"));

	memset(tmpBuff,'\0',100);
	fp = fopen("/tmp/tmp.txt","r");
	if(fp == NULL)
	{
		return nBytesSent;
	}

	while(fgets(tmpBuff, 100, fp)){
		len = strlen(tmpBuff);
		tmpBuff[len-1] = '\0';
		snprintf(tmpBuff2,100,"ls -ld /tmp/usb/%s/%s >/tmp/tmp2.txt",strRootDir,tmpBuff);
		system(tmpBuff2);

		memset(tmpBuff2,'\0',100);
		fp2 = fopen("/tmp/tmp2.txt","r");
		if(fp2 == NULL){
			return nBytesSent;
		}
		
		if(fgets(tmpBuff2,100,fp2)){
			if(tmpBuff2[0] != 'd'){
				memset(tmpBuff,'\0',100);
				memset(tmpBuff2,'\0',100);
				fclose(fp2);
				continue;
			}
			p = tmpBuff2;

			while(i < 3){
				while(*p == ' '){
					p++;
				}
				p = strstr(p," ");
				i++;
			}

			while(*p == ' ')
				p++;

			p2 = strstr(p," ");
			*p2 = '\0';
			i  = 0;
		}

		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		for(i = 0;i < number;i++)
		{
			memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
			*((char*)&s_group) = (char)(i+1);
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

			if(s_group.storage_group_sharefolder_flag == 1){
				memset(Location,'\0',30);
				snprintf(Location,30,"/tmp/usb/%s/%s",strRootDir,tmpBuff);
				if(!strcmp(Location,s_group.storage_group_sharefolder)){
					flag = 1;
					break;
				}
			}
		}			
		
		if(flag == 0){
			nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">"
				"<td align=center width=\"25%%\" ><font size=\"2\">/tmp/usb/%s/%s</td>\n"
      			"<td align=center width=\"25%%\" ><font size=\"2\">--</td>\n"
      			"<td align=center width=\"25%%\" ><font size=\"2\"><input type=\"checkbox\" value=\"/tmp/usb/%s/%s\" name=\"select%d\" onClick=\"SelectClick(%d)\"></td>\n"
      			"<td align=center width=\"25%%\" ><input type=\"checkbox\" value=\"/tmp/usb/%s/%s\" name=\"delete%d\" onClick=\"DeleteClick(%d)\"></td></tr>\n"),
				strRootDir,tmpBuff,strRootDir,tmpBuff,index,index,strRootDir,tmpBuff,index,index);
			index++;
		}
		
		fclose(fp2);
		memset(tmpBuff,'\0',100);
		memset(tmpBuff2,'\0',100);
		flag = 0;
	}
	fclose(fp);

	nBytesSent += req_format_write(wp,(
		"<input type=\"hidden\"  name=\"DirNum\" value=\"%d\">\n"),
		index);
	return nBytesSent;
		
}

int getDiskFileSystem(unsigned char *name, unsigned char *filesystem, unsigned char *tmpfile)
{
	FILE *fp;
	unsigned char tmpbuf[256] = {0}, type[10] = {0};
	unsigned char *ptr = NULL, *ptr2 = NULL, *ptr3 = NULL;
	int j = 0, ret = -1, len = 0; 
	
	if (!name || !filesystem || !tmpfile)
		return ret;
	snprintf(tmpbuf, 256, "mount | grep %s > %s", name, tmpfile);
	system(tmpbuf);
	//sleep(5);
	fp = fopen(tmpfile,"r");
	if(fp == NULL)
		return ret;
	
	memset(tmpbuf, '\0', sizeof(tmpbuf));
	while (fgets(tmpbuf, 256, fp)) 
	{
		ptr = strstr(tmpbuf, name);
		if (ptr)
		{
			//exist
			ptr = strstr(tmpbuf, "type");
			if (ptr)
			{
				#if 1
				if (sscanf(ptr, "%s %[^ ]", type, filesystem) > 0)
				{
					ret = 0;
					break;
				}
				#else
				ptr2 = strstr(ptr, " ");
				if (ptr2)
				{
					ptr3 = strstr(ptr2 + 1, " ");
					//find
					if (ptr3)
					{
						len = ptr3 - ptr2 - 1;
						if (len >= 0)
						{
							memcpy(filesystem, ptr2 + 1, len);
							printf("%s %d name=%s filesystem=%s \n", __FUNCTION__, __LINE__, name, filesystem);
							ret = 0;
							break;
						}
					}
				}
				#endif
			}

		}
		memset(tmpbuf, '\0', sizeof(tmpbuf));
		memset(type, '\0', sizeof(type));
	}

	if (ret == 0)
	{	
		if (!strncmp(filesystem, "vfat", strlen("vfat")))
		{
			strncpy(filesystem, FAT_SYSTYPE_PREFIX, strlen(FAT_SYSTYPE_PREFIX));
			filesystem[strlen(FAT_SYSTYPE_PREFIX)] = '\0';
		}
	}
	//printf("%s %d filesystem=%s\n", __FUNCTION__, __LINE__, filesystem);	
	fclose(fp);
	sprintf(tmpbuf, "rm %s", tmpfile);
	system(tmpbuf);

	return ret;
}

int DiskList(request *wp, int argc, char **argv)
{
	int 			nBytesSent = 0,len = 0;
	int				i,j = 0;
	char			capability[20],freeSize[20], used[20], filesystem[20] = {0};
	long long		num1,num2;
	char			*ptr;
	FILE 			*fp;
	int				total_size,free_size;
	char			tmpBuff[100];
	unsigned char	local[10]; 
	float size_float = 0.0;
    struct stat stat_buff;
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
      	"<td align=center width=\"20%%\" ><font size=\"2\"><b>Partition</b></font></td>\n"
      	"<td align=center width=\"15%%\" ><font size=\"2\"><b>Total</b></font></td>\n"
		"<td align=center width=\"15%%\" ><font size=\"2\"><b>Available</b></font></td>\n"
		"<td align=center width=\"20%%\" ><font size=\"2\"><b>Used</b></font></td>\n"
		"<td align=center width=\"10%%\" ><font size=\"2\"><b>Use%%</b></font></td>\n"
		"<td align=center width=\"20%%\" ><font size=\"2\"><b>System Type</b></font></td>\n"
		"</tr>\n"
      	/*"<td align=center width=\"25%%\" ><font size=\"2\"><b>Create Share</b></font></td></tr>\n"*/));

	memset(tmpBuff,0,100);
	system("df >/tmp/df_tmp.txt");
	sync();	
	fp = fopen("/tmp/df_tmp.txt","r");
	if(fp == NULL){
		return nBytesSent;
	}	
	while (fgets(tmpBuff, 100, fp)) {
		ptr = strstr(tmpBuff, "/dev/sd");
		if (ptr) {
			local[j] =  ptr - tmpBuff;
			while(j++ < 5)
			{
				ptr = strstr(ptr," ");
				while(*ptr == ' '){
					*ptr++ = '\0';
				}
				local[j] = ptr - tmpBuff;
			}
			local[j] = ptr - tmpBuff;

			memset(capability,'\0',20);
			memset(freeSize,'\0',20);
			size_float =  atoll(tmpBuff+local[1])/(1000*1000.0);
			snprintf(capability,20,"%0.3f(G)",size_float);
							
			size_float =  atoll(tmpBuff+local[3])/(1000*1000.0);
			snprintf(freeSize,20,"%0.3f(G)",size_float);
						
			size_float =  atoll(tmpBuff+local[2])/(1000*1000.0);
			snprintf(used,20,"%0.3f(G)",size_float);
			
			//get filesystem type
			memset(filesystem, '\0', sizeof(filesystem));
			if (getDiskFileSystem(tmpBuff+local[0], filesystem, "/tmp/boa_mount_tmp.txt") != 0)//fail
			{
				memcpy(filesystem, "unkown", strlen("unkown"));
			}
			
			nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">"
				"<td align=center width=\"20%%\" ><font size=\"2\">%s</td>\n"
      			"<td align=center width=\"15%%\" ><font size=\"2\">%s</td>\n"
     			"<td align=center width=\"15%%\" ><font size=\"2\">%s</td>\n"
     			"<td align=center width=\"20%%\" ><font size=\"2\">%s</td>\n"     			
				"<td align=center width=\"10%%\" ><font size=\"2\">%s</td>\n"
				"<td align=center width=\"20%%\" ><font size=\"2\">%s</td>\n"
				"</tr>\n"
      			/* "<td align=center width=\"25%%\" ><input type=\"submit\" name=\"create_share\" value=\"Create Share\" onClick=\"CreateShare('%s')\"></td></tr>\n" */),
				tmpBuff+local[0], capability, freeSize, used, tmpBuff+local[4], filesystem/* ,tmpBuff+local[0] */);

			memset(tmpBuff,0,100);
		}
		j = 0;
	}
	fclose(fp);
	system("rm /tmp/df_tmp.txt");

	return nBytesSent;
}

int Storage_DispalyUser(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	STORAGE_USER_T s_user;
	int i;
	int number;
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
      	"<td align=center width=\"50%%\" ><font size=\"2\"><b>User Name</b></font></td>\n"
      	#if 0
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Group</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Edit</b></font></td>\n"
		#endif
      	"<td align=center width=\"50%%\" ><font size=\"2\"><b>Select</b></font></td></tr>\n"));

	apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
	
	for(i = 0;i < number;i++)
	{
		*((char*)&s_user) = (char)(i+1);
		apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
		
		nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">"
			"<td align=center width=\"50%%\" ><font size=\"2\">%s</td>\n"
			#if 0
      		"<td align=center width=\"25%%\" ><font size=\"2\">%s</td>\n"
      		"<td align=center width=\"25%%\" ><input type=\"submit\" value=\"Edit\" onclick=\"UserEditClick('%d')\"></td>\n"
			#endif
			"<td align=center width=\"50%%\" ><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td></tr>\n"),
			s_user.storage_user_name, /*s_user.storage_user_group,(i+1),*/(i+1));
	}
	return nBytesSent;
}

int Storage_DispalyGroup(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	STORAGE_GROUP_T s_group;
	int i;
	int number;
	
	nBytesSent += req_format_write(wp, ("<tr class=\"tbl_head\">"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Group Name</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Access</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Edit</b></font></td>\n"
      	"<td align=center width=\"25%%\" ><font size=\"2\"><b>Delete</b></font></td></tr>\n"));

	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);

	for(i = 0;i < number;i++)
	{
		*((char*)&s_group) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
		
		nBytesSent += req_format_write(wp, ("<tr class=\"tbl_body\">"
			"<td align=center width=\"25%%\" ><font size=\"2\">%s</td>\n"
      		"<td align=center width=\"25%%\" ><font size=\"2\">%s</td>\n"
      		"<td align=center width=\"25%%\" ><input type=\"submit\" value=\"Edit\" onClick=\"GroupEditClick('%d')\"></td>\n"
      		"<td align=center width=\"25%%\" ><input type=\"checkbox\" name=\"select%d\" value=\"ON\"></td>\n"),
			s_group.storage_group_name, s_group.storage_group_access,(i+1),(i+1));
		
	}
	return nBytesSent;
}

int Storage_GetGroupMember(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	STORAGE_GROUP_T s_group;
	int i;
	int number;

	nBytesSent += req_format_write(wp,
		("<select name=\"Group\">\n"));
	
	apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);

	for(i = 0;i < number;i++)
	{
		*((char*)&s_group) = (char)(i+1);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

		nBytesSent += req_format_write(wp,
			("<option value=\"%d\">%s</option>\n"),
			(i+1),s_group.storage_group_name);

	}

	nBytesSent += req_format_write(wp,
		("</select>\n"));

	return nBytesSent;
}

int StorageGetFolderPath(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	int i;
	FILE *fp;
	unsigned char tmpbuf[256] = {0}, mounton[10] = {0}, path[64] = {0};
	unsigned char *ptr = NULL;

	nBytesSent += req_format_write(wp,
		("<select name=\"folderpath\">\n"));
	
	system("mount > /tmp/mount_getfolderpath_tmp.txt");
	sync();
	fp = fopen("/tmp/mount_getfolderpath_tmp.txt","r");
	if(fp)
	{
		memset(tmpbuf, '\0', sizeof(tmpbuf));
		i = 0;
		//printf("%s %d \n", __FUNCTION__, __LINE__);
		while (fgets(tmpbuf, 256, fp)) 
		{
			//printf("%s %d tmpbuf=%s\n", __FUNCTION__, __LINE__, tmpbuf);
			ptr = strstr(tmpbuf, "/dev/sd");
			if (ptr)
			{
				//exist
				ptr = strstr(tmpbuf, "on");
				if (ptr)
				{
					if (sscanf(ptr, "%s %[^ ]", mounton, path) > 0)
					{
						//printf("%s %d mounton=%s path=%s\n", __FUNCTION__, __LINE__, mounton, path);
						if (i == 0)//i = 0, set default selected
						{
							nBytesSent += req_format_write(wp,
								("<option value=\"%s\" selected>%s</option>\n"),
								path,path);
						}
						else
						{
							nBytesSent += req_format_write(wp,
								("<option value=\"%s\">%s</option>\n"),
								path,path);
						}
						i++;
					}

				}

			}
			memset(tmpbuf, '\0', sizeof(tmpbuf));
			memset(mounton, '\0', sizeof(mounton));
			memset(path, '\0', sizeof(path));
		}
		
		fclose(fp);
		system("rm /tmp/mount_getfolderpath_tmp.txt");
	}

	nBytesSent += req_format_write(wp,
		("</select>\n"));

	return nBytesSent;
}

int StorageGetAccount(request *wp, int argc, char **argv)
{
	int nBytesSent = 0;
	STORAGE_USER_T s_user;
	int i;
	int number;

	nBytesSent += req_format_write(wp,
		("<tr>\
		<td width=\"20%%\"><font size=2><b>Owner:</b></td>\
		<td width=\"80%%\"><font size=2>\n"
		));

	apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
	if (number < 1)
	{
		nBytesSent += req_format_write(wp,("No accounts available, please add account first"));

	}
	
	for(i = 1;i <=number;i++)
	{
		*((char*)&s_user) = (char)i;
		apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);

		nBytesSent += req_format_write(wp,
			("<input type=\"checkbox\" name=\"%s%d\" value=\"ON\">&nbsp;&nbsp;%s\n"),
			SAMBA_USER_NAME,i,s_user.storage_user_name);
		if (i % 4 == 0)
		{
			nBytesSent += req_format_write(wp,
				("</td>\
				  </tr>\
				  <tr>\
				  <td width=\"20%%\"><font size=2><b></b></td>\
				  <td width=\"50%%\"><font size=2>\n"
				));
		}
	}
	
	nBytesSent += req_format_write(wp,
									("</td>\
									  </tr>\n"
									));
	return nBytesSent;
}


void formDiskCreateFolder(request *wp, char * path, char * query)
{
	char *submitUrl,*strLocation,*strFolder;
	char cmdBuffer[40];
	
	strLocation = req_get_cstream_var(wp,("Location"),"");
	strFolder = req_get_cstream_var(wp,("newfolder"),"");

	memset(cmdBuffer,'\0',40);
	snprintf(cmdBuffer,40,"mkdir %s/%s",strLocation,strFolder);
	system(cmdBuffer);

setOk_DiskCreateFolder:
	apmib_update_web(CURRENT_SETTING);
	
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;
}

void formDiskCreateShare(request *wp, char * path, char * query)
{
	char *submitUrl,*strFolderName,*strFolderPath,*strAccount, *strPermission;
	char *strAddShareFolder, *strDelShareFolder, *strDelAllShareFolder, *strVal;
	char tmpBuff[128] = {0}, tmpBuff2[128] = {0};
	FILE	*fp;
	STORAGE_SHAREINFO_T	sharefolder, sharefolder_entry;
	STORAGE_USER_T account;
	int					number,i,j, anon_access;

	char tmpSambaUser[20] = {0};
	
	strAddShareFolder = req_get_cstream_var(wp, ("Add_Share_Folder"), "");
	strDelShareFolder = req_get_cstream_var(wp, ("Delete_Selected"), "");
	strDelAllShareFolder = req_get_cstream_var(wp, ("Delete_All"), "");

	memset((void *)&sharefolder_entry, 0x00, sizeof(sharefolder_entry));
	if (strAddShareFolder[0])
	{
		strFolderName = req_get_cstream_var(wp,("foldername"),""); 
		strFolderPath = req_get_cstream_var(wp,("folderpath"),""); 
		strPermission = req_get_cstream_var(wp,("permission"),"");
		if(!strFolderName[0]){
			strcpy(tmpBuff, ("Folder Name should not be NULL!"));
			goto setErr_DiskCreateShare;
		}
		else
		{
			if (strlen(strFolderName) > (MAX_SHAREINFO_FOLDERNAME_LEN-1))
			{
				strcpy(tmpBuff, ("Folder Name too long!"));
				goto setErr_DiskCreateShare;
			}
			strcpy(sharefolder_entry.storage_sharefolder_name, strFolderName);
		}
		
		if(!strFolderPath[0]){
			strcpy(tmpBuff, ("Folder Path should not be NULL!"));
			goto setErr_DiskCreateShare;
		}
		else
		{
			if (strlen(strFolderPath) > (MAX_SHAREINFO_PATH_LEN-1))
			{
				strcpy(tmpBuff, ("Folder Path too long!"));
				goto setErr_DiskCreateShare;
			}
			strcpy(sharefolder_entry.storage_sharefolder_path, strFolderPath);
			//printf("%s %d path=%s strFolderPath=%s \n", __FUNCTION__, __LINE__, sharefolder_entry.storage_sharefolder_path, strFolderPath);
		}
			
		apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
		memset((void *)&tmpBuff2, 0x00, sizeof(tmpBuff2));
		j = 0;
		for(i = 1;i <=number;i++)
		{
			memset((void *)&account, 0x00, sizeof(account));
			*((char*)&account) = (char)i;
			apmib_get(MIB_STORAGE_USER_TBL,(void*)&account);

			sprintf(tmpSambaUser,"%s%d",SAMBA_USER_NAME,i);		
			strAccount = req_get_cstream_var(wp,(tmpSambaUser),"");		

			if (strAccount[0])
			{
				if (!strcmp(strAccount, "ON"))
				{
					j++;
					if (j == 1)//first one
						strcat(tmpBuff2, account.storage_user_name);
					else
					{
						strcat(tmpBuff2, ",");
						strcat(tmpBuff2, account.storage_user_name);
					}
				}
			}

		}
		//printf("%s %d tmpBuff2=%s \n", __FUNCTION__, __LINE__, tmpBuff2);
		if(!tmpBuff2[0]){
			strcpy(tmpBuff, ("Owner should not be NULL!"));
			goto setErr_DiskCreateShare;
		}
		else
		{
			if (strlen(tmpBuff2) > (MAX_USER_NAME_LEN-1))
			{
				//printf("%s %d tmpBuff2=%s \n", __FUNCTION__, __LINE__, tmpBuff2);
				strcpy(tmpBuff, ("Owner too long!"));
				goto setErr_DiskCreateShare;
			}
			strcpy(sharefolder_entry.storage_account, tmpBuff2);
		}
		
		if (strPermission) {
			if ( strPermission[0] == '0' )
				sharefolder_entry.storage_permission = SAMBA_SHAREINFO_READONLY;
			else if ( strPermission[0] == '1' )
				sharefolder_entry.storage_permission = SAMBA_SHAREINFO_READWRITE;
			else {
				strcpy(tmpBuff, ("Error! Invalid permission."));
				goto setErr_DiskCreateShare;
			}
		}
		else {
			strcpy(tmpBuff, ("Error! permission cannot be empty."));
			goto setErr_DiskCreateShare;
		}

		
		apmib_get(MIB_STORAGE_SHAREINFO_TBL_NUM,(void*)&number);
		if(number >= MAX_SHAREINFO_LEN){
			snprintf(tmpBuff,100,"Share Folder number shoule not be more than %d",MAX_SHAREINFO_LEN);
			goto setErr_DiskCreateShare;
		}
		for(i = 1;i <=number;i++)
		{
			memset(&sharefolder,'\0',sizeof(STORAGE_SHAREINFO_T));
			*((char*)&sharefolder) = (char)(i);
			apmib_get(MIB_STORAGE_SHAREINFO_TBL,(void*)&sharefolder);
		
			if((strlen(sharefolder.storage_sharefolder_name) == strlen(strFolderName)
				&& !strncmp(sharefolder.storage_sharefolder_name,strFolderName,strlen(strFolderName)))&&
				(strlen(sharefolder.storage_sharefolder_path) == strlen(strFolderPath)
				&& !strncmp(sharefolder.storage_sharefolder_path,strFolderPath,strlen(strFolderPath))))
			{
				strcpy(tmpBuff, ("Share Folder Name is already exist,Please choose another Share Folder Name"));
				goto setErr_DiskCreateShare;
			}
		}

		
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_STORAGE_SHAREINFO_DEL, (void *)&sharefolder_entry);
		if ( apmib_set(MIB_STORAGE_SHAREINFO_ADD, (void *)&sharefolder_entry) == 0) {
			strcpy(tmpBuff, ("Add table entry error!"));
			goto setErr_DiskCreateShare;
		}

		//take effect
		
		sprintf(tmpBuff2, "mkdir %s/\"%s\"", strFolderPath, strFolderName);
		//printf("%s %d tmpBuff2=%s \n", __FUNCTION__, __LINE__, tmpBuff2);
		system(tmpBuff2);
		
		#if 0
		apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anon_access);
		system("killall usbStorageAppController 2>/dev/null");
		sprintf(tmpBuff2,"usbStorageAppController -a %d &",anon_access);
		system(tmpBuff2);
		#endif
	}
	
		/* Delete entry */
	if (strDelShareFolder[0]) {
		if ( !apmib_get(MIB_STORAGE_SHAREINFO_TBL_NUM, (void *)&number)) {
			strcpy(tmpBuff, ("Get entry number error!"));
			goto setErr_DiskCreateShare;
		}

		for (i=number; i>0; i--) {
			snprintf(tmpBuff2, 20, "select%d", i);

			strVal = req_get_cstream_var(wp, tmpBuff2, "");
			if ( !strcmp(strVal, "ON") ) {
				*((char *)&sharefolder_entry) = (char)i;
				if ( !apmib_get(MIB_STORAGE_SHAREINFO_TBL, (void *)&sharefolder_entry)) {
					strcpy(tmpBuff, ("Get table entry error!"));
					goto setErr_DiskCreateShare;
				}
				if ( !apmib_set(MIB_STORAGE_SHAREINFO_DEL, (void *)&sharefolder_entry)) {
					strcpy(tmpBuff, ("Delete table entry error!"));
					goto setErr_DiskCreateShare;
				}
			}
		}
	}

	/* Delete all entry */
	if ( strDelAllShareFolder[0]) {
		if ( !apmib_set(MIB_STORAGE_SHAREINFO_DELALL, (void *)&sharefolder_entry)) {
			strcpy(tmpBuff, ("Delete all table error!"));
			goto setErr_DiskCreateShare;
		}
	}
	#if 0
	if(strShareAll[0]){
		//printf("in strShareAll\n");
		memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
		*((char*)&s_group) = (char)atoi(strGroup);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
		s_group.storage_group_sharefolder_flag = 1;
		strcpy(s_group.storage_group_sharefolder,strLocation);
		strcpy(s_group.storage_group_displayname,strDisplayName);
		
		*((char*)&s_groups) = (char)atoi(strGroup);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)(s_groups));
		memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
		apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);

		/*memset(cmdBuffer,'\0',50);
		snprintf(cmdBuffer,50,"chgrp %s %s",s_group.storage_group_name,strLocation);
		system(cmdBuffer);*/
		
		storage_UpdateSambaConf();
		goto setOk_DiskCreateShare;
	}

	for(i = 0;i < atoi(strDirNum);i++)
	{
		//delete Dir
		memset(cmdBuffer,'\0',50);
		snprintf(cmdBuffer,50,"delete%d",i);
		strDelete =  req_get_cstream_var(wp,(cmdBuffer),""); 
		memset(cmdBuffer,'\0',50);
		if(strDelete[0]){
			snprintf(cmdBuffer,50,"rm -rf %s",strDelete);
			system(cmdBuffer);
		}
		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		for(j = 0;j < number;j++)
		{
			memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
			*((char*)&s_group) = (char)(j+1);
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

			if(s_group.storage_group_sharefolder_flag == 1){
				if(!strcmp(s_group.storage_group_sharefolder,strDelete)){
					s_group.storage_group_sharefolder_flag = 0;
					memset(s_group.storage_group_sharefolder,'\0',MAX_FOLDER_NAME_LEN);
					memset(s_group.storage_group_displayname,'\0',MAX_DISPLAY_NAME_LEN);

					*((char*)&s_groups) = (char)(j+1);
					apmib_get(MIB_STORAGE_GROUP_TBL,(void*)(s_groups));
					memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
					apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);
					break;
				}
			}
		}

		snprintf(cmdBuffer,50,"select%d",i);
		strSelect =  req_get_cstream_var(wp,(cmdBuffer),""); 
		if(!strSelect[0])
			continue;

		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		for(j = 0;j < number;j++)
		{
			memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
			*((char*)&s_group) = (char)(j+1);
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

			//printf("flag2:%d.\n",s_group.storage_group_sharefolder_flag);
			if(atoi(strGroup) == (j+1)){
				if(s_group.storage_group_sharefolder_flag == 1){
					memset(tmpBuff,'\0',100);
					strcpy(tmpBuff,"group is already have share folder");
					goto setErr_DiskCreateShare;
				}
				strcpy(s_group.storage_group_sharefolder,strSelect);
				strcpy(s_group.storage_group_displayname,strDisplayName);
				s_group.storage_group_sharefolder_flag = 1;

				*((char*)&s_groups) = (char)atoi(strGroup);
				apmib_get(MIB_STORAGE_GROUP_TBL,(void*)(s_groups));
				memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
				apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);

				/*memset(cmdBuffer,'\0',50);
				snprintf(cmdBuffer,50,"chgrp %s %s",s_group.storage_group_name,strSelect);
				system(cmdBuffer);*/
				
				storage_UpdateSambaConf();
				goto setOk_DiskCreateShare;
			}
		}
	}
	#endif
setOk_DiskCreateShare:
	apmib_update_web(CURRENT_SETTING);
	
	apmib_get(MIB_STORAGE_ANON_ENABLE,(void*)&anon_access);
	system("killall usbStorageAppController 2>/dev/null");
	sprintf(tmpBuff2,"usbStorageAppController -a %d &",anon_access);
	system(tmpBuff2);
	
	submitUrl = req_get_cstream_var(wp, "submit_url", "");
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;
	
setErr_DiskCreateShare:
	ERR_MSG(tmpBuff);

}

void formDiskCfg(request *wp, char * path, char * query)
{
	char *submitUrl,*strLocation,*strDeleteAll,*strDeleteSelect,*strDeleteVal;
	int number,i,shareNum = 0;
	char tmpBuff[20];
	
	STORAGE_GROUP_T	s_group;
	STORAGE_GROUP_T s_groups[2];
		
	submitUrl = req_get_cstream_var(wp, "submit_url", "");	
	
	if(strcmp(submitUrl,"/storage_createsharefolder.htm")){
		strDeleteAll =  req_get_cstream_var(wp, "Delete_All", "");
		if(strDeleteAll[0]){
			apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
			for(i = 0;i < number;i++)
			{
				memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
				*((char*)&s_group) = (char)(i+1);
				apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

				if(s_group.storage_group_sharefolder_flag == 1){
					s_group.storage_group_sharefolder_flag = 0;
					memset(s_group.storage_group_sharefolder,'\0',MAX_FOLDER_NAME_LEN);
					memset(s_group.storage_group_displayname,'\0',MAX_DISPLAY_NAME_LEN);
					
					memset(s_groups,'\0',2*sizeof(STORAGE_GROUP_T));
					*((char*)s_groups) = (char)(i+1);
					apmib_get(MIB_STORAGE_GROUP_TBL,(void*)s_groups);
					memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
					apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);
				}
			}
			goto setOk_DiskCfg;
		}
				
		strDeleteSelect =  req_get_cstream_var(wp, "Delete_Selected", "");
		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		for(i = 0;i < number;i++)
		{
			memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
			*((char*)&s_group) = (char)(i+1);
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

			if(s_group.storage_group_sharefolder_flag == 1){
				memset(tmpBuff,'\0',20);
				snprintf(tmpBuff,20,"delete%d",i);
				strDeleteVal = req_get_cstream_var(wp, tmpBuff, "");

				if(!strcmp(strDeleteVal,s_group.storage_group_name)){
					s_group.storage_group_sharefolder_flag = 0;
					memset(s_group.storage_group_sharefolder,'\0',MAX_FOLDER_NAME_LEN);
					memset(s_group.storage_group_displayname,'\0',MAX_DISPLAY_NAME_LEN);

					memset(s_groups,'\0',2*sizeof(STORAGE_GROUP_T));
					*((char*)s_groups) = (char)(i+1);
					apmib_get(MIB_STORAGE_GROUP_TBL,(void*)s_groups);
					memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
					apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);
				}
			}
		}
		storage_UpdateSambaConf();
	}else{
		strLocation =  req_get_cstream_var(wp, "Create_Share", "");
		apmib_set(MIB_STORAGE_FOLDER_LOCAL,(void*)strLocation);
		goto setOk_DiskCfg;
	}				
setOk_DiskCfg:
	apmib_update_web(CURRENT_SETTING);
	if(submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;

}

void formDiskManagementAnon(request *wp, char * path, char * query)
{
	char *submitUrl,*strAnonDiskEnable,*strAnonEnable;
	char *strAnonFtpEnable;
	int mib_val = 0;
	char cmdBuffer[100] = {0};

	strAnonEnable = req_get_cstream_var(wp,("AnonEnabled"),"");

	if(!strcmp(strAnonEnable,"ON")){
		mib_val = 1;
		apmib_set(MIB_STORAGE_ANON_ENABLE,(void*)&mib_val);
		#if 0
		//strAnonFtpEnable = req_get_cstream_var(wp,("anonymous_ftp_enable"),""); 
		strAnonDiskEnable = req_get_cstream_var(wp,("anonymous_disk_enable"),"");		
		/*if(!strcmp(strAnonFtpEnable,"enabled")){
			mib_val = 1;
			apmib_set(MIB_STORAGE_ANON_FTP_ENABLE,(void*)&mib_val);
		}else{
			mib_val = 0;
			apmib_set(MIB_STORAGE_ANON_FTP_ENABLE,(void*)&mib_val);
		}*/

		if(!strcmp(strAnonDiskEnable,"enabled")){
			mib_val = 1;
			apmib_set(MIB_STORAGE_ANON_DISK_ENABLE,(void*)&mib_val);
		}else{
			mib_val = 0;
			apmib_set(MIB_STORAGE_ANON_DISK_ENABLE,(void*)&mib_val);
		}
		#endif
	}else{
		mib_val = 0;
		apmib_set(MIB_STORAGE_ANON_ENABLE,(void*)&mib_val);
		
		apmib_set(MIB_STORAGE_ANON_DISK_ENABLE,(void*)&mib_val);
		//apmib_set(MIB_STORAGE_ANON_FTP_ENABLE,(void*)&mib_val);
	}

setOk_AnonAccessCfg:
	apmib_update_web(CURRENT_SETTING);
	system("killall usbStorageAppController 2>/dev/null");
	snprintf(cmdBuffer,100,"usbStorageAppController -a %d &",mib_val);
	system(cmdBuffer);
	//storage_UpdateSambaConf();
	
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;

}

void formDiskManagementUser(request *wp, char * path, char * query)
{
	char *submitUrl, *strDeleAll,*strVal,*strUserIndex;
	char tmpBuff[20];
	char cmdBuffer[30];
	int number,i;
	STORAGE_USER_T	s_user;
	int	index;
	
	submitUrl = req_get_cstream_var(wp, "submit_url", "");
	if(strcmp(submitUrl,"/storage_edituser.htm")){
		apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
		strDeleAll = req_get_cstream_var(wp,("Delete_All"),"");
		if(strDeleAll[0]){
		
			for(i = 0; i < number;i++)
			{
				memset(&s_user,'\0',sizeof(STORAGE_USER_T));
				*((char*)&s_user) = (char)(i+1);
				apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
			
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"deluser \"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
#ifndef CONFIG_APP_SAMBA_3_6_24				
				snprintf(cmdBuffer,30,"smbpasswd -del \"%s\"",s_user.storage_user_name);
#else
				snprintf(cmdBuffer,30,"smbpasswd -x \"%s\"",s_user.storage_user_name);
#endif
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"rm -rf /home/\"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
			}

			apmib_set(MIB_STORAGE_USER_DELALL,(void*)&s_user);
			goto setOk_deleteUser;
		}


		for(i = number;i > 0;i--)
		{
			memset(tmpBuff,'\0',20);
			snprintf(tmpBuff, 20, "select%d", i);
			strVal =  req_get_cstream_var(wp,tmpBuff,"");

			if(!strcmp(strVal,"ON")){
				*((char*)&s_user) = (char)i;
				apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
	
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"deluser \"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
#ifndef CONFIG_APP_SAMBA_3_6_24	
				snprintf(cmdBuffer,30,"smbpasswd -del \"%s\"",s_user.storage_user_name);
#else
				snprintf(cmdBuffer,30,"smbpasswd -x \"%s\"",s_user.storage_user_name);
#endif
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"rm -rf /home/\"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
			
				apmib_set(MIB_STORAGE_USER_DEL,(void*)&s_user);
			}	
		}
	}else{
		strUserIndex = req_get_cstream_var(wp, "userindex", "");
		index = atoi(strUserIndex);
		if(strUserIndex[0])
			apmib_set(MIB_STORAGE_USER_EDIT_INDEX,(void*)&index);
		goto setOk_deleteUser;
	}
	
setOk_deleteUser:
	apmib_update_web(CURRENT_SETTING);
	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;
}

void formDiskManagementGroup(request *wp, char * path, char * query)
{
	char *submitUrl, *strDeleAll,*strVal,*strGroupIndex;
	char tmpBuff[20];
	char cmdBuffer[30];
	int number,i,user_num,j;
	STORAGE_GROUP_T	s_group;
	STORAGE_USER_T	s_user;
	STORAGE_USER_T	s_users[2] = {0};
	int			 index;
	submitUrl = req_get_cstream_var(wp, "submit_url", "");
		
	if(strcmp(submitUrl,"/storage_editgroup.htm")){
		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&user_num);

		/*Delete All Group Process*/
		strDeleAll = req_get_cstream_var(wp,("Delete_All"),"");
		if(strDeleAll[0]){
			for(i = 0;i < user_num;i++)
			{
				memset(&s_user,'\0',sizeof(STORAGE_USER_T));
				*((char*)&s_user) = (char)(i+1);
				apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
				
				strcpy(s_user.storage_user_group,"--");
				*((char*)s_users) = (char)(i+1);
				apmib_get(MIB_STORAGE_USER_TBL,(void*)(s_users));
				memcpy(&(s_users[1]),&s_user,sizeof(STORAGE_USER_T));
				apmib_set(MIB_STORAGE_USER_MOD,(void*)s_users);

				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"deluser \"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"rm -rf  /home/\"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"adduser \"%s\"",s_user.storage_user_name);
				system(cmdBuffer);
				//may be need modify

			}

			for(i = 0;i < number;i++)
			{
				memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
				*((char*)&s_group) = (char)(i+1);
				apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
					
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"delgroup %s",s_group.storage_group_name);
				system(cmdBuffer);

			}
			
			apmib_set(MIB_STORAGE_GROUP_DELALL,(void*)&s_group);
			storage_UpdateSambaConf();
			goto setOk_deleteGroup;
		}
	

		/*Delete Selected Group Process*/
		for(i = number;i > 0;i--)
		{
			memset(tmpBuff,'\0',20);
			snprintf(tmpBuff, 20, "select%d", i);
			strVal =  req_get_cstream_var(wp,tmpBuff,"");

			if(!strcmp(strVal,"ON")){
				*((char*)&s_group) = (char)i;
				apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
				apmib_set(MIB_STORAGE_GROUP_DEL,(void*)&s_group);

				
				memset(cmdBuffer,'\0',30);
				snprintf(cmdBuffer,30,"delgroup %s",s_group.storage_group_name);
				system(cmdBuffer);

				//apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&user_num);
				for(j = 0;j < user_num;j++)
				{
					*((char*)&s_user) = (char)(j+1);
					apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
					
					if(!strcmp(s_user.storage_user_group,s_group.storage_group_name)){
						memset(s_user.storage_user_group,'\0',10);
						strcpy(s_user.storage_user_group,"--");
						*((char*)s_users) = (char)(j+1);
						apmib_get(MIB_STORAGE_USER_TBL,(void*)(s_users));
						memcpy(&(s_users[1]),&s_user,sizeof(STORAGE_USER_T));
						apmib_set(MIB_STORAGE_USER_MOD,(void*)s_users);

						memset(cmdBuffer,'\0',30);
						snprintf(cmdBuffer,30,"deluser \"%s\"",s_user.storage_user_name);
						system(cmdBuffer);
						memset(cmdBuffer,'\0',30);
						snprintf(cmdBuffer,30,"rm -rf  /home/\"%s\"",s_user.storage_user_name);
						system(cmdBuffer);
						memset(cmdBuffer,'\0',30);
						snprintf(cmdBuffer,30,"adduser \"%s\"",s_user.storage_user_name);
						system(cmdBuffer);
						//may be need modify
					}
				}
						
			}
		}
		storage_UpdateSambaConf();
		goto setOk_deleteGroup;
	}else{
		strGroupIndex = req_get_cstream_var(wp, "groupindex", "");
		index = atoi(strGroupIndex);
		if(strGroupIndex[0]){
			apmib_set(MIB_STORAGE_GROUP_EDIT_INDEX,(void*)&index);
		}
		goto setOk_deleteGroup;
	}
		
setOk_deleteGroup:
	apmib_update_web(CURRENT_SETTING);
			
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;
}

void formDiskCreateUser(request *wp, char * path, char * query)
{
		char *submitUrl, *strName, *strPasswd, *strConfPasswd,*strGroup;
		char tmpBuff[100];
		char cmdBuffer[100];
		STORAGE_USER_T	s_user;
		STORAGE_GROUP_T	s_group;
		
		int number,i;

		strName = req_get_cstream_var(wp,("username"),"");
		strPasswd = req_get_cstream_var(wp,("newpass"),"");
		strConfPasswd = req_get_cstream_var(wp,("confpass"),"");
		strGroup = req_get_cstream_var(wp,("Group"),"");

		if(!strName[0]){
			strcpy(tmpBuff, ("userName should not be NULL!"));
			goto setErr_createUser;
		}
	
		if(!strPasswd[0] || !strConfPasswd[0]){
			strcpy(tmpBuff, ("passwd or confpasswd should not be NULL!"));
			goto setErr_createUser;
		}

		if(strcmp(strPasswd,strConfPasswd)){
			strcpy(tmpBuff, ("passwd should be equal to confpasswd"));
			goto setErr_createUser;
		}

		if(!strcmp(strName,"root") || !strcmp(strName,"nobody")){
			strcpy(tmpBuff,"user Name should not be nobody or root");
			goto setErr_createUser;
		}

		if(strchr(strName,',') || strchr(strName,';') || strchr(strName,' ')){
			strcpy(tmpBuff,"Invalid samba user Name,please do not contain commas, colons or spaces");
			goto setErr_createUser;
		}

		apmib_get(MIB_STORAGE_USER_TBL_NUM,(void*)&number);
		if(number >= MAX_USER_NUM){
			snprintf(tmpBuff,100,"user num shoule not be more than %d",MAX_USER_NUM);
			goto setErr_createUser;
		}
		//printf("%s %d number=%d\n", __FUNCTION__, __LINE__, number);
		for(i = 1;i <= number;i++)
		{
			memset(&s_user,'\0',sizeof(STORAGE_USER_T));
			*((char*)&s_user) = (char)(i);
			apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
			if(strlen(s_user.storage_user_name) == strlen(strName)
				&& !strncmp(s_user.storage_user_name,strName,strlen(strName))){
				strcpy(tmpBuff, ("user name is already exist,Please choose another user name"));
				goto setErr_createUser;
			}
		}

		*((char*)&s_group) = (char)(atoi(strGroup));
		if(atoi(strGroup) == 0){
			memset(&s_user,'\0',sizeof(STORAGE_USER_T));
			strcpy(s_user.storage_user_group,"--");	
		}else{
			*((char*)&s_group) = (char)(atoi(strGroup));
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
			memset(&s_user,'\0',sizeof(STORAGE_USER_T));
			strcpy(s_user.storage_user_group,s_group.storage_group_name);
		}	
				
		strcpy(s_user.storage_user_name,strName);
		strcpy(s_user.storage_user_password,strPasswd);
		
		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_STORAGE_SHAREINFO_DEL, (void *)&s_user);
		if (apmib_set(MIB_STORAGE_USER_ADD,(void*)&s_user)==0){
			strcpy(tmpBuff, ("Add table entry error!"));
			goto setErr_createUser;
		}

		memset(cmdBuffer,'\0',100);
		if(atoi(strGroup) == 0){
			snprintf(cmdBuffer,100,"adduser \"%s\"",strName);
		}else{
			snprintf(cmdBuffer,100,"adduser -G \"%s\" \"%s\"",s_group.storage_group_name,strName);
		}

		system(cmdBuffer);
		memset(cmdBuffer,'\0',100);
#ifndef CONFIG_APP_SAMBA_3_6_24
		snprintf(cmdBuffer,100,"smbpasswd \"%s\" \"%s\"",strName,strPasswd);
#else
		snprintf(cmdBuffer,100,"echo -e \"\"%s\"\\n\"%s\"\\n\" | smbpasswd -a \"%s\"",strPasswd,strPasswd,strName);
		fprintf(stderr,"samba-3.6.24\n");
#endif		
		printf("%s %d cmdBuffer=%s \n", __FUNCTION__, __LINE__,cmdBuffer );
		system(cmdBuffer);
	
	setOk_createUser:
		apmib_update_web(CURRENT_SETTING);
	
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	
		if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
		return;
	
	setErr_createUser:
		ERR_MSG(tmpBuff);
}


void formDiskEditUser(request *wp, char * path, char * query)
{
	char 	*submitUrl, *strOrigPasswd,*strNewPasswd,*strConfPasswd,*strGroup;
	int	    index;
	char	tmpBuff[100];
	char	cmdBuffer[50];
	
	STORAGE_USER_T 	s_user;
	STORAGE_GROUP_T	s_group;
	STORAGE_USER_T	s_users[2] = {0};

	memset(tmpBuff,'\0',100);
	memset(cmdBuffer,'\0',50);
	strOrigPasswd = req_get_cstream_var(wp,("origpass"),"");
	strNewPasswd = req_get_cstream_var(wp,("newpass"),"");
	strConfPasswd = req_get_cstream_var(wp,("confpass"),"");
	strGroup = req_get_cstream_var(wp,("Group"),"");

	apmib_get(MIB_STORAGE_USER_EDIT_INDEX,(void*)&index);
	memset(&s_user,'\0',sizeof(STORAGE_USER_T));
	*((char*)&s_user) = (char)index;
	apmib_get(MIB_STORAGE_USER_TBL,(void*)&s_user);
	
	if(strcmp(strOrigPasswd,s_user.storage_user_password)){
		strcpy(tmpBuff,"Orig Password is wrong,Please Enter the password again");
		goto setError_EditUser;
	}
	if(!strNewPasswd[0] || !strConfPasswd[0]){
		strcpy(tmpBuff,"newpassword or confpassword should not be empty");
		goto setError_EditUser;
	}
	if(strcmp(strNewPasswd,strConfPasswd)){
		strcpy(tmpBuff,"newpassword is not equal confpassword");
		goto setError_EditUser;
	}

	strcpy(s_user.storage_user_password,strNewPasswd);	
	if(atoi(strGroup) == 0)
		strcpy(s_user.storage_user_group,"--");
	else{
		memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
		*((char*)&s_group) = (char)atoi(strGroup);
		apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
		strcpy(s_user.storage_user_group,s_group.storage_group_name);
	}

	*((char*)s_users) = (char)index;
	apmib_get(MIB_STORAGE_USER_TBL,(void*)s_users);
	memcpy(&(s_users[1]),&s_user,sizeof(STORAGE_USER_T));
	apmib_set(MIB_STORAGE_USER_MOD,(void*)s_users);
//delete user
#ifndef CONFIG_APP_SAMBA_3_6_24
	snprintf(cmdBuffer,50,"smbpasswd -del \"%s\"",s_user.storage_user_name);
#else
	snprintf(cmdBuffer,50,"smbpasswd -x \"%s\"",s_user.storage_user_name);
#endif
	system(cmdBuffer);
	memset(cmdBuffer,'\0',50);
//add user
#ifndef CONFIG_APP_SAMBA_3_6_24
	snprintf(cmdBuffer,50,"smbpasswd \"%s\" \"%s\"",s_user.storage_user_name,s_user.storage_user_password);
#else
	snprintf(cmdBuffer,50,"echo -e \"\"%s\"\\n\"%s\"\\n\" | smbpasswd -a \"%s\"",s_user.storage_user_password,s_user.storage_user_password,s_user.storage_user_name);
	fprintf(stderr,"samba-3.6.24");
#endif
	system(cmdBuffer);
	
setOk_EditUser:
	apmib_update_web(CURRENT_SETTING);
	storage_UpdateSambaConf();
		
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;	

setError_EditUser:
	ERR_MSG(tmpBuff);
}

void formDiskEditGroup(request *wp, char * path, char * query)
{
	char 	*submitUrl, *strAccess;
	int		 index;
	
	STORAGE_GROUP_T s_group;
	STORAGE_GROUP_T	s_groups[2] = {0};
	strAccess = req_get_cstream_var(wp,("Access"),"");

	apmib_get(MIB_STORAGE_GROUP_EDIT_INDEX,(void*)&index);
	memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
	*((char*)&s_group) = (char)index;
	apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);
	strcpy(s_group.storage_group_access,strAccess);

	*((char*)s_groups) = (char)index;
	apmib_get(MIB_STORAGE_GROUP_TBL,(void*)s_groups);
	memcpy(&(s_groups[1]),&s_group,sizeof(STORAGE_GROUP_T));
	apmib_set(MIB_STORAGE_GROUP_MOD,(void*)s_groups);

setOk_EditGroup:
	apmib_update_web(CURRENT_SETTING);
	storage_UpdateSambaConf();
		
	submitUrl = req_get_cstream_var(wp, "submit-url", "");	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;	
}


void formDiskCreateGroup(request *wp, char * path, char * query)
{
		char *submitUrl, *strName,*strAccess;
		char tmpBuff[100];
		char cmdBuffer[100];
		STORAGE_GROUP_T	s_group;
		unsigned char	 number,i;
	
		strName = req_get_cstream_var(wp,("groupname"),"");
		strAccess = req_get_cstream_var(wp,("Access"),"");

		if(!strcmp(strName,"root") || !strcmp(strName,"nobody")){
			strcpy(tmpBuff,"group name should not be root or nobody");
			goto setErr_createGroup;
		}
			
		apmib_get(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
		
		if(number >= MAX_GROUP_NUM){
			snprintf(tmpBuff,100,"group num shoule not be more than %d",MAX_GROUP_NUM);
			goto setErr_createGroup;
		}

		for(i = 0;i <number;i++)
		{
			memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
			*((char*)&s_group) = (char)(i+1);
			apmib_get(MIB_STORAGE_GROUP_TBL,(void*)&s_group);

			if(!strcmp(strName,s_group.storage_group_name)){
				strcpy(tmpBuff,("group name repeat"));
				goto setErr_createGroup;
			}

			if(strlen(s_group.storage_group_name) == strlen(strName)
				&& strncmp(s_group.storage_group_name,strName,strlen(strName))){
				strcpy(tmpBuff, ("group name is already exist,Please choose another group name"));
				goto setErr_createGroup;
			}
		}
	
		number++;
		apmib_set(MIB_STORAGE_GROUP_TBL_NUM,(void*)&number);
	
		memset(&s_group,'\0',sizeof(STORAGE_GROUP_T));
		strcpy(s_group.storage_group_name,strName);
		strcpy(s_group.storage_group_access,strAccess);
		s_group.storage_group_sharefolder_flag = 0;
		apmib_set(MIB_STORAGE_GROUP_ADD,(void*)&s_group);

		memset(cmdBuffer,'\0',100);
		snprintf(cmdBuffer,100,"addgroup \"%s\"",strName);
		system(cmdBuffer);
		
	setOk_createGroup:
		apmib_update_web(CURRENT_SETTING);
		
		submitUrl = req_get_cstream_var(wp, "submit-url", "");	
		if (submitUrl[0])
			send_redirect_perm(wp, submitUrl);
		return;
		
	setErr_createGroup:
		ERR_MSG(tmpBuff);
}

static int diskFormat(unsigned char *partition, unsigned char *systype)
{
	unsigned char tmpbuff[128] = {0};

	if (!partition || !systype)
		return -1;
	
	//umount first
	sprintf(tmpbuff,  "umount /dev/%s >/dev/null 2>&1", partition);
	system(tmpbuff);
	
	//format
	if (!strncmp(systype, EXT_SYSTYPE_PREFIX, strlen(EXT_SYSTYPE_PREFIX)))
	{
		sprintf(tmpbuff,  "%s -t %s /dev/%s >/dev/null 2>&1", EXT_FORMAT_TOOL, systype, partition);
	}
	else if (!strncmp(systype, FAT_SYSTYPE_PREFIX, strlen(FAT_SYSTYPE_PREFIX)))
	{
		if (!strncmp(systype, FAT16_SYSTYPE, strlen(FAT16_SYSTYPE)))
		{
			sprintf(tmpbuff,  "%s -F 16 /dev/%s >/dev/null 2>&1", FAT_FORMAT_TOOL,partition);
		}
		else if (!strncmp(systype, FAT32_SYSTYPE, strlen(FAT32_SYSTYPE)))
		{
			sprintf(tmpbuff,  "%s -F 32 /dev/%s >/dev/null 2>&1", FAT_FORMAT_TOOL,partition);
		}
		else
			sprintf(tmpbuff,  "%s /dev/%s >/dev/null 2>&1", FAT_FORMAT_TOOL,partition);
	}
	else if (!strncmp(systype, NTFS_SYSTYPE_PREFIX, strlen(NTFS_SYSTYPE_PREFIX)))
	{
		sprintf(tmpbuff,  "%s -f /dev/%s >/dev/null 2>&1", NTFS_FORMAT_TOOL, partition);
	}
	else
	{
		printf("%s %d currently not support! systype=%s \n", __FUNCTION__, __LINE__, systype);
		return -1;
	}	
	
	//printf("%s %d tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
	system(tmpbuff);

	//mount back
	sprintf(tmpbuff, "mkdir -p /var/tmp/usb/%s >/dev/null 2>&1", partition);
	system(tmpbuff);
	if (!strncmp(systype, NTFS_SYSTYPE_PREFIX, strlen(NTFS_SYSTYPE_PREFIX)))
	{
		sprintf(tmpbuff,  "%s /dev/%s /var/tmp/usb/%s -o force >/dev/null 2>&1", NTFS_COMMAND, partition, partition);
		system(tmpbuff);
	}
	else
	{	
		if( !strncmp(systype, "ext4", strlen("ext4")) ){
			sprintf(tmpbuff,  "mount -t ext4 /dev/%s /var/tmp/usb/%s >/dev/null 2>&1", partition, partition);
		}else{
			sprintf(tmpbuff,  "mount /dev/%s /var/tmp/usb/%s >/dev/null 2>&1", partition, partition);
		}	
		system(tmpbuff);
	}
	
	return 0;		
}

void formDiskFormat(request *wp, char * path, char * query)
{
	char *submitUrl,*strPartition,*strSystype;
	char tmpBuff[100];
	char cmdBuffer[100] = {0};

	strPartition = req_get_cstream_var(wp,("partition"),"");
	strSystype = req_get_cstream_var(wp,("systype"),"");

	
	if(!strPartition[0]){
		strcpy(tmpBuff, ("partition should not be NULL!"));
		goto setErr_diskFormat;
	}

	if(!strSystype[0]){
		strcpy(tmpBuff, ("system type should not be NULL!"));
		goto setErr_diskFormat;
	}
	//printf("%s %d strPartition=%s strSystype=%s \n", __FUNCTION__, __LINE__, strPartition, strSystype);
	if (diskFormat(strPartition, strSystype) != 0)
	{
		strcpy(tmpBuff, ("Format error!"));
		goto setErr_diskFormat;
	}

setOk_diskFormat:
	submitUrl = req_get_cstream_var(wp, "submit_url", "");	
	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	return;
		
setErr_diskFormat:
	ERR_MSG(tmpBuff);
	return;
}

int diskCheck(add_partition_info *addinfo, int partition_number, unsigned long long totalsize, int *flag)
{
	#define MAX_DISK_SIZE_GB  2000
	int num1, i;
	
	//safety check
	if (!addinfo || !flag)
		return -1;
	
	if (partition_number > MAX_ADD_PARTITION_NUMBER)
		return -1;//over size
		
	*flag = 0;
	num1 = (totalsize*1024)/(1000*1000*1000); //GB
	if (num1 > MAX_DISK_SIZE_GB)
	{
		*flag  = 1;
		return 0;
	}
	
	for (i = 0; i < partition_number; i++)
	{
		num1 = addinfo[i].size /(1000); //GB
		if (num1 > MAX_DISK_SIZE_GB)
		{
			*flag  = 1;
			return 0;
		}
	}

	return 0;
	
}

int doDiskPartition(add_partition_info *addinfo, int partition_number, unsigned char *dev, int flag)
{
	unsigned char tmpbuff[512] = {0};
	unsigned long long start = 0, end = 0;
	int primary = 0, extend = 0, logical = 0;
	unsigned long long totalsize = 0, partedsize = 0;
	int i = 0;
	
	if (!addinfo || !dev)
		return -1;
	
	if (partition_number > MAX_ADD_PARTITION_NUMBER)
		return -1;//over size

	if (partition_number <= 4)
	{
		primary = partition_number;
		extend = 0;
		logical = 0;
	}
	else
	{
		primary = 3;
		extend = 1;
		logical = partition_number - primary;
	}

	for (i = 0; i < partition_number; i++)
	{
		totalsize += addinfo[i].size;
	}
	
	memset(tmpbuff, "\0", sizeof(tmpbuff));
	if (flag)
	{
		//greater than 2TB , using gpt
		sprintf(tmpbuff, "parted /dev/%s -s mklabel gpt >/dev/null 2>&1", dev);
	}
	else
	{
		//lessthan 2TB , using MBR
		sprintf(tmpbuff, "parted /dev/%s -s mklabel msdos >/dev/null 2>&1", dev);
	}
	system(tmpbuff);
	#if 0
	printf("%s %d tmpbuff=%s flag=%d primary=%d extend=%d logical=%d totalsize=%llu\n", __FUNCTION__, __LINE__,
		tmpbuff, flag, primary, extend, logical, totalsize);
	for (i = 0; i < partition_number; i++)
	{
		printf("%s %d i=%d number=%d size=%llu \n", __FUNCTION__, __LINE__, i, addinfo[i].number, addinfo[i].size);
	}
	#endif
	
	//now add partition.
	//add primary partition.
	start = 0;
	end = 0;
	for (i = 0; i < primary; i++)
	{
		memset(tmpbuff, "\0", sizeof(tmpbuff));
		if (i == 0)
		{
			start += 0;
			end += addinfo[i].size;
			//printf("%s %d start=%llu end=%llu\n", __FUNCTION__, __LINE__, start, end);
		}
		else
		{
			start += addinfo[i-1].size;
			end = start + addinfo[i].size;
		}
		sprintf(tmpbuff, "parted /dev/%s -s mkpart primary %lluMB %lluMB >/dev/null 2>&1", dev, start, end);
		system(tmpbuff);
		//printf("%s %d tmpbuff=%s start=%llu end=%llu\n", __FUNCTION__, __LINE__, tmpbuff, start, end);
		partedsize += addinfo[i].size;
	}
	//add extend partition
	if (extend)
	{
		memset(tmpbuff, "\0", sizeof(tmpbuff));
		sprintf(tmpbuff, "parted /dev/%s -s mkpart extended %lluMB %lluMB >/dev/null 2>&1", dev, partedsize, totalsize);
		system(tmpbuff);
		//printf("%s %d tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
	}
	//add logical partition
	if (logical)
	{
		for ( ; i < partition_number; i++)
		{
			if (i == primary)
			{
				start = partedsize;
				end = start + addinfo[i].size;
			}
			else
			{
				start += addinfo[i-1].size;
				end = start + addinfo[i].size;
			}
			sprintf(tmpbuff, "parted /dev/%s -s mkpart logical %lluMB %lluMB >/dev/null 2>&1", dev, start, end);
			system(tmpbuff);
			//printf("%s %d tmpbuff=%s \n", __FUNCTION__, __LINE__, tmpbuff);
		}
	}
	//update kernel partition table
	system("partprobe >/dev/null 2>&1");

	return 0;
	
}

int diskPartition(add_partition_info *addinfo, int partition_number, unsigned char *dev)
{
	unsigned char tmpbuff[512] = {0};
	struct __disk_partition_info partition_info[MAX_DEVICE_NUMBER];
	disk_parted_print_info parted_print_info[MAX_LIST_PARTITION_NUMBER];
	int i, j, number, total, flag;
	unsigned long long totalsize;

	//safety check
	if (!addinfo || !dev)
		return -1;
	
	if (partition_number > MAX_ADD_PARTITION_NUMBER)
		return -1;//over size

	
	memset(partition_info, 0x00, sizeof(partition_info));
	partitionInfo(partition_info, "cat /proc/partitions", "/var/tmp/boa_parted_formp.txt", tmpbuff, 512, 2, &number);
	
	for (i = 0; i < number; i++)
	{
		if (!strncasecmp(dev, partition_info[i].name, 3))
			break;
	}
	if (i == number)//dev not exist
		return -1;

	
	//if dev already parted , delete original partition
	for (i = 0; i < number; i++)
	{
		if (!strncasecmp(dev, partition_info[i].name, 3))
		{
			total = 0;
			memset(parted_print_info, 0x00, sizeof(parted_print_info));
			memset(tmpbuff, '\0', sizeof(tmpbuff));
			sprintf(tmpbuff, "/dev/%s", partition_info[i].name);
			pharsePartedPrint(parted_print_info, "parted", tmpbuff, "/var/tmp/boa_parted_tmpp.txt", &total);
			for (j = 0; j < total; j++)
			{
				memset(tmpbuff, '\0', sizeof(tmpbuff));
				sprintf(tmpbuff, "parted /dev/%s -s rm %s >/dev/null 2>&1", partition_info[i].name, parted_print_info[j].number);
				//printf("%s %d: del all disk partition cmd=%s \n", __FUNCTION__, __LINE__, tmpbuff);
				system(tmpbuff);
			}
			totalsize = partition_info[i].totalsize;
		}
	}

	//check disk total size greater than 2TB? or single partition size greater than 2TB?
	flag = 0;
	diskCheck(addinfo, partition_number, totalsize, &flag);
	//printf("%s %d flag=%d\n", __FUNCTION__, __LINE__, flag);
	if (doDiskPartition(addinfo, partition_number, dev, flag) < 0)
		return -1;

	return 0;	
}

void formDiskPartition(request *wp, char * path, char * query)
{
	char *submitUrl,*strDev,*strParNum;
	char *strAdd, *strDelSel, *strDelAll, *strVal;
	char *endptr;
	unsigned char tmpbuff[512] = {0};
	unsigned char lan_ip_buf[30] = {0}, lan_ip[30] = {0};
	int number = 0, i = 0, j = 0, total = 0, partition_number, waittime = 0;
	struct __disk_partition_info partition_info[MAX_DEVICE_NUMBER];
	disk_parted_print_info parted_print_info[MAX_LIST_PARTITION_NUMBER];
	add_partition_info addinfo[MAX_ADD_PARTITION_NUMBER];
		
	strAdd = req_get_cstream_var(wp, ("Add_Partition"), "");
	strDelSel = req_get_cstream_var(wp, ("Delete_Selected"), "");
	strDelAll = req_get_cstream_var(wp, ("Delete_All"), "");
	memset(partition_info, 0x00, sizeof(partition_info));
	partitionInfo(partition_info, "cat /proc/partitions", "/var/tmp/boa_parted_form.txt", tmpbuff, 512, 2, &number);

	if (strAdd[0])
	{
		strDev = req_get_cstream_var(wp,("devicename"),""); 
		strParNum = req_get_cstream_var(wp,("partitionnum"),""); 
		
			
		if(!strDev[0]){
			strcpy(tmpbuff, ("Device Name should not be NULL!"));
			goto setErr_DiskPartition;
		}
		//delete original partition first.
		for (i = 0; i < number; i++)
		{
			if (!strncasecmp(strDev, partition_info[i].name, 3))
			{
				total = 0;
				memset(parted_print_info, 0x00, sizeof(parted_print_info));
				memset(tmpbuff, '\0', sizeof(tmpbuff));
				sprintf(tmpbuff, "/dev/%s", partition_info[i].name);
				pharsePartedPrint(parted_print_info, "parted", tmpbuff, "/var/tmp/boa_parted_tmp.txt", &total);
				if (total)
				{
					strcpy(tmpbuff, ("Device already partitioned, if want to re-partition, please delete original partition first!"));
					goto setErr_DiskPartition;
				}
			}
		}
		
		if(!strParNum[0]){
			strcpy(tmpbuff, ("Partition Number should not be NULL!"));
			goto setErr_DiskPartition;
		}
		else
		{
			partition_number = atoi(strParNum);
		}
		memset(addinfo, 0x00, sizeof(addinfo));
		//printf("%s %d sizeof(addinfo)=%d partition_number=%d \n", __FUNCTION__, __LINE__, sizeof(addinfo), partition_number);
		for (i = 1; i <= partition_number; i++)
		{
			memset(tmpbuff, '\0', sizeof(tmpbuff));
			if (i == partition_number)
				sprintf(tmpbuff, "hiddenpartitionsize");
			else				
				sprintf(tmpbuff, "newpartitionsize%d", i);
			strVal = req_get_cstream_var(wp,(tmpbuff),""); 
			if (!strVal[0]){
				memset(tmpbuff, '\0', sizeof(tmpbuff));
				sprintf(tmpbuff, "the %d partition size should not be NULL!", i);
				goto setErr_DiskPartition;
			}
			else
			{
				addinfo[i-1].number = i;
				addinfo[i-1].size = strtoull(strVal, &endptr, 0);
				//printf("%s %d strVal=%s size=%llu\n", __FUNCTION__, __LINE__, strVal, addinfo[i-1].size);
			}
		}
		
		if (diskPartition(addinfo, partition_number, strDev) < 0)
		{
			memset(tmpbuff, '\0', sizeof(tmpbuff));
			sprintf(tmpbuff, "partition error!", i);
			goto setErr_DiskPartition;
		}	
		
	}
	
	/* Delete entry */
	if (strDelSel[0]) {
		for (i = 0; i < number; i++)
		{
			total = 0;
			memset(tmpbuff, '\0', sizeof(tmpbuff));			
			memset(parted_print_info, 0x00, sizeof(parted_print_info));
			sprintf(tmpbuff, "/dev/%s", partition_info[i].name);
			pharsePartedPrint(parted_print_info, "parted", tmpbuff, "/var/tmp/boa_parted_tmp.txt", &total);
			for(j = 0; j < total; j++){
				sprintf(tmpbuff, "sd%d_select%d", i+1,j+1);
				strVal = req_get_cstream_var(wp,(tmpbuff),"");
				if(!strcmp(strVal,"ON")){										
					//umount it first
					memset(tmpbuff, '\0', sizeof(tmpbuff));
					sprintf(tmpbuff, "umount /dev/%s%s >/dev/null 2>&1", partition_info[i].name, parted_print_info[j].number);
					system(tmpbuff);
					//del
					memset(tmpbuff, '\0', sizeof(tmpbuff));
					sprintf(tmpbuff, "parted /dev/%s -s rm %s >/dev/null 2>&1", partition_info[i].name, parted_print_info[j].number);
					//printf("%s %d: del sel disk partition cmd=%s \n", __FUNCTION__, __LINE__, tmpbuff);
					system(tmpbuff);
				}
			}
			//update kernel partition table
			system("partprobe");
		}
	}

	/* Delete all entry */
	if ( strDelAll[0]) {
		for (i = 0; i < number; i++)
		{
			total = 0;
			memset(parted_print_info, 0x00, sizeof(parted_print_info));
			memset(tmpbuff, '\0', sizeof(tmpbuff));
			sprintf(tmpbuff, "/dev/%s", partition_info[i].name);
			pharsePartedPrint(parted_print_info, "parted", tmpbuff, "/var/tmp/boa_parted_tmp.txt", &total);
			for (j = total-1; j >= 0; j--)
			{
				//umount it first
				memset(tmpbuff, '\0', sizeof(tmpbuff));
				sprintf(tmpbuff, "umount /dev/%s%s >/dev/null 2>&1", partition_info[i].name, parted_print_info[j].number);
				system(tmpbuff);
				//delete
				memset(tmpbuff, '\0', sizeof(tmpbuff));
				sprintf(tmpbuff, "parted /dev/%s -s rm %s >/dev/null 2>&1", partition_info[i].name, parted_print_info[j].number);
				//printf("%s %d: del all disk partition cmd=%s \n", __FUNCTION__, __LINE__, tmpbuff);
				system(tmpbuff);
			}
			//update kernel partition table
			system("partprobe >/dev/null 2>&1");
		}

	}
	
setOk_DiskPartition:
	
	submitUrl = req_get_cstream_var(wp, "submit_url", "");
	if (submitUrl[0])
	{
		if (strAdd[0])
		{
			#if defined(CONFIG_RTL_92D_SUPPORT) || defined(CONFIG_POCKET_AP_SUPPORT)
			waittime = 100;
			#else
			waittime = 80;
			#endif
			memset(tmpbuff, '\0', sizeof(tmpbuff));
			#ifdef HOME_GATEWAY
			sprintf(tmpbuff, "%s","Partition operation successfully!<br><br>The Router is booting.<br>Do not turn off or reboot the Device during this time.<br>");
			#else
			sprintf(tmpbuff, "%s", "Partition operation successfully!<br><br>The AP is booting.<br>");
			#endif
			
			apmib_get( MIB_IP_ADDR,  (void *)lan_ip_buf);
			sprintf(lan_ip,"%s",inet_ntoa(*((struct in_addr *)lan_ip_buf)) );
			OK_MSG_FW(tmpbuff, submitUrl,waittime,lan_ip);
			REBOOT_WAIT_COMMAND(2); 	
		}
		else
		{
			send_redirect_perm(wp, submitUrl);
		}
	}
	
	//if (submitUrl[0])
		//send_redirect_perm(wp, submitUrl);
	return;
	
setErr_DiskPartition:
	ERR_MSG(tmpbuff);

}
#endif

