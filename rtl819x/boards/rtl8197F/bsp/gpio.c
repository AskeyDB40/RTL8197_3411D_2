/*
 * Realtek Semiconductor Corp.
 *
 * bsp/gpio.c
 *     DesignWare GPIO chip intialization and handlers
 *
 *  Copyright (C) 2015 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/platform_data/rtl819x-gpio.h>
#include <bspchip.h>

#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_8211F_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
/* for 97F with Giga ETH PHY series: V200, V313, V315, V333... */
#define BSP_RESET_BTN_PIN		BSP_GPIO_PIN_G6
#define BSP_WPS_BTN_PIN			BSP_GPIO_PIN_H0
#define BSP_RESET_LED_PIN		BSP_GPIO_PIN_H1
#define BSP_WPS_LED_PIN			BSP_GPIO_PIN_H1
#else
/* for 97F only with 10/100 ETH PHY series: V100, V120, V125, V302... */
#define BSP_RESET_BTN_PIN		BSP_GPIO_PIN_C3

#ifndef CONFIG_I2C_DESIGNWARE_PLATFORM //joyce
#define BSP_WPS_BTN_PIN			BSP_GPIO_PIN_C1
#endif

#define BSP_RESET_LED_PIN		BSP_GPIO_PIN_B7
#define BSP_WPS_LED_PIN			BSP_GPIO_PIN_B7
#endif

#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
#define BSP_8367R_RST_PIN		BSP_GPIO_PIN_H2
#define BSP_8367R_I2C_PIN1		BSP_GPIO_PIN_C2
#define BSP_8367R_I2C_PIN2		BSP_GPIO_PIN_C3
#define BSP_8367R_RST_PIN_97FN		BSP_GPIO_PIN_F1
#define BSP_8367R_I2C_PIN1_97FN		BSP_GPIO_PIN_G6
#define BSP_8367R_I2C_PIN2_97FN		BSP_GPIO_PIN_C3
#endif

static DEFINE_SPINLOCK(rtl819x_gpio_lock);

static unsigned int rtl819x_gpio_mux(u32 pin, u32 *value, u32 *address );
static void rtl819x_gpio_pin_enable(u32 pin);

extern unsigned int rtl819x_bond_option(void);

int rtl819x_gpio_request(struct gpio_chip *chip, unsigned pin)
{
	/* The pins you don't want to share */
	switch(pin) {
		case 16384:
			printk(KERN_ERR "rtl819x_gpio: invalid pin number %d\n", pin);
			return -EINVAL;
			break;
	}
	
	rtl819x_gpio_pin_enable(pin);
	
	return 0;
}

static int rtl819x_gpio_get_value(struct gpio_chip *chip, unsigned pin)
{
	unsigned int data;

	data = (__raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin)) >> BSP_GPIO_BIT(pin) ) & 1;
	
	pr_debug("[%s][%d]: pin%d, value is %d\n", __FUNCTION__, __LINE__, pin, data);
	return data;
}

static void rtl819x_gpio_set_value(struct gpio_chip *chip,
				  unsigned pin, int value)
{
	unsigned int data;

	data = __raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin));

	if (value == 0)
		data &= ~(1 << BSP_GPIO_BIT(pin));
	else
		data |= (1 << BSP_GPIO_BIT(pin));

	//printk("%s(%d)yyy 0x%08x\n",__FUNCTION__,__LINE__,data);

	__raw_writel(data, (void __iomem*)BSP_GPIO_DAT_REG(pin));
	
	pr_debug("[%s][%d]: output %d to pin%d, DAT_REG=0x%x\n"
			, __FUNCTION__, __LINE__, value, pin, __raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin)));
	//printk("%s(%d)xxx 0x%08x\n",__FUNCTION__,__LINE__,
	//	__raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin)));
}

static int rtl819x_gpio_direction_input(struct gpio_chip *chip,
				       unsigned pin)
{
	unsigned long flags;

	if (pin >= BSP_GPIO_PIN_MAX)
		return -EINVAL;
	
	/* these pins are only for output */
	switch(pin) {
	case BSP_GPIO_PIN_A0:
	case BSP_GPIO_PIN_A4:
	case BSP_GPIO_PIN_A6:
	case BSP_GPIO_PIN_A7:
	case BSP_GPIO_PIN_B0:
	case BSP_GPIO_PIN_B6:
	case BSP_GPIO_PIN_C0:
	case BSP_GPIO_PIN_C4:
	case BSP_GPIO_PIN_E5:
	case BSP_GPIO_PIN_F0:
	case BSP_GPIO_PIN_F1:
	case BSP_GPIO_PIN_F2:
	case BSP_GPIO_PIN_F3:
	case BSP_GPIO_PIN_F7:
	case BSP_GPIO_PIN_G2:
	case BSP_GPIO_PIN_G4:
	case BSP_GPIO_PIN_G5:
	case BSP_GPIO_PIN_H4:
	case BSP_GPIO_PIN_H5:
		printk(KERN_ERR "rtl819x_gpio: request rejection, pin%d is only for output\n", pin);
		return -EINVAL;
	}
	
	spin_lock_irqsave(&rtl819x_gpio_lock, flags);

	/* 0 : input */
	__raw_writel(__raw_readl((void __iomem*)BSP_GPIO_DIR_REG(pin)) 
			& ~(1 << BSP_GPIO_BIT(pin)), 
			(void __iomem*)BSP_GPIO_DIR_REG(pin));
	pr_debug("[%s][%d]: set pin%d as input pin, DIR_REG=0x%x\n"
			, __FUNCTION__, __LINE__, pin, __raw_readl((void __iomem*)BSP_GPIO_DIR_REG(pin)));

	spin_unlock_irqrestore(&rtl819x_gpio_lock, flags);
	
	return 0;
}

static int rtl819x_gpio_direction_output(struct gpio_chip *chip,
					unsigned pin, int value)
{
	unsigned long flags, data;

	if (pin >= BSP_GPIO_PIN_MAX)
		return -1;

	spin_lock_irqsave(&rtl819x_gpio_lock, flags);

	__raw_writel(__raw_readl((void __iomem*)BSP_GPIO_DIR_REG(pin)) 
			| (1 << BSP_GPIO_BIT(pin)), 
			(void __iomem*)BSP_GPIO_DIR_REG(pin) );

	data = __raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin));
	if (value == 0)
		data &= ~(1 << BSP_GPIO_BIT(pin));
	else
		data |= (1 << BSP_GPIO_BIT(pin));

	__raw_writel(data, (void __iomem*)BSP_GPIO_DAT_REG(pin));

	pr_debug("[%s][%d]: set pin%d as output pin, default value=%d, DIR_REG=0x%x DAT_REG=0x%x\n"
			, __FUNCTION__, __LINE__, pin, value, 
			__raw_readl((void __iomem*)BSP_GPIO_DIR_REG(pin)), __raw_readl((void __iomem*)BSP_GPIO_DAT_REG(pin)));

	spin_unlock_irqrestore(&rtl819x_gpio_lock, flags);

	return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int rtl819x_gpio_get_direction(struct gpio_chip *chip, unsigned pin)
{	
	return __raw_readl((void __iomem*)BSP_GPIO_DIR_REG(pin)) 
			& 0x1 << BSP_GPIO_BIT(pin) ? 0 : 1;
}
#else
int gpio_to_irq(unsigned gpio)
{
	return __gpio_to_irq(gpio);
}
EXPORT_SYMBOL(gpio_to_irq);
#endif

static int rtl819x_gpio_to_irq(struct gpio_chip *chip, unsigned pin)
{
	pr_debug("rtl819x_gpio: GPIO%d requests IRQ%d\n", pin, BSP_GPIO_TO_IRQ(pin));
		return BSP_GPIO_TO_IRQ(pin);
}

static struct gpio_chip rtl819x_gpio_peripheral = {

	.label				= "rtl819x_gpio",
	.request			= rtl819x_gpio_request,
	.get				= rtl819x_gpio_get_value,
	.set				= rtl819x_gpio_set_value,
	.direction_input	= rtl819x_gpio_direction_input,
	.direction_output	= rtl819x_gpio_direction_output,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	.get_direction		= rtl819x_gpio_get_direction,
#endif
	.to_irq				= rtl819x_gpio_to_irq,
	.base				= 0,
};

/* LED devices */
static struct rtl819x_gpio_platdata rtl_pdata_led1 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "reset led", //=System LED
};

#if !defined(CONFIG_SERIAL_RTL_UART1_PINMUX1)
static struct platform_device rtl_led_dev1 = {
	.name		= "rtl819x_led",
	.id			= 0,
	.dev		= {
		.platform_data = &rtl_pdata_led1
	},
};
#endif
#if !defined(CONFIG_P3_GPIO)
static struct rtl819x_gpio_platdata rtl_pdata_led2 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "wps led",
};

#if !defined(CONFIG_SERIAL_RTL_UART1_PINMUX1)
static struct platform_device rtl_led_dev2 = {
	.name		= "rtl819x_led",
	.id			= 1,
	.dev		= {
		.platform_data = &rtl_pdata_led2
	},
};
#endif
#endif
/* Button devices */
static struct rtl819x_gpio_platdata rtl_pdata_btn1 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "reset btn",
};

static struct platform_device rtl_btn_dev1 = {
	.name		= "rtl819x_btn",
	.id			= 0,
	.dev		= {
		.platform_data = &rtl_pdata_btn1
	},
};
#if !defined(CONFIG_I2C_DESIGNWARE_PLATFORM)//joyce
#if !defined(CONFIG_P3_GPIO)
static struct rtl819x_gpio_platdata rtl_pdata_btn2 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "wps btn",
};

static struct platform_device rtl_btn_dev2 = {
	.name		= "rtl819x_btn",
	.id			= 1,
	.dev		= {
		.platform_data = &rtl_pdata_btn2
	},
};
#endif
#endif
/* Other devices */
#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
static struct rtl819x_gpio_platdata rtl_pdata_gpio1 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "8367r reset pin",
};

static struct platform_device rtl_gpio_dev1 = {
	.name		= "rtl819x_8367r_reset_pin",
	.id			= 0,
	.dev		= {
		.platform_data = &rtl_pdata_gpio1
	},
};

static struct rtl819x_gpio_platdata rtl_pdata_gpio2 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "8367r i2c pin1",
};

static struct platform_device rtl_gpio_dev2 = {
	.name		= "rtl819x_8367r_i2c_pin",
	.id			= 1,
	.dev		= {
		.platform_data = &rtl_pdata_gpio2
	},
};

static struct rtl819x_gpio_platdata rtl_pdata_gpio3 = {
	.flags		= RTL819X_GPIO_ACTLOW,
	.name		= "8367r i2c pin2",
};

static struct platform_device rtl_gpio_dev3 = {
	.name		= "rtl819x_8367r_i2c_pin",
	.id			= 2,
	.dev		= {
		.platform_data = &rtl_pdata_gpio3
	},
};
#endif

/* devices we initialise */
static struct platform_device __initdata *rtl_devs[] = {
#if !defined(CONFIG_SERIAL_RTL_UART1_PINMUX1)
	&rtl_led_dev1,
#if !defined(CONFIG_P3_GPIO) /* There is no WPS in P3 */
	&rtl_led_dev2, 
#endif
#endif
	&rtl_btn_dev1,
#if !defined(CONFIG_I2C_DESIGNWARE_PLATFORM) //joyce
#if !defined(CONFIG_P3_GPIO)
	&rtl_btn_dev2, 
#endif 
#endif
#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
	&rtl_gpio_dev1,
	&rtl_gpio_dev2,
	&rtl_gpio_dev3,
#endif
};

static void rtl819x_gpio_pin_enable(u32 pin)
{
	unsigned long flags;
	unsigned int mask  = 0;
	unsigned int mux = 0;
	unsigned int mux_reg = 0;
	unsigned int val = 0;

    if (pin >= TOTAL_PIN_MAX)
		return;

	spin_lock_irqsave(&rtl819x_gpio_lock, flags);

	/* pin MUX1 */
	mask = rtl819x_gpio_mux(pin,&val,&mux_reg);

	//mux  = __raw_readl((void __iomem*) BSP_PINMUX_SEL_REG(pin));
	mux  = __raw_readl((void __iomem*)mux_reg);

	//if (mask != 0 && (mux & mask) == 0)
	if (mask != 0)
	    __raw_writel( ((mux&(~mask)) | (val)), (void __iomem*)mux_reg);

	/* 0 as BSP_GPIO pin */
    if(pin < BSP_GPIO_PIN_MAX)
    {
        __raw_writel(__raw_readl((void __iomem*)BSP_GPIO_CNR_REG(pin)) & ~(1<<BSP_GPIO_BIT(pin)), 
					(void __iomem*)BSP_GPIO_CNR_REG(pin));
    }
	
	pr_debug("[%s][%d]: mask=0x%x mux=0x%x mux_reg=0x%x val=0x%x, \
			CNR_REG=0x%x MUX_REG=0x%x\n"
			, __FUNCTION__, __LINE__, mask, mux, mux_reg, val
			, __raw_readl((void __iomem*)BSP_GPIO_CNR_REG(pin))
			, __raw_readl((void __iomem*)mux_reg));

	spin_unlock_irqrestore(&rtl819x_gpio_lock, flags);
}

void rtl819x_gpio_pin_disable(u32 pin)
{
	unsigned long flags;

	spin_lock_irqsave(&rtl819x_gpio_lock, flags);

	/* 1 as peripheral pin */
	__raw_writel(__raw_readl((void __iomem*)BSP_GPIO_CNR_REG(pin)) | (1<<BSP_GPIO_BIT(pin)), 
					(void __iomem*)BSP_GPIO_CNR_REG(pin));

	spin_unlock_irqrestore(&rtl819x_gpio_lock, flags);
}

static int __init rtl819x_gpio_peripheral_init(void)
{
	int err;

	printk("Realtek GPIO controller driver init\n");
	
	rtl819x_gpio_peripheral.ngpio = BSP_GPIO_PIN_MAX;
	err = gpiochip_add(&rtl819x_gpio_peripheral);

	if (err) {
		panic("cannot add rtl89x BSP_GPIO chip, error=%d", err);
		return err;
	}

	rtl_pdata_led1.gpio = BSP_RESET_LED_PIN;
#if !defined(CONFIG_P3_GPIO)
	rtl_pdata_led2.gpio = BSP_WPS_LED_PIN;
#endif
	rtl_pdata_btn1.gpio = BSP_RESET_BTN_PIN;
#if !defined(CONFIG_I2C_DESIGNWARE_PLATFORM)//joyce
#if !defined(CONFIG_P3_GPIO)
	rtl_pdata_btn2.gpio = BSP_WPS_BTN_PIN;
#endif
#endif

/* replace GPIO pin numbers for customized 97F package */
#if defined(CONFIG_RTL_8211F_SUPPORT)
	if(rtl819x_bond_option() == BSP_BOND_97FN) { //for 97FH
#if !defined(CONFIG_FREE_WBB_PIN)	
		rtl_pdata_led1.gpio = RTK_WIFI_GPIO_1; 
		rtl_pdata_led2.gpio = RTK_WIFI_GPIO_1; 
		rtl_pdata_btn1.gpio = RTK_WIFI_GPIO_0;
		rtl_pdata_btn2.gpio = RTK_WIFI_GPIO_6;
#else	
		rtl_pdata_led1.gpio = BSP_GPIO_PIN_E3;
		rtl_pdata_btn1.gpio = BSP_GPIO_PIN_E4;
		#if !defined(CONFIG_P3_GPIO)
		rtl_pdata_btn2.gpio = BSP_GPIO_PIN_E2;
		rtl_pdata_led2.gpio = BSP_GPIO_PIN_E3;
		#endif
#endif
	}
#endif

	/* For P3 GPIO customized */
#if defined(CONFIG_P3_GPIO)
  rtl_pdata_led1.gpio = RTK_WIFI_GPIO_1; 
  rtl_pdata_btn1.gpio = BSP_GPIO_PIN_E4;  /* Reset btn */
#endif

#if defined(CONFIG_RTL_8367R_SUPPORT) || defined(CONFIG_RTL_83XX_SUPPORT)
	if(rtl819x_bond_option() == BSP_BOND_97FN) {
		rtl_pdata_gpio1.gpio = BSP_8367R_RST_PIN_97FN;
		rtl_pdata_gpio2.gpio = BSP_8367R_I2C_PIN1_97FN;
		rtl_pdata_gpio3.gpio = BSP_8367R_I2C_PIN2_97FN;
	} else {
		rtl_pdata_gpio1.gpio = BSP_8367R_RST_PIN;
		rtl_pdata_gpio2.gpio = BSP_8367R_I2C_PIN1;
		rtl_pdata_gpio3.gpio = BSP_8367R_I2C_PIN2;
	}
#endif
	
	platform_add_devices(rtl_devs, ARRAY_SIZE(rtl_devs));
	
	return 0;
}
arch_initcall(rtl819x_gpio_peripheral_init);

#if defined(CONFIG_RTL819X_FLOAT_PIN_INIT)
static int __init rtl819x_float_pin_init(void)
{
	int id;
	int fb_float_pins[] = {
		BSP_GPIO_PIN_E6, BSP_GPIO_PIN_E7, BSP_GPIO_PIN_F2, 
		BSP_GPIO_PIN_F3, BSP_GPIO_PIN_F4, BSP_GPIO_PIN_F5, 
		BSP_GPIO_PIN_G3, BSP_GPIO_PIN_G4, BSP_GPIO_PIN_G5, 
		BSP_GPIO_PIN_G7
	}, fn_float_pins[] = {
		/* 
		 * Reserved even they are floating:
		 * SPI-nor RSTN: A3
		 * WBB related: E2, E3, E4, E5, F0, F1
		 * WBB check config from wifi driver : CONFIG_FREE_WBB_PIN
		 */
		/*BSP_GPIO_PIN_A3,*/ BSP_GPIO_PIN_A5, BSP_GPIO_PIN_A6, 
		BSP_GPIO_PIN_A7, BSP_GPIO_PIN_B5, BSP_GPIO_PIN_C2, 
		BSP_GPIO_PIN_C4, BSP_GPIO_PIN_C5, BSP_GPIO_PIN_C6, 
		BSP_GPIO_PIN_C7, BSP_GPIO_PIN_D0, BSP_GPIO_PIN_D1, 
		BSP_GPIO_PIN_D2, BSP_GPIO_PIN_D3, BSP_GPIO_PIN_D4, 
		BSP_GPIO_PIN_D5, BSP_GPIO_PIN_D6, BSP_GPIO_PIN_D7, 
		BSP_GPIO_PIN_E0, /*BSP_GPIO_PIN_E2, BSP_GPIO_PIN_E3, 
		BSP_GPIO_PIN_E4, BSP_GPIO_PIN_E5,*/ BSP_GPIO_PIN_E6, 
		BSP_GPIO_PIN_E7, /*BSP_GPIO_PIN_F0, BSP_GPIO_PIN_F1,*/ 
		BSP_GPIO_PIN_F2, BSP_GPIO_PIN_F3, BSP_GPIO_PIN_F4, 
		BSP_GPIO_PIN_F5, BSP_GPIO_PIN_F6, BSP_GPIO_PIN_F7, 
		BSP_GPIO_PIN_G0, BSP_GPIO_PIN_G1, BSP_GPIO_PIN_G2, 
		BSP_GPIO_PIN_G3, BSP_GPIO_PIN_G4, BSP_GPIO_PIN_G5
	}, fs_float_pins[] = {
		BSP_GPIO_PIN_E6, BSP_GPIO_PIN_E7, BSP_GPIO_PIN_F2, 
		BSP_GPIO_PIN_F3, BSP_GPIO_PIN_F4, BSP_GPIO_PIN_F5, 
		BSP_GPIO_PIN_F6, BSP_GPIO_PIN_F7, BSP_GPIO_PIN_G0, 
		BSP_GPIO_PIN_G1, BSP_GPIO_PIN_G2, BSP_GPIO_PIN_G3, 
		BSP_GPIO_PIN_G4, BSP_GPIO_PIN_G5, BSP_GPIO_PIN_G7
	};
	int *float_pin = NULL, loop, loops = 0;
	int pabcd_init_state = 0xFFE420E8, pefgh_init_state = 0x000B7801;

	id = rtl819x_bond_option();
	switch(id) {
	case BSP_BOND_97FB:
		float_pin = fb_float_pins;
		loops = sizeof(fb_float_pins)/sizeof(fb_float_pins[0]);
		break;
	case BSP_BOND_97FN:
		float_pin = fn_float_pins;
		loops = sizeof(fn_float_pins)/sizeof(fn_float_pins[0]);
		break;
	case BSP_BOND_97FS:
		float_pin = fs_float_pins;
		loops = sizeof(fs_float_pins)/sizeof(fs_float_pins[0]);
		break;
	}

	for(loop = 0; loop < loops; loop++, float_pin++){
		int init_state = ((*float_pin) >= BSP_GPIO_2ND_REG) ? pefgh_init_state : pabcd_init_state;

		rtl819x_gpio_pin_enable(*float_pin);
		rtl819x_gpio_direction_output(NULL, *float_pin, 
				(init_state >> BSP_GPIO_BIT(*float_pin)) & 0x1);
	}
	
	return 0;
}
pure_initcall(rtl819x_float_pin_init);
#endif

static unsigned int rtl819x_gpio_mux(u32 pin, u32 *value, u32 *address )
{
	unsigned int mask = 0;

	switch(pin) {
	case BSP_GPIO_PIN_A0:
		mask = 0xf<<28;
		*value = 0x8<<28;
		*address = BSP_PIN_MUX_SEL7;
		break;
	case BSP_GPIO_PIN_A1:
		mask = 0xf<<28;
		*value = 0x8<<28;
		*address = BSP_PIN_MUX_SEL6;
		break;
	case BSP_GPIO_PIN_A2:
		mask = 0xf<<24;
		*value = 0xb<<24;
		*address = BSP_PIN_MUX_SEL6;
		break;
	case BSP_GPIO_PIN_A3:
		mask = 0xf<<16;
		*value = 0x8<<16;
		*address = BSP_PIN_MUX_SEL7;
		break;
	case BSP_GPIO_PIN_A4:
		mask = 0xf<<24;
		*value = 0x7<<24;
		*address = BSP_PIN_MUX_SEL7;
		break;
	case BSP_GPIO_PIN_A5:
		mask = 0xf<<20;
		*value = 0x6<<20;
		*address = BSP_PIN_MUX_SEL7;
		break;
	case BSP_GPIO_PIN_A6:
		mask = 0xf<<16;
		*value = 0x5<<16;
		*address = BSP_PIN_MUX_SEL0;
		break;
	case BSP_GPIO_PIN_A7:
		mask = 0xf<<20;
		*value = 0x6<<20;
		*address = BSP_PIN_MUX_SEL0;
		break;

	case BSP_GPIO_PIN_B0:
		mask = 0xf<<24;
		*value = 0x8<<24;
		*address = BSP_PIN_MUX_SEL0;
		break;
	case BSP_GPIO_PIN_B1:
		mask = 0xf<<16;
		*value = 0xa<<16;
		*address = BSP_PIN_MUX_SEL2;
		break;
	case BSP_GPIO_PIN_B2:
		mask = 0xf<<16;
		*value = 0x8<<16;
		*address = BSP_PIN_MUX_SEL1;
		break;
	case BSP_GPIO_PIN_B3:
		mask = 0xf<<20;
		*value = 0x8<<20;
		*address = BSP_PIN_MUX_SEL1;
		break;
	case BSP_GPIO_PIN_B4:
		mask = 0xf<<24;
		*value = 0x8<<24;
		*address = BSP_PIN_MUX_SEL1;
		break;
	case BSP_GPIO_PIN_B5:
		mask = 0xf<<28;
		*value = 0x7<<28;
		*address = BSP_PIN_MUX_SEL1;
		break;
	case BSP_GPIO_PIN_B6:
		mask = 0xf<<28;
		*value = 0x8<<28;
		*address = BSP_PIN_MUX_SEL0;
		break;
	case BSP_GPIO_PIN_B7:
		mask = 0xf<<24;
		*value = 0x8<<24;
		*address = BSP_PIN_MUX_SEL2;
		break;

	case BSP_GPIO_PIN_C0:
		mask = 0xf<<20;
		*value = 0x6<<20;
		*address = BSP_PIN_MUX_SEL2;
		break;
	case BSP_GPIO_PIN_C1:
		mask = 0xf<<12;
		*value = 0x7<<12;
		*address = BSP_PIN_MUX_SEL2;
		break;
	case BSP_GPIO_PIN_C2:
		mask = 0xf<<8;
		*value = 0x6<<8;
		*address = BSP_PIN_MUX_SEL2;
		break;
	case BSP_GPIO_PIN_C3:
		mask = 0xf<<4;
		*value = 0x6<<4;
		*address = BSP_PIN_MUX_SEL2;
		break;
	case BSP_GPIO_PIN_C4:
		mask = 0xf<<16;
		*value = 0x2<<16;
		*address = BSP_PIN_MUX_SEL16;
		break;
	case BSP_GPIO_PIN_C5:
		mask = 0xf<<12;
		*value = 0x6<<12;
		*address = BSP_PIN_MUX_SEL16;
		break;
	case BSP_GPIO_PIN_C6:
		mask = 0xf<<8;
		*value = 0x8<<8;
		*address = BSP_PIN_MUX_SEL16;
		break;
	case BSP_GPIO_PIN_C7:
		mask = 0xf<<4;
		*value = 0x5<<4;
		*address = BSP_PIN_MUX_SEL16;
		break;

	case BSP_GPIO_PIN_D0:
		mask = 0xf<<0;
		*value = 0x5<<0;
		*address = BSP_PIN_MUX_SEL16;
		break;
	case BSP_GPIO_PIN_D1:
		mask = 0xf<<28;
		*value = 0x5<<28;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D2:
		mask = 0xf<<24;
		*value = 0x5<<24;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D3:
		mask = 0xf<<20;
		*value = 0x5<<20;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D4:
		mask = 0xf<<16;
		*value = 0x5<<16;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D5:
		mask = 0xf<<12;
		*value = 0x7<<12;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D6:
		mask = 0xf<<8;
		*value = 0x6<<8;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_D7:
		mask = 0xf<<4;
		*value = 0x7<<4;
		*address = BSP_PIN_MUX_SEL15;
		break;
	
	case BSP_GPIO_PIN_E0:
		mask = 0xf<<0;
		*value = 0x7<<0;
		*address = BSP_PIN_MUX_SEL15;
		break;
	case BSP_GPIO_PIN_E1:
		mask = 0xf<<28;
		*value = 0x1<<28;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E2:
		mask = 0xf<<24;
		*value = 0x1<<24;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E3:
		mask = 0xf<<20;
		*value = 0x1<<20;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E4:
		mask = 0xf<<16;
		*value = 0x1<<16;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E5:
		mask = 0xf<<12;
		*value = 0x1<<12;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E6:
		mask = 0xf<<8;
		*value = 0x1<<8;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_E7:
		mask = 0xf<<4;
		*value = 0x1<<4;
		*address = BSP_PIN_MUX_SEL17;
		break;

	case BSP_GPIO_PIN_F0:
		mask = 0xf<<0;
		*value = 0x1<<0;
		*address = BSP_PIN_MUX_SEL17;
		break;
	case BSP_GPIO_PIN_F1:
		mask = 0xf<<28;
		*value = 0x2<<28;
		*address = BSP_PIN_MUX_SEL18;
		break;
	case BSP_GPIO_PIN_F2:
		mask = 0xf<<24;
		*value = 0x4<<24;
		*address = BSP_PIN_MUX_SEL18;
		break;
	case BSP_GPIO_PIN_F3:
		mask = 0xf<<20;
		*value = 0x4<<20;
		*address = BSP_PIN_MUX_SEL18;
		break;
	case BSP_GPIO_PIN_F4:
		mask = 0xf<<16;
		*value = 0x6<<16;
		*address = BSP_PIN_MUX_SEL18;
		break;
	case BSP_GPIO_PIN_F5:
		mask = 0xf<<12;
		*value = 0x6<<12;
		*address = BSP_PIN_MUX_SEL18;
		break;
	case BSP_GPIO_PIN_F6:
		mask = 0xf<<24;
		*value = 0x6<<24;
		*address = BSP_PIN_MUX_SEL8;
		break;
	case BSP_GPIO_PIN_F7:
		mask = 0xf<<28;
		*value = 0x6<<28;
		*address = BSP_PIN_MUX_SEL8;
		break;

	case BSP_GPIO_PIN_G0:
		mask = 0xf<<20;
		*value = 0x6<<20;
		*address = BSP_PIN_MUX_SEL8;
		break;
	case BSP_GPIO_PIN_G1:
		mask = 0xf<<16;
		*value = 0x7<<16;
		*address = BSP_PIN_MUX_SEL8;
		break;
	case BSP_GPIO_PIN_G2:
		mask = 0xf<<12;
		*value = 0x7<<12;
		*address = BSP_PIN_MUX_SEL8;
		break;
	case BSP_GPIO_PIN_G3:
		mask = 0xf<<28;
		*value = 0x2<<28;
		*address = BSP_PIN_MUX_SEL9;
		break;
	case BSP_GPIO_PIN_G4:
		mask = 0xf<<24;
		*value = 0x1<<24;
		*address = BSP_PIN_MUX_SEL9;
		break;
	case BSP_GPIO_PIN_G5:
		mask = 0xf<<20;
		*value = 0x0<<20;
		*address = BSP_PIN_MUX_SEL9;
		break;
	case BSP_GPIO_PIN_G6:
		mask = 0xf<<28;
		*value = 0x3<<28;
		*address = BSP_PIN_MUX_SEL13;
		break;
	case BSP_GPIO_PIN_G7:
		mask = 0xf<<24;
		*value = 0x3<<24;
		*address = BSP_PIN_MUX_SEL13;
		break;

	case BSP_GPIO_PIN_H0:
		mask = 0xf<<20;
		*value = 0x3<<20;
		*address = BSP_PIN_MUX_SEL13;
		break;
	case BSP_GPIO_PIN_H1:
		mask = 0xf<<16;
		*value = 0x2<<16;
		*address = BSP_PIN_MUX_SEL13;
		break;
	case BSP_GPIO_PIN_H2:
		mask = 0xf<<28;
		*value = 0x2<<28;
		*address = BSP_PIN_MUX_SEL14;
		break;
	case BSP_GPIO_PIN_H3:
		mask = 0xf<<24;
		*value = 0x1<<24;
		*address = BSP_PIN_MUX_SEL12;
		break;
	case BSP_GPIO_PIN_H4:
		mask = 0xf<<28;
		*value = 0x1<<28;
		*address = BSP_PIN_MUX_SEL12;
		break;
	case BSP_GPIO_PIN_H5:
		mask = 0xf<<0;
		*value = 0x1<<0;
		*address = BSP_PIN_MUX_SEL8;
		break;
	case BSP_GPIO_PIN_H6:
		break;

	case BSP_GPIO_PIN_H7:
		break;

	case BSP_UART1_PIN:
		break;
	case BSP_UART2_PIN:
		break;
	default:
		break;
	}

	return mask;
}

