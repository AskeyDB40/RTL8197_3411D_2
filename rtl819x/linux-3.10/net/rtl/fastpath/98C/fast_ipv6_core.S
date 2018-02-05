	.file	1 "fast_ipv6_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"ipv6_fast_forward\000"
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_exit
	.set	nomips16
	.ent	ipv6_fast_forward_exit
	.type	ipv6_fast_forward_exit, @function
ipv6_fast_forward_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(ipv6_res)
	lw	$2,%lo(ipv6_res)($2)
	beq	$2,$0,$L2
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

$L2:
	lw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	sw	$0,%lo(fast_ipv6_fw)($2)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_exit
	.size	ipv6_fast_forward_exit, .-ipv6_fast_forward_exit
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_init
	.set	nomips16
	.ent	ipv6_fast_forward_init
	.type	ipv6_fast_forward_init, @function
ipv6_fast_forward_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC0)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(ipv6_fast_forward_proc_fops)
	addiu	$4,$4,%lo($LC0)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(ipv6_fast_forward_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lui	$3,%hi(ipv6_res)
	sw	$2,%lo(ipv6_res)($3)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_init
	.size	ipv6_fast_forward_init, .-ipv6_fast_forward_init
	.section	.text.ipv6_fast_forward_single_open,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_single_open
	.set	nomips16
	.ent	ipv6_fast_forward_single_open
	.type	ipv6_fast_forward_single_open, @function
ipv6_fast_forward_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	move	$4,$5
	lui	$5,%hi(ipv6_forward_read_proc)
	addiu	$5,$5,%lo(ipv6_forward_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_single_open
	.size	ipv6_fast_forward_single_open, .-ipv6_fast_forward_single_open
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"%c\012\000"
	.section	.text.ipv6_forward_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	ipv6_forward_read_proc
	.type	ipv6_forward_read_proc, @function
ipv6_forward_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	lw	$6,%lo(fast_ipv6_fw)($2)
	lui	$5,%hi($LC1)
	addiu	$5,$5,%lo($LC1)
	jal	seq_printf
	addiu	$6,$6,48

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ipv6_forward_read_proc
	.size	ipv6_forward_read_proc, .-ipv6_forward_read_proc
	.rdata
	.align	2
$LC2:
	.space	4
	.section	.text.init_V6_table_path,"ax",@progbits
	.align	2
	.globl	init_V6_table_path
	.set	nomips16
	.ent	init_V6_table_path
	.type	init_V6_table_path, @function
init_V6_table_path:
	.frame	$sp,48,$31		# vars= 0, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,20($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32800			# 0x8020

	lui	$17,%hi(v6_table_path)
	beq	$2,$0,$L12
	sw	$2,%lo(v6_table_path)($17)

	lui	$3,%hi($LC2)
	lw	$4,%lo($LC2)($3)
	lui	$6,%hi(v6_path_list_inuse)
	lui	$3,%hi(v6_path_list_free)
	sw	$4,0($2)
	sll	$2,$18,2
	sll	$4,$18,4
	subu	$4,$4,$2
	lw	$21,%lo(v6_table_path)($17)
	addiu	$2,$6,%lo(v6_path_list_inuse)
	addiu	$16,$3,%lo(v6_path_list_free)
	sw	$0,%lo(v6_path_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(v6_path_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(v6_path_list_inuse)($6)
	sw	$18,%lo(v6_path_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,4($21)
	lw	$2,%lo(v6_table_path)($17)
	lw	$4,4($2)
	beq	$4,$0,$L12
	nop

	blez	$18,$L13
	move	$2,$0

	move	$3,$0
	j	$L14
	move	$5,$17

$L21:
	lw	$4,%lo(v6_table_path)($5)
	lw	$4,4($4)
$L14:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(v6_table_path)($17)
	addiu	$3,$3,1
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(v6_table_path)($17)
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L21
	addiu	$2,$2,12

$L13:
	blez	$19,$L15
	move	$17,$0

	j	$L22
	lw	$4,4($20)

$L16:
	sw	$0,60($2)
	lw	$5,4($16)
	sw	$5,64($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L15
	sw	$2,8($16)

	lw	$4,4($20)
$L22:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,60
	bne	$2,$0,$L16
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L17:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L15:
	j	$L17
	move	$2,$0

$L12:
	j	$L17
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_V6_table_path
	.size	init_V6_table_path, .-init_V6_table_path
	.section	.text.V6_cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	V6_cache_timeout
	.type	V6_cache_timeout, @function
V6_cache_timeout:
	.frame	$sp,64,$31		# vars= 32, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lbu	$2,48($4)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L28
	move	$16,$4
	.set	macro
	.set	reorder

	lw	$31,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L28:
	lw	$10,4($4)
	lw	$9,8($4)
	lw	$8,12($4)
	lw	$4,16($4)
	lhu	$18,38($16)
	sw	$10,32($sp)
	sw	$9,36($sp)
	sw	$8,40($sp)
	sw	$4,44($sp)
	lw	$6,20($16)
	xor	$7,$9,$10
	lw	$5,24($16)
	xor	$7,$7,$8
	lw	$3,28($16)
	xor	$7,$7,$4
	lw	$2,32($16)
	xor	$7,$7,$6
	xor	$7,$7,$5
	xor	$7,$7,$3
	xor	$7,$7,$2
	srl	$10,$10,8
	xor	$7,$7,$10
	srl	$9,$9,8
	xor	$7,$7,$9
	srl	$8,$8,8
	xor	$7,$7,$8
	srl	$4,$4,8
	srl	$11,$6,16
	xor	$7,$7,$4
	lw	$10,0($16)
	xor	$7,$7,$11
	srl	$11,$5,16
	srl	$9,$3,16
	xor	$7,$7,$11
	lui	$4,%hi(V6_max_udp_frag_entry)
	lw	$8,%lo(V6_max_udp_frag_entry)($4)
	xor	$7,$7,$9
	lui	$17,%hi(V6_udp_cache_table)
	srl	$9,$2,16
	xor	$7,$7,$9
	xor	$18,$18,$10
	lw	$4,%lo(V6_udp_cache_table)($17)
	xor	$18,$18,$7
	srl	$7,$7,16
	xor	$18,$18,$7
	addiu	$8,$8,-1
	sw	$2,28($sp)
	and	$18,$18,$8
	sw	$6,16($sp)
	sw	$5,20($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	lw	$2,80($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L25
	sb	$0,48($16)
	.set	macro
	.set	reorder

	lw	$4,84($16)
	sll	$3,$18,4
	sll	$18,$18,2
	sw	$4,84($2)
	subu	$18,$3,$18
$L26:
	lw	$3,84($16)
	lw	$4,80($16)
	lui	$2,%hi(V6_udp_cache_list_free)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	sw	$4,0($3)
	lw	$3,%lo(V6_udp_cache_table)($17)
	addiu	$5,$16,88
	lw	$3,4($3)
	addu	$18,$3,$18
	lw	$3,8($18)
	addiu	$3,$3,-1
	sw	$3,8($18)
	sw	$0,88($16)
	lw	$3,4($2)
	sw	$3,92($16)
	lw	$3,4($2)
	sw	$16,0($3)
	lw	$4,8($2)
	lw	$3,%lo(V6_udp_cache_table)($17)
	sw	$5,4($2)
	addiu	$4,$4,1
	sw	$4,8($2)
	lhu	$2,2($3)
	addiu	$2,$2,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$2,2($3)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$31,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L25:
	lw	$2,%lo(V6_udp_cache_table)($17)
	sll	$4,$18,4
	lw	$3,84($16)
	lw	$2,4($2)
	sll	$18,$18,2
	subu	$18,$4,$18
	addu	$2,$2,$18
	.set	noreorder
	.set	nomacro
	j	$L26
	sw	$3,4($2)
	.set	macro
	.set	reorder

	.end	V6_cache_timeout
	.size	V6_cache_timeout, .-V6_cache_timeout
	.section	.text.rtk_flushV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_flushV6Connection
	.set	nomips16
	.ent	rtk_flushV6Connection
	.type	rtk_flushV6Connection, @function
rtk_flushV6Connection:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(v6_table_path)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lw	$4,%lo(v6_table_path)($16)
	.set	macro
	.set	reorder

	lui	$2,%hi(v6_path_table_list_max)
	lw	$10,%lo(v6_path_table_list_max)($2)
	.set	noreorder
	.set	nomacro
	beq	$10,$0,$L31
	lw	$4,%lo(v6_table_path)($16)
	.set	macro
	.set	reorder

	lui	$5,%hi(v6_path_list_inuse)
	lui	$3,%hi(v6_path_list_free)
	addiu	$5,$5,%lo(v6_path_list_inuse)
	addiu	$3,$3,%lo(v6_path_list_free)
	move	$8,$0
	move	$9,$0
$L38:
	lw	$2,4($4)
	addu	$2,$2,$8
	lw	$2,0($2)
	bne	$2,$0,$L37
	.set	noreorder
	.set	nomacro
	j	$L46
	addiu	$9,$9,1
	.set	macro
	.set	reorder

$L44:
	lw	$6,56($2)
	sw	$6,56($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($16)
	lw	$4,4($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	beq	$4,$0,$L35
$L45:
	lw	$6,64($2)
	sw	$6,64($4)
$L36:
	lw	$4,64($2)
	lw	$7,60($2)
	addiu	$6,$2,60
	sw	$7,0($4)
	lw	$4,8($5)
	addiu	$4,$4,-1
	sw	$4,8($5)
	sw	$0,60($2)
	lw	$4,4($3)
	sw	$4,64($2)
	lw	$4,4($3)
	sw	$2,0($4)
	lw	$4,8($3)
	sw	$6,4($3)
	addiu	$4,$4,1
	sw	$4,8($3)
	lw	$2,52($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L43
	lw	$4,%lo(v6_table_path)($16)
	.set	macro
	.set	reorder

$L37:
	lw	$4,52($2)
	bne	$4,$0,$L44
	lw	$4,%lo(v6_table_path)($16)
	lw	$6,56($2)
	lw	$4,4($4)
	addu	$4,$4,$8
	sw	$6,4($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($16)
	lw	$4,4($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	bne	$4,$0,$L45
$L35:
	lw	$4,64($2)
	.set	noreorder
	.set	nomacro
	j	$L36
	sw	$4,4($5)
	.set	macro
	.set	reorder

$L43:
	addiu	$9,$9,1
$L46:
	sltu	$2,$9,$10
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L38
	addiu	$8,$8,12
	.set	macro
	.set	reorder

$L31:
	lhu	$2,2($4)
	addiu	$2,$2,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$2,2($4)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	rtk_flushV6Connection
	.size	rtk_flushV6Connection, .-rtk_flushV6Connection
	.section	.text.ipv6_fast_forward_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	ipv6_fast_forward_single_write
	.type	ipv6_fast_forward_single_write, @function
ipv6_fast_forward_single_write:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sltu	$2,$6,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L52
	move	$16,$6
	.set	macro
	.set	reorder

$L48:
	li	$2,-14			# 0xfffffffffffffff2
$L50:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L52:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L50
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$2,$5,1
	or	$3,$2,$5
	lw	$2,24($28)
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L48
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 1181 "net/rtl/fastpath/fast_ipv6_core.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L48
	lb	$2,16($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(fast_ipv6_fw)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L53
	sw	$2,%lo(fast_ipv6_fw)($3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L50
	move	$2,$16
	.set	macro
	.set	reorder

$L53:
	jal	rtk_flushV6Connection
	.set	noreorder
	.set	nomacro
	j	$L50
	move	$2,$16
	.set	macro
	.set	reorder

	.end	ipv6_fast_forward_single_write
	.size	ipv6_fast_forward_single_write, .-ipv6_fast_forward_single_write
	.section	.text.rtk_idleV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_idleV6Connection
	.set	nomips16
	.ent	rtk_idleV6Connection
	.type	rtk_idleV6Connection, @function
rtk_idleV6Connection:
	.frame	$sp,112,$31		# vars= 64, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$22,104($sp)
	sw	$21,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
	lui	$18,%hi(v6_table_path)
	move	$17,$4
	lw	$4,%lo(v6_table_path)($18)
	lui	$2,%hi(jiffies)
	move	$21,$5
	lw	$22,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lui	$19,%hi(v6_path_table_list_max)
	.set	macro
	.set	reorder

	lw	$8,0($17)
	lw	$4,40($17)
	lw	$7,4($17)
	lw	$6,8($17)
	lw	$5,12($17)
	xor	$2,$8,$4
	lw	$12,16($17)
	xor	$2,$2,$7
	lw	$11,20($17)
	xor	$2,$2,$6
	lw	$10,24($17)
	xor	$2,$2,$5
	lw	$9,28($17)
	xor	$2,$2,$12
	xor	$2,$2,$11
	xor	$2,$2,$10
	srl	$3,$8,8
	xor	$2,$2,$9
	xor	$2,$2,$3
	srl	$3,$7,8
	xor	$2,$2,$3
	srl	$3,$6,8
	xor	$2,$2,$3
	srl	$3,$5,8
	xor	$2,$2,$3
	srl	$3,$12,16
	xor	$2,$2,$3
	srl	$13,$11,16
	lw	$3,36($17)
	xor	$2,$2,$13
	srl	$13,$10,16
	lhu	$24,34($17)
	xor	$2,$2,$13
	srl	$13,$9,16
	xor	$2,$2,$13
	srl	$13,$3,4
	xor	$2,$2,$13
	lw	$15,%lo(v6_path_table_list_max)($19)
	xor	$2,$2,$24
	lw	$13,%lo(v6_table_path)($18)
	srl	$14,$2,12
	xor	$2,$14,$2
	sw	$8,16($sp)
	sw	$7,20($sp)
	sw	$6,24($sp)
	sw	$5,28($sp)
	sw	$12,32($sp)
	sw	$11,36($sp)
	sw	$10,40($sp)
	sw	$9,44($sp)
	addiu	$15,$15,-1
	and	$2,$2,$15
	lw	$14,4($13)
	sll	$25,$2,4
	sll	$2,$2,2
	subu	$2,$25,$2
	addu	$2,$14,$2
	lw	$16,0($2)
	move	$25,$4
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L55
	move	$2,$3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L60
	addiu	$20,$17,16
	.set	macro
	.set	reorder

$L56:
	lw	$16,52($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L75
	lw	$13,%lo(v6_table_path)($18)
	.set	macro
	.set	reorder

$L60:
	lw	$2,36($16)
	bne	$2,$3,$L56
	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L56
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L70
	addiu	$4,$16,16
	.set	macro
	.set	reorder

$L73:
	lw	$16,52($16)
	lw	$3,36($17)
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L60
	lw	$4,40($17)
	.set	macro
	.set	reorder

	lw	$13,%lo(v6_table_path)($18)
$L75:
	lw	$15,%lo(v6_path_table_list_max)($19)
	lhu	$24,34($17)
	lw	$14,4($13)
	lw	$8,0($17)
	lw	$7,4($17)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$12,16($17)
	lw	$11,20($17)
	lw	$10,24($17)
	lw	$9,28($17)
	move	$25,$4
	move	$2,$3
	addiu	$15,$15,-1
$L55:
	xor	$3,$11,$12
	xor	$3,$3,$10
	xor	$3,$3,$9
	xor	$3,$3,$8
	xor	$3,$3,$7
	xor	$3,$3,$6
	srl	$4,$12,8
	xor	$3,$3,$5
	xor	$3,$3,$4
	srl	$4,$11,8
	xor	$3,$3,$4
	srl	$4,$10,8
	xor	$3,$3,$4
	srl	$4,$9,8
	xor	$3,$3,$4
	srl	$4,$8,16
	xor	$3,$3,$4
	srl	$4,$7,16
	xor	$3,$3,$4
	srl	$4,$6,16
	xor	$3,$3,$4
	srl	$4,$5,16
	xor	$3,$3,$4
	xor	$3,$3,$2
	xor	$24,$3,$24
	srl	$25,$25,4
	xor	$24,$24,$25
	srl	$3,$24,12
	xor	$24,$3,$24
	and	$15,$24,$15
	sll	$3,$15,4
	sll	$15,$15,2
	subu	$15,$3,$15
	addu	$14,$14,$15
	sw	$12,48($sp)
	sw	$11,52($sp)
	sw	$10,56($sp)
	sw	$9,60($sp)
	sw	$8,64($sp)
	sw	$7,68($sp)
	sw	$6,72($sp)
	sw	$5,76($sp)
	lw	$16,0($14)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L61
	addiu	$19,$17,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L76
	lw	$3,36($16)
	.set	macro
	.set	reorder

$L62:
	lw	$16,52($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L61
	lw	$13,%lo(v6_table_path)($18)
	.set	macro
	.set	reorder

	lw	$2,36($17)
	lw	$3,36($16)
$L76:
	bne	$3,$2,$L62
	lw	$3,40($16)
	lw	$2,40($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L62
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L62
	addiu	$4,$16,16
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L62
	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L62
	lw	$2,44($16)
	subu	$21,$22,$21
	subu	$21,$21,$2
	.set	noreorder
	.set	nomacro
	bgez	$21,$L64
	lw	$13,%lo(v6_table_path)($18)
	.set	macro
	.set	reorder

$L63:
	lw	$2,%lo(v6_table_path)($18)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L58
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L64:
$L61:
	lhu	$2,2($13)
	addiu	$2,$2,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$2,2($13)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	move	$2,$0
$L58:
	lw	$31,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L70:
	move	$5,$20
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L73
	lw	$24,32($16)
	lw	$2,32($17)
	.set	noreorder
	.set	nomacro
	bne	$24,$2,$L73
	subu	$3,$22,$21
	.set	macro
	.set	reorder

	lw	$2,44($16)
	subu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L63
	lw	$15,%lo(v6_path_table_list_max)($19)
	.set	macro
	.set	reorder

	lw	$13,%lo(v6_table_path)($18)
	andi	$24,$24,0xffff
	addiu	$15,$15,-1
	lw	$25,40($17)
	lw	$2,36($17)
	lw	$14,4($13)
	lw	$8,0($17)
	lw	$7,4($17)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$12,16($17)
	lw	$11,20($17)
	lw	$10,24($17)
	.set	noreorder
	.set	nomacro
	j	$L55
	lw	$9,28($17)
	.set	macro
	.set	reorder

	.end	rtk_idleV6Connection
	.size	rtk_idleV6Connection, .-rtk_idleV6Connection
	.section	.text.rtk_delV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_delV6Connection
	.set	nomips16
	.ent	rtk_delV6Connection
	.type	rtk_delV6Connection, @function
rtk_delV6Connection:
	.frame	$sp,80,$31		# vars= 32, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$22,72($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lui	$18,%hi(v6_table_path)
	move	$17,$4
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lw	$4,%lo(v6_table_path)($18)
	.set	macro
	.set	reorder

	lw	$11,0($17)
	lw	$12,40($17)
	lw	$10,4($17)
	lw	$9,8($17)
	lw	$8,12($17)
	xor	$12,$11,$12
	lw	$7,16($17)
	xor	$12,$12,$10
	lw	$6,20($17)
	xor	$12,$12,$9
	lw	$5,24($17)
	xor	$12,$12,$8
	lw	$4,28($17)
	xor	$12,$12,$7
	xor	$12,$12,$6
	xor	$12,$12,$5
	xor	$12,$12,$4
	srl	$2,$11,8
	xor	$12,$12,$2
	srl	$2,$10,8
	xor	$12,$12,$2
	srl	$2,$9,8
	xor	$12,$12,$2
	srl	$2,$8,8
	xor	$12,$12,$2
	srl	$2,$7,16
	xor	$12,$12,$2
	srl	$3,$6,16
	lw	$2,36($17)
	xor	$12,$12,$3
	srl	$3,$5,16
	srl	$13,$4,16
	xor	$12,$12,$3
	xor	$12,$12,$13
	lhu	$3,34($17)
	srl	$13,$2,4
	xor	$12,$12,$13
	lui	$13,%hi(v6_path_table_list_max)
	lw	$13,%lo(v6_path_table_list_max)($13)
	xor	$12,$12,$3
	lw	$3,%lo(v6_table_path)($18)
	srl	$19,$12,12
	sw	$5,40($sp)
	sw	$4,44($sp)
	xor	$12,$19,$12
	sw	$11,16($sp)
	sw	$10,20($sp)
	sw	$9,24($sp)
	sw	$8,28($sp)
	sw	$7,32($sp)
	sw	$6,36($sp)
	addiu	$5,$13,-1
	and	$4,$12,$5
	lw	$5,4($3)
	sll	$19,$4,4
	sll	$4,$4,2
	subu	$19,$19,$4
	addu	$4,$5,$19
	lw	$16,0($4)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L78
	lui	$21,%hi(v6_path_list_free)
	.set	macro
	.set	reorder

	lui	$22,%hi(v6_path_list_inuse)
	addiu	$21,$21,%lo(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L85
	addiu	$20,$17,16
	.set	macro
	.set	reorder

$L79:
	lw	$16,52($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L84
	lw	$3,%lo(v6_table_path)($18)
	.set	macro
	.set	reorder

	lw	$2,36($17)
$L85:
	lw	$3,36($16)
	bne	$3,$2,$L79
	lw	$3,40($16)
	lw	$2,40($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L79
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$17
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L79
	addiu	$4,$16,16
	.set	macro
	.set	reorder

	move	$5,$20
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L79
	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L79
	lw	$2,52($16)
	beq	$2,$0,$L80
	lw	$3,56($16)
	sw	$3,56($2)
$L81:
	lw	$2,56($16)
	lw	$3,52($16)
	sw	$3,0($2)
	lw	$2,%lo(v6_table_path)($18)
	lw	$2,4($2)
	addu	$2,$2,$19
	lw	$3,8($2)
	addiu	$3,$3,-1
	sw	$3,8($2)
	lw	$3,60($16)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L82
	move	$2,$22
	.set	macro
	.set	reorder

	lw	$4,64($16)
	sw	$4,64($3)
$L83:
	lw	$4,64($16)
	lw	$5,60($16)
	addiu	$3,$16,60
	sw	$5,0($4)
	lw	$4,8($2)
	addiu	$4,$4,-1
	sw	$4,8($2)
	sw	$0,60($16)
	lw	$2,4($21)
	sw	$2,64($16)
	lw	$2,4($21)
	sw	$16,0($2)
	lw	$2,8($21)
	sw	$3,4($21)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L79
	sw	$2,8($21)
	.set	macro
	.set	reorder

$L84:
$L78:
	lhu	$2,2($3)
	addiu	$2,$2,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$2,2($3)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$31,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,80
	.set	macro
	.set	reorder

$L80:
	lw	$2,%lo(v6_table_path)($18)
	lw	$3,56($16)
	lw	$2,4($2)
	addu	$2,$2,$19
	.set	noreorder
	.set	nomacro
	j	$L81
	sw	$3,4($2)
	.set	macro
	.set	reorder

$L82:
	lw	$3,64($16)
	.set	noreorder
	.set	nomacro
	j	$L83
	sw	$3,4($22)
	.set	macro
	.set	reorder

	.end	rtk_delV6Connection
	.size	rtk_delV6Connection, .-rtk_delV6Connection
	.section	.text.ipv6_fast_enter,"ax",@progbits
	.align	2
	.globl	ipv6_fast_enter
	.set	nomips16
	.ent	ipv6_fast_enter
	.type	ipv6_fast_enter, @function
ipv6_fast_enter:
	.frame	$sp,336,$31		# vars= 280, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-336
	sw	$31,332($sp)
	sw	$fp,328($sp)
	sw	$23,324($sp)
	sw	$22,320($sp)
	sw	$21,316($sp)
	sw	$20,312($sp)
	sw	$19,308($sp)
	sw	$18,304($sp)
	sw	$17,300($sp)
	sw	$16,296($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header
	move	$17,$4
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_pppoe_flag
	move	$18,$2
	.set	macro
	.set	reorder

	bne	$2,$0,$L88
	lbu	$3,12($16)
	lbu	$2,13($16)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,34525			# 0x86dd
	beq	$3,$2,$L88
$L89:
	move	$2,$0
$L101:
	lw	$31,332($sp)
$L135:
	lw	$fp,328($sp)
	lw	$23,324($sp)
	lw	$22,320($sp)
	lw	$21,316($sp)
	lw	$20,312($sp)
	lw	$19,308($sp)
	lw	$18,304($sp)
	lw	$17,300($sp)
	lw	$16,296($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,336
	.set	macro
	.set	reorder

$L88:
	.set	noreorder
	.set	nomacro
	jal	__ipv6_addr_type
	addiu	$4,$18,8
	.set	macro
	.set	reorder

	andi	$2,$2,0x20
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L101
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	__ipv6_addr_type
	addiu	$4,$18,24
	.set	macro
	.set	reorder

	andi	$2,$2,0x22
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L89
	li	$4,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$2,6($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L90
	addiu	$3,$18,40
	.set	macro
	.set	reorder

	li	$4,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L91
	li	$4,44			# 0x2c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L101
	move	$2,$0
	.set	macro
	.set	reorder

$L91:
	lw	$6,8($18)
	lw	$5,12($18)
	lw	$4,16($18)
	lw	$2,20($18)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	lw	$2,36($18)
	lw	$6,24($18)
	lw	$5,28($18)
	lw	$4,32($18)
	sw	$2,44($sp)
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$4,40($sp)
	lbu	$23,6($18)
	li	$2,44			# 0x2c
	lhu	$19,40($18)
	.set	noreorder
	.set	nomacro
	beq	$23,$2,$L92
	lhu	$20,2($3)
	.set	macro
	.set	reorder

	andi	$fp,$23,0xffff
	move	$21,$0
	sw	$0,240($sp)
	sw	$0,244($sp)
	sw	$0,248($sp)
$L93:
	lui	$22,%hi(v6_table_path)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lw	$4,%lo(v6_table_path)($22)
	.set	macro
	.set	reorder

	lw	$10,16($sp)
	lw	$9,20($sp)
	lw	$8,24($sp)
	lw	$7,28($sp)
	lw	$6,32($sp)
	xor	$11,$9,$10
	lw	$5,36($sp)
	xor	$11,$11,$8
	lw	$4,40($sp)
	xor	$11,$11,$7
	lw	$3,44($sp)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$2,$5,16
	xor	$11,$11,$2
	srl	$2,$4,16
	xor	$11,$11,$2
	srl	$2,$3,16
	xor	$11,$11,$2
	xor	$11,$11,$20
	lui	$2,%hi(v6_path_table_list_max)
	lw	$12,%lo(v6_path_table_list_max)($2)
	xor	$11,$11,$fp
	srl	$2,$19,4
	xor	$11,$11,$2
	lw	$2,%lo(v6_table_path)($22)
	srl	$13,$11,12
	sw	$5,132($sp)
	sw	$4,136($sp)
	sw	$3,140($sp)
	sw	$10,112($sp)
	sw	$9,116($sp)
	sw	$8,120($sp)
	sw	$7,124($sp)
	sw	$6,128($sp)
	addiu	$4,$12,-1
	xor	$11,$13,$11
	and	$3,$11,$4
	lw	$4,4($2)
	sll	$5,$3,4
	sll	$3,$3,2
	subu	$3,$5,$3
	addu	$3,$4,$3
	lw	$16,0($3)
	bne	$16,$0,$L121
	.set	noreorder
	.set	nomacro
	j	$L133
	lhu	$3,2($2)
	.set	macro
	.set	reorder

$L103:
	lw	$16,52($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L125
	lw	$2,%lo(v6_table_path)($22)
	.set	macro
	.set	reorder

$L121:
	lw	$2,36($16)
	bne	$2,$19,$L103
	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L103
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$5,$sp,16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L103
	addiu	$4,$16,16
	.set	macro
	.set	reorder

	addiu	$5,$sp,32
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L103
	lw	$2,32($16)
	bne	$2,$23,$L103
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_route_input
	move	$4,$17
	.set	macro
	.set	reorder

	li	$12,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$12,$L125
	lw	$2,%lo(v6_table_path)($22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L126
	lw	$2,%lo(v6_table_path)($22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_ipv6_dst_check
	move	$4,$17
	.set	macro
	.set	reorder

	li	$31,-1			# 0xffffffffffffffff
	beq	$2,$31,$L128
	.set	noreorder
	.set	nomacro
	jal	rtl_check_dst_input
	move	$4,$17
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L128
	lw	$12,248($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$12,$0,$L129
	lw	$2,244($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L134
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$21,$0,$L134
	.set	noreorder
	.set	nomacro
	jal	del_timer
	addiu	$4,$21,52
	.set	macro
	.set	reorder

	lw	$9,4($21)
	lw	$8,8($21)
	lw	$4,12($21)
	lw	$3,16($21)
	lw	$10,0($21)
	lhu	$19,38($21)
	sw	$9,224($sp)
	sw	$8,228($sp)
	sw	$4,232($sp)
	sw	$3,236($sp)
	lw	$7,20($21)
	xor	$11,$8,$9
	lw	$6,24($21)
	xor	$11,$11,$4
	lw	$5,28($21)
	xor	$11,$11,$3
	lw	$2,32($21)
	xor	$11,$11,$7
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$2
	srl	$9,$9,8
	xor	$9,$11,$9
	srl	$8,$8,8
	xor	$8,$9,$8
	srl	$4,$4,8
	xor	$8,$8,$4
	srl	$3,$3,8
	srl	$4,$7,16
	xor	$8,$8,$3
	xor	$8,$8,$4
	srl	$3,$6,16
	xor	$8,$8,$3
	srl	$3,$5,16
	xor	$8,$8,$3
	lui	$3,%hi(V6_max_udp_frag_entry)
	srl	$4,$2,16
	lw	$9,%lo(V6_max_udp_frag_entry)($3)
	lui	$3,%hi(V6_udp_cache_table)
	xor	$8,$8,$4
	xor	$19,$19,$10
	lw	$4,%lo(V6_udp_cache_table)($3)
	xor	$19,$19,$8
	srl	$8,$8,16
	xor	$19,$19,$8
	addiu	$9,$9,-1
	sw	$2,220($sp)
	sw	$3,288($sp)
	and	$19,$19,$9
	sw	$7,208($sp)
	sw	$6,212($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$5,216($sp)
	.set	macro
	.set	reorder

	lw	$2,80($21)
	sb	$0,48($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L113
	lw	$3,288($sp)
	.set	macro
	.set	reorder

	lw	$5,84($21)
	sll	$4,$19,4
	sll	$19,$19,2
	sw	$5,84($2)
	subu	$19,$4,$19
$L114:
	lw	$4,84($21)
	lw	$5,80($21)
	lui	$2,%hi(V6_udp_cache_list_free)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	sw	$5,0($4)
	lw	$4,%lo(V6_udp_cache_table)($3)
	addiu	$5,$21,88
	lw	$4,4($4)
	addu	$19,$4,$19
	lw	$4,8($19)
	addiu	$4,$4,-1
	sw	$4,8($19)
	sw	$0,88($21)
	lw	$4,4($2)
	sw	$4,92($21)
	lw	$4,4($2)
	sw	$21,0($4)
	lw	$4,8($2)
	lw	$3,%lo(V6_udp_cache_table)($3)
	sw	$5,4($2)
	addiu	$4,$4,1
	sw	$4,8($2)
	lhu	$2,2($3)
	addiu	$2,$2,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$2,2($3)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	move	$4,$17
$L134:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$5,$0
	.set	macro
	.set	reorder

	lbu	$2,7($18)
	sltu	$3,$2,2
	bne	$3,$0,$L115
	addiu	$2,$2,-1
	sb	$2,7($18)
	lw	$5,48($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_skb_dst_mtu
	move	$18,$2
	.set	macro
	.set	reorder

	sltu	$2,$2,$18
	beq	$2,$0,$L116
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_is_gso
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L115
$L116:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_allfrag
	move	$4,$17
	.set	macro
	.set	reorder

	beq	$2,$0,$L130
$L115:
$L117 = .
	.set	noreorder
	.set	nomacro
	jal	kfree_skb
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,%lo(v6_table_path)($22)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L101
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L92:
	lbu	$4,40($18)
	li	$2,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L94
	sw	$3,240($sp)
	.set	macro
	.set	reorder

	li	$fp,44			# 0x2c
	move	$21,$0
	sw	$0,244($sp)
	.set	noreorder
	.set	nomacro
	j	$L93
	sw	$0,248($sp)
	.set	macro
	.set	reorder

$L126:
$L125:
	lhu	$3,2($2)
$L133:
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L101
	move	$2,$0
	.set	macro
	.set	reorder

$L90:
	lw	$4,12($3)
	li	$2,458752			# 0x70000
	and	$2,$4,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L91
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L135
	lw	$31,332($sp)
	.set	macro
	.set	reorder

$L94:
	andi	$4,$20,0xfff9
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L95
	andi	$2,$20,0xfff8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L97
	sw	$0,244($sp)
	.set	macro
	.set	reorder

	andi	$20,$20,0x1
	xori	$20,$20,0x1
	sw	$20,244($sp)
$L97:
	lw	$11,16($sp)
	lw	$10,20($sp)
	lw	$9,24($sp)
	lw	$8,28($sp)
	lw	$7,32($sp)
	xor	$12,$10,$11
	lw	$6,36($sp)
	xor	$12,$12,$9
	lw	$5,40($sp)
	xor	$12,$12,$8
	lw	$2,44($sp)
	xor	$12,$12,$7
	xor	$12,$12,$6
	xor	$12,$12,$5
	xor	$12,$12,$2
	srl	$4,$11,8
	xor	$12,$12,$4
	srl	$4,$10,8
	xor	$12,$12,$4
	srl	$4,$9,8
	xor	$12,$12,$4
	srl	$4,$8,8
	xor	$12,$12,$4
	srl	$4,$7,16
	lw	$16,4($3)
	xor	$12,$12,$4
	srl	$3,$6,16
	xor	$12,$12,$3
	srl	$3,$5,16
	xor	$12,$12,$3
	lui	$3,%hi(V6_max_udp_frag_entry)
	srl	$4,$2,16
	lw	$13,%lo(V6_max_udp_frag_entry)($3)
	lui	$3,%hi(V6_udp_cache_table)
	xor	$12,$12,$4
	xori	$19,$16,0x11
	lw	$4,%lo(V6_udp_cache_table)($3)
	xor	$19,$19,$12
	srl	$12,$12,16
	xor	$19,$19,$12
	addiu	$13,$13,-1
	and	$19,$19,$13
	sw	$5,72($sp)
	sw	$2,76($sp)
	sw	$5,88($sp)
	sw	$2,92($sp)
	sw	$3,288($sp)
	sw	$11,48($sp)
	sw	$10,52($sp)
	sw	$9,56($sp)
	sw	$8,60($sp)
	sw	$7,64($sp)
	sw	$6,68($sp)
	sw	$11,96($sp)
	sw	$10,100($sp)
	sw	$9,104($sp)
	sw	$8,108($sp)
	sw	$7,80($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$6,84($sp)
	.set	macro
	.set	reorder

	lw	$3,288($sp)
	sll	$5,$19,4
	sll	$19,$19,2
	lw	$2,%lo(V6_udp_cache_table)($3)
	subu	$19,$5,$19
	lw	$4,4($2)
	addu	$4,$4,$19
	lw	$21,0($4)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L125
	addiu	$19,$sp,48
	.set	macro
	.set	reorder

	addiu	$22,$sp,64
	.set	noreorder
	.set	nomacro
	j	$L122
	li	$20,17			# 0x11
	.set	macro
	.set	reorder

$L98:
	lw	$21,80($21)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L125
	lw	$2,%lo(V6_udp_cache_table)($3)
	.set	macro
	.set	reorder

$L122:
	lw	$2,0($21)
	.set	noreorder
	.set	nomacro
	bne	$16,$2,$L98
	addiu	$4,$21,4
	.set	macro
	.set	reorder

	move	$5,$19
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcmp
	sw	$3,288($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L98
	lw	$3,288($sp)
	.set	macro
	.set	reorder

	addiu	$4,$21,20
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L98
	lw	$3,288($sp)
	.set	macro
	.set	reorder

	lw	$2,36($21)
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L98
	lw	$2,%lo(V6_udp_cache_table)($3)
	.set	macro
	.set	reorder

	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L89
	li	$fp,17			# 0x11
	.set	macro
	.set	reorder

	lw	$19,40($21)
	lw	$20,44($21)
	sw	$0,248($sp)
	.set	noreorder
	.set	nomacro
	j	$L93
	li	$23,17			# 0x11
	.set	macro
	.set	reorder

$L95:
	lw	$2,240($sp)
	li	$3,1			# 0x1
	lhu	$20,50($18)
	lhu	$19,8($2)
	li	$fp,17			# 0x11
	move	$21,$0
	sw	$0,244($sp)
	sw	$3,248($sp)
	.set	noreorder
	.set	nomacro
	j	$L93
	li	$23,17			# 0x11
	.set	macro
	.set	reorder

$L130:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	sw	$2,44($16)
	lw	$2,%lo(v6_table_path)($22)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	ip_finish_output6
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$3,%hi(statistic_ipv6_fp)
	lw	$4,%lo(statistic_ipv6_fp)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L101
	sw	$4,%lo(statistic_ipv6_fp)($3)
	.set	macro
	.set	reorder

$L129:
	lw	$31,240($sp)
	lui	$3,%hi(V6_udp_cache_table)
	lui	$12,%hi(V6_max_udp_frag_entry)
	lw	$11,4($31)
	lw	$10,16($sp)
	lw	$9,20($sp)
	lw	$7,28($sp)
	lw	$6,32($sp)
	lw	$5,36($sp)
	lw	$2,40($sp)
	lw	$8,24($sp)
	lw	$4,%lo(V6_udp_cache_table)($3)
	lw	$12,%lo(V6_max_udp_frag_entry)($12)
	lw	$21,44($sp)
	sw	$10,144($sp)
	sw	$9,148($sp)
	sw	$8,152($sp)
	sw	$7,156($sp)
	sw	$6,160($sp)
	sw	$5,164($sp)
	sw	$2,168($sp)
	sw	$10,192($sp)
	sw	$9,196($sp)
	sw	$8,200($sp)
	sw	$7,204($sp)
	sw	$6,176($sp)
	sw	$5,180($sp)
	sw	$2,184($sp)
	sw	$2,284($sp)
	sw	$3,288($sp)
	sw	$5,280($sp)
	sw	$6,276($sp)
	sw	$7,272($sp)
	sw	$21,172($sp)
	sw	$21,188($sp)
	sw	$8,268($sp)
	sw	$9,264($sp)
	sw	$10,260($sp)
	sw	$11,252($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$12,256($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi(V6_udp_cache_list_free)
	lw	$13,%lo(V6_udp_cache_list_free)($4)
	lw	$2,284($sp)
	lw	$3,288($sp)
	lw	$5,280($sp)
	lw	$6,276($sp)
	lw	$7,272($sp)
	lw	$8,268($sp)
	lw	$9,264($sp)
	lw	$10,260($sp)
	lw	$11,252($sp)
	.set	noreorder
	.set	nomacro
	beq	$13,$0,$L132
	lw	$12,256($sp)
	.set	macro
	.set	reorder

	sw	$11,0($13)
	lw	$24,148($sp)
	lw	$15,152($sp)
	lw	$14,156($sp)
	lw	$25,144($sp)
	sw	$24,8($13)
	sw	$14,16($13)
	sw	$25,4($13)
	sw	$15,12($13)
	lw	$15,172($sp)
	lw	$25,164($sp)
	lw	$24,168($sp)
	lw	$31,160($sp)
	lw	$14,88($13)
	andi	$19,$19,0xffff
	andi	$20,$20,0xffff
	sw	$15,32($13)
	li	$15,1			# 0x1
	sw	$31,20($13)
	sw	$25,24($13)
	sw	$24,28($13)
	sw	$23,36($13)
	sw	$19,40($13)
	sw	$20,44($13)
	.set	noreorder
	.set	nomacro
	beq	$14,$0,$L110
	sb	$15,48($13)
	.set	macro
	.set	reorder

	lw	$15,92($13)
	addiu	$4,$4,%lo(V6_udp_cache_list_free)
	sw	$15,92($14)
$L111:
	xor	$24,$9,$10
	xor	$24,$24,$8
	xor	$24,$24,$7
	xor	$24,$24,$6
	xor	$24,$24,$5
	xor	$24,$24,$2
	lw	$14,92($13)
	lw	$15,88($13)
	xor	$24,$24,$21
	srl	$10,$10,8
	xor	$10,$24,$10
	srl	$9,$9,8
	xor	$9,$10,$9
	srl	$8,$8,8
	sw	$15,0($14)
	xor	$8,$9,$8
	srl	$7,$7,8
	xor	$7,$8,$7
	srl	$6,$6,16
	lw	$8,8($4)
	xor	$6,$7,$6
	srl	$5,$5,16
	xor	$5,$6,$5
	srl	$2,$2,16
	xor	$2,$5,$2
	srl	$21,$21,16
	addiu	$5,$8,-1
	sw	$5,8($4)
	xor	$21,$2,$21
	sw	$0,80($13)
	lw	$2,%lo(V6_udp_cache_table)($3)
	xor	$11,$21,$11
	srl	$21,$21,16
	xor	$11,$11,$21
	xor	$fp,$11,$fp
	addiu	$12,$12,-1
	lw	$4,4($2)
	and	$12,$fp,$12
	sll	$2,$12,4
	sll	$12,$12,2
	subu	$2,$2,$12
	addu	$4,$4,$2
	lw	$4,4($4)
	addiu	$6,$13,80
	sw	$4,84($13)
	lw	$5,%lo(V6_udp_cache_table)($3)
	addiu	$4,$13,52
	lw	$5,4($5)
	addu	$5,$5,$2
	lw	$5,4($5)
	sw	$13,0($5)
	lw	$5,%lo(V6_udp_cache_table)($3)
	lw	$5,4($5)
	addu	$5,$5,$2
	sw	$6,4($5)
	lw	$5,%lo(V6_udp_cache_table)($3)
	lw	$5,4($5)
	addu	$2,$5,$2
	lw	$5,8($2)
	addiu	$5,$5,1
	sw	$5,8($2)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	sw	$2,60($13)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$3,288($sp)
	.set	macro
	.set	reorder

	lw	$3,288($sp)
	lw	$2,%lo(V6_udp_cache_table)($3)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L134
	move	$4,$17
	.set	macro
	.set	reorder

$L132:
	lw	$2,%lo(V6_udp_cache_table)($3)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
$L128:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L125
	lw	$2,%lo(v6_table_path)($22)
	.set	macro
	.set	reorder

$L113:
	lw	$2,%lo(V6_udp_cache_table)($3)
	sll	$5,$19,4
	lw	$4,84($21)
	lw	$2,4($2)
	sll	$19,$19,2
	subu	$19,$5,$19
	addu	$2,$2,$19
	.set	noreorder
	.set	nomacro
	j	$L114
	sw	$4,4($2)
	.set	macro
	.set	reorder

$L110:
	lw	$14,92($13)
	addiu	$4,$4,%lo(V6_udp_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L111
	sw	$14,4($4)
	.set	macro
	.set	reorder

	.end	ipv6_fast_enter
	.size	ipv6_fast_enter, .-ipv6_fast_enter
	.section	.text.V6_udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	V6_udp_fragCache_init
	.set	nomips16
	.ent	V6_udp_fragCache_init
	.type	V6_udp_fragCache_init, @function
V6_udp_fragCache_init:
	.frame	$sp,56,$31		# vars= 8, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$20,$4
	lw	$4,20($19)
	li	$5,32800			# 0x8020
	jal	kmem_cache_alloc
	lui	$17,%hi(V6_udp_cache_table)

	lui	$3,%hi($LC2)
	lw	$3,%lo($LC2)($3)
	sw	$2,%lo(V6_udp_cache_table)($17)
	sw	$3,0($2)
	lw	$21,%lo(V6_udp_cache_table)($17)
	beq	$21,$0,$L137
	lui	$2,%hi(V6_udp_cache_list_free)

	sll	$3,$20,2
	sll	$4,$20,4
	subu	$4,$4,$3
	addiu	$16,$2,%lo(V6_udp_cache_list_free)
	lui	$18,%hi(V6_max_udp_frag_entry)
	li	$5,32			# 0x20
	sw	$0,%lo(V6_udp_cache_list_free)($2)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$20,%lo(V6_max_udp_frag_entry)($18)

	sw	$2,4($21)
	lw	$3,%lo(V6_udp_cache_table)($17)
	lw	$4,4($3)
	beq	$4,$0,$L139
	li	$2,-1			# 0xffffffffffffffff

	blez	$20,$L140
	move	$2,$0

	move	$3,$0
	j	$L141
	move	$5,$17

$L146:
	lw	$4,%lo(V6_udp_cache_table)($5)
	lw	$4,4($4)
$L141:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(V6_udp_cache_table)($17)
	addiu	$3,$3,1
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(V6_udp_cache_table)($17)
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$20,$L146
	addiu	$2,$2,12

$L140:
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	blez	$2,$L142
	lui	$20,%hi(V6_cache_timeout)

	addiu	$20,$20,%lo(V6_cache_timeout)
	j	$L143
	move	$17,$0

$L147:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,72($2)
	sw	$20,68($2)
	sw	$0,88($2)
	lw	$3,4($16)
	addiu	$4,$2,88
	sw	$3,92($2)
	lw	$3,4($16)
	sw	$2,0($3)
	lw	$3,8($16)
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	sw	$4,4($16)
	addiu	$3,$3,1
	slt	$2,$17,$2
	beq	$2,$0,$L142
	sw	$3,8($16)

$L143:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,52
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L147
	move	$7,$0

$L137:
	move	$2,$0
$L139:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L142:
	j	$L139
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	V6_udp_fragCache_init
	.size	V6_udp_fragCache_init, .-V6_udp_fragCache_init
	.section	.text.rtl_V6_Cache_Timer_update,"ax",@progbits
	.align	2
	.globl	rtl_V6_Cache_Timer_update
	.set	nomips16
	.ent	rtl_V6_Cache_Timer_update
	.type	rtl_V6_Cache_Timer_update, @function
rtl_V6_Cache_Timer_update:
	.frame	$sp,88,$31		# vars= 48, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$19,80($sp)
	sw	$18,76($sp)
	sw	$17,72($sp)
	sw	$16,68($sp)
	lui	$2,%hi(jiffies)
	move	$5,$0
	lw	$17,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	move	$16,$4

	li	$3,17			# 0x11
	beq	$2,$3,$L156
	move	$4,$16

	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L157
	lw	$31,84($sp)

$L159:
	lw	$19,80($sp)
	lw	$18,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	li	$2,-1			# 0xffffffffffffffff
	j	$31
	addiu	$sp,$sp,88

$L156:
	jal	rtl_get_ct_udp_status
	move	$4,$16

	andi	$2,$2,0x2
	bne	$2,$0,$L158
	nop

	move	$4,$0
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	move	$19,$2
$L151:
	addiu	$18,$sp,16
	li	$6,44			# 0x2c
	move	$5,$0
	jal	memset
	move	$4,$18

	move	$4,$16
	jal	rtl_get_ct_protonum_ipv6
	move	$5,$0

	move	$5,$0
	move	$6,$0
	move	$7,$18
	move	$4,$16
	jal	rtl_get_ct_v6_ip_by_dir
	sw	$2,48($sp)

	addiu	$7,$sp,32
	move	$4,$16
	move	$5,$0
	jal	rtl_get_ct_v6_ip_by_dir
	li	$6,1			# 0x1

	move	$4,$16
	move	$5,$0
	jal	rtl_get_ct_port_by_dir
	move	$6,$0

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,52($sp)

	move	$4,$18
	move	$5,$19
	jal	rtk_idleV6Connection
	sw	$2,56($sp)

	beq	$2,$0,$L159
	lw	$31,84($sp)

	addu	$5,$19,$17
	jal	rtl_check_for_acc
	move	$4,$16

	lw	$31,84($sp)
	lw	$19,80($sp)
	lw	$18,76($sp)
	lw	$17,72($sp)
	lw	$16,68($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,88

$L158:
	li	$4,1			# 0x1
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	j	$L151
	move	$19,$2

$L157:
	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L159
	lw	$31,84($sp)

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	j	$L151
	move	$19,$2

	.set	macro
	.set	reorder
	.end	rtl_V6_Cache_Timer_update
	.size	rtl_V6_Cache_Timer_update, .-rtl_V6_Cache_Timer_update
	.section	.text.rtk_addV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_addV6Connection
	.set	nomips16
	.ent	rtk_addV6Connection
	.type	rtk_addV6Connection, @function
rtk_addV6Connection:
	.frame	$sp,176,$31		# vars= 120, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-176
	sw	$31,172($sp)
	sw	$fp,168($sp)
	sw	$23,164($sp)
	sw	$22,160($sp)
	sw	$21,156($sp)
	sw	$20,152($sp)
	sw	$19,148($sp)
	sw	$18,144($sp)
	sw	$17,140($sp)
	sw	$16,136($sp)
	move	$19,$5
	move	$16,$4
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_ipv6_dst
	move	$4,$5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ip6_route_input
	sw	$2,120($sp)
	.set	macro
	.set	reorder

	li	$4,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L162
	move	$3,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	beq	$2,$0,$L186
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_ipv6_dst_dev
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release
	sw	$2,132($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$2,132($sp)
	sw	$2,128($sp)
$L164:
	lui	$20,%hi(v6_table_path)
	lw	$4,%lo(v6_table_path)($20)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lui	$23,%hi(v6_path_table_list_max)
	.set	macro
	.set	reorder

	lw	$10,0($16)
	lw	$11,40($16)
	lw	$9,4($16)
	lw	$8,8($16)
	lw	$7,12($16)
	xor	$11,$10,$11
	lw	$6,16($16)
	xor	$11,$11,$9
	lw	$5,20($16)
	xor	$11,$11,$8
	lw	$4,24($16)
	xor	$11,$11,$7
	lw	$3,28($16)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$12,$5,16
	lw	$2,36($16)
	xor	$11,$11,$12
	srl	$12,$4,16
	srl	$13,$3,16
	xor	$11,$11,$12
	lhu	$12,34($16)
	xor	$11,$11,$13
	srl	$13,$2,4
	xor	$11,$11,$13
	lw	$13,%lo(v6_path_table_list_max)($23)
	xor	$11,$11,$12
	lw	$12,%lo(v6_table_path)($20)
	srl	$14,$11,12
	sw	$4,72($sp)
	sw	$3,76($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$7,60($sp)
	sw	$6,64($sp)
	sw	$5,68($sp)
	addiu	$4,$13,-1
	xor	$11,$14,$11
	and	$3,$11,$4
	lw	$4,4($12)
	sll	$21,$3,4
	sll	$3,$3,2
	subu	$21,$21,$3
	addu	$3,$4,$21
	lw	$17,0($3)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L165
	addiu	$18,$16,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L193
	lw	$3,36($17)
	.set	macro
	.set	reorder

$L166:
	lw	$17,52($17)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L190
	lui	$22,%hi(v6_path_list_free)
	.set	macro
	.set	reorder

	lw	$2,36($16)
	lw	$3,36($17)
$L193:
	bne	$3,$2,$L166
	lw	$3,40($17)
	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L166
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L166
	addiu	$4,$17,16
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L166
	lw	$12,32($17)
	lw	$2,32($16)
	bne	$12,$2,$L166
	.set	noreorder
	.set	nomacro
	j	$L189
	lw	$10,16($16)
	.set	macro
	.set	reorder

$L175:
	lui	$22,%hi(v6_path_list_free)
$L192:
	lw	$18,%lo(v6_path_list_free)($22)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L191
	lw	$2,%lo(v6_table_path)($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	sw	$2,132($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$17,$2
	.set	macro
	.set	reorder

	sw	$2,116($sp)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$4,16($17)
	lw	$2,20($17)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	lw	$6,24($17)
	lw	$5,28($17)
	lw	$4,32($17)
	lw	$2,36($17)
	lhu	$7,40($17)
	addiu	$23,$17,40
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$4,40($sp)
	sw	$2,44($sp)
	sw	$7,112($sp)
	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	lhu	$fp,2($23)
	.set	macro
	.set	reorder

	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	lw	$2,28($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,40($16)
	sh	$2,40($17)
	lw	$2,12($16)
	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,36($16)
	sh	$2,2($23)
	lw	$3,132($sp)
	li	$2,34525			# 0x86dd
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L187
	lui	$2,%hi(nf_hooks+640)
	.set	macro
	.set	reorder

$L179:
	lw	$5,116($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$5,16($sp)
	sw	$4,12($17)
	sw	$3,16($17)
	sw	$5,8($17)
	sw	$2,20($17)
	lw	$2,112($sp)
	sh	$2,40($17)
	lw	$4,36($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	lw	$5,32($sp)
	sw	$4,28($17)
	sw	$3,36($17)
	sw	$5,24($17)
	sw	$2,32($17)
	sh	$fp,2($23)
	lw	$5,16($16)
	lw	$4,20($16)
	lw	$3,24($16)
	lw	$2,28($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$2,12($16)
	lw	$3,8($16)
	lw	$5,0($16)
	lw	$4,4($16)
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$3,24($18)
	sw	$2,28($18)
	lw	$3,32($16)
	lw	$2,60($18)
	sw	$3,32($18)
	lw	$3,40($16)
	sw	$3,36($18)
	lw	$3,36($16)
	sw	$3,40($18)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L182
	sw	$3,44($18)
	.set	macro
	.set	reorder

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
$L183:
	lw	$3,64($18)
	lw	$4,60($18)
	lui	$2,%hi(v6_path_list_inuse)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	sw	$4,0($3)
	lw	$4,8($22)
	addiu	$5,$18,60
	addiu	$3,$18,52
	addiu	$4,$4,-1
	sw	$4,8($22)
	sw	$0,60($18)
	lw	$4,4($2)
	sw	$4,64($18)
	lw	$4,4($2)
	sw	$18,0($4)
	lw	$4,8($2)
	sw	$5,4($2)
	addiu	$4,$4,1
	sw	$4,8($2)
	sw	$0,52($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$2,56($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	sw	$3,4($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$21,$2,$21
	lw	$2,8($21)
	addiu	$2,$2,1
	sw	$2,8($21)
	lw	$2,%lo(v6_table_path)($20)
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	li	$3,1			# 0x1
$L162:
	lw	$31,172($sp)
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L165:
	lui	$22,%hi(v6_path_list_free)
$L190:
	lw	$18,%lo(v6_path_list_free)($22)
	beq	$18,$0,$L185
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_ipv6_hdr
	sw	$2,132($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$17,$2
	.set	macro
	.set	reorder

	sw	$2,124($sp)
	lw	$6,8($17)
	lw	$5,12($17)
	lw	$4,16($17)
	lw	$2,20($17)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$2,28($sp)
	lw	$6,24($17)
	lw	$5,28($17)
	lw	$4,32($17)
	lw	$2,36($17)
	lhu	$7,40($17)
	addiu	$fp,$17,40
	sw	$6,32($sp)
	sw	$5,36($sp)
	sw	$4,40($sp)
	sw	$7,116($sp)
	sw	$2,44($sp)
	lhu	$2,2($fp)
	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	sw	$2,112($sp)
	.set	macro
	.set	reorder

	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	lw	$2,12($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,36($16)
	sh	$2,40($17)
	lw	$2,28($16)
	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,40($16)
	sh	$2,2($fp)
	lw	$3,132($sp)
	li	$2,34525			# 0x86dd
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L188
	lui	$2,%hi(nf_hooks+640)
	.set	macro
	.set	reorder

$L170:
	lw	$5,124($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lw	$3,24($sp)
	lw	$2,28($sp)
	lw	$5,16($sp)
	sw	$4,12($17)
	sw	$3,16($17)
	sw	$5,8($17)
	sw	$2,20($17)
	lw	$7,116($sp)
	sh	$7,40($17)
	lw	$4,36($sp)
	lw	$2,40($sp)
	lw	$3,44($sp)
	lw	$5,32($sp)
	sw	$4,28($17)
	sw	$3,36($17)
	sw	$5,24($17)
	sw	$2,32($17)
	lw	$2,112($sp)
	sh	$2,2($fp)
	lw	$5,0($16)
	lw	$4,4($16)
	lw	$3,8($16)
	lw	$2,12($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$3,24($16)
	lw	$2,28($16)
	lw	$5,16($16)
	lw	$4,20($16)
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$2,28($18)
	sw	$3,24($18)
	lw	$3,32($16)
	lw	$2,60($18)
	sw	$3,32($18)
	lw	$3,36($16)
	sw	$3,36($18)
	lw	$3,40($16)
	sw	$3,40($18)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L173
	sw	$3,44($18)
	.set	macro
	.set	reorder

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
$L174:
	lw	$3,64($18)
	lw	$4,60($18)
	lui	$2,%hi(v6_path_list_inuse)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	sw	$4,0($3)
	lw	$4,8($22)
	addiu	$5,$18,60
	addiu	$3,$18,52
	addiu	$4,$4,-1
	sw	$4,8($22)
	sw	$0,60($18)
	lw	$4,4($2)
	sw	$4,64($18)
	lw	$4,4($2)
	sw	$18,0($4)
	lw	$4,8($2)
	sw	$5,4($2)
	addiu	$4,$4,1
	sw	$4,8($2)
	sw	$0,52($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$2,56($18)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$2,$2,$21
	sw	$3,4($2)
	lw	$2,%lo(v6_table_path)($20)
	lw	$2,4($2)
	addu	$21,$2,$21
	lw	$2,8($21)
	addiu	$2,$2,1
	sw	$2,8($21)
$L185:
	lw	$12,32($16)
$L167 = .
	lw	$10,16($16)
$L189:
	lw	$11,36($16)
	lw	$9,20($16)
	lw	$8,24($16)
	lw	$7,28($16)
	xor	$11,$10,$11
	lw	$6,0($16)
	xor	$11,$11,$9
	lw	$5,4($16)
	xor	$11,$11,$8
	lw	$4,8($16)
	xor	$11,$11,$7
	lw	$3,12($16)
	xor	$11,$11,$6
	xor	$11,$11,$5
	xor	$11,$11,$4
	xor	$11,$11,$3
	srl	$2,$10,8
	xor	$11,$11,$2
	srl	$2,$9,8
	xor	$11,$11,$2
	srl	$2,$8,8
	xor	$11,$11,$2
	srl	$2,$7,8
	xor	$11,$11,$2
	srl	$2,$6,16
	xor	$11,$11,$2
	srl	$13,$5,16
	lw	$2,40($16)
	xor	$11,$11,$13
	srl	$13,$4,16
	xor	$11,$11,$13
	srl	$13,$3,16
	xor	$11,$11,$13
	srl	$13,$2,4
	xor	$11,$11,$13
	andi	$12,$12,0xffff
	lw	$13,%lo(v6_path_table_list_max)($23)
	xor	$11,$11,$12
	lw	$12,%lo(v6_table_path)($20)
	srl	$14,$11,12
	sw	$4,104($sp)
	sw	$3,108($sp)
	sw	$10,80($sp)
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$7,92($sp)
	sw	$6,96($sp)
	sw	$5,100($sp)
	addiu	$4,$13,-1
	xor	$11,$14,$11
	and	$3,$11,$4
	lw	$4,4($12)
	sll	$21,$3,4
	sll	$3,$3,2
	subu	$21,$21,$3
	addu	$3,$4,$21
	lw	$17,0($3)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L175
	addiu	$18,$16,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L194
	lw	$3,36($17)
	.set	macro
	.set	reorder

$L176:
	lw	$17,52($17)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L192
	lui	$22,%hi(v6_path_list_free)
	.set	macro
	.set	reorder

	lw	$2,40($16)
	lw	$3,36($17)
$L194:
	bne	$3,$2,$L176
	lw	$3,40($17)
	lw	$2,36($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L176
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L176
	addiu	$4,$17,16
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	bne	$2,$0,$L176
	lw	$3,32($17)
	lw	$2,32($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L176
	lw	$2,%lo(v6_table_path)($20)
	.set	macro
	.set	reorder

$L191:
	lhu	$3,2($2)
	addiu	$3,$3,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$3,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$31,172($sp)
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	move	$3,$0
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L188:
	lw	$3,%lo(nf_hooks+640)($2)
	addiu	$2,$2,%lo(nf_hooks+640)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L171
	lw	$6,120($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$7,$0
	.set	macro
	.set	reorder

$L171:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,%lo(nf_hooks+672)($2)
	addiu	$2,$2,%lo(nf_hooks+672)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L172
	lw	$7,128($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	li	$5,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$6,$0
	.set	macro
	.set	reorder

$L172:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L170
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L187:
	lw	$3,%lo(nf_hooks+640)($2)
	addiu	$2,$2,%lo(nf_hooks+640)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L180
	lw	$6,128($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$7,$0
	.set	macro
	.set	reorder

$L180:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,%lo(nf_hooks+672)($2)
	addiu	$2,$2,%lo(nf_hooks+672)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L181
	lw	$7,120($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	li	$5,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt6_do_table
	move	$6,$0
	.set	macro
	.set	reorder

$L181:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L179
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L186:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ipv6_dst
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L164
	sw	$0,128($sp)
	.set	macro
	.set	reorder

$L173:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	.set	noreorder
	.set	nomacro
	j	$L174
	sw	$2,4($22)
	.set	macro
	.set	reorder

$L182:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	.set	noreorder
	.set	nomacro
	j	$L183
	sw	$2,4($22)
	.set	macro
	.set	reorder

	.end	rtk_addV6Connection
	.size	rtk_addV6Connection, .-rtk_addV6Connection
	.globl	fast_ipv6_fw
	.section	.bss.fast_ipv6_fw,"aw",@nobits
	.align	2
	.type	fast_ipv6_fw, @object
	.size	fast_ipv6_fw, 4
fast_ipv6_fw:
	.space	4
	.globl	ipv6_fast_forward_proc_fops
	.section	.data.ipv6_fast_forward_proc_fops,"aw",@progbits
	.align	2
	.type	ipv6_fast_forward_proc_fops, @object
	.size	ipv6_fast_forward_proc_fops, 108
ipv6_fast_forward_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	ipv6_fast_forward_single_write
	.space	28
	.word	ipv6_fast_forward_single_open
	.space	4
	.word	single_release
	.space	52
	.section	.bss.ipv6_res,"aw",@nobits
	.align	2
	.type	ipv6_res, @object
	.size	ipv6_res, 4
ipv6_res:
	.space	4
	.section	.bss.v6_path_table_list_max,"aw",@nobits
	.align	2
	.type	v6_path_table_list_max, @object
	.size	v6_path_table_list_max, 4
v6_path_table_list_max:
	.space	4
	.section	.bss.V6_max_udp_frag_entry,"aw",@nobits
	.align	2
	.type	V6_max_udp_frag_entry, @object
	.size	V6_max_udp_frag_entry, 4
V6_max_udp_frag_entry:
	.space	4
	.globl	v6_path_list_inuse
	.section	.bss.v6_path_list_inuse,"aw",@nobits
	.align	2
	.type	v6_path_list_inuse, @object
	.size	v6_path_list_inuse, 12
v6_path_list_inuse:
	.space	12
	.globl	v6_path_list_free
	.section	.bss.v6_path_list_free,"aw",@nobits
	.align	2
	.type	v6_path_list_free, @object
	.size	v6_path_list_free, 12
v6_path_list_free:
	.space	12
	.globl	v6_table_path
	.section	.bss.v6_table_path,"aw",@nobits
	.align	2
	.type	v6_table_path, @object
	.size	v6_table_path, 4
v6_table_path:
	.space	4
	.globl	V6_udp_cache_list_free
	.section	.bss.V6_udp_cache_list_free,"aw",@nobits
	.align	2
	.type	V6_udp_cache_list_free, @object
	.size	V6_udp_cache_list_free, 12
V6_udp_cache_list_free:
	.space	12
	.globl	V6_udp_cache_table
	.section	.bss.V6_udp_cache_table,"aw",@nobits
	.align	2
	.type	V6_udp_cache_table, @object
	.size	V6_udp_cache_table, 4
V6_udp_cache_table:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 1459) 4.4.7"
