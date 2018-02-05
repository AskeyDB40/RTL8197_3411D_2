	.file	1 "filter_ipv6.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
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
	beq	$4,$0,$L10
	move	$17,$5

	lw	$16,184($4)
	beq	$16,$0,$L10
	li	$2,-268435456			# 0xfffffffff0000000

	lw	$3,0($16)
	and	$3,$3,$2
	li	$2,1610612736			# 0x60000000
	bne	$3,$2,$L10
	nop

	lbu	$2,6($16)
	move	$4,$2
	jal	ipv6_ext_hdr
	sb	$2,16($sp)

	bne	$2,$0,$L12
	move	$4,$18

	lbu	$2,16($sp)
	bne	$2,$17,$L10
	li	$2,40			# 0x28

	addu	$2,$16,$2
$L13:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

$L12:
	li	$5,40			# 0x28
	addiu	$6,$sp,16
	jal	ipv6_skip_exthdr
	addiu	$7,$sp,18

	bltz	$2,$L10
	nop

	lbu	$3,16($sp)
	beq	$3,$17,$L13
	addu	$2,$16,$2

$L10:
	lw	$31,36($sp)
	move	$2,$0
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

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
	lui	$19,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($19)
	andi	$3,$2,0xe
	beq	$3,$0,$L28
	move	$17,$4

	andi	$3,$2,0x2
	bne	$3,$0,$L45
	andi	$2,$2,0x8

	bne	$2,$0,$L20
	move	$16,$0

$L49:
	lw	$20,184($17)
$L21:
	lw	$2,20($20)
	lui	$18,%hi(dos_pkt)
	addiu	$18,$18,%lo(dos_pkt)
	addiu	$20,$20,8
	andi	$2,$2,0x3ff
	li	$6,16			# 0x10
	sll	$2,$2,5
	addu	$18,$18,$2
	move	$4,$20
	addiu	$21,$18,16
	jal	memcmp
	move	$5,$21

	beq	$2,$0,$L56
	move	$4,$20

	lb	$2,0($18)
	beq	$2,$0,$L46
	move	$5,$20

$L56:
	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$21

	bne	$2,$0,$L16
	li	$2,1			# 0x1

	lb	$3,0($18)
	beq	$3,$2,$L47
	lui	$20,%hi(cur_p_pkt)

$L16:
	move	$2,$16
$L57:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L28:
	move	$16,$0
	move	$2,$16
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L45:
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	lw	$18,184($17)
	beq	$2,$0,$L34
	nop

	lbu	$3,0($2)
	li	$2,129			# 0x81
	beq	$3,$2,$L48
	lui	$2,%hi(LogIpv6Flag)

$L34:
	lw	$2,%lo(dos_item_ipv6)($19)
	move	$16,$0
$L52:
	andi	$2,$2,0x8
$L55:
	beq	$2,$0,$L49
	nop

$L20:
	move	$4,$17
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	lw	$20,184($17)
	beq	$2,$0,$L39
	nop

	lbu	$3,0($2)
	li	$2,135			# 0x87
	beq	$3,$2,$L50
	lui	$2,%hi(LogIpv6Flag)

$L39:
	move	$2,$0
	j	$L21
	or	$16,$16,$2

$L47:
	sw	$18,%lo(cur_p_pkt)($20)
$L25:
	lw	$2,%lo(dos_item_ipv6)($19)
	andi	$2,$2,0x4
	beq	$2,$0,$L57
	move	$2,$16

	move	$4,$17
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	lw	$18,184($17)
	lw	$17,%lo(cur_p_pkt)($20)
	beq	$2,$0,$L44
	nop

	lbu	$3,0($2)
	li	$2,2			# 0x2
	beq	$3,$2,$L51
	lui	$19,%hi(dosFilter_block)

$L44:
	move	$2,$0
	or	$16,$16,$2
$L54:
	move	$2,$16
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L48:
	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x2
	beq	$4,$0,$L58
	lui	$4,%hi(smurf_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	bne	$5,$4,$L19
	lui	$4,%hi(smurf_pkts)

	lw	$2,%lo(dos_item_ipv6)($19)
	j	$L52
	li	$16,-1			# 0xffffffffffffffff

$L19:
$L58:
	lui	$6,%hi(Ipv6_smurf_threshold)
	addiu	$4,$4,%lo(smurf_pkts)
	lw	$5,4($4)
	lw	$6,%lo(Ipv6_smurf_threshold)($6)
	addiu	$5,$5,1
	slt	$7,$6,$5
	beq	$7,$0,$L34
	sw	$5,4($4)

	blez	$6,$L34
	lui	$4,%hi(attack_v6_saddr+16)

	ori	$3,$3,0x2
	li	$8,1			# 0x1
	lui	$7,%hi(dosFilter_block)
	addiu	$4,$4,%lo(attack_v6_saddr+16)
	addiu	$5,$18,8
	li	$6,16			# 0x10
	move	$16,$0
	sw	$8,%lo(dosFilter_block)($7)
	jal	memcpy
	sw	$3,%lo(LogIpv6Flag)($2)

	lui	$4,%hi(attack_v6_daddr+16)
	addiu	$4,$4,%lo(attack_v6_daddr+16)
	addiu	$5,$18,24
	jal	memcpy
	li	$6,16			# 0x10

	lw	$2,%lo(dos_item_ipv6)($19)
	j	$L55
	andi	$2,$2,0x8

$L50:
	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x8
	beq	$4,$0,$L59
	lui	$4,%hi(ipv6Ns_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	bne	$5,$4,$L23
	lui	$4,%hi(ipv6Ns_pkts)

	li	$2,-1			# 0xffffffffffffffff
	j	$L21
	or	$16,$16,$2

$L23:
$L59:
	lui	$6,%hi(Ipv6_ns_threshold)
	addiu	$4,$4,%lo(ipv6Ns_pkts)
	lw	$5,12($4)
	lw	$6,%lo(Ipv6_ns_threshold)($6)
	addiu	$5,$5,1
	slt	$7,$6,$5
	beq	$7,$0,$L39
	sw	$5,12($4)

	blez	$6,$L39
	lui	$4,%hi(attack_v6_saddr+48)

	addiu	$5,$20,8
	ori	$3,$3,0x8
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

	move	$2,$0
	lw	$20,184($17)
	j	$L21
	or	$16,$16,$2

$L46:
	li	$6,16			# 0x10
	move	$4,$21
	jal	memcpy
	lui	$20,%hi(cur_p_pkt)

	li	$2,1			# 0x1
	sb	$2,0($18)
	j	$L25
	sw	$18,%lo(cur_p_pkt)($20)

$L51:
	li	$2,1			# 0x1
	lw	$3,%lo(dosFilter_block)($19)
	beq	$3,$2,$L53
	lui	$4,%hi(attack_v6_saddr+32)

$L27:
	lw	$2,8($17)
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	lw	$3,%lo(Ipv6_icmp_too_big_threshold)($3)
	addiu	$2,$2,1
	slt	$4,$3,$2
	beq	$4,$0,$L44
	sw	$2,8($17)

	blez	$3,$L44
	lui	$2,%hi(LogIpv6Flag)

	lui	$4,%hi(attack_v6_saddr+32)
	lw	$3,%lo(LogIpv6Flag)($2)
	li	$7,1			# 0x1
	addiu	$4,$4,%lo(attack_v6_saddr+32)
	addiu	$5,$18,8
	ori	$3,$3,0x4
	li	$6,16			# 0x10
	sw	$7,%lo(dosFilter_block)($19)
	jal	memcpy
	sw	$3,%lo(LogIpv6Flag)($2)

	lui	$4,%hi(attack_v6_daddr+32)
	addiu	$4,$4,%lo(attack_v6_daddr+32)
	addiu	$5,$18,24
	jal	memcpy
	li	$6,16			# 0x10

	move	$2,$0
	j	$L54
	or	$16,$16,$2

$L53:
	addiu	$5,$18,8
	addiu	$4,$4,%lo(attack_v6_saddr+32)
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L27
	li	$2,-1			# 0xffffffffffffffff

	j	$L54
	or	$16,$16,$2

	.set	macro
	.set	reorder
	.end	dos_filter_ipv6__action
	.size	dos_filter_ipv6__action, .-dos_filter_ipv6__action
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
	.frame	$sp,96,$31		# vars= 0, regs= 6/0, args= 72, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	lui	$12,%hi(dos_item_ipv6)
	lw	$11,%lo(dos_item_ipv6)($12)
	andi	$2,$11,0xe
	beq	$2,$0,$L79
	lui	$18,%hi(dos_pkt)

	li	$5,32768			# 0x8000
	addiu	$2,$18,%lo(dos_pkt)
	move	$6,$0
	addu	$5,$2,$5
	move	$7,$0
	li	$4,1			# 0x1
	andi	$9,$11,0x4
	j	$L65
	andi	$8,$11,0x8

$L63:
	addiu	$2,$2,32
	beq	$2,$5,$L80
	nop

$L65:
	lb	$3,0($2)
	bne	$3,$4,$L63
	nop

	beq	$9,$0,$L64
	nop

	lw	$3,8($2)
	addu	$10,$7,$3
	slt	$3,$0,$3
	movn	$7,$10,$3
$L64:
	beq	$8,$0,$L63
	nop

	lw	$3,12($2)
	addiu	$2,$2,32
	addu	$10,$6,$3
	slt	$3,$0,$3
	bne	$2,$5,$L65
	movn	$6,$10,$3

$L80:
	andi	$11,$11,0x2
	bne	$11,$0,$L81
	lui	$19,%hi(smurf_pkts)

	move	$3,$0
$L66:
	lui	$2,%hi(Ipv6_smurf_threshold)
	lw	$2,%lo(Ipv6_smurf_threshold)($2)
	slt	$3,$2,$3
	beq	$3,$0,$L86
	lui	$17,%hi(LogIpv6Flag)

	bne	$11,$0,$L69
	nop

$L86:
	lw	$16,%lo(LogIpv6Flag)($17)
	lui	$2,%hi(Ipv6_icmp_too_big_threshold)
$L91:
	lw	$2,%lo(Ipv6_icmp_too_big_threshold)($2)
	slt	$7,$2,$7
	beq	$7,$0,$L71
	nop

	lw	$3,%lo(dos_item_ipv6)($12)
	andi	$3,$3,0x4
	bne	$3,$0,$L82
	nop

$L71:
	lui	$2,%hi(Ipv6_ns_threshold)
$L90:
	lw	$2,%lo(Ipv6_ns_threshold)($2)
	slt	$6,$2,$6
	beq	$6,$0,$L62
	nop

	lw	$3,%lo(dos_item_ipv6)($12)
	andi	$3,$3,0x8
	beq	$3,$0,$L62
	nop

	blez	$2,$L87
	andi	$2,$16,0x2

	ori	$16,$16,0x8
	sw	$16,%lo(LogIpv6Flag)($17)
$L62:
	andi	$2,$16,0x2
$L87:
	bne	$2,$0,$L88
	lui	$3,%hi(attack_v6_saddr)

$L72:
	andi	$2,$16,0x4
	bne	$2,$0,$L89
	lui	$3,%hi(attack_v6_saddr)

$L73:
	andi	$16,$16,0x8
$L85:
	beq	$16,$0,$L74
	lui	$2,%hi(attack_v6_daddr)

	lui	$3,%hi(attack_v6_saddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lhu	$5,48($3)
	lui	$4,%hi($LC2)
	lhu	$6,50($3)
	addiu	$4,$4,%lo($LC2)
	lhu	$7,52($3)
	lhu	$25,54($3)
	lhu	$24,56($3)
	lhu	$16,58($3)
	lhu	$15,60($3)
	lhu	$14,62($3)
	lhu	$13,48($2)
	lhu	$12,50($2)
	lhu	$11,52($2)
	lhu	$10,54($2)
	lhu	$9,56($2)
	lhu	$8,58($2)
	lhu	$3,60($2)
	lhu	$2,62($2)
	sw	$25,16($sp)
	sw	$24,20($sp)
	sw	$16,24($sp)
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

$L74:
	addiu	$4,$18,%lo(dos_pkt)
	move	$5,$0
	li	$6,32768			# 0x8000
	jal	memset
	sw	$0,%lo(LogIpv6Flag)($17)

	addiu	$4,$19,%lo(smurf_pkts)
	lw	$31,92($sp)
	move	$5,$0
	lw	$20,88($sp)
	li	$6,32			# 0x20
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	j	memset
	addiu	$sp,$sp,96

$L81:
	addiu	$2,$19,%lo(smurf_pkts)
	lw	$2,4($2)
	slt	$3,$2,0
	movn	$2,$0,$3
	j	$L66
	move	$3,$2

$L79:
	lui	$17,%hi(LogIpv6Flag)
	lw	$16,%lo(LogIpv6Flag)($17)
	andi	$2,$16,0x2
	beq	$2,$0,$L72
	lui	$19,%hi(smurf_pkts)

	lui	$3,%hi(attack_v6_saddr)
$L88:
	lui	$2,%hi(attack_v6_daddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lhu	$5,16($3)
	lui	$4,%hi($LC0)
	lhu	$6,18($3)
	addiu	$4,$4,%lo($LC0)
	lhu	$7,20($3)
	lhu	$25,22($3)
	lhu	$24,24($3)
	lhu	$20,26($3)
	lhu	$15,28($3)
	lhu	$14,30($3)
	lhu	$13,16($2)
	lhu	$12,18($2)
	lhu	$11,20($2)
	lhu	$10,22($2)
	lhu	$9,24($2)
	lhu	$8,26($2)
	lhu	$3,28($2)
	lhu	$2,30($2)
	sw	$25,16($sp)
	sw	$24,20($sp)
	sw	$20,24($sp)
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
	beq	$2,$0,$L73
	lui	$3,%hi(attack_v6_saddr)

$L89:
	lui	$2,%hi(attack_v6_daddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lhu	$5,32($3)
	lui	$4,%hi($LC1)
	lhu	$6,34($3)
	addiu	$4,$4,%lo($LC1)
	lhu	$7,36($3)
	lhu	$20,38($3)
	lhu	$25,40($3)
	lhu	$24,42($3)
	lhu	$15,44($3)
	lhu	$14,46($3)
	lhu	$13,32($2)
	lhu	$12,34($2)
	lhu	$11,36($2)
	lhu	$10,38($2)
	lhu	$9,40($2)
	lhu	$8,42($2)
	lhu	$3,44($2)
	lhu	$2,46($2)
	sw	$20,16($sp)
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

	j	$L85
	andi	$16,$16,0x8

$L82:
	blez	$2,$L90
	lui	$2,%hi(Ipv6_ns_threshold)

	ori	$16,$16,0x4
	j	$L90
	sw	$16,%lo(LogIpv6Flag)($17)

$L69:
	lw	$16,%lo(LogIpv6Flag)($17)
	blez	$2,$L91
	lui	$2,%hi(Ipv6_icmp_too_big_threshold)

	ori	$16,$16,0x2
	j	$L91
	sw	$16,%lo(LogIpv6Flag)($17)

	.set	macro
	.set	reorder
	.end	dos_timer_ipv6_handler
	.size	dos_timer_ipv6_handler, .-dos_timer_ipv6_handler
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
	move	$5,$0
	addiu	$4,$4,%lo(attack_v6_saddr)
	jal	memset
	li	$6,64			# 0x40

	lui	$4,%hi(attack_v6_daddr)
	lw	$31,20($sp)
	addiu	$4,$4,%lo(attack_v6_daddr)
	move	$5,$0
	li	$6,64			# 0x40
	j	memset
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	clear_attack_address_ipv6
	.size	clear_attack_address_ipv6, .-clear_attack_address_ipv6
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
	.section	.bss.ipv6Ns_pkts,"aw",@nobits
	.align	2
	.type	ipv6Ns_pkts, @object
	.size	ipv6Ns_pkts, 32
ipv6Ns_pkts:
	.space	32
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
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
