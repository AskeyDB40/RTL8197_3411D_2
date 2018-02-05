#include <asm/flushall.h>

static inline void _flush(unsigned long start, unsigned long end, unsigned int op, unsigned int lineSize)
{
	unsigned long p;
    p = end & ~(lineSize - 1);
	if(p <= end)
		end = (end+lineSize) & ~(lineSize - 1);

	__asm__ __volatile__ (
		"li	$v0, %0\n"
		"li	$v1, %1\n"
		"1:\n"
		"cache	%2, 0($v0)\n"
		"addiu	$v0, $v0, %3\n"
		"bne	$v0, $v1, 1b\n"
		: : "i" (start), "i" (end), "i" (op), "i" (lineSize)
	);
}

#define __build_mips_cache_func(type, cache, op)				\
void mips_##type##_##cache##_all(void)						\
{										\
	unsigned long start = VMLINUX_LOAD_ADDRESS;				\
        unsigned long end = start + cpu_##cache##_size - cpu_##cache##_line;	\
										\
	_flush(start, end, op, cpu_##cache##_line);				\
}

__build_mips_cache_func(flush, dcache, DCACHE_INDEX_WRITEBACK_INVALIDATE)
__build_mips_cache_func(flush, l2cache, SCACHE_INDEX_WRITEBACK_INVALIDATE)
__build_mips_cache_func(invalid, icache, ICACHE_INDEX_INVALIDATE)
