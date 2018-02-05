#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

/* config */
#if 0
#define RTK_WAPI_SUPPORT	
#define RTK_1X_SUPPORT
#define RTK_HOMEKIT_SUPPORT	
#define RTK_FLATFS_SUPPORT				

#define CONFIG_RTL_WAPI_SIZE			0x20000
#define CONFIG_RTL_1X_SIZE				0x20000
#define CONFIG_RTL_HOMEKIT_SIZE			0x20000
#define CONFIG_RTL_FLATFS_SIZE			0x20000
#endif

//#define MTD_PARTITION_TEST
#if defined(MTD_PARTITION_TEST)
#define MTD_PARTITION_TEST_SIZE	0x100000
#endif

#if !(defined(CONFIG_MTD_M25P80) || defined(CONFIG_RTL819X_SPI_FLASH) || defined(CONFIG_MTD_NAND))
#error "nor and nand flash not support"
#endif

/*  RTK_FLASH_SIZE  */
#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_RTL819X_SPI_FLASH)
#if defined(CONFIG_RTL_TWO_SPI_FLASH_ENABLE)
	#if defined(CONFIG_MTD_CONCAT)
		#define RTK_FLASH_SIZE	(CONFIG_RTL_SPI_FLASH1_SIZE+CONFIG_RTL_SPI_FLASH2_SIZE)
	#else
		#define RTK_FLASH_SIZE	(CONFIG_RTL_SPI_FLASH1_SIZE)
	#endif
#else
	#define RTK_FLASH_SIZE	CONFIG_RTL_FLASH_SIZE
#endif
#endif
#if defined(CONFIG_MTD_NAND)
#define RTK_FLASH_SIZE	CONFIG_RTL_FLASH_SIZE
#endif

#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
#define RTK_FLASH_SIZE	CONFIG_RTL_FLASH_DUAL_IMAGE_OFFSET
#endif


#define RTK_LAST_PART_NAME	"rootfs"

/************** MTD PARTITION *****************************/
#if defined(CONFIG_RTL_FLASH_MAPPING_ENABLE)
/**********************SPI NOR FLASH **********************/
#if defined(CONFIG_ROOTFS_JFFS2) || defined(CONFIG_ROOTFS_SQUASH)
#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_RTL819X_SPI_FLASH)
static struct mtd_partition rtl819x_parts[] = {
	{
	    name:           "boot+cfg+linux",
	    size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET-0),
	    offset:         0x00000000,
	},
	{
	    name:           "rootfs",
	    size:           (RTK_FLASH_SIZE-CONFIG_RTL_ROOT_IMAGE_OFFSET),
	    offset:         CONFIG_RTL_ROOT_IMAGE_OFFSET,
	}
#if defined(CONFIG_BT_REPEATER_CONFIG)
#if CONFIG_RTL_BT_PARTITION_SIZE	!= 0x0
		,
		{
			name:			"bluetooth",
			size:			(CONFIG_RTL_BT_PARTITION_SIZE),
			offset: 		RTK_FLASH_SIZE,
		}
#endif
#endif
#if CONFIG_RTL_WAPI_PARTITION_SIZE != 0x0
	,
	{
	    name:           "wapi",
	    size:           (CONFIG_RTL_WAPI_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_1X_PARTITION_SIZE != 0x0
	,
	{
	    name:           "1x",
	    size:           (CONFIG_RTL_1X_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_HOMEKIT_PARTITION_SIZE != 0x0
	,
	{
	    name:           "homekit",
	    size:           (CONFIG_RTL_HOMEKIT_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE != 0x0
	,
	{
	    name:           "cwmp transfer",
	    size:           (CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE != 0x0
	,
	{
	    name:           "cwmp notification",
	    size:           (CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_CACERT_PARTITION_SIZE != 0x0
     ,
     {
         name:           "cwmp cacert",
         size:           (CONFIG_RTL_CWMP_CACERT_PARTITION_SIZE),
         offset:         RTK_FLASH_SIZE,
     }
#endif
#if CONFIG_RTL_JFFS2_FILE_PARTITION_SIZE != 0x0
     ,   
     {   
         name:           "jffs2 file",
         size:           (CONFIG_RTL_JFFS2_FILE_PARTITION_SIZE),
         offset:         RTK_FLASH_SIZE,
     }   
#endif
#if 0//defined(MTD_PARTITION_TEST)
	,
	{
	    name:           "mtd_test",
	    size:           (MTD_PARTITION_TEST_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}

#endif
		// dual image support
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
	,
	{
	    name:           "boot+cfg+linux2",
	    size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET-0),
	    offset:         0x00000000+RTK_FLASH_SIZE,
	}
	,
	{
	    name:           "rootfs2",
	    size:           (RTK_FLASH_SIZE-CONFIG_RTL_ROOT_IMAGE_OFFSET),
	    offset:         CONFIG_RTL_ROOT_IMAGE_OFFSET+RTK_FLASH_SIZE,
	}
#if CONFIG_RTL_WAPI_PARTITION_SIZE != 0x0
	,
	{
	    name:           "wapi2",
	    size:           (CONFIG_RTL_WAPI_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_1X_PARTITION_SIZE != 0x0
	,
	{
	    name:           "1x2",
	    size:           (CONFIG_RTL_1X_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_HOMEKIT_PARTITION_SIZE != 0x0
	,
	{
	    name:           "homekit2",
	    size:           (CONFIG_RTL_HOMEKIT_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE != 0x0
	,
	{
	    name:           "cwmp transfer2",
	    size:           (CONFIG_RTL_CWMP_TRANSFER_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE != 0x0
	,
	{
	    name:           "cwmp notification2",
	    size:           (CONFIG_RTL_CWMP_NOTIFICATION_PARTITION_SIZE),
	    offset:         RTK_FLASH_SIZE,
	}
#endif
#if CONFIG_RTL_CWMP_CACERT_PARTITION_SIZE != 0x0
     ,   
     {   
         name:           "cwmp cacert2",
         size:           (CONFIG_RTL_CWMP_CACERT_PARTITION_SIZE),
         offset:         RTK_FLASH_SIZE,
     }
#endif
#if CONFIG_RTL_JFFS2_FILE_PARTITION_SIZE != 0x0
     ,
     {
         name:           "jffs2 file2",
         size:           (CONFIG_RTL_JFFS2_FILE_PARTITION_SIZE),
         offset:         RTK_FLASH_SIZE,
     }
#endif
#endif
};
#endif
/*********************************************************/
/******************* NAND FLASH ***************************/
#if !defined(CONFIG_RTK_NAND_FLASH_STORAGE)
#if defined(CONFIG_MTD_NAND)
static struct mtd_partition rtl819x_parts[] = {
	{
	    name:           "boot",
	    size:           0x500000,
	    offset:         0x00000000,
	},
	{
	    name:           "setting",
	    size:            0x300000,
	    offset:          0x500000,
	},
	{
	    name:           "linux",
	    size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET - 0x800000),
	    offset:         0x800000,
	},
	{
	    name:           "rootfs",
	    size:           (RTK_FLASH_SIZE - CONFIG_RTL_ROOT_IMAGE_OFFSET),
	    offset:         CONFIG_RTL_ROOT_IMAGE_OFFSET,
	}
#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
	,
	{
	    name:           "boot2",
	    size:           0x500000,
	    offset:         0x00000000+RTK_FLASH_SIZE,
	}
	,
	{
	    name:           "setting2",
	    size:            0x300000,
	    offset:          0x500000+RTK_FLASH_SIZE,
	},
	{
	    name:           "linux2",
	    size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET - 0x800000),
	    offset:         0x800000+RTK_FLASH_SIZE,
	},
	{
	    name:           "rootfs2",
	    size:           (RTK_FLASH_SIZE - CONFIG_RTL_ROOT_IMAGE_OFFSET),
	    offset:         CONFIG_RTL_ROOT_IMAGE_OFFSET+RTK_FLASH_SIZE,
	}
#endif
};
#endif
#endif
	/***************RAMFS as rootfs *****************/
#elif defined(CONFIG_ROOTFS_RAMFS)
#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_RTL819X_SPI_FLASH)
static struct mtd_partition rtl819x_parts[] = {
    {
            name:        "boot+cfg+linux+rootfs",
            size:        (RTK_FLASH_SIZE-0),
            offset:      0x00000000,
    },
};
#endif

#if defined(CONFIG_MTD_NAND)
static struct mtd_partition rtl819x_parts[] = {
	{
	    name:           "boot",
	    size:           0x500000,
	    offset:         0x00000000,
	},
	{
	    name:           "setting",
	    size:            0x300000,
	    offset:          0x500000,
	}
};
#endif
#endif
/*********************************************************/
#else
	/*****************not support CONFIG_RTL_FLASH_MAPPING_ENABLE ***************/ 
static struct mtd_partition rtl819x_parts[] = {
    {
            name: 		"boot+cfg+linux",
            size:  		0x00130000,
            offset:		0x00000000,
    },
    {
            name:           "rootfs",                
	   		size:        	0x002D0000,
            offset:         0x00130000,
    }
}
#endif

#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifndef CONFIG_MTD_CONCAT
static struct mtd_partition rtl819x_parts2[] = {
    {
            name: 		"data",
            size:  		CONFIG_RTL_SPI_FLASH2_SIZE,
            offset:		0x00000000,
    }
};
#endif
#endif

/*********************************************************/
static  int rtkxxpart_check_parttion_erasesize_aligned(struct mtd_info* master)
{
	int i=0;
	uint64_t offset=0,size=0;
	/* check if mtd partition offset && size erasesize aligned */
	for(i = 0;i < ARRAY_SIZE(rtl819x_parts);i++){
		offset=rtl819x_parts[i].offset;
		size=rtl819x_parts[i].size;
		//printk("%s offset=0x%llx size=0x%llx erasesize=0x%x\n",
		//	rtl819x_parts[i].name,rtl819x_parts[i].offset,
		//		rtl819x_parts[i].size,master->erasesize);
		if(do_div(offset,master->erasesize)!=0
			|| do_div(size, master->erasesize) != 0){
			printk("%s offset=0x%llx size=0x%llx erasesize=0x%x!!!! not aligned!!!\n",
				rtl819x_parts[i].name,rtl819x_parts[i].offset,
				rtl819x_parts[i].size,master->erasesize);
			return -1;
		}
	}

	return 0;
}

static int rtkxxpart_detect_flash_map(struct mtd_info* master)
{
	int i;
	unsigned int size = 0;
	int dualpartition  = 0;	
	unsigned char lastpartion2[16];

#if defined(CONFIG_ROOTFS_RAMFS)
	return rtkxxpart_check_parttion_erasesize_aligned(master);
#endif

#if defined(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE)
	strcpy(lastpartion2,RTK_LAST_PART_NAME);
	strcat(lastpartion2,"2");
#endif
	
#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
	for(i = 0; i < ARRAY_SIZE(rtl819x_parts);i++){
	#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_RTL819X_SPI_FLASH)
        if(!strcmp(rtl819x_parts[i].name,"boot+cfg+linux2"))
    #elif defined(CONFIG_MTD_NAND)
		if(!strcmp(rtl819x_parts[i].name,"boot2"))
    #endif
			dualpartition = i;
	}
#else
	dualpartition = ARRAY_SIZE(rtl819x_parts);
#endif

	for(i = dualpartition - 1; i >= 0;i--){
        if(strcmp(rtl819x_parts[i].name,RTK_LAST_PART_NAME)){
            size += rtl819x_parts[i].size;
            rtl819x_parts[i].offset = RTK_FLASH_SIZE - size;
        }else{
            if(rtl819x_parts[i].size < size){
                printk("donnot have space for rootfs partition\n");
                return -1;
            }
            rtl819x_parts[i].size = rtl819x_parts[i].size - size;
            break;
        }
	}

#ifdef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
	size = 0;
	for(i = ARRAY_SIZE(rtl819x_parts) - 1; i >= dualpartition;i--){
        if(strcmp(rtl819x_parts[i].name,lastpartion2)){
            size += rtl819x_parts[i].size;
            rtl819x_parts[i].offset = (RTK_FLASH_SIZE*2) - size;
        }else{
            if(rtl819x_parts[i].size < size){
                printk("donnot have space for rootfs2 partition\n");
                return -1;
            }
            rtl819x_parts[i].size = rtl819x_parts[i].size - size;
            break;
        }
	}
#endif

	if(rtkxxpart_check_parttion_erasesize_aligned(master) != 0)
		return -1;


#ifdef CONFIG_MTD_NAND
	/* check if nand reserve %10 space for skip/remap bbt */
	int rtkn_check_nand_partition(struct mtd_partition *parts,int partnum);
	if(rtkn_check_nand_partition(rtl819x_parts,ARRAY_SIZE(rtl819x_parts)) < 0){
		return -1;
	}
#endif

	return 0;
}


/* may not need rtl819x_parts */
static int rtkxxpart_parse(struct mtd_info *master,
			     struct mtd_partition **pparts,
			     struct mtd_part_parser_data *data)
{
	int nrparts = 0;
	struct mtd_partition *parts=NULL;

	/* for rtkxxpart, if enabled CONFIG_RTL_TWO_SPI_FLASH_ENABLE but not enabled CONFIG_MTD_CONCAT, origin defined the mtddevicenum */
	if( data == NULL || data->origin == 0){
		nrparts = sizeof(rtl819x_parts)/sizeof(struct mtd_partition);
	}
#if defined(CONFIG_RTL_TWO_SPI_FLASH_ENABLE)  && !defined(CONFIG_MTD_CONCAT)
	else{
		nrparts = sizeof(rtl819x_parts2)/sizeof(struct mtd_partition);
	}
#endif
	parts = kzalloc(sizeof(*parts) * nrparts + 10 * nrparts, GFP_KERNEL);
	if (!parts) {
		return -ENOMEM;
	}	

	if(rtkxxpart_detect_flash_map(master) < 0){
		printk("dynamic alloc partition fail\n");
		master->size = 0;
		/* COVERITY: RESOURCE_LEAK */
		if(parts)
			kfree(parts);
		return -1;
	}

	if(data == NULL || data->origin == 0){
		memcpy(parts,rtl819x_parts,sizeof(rtl819x_parts));
	}
#if defined(CONFIG_RTL_TWO_SPI_FLASH_ENABLE) && !defined(CONFIG_MTD_CONCAT)
	else
		memcpy(parts,rtl819x_parts2,sizeof(rtl819x_parts2));
#endif
	*pparts = parts;
	
	return nrparts;
};

static struct mtd_part_parser rtkxxpart_mtd_parser = {
	.owner = THIS_MODULE,
	.parse_fn = rtkxxpart_parse,
	.name = "rtkxxpart",
};

static int __init rtkxxpart_init(void)
{
	return register_mtd_parser(&rtkxxpart_mtd_parser);
}

static void __exit rtkxxpart_exit(void)
{
	deregister_mtd_parser(&rtkxxpart_mtd_parser);
}

module_init(rtkxxpart_init);
module_exit(rtkxxpart_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MTD partitioning for realtek flash memories");
