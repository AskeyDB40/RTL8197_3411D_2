#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/init.h>
#include <linux/version.h>
#include <asm/delay.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#endif
#include "../../rtl_gpiocommon.h"

#define PIN_MUX_SEL_1		0xB8000100	
#define PIN_MUX_SEL_2		0xB8000104
#define PIN_MUX_SEL_3		0xB8000108
#define PIN_MUX_SEL_4		0xB800010C
#define PIN_MUX_SEL_5		0xB8000110

/* CONFIG MACRO */
#undef CONFIG_P0_RGMII 				//port0 RGMII
#undef CONFIG_P5_RGMII 				//port5 RGMII
#undef CONFIG_I2S					//I2S support
#define CONFIG_RTL_PCIE				//pcie wifi need 
#define CONFIG_SERIAL_RTL_UART0		//uart0 default enable
#undef CONFIG_SF_CS1
#undef CONFIG_NF_CE1

/* list macro need check */
#undef CONFIG_EXT_HW_RESET
#undef CONFIG_SWCORE_DYING_GASP

static PinMuxTbl MuxTable[] = 
{
/* #################pin mux sel 1 ########################*/
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_SATA_AHCI_PLATFORM))
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x7,{GPIO_ID('D',0),0x3},0x0},					//PIN_GPIO_0
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x38,{GPIO_ID('D',1),0x18},0x0},				//PIN_GPIO_1
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x1c0,{GPIO_ID('D',2),0xc0},0x0},				//PIN_GPIO_2
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xe00,{GPIO_ID('D',3),0x600},0x0},				//PIN_GPIO_3
#endif
/* uart1 use, default enable*/
#if !defined(CONFIG_SERIAL_RTL_UART0)
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x1000,{GPIO_ID('D',4),0x1000},0x0},			//PIN_GPIO_4
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x2000,{GPIO_ID('D',5),0x2000},0x0},			//PIN_GPIO_5
#endif
#ifdef CONFIG_MTD_NAND		//CHECK
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc000,{GPIO_ID('C',1),0x0},0x0},				//PIN_MF_CK
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x30000,{GPIO_ID('C',2),0x0},0x0},				//PIN_MF_D0
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc0000,{GPIO_ID('C',3),0x0},0x0},				//PIN_MF_D1		
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x300000,{GPIO_ID('C',4),0x0},0x0},			//PIN_MF_D2
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc00000,{GPIO_ID('C',5),0x0},0x0},			//PIN_MF_D3
#endif
#if 0 //spi flash not support
#else
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc000,{GPIO_ID('C',1),0xc000},0x0},			//PIN_MF_CK
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x30000,{GPIO_ID('C',2),0x30000},0x0},			//PIN_MF_D0
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc0000,{GPIO_ID('C',3),0xc0000},0x0},			//PIN_MF_D1
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x300000,{GPIO_ID('C',4),0x300000},0x0},		//PIN_MF_D2
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc00000,{GPIO_ID('C',5),0xc00000},0x0},		//PIN_MF_D3
#endif
#if (!defined(CONFIG_MTD_NAND) && !defined(CONFIG_RTL819X_SPI_FLASH))
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0x3000000,{GPIO_ID('C',6),0x3000000},0x0},		//PIN_MF_CS0N
#endif
#if (!defined(CONFIG_MTD_NAND) && !defined(CONFIG_RTL819X_SPI_FLASH))
{PIN_GPIO_MODE,PIN_MUX_SEL_1,0xc000000,{GPIO_ID('A',1),0xc000000},0x0},		//PIN_MF_CS1N
#endif
/* #################pin mux sel 2 ########################*/
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_SERIAL_RTL_UART1) && !defined(CONFIG_I2S))
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0xf,{GPIO_ID('A',2),0x6},0x0},					//PIN_JATG_CLK
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0xf0,{GPIO_ID('A',3),0x60},0x00},				//PIN_JATG_TRSTN 
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0xf00,{GPIO_ID('A',4),0x600},0x00},			//PIN_JATG_TMS
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0xf000,{GPIO_ID('A',5),0x6000},0x00},			//PIN_JATG_TDI 
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0xf0000,{GPIO_ID('A',6),0x60000},0x00},		//PIN_JATG_TDO 
#endif
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_RTL_PCIE))
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0x300000,{GPIO_ID('B',1),0x300000},0x00},		//PIN_RSTN			
#endif
/* use pin mux bit 22 start */
#if !defined(CONFIG_SERIAL_RTL_UART2)
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0x400000,{GPIO_ID('A',7),0x400000},0x00},		//PIN_U0_RX		
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0x400000,{GPIO_ID('B',0),0x400000},0x00},		//PIN_U0_TX	
#endif
/* use pin mux bit 22 end */
#if (!defined(CONFIG_SERIAL_RTL_UART2) && !defined(CONFIG_MTD_NAND))
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0x1800000,{GPIO_ID('D',6),0x1800000},0x00},	//PIN_U0_CTS
#endif
#if (!defined(CONFIG_SERIAL_RTL_UART2) && !defined(CONFIG_MTD_NAND))
{PIN_GPIO_MODE,PIN_MUX_SEL_2,0x6000000,{GPIO_ID('D',7),0x6000000},0x00},	//PIN_U0_RTS		
#endif
/* #################pin mux sel 3 ########################*/
#if !defined(CONFIG_USING_JTAG)
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x7,{GPIO_ID('B',2),0x3},0x0},					//PIN_LED_S0		led0 blink
#endif
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_RTL8198C_I2C) && !defined(CONFIG_I2S))
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38,{GPIO_ID('B',3),0x18},0x0},				//PIN_LED_S1		led1 blink
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x1c0,{GPIO_ID('B',4),0xc0},0x0},				//PIN_LED_S2		led2 blink
#endif
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_EXT_HW_RESET))
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0xe00,{GPIO_ID('B',5),0x600},0x0},				//PIN_LED_S3		led3 blink //check		EXTRST modeHW_RESET#
#endif
#if !defined(CONFIG_USING_JTAG)
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x7000,{GPIO_ID('B',6),0x3000},0x0},			//PIN_LED_P0		led4 blink
#endif
/* use pin mux bit 15~17 start*/
#if (!defined(CONFIG_P0_RGMII)  && !defined(CONFIG_MTD_NAND))
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('E',1),0x18000},0x0},			//PIN_P0_TXCTL		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('E',2),0x18000},0x0},			//PIN_P0_TXD3		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('E',3),0x18000},0x0},			//PIN_P0_TXD2		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('E',4),0x18000},0x0},			//PIN_P0_TXD1		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('E',5),0x18000},0x0},			//PIN_P0_TXD0		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000,{GPIO_ID('F',4),0x18000},0x0},			//PIN_P0_TXC		
/* use pin mux bit 15~17 end*/
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x1c0000,{GPIO_ID('F',2),0xc0000},0x0},		//PIN_P0_RXD0			
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0xe00000,{GPIO_ID('F',1),0x600000},0x0},		//PIN_P0_RXD1		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x7000000,{GPIO_ID('F',0),0x3000000},0x0},		//PIN_P0_RXD2		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0x38000000,{GPIO_ID('E',7),0x18000000},0x0},	//PIN_P0_RXD3		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0xc0000000,{GPIO_ID('E',6),0xc0000000},0x0},	//PIN_P0_RXCTL		
{PIN_GPIO_MODE,PIN_MUX_SEL_3,0xc0000000,{GPIO_ID('F',3),0xc0000000},0x0},	//PIN_P0_RXC		
#endif
/* #################pin mux sel 4 ########################*/
/* use pin mux bit 0~2 end*/ 
#if !defined(CONFIG_P0_RGMII)
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x7,{GPIO_ID('F',5),0x3},0x0},					//PIN_P0_MDC
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x7,{GPIO_ID('F',6),0x3},0x0},					//PIN_P0_MDIO
#endif
/* use pin mux bit 0~2 end*/
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_RTL8198C_I2C) && !defined(CONFIG_SERIAL_RTL_UART1) && !defined(CONFIG_P5_RGMII) && !defined(CONFIG_SWCORE_DYING_GASP))
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x78,{GPIO_ID('H',0),0x18},0x0},				//PIN_P5_GTXC					//check	SWCORE mode DYING_GASP
#endif
#if (!defined(CONFIG_USING_JTAG) && !defined(CONFIG_SERIAL_RTL_UART1) && !defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x380,{GPIO_ID('G',0),0x180},0x0},				//PIN_P5_TXD7
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1c00,{GPIO_ID('G',1),0xc00},0x0},			//PIN_P5_TXD6
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1e000,{GPIO_ID('G',2),0x6000},0x0},			//PIN_P5_TXD5
#endif
#if (!defined(CONFIG_SERIAL_RTL_UART0) && !defined(CONFIG_RTL8198C_I2C) && !defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1e0000,{GPIO_ID('G',3),0x60000},0x0},		//PIN_P5_TXD4
#endif
#if (!defined(CONFIG_SATA_AHCI_PLATFORM)  && !defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x600000,{GPIO_ID('F',7),0x600000},0x0},		//PIN_P5_TXC
#endif
/* use pin mux bit 23~24 start*/
#if !defined(CONFIG_P5_RGMII) 
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1800000,{GPIO_ID('H',1),0x1800000},0x0},		//PIN_P5_TXD3		
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1800000,{GPIO_ID('H',2),0x1800000},0x0},		//PIN_P5_TXD2		
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1800000,{GPIO_ID('H',3),0x1800000},0x0},		//PIN_P5_TXD1		
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1800000,{GPIO_ID('H',4),0x1800000},0x0},		//PIN_P5_TXD0		
{PIN_GPIO_MODE,PIN_MUX_SEL_4,0x1800000,{GPIO_ID('H',5),0x1800000},0x0},		//PIN_P5_TXCTL		
#endif
/* use pin mux bit 23~24 end*/

/* #################pin mux sel 5 ########################*/
#if (!defined(CONFIG_P5_RGMII) && !defined(CONFIG_SATA_AHCI_PLATFORM))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x3,{GPIO_ID('H',6),0x3},0x0},					//PIN_P5_RXCTL		
#endif
#if (!defined(CONFIG_SERIAL_RTL_UART0) && !defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x1c,{GPIO_ID('G',4),0xc},0x0},				//PIN_P5_RXD7
#endif
#if (!defined(CONFIG_I2S) && !defined(CONFIG_SWCORE_DYING_GASP) && !defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0xe0,{GPIO_ID('G',5),0x60},0x0},				//PIN_P5_RXD6	//check	SWCORE mode DYING_GASP
#endif
#if (!defined(CONFIG_P5_RGMII))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x700,{GPIO_ID('G',6),0x300},0x0},				//PIN_P5_RXD5
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x3800,{GPIO_ID('G',7),0x1800},0x0},			//PIN_P5_RXD4
#endif
#if (!defined(CONFIG_P5_RGMII) && !defined(CONFIG_SATA_AHCI_PLATFORM))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0xc000,{GPIO_ID('H',7),0xc000},0x0},			//PIN_P5_RXD3		
#endif
#if !defined(CONFIG_P5_RGMII)
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x30000,{GPIO_ID('C',0),0x30000},0x0},			//PIN_P5_RXD2
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0xc0000,{GPIO_ID('E',0),0xc0000},0x0},			//PIN_P5_RXD1
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0x300000,{GPIO_ID('B',7),0x300000},0x0},		//PIN_P5_RXD0
#endif
#if (!defined(CONFIG_P5_RGMII) && !defined(CONFIG_SATA_AHCI_PLATFORM))
{PIN_GPIO_MODE,PIN_MUX_SEL_5,0xc00000,{GPIO_ID('C',7),0xc00000},0x0},		//PIN_P5_RXC
#endif
};


/* test */
#ifdef __KERNEL__
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
static uint32 pinmuxsel1,pinmuxsel2,pinmuxsel3,pinmuxsel4,pinmuxsel5;

void getPinMuxInitalVal(void)
{
	pinmuxsel1 = REG32(PIN_MUX_SEL_1);
	pinmuxsel2 = REG32(PIN_MUX_SEL_2);
	pinmuxsel3 = REG32(PIN_MUX_SEL_3);
	pinmuxsel4 = REG32(PIN_MUX_SEL_4);
	pinmuxsel5 = REG32(PIN_MUX_SEL_5);
	
	return;
}

void setPinMuxInitalVal(void)
{
	REG32(PIN_MUX_SEL_1) = pinmuxsel1;
	REG32(PIN_MUX_SEL_2) = pinmuxsel2;
	REG32(PIN_MUX_SEL_3) = pinmuxsel3;
	REG32(PIN_MUX_SEL_4) = pinmuxsel4;
	REG32(PIN_MUX_SEL_5) = pinmuxsel5;

	return;
}
#endif

int getPinMuxVal(uint32 gpioId,uint32* pData)
{	
	int i;
	for(i = 0;i < sizeof(MuxTable)/sizeof(PinMuxTbl);i++){
		if(gpioId == MuxTable[i].gpiopin.gpionum){
			*pData = REG32(MuxTable[i].muxReg) & MuxTable[i].muxBitMask;
			break;
		}
	}
	
	if(i == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		gpioPrintf("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}
	return SUCCESS;
}


int setPinMuxVal(uint32 gpioId)
{
	int i;
	for(i = 0;i < sizeof(MuxTable)/sizeof(PinMuxTbl);i++){
		if(gpioId == MuxTable[i].gpiopin.gpionum){
			if((REG32(MuxTable[i].muxReg) & MuxTable[i].muxBitMask) == MuxTable[i].gpiopin.muxBitVal)
				return SUCCESS;
			REG32(MuxTable[i].muxReg) = REG32(MuxTable[i].muxReg) & (~(MuxTable[i].muxBitMask)) | MuxTable[i].gpiopin.muxBitVal;
			break;
		}
	}

	if(i == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		gpioPrintf("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}
}
#endif

int checkPinMuxVal(uint32 gpioId,uint32 data)
{
	int j;
	for(j = 0;j < sizeof(MuxTable)/sizeof(PinMuxTbl);j++){
		if(gpioId == MuxTable[j].gpiopin.gpionum)
			break;
	}
	if(j == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		//gpioDebug("GPIO%c%d not exist in MuxTable\n",port,pin);
		return FAILED;
	}

	if(data != (MuxTable[j].muxBitMask & MuxTable[j].gpiopin.muxBitVal)){
		return FAILED;
	}
	return SUCCESS;
}

int checkPinExist(char port,uint32 pin)
{
	int j;
	for(j = 0;j < sizeof(MuxTable)/sizeof(PinMuxTbl);j++){
		if(GPIO_ID(port,pin) == MuxTable[j].gpiopin.gpionum)
			break;
	}
	if(j == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		gpioDebug("GPIO%c%d not exist in MuxTable\n",port,pin);
		return FAILED;
	}
	return SUCCESS;
}


#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
#define RESETPIN		GPIO_ID('H',5)
#define WPSPIN			GPIO_ID('H',6)

void getRestWpsPin(uint32 *resetPin,uint32 *wpsPin)
{
	*resetPin = RESETPIN;
	*wpsPin = WPSPIN;
}
#endif


