	.file	1 "filter_ipv6.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.clear_attack_address_ipv6,"ax",@progbits
	.align	2
	.globl	clear_attack_address_ipv6
	.set	nomips16
	.ent	clear_attack_address_ipv6
	.type	clear_attack_address_ipv6, @function
clear_attack_address_ipv6:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$4,%hi(attack_v6_saddr)
	addiu	$4,$4,%lo(attack_v6_saddr)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	lw	$31,20($sp)
	lui	$4,%hi(attack_v6_daddr)
	addiu	$4,$4,%lo(attack_v6_daddr)
	move	$5,$0
	li	$6,64			# 0x40
	j	memset
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	clear_attack_address_ipv6
	.size	clear_attack_address_ipv6, .-clear_attack_address_ipv6
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"DoS: ipv6 Smurf Attack source=%04x:%04x:%04x:%04x:%04x:%"
	.ascii	"04x:%04x:%04x destination=%04x:%04x:%04x:%04x:%04x:%04x:"
	.ascii	"%04x:%04x\012\000"
	.align	2
$LC1:
	.ascii	"DoS: ipv6 icmp too big Attack source=%04x:%04x:%04x:%04x"
	.ascii	":%04x:%04x:%04x:%04x destination=%04x:%04x:%04x:%04x:%04"
	.ascii	"x:%04x:%04x:%04x\012\000"
	.align	2
$LC2:
	.ascii	"DoS: ipv6 neighbor solication Attack source=%04x:%04x:%0"
	.ascii	"4x:%04x:%04x:%04x:%04x:%04x destination=%04x:%04x:%04x:%"
	.ascii	"04x:%04x:%04x:%04x:%04x\012\000"
	.section	.text.dos_timer_ipv6_handler,"ax",@progbits
	.align	2
	.globl	dos_timer_ipv6_handler
	.set	nomips16
	.ent	dos_timer_ipv6_handler
	.type	dos_timer_ipv6_handler, @function
dos_timer_ipv6_handler:
	.frame	$sp,96,$31		# vars= 0, regs= 5/0, args= 72, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$19,88($sp)
	sw	$18,84($sp)
	sw	$17,80($sp)
	sw	$16,76($sp)
	lui	$2,%hi(dos_item_ipv6)
	lw	$11,%lo(dos_item_ipv6)($2)
	andi	$2,$11,0xe
	bne	$2,$0,$L4
	lui	$18,%hi(dos_pkt)

	lui	$17,%hi(LogIpv6Flag)
	lw	$16,%lo(LogIpv6Flag)($17)
	lui	$19,%hi(smurf_pkts)
$L5:
	andi	$2,$16,0x2
	bne	$2,$0,$L40
	lui	$4,%hi(attack_v6_saddr)

	andi	$2,$16,0x4
$L44:
	bne	$2,$0,$L41
	lui	$4,%hi(attack_v6_saddr)

$L24:
	andi	$16,$16,0x8
$L39:
	beq	$16,$0,$L42
	addiu	$4,$18,%lo(dos_pkt)

	lui	$4,%hi(attack_v6_saddr)
	lui	$3,%hi(attack_v6_daddr)
	addiu	$4,$4,%lo(attack_v6_saddr)
	addiu	$3,$3,%lo(attack_v6_daddr)
	lhu	$5,48($4)
	lhu	$6,50($4)
	lhu	$7,52($4)
	lhu	$14,62($4)
	lhu	$2,62($3)
	lhu	$16,54($4)
	lhu	$25,56($4)
	lhu	$24,58($4)
	lhu	$15,60($4)
	lhu	$13,48($3)
	lhu	$12,50($3)
	lhu	$11,52($3)
	lhu	$10,54($3)
	lhu	$9,56($3)
	lhu	$8,58($3)
	lhu	$3,60($3)
	lui	$4,%hi($LC2)
	addiu	$4,$4,%lo($LC2)
	sw	$16,16($sp)
	sw	$25,20($sp)
	sw	$24,24($sp)
	sw	$15,28($sp)
	sw	$14,32($sp)
	sw	$13,36($sp)
	sw	$12,40($sp)
	sw	$11,44($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$3,60($sp)
	jal	scrlog_printk
	sw	$2,64($sp)

	addiu	$4,$18,%lo(dos_pkt)
$L42:
	move	$5,$0
	li	$6,32768			# 0x8000
	jal	memset
	sw	$0,%lo(LogIpv6Flag)($17)

	addiu	$4,$19,%lo(smurf_pkts)
	lw	$31,92($sp)
	lw	$19,88($sp)
	lw	$18,84($sp)
	lw	$17,80($sp)
	lw	$16,76($sp)
	move	$5,$0
	li	$6,32			# 0x20
	j	memset
	addiu	$sp,$sp,96

$L4:
	addiu	$2,$18,%lo(dos_pkt)
	lb	$3,0($2)
	lui	$5,%hi(dos_pkt+32736)
	li	$4,1			# 0x1
	addiu	$5,$5,%lo(dos_pkt+32736)
	move	$6,$0
	move	$7,$0
	andi	$9,$11,0x4
	beq	$3,$4,$L37
	andi	$8,$11,0x8

$L6:
	beq	$2,$5,$L8
	nop

$L38:
	addiu	$2,$2,32
	lb	$3,0($2)
	bne	$3,$4,$L6
	nop

$L37:
	beq	$9,$0,$L7
	nop

	lw	$3,8($2)
	slt	$10,$0,$3
	addu	$3,$7,$3
	movn	$7,$3,$10
$L7:
	beq	$8,$0,$L6
	nop

	lw	$3,12($2)
	slt	$10,$0,$3
	addu	$3,$6,$3
	bne	$2,$5,$L38
	movn	$6,$3,$10

$L8:
	andi	$2,$11,0x2
	bne	$2,$0,$L10
	lui	$19,%hi(smurf_pkts)

	move	$4,$0
$L11:
	lui	$3,%hi(Ipv6_smurf_threshold)
	lw	$3,%lo(Ipv6_smurf_threshold)($3)
	slt	$4,$3,$4
	beq	$4,$0,$L43
	lui	$17,%hi(LogIpv6Flag)

	bne	$2,$0,$L14
	nop

$L29:
	lui	$17,%hi(LogIpv6Flag)
$L43:
	lw	$16,%lo(LogIpv6Flag)($17)
$L13:
	lui	$2,%hi(Ipv6_icmp_too_big_threshold)
	lw	$2,%lo(Ipv6_icmp_too_big_threshold)($2)
	slt	$7,$2,$7
	beq	$7,$0,$L31
	andi	$3,$11,0x4

	bne	$3,$0,$L18
	nop

$L31:
	lw	$16,%lo(LogIpv6Flag)($17)
$L17:
	lui	$2,%hi(Ipv6_ns_threshold)
	lw	$2,%lo(Ipv6_ns_threshold)($2)
	slt	$6,$2,$6
	beq	$6,$0,$L33
	andi	$11,$11,0x8

	beq	$11,$0,$L33
	nop

	blez	$2,$L33
	ori	$16,$16,0x8

	j	$L5
	sw	$16,%lo(LogIpv6Flag)($17)

$L10:
	addiu	$3,$19,%lo(smurf_pkts)
	lw	$3,4($3)
	slt	$4,$3,0
	movn	$3,$0,$4
	j	$L11
	move	$4,$3

$L33:
	lw	$16,%lo(LogIpv6Flag)($17)
	andi	$2,$16,0x2
	beq	$2,$0,$L44
	andi	$2,$16,0x4

	lui	$4,%hi(attack_v6_saddr)
$L40:
	lui	$3,%hi(attack_v6_daddr)
	addiu	$4,$4,%lo(attack_v6_saddr)
	addiu	$3,$3,%lo(attack_v6_daddr)
	lhu	$5,16($4)
	lhu	$6,18($4)
	lhu	$7,20($4)
	lhu	$14,30($4)
	lhu	$2,30($3)
	lhu	$31,22($4)
	lhu	$25,24($4)
	lhu	$24,26($4)
	lhu	$15,28($4)
	lhu	$13,16($3)
	lhu	$12,18($3)
	lhu	$11,20($3)
	lhu	$10,22($3)
	lhu	$9,24($3)
	lhu	$8,26($3)
	lhu	$3,28($3)
	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	sw	$31,16($sp)
	sw	$25,20($sp)
	sw	$24,24($sp)
	sw	$15,28($sp)
	sw	$14,32($sp)
	sw	$13,36($sp)
	sw	$12,40($sp)
	sw	$11,44($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$3,60($sp)
	jal	scrlog_printk
	sw	$2,64($sp)

	andi	$2,$16,0x4
	beq	$2,$0,$L24
	lui	$4,%hi(attack_v6_saddr)

$L41:
	lui	$3,%hi(attack_v6_daddr)
	addiu	$4,$4,%lo(attack_v6_saddr)
	addiu	$3,$3,%lo(attack_v6_daddr)
	lhu	$5,32($4)
	lhu	$6,34($4)
	lhu	$7,36($4)
	lhu	$14,46($4)
	lhu	$31,38($4)
	lhu	$25,40($4)
	lhu	$24,42($4)
	lhu	$15,44($4)
	lhu	$13,32($3)
	lhu	$12,34($3)
	lhu	$11,36($3)
	lhu	$10,38($3)
	lhu	$9,40($3)
	lhu	$8,42($3)
	lhu	$2,46($3)
	lhu	$3,44($3)
	lui	$4,%hi($LC1)
	addiu	$4,$4,%lo($LC1)
	sw	$31,16($sp)
	sw	$25,20($sp)
	sw	$24,24($sp)
	sw	$15,28($sp)
	sw	$14,32($sp)
	sw	$13,36($sp)
	sw	$12,40($sp)
	sw	$11,44($sp)
	sw	$10,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$3,60($sp)
	jal	scrlog_printk
	sw	$2,64($sp)

	j	$L39
	andi	$16,$16,0x8

$L18:
	blez	$2,$L31
	ori	$16,$16,0x4

	j	$L17
	sw	$16,%lo(LogIpv6Flag)($17)

$L14:
	blez	$3,$L29
	lui	$17,%hi(LogIpv6Flag)

	lw	$16,%lo(LogIpv6Flag)($17)
	ori	$16,$16,0x2
	j	$L13
	sw	$16,%lo(LogIpv6Flag)($17)

	.set	macro
	.set	reorder
	.end	dos_timer_ipv6_handler
	.size	dos_timer_ipv6_handler, .-dos_timer_ipv6_handler
	.section	.text._rtl_get_ipv6_skip_exthdr,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	_rtl_get_ipv6_skip_exthdr
	.type	_rtl_get_ipv6_skip_exthdr, @function
_rtl_get_ipv6_skip_exthdr:
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
	move	$18,$4
	bne	$4,$0,$L52
	andi	$16,$5,0x00ff

$L46:
	move	$2,$0
$L49:
	lw	$31,36($sp)
$L54:
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

$L52:
	lw	$17,184($4)
	beq	$17,$0,$L49
	move	$2,$0

	lw	$3,0($17)
	li	$2,-268435456			# 0xfffffffff0000000
	and	$3,$3,$2
	li	$2,1610612736			# 0x60000000
	bne	$3,$2,$L49
	move	$2,$0

	lbu	$2,6($17)
	move	$4,$2
	jal	ipv6_ext_hdr
	sb	$2,16($sp)

	bne	$2,$0,$L53
	lbu	$2,16($sp)

	bne	$2,$16,$L46
	li	$2,40			# 0x28

	j	$L49
	addu	$2,$17,$2

$L53:
	move	$4,$18
	li	$5,40			# 0x28
	addiu	$6,$sp,16
	jal	ipv6_skip_exthdr
	addiu	$7,$sp,18

	bltz	$2,$L46
	lbu	$3,16($sp)

	bne	$3,$16,$L46
	addu	$2,$17,$2

	j	$L54
	lw	$31,36($sp)

	.set	macro
	.set	reorder
	.end	_rtl_get_ipv6_skip_exthdr
	.size	_rtl_get_ipv6_skip_exthdr, .-_rtl_get_ipv6_skip_exthdr
	.section	.text.rtl_get_tcpv6_header,"ax",@progbits
	.align	2
	.globl	rtl_get_tcpv6_header
	.set	nomips16
	.ent	rtl_get_tcpv6_header
	.type	rtl_get_tcpv6_header, @function
rtl_get_tcpv6_header:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	li	$5,6			# 0x6
	j	_rtl_get_ipv6_skip_exthdr
	nop

	.set	macro
	.set	reorder
	.end	rtl_get_tcpv6_header
	.size	rtl_get_tcpv6_header, .-rtl_get_tcpv6_header
	.section	.text.dos_filter_ipv6__action,"ax",@progbits
	.align	2
	.globl	dos_filter_ipv6__action
	.set	nomips16
	.ent	dos_filter_ipv6__action
	.type	dos_filter_ipv6__action, @function
dos_filter_ipv6__action:
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
	lui	$17,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($17)
	move	$18,$4
	andi	$3,$2,0xe
	beq	$3,$0,$L59
	move	$16,$0

	andi	$3,$2,0x2
	bne	$3,$0,$L76
	nop

$L61:
	andi	$2,$2,0x8
	bne	$2,$0,$L67
	move	$4,$18

$L75:
	lw	$20,184($18)
$L68:
	lw	$2,20($20)
	lui	$19,%hi(dos_pkt)
	addiu	$19,$19,%lo(dos_pkt)
	andi	$2,$2,0x3ff
	sll	$2,$2,5
	addu	$19,$19,$2
	addiu	$20,$20,8
	addiu	$21,$19,16
	move	$4,$20
	move	$5,$21
	jal	memcmp
	li	$6,16			# 0x10

	beq	$2,$0,$L85
	move	$4,$20

	lb	$2,0($19)
	beq	$2,$0,$L77
	move	$5,$20

$L85:
	move	$5,$21
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L84
	move	$2,$16

	lb	$3,0($19)
	li	$2,1			# 0x1
	beq	$3,$2,$L78
	lui	$20,%hi(cur_p_pkt)

$L59:
	move	$2,$16
$L84:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L67:
$L83:
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	beq	$2,$0,$L68
	lw	$20,184($18)

	lbu	$3,0($2)
	li	$2,135			# 0x87
	bne	$3,$2,$L68
	lui	$2,%hi(LogIpv6Flag)

	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x8
	beq	$4,$0,$L86
	lui	$5,%hi(ipv6Ns_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	beq	$5,$4,$L79
	lui	$5,%hi(ipv6Ns_pkts)

$L86:
	addiu	$5,$5,%lo(ipv6Ns_pkts)
	lw	$4,12($5)
	lui	$6,%hi(Ipv6_ns_threshold)
	lw	$6,%lo(Ipv6_ns_threshold)($6)
	addiu	$4,$4,1
	sw	$4,12($5)
	slt	$4,$6,$4
	beq	$4,$0,$L68
	nop

	blez	$6,$L68
	lui	$4,%hi(attack_v6_saddr+48)

	ori	$3,$3,0x8
	addiu	$5,$20,8
	li	$8,1			# 0x1
	lui	$7,%hi(dosFilter_block)
	addiu	$4,$4,%lo(attack_v6_saddr+48)
	li	$6,16			# 0x10
	sw	$8,%lo(dosFilter_block)($7)
	jal	memcpy
	sw	$3,%lo(LogIpv6Flag)($2)

	lui	$4,%hi(attack_v6_daddr+48)
	addiu	$5,$20,24
	addiu	$4,$4,%lo(attack_v6_daddr+48)
	jal	memcpy
	li	$6,16			# 0x10

	j	$L68
	lw	$20,184($18)

$L76:
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	beq	$2,$0,$L74
	lw	$16,184($18)

	lbu	$3,0($2)
	li	$2,129			# 0x81
	beq	$3,$2,$L63
	lui	$2,%hi(LogIpv6Flag)

$L74:
	lw	$2,%lo(dos_item_ipv6)($17)
	move	$16,$0
$L82:
	andi	$2,$2,0x8
	beq	$2,$0,$L75
	move	$4,$18

	j	$L83
	nop

$L78:
	sw	$19,%lo(cur_p_pkt)($20)
$L71:
	lw	$2,%lo(dos_item_ipv6)($17)
	andi	$2,$2,0x4
	beq	$2,$0,$L84
	move	$2,$16

	move	$4,$18
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	lw	$18,184($18)
	beq	$2,$0,$L59
	lw	$17,%lo(cur_p_pkt)($20)

	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L84
	move	$2,$16

	lui	$19,%hi(dosFilter_block)
	lw	$3,%lo(dosFilter_block)($19)
	li	$2,1			# 0x1
	beq	$3,$2,$L80
	lui	$4,%hi(attack_v6_saddr+32)

$L72:
	lw	$2,8($17)
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	addiu	$2,$2,1
	sw	$2,8($17)
	lw	$3,%lo(Ipv6_icmp_too_big_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L84
	move	$2,$16

	blez	$3,$L59
	lui	$2,%hi(LogIpv6Flag)

	lw	$3,%lo(LogIpv6Flag)($2)
	lui	$4,%hi(attack_v6_saddr+32)
	addiu	$5,$18,8
	ori	$3,$3,0x4
	li	$7,1			# 0x1
	addiu	$4,$4,%lo(attack_v6_saddr+32)
	li	$6,16			# 0x10
	sw	$7,%lo(dosFilter_block)($19)
	jal	memcpy
	sw	$3,%lo(LogIpv6Flag)($2)

	lui	$4,%hi(attack_v6_daddr+32)
	addiu	$4,$4,%lo(attack_v6_daddr+32)
	addiu	$5,$18,24
	jal	memcpy
	li	$6,16			# 0x10

	j	$L84
	move	$2,$16

$L77:
	move	$4,$21
	jal	memcpy
	li	$6,16			# 0x10

	lui	$20,%hi(cur_p_pkt)
	li	$2,1			# 0x1
	sb	$2,0($19)
	j	$L71
	sw	$19,%lo(cur_p_pkt)($20)

$L63:
	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x2
	beq	$4,$0,$L87
	lui	$5,%hi(smurf_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	beq	$5,$4,$L81
	lui	$5,%hi(smurf_pkts)

$L87:
	addiu	$5,$5,%lo(smurf_pkts)
	lw	$4,4($5)
	lui	$6,%hi(Ipv6_smurf_threshold)
	lw	$6,%lo(Ipv6_smurf_threshold)($6)
	addiu	$4,$4,1
	sw	$4,4($5)
	slt	$4,$6,$4
	beq	$4,$0,$L74
	nop

	blez	$6,$L74
	lui	$4,%hi(attack_v6_saddr+16)

	ori	$3,$3,0x2
	addiu	$5,$16,8
	li	$8,1			# 0x1
	lui	$7,%hi(dosFilter_block)
	addiu	$4,$4,%lo(attack_v6_saddr+16)
	li	$6,16			# 0x10
	sw	$8,%lo(dosFilter_block)($7)
	jal	memcpy
	sw	$3,%lo(LogIpv6Flag)($2)

	lui	$4,%hi(attack_v6_daddr+16)
	addiu	$5,$16,24
	addiu	$4,$4,%lo(attack_v6_daddr+16)
	jal	memcpy
	li	$6,16			# 0x10

	move	$16,$0
	j	$L82
	lw	$2,%lo(dos_item_ipv6)($17)

$L79:
	j	$L68
	li	$16,-1			# 0xffffffffffffffff

$L81:
	lw	$2,%lo(dos_item_ipv6)($17)
	j	$L61
	li	$16,-1			# 0xffffffffffffffff

$L80:
	addiu	$4,$4,%lo(attack_v6_saddr+32)
	addiu	$5,$18,8
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L72
	nop

	j	$L59
	li	$16,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	dos_filter_ipv6__action
	.size	dos_filter_ipv6__action, .-dos_filter_ipv6__action
	.section	.bss.dos_pkt,"aw",@nobits
	.align	2
	.type	dos_pkt, @object
	.size	dos_pkt, 32768
dos_pkt:
	.space	32768
	.section	.bss.cur_p_pkt,"aw",@nobits
	.align	2
	.type	cur_p_pkt, @object
	.size	cur_p_pkt, 4
cur_p_pkt:
	.space	4
	.section	.bss.attack_v6_saddr,"aw",@nobits
	.align	2
	.type	attack_v6_saddr, @object
	.size	attack_v6_saddr, 64
attack_v6_saddr:
	.space	64
	.section	.bss.attack_v6_daddr,"aw",@nobits
	.align	2
	.type	attack_v6_daddr, @object
	.size	attack_v6_daddr, 64
attack_v6_daddr:
	.space	64
	.section	.bss.LogIpv6Flag,"aw",@nobits
	.align	2
	.type	LogIpv6Flag, @object
	.size	LogIpv6Flag, 4
LogIpv6Flag:
	.space	4
	.section	.bss.smurf_pkts,"aw",@nobits
	.align	2
	.type	smurf_pkts, @object
	.size	smurf_pkts, 32
smurf_pkts:
	.space	32
	.section	.bss.ipv6Ns_pkts,"aw",@nobits
	.align	2
	.type	ipv6Ns_pkts, @object
	.size	ipv6Ns_pkts, 32
ipv6Ns_pkts:
	.space	32
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 1459) 4.4.7"
