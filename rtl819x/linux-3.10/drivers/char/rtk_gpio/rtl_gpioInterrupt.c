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
#include <linux/interrupt.h>
#include "bspchip.h"
#include "rtl_gpiocommon.h"
#include "rtl_gpioOps.h"

static int gpioABCD_enabled = 0,gpioEFGH_enabled = 0;
#define GPIOHANDLER_NUM(gpioId)			(((gpioId >> 16) & 0xffff)*8 + (gpioId & 0xffff))

static int (*gpioHandlerTbl[])(void) = {
	/* GPIOA */
	NULL,						//GPIOA0
	NULL,						//GPIOA1
	NULL,						//GPIOA2
	NULL,						//GPIOA3
	NULL,						//GPIOA4
	NULL,						//GPIOA5
	NULL,						//GPIOA6
	NULL,						//GPIOA7
	/* GPIOB */
	NULL,						//GPIOB0
	NULL,						//GPIOB1
	NULL,						//GPIOB2
	NULL,						//GPIOB3
	NULL,						//GPIOB4
	NULL,						//GPIOB5
	NULL,						//GPIOB6
	NULL,						//GPIOB7
	/* GPIOC */
	NULL,						//GPIOC0
	NULL,						//GPIOC1
	NULL,						//GPIOC2
	NULL,						//GPIOC3
	NULL,						//GPIOC4
	NULL,						//GPIOC5
	NULL,						//GPIOC6
	NULL,						//GPIOC7
	/* GPIOD */
	NULL,						//GPIOD0
	NULL,						//GPIOD1
	NULL,						//GPIOD2
	NULL,						//GPIOD3
	NULL,						//GPIOD4
	NULL,						//GPIOD5
	NULL,						//GPIOD6
	NULL,						//GPIOD7
	/* GPIOE */
	NULL,						//GPIOE0
	NULL,						//GPIOE1
	NULL,						//GPIOE2
	NULL,						//GPIOE3
	NULL,						//GPIOE4
	NULL,						//GPIOE5
	NULL,						//GPIOE6
	NULL,						//GPIOE7
	/* GPIOF */
	NULL,						//GPIOF0
	NULL,						//GPIOF1
	NULL,						//GPIOF2
	NULL,						//GPIOF3
	NULL,						//GPIOF4
	NULL,						//GPIOF5
	NULL,						//GPIOF6
	NULL,						//GPIOF7
	/* GPIOG */
	NULL,						//GPIOG0
	NULL,						//GPIOG1
	NULL,						//GPIOG2
	NULL,						//GPIOG3
	NULL,						//GPIOG4
	NULL,						//GPIOG5
	NULL,						//GPIOG6
	NULL,						//GPIOG7
	/* GPIOH */
	NULL,						//GPIOH0
	NULL,						//GPIOH1
	NULL,						//GPIOH2
	NULL,						//GPIOH3
	NULL,						//GPIOH4
	NULL,						//GPIOH5
	NULL,						//GPIOH6
	NULL,						//GPIOH7
};

static irqreturn_t gpioABCD_interrupt_isr(int irq, void *dev_instance, struct pt_regs *regs)
{
	int i;
	uint32 status;
	status = REG32(GPIO_PABCD_ISR);
	int (*gpioHandler)(void);

	for(i = 0;i < 32;i++){
		if((status & (1 << i)) != 0){
		
			REG32(GPIO_PABCD_ISR) = REG32(GPIO_PABCD_ISR) | (1<<i);
			gpioHandler = gpioHandlerTbl[i];
			
			if(gpioHandler == NULL){
				gpioDebug("GPIO%c%d hander is null\n",(i/8 + 'A'),i%8);
				continue;
			}
			if((*gpioHandler)() < 0){
				gpioDebug("GPIO%c%d hander is null\n",(i/8 + 'A'),i%8);
			}
		}
	}
	
	return IRQ_HANDLED;
}

static irqreturn_t gpioEFGH_interrupt_isr(int irq, void *dev_instance, struct pt_regs *regs)
{
	#define GPIOEFGH_HANDLER_OFFSET		32
	int i;
	uint32 status;
	status = REG32(GPIO_PEFGH_ISR);
	int (*gpioHandler)(void);

	for(i = 0;i < 32;i++){
		if((status & (1 << i)) != 0){
			REG32(GPIO_PEFGH_ISR) = REG32(GPIO_PEFGH_ISR) | (1<<i);
			gpioHandler = gpioHandlerTbl[i+GPIOEFGH_HANDLER_OFFSET];
			
			if(gpioHandler == NULL){
				gpioDebug("GPIO%c%d hander is null\n",(i/8 + 'E'),i%8);
				continue;
			}
			if((*gpioHandler)() < 0){
				gpioDebug("GPIO%c%d hander is null\n",(i/8 + 'E'),i%8);
			}
		}
	}
	
	return IRQ_HANDLED;
}

int setGpioInterruptHandler(uint32 gpioId,int (*gpioHandler)(void))
{
	if(gpioHandlerTbl[GPIOHANDLER_NUM(gpioId)] == NULL){
		gpioDebug("set GPIO%c%d interrupt handler\n",(((gpioId >> 16) & 0xffff) + 'A'),(gpioId & 0xffff));
		gpioHandlerTbl[GPIOHANDLER_NUM(gpioId)] = gpioHandler;
	}else{
		gpioDebug("GPIO%c%d interrupt handler is set\n",(((gpioId >> 16) & 0xffff) + 'A'),(gpioId & 0xffff));
		gpioHandlerTbl[GPIOHANDLER_NUM(gpioId)] = gpioHandler;
	}

	if(((gpioId >> 16) & 0xffff) < 4){
		if(gpioABCD_enabled == 0){
			if (request_irq(BSP_GPIO_ABCD_IRQ, gpioABCD_interrupt_isr, IRQF_SHARED, "rtl_gpioABCD", &gpioId)) {
				printk("gpio request_irq(%d) error!\n", BSP_GPIO_ABCD_IRQ);
				return FAILED;
		    }
		    gpioABCD_enabled = 1;
	    }
	}else{
		if(gpioEFGH_enabled == 0){
			if (request_irq(BSP_GPIO_EFGH_IRQ, gpioEFGH_interrupt_isr, IRQF_SHARED, "rtl_gpioEFGH", &gpioId)) {
				printk("gpio request_irq(%d) error!\n", BSP_GPIO_EFGH_IRQ);
				return FAILED;
		    }
		    gpioEFGH_enabled = 1;
	    }
    }    
	return SUCCESS;
}


#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
int setGpioInterruptHandlerUser(uint32 gpioId,enum GPIO_IRQ_TEST irq_type)
{
	if(irq_type == GPIO_RESET_PIN_TEST)
		gpioHandlerTbl[GPIOHANDLER_NUM(gpioId)] = gpiohandlerResetPin;
	else if(irq_type == GPIO_WPS_PIN_TEST)
		gpioHandlerTbl[GPIOHANDLER_NUM(gpioId)] = gpiohandlerWpsPin;
	else{
		gpioDebug("irq_type=%d is not support\n",irq_type);
		return FAILED;
	}

	if(((gpioId >> 16) & 0xffff) < 4){
		if(gpioABCD_enabled == 0){
			if (request_irq(BSP_GPIO_ABCD_IRQ, gpioABCD_interrupt_isr, IRQF_SHARED, "rtl_gpioABCD", &gpioId)) {
				printk("gpio request_irq(%d) error!\n", BSP_GPIO_ABCD_IRQ);
				return FAILED;
		    }
		    gpioABCD_enabled = 1;
	    }
	}else{
		if(gpioEFGH_enabled == 0){
			if (request_irq(BSP_GPIO_EFGH_IRQ, gpioEFGH_interrupt_isr, IRQF_SHARED, "rtl_gpioEFGH", &gpioId)) {
				printk("gpio request_irq(%d) error!\n", BSP_GPIO_EFGH_IRQ);
				return FAILED;
		    }
		    gpioEFGH_enabled = 1;
	    }
    }    
	return SUCCESS;
}
#endif
#else
#include <stdio.h>
#include "errno.h"
#include "rtl_gpiocommon.h"
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

int setGpioInterruptHandler0(uint32 gpioId,enum GPIO_IRQ_TEST irq_type)
{
	char dataBuf[16] = {0};
	char cmdBuf[128] = {0};
	snprintf(cmdBuf,128,"echo %08x > /proc/gpioNum",gpioId);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

	snprintf(cmdBuf,128,"echo %x%x%x > /proc/gpioAction",RTL_GPIO_SET_ACTION,RTL_GPIO_SET_IRQHANDLER,irq_type);
	if(gpioCmdRet(cmdBuf) == FAILED)
		return FAILED;

}

#endif
