#include "rtl_gpiocommon.h"

#ifdef __KERNEL__
#else
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


int setGpioControl(uint32 gpioId,uint32 data)
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;
	
	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_CONTROL,data);
	return gpioCmdRet(cmdBuf);
}

int setGpioDir(uint32 gpioId,uint32 data)
{
	FILE *file;
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_DIR,data);
	return gpioCmdRet(cmdBuf);
}

int setGpioData(uint32 gpioId,uint32 data)
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_DAT,data);
	return gpioCmdRet(cmdBuf);
}

int getGpioControl(uint32 pinNum,uint32 *data)
{
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_CONTROL);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL)
		return FAILED;

	if(fgets(dataBuf,16,file) == NULL){
		pclose(file);
		return FAILED;
	}

	*data = atoi(dataBuf);
	pclose(file);
	return SUCCESS;
	
}


int getGpioDir(uint32 pinNum,uint32 *data)
{
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_DIR);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL)
		return FAILED;

	if(fgets(dataBuf,16,file) == NULL){
		pclose(file);
		return FAILED;
	}

	*data = atoi(dataBuf);
	pclose(file);
	return SUCCESS;
	
}


int getGpioData(uint32 pinNum,uint32 *data)
{
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_DAT);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL)
		return FAILED;

	if(fgets(dataBuf,16,file) == NULL){
		pclose(file);
		return FAILED;
	}

	*data = atoi(dataBuf);
	pclose(file);
	return SUCCESS;
	
}

int setGpioInterruptEnable( uint32 gpioId, uint32 data )
{	
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	if(data > GPIO_INT_FALLING_EDGE_ENABLE)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_INTERRUPT_ENABLE,data);
	return gpioCmdRet(cmdBuf);
}

int getGpioInterruptEnable( uint32 pinNum, uint32* pData )
{	
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_INTERRUPT_ENABLE);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL)
		return FAILED;

	if(fgets(dataBuf,16,file) == NULL){
		pclose(file);
		return FAILED;
	}

	*pData = atoi(dataBuf);
	pclose(file);
	return SUCCESS;

}

int32 setGpioInterruptStatus( uint32 gpioId, uint32 data )
{
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_INTERRUPT_STATUS,data);
	return gpioCmdRet(cmdBuf);

}

int32 getGpioInterruptStatus( uint32 pinNum, uint32* pData )
{
	FILE *file;
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",pinNum);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x > /proc/gpioAction",RTL_GPIO_GET_ACTION,RTL_GPIO_INTERRUPT_STATUS);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	//system("cat /proc/gpioAction > /var/gpio.txt");
	file = popen("cat /proc/gpioAction","r");
	if(file == NULL)
		return FAILED;

	if(fgets(dataBuf,16,file) == NULL){
		pclose(file);
		return FAILED;
	}

	*pData = atoi(dataBuf);
	pclose(file);
	return SUCCESS;

}

#endif
