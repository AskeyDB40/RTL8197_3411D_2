	.file	1 "filter_ipv4.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.clear_attack_address_ipv4,"ax",@progbits
	.align	2
	.globl	clear_attack_address_ipv4
	.set	nomips16
	.ent	clear_attack_address_ipv4
	.type	clear_attack_address_ipv4, @function
clear_attack_address_ipv4:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$7,%hi(attack_saddr)
	lui	$6,%hi(attack_daddr)
	addiu	$7,$7,%lo(attack_saddr)
	addiu	$6,$6,%lo(attack_daddr)
	move	$2,$0
	li	$5,108			# 0x6c
$L2:
	addu	$4,$7,$2
	addu	$3,$6,$2
	addiu	$2,$2,4
	sw	$0,0($4)
	bne	$2,$5,$L2
	sw	$0,0($3)

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	clear_attack_address_ipv4
	.size	clear_attack_address_ipv4, .-clear_attack_address_ipv4
	.section	.text.dos_ipv4_init,"ax",@progbits
	.align	2
	.globl	dos_ipv4_init
	.set	nomips16
	.ent	dos_ipv4_init
	.type	dos_ipv4_init, @function
dos_ipv4_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(_tcpDosScanBitmap)
	addiu	$4,$16,%lo(_tcpDosScanBitmap)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	li	$2,1			# 0x1
	addiu	$3,$16,%lo(_tcpDosScanBitmap)
	sb	$2,%lo(_tcpDosScanBitmap)($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	sb	$2,3($3)
	sb	$2,63($3)
	sb	$2,58($3)
	sb	$2,41($3)
	sb	$2,40($3)
	sb	$2,33($3)
	sb	$2,32($3)
	sb	$2,9($3)
	sb	$2,8($3)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	dos_ipv4_init
	.size	dos_ipv4_init, .-dos_ipv4_init
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"DoS: Whole System SYN Flood Attack\012\000"
	.align	2
$LC1:
	.ascii	"DoS: Whole System FIN Flood Attack\012\000"
	.align	2
$LC2:
	.ascii	"DoS: Whole System UDP Flood Attack\012\000"
	.align	2
$LC3:
	.ascii	"DoS: Whole System ICMP Flood Attack\012\000"
	.align	2
$LC4:
	.ascii	"DoS: Per-source SYN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC5:
	.ascii	"DoS: Per-source FIN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC6:
	.ascii	"DoS: Per-source UDP Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC7:
	.ascii	"DoS: Per-source ICMP Flood Attack source=%u.%u.%u.%u des"
	.ascii	"tination=%u.%u.%u.%u\012\000"
	.align	2
$LC8:
	.ascii	"DoS: Port Scan Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC9:
	.ascii	"DoS: Tcp Scan Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.align	2
$LC10:
	.ascii	"DoS: Tcp SYN With Data Attack source=%u.%u.%u.%u destina"
	.ascii	"tion=%u.%u.%u.%u\012\000"
	.align	2
$LC11:
	.ascii	"DoS: IP Land Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC12:
	.ascii	"DoS: UdpEchoChargen Attack source=%u.%u.%u.%u destinatio"
	.ascii	"n=%u.%u.%u.%u\012\000"
	.align	2
$LC13:
	.ascii	"DoS: UdpBomb Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC14:
	.ascii	"DoS: PingOfDeath Attack source=%u.%u.%u.%u destination=%"
	.ascii	"u.%u.%u.%u\012\000"
	.align	2
$LC15:
	.ascii	"DoS: IcmpSmurf Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC16:
	.ascii	"DoS: IpSpoof Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC17:
	.ascii	"DoS: TearDrop Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.section	.text.dos_timer_ipv4_handler,"ax",@progbits
	.align	2
	.globl	dos_timer_ipv4_handler
	.set	nomips16
	.ent	dos_timer_ipv4_handler
	.type	dos_timer_ipv4_handler, @function
dos_timer_ipv4_handler:
	.frame	$sp,56,$31		# vars= 0, regs= 4/0, args= 40, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$2,%hi(dos_item_global)
	lw	$15,%lo(dos_item_global)($2)
	andi	$2,$15,0x1e
	bne	$2,$0,$L9
	lui	$18,%hi(dos_pkt)

	lui	$17,%hi(LogFlag)
	lw	$16,%lo(LogFlag)($17)
$L10:
	andi	$2,$16,0x2
	bne	$2,$0,$L83
	lui	$4,%hi($LC0)

	andi	$2,$16,0x4
$L103:
	bne	$2,$0,$L84
	lui	$4,%hi($LC1)

	andi	$2,$16,0x8
$L104:
	bne	$2,$0,$L85
	lui	$4,%hi($LC2)

	andi	$2,$16,0x10
$L105:
	bne	$2,$0,$L86
	lui	$4,%hi($LC3)

	andi	$2,$16,0x20
$L106:
	bne	$2,$0,$L87
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x40
$L107:
	bne	$2,$0,$L88
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x80
$L108:
	bne	$2,$0,$L89
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x100
$L109:
	bne	$2,$0,$L90
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x200
$L110:
	bne	$2,$0,$L91
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x8000
$L111:
	bne	$2,$0,$L92
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,16,1
$L112:
	bne	$2,$0,$L93
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x800
$L113:
	bne	$2,$0,$L94
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,18,1
$L114:
	bne	$2,$0,$L95
	lui	$2,%hi(attack_saddr)

	ext	$2,$16,17,1
$L115:
	bne	$2,$0,$L96
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x4000
$L116:
	bne	$2,$0,$L97
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x400
$L117:
	bne	$2,$0,$L98
	lui	$2,%hi(attack_saddr)

	andi	$2,$16,0x1000
$L118:
	bne	$2,$0,$L99
	lui	$2,%hi(attack_saddr)

$L48:
	andi	$16,$16,0x2000
$L82:
	beq	$16,$0,$L100
	addiu	$4,$18,%lo(dos_pkt)

	lui	$2,%hi(attack_saddr)
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,41($2)
	lbu	$7,42($2)
	lbu	$10,43($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+40)
	lui	$2,%hi(attack_daddr+40)
	lbu	$8,41($3)
	lbu	$9,%lo(attack_daddr+40)($2)
	lbu	$5,%lo(attack_saddr+40)($4)
	lbu	$2,43($3)
	lbu	$3,42($3)
	lui	$4,%hi($LC17)
	addiu	$4,$4,%lo($LC17)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	addiu	$4,$18,%lo(dos_pkt)
$L100:
	move	$5,$0
	li	$6,40960			# 0xa000
	jal	memset
	sw	$0,%lo(LogFlag)($17)

	lui	$2,%hi(icmp_echo_request_time)
	lw	$2,%lo(icmp_echo_request_time)($2)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addiu	$2,$2,100
	subu	$2,$2,$3
	bgez	$2,$L101
	lw	$31,52($sp)

	lui	$2,%hi(icmp_echo_request)
	sw	$0,%lo(icmp_echo_request)($2)
	lw	$31,52($sp)
$L101:
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,56

$L9:
	addiu	$2,$18,%lo(dos_pkt)
	lb	$3,0($2)
	lui	$5,%hi(dos_pkt+40920)
	li	$4,1			# 0x1
	addiu	$5,$5,%lo(dos_pkt+40920)
	move	$6,$0
	move	$7,$0
	move	$8,$0
	move	$9,$0
	andi	$13,$15,0x2
	andi	$12,$15,0x4
	andi	$11,$15,0x8
	beq	$3,$4,$L80
	andi	$10,$15,0x10

$L11:
	beq	$2,$5,$L15
	nop

$L81:
	addiu	$2,$2,40
	lb	$3,0($2)
	bne	$3,$4,$L11
	nop

$L80:
	beq	$13,$0,$L12
	nop

	lw	$3,4($2)
	slt	$14,$0,$3
	addu	$3,$9,$3
	movn	$9,$3,$14
$L12:
	beq	$12,$0,$L13
	nop

	lw	$3,8($2)
	slt	$14,$0,$3
	addu	$3,$8,$3
	movn	$8,$3,$14
$L13:
	beq	$11,$0,$L14
	nop

	lw	$3,12($2)
	slt	$14,$0,$3
	addu	$3,$7,$3
	movn	$7,$3,$14
$L14:
	beq	$10,$0,$L11
	nop

	lw	$3,16($2)
	slt	$14,$0,$3
	addu	$3,$6,$3
	bne	$2,$5,$L81
	movn	$6,$3,$14

$L15:
	lui	$2,%hi(whole_syn_threshold)
	lw	$2,%lo(whole_syn_threshold)($2)
	slt	$9,$2,$9
	bne	$9,$0,$L17
	lui	$17,%hi(LogFlag)

$L102:
	lw	$16,%lo(LogFlag)($17)
$L18:
	lui	$2,%hi(whole_fin_threshold)
	lw	$2,%lo(whole_fin_threshold)($2)
	slt	$8,$2,$8
	beq	$8,$0,$L58
	andi	$3,$15,0x4

	bne	$3,$0,$L23
	nop

$L58:
	lw	$16,%lo(LogFlag)($17)
$L22:
	lui	$2,%hi(whole_udp_threshold)
	lw	$2,%lo(whole_udp_threshold)($2)
	slt	$7,$2,$7
	beq	$7,$0,$L60
	andi	$3,$15,0x8

	bne	$3,$0,$L27
	nop

$L60:
	lw	$16,%lo(LogFlag)($17)
$L26:
	lui	$2,%hi(whole_icmp_threshold)
	lw	$2,%lo(whole_icmp_threshold)($2)
	slt	$6,$2,$6
	beq	$6,$0,$L62
	andi	$15,$15,0x10

	beq	$15,$0,$L62
	nop

	blez	$2,$L62
	ori	$16,$16,0x10

	j	$L10
	sw	$16,%lo(LogFlag)($17)

$L17:
	andi	$3,$15,0x2
	beq	$3,$0,$L102
	nop

	blez	$2,$L102
	lw	$16,%lo(LogFlag)($17)

	ori	$16,$16,0x2
	j	$L18
	sw	$16,%lo(LogFlag)($17)

$L62:
	lw	$16,%lo(LogFlag)($17)
	andi	$2,$16,0x2
	beq	$2,$0,$L103
	andi	$2,$16,0x4

	lui	$4,%hi($LC0)
$L83:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC0)

	andi	$2,$16,0x4
	beq	$2,$0,$L104
	andi	$2,$16,0x8

	lui	$4,%hi($LC1)
$L84:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)

	andi	$2,$16,0x8
	beq	$2,$0,$L105
	andi	$2,$16,0x10

	lui	$4,%hi($LC2)
$L85:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC2)

	andi	$2,$16,0x10
	beq	$2,$0,$L106
	andi	$2,$16,0x20

	lui	$4,%hi($LC3)
$L86:
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)

	andi	$2,$16,0x20
	beq	$2,$0,$L107
	andi	$2,$16,0x40

	lui	$2,%hi(attack_saddr)
$L87:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,5($2)
	lbu	$7,6($2)
	lbu	$10,7($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+4)
	lui	$2,%hi(attack_daddr+4)
	lbu	$8,5($3)
	lbu	$9,%lo(attack_daddr+4)($2)
	lbu	$5,%lo(attack_saddr+4)($4)
	lbu	$2,7($3)
	lbu	$3,6($3)
	lui	$4,%hi($LC4)
	addiu	$4,$4,%lo($LC4)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x40
	beq	$2,$0,$L108
	andi	$2,$16,0x80

	lui	$2,%hi(attack_saddr)
$L88:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$10,11($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+8)
	lui	$2,%hi(attack_daddr+8)
	lbu	$8,9($3)
	lbu	$9,%lo(attack_daddr+8)($2)
	lbu	$5,%lo(attack_saddr+8)($4)
	lbu	$2,11($3)
	lbu	$3,10($3)
	lui	$4,%hi($LC5)
	addiu	$4,$4,%lo($LC5)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x80
	beq	$2,$0,$L109
	andi	$2,$16,0x100

	lui	$2,%hi(attack_saddr)
$L89:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,13($2)
	lbu	$7,14($2)
	lbu	$10,15($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+12)
	lui	$2,%hi(attack_daddr+12)
	lbu	$8,13($3)
	lbu	$9,%lo(attack_daddr+12)($2)
	lbu	$5,%lo(attack_saddr+12)($4)
	lbu	$2,15($3)
	lbu	$3,14($3)
	lui	$4,%hi($LC6)
	addiu	$4,$4,%lo($LC6)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x100
	beq	$2,$0,$L110
	andi	$2,$16,0x200

	lui	$2,%hi(attack_saddr)
$L90:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,17($2)
	lbu	$7,18($2)
	lbu	$10,19($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+16)
	lui	$2,%hi(attack_daddr+16)
	lbu	$8,17($3)
	lbu	$9,%lo(attack_daddr+16)($2)
	lbu	$5,%lo(attack_saddr+16)($4)
	lbu	$2,19($3)
	lbu	$3,18($3)
	lui	$4,%hi($LC7)
	addiu	$4,$4,%lo($LC7)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x200
	beq	$2,$0,$L111
	andi	$2,$16,0x8000

	lui	$2,%hi(attack_saddr)
$L91:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,25($2)
	lbu	$7,26($2)
	lbu	$10,27($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+24)
	lui	$2,%hi(attack_daddr+24)
	lbu	$8,25($3)
	lbu	$9,%lo(attack_daddr+24)($2)
	lbu	$5,%lo(attack_saddr+24)($4)
	lbu	$2,27($3)
	lbu	$3,26($3)
	lui	$4,%hi($LC8)
	addiu	$4,$4,%lo($LC8)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x8000
	beq	$2,$0,$L112
	ext	$2,$16,16,1

	lui	$2,%hi(attack_saddr)
$L92:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,49($2)
	lbu	$7,50($2)
	lbu	$10,51($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+48)
	lui	$2,%hi(attack_daddr+48)
	lbu	$8,49($3)
	lbu	$9,%lo(attack_daddr+48)($2)
	lbu	$5,%lo(attack_saddr+48)($4)
	lbu	$2,51($3)
	lbu	$3,50($3)
	lui	$4,%hi($LC9)
	addiu	$4,$4,%lo($LC9)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,16,1
	beq	$2,$0,$L113
	andi	$2,$16,0x800

	lui	$2,%hi(attack_saddr)
$L93:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,53($2)
	lbu	$7,54($2)
	lbu	$10,55($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+52)
	lui	$2,%hi(attack_daddr+52)
	lbu	$8,53($3)
	lbu	$9,%lo(attack_daddr+52)($2)
	lbu	$5,%lo(attack_saddr+52)($4)
	lbu	$2,55($3)
	lbu	$3,54($3)
	lui	$4,%hi($LC10)
	addiu	$4,$4,%lo($LC10)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x800
	beq	$2,$0,$L114
	ext	$2,$16,18,1

	lui	$2,%hi(attack_saddr)
$L94:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,33($2)
	lbu	$7,34($2)
	lbu	$10,35($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+32)
	lui	$2,%hi(attack_daddr+32)
	lbu	$8,33($3)
	lbu	$9,%lo(attack_daddr+32)($2)
	lbu	$5,%lo(attack_saddr+32)($4)
	lbu	$2,35($3)
	lbu	$3,34($3)
	lui	$4,%hi($LC11)
	addiu	$4,$4,%lo($LC11)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,18,1
	beq	$2,$0,$L115
	ext	$2,$16,17,1

	lui	$2,%hi(attack_saddr)
$L95:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,61($2)
	lbu	$7,62($2)
	lbu	$10,63($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+60)
	lui	$2,%hi(attack_daddr+60)
	lbu	$8,61($3)
	lbu	$9,%lo(attack_daddr+60)($2)
	lbu	$5,%lo(attack_saddr+60)($4)
	lbu	$2,63($3)
	lbu	$3,62($3)
	lui	$4,%hi($LC12)
	addiu	$4,$4,%lo($LC12)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	ext	$2,$16,17,1
	beq	$2,$0,$L116
	andi	$2,$16,0x4000

	lui	$2,%hi(attack_saddr)
$L96:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,57($2)
	lbu	$7,58($2)
	lbu	$10,59($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+56)
	lui	$2,%hi(attack_daddr+56)
	lbu	$8,57($3)
	lbu	$9,%lo(attack_daddr+56)($2)
	lbu	$5,%lo(attack_saddr+56)($4)
	lbu	$2,59($3)
	lbu	$3,58($3)
	lui	$4,%hi($LC13)
	addiu	$4,$4,%lo($LC13)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x4000
	beq	$2,$0,$L117
	andi	$2,$16,0x400

	lui	$2,%hi(attack_saddr)
$L97:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,45($2)
	lbu	$7,46($2)
	lbu	$10,47($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+44)
	lui	$2,%hi(attack_daddr+44)
	lbu	$8,45($3)
	lbu	$9,%lo(attack_daddr+44)($2)
	lbu	$5,%lo(attack_saddr+44)($4)
	lbu	$2,47($3)
	lbu	$3,46($3)
	lui	$4,%hi($LC14)
	addiu	$4,$4,%lo($LC14)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x400
	beq	$2,$0,$L118
	andi	$2,$16,0x1000

	lui	$2,%hi(attack_saddr)
$L98:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,29($2)
	lbu	$7,30($2)
	lbu	$10,31($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+28)
	lui	$2,%hi(attack_daddr+28)
	lbu	$8,29($3)
	lbu	$9,%lo(attack_daddr+28)($2)
	lbu	$5,%lo(attack_saddr+28)($4)
	lbu	$2,31($3)
	lbu	$3,30($3)
	lui	$4,%hi($LC15)
	addiu	$4,$4,%lo($LC15)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x1000
	beq	$2,$0,$L48
	lui	$2,%hi(attack_saddr)

$L99:
	addiu	$2,$2,%lo(attack_saddr)
	lui	$3,%hi(attack_daddr)
	lbu	$6,37($2)
	lbu	$7,38($2)
	lbu	$10,39($2)
	addiu	$3,$3,%lo(attack_daddr)
	lui	$4,%hi(attack_saddr+36)
	lui	$2,%hi(attack_daddr+36)
	lbu	$8,37($3)
	lbu	$9,%lo(attack_daddr+36)($2)
	lbu	$5,%lo(attack_saddr+36)($4)
	lbu	$2,39($3)
	lbu	$3,38($3)
	lui	$4,%hi($LC16)
	addiu	$4,$4,%lo($LC16)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	j	$L82
	andi	$16,$16,0x2000

$L27:
	blez	$2,$L60
	ori	$16,$16,0x8

	j	$L26
	sw	$16,%lo(LogFlag)($17)

$L23:
	blez	$2,$L58
	ori	$16,$16,0x4

	j	$L22
	sw	$16,%lo(LogFlag)($17)

	.set	macro
	.set	reorder
	.end	dos_timer_ipv4_handler
	.size	dos_timer_ipv4_handler, .-dos_timer_ipv4_handler
	.section	.text.dos_filter_ipv4__action,"ax",@progbits
	.align	2
	.globl	dos_filter_ipv4__action
	.set	nomips16
	.ent	dos_filter_ipv4__action
	.type	dos_filter_ipv4__action, @function
dos_filter_ipv4__action:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	nop

	lw	$6,0($2)
	lui	$5,%hi(dos_item_global)
	move	$4,$2
	lw	$2,%lo(dos_item_global)($5)
	ext	$6,$6,24,4
	sll	$6,$6,2
	andi	$2,$2,0x3fe
	beq	$2,$0,$L120
	addu	$6,$4,$6

	lw	$2,12($4)
	lui	$3,%hi(ConnectedIp)
	addiu	$3,$3,%lo(ConnectedIp)
	andi	$7,$2,0xff
	sll	$7,$7,2
	addu	$3,$7,$3
	lw	$8,0($3)
	andi	$7,$2,0x3ff
	sll	$9,$7,5
	lui	$3,%hi(dos_pkt)
	sll	$7,$7,3
	addu	$7,$7,$9
	addiu	$3,$3,%lo(dos_pkt)
	beq	$2,$8,$L240
	addu	$3,$3,$7

	lw	$7,28($3)
	beq	$2,$7,$L123
	nop

$L122:
	lb	$7,0($3)
	bne	$7,$0,$L120
	lui	$7,%hi(cur_p_pkt)

	sw	$2,28($3)
	li	$2,1			# 0x1
	sb	$2,0($3)
	sw	$3,%lo(cur_p_pkt)($7)
$L124:
	lw	$3,%lo(dos_item_global)($5)
	andi	$2,$3,0x22
	beq	$2,$0,$L127
	move	$2,$0

	lbu	$8,9($4)
	li	$2,6			# 0x6
	beq	$8,$2,$L241
	lw	$10,%lo(cur_p_pkt)($7)

	move	$2,$0
$L127:
	andi	$8,$3,0x44
$L278:
	beq	$8,$0,$L279
	andi	$8,$3,0x88

	lbu	$9,9($4)
	li	$8,6			# 0x6
	beq	$9,$8,$L242
	lw	$10,%lo(cur_p_pkt)($7)

$L132:
	lw	$3,%lo(dos_item_global)($5)
$L311:
	andi	$8,$3,0x88
$L279:
	beq	$8,$0,$L280
	andi	$8,$3,0x110

	lbu	$9,9($4)
	li	$8,17			# 0x11
	beq	$9,$8,$L243
	lw	$10,%lo(cur_p_pkt)($7)

$L135:
	lw	$3,%lo(dos_item_global)($5)
$L310:
	andi	$8,$3,0x110
$L280:
	beq	$8,$0,$L281
	li	$8,3145728			# 0x300000

	lbu	$8,9($4)
	li	$9,1			# 0x1
	beq	$8,$9,$L244
	lw	$10,%lo(cur_p_pkt)($7)

$L138:
	lw	$3,%lo(dos_item_global)($5)
$L309:
	li	$8,3145728			# 0x300000
$L281:
	and	$8,$3,$8
	beq	$8,$0,$L282
	andi	$8,$3,0x200

	lw	$8,0($4)
	lbu	$9,9($4)
	ext	$11,$8,24,4
	li	$8,1			# 0x1
	beq	$9,$8,$L245
	lw	$10,%lo(cur_p_pkt)($7)

$L141:
	lw	$3,%lo(dos_item_global)($5)
$L308:
	andi	$8,$3,0x200
$L282:
	beq	$8,$0,$L283
	ext	$3,$3,22,1

	lbu	$3,9($4)
	li	$8,6			# 0x6
	beq	$3,$8,$L230
	lw	$7,%lo(cur_p_pkt)($7)

	lw	$8,24($7)
$L145:
	li	$9,17			# 0x11
	beq	$3,$9,$L246
	nop

$L148:
	lw	$3,%lo(dos_item_global)($5)
$L312:
	ext	$7,$3,23,1
	beq	$7,$0,$L151
	nop

	slt	$8,$8,11
	bne	$8,$0,$L144
	lui	$7,%hi(LogFlag)

	lw	$8,%lo(LogFlag)($7)
$L236:
	ori	$8,$8,0x200
	sw	$8,%lo(LogFlag)($7)
	lw	$8,12($4)
	lui	$7,%hi(attack_saddr+24)
	sw	$8,%lo(attack_saddr+24)($7)
	lw	$8,16($4)
	lui	$7,%hi(attack_daddr+24)
	sw	$8,%lo(attack_daddr+24)($7)
$L144:
	ext	$3,$3,22,1
$L283:
	bne	$3,$0,$L284
	lui	$7,%hi(dos_item_global_0)

$L120:
	move	$2,$0
$L306:
	lui	$7,%hi(dos_item_global_0)
$L284:
	lw	$3,%lo(dos_item_global_0)($7)
	andi	$8,$3,0x1
	beq	$8,$0,$L285
	andi	$3,$3,0x2

	lbu	$8,9($4)
	li	$3,6			# 0x6
	beq	$8,$3,$L247
	nop

$L155:
	lw	$3,%lo(dos_item_global_0)($7)
	andi	$3,$3,0x2
$L285:
	beq	$3,$0,$L286
	lw	$3,%lo(dos_item_global)($5)

	lbu	$3,9($4)
	li	$7,17			# 0x11
	beq	$3,$7,$L169
	li	$7,6			# 0x6

	beq	$3,$7,$L169
	nop

$L168:
	lw	$3,%lo(dos_item_global)($5)
$L286:
	li	$7,536870912			# 0x20000000
	and	$8,$3,$7
	beq	$8,$0,$L171
	li	$8,1			# 0x1

	lbu	$3,9($4)
	beq	$3,$8,$L248
	nop

	li	$7,17			# 0x11
	beq	$3,$7,$L174
	li	$7,8			# 0x8

	li	$7,6			# 0x6
	beq	$3,$7,$L249
	li	$7,20			# 0x14

$L173:
	lw	$3,%lo(dos_item_global)($5)
$L171:
	li	$7,1073741824			# 0x40000000
	and	$8,$3,$7
	beq	$8,$0,$L176
	nop

	lw	$3,0($4)
	lbu	$8,9($4)
	ext	$9,$3,24,4
	li	$3,1			# 0x1
	beq	$8,$3,$L250
	li	$3,8			# 0x8

$L177:
	lw	$3,%lo(dos_item_global)($5)
$L176:
	bltz	$3,$L251
	nop

$L178:
	andi	$7,$3,0x8000
	beq	$7,$0,$L287
	li	$7,65536			# 0x10000

	lbu	$7,9($4)
	li	$3,6			# 0x6
	beq	$7,$3,$L252
	lui	$3,%hi(_tcpDosScanBitmap)

$L181:
	lw	$3,%lo(dos_item_global)($5)
$L302:
	li	$7,65536			# 0x10000
$L287:
	and	$8,$3,$7
	beq	$8,$0,$L182
	nop

	lbu	$8,9($4)
	li	$3,6			# 0x6
	beq	$8,$3,$L253
	li	$3,2			# 0x2

$L183:
	lw	$3,%lo(dos_item_global)($5)
$L182:
	andi	$7,$3,0x800
	beq	$7,$0,$L288
	ext	$7,$3,18,1

	lw	$7,12($4)
	lw	$3,16($4)
	beq	$7,$3,$L254
	nop

$L186:
	lw	$3,%lo(dos_item_global)($5)
	ext	$7,$3,18,1
$L288:
	beq	$7,$0,$L289
	li	$7,131072			# 0x20000

	lbu	$3,9($4)
	li	$7,17			# 0x11
	beq	$3,$7,$L255
	li	$8,7			# 0x7

$L188:
	lw	$3,%lo(dos_item_global)($5)
$L301:
	li	$7,131072			# 0x20000
$L289:
	and	$8,$3,$7
	beq	$8,$0,$L190
	nop

	lbu	$8,9($4)
	li	$3,17			# 0x11
	beq	$8,$3,$L256
	nop

$L191:
	lw	$3,%lo(dos_item_global)($5)
$L190:
	andi	$7,$3,0x4000
	beq	$7,$0,$L290
	andi	$7,$3,0x400

	lhu	$3,6($4)
	andi	$7,$3,0x2000
	beq	$7,$0,$L257
	andi	$3,$3,0x1fff

$L193:
	lw	$3,%lo(dos_item_global)($5)
$L299:
	andi	$7,$3,0x400
$L290:
	beq	$7,$0,$L291
	li	$7,524288			# 0x80000

	lw	$3,0($4)
	lbu	$7,9($4)
	ext	$8,$3,24,4
	li	$3,1			# 0x1
	beq	$7,$3,$L258
	sll	$8,$8,2

$L195:
	lw	$3,%lo(dos_item_global)($5)
$L300:
	li	$7,524288			# 0x80000
$L291:
	and	$8,$3,$7
	beq	$8,$0,$L196
	nop

	lbu	$8,9($4)
	li	$3,17			# 0x11
	beq	$8,$3,$L259
	lui	$3,%hi(dos_lan_addr)

$L197:
	lw	$3,%lo(dos_item_global)($5)
$L196:
	ext	$7,$3,24,1
	beq	$7,$0,$L292
	ext	$7,$3,25,1

	lw	$3,0($4)
	lbu	$7,9($4)
	ext	$8,$3,24,4
	li	$3,1			# 0x1
	beq	$7,$3,$L260
	li	$7,8			# 0x8

$L199:
	lw	$3,%lo(dos_item_global)($5)
$L303:
	ext	$7,$3,25,1
$L292:
	beq	$7,$0,$L293
	ext	$7,$3,26,1

	lbu	$7,9($4)
	li	$3,17			# 0x11
	beq	$7,$3,$L261
	li	$7,7			# 0x7

$L203:
	lw	$3,%lo(dos_item_global)($5)
$L304:
	ext	$7,$3,26,1
$L293:
	beq	$7,$0,$L294
	li	$7,134217728			# 0x8000000

	lbu	$7,9($4)
	li	$3,6			# 0x6
	beq	$7,$3,$L262
	nop

$L206:
	lw	$3,%lo(dos_item_global)($5)
$L305:
	li	$7,134217728			# 0x8000000
$L294:
	and	$8,$3,$7
	beq	$8,$0,$L208
	nop

	lbu	$8,9($4)
	li	$3,6			# 0x6
	beq	$8,$3,$L263
	nop

$L209:
	lw	$3,%lo(dos_item_global)($5)
$L208:
	ext	$6,$3,28,1
	beq	$6,$0,$L295
	andi	$6,$3,0x1000

	lw	$3,12($4)
	li	$7,-268435456			# 0xfffffffff0000000
	li	$6,-536870912			# 0xffffffffe0000000
	and	$7,$3,$7
	beq	$7,$6,$L211
	li	$6,-1			# 0xffffffffffffffff

	beq	$3,$6,$L211
	lui	$6,%hi(dos_lan_mask)

	lw	$6,%lo(dos_lan_mask)($6)
	nor	$6,$0,$6
	and	$3,$6,$3
	beq	$6,$3,$L211
	nop

	lw	$3,%lo(dos_item_global)($5)
$L210:
	andi	$6,$3,0x1000
$L295:
	beq	$6,$0,$L296
	andi	$3,$3,0x2000

	lui	$3,%hi(dos_lan_addr)
	lw	$7,%lo(dos_lan_addr)($3)
	lui	$6,%hi(dos_lan_mask)
	lw	$3,12($4)
	lw	$6,%lo(dos_lan_mask)($6)
	xor	$3,$7,$3
	and	$3,$3,$6
	beq	$3,$0,$L264
	li	$3,1			# 0x1

$L214:
	lw	$3,%lo(dos_item_global)($5)
$L298:
	andi	$3,$3,0x2000
$L296:
	beq	$3,$0,$L297
	lw	$31,20($sp)

	lw	$3,12($4)
	lui	$5,%hi(ConnectedIp)
	addiu	$5,$5,%lo(ConnectedIp)
	andi	$6,$3,0xff
	sll	$6,$6,2
	addu	$5,$6,$5
	lw	$7,0($5)
	andi	$6,$3,0x3ff
	sll	$8,$6,5
	lui	$5,%hi(dos_pkt)
	sll	$6,$6,3
	addu	$6,$6,$8
	addiu	$5,$5,%lo(dos_pkt)
	beq	$3,$7,$L265
	addu	$5,$5,$6

	lw	$6,28($5)
	beq	$3,$6,$L218
	nop

$L217:
	lb	$6,0($5)
	bne	$6,$0,$L297
	lw	$31,20($sp)

	sw	$3,28($5)
	li	$3,1			# 0x1
	sb	$3,0($5)
	lui	$3,%hi(cur_p_pkt)
	sw	$5,%lo(cur_p_pkt)($3)
$L219:
	lhu	$6,4($4)
	lhu	$3,32($5)
	beq	$6,$3,$L225
	nop

	bne	$3,$0,$L215
	nop

$L225:
	lhu	$3,6($4)
	andi	$7,$3,0x3fff
	beq	$7,$0,$L215
	nop

	lhu	$7,34($5)
	sll	$8,$3,3
	andi	$8,$8,0xffff
	slt	$8,$8,$7
	bne	$8,$0,$L220
	nop

	andi	$3,$3,0x2000
	bne	$3,$0,$L221
	nop

	sh	$0,34($5)
	sh	$0,32($5)
$L215:
	lw	$31,20($sp)
$L297:
	j	$31
	addiu	$sp,$sp,24

$L264:
	lbu	$6,9($4)
	beq	$6,$3,$L298
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x1000
	sw	$3,%lo(LogFlag)($2)
	lw	$6,12($4)
	lui	$3,%hi(attack_saddr+36)
	li	$2,-1			# 0xffffffffffffffff
	sw	$6,%lo(attack_saddr+36)($3)
	lw	$6,16($4)
	lui	$3,%hi(attack_daddr+36)
	j	$L214
	sw	$6,%lo(attack_daddr+36)($3)

$L257:
	beq	$3,$0,$L193
	sll	$3,$3,3

	lw	$8,0($4)
	lhu	$7,2($4)
	ext	$8,$8,24,4
	sll	$8,$8,2
	subu	$3,$3,$8
	addu	$3,$3,$7
	li	$7,65536			# 0x10000
	sltu	$3,$3,$7
	bne	$3,$0,$L299
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x4000
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+44)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+44)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+44)
	j	$L193
	sw	$7,%lo(attack_daddr+44)($3)

$L211:
	lui	$2,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($2)
	li	$3,268435456			# 0x10000000
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$6,12($4)
	lui	$3,%hi(attack_saddr+84)
	li	$2,-1			# 0xffffffffffffffff
	sw	$6,%lo(attack_saddr+84)($3)
	lw	$6,16($4)
	lui	$3,%hi(attack_daddr+84)
	sw	$6,%lo(attack_daddr+84)($3)
	j	$L210
	lw	$3,%lo(dos_item_global)($5)

$L169:
	lhu	$3,2($6)
	li	$7,7			# 0x7
	beq	$3,$7,$L170
	li	$8,17			# 0x11

	beq	$3,$8,$L170
	li	$9,19			# 0x13

	beq	$3,$9,$L170
	nop

	lhu	$3,0($6)
	beq	$3,$7,$L170
	nop

	beq	$3,$8,$L170
	nop

	bne	$3,$9,$L286
	lw	$3,%lo(dos_item_global)($5)

$L170:
	lw	$3,12($4)
	lw	$7,16($4)
	bne	$3,$7,$L168
	nop

	lui	$2,%hi(attack_saddr+104)
	sw	$3,%lo(attack_saddr+104)($2)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+104)
	li	$2,-1			# 0xffffffffffffffff
	j	$L168
	sw	$7,%lo(attack_daddr+104)($3)

$L251:
	lw	$3,0($4)
	lbu	$7,9($4)
	ext	$8,$3,24,4
	li	$3,1			# 0x1
	beq	$7,$3,$L266
	nop

$L179:
	j	$L178
	lw	$3,%lo(dos_item_global)($5)

$L265:
	lw	$6,28($5)
	bne	$3,$6,$L217
	nop

	j	$L215
	sb	$0,0($5)

$L247:
	lw	$3,0($4)
	ext	$3,$3,24,4
	sll	$3,$3,2
	addu	$3,$4,$3
	lbu	$11,12($3)
	andi	$11,$11,0xf0
	sra	$11,$11,2
	sltu	$8,$11,21
	bne	$8,$0,$L155
	lui	$12,%hi($L163)

	addiu	$3,$3,20
	addiu	$11,$11,-20
	addiu	$12,$12,%lo($L163)
	move	$8,$0
	li	$15,10			# 0xa
	li	$14,3			# 0x3
	li	$13,4			# 0x4
$L233:
	sltu	$9,$8,$11
	beq	$9,$0,$L155
	nop

	lbu	$9,0($3)
	sltu	$10,$9,9
	bne	$10,$0,$L267
	nop

$L157:
	lw	$3,12($4)
	lui	$2,%hi(attack_saddr+100)
	sw	$3,%lo(attack_saddr+100)($2)
	lw	$8,16($4)
	lui	$3,%hi(attack_daddr+100)
	li	$2,-1			# 0xffffffffffffffff
	j	$L155
	sw	$8,%lo(attack_daddr+100)($3)

$L258:
	addu	$8,$4,$8
	lbu	$7,0($8)
	li	$3,8			# 0x8
	bne	$7,$3,$L300
	lw	$3,%lo(dos_item_global)($5)

	lui	$3,%hi(dos_lan_addr)
	lw	$8,%lo(dos_lan_addr)($3)
	lui	$7,%hi(dos_lan_mask)
	lw	$3,12($4)
	lw	$7,%lo(dos_lan_mask)($7)
	xor	$3,$8,$3
	and	$3,$3,$7
	bne	$3,$0,$L300
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x400
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+28)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+28)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+28)
	j	$L195
	sw	$7,%lo(attack_daddr+28)($3)

$L255:
	lhu	$7,2($6)
	beq	$7,$8,$L189
	nop

	beq	$7,$3,$L189
	nop

	li	$9,19			# 0x13
	beq	$7,$9,$L189
	nop

	lhu	$7,0($6)
	beq	$7,$8,$L189
	nop

	beq	$7,$3,$L189
	nop

	bne	$7,$9,$L301
	lw	$3,%lo(dos_item_global)($5)

$L189:
	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	li	$3,262144			# 0x40000
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+60)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+60)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+60)
	j	$L188
	sw	$7,%lo(attack_daddr+60)($3)

$L256:
	lhu	$3,6($4)
	andi	$3,$3,0x3fff
	bne	$3,$0,$L190
	lw	$3,%lo(dos_item_global)($5)

	lw	$3,0($4)
	lhu	$9,2($4)
	lhu	$8,4($6)
	ext	$3,$3,24,4
	sll	$3,$3,2
	subu	$3,$9,$3
	slt	$3,$8,$3
	beq	$3,$0,$L190
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+56)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+56)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+56)
	j	$L191
	sw	$7,%lo(attack_daddr+56)($3)

$L240:
	lw	$7,28($3)
	bne	$2,$7,$L122
	nop

	j	$L120
	sb	$0,0($3)

$L252:
	lbu	$7,13($6)
	addiu	$3,$3,%lo(_tcpDosScanBitmap)
	andi	$7,$7,0x3f
	addu	$3,$7,$3
	lb	$3,0($3)
	beq	$3,$0,$L302
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x8000
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+48)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+48)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+48)
	j	$L181
	sw	$7,%lo(attack_daddr+48)($3)

$L253:
	lbu	$8,13($6)
	andi	$8,$8,0x3f
	bne	$8,$3,$L182
	lw	$3,%lo(dos_item_global)($5)

	lw	$8,0($4)
	lw	$3,12($6)
	lhu	$9,2($4)
	ext	$8,$8,24,4
	sll	$8,$8,2
	srl	$3,$3,28
	subu	$8,$9,$8
	sll	$3,$3,2
	beq	$8,$3,$L184
	nop

	lui	$2,%hi(LogFlag)
$L276:
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+52)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+52)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+52)
	j	$L183
	sw	$7,%lo(attack_daddr+52)($3)

$L254:
	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x800
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+32)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+32)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+32)
	j	$L186
	sw	$7,%lo(attack_daddr+32)($3)

$L259:
	lw	$9,%lo(dos_lan_addr)($3)
	lui	$8,%hi(dos_lan_mask)
	lw	$3,12($4)
	lw	$8,%lo(dos_lan_mask)($8)
	xor	$3,$9,$3
	and	$3,$3,$8
	bne	$3,$0,$L196
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+64)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+64)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+64)
	j	$L197
	sw	$7,%lo(attack_daddr+64)($3)

$L260:
	sll	$8,$8,2
	addu	$8,$4,$8
	lbu	$3,0($8)
	beq	$3,$7,$L200
	nop

	bne	$3,$0,$L303
	lw	$3,%lo(dos_item_global)($5)

$L200:
	lw	$3,12($4)
	li	$8,-268435456			# 0xfffffffff0000000
	li	$7,-536870912			# 0xffffffffe0000000
	and	$8,$3,$8
	beq	$8,$7,$L201
	li	$7,-1			# 0xffffffffffffffff

	beq	$3,$7,$L201
	lui	$7,%hi(dos_lan_mask)

	lw	$7,%lo(dos_lan_mask)($7)
	nor	$7,$0,$7
	and	$3,$7,$3
	bne	$7,$3,$L303
	lw	$3,%lo(dos_item_global)($5)

$L201:
	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	li	$3,16777216			# 0x1000000
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+68)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+68)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+68)
	j	$L199
	sw	$7,%lo(attack_daddr+68)($3)

$L261:
	lhu	$3,0($6)
	beq	$3,$7,$L204
	li	$7,19			# 0x13

	beq	$3,$7,$L204
	li	$7,135			# 0x87

	bne	$3,$7,$L304
	lw	$3,%lo(dos_item_global)($5)

$L204:
	lhu	$7,2($6)
	li	$3,135			# 0x87
	bne	$7,$3,$L304
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	li	$3,33554432			# 0x2000000
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+72)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+72)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+72)
	j	$L203
	sw	$7,%lo(attack_daddr+72)($3)

$L262:
	lbu	$3,13($6)
	andi	$7,$3,0x17
	beq	$7,$0,$L207
	andi	$8,$3,0x3

	li	$7,3			# 0x3
	beq	$8,$7,$L207
	andi	$3,$3,0x11

	li	$7,1			# 0x1
	bne	$3,$7,$L305
	lw	$3,%lo(dos_item_global)($5)

$L207:
	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	li	$3,67108864			# 0x4000000
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+76)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+76)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+76)
	j	$L206
	sw	$7,%lo(attack_daddr+76)($3)

$L250:
	sll	$9,$9,2
	addu	$9,$4,$9
	lbu	$8,0($9)
	bne	$8,$3,$L177
	nop

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+92)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+92)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+92)
	sw	$7,%lo(attack_daddr+92)($3)
	j	$L176
	lw	$3,%lo(dos_item_global)($5)

$L248:
	lhu	$3,6($4)
	andi	$3,$3,0x3fff
	bne	$3,$0,$L171
	lw	$3,%lo(dos_item_global)($5)

	lw	$3,0($4)
	lhu	$8,2($4)
	ext	$3,$3,24,4
	sll	$3,$3,2
	subu	$3,$8,$3
	slt	$3,$3,4
	beq	$3,$0,$L173
	nop

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	j	$L238
	sw	$7,%lo(LogFlag)($2)

$L263:
	lbu	$3,13($6)
	andi	$3,$3,0x4
	beq	$3,$0,$L208
	lw	$3,%lo(dos_item_global)($5)

	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$6,12($4)
	lui	$3,%hi(attack_saddr+80)
	li	$2,-1			# 0xffffffffffffffff
	sw	$6,%lo(attack_saddr+80)($3)
	lw	$6,16($4)
	lui	$3,%hi(attack_daddr+80)
	j	$L209
	sw	$6,%lo(attack_daddr+80)($3)

$L249:
	lw	$3,0($4)
	lhu	$8,2($4)
	ext	$3,$3,24,4
	sll	$3,$3,2
	subu	$3,$8,$3
$L223:
	slt	$3,$3,$7
	beq	$3,$0,$L173
	li	$3,536870912			# 0x20000000

	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
$L238:
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+88)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+88)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+88)
	sw	$7,%lo(attack_daddr+88)($3)
	j	$L171
	lw	$3,%lo(dos_item_global)($5)

$L266:
	sll	$8,$8,2
	addu	$8,$4,$8
	lbu	$3,0($8)
	bne	$3,$0,$L178
	lw	$3,%lo(dos_item_global)($5)

	lui	$3,%hi(icmp_echo_request)
	lw	$3,%lo(icmp_echo_request)($3)
	bne	$3,$0,$L179
	nop

	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	li	$3,-2147483648			# 0xffffffff80000000
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+96)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+96)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+96)
	sw	$7,%lo(attack_daddr+96)($3)
	j	$L178
	lw	$3,%lo(dos_item_global)($5)

$L267:
	sll	$9,$9,2
	addu	$9,$12,$9
	lw	$9,0($9)
	j	$9
	nop

	.section	.rodata.dos_filter_ipv4__action,"a",@progbits
	.align	2
	.align	2
$L163:
	.word	$L159
	.word	$L159
	.word	$L160
	.word	$L161
	.word	$L157
	.word	$L157
	.word	$L157
	.word	$L157
	.word	$L162
	.section	.text.dos_filter_ipv4__action
$L159:
	addiu	$3,$3,1
	j	$L233
	addiu	$8,$8,1

$L162:
	lbu	$9,1($3)
	bne	$9,$15,$L157
	addiu	$3,$3,10

	j	$L233
	addiu	$8,$8,10

$L161:
	lbu	$9,1($3)
	bne	$9,$14,$L157
	addiu	$3,$3,3

	j	$L233
	addiu	$8,$8,3

$L160:
	lbu	$9,1($3)
	bne	$9,$13,$L157
	addiu	$3,$3,4

	j	$L233
	addiu	$8,$8,4

$L174:
	lw	$3,0($4)
	lhu	$8,2($4)
	ext	$3,$3,24,4
	sll	$3,$3,2
	j	$L223
	subu	$3,$8,$3

$L218:
	lb	$6,0($5)
	li	$3,1			# 0x1
	bne	$6,$3,$L215
	nop

	lui	$3,%hi(cur_p_pkt)
	j	$L219
	sw	$5,%lo(cur_p_pkt)($3)

$L123:
	lb	$7,0($3)
	li	$2,1			# 0x1
	bne	$7,$2,$L306
	move	$2,$0

	lui	$7,%hi(cur_p_pkt)
	j	$L124
	sw	$3,%lo(cur_p_pkt)($7)

$L151:
	slt	$8,$8,201
	bne	$8,$0,$L268
	lui	$7,%hi(LogFlag)

	lw	$8,%lo(LogFlag)($7)
	j	$L236
	lw	$3,%lo(dos_item_global)($5)

$L220:
	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	ori	$3,$3,0x2000
	sw	$3,%lo(LogFlag)($2)
	lw	$5,12($4)
	lui	$3,%hi(attack_saddr+40)
	li	$2,-1			# 0xffffffffffffffff
	sw	$5,%lo(attack_saddr+40)($3)
	lw	$4,16($4)
	lui	$3,%hi(attack_daddr+40)
	j	$L215
	sw	$4,%lo(attack_daddr+40)($3)

$L245:
	sll	$11,$11,2
	addu	$11,$4,$11
	lbu	$8,0($11)
	li	$9,8			# 0x8
	beq	$8,$9,$L307
	lui	$9,%hi(dosFilter_block)

	bne	$8,$0,$L141
	nop

$L307:
	lw	$11,%lo(dosFilter_block)($9)
	li	$8,1			# 0x1
	beq	$11,$8,$L269
	lui	$8,%hi(attack_saddr+20)

$L143:
	lw	$8,20($10)
	addiu	$8,$8,1
	sw	$8,20($10)
	lui	$10,%hi(per_ping_threshold)
	lw	$10,%lo(per_ping_threshold)($10)
	slt	$8,$10,$8
	beq	$8,$0,$L141
	li	$8,2097152			# 0x200000

	and	$3,$3,$8
	beq	$3,$0,$L308
	lw	$3,%lo(dos_item_global)($5)

	blez	$10,$L141
	lui	$3,%hi(LogFlag)

	lw	$11,12($4)
	lui	$10,%hi(attack_saddr+20)
	sw	$11,%lo(attack_saddr+20)($10)
	lw	$10,%lo(LogFlag)($3)
	lw	$11,16($4)
	or	$8,$10,$8
	lui	$10,%hi(attack_daddr+20)
	sw	$11,%lo(attack_daddr+20)($10)
	li	$10,1			# 0x1
	sw	$10,%lo(dosFilter_block)($9)
	j	$L141
	sw	$8,%lo(LogFlag)($3)

$L244:
	lui	$9,%hi(dosFilter_block)
	lw	$11,%lo(dosFilter_block)($9)
	beq	$11,$8,$L270
	lui	$8,%hi(attack_saddr+16)

$L139:
	lw	$8,16($10)
	addiu	$8,$8,1
	sw	$8,16($10)
	lui	$10,%hi(per_icmp_threshold)
	lw	$10,%lo(per_icmp_threshold)($10)
	slt	$8,$10,$8
	beq	$8,$0,$L138
	andi	$3,$3,0x100

	beq	$3,$0,$L309
	lw	$3,%lo(dos_item_global)($5)

	blez	$10,$L309
	lui	$8,%hi(attack_saddr+16)

	lw	$10,12($4)
	lui	$3,%hi(LogFlag)
	sw	$10,%lo(attack_saddr+16)($8)
	lw	$11,16($4)
	lw	$8,%lo(LogFlag)($3)
	lui	$10,%hi(attack_daddr+16)
	sw	$11,%lo(attack_daddr+16)($10)
	ori	$8,$8,0x100
	li	$10,1			# 0x1
	sw	$10,%lo(dosFilter_block)($9)
	j	$L138
	sw	$8,%lo(LogFlag)($3)

$L243:
	lui	$9,%hi(dosFilter_block)
	lw	$11,%lo(dosFilter_block)($9)
	li	$8,1			# 0x1
	beq	$11,$8,$L271
	lui	$8,%hi(attack_saddr+12)

$L136:
	lw	$8,12($10)
	addiu	$8,$8,1
	sw	$8,12($10)
	lui	$10,%hi(per_udp_threshold)
	lw	$10,%lo(per_udp_threshold)($10)
	slt	$8,$10,$8
	beq	$8,$0,$L135
	andi	$3,$3,0x80

	beq	$3,$0,$L310
	lw	$3,%lo(dos_item_global)($5)

	blez	$10,$L310
	lui	$8,%hi(attack_saddr+12)

	lw	$10,12($4)
	lui	$3,%hi(LogFlag)
	sw	$10,%lo(attack_saddr+12)($8)
	lw	$11,16($4)
	lw	$8,%lo(LogFlag)($3)
	lui	$10,%hi(attack_daddr+12)
	sw	$11,%lo(attack_daddr+12)($10)
	ori	$8,$8,0x80
	li	$10,1			# 0x1
	sw	$10,%lo(dosFilter_block)($9)
	j	$L135
	sw	$8,%lo(LogFlag)($3)

$L242:
	lbu	$8,13($6)
	li	$9,1			# 0x1
	andi	$8,$8,0x3f
	bne	$8,$9,$L132
	lui	$9,%hi(dosFilter_block)

	lw	$11,%lo(dosFilter_block)($9)
	beq	$11,$8,$L272
	lui	$8,%hi(attack_saddr+8)

$L133:
	lw	$8,8($10)
	addiu	$8,$8,1
	sw	$8,8($10)
	lui	$10,%hi(per_fin_threshold)
	lw	$10,%lo(per_fin_threshold)($10)
	slt	$8,$10,$8
	beq	$8,$0,$L132
	andi	$3,$3,0x40

	beq	$3,$0,$L311
	lw	$3,%lo(dos_item_global)($5)

	blez	$10,$L311
	lui	$8,%hi(attack_saddr+8)

	lw	$10,12($4)
	lui	$3,%hi(LogFlag)
	sw	$10,%lo(attack_saddr+8)($8)
	lw	$11,16($4)
	lw	$8,%lo(LogFlag)($3)
	lui	$10,%hi(attack_daddr+8)
	sw	$11,%lo(attack_daddr+8)($10)
	ori	$8,$8,0x40
	li	$10,1			# 0x1
	sw	$10,%lo(dosFilter_block)($9)
	j	$L132
	sw	$8,%lo(LogFlag)($3)

$L241:
	lbu	$8,13($6)
	li	$2,2			# 0x2
	andi	$8,$8,0x3f
	bne	$8,$2,$L127
	move	$2,$0

	lui	$9,%hi(dosFilter_block)
	lw	$8,%lo(dosFilter_block)($9)
	li	$2,1			# 0x1
	beq	$8,$2,$L273
	lui	$2,%hi(attack_saddr+4)

$L126:
	lw	$2,4($10)
	lui	$8,%hi(per_syn_threshold)
	addiu	$2,$2,1
	sw	$2,4($10)
	lw	$8,%lo(per_syn_threshold)($8)
	slt	$2,$8,$2
	bne	$2,$0,$L128
	nop

	j	$L127
	lw	$3,%lo(dos_item_global)($5)

$L268:
	j	$L144
	lw	$3,%lo(dos_item_global)($5)

$L246:
	lhu	$3,36($7)
	beq	$3,$0,$L149
	nop

	lhu	$9,2($6)
	beq	$3,$9,$L150
	nop

	addiu	$8,$8,1
	sw	$8,24($7)
	move	$9,$3
$L150:
	bne	$9,$0,$L312
	lw	$3,%lo(dos_item_global)($5)

$L149:
	lhu	$3,2($6)
	j	$L148
	sh	$3,36($7)

$L230:
	lhu	$3,36($7)
	bne	$3,$0,$L231
	nop

	lw	$8,24($7)
$L146:
	lhu	$3,2($6)
$L277:
	sh	$3,36($7)
	j	$L145
	lbu	$3,9($4)

$L221:
	sh	$6,32($5)
	lw	$3,0($4)
	lhu	$4,2($4)
	ext	$3,$3,24,4
	andi	$3,$3,0x00ff
	addu	$7,$4,$7
	sll	$3,$3,2
	subu	$7,$7,$3
	j	$L215
	sh	$7,34($5)

$L231:
	lhu	$9,2($6)
	beq	$3,$9,$L274
	nop

	lw	$8,24($7)
	move	$9,$3
	addiu	$8,$8,1
	beq	$9,$0,$L146
	sw	$8,24($7)

$L275:
	j	$L145
	lbu	$3,9($4)

$L184:
	lhu	$3,6($4)
	andi	$3,$3,0x2000
	beq	$3,$0,$L182
	lw	$3,%lo(dos_item_global)($5)

	j	$L276
	lui	$2,%hi(LogFlag)

$L270:
	lw	$11,%lo(attack_saddr+16)($8)
	lw	$8,12($4)
	bne	$11,$8,$L139
	nop

	j	$L138
	li	$2,-1			# 0xffffffffffffffff

$L271:
	lw	$11,%lo(attack_saddr+12)($8)
	lw	$8,12($4)
	bne	$11,$8,$L136
	nop

	j	$L135
	li	$2,-1			# 0xffffffffffffffff

$L269:
	lw	$11,%lo(attack_saddr+20)($8)
	lw	$8,12($4)
	bne	$11,$8,$L143
	nop

	j	$L141
	li	$2,-1			# 0xffffffffffffffff

$L274:
	bne	$9,$0,$L275
	lw	$8,24($7)

	j	$L277
	lhu	$3,2($6)

$L128:
	andi	$3,$3,0x20
	beq	$3,$0,$L313
	lw	$3,%lo(dos_item_global)($5)

	blez	$8,$L313
	nop

	lw	$8,12($4)
	lui	$3,%hi(attack_saddr+4)
	lui	$2,%hi(LogFlag)
	sw	$8,%lo(attack_saddr+4)($3)
	lw	$10,16($4)
	lw	$3,%lo(LogFlag)($2)
	lui	$8,%hi(attack_daddr+4)
	sw	$10,%lo(attack_daddr+4)($8)
	ori	$3,$3,0x20
	li	$8,1			# 0x1
	sw	$8,%lo(dosFilter_block)($9)
	sw	$3,%lo(LogFlag)($2)
	lw	$3,%lo(dos_item_global)($5)
$L313:
	j	$L127
	move	$2,$0

$L272:
	lw	$11,%lo(attack_saddr+8)($8)
	lw	$8,12($4)
	bne	$11,$8,$L133
	nop

	j	$L132
	li	$2,-1			# 0xffffffffffffffff

$L273:
	lw	$11,%lo(attack_saddr+4)($2)
	lw	$8,12($4)
	bne	$11,$8,$L126
	li	$2,-1			# 0xffffffffffffffff

	j	$L278
	andi	$8,$3,0x44

	.set	macro
	.set	reorder
	.end	dos_filter_ipv4__action
	.size	dos_filter_ipv4__action, .-dos_filter_ipv4__action
	.section	.text.filter_out,"ax",@progbits
	.align	2
	.globl	filter_out
	.set	nomips16
	.ent	filter_out
	.type	filter_out, @function
filter_out:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	nop

	lw	$4,0($2)
	lbu	$3,9($2)
	ext	$5,$4,24,4
	li	$4,1			# 0x1
	beq	$3,$4,$L317
	lw	$31,20($sp)

$L318:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L317:
	sll	$5,$5,2
	addu	$2,$2,$5
	lbu	$4,0($2)
	li	$2,8			# 0x8
	bne	$4,$2,$L318
	nop

	lui	$2,%hi(jiffies)
	lw	$4,%lo(jiffies)($2)
	lw	$31,20($sp)
	lui	$2,%hi(icmp_echo_request_time)
	sw	$4,%lo(icmp_echo_request_time)($2)
	lui	$2,%hi(icmp_echo_request)
	sw	$3,%lo(icmp_echo_request)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_out
	.size	filter_out, .-filter_out
	.section	.rodata.str1.4
	.align	2
$LC18:
	.ascii	"%u.%u.%u.%u\000"
	.section	.text.IP2NAME,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	IP2NAME
	.type	IP2NAME, @function
IP2NAME:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	move	$16,$5
	sw	$4,32($sp)
	move	$5,$0
	move	$4,$16
	jal	memset
	li	$6,16			# 0x10

	lbu	$3,34($sp)
	lbu	$2,35($sp)
	lbu	$6,32($sp)
	lbu	$7,33($sp)
	lui	$5,%hi($LC18)
	move	$4,$16
	addiu	$5,$5,%lo($LC18)
	sw	$3,16($sp)
	jal	sprintf
	sw	$2,20($sp)

	move	$2,$16
	lw	$31,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	IP2NAME
	.size	IP2NAME, .-IP2NAME
	.section	.rodata.str1.4
	.align	2
$LC19:
	.ascii	"%s\012\000"
	.align	2
$LC20:
	.ascii	"connectip = %s\012\000"
	.align	2
$LC21:
	.ascii	"pkt---IP:%s, syn_cnt:%d, fin_cnt:%d, udp_nct:%d, icmp_cn"
	.ascii	"t:%d, scan_cnt:%d\012\000"
	.align	2
$LC22:
	.ascii	"attack_saddr[%s]=%s,attack_daddr[%s]=%s\012\000"
	.align	2
$LC23:
	.ascii	"op_mode = %d, lan_addr = %s, lan_mask = %s, dos_item_glo"
	.ascii	"bal = 0x%x,dos_item_global_0 = 0x%x\012\000"
	.section	.text.dos_read_proc_handler,"ax",@progbits
	.align	2
	.globl	dos_read_proc_handler
	.set	nomips16
	.ent	dos_read_proc_handler
	.type	dos_read_proc_handler, @function
dos_read_proc_handler:
	.frame	$sp,80,$31		# vars= 8, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	sw	$23,68($sp)
	sw	$22,64($sp)
	sw	$21,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$5,%hi($LC19)
	lui	$6,%hi(dos_flag)
	addiu	$5,$5,%lo($LC19)
	addiu	$6,$6,%lo(dos_flag)
	lui	$fp,%hi(ip_name0)
	lui	$16,%hi(ConnectedIp)
	lui	$17,%hi(ConnectedIp+1024)
	lui	$20,%hi($LC20)
	move	$19,$4
	jal	seq_printf
	addiu	$16,$16,%lo(ConnectedIp)

	addiu	$17,$17,%lo(ConnectedIp+1024)
	addiu	$20,$20,%lo($LC20)
	addiu	$18,$fp,%lo(ip_name0)
$L323:
	lw	$4,0($16)
	bne	$4,$0,$L334
	addiu	$16,$16,4

	bne	$16,$17,$L323
	nop

	lui	$16,%hi(dos_pkt)
$L339:
	jal	local_bh_disable
	addiu	$16,$16,%lo(dos_pkt)

	lb	$2,0($16)
	lui	$18,%hi(dos_pkt+40920)
	lui	$21,%hi($LC21)
	li	$17,1			# 0x1
	addiu	$18,$18,%lo(dos_pkt+40920)
	addiu	$21,$21,%lo($LC21)
	beq	$2,$17,$L335
	addiu	$20,$fp,%lo(ip_name0)

$L324:
	beq	$16,$18,$L325
	nop

	addiu	$16,$16,40
$L340:
	lb	$2,0($16)
	bne	$2,$17,$L324
	nop

$L335:
	lw	$4,28($16)
	jal	IP2NAME
	move	$5,$20

	lw	$7,4($16)
	lw	$10,8($16)
	lw	$9,12($16)
	lw	$8,16($16)
	lw	$3,24($16)
	move	$6,$2
	move	$4,$19
	move	$5,$21
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	jal	seq_printf
	sw	$3,28($sp)

	bne	$16,$18,$L340
	addiu	$16,$16,40

	addiu	$16,$16,-40
$L325:
	lui	$16,%hi(attack_daddr)
	lui	$17,%hi(attack_saddr)
	lui	$18,%hi(rtl_dos_name)
	lui	$21,%hi(attack_daddr+108)
	lui	$2,%hi(ip_name1)
	addiu	$16,$16,%lo(attack_daddr)
	addiu	$17,$17,%lo(attack_saddr)
	addiu	$18,$18,%lo(rtl_dos_name)
	addiu	$21,$21,%lo(attack_daddr+108)
	addiu	$23,$fp,%lo(ip_name0)
	j	$L330
	addiu	$22,$2,%lo(ip_name1)

$L327:
	lw	$4,0($17)
	beq	$4,$0,$L329
	nop

$L328:
	jal	IP2NAME
	move	$5,$23

	lw	$4,0($16)
	lw	$20,0($18)
	move	$5,$22
	jal	IP2NAME
	sw	$2,36($sp)

	lw	$7,36($sp)
	lui	$5,%hi($LC22)
	move	$6,$20
	move	$4,$19
	addiu	$5,$5,%lo($LC22)
	sw	$2,20($sp)
	jal	seq_printf
	sw	$20,16($sp)

$L329:
	addiu	$16,$16,4
	addiu	$17,$17,4
	beq	$16,$21,$L337
	addiu	$18,$18,4

$L330:
	lw	$2,0($16)
	beq	$2,$0,$L327
	nop

	j	$L328
	lw	$4,0($17)

$L334:
	jal	IP2NAME
	move	$5,$18

	move	$6,$2
	move	$4,$19
	jal	seq_printf
	move	$5,$20

	bne	$16,$17,$L323
	nop

	j	$L339
	lui	$16,%hi(dos_pkt)

$L337:
	jal	local_bh_enable
	nop

	lui	$2,%hi(dos_lan_addr)
	lw	$4,%lo(dos_lan_addr)($2)
	lui	$2,%hi(dos_op_mode)
	lb	$6,%lo(dos_op_mode)($2)
	addiu	$5,$fp,%lo(ip_name0)
	jal	IP2NAME
	sw	$6,32($sp)

	move	$7,$2
	lui	$2,%hi(dos_lan_mask)
	lw	$4,%lo(dos_lan_mask)($2)
	lui	$2,%hi(ip_name1)
	addiu	$5,$2,%lo(ip_name1)
	jal	IP2NAME
	sw	$7,36($sp)

	lui	$3,%hi(dos_item_global)
	lw	$8,%lo(dos_item_global)($3)
	lui	$3,%hi(dos_item_global_0)
	lw	$3,%lo(dos_item_global_0)($3)
	lw	$6,32($sp)
	lw	$7,36($sp)
	lui	$5,%hi($LC23)
	move	$4,$19
	addiu	$5,$5,%lo($LC23)
	sw	$2,16($sp)
	sw	$8,20($sp)
	jal	seq_printf
	sw	$3,24($sp)

	lw	$31,76($sp)
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,80

	.set	macro
	.set	reorder
	.end	dos_read_proc_handler
	.size	dos_read_proc_handler, .-dos_read_proc_handler
	.section	.rodata.str1.4
	.align	2
$LC24:
	.ascii	"NULL\000"
	.align	2
$LC25:
	.ascii	"PerSynFlood\000"
	.align	2
$LC26:
	.ascii	"PerFinFlood\000"
	.align	2
$LC27:
	.ascii	"PerUdpFlood\000"
	.align	2
$LC28:
	.ascii	"PerIcmpFlood\000"
	.align	2
$LC29:
	.ascii	"PerPingFlood\000"
	.align	2
$LC30:
	.ascii	"TcpUdpPortScan\000"
	.align	2
$LC31:
	.ascii	"IcmpSmurf\000"
	.align	2
$LC32:
	.ascii	"IpLand\000"
	.align	2
$LC33:
	.ascii	"IpSpoof\000"
	.align	2
$LC34:
	.ascii	"TearDrop\000"
	.align	2
$LC35:
	.ascii	"PingOfDeath\000"
	.align	2
$LC36:
	.ascii	"TcpScan\000"
	.align	2
$LC37:
	.ascii	"TcpSynWithData\000"
	.align	2
$LC38:
	.ascii	"UdpBomb\000"
	.align	2
$LC39:
	.ascii	"UdpEchoChargen\000"
	.align	2
$LC40:
	.ascii	"UdpFraggle\000"
	.align	2
$LC41:
	.ascii	"IcmpEchoStorm\000"
	.align	2
$LC42:
	.ascii	"UdpSnorkAttack\000"
	.align	2
$LC43:
	.ascii	"TcpInvalidFlags\000"
	.align	2
$LC44:
	.ascii	"TcpRstAttack\000"
	.align	2
$LC45:
	.ascii	"BcMcSrcIpAttack\000"
	.align	2
$LC46:
	.ascii	"ShortIpPkt\000"
	.align	2
$LC47:
	.ascii	"IcmpPingSweep\000"
	.align	2
$LC48:
	.ascii	"IcmpEchoWithoutReply\000"
	.align	2
$LC49:
	.ascii	"TcpInvalidOptions\000"
	.align	2
$LC50:
	.ascii	"LoopBackEchoChargen\000"
	.align	2
$LC51:
	.ascii	"MaxDosType\000"
	.section	.rodata.rtl_dos_name,"a",@progbits
	.align	2
	.type	rtl_dos_name, @object
	.size	rtl_dos_name, 112
rtl_dos_name:
	.word	$LC24
	.word	$LC25
	.word	$LC26
	.word	$LC27
	.word	$LC28
	.word	$LC29
	.word	$LC30
	.word	$LC31
	.word	$LC32
	.word	$LC33
	.word	$LC34
	.word	$LC35
	.word	$LC36
	.word	$LC37
	.word	$LC38
	.word	$LC39
	.word	$LC40
	.word	$LC41
	.word	$LC42
	.word	$LC43
	.word	$LC44
	.word	$LC45
	.word	$LC46
	.word	$LC47
	.word	$LC48
	.word	$LC49
	.word	$LC50
	.word	$LC51
	.section	.bss._tcpDosScanBitmap,"aw",@nobits
	.align	2
	.type	_tcpDosScanBitmap, @object
	.size	_tcpDosScanBitmap, 64
_tcpDosScanBitmap:
	.space	64
	.section	.bss.LogFlag,"aw",@nobits
	.align	2
	.type	LogFlag, @object
	.size	LogFlag, 4
LogFlag:
	.space	4
	.section	.bss.dos_pkt,"aw",@nobits
	.align	2
	.type	dos_pkt, @object
	.size	dos_pkt, 40960
dos_pkt:
	.space	40960
	.section	.bss.cur_p_pkt,"aw",@nobits
	.align	2
	.type	cur_p_pkt, @object
	.size	cur_p_pkt, 4
cur_p_pkt:
	.space	4
	.section	.bss.attack_saddr,"aw",@nobits
	.align	2
	.type	attack_saddr, @object
	.size	attack_saddr, 108
attack_saddr:
	.space	108
	.section	.bss.attack_daddr,"aw",@nobits
	.align	2
	.type	attack_daddr, @object
	.size	attack_daddr, 108
attack_daddr:
	.space	108
	.section	.bss.ip_name0,"aw",@nobits
	.align	2
	.type	ip_name0, @object
	.size	ip_name0, 16
ip_name0:
	.space	16
	.section	.bss.ip_name1,"aw",@nobits
	.align	2
	.type	ip_name1, @object
	.size	ip_name1, 16
ip_name1:
	.space	16
	.section	.bss.icmp_echo_request,"aw",@nobits
	.align	2
	.type	icmp_echo_request, @object
	.size	icmp_echo_request, 4
icmp_echo_request:
	.space	4
	.section	.bss.icmp_echo_request_time,"aw",@nobits
	.align	2
	.type	icmp_echo_request_time, @object
	.size	icmp_echo_request_time, 4
icmp_echo_request_time:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 1459) 4.4.7"
