#include "rtl_gpiocommon.h"

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
#include "rtl_pinMuxOps.h"

static int DebugEnable = 0;

/*
@func int32 | getGpioMuxBit | Get the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32* | data | Pointer to store return value
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 getGpioMuxBit( uint32 gpioId, uint32* pData )
{
	int i;
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)\n", __FUNCTION__, __LINE__, port, pin );

	if ( port >= GPIO_PORT_MAX ) return FAILED;
	if ( pData == NULL ) return FAILED;

	#if 0
	for(i = 0;i < sizeof(MuxTable)/sizeof(PinMuxTbl);i++){
		if(gpioId == MuxTable[i].gpiopin.gpionum){
			*pData = REG32(MuxTable[i].muxReg) & MuxTable[i].muxBitMask;
			break;
		}
	}
	
	if(i == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		printk("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}
	#endif

	if(getPinMuxVal(gpioId,pData) == FAILED)
		return FAILED;
	
	if(DebugEnable)
		printk( "[%s():%d] (port=%d,pin=%d)=%d\n", __FUNCTION__, __LINE__, port, pin, *pData );

	return SUCCESS;
}

/*
@func int32 | setGpioMuxBit | Set the bit value of a specified GPIO ID.
@parm uint32 | gpioId | GPIO ID
@parm uint32 | data | Data to write
@rvalue SUCCESS | success.
@rvalue FAILED | failed. Parameter error.
@comm
*/
int32 setGpioMuxBit( uint32 gpioId)
{
	int i;
	uint32 port = GPIO_PORT( gpioId );
	uint32 pin = GPIO_PIN( gpioId );

	if(DebugEnable)
		printk("[%s():%d] (port=%d,pin=%d)\n", __FUNCTION__, __LINE__, port, pin );

	if ( port >= GPIO_PORT_MAX ) return FAILED;


	#if 0
	for(i = 0;i < sizeof(MuxTable)/sizeof(PinMuxTbl);i++){
		if(gpioId == MuxTable[i].gpiopin.gpionum){
			if((REG32(MuxTable[i].muxReg) & MuxTable[i].muxBitMask) == MuxTable[i].gpiopin.muxBitVal)
				return SUCCESS;
			REG32(MuxTable[i].muxReg) = REG32(MuxTable[i].muxReg) & (~(MuxTable[i].muxBitMask)) | MuxTable[i].gpiopin.muxBitVal;
			break;
		}
	}

	if(i == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		printk("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}
	#endif

	if(setPinMuxVal(gpioId) == FAILED)
		return FAILED;
	
	return SUCCESS;
}

static int setGpioPinMux(uint32 pinNum)
{
	return setGpioMuxBit(pinNum);
}

static int setJtagPinMux(uint32 pinNum)
{
	return 0;
}

static int getGpioPinMux(uint32 pinNum,uint32 *data)
{
	return getGpioMuxBit(pinNum,data);
}

static int getJtagPinMux(uint32 pinNum,uint32 *data)
{
	return 0;
}
#else
	/* user use */
#include "stdio.h"
#include "errno.h"
extern int errno;
	
static int gpioCmdRet(char* cmdbuf){
	FILE * file;

	file = popen(cmdbuf, "r");
	if(file != NULL){
		/* ret value */
		if(errno != 0){
			pclose(file);
			return FAILED;
		}else{
			pclose(file);
			return SUCCESS;
		}
	}else
		return FAILED;
}
	
static int setGpioPinMux(uint32 pinNum)
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_MUX);
	return gpioCmdRet(cmdBuf);
}

static int setJtagPinMux(uint32 pinNum)
{
	return 0;
}

static int getGpioPinMux(uint32 pinNum,uint32 *data)
{
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED){
		printf("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_MUX);
	if(gpioCmdRet(cmdBuf) == FAILED){
		printf("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL){
		printf("%s:%d\n",__func__,__LINE__);
		return FAILED;
	}

	if(fgets(dataBuf,16,file) == NULL){
		printf("%s:%d\n",__func__,__LINE__);
		pclose(file);
		return FAILED;
	}

	*data = atoi(dataBuf);

	pclose(file);
	return SUCCESS;
	
}

static int getJtagPinMux(uint32 pinNum,uint32 *data)
{
	return 0;
}

/* set/get pin mux all */
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
int setPinMuxAllVal(void)
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_MUX_ALL);
	return gpioCmdRet(cmdBuf);
}

int getPinMuxAllVal(void)
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_MUX_ALL);
	return gpioCmdRet(cmdBuf);
}
#endif
#endif

static int (*setPinMuxFunc[])(uint32 pinNum) = {
	setGpioPinMux,
	setJtagPinMux,
};

static int (*getPinMuxFunc[])(uint32 pinNum,uint32 *data) = {
	getGpioPinMux,
	getJtagPinMux,
};

int setPinMux(enum PIN_MODE mod,uint32 pinNum)
{
	int (*setPinMuxProc)(uint32 pinNum);
	setPinMuxProc = setPinMuxFunc[mod];
	return (*setPinMuxProc)(pinNum);
}

int getPinMux(enum PIN_MODE mod,uint32 pinNum,uint32* data)
{
	int (*getPinMuxProc)(uint32 pinNum,uint32* data);
	getPinMuxProc = getPinMuxFunc[mod];
	return (*getPinMuxProc)(pinNum,data);
}





