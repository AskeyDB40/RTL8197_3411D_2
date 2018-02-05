/*
 * Copyright 2008, Realtek Semiconductor Corp.
 *
 * Tony Wu (tonywu@realtek.com)
 * Dec. 07, 2008
 */
#include <linux/clockchips.h>
#include <linux/init.h>
#include <linux/interrupt.h>

#include <asm/time.h>

#if defined(CONFIG_RTL_WTDOG)
int is_fault=0; // kernel fault flag

static void rlx_watchdog_trigger_check(void)
{
#if defined(CONFIG_RTL_WTDOG)
	if (!is_fault){
	#ifdef CONFIG_RTL_8198B
		REG32(BSP_WDTCNTRR) |= BSP_WDT_KICK;
	#else
		#ifdef CONFIG_RTK_VOIP
			#if 0//ndef CONFIG_RTL_WTDOG_SOFTIRQ_KICK
			extern int bBspWatchdog;
			*(volatile unsigned long *)(0xB800311c) |= 
			                        ( bBspWatchdog ? ( 1 << 23 ) : ( ( 1 << 23 ) | ( 0xA5 << 24 ) ) );
			#endif
		#else
			#if 0// ndef CONFIG_RTL_WTDOG_SOFTIRQ_KICK
				*(volatile unsigned long *)(0xB800311c) |=  1 << 23;
			#endif
		#endif
	#endif
	}else {
	#ifdef CONFIG_RTK_VOIP
		// run gdb cause Break instruction exception and call do_bp()
		extern int bBspWatchdog;

		if( !bBspWatchdog )
		        is_fault = 0;
		else
	#endif
		{
	// quick fix for warn reboot fail issue
	#if defined(CONFIG_RTL8192SE) || defined(CONFIG_RTL8192CD)
		#if !defined(CONFIG_RTL865X_PANAHOST) && !defined(CONFIG_RTL8197B_PANA)
			extern void force_stop_wlan_hw(void);
			force_stop_wlan_hw();
		#endif
	#endif
			local_irq_disable();
	#ifdef CONFIG_RTL_8198B
			REG32(BSP_WDTCTRLR) = BSP_WDT_ENABLE;
	#else
			*(volatile unsigned long *)(0xB800311c)=0; /*this is to enable 865xc watch dog reset*/
	#endif
			for(;;);
		}
	}
#endif
}

#endif



int ext_timer_state(void)
{
	return 0;
}

int ext_timer_set_base_clock(unsigned int hz)
{
	return 0;
}

static int ext_timer_set_next_event(unsigned long delta,
				 struct clock_event_device *evt)
{
	return -EINVAL;
}

static void ext_timer_set_mode(enum clock_event_mode mode,
			    struct clock_event_device *evt)
{
	return;
}

static void ext_timer_event_handler(struct clock_event_device *dev)
{
}

static struct clock_event_device ext_clockevent = {
	.name		= "EXT",
	.features	= CLOCK_EVT_FEAT_PERIODIC,
	.set_next_event	= ext_timer_set_next_event,
	.set_mode	= ext_timer_set_mode,
	.event_handler	= ext_timer_event_handler,
};

static irqreturn_t ext_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *cd = &ext_clockevent;
	extern void bsp_timer_ack(void);
#if defined(CONFIG_RTL_WTDOG)
	/* rlx watchdog */
	rlx_watchdog_trigger_check();
#endif
	
	/* Ack the RTC interrupt. */
	bsp_timer_ack();

	cd->event_handler(cd);
	return IRQ_HANDLED;
}

static struct irqaction ext_irqaction = {
	.handler	= ext_timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_PERCPU | IRQF_TIMER,
	.name		= "EXT",
};

int __cpuinit ext_clockevent_init(int irq)
{
	struct clock_event_device *cd;

	cd = &ext_clockevent;
	cd->rating = 100;
	cd->irq = irq;
	clockevent_set_clock(cd, 32768);
	cd->max_delta_ns = clockevent_delta2ns(0x7fffffff, cd);
	cd->min_delta_ns = clockevent_delta2ns(0x300, cd);
	cd->cpumask = cpumask_of(0);

	clockevents_register_device(&ext_clockevent);

	return setup_irq(irq, &ext_irqaction);
}
