#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
//#include <platform.h>		// GPIO_ABCD_IRQ, PABCD_ISR
#include <asm/delay.h>

#

#if defined( LINUX_VERSION_CODE ) && (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,30))
#define LINUX_KERNEL_VERSION_2_6_30		1
#endif

#ifdef LINUX_KERNEL_VERSION_2_6_30
#include "bspchip.h"		// BSP_GPIO_EFGH_IRQ
#endif


#if 0
  #define DEBUG(format, args...) printk("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
  #define DEBUG(args...)
#endif


// ------------------------------------------------------------------
// ------------------------------------------------------------------
// Basic setup information 
void set_pin_mux();

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// I2C level function 


#include "rtl819x_i2c.h"

#define I2C_GPIO_ID( port, pin )		( ( ( port - 'A' ) << 16 ) | ( pin ) )

#ifdef CONFIG_SND_RTL819XD_SOC_ALC5651
#define ALC5651_SCL 		I2C_GPIO_ID( 'B', 3 )	// SCL = B3
#define ALC5651_SDA 		I2C_GPIO_ID( 'B', 4 )	// SDA = B4
#endif
#ifdef CONFIG_SND_RTL819XD_SOC_ALC5621
#if defined(CONFIG_RTL_8197F)
#define ALC5651_SCL 		I2C_GPIO_ID( 'C', 0 )	// SCL = C0
#define ALC5651_SDA 		I2C_GPIO_ID( 'C', 1 )	// SDA = C1
#else
#define ALC5651_SCL 		I2C_GPIO_ID( 'F', 2 )	// SCL = F2
#define ALC5651_SDA 		I2C_GPIO_ID( 'F', 1 )	// SDA = F1
#endif
#endif


#define ALC5651_I2C_ADDR	0x34
#define ALC5651_I2C_WRITE	0x00
#define ALC5651_I2C_READ	0x01

static i2c_dev_t alc5651_i2c_dev = {
	.sclk	= ALC5651_SCL,
	.sdio	= ALC5651_SDA,
};

// Register address byte
//  7  : not used
//  6~3: A[3:0] UART's internal register select
//  2~1: channel select: CH1 = 0?!, CH0 = 0
//  0  : not used 
#define MK_SC16IS7X0_REG_ADDR( uart_reg )	( ( uart_reg & 0x0F ) << 3 )

unsigned int serial_in_i2c(unsigned int addr, int offset)
{
	unsigned short int data_hibyte=0;
	unsigned short int data_lowbyte=0;
	unsigned short int data;
	
	set_pin_mux();

	//printk( "serial_in_i2c(%X):%X\n", addr, offset );
	
	if( addr != ALC5651_I2C_ADDR )
		return 0;
	
	// start 
	i2c_start_condition( &alc5651_i2c_dev );
	
	// addr + write
	i2c_serial_write_byte( &alc5651_i2c_dev, ALC5651_I2C_ADDR | 
											   ALC5651_I2C_WRITE );
	
	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	
	// write register address 
	i2c_serial_write_byte( &alc5651_i2c_dev, offset );
	
	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	
	// start 
	i2c_start_condition( &alc5651_i2c_dev );

	// addr + read
	i2c_serial_write_byte( &alc5651_i2c_dev, ALC5651_I2C_ADDR | 
											   ALC5651_I2C_READ );
	
	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
		
	// read data_hibyte
	i2c_serial_read( &alc5651_i2c_dev, &data_hibyte );

	//write ACK
	i2c_ACK_w(&alc5651_i2c_dev, 0);

	// read data_lowbyte
	i2c_serial_read( &alc5651_i2c_dev, &data_lowbyte );

	data = (data_hibyte<<8) | data_lowbyte;

	// write negative-ACK
	i2c_ACK_w( &alc5651_i2c_dev, 1 );
	
	// stop
	i2c_stop_condition( &alc5651_i2c_dev );
	
	//printk( "in[%X]\n", data );
	
	return data;
}
EXPORT_SYMBOL_GPL(serial_in_i2c);

unsigned int serial_out_i2c(unsigned int addr, int offset, int value)
{
	//printk( "serial_out_i2c(%X):%X,%X\n", addr, offset, value );
	unsigned short int data_hibyte;
	unsigned short int data_lowbyte;

	set_pin_mux();

	data_hibyte =(unsigned char) (value>>8);
	data_lowbyte =(unsigned char) (value & 0xff);

	if( addr != ALC5651_I2C_ADDR )
		return 0;
	//printk("(%d)", __LINE__);
	// start 
	i2c_start_condition( &alc5651_i2c_dev );
	
	// addr + write
	i2c_serial_write_byte( &alc5651_i2c_dev, ALC5651_I2C_ADDR | ALC5651_I2C_WRITE );
	
	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write register address 
	i2c_serial_write_byte( &alc5651_i2c_dev, offset );
	
	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write data hibyte
	i2c_serial_write_byte( &alc5651_i2c_dev, data_hibyte );

	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// write data lowbyte
	i2c_serial_write_byte( &alc5651_i2c_dev, data_lowbyte );

	// read ACK 
	if( i2c_ACK( &alc5651_i2c_dev ) != 0 )
		return 0;
	//printk("(%d)", __LINE__);
	// stop
	i2c_stop_condition( &alc5651_i2c_dev );
	
	return 0;
}
EXPORT_SYMBOL_GPL(serial_out_i2c);

static void __init alc5651_init_i2c( void )
{
	int temp;
	// init SCL / SDA 
	i2c_init_SCL_SDA( &alc5651_i2c_dev );
	
	serial_in_i2c( ALC5651_I2C_ADDR, 0 );		// avoid NO ACK at first time access

	serial_out_i2c( ALC5651_I2C_ADDR, 0x0, 0x707 );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x0, 0x707 );
	
	serial_in_i2c( ALC5651_I2C_ADDR, 0 );
	serial_in_i2c( ALC5651_I2C_ADDR, 0 );


// enable playback
	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3E );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x3E, (temp) | 0x8000 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3C );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x3C, (temp) | 0x2000 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3C );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x3C, (temp) | 0x7F0 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3A );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x3A, (temp) | 0x8030 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3E );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x3E, (temp) | 0x600 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x0C );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x0C, (temp&(~0xBFBF)) | 0x1010 );


	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x1C );
	serial_out_i2c( ALC5651_I2C_ADDR, 0x1C, (temp) | 0x300 );

//	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x0C );
//	serial_out_i2c( ALC5651_I2C_ADDR, 0x0C, (temp&(~0xBFBF)) | 0x1010 );


	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0 );
	//printk("reg0=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x2 );
	//printk("reg2=%x\n", temp);

	serial_out_i2c( ALC5651_I2C_ADDR, 0x2, 0x0 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x2 );
	printk("reg2=%x\n", temp);

	serial_out_i2c( ALC5651_I2C_ADDR, 0x2, 0xFFFF );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x2 );
	printk("reg2=%x\n", temp);



	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x4 );
	//printk("reg4=%x\n", temp);

	serial_out_i2c( ALC5651_I2C_ADDR, 0x4, 0x0 );

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x4 );
	//printk("reg4=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0xA );
	//printk("regA=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0xC );
	//printk("regC=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x16 );
	//printk("reg16=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x1C );
	//printk("reg1C=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x34 );
	//printk("reg34=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x38 );
	//printk("reg38=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3A );
	//printk("reg3A=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3C );
	//printk("reg3C=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x3E );
	//printk("reg3E=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x40 );
	//printk("reg40=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x42 );
	//printk("reg42=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x44 );
	//printk("reg44=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x5A );
	//printk("reg5A=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x5C );
	//printk("reg5C=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x5E );
	//printk("reg5E=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x68 );
	//printk("reg68=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x6A );
	//printk("reg6A=%x\n", temp);

	temp = serial_in_i2c( ALC5651_I2C_ADDR, 0x6C );
	//printk("reg6C=%x\n", temp);
	printk("%s Done\n",__FUNCTION__);
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------


#define rtlRegRead(addr)        \
        (*(volatile u32 *)addr)

#define rtlRegWrite(addr, val)  \
        ((*(volatile u32 *)addr) = (val))

static inline u32 rtlRegMask(u32 addr, u32 mask, u32 value)
{
	u32 reg;

	reg = rtlRegRead(addr);
	reg &= ~mask;
	reg |= value & mask;
	rtlRegWrite(addr, reg);
	reg = rtlRegRead(addr); /* flush write to the hardware */

	return reg;
}


void set_pin_mux()
{
#if defined(CONFIG_RTL_8197F)
	/* enable hw */
	rtlRegMask(0xB8000010, 1 << 22, 1 << 22); //active I2S

	/* init pin mux */
	//set GPIO C[0], P0_TXCTL to I2C_SCL   //0x808h, [23:20] 0110
	//set GPIO C[1], P0_RXCTL to I2C_SDA   //0x808h, [15:12] 0111
	//set GPIO B[1], P0_RXC   to I2S_SD1_I //0x808h, [19:16] 0110 (IISV) //0110:IISV, 0111:IIS, 1000:IISA
	rtlRegMask(0xB8000808, 7<<20 | 7<<12 | 7<<16, 6<<20 | 7<<12 | 6<<16); 

	//set GPIO B[2], P0_RXD3  to I2S_MCLK  //0x804h, [19:16] 0101
	//set GPIO B[3], P0_RXD2  to I2S_SCLK  //0x804h, [23:20] 0101
	//set GPIO B[4], P0_RXD1  to I2S_WS    //0x804h, [27:24] 0101
	//set GPIO B[5], P0_RXD0  to I2S_SD1_O //0x804h, [31:28] 0100
	rtlRegMask(0xB8000804, 7<<16 | 7<<20 | 7<<24 | 7<<28, 5<<16 | 5<<20 | 5<<24 | 4<<28); 

#elif defined(CONFIG_RTL_8198C)
	rtlRegMask(0xb8000010, 0x400000, 0x400000);//enable iis controller clock
    // rtlRegMask(0xb8000010, 0x2000000, 0x2000000);//enable 24p576mHz clock
	
    // GPIO_F2(18,GM0_RXD0) & GPIO_F1(21,GM0_RXD1) for audio driver 
    rtlRegMask(0xB8000108, (7<<18)|(7<<21), (3<<18)|(3<<21)); 
    
    // LED_S1,LED_S2,LED_S3, for i2s
    rtlRegMask(0xB8000108, (7<<3)|(7<<6)|(7<<9), (4<<3)|(4<<6)|(4<<9)); 
    
    // LED_S0 for i2s
    rtlRegMask(0xB8000108, (7<<0), (4<<0));     
    
    // GM5_GTXC for IIS
    rtlRegMask(0xB800010C, (0xF<<3), (5<<3));   
	
	rtlRegMask(0xb800002c, (1<<0|1<<11),  (1<<0|1<<11));
#else

	rtlRegMask(0xb8000010, 0x03DCB000, 0x01DCB000);//enable iis controller clock
	rtlRegMask(0xb8000058, 0x00000001, 0x00000001);//enable 24p576mHz clock

	/* Configure the I2S pins in correct mode */
#if 1 // set the jtag as iis-audio
	rtlRegMask(0xb8000040, 0x00000007, 0x00000003);//change pin mux to iis-voice pin
#else // set the led-phase or lec-sig as iis-audio
	rtlRegMask(0xb8000044, 0x001F80DB, 0x00000049);//change pin mux to iis-voice pin
#endif
#endif


}

static int __init alc5651_init(void)
{
	int ret;
    DEBUG ("alc5651_init.\n");
	set_pin_mux();
	alc5651_init_i2c();

    DEBUG ("alc5651_init done.\n");
	return 0;
}

module_init(alc5651_init);

MODULE_AUTHOR("Realtek");
MODULE_DESCRIPTION("i2c driver for ALC5651");
MODULE_LICENSE("GPL");
