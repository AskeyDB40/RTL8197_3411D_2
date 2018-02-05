#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define KERNEL_3_10
#ifdef KERNEL_3_10
#include <linux/fs.h>
#endif

/* get mtdnum api function 
/* mtd_name: rootfs, linux and so on
/* num: int* pointer,store mtd num
/* return val: 0 fail 1 success */
static int rtl_get_mtdnum(char* mtd_name,int *num)
{
	FILE *file;
	char buf[128],name[64];

	int mtdnum;
	unsigned long long size;
	unsigned int erasesize;
	
	memset(buf,0,128);
	memset(name,0,64);
	
	if((file = popen("cat /proc/mtd","r")) == NULL)
		return 0;

	while(fgets(buf, 128, file) != NULL) {
		if(strstr(buf,"mtd")){
			sscanf(buf,"mtd%d: %x %x \"%[^\"]\"\n",&mtdnum,&size,&erasesize,name);
	
			if(!strcmp(name,mtd_name)){
				*num = mtdnum;
				pclose(file);
				return 1;
			}
		}
	}
	
	pclose(file);
	return 0;
}

/* get mtdblockN api function 
/* mtd_name: rootfs, linux and so on
/* buf: store mtdblockN
/* return val: 0 fail 1 success */
int rtl_name_to_mtdblock(char* mtd_name,char* buf)
{
	int mtd_num;
#ifdef CONFIG_PROC_FS	
	if(rtl_get_mtdnum(mtd_name,&mtd_num) == 0){
		return 0;
	}
#else
	// when CONFIG_PROC_FS not defined, should not happen 
#ifndef CONFIG_MTD_NAND
	if(!strcmp("boot+cfg+linux",mtd_name))
		mtd_num = 0;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 1;
	else if(!strcmp("boot+cfg+linux2",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 3;
	else
		return 0;
#else
	if(!strcmp("boot",mtd_name))
		mtd_num = 0;
	else if(!strcmp("setting",mtd_name))
		mtd_num = 1;
	else if(!strcmp("linux",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 3;
	else if(!strcmp("setting2",mtd_name))
		mtd_num = 4;
	else if(!strcmp("linux2",mtd_name))
		mtd_num = 5;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 6;
	else
		return 0;
#endif
#endif

	sprintf(buf,"/dev/mtdblock%d",mtd_num);
	return 1;
}


/* get mtdN api function 
/* mtd_name: rootfs, linux and so on
/* buf: store mtdN
/* return val: 0 fail 1 success */
int rtl_name_to_mtdchar(char* mtd_name,char* buf)
{
	int mtd_num;
#ifdef CONFIG_PROC_FS	
	if(rtl_get_mtdnum(mtd_name,&mtd_num) == 0){
		return 0;
	}
#else
	// when CONFIG_PROC_FS not defined, should not happen 
#ifndef CONFIG_MTD_NAND
	if(!strcmp("boot+cfg+linux",mtd_name))
		mtd_num = 0;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 1;
	else if(!strcmp("boot+cfg+linux2",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 3;
	else
		return 0;
#else
	if(!strcmp("boot",mtd_name))
		mtd_num = 0;
	else if(!strcmp("setting",mtd_name))
		mtd_num = 1;
	else if(!strcmp("linux",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 3;
	else if(!strcmp("setting2",mtd_name))
		mtd_num = 4;
	else if(!strcmp("linux2",mtd_name))
		mtd_num = 5;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 6;
	else
		return 0;
#endif
#endif

	sprintf(buf,"/dev/mtd%d",mtd_num);
	return 1;
}


/* error happen,sometimes check this code */
#if 0
/* mtd write api function 
/* mtd_name: rootfs, linux and so on
/* offset: offset from this partition
/* buf: read buffer
/* len: read size
/* return val: 0 fail 1 success */
int rtl_flash_read(char* mtd_name,char* buf,loff_t offset,int len)
{
	int mtd_num;
	int fh;
	int ok=1;
	char dev[32];
	memset(dev,0,32);

#ifdef CONFIG_PROC_FS	
	if(rtl_get_mtdnum(mtd_name,&mtd_num) == 0)
		return 0;
#else
	// when CONFIG_PROC_FS not defined, should not happen 
#ifndef CONFIG_MTD_NAND
	if(!strcmp("boot+cfg+linux",mtd_name))
		mtd_num = 0;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 1;
	else if(!strcmp("boot+cfg+linux2",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 3;
	else
		return 0;
#else
	if(!strcmp("boot",mtd_name))
		mtd_num = 0;
	else if(!strcmp("setting",mtd_name))
		mtd_num = 1;
	else if(!strcmp("linux",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 3;
	else if(!strcmp("setting2",mtd_name))
		mtd_num = 4;
	else if(!strcmp("linux2",mtd_name))
		mtd_num = 5;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 6;
	else
		return 0;
#endif
#endif
	snprintf(dev,32,"/dev/mtdblock%d",mtd_num);
	
#ifdef __mips__
	fh = open(dev, O_RDWR);
#endif
#ifdef __i386__
	#error "3662376723"
	fh = open(dev, O_RDONLY);
#endif
	if (fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if (read(fh, buf, len) != len)
		ok = 0;

	close(fh);
	return ok;
	
}


/* mtd write api function 
/* mtd_name: rootfs, linux and so on
/* offset: offset from this partition
/* buf: write buffer
/* len: write size
/* return val: 0 fail 1 success */
int rtl_flash_write(char* mtd_name,char* buf,loff_t offset,int len)
{
	int mtd_num;
	int fh;
	int ok=1;
	char dev[32];
	memset(dev,0,32);

#ifdef CONFIG_PROC_FS
	if(rtl_get_mtdnum(mtd_name,&mtd_num) == 0)
		return 0;
#else
	// when CONFIG_PROC_FS not defined, should not happen 
#ifndef CONFIG_MTD_NAND
	if(!strcmp("boot+cfg+linux",mtd_name))
		mtd_num = 0;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 1;
	else if(!strcmp("boot+cfg+linux2",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 3;
	else
		return 0;
#else
	if(!strcmp("boot",mtd_name))
		mtd_num = 0;
	else if(!strcmp("setting",mtd_name))
		mtd_num = 1;
	else if(!strcmp("linux",mtd_name))
		mtd_num = 2;
	else if(!strcmp("rootfs",mtd_name))
		mtd_num = 3;
	else if(!strcmp("setting2",mtd_name))
		mtd_num = 4;
	else if(!strcmp("linux2",mtd_name))
		mtd_num = 5;
	else if(!strcmp("rootfs2",mtd_name))
		mtd_num = 6;
	else
		return 0;
#endif
#endif
	snprintf(dev,32,"/dev/mtdblock%d",mtd_num);

#ifdef CONFIG_MTD_NAND
	fh = open(dev, O_RDWR | O_CREAT);	
#else
	fh = open(dev, O_RDWR);
#endif

	if ( fh == -1 )
		return 0;

	lseek(fh, offset, SEEK_SET);

	if(write(fh, buf, len) != len)
		ok = 0;

	sync();
#ifdef KERNEL_3_10
	if(ioctl(fh,BLKFLSBUF,NULL) < 0){
	    printf("flush mtd system cache error\n");
	    ok = 0;
	}
#endif

	close(fh);

	return ok;

}
#endif
