#ifndef __RTKN_SPI_NAND_CHIP_PARAM_H__
#define __RTKN_SPI_NAND_CHIP_PARAM_H__

#include "rtknflash.h"

#define FALSE	0
#define	TRUE	1

/* spi nand chip support */
/* Winbond W25N01GV  */
#define WINBOND_RDID_W25N01GV           (0xEFAA21)
#define WINBOND_MANUFACTURER_ID         (0xEF)
#define WINBOND_DEVICE_ID_W25N01GV      (0xAA21)

/* Winbond W25N02GV */
#define WINBOND_RDID_W25N02GV           (0xEFAB21)
#define WINBOND_MANUFACTURER_ID         (0xEF)
#define WINBOND_DEVICE_ID_W25N02GV      (0xAB21)

/* ESMT F50L1G41A */
#define ESMT_RDID_F50L1G41A				(0xC8217F)
#define ESMT_MANUFACTURER_ID			(0xC8)
#define ESMT_DEVICE_ID_F50L1G41A      	(0x217F)

/* MXIC MX35LF1GE4AB */
#define MXIC_RDID_MX35LF1GE4AB			(0xC212)
#define MXIC_MANUFACTURER_ID			(0xC2)
#define MXIC_DEVICE_ID_MX35LF1GE4AB     (0x12)

/* GD GD5F1GQ4U */
#define GD_RDID_GD5F1GQ4U				(0xC8D1)
#define GD_MANUFACTURER_ID				(0xC8)
#define GD_DEVICE_ID_GD5F1GQ4U     		(0xD1)

#if 0
/*Etron */
#define ETRON_RDID_EM73C044SNB			(0xD511)
#define ETRON_MANUFACTURER_ID			(0xD5)
#define ETRON_DEVICE_ID_EM73C044SNB     (0x11)
#endif

/* Toshiba */
#define TOSHIBA_RDID_TC58CVG0S3HxAIx		(0x98C2)
#define TOSHIBA_MANUFACTURER_ID				(0x98)
#define TOSHIBA_DEVICE_ID_TC58CVG0S3HxAIx	(0xC2)

#if 1
/* Micron */
#define MICRON_RDID_MT29F1G01ABAFD			(0x2C14)
#define MICRON_MANUFACTURER_ID				(0x2C)
#define MICRON_DEVICE_ID_MT29F1G01ABAFD		(0x14)
#endif

int rtkn_default_get_dieon_ecc_status(void);
void rtkn_toshiba8_get_dieon_ecc_info(void);
/*
 id: spi nand flash id 
 isLastPage: 0 
 id_len: spi nand flash id len 
 pagesize: 
 oobsize:
 num_chunk_per_block:
 num_block:
 maxclk
pipe_lat
support_dio
support_qio
enable_dieon_ecc
get_dieon_ecc_status
get_dieon_ecc_info
*/

static nand_chip_param_T nand_chip_id[] = 
{
	/* mxic */
	{
		.id = MXIC_RDID_MX35LF1GE4AB,
		.isLastPage = 0,
		.id_len = 2,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 104000000,
		.pipe_lat = 0,
		.support_dio = FALSE,
		.support_qio = FALSE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = FALSE,
		.romcode_write_dieon_ecc = TRUE,	
		.romcode_read_dieon_ecc = TRUE,
		.get_dieon_ecc_status = NULL,
		.get_dieon_ecc_info = NULL,
	},
	/* ESMT */
	{
		.id = ESMT_RDID_F50L1G41A,
		.isLastPage = 0,
		.id_len = 3,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 104000000,
		.pipe_lat = 0,
		.support_dio = FALSE,
		.support_qio = FALSE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = FALSE,
		.romcode_write_dieon_ecc = TRUE,	
		.romcode_read_dieon_ecc = TRUE,		//CHECK
		.get_dieon_ecc_status = NULL,
		.get_dieon_ecc_info = NULL,
	},	
	/* TOSHIBA */
	{
		.id = TOSHIBA_RDID_TC58CVG0S3HxAIx,
		.isLastPage = 0,
		.id_len = 2,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 104000000,
		.pipe_lat = 0,
		.support_dio = FALSE,
		.support_qio = FALSE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = FALSE,
		.enable_dieon_ecc = TRUE,
		.romcode_write_dieon_ecc = TRUE,	
		.romcode_read_dieon_ecc = TRUE,		//only for c-cut
		.get_dieon_ecc_status = rtkn_default_get_dieon_ecc_status,
		.get_dieon_ecc_info = rtkn_toshiba8_get_dieon_ecc_info,
	},	
	/* GIGADEVICE */	
	{
		.id = GD_RDID_GD5F1GQ4U,
		.isLastPage = 0,
		.id_len = 2,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 120000000,
		.pipe_lat = 0,
		.support_dio = TRUE,
		.support_qio = TRUE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = FALSE,
		.romcode_write_dieon_ecc = TRUE,	
		.romcode_read_dieon_ecc = TRUE,		//only for c-cut
		.get_dieon_ecc_status = rtkn_default_get_dieon_ecc_status,
		.get_dieon_ecc_info = NULL,
	},	
	/* winbond W25N01GV*/
	{
		.id = WINBOND_RDID_W25N01GV,
		.isLastPage = 0,
		.id_len = 3,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 104000000,
		.pipe_lat = 1,
		.support_dio = TRUE,
		.support_qio = TRUE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = FALSE,
		.romcode_write_dieon_ecc = FALSE,	
		.romcode_read_dieon_ecc = FALSE,		//only for c-cut
		.get_dieon_ecc_status = NULL,
		.get_dieon_ecc_info = NULL,
	},
	/* winbond W25N02GV*/
	{
		.id = WINBOND_RDID_W25N02GV,
		.isLastPage = 0,
		.id_len = 3,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 2,
		.maxclk = 104000000,
		.pipe_lat = 1,
		.support_dio = TRUE,
		.support_qio = TRUE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = FALSE,
		.romcode_write_dieon_ecc = FALSE,	
		.romcode_read_dieon_ecc = FALSE,		//only for c-cut
		.get_dieon_ecc_status = NULL,
		.get_dieon_ecc_info = NULL,
	},
	/* micron */
	{
		.id = MICRON_RDID_MT29F1G01ABAFD,
		.isLastPage = 0,
		.id_len = 2,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.dienum = 1,
		.maxclk = 133000000,
		.pipe_lat = 0,
		.support_dio = TRUE,
		.support_qio = TRUE,
		.support_read_cache_2 = TRUE,
		.support_read_cache_4 = TRUE,
		.support_programme_cache_4 = TRUE,
		.enable_dieon_ecc = TRUE,
		.romcode_write_dieon_ecc = TRUE,	
		.romcode_read_dieon_ecc = TRUE,		//only for c-cut
		.get_dieon_ecc_status = rtkn_default_get_dieon_ecc_status,
		.get_dieon_ecc_info = NULL,
	},	
#if 0
	/* etron */
	{
		.id = ETRON_RDID_EM73C044SNB,
		.isLastPage = 0,
		.id_len = 2,
		.pagesize = 0x800,
		.oobsize = 64,
		.num_chunk_per_block = 64,
		.num_block = 1024,
		.maxclk = 80000000,
		.pipe_lat = 0,
		.support_dio = TRUE,
		.support_qio = TRUE,
		.enable_dieon_ecc = FALSE,
		.get_dieon_ecc_status = NULL,
		.get_dieon_ecc_info = NULL,
	},
#endif
};


enum RTKN_IO_READ
{
	RTKN_SIO = 0,
	RTKN_DIO,
	RTKN_CACHE_2,
	RTKN_QIO,
	RTKN_CACHE_4
};

enum RTKN_IO_WRITE
{
	RTKN_WRITE_SIO = 0,
	RTKN_PROGRAMME_4
};	

#endif