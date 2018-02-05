/*
 * MIPS idle loop and WAIT instruction support.
 *
 * Copyright (C) xxxx  the Anonymous
 * Copyright (C) 1994 - 2006 Ralf Baechle
 * Copyright (C) 2003, 2004  Maciej W. Rozycki
 * Copyright (C) 2001, 2004, 2011, 2012	 MIPS Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/export.h>
#include <linux/init.h>
#include <linux/irqflags.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <asm/cpu.h>
#include <asm/cpu-info.h>
#include <asm/idle.h>
#include <asm/rlxregs.h>

void arch_cpu_idle(void)
{

//port linux 2.6.30 decision algo to here
/*
 * 08-12-2008, due to the WLAN performance is no difference in 10/100 and giga board when gCpuCanSuspend=1
 * (that means CPU always can suspend), the following code is disabled.
 * ==> only apply to RTL865X, it still need to check the WLAN throughput in RTL8196B
 */
#if 1

#if defined(CONFIG_RTL_8196C) || defined(CONFIG_RTL_819XD) || defined(CONFIG_RTL_8196E)

#ifdef CONFIG_CPU_HAS_SLEEP
	extern int cpu_can_suspend, cpu_can_suspend_check_init;
	extern void suspend_check_interrupt_init(void);

	if (cpu_can_suspend_check_init) {
		if (!cpu_can_suspend)
                    cpu_no_wait();
             else
                    cpu_wait(); /* We stop the CPU to conserve power */
	}
	else {
		suspend_check_interrupt_init();
		cpu_can_suspend_check_init = 1;
	}
#else
        /* We stop the CPU to conserve power */
        cpu_wait();
#endif

#else
        /* We stop the CPU to conserve power */
	cpu_wait();
#endif

#endif


}
