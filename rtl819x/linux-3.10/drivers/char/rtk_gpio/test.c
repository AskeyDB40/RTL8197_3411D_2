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

#include "rtl_gpiocommon.h"
#include "test.h"

static int errorBuf[64];
int32 rtl_gpio_test(void)
{
	char port= 'A';
	int pin = 0,j,i = 0,res = 0,error = 0;
	uint32 data;
	memset(errorBuf,0x0,64);
	getPinMuxAllVal();

	for(port='A';port<='H';port++){
		for(pin = 0;pin < 8;pin++){
			#if 0
			for(j = 0;j < sizeof(MuxTable)/sizeof(PinMuxTbl);j++){
				if(GPIO_ID(port,pin) == MuxTable[j].gpiopin.gpionum)
					break;
			}
			if(j == sizeof(MuxTable)/sizeof(PinMuxTbl)){
				gpioDebug("GPIO%c%d not exist in MuxTable\n",port,pin);
				continue;
			}
			#endif
			if(checkPinExist(port,pin) == FAILED)
				continue;

			gpioDebug("test GPIO%c%d start\n",port,pin);
		
			/* restore pin mux */
			setPinMuxAllVal();

			/* pin mux */
			if(setPinMux(PIN_GPIO_MODE,GPIO_ID(port,pin)) == FAILED){
				gpioDebug("test GPIO%c%d failed: set pin mux failed\n",port,pin);
				goto Error;
			}
			if(getPinMux(PIN_GPIO_MODE,GPIO_ID(port,pin),&data) == FAILED){
				gpioDebug("test GPIO%c%d failed: get pin mux failed\n",port,pin);
				goto Error;
			}

			#if 0
			if(data != (MuxTable[j].muxBitMask & MuxTable[j].gpiopin.muxBitVal)){
				gpioDebug("test GPIO%c%d failed: pin data not correct\n",port,pin);
				goto Error;
			}
			#endif
			if(checkPinMuxVal(GPIO_ID(port,pin),data) == FAILED){
				gpioDebug("test GPIO%c%d failed: pin data not correct\n",port,pin);
				goto Error;
			}

			/* set controlt register and dir register */
			setGpioControl(GPIO_ID(port,pin),GPIO_GPIO_PIN);
			setGpioDir(GPIO_ID(port,pin),GPIO_DIR_OUT);
			
			setGpioData(GPIO_ID(port,pin),GPIO_DATA_HIGH);
			getGpioData(GPIO_ID(port,pin),&data);
			if(data != GPIO_DATA_HIGH){
				gpioDebug("test GPIO%c%d failed high: data reg is not the writen value\n",port,pin);
				goto Error;
			}
			
			setGpioData(GPIO_ID(port,pin),GPIO_DATA_LOW);
			getGpioData(GPIO_ID(port,pin),&data);
			if(data != GPIO_DATA_LOW){
				gpioDebug("test GPIO%c%d failed low: data reg is not the writen value\n",port,pin);
				goto Error;
			}
				
			gpioDebug("test GPIO%c%d success end\n",port,pin);
			continue;
Error:
			
			gpioDebug("test GPIO%c%d fail end\n",port,pin);
			errorBuf[i++] = GPIO_ID(port,pin);	
			error = 1;
		}
	}
	

	gpioPrintf("test fail gpio pin\n");
	if(error != 0){
		for(j =0 ;j < 64;j++){
			if(errorBuf[j] != 0){
				gpioPrintf("GPIO%c%d\n",(((errorBuf[j] >> 16) & 0xffff) + 'A'),errorBuf[j] & 0xffff);
			}
		}
		return -1;
	}else{
		gpioPrintf("all GPIO test success\n");
		return 0;
	}
}

int32 rtl_gpio_interrrupt_test()
{
	uint32 wpspin,resetpin;
	char port;
	uint32 pin;
	int j,i = 0,res = 0;
	uint32 data;
	memset(errorBuf,0x0,64);
	//gpiohandlerval(port,pin);

	getRestWpsPin(&resetpin,&wpspin);
	port = GPIO_PORT_NAME(resetpin);
	pin = GPIO_PIN(resetpin);
	
	getPinMuxAllVal();
	/* test reset pin */
	#if 0
	for(j = 0;j < sizeof(MuxTable)/sizeof(PinMuxTbl);j++){
		if(GPIORESETPIN == MuxTable[j].gpiopin.gpionum)
			break;
	}

	if(j == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		gpioDebug("GPIO%c%d not exist in MuxTable\n",(port+'A'),pin);
		return FAILED;
	}
	#endif

	if(checkPinExist(port,pin) == FAILED)
		return FAILED;

	gpioDebug("test GPIO%c%d start\n",(port),pin);

	/* restore pin mux */
	setPinMuxAllVal();

	/* pin mux */
	if(setPinMux(PIN_GPIO_MODE,resetpin) == FAILED){
		gpioDebug("test GPIO%c%d failed: set pin mux failed\n",(port),pin);
		goto Error;
	}
	if(getPinMux(PIN_GPIO_MODE,resetpin,&data) == FAILED){
		gpioDebug("test GPIO%c%d failed: get pin mux failed\n",(port),pin);
		goto Error;
	}
	#if 0
	if(data != (MuxTable[j].muxBitMask & MuxTable[j].gpiopin.muxBitVal)){
		gpioDebug("test GPIO%c%d failed: pin data not correct\n",(port+'A'),pin);
		goto Error;
	}
	#endif

	if(checkPinMuxVal(resetpin,data) == FAILED){
		gpioDebug("test GPIO%c%d failed: pin data not correct\n",port,pin);
		goto Error;
	}

	/* set controlt register and dir register */
	setGpioControl(resetpin,GPIO_GPIO_PIN);
	setGpioDir(resetpin,GPIO_DIR_IN);

	setGpioInterruptEnable(resetpin,GPIO_INT_DUAL_ENABLE);
#ifdef __KERNEL__
	setGpioInterruptHandler0(resetpin,gpiohandlerResetPin);
#else
	setGpioInterruptHandler0(resetpin,GPIO_RESET_PIN_TEST);
#endif
#if 0
	printk("test gpio%c%d interrupt start\n",port,pin);
	while(1){
		if(gpioInterruptTimesH5 >= 10)
			break;
		printk("%d\n",gpioInterruptTimesH5);
	}
	printk("test gpio%c%d interrupt end\n",port,pin);
#endif

	/* test wps pin */
	port = GPIO_PORT_NAME(wpspin);
	pin = GPIO_PIN(wpspin);
	//gpiohandlerval(port,pin);
	#if 0
	for(j = 0;j < sizeof(MuxTable)/sizeof(PinMuxTbl);j++){
		if(GPIOWPSPIN == MuxTable[j].gpiopin.gpionum)
				break;
	}
	if(j == sizeof(MuxTable)/sizeof(PinMuxTbl)){
		gpioDebug("GPIO%c%d not exist in MuxTable\n",(port+'A'),pin);
		return FAILED;
	}
	#endif

	if(checkPinExist(port,pin) == FAILED)
		return FAILED;
	
	gpioDebug("test GPIO%c%d start\n",(port),pin);

	/* restore pin mux */
	setPinMuxAllVal();

	/* pin mux */
	if(setPinMux(PIN_GPIO_MODE,wpspin) == FAILED){
		gpioDebug("test GPIO%c%d failed: set pin mux failed\n",(port),pin);
		goto Error;
	}
	if(getPinMux(PIN_GPIO_MODE,wpspin,&data) == FAILED){
		gpioDebug("test GPIO%c%d failed: get pin mux failed\n",(port),pin);
		goto Error;
	}
	#if 0
	if(data != (MuxTable[j].muxBitMask & MuxTable[j].gpiopin.muxBitVal)){
		gpioDebug("test GPIO%c%d failed: pin data not correct\n",(port+'A'),pin);
		goto Error;
	}
	#endif
	if(checkPinMuxVal(wpspin,data) == FAILED){
		gpioDebug("test GPIO%c%d failed: pin data not correct\n",port,pin);
		goto Error;
	}

	/* set controlt register and dir register */
	setGpioControl(wpspin,GPIO_GPIO_PIN);
	setGpioDir(wpspin,GPIO_DIR_IN);

	setGpioInterruptEnable(wpspin,GPIO_INT_DUAL_ENABLE);
#ifdef __KERNEL__
	setGpioInterruptHandler0(wpspin,gpiohandlerWpsPin);
#else
	setGpioInterruptHandler0(wpspin,GPIO_WPS_PIN_TEST);
#endif
#if 0
	printk("test gpio%c%d interrupt start\n",port,pin);
	while(1){
		if(gpioInterruptTimesH6 >= 10)
			break;
		printk("%d\n",gpioInterruptTimesH6);
	}
	printk("test gpio%c%d interrupt end\n",port,pin);
#endif

	return 0;

Error:
	return FAILED;
	
}

#ifndef __KERNEL__
int main(int argv,char** argc)
{
	if(rtl_gpio_test() < 0){
		printf("%s:%d:gpio normal test fail\n");
	}else{
		printf("%s:%d:gpio normal test success\n");
	}
	if(rtl_gpio_interrrupt_test() < 0){
		printf("%s:%d,gpio interrupt test fail\n");
		return 0;
	}else{
		printf("%s:%d,gpio interrupt test success\n");
		return -1;
	}

}
#endif

