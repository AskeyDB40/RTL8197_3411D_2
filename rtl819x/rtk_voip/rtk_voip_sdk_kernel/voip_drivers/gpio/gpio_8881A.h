/*
* Copyright c                  Realtek Semiconductor Corporation, 2009
* All rights reserved.
* 
* Program : 8881A GPIO Header File
* Abstract :
* Author :
* 
*/
 
#ifndef __GPIO_8881A_
#define __GPIO_8881A_

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8881A

/******** GPIO define ********/

/* define GPIO port */
enum GPIO_PORT
{
	GPIO_PORT_A = 0,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_E,
	GPIO_PORT_F,
	GPIO_PORT_G,
	GPIO_PORT_H,
	GPIO_PORT_MAX,
	GPIO_PORT_UNDEF,
};

/* define GPIO control pin */
enum GPIO_CONTROL
{
	GPIO_CONT_GPIO = 0,
	GPIO_CONT_PERI = 0x1,
};

/* define GPIO direction */
enum GPIO_DIRECTION
{
	GPIO_DIR_IN = 0,
	GPIO_DIR_OUT =1,
};

/* define GPIO Interrupt Type */
enum GPIO_INTERRUPT_TYPE
{
	GPIO_INT_DISABLE = 0,
	GPIO_INT_FALLING_EDGE,
	GPIO_INT_RISING_EDGE,
	GPIO_INT_BOTH_EDGE,
};

/*************** Define RTL8954C Family GPIO Register Set ************************/
#define GPABCDCNR		0xB8003500

#define	GPABCDDIR		0xB8003508
#define	GPABCDDATA		0xB800350C
#define	GPABCDISR		0xB8003510
#define	GPABIMR			0xB8003514
#define	GPCDIMR			0xB8003518
#define GPEFGHCNR		0xB800351C

#define	GPEFGHDIR		0xB8003524
#define	GPEFGHDATA		0xB8003528
#define	GPEFGHISR		0xB800352C
#define	GPEFIMR			0xB8003530
#define	GPGHIMR			0xB8003534
/**************************************************************************/
/* Register access macro (REG*()).*/
#define REG32(reg) 			(*((volatile uint32 *)(reg)))
#define REG16(reg) 			(*((volatile uint16 *)(reg)))
#define REG8(reg) 			(*((volatile uint8 *)(reg)))

/*********************  Function Prototype in gpio.c  ***********************/
int32 _rtl8881A_initGpioPin(uint32 gpioId, enum GPIO_CONTROL dedicate, 
                                           enum GPIO_DIRECTION, 
                                           enum GPIO_INTERRUPT_TYPE interruptEnable );
int32 _rtl8881A_getGpioDataBit( uint32 gpioId, uint32* pData );
int32 _rtl8881A_setGpioDataBit( uint32 gpioId, uint32 data );

#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM8881A */


////// For 8881A ATA SLIC /////
#if defined(CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)

#define GPIO "ATA"
/* 8972d
#define PIN_RESET1		GPIO_ID(GPIO_PORT_G,0)	//output
//#define PIN_RESET2		GPIO_ID(GPIO_PORT_D,6)	//output

#define PIN_CS1			GPIO_ID(GPIO_PORT_B,7)	//output 
#define PIN_CS2			GPIO_ID(GPIO_PORT_C,0)	//output 

#define PIN_CLK			GPIO_ID(GPIO_PORT_C,4)	//output
#define PIN_DI			GPIO_ID(GPIO_PORT_C,5) 	//input
#define PIN_DO 			GPIO_ID(GPIO_PORT_A,1) 	//output
*/

#define PIN_RESET1		GPIO_ID(GPIO_PORT_E,6)	//output
//#define PIN_RESET2		GPIO_ID(GPIO_PORT_D,6)	//output

#define PIN_CS1			GPIO_ID(GPIO_PORT_F,1)	//output 
#define PIN_CS2			GPIO_ID(GPIO_PORT_F,6)	//output 

#define PIN_CLK			GPIO_ID(GPIO_PORT_F,2)	//output
#define PIN_DI			GPIO_ID(GPIO_PORT_E,7) 	//input
#define PIN_DO 			GPIO_ID(GPIO_PORT_F,0) 	//output
#define PIN_VOIP0_LED	GPIO_ID(GPIO_PORT_F,4)   


/* LED */ 
/* use below GPIO as SLIC CS */
//#define PIN_VOIP0_LED	GPIO_ID(GPIO_PORT_G,5)    
#define PIN_VOIP1_LED	GPIO_ID(GPIO_PORT_G,6)    

#endif //CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC


#endif /* __GPIO_8881A_ */

