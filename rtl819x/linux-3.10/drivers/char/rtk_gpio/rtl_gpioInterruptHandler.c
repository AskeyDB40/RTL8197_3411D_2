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
#include <linux/interrupt.h>
#include "rtl_gpiocommon.h"

#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
static int gpioInterruptTimesRestPin = 0,gpioInterruptTimesWpsPin = 0;
int gpiohandlerResetPin(void)	
{											
	gpioInterruptTimesRestPin++;		
	gpioPrintf("reset pin times = %d\n",gpioInterruptTimesRestPin);
	return 0;								
}

int gpiohandlerWpsPin(void)	
{
	gpioInterruptTimesWpsPin++;
	gpioPrintf("wps pin times = %d\n",gpioInterruptTimesWpsPin);
	return 0;
}
#endif

