/*
 * Realtek Semiconductor Corp.
 *
 * bsp/irq.c
 *   bsp interrupt initialization and handler code
 *
 * Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
 */
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/timex.h>
#include <linux/random.h>
#include <linux/irq.h>
//#include <linux/version.h> //mark_bb

#include <asm/bitops.h>
#include <asm/bootinfo.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/irq_cpu.h>
#include <asm/irq_vec.h>
//#include <asm/system.h>
#include <linux/proc_fs.h> 
#include <linux/seq_file.h> 
#include <asm/rlxregs.h>

#include "bspchip.h"

#if defined(__KERNEL__ ) && defined(CONFIG_RTL_8196E)
#include <generated/uapi/linux/version.h>
#endif

irqreturn_t bsp_ictl_irq_dispatch(int, void *);

static struct irqaction irq_cascade = { 
	.handler = bsp_ictl_irq_dispatch,
	.name = "cascade",
};

#if defined(CONFIG_RTL_8196E) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)) // modified by lynn_pu, 2014-10-16
static void bsp_ictl_irq_mask(struct irq_data *d)
{
	REG32(BSP_GIMR) &= ~(1 << (d->irq - BSP_IRQ_ICTL_BASE));
}

static void bsp_ictl_irq_unmask(struct irq_data *d)
{
	REG32(BSP_GIMR) |= (1 << (d->irq - BSP_IRQ_ICTL_BASE));
}

static struct irq_chip bsp_ictl_irq = {
	.name = "Sheipa ICTL",
	.irq_ack = bsp_ictl_irq_mask,
	.irq_mask = bsp_ictl_irq_mask,
	.irq_unmask = bsp_ictl_irq_unmask,
};
#else
static void bsp_ictl_irq_mask(unsigned int irq)
{
	REG32(BSP_GIMR) &= ~(1 << (irq - BSP_IRQ_ICTL_BASE));
}

static void bsp_ictl_irq_unmask(unsigned int irq)
{
	REG32(BSP_GIMR) |= (1 << (irq - BSP_IRQ_ICTL_BASE));
}

static struct irq_chip bsp_ictl_irq = {
	.name = "ICTL",
	.ack = bsp_ictl_irq_mask,
	.mask = bsp_ictl_irq_mask,
	.unmask = bsp_ictl_irq_unmask,
};
#endif

irqreturn_t bsp_ictl_irq_dispatch(int cpl, void *dev_id)
{
	unsigned int pending;

	pending = REG32(BSP_GIMR) & REG32(BSP_GISR) & BSP_IRQ_ICTL_MASK;

	//if (pending & BSP_UART0_IP)
	//	do_IRQ(BSP_UART0_IRQ); //for test
	//else if (pending & BSP_TC1_IP)
	//    do_IRQ(BSP_TC1_IRQ);
	//else {
		printk("Spurious Interrupt2:0x%x\n",pending);
		spurious_interrupt();
	//}
	return IRQ_HANDLED;
}

void bsp_irq_dispatch(void)
{
	unsigned int pending;

	pending = read_c0_cause() & read_c0_status() & ST0_IM;

	if (pending & CAUSEF_IP0)
		do_IRQ(0);
	else if (pending & CAUSEF_IP1)
		do_IRQ(1);
	else {
		printk("Spurious Interrupt:0x%x\n",pending);
		spurious_interrupt();
	}
}

static void __init bsp_ictl_irq_init(unsigned int irq_base)
{
	int i;

#if defined(CONFIG_RTL_8196E) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)) // modified by lynn_pu, 2014-10-16
	for (i=0; i < BSP_IRQ_ICTL_NUM; i++)
		irq_set_chip_and_handler(irq_base + i, &bsp_ictl_irq, handle_level_irq);
#else
	for (i=0; i < BSP_IRQ_ICTL_NUM; i++)
		set_irq_chip_and_handler(irq_base + i, &bsp_ictl_irq, handle_level_irq);
#endif

	//enable cascade
	setup_irq(BSP_ICTL_IRQ, &irq_cascade);
}

void __init bsp_irq_init(void)
{
	/* disable ict interrupt */
	REG32(BSP_GIMR) = 0;

	/* initialize IRQ action handlers */
	rlx_cpu_irq_init(BSP_IRQ_CPU_BASE);
	rlx_vec_irq_init(BSP_IRQ_LOPI_BASE);
	bsp_ictl_irq_init(BSP_IRQ_ICTL_BASE);

	/* Set IRR */
	REG32(BSP_IRR0) = BSP_IRR0_SETTING;
	REG32(BSP_IRR1) = BSP_IRR1_SETTING;
	REG32(BSP_IRR2) = BSP_IRR2_SETTING;
	REG32(BSP_IRR3) = BSP_IRR3_SETTING;  

	/* enable global interrupt mask */
	REG32(BSP_GIMR) = BSP_TC0_IE | BSP_UART0_IE;

#if defined(CONFIG_RTL8192CD) || defined(CONFIG_PCI) || defined(CONFIG_WLAN)
	REG32(BSP_GIMR) |= BSP_PCIE_IE;
#endif
#ifdef CONFIG_USB
	REG32(BSP_GIMR) |= BSP_USB_H_IE;
#endif
#ifdef CONFIG_DWC_OTG  //wei add
	REG32(BSP_GIMR) |= BSP_OTG_IE;  //mac
#endif
#ifdef CONFIG_RTL_819X_SWCORE
	REG32(BSP_GIMR) |= (BSP_SW_IE);
#endif
	//printk("BSP_IRQ_ICTL_MASK=0x%X\n", BSP_IRQ_ICTL_MASK);
}
#if defined(CONFIG_ARCH_SUSPEND_POSSIBLE)//michaelxxx 
   #define CONFIG_RTL819X_SUSPEND_CHECK_INTERRUPT 
    
   #ifdef CONFIG_RTL819X_SUSPEND_CHECK_INTERRUPT 
   #include <linux/proc_fs.h> 
   #include <linux/kernel_stat.h> 
   #include <asm/uaccess.h> 
   //#define INT_HIGH_WATER_MARK 1850 //for window size = 1, based on LAN->WAN test result 
   //#define INT_LOW_WATER_MARK  1150 
   //#define INT_HIGH_WATER_MARK 9190 //for window size = 5, based on LAN->WAN test result 
   //#define INT_LOW_WATER_MARK  5500 
   #define INT_HIGH_WATER_MARK 3200  //for window size = 5, based on WLAN->WAN test result 
   #define INT_LOW_WATER_MARK  2200 
   #define INT_WINDOW_SIZE_MAX 10 
   static int suspend_check_enable = 1; 
   static int suspend_check_high_water_mark = INT_HIGH_WATER_MARK; 
   static int suspend_check_low_water_mark = INT_LOW_WATER_MARK; 
   static int suspend_check_win_size = 5; 
   static struct timer_list suspend_check_timer; 
   static int index=0, prev_count = 0; 
   static int eth_int_count[INT_WINDOW_SIZE_MAX]; 
   static int wlan_int_count[INT_WINDOW_SIZE_MAX]; 
   int cpu_can_suspend = 1; 
   int cpu_can_suspend_check_init = 0; 

   static int read_proc_suspend_check(struct seq_file *s, void *v)
    
   { 
        seq_printf(s, "enable=%d, winsize=%d(%d), high=%d, low=%d, suspend=%d, prev_count= %d\n", 
                   suspend_check_enable, suspend_check_win_size, INT_WINDOW_SIZE_MAX, 
                   suspend_check_high_water_mark, suspend_check_low_water_mark, cpu_can_suspend, prev_count); 
    
        return 0;
   } 
    
   static int write_proc_suspend_check(struct file *file, const char *buffer, 
                 unsigned long count, void *data) 
   { 
           char tmp[128]; 
    
           if (buffer && !copy_from_user(tmp, buffer, 128)) { 
                   sscanf(tmp, "%d %d %d %d %d", 
                           &suspend_check_enable, &suspend_check_win_size, 
                           &suspend_check_high_water_mark, &suspend_check_low_water_mark, &cpu_can_suspend); 
                   if (suspend_check_win_size >= INT_WINDOW_SIZE_MAX) 
                           suspend_check_win_size = INT_WINDOW_SIZE_MAX - 1; 
                   if (suspend_check_enable) { 
                           mod_timer(&suspend_check_timer, jiffies + 100); 
                   } 
                   else { 
                           del_timer(&suspend_check_timer); 
                   } 
           } 
           return count; 
   } 
    
   static void suspend_check_timer_fn(unsigned long arg) 
   { 
           int count, j; 
    
           index++; 
           if (INT_WINDOW_SIZE_MAX <= index) 
                   index = 0; 
           eth_int_count[index] = kstat_irqs(BSP_SWCORE_IRQ); 
           wlan_int_count[index] = kstat_irqs(BSP_PCIE_IRQ); 
           j = index - suspend_check_win_size; 
           if (j < 0) 
                   j += INT_WINDOW_SIZE_MAX; 
           count = (eth_int_count[index] - eth_int_count[j]) + 
                   (wlan_int_count[index]- wlan_int_count[j]); //unit: number of interrupt occurred 

           prev_count = count;

           if (cpu_can_suspend) { 
                   if (count > suspend_check_high_water_mark) { 
                           cpu_can_suspend = 0; 
                           //printk("\n<<<RTL8196C LEAVE SLEEP>>>\n"); /* for Debug Only*/ 
                   } 
           } 
           else { 
                   if (count < suspend_check_low_water_mark) { 
                           cpu_can_suspend = 1; 
                           //printk("\n<<<RTL8196C ENTER SLEEP>>>\n"); /* for Debug Only*/ 
                   } 
           } 
   #if 0 /* for Debug Only*/ 
           printk("###index=%d, count=%d (%d+%d) suspend=%d###\n",index, count, 
                   (eth_int_count[index] - eth_int_count[j]), 
                   (wlan_int_count[index]- wlan_int_count[j]), 
                   cpu_can_suspend); 
   #endif 
           mod_timer(&suspend_check_timer, jiffies + 100); 
   } 
#if 1 //mark_bb    

extern struct proc_dir_entry proc_root;

int suspend_single_open(struct inode *inode, struct file *file)
{      
        return(single_open(file, read_proc_suspend_check,NULL));
}

static ssize_t suspend_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return write_proc_suspend_check(file, userbuf,count, off);
}

  struct file_operations suspendcheck_proc_fops = {
                .open           = suspend_single_open,
                .write		    = suspend_single_write,
                .read           = seq_read,
                .llseek         = seq_lseek,
                .release = single_release,
           };
   void suspend_check_interrupt_init(void) 
   { 
           struct proc_dir_entry *res = NULL; 
           int i; 
    
         res = proc_create_data("suspend_check", 0, &proc_root,&suspendcheck_proc_fops, NULL);
         
	if (res == NULL) 
		printk("can't create proc entry for suspend check");
    
           for (i=0; i<INT_WINDOW_SIZE_MAX; i++) { 
                   wlan_int_count[i] = 0; 
                   eth_int_count[i] = 0; 
           } 
           init_timer(&suspend_check_timer); 
           suspend_check_timer.data = 0; 
           suspend_check_timer.function = suspend_check_timer_fn; 
           suspend_check_timer.expires = jiffies + 100; /* in jiffies */ 
           add_timer(&suspend_check_timer); 
   	}
#endif
   #endif // CONFIG_RTL819X_SUSPEND_CHECK_INTERRUPT 
#endif //CONFIG_ARCH_SUSPEND_POSSIBLE 

