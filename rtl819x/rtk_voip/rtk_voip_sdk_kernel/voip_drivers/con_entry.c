#include "rtk_voip.h"
#include "voip_init.h"
#include "voip_addrspace.h"

#ifndef NO_SPECIAL_ADDRSPACE
unsigned long physical_addr_offset = CONFIG_RTK_VOIP_2_PHYSICAL_OFFSET;
#endif


#if defined CONFIG_RTK_VOIP_DRIVERS_PCM8186
#if RTL8186PV_RELAY_USE_Z  // For Z-version board 8186PV
#define RELAY_SW_CTRL_GPIOC	//for gpioC use. pull relay high. for Z-version board 1*FXS 1*FXO.
#else
#if (!defined (CONFIG_RTK_VOIP_DRIVERS_PCM8186V_OC)) && (!defined (CONFIG_RTK_VOIP_DRIVERS_PCM8651_2S_OC))
#ifdef CONFIG_RTK_VOIP_DRIVERS_8186V_ROUTER
#define RELAY_SW_CTRL_GPIOD	//for gpioD used.pull relay high. for new 4-LAN EV board. 2006-07.
#else
#define RELAY_SW_CTRL_GPIOE	//for gpioE used.pull relay high.
#endif
#endif
#endif
#endif


#define RTL8881A_CLOCK_MANAGE_REG 0xb8000010
#define RTL8881A_CLOCK_MASK_NEW  0x02DCB000
#define RTL8881A_CLOCK_VALUE_PCM_NEW 0x009CB000 //using internal clock for 24.576Mhz, turn on pcm, voipacc clock

#define RTL8881A_PLL2_REG   0xb8000058
#define RTL8881A_PLL2_VALUE 0x00000001 // for  24.576Mhz
#define RTL8881A_SHARE_PIN_REG 0xb800004c	//pcm  iis share pin config register
#define RTL8881A_SHARE_PIN_MASK 0x77777
#define RTL8881A_SHARE_PIN_VALUE_PCM 0x55555
static int __init voip_con_start_setup_init( void )
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY
	extern void rtl8972B_hw_init(int mode);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxC
	extern void rtl8954C_hw_init(int mode);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxD
	extern void rtl8972D_hw_init(int mode);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8881A
	extern void rtl8972D_hw_init(int mode);
	unsigned int temp;
	
	temp = *((volatile unsigned int *)RTL8881A_CLOCK_MANAGE_REG) & (~RTL8881A_CLOCK_MASK_NEW);
	*((volatile unsigned int *)RTL8881A_CLOCK_MANAGE_REG) = temp | RTL8881A_CLOCK_VALUE_PCM_NEW;			 
	*((volatile unsigned int *)RTL8881A_PLL2_REG) |= RTL8881A_PLL2_VALUE;
		
	temp = *((volatile unsigned int *)0xb800000c);
	temp = *((volatile unsigned int *)RTL8881A_SHARE_PIN_REG) & (~RTL8881A_SHARE_PIN_MASK);
	*((volatile unsigned int *)RTL8881A_SHARE_PIN_REG) = temp | RTL8881A_SHARE_PIN_VALUE_PCM;

#endif


#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxE
	extern void rtl8954E_hw_init(int mode);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672
	extern void rtl8672_hw_init(void);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8676
	extern void rtl8676_hw_init(int mode);
#endif
	
	extern void start_os_timer( void );
#ifndef CONFIG_RTK_VOIP_IPC_ARCH_FULLY_OFFLOAD
	extern void init_rtl_hw_spi_IP( void );
#endif
	
#ifdef  RELAY_SW_CTRL_GPIOC//for gpioC used.pull relay high.	
	#define GPCD_DIR  *((volatile unsigned int *)0xbd010134)
	#define GPCD_DATA  *((volatile unsigned int *)0xbd010130)	
	BOOT_MSG("GPCD_DIR = %x\n",GPCD_DIR);
	GPCD_DIR = GPCD_DIR | 0x01; 
	BOOT_MSG("GPCD_DIR = %x\n",GPCD_DIR);
	GPCD_DATA = GPCD_DATA & 0xfffffffe;
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8972B_hw_init(1); /* need init iis */
    #else
	rtl8972B_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxC
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8954C_hw_init(1); /* need init iis */
    #else
	rtl8954C_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM89xxC */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxD
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8972D_hw_init(1); /* need init iis */
    #else
	rtl8972D_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM89xxD || CONFIG_RTK_VOIP_DRIVERS_PCM8881A */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8881A
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8972D_hw_init(1); /* need init iis */
    #else
	rtl8972D_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM89xxD || CONFIG_RTK_VOIP_DRIVERS_PCM8881A */


#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxE
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8954E_hw_init(1); /* need init iis */
    #else
	rtl8954E_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM89xxE */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672
	rtl8672_hw_init();
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM8672 */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8676
#ifdef CONFIG_RTK_VOIP_8676_ISI_ZSI
	//rtl8676_hw_init(1); /* normal + ISI/ZSI */	// disable it, handle by 8676 bsp setting
#else
	//rtl8676_hw_init(0); /* normal */		// disable it, handle by 8676 bsp setting
#endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM8676 */
	
	start_os_timer();

#ifndef CONFIG_RTK_VOIP_IPC_ARCH_FULLY_OFFLOAD
#ifndef CONFIG_RTK_VOIP_DRIVERS_MIRROR_SLIC
#ifdef CONFIG_RTK_VOIP_HARDWARE_SPI	
	init_rtl_hw_spi_IP();
#endif
#endif
#endif
	
	return 0;
}

voip_initcall_entry( voip_con_start_setup_init );

