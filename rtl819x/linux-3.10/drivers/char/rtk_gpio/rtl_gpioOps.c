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
#include "rtl_gpiocommon.h"
#include "rtl_gpioOps.h"

static int DebugEnable = 0;

static uint32 regGpioControlRead[] =
{
	GPIO_PABCD_CNR, 	/* Port A */
	GPIO_PABCD_CNR, 	/* Port B */
	GPIO_PABCD_CNR, 	/* Port C */
	GPIO_PABCD_CNR, 	/* Port D */
	GPIO_PEFGH_CNR,		/* Port E */
	GPIO_PEFGH_CNR,		/* Port F */
	GPIO_PEFGH_CNR,		/* Port G */
	GPIO_PEFGH_CNR		/* Port H */
};

static uint32 regGpioControlWrite[] =
{
	GPIO_PABCD_CNR, 	/* Port A */
	GPIO_PABCD_CNR, 	/* Port B */
	GPIO_PABCD_CNR, 	/* Port C */
	GPIO_PABCD_CNR, 	/* Port D */
	GPIO_PEFGH_CNR,		/* Port E */
	GPIO_PEFGH_CNR,		/* Port F */
	GPIO_PEFGH_CNR,		/* Port G */
	GPIO_PEFGH_CNR		/* Port H */
};


static uint32 bitStartGpioControlWrite[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E*/
	8, 			/* Port F */
	16,  		/* Port G */
	24, 		/* Port H */    
};


static uint32 bitStartGpioControlRead[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */	
	16, 		/* Port G */
	24			/* Port H */
};


/* Direction */
static uint32 regGpioDirectionRead[] =
{
	GPIO_PABCD_DIR, 	/* Port A */
	GPIO_PABCD_DIR, 	/* Port B */
	GPIO_PABCD_DIR, 	/* Port C */
	GPIO_PABCD_DIR, 	/* Port D */
	GPIO_PEFGH_DIR,		/* Port E */
	GPIO_PEFGH_DIR,		/* Port F */
	GPIO_PEFGH_DIR,		/* Port G */
	GPIO_PEFGH_DIR		/* Port H */
};


static uint32 regGpioDirectionWrite[] =
{
	GPIO_PABCD_DIR, 	/* Port A */
	GPIO_PABCD_DIR, 	/* Port B */
	GPIO_PABCD_DIR, 	/* Port C */
	GPIO_PABCD_DIR, 	/* Port D */
	GPIO_PEFGH_DIR,		/* Port E */
	GPIO_PEFGH_DIR,		/* Port F */
	GPIO_PEFGH_DIR,		/* Port G */
	GPIO_PEFGH_DIR		/* Port H */
};


static uint32 bitStartGpioDirectionWrite[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E*/
	8, 			/* Port F */
	16,  		/* Port G */
	24, 		/* Port H */    
};


static uint32 bitStartGpioDirectionRead[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */	
	16, 		/* Port G */
	24			/* Port H */
};

/* Data */
static uint32 regGpioDataRead[] =
{
	GPIO_PABCD_DAT, 	/* Port A */
	GPIO_PABCD_DAT, 	/* Port B */
	GPIO_PABCD_DAT, 	/* Port C */
	GPIO_PABCD_DAT, 	/* Port D */
	GPIO_PEFGH_DAT, 	/* Port E */
	GPIO_PEFGH_DAT, 	/* Port F */
	GPIO_PEFGH_DAT, 	/* Port G */
	GPIO_PEFGH_DAT, 	/* Port H */
};

static uint32 regGpioDataWrite[] =
{
	GPIO_PABCD_DAT, 	/* Port A */
	GPIO_PABCD_DAT, 	/* Port B */
	GPIO_PABCD_DAT, 	/* Port C */
	GPIO_PABCD_DAT, 	/* Port D */
	GPIO_PEFGH_DAT, 	/* Port E */
	GPIO_PEFGH_DAT, 	/* Port F */
	GPIO_PEFGH_DAT, 	/* Port G */
	GPIO_PEFGH_DAT, 	/* Port H */
};

static uint32 bitStartGpioDataWrite[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */
	16,  		/* Port G */
	24, 		/* Port H */
};

static uint32 bitStartGpioDataRead[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */
	16, 		/* Port G */
	24			/* Port H */
};

/* interrupt imr */
static uint32 regGpioInterruptEnableRead[] =
{
	GPIO_PAB_IMR,	/* Port A */
	GPIO_PAB_IMR,	/* Port B */
	GPIO_PCD_IMR, 	/* Port C */
	GPIO_PCD_IMR,	/* Port D */
	GPIO_PEF_IMR,	/* Port E */
	GPIO_PEF_IMR,	/* Port F */
	GPIO_PGH_IMR,	/* Port G */
	GPIO_PGH_IMR	/* Port H */

};

static uint32 regGpioInterruptEnableWrite[] =
{
	GPIO_PAB_IMR,	/* Port A */
	GPIO_PAB_IMR,	/* Port B */
	GPIO_PCD_IMR, 	/* Port C */
	GPIO_PCD_IMR,	/* Port D */
	GPIO_PEF_IMR,	/* Port E */
	GPIO_PEF_IMR,	/* Port F */
	GPIO_PGH_IMR,	/* Port G */
	GPIO_PGH_IMR	/* Port H */

};

static uint32 bitStartGpioInterruptEnableRead[] =
{
	0, 				/* Port A */
	16,  			/* Port B */
	0,				/* Port C */
	16, 			/* Port D */
	0,  			/* Port E */
	16, 			/* Port F */
	0,  			/* Port G */
	16
};

static uint32 bitStartGpioInterruptEnableWrite[] =
{
	0, 				/* Port A */
	16,  			/* Port B */
	0,				/* Port C */
	16, 			/* Port D */
	0,  			/* Port E */
	16, 			/* Port F */
	0,  			/* Port G */
	16				/* Port H */
};


/* interrupt status */
static uint32 regGpioInterruptStatusWrite[] =
{
	GPIO_PABCD_ISR, 	/* Port A */
	GPIO_PABCD_ISR, 	/* Port B */
	GPIO_PABCD_ISR, 	/* Port C */
	GPIO_PABCD_ISR, 	/* Port D */
	GPIO_PEFGH_ISR,		/* Port E */
	GPIO_PEFGH_ISR,		/* Port F*/
	GPIO_PEFGH_ISR,		/* Port G*/
	GPIO_PEFGH_ISR		/* Port H*/
};


static uint32 regGpioInterruptStatusRead[] =
{
	GPIO_PABCD_ISR, 	/* Port A */
	GPIO_PABCD_ISR, 	/* Port B */
	GPIO_PABCD_ISR, 	/* Port C */
	GPIO_PABCD_ISR, 	/* Port D */
	GPIO_PEFGH_ISR,		/* Port E */
	GPIO_PEFGH_ISR,		/* Port F*/
	GPIO_PEFGH_ISR,		/* Port G*/
	GPIO_PEFGH_ISR		/* Port H*/
};

/* need check */
static uint32 bitStartGpioInterruptStatusWrite[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */
	16, 		/* Port G */
	24			/* Port H */
};

static uint32 bitStartGpioInterruptStatusRead[] =
{
	0, 			/* Port A */
	8, 			/* Port B */
	16,  		/* Port C */
	24, 		/* Port D */
	0, 			/* Port E */
	8, 			/* Port F */
	16, 		/* Port G */
	24, 		/* Port H */
};

/*
@func int32 | _setGpio | abstract GPIO registers 
@parm enum GPIO_FUNC | func | control/data/interrupt register
@parm enum GPIO_PORT | port | GPIO port
@parm uint32 | pin | pin number
@parm uint32 | data | value
@rvalue NONE
@comm
This function is for internal use only. You don't need to care what register address of GPIO is.
This function abstracts these information.
*/

static void _setGpio( enum GPIO_FUNC func, enum GPIO_PORT port, uint32 pin, uint32 data )
{	
	if(DebugEnable)
		printk( "[%s():%d] func=%d port=%d pin=%d data=%d\n", __FUNCTION__, __LINE__, func, port, pin, data);
	switch( func )
	{
		case GPIO_FUNC_CONTROL:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDirectionWrite[port]=0x%08x  bitStartGpioDirectionWrite[port]=%d\n", __FUNCTION__, __LINE__, regGpioControlWrite[port], bitStartGpioControlWrite[port] );

			if ( data ){
				REG32(regGpioControlWrite[port]) |= (uint32)1 << (pin+bitStartGpioControlWrite[port]);// 1 << (6+8=14)
				if(DebugEnable)
					printk("REG32(regGpioDirectionWrite[%d]) 0x%08x \n operbit=0x%08x\n",port,REG32(regGpioControlWrite[port]),(uint32)1 << (pin+bitStartGpioControlWrite[port]));
			}else{
				REG32(regGpioControlWrite[port]) &= ~((uint32)1 << (pin+bitStartGpioControlWrite[port]));
				if(DebugEnable)
					printk("REG32(regGpioDirectionWrite[%d]) 0x%08x \n operbit=0x%08x\n",port,REG32(regGpioControlWrite[port]),~((uint32)1 << (pin+bitStartGpioControlWrite[port])));
			}
			break;
		case GPIO_FUNC_DIRECTION:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDirectionWrite[port]=0x%08x  bitStartGpioDirectionWrite[port]=%d\n", __FUNCTION__, __LINE__, regGpioDirectionWrite[port], bitStartGpioDirectionWrite[port] );

			if ( data ){
				REG32(regGpioDirectionWrite[port]) |= (uint32)1 << (pin+bitStartGpioDirectionWrite[port]);// 1 << (6+8=14)
				if(DebugEnable)
					printk("REG32(regGpioDirectionWrite[%d]) 0x%08x \n operbit=0x%08x\n",port,REG32(regGpioDirectionWrite[port]),(uint32)1 << (pin+bitStartGpioDirectionWrite[port]));
			}else{
				REG32(regGpioDirectionWrite[port]) &= ~((uint32)1 << (pin+bitStartGpioDirectionWrite[port]));
				if(DebugEnable)
					printk("REG32(regGpioDirectionWrite[%d]) 0x%08x \n operbit=0x%08x\n",port,REG32(regGpioDirectionWrite[port]),~((uint32)1 << (pin+bitStartGpioDirectionWrite[port])));
			}
			
			break;
		case GPIO_FUNC_DATA:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDataWrite[port]=0x%08x  bitStartGpioDataWrite[port]=%d\n", __FUNCTION__, __LINE__, regGpioDataWrite[port], bitStartGpioDataWrite[port]);

			if ( data ){
				REG32(regGpioDataWrite[port]) |= (uint32)1 << (pin+bitStartGpioDataWrite[port]);
				if(DebugEnable)
					printk("REG32(regGpioDataWrite[%d]) 0x%08x shift %d\n operbit=0x%08x\n", port, REG32(regGpioDataWrite[port]),(pin+bitStartGpioDataWrite[port]),(uint32)1 << (pin+bitStartGpioDataWrite[port]));
			}else{
				REG32(regGpioDataWrite[port]) &= ~((uint32)1 << (pin+bitStartGpioDataWrite[port]));
				if(DebugEnable)
					printk("REG32(regGpioDataWrite[%d]) 0x%08x shift %d\n operbit=0x%08x\n", port, REG32(regGpioDataWrite[port]),(pin+bitStartGpioDataWrite[port]),~((uint32)1 << (pin+bitStartGpioDataWrite[port])));
			}
			break;
			

		case GPIO_FUNC_INTERRUPT_ENABLE:
			if(DebugEnable)
				printk( "[%s():%d] regGpioInterruptEnableWrite[port]=0x%08x  bitStartGpioInterruptEnableWrite[port]=%d\n", __FUNCTION__, __LINE__, regGpioInterruptEnableWrite[port], bitStartGpioInterruptEnableWrite[port] );

			if (data == GPIO_INT_DUAL_ENABLE)
				REG32(regGpioInterruptEnableWrite[port]) |= (uint32)0b11 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]);
			else if(data == GPIO_INT_FALLING_EDGE_ENABLE)
				REG32(regGpioInterruptEnableWrite[port]) = (REG32(regGpioInterruptEnableWrite[port]) & ~((uint32)0b11 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]))) | ((uint32)0b01 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]));
			else if(data == GPIO_INT_RISING_EDGE_ENABLE)
				REG32(regGpioInterruptEnableWrite[port]) = (REG32(regGpioInterruptEnableWrite[port]) & ~((uint32)0b11 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]))) | ((uint32)0b10 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]));
			else
				REG32(regGpioInterruptEnableWrite[port]) &= ~((uint32)0b11 << ((pin*2)+bitStartGpioInterruptEnableWrite[port]));
			if(DebugEnable)
				printk("REG32(regGpioInterruptEnableWrite[%d]) 0x%08x shift %d\n operbit=0x%08x\n", port, REG32(regGpioInterruptEnableWrite[port]),(pin*2)+bitStartGpioInterruptEnableWrite[port],~((uint32)0b11 << ((pin*2)+bitStartGpioInterruptEnableWrite[port])));
			break;

		case GPIO_FUNC_INTERRUPT_STATUS:
			if(DebugEnable)
				printk( "[%s():%d] regGpioInterruptStatusWrite[port]=0x%08x  bitStartGpioInterruptStatusWrite[port]=%d\n", __FUNCTION__, __LINE__, regGpioInterruptStatusWrite[port], bitStartGpioInterruptStatusWrite[port] );

			if ( data )
				REG32(regGpioInterruptStatusWrite[port]) |= (uint32)1 << (pin+bitStartGpioInterruptStatusWrite[port]);
			else
				REG32(regGpioInterruptStatusWrite[port]) &= ~((uint32)1 << (pin+bitStartGpioInterruptStatusWrite[port]));
			break;

		case GPIO_FUNC_MAX:
			break;
	}
}


/*
@func int32 | _getGpio | abstract GPIO registers 
@parm enum GPIO_FUNC | func | control/data/interrupt register
@parm enum GPIO_PORT | port | GPIO port
@parm uint32 | pin | pin number
@rvalue uint32 | value
@comm
This function is for internal use only. You don't need to care what register address of GPIO is.
This function abstracts these information.
*/
static uint32 _getGpio( enum GPIO_FUNC func, enum GPIO_PORT port, uint32 pin )
{
	if(DebugEnable)
		printk("[%s():%d] func=%d port=%d pin=%d\n", __FUNCTION__, __LINE__, func, port, pin);
	switch( func )
	{
		case GPIO_FUNC_CONTROL:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDirectionRead[port]=0x%08x  bitStartGpioDirectionRead[port]=%d\n", __FUNCTION__, __LINE__, regGpioControlRead[port], bitStartGpioControlRead[port] );

			if ( REG32(regGpioControlRead[port]) & ( (uint32)1 << (pin+bitStartGpioControlRead[port]) ) )
				return 1;
			else
				return 0;
			break;		
		case GPIO_FUNC_DIRECTION:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDirectionRead[port]=0x%08x  bitStartGpioDirectionRead[port]=%d\n", __FUNCTION__, __LINE__, regGpioDirectionRead[port], bitStartGpioDirectionRead[port] );

			if ( REG32(regGpioDirectionRead[port]) & ( (uint32)1 << (pin+bitStartGpioDirectionRead[port]) ) )
				return 1;
			else
				return 0;
			break;
			
		case GPIO_FUNC_DATA:
			if(DebugEnable)
				printk( "[%s():%d] regGpioDataRead[port]=0x%08x  bitStartGpioDataRead[port]=%d get=0x%08x REG32(regGpioDataRead[port]) 0x%08x\n", __FUNCTION__, __LINE__, regGpioDataRead[port], bitStartGpioDataRead[port] ,REG32(regGpioDataRead[port]) & ( (uint32)1 << (pin+bitStartGpioDataRead[port]) ),REG32(regGpioDataRead[port]));

			if ( REG32(regGpioDataRead[port]) & ( (uint32)1 << (pin+bitStartGpioDataRead[port]) ) )
				return 1;
			else
				return 0;
			break;
			
		case GPIO_FUNC_INTERRUPT_ENABLE:
			if(DebugEnable)
				printk( "[%s():%d] regGpioInterruptEnableRead[port]=0x%08x  bitStartGpioInterruptEnableRead[port]=%d\n", __FUNCTION__, __LINE__, regGpioInterruptEnableRead[port], bitStartGpioInterruptEnableRead[port] );

			if ( REG32(regGpioInterruptEnableRead[port]) & ( (uint32)1 <<(pin+bitStartGpioInterruptEnableRead[port]) ))
				return 1;
			else
				return 0;
			break;

		case GPIO_FUNC_INTERRUPT_STATUS:
			if(DebugEnable)
				printk( "[%s():%d] regGpioInterruptStatusRead[port]=0x%08x  bitStartGpioInterruptEnableRead[port]=%d\n", __FUNCTION__, __LINE__, regGpioInterruptStatusRead[port], bitStartGpioInterruptStatusRead[port] );

			if ( REG32(regGpioInterruptStatusRead[port]) & ( (uint32)1 << (pin+bitStartGpioInterruptStatusRead[port]) ) )
				return 1;
			else
				return 0;
			break;
			
		case GPIO_FUNC_MAX:
			break;
	}
	return 0xeeeeeeee;
}

/*************************************************************************************/
#if 0
/*
@func int32 | initGpioPin | Initiate a specifed GPIO port.
@parm uint32 | gpioId | The GPIO port that will be configured
@parm enum GPIO_PERIPHERAL | dedicate | Dedicated peripheral type
@parm enum GPIO_DIRECTION | direction | Data direction, in or out
@parm enum GPIO_INTERRUPT_TYPE | interruptEnable | Interrupt mode
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
This function is used to initialize GPIO port.
*/
int32 initGpioPin( uint32 gpioId, enum GPIO_DIRECTION direction, 
                                           enum GPIO_INTERRUPT_TYPE interruptEnable )                                    
{

	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if(DebugEnable)
		printk("initGpioPin port %d pin %d \n",port,pin);
		
	_setGpio( GPIO_FUNC_DIRECTION, port, pin, direction );

	_setGpio( GPIO_FUNC_INTERRUPT_ENABLE, port, pin, interruptEnable );


	return SUCCESS;
}
#endif

/*
@func int32 | getGpioControlBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioControlBit( uint32 gpioId, uint32* pData )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;
	
	*pData = _getGpio( GPIO_FUNC_CONTROL, port, pin );
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}


/*
@func int32 | getGpioControlBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioControlBit( uint32 gpioId, uint32 data )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, data );

	_setGpio( GPIO_FUNC_CONTROL, port, pin, data );
	
	return SUCCESS;
}

/*
@func int32 | getGpioDirBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioDirBit( uint32 gpioId, uint32* pData )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;
	
	*pData = _getGpio( GPIO_FUNC_DIRECTION, port, pin );
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}


/*
@func int32 | setGpioDirBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioDirBit( uint32 gpioId, uint32 data )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, data );

	_setGpio( GPIO_FUNC_DIRECTION, port, pin, data );
	
	return SUCCESS;
}

/*
@func int32 | getGpioDataBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioDataBit( uint32 gpioId, uint32* pData )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;
	
	*pData = _getGpio( GPIO_FUNC_DATA, port, pin );
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}



/*
@func int32 | setGpioDataBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioDataBit( uint32 gpioId, uint32 data )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, data );

	_setGpio( GPIO_FUNC_DATA, port, pin, data );
	
	return SUCCESS;
}


/*
@func int32 | getGpioInterruptBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioInterruptBit( uint32 gpioId, uint32* pData )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;
	
	*pData = _getGpio( GPIO_FUNC_INTERRUPT_ENABLE, port, pin );
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}



/*
@func int32 | setGpioInterruptBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioInterruptBit( uint32 gpioId, uint32 data )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, data );

	_setGpio( GPIO_FUNC_INTERRUPT_ENABLE, port, pin, data );
	
	return SUCCESS;
}


/*
@func int32 | getGpioInterruptStatusBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioInterruptStatusBit( uint32 gpioId, uint32* pData )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;
	
	*pData = _getGpio( GPIO_FUNC_INTERRUPT_STATUS, port, pin );
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}



/*
@func int32 | setGpioInterruptStatusBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioInterruptStatusBit( uint32 gpioId, uint32 data )
{
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if ( port >= GPIO_PORT_MAX ) return FAILED;

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, data );

	_setGpio( GPIO_FUNC_INTERRUPT_STATUS, port, pin, data );
	
	return SUCCESS;
}




