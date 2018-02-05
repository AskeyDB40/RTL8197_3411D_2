/*
* Copyright c                  Realtek Semiconductor Corporation, 2006  
* All rights reserved.
* 
* Program : GPIO Header File 
* Abstract : 
* Author :                
* 
*/
#ifndef __GPIO__
#define __GPIO__

#include <linux/kconfig.h>
typedef unsigned long long	uint64;
typedef long long		int64;
typedef unsigned int	uint32;

#ifdef int32
#undef int32
#endif
typedef int			int32;

typedef unsigned short	uint16;
typedef short			int16;
typedef unsigned char	uint8;
typedef char			int8;
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SUCCESS 	0
#endif
#ifndef FAILED
#define FAILED -1
#endif

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
//#define REG32(reg) 			(*((volatile uint32 *)(reg)))
//#define REG16(reg) 			(*((volatile uint16 *)(reg)))
//#define REG8(reg) 			(*((volatile uint8 *)(reg)))

#define _GPIO_DEBUG_
#ifdef _GPIO_DEBUG_ 
#define GPIO_PRINT(level, fmt, args...) do { if (gpio_debug >= level) printk(fmt, ## args); } while (0)
#else
#define GPIO_PRINT(fmt, args...)
#endif

/*
 * Every pin of GPIO port can be mapped to a unique ID. All the access to a GPIO pin must use the ID.
 * This macro is used to map the port and pin into the ID.
 */
#define GPIO_ID(port,pin) ((uint32)port<<16|(uint32)pin)

/* This is reversed macro. */
#define GPIO_PORT(id) (id>>16)
#define GPIO_PIN(id) (id&0xffff)


/*********************  Function Prototype in gpio.c  ***********************/
int32 _rtl819x_initGpioPin(uint32 gpioId, enum GPIO_CONTROL dedicate, 
                                           enum GPIO_DIRECTION, 
                                           enum GPIO_INTERRUPT_TYPE interruptEnable );
int32 _rtl819x_getGpioDataBit( uint32 gpioId, uint32* pData );
int32 _rtl819x_setGpioDataBit( uint32 gpioId, uint32 data );


#define RTK_GPIO_INIT(pid, dedicate, dir, interrupt)  _rtl819x_initGpioPin(pid, dedicate, dir, interrupt)
#define RTK_GPIO_GET(pid, pData) _rtl819x_getGpioDataBit(pid, pData) 
#define RTK_GPIO_SET(pid, data) _rtl819x_setGpioDataBit(pid, data)
// define generic naming for kernel's driver 
#define _rtl_generic_initGpioPin(pid, dedicate, dir, interrupt)	RTK_GPIO_INIT(pid, dedicate, dir, interrupt)
#define _rtl_generic_getGpioDataBit(pid, pData)					RTK_GPIO_GET(pid, pData)
#define _rtl_generic_setGpioDataBit(pid, data)					RTK_GPIO_SET(pid, data)

extern int gpio_debug;

#endif/*__GPIO__*/
