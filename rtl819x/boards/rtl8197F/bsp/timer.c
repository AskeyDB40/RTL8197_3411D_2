/*
 * Realtek Semiconductor Corp.
 *
 * bsp/timer.c
 *     bsp timer initialization code
 *
 * Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/timex.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>

#include <asm/time.h>

#include "bspchip.h"

const unsigned int cpu_clksel_table[]={ 450, 500, 550, 600, 650,700,
										750, 800, 850, 900, 950, 1000,										
										1050, 1100, 1150, 1200 };

const unsigned int cpu_clkdiv_table[]={1, 2, 4, 8};

#ifdef CONFIG_PROC_FS
extern struct proc_dir_entry proc_root;
struct file_operations watchdog_reboot_proc_fops;
struct file_operations watchdog_cmd_proc_fops;
struct file_operations watchdog_kick_proc_fops;
#endif

#ifdef CONFIG_CEVT_EXT
void inline bsp_timer_ack(void)
{
	unsigned volatile int eoi;
	eoi = REG32(BSP_TIMER0_EOI);
}

void __init bsp_timer_init(void)
{
	change_c0_cause(CAUSEF_DC, 0);

	/* disable timer */
	REG32(BSP_TIMER0_TCR) = 0x00000000;

	/* initialize timer registers */
	REG32(BSP_TIMER0_TLCR) = BSP_TIMER0_FREQ / HZ;

	/* hook up timer interrupt handler */
	ext_clockevent_init(BSP_TIMER0_IRQ);

	/* enable timer */
	REG32(BSP_TIMER0_TCR) = 0x00000003;       /* 0000-0000-0000-0011 */
}
#endif /* CONFIG_CEVT_EXT */

#ifdef CONFIG_RTL_WTDOG
int bBspWatchdog = 0;

void bsp_enable_watchdog( void )
{
	bBspWatchdog = 1;
	*(volatile unsigned long *)(0xb800311C)=0x00240000; // 2^24
}

void bsp_disable_watchdog( void )
{
	*(volatile unsigned long *)(0xb800311C)=0xA5240000;
	bBspWatchdog = 0;
}
#endif

#ifdef CONFIG_RTL_EXT_TC0
void inline bsp_ext_timer_ack(void)
{
    REG32(BSP_TCIR) |= BSP_TC0IP;
}

#ifdef CONFIG_RTL_NIC_QUEUE	
extern void rtl865x_tbf_add_token(unsigned int token);
#endif

static irqreturn_t ext_timer_interrupt(int irq, void *dev_id)
{
    bsp_ext_timer_ack();
#ifdef CONFIG_RTL_NIC_QUEUE	
	rtl865x_tbf_add_token(6250);
#endif
    return IRQ_HANDLED;
}

static struct irqaction ext_irqaction = {
    .handler    = ext_timer_interrupt,
    .flags      = IRQF_DISABLED | IRQF_PERCPU | IRQF_TIMER,
    .name       = "EXT",
};


void bsp_tc0_init(void)
{
	REG32(BSP_TCCNR) |= (BSP_TC0EN | BSP_TC0MODE_TIMER);
	REG32(BSP_TC0DATA) = (((200000000) / BSP_DIVISOR) / 2000) << BSP_TCD_OFFSET;

	setup_irq(BSP_TC0_IRQ, &ext_irqaction);

	/*enable tc0 irq*/
	REG32(BSP_TCIR) |= BSP_TC0IE;
}
#endif
#ifdef CONFIG_CEVT_R4K
unsigned int __cpuinit get_c0_compare_int(void)
{
	return BSP_COMPARE_IRQ;
}

//void __init bsp_timer_init(void)
void __init plat_time_init(void) // mips-ori
{
	unsigned int freq, div, hwstrap;
	
	/* set cp0_compare_irq and cp0_perfcount_irq */
	cp0_compare_irq = BSP_COMPARE_IRQ;
	cp0_perfcount_irq = BSP_PERFCOUNT_IRQ;

	if (cp0_perfcount_irq == cp0_compare_irq)
		cp0_perfcount_irq = -1;

	//mips_hpt_frequency = BSP_CPU0_FREQ / 2;
	hwstrap = REG32(BSP_HW_STRAP);
	if ((hwstrap >> 14) & 0x1) {
		freq = 200 * 1000000;
	}
	else {
		div = (hwstrap >> 19) & 0x3;
		freq = (hwstrap >> 15) & 0xF;
		freq = cpu_clksel_table[freq] * 1000000 / cpu_clkdiv_table[div];
	}
	mips_hpt_frequency = freq / 2;

	//write_c0_count(0);
	//mips_clockevent_init(cp0_compare_irq); // mips-ori

	if (-1 != cp0_perfcount_irq)
		printk("TBD if not shared with timer\n");

#ifdef CONFIG_RTL_WTDOG
	REG32(BSP_CDBR)=(BSP_DIVISOR) << BSP_DIVF_OFFSET;
	bsp_enable_watchdog();
#endif /* CONFIG_RTL_WTDOG */

#ifdef CONFIG_RTL_EXT_TC0
	bsp_tc0_init();
#endif /*CONFIG_RTL_TC0*/
}
#endif /* CONFIG_CEVT_R4K */

#ifdef CONFIG_PROC_FS
#ifdef CONFIG_RTL_USERSPACE_WTDOG
#define RTL_WATCHDOG_KICK	0x01

static int watchdog_kick_state = 0;
static int watchdog_default_flag = 0;
static int watchdog_default_val;

#define LXBUS_CLOCK		(200000000)

typedef struct wtdog_regtbl
{
	unsigned char oversel_h;
	unsigned char oversel_l;
	unsigned int wtdog_val;
}WTDOG_REGTBL_T,*WTDOG_REGTBL_Tp;

static WTDOG_REGTBL_T wtdog_tbl[] = {
	{0x00,0x00,0x8000},
	{0x00,0x01,0x10000},
	{0x00,0x02,0x20000},
	{0x00,0x03,0x40000},
	{0x01,0x00,0x80000},
	{0x01,0x01,0x100000},
	{0x01,0x02,0x200000},
	{0x01,0x03,0x400000},
	{0x02,0x00,0x800000},
	{0x02,0x01,0x1000000},
};

/* watchdog start */
static int read_watchdog_cmd_proc(struct seq_file *s, void *v)
{
	int i;
	unsigned int wtdog_enable,wtdog_intevl,wtdog_cdbr,wtdog_maxtime;
	unsigned int higl_oversel,low_oversel;

	wtdog_enable = ((REG32(BSP_WDTCNR) >> 24) & 0xA5) ==  0xA5 ? 0 : 1;

	wtdog_cdbr = (REG32(BSP_CDBR) >> 16) & 0xffff;
	higl_oversel = (REG32(BSP_WDTCNR) >> 17) & 0x3;
	low_oversel = (REG32(BSP_WDTCNR) >> 21) & 0x3;

	for(i = 0;i < sizeof(wtdog_tbl)/sizeof(WTDOG_REGTBL_T);i++){
		if(wtdog_tbl[i].oversel_h == higl_oversel
			&& wtdog_tbl[i].oversel_l == low_oversel){
			wtdog_intevl = wtdog_tbl[i].wtdog_val/(LXBUS_CLOCK/wtdog_cdbr);
			break;
		}
	}
	if(i == sizeof(wtdog_tbl)/sizeof(WTDOG_REGTBL_T)){
		seq_printf(s,"watchdog register not correct,please check\n");
		return 0;
	}

	i = sizeof(wtdog_tbl)/sizeof(WTDOG_REGTBL_T) -1;
	wtdog_maxtime = wtdog_tbl[i].wtdog_val/(LXBUS_CLOCK/wtdog_cdbr);
	
	if(wtdog_enable == 0)
		seq_printf(s,"watchdog disabled,default watchdog_time=%ds,max watchdog time=%ds\n",watchdog_default_val,wtdog_maxtime);
	else
		seq_printf(s,"watchdog enable,intervel:%dsec,default watchdog_time=%ds,max watchdog time=%ds\n", wtdog_intevl,watchdog_default_val,wtdog_maxtime);

	return 0;
}

int watchdog_cmd_open(struct inode *inode, struct file *file)
{
        return(single_open(file, read_watchdog_cmd_proc, NULL));
}

static ssize_t watchdog_cmd_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	char flag[64];
	int enable,interval;

	extern void bsp_enable_watchdog(void);
	extern void bsp_disable_watchdog(void);
	
	if (count < 2)
		return -EFAULT;
	if (userbuf && !copy_from_user(&flag, userbuf, 63)) {
		int i;
		unsigned int wtdog_intervel,wtdog_intervel0 = 0,wtdog_cdbr,wtdog_maxtime;
		sscanf(flag,"enable %d interval %d",&enable,&interval);

		if(enable == 0){
			/* disable watchdog */
			bsp_disable_watchdog();
		}else if(enable == 1){
			if(watchdog_default_flag == 0){
				watchdog_default_flag = 1;
				watchdog_default_val = interval;
			}else{
				if(interval < watchdog_default_val){
					printk("\t\nwatchdog timeout time should not less than default val,default=%d\n",watchdog_default_val);
					return -1;
				}
			}

			wtdog_cdbr = (REG32(BSP_CDBR) >> 16) & 0xffff;
			i = sizeof(wtdog_tbl)/sizeof(WTDOG_REGTBL_T) -1;
			wtdog_maxtime = wtdog_tbl[i].wtdog_val/(LXBUS_CLOCK/wtdog_cdbr);

			if(interval > wtdog_maxtime){
				printk("\t\n watchdog max intervale time is %d,please check the set value\n",  wtdog_maxtime);
				return -1;
			}

			for(i = 0;i < sizeof(wtdog_tbl)/sizeof(WTDOG_REGTBL_T);i++){
				wtdog_intervel = wtdog_tbl[i].wtdog_val/(LXBUS_CLOCK/wtdog_cdbr);

				if(interval >= wtdog_intervel0
					&& interval <= wtdog_intervel)
					goto END;
				wtdog_intervel0 = wtdog_intervel;
			}
			
END:
			REG32(BSP_WDTCNR) = ( wtdog_tbl[i].oversel_l << 21) | ( wtdog_tbl[i].oversel_h << 17) ;
		}
		return count;
	}
	
	return -EFAULT;	
}

struct file_operations watchdog_cmd_proc_fops = {
        .open           = watchdog_cmd_open,
        .write         = watchdog_cmd_single_write,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release        = single_release,
};

/* watchdog kick */
static int read_watchdog_kick_proc(struct seq_file *s, void *v)
{
	char flag = '0';

	if(watchdog_kick_state == RTL_WATCHDOG_KICK)
		flag = '1';

	seq_printf(s,"%c\n", flag);

	return 0;
}

int watchdog_kick_open(struct inode *inode, struct file *file)
{
        return(single_open(file, read_watchdog_kick_proc, NULL));
}

static ssize_t watchdog_kick_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	char flag[20];

	if (count < 2)
		return -EFAULT;
#ifdef CONFIG_RTL_WTDOG
	{ 
		/*If kernel fault. reboot whole system so softwatch dog can not kick even*/
		extern int is_fault;
		if(is_fault)
			return count;
	}
#endif
	if (userbuf && !copy_from_user(&flag, userbuf, 1)) {
		if(flag[0] == '1'){
			watchdog_kick_state = RTL_WATCHDOG_KICK;
			/* kick watchdog here*/
			*(volatile unsigned long *)(0xB800311c) |=  1 << 23;
		}else {
			watchdog_kick_state = 0;
		}
		return count;
	}
	
	return -EFAULT;	
}

struct file_operations watchdog_kick_proc_fops = {
        .open           = watchdog_kick_open,
        .write         = watchdog_kick_single_write,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release        = single_release,
};
#endif /* CONFIG_RTL_USERSPACE_WTDOG */

static int read_proc(struct seq_file *s, void *v)
{
	return 0;	
}

static ssize_t write_watchdog_reboot(struct file *file, const char *buffer,
				size_t count, loff_t *data)
{
	char tmp[16];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 8)) {	
		if (tmp[0] == '1') {
			local_irq_disable();	
			printk("reboot...\n");
			*(volatile unsigned long *)(0xB800311c)=0; /*this is to enable 865xc watch dog reset*/
			for(;;);
		}

		return count;
	}
	return -EFAULT;
}
int watchdog_reboot_open(struct inode *inode, struct file *file)
{
        return(single_open(file, read_proc, NULL));
}

struct file_operations watchdog_reboot_proc_fops = {
        .open           = watchdog_reboot_open,
	 .write           = write_watchdog_reboot,
        .read            = seq_read,
        .llseek          = seq_lseek,
        .release        = single_release,
};

int __init bsp_watchdog_proc_init(void)
{
	proc_create_data("watchdog_reboot", 0, &proc_root,
			&watchdog_reboot_proc_fops, NULL);
#ifdef  CONFIG_RTL_USERSPACE_WTDOG
	proc_create_data("watchdog_cmd", 0, &proc_root,
			&watchdog_cmd_proc_fops, NULL);
			
	proc_create_data("watchdog_kick", 0, &proc_root,
			&watchdog_kick_proc_fops, NULL);
#endif

	return 0;
}
module_init(bsp_watchdog_proc_init);
#endif /* CONFIG_PROC_FS */