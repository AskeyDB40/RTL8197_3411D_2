#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <asm/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/reboot.h>
#include <linux/kmod.h>
#include <linux/proc_fs.h>
#include  "bspchip.h"
#include <linux/seq_file.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif
#include "rtl_gpiocommon.h"
#include "rtl_gpioMuxTable.h"

static unsigned int gpioNum = 0,gpioVal = 0;
static unsigned char gpioAction[8] = {0};
extern struct proc_dir_entry proc_root;

/* gpioNum proc */
static int gpioNum_write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	char tmp[9] = {0};
	if (count < 9)
		return -EFAULT;
	if (buffer && !copy_from_user(tmp, buffer, 8)) {
		gpioNum = simple_strtol(tmp,tmp+8,16);
		return count;
	}
	return -EFAULT;
}

static ssize_t gpioNum_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return gpioNum_write_proc(file, userbuf,count, off);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
static int gpioNum_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
#else
static int gpioNum_read_proc(struct seq_file *s, void *v)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
	int len;

	len = sprintf(page, "%x\n", gpioNum);
	if (len <= off+count) *eof = 1;
	  *start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	  return len;
#else
	seq_printf(s, "%x\n", gpioNum);
	return 0;
#endif
}

int gpioNum_single_open(struct inode *inode, struct file *file)
{
        return(single_open(file, gpioNum_read_proc, NULL));
}

struct file_operations gpioNum_proc_fops = {
        .open           = gpioNum_single_open,
		.write          = gpioNum_single_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};


/* gpioAction proc */
static int gpioAction_write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	uint32 gpioId;
	unsigned char action,gpioFlag,gpioValue;
	
	
	if (count < 3)
		return -EINVAL;
	
	if (buffer && !copy_from_user(gpioAction, buffer, (count-1))) {
		action = gpioAction[0] - '0';
		gpioFlag = gpioAction[1] - '0';
		gpioValue = gpioAction[2] - '0';
		
		switch(action){
			case RTL_GPIO_SET_ACTION:
				/* set */
				switch(gpioFlag){
					case RTL_GPIO_MUX:
						/* set pin mux */
						if(setPinMux(PIN_GPIO_MODE,gpioNum) == FAILED)
							return -EINVAL;
					break;
					case RTL_GPIO_CONTROL:
						/* set control reg */
						setGpioControlBit(gpioNum,GPIO_GPIO_PIN);
					break;
					case RTL_GPIO_DIR:
						/* set dir reg */
						if(gpioValue < GPIO_DIR_IN 
							|| gpioValue > GPIO_DIR_OUT){
							return -EINVAL;
						}
						setGpioDirBit(gpioNum,gpioValue);
					break;
					case RTL_GPIO_DAT:
						/* set data reg */
						if(gpioValue < GPIO_DATA_HIGH 
							|| gpioValue > GPIO_DATA_LOW)
							return -EINVAL;
						setGpioDataBit(gpioNum,gpioValue);
					break;
					case RTL_GPIO_INTERRUPT_ENABLE:
						if(gpioValue < GPIO_INT_DISABLE 
							|| gpioValue > GPIO_INT_FALLING_EDGE_ENABLE)
							return -EINVAL;
						setGpioInterruptBit(gpioNum,gpioValue);
					break;
					case RTL_GPIO_INTERRUPT_STATUS:
						if(gpioValue < GPIO_INT_DO_NOTHING
							|| gpioValue > GPIO_INT_CLEAN)
							return -EINVAL;
						setGpioInterruptStatusBit(gpioNum,gpioValue);
					break;
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
					case RTL_GPIO_MUX_ALL:
						setPinMuxInitalVal();
					break;
					case RTL_GPIO_SET_IRQHANDLER:
						setGpioInterruptHandlerUser(gpioNum,gpioValue);
					break;
#endif
					default:
						printk("set gpioAction bit1=%d not support\n",gpioFlag);
						return -EINVAL;
				}
			break;
			case RTL_GPIO_GET_ACTION:
				/* get */
				switch(gpioFlag){
					case RTL_GPIO_MUX:
						/* get pin mux */
						getPinMux(PIN_GPIO_MODE,gpioNum,&gpioVal);
					break;
					case RTL_GPIO_CONTROL:
						/* get control reg */
						getGpioControlBit(gpioNum,&gpioVal);
					break;
					case RTL_GPIO_DIR:
						/* get dir reg */
						getGpioDirBit(gpioNum,&gpioVal);
					break;
					case RTL_GPIO_DAT:
						/* get data reg */
						getGpioDataBit(gpioNum,&gpioVal);
					break;
					case RTL_GPIO_INTERRUPT_ENABLE:
						/* get interrupt enable reg */
						getGpioInterruptBit(gpioNum,&gpioVal);
					break;
					case RTL_GPIO_INTERRUPT_STATUS:
						getGpioInterruptStatusBit(gpioNum,&gpioVal);
					break;
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
					case RTL_GPIO_MUX_ALL:
						getPinMuxInitalVal();
					break;
#endif
					default:
						printk("get gpioAction bit1=%c not support\n",gpioFlag);
						return -EINVAL;
				}
				
			break;
			default:
				printk("gpioAction bit0=%c not support\n",action);
				return -EINVAL;
		}
		return count;
	}
	return -EFAULT;
}

static ssize_t gpioAction_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return gpioAction_write_proc(file, userbuf,count, off);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
static int gpioAction_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
#else
static int gpioAction_read_proc(struct seq_file *s, void *v)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
	int len;

	len = sprintf(page, "%u\n", gpioVal);
	if (len <= off+count) *eof = 1;
	  *start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	  return len;
#else
	seq_printf(s, "%u\n", gpioVal);
	return 0;
#endif
}

int gpioAction_single_open(struct inode *inode, struct file *file)
{
        return(single_open(file, gpioAction_read_proc, NULL));
}

struct file_operations gpioAction_proc_fops = {
        .open           = gpioAction_single_open,
		.write          = gpioAction_single_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};


#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
static unsigned char gpioTest;

static int gpioTest_write_proc(struct file *file, const char *buffer,
				unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(&gpioTest, buffer, 1)) {
		switch(gpioTest){
		case '1':
			printk("%s:%d:start gpio normal test\n",__func__,__LINE__);
			if(rtl_gpio_test() < 0)
				printk("%s:%d:gpio normal test fail\n",__func__,__LINE__);
			else
				printk("%s:%d:gpio normal test success\n",__func__,__LINE__);
			break;
		case '2':
			printk("%s:%d:start gpio interrupt test\n",__func__,__LINE__);
			if(rtl_gpio_interrrupt_test() < 0)
				printk("%s:%d:gpio interrupt test fail\n",__func__,__LINE__);
			else
				printk("%s:%d:gpio interrupt test success\n",__func__,__LINE__);
			break;
			break;
		default:
			printk("%s:%d:cmd option not support\n",__func__,__LINE__);
		}
		return count;
	}
	return -EFAULT;
}

static ssize_t gpioTest_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return gpioTest_write_proc(file, userbuf,count, off);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
static int gpioTest_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
#else
static int gpioTest_read_proc(struct seq_file *s, void *v)
#endif
{
	return 0;
}

int gpioTest_single_open(struct inode *inode, struct file *file)
{
        return(single_open(file, gpioTest_read_proc, NULL));
}

struct file_operations gpioTest_proc_fops = {
        .open           = gpioTest_single_open,
		.write          = gpioTest_single_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif


static int __init rtl_gpio_init(void)
{
	printk("load Realtek GPIO Driver \n");

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
	struct proc_dir_entry *res=NULL;
	res = create_proc_entry("gpioNum", 0, NULL);
	if (res) {
		res->write_proc = gpioNum_write_proc;
		res->read_proc = gpioNum_read_proc;
	}
	else
		printk("create gpioNum failed!\n");

	res = create_proc_entry("gpioAction", 0, NULL);
	if (res) {
		res->write_proc = gpioAction_write_proc;
		res->read_proc = gpioAction_read_proc;
	}
	else
		printk("create gpioAction failed!\n");
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
	res = create_proc_entry("gpioTest", 0, NULL);
	if (res) {
		res->write_proc = gpioTest_write_proc;
		res->read_proc = gpioTest_read_proc;
	}
	else
		printk("create gpioTest failed!\n");
#endif
#else
	proc_create_data("gpioNum", 0, &proc_root,
			 &gpioNum_proc_fops, NULL);
	proc_create_data("gpioAction", 0, &proc_root,
			 &gpioAction_proc_fops, NULL);
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
	proc_create_data("gpioTest", 0, &proc_root,
			 &gpioTest_proc_fops, NULL);
#endif
#endif

	return 0;
}

static void __exit rtl_gpio_exit(void)
{
	printk("Unload Realtek GPIO Driver \n");
}

module_exit(rtl_gpio_exit);
module_init(rtl_gpio_init);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver");
