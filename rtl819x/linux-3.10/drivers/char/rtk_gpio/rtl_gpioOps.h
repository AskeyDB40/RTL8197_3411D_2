#ifndef __RTL_GPIO_H__
#define __RTL_GPIO_H__

#define GPIO_REG_BASE		0xB8003500

/* GPIO ABCD */
#define GPIO_PABCD_CNR		(GPIO_REG_BASE + 0x0)
#define GPIO_PABCD_DIR		(GPIO_REG_BASE + 0x8)
#define GPIO_PABCD_DAT		(GPIO_REG_BASE + 0xC)

/* GPIO ABCD Interrupt */
#define GPIO_PABCD_ISR		(GPIO_REG_BASE + 0x10)
#define GPIO_PAB_IMR		(GPIO_REG_BASE + 0x14)
#define GPIO_PCD_IMR		(GPIO_REG_BASE + 0x18)

/* GPIO EFGH */
#define GPIO_PEFGH_CNR		(GPIO_REG_BASE + 0x1C)
#define GPIO_PEFGH_DIR		(GPIO_REG_BASE + 0x24)
#define GPIO_PEFGH_DAT		(GPIO_REG_BASE + 0x28)

/* GPIO EFGH Interrupt */
#define GPIO_PEFGH_ISR		(GPIO_REG_BASE + 0x2C)
#define GPIO_PEF_IMR		(GPIO_REG_BASE + 0x30)
#define GPIO_PGH_IMR		(GPIO_REG_BASE + 0x34)


/****************************GPIO Define*********************************/
enum GPIO_FUNC	
{
	GPIO_FUNC_CONTROL,
	GPIO_FUNC_DIRECTION,
	GPIO_FUNC_DATA,
	GPIO_FUNC_INTERRUPT_STATUS,
	GPIO_FUNC_INTERRUPT_ENABLE,
	GPIO_FUNC_MAX,
};

#endif