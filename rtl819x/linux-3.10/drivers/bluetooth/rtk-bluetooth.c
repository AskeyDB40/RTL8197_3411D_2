#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/rfkill.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/cacheflush.h>
unsigned int CurrentBTState=0;
#define RTL_R32(addr)		(*(volatile unsigned long *)(addr))
#define RTL_W32(addr, l)	((*(volatile unsigned long*)(addr)) = (l))
struct rfkill *bt_rfk;
void rtl819x_bt_setup(void);

static int rtk_bt_set_state(void *data, bool blocked)
{
	int rc = 0;
	unsigned int NowState=0;
	if (blocked)
		NowState=0;
	else 
		NowState =1;
	if (CurrentBTState == NowState) {
		return 0;
	}
	
	if (!blocked) {
		RTL_W32(0xB800350C, (RTL_R32(0xB800350C) | (0x40000000)));
		CurrentBTState = NowState;
		return 0;
	}
	RTL_W32(0xB800350C, (RTL_R32(0xB800350C) & (~(0x40000000))));
	CurrentBTState = NowState;
	return 0;
}
static void rtk_bt_get_state(struct rfkill *rfkill, void *data)
{
	int val;
	
	if (RTL_R32(0xB800350C) & (1 << 30))
	{
		val = 1;
	}else{
		val=0;
	}
	return val;
}
static const struct rfkill_ops rtk_bt_rfkill_ops = {
	.query = rtk_bt_get_state,
	.set_block = rtk_bt_set_state,
};

static int __init bt_control_probe(struct platform_device *pdev)
{
	int rc = 0;
	bt_rfk = rfkill_alloc("rtk-bluetooth", &pdev->dev, RFKILL_TYPE_BLUETOOTH,&rtk_bt_rfkill_ops,pdev);
	if (!bt_rfk)
	{
		printk("\n error in rfkill_alloc\n");
		return -ENOMEM;
	}
	if (bt_rfk) {
		if (rfkill_register(bt_rfk) < 0) {
			printk("\n error in rfkill_register\n");
			rfkill_destroy(bt_rfk);
			bt_rfk = NULL;
		}else {
			platform_set_drvdata(pdev, bt_rfk);
		}
	}
	
	return rc;
}
static struct platform_driver bt_control_driver = {
	.probe = bt_control_probe,
	.driver = {
		.name = "rtk-bluetooth",
		.owner = THIS_MODULE,
	},
};

static int __init bt_control_init(void)
{
	rtl819x_bt_setup();
	return platform_driver_register(&bt_control_driver);
}

static void __exit bt_control_exit(void)
{
	platform_driver_unregister(&bt_control_driver);
}

module_init(bt_control_init);
module_exit(bt_control_exit);



static struct platform_device rtl819x_bt_device = {
	.name			= "rtk-bluetooth",
	.id=-1,
};

void rtl819x_bt_setup(void)
{
	//Set GPIO D6 as GPIO PIN
	RTL_W32(0xB800083C, (RTL_R32(0xB800083C) & (~(0x600))));//SET GPIO FUNCTION
	RTL_W32(0xB800083C, (RTL_R32(0xB800083C) | 0x600));
	
	//Set GPIO D6 as GPIO OUTPUT PIN
	RTL_W32(0xB8003508, (RTL_R32(0xB8003508) | ((0x40000000))));//SET GPIO DIR OUT
 	RTL_W32(0xB800350C, (RTL_R32(0xB800350C) & (~(0x40000000))));//SET GPIO 0
#if 0   
 	//Set GPIO E3 as GPIO PIN
	RTL_W32(0xB8000844, (RTL_R32(0xB8000844) & (~(0x100000))));//SET GPIO FUNCTION
	RTL_W32(0xB8000844, (RTL_R32(0xB8000844) | 0x100000));
    	//Set GPIO E3 as GPIO INPUT PIN, don't know what is its function?
	RTL_W32(0xB800351C, (RTL_R32(0xB800351C) & ((~0x8))));//SET GPIO ?
	RTL_W32(0xB8003524, (RTL_R32(0xB8003524) | ((0x8))));//SET GPIO E3 DIR INPUT
	RTL_W32(0xB8003528, (RTL_R32(0xB8003528) & (~(0x8))));//SET GPIO E3 0
#endif
	printk("%s %d 0xB8000844=%X\n",__FUNCTION__, __LINE__, RTL_R32(0xB8000844));
	printk("%s %d 0xB8003524=%X\n",__FUNCTION__, __LINE__, RTL_R32(0xB8003524));
	printk("%s %d 0xB800351C=%X\n",__FUNCTION__, __LINE__, RTL_R32(0xB800351C));
	CurrentBTState=0;
    	
	/*register platform device*/
	platform_device_register(&rtl819x_bt_device);
	printk("%s %d 0xb8000808=%X\n",__FUNCTION__, __LINE__, RTL_R32(0xb8000808));
}


MODULE_DESCRIPTION("rtk bt control");
MODULE_LICENSE("GPL");
