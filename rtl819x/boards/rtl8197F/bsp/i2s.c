/*
 * Realtek Semiconductor Corp.
 *
 * bsp/i2s.c
 *
 *  Copyright (C) 2016 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>

#include "bspchip.h"
#include "i2s.h"

static struct platform_device rtl819x_i2s_device = {
	.name			= "rtl819x-iis",
	.id=-1,
};

static struct platform_device rtl819x_i2c_device = {
	.name			= "rt5621",
	.id=-1,
};

static int __init rtl819x_i2s_setup(void)
{
	/*register platform device*/
	platform_device_register(&rtl819x_i2s_device);
	platform_device_register(&rtl819x_i2c_device);

	return 0;
}

arch_initcall(rtl819x_i2s_setup);
