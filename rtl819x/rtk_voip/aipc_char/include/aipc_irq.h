#ifndef _AIPC_IRQ_H_
#define _AIPC_IRQ_H_

#include "aipc_global.h"
#include "aipc_reg.h"
#include "aipc_mem.h"

#ifdef CONFIG_RTK_VOIP_PLATFORM_8686
#define AIPC_IRQ_CPU_T_DSP	1		// IPC IRQ line number
#define AIPC_IRQ_DSP_T_CPU	0		// IPC IRQ line number
#elif defined (CONFIG_RTK_VOIP_DRIVERS_PCM89xxE)
#define AIPC_IRQ_CPU_T_DSP	44		// IPC IRQ line number
#define AIPC_IRQ_DSP_T_CPU	45		// IPC IRQ line number
#else
#error "AIPC IRQ number is not defined!"
#endif

#ifdef __KERNEL__
#define AIPC_INT_PRIORITY	0		// IPC IRQ priority
#elif defined(__ECOS)
#define AIPC_INT_PRIORITY	0		// IPC IRQ priority
#include <stdio.h>
//#define printk 	printf
#else
#error "not support type"
#endif

/*
*	Variable define
*/
#define INT_NAME_SIZE	32

/*
*	Enum
*/
typedef enum {
	T_CPU = 0,		//Trigger CPU
	T_DSP			//Trigger DSP
} T_COP;

typedef enum {
	CPU_LOCK = 0,		//CPU Lock Mutex
	DSP_LOCK			//DSP Lock Mutex
} MUTEX_LOCK;

typedef enum {
	OWN_NONE = 0,		//Nobody own
	OWN_CPU,		    //CPU own
	OWN_DSP				//DSP own
} MUTEX_OWN;

#ifdef IPC_HW_MUTEX_ASM

static void
aipc_cpu_hwmutex_lock(void)
{
	
	unsigned int tmp_mutex;
	unsigned int tmp_own;
	unsigned int res_mutex;
	unsigned int res_own;
	MUTEX_OWN mt_own = OWN_DSP;
	
	unsigned int addr_mutex     = R_AIPC_CPU_MUTEX;
	unsigned int addr_own       = R_AIPC_OWN_MUTEX;
	unsigned int ptr_res_mutex  = (unsigned int)&res_mutex;
	unsigned int ptr_res_own    = (unsigned int)&res_own;
	
    __asm__ __volatile__ (
      
    "1: 			                    \n"
    "   lw      %0, 0(%4)              	\n"
    "   lw      %1, 0(%5)               \n"
	"   sw      %0, %2               	\n"
	"   sw      %1, %3               	\n"
    "   nop                             \n"
    "   beq     %1 , %6, 1b             \n"
    "   nop                             \n"
	:  "=&r" (tmp_mutex) , "=&r" (tmp_own) , "=m" (ptr_res_mutex) , "=m" (ptr_res_own)
	:  "r" (addr_mutex)  , "r" (addr_own)  , "r" (mt_own)
	:  "memory");

}

static void
aipc_cpu_hwmutex_unlock(void)
{
	REG32(R_AIPC_CPU_MUTEX) = 0;
}

static void
aipc_dsp_hwmutex_lock(void)
{
	
	unsigned int tmp_mutex;
	unsigned int tmp_own;
	unsigned int res_mutex;
	unsigned int res_own;
	MUTEX_OWN mt_own = OWN_CPU;
	
	unsigned int addr_mutex     = R_AIPC_DSP_MUTEX;
	unsigned int addr_own       = R_AIPC_OWN_MUTEX;
	unsigned int ptr_res_mutex  = (unsigned int)&res_mutex;
	unsigned int ptr_res_own    = (unsigned int)&res_own;
	
    __asm__ __volatile__ (
      
    "1: 			                    \n"
    "   lw      %0, 0(%4)              	\n"
    "   lw      %1, 0(%5)               \n"
	"   sw      %0, %2               	\n"
	"   sw      %1, %3               	\n"
    "   nop                             \n"
    "   beq     %1 , %6, 1b             \n"
    "   nop                             \n"
	:  "=&r" (tmp_mutex) , "=&r" (tmp_own) , "=m" (ptr_res_mutex) , "=m" (ptr_res_own)
	:  "r" (addr_mutex)  , "r" (addr_own)  , "r" (mt_own)
	:  "memory");

}

static void
aipc_dsp_hwmutex_unlock(void)
{
	REG32(R_AIPC_DSP_MUTEX) = 0;
}
#endif


#ifdef IPC_HW_MUTEX_CCODE

/*
*	Inline function
*/
static inline void
aipc_mutex_lock(MUTEX_LOCK mut)
{
	volatile unsigned int status=0;
	volatile unsigned int own=OWN_NONE;
	#ifdef HW_MUTEX_RD_CNT
	int i = 0;
	#endif

	if (mut==CPU_LOCK){
		do{
	
			#ifdef HW_MUTEX_RD_CNT
			for (i=0;i<HW_MUTEX_RD_CNT;i++){
				status = REG32(R_AIPC_CPU_MUTEX);
				own    = REG32(R_AIPC_OWN_MUTEX);		
			}
			#else
			status = REG32(R_AIPC_CPU_MUTEX);
			own    = REG32(R_AIPC_OWN_MUTEX);
			#endif

			#ifdef HW_MUTEX_DBG
			if(own==OWN_DSP){
				printk("CPU lock failed.  own=%u\n" , own);
			}		
			else if(own==OWN_CPU){
				printk("CPU lock success. own=%u\n" , own);
			}
			else{
				printk("own=%u\n" , own);
			}
			#endif
		}while( own!=OWN_CPU );
	}
	else {
		do{
			#ifdef HW_MUTEX_RD_CNT
			for (i=0;i<HW_MUTEX_RD_CNT;i++){
				status = REG32(R_AIPC_DSP_MUTEX);
				own    = REG32(R_AIPC_OWN_MUTEX);
			}
			#else
			status = REG32(R_AIPC_DSP_MUTEX);
			own    = REG32(R_AIPC_OWN_MUTEX);	
			#endif
			
			#ifdef HW_MUTEX_DBG
			if(own==OWN_CPU){
				printk("DSP lock failed.  own=%u\n" , own);
			}		
			else if(own==OWN_DSP){
				printk("DSP lock success. own=%u\n" , own);
			}
			else{
				printk("own=%u\n" , own);
			}
			#endif
		}while( own!=OWN_DSP );
	}
}

static inline unsigned int 
aipc_mutex_trylock(MUTEX_LOCK mut)		//need to check lock is success or not
{
	volatile unsigned int status=0;
	volatile unsigned int own=OWN_NONE;

	if (mut==CPU_LOCK){
		status = REG32(R_AIPC_CPU_MUTEX);
		own    = REG32(R_AIPC_OWN_MUTEX);
		
		#ifdef HW_MUTEX_DBG
		if(own==OWN_DSP){
			printk("CPU lock failed.  own=%u\n" , own);
		}		
		else if(own==OWN_CPU){
			printk("CPU lock success. own=%u\n" , own);
		}
		else{
			printk("own=%u\n" , own);
		}
		#endif
	}
	else {
		status = REG32(R_AIPC_DSP_MUTEX);
		own    = REG32(R_AIPC_OWN_MUTEX);
		
		#ifdef HW_MUTEX_DBG
		if(own==OWN_CPU){
			printk("DSP lock failed.  own=%u\n" , own);
		}		
		else if(own==OWN_DSP){
			printk("DSP lock success. own=%u\n" , own);
		}
		else{
			printk("own=%u\n" , own);
		}
		#endif
	}

	return own;
}

static inline void
aipc_mutex_unlock(MUTEX_LOCK mut)
{
	volatile unsigned int own=0;

	own = REG32(R_AIPC_OWN_MUTEX);

	if (mut==CPU_LOCK){
		if (own==OWN_CPU){	// locked by CPU
			REG32(R_AIPC_CPU_MUTEX) = 0;
			#ifdef HW_MUTEX_DBG
			printk("CPU unlock mutex\n");
			#endif
		}
	}
	else {
		if (own==OWN_DSP){	// locked by DSP
			REG32(R_AIPC_DSP_MUTEX) = 0;
			#ifdef HW_MUTEX_DBG
			printk("DSP unlock mutex\n");
			#endif
		}
	}
}

static inline unsigned int 
aipc_mutex_own(void)
{
	volatile unsigned int own=0;
	own = REG32(R_AIPC_OWN_MUTEX);

	#ifdef HW_MUTEX_DBG
	if (own==OWN_NONE)
		printk("Mutex own=%s REG32(R_AIPC_OWN_MUTEX)=0x%x\n" , "NONE" , own);
	else if (own==OWN_CPU)
		printk("Mutex own=%s REG32(R_AIPC_OWN_MUTEX)=0x%x\n" , "CPU" , own);
	else if (own==OWN_DSP)
		printk("Mutex own=%s REG32(R_AIPC_OWN_MUTEX)=0x%x\n" , "DSP" , own);
	else
		printk("Strange value!!!! REG32(R_AIPC_OWN_MUTEX)=0x%x\n" , own);
	#endif
	
	return own;
}
#endif

extern int aipc_int_assert(T_COP cop);
extern int aipc_int_deassert(T_COP cop);
extern int aipc_int_mask(T_COP cop);
extern int aipc_int_unmask(T_COP cop);

#endif

