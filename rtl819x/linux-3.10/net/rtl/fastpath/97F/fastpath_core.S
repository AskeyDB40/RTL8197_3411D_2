	.file	1 "fastpath_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
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
	beq	$2,$0,$L15
	nop

	lw	$5,8($4)
	lw	$3,4($4)
	lhu	$2,2($4)
	lbu	$6,1($4)
	xor	$3,$5,$3
	lui	$5,%hi(max_udp_frag_entry)
	xor	$2,$2,$3
	lw	$5,%lo(max_udp_frag_entry)($5)
	srl	$3,$3,16
	xor	$2,$2,$3
	lw	$3,44($4)
	xor	$6,$2,$6
	srl	$2,$2,8
	xor	$2,$6,$2
	addiu	$5,$5,-1
	and	$2,$2,$5
	beq	$3,$0,$L12
	sb	$0,0($4)

	lw	$6,48($4)
	sll	$5,$2,4
	sll	$2,$2,2
	sw	$6,48($3)
	subu	$5,$5,$2
	lui	$3,%hi(udp_cache_table)
$L13:
	lw	$6,48($4)
	lw	$7,44($4)
	lui	$2,%hi(udp_cache_list_free)
	addiu	$2,$2,%lo(udp_cache_list_free)
	sw	$7,0($6)
	lw	$7,%lo(udp_cache_table)($3)
	lui	$3,%hi(statistic_udp_frag_free)
	addiu	$6,$4,52
	lw	$7,0($7)
	addu	$5,$7,$5
	lw	$7,8($5)
	addiu	$7,$7,-1
	sw	$7,8($5)
	sw	$0,52($4)
	lw	$5,4($2)
	sw	$5,56($4)
	lw	$5,4($2)
	sw	$4,0($5)
	lw	$5,8($2)
	lw	$4,%lo(statistic_udp_frag_free)($3)
	sw	$6,4($2)
	addiu	$5,$5,1
	addiu	$4,$4,1
	sw	$5,8($2)
	sw	$4,%lo(statistic_udp_frag_free)($3)
$L15:
	j	$31
	nop

$L12:
	lui	$3,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($3)
	sll	$5,$2,4
	sll	$2,$2,2
	lw	$7,0($6)
	lw	$6,48($4)
	subu	$5,$5,$2
	addu	$2,$7,$5
	j	$L13
	sw	$6,4($2)

	.set	macro
	.set	reorder
	.end	cache_timeout
	.size	cache_timeout, .-cache_timeout
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
	
	lw	$3,4($4)
	lw	$5,8($4)
	lhu	$2,2($4)
	lbu	$6,1($4)
	xor	$5,$5,$3
	lui	$3,%hi(max_udp_frag_entry)
	xor	$2,$2,$5
	lw	$3,%lo(max_udp_frag_entry)($3)
	srl	$5,$5,16
	xor	$2,$2,$5
	lw	$5,40($4)
	xor	$6,$2,$6
	srl	$2,$2,8
	xor	$2,$6,$2
	addiu	$3,$3,-1
	and	$2,$2,$3
	beq	$5,$0,$L17
	sb	$0,0($4)

	lw	$7,44($4)
	sll	$3,$2,4
	lui	$6,%hi(negative_cache_table)
	sw	$7,44($5)
	sll	$2,$2,2
$L18:
	lw	$5,44($4)
	lw	$7,40($4)
	subu	$3,$3,$2
	lui	$2,%hi(negative_cache_list_free)
	sw	$7,0($5)
	lw	$6,%lo(negative_cache_table)($6)
	addiu	$2,$2,%lo(negative_cache_list_free)
	addiu	$5,$4,48
	addu	$3,$6,$3
	lw	$6,8($3)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,48($4)
	lw	$3,4($2)
	sw	$3,52($4)
	lw	$3,4($2)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$5,4($2)
	addiu	$3,$3,1
	j	$31
	sw	$3,8($2)

$L17:
	lui	$6,%hi(negative_cache_table)
	lw	$8,%lo(negative_cache_table)($6)
	sll	$3,$2,4
	lw	$7,44($4)
	sll	$2,$2,2
	subu	$5,$3,$2
	addu	$5,$8,$5
	j	$L18
	sw	$7,4($5)

	.set	macro
	.set	reorder
	.end	negative_cache_timeout
	.size	negative_cache_timeout, .-negative_cache_timeout
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
	lw	$9,4($4)
	lw	$7,20($4)
	lw	$10,24($4)
	lw	$13,8($4)
	xor	$2,$7,$9
	srl	$8,$9,8
	lw	$3,0($4)
	xor	$2,$2,$10
	xor	$2,$2,$8
	srl	$8,$7,16
	xor	$2,$2,$8
	srl	$11,$13,4
	xori	$14,$3,0x2
	li	$12,6			# 0x6
	xor	$2,$2,$11
	li	$8,17			# 0x11
	lui	$11,%hi(path_table_list_max)
	movz	$8,$12,$14
	lw	$12,%lo(path_table_list_max)($11)
	lui	$11,%hi(table_path)
	xor	$2,$2,$8
	lw	$11,%lo(table_path)($11)
	srl	$14,$2,12
	xor	$2,$14,$2
	addiu	$12,$12,-1
	and	$2,$2,$12
	lw	$11,0($11)
	sll	$14,$2,4
	sll	$2,$2,2
	subu	$2,$14,$2
	addu	$2,$11,$2
	lw	$2,0($2)
	lui	$14,%hi(jiffies)
	lw	$16,%lo(jiffies)($14)
	beq	$2,$0,$L52
	nop

	li	$15,16711680			# 0xff0000
	li	$14,65536			# 0x10000
	lw	$25,16($4)
	lw	$24,12($4)
	ori	$15,$15,0xff
	j	$L43
	ori	$14,$14,0xff

$L41:
	lw	$2,80($2)
	beq	$2,$0,$L53
	srl	$4,$7,8

$L43:
	lw	$4,4($2)
	bne	$3,$4,$L41
	nop

	lw	$4,0($2)
	and	$4,$4,$15
	bne	$4,$14,$L41
	nop

	lw	$4,12($2)
	bne	$9,$4,$L41
	nop

	lhu	$4,10($2)
	bne	$13,$4,$L41
	nop

	lw	$4,20($2)
	bne	$4,$24,$L41
	nop

	lhu	$4,24($2)
	bne	$4,$25,$L41
	nop

	lw	$4,16($2)
	bne	$7,$4,$L41
	nop

	lhu	$4,8($2)
	bne	$10,$4,$L41
	subu	$4,$16,$5

	lw	$2,36($2)
	subu	$4,$4,$2
	bltz	$4,$L51
	nop

$L40:
	srl	$4,$7,8
$L53:
	srl	$2,$10,4
	xor	$2,$4,$2
	xor	$2,$2,$7
	xor	$2,$2,$8
	xor	$2,$2,$25
	srl	$4,$24,16
	xor	$2,$2,$24
	xor	$2,$2,$4
	srl	$4,$2,12
	xor	$2,$4,$2
	and	$12,$2,$12
	sll	$2,$12,4
	sll	$12,$12,2
	subu	$12,$2,$12
	addu	$11,$11,$12
	lw	$2,0($11)
	beq	$2,$0,$L44
	li	$11,16711680			# 0xff0000

	li	$8,131072			# 0x20000
	ori	$11,$11,0xff
	j	$L48
	ori	$8,$8,0xff

$L45:
	lw	$2,80($2)
	beq	$2,$0,$L44
	nop

$L48:
	lw	$4,4($2)
	bne	$3,$4,$L45
	nop

	lw	$4,0($2)
	and	$4,$4,$11
	bne	$4,$8,$L45
	nop

	lw	$4,16($2)
	bne	$24,$4,$L45
	nop

	lhu	$4,8($2)
	bne	$25,$4,$L45
	nop

	lw	$4,20($2)
	bne	$7,$4,$L45
	nop

	lhu	$4,24($2)
	bne	$10,$4,$L45
	nop

	lw	$4,28($2)
	bne	$9,$4,$L45
	nop

	lhu	$4,32($2)
	bne	$13,$4,$L45
	nop

	lw	$2,36($2)
	subu	$5,$16,$5
	subu	$5,$5,$2
	bgez	$5,$L44
	nop

$L51:
	sw	$2,0($6)
	j	$L42
	li	$2,-1			# 0xffffffffffffffff

$L44:
	move	$2,$0
$L42:
	lw	$16,4($sp)
	j	$31
	addiu	$sp,$sp,8

$L52:
	lw	$25,16($4)
	j	$L40
	lw	$24,12($4)

	.set	macro
	.set	reorder
	.end	rtk_idleNaptConnection
	.size	rtk_idleNaptConnection, .-rtk_idleNaptConnection
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
	beq	$3,$0,$L62
	move	$2,$0

	move	$6,$0
$L59:
	lw	$5,8($3)
	sltu	$7,$5,$6
	bne	$7,$0,$L56
	and	$8,$4,$5

	lw	$7,4($3)
	beq	$8,$7,$L61
	nop

$L56:
	move	$5,$6
$L57:
	lw	$3,44($3)
	bne	$3,$0,$L59
	move	$6,$5

$L62:
	j	$31
	nop

$L61:
	j	$L57
	addiu	$2,$3,16

	.set	macro
	.set	reorder
	.end	FastPath_Route
	.size	FastPath_Route, .-FastPath_Route
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
	lhu	$3,16($sp)
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
	lw	$8,0($8)
	sll	$11,$2,4
	sll	$2,$2,2
	subu	$2,$11,$2
	addu	$2,$8,$2
	lw	$2,0($2)
	beq	$2,$0,$L81
	nop

	j	$L75
	li	$11,255			# 0xff

$L65:
	lw	$2,80($2)
	beq	$2,$0,$L81
	nop

$L75:
	lhu	$8,10($2)
	bne	$8,$5,$L65
	nop

	lhu	$8,8($2)
	bne	$8,$7,$L65
	nop

	lw	$8,12($2)
	bne	$8,$4,$L65
	nop

	lw	$8,16($2)
	bne	$8,$6,$L65
	nop

	lbu	$12,0($2)
	bne	$12,$11,$L65
	nop

	lbu	$12,2($2)
	li	$6,1			# 0x1
	beq	$12,$6,$L79
	sb	$0,0($2)

	lhu	$13,32($2)
	lw	$6,28($2)
$L67:
	beq	$6,$0,$L81
	nop

	li	$11,1			# 0x1
	beq	$12,$11,$L80
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
$L70:
	lw	$2,%lo(table_path)($9)
	sll	$3,$10,4
	sll	$10,$10,2
	lw	$2,0($2)
	subu	$10,$3,$10
	addu	$10,$2,$10
	lw	$2,0($10)
	beq	$2,$0,$L81
	nop

	j	$L74
	li	$6,255			# 0xff

$L71:
	lw	$2,80($2)
	beq	$2,$0,$L81
	nop

$L74:
	lw	$3,20($2)
	bne	$8,$3,$L71
	nop

	lhu	$3,24($2)
	bne	$3,$7,$L71
	nop

	lw	$3,28($2)
	bne	$4,$3,$L71
	nop

	lhu	$3,32($2)
	bne	$3,$5,$L71
	nop

	lbu	$3,0($2)
	bne	$3,$6,$L71
	nop

	j	$L71
	sb	$0,0($2)

$L81:
	j	$31
	nop

$L80:
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
	j	$L70
	and	$10,$6,$10

$L79:
	lhu	$13,24($2)
	j	$L67
	lw	$6,20($2)

	.set	macro
	.set	reorder
	.end	mark_path_invalid
	.size	mark_path_invalid, .-mark_path_invalid
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
	.text
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
	
	beq	$4,$0,$L89
	nop

	lui	$5,%hi($LC0)
	lui	$6,%hi($LC1)
	lui	$7,%hi($LC2)
	addiu	$5,$5,%lo($LC0)
	addiu	$6,$6,%lo($LC1)
	addiu	$7,$7,%lo($LC2)
	j	sprintf
	nop

$L89:
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
	.text
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
	beq	$2,$0,$L92
	move	$7,$0

$L99:
	lw	$2,88($2)
	bne	$2,$0,$L99
	addiu	$7,$7,1

$L92:
	lui	$2,%hi(napt_list_inuse)
	lw	$2,%lo(napt_list_inuse)($2)
	beq	$2,$0,$L95
	move	$6,$0

$L98:
	lw	$2,44($2)
	bne	$2,$0,$L98
	addiu	$6,$6,1

$L95:
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
	move	$18,$4
	lw	$4,28($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$17,%hi(table_path)
	move	$21,$2
	beq	$2,$0,$L103
	sw	$2,%lo(table_path)($17)

	lui	$6,%hi(path_list_inuse)
	sll	$2,$18,2
	sll	$4,$18,4
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
	sw	$18,%lo(path_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($21)
	lw	$2,%lo(table_path)($17)
	lw	$4,0($2)
	beq	$4,$0,$L103
	nop

	blez	$18,$L104
	move	$2,$0

	move	$3,$0
	j	$L105
	move	$5,$17

$L112:
	lw	$4,%lo(table_path)($5)
	lw	$4,0($4)
$L105:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_path)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_path)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L112
	addiu	$2,$2,12

$L104:
	blez	$19,$L106
	move	$17,$0

	j	$L113
	lw	$4,28($20)

$L107:
	sw	$0,88($2)
	lw	$5,4($16)
	sw	$5,92($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L106
	sw	$2,8($16)

	lw	$4,28($20)
$L113:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,88
	bne	$2,$0,$L107
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L108:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L106:
	j	$L108
	move	$2,$0

$L103:
	j	$L108
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_path
	.size	init_table_path, .-init_table_path
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
	move	$18,$4
	lw	$4,28($20)
	move	$19,$5
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$17,%hi(table_napt)
	move	$21,$2
	beq	$2,$0,$L115
	sw	$2,%lo(table_napt)($17)

	lui	$6,%hi(napt_list_inuse)
	sll	$2,$18,2
	sll	$4,$18,4
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
	sw	$18,%lo(napt_table_list_max)($3)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($21)
	lw	$2,%lo(table_napt)($17)
	lw	$4,0($2)
	beq	$4,$0,$L115
	nop

	blez	$18,$L116
	move	$2,$0

	move	$3,$0
	j	$L117
	move	$5,$17

$L124:
	lw	$4,%lo(table_napt)($5)
	lw	$4,0($4)
$L117:
	addu	$4,$4,$2
	sw	$0,8($4)
	lw	$4,%lo(table_napt)($17)
	addiu	$3,$3,1
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$0,0($4)
	lw	$4,%lo(table_napt)($17)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$4,4($4)
	bne	$3,$18,$L124
	addiu	$2,$2,12

$L116:
	blez	$19,$L118
	move	$17,$0

	j	$L125
	lw	$4,28($20)

$L119:
	sw	$0,44($2)
	lw	$5,4($16)
	sw	$5,48($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L118
	sw	$2,8($16)

	lw	$4,28($20)
$L125:
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,44
	bne	$2,$0,$L119
	slt	$3,$17,$19

	li	$2,-2			# 0xfffffffffffffffe
$L120:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L118:
	j	$L120
	move	$2,$0

$L115:
	j	$L120
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_table_napt
	.size	init_table_napt, .-init_table_napt
	.section	.rodata.str1.4
	.align	2
$LC4:
	.ascii	"fp_napt\000"
	.align	2
$LC5:
	.ascii	"fp_path\000"
	.text
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
	beq	$2,$0,$L127
	lui	$4,%hi($LC4)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC4)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_napt)($16)
$L127:
	lui	$16,%hi(fp_path)
	lw	$2,%lo(fp_path)($16)
	beq	$2,$0,$L129
	lui	$4,%hi($LC5)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC5)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(fp_path)($16)
$L129:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	remove_fastpath_debug_proc
	.size	remove_fastpath_debug_proc, .-remove_fastpath_debug_proc
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
	lui	$4,%hi($LC4)
	lui	$7,%hi(fastpath_napt_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC4)
	addiu	$7,$7,%lo(fastpath_napt_proc_fops)
	move	$5,$0
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC5)
	lui	$7,%hi(fastpath_path_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	lui	$3,%hi(fp_napt)
	addiu	$4,$4,%lo($LC5)
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
$LC6:
	.ascii	"\012udp frag cache entry:\012\000"
	.align	2
$LC7:
	.ascii	"src_ip: 0x%08x src_port:0x%04x dst_ip: 0x%08x dst_port:0"
	.ascii	"x%04x frag_id:0x%04x protocol:0x%x status:0x%x \012\000"
	.align	2
$LC8:
	.ascii	"total num: %d \012\000"
	.text
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
	lui	$4,%hi($LC6)
	addiu	$4,$4,%lo($LC6)
	jal	printk
	lui	$21,%hi($LC7)

	addiu	$21,$21,%lo($LC7)
	move	$16,$0
	move	$20,$0
	lui	$19,%hi(udp_cache_table)
	li	$18,768			# 0x300
	lw	$2,%lo(udp_cache_table)($19)
$L139:
	lw	$2,0($2)
	addu	$2,$2,$16
	lw	$17,0($2)
	beq	$17,$0,$L133
	nop

$L134:
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
	bne	$17,$0,$L134
	nop

$L133:
	addiu	$16,$16,12
	bne	$16,$18,$L139
	lw	$2,%lo(udp_cache_table)($19)

	move	$5,$20
	lw	$31,60($sp)
	lw	$21,56($sp)
	lw	$20,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	lui	$4,%hi($LC8)
	addiu	$4,$4,%lo($LC8)
	j	printk
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
	.end	rtl_readUdpFragEntry
	.size	rtl_readUdpFragEntry, .-rtl_readUdpFragEntry
	.section	.rodata.str1.4
	.align	2
$LC9:
	.ascii	"UDP\000"
	.align	2
$LC10:
	.ascii	"TCP\000"
	.align	2
$LC11:
	.ascii	"~Path: [%s] in-S=0x%08X:%-5u in-D=0x%08X:%-5u out-S=0x%0"
	.ascii	"8X:%-5u out-D=0x%08X:%-5u out-ifname=%-5s <%u> {%d}\000"
	.align	2
$LC12:
	.ascii	",[PreMark:%d,PostMark:%d]\000"
	.align	2
$LC13:
	.ascii	",[policy_route_mark:%d]\000"
	.align	2
$LC14:
	.ascii	",[dscp_in:%d,dscp_out:%d]\000"
	.align	2
$LC15:
	.ascii	",[vlanPrio_out:%d]\000"
	.align	2
$LC16:
	.ascii	"\012\000"
	.text
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
	bne	$16,$0,$L147
	lui	$3,%hi($LC10)

$L141:
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

$L147:
	lui	$fp,%hi($LC9)
	lui	$23,%hi($LC11)
	lui	$22,%hi($LC12)
	lui	$21,%hi($LC13)
	lui	$20,%hi($LC14)
	lui	$19,%hi($LC15)
	lui	$18,%hi($LC16)
	addiu	$3,$3,%lo($LC10)
	addiu	$fp,$fp,%lo($LC9)
	addiu	$23,$23,%lo($LC11)
	addiu	$22,$22,%lo($LC12)
	addiu	$21,$21,%lo($LC13)
	addiu	$20,$20,%lo($LC14)
	addiu	$19,$19,%lo($LC15)
	addiu	$18,$18,%lo($LC16)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L148:
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
	beq	$16,$0,$L141
	lw	$3,56($sp)

	j	$L148
	lw	$2,16($16)

	.set	macro
	.set	reorder
	.end	fastpath_table_path
	.size	fastpath_table_path, .-fastpath_table_path
	.section	.rodata.str1.4
	.align	2
$LC17:
	.ascii	"~Napt: [%s] int=0x%08X:%-5u ext=0x%08X:%-5u rem=0x%08X:%"
	.ascii	"-5u flags=0x%08X \012\000"
	.text
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
	bne	$16,$0,$L156
	lui	$20,%hi($LC10)

	lw	$31,60($sp)
$L158:
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,64

$L156:
	lui	$19,%hi($LC9)
	lui	$18,%hi($LC17)
	addiu	$20,$20,%lo($LC10)
	addiu	$19,$19,%lo($LC9)
	addiu	$18,$18,%lo($LC17)
	li	$17,2			# 0x2
	lw	$2,16($16)
$L157:
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
	beq	$16,$0,$L158
	lw	$31,60($sp)

	j	$L157
	lw	$2,16($16)

	.set	macro
	.set	reorder
	.end	fastpath_table_napt
	.size	fastpath_table_napt, .-fastpath_table_napt
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
	.section	.rodata.str1.4
	.align	2
$LC18:
	.ascii	"lo\000"
	.text
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
	bne	$2,$0,$L164
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lbu	$19,9($17)
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L165
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L165
$L164:
	move	$4,$0
$L166:
	lw	$31,92($sp)
$L313:
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	move	$2,$4
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

$L165:
	lw	$21,0($17)
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

	move	$4,$2
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L313
	lw	$31,92($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi($LC18)
	addiu	$2,$2,%lo($LC18)
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($16)
1:	lbu	$1,($2)
	addiu	$16,1
	bne	$1,$3,2f
	addiu	$2,1
	bnez	$3,1b
	lbu	$3,($16)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L164
	lhu	$2,6($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	lw	$fp,12($17)
	lw	$3,16($17)
	lhu	$20,0($21)
	lhu	$23,2($21)
	.set	noreorder
	.set	nomacro
	beq	$19,$4,$L282
	andi	$5,$2,0x3fff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L283
	lui	$4,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

	move	$22,$0
$L316:
	move	$9,$0
	move	$8,$0
	li	$11,1			# 0x1
$L184:
	xor	$7,$3,$fp
	srl	$4,$fp,8
	srl	$2,$3,16
	xor	$4,$7,$4
	xor	$4,$4,$2
	xor	$4,$4,$19
	lui	$2,%hi(path_table_list_max)
	lw	$6,%lo(path_table_list_max)($2)
	xor	$4,$4,$23
	srl	$2,$20,4
	xor	$4,$4,$2
	lui	$2,%hi(table_path)
	lw	$5,%lo(table_path)($2)
	srl	$2,$4,12
	xor	$4,$2,$4
	addiu	$6,$6,-1
	and	$2,$4,$6
	lw	$4,0($5)
	sll	$5,$2,4
	sll	$2,$2,2
	subu	$2,$5,$2
	addu	$2,$4,$2
	lw	$16,0($2)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L188
	li	$12,2			# 0x2
	.set	macro
	.set	reorder

	li	$4,255			# 0xff
	li	$10,1			# 0x1
	li	$6,17			# 0x11
	.set	noreorder
	.set	nomacro
	j	$L263
	li	$5,6			# 0x6
	.set	macro
	.set	reorder

$L189:
	lw	$16,80($16)
	beq	$16,$0,$L188
$L263:
	lhu	$2,10($16)
	bne	$2,$20,$L189
	lhu	$2,8($16)
	bne	$2,$23,$L189
	lw	$2,12($16)
	bne	$2,$fp,$L189
	lw	$2,16($16)
	bne	$2,$3,$L189
	lbu	$2,0($16)
	bne	$2,$4,$L189
	lw	$2,4($16)
	beq	$2,$12,$L284
	bne	$2,$10,$L189
	.set	noreorder
	.set	nomacro
	bne	$19,$6,$L189
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
$L321:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L314
	addiu	$5,$16,60
	.set	macro
	.set	reorder

	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	sw	$11,44($sp)
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,48($sp)
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L164
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	addiu	$5,$16,60
$L314:
	move	$4,$18
	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_recheck_fp_policy_route_mark
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,60($16)
	move	$4,$18
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
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L285
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	move	$6,$0
	move	$4,$18
	sw	$3,48($sp)
	sw	$7,32($sp)
	sw	$8,40($sp)
	sw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$5,28($16)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_dst_check
	move	$6,$17
	.set	macro
	.set	reorder

	li	$4,-1			# 0xffffffffffffffff
	lw	$3,48($sp)
	lw	$7,32($sp)
	lw	$8,40($sp)
	lw	$9,36($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L286
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$19,$2,$L196
	addiu	$10,$21,16
	.set	macro
	.set	reorder

	lhu	$10,6($21)
	addiu	$2,$21,6
	movz	$2,$0,$10
	move	$10,$2
$L196:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L198
	lui	$4,%hi(udp_cache_list_free)
	.set	macro
	.set	reorder

	lw	$2,%lo(udp_cache_list_free)($4)
	lui	$5,%hi(max_udp_frag_entry)
	lw	$8,%lo(max_udp_frag_entry)($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L287
	lhu	$5,4($17)
	.set	macro
	.set	reorder

	lw	$6,52($2)
	li	$9,1			# 0x1
	sh	$20,12($2)
	sh	$23,14($2)
	sb	$9,0($2)
	sh	$5,2($2)
	sw	$fp,4($2)
	sw	$3,8($2)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L200
	sb	$19,1($2)
	.set	macro
	.set	reorder

	lw	$9,56($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	sw	$9,56($6)
$L201:
	lw	$6,56($2)
	lw	$12,52($2)
	xor	$9,$5,$7
	srl	$7,$7,16
	sw	$12,0($6)
	lw	$5,8($4)
	lui	$6,%hi(udp_cache_table)
	xor	$7,$9,$7
	addiu	$5,$5,-1
	sw	$5,8($4)
	sw	$0,44($2)
	lw	$5,%lo(udp_cache_table)($6)
	srl	$4,$7,8
	xor	$7,$7,$19
	xor	$7,$7,$4
	addiu	$4,$8,-1
	and	$4,$7,$4
	lw	$7,0($5)
	sll	$5,$4,4
	sll	$4,$4,2
	subu	$5,$5,$4
	addu	$4,$7,$5
	lw	$4,4($4)
	addiu	$8,$2,44
	sw	$4,48($2)
	lw	$7,%lo(udp_cache_table)($6)
	addiu	$4,$2,16
	lw	$7,0($7)
	addu	$7,$7,$5
	lw	$7,4($7)
	sw	$2,0($7)
	lw	$7,%lo(udp_cache_table)($6)
	lw	$7,0($7)
	addu	$7,$7,$5
	sw	$8,4($7)
	lw	$6,%lo(udp_cache_table)($6)
	lw	$6,0($6)
	addu	$5,$6,$5
	lw	$6,8($5)
	addiu	$6,$6,1
	sw	$6,8($5)
	lui	$5,%hi(jiffies)
	lw	$5,%lo(jiffies)($5)
	addiu	$5,$5,1000
	sw	$5,24($2)
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(statistic_udp_frag_add)
	lw	$4,%lo(statistic_udp_frag_add)($2)
	lw	$3,48($sp)
	lw	$10,32($sp)
	lw	$11,44($sp)
	addiu	$4,$4,1
	sw	$4,%lo(statistic_udp_frag_add)($2)
$L202:
	move	$4,$18
$L319:
	move	$5,$17
	move	$6,$19
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	fast_path_before_nat_check
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	move	$4,$2
	li	$2,1			# 0x1
	lw	$3,48($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L166
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	li	$2,6			# 0x6
	beq	$19,$2,$L288
$L205:
	lbu	$2,1($16)
	sltu	$4,$2,13
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L206
	lui	$4,%hi($L212)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addiu	$4,$4,%lo($L212)
	addu	$2,$4,$2
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
	.align	2
$L212:
	.word	$L207
	.word	$L208
	.word	$L209
	.word	$L209
	.word	$L210
	.word	$L206
	.word	$L206
	.word	$L206
	.word	$L211
	.word	$L206
	.word	$L206
	.word	$L206
	.word	$L211
	.text
$L283:
$L310:
	lw	$5,%lo(statistic_udp_frag_rx)($4)
	addiu	$5,$5,1
	sw	$5,%lo(statistic_udp_frag_rx)($4)
	li	$4,8192			# 0x2000
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L289
	move	$9,$0
	.set	macro
	.set	reorder

$L172:
	andi	$4,$2,0x2000
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L315
	move	$9,$0
	.set	macro
	.set	reorder

	andi	$2,$2,0x1fff
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L173
	move	$8,$0
	.set	macro
	.set	reorder

	li	$9,1			# 0x1
$L173:
	lw	$7,12($17)
	lw	$10,16($17)
	lhu	$6,4($17)
	lbu	$12,9($17)
	xor	$4,$10,$7
	lui	$11,%hi(max_udp_frag_entry)
	xor	$2,$6,$4
	lw	$5,%lo(max_udp_frag_entry)($11)
	srl	$4,$4,16
	xor	$2,$2,$4
	xor	$4,$2,$12
	srl	$2,$2,8
	xor	$4,$4,$2
	addiu	$5,$5,-1
	lui	$2,%hi(negative_cache_table)
	and	$4,$4,$5
	lw	$5,%lo(negative_cache_table)($2)
	sll	$14,$4,4
	sll	$13,$4,2
	subu	$4,$14,$13
	addu	$4,$5,$4
	lw	$4,0($4)
	beq	$4,$0,$L175
	.set	noreorder
	.set	nomacro
	j	$L265
	li	$15,3			# 0x3
	.set	macro
	.set	reorder

$L176:
	lw	$4,40($4)
	beq	$4,$0,$L175
$L265:
	lhu	$5,2($4)
	bne	$5,$6,$L176
	lw	$5,4($4)
	bne	$7,$5,$L176
	lw	$5,8($4)
	bne	$10,$5,$L176
	lbu	$5,1($4)
	bne	$5,$12,$L176
	lbu	$5,0($4)
	bne	$5,$15,$L176
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L175:
	lui	$4,%hi(udp_cache_table)
	lw	$4,%lo(udp_cache_table)($4)
	subu	$13,$14,$13
	lw	$4,0($4)
	addu	$13,$4,$13
	lw	$22,0($13)
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L179
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L320
	lhu	$4,2($22)
	.set	macro
	.set	reorder

$L180:
	lw	$22,44($22)
	beq	$22,$0,$L179
	lhu	$4,2($22)
$L320:
	bne	$4,$6,$L180
	lw	$4,4($22)
	bne	$7,$4,$L180
	lw	$4,8($22)
	bne	$10,$4,$L180
	lbu	$4,1($22)
	bne	$4,$12,$L180
	lbu	$4,0($22)
	bne	$4,$5,$L180
	beq	$22,$0,$L179
	lhu	$20,12($22)
	lhu	$23,14($22)
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L184
	move	$11,$0
	.set	macro
	.set	reorder

	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L184
	li	$11,1			# 0x1
	.set	macro
	.set	reorder

$L282:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L169
	li	$4,8192			# 0x2000
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L169
	lui	$4,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

	lw	$5,%lo(statistic_udp_frag_rx)($4)
	addiu	$5,$5,1
	.set	noreorder
	.set	nomacro
	j	$L172
	sw	$5,%lo(statistic_udp_frag_rx)($4)
	.set	macro
	.set	reorder

$L188:
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

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$2
	.set	macro
	.set	reorder

$L315:
	.set	noreorder
	.set	nomacro
	j	$L173
	move	$8,$0
	.set	macro
	.set	reorder

$L169:
	lw	$4,12($21)
	andi	$6,$4,0x100
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L290
	move	$7,$23
	.set	macro
	.set	reorder

	andi	$4,$4,0x600
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L166
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L316
	move	$22,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L310
	lui	$4,%hi(statistic_udp_frag_rx)
	.set	macro
	.set	reorder

$L289:
	.set	noreorder
	.set	nomacro
	j	$L173
	li	$8,1			# 0x1
	.set	macro
	.set	reorder

$L290:
	move	$4,$fp
	li	$2,6			# 0x6
	move	$5,$20
	move	$6,$3
	.set	noreorder
	.set	nomacro
	jal	mark_path_invalid
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L179:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L185
	lui	$6,%hi(statistic_udp_frag_ps)
	.set	macro
	.set	reorder

	move	$22,$0
	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L184
	li	$11,1			# 0x1
	.set	macro
	.set	reorder

$L284:
	.set	noreorder
	.set	nomacro
	bne	$19,$5,$L189
	lui	$2,%hi(rtl_check_bypass_packet_hook)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L321
	lw	$2,%lo(rtl_check_bypass_packet_hook)($2)
	.set	macro
	.set	reorder

$L185:
	lw	$7,%lo(statistic_udp_frag_ps)($6)
	lui	$4,%hi(negative_cache_list_free)
	lw	$5,%lo(negative_cache_list_free)($4)
	addiu	$7,$7,1
	sw	$7,%lo(statistic_udp_frag_ps)($6)
	lbu	$6,9($17)
	lw	$8,%lo(max_udp_frag_entry)($11)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L164
	lhu	$7,4($17)
	.set	macro
	.set	reorder

	lw	$9,48($5)
	li	$10,3			# 0x3
	sb	$10,0($5)
	sh	$7,2($5)
	sw	$fp,4($5)
	sw	$3,8($5)
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L186
	sb	$6,1($5)
	.set	macro
	.set	reorder

	lw	$10,52($5)
	addiu	$4,$4,%lo(negative_cache_list_free)
	sw	$10,52($9)
$L187:
	lw	$9,52($5)
	lw	$10,48($5)
	xor	$3,$3,$fp
	xor	$7,$7,$3
	sw	$10,0($9)
	lw	$9,8($4)
	srl	$3,$3,16
	xor	$7,$7,$3
	xor	$6,$7,$6
	addiu	$3,$9,-1
	srl	$7,$7,8
	sw	$3,8($4)
	xor	$6,$6,$7
	sw	$0,40($5)
	addiu	$8,$8,-1
	lw	$4,%lo(negative_cache_table)($2)
	and	$8,$6,$8
	sll	$3,$8,4
	sll	$8,$8,2
	subu	$3,$3,$8
	addu	$4,$4,$3
	lw	$4,4($4)
	addiu	$7,$5,40
	sw	$4,44($5)
	lw	$6,%lo(negative_cache_table)($2)
	addiu	$4,$5,12
	addu	$6,$6,$3
	lw	$6,4($6)
	sw	$5,0($6)
	lw	$6,%lo(negative_cache_table)($2)
	addu	$6,$6,$3
	sw	$7,4($6)
	lw	$2,%lo(negative_cache_table)($2)
	addu	$3,$2,$3
	lw	$2,8($3)
	addiu	$2,$2,1
	sw	$2,8($3)
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	addiu	$2,$2,1000
	.set	noreorder
	.set	nomacro
	jal	add_timer
	sw	$2,20($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L186:
	lw	$9,52($5)
	addiu	$4,$4,%lo(negative_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L187
	sw	$9,4($4)
	.set	macro
	.set	reorder

$L286:
	.set	noreorder
	.set	nomacro
	jal	rtl_dst_release_ipv4
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L285:
	lw	$5,24($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_policy_route_mark_set
	move	$6,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L211:
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $3			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L291
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L235:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L275
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L292
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	lhu	$4,2($21)
	srl	$5,$3,16
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	lhu	$2,0($10)
	andi	$4,$4,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$5,$6,$5
	addu	$2,$2,$4
$L277:
	andi	$3,$3,0xffff
	lhu	$4,32($16)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L293
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L238:
	lhu	$2,32($16)
	sh	$2,2($21)
$L275:
	lw	$2,28($16)
	sw	$2,16($17)
$L207:
	lbu	$2,8($17)
	lbu	$4,9($17)
	lhu	$5,10($17)
	addiu	$3,$2,-1
	andi	$3,$3,0x00ff
	sll	$2,$2,8
	sll	$6,$3,8
	or	$6,$6,$4
	or	$4,$2,$4
	subu	$4,$4,$6
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $5			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L294
	sb	$3,8($17)
	.set	macro
	.set	reorder

	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L255:
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

	lhu	$5,74($16)
	li	$2,65535			# 0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L256
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lbu	$3,2($16)
	bne	$3,$2,$L256
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L256:
	lhu	$5,76($16)
	li	$2,65535			# 0xffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L257
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,2($16)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L317
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_vlanId
	move	$4,$18
	.set	macro
	.set	reorder

$L257:
	lui	$2,%hi(jiffies)
$L317:
	lw	$2,%lo(jiffies)($2)
	move	$4,$17
	move	$5,$21
	sw	$2,36($16)
	.set	noreorder
	.set	nomacro
	jal	fast_path_post_process_xmit_check
	move	$6,$18
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L166
	li	$4,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	ip_finish_output3
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$2,%hi(statistic_fp)
	lw	$3,%lo(statistic_fp)($2)
	li	$4,1			# 0x1
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L166
	sw	$3,%lo(statistic_fp)($2)
	.set	macro
	.set	reorder

$L294:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L255
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L206:
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $fp			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	bltz	$2,$L295
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$4,$4,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	sh	$4,10($17)
$L242:
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$2,16
	subu	$5,$0,$5
	andi	$2,$2,0xffff
	subu	$2,$5,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $3			
	rotr	$5, $5, 16		

 # 0 "" 2
 #NO_APP
	srl	$6,$5,16
	addu	$2,$2,$6
	andi	$5,$5,0xffff
	addu	$2,$2,$5
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bltz	$2,$L296
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L244:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L245
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L297
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$fp, $fp			
	rotr	$fp, $fp, 16		

 # 0 "" 2
 #NO_APP
	lhu	$4,0($21)
	srl	$5,$fp,16
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	lhu	$2,0($10)
	andi	$4,$4,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$5,$6,$5
	addu	$2,$2,$4
$L281:
	andi	$fp,$fp,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$fp,$5,$fp
	subu	$2,$2,$4
	addu	$2,$2,$fp
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
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	sh	$2,0($10)
$L252:
	lw	$4,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$2,$2,0xffff
	lhu	$6,2($21)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	addu	$2,$2,$6
	andi	$3,$3,0xffff
	lhu	$4,32($16)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L299
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L247:
	lhu	$2,24($16)
	sh	$2,0($21)
	lhu	$2,32($16)
	sh	$2,2($21)
$L245:
	lw	$2,20($16)
	.set	noreorder
	.set	nomacro
	j	$L275
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L296:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L244
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L295:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$4,$4,$2
	nor	$4,$0,$4
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	.set	noreorder
	.set	nomacro
	j	$L242
	sh	$4,10($17)
	.set	macro
	.set	reorder

$L299:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L247
	sh	$2,0($10)
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
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	j	$L252
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L297:
	beq	$10,$0,$L247
	lhu	$2,0($10)
	beq	$2,$0,$L247
	lw	$4,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$fp, $fp			
	rotr	$fp, $fp, 16		

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	srl	$5,$fp,16
	andi	$2,$2,0xffff
	lhu	$6,0($21)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	.set	noreorder
	.set	nomacro
	j	$L281
	addu	$2,$2,$6
	.set	macro
	.set	reorder

$L209:
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$fp, $fp			
	rotr	$fp, $fp, 16		

 # 0 "" 2
 #NO_APP
	srl	$3,$fp,16
	addu	$2,$2,$3
	andi	$fp,$fp,0xffff
	addu	$fp,$2,$fp
	lhu	$2,10($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$fp,$2
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
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L221:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L222
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L301
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$6,$2,16
	subu	$6,$0,$6
	andi	$2,$2,0xffff
	subu	$6,$6,$2
	lw	$3,12($17)
	lhu	$4,0($21)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$4,$4,0xffff
	lhu	$2,0($10)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$2,$4
	addu	$5,$6,$5
	andi	$3,$3,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L302
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L227:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L224
	li	$2,10240			# 0x2800
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L224
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,10240			# 0x2800
	.set	macro
	.set	reorder

$L224:
	lhu	$2,24($16)
	sh	$2,0($21)
$L222:
	lw	$2,20($16)
$L312:
	.set	noreorder
	.set	nomacro
	j	$L207
	sw	$2,12($17)
	.set	macro
	.set	reorder

$L302:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L227
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L301:
	beq	$10,$0,$L224
	lhu	$2,0($10)
	beq	$2,$0,$L224
	lw	$3,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	srl	$6,$3,16
	subu	$6,$0,$6
	andi	$3,$3,0xffff
	subu	$6,$6,$3
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	lw	$3,12($17)
	andi	$2,$2,0xffff
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	lhu	$4,0($21)
	srl	$5,$3,16
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	addu	$5,$6,$5
	andi	$3,$3,0xffff
	lhu	$4,24($16)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$3,$5,$3
	subu	$2,$2,$4
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L303
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L224
	sh	$2,0($10)
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
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L221
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L208:
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$fp, $fp			
	rotr	$fp, $fp, 16		

 # 0 "" 2
 #NO_APP
	srl	$3,$fp,16
	addu	$2,$2,$3
	andi	$fp,$fp,0xffff
	addu	$fp,$2,$fp
	lhu	$2,10($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$fp,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L304
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L214:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L222
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L305
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$3,$2,16
	subu	$3,$0,$3
	andi	$2,$2,0xffff
	subu	$2,$3,$2
	lw	$3,12($17)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$3,16
	addu	$2,$2,$4
	andi	$3,$3,0xffff
	addu	$3,$2,$3
	lhu	$2,0($10)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L306
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,0($10)
$L219:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L222
	li	$2,10240			# 0x2800
	.set	macro
	.set	reorder

	lhu	$3,2($17)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L222
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,10240			# 0x2800
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L312
	lw	$2,20($16)
	.set	macro
	.set	reorder

$L303:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L224
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L306:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L219
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L305:
	beq	$10,$0,$L222
	lhu	$3,0($10)
	beq	$3,$0,$L222
	lw	$2,20($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
	lw	$4,12($17)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$2,$2,$4
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bltz	$2,$L307
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L222
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L304:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L214
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L307:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L222
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L291:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L235
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L210:
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $3			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	addu	$2,$2,$5
	andi	$4,$4,0xffff
	addu	$4,$2,$4
	lhu	$2,10($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$4,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L308
	andi	$4,$2,0xffff
	.set	macro
	.set	reorder

	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,10($17)
$L229:
	.set	noreorder
	.set	nomacro
	beq	$11,$0,$L275
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	beq	$19,$2,$L309
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$2,16
	subu	$4,$0,$4
	andi	$2,$2,0xffff
	subu	$2,$4,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	srl	$4,$3,16
	addu	$2,$2,$4
	andi	$3,$3,0xffff
	addu	$3,$2,$3
	lhu	$2,0($10)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bltz	$2,$L272
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L318:
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L275
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L293:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L238
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L292:
	beq	$10,$0,$L238
	lhu	$2,0($10)
	beq	$2,$0,$L238
	lw	$4,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$4,16
	subu	$5,$0,$5
	andi	$4,$4,0xffff
	subu	$4,$5,$4
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	andi	$2,$2,0xffff
	lhu	$6,2($21)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	addu	$5,$4,$5
	.set	noreorder
	.set	nomacro
	j	$L277
	addu	$2,$2,$6
	.set	macro
	.set	reorder

$L309:
	beq	$10,$0,$L275
	lhu	$4,0($10)
	beq	$4,$0,$L275
	lw	$2,28($16)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$2,16
	subu	$5,$0,$5
	andi	$2,$2,0xffff
	subu	$2,$5,$2
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	srl	$5,$3,16
	addu	$2,$2,$5
	andi	$3,$3,0xffff
	addu	$2,$2,$3
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			

 # 0 "" 2
 #NO_APP
	andi	$4,$4,0xffff
	addu	$2,$2,$4
	.set	noreorder
	.set	nomacro
	bgez	$2,$L318
	andi	$3,$2,0xffff
	.set	macro
	.set	reorder

$L272:
	subu	$2,$0,$2
	andi	$3,$2,0xffff
	sra	$2,$2,16
	addu	$2,$3,$2
	sra	$3,$2,16
	addu	$2,$3,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L275
	sh	$2,0($10)
	.set	macro
	.set	reorder

$L308:
	subu	$2,$0,$2
	andi	$4,$2,0xffff
	sra	$2,$2,16
	addu	$2,$4,$2
	sra	$4,$2,16
	addu	$2,$4,$2
	nor	$2,$0,$2
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L229
	sh	$2,10($17)
	.set	macro
	.set	reorder

$L288:
	lw	$2,12($21)
	andi	$2,$2,0x1000
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L205
	li	$2,40			# 0x28
	.set	macro
	.set	reorder

	lhu	$4,2($17)
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L205
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_trim
	li	$5,40			# 0x28
	.set	macro
	.set	reorder

	lw	$11,44($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	j	$L205
	lw	$3,48($sp)
	.set	macro
	.set	reorder

$L200:
	lw	$6,56($2)
	addiu	$4,$4,%lo(udp_cache_list_free)
	.set	noreorder
	.set	nomacro
	j	$L201
	sw	$6,4($4)
	.set	macro
	.set	reorder

$L287:
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

	.set	noreorder
	.set	nomacro
	j	$L166
	move	$4,$0
	.set	macro
	.set	reorder

$L198:
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L319
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$4,$22,16
	sw	$3,48($sp)
	sw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	jal	del_timer
	sw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$2,4($22)
	lw	$4,8($22)
	lhu	$5,2($22)
	lbu	$6,1($22)
	xor	$4,$4,$2
	lui	$2,%hi(max_udp_frag_entry)
	xor	$5,$5,$4
	lw	$2,%lo(max_udp_frag_entry)($2)
	srl	$4,$4,16
	xor	$5,$5,$4
	lw	$4,44($22)
	xor	$6,$5,$6
	srl	$5,$5,8
	sb	$0,0($22)
	xor	$5,$6,$5
	addiu	$2,$2,-1
	and	$2,$5,$2
	lw	$3,48($sp)
	lw	$10,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L203
	lw	$11,44($sp)
	.set	macro
	.set	reorder

	lw	$6,48($22)
	sll	$5,$2,4
	sll	$2,$2,2
	sw	$6,48($4)
	subu	$5,$5,$2
	lui	$4,%hi(udp_cache_table)
$L204:
	lw	$6,48($22)
	lw	$7,44($22)
	lui	$2,%hi(udp_cache_list_free)
	addiu	$2,$2,%lo(udp_cache_list_free)
	sw	$7,0($6)
	lw	$6,%lo(udp_cache_table)($4)
	lui	$4,%hi(statistic_udp_frag_free)
	addiu	$7,$22,52
	lw	$6,0($6)
	addu	$5,$6,$5
	lw	$6,8($5)
	addiu	$6,$6,-1
	sw	$6,8($5)
	sw	$0,52($22)
	lw	$5,4($2)
	sw	$5,56($22)
	lw	$5,4($2)
	sw	$22,0($5)
	lw	$6,8($2)
	lw	$5,%lo(statistic_udp_frag_free)($4)
	sw	$7,4($2)
	addiu	$6,$6,1
	addiu	$5,$5,1
	sw	$6,8($2)
	.set	noreorder
	.set	nomacro
	j	$L202
	sw	$5,%lo(statistic_udp_frag_free)($4)
	.set	macro
	.set	reorder

$L203:
	lui	$4,%hi(udp_cache_table)
	lw	$6,%lo(udp_cache_table)($4)
	sll	$7,$2,4
	sll	$5,$2,2
	lw	$2,0($6)
	lw	$6,48($22)
	subu	$5,$7,$5
	addu	$2,$2,$5
	.set	noreorder
	.set	nomacro
	j	$L204
	sw	$6,4($2)
	.set	macro
	.set	reorder

	.end	enter_fast_path
	.size	enter_fast_path, .-enter_fast_path
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
	beq	$2,$3,$L325
	li	$3,450			# 0x1c2

	beq	$2,$3,$L326
	li	$3,150			# 0x96

	beq	$2,$3,$L324
	lui	$18,%hi(jiffies)

$L323:
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

$L324:
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-30000

	j	$L323
	nop

$L326:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	jal	rtl_set_fdb_aging
	move	$4,$16

	j	$L323
	nop

$L325:
	lui	$18,%hi(jiffies)
	lw	$5,%lo(jiffies)($18)
	move	$4,$16
	jal	rtl_set_fdb_aging
	addiu	$5,$5,-15000

	j	$L323
	nop

	.set	macro
	.set	reorder
	.end	rtl_br_fdb_time_update
	.size	rtl_br_fdb_time_update, .-rtl_br_fdb_time_update
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
	beq	$2,$3,$L343
	nop

	move	$4,$16
	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L344
	nop

$L334:
	li	$2,-1			# 0xffffffffffffffff
$L337:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

$L343:
	jal	rtl_get_ct_udp_status
	move	$4,$16

	andi	$2,$2,0x2
	beq	$2,$0,$L345
	li	$4,1			# 0x1

$L339:
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	move	$19,$2
	li	$20,1			# 0x1
$L333:
	move	$5,$0
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	beq	$18,$2,$L346
	move	$5,$0

	li	$6,1			# 0x1
	jal	rtl_get_ct_ip_by_dir
	move	$4,$16

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_get_ct_ip_by_dir
	move	$18,$2

	bne	$18,$2,$L334
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
	j	$L342
	li	$5,1			# 0x1

$L345:
	j	$L339
	move	$4,$0

$L346:
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
$L342:
	move	$6,$0
	jal	rtl_get_ct_port_by_dir
	sw	$2,40($sp)

	addiu	$4,$sp,20
	move	$5,$19
	addiu	$6,$sp,16
	jal	rtk_idleNaptConnection
	sw	$2,44($sp)

	beq	$2,$0,$L334
	move	$4,$16

	lw	$5,16($sp)
	jal	rtl_set_ct_timeout_expires
	addu	$5,$19,$5

	addu	$5,$19,$17
	jal	rtl_check_for_acc
	move	$4,$16

	j	$L337
	move	$2,$0

$L344:
	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L337
	li	$2,-1			# 0xffffffffffffffff

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	move	$19,$2
	j	$L333
	li	$20,2			# 0x2

	.set	macro
	.set	reorder
	.end	rtl_fpTimer_update
	.size	rtl_fpTimer_update, .-rtl_fpTimer_update
	.align	2
	.globl	rtk_delNaptConnection
	.set	nomips16
	.ent	rtk_delNaptConnection
	.type	rtk_delNaptConnection, @function
rtk_delNaptConnection:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lw	$3,0($4)
	li	$2,6			# 0x6
	li	$18,17			# 0x11
	xori	$3,$3,0x2
	move	$16,$4
	movz	$18,$2,$3
 #APP
 # 39 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push						
	.set	reorder						
	.set	noat						
	di	$17					
	andi	$17, 1					
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	filter_delconnect
	lw	$4,20($4)
	.set	macro
	.set	reorder

	lw	$5,4($16)
	lw	$6,8($16)
	lw	$7,20($16)
	lw	$10,24($16)
	xor	$3,$6,$5
	xor	$3,$3,$7
	srl	$2,$5,16
	lw	$4,0($16)
	xor	$3,$3,$10
	xor	$3,$3,$2
	srl	$2,$7,16
	xor	$3,$3,$2
	lui	$2,%hi(napt_table_list_max)
	lw	$11,%lo(napt_table_list_max)($2)
	lui	$9,%hi(table_napt)
	andi	$2,$4,0xffff
	xor	$3,$3,$2
	lw	$8,%lo(table_napt)($9)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$11,$11,-1
	and	$2,$3,$11
	lw	$3,0($8)
	sll	$8,$2,4
	sll	$2,$2,2
	subu	$8,$8,$2
	addu	$2,$3,$8
	lw	$2,0($2)
	bne	$2,$0,$L377
	j	$L350
$L351:
	lw	$2,36($2)
	beq	$2,$0,$L350
$L377:
	lw	$3,4($2)
	bne	$4,$3,$L351
	lw	$3,8($2)
	bne	$5,$3,$L351
	lw	$3,12($2)
	bne	$6,$3,$L351
	lw	$11,16($2)
	lw	$3,12($16)
	bne	$11,$3,$L351
	lw	$11,20($2)
	lw	$3,16($16)
	bne	$11,$3,$L351
	lw	$3,24($2)
	bne	$7,$3,$L351
	lw	$3,28($2)
	bne	$10,$3,$L351
	lw	$3,36($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L352
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$4,40($2)
	sw	$4,40($3)
$L353:
	lw	$3,40($2)
	lw	$4,36($2)
	sw	$4,0($3)
	lw	$3,%lo(table_napt)($9)
	lw	$3,0($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,-1
	sw	$3,8($8)
	lw	$3,44($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L354
	lui	$4,%hi(napt_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$5,48($3)
$L355:
	lw	$5,48($2)
	lw	$6,44($2)
	lui	$3,%hi(napt_list_free)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$6,0($5)
	lw	$6,8($4)
	addiu	$5,$2,44
	lui	$12,%hi(path_table_list_max)
	addiu	$6,$6,-1
	sw	$6,8($4)
	sw	$0,44($2)
	lw	$6,4($3)
	lui	$4,%hi(table_path)
	li	$14,1			# 0x1
	sw	$6,48($2)
	lw	$6,4($3)
	sw	$2,0($6)
	lw	$2,8($3)
	sw	$5,4($3)
	lw	$8,%lo(path_table_list_max)($12)
	addiu	$2,$2,1
	sw	$2,8($3)
	lw	$11,4($16)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$13,8($16)
	xor	$6,$3,$11
	srl	$2,$11,8
	xor	$6,$6,$5
	xor	$6,$6,$2
	srl	$2,$3,16
	xor	$6,$6,$2
	srl	$2,$13,4
	xor	$2,$6,$2
	xor	$2,$2,$18
	lw	$6,%lo(table_path)($4)
	srl	$7,$2,12
	xor	$2,$7,$2
	addiu	$8,$8,-1
	and	$2,$2,$8
	lw	$9,0($6)
	sll	$10,$2,4
	sll	$2,$2,2
	subu	$10,$10,$2
	addu	$2,$9,$10
	lw	$2,0($2)
	lw	$6,12($16)
	lw	$7,16($16)
$L356:
	beq	$2,$0,$L362
	lw	$24,4($2)
	lw	$15,0($16)
	beq	$24,$15,$L379
$L357:
	.set	noreorder
	.set	nomacro
	j	$L356
	lw	$2,80($2)
	.set	macro
	.set	reorder

$L350:
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$17, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	li	$2,-4			# 0xfffffffffffffffc
$L373:
	lw	$31,28($sp)
$L381:
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L379:
	lbu	$15,2($2)
	bne	$15,$14,$L357
	lw	$15,12($2)
	bne	$11,$15,$L357
	lhu	$15,10($2)
	bne	$13,$15,$L357
	lw	$15,20($2)
	bne	$15,$6,$L357
	lhu	$15,24($2)
	bne	$15,$7,$L357
	lw	$15,16($2)
	bne	$3,$15,$L357
	lhu	$15,8($2)
	bne	$5,$15,$L357
	lw	$3,80($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L358
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$5,84($2)
	sw	$5,84($3)
$L359:
	lw	$3,84($2)
	lw	$5,80($2)
	sw	$5,0($3)
	lw	$3,%lo(table_path)($4)
	lw	$3,0($3)
	addu	$10,$3,$10
	lw	$3,8($10)
	addiu	$3,$3,-1
	sw	$3,8($10)
	lw	$5,88($2)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L360
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$6,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$6,92($5)
$L361:
	lw	$6,92($2)
	lw	$7,88($2)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$7,0($6)
	lw	$7,8($3)
	addiu	$6,$2,88
	addiu	$7,$7,-1
	sw	$7,8($3)
	sw	$0,88($2)
	lw	$3,4($5)
	sw	$3,92($2)
	lw	$3,4($5)
	sw	$2,0($3)
	lw	$3,8($5)
	lw	$2,%lo(table_path)($4)
	sw	$6,4($5)
	addiu	$3,$3,1
	sw	$3,8($5)
	lw	$8,%lo(path_table_list_max)($12)
	lw	$9,0($2)
	lw	$3,20($16)
	lw	$5,24($16)
	lw	$6,12($16)
	lw	$7,16($16)
	addiu	$8,$8,-1
$L362:
	xor	$10,$3,$6
	xor	$10,$10,$18
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
$L364:
	beq	$2,$0,$L370
	lw	$11,4($2)
	lw	$10,0($16)
	beq	$11,$10,$L380
$L365:
	.set	noreorder
	.set	nomacro
	j	$L364
	lw	$2,80($2)
	.set	macro
	.set	reorder

$L380:
	lbu	$10,2($2)
	bne	$10,$9,$L365
	lw	$10,16($2)
	bne	$10,$6,$L365
	lhu	$10,8($2)
	bne	$10,$7,$L365
	lw	$10,20($2)
	bne	$10,$3,$L365
	lhu	$10,24($2)
	bne	$10,$5,$L365
	lw	$11,28($2)
	lw	$10,4($16)
	bne	$11,$10,$L365
	lhu	$11,32($2)
	lw	$10,8($16)
	bne	$11,$10,$L365
	lw	$3,80($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L366
	sb	$0,0($2)
	.set	macro
	.set	reorder

	lw	$5,84($2)
	sw	$5,84($3)
$L367:
	lw	$3,84($2)
	lw	$5,80($2)
	sw	$5,0($3)
	lw	$3,%lo(table_path)($4)
	lw	$3,0($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,-1
	sw	$3,8($8)
	lw	$4,88($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L368
	lui	$3,%hi(path_list_inuse)
	.set	macro
	.set	reorder

	lw	$5,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	sw	$5,92($4)
$L369:
	lw	$5,92($2)
	lw	$6,88($2)
	lui	$4,%hi(path_list_free)
	addiu	$4,$4,%lo(path_list_free)
	sw	$6,0($5)
	lw	$6,8($3)
	addiu	$5,$2,88
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,88($2)
	lw	$3,4($4)
	sw	$3,92($2)
	lw	$3,4($4)
	sw	$2,0($3)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	sw	$2,8($4)
$L370:
	andi	$2,$17,0x1
	bne	$2,$0,$L372
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$17, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L381
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L368:
	lw	$4,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L369
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L366:
	lw	$3,%lo(table_path)($4)
	lw	$5,84($2)
	lw	$3,0($3)
	addu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L367
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L372:
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push						
	.set	noreorder					
	.set	noat						
	beqz	$17, 1f					
	di							
	ei							
1:								
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L373
	move	$2,$0
	.set	macro
	.set	reorder

$L360:
	lw	$5,92($2)
	addiu	$3,$3,%lo(path_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L361
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L358:
	lw	$3,%lo(table_path)($4)
	lw	$5,84($2)
	lw	$3,0($3)
	addu	$3,$3,$10
	.set	noreorder
	.set	nomacro
	j	$L359
	sw	$5,4($3)
	.set	macro
	.set	reorder

$L354:
	lw	$3,48($2)
	addiu	$4,$4,%lo(napt_list_inuse)
	.set	noreorder
	.set	nomacro
	j	$L355
	sw	$3,4($4)
	.set	macro
	.set	reorder

$L352:
	lw	$3,%lo(table_napt)($9)
	lw	$4,40($2)
	lw	$3,0($3)
	addu	$3,$3,$8
	.set	noreorder
	.set	nomacro
	j	$L353
	sw	$4,4($3)
	.set	macro
	.set	reorder

	.end	rtk_delNaptConnection
	.size	rtk_delNaptConnection, .-rtk_delNaptConnection
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
	bne	$5,$0,$L455
	move	$19,$6
	.set	macro
	.set	reorder

$L383:
	li	$2,-1			# 0xffffffffffffffff
$L389:
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

$L455:
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	sw	$7,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L383
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lw	$3,0($16)
	lw	$4,20($16)
	xori	$3,$3,0x2
	li	$23,17			# 0x11
	.set	noreorder
	.set	nomacro
	jal	filter_addconnect
	movz	$23,$2,$3
	.set	macro
	.set	reorder

 #APP
 # 39 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
		.set	push						
	.set	reorder						
	.set	noat						
	di	$2					
	andi	$2, 1					
	sll $0, $0, 3			
	.set	pop						

 # 0 "" 2
 #NO_APP
	sw	$2,76($sp)
	lw	$5,4($16)
	lw	$9,8($16)
	lw	$10,20($16)
	lw	$11,24($16)
	xor	$3,$9,$5
	xor	$3,$3,$10
	srl	$2,$5,16
	lw	$4,0($16)
	xor	$3,$3,$11
	xor	$3,$3,$2
	srl	$2,$10,16
	xor	$3,$3,$2
	lui	$2,%hi(napt_table_list_max)
	lw	$6,%lo(napt_table_list_max)($2)
	lui	$8,%hi(table_napt)
	andi	$2,$4,0xffff
	xor	$3,$3,$2
	lw	$12,%lo(table_napt)($8)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$6,$6,-1
	and	$2,$3,$6
	lw	$3,0($12)
	sll	$6,$2,4
	sll	$2,$2,2
	subu	$6,$6,$2
	addu	$2,$3,$6
	lw	$2,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L451
	lw	$7,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L474
	lui	$3,%hi(napt_list_free)
	.set	macro
	.set	reorder

$L387:
	lw	$2,36($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L386
	lui	$3,%hi(napt_list_free)
	.set	macro
	.set	reorder

$L451:
	lw	$3,4($2)
	bne	$4,$3,$L387
	lw	$3,8($2)
	bne	$5,$3,$L387
	lw	$3,12($2)
	bne	$9,$3,$L387
	lw	$12,16($2)
	lw	$3,12($16)
	bne	$12,$3,$L387
	lw	$12,20($2)
	lw	$3,16($16)
	bne	$12,$3,$L387
	lw	$3,24($2)
	bne	$10,$3,$L387
	lw	$3,28($2)
	bne	$11,$3,$L387
	move	$4,$19
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	lw	$3,76($sp)
	andi	$2,$3,0x1
	bne	$2,$0,$L388
	move	$2,$3
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	j	$L389
	move	$2,$0
	.set	macro
	.set	reorder

$L386:
$L474:
	lw	$2,%lo(napt_list_free)($3)
	beq	$2,$0,$L391
	sw	$4,4($2)
	lw	$5,4($16)
	lw	$4,44($2)
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
	li	$7,-1			# 0xffffffffffffffff
	sb	$7,0($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L392
	sw	$5,28($2)
	.set	macro
	.set	reorder

	lw	$5,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	sw	$5,48($4)
$L393:
	lw	$5,48($2)
	lw	$7,44($2)
	lui	$4,%hi(napt_list_inuse)
	addiu	$4,$4,%lo(napt_list_inuse)
	sw	$7,0($5)
	lw	$9,8($3)
	addiu	$7,$2,44
	addiu	$5,$2,36
	addiu	$9,$9,-1
	sw	$9,8($3)
	sw	$0,44($2)
	lw	$3,4($4)
	sw	$3,48($2)
	lw	$3,4($4)
	sw	$2,0($3)
	lw	$3,8($4)
	sw	$7,4($4)
	addiu	$3,$3,1
	sw	$3,8($4)
	sw	$0,36($2)
	lw	$3,%lo(table_napt)($8)
	lw	$3,0($3)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$3,40($2)
	lw	$3,%lo(table_napt)($8)
	lw	$3,0($3)
	addu	$3,$3,$6
	lw	$3,4($3)
	sw	$2,0($3)
	lw	$2,%lo(table_napt)($8)
	lw	$2,0($2)
	addu	$2,$2,$6
	sw	$5,4($2)
	lw	$2,%lo(table_napt)($8)
	lw	$2,0($2)
	addu	$6,$2,$6
	lw	$2,8($6)
	addiu	$2,$2,1
	sw	$2,8($6)
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
	sw	$2,60($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_protocol
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	move	$4,$17
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,124($sp)
	.set	macro
	.set	reorder

	lw	$21,0($2)
	move	$4,$17
	move	$20,$2
	andi	$21,$21,0xf
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
	sw	$4,88($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,88($sp)
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
	sw	$2,96($sp)
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
	sw	$2,92($sp)
	.set	macro
	.set	reorder

	sw	$2,100($sp)
	lw	$6,12($20)
	move	$4,$17
	sw	$6,116($sp)
	lhu	$2,0($21)
	sw	$2,108($sp)
	lw	$3,16($20)
	sw	$3,112($sp)
	lhu	$5,2($21)
	.set	noreorder
	.set	nomacro
	jal	rtl_store_skb_dst
	sw	$5,104($sp)
	.set	macro
	.set	reorder

	lui	$6,%hi(path_table_list_max)
	lw	$2,%lo(path_table_list_max)($6)
	lw	$fp,4($16)
	lui	$3,%hi(path_list_free)
	addiu	$2,$2,-1
	sw	$2,84($sp)
	lw	$4,8($16)
	lw	$18,%lo(path_list_free)($3)
	sw	$4,80($sp)
	lw	$5,20($16)
	sw	$5,64($sp)
	lw	$6,24($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L422
	sw	$6,72($sp)
	.set	macro
	.set	reorder

	lw	$2,0($16)
	sw	$2,4($18)
	lw	$9,4($16)
	sw	$9,12($18)
	lw	$2,8($16)
	sh	$2,10($18)
	lw	$2,20($16)
	sw	$2,16($18)
	lw	$2,24($16)
	sh	$2,8($18)
	lw	$8,12($16)
	sw	$8,20($18)
	lw	$2,16($16)
	sh	$2,24($18)
	lw	$7,20($16)
	sw	$7,28($18)
	lw	$2,24($16)
	sh	$2,32($18)
	lui	$2,%hi(route_list_inuse)
	lw	$3,%lo(route_list_inuse)($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L398
	move	$10,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L399:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L396
	and	$6,$2,$7
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$6,$5,$L456
$L396:
	move	$2,$4
$L397:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L399
	move	$4,$2
	.set	macro
	.set	reorder

$L398:
	li	$2,1			# 0x1
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$10,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$3,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($3)
	beq	$2,$0,$L400
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$9,12($18)
	lw	$8,20($18)
$L400:
	beq	$9,$8,$L401
	lbu	$2,1($18)
	ori	$2,$2,0x1
	sb	$2,1($18)
$L401:
	lhu	$3,10($18)
	lhu	$2,24($18)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L479
	li	$2,255			# 0xff
	.set	macro
	.set	reorder

	lbu	$2,1($18)
	ori	$2,$2,0x2
	sb	$2,1($18)
	li	$2,255			# 0xff
$L479:
	sh	$2,78($18)
	sw	$0,44($18)
	sw	$0,48($18)
	sw	$0,60($18)
	sb	$0,72($18)
	sb	$0,73($18)
	lw	$2,4($16)
	move	$5,$0
	addiu	$4,$sp,36
	sw	$2,12($20)
	lw	$2,8($16)
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
	beq	$4,$0,$L403
	lw	$4,4($16)
	lw	$5,60($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_arp_req_get_ha
	addiu	$6,$sp,36
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L403
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$22,$2
	li	$2,1			# 0x1
	beq	$22,$2,$L457
$L404:
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

	addiu	$5,$sp,36
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_src_phy_port
	move	$6,$0
	.set	macro
	.set	reorder

	lw	$5,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L403
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L405
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	move	$5,$0
	.set	macro
	.set	reorder

$L405:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L458
	lw	$2,44($18)
	sw	$2,44($18)
$L478:
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
	beq	$2,$3,$L408
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ipt_do_table
	li	$5,4			# 0x4
	.set	macro
	.set	reorder

$L408:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L459
	lw	$2,48($18)
	sw	$2,48($18)
$L403:
	lw	$2,12($16)
	li	$6,14			# 0xe
	move	$5,$0
	sw	$2,12($20)
	lw	$2,16($16)
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

	lw	$6,56($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L411
	move	$5,$6
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
	bne	$2,$0,$L480
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

	move	$22,$2
	li	$2,1			# 0x1
	beq	$22,$2,$L460
$L412:
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

	lw	$5,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_smac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,124($sp)
	li	$2,2048			# 0x800
	bne	$3,$2,$L411
	lw	$5,44($18)
	bne	$5,$0,$L461
	lui	$2,%hi(nf_hooks+160)
$L476:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L414
	lw	$6,60($sp)
	.set	macro
	.set	reorder

	lw	$7,56($sp)
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

	bne	$2,$0,$L462
	lw	$2,48($18)
	sw	$2,48($18)
$L411:
	move	$4,$18
$L480:
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

	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$23,$2,$L463
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
$L418:
	sw	$19,52($18)
	lui	$4,%hi(jiffies)
	lw	$3,%lo(jiffies)($4)
	lw	$2,88($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L419
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$3,92($18)
	lui	$4,%hi(path_list_free)
	addiu	$4,$4,%lo(path_list_free)
	sw	$4,68($sp)
	sw	$3,92($2)
$L420:
	lw	$2,92($18)
	lw	$3,88($18)
	lui	$22,%hi(path_list_inuse)
	addiu	$22,$22,%lo(path_list_inuse)
	sw	$3,0($2)
	lw	$6,68($sp)
	lw	$4,64($sp)
	lw	$5,72($sp)
	lw	$2,8($6)
	xor	$3,$4,$fp
	xor	$3,$3,$5
	addiu	$2,$2,-1
	sw	$2,8($6)
	sw	$0,88($18)
	lw	$2,4($22)
	srl	$fp,$fp,8
	xor	$3,$3,$fp
	sw	$2,92($18)
	lw	$4,4($22)
	lw	$6,64($sp)
	lui	$fp,%hi(table_path)
	sw	$18,0($4)
	lw	$4,8($22)
	lw	$5,80($sp)
	srl	$2,$6,16
	xor	$3,$3,$2
	addiu	$4,$4,1
	srl	$2,$5,4
	addiu	$5,$18,88
	sw	$5,4($22)
	sw	$4,8($22)
	xor	$2,$3,$2
	sw	$0,80($18)
	lw	$6,84($sp)
	lw	$3,%lo(table_path)($fp)
	xor	$2,$2,$23
	srl	$4,$2,12
	xor	$4,$4,$2
	lw	$3,0($3)
	and	$4,$4,$6
	sll	$5,$4,4
	sll	$2,$4,2
	subu	$2,$5,$2
	addu	$3,$3,$2
	lw	$4,4($3)
	addiu	$3,$18,80
	sw	$4,84($18)
	lw	$4,%lo(table_path)($fp)
	lw	$4,0($4)
	addu	$4,$4,$2
	lw	$4,4($4)
	sw	$18,0($4)
	lw	$4,%lo(table_path)($fp)
	lw	$4,0($4)
	addu	$4,$4,$2
	sw	$3,4($4)
	lw	$3,%lo(table_path)($fp)
	lui	$4,%hi(path_list_free)
	lw	$3,0($3)
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
	sw	$2,84($sp)
	lw	$6,24($16)
	sw	$6,120($sp)
	lw	$2,12($16)
	sw	$2,64($sp)
	lw	$3,16($16)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L422
	sw	$3,80($sp)
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
	beq	$3,$0,$L426
	move	$8,$0
	.set	macro
	.set	reorder

	move	$4,$0
$L427:
	lw	$2,8($3)
	sltu	$5,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L424
	and	$9,$2,$6
	.set	macro
	.set	reorder

	lw	$5,4($3)
	beq	$9,$5,$L464
$L424:
	move	$2,$4
$L425:
	lw	$3,44($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L427
	move	$4,$2
	.set	macro
	.set	reorder

$L426:
	li	$2,2			# 0x2
	sb	$2,2($18)
	li	$2,-1			# 0xffffffffffffffff
	sw	$8,40($18)
	sb	$2,0($18)
	sb	$0,1($18)
	sw	$19,52($18)
	lui	$5,%hi(rtl_init_bypass_packet_hook)
	lw	$2,%lo(rtl_init_bypass_packet_hook)($5)
	beq	$2,$0,$L428
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$7,16($18)
	lw	$6,28($18)
$L428:
	beq	$7,$6,$L429
	lbu	$2,1($18)
	ori	$2,$2,0x4
	sb	$2,1($18)
$L429:
	lhu	$3,8($18)
	lhu	$2,32($18)
	beq	$3,$2,$L430
	lbu	$2,1($18)
	ori	$2,$2,0x8
	sb	$2,1($18)
$L430:
	lbu	$3,1($20)
	move	$4,$17
	andi	$3,$3,0x3
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_orig_dscp
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	lw	$3,128($sp)
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

	lw	$6,56($sp)
	beq	$6,$0,$L432
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
	beq	$2,$0,$L465
	move	$4,$17
	.set	macro
	.set	reorder

$L432:
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

	lw	$4,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L438
	lw	$5,60($sp)
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
	beq	$2,$0,$L466
	move	$4,$17
	.set	macro
	.set	reorder

$L438:
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

	li	$2,6			# 0x6
	beq	$23,$2,$L467
	li	$4,1			# 0x1
	move	$5,$19
	.set	noreorder
	.set	nomacro
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	sw	$2,56($18)
$L442:
	sw	$19,52($18)
	lui	$6,%hi(jiffies)
	lw	$3,%lo(jiffies)($6)
	lw	$2,88($18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L443
	sw	$3,36($18)
	.set	macro
	.set	reorder

	lw	$3,92($18)
	sw	$3,92($2)
$L444:
	lw	$2,92($18)
	lw	$3,88($18)
	lw	$6,64($sp)
	lw	$5,80($sp)
	sw	$3,0($2)
	lw	$2,68($sp)
	xor	$4,$5,$6
	srl	$5,$6,16
	lw	$3,8($2)
	xor	$2,$4,$5
	lw	$4,68($sp)
	addiu	$3,$3,-1
	xor	$2,$2,$23
	sw	$3,8($4)
	sw	$0,88($18)
	lw	$3,4($22)
	lw	$5,72($sp)
	sw	$3,92($18)
	lw	$6,4($22)
	xor	$2,$5,$2
	srl	$7,$5,8
	sw	$18,0($6)
	lw	$4,8($22)
	lw	$6,120($sp)
	addiu	$3,$18,88
	addiu	$4,$4,1
	sw	$3,4($22)
	sw	$4,8($22)
	srl	$3,$6,4
	sw	$0,80($18)
	xor	$2,$2,$7
	lw	$4,%lo(table_path)($fp)
	lw	$5,84($sp)
	xor	$2,$2,$3
	srl	$3,$2,12
	xor	$2,$3,$2
	lw	$4,0($4)
	and	$3,$5,$2
	sll	$2,$3,4
	sll	$3,$3,2
	subu	$2,$2,$3
	addu	$3,$4,$2
	lw	$3,4($3)
	addiu	$4,$18,80
	sw	$3,84($18)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$3,$3,$2
	lw	$3,4($3)
	sw	$18,0($3)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$4,4($3)
	lw	$3,%lo(table_path)($fp)
	lw	$3,0($3)
	addu	$2,$3,$2
	lw	$3,8($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L422:
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

	lw	$5,100($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,116($sp)
	sw	$6,12($20)
	lw	$2,108($sp)
	sh	$2,0($21)
	lw	$3,112($sp)
	sw	$3,16($20)
	lw	$4,104($sp)
	sh	$4,2($21)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dst
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,92($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_phy_port
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,88($sp)
	move	$4,$17
	sb	$5,1($20)
	.set	noreorder
	.set	nomacro
	jal	rtl_restore_skb_vlanpri
	lw	$5,96($sp)
	.set	macro
	.set	reorder

	lw	$6,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L481
	lw	$2,56($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$6
	.set	macro
	.set	reorder

	lw	$2,56($sp)
$L481:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L482
	lw	$3,76($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_dev_put
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$3,76($sp)
$L482:
	andi	$2,$3,0x1
	beq	$2,$0,$L468
	lw	$2,76($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	move	$4,$19
$L475:
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_conntrack_drop_check_hook
	move	$6,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L389
	move	$2,$0
	.set	macro
	.set	reorder

$L456:
	.set	noreorder
	.set	nomacro
	j	$L397
	addiu	$10,$3,16
	.set	macro
	.set	reorder

$L464:
	.set	noreorder
	.set	nomacro
	j	$L425
	addiu	$8,$3,16
	.set	macro
	.set	reorder

$L468:
	move	$2,$3
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	j	$L475
	move	$4,$19
	.set	macro
	.set	reorder

$L463:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L418
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L467:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_get_timeouts
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L442
	sw	$2,56($18)
	.set	macro
	.set	reorder

$L391:
	lw	$4,76($sp)
	andi	$2,$4,0x1
	bne	$2,$0,$L449
	move	$2,$4
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	j	$L389
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L465:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$3,$2
	li	$2,1			# 0x1
	beq	$3,$2,$L469
$L434:
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

	lw	$5,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dmac
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,92($sp)
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

	lw	$3,124($sp)
	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L470
	lui	$2,%hi(nf_hooks+128)
	.set	macro
	.set	reorder

$L435:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$0,$L471
	lw	$2,44($18)
	.set	noreorder
	.set	nomacro
	j	$L432
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L466:
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_proto_check
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	move	$16,$2
	li	$2,1			# 0x1
	beq	$16,$2,$L472
$L439:
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

	lw	$5,124($sp)
	li	$2,2048			# 0x800
	bne	$5,$2,$L438
	lw	$5,44($18)
	bne	$5,$0,$L473
	lui	$2,%hi(nf_hooks+160)
$L477:
	lw	$3,%lo(nf_hooks+160)($2)
	addiu	$2,$2,%lo(nf_hooks+160)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L438
	lw	$6,56($sp)
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

	j	$L438
$L392:
	lw	$4,48($2)
	addiu	$3,$3,%lo(napt_list_free)
	.set	noreorder
	.set	nomacro
	j	$L393
	sw	$4,4($3)
	.set	macro
	.set	reorder

$L443:
	lw	$2,92($18)
	lw	$3,68($sp)
	.set	noreorder
	.set	nomacro
	j	$L444
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L419:
	lw	$2,92($18)
	lui	$5,%hi(path_list_free)
	addiu	$5,$5,%lo(path_list_free)
	sw	$5,68($sp)
	.set	noreorder
	.set	nomacro
	j	$L420
	sw	$2,4($5)
	.set	macro
	.set	reorder

$L449:
	lw	$2,76($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	j	$L389
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L471:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L432
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L470:
	lw	$3,%lo(nf_hooks+128)($2)
	addiu	$2,$2,%lo(nf_hooks+128)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L435
	lw	$6,56($sp)
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

	j	$L435
$L469:
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	lw	$3,128($sp)
	bne	$2,$3,$L434
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L434
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L434
$L457:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$22,$L404
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L404
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L404
$L460:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$22,$L412
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L412
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L412
$L472:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_mac_header_was_set
	move	$4,$17
	.set	macro
	.set	reorder

	bne	$2,$16,$L439
	.set	noreorder
	.set	nomacro
	jal	rtl_eth_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,13($2)
	lbu	$4,12($2)
	sll	$2,$3,8
	or	$4,$2,$4
	li	$2,8			# 0x8
	beq	$4,$2,$L439
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_set_mac_header
	li	$5,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	j	$L439
$L461:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L476
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

$L473:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L477
	lui	$2,%hi(nf_hooks+160)
	.set	macro
	.set	reorder

$L458:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L478
	sw	$2,44($18)
	.set	macro
	.set	reorder

$L462:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L411
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L459:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_mark
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L403
	sw	$2,48($18)
	.set	macro
	.set	reorder

$L388:
	lw	$2,76($sp)
 #APP
 # 58 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/irqflags.h" 1
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
	j	$L389
	move	$2,$0
	.set	macro
	.set	reorder

	.end	rtk_addNaptConnection
	.size	rtk_addNaptConnection, .-rtk_addNaptConnection
	.align	2
	.globl	negative_fragCache_init
	.set	nomips16
	.ent	negative_fragCache_init
	.type	negative_fragCache_init, @function
negative_fragCache_init:
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
	lui	$18,%hi(kmalloc_caches)
	addiu	$18,$18,%lo(kmalloc_caches)
	lw	$4,40($18)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lui	$3,%hi(negative_cache_table)
	beq	$2,$0,$L484
	sw	$2,%lo(negative_cache_table)($3)

	lui	$4,%hi(negative_cache_list_free)
	addiu	$16,$4,%lo(negative_cache_list_free)
	sw	$0,%lo(negative_cache_list_free)($4)
	sw	$0,8($16)
	sw	$16,4($16)
	move	$4,$0
	move	$8,$3
	j	$L486
	li	$7,64			# 0x40

$L491:
	lw	$2,%lo(negative_cache_table)($8)
$L486:
	sll	$5,$4,2
	sll	$6,$4,4
	subu	$5,$6,$5
	addu	$2,$2,$5
	sw	$0,8($2)
	lw	$2,%lo(negative_cache_table)($3)
	addiu	$4,$4,1
	addu	$2,$2,$5
	sw	$0,0($2)
	lw	$2,%lo(negative_cache_table)($3)
	addu	$5,$2,$5
	bne	$4,$7,$L491
	sw	$5,4($5)

	lui	$20,%hi(negative_cache_timeout)
	addiu	$20,$20,%lo(negative_cache_timeout)
	move	$17,$0
	j	$L487
	li	$19,64			# 0x40

$L493:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	sw	$2,32($2)
	sw	$20,28($2)
	sw	$0,48($2)
	lw	$4,4($16)
	addiu	$3,$2,48
	sw	$4,52($2)
	lw	$4,4($16)
	sw	$2,0($4)
	lw	$2,8($16)
	sw	$3,4($16)
	addiu	$2,$2,1
	beq	$17,$19,$L492
	sw	$2,8($16)

$L487:
	lw	$4,28($18)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,12
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L493
	move	$7,$0

$L484:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,48

$L492:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	negative_fragCache_init
	.size	negative_fragCache_init, .-negative_fragCache_init
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
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	lui	$17,%hi(udp_cache_table)

	move	$20,$2
	beq	$2,$0,$L495
	sw	$2,%lo(udp_cache_table)($17)

	lui	$2,%hi(udp_cache_list_free)
	sll	$3,$18,2
	sll	$4,$18,4
	addiu	$16,$2,%lo(udp_cache_list_free)
	subu	$4,$4,$3
	sw	$0,%lo(udp_cache_list_free)($2)
	li	$5,32			# 0x20
	lui	$2,%hi(max_udp_frag_entry)
	sw	$18,%lo(max_udp_frag_entry)($2)
	sw	$0,8($16)
	jal	__kmalloc
	sw	$16,4($16)

	sw	$2,0($20)
	lw	$3,%lo(udp_cache_table)($17)
	lw	$3,0($3)
	beq	$3,$0,$L497
	li	$2,-1			# 0xffffffffffffffff

	blez	$18,$L498
	nop

	move	$2,$0
	move	$4,$0
	j	$L500
	move	$5,$17

$L504:
	lw	$3,%lo(udp_cache_table)($5)
	lw	$3,0($3)
$L500:
	addu	$3,$3,$2
	sw	$0,8($3)
	lw	$3,%lo(udp_cache_table)($17)
	addiu	$4,$4,1
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(udp_cache_table)($17)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$3,4($3)
	bne	$4,$18,$L504
	addiu	$2,$2,12

	lui	$20,%hi(cache_timeout)
	addiu	$20,$20,%lo(cache_timeout)
	j	$L501
	move	$17,$0

$L505:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	slt	$3,$17,$18
	sw	$2,36($2)
	sw	$20,32($2)
	sw	$0,52($2)
	lw	$5,4($16)
	addiu	$4,$2,52
	sw	$5,56($2)
	lw	$5,4($16)
	sw	$2,0($5)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$3,$0,$L498
	sw	$2,8($16)

$L501:
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,16
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L505
	move	$7,$0

$L495:
	move	$2,$0
$L497:
	lw	$31,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,48

$L498:
	j	$L497
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	udp_fragCache_init
	.size	udp_fragCache_init, .-udp_fragCache_init
	.globl	fastpath_packet_bypassed_num
	.data
	.align	2
	.type	fastpath_packet_bypassed_num, @object
	.size	fastpath_packet_bypassed_num, 4
fastpath_packet_bypassed_num:
	.word	10
	.globl	rtl_init_bypass_packet_hook
	.section	.bss,"aw",@nobits
	.align	2
	.type	rtl_init_bypass_packet_hook, @object
	.size	rtl_init_bypass_packet_hook, 4
rtl_init_bypass_packet_hook:
	.space	4
	.globl	rtl_check_bypass_packet_hook
	.align	2
	.type	rtl_check_bypass_packet_hook, @object
	.size	rtl_check_bypass_packet_hook, 4
rtl_check_bypass_packet_hook:
	.space	4
	.globl	fastpath_napt_proc_fops
	.data
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
	.local	napt_table_list_max
	.comm	napt_table_list_max,4,4
	.local	path_table_list_max
	.comm	path_table_list_max,4,4
	.local	max_udp_frag_entry
	.comm	max_udp_frag_entry,4,4
	.globl	route_list_inuse
	.section	.bss
	.align	2
	.type	route_list_inuse, @object
	.size	route_list_inuse, 12
route_list_inuse:
	.space	12
	.globl	route_list_free
	.align	2
	.type	route_list_free, @object
	.size	route_list_free, 12
route_list_free:
	.space	12
	.globl	napt_list_inuse
	.align	2
	.type	napt_list_inuse, @object
	.size	napt_list_inuse, 12
napt_list_inuse:
	.space	12
	.globl	napt_list_free
	.align	2
	.type	napt_list_free, @object
	.size	napt_list_free, 12
napt_list_free:
	.space	12
	.globl	table_napt
	.align	2
	.type	table_napt, @object
	.size	table_napt, 4
table_napt:
	.space	4
	.globl	path_list_inuse
	.align	2
	.type	path_list_inuse, @object
	.size	path_list_inuse, 12
path_list_inuse:
	.space	12
	.globl	path_list_free
	.align	2
	.type	path_list_free, @object
	.size	path_list_free, 12
path_list_free:
	.space	12
	.globl	table_path
	.align	2
	.type	table_path, @object
	.size	table_path, 4
table_path:
	.space	4
	.globl	fp_arp
	.align	2
	.type	fp_arp, @object
	.size	fp_arp, 4
fp_arp:
	.space	4
	.globl	fp_path
	.align	2
	.type	fp_path, @object
	.size	fp_path, 4
fp_path:
	.space	4
	.globl	fp_route
	.align	2
	.type	fp_route, @object
	.size	fp_route, 4
fp_route:
	.space	4
	.globl	fp_napt
	.align	2
	.type	fp_napt, @object
	.size	fp_napt, 4
fp_napt:
	.space	4
	.globl	udp_cache_list_free
	.align	2
	.type	udp_cache_list_free, @object
	.size	udp_cache_list_free, 12
udp_cache_list_free:
	.space	12
	.globl	udp_cache_table
	.align	2
	.type	udp_cache_table, @object
	.size	udp_cache_table, 4
udp_cache_table:
	.space	4
	.globl	negative_cache_list_free
	.align	2
	.type	negative_cache_list_free, @object
	.size	negative_cache_list_free, 12
negative_cache_list_free:
	.space	12
	.globl	negative_cache_table
	.align	2
	.type	negative_cache_table, @object
	.size	negative_cache_table, 4
negative_cache_table:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
