#ifndef __RTL_GPIOCOMMON_H__
#define __RTL_GPIOCOMMON_H__

typedef unsigned int 	uint32;
typedef int				int32;

#define RTL_GPIO_SET_ACTION				0x1
#define RTL_GPIO_GET_ACTION				0x2

#define RTL_GPIO_MUX					0x1
#define RTL_GPIO_CONTROL				0x2
#define RTL_GPIO_DIR					0x3
#define RTL_GPIO_DAT					0x4
#define RTL_GPIO_INTERRUPT_ENABLE		0x5
#define RTL_GPIO_INTERRUPT_STATUS		0x6
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
#define RTL_GPIO_MUX_ALL				0x7
#endif
#define RTL_GPIO_SET_IRQHANDLER			0x8

#define FAILED	-1
#define SUCCESS	0

#define GPIO_ID(port,pin)		((( port - 'A' ) << 16 ) | ( pin ))
#define GPIO_PORT_NAME(id) 		((id>>16) + 'A')
#define GPIO_PORT(id) 			(id>>16)
#define GPIO_PIN(id) 			(id&0xffff)

enum PIN_MODE
{
	PIN_GPIO_MODE = 0,
	PIN_JTAG_MODE,
};

/* define GPIO control */
enum GPIO_CONTROL
{
	GPIO_GPIO_PIN = 0,
	GPIO_PERIPHERAL_PIN =1,
};

/* define GPIO direction */
enum GPIO_DIRECTION
{
	GPIO_DIR_IN = 0,
	GPIO_DIR_OUT =1,
};

/* define GPIO data */
enum GPIO_DATA
{
	GPIO_DATA_HIGH = 0,
	GPIO_DATA_LOW =1,
};

/* define GPIO Interrupt Type */
enum GPIO_INTERRUPT_TYPE
{
	GPIO_INT_DISABLE = 0,
	GPIO_INT_DUAL_ENABLE,
	GPIO_INT_RISING_EDGE_ENABLE,
	GPIO_INT_FALLING_EDGE_ENABLE,
};

/* define GPIO Interrupt Status */
enum GPIO_INTERRUPT_STATUS
{
	GPIO_INT_DO_NOTHING = 0,
	GPIO_INT_CLEAN,
};

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
};


typedef struct PinMuxTable{
	uint32				pinModMux;
	uint32				muxReg;
	uint32				muxBitMask;
	
	struct gpiopin{
		uint32 			gpionum;
		uint32			muxBitVal;
	};
	
	struct jtagpin{
		uint32			muxBitVal;
	};
	
	struct gpiopin gpiopin;
	struct jtagpin jtagpin;

}PinMuxTbl;

#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
enum GPIO_IRQ_TEST{
	GPIO_RESET_PIN_TEST = 0,
	GPIO_WPS_PIN_TEST,
};
#endif

int getPinMux(enum PIN_MODE mod,uint32 pinNum,uint32* data);
int setPinMux(enum PIN_MODE mod,uint32 pinNum);
#ifdef __KERNEL__
int32 setGpioControlBit( uint32 gpioId, uint32 data );
int32 getGpioControlBit( uint32 gpioId, uint32* pData );
int32 setGpioDirBit( uint32 gpioId, uint32 data );
int32 getGpioDirBit( uint32 gpioId, uint32* pData );
int32 setGpioDataBit( uint32 gpioId, uint32 data );
int32 getGpioDataBit( uint32 gpioId, uint32* pData );
int32 setGpioInterruptBit( uint32 gpioId, uint32 data );
int32 getGpioInterruptBit( uint32 gpioId, uint32* pData );
int32 setGpioInterruptStatusBit( uint32 gpioId, uint32 data );
int32 getGpioInterruptStatusBit( uint32 gpioId, uint32* pData );
int setGpioInterruptHandler(uint32 gpioId,int (*gpioHandler)(void));
#define setGpioControl 				setGpioControlBit
#define setGpioDir					setGpioDirBit
#define setGpioData					setGpioDataBit
#define getGpioControl 				getGpioControlBit
#define getGpioDir					getGpioDirBit
#define getGpioData					getGpioDataBit
#define setGpioInterruptEnable		setGpioInterruptBit
#define getGpioInterruptEnable		getGpioInterruptBit
#define setGpioInterruptStatus		setGpioInterruptStatusBit
#define getGpioInterruptStatus		getGpioInterruptStatusBit
#define setPinMuxAllVal				setPinMuxInitalVal
#define getPinMuxAllVal				getPinMuxInitalVal
#define setGpioInterruptHandler0	setGpioInterruptHandler
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
int setGpioInterruptHandlerUser(uint32 gpioId,enum GPIO_IRQ_TEST irq_type);
#endif
#else
int setGpioControl(uint32 gpioId,uint32 data);
int setGpioDir(uint32 gpioId,uint32 data);
int setGpioData(uint32 gpioId,uint32 data);
int getGpioControl(uint32 pinNum,uint32 *data);
int getGpioDir(uint32 pinNum,uint32 *data);
int getGpioData(uint32 pinNum,uint32 *data);
int setGpioInterruptEnable( uint32 gpioId, uint32 data );
int getGpioInterruptEnable( uint32 gpioId, uint32* pData );
int32 setGpioInterruptStatus( uint32 gpioId, uint32 data );
int32 getGpioInterruptStatus( uint32 gpioId, uint32* pData );
int getPinMuxAllVal(void);
int setPinMuxAllVal(void);
int setGpioInterruptHandler0(uint32 gpioId,enum GPIO_IRQ_TEST irq_type);
#endif

#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
int32 rtl_gpio_test(void);
int32 rtl_gpio_interrrupt_test(void);
int gpiohandlerResetPin(void);
int gpiohandlerWpsPin(void)	;
#endif

#ifdef __KERNEL__
#define REG32(reg)           (*(volatile unsigned long *)(reg))
#endif

//#define GPIO_DEBUG
#ifndef GPIO_DEBUG
#define gpioDebug(fmt,args...)
#else
#ifdef __KERNEL__
#define gpioDebug(fmt,args...)	printk(fmt, ##args)
#else
#define gpioDebug(fmt,args...)	printf(fmt, ##args)
#endif
#endif

#ifdef __KERNEL__
#define gpioPrintf(fmt,args...)		printk(fmt, ##args)
#else
#define gpioPrintf(fmt,args...)		printf(fmt, ##args)
#endif

#endif