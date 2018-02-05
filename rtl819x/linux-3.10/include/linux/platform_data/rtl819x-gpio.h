#ifndef __RTL819X_GPIO_H
#define __RTL819X_GPIO_H

#define RTL819X_GPIO_ACTLOW (1<<0)

struct rtl819x_gpio_platdata {
	unsigned int		gpio;
	unsigned int		flags;
	char				*name;
};

#endif /* __RTL819X_GPIO_H */