/*
 * SHEIPA SPI controller driver
 *
 * Author: Realtek PSP Group
 *
 * Copyright 2015, Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include "bspchip.h"

#ifndef CONFIG_MTD_RTKXX_PARTS
#define ERASE_SIZE	0x1000		//tmp code,modify if flash driver modified
#ifdef CONFIG_RTL_802_1X_CLIENT_SUPPORT
#define RTL_802_1X_CLIENT_SIZE 0x10000
#else
#define RTL_802_1X_CLIENT_SIZE ERASE_SIZE
#endif

#ifdef CONFIG_RTL_WAPI_SUPPORT
#define RTL_WAPI_SIZE 0x10000
#else
#define RTL_WAPI_SIZE ERASE_SIZE
#endif

#ifdef CONFIG_APPLE_HOMEKIT_BLOCK_SUPPORT
#define RTL_HOMEKIT_SIZE 0x10000
#else
#define RTL_HOMEKIT_SIZE ERASE_SIZE
#endif

#ifndef CONFIG_RTL_FLATFS_IMAGE_OFFSET
#define CONFIG_RTL_FLATFS_IMAGE_OFFSET  0x3E0000
#endif


#ifdef CONFIG_RTL_FLASH_MAPPING_ENABLE
#if defined(CONFIG_ROOTFS_JFFS2)
static struct mtd_partition sheipa_mtd_parts[] = {
        {
                name:           "boot+cfg",
                size:           (CONFIG_RTL_LINUX_IMAGE_OFFSET-0),
                offset:         0x00000000,
        },
        {
                name:           "jffs2(linux+root fs)",                
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
                size:        (CONFIG_RTL_SPI_FLASH1_SIZE + \
                			  CONFIG_RTL_SPI_FLASH2_SIZE - \
                			  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                			  RTL_802_1X_CLIENT - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
                size:        (CONFIG_RTL_SPI_FLASH1_SIZE - \
                			  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                			  RTL_802_1X_CLIENT - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#else
                size:        (WINDOW_SIZE - CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                			  RTL_802_1X_CLIENT - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
                offset:      (CONFIG_RTL_ROOT_IMAGE_OFFSET),
        }
		,
		{
			name:	"1x",
			size:	(RTL_802_1X_CLIENT_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE + \
			CONFIG_RTL_SPI_FLASH2_SIZE - \
					 RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE - \
			RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#else
			offset:	(WINDOW_SIZE - RTL_802_1X_CLIENT_SIZE - \
			RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif		
		}
		,
		{
			name:	"wapi",
			size:	(RTL_WAPI_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE + \
					CONFIG_RTL_SPI_FLASH2_SIZE - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE ),
#else
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE  ),
#endif
#else
			offset:	(WINDOW_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE ),
#endif		
		}
		,
		{
			name:	"homekit",
			size:	(RTL_HOMEKIT_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE + \
				CONFIG_RTL_SPI_FLASH2_SIZE - \
				RTL_HOMEKIT_SIZE ),
#else
			offset:	(CONFIG_RTL_SPI_FLASH1_SIZE - \
				RTL_HOMEKIT_SIZE ),
#endif
#else
			offset:	(WINDOW_SIZE - RTL_HOMEKIT_SIZE),
#endif		
		}


};
#elif defined(CONFIG_ROOTFS_RAMFS)
static struct mtd_partition sheipa_mtd_parts[] = {
        {
                name:        "boot+cfg+linux+rootfs",
                size:        (CONFIG_RTL_FLASH_SIZE-0),
                offset:      0x00000000,
        },
};

#elif defined(CONFIG_ROOTFS_SQUASH)
#ifndef CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
static struct mtd_partition sheipa_mtd_parts[] = {
        {
                name: "boot+cfg+linux",
                size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET-0),
                offset:         0x00000000,
        },
        {
                name:           "root fs",  
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
                size:        (CONFIG_RTL_SPI_FLASH1_SIZE + \
                			  CONFIG_RTL_SPI_FLASH2_SIZE - \
                			  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                			  RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
		  		size:        (CONFIG_RTL_SPI_FLASH1_SIZE - \
		  					  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
		  					  RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#else
#if !defined(CONFIG_MTD_CHAR)
                size:        (CONFIG_RTL_FLASH_SIZE - \
                			  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                			  RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
				size:        (CONFIG_RTL_FLATFS_IMAGE_OFFSET - \
							  CONFIG_RTL_ROOT_IMAGE_OFFSET - \
							  RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif                	
#endif
                offset:         (CONFIG_RTL_ROOT_IMAGE_OFFSET),
        }
		,
		{
			name:	"1x",
			size:	(RTL_802_1X_CLIENT_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE + \
					CONFIG_RTL_SPI_FLASH2_SIZE - \
					RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE - \
			RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#else
#ifdef CONFIG_MTD_CHAR
			offset: (CONFIG_RTL_FLATFS_IMAGE_OFFSET - \
					RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_FLASH_SIZE - \
					RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#endif		
		}
		,
		{
			name:	"wapi",
			size:	(RTL_WAPI_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE + \
					CONFIG_RTL_SPI_FLASH2_SIZE - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#else
#ifdef CONFIG_MTD_CHAR
			offset: (CONFIG_RTL_FLATFS_IMAGE_OFFSET - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_FLASH_SIZE - \
					RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
#endif
#endif		
		}
		,
		{
			name:	"homekit",
			size:	(RTL_HOMEKIT_SIZE-0),
#ifdef CONFIG_RTL_TWO_SPI_FLASH_ENABLE
#ifdef CONFIG_MTD_CONCAT
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE + \
					CONFIG_RTL_SPI_FLASH2_SIZE - \
					RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_SPI_FLASH1_SIZE - \
					RTL_HOMEKIT_SIZE),
#endif
#else
#ifdef CONFIG_MTD_CHAR
			offset: (CONFIG_RTL_FLATFS_IMAGE_OFFSET - \
					RTL_HOMEKIT_SIZE),
#else
			offset: (CONFIG_RTL_FLASH_SIZE - \
					RTL_HOMEKIT_SIZE),
#endif
#endif		
		}

#if defined(CONFIG_MTD_CHAR)   
	,     
          {
                name:           "flatfs",  

                size:        (CONFIG_RTL_FLASH_SIZE-CONFIG_RTL_FLATFS_IMAGE_OFFSET),
                offset:         (CONFIG_RTL_FLATFS_IMAGE_OFFSET),
        }
#endif   

};

#else //!CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
static struct mtd_partition sheipa_mtd_parts[] = {
        {
                name: "boot+cfg+linux(bank1)",
                size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET-0),
                offset:         0x00000000,
        },
        {
                name:           "root fs(bank1)",                
                size:        	(CONFIG_RTL_FLASH_SIZE - \
                				 CONFIG_RTL_ROOT_IMAGE_OFFSET - \
                				 RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
                offset:         (CONFIG_RTL_ROOT_IMAGE_OFFSET),
        },
		{
			name:	"1x",
			size:	(RTL_802_1X_CLIENT_SIZE - 0),
			offset:	(CONFIG_RTL_FLASH_SIZE - \
					 RTL_802_1X_CLIENT_SIZE - RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
		},
		{
			name:	"wapi",
			size:	(RTL_WAPI_SIZE - 0),
			offset: (CONFIG_RTL_FLASH_SIZE - \
					 RTL_WAPI_SIZE - RTL_HOMEKIT_SIZE),
		},
		{
			name:	"homekit",
			size:	(RTL_WAPI_SIZE - 0),
			offset: (CONFIG_RTL_FLASH_SIZE - \
					 RTL_HOMEKIT_SIZE),
		},

        {
                name: "linux(bank2)",
                size:           (CONFIG_RTL_ROOT_IMAGE_OFFSET-0),
                offset:         CONFIG_RTL_FLASH_DUAL_IMAGE_OFFSET,
        },
        {
                name:           "root fs(bank2)",                
                size:        (CONFIG_RTL_FLASH_SIZE-CONFIG_RTL_ROOT_IMAGE_OFFSET),
                offset:         CONFIG_RTL_FLASH_DUAL_IMAGE_OFFSET+(CONFIG_RTL_ROOT_IMAGE_OFFSET),
        }

};
#endif //CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE

#else
#error "unknow flash filesystem type"
#endif

#else // !CONFIG_RTL_FLASH_MAPPING_ENABLE

static struct mtd_partition sheipa_mtd_parts[] = {
	[0] = {
		.name	= "boot+cfg+linux",
		.offset	= 0,
		.size	= 0x001f0000,
		.mask_flags = 0
	},
	[1] = {
		.name	= "rootfs",
		.offset	= MTDPART_OFS_APPEND,
		.size	= MTDPART_SIZ_FULL,
		.mask_flags = 0
	},
};

#endif
#endif



static struct flash_platform_data sheipa_mtd_data = {
		.name		= "m25p80",
		.type		= "m25p80",
#ifndef CONFIG_MTD_RTKXX_PARTS
		.parts		= sheipa_mtd_parts,
		.nr_parts	= ARRAY_SIZE(sheipa_mtd_parts),
#else
		.parts		= NULL,
		.nr_parts	= 0,
#endif
};

static struct spi_board_info sheipa_spi_devs[] __initdata = {
	{
		.modalias	= "m25p80",
		.max_speed_hz	= 15000000,
		.bus_num	= 0,
		.chip_select	= 0,
		.mode		= SPI_CPHA | SPI_CPOL,
		.platform_data	= &sheipa_mtd_data,
	},
};

static struct resource sheipa_spi_resources[] = {
	[0] = {
		.start	= BSP_SPIC_BASE,
		.end	= BSP_SPIC_BASE + BSP_SPIC_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= BSP_SPIC_AUTO_BASE,
		.end	= BSP_SPIC_AUTO_BASE + BSP_SPIC_AUTO_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	}
};

static struct platform_device sheipa_spi_device = {
	.name		= "spi-sheipa",
	.id		= 0,
	.resource	= sheipa_spi_resources,
	.num_resources	= ARRAY_SIZE(sheipa_spi_resources),
};

int __init plat_spi_init(void)
{
	printk("INFO: registering sheipa spi device\n");
	spi_register_board_info(sheipa_spi_devs, ARRAY_SIZE(sheipa_spi_devs));
	platform_device_register(&sheipa_spi_device);
	return 0;
}
subsys_initcall(plat_spi_init);


