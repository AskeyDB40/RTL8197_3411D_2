#ifndef WINBOND_SPI_NAND_H
#define WINBOND_SPI_NAND_H




#define Strtoul simple_strtoul

#ifdef __UBOOT__
#define CONF_SPI_NAND_UBOOT_COMMAND 1
#else
#define CONF_SPI_NAND_UBOOT_COMMAND 0
#endif

/***********************************************
  * Winbond's opcode
  ***********************************************/
#define NAND_SPI_RESET_OP         (0xFF)
#define NAND_SPI_DIE_SELECT_OP    (0xC2)
#define NAND_SPI_SET_FEATURE_OP   (0x1F)
#define NAND_SPI_BLOCK_ERASE_OP   (0xD8)
#define NAND_SPI_RDID_OP          (0x9F)
#define NAND_SPI_GET_FEATURE_OP   (0x0F)
#define NAND_SPI_NORMAL_READ      (0x03)
#define NAND_SPI_FAST_READ        (0x0B)
#define NAND_SPI_FAST_READ_X2     (0x3B)
#define NAND_SPI_FAST_READ_X4     (0x6B)
#define NAND_SPI_FAST_READ_DIO    (0xBB)
#define NAND_SPI_FAST_READ_QIO    (0xEB)
#define NAND_SPI_PROGRAM_LOAD     (0x02)
#define NAND_SPI_PROGRAM_LOAD_X4  (0x32)
#define NAND_SPI_READ_STATUS_OP	 (0x0F)
#define NAND_SPI_WRITE_OP    (nand_spi_cmd_info.w_cmd)
#define NAND_SPI_READ_OP     (nand_spi_cmd_info.r_cmd)




/***********************************************
  * Winbond's driver function 
  ***********************************************/
#define NAND_SPI_RESET_FUNC           (snffcn_pio_raw_cmd)
#define NAND_SPI_DIE_SELECT_FUNC      (snffcn_pio_raw_cmd)
#define NAND_SPI_SET_FEATURE_FUNC     (snffcn_pio_raw_cmd)  /*quad_enable(),  winbond_wait_spi_nand_ready(), winbond_wait_spi_nand_ready()*/
#define NAND_SPI_BLOCK_ERASE_FUNC     (snffcn_pio_raw_cmd) /*block_erase()*/
#define NAND_SPI_RDID_FUNC            (snffcn_pio_raw_cmd)
#define NAND_SPI_GET_FEATURE_FUNC     (snffcn_pio_raw_cmd)
#define NAND_SPI_READ_STATUS_FUNC     (snffcn_pio_raw_cmd)
#define NAND_SPI_WRITE_FUNC           (snffcn_pio_write_data)
#define NAND_SPI_READ_FUNC            (snffcn_pio_read_data)



/********************************************
  Export functions
  ********************************************/
//plr_nand_spi_info_t *probe_winbond_spi_nand_chip(void);
int nand_spi_block_erase(unsigned int blk_pge_addr);
void nand_spi_wait_spi_nand_ready(void);
void nand_spi_pio_read_data(void *ram_addr, unsigned int wr_bytes, unsigned int blk_pge_addr, unsigned int col_addr);
void nand_spi_pio_write_data(void *ram_addr, unsigned int wr_bytes, unsigned int blk_pge_addr, unsigned int col_addr);
void nand_spi_dma_read_data(void *dma_addr, unsigned int dma_len, unsigned int blk_pge_addr,unsigned int chunk_oob_size); //Less or More than 2112 Bytes
void nand_spi_dma_write_data(void *dma_addr, unsigned int dma_len, unsigned int blk_pge_addr,unsigned int chunk_oob_size); //Les  or More than 2112 Bytes
void nand_spi_chunk_read(void *dma_addr, unsigned int blk_pge_addr,unsigned int chunk_size,unsigned int oobsize); //Only 2112 Bytes
void nand_spi_chunk_write(void *dma_addr, unsigned int blk_pge_addr,unsigned int chunk_size,unsigned int oobsize); //Only 2112 Bytes
void nand_spi_chunk_write_with_ecc_encode(void * dma_addr,unsigned int blk_pge_addr, void *p_eccbuf,unsigned int chunk_size,unsigned int oob_size);
int nand_spi_chunk_read_with_ecc_decode(void * dma_addr,unsigned int blk_pge_addr, void *p_eccbuf,unsigned int chunk_size,unsigned int oob_size);
int rtkn_set_chip_param(unsigned int chipid);

//#define printf	prom_printf



#endif //#ifdef WINBOND_SPI_NAND_H

