	.file	1 "fast_l2tp_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.fastpath_l2tp_single_open,"ax",@progbits
	.align	2
	.globl	fastpath_l2tp_single_open
	.set	nomips16
	.ent	fastpath_l2tp_single_open
	.type	fastpath_l2tp_single_open, @function
fastpath_l2tp_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(l2tp_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(l2tp_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_l2tp_single_open
	.size	fastpath_l2tp_single_open, .-fastpath_l2tp_single_open
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%c\012\000"
	.section	.text.l2tp_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	l2tp_read_proc
	.type	l2tp_read_proc, @function
l2tp_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$6,%lo(fast_l2tp_fw)($2)
	lui	$5,%hi($LC0)
	addiu	$5,$5,%lo($LC0)
	jal	seq_printf
	addiu	$6,$6,48

	move	$2,$0
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	l2tp_read_proc
	.size	l2tp_read_proc, .-l2tp_read_proc
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"ppp0\000"
	.section	.text.l2tp_id,"ax",@progbits
	.align	2
	.globl	l2tp_id
	.set	nomips16
	.ent	l2tp_id
	.type	l2tp_id, @function
l2tp_id:
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$16,$4
	sw	$5,16($sp)
	jal	rtl_get_skb_data
	sw	$6,20($sp)

	move	$4,$16
	jal	rtl_ip_hdr
	move	$17,$2

	lw	$5,16($sp)
	lw	$6,20($sp)
	beq	$2,$0,$L3
	nop

	lbu	$3,0($2)
	andi	$3,$3,0xf
	sll	$3,$3,2
	beq	$6,$0,$L5
	addu	$3,$2,$3

	lhu	$7,0($3)
	li	$4,1701			# 0x6a5
	beq	$7,$4,$L6
	nop

$L5:
	beq	$5,$0,$L7
	li	$4,1701			# 0x6a5

	lhu	$7,2($3)
	beq	$7,$4,$L6
	nop

$L7:
	beq	$6,$0,$L12
	li	$2,1701			# 0x6a5

	lhu	$4,0($3)
	beq	$4,$2,$L13
	nop

$L12:
	beq	$5,$0,$L3
	li	$2,1701			# 0x6a5

	lhu	$4,2($3)
	beq	$4,$2,$L13
	nop

$L3:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

$L13:
	lhu	$2,8($3)
	andi	$2,$2,0x4000
	beq	$2,$0,$L15
	addiu	$16,$3,10

	addiu	$16,$3,12
$L15:
	lui	$18,%hi(l2tpInfo)
	addiu	$17,$18,%lo(l2tpInfo)
	lw	$4,12($17)
	jal	filter_checkConnect
	nop

	bne	$2,$0,$L28
	nop

$L16:
	lhu	$3,6($16)
	li	$2,49185			# 0xc021
	bne	$3,$2,$L3
	nop

	lbu	$2,8($16)
	li	$3,5			# 0x5
	beq	$2,$3,$L29
	addiu	$4,$18,%lo(l2tpInfo)

$L17:
	li	$3,6			# 0x6
	beq	$2,$3,$L30
	nop

$L18:
	li	$3,1			# 0x1
	bne	$2,$3,$L3
	nop

	lhu	$5,10($16)
	slt	$2,$5,5
	bne	$2,$0,$L3
	li	$2,5			# 0x5

	lbu	$4,12($16)
	lbu	$3,13($16)
	beq	$4,$2,$L31
	addiu	$6,$16,8

	li	$2,4			# 0x4
	j	$L21
	li	$7,5			# 0x5

$L22:
	lbu	$4,0($3)
	lbu	$3,1($3)
	beq	$4,$7,$L32
	nop

$L21:
	addu	$2,$2,$3
	slt	$4,$2,$5
	bne	$4,$0,$L22
	addu	$3,$6,$2

	j	$L3
	nop

$L6:
	lhu	$4,8($3)
	sll	$7,$4,16
	sra	$7,$7,16
	bgez	$7,$L7
	andi	$4,$4,0x4000

	beq	$4,$0,$L9
	addiu	$5,$3,10

	addiu	$5,$3,12
$L9:
	lhu	$4,14($5)
	li	$6,4			# 0x4
	beq	$4,$6,$L34
	li	$6,11			# 0xb

	beq	$4,$6,$L35
	li	$6,12			# 0xc

	bne	$4,$6,$L3
	nop

	lhu	$4,2($5)
	beq	$4,$0,$L3
	lui	$17,%hi(state)

	lw	$4,%lo(state)($17)
	sltu	$4,$4,2
	beq	$4,$0,$L3
	lui	$18,%hi(l2tpInfo)

	lhu	$4,0($5)
	addiu	$18,$18,%lo(l2tpInfo)
	sh	$4,20($18)
	lhu	$4,2($5)
	sh	$4,22($18)
	lw	$4,12($2)
	sw	$4,16($18)
	lw	$4,16($2)
	sw	$4,12($18)
	lhu	$2,0($3)
	jal	filter_addconnect
	sh	$2,40($18)

	li	$2,1			# 0x1
	lui	$4,%hi($LC1)
	li	$5,4			# 0x4
	addiu	$4,$4,%lo($LC1)
	jal	rtl865x_setNetifType
	sh	$2,38($18)

	li	$2,2			# 0x2
	j	$L3
	sw	$2,%lo(state)($17)

$L28:
	lw	$4,12($17)
	jal	filter_addconnect
	nop

	j	$L16
	nop

$L30:
	lhu	$3,20($17)
	lhu	$2,0($16)
	bne	$3,$2,$L3
	nop

	lhu	$3,22($17)
	lhu	$2,2($16)
	bne	$3,$2,$L3
	move	$4,$17

	move	$5,$0
	jal	memset
	li	$6,44			# 0x2c

	lui	$3,%hi(state)
	lbu	$2,8($16)
	j	$L18
	sw	$0,%lo(state)($3)

$L32:
	addiu	$2,$2,2
$L19:
	addu	$6,$6,$2
	lui	$2,%hi(l2tp_ppp_imagic)
	lw	$3,0($6)
	j	$L3
	sw	$3,%lo(l2tp_ppp_imagic)($2)

$L35:
	lui	$18,%hi(state)
	lw	$2,%lo(state)($18)
	bne	$2,$0,$L3
	nop

	jal	rtl_is_wan_dev
	move	$4,$16

	beq	$2,$0,$L3
	nop

	jal	rtl_get_skb_dev
	move	$4,$16

	lui	$4,%hi(l2tpInfo+24)
	lui	$3,%hi(l2tpInfo)
	addiu	$4,$4,%lo(l2tpInfo+24)
	addiu	$5,$17,-8
	li	$6,6			# 0x6
	jal	memcpy
	sw	$2,%lo(l2tpInfo)($3)

	lui	$4,%hi(l2tpInfo+30)
	addiu	$4,$4,%lo(l2tpInfo+30)
	addiu	$5,$17,-14
	jal	memcpy
	li	$6,6			# 0x6

	lui	$4,%hi(l2tpInfo+36)
	addiu	$4,$4,%lo(l2tpInfo+36)
	addiu	$5,$17,-2
	jal	memcpy
	li	$6,2			# 0x2

	li	$2,1			# 0x1
	j	$L3
	sw	$2,%lo(state)($18)

$L34:
	lui	$4,%hi(l2tpInfo)
	addiu	$4,$4,%lo(l2tpInfo)
	lhu	$3,20($4)
	lhu	$2,0($5)
	bne	$3,$2,$L3
	move	$5,$0

	jal	memset
	li	$6,44			# 0x2c

	lui	$4,%hi($LC1)
	addiu	$4,$4,%lo($LC1)
	jal	rtl865x_setNetifType
	li	$5,2			# 0x2

	lui	$2,%hi(state)
	j	$L3
	sw	$0,%lo(state)($2)

$L29:
	move	$5,$0
	jal	memset
	li	$6,44			# 0x2c

	lui	$3,%hi(state)
	lbu	$2,8($16)
	j	$L17
	sw	$0,%lo(state)($3)

$L31:
	j	$L19
	li	$2,6			# 0x6

	.set	macro
	.set	reorder
	.end	l2tp_id
	.size	l2tp_id, .-l2tp_id
	.section	.text.fastpath_l2tp_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_l2tp_single_write
	.type	fastpath_l2tp_single_write, @function
fastpath_l2tp_single_write:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sltu	$3,$6,2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L43
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L43
	addiu	$3,$5,1
	.set	macro
	.set	reorder

	or	$2,$3,$5
	lw	$3,24($28)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L43
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 732 "net/rtl/fastpath/fast_l2tp_core.c" 1
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
	bne	$6,$0,$L43
	lui	$3,%hi(fast_l2tp_fw)
	.set	macro
	.set	reorder

	lb	$2,16($sp)
	addiu	$2,$2,-48
	sw	$2,%lo(fast_l2tp_fw)($3)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L44
	lui	$4,%hi(l2tpInfo)
	.set	macro
	.set	reorder

	addiu	$2,$4,%lo(l2tpInfo)
	lui	$3,%hi(l2tp_tx_id_hook)
	sw	$0,%lo(l2tp_tx_id_hook)($3)
	sh	$0,20($2)
$L39:
	addiu	$4,$4,%lo(l2tpInfo)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,44			# 0x2c
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	sw	$0,%lo(state)($2)
	move	$2,$16
$L37:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L44:
	lui	$3,%hi(l2tp_id)
	lui	$2,%hi(l2tp_tx_id_hook)
	addiu	$3,$3,%lo(l2tp_id)
	.set	noreorder
	.set	nomacro
	j	$L39
	sw	$3,%lo(l2tp_tx_id_hook)($2)
	.set	macro
	.set	reorder

$L43:
	.set	noreorder
	.set	nomacro
	j	$L37
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	fastpath_l2tp_single_write
	.size	fastpath_l2tp_single_write, .-fastpath_l2tp_single_write
	.section	.text.is_l2tp_device,"ax",@progbits
	.align	2
	.globl	is_l2tp_device
	.set	nomips16
	.ent	is_l2tp_device
	.type	is_l2tp_device, @function
is_l2tp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(l2tpInfo+4)
	move	$16,$4
	lw	$4,%lo(l2tpInfo+4)($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L47
	move	$2,$0
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_name
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($2)
1:	lbu	$1,($16)
	addiu	$2,1
	bne	$1,$3,2f
	addiu	$16,1
	bnez	$3,1b
	lbu	$3,($2)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lw	$31,20($sp)
	sltu	$2,$3,1
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

$L47:
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	is_l2tp_device
	.size	is_l2tp_device, .-is_l2tp_device
	.section	.text.set_l2tp_device,"ax",@progbits
	.align	2
	.globl	set_l2tp_device
	.set	nomips16
	.ent	set_l2tp_device
	.type	set_l2tp_device, @function
set_l2tp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$2,%lo(fast_l2tp_fw)($2)
	beq	$2,$0,$L48
	nop

	jal	rtl_get_dev_by_name
	nop

	lui	$3,%hi(l2tpInfo+4)
	sw	$2,%lo(l2tpInfo+4)($3)
$L48:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_l2tp_device
	.size	set_l2tp_device, .-set_l2tp_device
	.section	.text.event_ppp_dev_down,"ax",@progbits
	.align	2
	.globl	event_ppp_dev_down
	.set	nomips16
	.ent	event_ppp_dev_down
	.type	event_ppp_dev_down, @function
event_ppp_dev_down:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$16,%hi(l2tpInfo)
	addiu	$16,$16,%lo(l2tpInfo)
	lhu	$2,38($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L50
	move	$17,$4
	.set	macro
	.set	reorder

	lw	$4,4($16)
	beq	$4,$0,$L50
	jal	rtl_get_ppp_dev_name
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$3,($2)
1:	lbu	$1,($17)
	addiu	$2,1
	bne	$1,$3,2f
	addiu	$17,1
	bnez	$3,1b
	lbu	$3,($2)
	move	$3,$1
2:	subu	$3,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L52
	move	$4,$16
	.set	macro
	.set	reorder

$L50:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L52:
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,44			# 0x2c
	.set	macro
	.set	reorder

	lui	$2,%hi(state)
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	sw	$0,%lo(state)($2)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	event_ppp_dev_down
	.size	event_ppp_dev_down, .-event_ppp_dev_down
	.section	.text.fast_l2tp_rx,"ax",@progbits
	.align	2
	.globl	fast_l2tp_rx
	.set	nomips16
	.ent	fast_l2tp_rx
	.type	fast_l2tp_rx, @function
fast_l2tp_rx:
	.frame	$sp,40,$31		# vars= 0, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	move	$4,$16
	lbu	$19,0($2)
	jal	rtl_get_skb_data
	move	$17,$2

	move	$4,$16
	jal	rtl_get_skb_len
	move	$18,$2

	sltu	$2,$2,40
	bne	$2,$0,$L73
	andi	$19,$19,0xf

	sll	$19,$19,2
	li	$2,1701			# 0x6a5
	addu	$19,$17,$19
	lhu	$3,0($19)
	beq	$3,$2,$L74
	lui	$20,%hi(l2tpInfo)

$L73:
	move	$2,$0
$L54:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L74:
	lw	$2,%lo(l2tpInfo)($20)
	beq	$2,$0,$L75
	move	$4,$16

$L55:
	lbu	$2,28($18)
	li	$19,2			# 0x2
	move	$4,$18
	andi	$3,$2,0x40
	movz	$19,$0,$3
	andi	$2,$2,0x2
	addu	$19,$19,$2
	move	$6,$16
	jal	fast_l2tp_tx_lcp_echo_reply
	move	$5,$19

	li	$3,1			# 0x1
	beq	$2,$3,$L63
	addiu	$20,$20,%lo(l2tpInfo)

	lhu	$2,20($20)
	beq	$2,$0,$L54
	move	$2,$0

	lhu	$2,22($20)
	beq	$2,$0,$L73
	addu	$2,$18,$19

	lbu	$3,36($2)
	bne	$3,$0,$L73
	nop

	lbu	$3,37($2)
	li	$2,33			# 0x21
	bne	$3,$2,$L54
	move	$2,$0

	lui	$2,%hi(l2tpInfo)
	lw	$2,%lo(l2tpInfo)($2)
	beq	$2,$0,$L54
	move	$2,$0

	lw	$3,16($20)
	lw	$2,16($17)
	bne	$3,$2,$L54
	move	$2,$0

	lw	$5,4($20)
	beq	$5,$0,$L76
	lui	$4,%hi($LC1)

	addiu	$19,$19,38
$L77:
	li	$3,6			# 0x6
	addu	$18,$18,$19
	lbu	$2,9($18)
	beq	$2,$3,$L59
	li	$3,17			# 0x11

	beq	$2,$3,$L59
	li	$3,1			# 0x1

	beq	$2,$3,$L59
	li	$3,47			# 0x2f

	bne	$2,$3,$L54
	move	$2,$0

$L59:
	jal	rtl_set_skb_dev
	move	$4,$16

	move	$5,$19
	jal	skb_pull
	move	$4,$16

	jal	rtl_skb_reset_network_header
	move	$4,$16

	jal	rtl_skb_reset_transport_header
	move	$4,$16

	lw	$4,4($20)
	beq	$4,$0,$L54
	move	$2,$0

	jal	rtl_get_ppp_dev_priv
	nop

	beq	$2,$0,$L73
	nop

	lw	$4,4($20)
	jal	rtl_get_ppp_dev_priv
	nop

	move	$4,$16
	jal	rtl_get_skb_len
	move	$17,$2

	move	$6,$2
	move	$5,$0
	jal	rtl_inc_ppp_stats
	move	$4,$17

	j	$L54
	move	$2,$0

$L63:
	j	$L54
	li	$2,1			# 0x1

$L75:
	move	$5,$0
	jal	l2tp_id
	li	$6,1			# 0x1

	j	$L55
	nop

$L76:
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC1)

	move	$5,$2
	beq	$2,$0,$L73
	sw	$2,4($20)

	j	$L77
	addiu	$19,$19,38

	.set	macro
	.set	reorder
	.end	fast_l2tp_rx
	.size	fast_l2tp_rx, .-fast_l2tp_rx
	.section	.text.get_fast_l2tp_lastxmit,"ax",@progbits
	.align	2
	.globl	get_fast_l2tp_lastxmit
	.set	nomips16
	.ent	get_fast_l2tp_lastxmit
	.type	get_fast_l2tp_lastxmit, @function
get_fast_l2tp_lastxmit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(l2tpInfo)
	li	$3,1			# 0x1
	addiu	$2,$2,%lo(l2tpInfo)
	lhu	$4,38($2)
	beq	$4,$3,$L81
	nop

	j	$31
	move	$2,$0

$L81:
	lw	$2,8($2)
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	get_fast_l2tp_lastxmit
	.size	get_fast_l2tp_lastxmit, .-get_fast_l2tp_lastxmit
	.section	.text.check_for_fast_l2tp_to_wan,"ax",@progbits
	.align	2
	.globl	check_for_fast_l2tp_to_wan
	.set	nomips16
	.ent	check_for_fast_l2tp_to_wan
	.type	check_for_fast_l2tp_to_wan, @function
check_for_fast_l2tp_to_wan:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	jal	rtl_ip_hdr
	move	$4,$16

	lw	$3,12($2)
	li	$2,-256			# 0xffffffffffffff00
	lw	$31,20($sp)
	and	$2,$3,$2
	li	$3,-172032000			# 0xfffffffff5bf0000
	ori	$3,$3,0xc000
	lw	$16,16($sp)
	addu	$2,$2,$3
	sltu	$2,$0,$2
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	check_for_fast_l2tp_to_wan
	.size	check_for_fast_l2tp_to_wan, .-check_for_fast_l2tp_to_wan
	.section	.text.fast_l2tp_to_wan,"ax",@progbits
	.align	2
	.globl	fast_l2tp_to_wan
	.set	nomips16
	.ent	fast_l2tp_to_wan
	.type	fast_l2tp_to_wan, @function
fast_l2tp_to_wan:
	.frame	$sp,88,$31		# vars= 48, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	sw	$31,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lui	$2,%hi(fast_l2tp_fw)
	lw	$2,%lo(fast_l2tp_fw)($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L96
	move	$18,$4
	.set	macro
	.set	reorder

	lui	$17,%hi(l2tpInfo)
	addiu	$16,$17,%lo(l2tpInfo)
	lhu	$2,20($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L84
	move	$2,$0
	.set	macro
	.set	reorder

	lhu	$2,22($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L84
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,%lo(l2tpInfo)($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L96
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	lhu	$3,38($16)
	beq	$3,$2,$L98
$L96:
	move	$2,$0
$L84:
	lw	$31,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

$L98:
	jal	rtl_ip_hdr
	move	$4,$18
	lhu	$19,2($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$20,$2
	.set	macro
	.set	reorder

	sltu	$2,$2,52
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L101
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_cloned
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L99
	move	$4,$18
	.set	macro
	.set	reorder

$L101:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	beq	$2,$0,$L96
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	consume_skb
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	lw	$2,56($sp)
	move	$18,$2
	li	$5,52			# 0x34
$L103:
	lbu	$20,1($20)
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$5,%hi(l2tpInfo+24)
	move	$4,$2
	li	$6,14			# 0xe
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$5,%lo(l2tpInfo+24)
	.set	macro
	.set	reorder

	li	$6,268369920			# 0xfff0000
	lw	$2,36($sp)
	ori	$6,$6,0xffff
	li	$4,-251723776			# 0xfffffffff0ff0000
	move	$5,$0
	and	$6,$2,$6
	li	$2,1073741824			# 0x40000000
	or	$6,$6,$2
	lw	$2,16($16)
	ori	$4,$4,0xffff
	lw	$3,12($16)
	and	$6,$6,$4
	li	$4,83886080			# 0x5000000
	or	$6,$6,$4
	sw	$2,48($sp)
	sw	$6,36($sp)
	li	$6,16384			# 0x4000
	li	$2,64			# 0x40
	move	$4,$18
	sh	$6,42($sp)
	li	$6,17			# 0x11
	sb	$6,45($sp)
	sw	$3,52($sp)
	sb	$2,44($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$20,37($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$3,$sp,52
	addiu	$2,$2,-14
	sh	$2,38($sp)
	sh	$0,40($sp)
	lw	$2,36($sp)
	lw	$7,40($sp)
	sh	$0,46($sp)
	srl	$5,$2,22
	lw	$4,44($sp)
	addu	$2,$7,$2
	lw	$6,48($sp)
	sltu	$7,$2,$7
	addu	$2,$2,$4
	andi	$5,$5,0x3c
	addu	$7,$2,$7
	addiu	$2,$sp,36
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	addu	$5,$2,$5
	sltu	$2,$4,$6
	addu	$2,$2,$4
$L87:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L87
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$18
	addiu	$19,$19,18
 #APP
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $2, 16	
	addu	$2, $1		
	sltu	$1, $2, $1	
	srl	$2, $2, 16	
	addu	$2, $1		
	xori	$2, 0xffff	
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,46($sp)
	.set	macro
	.set	reorder

	li	$6,20			# 0x14
	addiu	$5,$sp,36
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,14
	.set	macro
	.set	reorder

	move	$4,$18
	lhu	$5,40($16)
	lhu	$2,22($16)
	lhu	$3,20($16)
	sh	$5,16($sp)
	li	$5,1701			# 0x6a5
	sh	$2,28($sp)
	li	$2,-253			# 0xffffffffffffff03
	sh	$5,18($sp)
	li	$5,2			# 0x2
	sh	$2,30($sp)
	li	$2,33			# 0x21
	sh	$3,26($sp)
	sh	$5,24($sp)
	sh	$19,20($sp)
	sh	$0,22($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,32($sp)
	.set	macro
	.set	reorder

	li	$6,18			# 0x12
	addiu	$5,$sp,16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,34
	.set	macro
	.set	reorder

	lw	$5,%lo(l2tpInfo)($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	timeoutCheck_skipp_pkt
	addiu	$4,$2,52
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L88
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$2,%lo(jiffies)($2)
	sw	$2,8($16)
$L88:
	lw	$4,4($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L102
	lui	$2,%hi(gQosEnabled)
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_priv
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L102
	lui	$2,%hi(gQosEnabled)
	.set	macro
	.set	reorder

	lw	$4,4($16)
	jal	rtl_get_ppp_dev_priv
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$16,$2
	.set	macro
	.set	reorder

	move	$6,$2
	li	$5,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_inc_ppp_stats
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(gQosEnabled)
$L102:
	lw	$2,%lo(gQosEnabled)($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L100
	move	$4,$18
	.set	macro
	.set	reorder

	jal	rtl_call_skb_ndo_start_xmit
	.set	noreorder
	.set	nomacro
	j	$L84
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L100:
	jal	dev_queue_xmit
	.set	noreorder
	.set	nomacro
	j	$L84
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L99:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L103
	li	$5,52			# 0x34
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L101
	move	$4,$18
	.set	macro
	.set	reorder

	.end	fast_l2tp_to_wan
	.size	fast_l2tp_to_wan, .-fast_l2tp_to_wan
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"fast_l2tp\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_init
	.set	nomips16
	.ent	fast_l2tp_init
	.type	fast_l2tp_init, @function
fast_l2tp_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC2)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(fastpath_l2tp_proc_fops)
	addiu	$7,$7,%lo(fastpath_l2tp_proc_fops)
	addiu	$4,$4,%lo($LC2)
	move	$5,$0
	addiu	$6,$6,%lo(proc_root)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$3,%hi(res1)
	sw	$2,%lo(res1)($3)
	lui	$2,%hi(l2tp_tx_id_hook)
	sw	$0,%lo(l2tp_tx_id_hook)($2)
	lui	$4,%hi(l2tpInfo)
	lui	$2,%hi(fast_l2tp_fw)
	addiu	$4,$4,%lo(l2tpInfo)
	move	$5,$0
	li	$6,44			# 0x2c
	sw	$0,%lo(fast_l2tp_fw)($2)
	lui	$2,%hi(state)
	jal	memset
	sw	$0,%lo(state)($2)

	move	$2,$0
	lw	$31,28($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_l2tp_init
	.size	fast_l2tp_init, .-fast_l2tp_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_l2tp_exit
	.set	nomips16
	.ent	fast_l2tp_exit
	.type	fast_l2tp_exit, @function
fast_l2tp_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res1)
	lw	$2,%lo(res1)($16)
	beq	$2,$0,$L105
	lui	$4,%hi($LC2)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC2)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res1)($16)
$L105:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_l2tp_exit
	.size	fast_l2tp_exit, .-fast_l2tp_exit
	.globl	fastpath_l2tp_proc_fops
	.section	.data.fastpath_l2tp_proc_fops,"aw",@progbits
	.align	2
	.type	fastpath_l2tp_proc_fops, @object
	.size	fastpath_l2tp_proc_fops, 108
fastpath_l2tp_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_l2tp_single_write
	.space	28
	.word	fastpath_l2tp_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	l2tp_ppp_imagic
	.section	.bss.l2tp_ppp_imagic,"aw",@nobits
	.align	2
	.type	l2tp_ppp_imagic, @object
	.size	l2tp_ppp_imagic, 4
l2tp_ppp_imagic:
	.space	4
	.globl	state
	.section	.bss.state,"aw",@nobits
	.align	2
	.type	state, @object
	.size	state, 4
state:
	.space	4
	.globl	l2tpInfo
	.section	.bss.l2tpInfo,"aw",@nobits
	.align	2
	.type	l2tpInfo, @object
	.size	l2tpInfo, 44
l2tpInfo:
	.space	44
	.globl	fast_l2tp_fw
	.section	.bss.fast_l2tp_fw,"aw",@nobits
	.align	2
	.type	fast_l2tp_fw, @object
	.size	fast_l2tp_fw, 4
fast_l2tp_fw:
	.space	4
	.globl	l2tp_tx_id_hook
	.section	.bss.l2tp_tx_id_hook,"aw",@nobits
	.align	2
	.type	l2tp_tx_id_hook, @object
	.size	l2tp_tx_id_hook, 4
l2tp_tx_id_hook:
	.space	4
	.section	.bss.res1,"aw",@nobits
	.align	2
	.type	res1, @object
	.size	res1, 4
res1:
	.space	4
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
