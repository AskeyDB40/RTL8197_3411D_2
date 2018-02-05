	.file	1 "fast_pptp_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.pptp_conn_single_open,"ax",@progbits
	.align	2
	.globl	pptp_conn_single_open
	.set	nomips16
	.ent	pptp_conn_single_open
	.type	pptp_conn_single_open, @function
pptp_conn_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(pptpconn_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(pptpconn_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	pptp_conn_single_open
	.size	pptp_conn_single_open, .-pptp_conn_single_open
	.section	.text.fastpath_pptp_single_open,"ax",@progbits
	.align	2
	.globl	fastpath_pptp_single_open
	.set	nomips16
	.ent	fastpath_pptp_single_open
	.type	fastpath_pptp_single_open, @function
fastpath_pptp_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(ppfw_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(ppfw_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	fastpath_pptp_single_open
	.size	fastpath_pptp_single_open, .-fastpath_pptp_single_open
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%d\012\000"
	.section	.text.pptpconn_read_proc,"ax",@progbits
	.align	2
	.globl	pptpconn_read_proc
	.set	nomips16
	.ent	pptpconn_read_proc
	.type	pptpconn_read_proc, @function
pptpconn_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(pptp_conn_check)
	lw	$6,%lo(pptp_conn_check)($2)
	lui	$5,%hi($LC0)
	jal	seq_printf
	addiu	$5,$5,%lo($LC0)

	move	$2,$0
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	pptpconn_read_proc
	.size	pptpconn_read_proc, .-pptpconn_read_proc
	.section	.text.ppfw_read_proc,"ax",@progbits
	.align	2
	.globl	ppfw_read_proc
	.set	nomips16
	.ent	ppfw_read_proc
	.type	ppfw_read_proc, @function
ppfw_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_pptp_fw)
	lw	$6,%lo(fast_pptp_fw)($2)
	lui	$5,%hi($LC0)
	jal	seq_printf
	addiu	$5,$5,%lo($LC0)

	move	$2,$0
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ppfw_read_proc
	.size	ppfw_read_proc, .-ppfw_read_proc
	.section	.text.sync_tx_pptp_gre_seqno,"ax",@progbits
	.align	2
	.globl	sync_tx_pptp_gre_seqno
	.set	nomips16
	.ent	sync_tx_pptp_gre_seqno
	.type	sync_tx_pptp_gre_seqno, @function
sync_tx_pptp_gre_seqno:
	.frame	$sp,40,$31		# vars= 8, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$18,$4
	.set	macro
	.set	reorder

	move	$16,$2
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	bne	$2,$0,$L18
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

$L18:
	jal	arch_local_irq_save
	lbu	$3,23($16)
	move	$17,$2
	li	$2,47			# 0x2f
	beq	$3,$2,$L19
$L7:
	move	$4,$17
$L22:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	arch_local_irq_restore
	addiu	$sp,$sp,40
	.set	macro
	.set	reorder

$L19:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$18
	.set	macro
	.set	reorder

	sltu	$2,$2,35
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L22
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$3,14($16)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addiu	$3,$3,14
	addu	$2,$2,$3
	lbu	$3,3($2)
	lbu	$4,1($2)
	lbu	$5,2($2)
	sb	$3,17($sp)
	andi	$4,$4,0x7
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L7
	sb	$5,16($sp)
	.set	macro
	.set	reorder

	lhu	$4,16($sp)
	li	$3,34827			# 0x880b
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L22
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,0($2)
	andi	$3,$3,0x10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L8
	move	$5,$0
	.set	macro
	.set	reorder

	lui	$3,%hi(pptpAccInfo)
	addiu	$3,$3,%lo(pptpAccInfo)
	lhu	$5,18($16)
	lhu	$4,46($3)
	.set	noreorder
	.set	nomacro
	beq	$5,$4,$L8
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lhu	$5,6($2)
	lhu	$4,18($3)
	beq	$5,$4,$L20
$L15:
	li	$5,1			# 0x1
$L8:
	lb	$3,1($2)
	.set	noreorder
	.set	nomacro
	bltz	$3,$L21
	lui	$3,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

$L9:
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L7
	addiu	$8,$16,14
	.set	macro
	.set	reorder

	lui	$4,%hi(pptpAccInfo)
	addiu	$4,$4,%lo(pptpAccInfo)
	addiu	$2,$16,30
	lhu	$3,44($4)
	addiu	$3,$3,1
	andi	$3,$3,0xffff
	sh	$3,44($4)
	sh	$3,18($16)
	lw	$7,18($16)
	lw	$3,14($16)
	sh	$0,24($16)
	lw	$4,22($16)
	srl	$5,$3,22
	addu	$3,$7,$3
	andi	$5,$5,0x3c
	lw	$6,26($16)
	sltu	$7,$3,$7
	addu	$3,$3,$4
	addu	$5,$8,$5
	addu	$7,$3,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	sltu	$3,$4,$6
	addu	$3,$3,$4
$L10:
	lw	$4,0($2)
	addiu	$2,$2,4
	addu	$3,$4,$3
	sltu	$4,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L10
	addu	$3,$4,$3
	.set	macro
	.set	reorder

 #APP
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $3, 16	
	addu	$3, $1		
	sltu	$1, $3, $1	
	srl	$3, $3, 16	
	addu	$3, $1		
	xori	$3, 0xffff	
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L7
	sh	$3,24($16)
	.set	macro
	.set	reorder

$L21:
	addiu	$3,$3,%lo(pptpAccInfo)
	lhu	$6,6($2)
	lhu	$4,18($3)
	.set	noreorder
	.set	nomacro
	bne	$6,$4,$L22
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$6,32($3)
	lw	$4,30($16)
	.set	noreorder
	.set	nomacro
	bne	$6,$4,$L22
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,40($3)
	srl	$4,$3,24
	sw	$3,20($sp)
	srl	$6,$3,16
	sb	$4,12($2)
	srl	$4,$3,8
	sb	$6,13($2)
	sb	$4,14($2)
	.set	noreorder
	.set	nomacro
	j	$L9
	sb	$3,15($2)
	.set	macro
	.set	reorder

$L20:
	lw	$5,32($3)
	lw	$4,30($16)
	.set	noreorder
	.set	nomacro
	bne	$5,$4,$L15
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$6,8($2)
	lw	$4,36($3)
	sb	$6,51($3)
	srl	$8,$6,24
	srl	$7,$6,16
	sb	$8,48($3)
	srl	$6,$6,8
	sb	$7,49($3)
	sb	$6,50($3)
	addiu	$9,$4,1
	srl	$8,$4,24
	sw	$9,36($3)
	srl	$7,$4,16
	sw	$4,20($sp)
	srl	$6,$4,8
	sb	$8,8($2)
	sb	$7,9($2)
	sb	$6,10($2)
	.set	noreorder
	.set	nomacro
	j	$L8
	sb	$4,11($2)
	.set	macro
	.set	reorder

	.end	sync_tx_pptp_gre_seqno
	.size	sync_tx_pptp_gre_seqno, .-sync_tx_pptp_gre_seqno
	.section	.text.is_pptp_device,"ax",@progbits
	.align	2
	.globl	is_pptp_device
	.set	nomips16
	.ent	is_pptp_device
	.type	is_pptp_device, @function
is_pptp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$2,%hi(pptpAccInfo+56)
	move	$16,$4
	lw	$4,%lo(pptpAccInfo+56)($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L25
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

$L25:
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

	.end	is_pptp_device
	.size	is_pptp_device, .-is_pptp_device
	.section	.text.set_pptp_device,"ax",@progbits
	.align	2
	.globl	set_pptp_device
	.set	nomips16
	.ent	set_pptp_device
	.type	set_pptp_device, @function
set_pptp_device:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	beq	$2,$0,$L26
	nop

	jal	rtl_get_dev_by_name
	nop

	lui	$3,%hi(pptpAccInfo+56)
	sw	$2,%lo(pptpAccInfo+56)($3)
$L26:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	set_pptp_device
	.size	set_pptp_device, .-set_pptp_device
	.section	.text.Check_GRE_rx_net_device,"ax",@progbits
	.align	2
	.globl	Check_GRE_rx_net_device
	.set	nomips16
	.ent	Check_GRE_rx_net_device
	.type	Check_GRE_rx_net_device, @function
Check_GRE_rx_net_device:
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
	lui	$18,%hi(pptpAccInfo)
	jal	rtl_ip_hdr
	move	$17,$4

	move	$16,$2
	addiu	$18,$18,%lo(pptpAccInfo)
	jal	rtl_get_skb_data
	move	$4,$17

	lbu	$20,0($16)
	move	$19,$2
	lw	$16,8($18)
	beq	$16,$0,$L31
	andi	$20,$20,0xf

	jal	rtl_get_skb_dev
	move	$4,$17

	beq	$16,$2,$L32
	sll	$20,$20,2

$L31:
	lw	$31,36($sp)
	move	$2,$0
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L32:
	lhu	$2,30($18)
	addu	$19,$19,$20
	lhu	$3,6($19)
	lw	$31,36($sp)
	xor	$2,$3,$2
	lw	$20,32($sp)
	lw	$19,28($sp)
	sltu	$2,$2,1
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	Check_GRE_rx_net_device
	.size	Check_GRE_rx_net_device, .-Check_GRE_rx_net_device
	.section	.text.fast_pptp_filter,"ax",@progbits
	.align	2
	.globl	fast_pptp_filter
	.set	nomips16
	.ent	fast_pptp_filter
	.type	fast_pptp_filter, @function
fast_pptp_filter:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	move	$4,$16
	jal	rtl_get_skb_len
	move	$19,$2

	sltu	$2,$2,56
	bne	$2,$0,$L33
	nop

	lbu	$2,0($19)
	lbu	$3,9($19)
	andi	$4,$2,0xf
	li	$2,6			# 0x6
	beq	$3,$2,$L46
	li	$2,1723			# 0x6bb

$L33:
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,40

$L46:
	sll	$4,$4,2
	addu	$4,$19,$4
	lhu	$3,0($4)
	bne	$3,$2,$L33
	li	$3,1			# 0x1

	lw	$2,12($4)
	srl	$2,$2,28
	sll	$2,$2,2
	addu	$4,$4,$2
	lhu	$2,2($4)
	bne	$2,$3,$L33
	li	$5,8			# 0x8

	lhu	$3,8($4)
	beq	$3,$5,$L47
	nop

	li	$2,13			# 0xd
	bne	$3,$2,$L33
	lui	$17,%hi(pptpAccInfo)

	addiu	$17,$17,%lo(pptpAccInfo)
	lw	$2,36($17)
	beq	$2,$0,$L49
	lui	$2,%hi(pptp_tcp_finished)

	lw	$2,20($17)
	beq	$2,$0,$L49
	lui	$2,%hi(pptp_tcp_finished)

	lw	$2,32($17)
	beq	$2,$0,$L49
	lui	$2,%hi(pptp_tcp_finished)

	lhu	$3,18($17)
	lhu	$2,12($4)
	bne	$3,$2,$L33
	lui	$2,%hi(pptp_tcp_finished)

$L49:
	sw	$0,%lo(pptp_tcp_finished)($2)
	j	$L33
	sw	$0,4($17)

$L47:
	lbu	$3,16($4)
	bne	$3,$2,$L33
	lui	$17,%hi(pptpAccInfo)

	addiu	$17,$17,%lo(pptpAccInfo)
	lw	$2,36($17)
	beq	$2,$0,$L45
	nop

	lw	$2,20($17)
	beq	$2,$0,$L45
	nop

	lw	$2,32($17)
	beq	$2,$0,$L45
	nop

	lhu	$3,18($17)
	lhu	$2,12($4)
	bne	$3,$2,$L33
	nop

	lhu	$5,30($17)
	lhu	$3,14($4)
	bne	$5,$3,$L33
	nop

	sh	$2,18($17)
$L48:
	lui	$3,%hi(pptp_tcp_finished)
	lhu	$2,14($4)
	li	$18,1			# 0x1
	move	$4,$16
	sw	$18,%lo(pptp_tcp_finished)($3)
	sh	$2,30($17)
	lw	$2,16($19)
	sw	$2,20($17)
	lw	$2,12($19)
	sw	$18,36($17)
	jal	rtl_get_skb_dev
	sw	$2,32($17)

	move	$4,$16
	jal	rtl_get_skb_data
	sw	$2,8($17)

	lui	$4,%hi(pptpAccInfo+24)
	addiu	$5,$2,-8
	li	$6,6			# 0x6
	jal	memcpy
	addiu	$4,$4,%lo(pptpAccInfo+24)

	jal	rtl_get_skb_data
	move	$4,$16

	lui	$4,%hi(pptpAccInfo+12)
	addiu	$4,$4,%lo(pptpAccInfo+12)
	addiu	$5,$2,-14
	jal	memcpy
	li	$6,6			# 0x6

	j	$L33
	sw	$18,4($17)

$L45:
	lhu	$2,12($4)
	j	$L48
	sh	$2,18($17)

	.set	macro
	.set	reorder
	.end	fast_pptp_filter
	.size	fast_pptp_filter, .-fast_pptp_filter
	.section	.text.fast_pptp_to_lan,"ax",@progbits
	.align	2
	.globl	fast_pptp_to_lan
	.set	nomips16
	.ent	fast_pptp_to_lan
	.type	fast_pptp_to_lan, @function
fast_pptp_to_lan:
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
	lui	$17,%hi(pptpAccInfo)
	lw	$16,0($4)
	move	$18,$4
	addiu	$17,$17,%lo(pptpAccInfo)
	jal	rtl_get_skb_dev
	move	$4,$16

	lw	$3,8($17)
	beq	$3,$2,$L78
	nop

$L76:
	move	$2,$0
$L51:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L78:
	jal	rtl_ip_hdr
	move	$4,$16

	lbu	$3,9($2)
	li	$2,47			# 0x2f
	bne	$3,$2,$L51
	move	$2,$0

	jal	rtl_get_skb_len
	move	$4,$16

	sltu	$2,$2,20
	bne	$2,$0,$L51
	move	$2,$0

	jal	rtl_ip_hdr
	move	$4,$16

	move	$4,$16
	jal	rtl_get_skb_data
	move	$19,$2

	lbu	$3,0($19)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$2,$2,$3
	li	$3,1			# 0x1
	lbu	$4,1($2)
	andi	$4,$4,0x7
	bne	$4,$3,$L76
	li	$3,34827			# 0x880b

	lhu	$4,2($2)
	bne	$4,$3,$L76
	nop

	lhu	$4,6($2)
	lhu	$3,30($17)
	bne	$4,$3,$L76
	nop

	lbu	$3,0($2)
	andi	$3,$3,0x10
	beq	$3,$0,$L69
	nop

	lw	$3,8($2)
	li	$20,12			# 0xc
	sw	$3,40($17)
$L52:
	lb	$5,1($2)
	addiu	$4,$20,4
	lhu	$3,4($2)
	slt	$5,$5,0
	beq	$3,$0,$L76
	movn	$20,$4,$5

	addu	$2,$2,$20
	lbu	$3,0($2)
	bne	$3,$0,$L54
	li	$21,1			# 0x1

	addiu	$2,$2,1
$L55:
	lbu	$2,0($2)
	li	$3,33			# 0x21
	beq	$2,$3,$L57
	li	$3,253			# 0xfd

	bne	$2,$3,$L51
	move	$2,$0

	lbu	$19,0($19)
	lw	$4,56($17)
	beq	$4,$0,$L51
	andi	$19,$19,0xf

	jal	rtl_get_ppp_dev_priv
	nop

	beq	$2,$0,$L76
	sll	$5,$19,2

	addu	$5,$5,$20
	move	$4,$16
	addu	$5,$21,$5
	jal	skb_pull
	addiu	$5,$5,-1

	jal	rtl_get_skb_data
	move	$4,$16

	li	$3,-3			# 0xfffffffffffffffd
	sb	$3,1($2)
	sb	$0,0($2)
	lw	$4,56($17)
	jal	rtl_get_ppp_dev_priv
	nop

	move	$19,$2
	jal	rtl_ppp_recv_lock
	move	$4,$2

	move	$4,$19
	move	$5,$16
	jal	rtl_ppp_receive_nonmp_frame
	li	$6,1			# 0x1

	move	$4,$19
	jal	rtl_ppp_recv_unlock
	move	$16,$2

	beq	$16,$0,$L74
	li	$19,1			# 0x1

	sw	$16,0($18)
$L59:
	jal	rtl_get_skb_data
	move	$4,$16

	andi	$18,$2,0x3
	bne	$18,$0,$L79
	nop

$L60:
	jal	rtl_skb_reset_network_header
	move	$4,$16

	jal	rtl_skb_reset_transport_header
	move	$4,$16

	lw	$4,56($17)
	beq	$4,$0,$L51
	move	$2,$0

	beq	$19,$0,$L80
	nop

$L61:
	lw	$5,56($17)
	jal	rtl_set_skb_dev
	move	$4,$16

	j	$L51
	li	$2,1			# 0x1

$L69:
	j	$L52
	li	$20,8			# 0x8

$L57:
	lbu	$2,0($19)
	addiu	$5,$21,1
	move	$4,$16
	move	$19,$0
	andi	$2,$2,0xf
	sll	$2,$2,2
	addu	$20,$2,$20
	jal	skb_pull
	addu	$5,$5,$20

	j	$L59
	nop

$L54:
	li	$4,255			# 0xff
	beq	$3,$4,$L81
	li	$3,3			# 0x3

	j	$L55
	move	$21,$0

$L81:
	lbu	$4,1($2)
	bne	$4,$3,$L55
	move	$21,$0

	lbu	$3,2($2)
	beq	$3,$0,$L56
	nop

	addiu	$2,$2,2
	j	$L55
	li	$21,2			# 0x2

$L80:
	jal	rtl_get_ppp_dev_priv
	nop

	beq	$2,$0,$L61
	nop

	lw	$4,56($17)
	jal	rtl_get_ppp_dev_priv
	nop

	move	$4,$16
	jal	rtl_get_skb_len
	move	$18,$2

	addiu	$6,$2,-2
	move	$4,$18
	jal	rtl_inc_ppp_stats
	move	$5,$0

	j	$L61
	nop

$L56:
	addiu	$2,$2,3
	j	$L55
	li	$21,3			# 0x3

$L79:
	jal	rtl_skb_headroom
	move	$4,$16

	sltu	$2,$2,$18
	bne	$2,$0,$L60
	nop

	jal	rtl_get_skb_data
	move	$4,$16

	move	$4,$16
	jal	rtl_get_skb_data
	subu	$20,$2,$18

	move	$4,$16
	jal	rtl_get_skb_len
	sw	$2,16($sp)

	move	$4,$20
	lw	$5,16($sp)
	jal	memmove
	move	$6,$2

	move	$4,$16
	move	$5,$18
	jal	rtl_set_skb_data
	li	$6,1			# 0x1

	move	$4,$16
	move	$5,$18
	jal	rtl_set_skb_tail
	li	$6,1			# 0x1

	j	$L60
	nop

$L74:
	j	$L51
	li	$2,-2			# 0xfffffffffffffffe

	.set	macro
	.set	reorder
	.end	fast_pptp_to_lan
	.size	fast_pptp_to_lan, .-fast_pptp_to_lan
	.section	.text.get_fastpptp_lastxmit,"ax",@progbits
	.align	2
	.globl	get_fastpptp_lastxmit
	.set	nomips16
	.ent	get_fastpptp_lastxmit
	.type	get_fastpptp_lastxmit, @function
get_fastpptp_lastxmit:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(fast_pptp_fw)
	lw	$2,%lo(fast_pptp_fw)($2)
	beq	$2,$0,$L84
	nop

	lui	$2,%hi(pptpAccInfo)
	addiu	$3,$2,%lo(pptpAccInfo)
	lw	$4,4($3)
	li	$3,1			# 0x1
	beq	$4,$3,$L86
	nop

	j	$31
	move	$2,$0

$L86:
	lw	$2,%lo(pptpAccInfo)($2)
	j	$31
	nop

$L84:
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	get_fastpptp_lastxmit
	.size	get_fastpptp_lastxmit, .-get_fastpptp_lastxmit
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"ppp0\000"
	.align	2
$LC2:
	.ascii	"RTL\000"
	.align	2
$LC3:
	.ascii	"pptpInfo.ppp0_dev == NULL\012\000"
	.align	2
$LC4:
	.ascii	"pptpInfo.ppp0_dev->priv == NULL\012\000"
	.section	.text.fast_pptp_to_wan,"ax",@progbits
	.align	2
	.globl	fast_pptp_to_wan
	.set	nomips16
	.ent	fast_pptp_to_wan
	.type	fast_pptp_to_wan, @function
fast_pptp_to_wan:
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
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$17,$4
	.set	macro
	.set	reorder

	move	$4,$17
	lw	$16,12($2)
	li	$2,-16777216			# 0xffffffffff000000
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	and	$16,$16,$2
	.set	macro
	.set	reorder

	lw	$18,12($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$3,12($2)
	li	$2,167772160			# 0xa000000
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L106
	li	$2,16711680			# 0xff0000
	.set	macro
	.set	reorder

$L88:
	lui	$18,%hi(pptpAccInfo)
$L112:
	addiu	$16,$18,%lo(pptpAccInfo)
$L113:
	lw	$4,56($16)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L89
	move	$2,$0
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_dev_name
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L89
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$4,56($16)
	jal	rtl_get_ppp_dev_name
	lui	$3,%hi($LC1)
	addiu	$3,$3,%lo($LC1)
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L89
	move	$2,$0
	.set	macro
	.set	reorder

	lw	$2,4($16)
	beq	$2,$0,$L90
	lw	$4,56($16)
	beq	$4,$0,$L91
	jal	rtl_get_ppp_dev_priv
	beq	$2,$0,$L90
	lw	$4,56($16)
	jal	rtl_get_ppp_dev_priv
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_vj_check
	move	$19,$2
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L89
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$20,1($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_lock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_xmit_pending
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L92
	lui	$5,%hi($LC2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L89
	move	$2,$0
	.set	macro
	.set	reorder

$L90:
	lw	$4,56($16)
	beq	$4,$0,$L91
	jal	rtl_get_ppp_dev_priv
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L107
	lui	$4,%hi($LC4)
	.set	macro
	.set	reorder

	move	$2,$0
$L89:
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

$L106:
	and	$18,$18,$2
	li	$2,4194304			# 0x400000
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L88
	li	$2,16384			# 0x4000
	.set	macro
	.set	reorder

	andi	$3,$3,0xff00
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L112
	lui	$18,%hi(pptpAccInfo)
	.set	macro
	.set	reorder

	lui	$2,%hi(pptp_conn_check)
	lw	$3,%lo(pptp_conn_check)($2)
	li	$2,3			# 0x3
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L113
	addiu	$16,$18,%lo(pptpAccInfo)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L89
	move	$2,$0
	.set	macro
	.set	reorder

$L92:
	li	$6,3			# 0x3
	addiu	$5,$5,%lo($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_cb
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$5,56($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_start_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_ppp_xmit_pending
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L108
	move	$4,$19
	.set	macro
	.set	reorder

	jal	rtl_get_ppp_xmit_pending
	move	$5,$0
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_set_ppp_xmit_pending
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ppp_xmit_unlock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headroom
	move	$4,$17
	.set	macro
	.set	reorder

	sltu	$2,$2,50
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L111
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_cloned
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L109
	move	$4,$17
	.set	macro
	.set	reorder

$L111:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L110
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	consume_skb
	sw	$2,56($sp)
	.set	macro
	.set	reorder

	lw	$2,56($sp)
	move	$17,$2
	li	$5,50			# 0x32
$L114:
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+24)
	move	$4,$2
	addiu	$5,$5,%lo(pptpAccInfo+24)
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$19,$2
	.set	macro
	.set	reorder

	lui	$5,%hi(pptpAccInfo+12)
	li	$6,6			# 0x6
	addiu	$4,$19,6
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$5,%lo(pptpAccInfo+12)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sb	$2,12($19)
	li	$2,268369920			# 0xfff0000
	sb	$0,13($19)
	ori	$2,$2,0xffff
	lw	$6,32($sp)
	li	$4,-251723776			# 0xfffffffff0ff0000
	ori	$4,$4,0xffff
	lw	$3,32($16)
	and	$6,$6,$2
	li	$2,1073741824			# 0x40000000
	or	$6,$6,$2
	lw	$2,20($16)
	and	$6,$6,$4
	li	$4,83886080			# 0x5000000
	or	$6,$6,$4
	move	$5,$0
	sw	$6,32($sp)
	li	$6,16384			# 0x4000
	move	$4,$17
	sh	$6,38($sp)
	sw	$2,44($sp)
	li	$6,47			# 0x2f
	li	$2,64			# 0x40
	sb	$6,41($sp)
	sw	$3,48($sp)
	sb	$2,40($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_ip_summed
	sb	$20,33($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$9,$sp,32
	lhu	$5,44($16)
	addiu	$2,$2,-14
	sh	$2,34($sp)
	addiu	$3,$sp,48
	addiu	$5,$5,1
	andi	$5,$5,0xffff
	lw	$2,32($sp)
	sh	$5,36($sp)
	lw	$8,36($sp)
	srl	$6,$2,22
	sh	$0,42($sp)
	andi	$6,$6,0x3c
	lw	$4,40($sp)
	addu	$2,$8,$2
	lw	$7,44($sp)
	sltu	$8,$2,$8
	addu	$2,$2,$4
	addu	$6,$9,$6
	addu	$8,$2,$8
	sltu	$4,$8,$4
	addu	$8,$8,$7
	addu	$4,$8,$4
	sltu	$2,$4,$7
	sh	$5,44($16)
	addu	$2,$2,$4
$L97:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L97
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$17
	sh	$5,46($16)
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
	sh	$2,42($sp)
	.set	macro
	.set	reorder

	addiu	$5,$sp,32
	li	$6,20			# 0x14
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$2,14
	.set	macro
	.set	reorder

	li	$2,48			# 0x30
	sb	$2,16($sp)
	li	$2,-127			# 0xffffffffffffff81
	move	$4,$17
	sb	$2,17($sp)
	li	$2,-30709			# 0xffffffffffff880b
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$2,18($sp)
	.set	macro
	.set	reorder

	move	$4,$17
	lw	$3,36($16)
	addiu	$2,$2,-50
	lhu	$7,18($16)
	addiu	$6,$3,1
	lw	$5,40($16)
	sw	$6,36($16)
	sh	$7,22($sp)
	sw	$3,24($sp)
	sw	$5,28($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	sh	$2,20($sp)
	.set	macro
	.set	reorder

	li	$6,16			# 0x10
	addiu	$4,$2,34
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	move	$4,$17
	lw	$5,8($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_dev
	sw	$2,%lo(pptpAccInfo)($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L89
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L107:
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L89
	move	$2,$0
	.set	macro
	.set	reorder

$L109:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L114
	li	$5,50			# 0x32
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L111
	move	$4,$17
	.set	macro
	.set	reorder

$L108:
	jal	rtl_ppp_xmit_unlock
	.set	noreorder
	.set	nomacro
	j	$L89
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L110:
	jal	consume_skb
	.set	noreorder
	.set	nomacro
	j	$L89
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L91:
	lui	$4,%hi($LC3)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L89
	move	$2,$0
	.set	macro
	.set	reorder

	.end	fast_pptp_to_wan
	.size	fast_pptp_to_wan, .-fast_pptp_to_wan
	.section	.text.fast_pptp_sync_rx_seq,"ax",@progbits
	.align	2
	.globl	fast_pptp_sync_rx_seq
	.set	nomips16
	.ent	fast_pptp_sync_rx_seq
	.type	fast_pptp_sync_rx_seq, @function
fast_pptp_sync_rx_seq:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	nop

	li	$4,1			# 0x1
	lbu	$3,21($2)
	andi	$5,$3,0x7
	beq	$5,$4,$L118
	li	$4,34827			# 0x880b

$L115:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

$L118:
	lhu	$5,22($2)
	bne	$5,$4,$L115
	nop

	lbu	$4,20($2)
	andi	$4,$4,0x10
	beq	$4,$0,$L119
	sll	$3,$3,24

	lui	$3,%hi(pptpAccInfo)
	addiu	$3,$3,%lo(pptpAccInfo)
	lw	$4,52($3)
	addiu	$5,$4,1
	sw	$4,28($2)
	sw	$5,52($3)
	lbu	$3,21($2)
	sll	$3,$3,24
$L119:
	sra	$3,$3,24
	bgez	$3,$L115
	nop

	lui	$3,%hi(pptpAccInfo+48)
	lw	$3,%lo(pptpAccInfo+48)($3)
	j	$L115
	sw	$3,32($2)

	.set	macro
	.set	reorder
	.end	fast_pptp_sync_rx_seq
	.size	fast_pptp_sync_rx_seq, .-fast_pptp_sync_rx_seq
	.section	.text.pptpconn_write_proc,"ax",@progbits
	.align	2
	.globl	pptpconn_write_proc
	.set	nomips16
	.ent	pptpconn_write_proc
	.type	pptpconn_write_proc, @function
pptpconn_write_proc:
	.frame	$sp,208,$31		# vars= 200, regs= 1/0, args= 0, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-208
	sw	$31,204($sp)
	sltu	$4,$6,2
	bne	$4,$0,$L121
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L121
	addu	$2,$5,$6
	.set	macro
	.set	reorder

	or	$4,$5,$6
	or	$2,$4,$2
	lw	$4,24($28)
	and	$2,$2,$4
	bne	$2,$0,$L121
	move	$4,$sp
 #APP
 # 928 "net/rtl/fastpath/fast_pptp_core.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$6,$0,$L121
	lb	$2,0($sp)
	addiu	$3,$2,-48
	andi	$3,$3,0x00ff
	sltu	$3,$3,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L124
	move	$3,$0
	.set	macro
	.set	reorder

	addiu	$4,$sp,1
$L123:
	sll	$5,$3,1
	sll	$3,$3,3
	addu	$3,$5,$3
	addu	$3,$3,$2
	lb	$2,0($4)
	addiu	$3,$3,-48
	addiu	$5,$2,-48
	andi	$5,$5,0x00ff
	sltu	$5,$5,10
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L123
	addiu	$4,$4,1
	.set	macro
	.set	reorder

	lui	$2,%hi(pptp_conn_check)
	sw	$3,%lo(pptp_conn_check)($2)
$L121:
	lw	$31,204($sp)
	li	$2,-14			# 0xfffffffffffffff2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,208
	.set	macro
	.set	reorder

$L124:
	lui	$2,%hi(pptp_conn_check)
	.set	noreorder
	.set	nomacro
	j	$L121
	sw	$3,%lo(pptp_conn_check)($2)
	.set	macro
	.set	reorder

	.end	pptpconn_write_proc
	.size	pptpconn_write_proc, .-pptpconn_write_proc
	.section	.text.pptp_conn_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	pptp_conn_single_write
	.type	pptp_conn_single_write, @function
pptp_conn_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	pptpconn_write_proc
	nop

	.set	macro
	.set	reorder
	.end	pptp_conn_single_write
	.size	pptp_conn_single_write, .-pptp_conn_single_write
	.section	.text.ppfw_write_proc,"ax",@progbits
	.align	2
	.globl	ppfw_write_proc
	.set	nomips16
	.ent	ppfw_write_proc
	.type	ppfw_write_proc, @function
ppfw_write_proc:
	.frame	$sp,32,$31		# vars= 0, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	sltu	$2,$6,2
	move	$17,$6
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L134
	move	$18,$5
	.set	macro
	.set	reorder

	addiu	$4,$6,32
	.set	noreorder
	.set	nomacro
	jal	__kmalloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L134
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L129
	or	$3,$18,$17
	.set	macro
	.set	reorder

	addu	$2,$18,$17
	or	$2,$3,$2
	lw	$3,24($28)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L129
	move	$4,$16
	.set	macro
	.set	reorder

	move	$5,$18
	move	$6,$17
 #APP
 # 954 "net/rtl/fastpath/fast_pptp_core.c" 1
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
	bne	$6,$0,$L140
	move	$4,$16
	.set	macro
	.set	reorder

	lb	$3,0($16)
	addiu	$2,$3,-48
	andi	$2,$2,0x00ff
	sltu	$2,$2,10
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L135
	addiu	$4,$16,1
	.set	macro
	.set	reorder

	move	$2,$0
$L131:
	sll	$5,$2,1
	sll	$2,$2,3
	addu	$2,$5,$2
	addu	$2,$2,$3
	lb	$3,0($4)
	addiu	$2,$2,-48
	addiu	$5,$3,-48
	andi	$5,$5,0x00ff
	sltu	$5,$5,10
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L131
	addiu	$4,$4,1
	.set	macro
	.set	reorder

$L130:
	lui	$4,%hi(pptpAccInfo)
	lui	$18,%hi(fast_pptp_fw)
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$2,%lo(fast_pptp_fw)($18)
	.set	macro
	.set	reorder

	lw	$2,%lo(fast_pptp_fw)($18)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L138
	lui	$3,%hi(sync_tx_pptp_gre_seqno)
	.set	macro
	.set	reorder

	move	$3,$0
$L132:
	lui	$2,%hi(sync_tx_pptp_gre_seqno_hook)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	kfree
	sw	$3,%lo(sync_tx_pptp_gre_seqno_hook)($2)
	.set	macro
	.set	reorder

$L128:
	move	$2,$17
$L139:
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

$L138:
	.set	noreorder
	.set	nomacro
	j	$L132
	addiu	$3,$3,%lo(sync_tx_pptp_gre_seqno)
	.set	macro
	.set	reorder

$L135:
	.set	noreorder
	.set	nomacro
	j	$L130
	move	$2,$0
	.set	macro
	.set	reorder

$L129:
	move	$4,$16
$L140:
	.set	noreorder
	.set	nomacro
	jal	kfree
	li	$17,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L139
	move	$2,$17
	.set	macro
	.set	reorder

$L134:
	.set	noreorder
	.set	nomacro
	j	$L128
	li	$17,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	ppfw_write_proc
	.size	ppfw_write_proc, .-ppfw_write_proc
	.section	.text.fastpath_pptp_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	fastpath_pptp_single_write
	.type	fastpath_pptp_single_write, @function
fastpath_pptp_single_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	j	ppfw_write_proc
	nop

	.set	macro
	.set	reorder
	.end	fastpath_pptp_single_write
	.size	fastpath_pptp_single_write, .-fastpath_pptp_single_write
	.section	.rodata.str1.4
	.align	2
$LC5:
	.ascii	"fast_pptp\000"
	.align	2
$LC6:
	.ascii	"pptp_conn_ck\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_init
	.set	nomips16
	.ent	fast_pptp_init
	.type	fast_pptp_init, @function
fast_pptp_init:
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
	lui	$7,%hi(fastpath_pptp_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$4,$4,%lo($LC5)
	move	$5,$0
	addiu	$7,$7,%lo(fastpath_pptp_proc_fops)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi($LC6)
	lui	$7,%hi(pptp_conn_proc_fops)
	addiu	$6,$16,%lo(proc_root)
	addiu	$7,$7,%lo(pptp_conn_proc_fops)
	lui	$3,%hi(res1)
	addiu	$4,$4,%lo($LC6)
	move	$5,$0
	sw	$2,%lo(res1)($3)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$4,%hi(pptpAccInfo)
	lui	$3,%hi(res_check_pptp)
	addiu	$4,$4,%lo(pptpAccInfo)
	move	$5,$0
	li	$6,64			# 0x40
	jal	memset
	sw	$2,%lo(res_check_pptp)($3)

	move	$2,$0
	lw	$31,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	fast_pptp_init
	.size	fast_pptp_init, .-fast_pptp_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	fast_pptp_exit
	.set	nomips16
	.ent	fast_pptp_exit
	.type	fast_pptp_exit, @function
fast_pptp_exit:
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
	beq	$2,$0,$L144
	lui	$4,%hi($LC5)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC5)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res1)($16)
$L144:
	lui	$16,%hi(res_check_pptp)
	lw	$2,%lo(res_check_pptp)($16)
	beq	$2,$0,$L143
	lui	$4,%hi($LC6)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC6)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res_check_pptp)($16)
$L143:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	fast_pptp_exit
	.size	fast_pptp_exit, .-fast_pptp_exit
	.globl	pptp_conn_proc_fops
	.section	.data.pptp_conn_proc_fops,"aw",@progbits
	.align	2
	.type	pptp_conn_proc_fops, @object
	.size	pptp_conn_proc_fops, 108
pptp_conn_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	pptp_conn_single_write
	.space	28
	.word	pptp_conn_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fastpath_pptp_proc_fops
	.section	.data.fastpath_pptp_proc_fops,"aw",@progbits
	.align	2
	.type	fastpath_pptp_proc_fops, @object
	.size	fastpath_pptp_proc_fops, 108
fastpath_pptp_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	fastpath_pptp_single_write
	.space	28
	.word	fastpath_pptp_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	fast_pptp_fw
	.section	.bss.fast_pptp_fw,"aw",@nobits
	.align	2
	.type	fast_pptp_fw, @object
	.size	fast_pptp_fw, 4
fast_pptp_fw:
	.space	4
	.globl	pptp_tcp_finished
	.section	.bss.pptp_tcp_finished,"aw",@nobits
	.align	2
	.type	pptp_tcp_finished, @object
	.size	pptp_tcp_finished, 4
pptp_tcp_finished:
	.space	4
	.section	.bss.pptpAccInfo,"aw",@nobits
	.align	2
	.type	pptpAccInfo, @object
	.size	pptpAccInfo, 64
pptpAccInfo:
	.space	64
	.section	.bss.pptp_conn_check,"aw",@nobits
	.align	2
	.type	pptp_conn_check, @object
	.size	pptp_conn_check, 4
pptp_conn_check:
	.space	4
	.section	.bss.res1,"aw",@nobits
	.align	2
	.type	res1, @object
	.size	res1, 4
res1:
	.space	4
	.section	.bss.res_check_pptp,"aw",@nobits
	.align	2
	.type	res_check_pptp, @object
	.size	res_check_pptp, 4
res_check_pptp:
	.space	4
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
