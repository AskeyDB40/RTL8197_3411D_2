	.file	1 "fastpath_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
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
	
	sw	$0,64($4)
	j	$31
	sw	$0,68($4)

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
	bne	$2,$0,$L8
	li	$2,1			# 0x1

	lw	$3,64($4)
	lui	$5,%hi(fastpath_packet_bypassed_num)
	addiu	$3,$3,1
	sw	$3,64($4)
	lw	$5,%lo(fastpath_packet_bypassed_num)($5)
	sltu	$3,$5,$3
	beq	$3,$0,$L9
	nop

$L8:
	sw	$2,68($4)
	move	$2,$0
$L9:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rtl_check_bypass_packet
	.size	rtl_check_bypass_packet, .-rtl_check_bypass_packet
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
	beq	$3,$0,$L34
	move	$2,$0

	move	$6,$0
$L31:
	lw	$5,8($3)
	sltu	$7,$5,$6
	bne	$7,$0,$L28
	and	$8,$4,$5

	lw	$7,4($3)
	beq	$8,$7,$L33
	nop

$L28:
	move	$5,$6
$L29:
	lw	$3,44($3)
	bne	$3,$0,$L31
	move	$6,$5

$L34:
	j	$31
	nop

$L33:
	j	$L29
	addiu	$2,$3,16

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.size	FastPath_Route, .-FastPath_Route
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
	
	srl	$2,$4,8
	xor	$2,$2,$4
	srl	$3,$6,16
	andi	$5,$5,0xffff
	xor	$2,$2,$6
	xor	$2,$2,$3
	srl	$8,$5,4
	lhu	$3,18($sp)
	xor	$2,$2,$8
	andi	$7,$7,0xffff
	lui	$8,%hi(path_table_list_max)
	lw	$10,%lo(path_table_list_max)($8)
	xor	$2,$2,$7
	lui	$9,%hi(table_path)
	xor	$2,$2,$3
	lw	$8,%lo(table_path)($9)
	srl	$11,$2,12
	xor	$2,$11,$2
	addiu	$10,$10,-1
	and	$2,$2,$10
	lw	$8,4($8)
	sll	$11,$2,4
	sll	$2,$2,2
	subu	$2,$11,$2
	addu	$2,$8,$2
	lw	$2,0($2)
	beq	$2,$0,$L53
	li	$11,255			# 0xff

	j	$L54
	lhu	$8,10($2)

$L37:
	lw	$2,80($2)
	beq	$2,$0,$L53
	nop

	lhu	$8,10($2)
$L54:
	bne	$8,$5,$L37
	nop

	lhu	$8,8($2)
	bne	$8,$7,$L37
	nop

	lw	$8,12($2)
	bne	$8,$4,$L37
	nop

	lw	$8,16($2)
	bne	$8,$6,$L37
	nop

	lbu	$12,0($2)
	bne	$12,$11,$L37
	nop

	lbu	$12,2($2)
	li	$6,1			# 0x1
	beq	$12,$6,$L51
	sb	$0,0($2)

	lhu	$13,32($2)
	lw	$6,28($2)
$L39:
	beq	$6,$0,$L53
	li	$11,1			# 0x1

	beq	$12,$11,$L52
	srl	$11,$4,16

	xor	$11,$4,$11
	xor	$11,$11,$5
	xor	$3,$11,$3
	xor	$3,$3,$6
	srl	$6,$6,8
	xor	$6,$3,$6
	srl	$2,$13,4
	xor	$2,$6,$2
	srl	$3,$2,12
	xor	$2,$3,$2
	and	$10,$2,$10
$L42:
	lw	$2,%lo(table_path)($9)
	sll	$3,$10,4
	sll	$10,$10,2
	lw	$2,4($2)
	subu	$10,$3,$10
	addu	$10,$2,$10
	lw	$2,0($10)
	beq	$2,$0,$L53
	li	$6,255			# 0xff

	j	$L55
	lw	$3,20($2)

$L43:
	lw	$2,80($2)
	beq	$2,$0,$L53
	nop

	lw	$3,20($2)
$L55:
	bne	$8,$3,$L43
	nop

	lhu	$3,24($2)
	bne	$3,$7,$L43
	nop

	lw	$3,28($2)
	bne	$4,$3,$L43
	nop

	lhu	$3,32($2)
	bne	$3,$5,$L43
	nop

	lbu	$3,0($2)
	bne	$3,$6,$L43
	nop

	j	$L43
	sb	$0,0($2)

$L53:
	j	$31
	nop

$L52:
	srl	$2,$8,8
	xor	$2,$8,$2
	xor	$3,$2,$3
	srl	$2,$7,4
	xor	$2,$3,$2
	xor	$2,$2,$6
	xor	$13,$2,$13
	srl	$6,$6,16
	xor	$6,$13,$6
	srl	$2,$6,12
	xor	$6,$2,$6
	j	$L42
	and	$10,$6,$10

$L51:
	lhu	$13,24($2)
	j	$L39
	lw	$6,20($2)

	.set	macro
	.set	reorder
	.end	mark_path_invalid
	.size	mark_path_invalid, .-mark_path_invalid
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
	.ascii	"%s:%s\012\000"
	.align	2
$LC1:
	.ascii	"Realtek FastPath\000"
	.align	2
$LC2:
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
	
	beq	$4,$0,$L63
	lui	$5,%hi($LC0)

	lui	$6,%hi($LC1)
	lui	$7,%hi($LC2)
	addiu	$5,$5,%lo($LC0)
	addiu	$6,$6,%lo($LC1)
	addiu	$7,$7,%lo($LC2)
	j	sprintf
	nop

$L63:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_fastpath_module_info
	.size	get_fastpath_module_info, .-get_fastpath_module_info
	.section	.rodata.str1.4
	.align	2
$LC3:
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
	beq	$2,$0,$L66
	move	$7,$0

$L73:
	lw	$2,88($2)
	bne	$2,$0,$L73
	addiu	$7,$7,1

$L66:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	beq	$2,$0,$L69
	move	$6,$0

$L72:
	lw	$2,44($2)
	bne	$2,$0,$L72
	addiu	$6,$6,1

$L69:
	lui	$5,%hi($LC3)
	addu	$4,$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC3)

	addu	$2,$2,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fastpath_dump_napt_entry_num
	.size	fastpath_dump_napt_entry_num, .-fastpath_dump_napt_entry_num
	.rdata
	.align	2
$LC4:
	.space	4
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
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$18,%hi(table_path)
	move	$21,$2
	beq	$2,$0,$L77
	sw	$2,%lo(table_path)($18)

	lui	$6,%hi(path_list_inuse)
	sll	$2,$17,2
	sll	$4,$17,4
	lui	$3,%hi(path_list_free)
	subu	$4,$4,$2
	addiu	$2,$6,%lo(path_list_inuse)
	addiu	$16,$3,%lo(path_list_free)
	sw	$0,%lo(path_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(path_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(path_list_inuse)($6)
	sw	$17,%lo(path_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,4($21)
	lw	$2,%lo(table_path)($18)
	lw	$4,4($2)
	beq	$4,$0,$L77
	nop

	blez	$17,$L78
	move	$2,$0

	move	$3,$0
	j	$L79
	move	$5,$18

$L86:
	lw	$4,%lo(table_path)($5)
	lw	$4,4($4)
$L79:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_path)($18)
	addiu	$3,$3,1
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_path)($18)
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$17,$L86
	addiu	$2,$2,12

$L78:
	blez	$19,$L80
	move	$17,$0

	j	$L87
	lw	$4,4($20)

$L81:
	sw	$0,88($2)
	lw	$5,4($16)
	sw	$5,92($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L80
	sw	$2,8($16)

	lw	$4,4($20)
$L87:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,88
	bne	$2,$0,$L81
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L82:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L80:
	lui	$2,%hi($LC4)
	lw	$4,%lo($LC4)($2)
	lw	$3,%lo(table_path)($18)
	move	$2,$0
	j	$L82
	sw	$4,0($3)

$L77:
	j	$L82
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_path
	.size	init_table_path, .-init_table_path
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
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$18,%hi(table_napt)
	move	$21,$2
	beq	$2,$0,$L89
	sw	$2,%lo(table_napt)($18)

	lui	$6,%hi(napt_list_inuse)
	sll	$2,$17,2
	sll	$4,$17,4
	lui	$3,%hi(napt_list_free)
	subu	$4,$4,$2
	addiu	$2,$6,%lo(napt_list_inuse)
	addiu	$16,$3,%lo(napt_list_free)
	sw	$0,%lo(napt_list_free)($3)
	li	$5,32			# 0x20
	lui	$3,%hi(napt_table_list_max)
	sw	$2,4($2)
	sw	$0,8($2)
	sw	$0,%lo(napt_list_inuse)($6)
	sw	$17,%lo(napt_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,4($21)
	lw	$2,%lo(table_napt)($18)
	lw	$4,4($2)
	beq	$4,$0,$L89
	nop

	blez	$17,$L90
	move	$2,$0

	move	$3,$0
	j	$L91
	move	$5,$18

$L98:
	lw	$4,%lo(table_napt)($5)
	lw	$4,4($4)
$L91:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_napt)($18)
	addiu	$3,$3,1
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_napt)($18)
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$17,$L98
	addiu	$2,$2,12

$L90:
	blez	$19,$L92
	move	$17,$0

	j	$L99
	lw	$4,24($20)

$L93:
	sw	$0,44($2)
	lw	$5,4($16)
	sw	$5,48($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L92
	sw	$2,8($16)

	lw	$4,24($20)
$L99:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,44
	bne	$2,$0,$L93
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L94:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L92:
	lui	$2,%hi($LC4)
	lw	$4,%lo($LC4)($2)
	lw	$3,%lo(table_napt)($18)
	move	$2,$0
	j	$L94
	sw	$4,0($3)

$L89:
	j	$L94
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_napt
	.size	init_table_napt, .-init_table_napt
	.section	.rodata.str1.4
	.align	2
$LC5:
	.ascii	"fp_napt\000"
	.align	2
$LC6:
	.ascii	"fp_path\000"
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
	beq	$2,$0,$L101
	lui	$4,%hi($LC5)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC5)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_napt)($16)
$L101:
	lui	$16,%hi(fp_path)
	lw	$2,%lo(fp_path)($16)
	beq	$2,$0,$L103
	lui	$4,%hi($LC6)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC6)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_path)($16)
$L103:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	remove_fastpath_debug_proc
	.size	remove_fastpath_debug_proc, .-remove_fastpath_debug_proc
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
	lui	$4,%hi($LC5)
	lui	$7,%hi(fastpath_napt_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC5)
	addiu	$7,$7,%lo(fastpath_napt_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC6)
	lui	$7,%hi(fastpath_path_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	lui	$3,%hi(fp_napt)
	addiu	$4,$4,%lo($LC6)
	addiu	$7,$7,%lo(fastpath_path_proc_fops)
	move	$5,$0
	sw	$2,%lo(fp_napt)($3)
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lw	$16,24($sp)
	lui	$3,%hi(fp_path)
	sw	$2,%lo(fp_path)($3)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	init_fastpath_debug_proc
	.size	init_fastpath_debug_proc, .-init_fastpath_debug_proc
	.section	.rodata.str1.4
	.align	2
$LC7:
	.ascii	"\012udp frag cache entry:\012\000"
	.align	2
$LC8:
	.ascii	"src_ip: 0x%08x src_port:0x%04x dst_ip: 0x%08x dst_port:0"
	.ascii	"x%04x frag_id:0x%04x protocol:0x%x status:0x%x \012\000"
	.align	2
$LC9:
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
	lui	$4,%hi($LC7)
	addiu	$4,$4,%lo($LC7)
	jal	printk
	lui	$21,%hi($LC8)

	addiu	$21,$21,%lo($LC8)
	move	$16,$0
	move	$20,$0
	lui	$19,%hi(udp_cache_table)
	li	$18,768			# 0x300
	lw	$2,%lo(udp_cache_table)($19)
$L113:
	lw	$2,4($2)
	addu	$2,$2,$16
	lw	$17,0($2)
	beq	$17,$0,$L107
	nop

$L108:
	lhu	$2,14($17)
	lw	$5,4($17)
	lhu	$6,12($17)
	lw	$7,8($17)
	sw	$2,16($sp)
	lhu	$2,2($17)
	move	$4,$21
	addiu	$20,$20,1
	sw	$2,20($sp)
	lbu	$2,1($17)
	sw	$2,24($sp)
	lbu	$2,0($17)
	jal	printk
	sw	$2,28($sp)

	lw	$17,44($17)
	bne	$17,$0,$L108
	nop

$L107:
	addiu	$16,$16,12
	bne	$16,$18,$L113
	lw	$2,%lo(udp_cache_table)($19)

	move	$5,$20
	lw	$31,60($sp)
	lw	$21,56($sp)
	lw	$20,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	lui	$4,%hi($LC9)
	addiu	$4,$4,%lo($LC9)
	j	printk
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	rtl_readUdpFragEntry
	.size	rtl_readUdpFragEntry, .-rtl_readUdpFragEntry
	.section	.rodata.str1.4
	.align	2
$LC10:
	.ascii	"UDP\000"
	.align	2
$LC11:
	.ascii	"TCP\000"
	.align	2
$LC12:
	.ascii	"~Path: [%s] in-S=0x%08X:%-5u in-D=0x%08X:%-5u out-S=0x%0"
	.ascii	"8X:%-5u out-D=0x%08X:%-5u out-ifname=%-5s <%u> {%d}\000"
	.align	2
$LC13:
	.ascii	",[PreMark:%d,PostMark:%d]\000"
	.align	2
$LC14:
	.ascii	",[policy_route_mark:%d]\000"
	.align	2
$LC15:
	.ascii	",[dscp_in:%d,dscp_out:%d]\000"
	.align	2
$LC16:
	.ascii	",[vlanPrio_out:%d]\000"
	.align	2
$LC17:
	.ascii	"\012\000"
	.section	.text.fastpath_table_path,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_table_path
	.type	fastpath_table_path, @function
fastpath_table_path:
	.frame	$sp,104,$31		# vars= 8, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	sw	$23,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lui	$2,%hi(path_list_inuse)
	lw	$16,%lo(path_list_inuse)($2)
	bne	$16,$0,$L121
	lui	$3,%hi($LC11)

$L115:
	lw	$31,100($sp)
	lw	$fp,96($sp)
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,104

$L121:
	lui	$fp,%hi($LC10)
	lui	$23,%hi($LC12)
	lui	$22,%hi($LC13)
	lui	$21,%hi($LC14)
	lui	$20,%hi($LC15)
	lui	$19,%hi($LC16)
	lui	$18,%hi($LC17)
	addiu	$3,$3,%lo($LC11)
	addiu	$fp,$fp,%lo($LC10)
	addiu	$23,$23,%lo($LC12)
	addiu	$22,$22,%lo($LC13)
	addiu	$21,$21,%lo($LC14)
	addiu	$20,$20,%lo($LC15)
	addiu	$19,$19,%lo($LC16)
	addiu	$18,$18,%lo($LC17)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L122:
	lhu	$7,10($16)
	lw	$5,4($16)
	lw	$6,12($16)
	sw	$2,16($sp)
	lhu	$2,8($16)
	xor	$5,$5,$17
	move	$4,$fp
	sw	$2,20($sp)
	lw	$2,20($16)
	movz	$4,$3,$5
	move	$5,$4
	sw	$2,24($sp)
	lhu	$2,24($16)
	move	$4,$23
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
	sw	$3,56($sp)
	jal	printk
	sw	$2,48($sp)

	lw	$6,48($16)
	lw	$5,44($16)
	jal	printk
	move	$4,$22

	lw	$5,60($16)
	jal	printk
	move	$4,$21

	lbu	$6,73($16)
	lbu	$5,72($16)
	jal	printk
	move	$4,$20

	lhu	$5,78($16)
	jal	printk
	move	$4,$19

	jal	printk
	move	$4,$18

	lw	$16,88($16)
	beq	$16,$0,$L115
	lw	$3,56($sp)

	j	$L122
	lw	$2,16($16)

	.set	macro
	.set	reorder
	.end	fastpath_table_path
	.size	fastpath_table_path, .-fastpath_table_path
	.section	.rodata.str1.4
	.align	2
$LC18:
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
	bne	$16,$0,$L130
	lui	$20,%hi($LC11)

	lw	$31,60($sp)
$L132:
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,64

$L130:
	lui	$19,%hi($LC10)
	lui	$18,%hi($LC18)
	addiu	$20,$20,%lo($LC11)
	addiu	$19,$19,%lo($LC10)
	addiu	$18,$18,%lo($LC18)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L131:
	lw	$5,4($16)
	lw	$6,8($16)
	lw	$7,12($16)
	sw	$2,16($sp)
	lw	$2,20($16)
	xor	$5,$5,$17
	move	$3,$19
	sw	$2,20($sp)
	lw	$2,24($16)
	movz	$3,$20,$5
	move	$5,$3
	sw	$2,24($sp)
	lw	$2,28($16)
	move	$4,$18
	sw	$2,28($sp)
	lw	$2,32($16)
	jal	printk
	sw	$2,32($sp)

	lw	$16,44($16)
	beq	$16,$0,$L132
	lw	$31,60($sp)

	j	$L131
	lw	$2,16($16)

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
	addiu	$5,$5,%lo(fastpath_table_path)
	move	$6,$0
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
	addiu	$5,$5,%lo(fastpath_table_napt)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_napt_single_open
	.size	fastpath_napt_single_open, .-fastpath_napt_single_open
	.section	.text.cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	cache_timeout
	.type	cache_timeout, @function
cache_timeout:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lbu	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L142
	move	$16,$4
	.set	macro
	.set	reorder

	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L142:
	lw	$3,4($16)
	lw	$4,8($4)
	lhu	$2,2($16)
	lbu	$18,1($16)
	xor	$4,$4,$3
	lui	$3,%hi(max_udp_frag_entry)
	xor	$2,$2,$4
	lw	$3,%lo(max_udp_frag_entry)($3)
	srl	$4,$4,16
	lui	$17,%hi(udp_cache_table)
	xor	$2,$2,$4
	lw	$4,%lo(udp_cache_table)($17)
	xor	$18,$2,$18
	srl	$2,$2,8
	xor	$18,$18,$2
	addiu	$3,$3,-1
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	and	$18,$18,$3
	.set	macro
	.set	reorder

	lw	$2,44($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L139
	sb	$0,0($16)
	.set	macro
	.set	reorder

	lw	$4,48($16)
	sll	$3,$18,4
	sll	$18,$18,2
	sw	$4,48($2)
	subu	$18,$3,$18
$L140:
	lw	$3,48($16)
	lw	$4,44($16)
	lui	$2,%hi(udp_cache_list_free)
	addiu	$2,$2,%lo(udp_cache_list_free)
	sw	$4,0($3)
	lw	$3,%lo(udp_cache_table)($17)
	addiu	$5,$16,52
	lw	$3,4($3)
	addu	$18,$3,$18
	lw	$3,8($18)
	addiu	$3,$3,-1
	sw	$3,8($18)
	sw	$0,52($16)
	lw	$3,4($2)
	sw	$3,56($16)
	lw	$3,4($2)
	sw	$16,0($3)
	lw	$4,8($2)
	lw	$3,%lo(udp_cache_table)($17)
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
	lui	$2,%hi(statistic_udp_frag_free)
	lw	$3,%lo(statistic_udp_frag_free)($2)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	addiu	$3,$3,1
	sw	$3,%lo(statistic_udp_frag_free)($2)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L139:
	lw	$2,%lo(udp_cache_table)($17)
	sll	$4,$18,4
	lw	$3,48($16)
	lw	$2,4($2)
	sll	$18,$18,2
	subu	$18,$4,$18
	addu	$2,$2,$18
	.set	noreorder
	.set	nomacro
	j	$L140
	sw	$3,4($2)
	.set	macro
	.set	reorder

	.end	cache_timeout
	.size	cache_timeout, .-cache_timeout
	.section	.text.rtk_idleNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_idleNaptConnection
	.set	nomips16
	.ent	rtk_idleNaptConnection
	.type	rtk_idleNaptConnection, @function
rtk_idleNaptConnection:
	.frame	$sp,48,$31		# vars= 8, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$17,%hi(table_path)
	move	$16,$4
	lw	$4,%lo(table_path)($17)
	sw	$5,16($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$6,20($sp)
	.set	macro
	.set	reorder

	lw	$12,4($16)
	lw	$9,20($16)
	lw	$14,24($16)
	lw	$24,8($16)
	xor	$4,$9,$12
	srl	$2,$12,8
	lw	$3,0($16)
	xor	$4,$4,$14
	xor	$4,$4,$2
	srl	$2,$9,16
	xor	$4,$4,$2
	srl	$2,$24,4
	xor	$4,$4,$2
	li	$10,17			# 0x11
	lui	$2,%hi(path_table_list_max)
	xori	$8,$3,0x2
	li	$7,6			# 0x6
	lw	$19,%lo(path_table_list_max)($2)
	movz	$10,$7,$8
	xor	$4,$4,$10
	lw	$13,%lo(table_path)($17)
	srl	$2,$4,12
	xor	$4,$2,$4
	addiu	$19,$19,-1
	and	$2,$4,$19
	lw	$18,4($13)
	sll	$4,$2,4
	sll	$2,$2,2
	subu	$2,$4,$2
	addu	$2,$18,$2
	lw	$2,0($2)
	lui	$4,%hi(jiffies)
	lw	$25,%lo(jiffies)($4)
	lw	$5,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L159
	lw	$6,20($sp)
	.set	macro
	.set	reorder

	li	$7,-16777216			# 0xffffffffff000000
	ori	$8,$7,0x100
	lw	$15,16($16)
	lw	$11,12($16)
	.set	noreorder
	.set	nomacro
	j	$L150
	ori	$7,$7,0xff00
	.set	macro
	.set	reorder

$L148:
	lw	$2,80($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L160
	srl	$4,$9,8
	.set	macro
	.set	reorder

$L150:
	lw	$4,4($2)
	bne	$3,$4,$L148
	lw	$4,0($2)
	and	$4,$4,$7
	bne	$4,$8,$L148
	lw	$4,12($2)
	bne	$12,$4,$L148
	lhu	$4,10($2)
	bne	$24,$4,$L148
	lw	$4,20($2)
	bne	$4,$11,$L148
	lhu	$4,24($2)
	bne	$4,$15,$L148
	lw	$4,16($2)
	bne	$9,$4,$L148
	lhu	$4,8($2)
	.set	noreorder
	.set	nomacro
	bne	$14,$4,$L148
	subu	$4,$25,$5
	.set	macro
	.set	reorder

	lw	$2,36($2)
	subu	$4,$4,$2
	bltz	$4,$L158
$L147:
	srl	$4,$9,8
$L160:
	srl	$2,$14,4
	xor	$2,$4,$2
	xor	$2,$2,$9
	xor	$2,$2,$10
	xor	$2,$2,$15
	srl	$4,$11,16
	xor	$2,$2,$11
	xor	$2,$2,$4
	srl	$4,$2,12
	xor	$2,$4,$2
	and	$2,$2,$19
	sll	$4,$2,4
	sll	$2,$2,2
	subu	$2,$4,$2
	addu	$18,$18,$2
	lw	$2,0($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L151
	li	$7,-16777216			# 0xffffffffff000000
	.set	macro
	.set	reorder

	ori	$8,$7,0x200
	.set	noreorder
	.set	nomacro
	j	$L155
	ori	$7,$7,0xff00
	.set	macro
	.set	reorder

$L152:
	lw	$2,80($2)
	beq	$2,$0,$L151
$L155:
	lw	$4,4($2)
	bne	$3,$4,$L152
	lw	$4,0($2)
	and	$4,$4,$7
	bne	$4,$8,$L152
	lw	$4,16($2)
	bne	$11,$4,$L152
	lhu	$4,8($2)
	bne	$15,$4,$L152
	lw	$4,20($2)
	bne	$9,$4,$L152
	lhu	$4,24($2)
	bne	$14,$4,$L152
	lw	$4,28($2)
	bne	$12,$4,$L152
	lhu	$4,32($2)
	bne	$24,$4,$L152
	lw	$2,36($2)
	subu	$5,$25,$5
	subu	$5,$5,$2
	bgez	$5,$L151
$L158:
	sw	$2,0($6)
	lw	$2,%lo(table_path)($17)
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
	j	$L149
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L151:
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
$L149:
	lw	$31,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,48
	.set	macro
	.set	reorder

$L159:
	lw	$15,16($16)
	.set	noreorder
	.set	nomacro
	j	$L147
	lw	$11,12($16)
	.set	macro
	.set	reorder

	.end	rtk_idleNaptConnection
	.size	rtk_idleNaptConnection, .-rtk_idleNaptConnection
	.section	.text.negative_cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	negative_cache_timeout
	.type	negative_cache_timeout, @function
negative_cache_timeout:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	move	$16,$4
	lw	$3,4($16)
	lw	$4,8($4)
	lhu	$2,2($16)
	lbu	$18,1($16)
	xor	$4,$4,$3
	lui	$3,%hi(max_udp_frag_entry)
	xor	$2,$2,$4
	lw	$3,%lo(max_udp_frag_entry)($3)
	srl	$4,$4,16
	lui	$17,%hi(negative_cache_table)
	xor	$2,$2,$4
	lw	$4,%lo(negative_cache_table)($17)
	xor	$18,$2,$18
	srl	$2,$2,8
	xor	$18,$18,$2
	addiu	$3,$3,-1
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	and	$18,$18,$3
	.set	macro
	.set	reorder

	lw	$2,40($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L162
	sb	$0,0($16)
	.set	macro
	.set	reorder

	lw	$4,44($16)
	sll	$3,$18,4
	sll	$18,$18,2
	sw	$4,44($2)
$L163:
	lw	$2,44($16)
	lw	$4,40($16)
	subu	$18,$3,$18
	addiu	$5,$16,48
	sw	$4,0($2)
	lw	$3,%lo(negative_cache_table)($17)
	lui	$2,%hi(negative_cache_list_free)
	addiu	$2,$2,%lo(negative_cache_list_free)
	addu	$18,$3,$18
	lw	$3,12($18)
	addiu	$3,$3,-1
	sw	$3,12($18)
	sw	$0,48($16)
	lw	$3,4($2)
	sw	$3,52($16)
	lw	$3,4($2)
	sw	$16,0($3)
	lw	$4,8($2)
	lw	$3,%lo(negative_cache_table)($17)
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
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L162:
	lw	$5,%lo(negative_cache_table)($17)
	sll	$3,$18,4
	lw	$4,44($16)
	sll	$18,$18,2
	subu	$2,$3,$18
	addu	$2,$5,$2
	.set	noreorder
	.set	nomacro
	j	$L163
	sw	$4,8($2)
	.set	macro
	.set	reorder

	.end	negative_cache_timeout
	.size	negative_cache_timeout, .-negative_cache_timeout
	.section	.rodata.str1.4
	.align	2
$LC19:
	.ascii	"lo\000"
	.section	.text.enter_fast_path,"ax",@progbits
	.align	2
	.globl	enter_fast_path
	.set	nomips16
	.ent	enter_fast_path
	.type	enter_fast_path, @function
enter_fast_path:
	.frame	$sp,120,$31		# vars= 56, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$fp,112($sp)
	sw	$23,108($sp)
	sw	$22,104($sp)
	sw	$21,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
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
	move	$17,$2
	.set	macro
	.set	reorder

	move	$16,$2
	lbu	$2,8($2)
	sltu	$2,$2,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L166
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$19,9($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L167
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L167
$L166:
	move	$2,$0
$L168:
	lw	$31,116($sp)
$L319:
	lw	$fp,112($sp)
	lw	$23,108($sp)
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
	addiu	$sp,$sp,120
	.set	macro
	.set	reorder

$L167:
	lw	$21,0($16)
	move	$4,$16
	move	$6,$18
	ext	$21,$21,24,4
	sll	$21,$21,2
	addu	$21,$16,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_pre_process_check
	move	$5,$21
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L319
	lw	$31,116($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi($LC19)
	addiu	$3,$3,%lo($LC19)
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($17)
1:	lbu	$1,($3)
	addiu	$17,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($17)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L166
	lw	$4,16($16)
	lhu	$3,6($16)
	lw	$fp,12($16)
	sw	$4,32($sp)
	lhu	$20,0($21)
	lhu	$23,2($21)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L287
	andi	$4,$3,0x3fff
	.set	macro
	.set	reorder

$L170:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L173
	lui	$2,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

	lw	$4,%lo(statistic_udp_frag_rx)($2)
	addiu	$4,$4,1
	sw	$4,%lo(statistic_udp_frag_rx)($2)
	li	$2,8192			# 0x2000
	beq	$3,$2,$L288
$L174:
	andi	$2,$3,0x2000
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L176
	andi	$3,$3,0x1fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L176
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$6,40($sp)
	sw	$0,36($sp)
$L175:
	lw	$2,16($16)
	lw	$22,12($16)
	lhu	$17,4($16)
	lbu	$6,9($16)
	xor	$3,$2,$22
	lui	$7,%hi(max_udp_frag_entry)
	xor	$4,$17,$3
	lw	$9,%lo(max_udp_frag_entry)($7)
	srl	$8,$3,16
	xor	$8,$4,$8
	lui	$3,%hi(negative_cache_table)
	xor	$5,$8,$6
	lw	$4,%lo(negative_cache_table)($3)
	srl	$8,$8,8
	xor	$5,$5,$8
	addiu	$9,$9,-1
	and	$5,$5,$9
	sw	$2,68($sp)
	sw	$3,76($sp)
	sw	$5,64($sp)
	sw	$6,60($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$7,72($sp)
	.set	macro
	.set	reorder

	lw	$3,76($sp)
	lw	$5,64($sp)
	lw	$2,68($sp)
	lw	$8,%lo(negative_cache_table)($3)
	sll	$4,$5,4
	sll	$5,$5,2
	subu	$5,$4,$5
	addu	$5,$8,$5
	lw	$4,4($5)
	lw	$6,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L177
	lw	$7,72($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L270
	li	$9,3			# 0x3
	.set	macro
	.set	reorder

$L178:
	lw	$4,40($4)
	beq	$4,$0,$L177
$L270:
	lhu	$5,2($4)
	bne	$5,$17,$L178
	lw	$5,4($4)
	bne	$22,$5,$L178
	lw	$5,8($4)
	bne	$2,$5,$L178
	lbu	$5,1($4)
	bne	$5,$6,$L178
	lbu	$5,0($4)
	bne	$5,$9,$L178
	lhu	$2,2($8)
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
	sh	$2,2($8)
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
	bne	$4,$0,$L168
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,12($16)
$L318:
	lw	$5,16($16)
	lhu	$17,4($16)
	lbu	$8,9($16)
	xor	$4,$5,$2
	lui	$6,%hi(udp_cache_table)
	lw	$10,%lo(max_udp_frag_entry)($7)
	xor	$9,$17,$4
	srl	$4,$4,16
	xor	$9,$9,$4
	lw	$4,%lo(udp_cache_table)($6)
	xor	$22,$9,$8
	srl	$9,$9,8
	xor	$22,$22,$9
	addiu	$10,$10,-1
	and	$22,$22,$10
	sw	$2,68($sp)
	sw	$3,76($sp)
	sw	$5,64($sp)
	sw	$6,60($sp)
	sw	$7,72($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$8,56($sp)
	.set	macro
	.set	reorder

	lw	$6,60($sp)
	sll	$9,$22,4
	sll	$22,$22,2
	lw	$6,%lo(udp_cache_table)($6)
	subu	$22,$9,$22
	lw	$2,68($sp)
	lw	$4,4($6)
	lw	$3,76($sp)
	lw	$5,64($sp)
	addu	$4,$4,$22
	lw	$22,0($4)
	lw	$7,72($sp)
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L181
	lw	$8,56($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L269
	li	$9,1			# 0x1
	.set	macro
	.set	reorder

$L182:
	lw	$22,44($22)
	beq	$22,$0,$L181
$L269:
	lhu	$4,2($22)
	bne	$4,$17,$L182
	lw	$4,4($22)
	bne	$2,$4,$L182
	lw	$4,8($22)
	bne	$5,$4,$L182
	lbu	$4,1($22)
	bne	$4,$8,$L182
	lbu	$4,0($22)
	bne	$4,$9,$L182
	lhu	$2,2($6)
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
	sh	$2,2($6)
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
	beq	$22,$0,$L322
	lw	$2,36($sp)
	.set	macro
	.set	reorder

	lw	$7,36($sp)
	lhu	$20,12($22)
	lhu	$23,14($22)
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L187
	sw	$0,52($sp)
	.set	macro
	.set	reorder

	li	$8,1			# 0x1
	li	$11,1			# 0x1
	sw	$8,36($sp)
	sw	$11,52($sp)
$L187:
	lui	$11,%hi(table_path)
	lw	$2,32($sp)
	lw	$3,32($sp)
	lw	$4,%lo(table_path)($11)
	xor	$2,$2,$fp
	srl	$3,$3,16
	sw	$2,48($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$3,44($sp)
	.set	macro
	.set	reorder

	lw	$5,48($sp)
	lw	$6,44($sp)
	srl	$4,$fp,8
	xor	$4,$5,$4
	xor	$4,$4,$6
	xor	$4,$4,$19
	lui	$2,%hi(path_table_list_max)
	xor	$4,$4,$23
	lw	$2,%lo(path_table_list_max)($2)
	srl	$3,$20,4
	lui	$7,%hi(table_path)
	xor	$3,$4,$3
	lw	$4,%lo(table_path)($7)
	srl	$5,$3,12
	xor	$3,$5,$3
	addiu	$2,$2,-1
	and	$2,$3,$2
	lw	$3,4($4)
	sll	$5,$2,4
	sll	$2,$2,2
	subu	$2,$5,$2
	addu	$2,$3,$2
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L192
	li	$5,255			# 0xff
	.set	macro
	.set	reorder

	li	$10,2			# 0x2
	li	$9,1			# 0x1
	li	$7,17			# 0x11
	.set	noreorder
	.set	nomacro
	j	$L268
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

$L193:
	lw	$17,80($17)
	beq	$17,$0,$L192
$L268:
	lhu	$2,10($17)
	bne	$2,$20,$L193
	lhu	$2,8($17)
	bne	$2,$23,$L193
	lw	$3,12($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$fp,$L193
	lw	$11,32($sp)
	.set	macro
	.set	reorder

	lw	$8,16($17)
	bne	$8,$11,$L193
	lbu	$2,0($17)
	bne	$2,$5,$L193
	lw	$2,4($17)
	beq	$2,$10,$L289
	bne	$2,$9,$L193
	.set	noreorder
	.set	nomacro
	bne	$19,$7,$L193
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
$L326:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L323
	addiu	$5,$17,60
	.set	macro
	.set	reorder

	sw	$3,76($sp)
	sw	$8,56($sp)
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L166
	lw	$8,56($sp)
	.set	macro
	.set	reorder

	addiu	$5,$17,60
$L323:
	move	$4,$18
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_recheck_fp_policy_route_mark
	sw	$8,56($sp)
	.set	macro
	.set	reorder

	lw	$5,60($17)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	addiu	$6,$sp,24
	.set	macro
	.set	reorder

	lw	$5,28($17)
	lw	$6,52($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_route_input
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L290
	lw	$8,56($sp)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	move	$6,$0
	move	$4,$18
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	sw	$8,56($sp)
	.set	macro
	.set	reorder

	lw	$5,28($17)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_dst_check
	move	$6,$16
	.set	macro
	.set	reorder

	li	$4,-1			# 0xffffffffffffffff
	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L291
	lw	$8,56($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L200
	addiu	$7,$21,16
	.set	macro
	.set	reorder

	lhu	$7,6($21)
	addiu	$2,$21,6
	movz	$2,$0,$7
	move	$7,$2
$L200:
	lw	$5,36($sp)
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L292
	lw	$11,40($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$11,$0,$L293
	addiu	$4,$22,16
	.set	macro
	.set	reorder

$L206:
	move	$4,$18
	move	$5,$16
	move	$6,$19
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	sw	$7,72($sp)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L294
	lw	$7,72($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	beq	$19,$2,$L295
$L210:
	lbu	$2,1($17)
	sltu	$4,$2,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L211
	lui	$4,%hi($L217)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addiu	$4,$4,%lo($L217)
	addu	$2,$4,$2
	lw	$2,0($2)
	j	$2
	.section	.rodata.enter_fast_path,"a",@progbits
	.align	2
	.align	2
$L217:
	.word	$L212
	.word	$L213
	.word	$L214
	.word	$L214
	.word	$L215
	.word	$L211
	.word	$L211
	.word	$L211
	.word	$L216
	.word	$L211
	.word	$L211
	.word	$L211
	.word	$L216
	.section	.text.enter_fast_path
$L287:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L171
	li	$2,8192			# 0x2000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L171
	lui	$2,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

	lw	$4,%lo(statistic_udp_frag_rx)($2)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L174
	sw	$4,%lo(statistic_udp_frag_rx)($2)
	.set	macro
	.set	reorder

$L288:
	li	$5,1			# 0x1
	sw	$0,40($sp)
	.set	noreorder
	.set	nomacro
	j	$L175
	sw	$5,36($sp)
	.set	macro
	.set	reorder

$L177:
	lhu	$2,2($8)
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
	sh	$2,2($8)
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
	j	$L318
	lw	$2,12($16)
	.set	macro
	.set	reorder

$L176:
	sw	$0,40($sp)
	.set	noreorder
	.set	nomacro
	j	$L175
	sw	$0,36($sp)
	.set	macro
	.set	reorder

$L192:
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
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_inDev
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_return_check
	move	$6,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L319
	lw	$31,116($sp)
	.set	macro
	.set	reorder

$L173:
	li	$8,1			# 0x1
	move	$22,$0
	sw	$0,40($sp)
	sw	$0,36($sp)
	.set	noreorder
	.set	nomacro
	j	$L187
	sw	$8,52($sp)
	.set	macro
	.set	reorder

$L171:
	lw	$2,12($21)
	ext	$5,$2,16,1
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L296
	li	$5,393216			# 0x60000
	.set	macro
	.set	reorder

	and	$2,$2,$5
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L170
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L319
	lw	$31,116($sp)
	.set	macro
	.set	reorder

$L181:
	lhu	$2,2($6)
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
	sh	$2,2($6)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$2,36($sp)
$L322:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L188
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	move	$22,$0
	sw	$3,36($sp)
	.set	noreorder
	.set	nomacro
	j	$L187
	sw	$4,52($sp)
	.set	macro
	.set	reorder

$L296:
	lw	$6,32($sp)
	li	$2,6			# 0x6
	move	$4,$fp
	move	$5,$20
	move	$7,$23
	.set	noreorder
	.set	nomacro
	jal	mark_path_invalid
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L168
	move	$2,$0
	.set	macro
	.set	reorder

$L289:
	.set	noreorder
	.set	nomacro
	bne	$19,$6,$L193
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L326
	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
	.set	macro
	.set	reorder

$L188:
	lui	$2,%hi(statistic_udp_frag_ps)
	lw	$5,%lo(statistic_udp_frag_ps)($2)
	lw	$4,%lo(negative_cache_table)($3)
	lw	$18,%lo(max_udp_frag_entry)($7)
	addiu	$5,$5,1
	sw	$5,%lo(statistic_udp_frag_ps)($2)
	lbu	$17,9($16)
	lhu	$16,4($16)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$3,76($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi(negative_cache_list_free)
	lw	$2,%lo(negative_cache_list_free)($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L297
	lw	$3,76($sp)
	.set	macro
	.set	reorder

	li	$6,3			# 0x3
	sb	$6,0($2)
	sh	$16,2($2)
	sw	$fp,4($2)
	lw	$6,32($sp)
	lw	$5,48($2)
	sb	$17,1($2)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L190
	sw	$6,8($2)
	.set	macro
	.set	reorder

	lw	$6,52($2)
	addiu	$4,$4,%lo(negative_cache_list_free)
	sw	$6,52($5)
$L191:
	lw	$5,52($2)
	lw	$6,48($2)
	lw	$7,32($sp)
	addiu	$18,$18,-1
	sw	$6,0($5)
	lw	$5,8($4)
	xor	$fp,$7,$fp
	xor	$16,$16,$fp
	srl	$fp,$fp,16
	xor	$16,$16,$fp
	xor	$17,$16,$17
	addiu	$5,$5,-1
	srl	$16,$16,8
	sw	$5,8($4)
	xor	$16,$17,$16
	sw	$0,40($2)
	lw	$5,%lo(negative_cache_table)($3)
	and	$18,$16,$18
	sll	$4,$18,4
	sll	$18,$18,2
	subu	$4,$4,$18
	addu	$5,$5,$4
	lw	$5,8($5)
	addiu	$6,$2,40
	sw	$5,44($2)
	lw	$5,%lo(negative_cache_table)($3)
	addu	$5,$5,$4
	lw	$5,8($5)
	sw	$2,0($5)
	lw	$5,%lo(negative_cache_table)($3)
	addu	$5,$5,$4
	sw	$6,8($5)
	lw	$5,%lo(negative_cache_table)($3)
	addu	$4,$5,$4
	lw	$5,12($4)
	addiu	$5,$5,1
	sw	$5,12($4)
	lw	$3,%lo(negative_cache_table)($3)
	lhu	$4,2($3)
	addiu	$4,$4,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$4,2($3)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addiu	$4,$2,12
	addiu	$3,$3,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$3,20($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L168
	move	$2,$0
	.set	macro
	.set	reorder

$L290:
	lw	$5,24($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	move	$6,$0
	.set	macro
	.set	reorder

	lui	$3,%hi(table_path)
	lw	$2,%lo(table_path)($3)
$L280:
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
	j	$L168
	move	$2,$0
	.set	macro
	.set	reorder

$L291:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$4,%hi(table_path)
	.set	noreorder
	.set	nomacro
	j	$L280
	lw	$2,%lo(table_path)($4)
	.set	macro
	.set	reorder

$L216:
	lw	$5,32($sp)
	lw	$4,28($17)
	lw	$6,44($sp)
	andi	$3,$5,0xffff
	andi	$2,$4,0xffff
	addu	$3,$3,$6
	lhu	$5,10($16)
	subu	$2,$3,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	bltz	$2,$L298
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	sh	$2,10($16)
$L240:
	lw	$8,52($sp)
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L279
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L299
	lw	$2,28($17)
	lhu	$6,2($21)
	lhu	$5,32($17)
	andi	$4,$2,0xffff
	subu	$3,$3,$4
	lhu	$4,0($7)
	srl	$2,$2,16
$L282:
	subu	$3,$3,$2
	subu	$2,$6,$5
	addu	$2,$2,$3
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L300
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($7)
$L243:
	lhu	$2,32($17)
	sh	$2,2($21)
$L279:
	lw	$2,28($17)
	sw	$2,16($16)
$L212:
	lbu	$5,8($16)
	lbu	$4,9($16)
	lhu	$7,10($16)
	addiu	$3,$5,-1
	andi	$3,$3,0x00ff
	sll	$2,$5,8
	or	$2,$2,$4
	sll	$6,$3,8
	addu	$2,$2,$7
	or	$4,$6,$4
	subu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L301
	sb	$3,8($16)
	.set	macro
	.set	reorder

	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($16)
$L260:
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

	lw	$6,48($17)
	lw	$5,44($17)
	.set	noreorder
	.set	nomacro
	jal	fastpath_set_qos_mark
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_iph_dscp
	move	$5,$16
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_remark_vlanpri
	move	$4,$17
	.set	macro
	.set	reorder

	lhu	$5,74($17)
	li	$2,65535			# 0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L261
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lbu	$3,2($17)
	bne	$3,$2,$L261
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L261:
	lhu	$5,76($17)
	li	$2,65535			# 0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L262
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L324
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L262:
	lui	$2,%hi(jiffies)
$L324:
	lw	$2,%lo(jiffies)($2)
	move	$4,$16
	move	$5,$21
	sw	$2,36($17)
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	move	$6,$18
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L263
	lui	$5,%hi(table_path)
	.set	macro
	.set	reorder

	lui	$4,%hi(table_path)
	lw	$2,%lo(table_path)($4)
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
	j	$L168
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L301:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L260
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L298:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L240
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L215:
	lw	$8,32($sp)
	lw	$4,28($17)
	lw	$11,44($sp)
	andi	$3,$8,0xffff
	andi	$2,$4,0xffff
	addu	$3,$3,$11
	lhu	$5,10($16)
	subu	$2,$3,$2
	srl	$4,$4,16
	subu	$2,$2,$4
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	bltz	$2,$L302
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	sh	$2,10($16)
$L234:
	lw	$2,52($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L279
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L303
	lw	$4,28($17)
	lhu	$2,0($7)
	andi	$5,$4,0xffff
	subu	$3,$3,$5
	srl	$4,$4,16
	subu	$3,$3,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L276
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L325:
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L279
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L211:
	lw	$3,20($17)
	srl	$2,$fp,16
	andi	$fp,$fp,0xffff
	addu	$fp,$fp,$2
	lhu	$4,10($16)
	andi	$2,$3,0xffff
	subu	$2,$fp,$2
	srl	$3,$3,16
	subu	$2,$2,$3
	addu	$2,$2,$4
	bltz	$2,$L304
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	andi	$3,$2,0xffff
	sh	$3,10($16)
$L247:
	lw	$11,32($sp)
	lw	$5,28($17)
	lw	$2,44($sp)
	andi	$4,$11,0xffff
	addu	$4,$4,$2
	andi	$2,$5,0xffff
	subu	$2,$4,$2
	srl	$5,$5,16
	subu	$2,$2,$5
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L305
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($16)
$L249:
	lw	$3,52($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L250
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L306
	lw	$2,20($17)
	lhu	$6,0($21)
	lhu	$5,24($17)
	andi	$3,$2,0xffff
	subu	$fp,$fp,$3
	lhu	$3,0($7)
	srl	$2,$2,16
$L286:
	subu	$fp,$fp,$2
	subu	$2,$6,$5
	addu	$2,$2,$fp
	addu	$2,$2,$3
	bltz	$2,$L307
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	andi	$3,$2,0xffff
	sh	$3,0($7)
$L257:
	lw	$2,28($17)
	lhu	$6,2($21)
	lhu	$5,32($17)
	andi	$8,$2,0xffff
	subu	$4,$4,$8
	srl	$2,$2,16
	subu	$4,$4,$2
	subu	$2,$6,$5
	addu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L308
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($7)
$L252:
	lhu	$2,24($17)
	sh	$2,0($21)
	lhu	$2,32($17)
	sh	$2,2($21)
$L250:
	lw	$2,20($17)
	.set	noreorder
	.set	nomacro
	j	$L279
	sw	$2,12($16)
	.set	macro
	.set	reorder

$L305:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L249
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L304:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$3,$3,$2
	nor	$3,$0,$3
	andi	$3,$3,0xffff
	.set	noreorder
	.set	nomacro
	j	$L247
	sh	$3,10($16)
	.set	macro
	.set	reorder

$L308:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L252
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L307:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$3,$3,$2
	nor	$3,$0,$3
	andi	$3,$3,0xffff
	.set	noreorder
	.set	nomacro
	j	$L257
	sh	$3,0($7)
	.set	macro
	.set	reorder

$L306:
	beq	$7,$0,$L252
	lhu	$3,0($7)
	beq	$3,$0,$L252
	lw	$2,20($17)
	lhu	$6,0($21)
	lhu	$5,24($17)
	andi	$8,$2,0xffff
	subu	$fp,$fp,$8
	.set	noreorder
	.set	nomacro
	j	$L286
	srl	$2,$2,16
	.set	macro
	.set	reorder

$L263:
	lw	$2,%lo(table_path)($5)
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
	j	$L168
	sw	$4,%lo(statistic_fp)($3)
	.set	macro
	.set	reorder

$L303:
	beq	$7,$0,$L279
	lhu	$4,0($7)
	beq	$4,$0,$L279
	lw	$5,28($17)
	andi	$2,$5,0xffff
	subu	$2,$3,$2
	srl	$5,$5,16
	subu	$2,$2,$5
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bgez	$2,$L325
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L276:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L279
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L302:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L234
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L214:
	lw	$4,20($17)
	srl	$6,$3,16
	lhu	$5,10($16)
	srl	$2,$4,16
	subu	$2,$0,$2
	andi	$4,$4,0xffff
	subu	$2,$2,$4
	addu	$2,$2,$6
	andi	$3,$3,0xffff
	addu	$2,$2,$3
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	bltz	$2,$L309
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($16)
$L226:
	lw	$6,52($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L227
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L310
	lw	$3,20($17)
	lw	$2,12($16)
	lhu	$6,0($21)
	srl	$4,$3,16
	subu	$4,$0,$4
	andi	$3,$3,0xffff
	lhu	$5,24($17)
	subu	$3,$4,$3
	srl	$4,$2,16
	addu	$4,$3,$4
	andi	$2,$2,0xffff
	lhu	$3,0($7)
	addu	$4,$4,$2
	subu	$2,$6,$5
	addu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L311
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($7)
$L232:
	lw	$2,12($21)
	ext	$2,$2,20,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L229
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L229
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

$L229:
	lhu	$2,24($17)
	sh	$2,0($21)
$L227:
	lw	$2,20($17)
$L321:
	.set	noreorder
	.set	nomacro
	j	$L212
	sw	$2,12($16)
	.set	macro
	.set	reorder

$L311:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L232
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L310:
	beq	$7,$0,$L229
	lhu	$3,0($7)
	beq	$3,$0,$L229
	lw	$4,20($17)
	lw	$2,12($16)
	lhu	$6,0($21)
	srl	$8,$4,16
	subu	$8,$0,$8
	andi	$4,$4,0xffff
	lhu	$5,24($17)
	subu	$4,$8,$4
	srl	$8,$2,16
	addu	$4,$4,$8
	andi	$2,$2,0xffff
	addu	$4,$4,$2
	subu	$2,$6,$5
	addu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L312
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L229
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L309:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L226
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L213:
	lw	$3,20($17)
	srl	$5,$fp,16
	lhu	$4,10($16)
	srl	$2,$3,16
	subu	$2,$0,$2
	andi	$3,$3,0xffff
	subu	$2,$2,$3
	addu	$2,$2,$5
	andi	$fp,$fp,0xffff
	addu	$2,$2,$fp
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L313
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,10($16)
$L219:
	lw	$5,52($sp)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L227
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L314
	lw	$4,20($17)
	lw	$3,12($16)
	lhu	$2,0($7)
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
	srl	$5,$3,16
	addu	$4,$4,$5
	andi	$3,$3,0xffff
	addu	$3,$4,$3
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L315
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	sh	$2,0($7)
$L224:
	lw	$2,12($21)
	ext	$2,$2,20,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L227
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L227
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L321
	lw	$2,20($17)
	.set	macro
	.set	reorder

$L312:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L229
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L315:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L224
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L314:
	beq	$7,$0,$L227
	lhu	$3,0($7)
	beq	$3,$0,$L227
	lw	$2,20($17)
	lw	$4,12($16)
	srl	$5,$2,16
	subu	$5,$0,$5
	andi	$2,$2,0xffff
	subu	$2,$5,$2
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L316
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L227
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L313:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L219
	sh	$2,10($16)
	.set	macro
	.set	reorder

$L316:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L227
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L190:
	lw	$5,52($2)
	addiu	$4,$4,%lo(negative_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L191
	sw	$5,4($4)
	.set	macro
	.set	reorder

$L297:
	lui	$2,%hi(negative_cache_table)
	.set	noreorder
	.set	nomacro
	j	$L280
	lw	$2,%lo(negative_cache_table)($2)
	.set	macro
	.set	reorder

$L300:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$L243
	sh	$2,0($7)
	.set	macro
	.set	reorder

$L299:
	beq	$7,$0,$L243
	lhu	$4,0($7)
	beq	$4,$0,$L243
	lw	$2,28($17)
	lhu	$6,2($21)
	lhu	$5,32($17)
	andi	$8,$2,0xffff
	subu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L282
	srl	$2,$2,16
	.set	macro
	.set	reorder

$L295:
	lw	$2,12($21)
	ext	$2,$2,20,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L210
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$4,2($16)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L210
	move	$4,$18
	.set	macro
	.set	reorder

	li	$5,40			# 0x28
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sw	$7,72($sp)
	.set	macro
	.set	reorder

	lw	$7,72($sp)
	.set	noreorder
	.set	nomacro
	j	$L210
	lw	$3,76($sp)
	.set	macro
	.set	reorder

$L294:
	lui	$4,%hi(table_path)
	lw	$3,%lo(table_path)($4)
	lhu	$4,2($3)
	addiu	$4,$4,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$4,2($3)
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
	j	$L319
	lw	$31,116($sp)
	.set	macro
	.set	reorder

$L293:
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	del_timer
	sw	$7,72($sp)
	.set	macro
	.set	reorder

	lw	$4,8($22)
	lw	$2,4($22)
	lhu	$5,2($22)
	lbu	$20,1($22)
	xor	$2,$4,$2
	lui	$4,%hi(max_udp_frag_entry)
	lw	$6,%lo(max_udp_frag_entry)($4)
	xor	$5,$5,$2
	srl	$4,$2,16
	lui	$2,%hi(udp_cache_table)
	xor	$5,$5,$4
	lw	$4,%lo(udp_cache_table)($2)
	xor	$20,$5,$20
	srl	$5,$5,8
	xor	$20,$20,$5
	addiu	$6,$6,-1
	sw	$2,68($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	and	$20,$20,$6
	.set	macro
	.set	reorder

	lw	$4,44($22)
	sb	$0,0($22)
	lw	$2,68($sp)
	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L207
	lw	$7,72($sp)
	.set	macro
	.set	reorder

	lw	$6,48($22)
	sll	$5,$20,4
	sll	$20,$20,2
	sw	$6,48($4)
	subu	$20,$5,$20
$L208:
	lw	$5,48($22)
	lw	$6,44($22)
	lui	$4,%hi(udp_cache_list_free)
	addiu	$4,$4,%lo(udp_cache_list_free)
	sw	$6,0($5)
	lw	$5,%lo(udp_cache_table)($2)
	addiu	$6,$22,52
	lw	$5,4($5)
	addu	$20,$5,$20
	lw	$5,8($20)
	addiu	$5,$5,-1
	sw	$5,8($20)
	sw	$0,52($22)
	lw	$5,4($4)
	sw	$5,56($22)
	lw	$5,4($4)
	sw	$22,0($5)
	lw	$5,8($4)
	lw	$2,%lo(udp_cache_table)($2)
	sw	$6,4($4)
	addiu	$5,$5,1
	sw	$5,8($4)
	lhu	$4,2($2)
	addiu	$4,$4,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$4,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lui	$2,%hi(statistic_udp_frag_free)
	lw	$4,%lo(statistic_udp_frag_free)($2)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L206
	sw	$4,%lo(statistic_udp_frag_free)($2)
	.set	macro
	.set	reorder

$L292:
	lui	$2,%hi(udp_cache_table)
	lui	$5,%hi(max_udp_frag_entry)
	lw	$4,%lo(udp_cache_table)($2)
	lw	$5,%lo(max_udp_frag_entry)($5)
	lhu	$22,4($16)
	sw	$2,68($sp)
	sw	$3,76($sp)
	sw	$5,64($sp)
	sw	$7,72($sp)
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$8,56($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi(udp_cache_list_free)
	lw	$6,%lo(udp_cache_list_free)($4)
	lw	$2,68($sp)
	lw	$3,76($sp)
	lw	$5,64($sp)
	lw	$7,72($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L317
	lw	$8,56($sp)
	.set	macro
	.set	reorder

	lw	$9,52($6)
	sw	$8,8($6)
	li	$8,1			# 0x1
	sh	$20,12($6)
	sh	$23,14($6)
	sb	$8,0($6)
	sh	$22,2($6)
	sw	$fp,4($6)
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L204
	sb	$19,1($6)
	.set	macro
	.set	reorder

	lw	$8,56($6)
	addiu	$4,$4,%lo(udp_cache_list_free)
	sw	$8,56($9)
$L205:
	lw	$9,56($6)
	lw	$10,52($6)
	lw	$8,48($sp)
	addiu	$5,$5,-1
	sw	$10,0($9)
	lw	$9,8($4)
	xor	$22,$22,$8
	srl	$8,$8,16
	addiu	$9,$9,-1
	sw	$9,8($4)
	sw	$0,44($6)
	lw	$4,%lo(udp_cache_table)($2)
	xor	$22,$22,$8
	srl	$8,$22,8
	xor	$22,$22,$19
	xor	$22,$22,$8
	and	$5,$22,$5
	lw	$8,4($4)
	sll	$4,$5,4
	sll	$5,$5,2
	subu	$4,$4,$5
	addu	$5,$8,$4
	lw	$5,4($5)
	addiu	$8,$6,44
	sw	$5,48($6)
	lw	$5,%lo(udp_cache_table)($2)
	lw	$5,4($5)
	addu	$5,$5,$4
	lw	$5,4($5)
	sw	$6,0($5)
	lw	$5,%lo(udp_cache_table)($2)
	lw	$5,4($5)
	addu	$5,$5,$4
	sw	$8,4($5)
	lw	$5,%lo(udp_cache_table)($2)
	lw	$5,4($5)
	addu	$4,$5,$4
	lw	$5,8($4)
	addiu	$5,$5,1
	sw	$5,8($4)
	lw	$2,%lo(udp_cache_table)($2)
	lhu	$4,2($2)
	addiu	$4,$4,1
 #APP
 # 137 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	sh	$4,2($2)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$4,$6,16
	addiu	$2,$2,1000
	sw	$2,24($6)
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$7,72($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(statistic_udp_frag_add)
	lw	$4,%lo(statistic_udp_frag_add)($2)
	lw	$3,76($sp)
	lw	$7,72($sp)
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	j	$L206
	sw	$4,%lo(statistic_udp_frag_add)($2)
	.set	macro
	.set	reorder

$L207:
	lw	$4,%lo(udp_cache_table)($2)
	sll	$6,$20,4
	lw	$5,48($22)
	lw	$4,4($4)
	sll	$20,$20,2
	subu	$20,$6,$20
	addu	$4,$4,$20
	.set	noreorder
	.set	nomacro
	j	$L208
	sw	$5,4($4)
	.set	macro
	.set	reorder

$L204:
	lw	$8,56($6)
	addiu	$4,$4,%lo(udp_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L205
	sw	$8,4($4)
	.set	macro
	.set	reorder

$L317:
	lw	$2,%lo(udp_cache_table)($2)
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
	lui	$2,%hi(statistic_udp_frag_tbl_full)
	lw	$3,%lo(statistic_udp_frag_tbl_full)($2)
	move	$4,$18
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	sw	$3,%lo(statistic_udp_frag_tbl_full)($2)
	.set	macro
	.set	reorder

	lui	$6,%hi(table_path)
	.set	noreorder
	.set	nomacro
	j	$L280
	lw	$2,%lo(table_path)($6)
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.size	enter_fast_path, .-enter_fast_path
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
	beq	$2,$3,$L330
	li	$3,450			# 0x1c2

	beq	$2,$3,$L331
	li	$3,150			# 0x96

	beq	$2,$3,$L329
	lui	$18,%hi(jiffies)

$L328:
	lw	$18,%lo(jiffies)($18)
	jal	rtl_get_fdb_aging
	move	$4,$16

	move	$16,$2
	jal	rtl_hold_time
	move	$4,$17

	subu	$16,$18,$16
	subu	$2,$16,$2
	slt	$2,$2,0
	li	$3,-1			# 0xffffffffffffffff
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	movn	$3,$0,$2
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

$L329:
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-30000

	j	$L328
	nop

$L331:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	jal	rtl_set_fdb_aging
	move	$4,$16

	j	$L328
	nop

$L330:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-15000

	j	$L328
	nop

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.size	rtl_br_fdb_time_update, .-rtl_br_fdb_time_update
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
	lw	$17,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	move	$16,$4

	li	$3,17			# 0x11
	beq	$2,$3,$L348
	nop

	move	$4,$16
	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L349
	nop

$L339:
	li	$2,-1			# 0xffffffffffffffff
$L342:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

$L348:
	jal	rtl_get_ct_udp_status
	move	$4,$16

	andi	$2,$2,0x2
	beq	$2,$0,$L350
	li	$4,1			# 0x1

$L344:
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	move	$19,$2
	li	$20,1			# 0x1
$L338:
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	beq	$18,$2,$L351
	move	$5,$0

	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	bne	$18,$2,$L339
	move	$4,$16

	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$20,20($sp)

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
	j	$L347
	li	$5,1			# 0x1

$L350:
	j	$L344
	move	$4,$0

$L351:
	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	sw	$20,20($sp)

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
$L347:
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,40($sp)

	addiu	$4,$sp,20
	move	$5,$19
	addiu	$6,$sp,16
	jal	rtk_idleNaptConnection
	sw	$2,44($sp)

	beq	$2,$0,$L339
	move	$4,$16

	lw	$5,16($sp)
	jal	rtl_set_ct_timeout_expires
	addu	$5,$19,$5

	addu	$5,$19,$17
	jal	rtl_check_for_acc
	move	$4,$16

	j	$L342
	move	$2,$0

$L349:
	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L342
	li	$2,-1			# 0xffffffffffffffff

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	move	$19,$2
	j	$L338
	li	$20,2			# 0x2

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.size	rtl_fpTimer_update, .-rtl_fpTimer_update
	.section	.text.rtk_delNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_delNaptConnection
	.set	nomips16
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$3,0($4)
	li	$2,6			# 0x6
	li	$19,17			# 0x11
	xori	$3,$3,0x2
	move	$16,$4
	movz	$19,$2,$3
 #APP
 # 39 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	reorder						
	.set	noat						
	di	$18					
	andi	$18, 1					
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	lw	$4,20($4)
	.set	noreorder
	.set	nomacro
	jal	filter_delconnect
	lui	$17,%hi(table_napt)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lw	$4,%lo(table_napt)($17)
	.set	macro
	.set	reorder

	lw	$5,4($16)
	lw	$7,8($16)
	lw	$8,20($16)
	lw	$10,24($16)
	xor	$3,$7,$5
	xor	$3,$3,$8
	srl	$2,$5,16
	lw	$4,0($16)
	xor	$3,$3,$10
	xor	$3,$3,$2
	srl	$2,$8,16
	xor	$3,$3,$2
	lui	$2,%hi(napt_table_list_max)
	lw	$9,%lo(napt_table_list_max)($2)
	andi	$2,$4,0xffff
	xor	$3,$3,$2
	lw	$6,%lo(table_napt)($17)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$9,$9,-1
	and	$2,$3,$9
	lw	$3,4($6)
	sll	$9,$2,4
	sll	$2,$2,2
	subu	$9,$9,$2
	addu	$2,$3,$9
	lw	$2,0($2)
	bne	$2,$0,$L382
	.set	noreorder
	.set	nomacro
	j	$L386
	lhu	$2,2($6)
	.set	macro
	.set	reorder

$L356:
	lw	$2,36($2)
	beq	$2,$0,$L355
$L382:
	lw	$3,4($2)
	bne	$4,$3,$L356
	lw	$3,8($2)
	bne	$5,$3,$L356
	lw	$3,12($2)
	bne	$7,$3,$L356
	lw	$11,16($2)
	lw	$3,12($16)
	bne	$11,$3,$L356
	lw	$11,20($2)
	lw	$3,16($16)
	bne	$11,$3,$L356
	lw	$3,24($2)
	bne	$8,$3,$L356
	lw	$3,28($2)
	bne	$10,$3,$L356
	lw	$3,36($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L357
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$4,40($2)
	sw	$4,40($3)
$L358:
	lw	$3,40($2)
	lw	$4,36($2)
	sw	$4,0($3)
	lw	$3,%lo(table_napt)($17)
	lw	$3,4($3)
	addu	$9,$3,$9
	lw	$3,8($9)
	addiu	$3,$3,-1
	sw	$3,8($9)
	lw	$3,44($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L359
	lui	$4,%hi(napt_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$5,48($3)
$L360:
	lw	$5,48($2)
	lw	$6,44($2)
	lui	$3,%hi(napt_list_free)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$6,0($5)
	lw	$6,8($4)
	addiu	$5,$2,44
	lui	$20,%hi(table_path)
	addiu	$6,$6,-1
	sw	$6,8($4)
	sw	$0,44($2)
	lw	$4,4($3)
	sw	$4,48($2)
	lw	$4,4($3)
	sw	$2,0($4)
	lw	$2,8($3)
	lw	$4,%lo(table_path)($20)
	sw	$5,4($3)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	sw	$2,8($3)
	.set	macro
	.set	reorder

	lw	$11,4($16)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$13,8($16)
	xor	$4,$3,$11
	srl	$2,$11,8
	xor	$4,$4,$5
	xor	$4,$4,$2
	srl	$2,$3,16
	xor	$4,$4,$2
	lui	$12,%hi(path_table_list_max)
	srl	$2,$13,4
	xor	$2,$4,$2
	lw	$8,%lo(path_table_list_max)($12)
	xor	$2,$2,$19
	lw	$4,%lo(table_path)($20)
	srl	$6,$2,12
	xor	$2,$6,$2
	addiu	$8,$8,-1
	and	$2,$2,$8
	lw	$9,4($4)
	sll	$10,$2,4
	sll	$2,$2,2
	subu	$10,$10,$2
	addu	$2,$9,$10
	lw	$2,0($2)
	lw	$6,12($16)
	lw	$7,16($16)
	li	$14,1			# 0x1
$L361:
	beq	$2,$0,$L367
	lw	$24,4($2)
	lw	$15,0($16)
	beq	$24,$15,$L384
$L362:
	.set	noreorder
	.set	nomacro
	j	$L361
	lw	$2,80($2)
	.set	macro
	.set	reorder

$L355:
	lhu	$2,2($6)
$L386:
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
	sh	$2,2($6)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$18, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	li	$2,-4			# 0xfffffffffffffffc
$L378:
	lw	$31,36($sp)
$L387:
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

$L384:
	lbu	$15,2($2)
	bne	$15,$14,$L362
	lw	$15,12($2)
	bne	$11,$15,$L362
	lhu	$15,10($2)
	bne	$13,$15,$L362
	lw	$15,20($2)
	bne	$15,$6,$L362
	lhu	$15,24($2)
	bne	$15,$7,$L362
	lw	$15,16($2)
	bne	$3,$15,$L362
	lhu	$15,8($2)
	bne	$5,$15,$L362
	lw	$3,80($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L363
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$4,84($2)
	sw	$4,84($3)
$L364:
	lw	$3,84($2)
	lw	$4,80($2)
	sw	$4,0($3)
	lw	$3,%lo(table_path)($20)
	lw	$3,4($3)
	addu	$10,$3,$10
	lw	$3,8($10)
	addiu	$3,$3,-1
	sw	$3,8($10)
	lw	$4,88($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L365
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$5,92($4)
$L366:
	lw	$4,92($2)
	lw	$6,88($2)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$6,0($4)
	lw	$4,8($3)
	addiu	$6,$2,88
	addiu	$4,$4,-1
	sw	$4,8($3)
	sw	$0,88($2)
	lw	$3,4($5)
	sw	$3,92($2)
	lw	$3,4($5)
	sw	$2,0($3)
	lw	$2,8($5)
	lw	$4,%lo(table_path)($20)
	sw	$6,4($5)
	addiu	$2,$2,1
	sw	$2,8($5)
	lw	$8,%lo(path_table_list_max)($12)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$9,4($4)
	lw	$6,12($16)
	lw	$7,16($16)
	addiu	$8,$8,-1
$L367:
	xor	$10,$3,$6
	xor	$10,$10,$19
	srl	$2,$3,8
	xor	$10,$10,$7
	xor	$10,$10,$2
	srl	$2,$6,16
	xor	$10,$10,$2
	srl	$2,$5,4
	xor	$2,$10,$2
	srl	$10,$2,12
	xor	$2,$10,$2
	and	$8,$2,$8
	sll	$2,$8,4
	sll	$8,$8,2
	subu	$8,$2,$8
	addu	$9,$9,$8
	lw	$2,0($9)
	li	$9,2			# 0x2
$L369:
	beq	$2,$0,$L375
	lw	$11,4($2)
	lw	$10,0($16)
	beq	$11,$10,$L385
$L370:
	.set	noreorder
	.set	nomacro
	j	$L369
	lw	$2,80($2)
	.set	macro
	.set	reorder

$L385:
	lbu	$10,2($2)
	bne	$10,$9,$L370
	lw	$10,16($2)
	bne	$10,$6,$L370
	lhu	$10,8($2)
	bne	$10,$7,$L370
	lw	$10,20($2)
	bne	$10,$3,$L370
	lhu	$10,24($2)
	bne	$10,$5,$L370
	lw	$11,28($2)
	lw	$10,4($16)
	bne	$11,$10,$L370
	lhu	$11,32($2)
	lw	$10,8($16)
	bne	$11,$10,$L370
	lw	$3,80($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L371
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$4,84($2)
	sw	$4,84($3)
$L372:
	lw	$3,84($2)
	lw	$4,80($2)
	sw	$4,0($3)
	lw	$3,%lo(table_path)($20)
	lw	$3,4($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,-1
	sw	$3,8($8)
	lw	$4,88($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L373
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$5,92($4)
$L374:
	lw	$4,92($2)
	lw	$6,88($2)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$6,0($4)
	lw	$6,8($3)
	addiu	$4,$2,88
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,88($2)
	lw	$3,4($5)
	sw	$3,92($2)
	lw	$3,4($5)
	sw	$2,0($3)
	lw	$2,8($5)
	sw	$4,4($5)
	lw	$4,%lo(table_path)($20)
	addiu	$2,$2,1
	sw	$2,8($5)
$L375:
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
	lw	$2,%lo(table_napt)($17)
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
	andi	$2,$18,0x1
	bne	$2,$0,$L377
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$18, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L387
	lw	$31,36($sp)
	.set	macro
	.set	reorder

$L373:
	lw	$4,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L374
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L371:
	lw	$3,%lo(table_path)($20)
	lw	$4,84($2)
	lw	$3,4($3)
	addu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L372
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L377:
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$18, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L378
	move	$2,$0
	.set	macro
	.set	reorder

$L365:
	lw	$4,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L366
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L363:
	lw	$3,%lo(table_path)($20)
	lw	$4,84($2)
	lw	$3,4($3)
	addu	$3,$3,$10
	.set	noreorder
	.set	nomacro
	j	$L364
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L359:
	lw	$3,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L360
	sw	$3,4($4)
	.set	macro
	.set	reorder

$L357:
	lw	$3,%lo(table_napt)($17)
	lw	$4,40($2)
	lw	$3,4($3)
	addu	$3,$3,$9
	.set	noreorder
	.set	nomacro
	j	$L358
	sw	$4,4($3)
	.set	macro
	.set	reorder

	.end	rtk_delNaptConnection
	.size	rtk_delNaptConnection, .-rtk_delNaptConnection
	.section	.text.rtk_addNaptConnection,"ax",@progbits
	.align	2
	.globl	rtk_addNaptConnection
	.set	nomips16
	.ent	rtk_addNaptConnection
	.type	rtk_addNaptConnection, @function
rtk_addNaptConnection:
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
	move	$17,$5
	move	$16,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L460
	move	$19,$6
	.set	macro
	.set	reorder

$L389:
	li	$2,-1			# 0xffffffffffffffff
$L395:
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
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L460:
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	sw	$7,132($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L389
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lw	$3,0($16)
	lw	$4,20($16)
	xori	$3,$3,0x2
	li	$5,17			# 0x11
	movz	$5,$2,$3
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	sw	$5,56($sp)
	.set	macro
	.set	reorder

 #APP
 # 39 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	reorder						
	.set	noat						
	di	$6					
	andi	$6, 1					
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	sw	$6,80($sp)
	lw	$4,4($16)
	lw	$2,8($16)
	lw	$3,20($16)
	lw	$5,24($16)
	xor	$2,$4,$2
	xor	$2,$2,$3
	xor	$2,$2,$5
	srl	$4,$4,16
	lhu	$5,2($16)
	xor	$2,$2,$4
	srl	$3,$3,16
	xor	$2,$2,$3
	lui	$3,%hi(napt_table_list_max)
	lw	$3,%lo(napt_table_list_max)($3)
	lui	$fp,%hi(table_napt)
	xor	$2,$2,$5
	lw	$4,%lo(table_napt)($fp)
	srl	$18,$2,12
	xor	$2,$18,$2
	addiu	$3,$3,-1
	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	and	$18,$2,$3
	.set	macro
	.set	reorder

	lw	$6,%lo(table_napt)($fp)
	sll	$3,$18,4
	sll	$18,$18,2
	lw	$2,4($6)
	subu	$18,$3,$18
	addu	$2,$2,$18
	lw	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L392
	lw	$7,132($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L396
	lw	$4,0($16)
	.set	macro
	.set	reorder

$L393:
	lw	$2,36($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L483
	lui	$3,%hi(napt_list_free)
	.set	macro
	.set	reorder

$L396:
	lw	$3,4($2)
	bne	$3,$4,$L393
	lw	$5,8($2)
	lw	$3,4($16)
	bne	$5,$3,$L393
	lw	$5,12($2)
	lw	$3,8($16)
	bne	$5,$3,$L393
	lw	$5,16($2)
	lw	$3,12($16)
	bne	$5,$3,$L393
	lw	$5,20($2)
	lw	$3,16($16)
	bne	$5,$3,$L393
	lw	$5,24($2)
	lw	$3,20($16)
	bne	$5,$3,$L393
	lw	$5,28($2)
	lw	$3,24($16)
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L393
	lw	$5,56($sp)
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	lw	$2,%lo(table_napt)($fp)
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
	lw	$3,80($sp)
	andi	$2,$3,0x1
	bne	$2,$0,$L394
	move	$2,$3
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L395
	move	$2,$0
	.set	macro
	.set	reorder

$L392:
	lui	$3,%hi(napt_list_free)
$L483:
	lw	$2,%lo(napt_list_free)($3)
	beq	$2,$0,$L397
	lw	$5,0($16)
	lw	$4,44($2)
	li	$6,-1			# 0xffffffffffffffff
	sw	$5,4($2)
	lw	$5,4($16)
	sw	$5,8($2)
	lw	$5,8($16)
	sw	$5,12($2)
	lw	$5,12($16)
	sw	$5,16($2)
	lw	$5,16($16)
	sw	$5,20($2)
	lw	$5,20($16)
	sw	$5,24($2)
	lw	$5,24($16)
	sw	$7,32($2)
	sb	$6,0($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L398
	sw	$5,28($2)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$5,48($4)
$L399:
	lw	$5,48($2)
	lw	$6,44($2)
	lui	$4,%hi(napt_list_inuse)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$6,0($5)
	lw	$7,8($3)
	addiu	$6,$2,44
	addiu	$5,$2,36
	addiu	$7,$7,-1
	sw	$7,8($3)
	sw	$0,44($2)
	lw	$3,4($4)
	lui	$22,%hi(table_path)
	sw	$3,48($2)
	lw	$3,4($4)
	sw	$2,0($3)
	lw	$3,8($4)
	sw	$6,4($4)
	addiu	$3,$3,1
	sw	$3,8($4)
	sw	$0,36($2)
	lw	$3,%lo(table_napt)($fp)
	lw	$3,4($3)
	addu	$3,$3,$18
	lw	$3,4($3)
	sw	$3,40($2)
	lw	$3,%lo(table_napt)($fp)
	lw	$3,4($3)
	addu	$3,$3,$18
	lw	$3,4($3)
	sw	$2,0($3)
	lw	$2,%lo(table_napt)($fp)
	lw	$2,4($2)
	addu	$2,$2,$18
	sw	$5,4($2)
	lw	$2,%lo(table_napt)($fp)
	lw	$2,4($2)
	addu	$18,$2,$18
	lw	$2,8($18)
	addiu	$2,$2,1
	sw	$2,8($18)
	li	$2,33			# 0x21
	sb	$2,17($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl865x_getLanDev
	sb	$0,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl865x_getWanDev
	sw	$2,64($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,128($sp)
	.set	macro
	.set	reorder

	lw	$21,0($2)
	move	$4,$17
	move	$20,$2
	ext	$21,$21,24,4
	sll	$21,$21,2
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	addu	$21,$2,$21
	.set	macro
	.set	reorder

	addiu	$5,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$2
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,28
	.set	macro
	.set	reorder

	lbu	$4,1($20)
	sw	$4,96($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,96($sp)
	sll	$2,$2,2
	move	$4,$17
	andi	$3,$5,0x3
	or	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_remark_vlanpri
	sb	$2,1($20)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ori_vlanpri
	sw	$2,104($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	sw	$2,100($sp)
	.set	macro
	.set	reorder

	sw	$2,108($sp)
	lw	$6,12($20)
	move	$4,$17
	sw	$6,124($sp)
	lhu	$2,0($21)
	sw	$2,116($sp)
	lw	$3,16($20)
	sw	$3,120($sp)
	lhu	$5,2($21)
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_dst
	sw	$5,112($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	_raw_spin_lock
	lw	$4,%lo(table_path)($22)
	.set	macro
	.set	reorder

	lui	$6,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($6)
	lw	$4,4($16)
	lui	$3,%hi(path_list_free)
	addiu	$2,$2,-1
	sw	$4,72($sp)
	sw	$2,92($sp)
	lw	$5,8($16)
	lw	$18,%lo(path_list_free)($3)
	sw	$5,88($sp)
	lw	$6,20($16)
	sw	$6,68($sp)
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L428
	sw	$2,84($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	lui	$4,%hi(route_list_inuse)
	sw	$2,4($18)
	lw	$8,4($16)
	sw	$8,12($18)
	lw	$2,8($16)
	sh	$2,10($18)
	lw	$2,20($16)
	sw	$2,16($18)
	lw	$2,24($16)
	sh	$2,8($18)
	lw	$9,12($16)
	sw	$9,20($18)
	lw	$2,16($16)
	sh	$2,24($18)
	lw	$7,20($16)
	sw	$7,28($18)
	lw	$2,24($16)
	sh	$2,32($18)
	lw	$3,%lo(route_list_inuse)($4)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L404
	move	$10,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L405:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L402
	and	$6,$2,$7
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$6,$5,$L461
$L402:
	move	$2,$4
$L403:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L405
	move	$4,$2
	.set	macro
	.set	reorder

$L404:
	li	$2,1			# 0x1
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$10,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$5,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($5)
	beq	$2,$0,$L406
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$8,12($18)
	lw	$9,20($18)
$L406:
	beq	$9,$8,$L407
	lbu	$2,1($18)
	ori	$2,$2,0x1
	sb	$2,1($18)
$L407:
	lhu	$3,10($18)
	lhu	$2,24($18)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L484
	li	$2,255			# 0xff
	.set	macro
	.set	reorder

	lbu	$2,1($18)
	ori	$2,$2,0x2
	sb	$2,1($18)
	li	$2,255			# 0xff
$L484:
	sh	$2,78($18)
	sw	$0,44($18)
	sw	$0,48($18)
	sw	$0,60($18)
	sb	$0,72($18)
	sb	$0,73($18)
	lw	$2,4($16)
	li	$6,14			# 0xe
	move	$5,$0
	sw	$2,12($20)
	lw	$2,8($16)
	addiu	$4,$sp,36
	sh	$2,0($21)
	lw	$2,20($16)
	sw	$2,16($20)
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$6,64($sp)
	beq	$6,$0,$L409
	lw	$4,4($16)
	move	$5,$6
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L409
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$23,$2
	li	$2,1			# 0x1
	beq	$23,$2,$L462
$L410:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	lw	$5,64($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_src_phy_port
	move	$6,$0
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L409
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L411
	lw	$6,64($sp)
	.set	macro
	.set	reorder

	lw	$7,60($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$5,$0
	.set	macro
	.set	reorder

$L411:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L463
	lw	$2,44($18)
	sw	$2,44($18)
$L482:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,60($18)
	lui	$2,%hi(nf_hooks+160)
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L414
	lw	$6,64($sp)
	.set	macro
	.set	reorder

	lw	$7,60($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L414:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L464
	lw	$2,48($18)
	sw	$2,48($18)
$L409:
	lw	$2,12($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,16($16)
	li	$6,14			# 0xe
	sh	$2,0($21)
	lw	$2,20($16)
	sw	$2,16($20)
	lw	$2,24($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L417
	lw	$5,60($sp)
	.set	macro
	.set	reorder

	lw	$4,20($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L485
	move	$4,$18
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$23,$2
	li	$2,1			# 0x1
	beq	$23,$2,$L465
$L418:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,128($sp)
	li	$2,2048			# 0x800
	bne	$5,$2,$L417
	lw	$5,44($18)
	bne	$5,$0,$L466
	lui	$2,%hi(nf_hooks+160)
$L480:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L420
	lw	$6,64($sp)
	.set	macro
	.set	reorder

	lw	$7,60($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L420:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L467
	lw	$2,48($18)
	sw	$2,48($18)
$L417:
	move	$4,$18
$L485:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_dscp_out
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_vlanpri
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_vlanid_out
	move	$5,$17
	.set	macro
	.set	reorder

	lw	$6,56($sp)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$6,$2,$L468
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$2,56($18)
$L424:
	lui	$2,%hi(jiffies)
	sw	$19,52($18)
	lw	$3,%lo(jiffies)($2)
	lw	$2,88($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L425
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$4,92($18)
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$3,76($sp)
	sw	$4,92($2)
$L426:
	lw	$2,92($18)
	lw	$3,88($18)
	lui	$23,%hi(path_list_inuse)
	addiu	$23,$23,%lo(path_list_inuse)
	sw	$3,0($2)
	lw	$4,76($sp)
	lw	$6,72($sp)
	lw	$5,68($sp)
	lw	$2,8($4)
	lw	$4,84($sp)
	xor	$3,$5,$6
	lw	$5,76($sp)
	addiu	$2,$2,-1
	xor	$3,$3,$4
	sw	$2,8($5)
	sw	$0,88($18)
	lw	$2,4($23)
	srl	$4,$6,8
	xor	$3,$3,$4
	sw	$2,92($18)
	lw	$5,4($23)
	lw	$6,68($sp)
	sw	$18,0($5)
	lw	$4,8($23)
	lw	$5,88($sp)
	srl	$2,$6,16
	xor	$3,$3,$2
	addiu	$4,$4,1
	srl	$2,$5,4
	addiu	$5,$18,88
	sw	$5,4($23)
	sw	$4,8($23)
	sw	$0,80($18)
	lw	$6,56($sp)
	xor	$3,$3,$2
	lw	$4,%lo(table_path)($22)
	xor	$3,$3,$6
	srl	$2,$3,12
	xor	$3,$2,$3
	lw	$2,92($sp)
	lw	$4,4($4)
	and	$3,$3,$2
	sll	$5,$3,4
	sll	$2,$3,2
	subu	$2,$5,$2
	addu	$4,$4,$2
	lw	$4,4($4)
	addiu	$3,$18,80
	sw	$4,84($18)
	lw	$4,%lo(table_path)($22)
	lw	$4,4($4)
	addu	$4,$4,$2
	lw	$4,4($4)
	sw	$18,0($4)
	lw	$4,%lo(table_path)($22)
	lw	$4,4($4)
	addu	$4,$4,$2
	sw	$3,4($4)
	lw	$3,%lo(table_path)($22)
	lui	$4,%hi(path_list_free)
	lw	$3,4($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
	lui	$3,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($3)
	lw	$5,20($16)
	lw	$18,%lo(path_list_free)($4)
	addiu	$2,$2,-1
	sw	$5,72($sp)
	sw	$2,88($sp)
	lw	$6,24($16)
	sw	$6,92($sp)
	lw	$2,12($16)
	sw	$2,68($sp)
	lw	$3,16($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L428
	sw	$3,84($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	lui	$4,%hi(route_list_inuse)
	sw	$2,4($18)
	lw	$2,20($16)
	sw	$2,12($18)
	lw	$2,24($16)
	sh	$2,10($18)
	lw	$7,12($16)
	sw	$7,16($18)
	lw	$2,16($16)
	sh	$2,8($18)
	lw	$2,20($16)
	sw	$2,20($18)
	lw	$2,24($16)
	sh	$2,24($18)
	lw	$6,4($16)
	sw	$6,28($18)
	lw	$2,8($16)
	sh	$2,32($18)
	lw	$3,%lo(route_list_inuse)($4)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L432
	move	$8,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L433:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L430
	and	$9,$2,$6
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$9,$5,$L469
$L430:
	move	$2,$4
$L431:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L433
	move	$4,$2
	.set	macro
	.set	reorder

$L432:
	li	$2,2			# 0x2
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$8,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$5,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($5)
	beq	$2,$0,$L434
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$7,16($18)
	lw	$6,28($18)
$L434:
	beq	$6,$7,$L435
	lbu	$2,1($18)
	ori	$2,$2,0x4
	sb	$2,1($18)
$L435:
	lhu	$3,8($18)
	lhu	$2,32($18)
	beq	$3,$2,$L436
	lbu	$2,1($18)
	ori	$2,$2,0x8
	sb	$2,1($18)
$L436:
	lbu	$3,1($20)
	move	$4,$17
	andi	$3,$3,0x3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	sw	$3,132($sp)
	.set	macro
	.set	reorder

	lw	$3,132($sp)
	sll	$2,$2,2
	li	$6,14			# 0xe
	or	$3,$3,$2
	li	$2,255			# 0xff
	sb	$3,1($20)
	sh	$2,78($18)
	sw	$0,44($18)
	sw	$0,48($18)
	sw	$0,60($18)
	sb	$0,72($18)
	sb	$0,73($18)
	lw	$2,20($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,24($16)
	sh	$2,0($21)
	lw	$2,12($16)
	sw	$2,16($20)
	lw	$2,16($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ori_vlanpri
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,60($sp)
	beq	$6,$0,$L438
	lw	$4,20($16)
	move	$5,$6
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L470
	move	$4,$17
	.set	macro
	.set	reorder

$L438:
	lw	$2,20($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,24($16)
	li	$6,14			# 0xe
	sh	$2,0($21)
	lw	$2,4($16)
	sw	$2,16($20)
	lw	$2,8($16)
	.set	noreorder
	.set	nomacro
	jal	memset
	sh	$2,2($21)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$4,64($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L444
	lw	$5,64($sp)
	.set	macro
	.set	reorder

	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L471
	move	$4,$17
	.set	macro
	.set	reorder

$L444:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	sw	$2,48($18)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_dscp_in
	move	$5,$17
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_set_entry_vlanid_in
	move	$5,$17
	.set	macro
	.set	reorder

	lw	$6,56($sp)
	li	$2,6			# 0x6
	beq	$6,$2,$L472
	li	$4,1			# 0x1
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$2,56($18)
$L448:
	lui	$2,%hi(jiffies)
	sw	$19,52($18)
	lw	$3,%lo(jiffies)($2)
	lw	$2,88($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L449
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$3,92($18)
	sw	$3,92($2)
$L450:
	lw	$2,92($18)
	lw	$3,88($18)
	lw	$6,68($sp)
	lw	$5,84($sp)
	sw	$3,0($2)
	lw	$3,76($sp)
	xor	$4,$5,$6
	srl	$5,$6,16
	lw	$2,8($3)
	xor	$4,$4,$5
	lw	$5,56($sp)
	addiu	$2,$2,-1
	sw	$2,8($3)
	sw	$0,88($18)
	lw	$3,4($23)
	lw	$6,72($sp)
	xor	$4,$4,$5
	sw	$3,92($18)
	lw	$5,4($23)
	xor	$4,$6,$4
	srl	$2,$6,8
	sw	$18,0($5)
	xor	$2,$4,$2
	lw	$4,8($23)
	lw	$5,92($sp)
	addiu	$6,$18,88
	addiu	$4,$4,1
	sw	$6,4($23)
	sw	$4,8($23)
	srl	$3,$5,4
	sw	$0,80($18)
	lw	$4,%lo(table_path)($22)
	lw	$6,88($sp)
	xor	$2,$2,$3
	srl	$3,$2,12
	xor	$2,$3,$2
	lw	$4,4($4)
	and	$3,$6,$2
	sll	$2,$3,4
	sll	$3,$3,2
	subu	$2,$2,$3
	addu	$3,$4,$2
	lw	$3,4($3)
	addiu	$4,$18,80
	sw	$3,84($18)
	lw	$3,%lo(table_path)($22)
	lw	$3,4($3)
	addu	$3,$3,$2
	lw	$3,4($3)
	sw	$18,0($3)
	lw	$3,%lo(table_path)($22)
	lw	$3,4($3)
	addu	$3,$3,$2
	sw	$4,4($3)
	lw	$3,%lo(table_path)($22)
	lw	$3,4($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L428:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	addiu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,28
	.set	macro
	.set	reorder

	lw	$5,108($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,124($sp)
	sw	$2,12($20)
	lw	$3,116($sp)
	sh	$3,0($21)
	lw	$4,120($sp)
	sw	$4,16($20)
	lw	$5,112($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dst
	sh	$5,2($21)
	.set	macro
	.set	reorder

	lw	$5,100($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,96($sp)
	move	$4,$17
	sb	$6,1($20)
	.set	noreorder
	.set	nomacro
	jal	rtl_restore_skb_vlanpri
	lw	$5,104($sp)
	.set	macro
	.set	reorder

	lw	$2,64($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L486
	lw	$3,60($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$3,60($sp)
$L486:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L487
	lw	$2,%lo(table_path)($22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$3
	.set	macro
	.set	reorder

	lw	$2,%lo(table_path)($22)
$L487:
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
	lw	$2,%lo(table_napt)($fp)
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
	lw	$4,80($sp)
	andi	$2,$4,0x1
	beq	$2,$0,$L473
	lw	$2,80($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	lw	$5,56($sp)
$L479:
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L395
	move	$2,$0
	.set	macro
	.set	reorder

$L461:
	.set	noreorder
	.set	nomacro
	j	$L403
	addiu	$10,$3,16
	.set	macro
	.set	reorder

$L469:
	.set	noreorder
	.set	nomacro
	j	$L431
	addiu	$8,$3,16
	.set	macro
	.set	reorder

$L473:
	move	$2,$4
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L479
	lw	$5,56($sp)
	.set	macro
	.set	reorder

$L468:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L424
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L472:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L448
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L397:
	lhu	$2,2($6)
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
	sh	$2,2($6)
 #APP
 # 139 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/spinlock.h" 1
	.set	push
	.set	noreorder
	.set	mips2
	sync
	.set	pop
 # 0 "" 2
 #NO_APP
	lw	$5,80($sp)
	andi	$2,$5,0x1
	bne	$2,$0,$L455
	move	$2,$5
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L395
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L470:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L474
$L440:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$4,$2,6
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,36
	.set	macro
	.set	reorder

	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,100($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_src_phy_port
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L475
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

$L441:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L476
	lw	$2,44($18)
	.set	noreorder
	.set	nomacro
	j	$L438
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L471:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$16,$2
	li	$2,1			# 0x1
	beq	$16,$2,$L477
$L445:
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,64($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,128($sp)
	li	$2,2048			# 0x800
	bne	$5,$2,$L444
	lw	$5,44($18)
	bne	$5,$0,$L478
	lui	$2,%hi(nf_hooks+160)
$L481:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L444
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,64($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

	j	$L444
$L398:
	lw	$4,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	.set	noreorder
	.set	nomacro
	j	$L399
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L449:
	lw	$2,92($18)
	lw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	j	$L450
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L425:
	lw	$2,92($18)
	lui	$3,%hi(path_list_free)
	addiu	$3,$3,%lo(path_list_free)
	sw	$3,76($sp)
	.set	noreorder
	.set	nomacro
	j	$L426
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L455:
	lw	$2,80($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L395
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L476:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L438
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L475:
	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L441
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,64($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$5,$0
	.set	macro
	.set	reorder

	j	$L441
$L474:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,132($sp)
	.set	macro
	.set	reorder

	lw	$3,132($sp)
	bne	$2,$3,$L440
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$4,13($2)
	sll	$2,$3,8
	or	$4,$4,$2
	li	$2,2048			# 0x800
	beq	$4,$2,$L440
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L440
$L462:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$23,$L410
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$4,13($2)
	sll	$2,$3,8
	or	$4,$4,$2
	li	$2,2048			# 0x800
	beq	$4,$2,$L410
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L410
$L465:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$23,$L418
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$4,13($2)
	sll	$2,$3,8
	or	$4,$4,$2
	li	$2,2048			# 0x800
	beq	$4,$2,$L418
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L418
$L477:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$16,$L445
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,12($2)
	lbu	$4,13($2)
	sll	$2,$3,8
	or	$4,$4,$2
	li	$2,2048			# 0x800
	beq	$4,$2,$L445
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L445
$L466:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L480
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

$L478:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L481
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

$L463:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L482
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L467:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L417
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L464:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L409
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L394:
	lw	$2,80($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$2, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L395
	move	$2,$0
	.set	macro
	.set	reorder

	.end	rtk_addNaptConnection
	.size	rtk_addNaptConnection, .-rtk_addNaptConnection
	.section	.text.negative_fragCache_init,"ax",@progbits
	.align	2
	.globl	negative_fragCache_init
	.set	nomips16
	.ent	negative_fragCache_init
	.type	negative_fragCache_init, @function
negative_fragCache_init:
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
	lw	$4,40($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$16,%hi(negative_cache_table)

	beq	$2,$0,$L489
	sw	$2,%lo(negative_cache_table)($16)

	lui	$3,%hi(negative_cache_list_free)
	addiu	$17,$3,%lo(negative_cache_list_free)
	sw	$0,%lo(negative_cache_list_free)($3)
	sw	$0,8($17)
	sw	$17,4($17)
	move	$3,$0
	move	$7,$16
	j	$L491
	li	$6,64			# 0x40

$L496:
	lw	$2,%lo(negative_cache_table)($7)
$L491:
	sll	$4,$3,2
	sll	$5,$3,4
	subu	$4,$5,$4
	addu	$2,$2,$4
	sw	$0,12($2)
	lw	$2,%lo(negative_cache_table)($16)
	addiu	$3,$3,1
	addu	$2,$2,$4
	sw	$0,4($2)
	lw	$2,%lo(negative_cache_table)($16)
	addu	$4,$2,$4
	addiu	$2,$4,4
	bne	$3,$6,$L496
	sw	$2,8($4)

	lui	$21,%hi(negative_cache_timeout)
	addiu	$21,$21,%lo(negative_cache_timeout)
	move	$18,$0
	j	$L492
	li	$20,64			# 0x40

$L498:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,32($2)
	sw	$21,28($2)
	sw	$0,48($2)
	lw	$4,4($17)
	addiu	$3,$2,48
	sw	$4,52($2)
	lw	$4,4($17)
	sw	$2,0($4)
	lw	$2,8($17)
	sw	$3,4($17)
	addiu	$2,$2,1
	beq	$18,$20,$L497
	sw	$2,8($17)

$L492:
	lw	$4,24($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$18,$18,1

	addiu	$4,$2,12
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L498
	move	$7,$0

$L489:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,56

$L497:
	lui	$2,%hi($LC4)
	lw	$3,%lo(negative_cache_table)($16)
	lw	$4,%lo($LC4)($2)
	li	$2,1			# 0x1
	sw	$4,0($3)
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	negative_fragCache_init
	.size	negative_fragCache_init, .-negative_fragCache_init
	.section	.text.udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	udp_fragCache_init
	.set	nomips16
	.ent	udp_fragCache_init
	.type	udp_fragCache_init, @function
udp_fragCache_init:
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
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	move	$18,$4
	lw	$4,20($20)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$19,%hi(udp_cache_table)

	move	$17,$2
	beq	$2,$0,$L500
	sw	$2,%lo(udp_cache_table)($19)

	lui	$2,%hi(udp_cache_list_free)
	sll	$3,$18,2
	sll	$4,$18,4
	subu	$4,$4,$3
	addiu	$16,$2,%lo(udp_cache_list_free)
	sw	$0,%lo(udp_cache_list_free)($2)
	li	$5,32			# 0x20
	lui	$2,%hi(max_udp_frag_entry)
	sw	$18,%lo(max_udp_frag_entry)($2)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,4($17)
	lw	$4,%lo(udp_cache_table)($19)
	lw	$3,4($4)
	beq	$3,$0,$L502
	li	$2,-1			# 0xffffffffffffffff

	blez	$18,$L503
	nop

	move	$2,$0
	move	$4,$0
	j	$L505
	move	$5,$19

$L509:
	lw	$3,%lo(udp_cache_table)($5)
	lw	$3,4($3)
$L505:
	addu	$3,$3,$2
	sw	$0,8($3)
	lw	$3,%lo(udp_cache_table)($19)
	addiu	$4,$4,1
	lw	$3,4($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(udp_cache_table)($19)
	lw	$3,4($3)
	addu	$3,$3,$2
	sw	$3,4($3)
	bne	$4,$18,$L509
	addiu	$2,$2,12

	lui	$21,%hi(cache_timeout)
	addiu	$21,$21,%lo(cache_timeout)
	j	$L506
	move	$17,$0

$L511:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	slt	$3,$17,$18
	sw	$2,36($2)
	sw	$21,32($2)
	sw	$0,52($2)
	lw	$5,4($16)
	addiu	$4,$2,52
	sw	$5,56($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L510
	sw	$2,8($16)

$L506:
	lw	$4,24($20)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,16
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L511
	move	$7,$0

$L500:
	move	$2,$0
$L502:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L510:
	lw	$4,%lo(udp_cache_table)($19)
$L503:
	lui	$2,%hi($LC4)
	lw	$3,%lo($LC4)($2)
	li	$2,1			# 0x1
	j	$L502
	sw	$3,0($4)

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.size	udp_fragCache_init, .-udp_fragCache_init
	.globl	fastpath_packet_bypassed_num
	.section	.data.fastpath_packet_bypassed_num,"aw",@progbits
	.align	2
	.type	fastpath_packet_bypassed_num, @object
	.size	fastpath_packet_bypassed_num, 4
fastpath_packet_bypassed_num:
	.word	10
	.globl	rtl_init_bypass_packet_hook
	.section	.bss.rtl_init_bypass_packet_hook,"aw",@nobits
	.align	2
	.type	rtl_init_bypass_packet_hook, @object
	.size	rtl_init_bypass_packet_hook, 4
rtl_init_bypass_packet_hook:
	.space	4
	.globl	rtl_check_bypass_packet_hook
	.section	.bss.rtl_check_bypass_packet_hook,"aw",@nobits
	.align	2
	.type	rtl_check_bypass_packet_hook, @object
	.size	rtl_check_bypass_packet_hook, 4
rtl_check_bypass_packet_hook:
	.space	4
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
	.section	.bss.max_udp_frag_entry,"aw",@nobits
	.align	2
	.type	max_udp_frag_entry, @object
	.size	max_udp_frag_entry, 4
max_udp_frag_entry:
	.space	4
	.globl	route_list_inuse
	.section	.bss.route_list_inuse,"aw",@nobits
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.globl	route_list_free
	.section	.bss.route_list_free,"aw",@nobits
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	napt_list_inuse
	.section	.bss.napt_list_inuse,"aw",@nobits
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	napt_list_free
	.section	.bss.napt_list_free,"aw",@nobits
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	table_napt
	.section	.bss.table_napt,"aw",@nobits
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.globl	path_list_inuse
	.section	.bss.path_list_inuse,"aw",@nobits
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	path_list_free
	.section	.bss.path_list_free,"aw",@nobits
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	table_path
	.section	.bss.table_path,"aw",@nobits
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.globl	fp_arp
	.section	.bss.fp_arp,"aw",@nobits
	.align	2
	.type	fp_arp, @object
	.size	fp_arp, 4
fp_arp:
	.space	4
	.globl	fp_path
	.section	.bss.fp_path,"aw",@nobits
	.align	2
	.type	fp_path, @object
	.size	fp_path, 4
fp_path:
	.space	4
	.globl	fp_route
	.section	.bss.fp_route,"aw",@nobits
	.align	2
	.type	fp_route, @object
	.size	fp_route, 4
fp_route:
	.space	4
	.globl	fp_napt
	.section	.bss.fp_napt,"aw",@nobits
	.align	2
	.type	fp_napt, @object
	.size	fp_napt, 4
fp_napt:
	.space	4
	.globl	udp_cache_list_free
	.section	.bss.udp_cache_list_free,"aw",@nobits
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.section	.bss.udp_cache_table,"aw",@nobits
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.globl	negative_cache_list_free
	.section	.bss.negative_cache_list_free,"aw",@nobits
	.align	2
	.type	negative_cache_list_free, @object
	.size	negative_cache_list_free, 12
negative_cache_list_free:
	.space	12
	.globl	negative_cache_table
	.section	.bss.negative_cache_table,"aw",@nobits
	.align	2
	.type	negative_cache_table, @object
	.size	negative_cache_table, 4
negative_cache_table:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 1459) 4.4.7"
