	.file	1 "fastpath_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	cache_timeout
	.type	cache_timeout, @function
cache_timeout:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lbu	$2,0($4)
	beq	$2,$0,$L6
	nop

	lw	$5,8($4)
	lw	$3,4($4)
	lhu	$2,2($4)
	xor	$3,$5,$3
	lui	$5,%hi(max_udp_frag_entry)
	lw	$6,%lo(max_udp_frag_entry)($5)
	srl	$8,$3,16
	xor	$2,$2,$3
	lbu	$7,1($4)
	xor	$2,$2,$8
	addiu	$3,$6,-1
	lw	$5,44($4)
	srl	$6,$2,8
	xor	$2,$2,$7
	sb	$0,0($4)
	xor	$2,$2,$6
	beq	$5,$0,$L3
	and	$2,$2,$3

	lw	$8,48($4)
	sll	$3,$2,2
	sll	$6,$2,4
	lui	$7,%hi(udp_cache_table)
	sw	$8,48($5)
	subu	$6,$6,$3
$L4:
	lw	$5,48($4)
	lui	$2,%hi(udp_cache_list_free)
	lw	$9,44($4)
	addiu	$2,$2,%lo(udp_cache_list_free)
	lui	$3,%hi(statistic_udp_frag_free)
	addiu	$8,$4,52
	sw	$9,0($5)
	lw	$7,%lo(udp_cache_table)($7)
	lw	$5,4($2)
	lw	$7,0($7)
	addu	$6,$7,$6
	lw	$7,8($6)
	addiu	$7,$7,-1
	sw	$7,8($6)
	sw	$5,56($4)
	sw	$0,52($4)
	sw	$4,0($5)
	lw	$5,8($2)
	lw	$4,%lo(statistic_udp_frag_free)($3)
	addiu	$5,$5,1
	addiu	$4,$4,1
	sw	$8,4($2)
	sw	$5,8($2)
	sw	$4,%lo(statistic_udp_frag_free)($3)
$L6:
	j	$31
	nop

$L3:
	lui	$7,%hi(udp_cache_table)
	sll	$5,$2,2
	lw	$3,%lo(udp_cache_table)($7)
	sll	$6,$2,4
	subu	$6,$6,$5
	lw	$2,0($3)
	lw	$3,48($4)
	addu	$2,$2,$6
	j	$L4
	sw	$3,4($2)

	.set	macro
	.set	reorder
	.end	cache_timeout
	.size	cache_timeout, .-cache_timeout
	.section	.text.negative_cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	negative_cache_timeout
	.type	negative_cache_timeout, @function
negative_cache_timeout:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lw	$5,8($4)
	lw	$3,4($4)
	lhu	$2,2($4)
	xor	$3,$5,$3
	lui	$5,%hi(max_udp_frag_entry)
	srl	$8,$3,16
	lw	$5,%lo(max_udp_frag_entry)($5)
	xor	$2,$2,$3
	lbu	$7,1($4)
	xor	$2,$2,$8
	addiu	$3,$5,-1
	lw	$6,40($4)
	srl	$5,$2,8
	xor	$2,$2,$7
	sb	$0,0($4)
	xor	$2,$2,$5
	beq	$6,$0,$L8
	and	$2,$2,$3

	lw	$5,44($4)
	sll	$7,$2,2
	lui	$3,%hi(negative_cache_table)
	sll	$2,$2,4
	sw	$5,44($6)
$L9:
	lw	$5,44($4)
	subu	$7,$2,$7
	lw	$8,40($4)
	lui	$2,%hi(negative_cache_list_free)
	addiu	$2,$2,%lo(negative_cache_list_free)
	addiu	$6,$4,48
	sw	$8,0($5)
	lw	$5,%lo(negative_cache_table)($3)
	lw	$3,4($2)
	addu	$5,$5,$7
	lw	$7,8($5)
	addiu	$7,$7,-1
	sw	$7,8($5)
	sw	$3,52($4)
	sw	$0,48($4)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$6,4($2)
	addiu	$3,$3,1
	j	$31
	sw	$3,8($2)

$L8:
	lui	$3,%hi(negative_cache_table)
	sll	$7,$2,2
	lw	$8,%lo(negative_cache_table)($3)
	sll	$2,$2,4
	lw	$6,44($4)
	subu	$5,$2,$7
	addu	$5,$8,$5
	j	$L9
	sw	$6,4($5)

	.set	macro
	.set	reorder
	.end	negative_cache_timeout
	.size	negative_cache_timeout, .-negative_cache_timeout
	.section	.text.fastpath_napt_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_napt_single_write
	.type	fastpath_napt_single_write, @function
fastpath_napt_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$6

	.set	macro
	.set	reorder
	.end	fastpath_napt_single_write
	.size	fastpath_napt_single_write, .-fastpath_napt_single_write
	.section	.text.fastpath_path_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_path_single_write
	.type	fastpath_path_single_write, @function
fastpath_path_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$6

	.set	macro
	.set	reorder
	.end	fastpath_path_single_write
	.size	fastpath_path_single_write, .-fastpath_path_single_write
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"TCP\000"
	.align	2
$LC1:
	.ascii	"UDP\000"
	.align	2
$LC2:
	.ascii	"~Path: [%s] in-S=0x%08X:%-5u in-D=0x%08X:%-5u out-S=0x%0"
	.ascii	"8X:%-5u out-D=0x%08X:%-5u out-ifname=%-5s <%u> {%d}\000"
	.align	2
$LC3:
	.ascii	",[PreMark:%d,PostMark:%d]\000"
	.align	2
$LC4:
	.ascii	",[policy_route_mark:%d]\000"
	.align	2
$LC5:
	.ascii	",[dscp_in:%d,dscp_out:%d]\000"
	.align	2
$LC6:
	.ascii	",[vlanPrio_out:%d]\000"
	.align	2
$LC7:
	.ascii	"\012\000"
	.section	.text.fastpath_table_path,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_table_path
	.type	fastpath_table_path, @function
fastpath_table_path:
	.frame	$sp,96,$31		# vars= 0, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$fp,88($sp)
	sw	$23,84($sp)
	sw	$22,80($sp)
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$16,%lo(path_list_inuse)($2)
	bne	$16,$0,$L18
	lui	$18,%hi($LC2)

$L13:
	lw	$31,92($sp)
	move	$2,$0
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	j	$31
	addiu	$sp,$sp,96

$L18:
	lui	$17,%hi($LC3)
	lui	$fp,%hi($LC5)
	lui	$23,%hi($LC6)
	lui	$22,%hi($LC7)
	li	$21,2			# 0x2
	lui	$20,%hi($LC1)
	lui	$19,%hi($LC0)
	addiu	$18,$18,%lo($LC2)
	addiu	$17,$17,%lo($LC3)
$L15:
	lw	$2,4($16)
	beq	$2,$21,$L19
	addiu	$5,$20,%lo($LC1)

$L14:
	lw	$2,16($16)
	move	$4,$18
	lhu	$7,10($16)
	lw	$6,12($16)
	sw	$2,16($sp)
	lhu	$2,8($16)
	sw	$2,20($sp)
	lw	$2,20($16)
	sw	$2,24($sp)
	lhu	$2,24($16)
	sw	$2,28($sp)
	lw	$2,28($16)
	sw	$2,32($sp)
	lhu	$2,32($16)
	sw	$2,36($sp)
	lw	$2,40($16)
	sw	$2,40($sp)
	lbu	$2,2($16)
	sw	$2,44($sp)
	lbu	$2,1($16)
	jal	printk
	sw	$2,48($sp)

	lw	$6,48($16)
	lw	$5,44($16)
	jal	printk
	move	$4,$17

	lui	$2,%hi($LC4)
	lw	$5,60($16)
	jal	printk
	addiu	$4,$2,%lo($LC4)

	lbu	$6,73($16)
	lbu	$5,72($16)
	jal	printk
	addiu	$4,$fp,%lo($LC5)

	lhu	$5,78($16)
	jal	printk
	addiu	$4,$23,%lo($LC6)

	jal	printk
	addiu	$4,$22,%lo($LC7)

	lw	$16,88($16)
	beq	$16,$0,$L13
	nop

	j	$L15
	nop

$L19:
	j	$L14
	addiu	$5,$19,%lo($LC0)

	.set	macro
	.set	reorder
	.end	fastpath_table_path
	.size	fastpath_table_path, .-fastpath_table_path
	.section	.rodata.str1.4
	.align	2
$LC8:
	.ascii	"~Napt: [%s] int=0x%08X:%-5u ext=0x%08X:%-5u rem=0x%08X:%"
	.ascii	"-5u flags=0x%08X \012\000"
	.section	.text.fastpath_table_napt,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_table_napt
	.type	fastpath_table_napt, @function
fastpath_table_napt:
	.frame	$sp,64,$31		# vars= 0, regs= 6/0, args= 40, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(napt_list_inuse)
	lw	$16,%lo(napt_list_inuse)($2)
	bne	$16,$0,$L26
	lui	$17,%hi($LC8)

$L21:
	lw	$31,60($sp)
	move	$2,$0
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,64

$L26:
	li	$20,2			# 0x2
	lui	$19,%hi($LC1)
	lui	$18,%hi($LC0)
	addiu	$17,$17,%lo($LC8)
$L23:
	lw	$2,4($16)
	beq	$2,$20,$L27
	addiu	$5,$19,%lo($LC1)

$L22:
	lw	$2,16($16)
	move	$4,$17
	lw	$6,8($16)
	lw	$7,12($16)
	sw	$2,16($sp)
	lw	$2,20($16)
	sw	$2,20($sp)
	lw	$2,24($16)
	sw	$2,24($sp)
	lw	$2,28($16)
	sw	$2,28($sp)
	lw	$2,32($16)
	jal	printk
	sw	$2,32($sp)

	lw	$16,44($16)
	beq	$16,$0,$L21
	nop

	j	$L23
	nop

$L27:
	j	$L22
	addiu	$5,$18,%lo($LC0)

	.set	macro
	.set	reorder
	.end	fastpath_table_napt
	.size	fastpath_table_napt, .-fastpath_table_napt
	.section	.text.fastpath_path_single_open,"ax",@progbits
	.align	2
	.globl	fastpath_path_single_open
	.set	nomips16
	.ent	fastpath_path_single_open
	.type	fastpath_path_single_open, @function
fastpath_path_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(fastpath_table_path)
	move	$6,$0
	addiu	$5,$5,%lo(fastpath_table_path)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_path_single_open
	.size	fastpath_path_single_open, .-fastpath_path_single_open
	.section	.text.fastpath_napt_single_open,"ax",@progbits
	.align	2
	.globl	fastpath_napt_single_open
	.set	nomips16
	.ent	fastpath_napt_single_open
	.type	fastpath_napt_single_open, @function
fastpath_napt_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(fastpath_table_napt)
	move	$6,$0
	addiu	$5,$5,%lo(fastpath_table_napt)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_napt_single_open
	.size	fastpath_napt_single_open, .-fastpath_napt_single_open
	.section	.text.rtl_init_bypass_packet,"ax",@progbits
	.align	2
	.globl	rtl_init_bypass_packet
	.set	nomips16
	.ent	rtl_init_bypass_packet
	.type	rtl_init_bypass_packet, @function
rtl_init_bypass_packet:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	sw	$0,68($4)
	j	$31
	sw	$0,64($4)

	.set	macro
	.set	reorder
	.end	rtl_init_bypass_packet
	.size	rtl_init_bypass_packet, .-rtl_init_bypass_packet
	.section	.text.rtl_check_bypass_packet,"ax",@progbits
	.align	2
	.globl	rtl_check_bypass_packet
	.set	nomips16
	.ent	rtl_check_bypass_packet
	.type	rtl_check_bypass_packet, @function
rtl_check_bypass_packet:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lw	$2,68($4)
	bne	$2,$0,$L35
	li	$3,1			# 0x1

	lw	$2,64($4)
	lui	$3,%hi(fastpath_packet_bypassed_num)
	lw	$3,%lo(fastpath_packet_bypassed_num)($3)
	addiu	$2,$2,1
	sltu	$3,$3,$2
	beq	$3,$0,$L34
	sw	$2,64($4)

	li	$3,1			# 0x1
$L35:
	move	$2,$0
	j	$31
	sw	$3,68($4)

$L34:
	j	$31
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	rtl_check_bypass_packet
	.size	rtl_check_bypass_packet, .-rtl_check_bypass_packet
	.section	.text.udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	udp_fragCache_init
	.set	nomips16
	.ent	udp_fragCache_init
	.type	udp_fragCache_init, @function
udp_fragCache_init:
	.frame	$sp,48,$31		# vars= 8, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,20($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$17,%hi(udp_cache_table)

	move	$20,$2
	beq	$2,$0,$L44
	sw	$2,%lo(udp_cache_table)($17)

	lui	$2,%hi(udp_cache_list_free)
	sll	$3,$18,2
	sll	$4,$18,4
	addiu	$16,$2,%lo(udp_cache_list_free)
	subu	$4,$4,$3
	li	$5,32			# 0x20
	sw	$0,%lo(udp_cache_list_free)($2)
	lui	$2,%hi(max_udp_frag_entry)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$18,%lo(max_udp_frag_entry)($2)

	sw	$2,0($20)
	lw	$2,%lo(udp_cache_table)($17)
	lw	$3,0($2)
	beq	$3,$0,$L41
	nop

	blez	$18,$L42
	move	$4,$0

	j	$L39
	move	$5,$0

$L46:
	lw	$3,0($2)
$L39:
	addu	$3,$3,$4
	addiu	$5,$5,1
	sw	$0,8($3)
	lw	$2,0($2)
	addu	$2,$2,$4
	sw	$0,0($2)
	lw	$2,%lo(udp_cache_table)($17)
	lw	$3,0($2)
	addu	$3,$3,$4
	addiu	$4,$4,12
	bne	$5,$18,$L46
	sw	$3,4($3)

	lui	$20,%hi(cache_timeout)
	move	$17,$0
	j	$L38
	addiu	$20,$20,%lo(cache_timeout)

$L47:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	lw	$3,4($16)
	sw	$2,36($2)
	addiu	$4,$2,52
	sw	$20,32($2)
	sw	$0,52($2)
	sw	$3,56($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$18,$L42
	sw	$2,8($16)

$L38:
	lw	$4,24($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	move	$5,$0
	addiu	$4,$2,16
	move	$6,$0
	bne	$2,$0,$L47
	move	$7,$0

$L44:
	move	$2,$0
$L37:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

$L42:
	lw	$31,44($sp)
	li	$2,1			# 0x1
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

$L41:
	j	$L37
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.size	udp_fragCache_init, .-udp_fragCache_init
	.section	.text.negative_fragCache_init,"ax",@progbits
	.align	2
	.globl	negative_fragCache_init
	.set	nomips16
	.ent	negative_fragCache_init
	.type	negative_fragCache_init, @function
negative_fragCache_init:
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$18,%hi(kmalloc_caches)
	addiu	$18,$18,%lo(kmalloc_caches)
	lw	$4,40($18)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$3,%hi(negative_cache_table)
	beq	$2,$0,$L53
	sw	$2,%lo(negative_cache_table)($3)

	lui	$5,%hi(negative_cache_list_free)
	move	$4,$0
	addiu	$16,$5,%lo(negative_cache_list_free)
	li	$8,64			# 0x40
	sw	$0,8($16)
	sw	$0,%lo(negative_cache_list_free)($5)
	sw	$16,4($16)
$L50:
	sll	$5,$4,2
	lw	$6,%lo(negative_cache_table)($3)
	sll	$7,$4,4
	addiu	$4,$4,1
	subu	$5,$7,$5
	addu	$2,$2,$5
	addu	$6,$6,$5
	sw	$0,8($2)
	sw	$0,0($6)
	lw	$2,%lo(negative_cache_table)($3)
	addu	$5,$2,$5
	bne	$4,$8,$L50
	sw	$5,4($5)

	lui	$19,%hi(negative_cache_timeout)
	li	$17,64			# 0x40
	j	$L51
	addiu	$19,$19,%lo(negative_cache_timeout)

$L57:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	lw	$3,4($16)
	sw	$2,32($2)
	addiu	$4,$2,48
	sw	$19,28($2)
	sw	$0,48($2)
	sw	$3,52($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$0,$L56
	sw	$2,8($16)

$L51:
	lw	$4,24($18)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,-1

	move	$5,$0
	addiu	$4,$2,12
	move	$6,$0
	bne	$2,$0,$L57
	move	$7,$0

$L53:
	lw	$31,44($sp)
	move	$2,$0
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,48

$L56:
	lw	$31,44($sp)
	li	$2,1			# 0x1
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	negative_fragCache_init
	.size	negative_fragCache_init, .-negative_fragCache_init
	.section	.text.rtk_addArp,"ax",@progbits
	.align	2
	.globl	rtk_addArp
	.set	nomips16
	.ent	rtk_addArp
	.type	rtk_addArp, @function
rtk_addArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addArp
	.size	rtk_addArp, .-rtk_addArp
	.section	.text.rtk_modifyArp,"ax",@progbits
	.align	2
	.globl	rtk_modifyArp
	.set	nomips16
	.ent	rtk_modifyArp
	.type	rtk_modifyArp, @function
rtk_modifyArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyArp
	.size	rtk_modifyArp, .-rtk_modifyArp
	.section	.text.rtk_delArp,"ax",@progbits
	.align	2
	.globl	rtk_delArp
	.set	nomips16
	.ent	rtk_delArp
	.type	rtk_delArp, @function
rtk_delArp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delArp
	.size	rtk_delArp, .-rtk_delArp
	.section	.text.rtk_addRoute,"ax",@progbits
	.align	2
	.globl	rtk_addRoute
	.set	nomips16
	.ent	rtk_addRoute
	.type	rtk_addRoute, @function
rtk_addRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addRoute
	.size	rtk_addRoute, .-rtk_addRoute
	.section	.text.rtk_modifyRoute,"ax",@progbits
	.align	2
	.globl	rtk_modifyRoute
	.set	nomips16
	.ent	rtk_modifyRoute
	.type	rtk_modifyRoute, @function
rtk_modifyRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_modifyRoute
	.size	rtk_modifyRoute, .-rtk_modifyRoute
	.section	.text.rtk_delRoute,"ax",@progbits
	.align	2
	.globl	rtk_delRoute
	.set	nomips16
	.ent	rtk_delRoute
	.type	rtk_delRoute, @function
rtk_delRoute:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	li	$2,-4			# 0xfffffffffffffffc

	.set	macro
	.set	reorder
	.end	rtk_delRoute
	.size	rtk_delRoute, .-rtk_delRoute
	.section	.text.rtk_addSession,"ax",@progbits
	.align	2
	.globl	rtk_addSession
	.set	nomips16
	.ent	rtk_addSession
	.type	rtk_addSession, @function
rtk_addSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addSession
	.size	rtk_addSession, .-rtk_addSession
	.section	.text.rtk_delSession,"ax",@progbits
	.align	2
	.globl	rtk_delSession
	.set	nomips16
	.ent	rtk_delSession
	.type	rtk_delSession, @function
rtk_delSession:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_delSession
	.size	rtk_delSession, .-rtk_delSession
	.section	.text.rtk_delNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_delNaptConnection
	.set	nomips16
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.frame	$sp,40,$31		# vars= 8, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	li	$2,17			# 0x11
	lw	$3,0($4)
	move	$16,$4
	li	$17,6			# 0x6
	xori	$3,$3,0x2
	jal	arch_local_irq_save
	movn	$17,$2,$3

	lw	$4,20($16)
	jal	filter_delconnect
	sw	$2,16($sp)

	lui	$9,%hi(napt_table_list_max)
	lw	$6,4($16)
	lui	$10,%hi(table_napt)
	lw	$7,8($16)
	lw	$8,20($16)
	srl	$3,$6,16
	lw	$11,24($16)
	xor	$4,$7,$6
	xor	$4,$4,$8
	lw	$5,0($16)
	xor	$4,$4,$11
	lw	$9,%lo(napt_table_list_max)($9)
	xor	$4,$4,$3
	lw	$12,%lo(table_napt)($10)
	srl	$13,$8,16
	addiu	$9,$9,-1
	andi	$3,$5,0xffff
	lw	$12,0($12)
	xor	$4,$4,$13
	lw	$2,16($sp)
	xor	$4,$4,$3
	srl	$3,$4,12
	xor	$3,$3,$4
	and	$3,$3,$9
	sll	$4,$3,2
	sll	$3,$3,4
	subu	$9,$3,$4
	addu	$3,$12,$9
	lw	$3,0($3)
	bne	$3,$0,$L94
	nop

	j	$L68
	nop

$L69:
	lw	$3,36($3)
	beq	$3,$0,$L68
	nop

$L94:
	lw	$4,4($3)
	bne	$5,$4,$L69
	nop

	lw	$4,8($3)
	bne	$6,$4,$L69
	nop

	lw	$4,12($3)
	bne	$7,$4,$L69
	nop

	lw	$12,16($3)
	lw	$4,12($16)
	bne	$12,$4,$L69
	nop

	lw	$12,20($3)
	lw	$4,16($16)
	bne	$12,$4,$L69
	nop

	lw	$4,24($3)
	bne	$8,$4,$L69
	nop

	lw	$4,28($3)
	bne	$11,$4,$L69
	nop

	lw	$4,36($3)
	beq	$4,$0,$L70
	sb	$0,0($3)

	lw	$5,40($3)
	sw	$5,40($4)
$L71:
	lw	$4,40($3)
	lw	$5,36($3)
	sw	$5,0($4)
	lw	$4,%lo(table_napt)($10)
	lw	$4,0($4)
	addu	$9,$4,$9
	lw	$4,8($9)
	addiu	$4,$4,-1
	sw	$4,8($9)
	lw	$4,44($3)
	beq	$4,$0,$L72
	lui	$6,%hi(napt_list_inuse)

	lw	$5,48($3)
	addiu	$6,$6,%lo(napt_list_inuse)
	sw	$5,48($4)
	lw	$4,48($3)
	lw	$5,44($3)
$L73:
	sw	$5,0($4)
	lui	$4,%hi(napt_list_free)
	addiu	$4,$4,%lo(napt_list_free)
	addiu	$8,$3,44
	lw	$9,8($6)
	lui	$5,%hi(table_path)
	lw	$7,4($4)
	addiu	$9,$9,-1
	sw	$0,44($3)
	sw	$7,48($3)
	sw	$9,8($6)
	sw	$3,0($7)
	lw	$3,8($4)
	sw	$8,4($4)
	lui	$8,%hi(path_table_list_max)
	addiu	$3,$3,1
	sw	$3,8($4)
	lw	$11,4($16)
	lw	$4,20($16)
	lw	$7,24($16)
	srl	$6,$11,8
	xor	$3,$4,$11
	lw	$13,8($16)
	xor	$3,$3,$7
	lw	$12,%lo(path_table_list_max)($8)
	xor	$3,$3,$6
	lw	$8,%lo(table_path)($5)
	srl	$6,$4,16
	addiu	$12,$12,-1
	xor	$6,$3,$6
	lw	$10,0($8)
	srl	$3,$13,4
	xor	$3,$6,$3
	xor	$3,$3,$17
	srl	$6,$3,12
	xor	$3,$6,$3
	and	$3,$3,$12
	sll	$6,$3,2
	sll	$3,$3,4
	subu	$9,$3,$6
	addu	$3,$10,$9
	lw	$3,0($3)
	beq	$3,$0,$L98
	nop

	lw	$15,0($16)
	lw	$6,12($16)
	lw	$8,16($16)
	j	$L81
	li	$14,1			# 0x1

$L76:
	lw	$3,80($3)
	beq	$3,$0,$L75
	nop

$L81:
	lw	$24,4($3)
	bne	$24,$15,$L76
	nop

	lbu	$24,2($3)
	bne	$24,$14,$L76
	nop

	lw	$24,12($3)
	bne	$11,$24,$L76
	nop

	lhu	$24,10($3)
	bne	$13,$24,$L76
	nop

	lw	$24,20($3)
	bne	$24,$6,$L76
	nop

	lhu	$24,24($3)
	bne	$24,$8,$L76
	nop

	lw	$24,16($3)
	bne	$4,$24,$L76
	nop

	lhu	$24,8($3)
	bne	$7,$24,$L76
	nop

	lw	$4,80($3)
	beq	$4,$0,$L77
	sb	$0,0($3)

	lw	$6,84($3)
	sw	$6,84($4)
$L78:
	lw	$4,84($3)
	lw	$6,80($3)
	sw	$6,0($4)
	lw	$4,%lo(table_path)($5)
	lw	$4,0($4)
	addu	$9,$4,$9
	lw	$4,8($9)
	addiu	$4,$4,-1
	sw	$4,8($9)
	lw	$6,88($3)
	beq	$6,$0,$L79
	lui	$4,%hi(path_list_inuse)

	lw	$7,92($3)
	addiu	$4,$4,%lo(path_list_inuse)
	sw	$7,92($6)
	lw	$6,92($3)
	lw	$7,88($3)
$L80:
	sw	$7,0($6)
	lui	$6,%hi(path_list_free)
	addiu	$6,$6,%lo(path_list_free)
	addiu	$8,$3,88
	lw	$9,8($4)
	lw	$7,4($6)
	addiu	$9,$9,-1
	sw	$0,88($3)
	sw	$7,92($3)
	sw	$9,8($4)
	sw	$3,0($7)
	lw	$4,8($6)
	lw	$3,%lo(table_path)($5)
	addiu	$4,$4,1
	sw	$8,4($6)
	sw	$4,8($6)
	lw	$7,24($16)
	lw	$4,20($16)
	lw	$6,12($16)
	lw	$8,16($16)
	lw	$10,0($3)
$L75:
	xor	$9,$4,$6
$L99:
	xor	$9,$9,$17
	srl	$3,$4,8
	xor	$9,$9,$8
	xor	$9,$9,$3
	srl	$3,$6,16
	xor	$9,$9,$3
	srl	$3,$7,4
	xor	$3,$9,$3
	srl	$9,$3,12
	xor	$3,$9,$3
	and	$3,$3,$12
	sll	$9,$3,2
	sll	$3,$3,4
	subu	$9,$3,$9
	addu	$10,$10,$9
	lw	$3,0($10)
	beq	$3,$0,$L82
	li	$10,2			# 0x2

	lw	$11,0($16)
	j	$L88
	nop

$L83:
	lw	$3,80($3)
	beq	$3,$0,$L82
	nop

$L88:
	lw	$12,4($3)
	bne	$12,$11,$L83
	nop

	lbu	$12,2($3)
	bne	$12,$10,$L83
	nop

	lw	$12,16($3)
	bne	$12,$6,$L83
	nop

	lhu	$12,8($3)
	bne	$12,$8,$L83
	nop

	lw	$12,20($3)
	bne	$12,$4,$L83
	nop

	lhu	$12,24($3)
	bne	$12,$7,$L83
	nop

	lw	$13,28($3)
	lw	$12,4($16)
	bne	$13,$12,$L83
	nop

	lhu	$13,32($3)
	lw	$12,8($16)
	bne	$13,$12,$L83
	nop

	lw	$4,80($3)
	beq	$4,$0,$L84
	sb	$0,0($3)

	lw	$6,84($3)
	sw	$6,84($4)
$L85:
	lw	$4,84($3)
	lw	$6,80($3)
	sw	$6,0($4)
	lw	$4,%lo(table_path)($5)
	lw	$4,0($4)
	addu	$9,$4,$9
	lw	$4,8($9)
	addiu	$4,$4,-1
	sw	$4,8($9)
	lw	$5,88($3)
	beq	$5,$0,$L86
	move	$6,$0

	lw	$6,92($3)
	lui	$4,%hi(path_list_inuse)
	addiu	$4,$4,%lo(path_list_inuse)
	sw	$6,92($5)
	lw	$5,92($3)
	lw	$6,88($3)
$L87:
	sw	$6,0($5)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	addiu	$7,$3,88
	lw	$8,8($4)
	lw	$6,4($5)
	addiu	$8,$8,-1
	sw	$0,88($3)
	sw	$6,92($3)
	sw	$8,8($4)
	sw	$3,0($6)
	lw	$3,8($5)
	sw	$7,4($5)
	addiu	$3,$3,1
	sw	$3,8($5)
$L82:
	jal	arch_local_irq_restore
	move	$4,$2

	j	$L90
	move	$2,$0

$L68:
	jal	arch_local_irq_restore
	move	$4,$2

	li	$2,-4			# 0xfffffffffffffffc
$L90:
	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,40

$L86:
	lw	$5,92($3)
	lui	$4,%hi(path_list_inuse)
	addiu	$4,$4,%lo(path_list_inuse)
	j	$L87
	sw	$5,4($4)

$L84:
	lw	$4,%lo(table_path)($5)
	lw	$6,84($3)
	lw	$4,0($4)
	addu	$4,$4,$9
	j	$L85
	sw	$6,4($4)

$L79:
	lw	$6,92($3)
	addiu	$4,$4,%lo(path_list_inuse)
	move	$7,$0
	j	$L80
	sw	$6,4($4)

$L77:
	lw	$4,%lo(table_path)($5)
	lw	$6,84($3)
	lw	$4,0($4)
	addu	$4,$4,$9
	j	$L78
	sw	$6,4($4)

$L98:
	lw	$6,12($16)
	lw	$8,16($16)
	j	$L99
	xor	$9,$4,$6

$L70:
	lw	$4,%lo(table_napt)($10)
	lw	$5,40($3)
	lw	$4,0($4)
	addu	$4,$4,$9
	j	$L71
	sw	$5,4($4)

$L72:
	lw	$4,48($3)
	addiu	$6,$6,%lo(napt_list_inuse)
	move	$5,$0
	j	$L73
	sw	$4,4($6)

	.set	macro
	.set	reorder
	.end	rtk_delNaptConnection
	.size	rtk_delNaptConnection, .-rtk_delNaptConnection
	.section	.text.rtk_idleNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_idleNaptConnection
	.set	nomips16
	.ent	rtk_idleNaptConnection
	.type	rtk_idleNaptConnection, @function
rtk_idleNaptConnection:
	.frame	$sp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x00010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-8
	sw	$16,4($sp)
	li	$7,6			# 0x6
	lw	$8,4($4)
	lui	$15,%hi(jiffies)
	lw	$3,20($4)
	lw	$9,24($4)
	srl	$10,$8,8
	xor	$12,$3,$8
	lw	$2,0($4)
	xor	$12,$12,$9
	lw	$13,8($4)
	xor	$12,$12,$10
	lui	$10,%hi(path_table_list_max)
	lw	$11,%lo(path_table_list_max)($10)
	srl	$10,$3,16
	xori	$14,$2,0x2
	lw	$15,%lo(jiffies)($15)
	xor	$12,$12,$10
	li	$10,17			# 0x11
	movn	$7,$10,$14
	srl	$14,$13,4
	xor	$12,$12,$14
	lui	$10,%hi(table_path)
	xor	$12,$12,$7
	addiu	$11,$11,-1
	srl	$14,$12,12
	lw	$10,%lo(table_path)($10)
	xor	$12,$14,$12
	and	$12,$12,$11
	lw	$10,0($10)
	sll	$14,$12,2
	sll	$12,$12,4
	subu	$12,$12,$14
	addu	$12,$10,$12
	lw	$12,0($12)
	beq	$12,$0,$L115
	li	$14,-16777216			# 0xffffffffff000000

	ori	$24,$14,0xff00
	lw	$25,12($4)
	lw	$16,16($4)
	j	$L106
	addiu	$14,$14,256

$L104:
	lw	$12,80($12)
	beq	$12,$0,$L103
	nop

$L106:
	lw	$4,4($12)
	bne	$2,$4,$L104
	nop

	lw	$4,0($12)
	and	$4,$4,$24
	bne	$4,$14,$L104
	nop

	lw	$4,12($12)
	bne	$8,$4,$L104
	nop

	lhu	$4,10($12)
	bne	$13,$4,$L104
	nop

	lw	$4,20($12)
	bne	$4,$25,$L104
	nop

	lhu	$4,24($12)
	bne	$4,$16,$L104
	nop

	lw	$4,16($12)
	bne	$3,$4,$L104
	nop

	lhu	$4,8($12)
	bne	$9,$4,$L104
	nop

	lw	$4,36($12)
	subu	$12,$15,$5
	subu	$12,$12,$4
	bgez	$12,$L116
	srl	$12,$3,8

	sw	$4,0($6)
	j	$L105
	li	$2,-1			# 0xffffffffffffffff

$L115:
	lw	$25,12($4)
	lw	$16,16($4)
$L103:
	srl	$12,$3,8
$L116:
	srl	$4,$9,4
	xor	$4,$12,$4
	xor	$4,$4,$3
	xor	$4,$4,$7
	xor	$4,$4,$25
	xor	$4,$4,$16
	srl	$12,$25,16
	xor	$4,$4,$12
	srl	$7,$4,12
	xor	$4,$7,$4
	and	$11,$4,$11
	sll	$4,$11,2
	sll	$11,$11,4
	subu	$11,$11,$4
	addu	$10,$10,$11
	lw	$4,0($10)
	beq	$4,$0,$L111
	li	$10,-16777216			# 0xffffffffff000000

	ori	$11,$10,0xff00
	j	$L112
	addiu	$10,$10,512

$L107:
	lw	$4,80($4)
	beq	$4,$0,$L111
	nop

$L112:
	lw	$7,4($4)
	bne	$2,$7,$L107
	nop

	lw	$7,0($4)
	and	$7,$7,$11
	bne	$7,$10,$L107
	nop

	lw	$7,16($4)
	bne	$7,$25,$L107
	nop

	lhu	$7,8($4)
	bne	$7,$16,$L107
	nop

	lw	$7,20($4)
	bne	$3,$7,$L107
	nop

	lhu	$7,24($4)
	bne	$9,$7,$L107
	nop

	lw	$7,28($4)
	bne	$8,$7,$L107
	nop

	lhu	$7,32($4)
	bne	$13,$7,$L107
	nop

	lw	$2,36($4)
	subu	$5,$15,$5
	subu	$5,$5,$2
	bgez	$5,$L111
	nop

	sw	$2,0($6)
	j	$L105
	li	$2,-1			# 0xffffffffffffffff

$L111:
	move	$2,$0
$L105:
	lw	$16,4($sp)
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	rtk_idleNaptConnection
	.size	rtk_idleNaptConnection, .-rtk_idleNaptConnection
	.section	.text.rtl_fpTimer_update,"ax",@progbits
	.align	2
	.globl	rtl_fpTimer_update
	.set	nomips16
	.ent	rtl_fpTimer_update
	.type	rtl_fpTimer_update, @function
rtl_fpTimer_update:
	.frame	$sp,72,$31		# vars= 32, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lui	$2,%hi(jiffies)
	move	$5,$0
	move	$16,$4
	lw	$20,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	nop

	li	$3,17			# 0x11
	beq	$2,$3,$L133
	move	$4,$16

	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	bne	$2,$3,$L121
	li	$2,-1			# 0xffffffffffffffff

	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L128
	move	$4,$16

	jal	rtl_tcp_get_timeouts
	li	$19,2			# 0x2

	move	$5,$0
	move	$6,$0
	move	$4,$16
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$17,$2

	beq	$17,$2,$L137
	move	$4,$16

	move	$5,$0
$L138:
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$17,$2

	beq	$17,$2,$L135
	move	$4,$16

$L128:
	li	$2,-1			# 0xffffffffffffffff
$L121:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

$L133:
	jal	rtl_get_ct_udp_status
	nop

	andi	$2,$2,0x2
	bne	$2,$0,$L129
	li	$4,1			# 0x1

	move	$4,$0
$L129:
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	move	$5,$0
	move	$6,$0
	move	$4,$16
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	move	$17,$2
	jal	rtl_get_ct_ip_by_dir
	li	$19,1			# 0x1

	bne	$17,$2,$L138
	move	$5,$0

	move	$4,$16
$L137:
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$19,20($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,24($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,28($sp)

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,32($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,36($sp)

	move	$4,$16
	move	$5,$0
$L132:
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,40($sp)

	addiu	$4,$sp,20
	move	$5,$18
	addiu	$6,$sp,16
	jal	rtk_idleNaptConnection
	sw	$2,44($sp)

	beq	$2,$0,$L128
	move	$4,$16

	lw	$5,16($sp)
	jal	rtl_set_ct_timeout_expires
	addu	$5,$18,$5

	move	$4,$16
	jal	rtl_check_for_acc
	addu	$5,$18,$20

	j	$L121
	move	$2,$0

$L135:
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$19,20($sp)

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,24($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	sw	$2,28($sp)

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,32($sp)

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$2,36($sp)

	move	$4,$16
	j	$L132
	li	$5,1			# 0x1

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.size	rtl_fpTimer_update, .-rtl_fpTimer_update
	.section	.text.rtl_br_fdb_time_update,"ax",@progbits
	.align	2
	.globl	rtl_br_fdb_time_update
	.set	nomips16
	.ent	rtl_br_fdb_time_update
	.type	rtl_br_fdb_time_update, @function
rtl_br_fdb_time_update:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$17,$4
	move	$16,$5
	move	$4,$0
	move	$5,$6
	jal	rtl_get_hw_fdb_age
	li	$6,2			# 0x2

	li	$3,300			# 0x12c
	beq	$2,$3,$L142
	li	$3,450			# 0x1c2

	beq	$2,$3,$L143
	li	$3,150			# 0x96

	beq	$2,$3,$L141
	lui	$18,%hi(jiffies)

	move	$4,$16
$L146:
	lw	$16,%lo(jiffies)($18)
	jal	rtl_get_fdb_aging
	nop

	move	$4,$17
	jal	rtl_hold_time
	move	$18,$2

	addu	$18,$2,$18
	li	$3,-1			# 0xffffffffffffffff
	subu	$16,$16,$18
	slt	$16,$16,0
	lw	$31,28($sp)
	movn	$3,$0,$16
	lw	$18,24($sp)
	move	$2,$3
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L141:
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-30000

	j	$L146
	move	$4,$16

$L143:
	lui	$18,%hi(jiffies)
	move	$4,$16
	lw	$5,%lo(jiffies)($18)
	jal	rtl_set_fdb_aging
	nop

	j	$L146
	move	$4,$16

$L142:
	lui	$18,%hi(jiffies)
	move	$4,$16
	lw	$5,%lo(jiffies)($18)
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-15000

	j	$L146
	move	$4,$16

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.size	rtl_br_fdb_time_update, .-rtl_br_fdb_time_update
	.section	.text.FastPath_Route,"ax",@progbits
	.align	2
	.globl	FastPath_Route
	.set	nomips16
	.ent	FastPath_Route
	.type	FastPath_Route, @function
FastPath_Route:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(route_list_inuse)
	lw	$3,%lo(route_list_inuse)($2)
	beq	$3,$0,$L151
	nop

	move	$8,$0
	j	$L150
	move	$2,$0

$L149:
	lw	$3,44($3)
	beq	$3,$0,$L154
	nop

$L150:
	lw	$5,8($3)
	sltu	$6,$5,$8
	bne	$6,$0,$L149
	and	$7,$4,$5

	lw	$6,4($3)
	bne	$7,$6,$L149
	nop

	addiu	$2,$3,16
	lw	$3,44($3)
	bne	$3,$0,$L150
	move	$8,$5

$L154:
	j	$31
	nop

$L151:
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.size	FastPath_Route, .-FastPath_Route
	.section	.text.rtk_addNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_addNaptConnection
	.set	nomips16
	.ent	rtk_addNaptConnection
	.type	rtk_addNaptConnection, @function
rtk_addNaptConnection:
	.frame	$sp,184,$31		# vars= 128, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$fp,176($sp)
	sw	$23,172($sp)
	sw	$22,168($sp)
	sw	$21,164($sp)
	sw	$20,160($sp)
	sw	$19,156($sp)
	sw	$18,152($sp)
	sw	$17,148($sp)
	sw	$16,144($sp)
	move	$18,$5
	move	$16,$4
	move	$22,$6
	beq	$5,$0,$L212
	move	$17,$7

	jal	rtl_eth_hdr
	move	$4,$5

	beq	$2,$0,$L212
	li	$21,6			# 0x6

	lw	$3,0($16)
	li	$2,17			# 0x11
	lw	$4,20($16)
	xori	$3,$3,0x2
	jal	filter_addconnect
	movn	$21,$2,$3

	jal	arch_local_irq_save
	nop

	lui	$6,%hi(napt_table_list_max)
	lw	$5,4($16)
	lui	$7,%hi(table_napt)
	lw	$8,8($16)
	lw	$9,20($16)
	lw	$10,24($16)
	xor	$3,$8,$5
	xor	$3,$3,$9
	sw	$2,72($sp)
	lw	$4,0($16)
	srl	$2,$5,16
	xor	$3,$3,$10
	lw	$6,%lo(napt_table_list_max)($6)
	xor	$3,$3,$2
	lw	$11,%lo(table_napt)($7)
	srl	$12,$9,16
	addiu	$6,$6,-1
	andi	$2,$4,0xffff
	lw	$11,0($11)
	xor	$3,$3,$12
	xor	$3,$3,$2
	srl	$2,$3,12
	xor	$2,$2,$3
	and	$2,$2,$6
	sll	$3,$2,2
	sll	$6,$2,4
	subu	$6,$6,$3
	addu	$2,$11,$6
	lw	$2,0($2)
	bne	$2,$0,$L214
	lui	$3,%hi(napt_list_free)

	j	$L234
	nop

$L159:
	lw	$2,36($2)
	beq	$2,$0,$L158
	lui	$3,%hi(napt_list_free)

$L214:
	lw	$3,4($2)
	bne	$4,$3,$L159
	nop

	lw	$3,8($2)
	bne	$5,$3,$L159
	nop

	lw	$3,12($2)
	bne	$8,$3,$L159
	nop

	lw	$11,16($2)
	lw	$3,12($16)
	bne	$11,$3,$L159
	nop

	lw	$11,20($2)
	lw	$3,16($16)
	bne	$11,$3,$L159
	nop

	lw	$3,24($2)
	bne	$9,$3,$L159
	nop

	lw	$3,28($2)
	bne	$10,$3,$L159
	nop

	move	$4,$22
	move	$5,$21
	jal	rtl_conntrack_drop_check_hook
	move	$6,$22

	lw	$3,72($sp)
	andi	$2,$3,0x1
	bne	$2,$0,$L160
	nop

	jal	arch_local_irq_restore
	move	$4,$3

	j	$L156
	move	$2,$0

$L158:
$L234:
	lw	$2,%lo(napt_list_free)($3)
	beq	$2,$0,$L162
	nop

	sw	$4,4($2)
	lw	$4,4($16)
	lw	$5,44($2)
	sw	$4,8($2)
	lw	$4,8($16)
	sw	$4,12($2)
	lw	$4,12($16)
	sw	$4,16($2)
	lw	$4,16($16)
	sw	$4,20($2)
	lw	$4,20($16)
	sw	$4,24($2)
	lw	$4,24($16)
	sw	$17,32($2)
	sw	$4,28($2)
	li	$4,-1			# 0xffffffffffffffff
	beq	$5,$0,$L163
	sb	$4,0($2)

	lw	$8,48($2)
	addiu	$4,$3,%lo(napt_list_free)
	sw	$8,48($5)
	lw	$5,48($2)
	lw	$3,44($2)
$L164:
	sw	$3,0($5)
	lui	$3,%hi(napt_list_inuse)
	lw	$10,8($4)
	addiu	$3,$3,%lo(napt_list_inuse)
	lw	$5,4($3)
	addiu	$9,$2,44
	addiu	$10,$10,-1
	addiu	$8,$2,36
	sw	$10,8($4)
	sw	$5,48($2)
	sw	$0,44($2)
	sw	$2,0($5)
	lw	$5,8($3)
	lw	$4,%lo(table_napt)($7)
	addiu	$5,$5,1
	sw	$9,4($3)
	sw	$5,8($3)
	sw	$0,36($2)
	lw	$3,0($4)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$3,40($2)
	lw	$3,0($4)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$2,0($3)
	lw	$2,%lo(table_napt)($7)
	lw	$3,0($2)
	addu	$3,$3,$6
	sw	$8,4($3)
	li	$3,33			# 0x21
	lw	$2,0($2)
	sb	$3,17($sp)
	addu	$6,$2,$6
	lw	$2,8($6)
	sb	$0,16($sp)
	addiu	$2,$2,1
	jal	rtl865x_getLanDev
	sw	$2,8($6)

	jal	rtl865x_getWanDev
	sw	$2,56($sp)

	move	$4,$18
	jal	rtl_get_skb_protocol
	move	$fp,$2

	move	$4,$18
	jal	rtl_ip_hdr
	sw	$2,120($sp)

	move	$4,$18
	lbu	$20,0($2)
	move	$19,$2
	andi	$20,$20,0xf
	sll	$20,$20,2
	jal	rtl_eth_hdr
	addu	$20,$2,$20

	li	$6,6			# 0x6
	addiu	$5,$2,6
	jal	memcpy
	addiu	$4,$sp,28

	jal	rtl_eth_hdr
	move	$4,$18

	li	$6,6			# 0x6
	move	$5,$2
	jal	memcpy
	addiu	$4,$sp,20

	move	$4,$18
	lbu	$3,1($19)
	jal	rtl_get_skb_orig_dscp
	sw	$3,88($sp)

	move	$4,$18
	lw	$5,88($sp)
	sll	$2,$2,2
	andi	$3,$5,0x3
	or	$2,$2,$3
	jal	rtl_get_skb_remark_vlanpri
	sb	$2,1($19)

	move	$4,$18
	jal	rtl_set_skb_ori_vlanpri
	sw	$2,96($sp)

	jal	rtl_get_skb_phy_port
	move	$4,$18

	move	$4,$18
	jal	rtl_get_skb_mark
	sw	$2,92($sp)

	move	$4,$18
	lw	$3,12($19)
	sw	$2,100($sp)
	sw	$3,104($sp)
	lhu	$3,0($20)
	sw	$3,108($sp)
	lw	$3,16($19)
	sw	$3,112($sp)
	lhu	$3,2($20)
	jal	rtl_store_skb_dst
	sw	$3,116($sp)

	lui	$4,%hi(path_table_list_max)
	lw	$3,4($16)
	lui	$5,%hi(path_list_free)
	lw	$2,%lo(path_table_list_max)($4)
	sw	$3,60($sp)
	lw	$3,8($16)
	addiu	$2,$2,-1
	lw	$17,%lo(path_list_free)($5)
	sw	$3,76($sp)
	lw	$3,20($16)
	sw	$2,84($sp)
	sw	$3,64($sp)
	lw	$3,24($16)
	beq	$17,$0,$L188
	sw	$3,80($sp)

	lw	$2,0($16)
	sw	$2,4($17)
	lw	$6,4($16)
	sw	$6,12($17)
	lhu	$3,10($16)
	sh	$3,10($17)
	lw	$2,20($16)
	sw	$2,16($17)
	lw	$2,24($16)
	sh	$2,8($17)
	lw	$5,12($16)
	sw	$5,20($17)
	lhu	$23,18($16)
	sh	$23,24($17)
	lw	$4,20($16)
	sw	$4,28($17)
	lw	$2,24($16)
	sh	$2,32($17)
	sw	$3,136($sp)
	sw	$5,132($sp)
	jal	FastPath_Route
	sw	$6,128($sp)

	li	$4,1			# 0x1
	sb	$4,2($17)
	li	$4,-1			# 0xffffffffffffffff
	sb	$4,0($17)
	lui	$4,%hi(rtl_init_bypass_packet_hook)
	sb	$0,1($17)
	lw	$7,%lo(rtl_init_bypass_packet_hook)($4)
	sw	$2,40($17)
	sw	$22,52($17)
	lw	$3,136($sp)
	lw	$5,132($sp)
	lw	$6,128($sp)
	beq	$7,$0,$L166
	nop

	jalr	$7
	move	$4,$17

	lw	$6,12($17)
	lw	$5,20($17)
	lhu	$3,10($17)
	lhu	$23,24($17)
$L166:
	beq	$6,$5,$L167
	nop

	lbu	$2,1($17)
	ori	$2,$2,0x1
	sb	$2,1($17)
$L167:
	beq	$3,$23,$L239
	li	$2,255			# 0xff

	lbu	$2,1($17)
	ori	$2,$2,0x2
	sb	$2,1($17)
	li	$2,255			# 0xff
$L239:
	move	$5,$0
	sw	$0,44($17)
	addiu	$4,$sp,36
	sw	$0,48($17)
	li	$6,14			# 0xe
	sw	$0,60($17)
	sb	$0,72($17)
	sb	$0,73($17)
	sh	$2,78($17)
	lw	$2,4($16)
	sw	$2,12($19)
	lw	$2,8($16)
	sh	$2,0($20)
	lw	$2,20($16)
	sw	$2,16($19)
	lw	$2,24($16)
	jal	memset
	sh	$2,2($20)

	move	$4,$18
	jal	rtl_set_skb_mark
	move	$5,$0

	lw	$3,56($sp)
	beq	$3,$0,$L169
	nop

	lw	$4,4($16)
	move	$5,$3
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36

	beq	$2,$0,$L216
	move	$4,$18

$L169:
	lw	$2,12($16)
	move	$5,$0
	addiu	$4,$sp,36
	li	$6,14			# 0xe
	sw	$2,12($19)
	lw	$2,16($16)
	sh	$2,0($20)
	lw	$2,20($16)
	sw	$2,16($19)
	lw	$2,24($16)
	jal	memset
	sh	$2,2($20)

	move	$4,$18
	jal	rtl_set_skb_mark
	move	$5,$0

	beq	$fp,$0,$L177
	move	$5,$fp

	lw	$4,20($16)
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36

	beq	$2,$0,$L217
	move	$4,$18

$L177:
	move	$4,$17
$L240:
	jal	rtl_set_entry_dscp_out
	move	$5,$18

	move	$4,$17
	jal	rtl_set_entry_vlanpri
	move	$5,$18

	move	$4,$17
	jal	rtl_set_entry_vlanid_out
	move	$5,$18

	li	$2,6			# 0x6
	beq	$21,$2,$L218
	li	$4,1			# 0x1

	move	$5,$22
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	sw	$2,56($17)
$L184:
	lui	$4,%hi(jiffies)
	lw	$3,%lo(jiffies)($4)
	lw	$2,88($17)
	sw	$22,52($17)
	beq	$2,$0,$L185
	sw	$3,36($17)

	lw	$4,92($17)
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$4,92($2)
	sw	$3,68($sp)
	lw	$2,92($17)
	lw	$3,88($17)
$L186:
	lw	$6,60($sp)
	lui	$23,%hi(path_list_inuse)
	lw	$5,64($sp)
	addiu	$23,$23,%lo(path_list_inuse)
	lw	$7,80($sp)
	srl	$4,$6,8
	sw	$3,0($2)
	xor	$3,$5,$6
	lw	$2,68($sp)
	xor	$3,$3,$7
	lw	$6,64($sp)
	xor	$3,$3,$4
	lw	$7,76($sp)
	lw	$5,8($2)
	srl	$4,$6,16
	lw	$2,4($23)
	xor	$3,$3,$4
	lw	$6,68($sp)
	srl	$4,$7,4
	addiu	$5,$5,-1
	xor	$3,$3,$4
	sw	$2,92($17)
	xor	$3,$3,$21
	sw	$5,8($6)
	srl	$6,$3,12
	sw	$0,88($17)
	lui	$7,%hi(table_path)
	sw	$17,0($2)
	xor	$6,$6,$3
	lw	$5,8($23)
	addiu	$4,$17,88
	lw	$3,84($sp)
	lw	$2,%lo(table_path)($7)
	addiu	$5,$5,1
	sw	$0,80($17)
	and	$6,$6,$3
	sw	$5,8($23)
	sll	$7,$6,2
	sw	$4,4($23)
	sll	$3,$6,4
	lw	$5,0($2)
	subu	$3,$3,$7
	addiu	$4,$17,80
	lui	$7,%hi(path_table_list_max)
	addu	$5,$5,$3
	lw	$5,4($5)
	lw	$6,%lo(path_table_list_max)($7)
	sw	$5,84($17)
	lui	$5,%hi(table_path)
	lw	$2,0($2)
	addiu	$6,$6,-1
	sw	$6,84($sp)
	lui	$6,%hi(path_list_free)
	addu	$2,$2,$3
	lw	$2,4($2)
	sw	$17,0($2)
	lw	$2,%lo(table_path)($5)
	lw	$17,%lo(path_list_free)($6)
	lw	$5,0($2)
	addu	$5,$5,$3
	sw	$4,4($5)
	lw	$2,0($2)
	addu	$3,$2,$3
	lw	$2,8($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	lw	$3,20($16)
	sw	$3,60($sp)
	lw	$3,24($16)
	sw	$3,76($sp)
	lw	$3,12($16)
	sw	$3,64($sp)
	lw	$3,16($16)
	beq	$17,$0,$L188
	sw	$3,80($sp)

	lw	$2,0($16)
	sw	$2,4($17)
	lw	$2,20($16)
	sw	$2,12($17)
	lw	$2,24($16)
	sh	$2,10($17)
	lw	$7,12($16)
	sw	$7,16($17)
	lhu	$6,18($16)
	sh	$6,8($17)
	lw	$2,20($16)
	sw	$2,20($17)
	lw	$2,24($16)
	sh	$2,24($17)
	lw	$3,4($16)
	sw	$3,28($17)
	move	$4,$3
	lhu	$5,10($16)
	sh	$5,32($17)
	sw	$3,136($sp)
	sw	$5,132($sp)
	sw	$6,128($sp)
	jal	FastPath_Route
	sw	$7,124($sp)

	li	$4,2			# 0x2
	sb	$4,2($17)
	li	$4,-1			# 0xffffffffffffffff
	sb	$4,0($17)
	lui	$4,%hi(rtl_init_bypass_packet_hook)
	sb	$0,1($17)
	lw	$8,%lo(rtl_init_bypass_packet_hook)($4)
	sw	$2,40($17)
	sw	$22,52($17)
	lw	$3,136($sp)
	lw	$5,132($sp)
	lw	$6,128($sp)
	lw	$7,124($sp)
	beq	$8,$0,$L189
	nop

	jalr	$8
	move	$4,$17

	lw	$7,16($17)
	lw	$3,28($17)
	lhu	$6,8($17)
	lhu	$5,32($17)
$L189:
	beq	$7,$3,$L190
	nop

	lbu	$2,1($17)
	ori	$2,$2,0x4
	sb	$2,1($17)
$L190:
	beq	$6,$5,$L191
	nop

	lbu	$2,1($17)
	ori	$2,$2,0x8
	sb	$2,1($17)
$L191:
	lbu	$3,1($19)
	move	$4,$18
	andi	$3,$3,0x3
	jal	rtl_get_skb_orig_dscp
	sw	$3,136($sp)

	move	$5,$0
	lw	$3,136($sp)
	sll	$2,$2,2
	li	$6,14			# 0xe
	addiu	$4,$sp,36
	or	$3,$3,$2
	li	$2,255			# 0xff
	sb	$3,1($19)
	sw	$0,44($17)
	sw	$0,48($17)
	sw	$0,60($17)
	sb	$0,72($17)
	sb	$0,73($17)
	sh	$2,78($17)
	lw	$2,20($16)
	sw	$2,12($19)
	lw	$2,24($16)
	sh	$2,0($20)
	lw	$2,12($16)
	sw	$2,16($19)
	lw	$2,16($16)
	jal	memset
	sh	$2,2($20)

	move	$4,$18
	jal	rtl_set_skb_mark
	move	$5,$0

	jal	rtl_set_skb_ori_vlanpri
	move	$4,$18

	beq	$fp,$0,$L193
	nop

	lw	$4,20($16)
	move	$5,$fp
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36

	beq	$2,$0,$L219
	move	$4,$18

$L193:
	lw	$2,20($16)
	move	$5,$0
	addiu	$4,$sp,36
	li	$6,14			# 0xe
	sw	$2,12($19)
	lw	$2,24($16)
	sh	$2,0($20)
	lw	$2,4($16)
	sw	$2,16($19)
	lw	$2,8($16)
	jal	memset
	sh	$2,2($20)

	move	$4,$18
	jal	rtl_set_skb_mark
	move	$5,$0

	lw	$3,56($sp)
	beq	$3,$0,$L199
	move	$5,$3

	lw	$4,4($16)
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36

	beq	$2,$0,$L220
	move	$4,$18

$L199:
	jal	rtl_get_skb_mark
	move	$4,$18

	move	$4,$17
	sw	$2,48($17)
	jal	rtl_set_entry_dscp_in
	move	$5,$18

	move	$4,$17
	jal	rtl_set_entry_vlanid_in
	move	$5,$18

	li	$2,6			# 0x6
	beq	$21,$2,$L221
	li	$4,1			# 0x1

	move	$5,$22
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	sw	$2,56($17)
$L203:
	lui	$4,%hi(jiffies)
	lw	$3,%lo(jiffies)($4)
	lw	$2,88($17)
	sw	$22,52($17)
	beq	$2,$0,$L204
	sw	$3,36($17)

	lw	$3,92($17)
	sw	$3,92($2)
	lw	$2,92($17)
	lw	$3,88($17)
$L205:
	lw	$7,64($sp)
	lw	$5,68($sp)
	lw	$6,80($sp)
	sw	$3,0($2)
	srl	$2,$7,16
	lw	$4,8($5)
	xor	$3,$6,$7
	xor	$2,$3,$2
	lw	$3,4($23)
	lw	$6,60($sp)
	addiu	$4,$4,-1
	sw	$4,8($5)
	xor	$2,$2,$21
	sw	$3,92($17)
	srl	$4,$6,8
	sw	$0,88($17)
	xor	$2,$6,$2
	sw	$17,0($3)
	xor	$2,$2,$4
	lw	$7,76($sp)
	lui	$4,%hi(table_path)
	lw	$5,8($23)
	srl	$3,$7,4
	sw	$0,80($17)
	xor	$2,$2,$3
	addiu	$5,$5,1
	sw	$5,8($23)
	srl	$6,$2,12
	lw	$5,84($sp)
	xor	$6,$6,$2
	lw	$3,%lo(table_path)($4)
	addiu	$4,$17,88
	sw	$4,4($23)
	and	$6,$6,$5
	lw	$4,0($3)
	sll	$5,$6,2
	sll	$2,$6,4
	lui	$6,%hi(table_path)
	subu	$2,$2,$5
	addiu	$5,$17,80
	addu	$4,$4,$2
	lw	$4,4($4)
	sw	$4,84($17)
	lw	$3,0($3)
	addu	$3,$3,$2
	lw	$3,4($3)
	sw	$17,0($3)
	lw	$3,%lo(table_path)($6)
	lw	$4,0($3)
	addu	$4,$4,$2
	sw	$5,4($4)
	lw	$3,0($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L188:
	jal	rtl_eth_hdr
	move	$4,$18

	li	$6,6			# 0x6
	addiu	$5,$sp,28
	jal	memcpy
	addiu	$4,$2,6

	jal	rtl_eth_hdr
	move	$4,$18

	li	$6,6			# 0x6
	move	$4,$2
	jal	memcpy
	addiu	$5,$sp,20

	lw	$5,100($sp)
	jal	rtl_set_skb_mark
	move	$4,$18

	move	$4,$18
	lw	$3,104($sp)
	sw	$3,12($19)
	lw	$3,108($sp)
	sh	$3,0($20)
	lw	$3,112($sp)
	sw	$3,16($19)
	lw	$3,116($sp)
	jal	rtl_set_skb_dst
	sh	$3,2($20)

	lw	$5,92($sp)
	jal	rtl_set_skb_phy_port
	move	$4,$18

	move	$4,$18
	lw	$3,88($sp)
	lw	$5,96($sp)
	jal	rtl_restore_skb_vlanpri
	sb	$3,1($19)

	lw	$3,56($sp)
	beq	$3,$0,$L206
	nop

	jal	rtl_dev_put
	move	$4,$3

$L206:
	beq	$fp,$0,$L207
	nop

	jal	rtl_dev_put
	move	$4,$fp

$L207:
	lw	$3,72($sp)
	andi	$2,$3,0x1
	beq	$2,$0,$L222
	nop

	lw	$4,72($sp)
	jal	arch_local_irq_restore
	nop

	move	$4,$22
$L235:
	move	$5,$21
	jal	rtl_conntrack_drop_check_hook
	move	$6,$22

	move	$2,$0
$L156:
	lw	$31,180($sp)
	lw	$fp,176($sp)
	lw	$23,172($sp)
	lw	$22,168($sp)
	lw	$21,164($sp)
	lw	$20,160($sp)
	lw	$19,156($sp)
	lw	$18,152($sp)
	lw	$17,148($sp)
	lw	$16,144($sp)
	j	$31
	addiu	$sp,$sp,184

$L222:
	jal	arch_local_irq_restore
	move	$4,$3

	j	$L235
	move	$4,$22

$L218:
	jal	rtl_tcp_get_timeouts
	move	$4,$22

	j	$L184
	sw	$2,56($17)

$L216:
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16

	move	$23,$2
	li	$2,1			# 0x1
	beq	$23,$2,$L223
	nop

$L170:
	jal	rtl_eth_hdr
	move	$4,$18

	li	$6,6			# 0x6
	addiu	$4,$2,6
	jal	memcpy
	addiu	$5,$sp,36

	lw	$5,56($sp)
	jal	rtl_set_skb_dmac
	move	$4,$18

	move	$4,$18
	addiu	$5,$sp,36
	jal	rtl_set_skb_src_phy_port
	move	$6,$0

	li	$2,2048			# 0x800
	lw	$3,120($sp)
	bne	$3,$2,$L169
	lui	$2,%hi(nf_hooks)

	addiu	$23,$2,%lo(nf_hooks)
	lui	$2,%hi(nf_hooks+128)
	lw	$3,128($23)
	addiu	$2,$2,%lo(nf_hooks+128)
	beq	$3,$2,$L171
	move	$4,$18

	lw	$6,56($sp)
	move	$5,$0
	jal	rtl_ipt_do_table
	move	$7,$fp

$L171:
	jal	rtl_get_skb_mark
	move	$4,$18

	bne	$2,$0,$L224
	nop

	lw	$2,44($17)
	sw	$2,44($17)
$L238:
	jal	rtl_get_skb_mark
	move	$4,$18

	sw	$2,60($17)
	lui	$2,%hi(nf_hooks+160)
	lw	$3,160($23)
	addiu	$2,$2,%lo(nf_hooks+160)
	beq	$3,$2,$L174
	move	$4,$18

	lw	$6,56($sp)
	li	$5,4			# 0x4
	jal	rtl_ipt_do_table
	move	$7,$fp

$L174:
	jal	rtl_get_skb_mark
	move	$4,$18

	bne	$2,$0,$L225
	nop

	lw	$2,48($17)
	j	$L169
	sw	$2,48($17)

$L217:
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16

	move	$23,$2
	li	$2,1			# 0x1
	beq	$23,$2,$L226
	nop

$L178:
	jal	rtl_eth_hdr
	move	$4,$18

	addiu	$5,$sp,36
	move	$4,$2
	jal	memcpy
	li	$6,6			# 0x6

	move	$4,$18
	jal	rtl_set_skb_smac
	move	$5,$fp

	li	$2,2048			# 0x800
	lw	$3,120($sp)
	bne	$3,$2,$L240
	move	$4,$17

	lw	$5,44($17)
	bne	$5,$0,$L227
	nop

	lui	$2,%hi(nf_hooks+160)
$L236:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	beq	$3,$2,$L180
	move	$4,$18

	lw	$6,56($sp)
	li	$5,4			# 0x4
	jal	rtl_ipt_do_table
	move	$7,$fp

$L180:
	jal	rtl_get_skb_mark
	move	$4,$18

	bne	$2,$0,$L228
	nop

	lw	$2,48($17)
	j	$L177
	sw	$2,48($17)

$L221:
	jal	rtl_tcp_get_timeouts
	move	$4,$22

	j	$L203
	sw	$2,56($17)

$L162:
	lw	$3,72($sp)
	andi	$2,$3,0x1
	bne	$2,$0,$L210
	nop

	jal	arch_local_irq_restore
	move	$4,$3

	j	$L156
	li	$2,-1			# 0xffffffffffffffff

$L163:
	lw	$5,48($2)
	addiu	$4,$3,%lo(napt_list_free)
	move	$3,$0
	j	$L164
	sw	$5,4($4)

$L219:
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L229
	nop

$L195:
	jal	rtl_eth_hdr
	move	$4,$18

	li	$6,6			# 0x6
	addiu	$4,$2,6
	jal	memcpy
	addiu	$5,$sp,36

	move	$4,$18
	jal	rtl_set_skb_dmac
	move	$5,$fp

	lw	$5,92($sp)
	jal	rtl_set_skb_phy_port
	move	$4,$18

	move	$4,$18
	addiu	$5,$sp,36
	jal	rtl_set_skb_src_phy_port
	li	$6,1			# 0x1

	li	$2,2048			# 0x800
	lw	$3,120($sp)
	beq	$3,$2,$L230
	lui	$2,%hi(nf_hooks+128)

$L196:
	jal	rtl_get_skb_mark
	move	$4,$18

	bne	$2,$0,$L231
	nop

	lw	$2,44($17)
	j	$L193
	sw	$2,44($17)

$L220:
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16

	move	$16,$2
	li	$2,1			# 0x1
	beq	$16,$2,$L232
	nop

$L200:
	jal	rtl_eth_hdr
	move	$4,$18

	addiu	$5,$sp,36
	move	$4,$2
	jal	memcpy
	li	$6,6			# 0x6

	lw	$5,56($sp)
	jal	rtl_set_skb_smac
	move	$4,$18

	li	$2,2048			# 0x800
	lw	$3,120($sp)
	bne	$3,$2,$L199
	nop

	lw	$5,44($17)
	bne	$5,$0,$L233
	nop

	lui	$2,%hi(nf_hooks+160)
$L237:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	beq	$3,$2,$L199
	move	$4,$18

	lw	$7,56($sp)
	li	$5,4			# 0x4
	jal	rtl_ipt_do_table
	move	$6,$fp

	j	$L199
	nop

$L185:
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$3,68($sp)
	move	$3,$0
	lw	$2,92($17)
	lw	$4,68($sp)
	j	$L186
	sw	$2,4($4)

$L204:
	lw	$2,92($17)
	move	$3,$0
	lw	$5,68($sp)
	j	$L205
	sw	$2,4($5)

$L210:
	lw	$4,72($sp)
	jal	arch_local_irq_restore
	nop

	j	$L156
	li	$2,-1			# 0xffffffffffffffff

$L231:
	jal	rtl_get_skb_mark
	move	$4,$18

	j	$L193
	sw	$2,44($17)

$L230:
	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	beq	$3,$2,$L196
	move	$4,$18

	lw	$7,56($sp)
	move	$5,$0
	jal	rtl_ipt_do_table
	move	$6,$fp

	j	$L196
	nop

$L212:
	j	$L156
	li	$2,-1			# 0xffffffffffffffff

$L232:
	jal	rtl_skb_mac_header_was_set
	move	$4,$18

	bne	$2,$16,$L200
	nop

	jal	rtl_eth_hdr
	move	$4,$18

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	beq	$3,$2,$L200
	nop

	move	$4,$18
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea

	j	$L200
	nop

$L229:
	move	$4,$18
	jal	rtl_skb_mac_header_was_set
	sw	$3,136($sp)

	lw	$3,136($sp)
	bne	$2,$3,$L195
	nop

	jal	rtl_eth_hdr
	move	$4,$18

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	beq	$3,$2,$L195
	nop

	move	$4,$18
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea

	j	$L195
	nop

$L223:
	jal	rtl_skb_mac_header_was_set
	move	$4,$18

	bne	$2,$23,$L170
	nop

	jal	rtl_eth_hdr
	move	$4,$18

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	beq	$3,$2,$L170
	nop

	move	$4,$18
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea

	j	$L170
	nop

$L226:
	jal	rtl_skb_mac_header_was_set
	move	$4,$18

	bne	$2,$23,$L178
	nop

	jal	rtl_eth_hdr
	move	$4,$18

	lbu	$3,12($2)
	lbu	$2,13($2)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,2048			# 0x800
	beq	$3,$2,$L178
	move	$4,$18

	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea

	j	$L178
	nop

$L228:
	jal	rtl_get_skb_mark
	move	$4,$18

	j	$L177
	sw	$2,48($17)

$L227:
	jal	rtl_set_skb_mark
	move	$4,$18

	j	$L236
	lui	$2,%hi(nf_hooks+160)

$L233:
	jal	rtl_set_skb_mark
	move	$4,$18

	j	$L237
	lui	$2,%hi(nf_hooks+160)

$L225:
	jal	rtl_get_skb_mark
	move	$4,$18

	j	$L169
	sw	$2,48($17)

$L224:
	jal	rtl_get_skb_mark
	move	$4,$18

	j	$L238
	sw	$2,44($17)

$L160:
	lw	$4,72($sp)
	jal	arch_local_irq_restore
	nop

	j	$L156
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtk_addNaptConnection
	.size	rtk_addNaptConnection, .-rtk_addNaptConnection
	.section	.text.mark_path_invalid,"ax",@progbits
	.align	2
	.globl	mark_path_invalid
	.set	nomips16
	.ent	mark_path_invalid
	.type	mark_path_invalid, @function
mark_path_invalid:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	srl	$3,$4,8
	lhu	$2,18($sp)
	xor	$3,$3,$4
	lui	$8,%hi(path_table_list_max)
	srl	$11,$6,16
	lw	$9,%lo(path_table_list_max)($8)
	andi	$5,$5,0xffff
	lui	$8,%hi(table_path)
	xor	$3,$3,$6
	addiu	$9,$9,-1
	xor	$3,$3,$11
	lw	$10,%lo(table_path)($8)
	srl	$11,$5,4
	xor	$3,$3,$11
	lw	$10,0($10)
	andi	$7,$7,0xffff
	xor	$3,$3,$7
	xor	$3,$3,$2
	srl	$11,$3,12
	xor	$3,$11,$3
	and	$3,$3,$9
	sll	$11,$3,2
	sll	$3,$3,4
	subu	$3,$3,$11
	addu	$10,$10,$3
	lw	$3,0($10)
	beq	$3,$0,$L258
	nop

	j	$L252
	li	$11,255			# 0xff

$L243:
	lw	$3,80($3)
	beq	$3,$0,$L258
	nop

$L252:
	lhu	$10,10($3)
	bne	$10,$5,$L243
	nop

	lhu	$10,8($3)
	bne	$10,$7,$L243
	nop

	lw	$10,12($3)
	bne	$10,$4,$L243
	nop

	lw	$10,16($3)
	bne	$10,$6,$L243
	nop

	lbu	$10,0($3)
	bne	$10,$11,$L243
	li	$10,1			# 0x1

	lbu	$11,2($3)
	beq	$11,$10,$L256
	sb	$0,0($3)

	lw	$10,28($3)
	lhu	$12,32($3)
$L245:
	beq	$10,$0,$L258
	nop

	li	$3,1			# 0x1
	beq	$11,$3,$L257
	srl	$3,$4,16

	xor	$3,$3,$4
	xor	$3,$3,$5
	xor	$3,$3,$2
	xor	$3,$3,$10
	srl	$10,$10,8
	xor	$10,$3,$10
	srl	$2,$12,4
	xor	$2,$10,$2
	srl	$3,$2,12
	xor	$2,$3,$2
	and	$9,$2,$9
$L248:
	lw	$2,%lo(table_path)($8)
	sll	$3,$9,2
	sll	$9,$9,4
	lw	$2,0($2)
	subu	$9,$9,$3
	addu	$9,$2,$9
	lw	$2,0($9)
	beq	$2,$0,$L258
	nop

	j	$L251
	li	$8,255			# 0xff

$L249:
	lw	$2,80($2)
	beq	$2,$0,$L258
	nop

$L251:
	lw	$3,20($2)
	bne	$3,$6,$L249
	nop

	lhu	$3,24($2)
	bne	$3,$7,$L249
	nop

	lw	$3,28($2)
	bne	$3,$4,$L249
	nop

	lhu	$3,32($2)
	bne	$3,$5,$L249
	nop

	lbu	$3,0($2)
	bne	$3,$8,$L249
	nop

	j	$L249
	sb	$0,0($2)

$L258:
	j	$31
	nop

$L257:
	srl	$3,$6,8
	xor	$3,$3,$6
	xor	$2,$3,$2
	srl	$3,$7,4
	xor	$2,$2,$3
	xor	$2,$2,$10
	xor	$12,$2,$12
	srl	$10,$10,16
	xor	$10,$12,$10
	srl	$2,$10,12
	xor	$10,$2,$10
	j	$L248
	and	$9,$10,$9

$L256:
	lw	$10,20($3)
	lhu	$12,24($3)
	j	$L245
	nop

	.set	macro
	.set	reorder
	.end	mark_path_invalid
	.size	mark_path_invalid, .-mark_path_invalid
	.section	.rodata.str1.4
	.align	2
$LC9:
	.ascii	"lo\000"
	.section	.text.enter_fast_path,"ax",@progbits
	.align	2
	.globl	enter_fast_path
	.set	nomips16
	.ent	enter_fast_path
	.type	enter_fast_path, @function
enter_fast_path:
	.frame	$sp,96,$31		# vars= 32, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$fp,88($sp)
	sw	$23,84($sp)
	sw	$22,80($sp)
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_dev_name
	move	$18,$4
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	move	$17,$2
	lbu	$2,8($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L363
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$19,9($17)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L261
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L261
$L363:
	move	$2,$0
$L260:
	lw	$31,92($sp)
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

$L261:
	lbu	$21,0($17)
	move	$4,$17
	move	$6,$18
	andi	$21,$21,0xf
	sll	$21,$21,2
	addu	$21,$17,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_pre_process_check
	move	$5,$21
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L260
	lui	$3,%hi($LC9)
	.set	macro
	.set	reorder

	addiu	$3,$3,%lo($LC9)
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($16)
1:	lbu	$1,($3)
	addiu	$16,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($16)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L363
	lhu	$10,6($17)
	lw	$23,12($17)
	lw	$3,16($17)
	lhu	$20,0($21)
	lhu	$fp,2($21)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L262
	andi	$2,$10,0x3fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L266
	lui	$2,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

	xor	$7,$3,$23
$L409:
	move	$22,$0
	move	$10,$0
	move	$9,$0
	li	$12,1			# 0x1
$L267:
	srl	$8,$3,16
	lui	$2,%hi(path_table_list_max)
	srl	$4,$23,8
	lw	$6,%lo(path_table_list_max)($2)
	xor	$4,$8,$4
	lui	$2,%hi(table_path)
	xor	$4,$4,$19
	lw	$5,%lo(table_path)($2)
	xor	$4,$4,$fp
	addiu	$6,$6,-1
	srl	$2,$20,4
	lw	$5,0($5)
	xor	$4,$4,$7
	xor	$4,$4,$2
	srl	$2,$4,12
	xor	$2,$2,$4
	and	$2,$2,$6
	sll	$4,$2,2
	sll	$2,$2,4
	subu	$2,$2,$4
	addu	$2,$5,$2
	lw	$16,0($2)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L279
	li	$4,255			# 0xff
	.set	macro
	.set	reorder

	li	$13,2			# 0x2
	li	$11,1			# 0x1
	li	$6,17			# 0x11
	.set	noreorder
	.set	nomacro
	j	$L366
	li	$5,6			# 0x6
	.set	macro
	.set	reorder

$L280:
	lw	$16,80($16)
	beq	$16,$0,$L279
$L366:
	lhu	$2,10($16)
	bne	$2,$20,$L280
	lhu	$2,8($16)
	bne	$2,$fp,$L280
	lw	$2,12($16)
	bne	$2,$23,$L280
	lw	$2,16($16)
	bne	$2,$3,$L280
	lbu	$2,0($16)
	bne	$2,$4,$L280
	lw	$2,4($16)
	beq	$2,$13,$L381
	bne	$2,$11,$L280
	.set	noreorder
	.set	nomacro
	bne	$19,$6,$L280
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

$L407:
	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L408
	addiu	$5,$16,60
	.set	macro
	.set	reorder

	sw	$3,48($sp)
	move	$4,$16
	sw	$7,40($sp)
	sw	$8,52($sp)
	sw	$9,32($sp)
	sw	$10,36($sp)
	.set	noreorder
	.set	nomacro
	jalr	$2
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$7,40($sp)
	lw	$8,52($sp)
	lw	$9,32($sp)
	lw	$10,36($sp)
	lw	$12,44($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L363
	addiu	$5,$16,60
	.set	macro
	.set	reorder

$L408:
	move	$4,$18
	sw	$3,48($sp)
	sw	$7,40($sp)
	sw	$8,52($sp)
	sw	$9,32($sp)
	sw	$10,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_recheck_fp_policy_route_mark
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$5,60($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	addiu	$6,$sp,24
	.set	macro
	.set	reorder

	lw	$5,28($16)
	lw	$6,52($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_route_input
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$7,40($sp)
	lw	$8,52($sp)
	lw	$9,32($sp)
	lw	$10,36($sp)
	lw	$12,44($sp)
	lw	$5,24($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L382
	move	$6,$0
	.set	macro
	.set	reorder

	move	$4,$18
	sw	$3,48($sp)
	sw	$7,40($sp)
	sw	$8,52($sp)
	sw	$9,32($sp)
	sw	$10,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	move	$4,$18
	lw	$5,28($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_dst_check
	move	$6,$17
	.set	macro
	.set	reorder

	li	$4,-1			# 0xffffffffffffffff
	lw	$3,48($sp)
	lw	$7,40($sp)
	lw	$8,52($sp)
	lw	$9,32($sp)
	lw	$10,36($sp)
	lw	$12,44($sp)
	beq	$2,$4,$L383
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L287
	addiu	$11,$21,16
	.set	macro
	.set	reorder

	lhu	$2,6($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L287
	move	$11,$0
	.set	macro
	.set	reorder

	addiu	$11,$21,6
$L287:
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L288
	lui	$4,%hi(udp_cache_list_free)
	.set	macro
	.set	reorder

	lui	$5,%hi(max_udp_frag_entry)
	lw	$2,%lo(udp_cache_list_free)($4)
	lhu	$9,4($17)
	lw	$13,%lo(max_udp_frag_entry)($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L385
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	lw	$5,52($2)
	sb	$6,0($2)
	sh	$9,2($2)
	sw	$23,4($2)
	sw	$3,8($2)
	sb	$19,1($2)
	sh	$20,12($2)
	sh	$fp,14($2)
	lw	$6,56($2)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L290
	addiu	$4,$4,%lo(udp_cache_list_free)
	.set	macro
	.set	reorder

	sw	$6,56($5)
	lw	$5,52($2)
	lw	$6,56($2)
$L291:
	xor	$9,$7,$9
	sw	$5,0($6)
	srl	$7,$7,16
	addiu	$13,$13,-1
	xor	$5,$9,$7
	lw	$9,8($4)
	srl	$6,$5,8
	li	$7,12			# 0xc
	xor	$10,$5,$19
	lui	$5,%hi(udp_cache_table)
	xor	$10,$10,$6
	addiu	$9,$9,-1
	and	$10,$10,$13
	lw	$6,%lo(udp_cache_table)($5)
	mult	$10,$7
	sw	$9,8($4)
	sw	$0,44($2)
	lw	$9,0($6)
	addiu	$10,$2,44
	addiu	$4,$2,16
	mflo	$7
	addu	$9,$9,$7
	lw	$9,4($9)
	sw	$9,48($2)
	lw	$6,0($6)
	addu	$6,$6,$7
	lw	$6,4($6)
	sw	$2,0($6)
	lui	$6,%hi(jiffies)
	lw	$5,%lo(udp_cache_table)($5)
	lw	$6,%lo(jiffies)($6)
	lw	$9,0($5)
	addiu	$6,$6,1000
	addu	$9,$9,$7
	sw	$10,4($9)
	lw	$5,0($5)
	addu	$7,$5,$7
	lw	$5,8($7)
	addiu	$5,$5,1
	sw	$5,8($7)
	sw	$6,24($2)
	sw	$3,48($sp)
	sw	$8,52($sp)
	sw	$11,32($sp)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(statistic_udp_frag_add)
	lw	$4,%lo(statistic_udp_frag_add)($2)
	lw	$3,48($sp)
	addiu	$4,$4,1
	lw	$8,52($sp)
	lw	$11,32($sp)
	lw	$12,44($sp)
	sw	$4,%lo(statistic_udp_frag_add)($2)
$L292:
	move	$4,$18
$L414:
	sw	$3,48($sp)
	move	$5,$17
	sw	$8,52($sp)
	move	$6,$19
	sw	$11,32($sp)
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	lw	$3,48($sp)
	lw	$8,52($sp)
	lw	$11,32($sp)
	lw	$12,44($sp)
	beq	$2,$4,$L260
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L386
	li	$4,1048576			# 0x100000
	.set	macro
	.set	reorder

$L295:
	lbu	$2,1($16)
	sltu	$4,$2,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L296
	lui	$4,%hi($L302)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addiu	$4,$4,%lo($L302)
	addu	$2,$4,$2
	lw	$2,0($2)
	j	$2
	.section	.rodata.enter_fast_path,"a",@progbits
	.align	2
	.align	2
$L302:
	.word	$L297
	.word	$L298
	.word	$L299
	.word	$L299
	.word	$L300
	.word	$L296
	.word	$L296
	.word	$L296
	.word	$L301
	.word	$L296
	.word	$L296
	.word	$L296
	.word	$L301
	.section	.text.enter_fast_path
$L266:
$L406:
	lw	$4,%lo(statistic_udp_frag_rx)($2)
	addiu	$4,$4,1
	sw	$4,%lo(statistic_udp_frag_rx)($2)
	li	$2,8192			# 0x2000
	beq	$10,$2,$L356
$L351:
	andi	$2,$10,0x2000
	bne	$2,$0,$L357
	andi	$10,$10,0x1fff
	move	$9,$0
	sltu	$10,$0,$10
$L268:
	lhu	$6,4($17)
	xor	$7,$3,$23
	srl	$2,$7,16
	lbu	$11,9($17)
	xor	$4,$6,$7
	xor	$4,$4,$2
	lui	$2,%hi(max_udp_frag_entry)
	lw	$12,%lo(max_udp_frag_entry)($2)
	srl	$5,$4,8
	xor	$4,$4,$11
	lui	$2,%hi(negative_cache_table)
	xor	$8,$4,$5
	addiu	$12,$12,-1
	and	$8,$8,$12
	lw	$5,%lo(negative_cache_table)($2)
	sll	$12,$8,2
	sll	$13,$8,4
	subu	$4,$13,$12
	addu	$4,$5,$4
	lw	$4,0($4)
	beq	$4,$0,$L269
	.set	noreorder
	.set	nomacro
	j	$L368
	li	$14,3			# 0x3
	.set	macro
	.set	reorder

$L270:
	lw	$4,40($4)
	beq	$4,$0,$L269
$L368:
	lhu	$5,2($4)
	bne	$5,$6,$L270
	lw	$5,4($4)
	bne	$23,$5,$L270
	lw	$5,8($4)
	bne	$3,$5,$L270
	lbu	$5,1($4)
	bne	$5,$11,$L270
	lbu	$5,0($4)
	bne	$5,$14,$L270
	bne	$4,$0,$L363
$L269:
	lui	$4,%hi(udp_cache_table)
	subu	$12,$13,$12
	lw	$4,%lo(udp_cache_table)($4)
	lw	$4,0($4)
	addu	$12,$4,$12
	lw	$22,0($12)
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L273
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	j	$L367
$L274:
	lw	$22,44($22)
	beq	$22,$0,$L273
$L367:
	lhu	$4,2($22)
	bne	$4,$6,$L274
	lw	$4,4($22)
	bne	$23,$4,$L274
	lw	$4,8($22)
	bne	$3,$4,$L274
	lbu	$4,1($22)
	bne	$4,$11,$L274
	lbu	$4,0($22)
	bne	$4,$5,$L274
	beq	$22,$0,$L273
	lhu	$20,12($22)
	lhu	$fp,14($22)
	bne	$9,$0,$L374
	.set	noreorder
	.set	nomacro
	j	$L267
	move	$12,$0
	.set	macro
	.set	reorder

$L279:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_inDev
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_return_check
	move	$6,$18
	.set	macro
	.set	reorder

	j	$L260
$L262:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L264
	li	$4,8192			# 0x2000
	.set	macro
	.set	reorder

	beq	$10,$4,$L264
	lui	$2,%hi(statistic_udp_frag_rx)
	lw	$4,%lo(statistic_udp_frag_rx)($2)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L351
	sw	$4,%lo(statistic_udp_frag_rx)($2)
	.set	macro
	.set	reorder

$L264:
	lw	$4,12($21)
	li	$5,65536			# 0x10000
	and	$5,$4,$5
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L387
	li	$5,393216			# 0x60000
	.set	macro
	.set	reorder

	and	$4,$4,$5
	bne	$4,$0,$L363
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L409
	xor	$7,$3,$23
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L406
	lui	$2,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

$L357:
	move	$10,$0
	.set	noreorder
	.set	nomacro
	j	$L268
	move	$9,$0
	.set	macro
	.set	reorder

$L356:
	move	$10,$0
	.set	noreorder
	.set	nomacro
	j	$L268
	li	$9,1			# 0x1
	.set	macro
	.set	reorder

$L361:
$L374:
	li	$9,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L267
	li	$12,1			# 0x1
	.set	macro
	.set	reorder

$L273:
	.set	noreorder
	.set	nomacro
	bne	$9,$0,$L361
	move	$22,$0
	.set	macro
	.set	reorder

	lui	$4,%hi(statistic_udp_frag_ps)
	lui	$7,%hi(negative_cache_list_free)
	lw	$9,%lo(statistic_udp_frag_ps)($4)
	lw	$5,%lo(negative_cache_list_free)($7)
	addiu	$9,$9,1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L363
	sw	$9,%lo(statistic_udp_frag_ps)($4)
	.set	macro
	.set	reorder

	lw	$4,48($5)
	sw	$3,8($5)
	li	$3,3			# 0x3
	sh	$6,2($5)
	sb	$3,0($5)
	sw	$23,4($5)
	sb	$11,1($5)
	lw	$6,52($5)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L277
	addiu	$3,$7,%lo(negative_cache_list_free)
	.set	macro
	.set	reorder

	sw	$6,52($4)
	lw	$4,48($5)
	lw	$6,52($5)
$L278:
	li	$7,12			# 0xc
	mult	$8,$7
	sw	$4,0($6)
	lw	$9,8($3)
	lw	$7,%lo(negative_cache_table)($2)
	addiu	$6,$5,40
	addiu	$9,$9,-1
	addiu	$4,$5,12
	mflo	$8
	sw	$0,40($5)
	sw	$9,8($3)
	addu	$3,$7,$8
	lw	$7,4($3)
	sw	$7,44($5)
	lw	$3,4($3)
	sw	$5,0($3)
	lw	$3,%lo(negative_cache_table)($2)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$2,$2,1000
	sw	$6,4($8)
	addiu	$3,$3,1
	sw	$3,8($8)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$2,20($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L260
	move	$2,$0
	.set	macro
	.set	reorder

$L387:
	li	$2,6			# 0x6
	move	$4,$23
	move	$5,$20
	sw	$2,16($sp)
	move	$6,$3
	.set	noreorder
	.set	nomacro
	jal	mark_path_invalid
	move	$7,$fp
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L260
	move	$2,$0
	.set	macro
	.set	reorder

$L381:
	.set	noreorder
	.set	nomacro
	bne	$19,$5,$L280
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	j	$L407
$L277:
	.set	noreorder
	.set	nomacro
	j	$L278
	sw	$6,4($3)
	.set	macro
	.set	reorder

$L301:
	lw	$4,28($16)
	andi	$3,$3,0xffff
	addu	$8,$3,$8
	andi	$3,$4,0xffff
	lhu	$2,10($17)
	subu	$3,$8,$3
	srl	$4,$4,16
	subu	$4,$3,$4
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L389
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L325:
	.set	noreorder
	.set	nomacro
	beq	$12,$0,$L378
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L390
	lw	$3,28($16)
	lhu	$2,2($21)
	andi	$5,$3,0xffff
	lhu	$4,32($16)
	subu	$8,$8,$5
	srl	$3,$3,16
	subu	$3,$8,$3
	addu	$2,$2,$3
	lhu	$3,0($11)
	subu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L376
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L412:
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($11)
$L328:
	lhu	$2,32($16)
	sh	$2,2($21)
$L378:
	lw	$2,28($16)
	sw	$2,16($17)
$L297:
	lbu	$2,8($17)
	lbu	$3,9($17)
	addiu	$4,$2,-1
	sll	$2,$2,8
	andi	$4,$4,0x00ff
	lhu	$5,10($17)
	sll	$6,$4,8
	or	$2,$2,$3
	or	$3,$6,$3
	subu	$3,$2,$3
	addu	$2,$5,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L391
	sb	$4,8($17)
	.set	macro
	.set	reorder

	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L345:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$6,48($16)
	lw	$5,44($16)
	.set	noreorder
	.set	nomacro
	jal	fastpath_set_qos_mark
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_set_iph_dscp
	move	$5,$17
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_remark_vlanpri
	move	$4,$16
	.set	macro
	.set	reorder

	li	$2,65535			# 0xffff
	lhu	$5,74($16)
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L346
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lbu	$3,2($16)
	bne	$3,$2,$L346
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L346:
	lhu	$5,76($16)
	li	$2,65535			# 0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L347
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L410
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L347:
	lui	$2,%hi(jiffies)
$L410:
	move	$4,$17
	lw	$2,%lo(jiffies)($2)
	move	$5,$21
	move	$6,$18
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	sw	$2,36($16)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	beq	$2,$3,$L365
	.set	noreorder
	.set	nomacro
	jal	ip_finish_output3
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$3,%hi(statistic_fp)
	lw	$4,%lo(statistic_fp)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L260
	sw	$4,%lo(statistic_fp)($3)
	.set	macro
	.set	reorder

$L391:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L345
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L296:
	lw	$4,20($16)
	andi	$5,$23,0xffff
	srl	$23,$23,16
	lhu	$2,10($17)
	addu	$23,$5,$23
	andi	$5,$4,0xffff
	subu	$5,$23,$5
	srl	$4,$4,16
	subu	$4,$5,$4
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L392
	sra	$4,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	sh	$2,10($17)
$L332:
	lw	$4,28($16)
	andi	$3,$3,0xffff
	addu	$8,$3,$8
	andi	$3,$4,0xffff
	lhu	$2,10($17)
	subu	$3,$8,$3
	srl	$4,$4,16
	subu	$4,$3,$4
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L393
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L334:
	.set	noreorder
	.set	nomacro
	beq	$12,$0,$L335
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L394
	lw	$3,20($16)
	lhu	$2,0($21)
	andi	$5,$3,0xffff
	lhu	$4,24($16)
	subu	$23,$23,$5
	srl	$3,$3,16
	subu	$3,$23,$3
	addu	$2,$2,$3
	lhu	$3,0($11)
	subu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L380
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L411:
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($11)
$L342:
	lw	$3,28($16)
	lhu	$2,2($21)
	andi	$5,$3,0xffff
	lhu	$4,32($16)
	subu	$8,$8,$5
	srl	$3,$3,16
	subu	$3,$8,$3
	addu	$2,$2,$3
	lhu	$3,0($11)
	subu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L395
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($11)
$L337:
	lhu	$2,24($16)
	sh	$2,0($21)
	lhu	$2,32($16)
	sh	$2,2($21)
$L335:
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	j	$L378
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L393:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L334
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L392:
	subu	$2,$0,$2
	sra	$4,$2,16
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L332
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L365:
	.set	noreorder
	.set	nomacro
	j	$L260
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L395:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L337
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L394:
	beq	$11,$0,$L337
	lhu	$3,0($11)
	beq	$3,$0,$L337
	lw	$4,20($16)
	lhu	$2,0($21)
	andi	$6,$4,0xffff
	lhu	$5,24($16)
	subu	$23,$23,$6
	srl	$4,$4,16
	subu	$4,$23,$4
	addu	$2,$2,$4
	subu	$2,$2,$5
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L411
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L380:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L342
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L389:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L325
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L300:
	lw	$4,28($16)
	andi	$3,$3,0xffff
	addu	$8,$3,$8
	andi	$3,$4,0xffff
	lhu	$2,10($17)
	subu	$3,$8,$3
	srl	$4,$4,16
	subu	$4,$3,$4
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L396
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L319:
	.set	noreorder
	.set	nomacro
	beq	$12,$0,$L378
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L397
	lw	$3,28($16)
	lhu	$2,0($11)
	andi	$4,$3,0xffff
	subu	$8,$8,$4
	srl	$3,$3,16
	subu	$3,$8,$3
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L375
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L413:
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L378
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L299:
	lw	$3,20($16)
	srl	$4,$23,16
	lhu	$2,10($17)
	andi	$23,$23,0xffff
	andi	$5,$3,0xffff
	srl	$3,$3,16
	addu	$3,$5,$3
	subu	$3,$4,$3
	addu	$23,$3,$23
	addu	$2,$2,$23
	.set	noreorder
	.set	nomacro
	bltz	$2,$L398
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L311:
	.set	noreorder
	.set	nomacro
	beq	$12,$0,$L312
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L399
	lw	$2,20($16)
	lw	$3,12($17)
	andi	$7,$2,0xffff
	lhu	$5,0($21)
	srl	$2,$2,16
	lhu	$4,24($16)
	srl	$6,$3,16
	addu	$2,$7,$2
	subu	$2,$6,$2
	andi	$3,$3,0xffff
	addu	$3,$2,$3
	addu	$2,$5,$3
	lhu	$3,0($11)
	subu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L400
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($11)
$L317:
	lw	$2,12($21)
	li	$3,1048576			# 0x100000
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L314
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L314
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

$L314:
	lhu	$2,24($16)
	sh	$2,0($21)
$L312:
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	j	$L297
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L398:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L311
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L400:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L317
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L399:
	beq	$11,$0,$L314
	lhu	$3,0($11)
	beq	$3,$0,$L314
	lw	$2,20($16)
	lw	$4,12($17)
	srl	$8,$2,16
	lhu	$6,0($21)
	andi	$2,$2,0xffff
	lhu	$5,24($16)
	srl	$7,$4,16
	addu	$2,$8,$2
	subu	$2,$7,$2
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	addu	$2,$6,$4
	subu	$2,$2,$5
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L401
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L314
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L390:
	beq	$11,$0,$L328
	lhu	$3,0($11)
	beq	$3,$0,$L328
	lw	$4,28($16)
	lhu	$2,2($21)
	andi	$6,$4,0xffff
	lhu	$5,32($16)
	subu	$8,$8,$6
	srl	$4,$4,16
	subu	$4,$8,$4
	addu	$2,$2,$4
	subu	$2,$2,$5
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L412
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L376:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L328
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L401:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L314
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L298:
	lw	$3,20($16)
	srl	$4,$23,16
	lhu	$2,10($17)
	andi	$23,$23,0xffff
	andi	$5,$3,0xffff
	srl	$3,$3,16
	addu	$3,$5,$3
	subu	$3,$4,$3
	addu	$23,$3,$23
	addu	$2,$2,$23
	.set	noreorder
	.set	nomacro
	bltz	$2,$L402
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($17)
$L304:
	.set	noreorder
	.set	nomacro
	beq	$12,$0,$L312
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L403
	lw	$4,20($16)
	lw	$3,12($17)
	andi	$6,$4,0xffff
	lhu	$2,0($11)
	srl	$4,$4,16
	srl	$5,$3,16
	addu	$4,$6,$4
	subu	$4,$5,$4
	andi	$3,$3,0xffff
	addu	$3,$4,$3
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L404
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($11)
$L309:
	lw	$2,12($21)
	li	$3,1048576			# 0x100000
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L312
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L312
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	j	$L312
$L397:
	beq	$11,$0,$L378
	lhu	$3,0($11)
	beq	$3,$0,$L378
	lw	$4,28($16)
	andi	$2,$4,0xffff
	subu	$2,$8,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L413
	sra	$3,$2,16
	.set	macro
	.set	reorder

$L375:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L378
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L404:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L309
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L403:
	beq	$11,$0,$L312
	lhu	$3,0($11)
	beq	$3,$0,$L312
	lw	$2,20($16)
	lw	$4,12($17)
	srl	$6,$2,16
	andi	$2,$2,0xffff
	srl	$5,$4,16
	addu	$2,$6,$2
	subu	$2,$5,$2
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L405
	sra	$3,$2,16
	.set	macro
	.set	reorder

	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L312
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L402:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L304
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L405:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L312
	sh	$2,0($11)
	.set	macro
	.set	reorder

$L396:
	subu	$2,$0,$2
	sra	$3,$2,16
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L319
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L386:
	lw	$2,12($21)
	and	$2,$4,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L295
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$4,2($17)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L295
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	lw	$12,44($sp)
	lw	$11,32($sp)
	lw	$8,52($sp)
	lw	$3,48($sp)
	j	$L295
$L290:
	.set	noreorder
	.set	nomacro
	j	$L291
	sw	$6,4($4)
	.set	macro
	.set	reorder

$L385:
	lui	$2,%hi(statistic_udp_frag_tbl_full)
	move	$4,$18
	lw	$3,%lo(statistic_udp_frag_tbl_full)($2)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	sw	$3,%lo(statistic_udp_frag_tbl_full)($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L260
	move	$2,$0
	.set	macro
	.set	reorder

$L288:
	.set	noreorder
	.set	nomacro
	beq	$10,$0,$L414
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$4,$22,16
	sw	$3,48($sp)
	sw	$8,52($sp)
	sw	$11,32($sp)
	.set	noreorder
	.set	nomacro
	jal	del_timer
	sw	$12,44($sp)
	.set	macro
	.set	reorder

	lui	$6,%hi(max_udp_frag_entry)
	lw	$2,4($22)
	lw	$5,8($22)
	lhu	$4,2($22)
	xor	$5,$5,$2
	lbu	$2,1($22)
	srl	$7,$5,16
	lw	$6,%lo(max_udp_frag_entry)($6)
	xor	$4,$4,$5
	lw	$5,44($22)
	xor	$4,$4,$7
	sb	$0,0($22)
	srl	$7,$4,8
	addiu	$6,$6,-1
	xor	$2,$4,$2
	lw	$3,48($sp)
	xor	$2,$2,$7
	lw	$8,52($sp)
	lw	$11,32($sp)
	lw	$12,44($sp)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L293
	and	$2,$2,$6
	.set	macro
	.set	reorder

	lw	$6,48($22)
	lui	$4,%hi(udp_cache_table)
	sw	$6,48($5)
$L294:
	lw	$5,48($22)
	li	$7,12			# 0xc
	lw	$6,44($22)
	mult	$2,$7
	addiu	$7,$22,52
	sw	$6,0($5)
	lui	$5,%hi(udp_cache_list_free)
	lw	$9,%lo(udp_cache_table)($4)
	addiu	$5,$5,%lo(udp_cache_list_free)
	mflo	$2
	lw	$6,4($5)
	lw	$9,0($9)
	lui	$4,%hi(statistic_udp_frag_free)
	addu	$2,$9,$2
	lw	$9,8($2)
	addiu	$9,$9,-1
	sw	$9,8($2)
	sw	$6,56($22)
	sw	$0,52($22)
	sw	$22,0($6)
	lw	$6,8($5)
	lw	$2,%lo(statistic_udp_frag_free)($4)
	addiu	$6,$6,1
	addiu	$2,$2,1
	sw	$7,4($5)
	sw	$6,8($5)
	.set	noreorder
	.set	nomacro
	j	$L292
	sw	$2,%lo(statistic_udp_frag_free)($4)
	.set	macro
	.set	reorder

$L383:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L260
	move	$2,$0
	.set	macro
	.set	reorder

$L382:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	move	$6,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L260
	move	$2,$0
	.set	macro
	.set	reorder

$L293:
	li	$5,12			# 0xc
	lui	$4,%hi(udp_cache_table)
	mult	$2,$5
	lw	$7,%lo(udp_cache_table)($4)
	lw	$6,48($22)
	lw	$7,0($7)
	mflo	$5
	addu	$5,$7,$5
	.set	noreorder
	.set	nomacro
	j	$L294
	sw	$6,4($5)
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.size	enter_fast_path, .-enter_fast_path
	.section	.rodata.str1.4
	.align	2
$LC10:
	.ascii	"napt num: %d, path num: %d.\012\000"
	.section	.text.fastpath_dump_napt_entry_num,"ax",@progbits
	.align	2
	.globl	fastpath_dump_napt_entry_num
	.set	nomips16
	.ent	fastpath_dump_napt_entry_num
	.type	fastpath_dump_napt_entry_num, @function
fastpath_dump_napt_entry_num:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$2,%lo(path_list_inuse)($2)
	move	$16,$5
	beq	$2,$0,$L416
	move	$7,$0

$L417:
	lw	$2,88($2)
	bne	$2,$0,$L417
	addiu	$7,$7,1

$L416:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	beq	$2,$0,$L418
	move	$6,$0

$L419:
	lw	$2,44($2)
	bne	$2,$0,$L419
	addiu	$6,$6,1

$L418:
	lui	$5,%hi($LC10)
	addu	$4,$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC10)

	addu	$2,$2,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fastpath_dump_napt_entry_num
	.size	fastpath_dump_napt_entry_num, .-fastpath_dump_napt_entry_num
	.section	.rodata.str1.4
	.align	2
$LC11:
	.ascii	"\012udp frag cache entry:\012\000"
	.align	2
$LC12:
	.ascii	"src_ip: 0x%08x src_port:0x%04x dst_ip: 0x%08x dst_port:0"
	.ascii	"x%04x frag_id:0x%04x protocol:0x%x status:0x%x \012\000"
	.align	2
$LC13:
	.ascii	"total num: %d \012\000"
	.section	.text.rtl_readUdpFragEntry,"ax",@progbits
	.align	2
	.globl	rtl_readUdpFragEntry
	.set	nomips16
	.ent	rtl_readUdpFragEntry
	.type	rtl_readUdpFragEntry, @function
rtl_readUdpFragEntry:
	.frame	$sp,64,$31		# vars= 0, regs= 7/0, args= 32, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$21,56($sp)
	sw	$20,52($sp)
	sw	$19,48($sp)
	sw	$18,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	lui	$4,%hi($LC11)
	addiu	$4,$4,%lo($LC11)
	lui	$21,%hi($LC12)
	move	$16,$0
	jal	printk
	move	$20,$0

	lui	$19,%hi(udp_cache_table)
	addiu	$21,$21,%lo($LC12)
	li	$18,768			# 0x300
$L427:
	lw	$2,%lo(udp_cache_table)($19)
	lw	$2,0($2)
	addu	$2,$2,$16
	lw	$17,0($2)
	beq	$17,$0,$L425
	nop

$L426:
	lhu	$2,14($17)
	move	$4,$21
	lw	$5,4($17)
	addiu	$20,$20,1
	lhu	$6,12($17)
	lw	$7,8($17)
	sw	$2,16($sp)
	lhu	$2,2($17)
	sw	$2,20($sp)
	lbu	$2,1($17)
	sw	$2,24($sp)
	lbu	$2,0($17)
	jal	printk
	sw	$2,28($sp)

	lw	$17,44($17)
	bne	$17,$0,$L426
	nop

$L425:
	addiu	$16,$16,12
	bne	$16,$18,$L427
	move	$5,$20

	lui	$4,%hi($LC13)
	lw	$31,60($sp)
	addiu	$4,$4,%lo($LC13)
	lw	$21,56($sp)
	lw	$20,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	j	printk
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	rtl_readUdpFragEntry
	.size	rtl_readUdpFragEntry, .-rtl_readUdpFragEntry
	.section	.rodata.str1.4
	.align	2
$LC14:
	.ascii	"fp_napt\000"
	.align	2
$LC15:
	.ascii	"fp_path\000"
	.section	.text.init_fastpath_debug_proc,"ax",@progbits
	.align	2
	.globl	init_fastpath_debug_proc
	.set	nomips16
	.ent	init_fastpath_debug_proc
	.type	init_fastpath_debug_proc, @function
init_fastpath_debug_proc:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	lui	$16,%hi(proc_root)
	lui	$4,%hi($LC14)
	lui	$7,%hi(fastpath_napt_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC14)
	move	$5,$0
	addiu	$7,$7,%lo(fastpath_napt_proc_fops)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC15)
	lui	$7,%hi(fastpath_path_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	lui	$3,%hi(fp_napt)
	addiu	$4,$4,%lo($LC15)
	move	$5,$0
	addiu	$7,$7,%lo(fastpath_path_proc_fops)
	sw	$2,%lo(fp_napt)($3)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$3,%hi(fp_path)
	lw	$31,28($sp)
	lw	$16,24($sp)
	sw	$2,%lo(fp_path)($3)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	init_fastpath_debug_proc
	.size	init_fastpath_debug_proc, .-init_fastpath_debug_proc
	.section	.text.remove_fastpath_debug_proc,"ax",@progbits
	.align	2
	.globl	remove_fastpath_debug_proc
	.set	nomips16
	.ent	remove_fastpath_debug_proc
	.type	remove_fastpath_debug_proc, @function
remove_fastpath_debug_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(fp_napt)
	lw	$2,%lo(fp_napt)($16)
	beq	$2,$0,$L432
	lui	$4,%hi($LC14)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC14)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_napt)($16)
$L432:
	lui	$16,%hi(fp_path)
	lw	$2,%lo(fp_path)($16)
	beq	$2,$0,$L431
	lui	$4,%hi($LC15)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC15)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_path)($16)
$L431:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	remove_fastpath_debug_proc
	.size	remove_fastpath_debug_proc, .-remove_fastpath_debug_proc
	.section	.text.init_table_napt,"ax",@progbits
	.align	2
	.globl	init_table_napt
	.set	nomips16
	.ent	init_table_napt
	.type	init_table_napt, @function
init_table_napt:
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
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($19)
	move	$20,$5
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$18,%hi(table_napt)

	move	$21,$2
	beq	$2,$0,$L440
	sw	$2,%lo(table_napt)($18)

	lui	$6,%hi(napt_list_inuse)
	lui	$3,%hi(napt_list_free)
	addiu	$2,$6,%lo(napt_list_inuse)
	sll	$5,$17,4
	sll	$4,$17,2
	addiu	$16,$3,%lo(napt_list_free)
	subu	$4,$5,$4
	li	$5,32			# 0x20
	sw	$0,8($2)
	sw	$2,4($2)
	lui	$2,%hi(napt_table_list_max)
	sw	$0,%lo(napt_list_free)($3)
	sw	$0,%lo(napt_list_inuse)($6)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$17,%lo(napt_table_list_max)($2)

	sw	$2,0($21)
	lw	$3,%lo(table_napt)($18)
	lw	$5,0($3)
	beq	$5,$0,$L435
	li	$2,-1			# 0xffffffffffffffff

	blez	$17,$L436
	move	$2,$0

	j	$L437
	move	$4,$0

$L444:
	lw	$5,0($3)
$L437:
	addu	$5,$5,$2
	addiu	$4,$4,1
	sw	$0,8($5)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(table_napt)($18)
	lw	$5,0($3)
	addu	$5,$5,$2
	addiu	$2,$2,12
	bne	$4,$17,$L444
	sw	$5,4($5)

$L436:
	blez	$20,$L441
	move	$17,$0

	j	$L438
	nop

$L445:
	lw	$3,4($16)
	sw	$0,44($2)
	sw	$3,48($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$20,$L441
	sw	$2,8($16)

$L438:
	lw	$4,24($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	bne	$2,$0,$L445
	addiu	$4,$2,44

	lw	$31,44($sp)
	li	$2,-2			# 0xfffffffffffffffe
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L441:
	move	$2,$0
$L435:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L440:
	j	$L435
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_napt
	.size	init_table_napt, .-init_table_napt
	.section	.text.init_table_path,"ax",@progbits
	.align	2
	.globl	init_table_path
	.set	nomips16
	.ent	init_table_path
	.type	init_table_path, @function
init_table_path:
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
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($19)
	move	$20,$5
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$18,%hi(table_path)

	move	$21,$2
	beq	$2,$0,$L452
	sw	$2,%lo(table_path)($18)

	lui	$6,%hi(path_list_inuse)
	lui	$3,%hi(path_list_free)
	addiu	$2,$6,%lo(path_list_inuse)
	sll	$5,$17,4
	sll	$4,$17,2
	addiu	$16,$3,%lo(path_list_free)
	subu	$4,$5,$4
	li	$5,32			# 0x20
	sw	$0,8($2)
	sw	$2,4($2)
	lui	$2,%hi(path_table_list_max)
	sw	$0,%lo(path_list_free)($3)
	sw	$0,%lo(path_list_inuse)($6)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$17,%lo(path_table_list_max)($2)

	sw	$2,0($21)
	lw	$3,%lo(table_path)($18)
	lw	$5,0($3)
	beq	$5,$0,$L447
	li	$2,-1			# 0xffffffffffffffff

	blez	$17,$L448
	move	$2,$0

	j	$L449
	move	$4,$0

$L456:
	lw	$5,0($3)
$L449:
	addu	$5,$5,$2
	addiu	$4,$4,1
	sw	$0,8($5)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(table_path)($18)
	lw	$5,0($3)
	addu	$5,$5,$2
	addiu	$2,$2,12
	bne	$4,$17,$L456
	sw	$5,4($5)

$L448:
	blez	$20,$L453
	move	$17,$0

	j	$L450
	nop

$L457:
	lw	$3,4($16)
	sw	$0,88($2)
	sw	$3,92($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$20,$L453
	sw	$2,8($16)

$L450:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	bne	$2,$0,$L457
	addiu	$4,$2,88

	lw	$31,44($sp)
	li	$2,-2			# 0xfffffffffffffffe
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L453:
	move	$2,$0
$L447:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L452:
	j	$L447
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_path
	.size	init_table_path, .-init_table_path
	.section	.rodata.str1.4
	.align	2
$LC16:
	.ascii	"%s:%s\012\000"
	.align	2
$LC17:
	.ascii	"Realtek FastPath\000"
	.align	2
$LC18:
	.ascii	"v1.03\000"
	.section	.text.get_fastpath_module_info,"ax",@progbits
	.align	2
	.globl	get_fastpath_module_info
	.set	nomips16
	.ent	get_fastpath_module_info
	.type	get_fastpath_module_info, @function
get_fastpath_module_info:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L460
	nop

	lui	$5,%hi($LC16)
	lui	$6,%hi($LC17)
	lui	$7,%hi($LC18)
	addiu	$5,$5,%lo($LC16)
	addiu	$6,$6,%lo($LC17)
	addiu	$7,$7,%lo($LC18)
	j	sprintf
	nop

$L460:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_fastpath_module_info
	.size	get_fastpath_module_info, .-get_fastpath_module_info
	.globl	fastpath_path_proc_fops
	.section	.data.fastpath_path_proc_fops,"aw",@progbits
	.align	2
	.type	fastpath_path_proc_fops, @object
	.size	fastpath_path_proc_fops, 108
fastpath_path_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_path_single_write
	.space	28
	.word	fastpath_path_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fastpath_napt_proc_fops
	.section	.data.fastpath_napt_proc_fops,"aw",@progbits
	.align	2
	.type	fastpath_napt_proc_fops, @object
	.size	fastpath_napt_proc_fops, 108
fastpath_napt_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_napt_single_write
	.space	28
	.word	fastpath_napt_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	rtl_check_bypass_packet_hook
	.section	.bss.rtl_check_bypass_packet_hook,"aw",@nobits
	.align	2
	.type	rtl_check_bypass_packet_hook, @object
	.size	rtl_check_bypass_packet_hook, 4
rtl_check_bypass_packet_hook:
	.space	4
	.globl	rtl_init_bypass_packet_hook
	.section	.bss.rtl_init_bypass_packet_hook,"aw",@nobits
	.align	2
	.type	rtl_init_bypass_packet_hook, @object
	.size	rtl_init_bypass_packet_hook, 4
rtl_init_bypass_packet_hook:
	.space	4
	.globl	fastpath_packet_bypassed_num
	.section	.data.fastpath_packet_bypassed_num,"aw",@progbits
	.align	2
	.type	fastpath_packet_bypassed_num, @object
	.size	fastpath_packet_bypassed_num, 4
fastpath_packet_bypassed_num:
	.word	10
	.globl	negative_cache_table
	.section	.bss.negative_cache_table,"aw",@nobits
	.align	2
	.type	negative_cache_table, @object
	.size	negative_cache_table, 4
negative_cache_table:
	.space	4
	.globl	negative_cache_list_free
	.section	.bss.negative_cache_list_free,"aw",@nobits
	.align	2
	.type	negative_cache_list_free, @object
	.size	negative_cache_list_free, 12
negative_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.section	.bss.udp_cache_table,"aw",@nobits
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.globl	udp_cache_list_free
	.section	.bss.udp_cache_list_free,"aw",@nobits
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	fp_napt
	.section	.bss.fp_napt,"aw",@nobits
	.align	2
	.type	fp_napt, @object
	.size	fp_napt, 4
fp_napt:
	.space	4
	.globl	fp_route
	.section	.bss.fp_route,"aw",@nobits
	.align	2
	.type	fp_route, @object
	.size	fp_route, 4
fp_route:
	.space	4
	.globl	fp_path
	.section	.bss.fp_path,"aw",@nobits
	.align	2
	.type	fp_path, @object
	.size	fp_path, 4
fp_path:
	.space	4
	.globl	fp_arp
	.section	.bss.fp_arp,"aw",@nobits
	.align	2
	.type	fp_arp, @object
	.size	fp_arp, 4
fp_arp:
	.space	4
	.globl	table_path
	.section	.bss.table_path,"aw",@nobits
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.globl	path_list_free
	.section	.bss.path_list_free,"aw",@nobits
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	path_list_inuse
	.section	.bss.path_list_inuse,"aw",@nobits
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	table_napt
	.section	.bss.table_napt,"aw",@nobits
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.globl	napt_list_free
	.section	.bss.napt_list_free,"aw",@nobits
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	napt_list_inuse
	.section	.bss.napt_list_inuse,"aw",@nobits
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	route_list_free
	.section	.bss.route_list_free,"aw",@nobits
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	route_list_inuse
	.section	.bss.route_list_inuse,"aw",@nobits
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.section	.bss.max_udp_frag_entry,"aw",@nobits
	.align	2
	.type	max_udp_frag_entry, @object
	.size	max_udp_frag_entry, 4
max_udp_frag_entry:
	.space	4
	.section	.bss.napt_table_list_max,"aw",@nobits
	.align	2
	.type	napt_table_list_max, @object
	.size	napt_table_list_max, 4
napt_table_list_max:
	.space	4
	.section	.bss.path_table_list_max,"aw",@nobits
	.align	2
	.type	path_table_list_max, @object
	.size	path_table_list_max, 4
path_table_list_max:
	.space	4
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
