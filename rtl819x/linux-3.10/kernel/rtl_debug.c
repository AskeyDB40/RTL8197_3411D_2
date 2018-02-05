/*HF*/
#include <linux/kernel.h>
#include <linux/ftrace.h>
#include <linux/rtl_debug.h>

static unsigned long rtl_mem_addr;
static unsigned long debug_counter_inited;
static RTL_CB_T rtl_debug_cb; /*bss.  clear during booting*/

/*Ring buffer*/
unsigned int rtl_update_idx(unsigned int index,unsigned int max_size)
{
	index++;
	if(index >= max_size)
		index=0;
	return index;
}
	
void rtl_set_mem_addr(unsigned long addr)
{
	/*keep uncache*/
	rtl_mem_addr=KSEG1ADDR(addr);
}

unsigned long rtl_get_mem_addr(void)
{
	return rtl_mem_addr;
}

RTL_DEBUG_COUNTER_Tp rtl_get_block(void)
{
	
	if(rtl_mem_addr && debug_counter_inited)
		return (RTL_DEBUG_COUNTER_Tp)rtl_mem_addr;	
	else
		return NULL;
}

RTL_DEBUG_COUNTER_Tp _rtl_get_block_by_addr(unsigned long addr)
{
	if(addr)
		return (RTL_DEBUG_COUNTER_Tp)addr;
	else
		return NULL;
}

void rtl_debug_counter_init(unsigned long addr)
{
	RTL_DEBUG_COUNTER_Tp counter;
	int i;
	counter=(RTL_DEBUG_COUNTER_Tp)addr;
	if(counter == NULL)
		return;
	memset(counter,0,sizeof(counter));
	counter->init_flag = INIT_FLAG_PATTERN;
	for(i=0;i<MAX_NUM_OF_IRQ;i++)
	{
		counter->irq_counts[MAX_NUM_OF_RECORDS-1].per_irq_count[i].irq_num=i;
	}

	for(i=0;i<MAX_NUM_OF_EXCPT;i++)
	{
		counter->except_counts[MAX_NUM_OF_RECORDS-1].per_exectp_count[i].except_num=i;
	}
	debug_counter_inited=1;
	rtl_set_mem_addr(addr);	
}

/* input:  the except num
  * output: none
  * function: use the count how many times this kind except occured.
  */
void rtl_except_count(unsigned int num)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	if(num >= MAX_NUM_OF_EXCPT)
		return;
	
	rtl_debug_cb.except_total_count++;
	/*last Entry always fresh*/
	counter->except_counts[MAX_NUM_OF_RECORDS-1].total_except_count=rtl_debug_cb.except_total_count;
	counter->except_counts[MAX_NUM_OF_RECORDS-1].per_exectp_count[num].except_count++;

	/*make a copy for history*/
	if((rtl_debug_cb.except_total_count%PERIOD_NUM_OF_EXCPT)==0)
	{
		memcpy(&counter->except_counts[rtl_debug_cb.except_idx],&counter->except_counts[MAX_NUM_OF_RECORDS-1],sizeof(EXCEPT_COUNTER_T));
		rtl_debug_cb.except_idx=rtl_update_idx(rtl_debug_cb.except_idx,(MAX_NUM_OF_RECORDS-1));
	}

	/*except trace*/
	rtl_except_trace_count(rtl_debug_cb.except_total_count, num);
}

void rtl_irq_count(unsigned int num)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	if(num >= MAX_NUM_OF_IRQ) {
		printk("irq num too big %d\n",num);
		return;
	}
	
	rtl_debug_cb.irq_total_count++;
	
	/*last Entry always fresh*/
	counter->irq_counts[MAX_NUM_OF_RECORDS-1].total_irq_count=rtl_debug_cb.irq_total_count;
	counter->irq_counts[MAX_NUM_OF_RECORDS-1].per_irq_count[num].irq_count++;

	/*make a copy for history*/
	if((rtl_debug_cb.irq_total_count%PERIOD_NUM_OF_IRQ)==0)
	{
		memcpy(&counter->irq_counts[rtl_debug_cb.irq_idx],&counter->irq_counts[MAX_NUM_OF_RECORDS-1],sizeof(IRQ_COUNTER_T));
		rtl_debug_cb.irq_idx=rtl_update_idx(rtl_debug_cb.irq_idx,(MAX_NUM_OF_RECORDS-1));
	}

	/*irq trace*/
	rtl_irq_trace_count(rtl_debug_cb.irq_total_count,num);
}

void rtl_except_trace_count(unsigned int total, unsigned int num)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	counter->except_trace[rtl_debug_cb.except_trace_idx].excp_num = num;
	counter->except_trace[rtl_debug_cb.except_trace_idx].total_count = total;
	rtl_debug_cb.except_trace_idx=rtl_update_idx(rtl_debug_cb.except_trace_idx, MAX_ARRAY_SIZE);
}

void rtl_irq_trace_count(unsigned int total, unsigned int num)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	counter->irq_trace[rtl_debug_cb.irq_trace_idx].irq_num = num;
	counter->irq_trace[rtl_debug_cb.irq_trace_idx].total_count = total;
	rtl_debug_cb.irq_trace_idx=rtl_update_idx(rtl_debug_cb.irq_trace_idx, MAX_ARRAY_SIZE);
}

void rtl_irq_disable_trace_count(unsigned long func)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	rtl_debug_cb.irq_disable_count++;
	counter->irq_disable_trace[rtl_debug_cb.irq_disable_idx].total_count = rtl_debug_cb.irq_disable_count;
	counter->irq_disable_trace[rtl_debug_cb.irq_disable_idx].func_ptr = func;
	rtl_debug_cb.irq_disable_idx=rtl_update_idx(rtl_debug_cb.irq_disable_idx, MAX_ARRAY_SIZE);
}

void rtl_irq_enable_trace_count(unsigned long func)
{
	RTL_DEBUG_COUNTER_Tp counter;	
	counter=rtl_get_block();
	if(counter == NULL)
		return;
	rtl_debug_cb.irq_enable_count++;
	counter->irq_enable_trace[rtl_debug_cb.irq_enable_idx].total_count = rtl_debug_cb.irq_enable_count;
	counter->irq_enable_trace[rtl_debug_cb.irq_enable_idx].func_ptr = func;	
	rtl_debug_cb.irq_enable_idx=rtl_update_idx(rtl_debug_cb.irq_enable_idx, MAX_ARRAY_SIZE);
}

void dump_irq_except_trace(RTL_DEBUG_COUNTER_Tp counter)
{
	int i=0;
	printk("irq enable             irq disable            except           irq\n");	
	printk("count      func        count      func        count      idx   count      idx\n");
	for(i=0;i<MAX_ARRAY_SIZE;i++)
	{
		printk("0x%08x 0x%08x  0x%08x 0x%08x  0x%08x 0x%02x  0x%08x 0x%02x\n", counter->irq_enable_trace[i].total_count,
			counter->irq_enable_trace[i].func_ptr, counter->irq_disable_trace[i].total_count,
			counter->irq_disable_trace[i].func_ptr, counter->except_trace[i].total_count,
			counter->except_trace[i].excp_num, counter->irq_trace[i].total_count,
			counter->irq_trace[i].irq_num);
	}
	printk("\n");
}
void dump_irq_except_count(RTL_DEBUG_COUNTER_Tp counter)
{
	int i=0,j=0;

	printk("irq counters:\n");
	printk("idx  ");
	for(i=0;i<MAX_NUM_OF_RECORDS;i++)
		printk("count[%d]   ",i);
	printk("\n");
		
	for(i=0;i<MAX_NUM_OF_IRQ;i++) 
	{
		printk("0x%02x",counter->irq_counts[(MAX_NUM_OF_RECORDS-1)].per_irq_count[i].irq_num);
		for(j=0;j<MAX_NUM_OF_RECORDS;j++)
		{
				printk(" 0x%08x",counter->irq_counts[j].per_irq_count[i].irq_count);
		}	
		printk("\n");
	}


	printk("except counters:\n");
	printk("idx  ");
	for(i=0;i<MAX_NUM_OF_RECORDS;i++)
		printk("count[%d]   ",i);
	printk("\n");
		
	for(i=0;i<MAX_NUM_OF_EXCPT;i++) 
	{
		printk("0x%02x",counter->except_counts[(MAX_NUM_OF_RECORDS-1)].per_exectp_count[i].except_num);
		for(j=0;j<MAX_NUM_OF_RECORDS;j++)
		{
				printk(" 0x%08x",counter->except_counts[j].per_exectp_count[i].except_count);
		}	
		printk("\n");
	}
}
void rtl_debug_counters_dump(unsigned long addr)
{
	RTL_DEBUG_COUNTER_Tp counter;
	counter=_rtl_get_block_by_addr(addr);
	if(counter == NULL)
		return;
	if(counter->init_flag != INIT_FLAG_PATTERN) {
		printk("counter not init\n");
		return; 
	}	
	dump_irq_except_trace(counter);
	dump_irq_except_count(counter);
}

void rtl_trace_hardirqs_on(void)
{
	rtl_irq_enable_trace_count(CALLER_ADDR0);
}
void rtl_trace_hardirqs_off(void)
{
	rtl_irq_disable_trace_count(CALLER_ADDR0);
}

