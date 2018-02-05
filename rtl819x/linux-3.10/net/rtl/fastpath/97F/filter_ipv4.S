	.file	1 "filter_ipv4.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
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
	.text
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
	andi	$5,$4,0xf
	li	$4,1			# 0x1
	beq	$3,$4,$L122
	lw	$31,20($sp)

$L123:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L122:
	sll	$5,$5,2
	addu	$2,$2,$5
	lbu	$4,0($2)
	li	$2,8			# 0x8
	bne	$4,$2,$L123
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
	.align	2
	.globl	dos_filter_ipv4__action
	.set	nomips16
	.ent	dos_filter_ipv4__action
	.type	dos_filter_ipv4__action, @function
dos_filter_ipv4__action:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	lw	$6,0($2)
	lui	$5,%hi(dos_item_global)
	move	$4,$2
	lw	$2,%lo(dos_item_global)($5)
	andi	$6,$6,0xf
	sll	$6,$6,2
	andi	$2,$2,0x3fe
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L125
	addu	$6,$4,$6
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	beq	$2,$8,$L245
	addu	$3,$3,$7
	.set	macro
	.set	reorder

	lw	$7,28($3)
	beq	$2,$7,$L128
$L127:
	lb	$7,0($3)
	.set	noreorder
	.set	nomacro
	bne	$7,$0,$L125
	lui	$7,%hi(cur_p_pkt)
	.set	macro
	.set	reorder

	sw	$2,28($3)
	li	$2,1			# 0x1
	sb	$2,0($3)
	sw	$3,%lo(cur_p_pkt)($7)
$L129:
	lw	$3,%lo(dos_item_global)($5)
	andi	$2,$3,0x22
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L132
	move	$2,$0
	.set	macro
	.set	reorder

	lbu	$8,9($4)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$8,$2,$L246
	lw	$10,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

	move	$2,$0
$L132:
	andi	$8,$3,0x44
$L285:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L286
	andi	$8,$3,0x88
	.set	macro
	.set	reorder

	lbu	$9,9($4)
	li	$8,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$9,$8,$L247
	lw	$10,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

$L137:
	lw	$3,%lo(dos_item_global)($5)
$L318:
	andi	$8,$3,0x88
$L286:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L287
	andi	$8,$3,0x110
	.set	macro
	.set	reorder

	lbu	$9,9($4)
	li	$8,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$9,$8,$L248
	lw	$10,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

$L140:
	lw	$3,%lo(dos_item_global)($5)
$L315:
	andi	$8,$3,0x110
$L287:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L288
	li	$8,3145728			# 0x300000
	.set	macro
	.set	reorder

	lbu	$8,9($4)
	li	$9,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$8,$9,$L249
	lw	$10,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

$L143:
	lw	$3,%lo(dos_item_global)($5)
$L314:
	li	$8,3145728			# 0x300000
$L288:
	and	$8,$3,$8
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L289
	andi	$8,$3,0x200
	.set	macro
	.set	reorder

	lw	$8,0($4)
	lbu	$9,9($4)
	andi	$11,$8,0xf
	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$9,$8,$L250
	lw	$10,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

$L146:
	lw	$3,%lo(dos_item_global)($5)
$L317:
	andi	$8,$3,0x200
$L289:
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L290
	ext	$3,$3,22,1
	.set	macro
	.set	reorder

	lbu	$3,9($4)
	li	$8,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$8,$L235
	lw	$7,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

	lw	$8,24($7)
$L150:
	li	$9,17			# 0x11
	beq	$3,$9,$L251
$L153:
	lw	$3,%lo(dos_item_global)($5)
$L319:
	ext	$7,$3,23,1
	beq	$7,$0,$L156
	slt	$8,$8,11
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L149
	lui	$7,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$8,%lo(LogFlag)($7)
$L241:
	ori	$8,$8,0x200
	sw	$8,%lo(LogFlag)($7)
	lw	$8,12($4)
	lui	$7,%hi(attack_saddr+24)
	sw	$8,%lo(attack_saddr+24)($7)
	lw	$8,16($4)
	lui	$7,%hi(attack_daddr+24)
	sw	$8,%lo(attack_daddr+24)($7)
$L149:
	ext	$3,$3,22,1
$L290:
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L291
	lui	$7,%hi(dos_item_global_0)
	.set	macro
	.set	reorder

$L125:
	move	$2,$0
$L313:
	lui	$7,%hi(dos_item_global_0)
$L291:
	lw	$3,%lo(dos_item_global_0)($7)
	andi	$8,$3,0x1
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L292
	andi	$3,$3,0x2
	.set	macro
	.set	reorder

	lbu	$8,9($4)
	li	$3,6			# 0x6
	beq	$8,$3,$L252
$L160:
	lw	$3,%lo(dos_item_global_0)($7)
	andi	$3,$3,0x2
$L292:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L293
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lbu	$3,9($4)
	li	$7,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L174
	li	$7,6			# 0x6
	.set	macro
	.set	reorder

	beq	$3,$7,$L174
$L173:
	lw	$3,%lo(dos_item_global)($5)
$L293:
	li	$7,536870912			# 0x20000000
	and	$8,$3,$7
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L176
	li	$8,1			# 0x1
	.set	macro
	.set	reorder

	lbu	$3,9($4)
	beq	$3,$8,$L253
	li	$7,17			# 0x11
	beq	$3,$7,$L179
	li	$7,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L254
	li	$3,20			# 0x14
	.set	macro
	.set	reorder

$L178:
	lw	$3,%lo(dos_item_global)($5)
$L176:
	li	$7,1073741824			# 0x40000000
	and	$8,$3,$7
	beq	$8,$0,$L181
	lw	$3,0($4)
	lbu	$8,9($4)
	andi	$9,$3,0xf
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$8,$3,$L255
	li	$3,8			# 0x8
	.set	macro
	.set	reorder

$L182:
	lw	$3,%lo(dos_item_global)($5)
$L181:
	bltz	$3,$L256
$L183:
	andi	$7,$3,0x8000
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L294
	li	$7,65536			# 0x10000
	.set	macro
	.set	reorder

	lbu	$7,9($4)
	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$7,$3,$L257
	lui	$3,%hi(_tcpDosScanBitmap)
	.set	macro
	.set	reorder

$L186:
	lw	$3,%lo(dos_item_global)($5)
$L310:
	li	$7,65536			# 0x10000
$L294:
	and	$8,$3,$7
	beq	$8,$0,$L187
	lbu	$8,9($4)
	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$8,$3,$L258
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

$L188:
	lw	$3,%lo(dos_item_global)($5)
$L187:
	andi	$7,$3,0x800
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L295
	ext	$7,$3,18,1
	.set	macro
	.set	reorder

	lw	$7,12($4)
	lw	$3,16($4)
	beq	$7,$3,$L259
$L191:
	lw	$3,%lo(dos_item_global)($5)
	ext	$7,$3,18,1
$L295:
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L296
	li	$7,131072			# 0x20000
	.set	macro
	.set	reorder

	lbu	$7,9($4)
	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$7,$3,$L260
	li	$7,1792			# 0x700
	.set	macro
	.set	reorder

$L193:
	lw	$3,%lo(dos_item_global)($5)
$L309:
	li	$7,131072			# 0x20000
$L296:
	and	$8,$3,$7
	beq	$8,$0,$L195
	lbu	$8,9($4)
	li	$3,17			# 0x11
	beq	$8,$3,$L261
$L196:
	lw	$3,%lo(dos_item_global)($5)
$L195:
	andi	$7,$3,0x4000
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L297
	andi	$7,$3,0x400
	.set	macro
	.set	reorder

	lhu	$3,6($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	andi	$7,$3,0x2000
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L262
	andi	$3,$3,0x1fff
	.set	macro
	.set	reorder

$L198:
	lw	$3,%lo(dos_item_global)($5)
$L305:
	andi	$7,$3,0x400
$L297:
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L298
	li	$7,524288			# 0x80000
	.set	macro
	.set	reorder

	lw	$3,0($4)
	lbu	$7,9($4)
	andi	$8,$3,0xf
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$7,$3,$L263
	sll	$8,$8,2
	.set	macro
	.set	reorder

$L200:
	lw	$3,%lo(dos_item_global)($5)
$L311:
	li	$7,524288			# 0x80000
$L298:
	and	$8,$3,$7
	beq	$8,$0,$L201
	lbu	$8,9($4)
	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$8,$3,$L264
	lui	$3,%hi(dos_lan_addr)
	.set	macro
	.set	reorder

$L202:
	lw	$3,%lo(dos_item_global)($5)
$L201:
	ext	$7,$3,24,1
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L299
	ext	$7,$3,25,1
	.set	macro
	.set	reorder

	lw	$3,0($4)
	lbu	$7,9($4)
	andi	$8,$3,0xf
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$7,$3,$L265
	li	$7,8			# 0x8
	.set	macro
	.set	reorder

$L204:
	lw	$3,%lo(dos_item_global)($5)
$L312:
	ext	$7,$3,25,1
$L299:
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L300
	ext	$7,$3,26,1
	.set	macro
	.set	reorder

	lbu	$7,9($4)
	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$7,$3,$L266
	li	$7,1792			# 0x700
	.set	macro
	.set	reorder

$L208:
	lw	$3,%lo(dos_item_global)($5)
$L307:
	ext	$7,$3,26,1
$L300:
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L301
	li	$7,134217728			# 0x8000000
	.set	macro
	.set	reorder

	lbu	$7,9($4)
	li	$3,6			# 0x6
	beq	$7,$3,$L267
$L211:
	lw	$3,%lo(dos_item_global)($5)
$L308:
	li	$7,134217728			# 0x8000000
$L301:
	and	$8,$3,$7
	beq	$8,$0,$L213
	lbu	$8,9($4)
	li	$3,6			# 0x6
	beq	$8,$3,$L268
$L214:
	lw	$3,%lo(dos_item_global)($5)
$L213:
	ext	$6,$3,28,1
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L302
	andi	$6,$3,0x1000
	.set	macro
	.set	reorder

	lw	$3,12($4)
	li	$6,224			# 0xe0
	andi	$7,$3,0xf0
	.set	noreorder
	.set	nomacro
	beq	$7,$6,$L216
	li	$6,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$6,$L216
	lui	$6,%hi(dos_lan_mask)
	.set	macro
	.set	reorder

	lw	$6,%lo(dos_lan_mask)($6)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	nor	$6,$0,$6
	and	$3,$3,$6
	beq	$6,$3,$L216
	lw	$3,%lo(dos_item_global)($5)
$L215:
	andi	$6,$3,0x1000
$L302:
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L303
	andi	$3,$3,0x2000
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_addr)
	lw	$7,%lo(dos_lan_addr)($3)
	lui	$3,%hi(dos_lan_mask)
	lw	$6,%lo(dos_lan_mask)($3)
	lw	$3,12($4)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	xor	$3,$3,$7
	and	$3,$3,$6
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L269
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

$L219:
	lw	$3,%lo(dos_item_global)($5)
$L304:
	andi	$3,$3,0x2000
$L303:
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L270
	lui	$5,%hi(ConnectedIp)
	.set	macro
	.set	reorder

$L220:
	lw	$31,20($sp)
$L306:
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

$L269:
	lbu	$6,9($4)
	.set	noreorder
	.set	nomacro
	beq	$6,$3,$L304
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L219
	sw	$6,%lo(attack_daddr+36)($3)
	.set	macro
	.set	reorder

$L262:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L198
	sll	$3,$3,3
	.set	macro
	.set	reorder

	lw	$8,0($4)
	lhu	$7,2($4)
	andi	$8,$8,0xf
	sll	$8,$8,2
	subu	$3,$3,$8
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 #NO_APP
	andi	$7,$7,0xffff
	addu	$3,$3,$7
	li	$7,65536			# 0x10000
	sltu	$3,$3,$7
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L305
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L198
	sw	$7,%lo(attack_daddr+44)($3)
	.set	macro
	.set	reorder

$L216:
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
	.set	noreorder
	.set	nomacro
	j	$L215
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L174:
	lhu	$3,2($6)
	li	$7,1792			# 0x700
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L175
	li	$8,4352			# 0x1100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$8,$L175
	li	$9,4864			# 0x1300
	.set	macro
	.set	reorder

	beq	$3,$9,$L175
	lhu	$3,0($6)
	beq	$3,$7,$L175
	beq	$3,$8,$L175
	.set	noreorder
	.set	nomacro
	bne	$3,$9,$L293
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L175:
	lw	$3,12($4)
	lw	$7,16($4)
	bne	$3,$7,$L173
	lui	$2,%hi(attack_saddr+104)
	sw	$3,%lo(attack_saddr+104)($2)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+104)
	li	$2,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$L173
	sw	$7,%lo(attack_daddr+104)($3)
	.set	macro
	.set	reorder

$L270:
	lw	$3,12($4)
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
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L271
	addu	$5,$5,$6
	.set	macro
	.set	reorder

	lw	$6,28($5)
	beq	$3,$6,$L223
$L222:
	lb	$6,0($5)
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L306
	lw	$31,20($sp)
	.set	macro
	.set	reorder

	sw	$3,28($5)
	li	$3,1			# 0x1
	sb	$3,0($5)
	lui	$3,%hi(cur_p_pkt)
	sw	$5,%lo(cur_p_pkt)($3)
$L224:
	lhu	$3,32($5)
	lhu	$7,4($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $7			

 # 0 "" 2
 #NO_APP
	andi	$6,$6,0xffff
	beq	$6,$3,$L229
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L306
	lw	$31,20($sp)
	.set	macro
	.set	reorder

$L229:
	lhu	$3,6($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xffff
	andi	$6,$3,0x3fff
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L220
	sll	$8,$3,3
	.set	macro
	.set	reorder

	lhu	$6,34($5)
	andi	$8,$8,0xffff
	slt	$8,$8,$6
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L225
	andi	$3,$3,0x2000
	.set	macro
	.set	reorder

	beq	$3,$0,$L272
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 #NO_APP
	sh	$7,32($5)
	lw	$7,0($4)
	lhu	$3,2($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	ext	$4,$7,0,4
	andi	$4,$4,0x00ff
	sll	$4,$4,2
	subu	$6,$6,$4
	addu	$6,$6,$3
	.set	noreorder
	.set	nomacro
	j	$L220
	sh	$6,34($5)
	.set	macro
	.set	reorder

$L256:
	lw	$3,0($4)
	lbu	$7,9($4)
	andi	$8,$3,0xf
	li	$3,1			# 0x1
	beq	$7,$3,$L273
$L184:
	.set	noreorder
	.set	nomacro
	j	$L183
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L258:
	lbu	$8,13($6)
	andi	$8,$8,0x3f
	.set	noreorder
	.set	nomacro
	bne	$8,$3,$L187
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lw	$9,0($4)
	lw	$3,12($6)
	lhu	$8,2($4)
	andi	$9,$9,0xf
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	sll	$9,$9,2
	andi	$8,$8,0xffff
	ext	$3,$3,4,4
	subu	$8,$8,$9
	sll	$3,$3,2
	beq	$8,$3,$L189
	lui	$2,%hi(LogFlag)
$L283:
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	sw	$7,%lo(LogFlag)($2)
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+52)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+52)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+52)
	.set	noreorder
	.set	nomacro
	j	$L188
	sw	$7,%lo(attack_daddr+52)($3)
	.set	macro
	.set	reorder

$L266:
	lhu	$3,0($6)
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L209
	li	$7,4864			# 0x1300
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L209
	li	$7,34560			# 0x8700
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$7,$L307
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L209:
	lhu	$7,2($6)
	li	$3,34560			# 0x8700
	.set	noreorder
	.set	nomacro
	bne	$7,$3,$L307
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L208
	sw	$7,%lo(attack_daddr+72)($3)
	.set	macro
	.set	reorder

$L267:
	lbu	$3,13($6)
	andi	$7,$3,0x17
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L212
	andi	$8,$3,0x3
	.set	macro
	.set	reorder

	li	$7,3			# 0x3
	.set	noreorder
	.set	nomacro
	beq	$8,$7,$L212
	andi	$3,$3,0x11
	.set	macro
	.set	reorder

	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$7,$L308
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L212:
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
	.set	noreorder
	.set	nomacro
	j	$L211
	sw	$7,%lo(attack_daddr+76)($3)
	.set	macro
	.set	reorder

$L245:
	lw	$7,28($3)
	bne	$2,$7,$L127
	.set	noreorder
	.set	nomacro
	j	$L125
	sb	$0,0($3)
	.set	macro
	.set	reorder

$L252:
	lw	$3,0($4)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$4,$3
	lbu	$11,12($3)
	andi	$11,$11,0xf0
	sra	$11,$11,2
	sltu	$8,$11,21
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L160
	lui	$12,%hi($L168)
	.set	macro
	.set	reorder

	addiu	$3,$3,20
	addiu	$11,$11,-20
	addiu	$12,$12,%lo($L168)
	move	$8,$0
	li	$15,10			# 0xa
	li	$14,3			# 0x3
	li	$13,4			# 0x4
$L238:
	sltu	$9,$8,$11
	beq	$9,$0,$L160
	lbu	$9,0($3)
	sltu	$10,$9,9
	bne	$10,$0,$L274
$L162:
	lw	$3,12($4)
	lui	$2,%hi(attack_saddr+100)
	sw	$3,%lo(attack_saddr+100)($2)
	lw	$8,16($4)
	lui	$3,%hi(attack_daddr+100)
	li	$2,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$L160
	sw	$8,%lo(attack_daddr+100)($3)
	.set	macro
	.set	reorder

$L268:
	lbu	$3,13($6)
	andi	$3,$3,0x4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L213
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L214
	sw	$6,%lo(attack_daddr+80)($3)
	.set	macro
	.set	reorder

$L253:
	lhu	$3,6($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0x3fff
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L176
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lw	$3,0($4)
	lhu	$8,2($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xf
	andi	$8,$8,0xffff
	sll	$3,$3,2
	subu	$3,$8,$3
	slt	$3,$3,4
	beq	$3,$0,$L178
	lui	$2,%hi(LogFlag)
	lw	$3,%lo(LogFlag)($2)
	or	$7,$3,$7
	.set	noreorder
	.set	nomacro
	j	$L243
	sw	$7,%lo(LogFlag)($2)
	.set	macro
	.set	reorder

$L259:
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
	.set	noreorder
	.set	nomacro
	j	$L191
	sw	$7,%lo(attack_daddr+32)($3)
	.set	macro
	.set	reorder

$L260:
	lhu	$3,2($6)
	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L194
	li	$8,4352			# 0x1100
	.set	macro
	.set	reorder

	beq	$3,$8,$L194
	li	$9,4864			# 0x1300
	beq	$3,$9,$L194
	lhu	$3,0($6)
	beq	$3,$7,$L194
	beq	$3,$8,$L194
	.set	noreorder
	.set	nomacro
	bne	$3,$9,$L309
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L194:
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
	.set	noreorder
	.set	nomacro
	j	$L193
	sw	$7,%lo(attack_daddr+60)($3)
	.set	macro
	.set	reorder

$L261:
	lhu	$3,6($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0x3fff
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L195
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lw	$3,0($4)
	lhu	$8,2($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0xf
	andi	$8,$8,0xffff
	sll	$3,$3,2
	subu	$3,$8,$3
	lhu	$8,4($6)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$8,$8,0xffff
	slt	$3,$8,$3
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L195
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L196
	sw	$7,%lo(attack_daddr+56)($3)
	.set	macro
	.set	reorder

$L257:
	lbu	$7,13($6)
	addiu	$3,$3,%lo(_tcpDosScanBitmap)
	andi	$7,$7,0x3f
	addu	$3,$7,$3
	lb	$3,0($3)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L310
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L186
	sw	$7,%lo(attack_daddr+48)($3)
	.set	macro
	.set	reorder

$L255:
	sll	$9,$9,2
	addu	$9,$4,$9
	lbu	$8,0($9)
	bne	$8,$3,$L182
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
	.set	noreorder
	.set	nomacro
	j	$L181
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L263:
	addu	$8,$4,$8
	lbu	$7,0($8)
	li	$3,8			# 0x8
	.set	noreorder
	.set	nomacro
	bne	$7,$3,$L311
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_addr)
	lw	$8,%lo(dos_lan_addr)($3)
	lui	$3,%hi(dos_lan_mask)
	lw	$7,%lo(dos_lan_mask)($3)
	lw	$3,12($4)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	xor	$3,$3,$8
	and	$3,$3,$7
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L311
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L200
	sw	$7,%lo(attack_daddr+28)($3)
	.set	macro
	.set	reorder

$L264:
	lw	$9,%lo(dos_lan_addr)($3)
	lui	$3,%hi(dos_lan_mask)
	lw	$8,%lo(dos_lan_mask)($3)
	lw	$3,12($4)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	xor	$3,$3,$9
	and	$3,$3,$8
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L201
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L202
	sw	$7,%lo(attack_daddr+64)($3)
	.set	macro
	.set	reorder

$L265:
	sll	$8,$8,2
	addu	$8,$4,$8
	lbu	$3,0($8)
	beq	$3,$7,$L205
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L312
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L205:
	lw	$3,12($4)
	li	$7,224			# 0xe0
	andi	$8,$3,0xf0
	.set	noreorder
	.set	nomacro
	beq	$8,$7,$L206
	li	$7,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$7,$L206
	lui	$7,%hi(dos_lan_mask)
	.set	macro
	.set	reorder

	lw	$7,%lo(dos_lan_mask)($7)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			
	rotr	$3, $3, 16		

 # 0 "" 2
 #NO_APP
	nor	$7,$0,$7
	and	$3,$3,$7
	.set	noreorder
	.set	nomacro
	bne	$7,$3,$L312
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L206:
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
	.set	noreorder
	.set	nomacro
	j	$L204
	sw	$7,%lo(attack_daddr+68)($3)
	.set	macro
	.set	reorder

$L223:
	lb	$6,0($5)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$6,$3,$L306
	lw	$31,20($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(cur_p_pkt)
	.set	noreorder
	.set	nomacro
	j	$L224
	sw	$5,%lo(cur_p_pkt)($3)
	.set	macro
	.set	reorder

$L254:
	lw	$8,0($4)
	lhu	$7,2($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 #NO_APP
	andi	$8,$8,0xf
	andi	$7,$7,0xffff
$L230:
	sll	$8,$8,2
	subu	$7,$7,$8
	slt	$3,$7,$3
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L178
	li	$3,536870912			# 0x20000000
	.set	macro
	.set	reorder

	lui	$2,%hi(LogFlag)
	lw	$7,%lo(LogFlag)($2)
	or	$3,$7,$3
	sw	$3,%lo(LogFlag)($2)
$L243:
	lw	$7,12($4)
	lui	$3,%hi(attack_saddr+88)
	li	$2,-1			# 0xffffffffffffffff
	sw	$7,%lo(attack_saddr+88)($3)
	lw	$7,16($4)
	lui	$3,%hi(attack_daddr+88)
	sw	$7,%lo(attack_daddr+88)($3)
	.set	noreorder
	.set	nomacro
	j	$L176
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L273:
	sll	$8,$8,2
	addu	$8,$4,$8
	lbu	$3,0($8)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L183
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	lui	$3,%hi(icmp_echo_request)
	lw	$3,%lo(icmp_echo_request)($3)
	bne	$3,$0,$L184
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
	.set	noreorder
	.set	nomacro
	j	$L183
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L274:
	sll	$9,$9,2
	addu	$9,$12,$9
	lw	$9,0($9)
	j	$9
	.rdata
	.align	2
	.align	2
$L168:
	.word	$L164
	.word	$L164
	.word	$L165
	.word	$L166
	.word	$L162
	.word	$L162
	.word	$L162
	.word	$L162
	.word	$L167
	.text
$L164:
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	j	$L238
	addiu	$8,$8,1
	.set	macro
	.set	reorder

$L167:
	lbu	$9,1($3)
	.set	noreorder
	.set	nomacro
	bne	$9,$15,$L162
	addiu	$3,$3,10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L238
	addiu	$8,$8,10
	.set	macro
	.set	reorder

$L166:
	lbu	$9,1($3)
	.set	noreorder
	.set	nomacro
	bne	$9,$14,$L162
	addiu	$3,$3,3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L238
	addiu	$8,$8,3
	.set	macro
	.set	reorder

$L165:
	lbu	$9,1($3)
	.set	noreorder
	.set	nomacro
	bne	$9,$13,$L162
	addiu	$3,$3,4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L238
	addiu	$8,$8,4
	.set	macro
	.set	reorder

$L179:
	lw	$8,0($4)
	lhu	$7,2($4)
	li	$3,8			# 0x8
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 #NO_APP
	andi	$8,$8,0xf
	.set	noreorder
	.set	nomacro
	j	$L230
	andi	$7,$7,0xffff
	.set	macro
	.set	reorder

$L271:
	lw	$6,28($5)
	bne	$3,$6,$L222
	.set	noreorder
	.set	nomacro
	j	$L220
	sb	$0,0($5)
	.set	macro
	.set	reorder

$L128:
	lb	$7,0($3)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$7,$2,$L313
	move	$2,$0
	.set	macro
	.set	reorder

	lui	$7,%hi(cur_p_pkt)
	.set	noreorder
	.set	nomacro
	j	$L129
	sw	$3,%lo(cur_p_pkt)($7)
	.set	macro
	.set	reorder

$L156:
	slt	$8,$8,201
	.set	noreorder
	.set	nomacro
	bne	$8,$0,$L275
	lui	$7,%hi(LogFlag)
	.set	macro
	.set	reorder

	lw	$8,%lo(LogFlag)($7)
	.set	noreorder
	.set	nomacro
	j	$L241
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L246:
	lbu	$8,13($6)
	li	$2,2			# 0x2
	andi	$8,$8,0x3f
	.set	noreorder
	.set	nomacro
	bne	$8,$2,$L132
	move	$2,$0
	.set	macro
	.set	reorder

	lui	$9,%hi(dosFilter_block)
	lw	$8,%lo(dosFilter_block)($9)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$8,$2,$L276
	lui	$2,%hi(attack_saddr+4)
	.set	macro
	.set	reorder

$L131:
	lw	$2,4($10)
	lui	$8,%hi(per_syn_threshold)
	addiu	$2,$2,1
	sw	$2,4($10)
	lw	$8,%lo(per_syn_threshold)($8)
	slt	$2,$8,$2
	bne	$2,$0,$L133
	.set	noreorder
	.set	nomacro
	j	$L132
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L249:
	lui	$9,%hi(dosFilter_block)
	lw	$11,%lo(dosFilter_block)($9)
	.set	noreorder
	.set	nomacro
	beq	$11,$8,$L277
	lui	$8,%hi(attack_saddr+16)
	.set	macro
	.set	reorder

$L144:
	lw	$8,16($10)
	addiu	$8,$8,1
	sw	$8,16($10)
	lui	$10,%hi(per_icmp_threshold)
	lw	$10,%lo(per_icmp_threshold)($10)
	slt	$8,$10,$8
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L143
	andi	$3,$3,0x100
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L314
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$10,$L314
	lui	$8,%hi(attack_saddr+16)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L143
	sw	$8,%lo(LogFlag)($3)
	.set	macro
	.set	reorder

$L248:
	lui	$9,%hi(dosFilter_block)
	lw	$11,%lo(dosFilter_block)($9)
	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$11,$8,$L278
	lui	$8,%hi(attack_saddr+12)
	.set	macro
	.set	reorder

$L141:
	lw	$8,12($10)
	addiu	$8,$8,1
	sw	$8,12($10)
	lui	$10,%hi(per_udp_threshold)
	lw	$10,%lo(per_udp_threshold)($10)
	slt	$8,$10,$8
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L140
	andi	$3,$3,0x80
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L315
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$10,$L315
	lui	$8,%hi(attack_saddr+12)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L140
	sw	$8,%lo(LogFlag)($3)
	.set	macro
	.set	reorder

$L250:
	sll	$11,$11,2
	addu	$11,$4,$11
	lbu	$8,0($11)
	li	$9,8			# 0x8
	.set	noreorder
	.set	nomacro
	beq	$8,$9,$L316
	lui	$9,%hi(dosFilter_block)
	.set	macro
	.set	reorder

	bne	$8,$0,$L146
$L316:
	lw	$11,%lo(dosFilter_block)($9)
	li	$8,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$11,$8,$L279
	lui	$8,%hi(attack_saddr+20)
	.set	macro
	.set	reorder

$L148:
	lw	$8,20($10)
	addiu	$8,$8,1
	sw	$8,20($10)
	lui	$10,%hi(per_ping_threshold)
	lw	$10,%lo(per_ping_threshold)($10)
	slt	$8,$10,$8
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L146
	li	$8,2097152			# 0x200000
	.set	macro
	.set	reorder

	and	$3,$3,$8
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L317
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$10,$L146
	lui	$3,%hi(LogFlag)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L146
	sw	$8,%lo(LogFlag)($3)
	.set	macro
	.set	reorder

$L247:
	lbu	$8,13($6)
	li	$9,1			# 0x1
	andi	$8,$8,0x3f
	.set	noreorder
	.set	nomacro
	bne	$8,$9,$L137
	lui	$9,%hi(dosFilter_block)
	.set	macro
	.set	reorder

	lw	$11,%lo(dosFilter_block)($9)
	.set	noreorder
	.set	nomacro
	beq	$11,$8,$L280
	lui	$8,%hi(attack_saddr+8)
	.set	macro
	.set	reorder

$L138:
	lw	$8,8($10)
	addiu	$8,$8,1
	sw	$8,8($10)
	lui	$10,%hi(per_fin_threshold)
	lw	$10,%lo(per_fin_threshold)($10)
	slt	$8,$10,$8
	.set	noreorder
	.set	nomacro
	beq	$8,$0,$L137
	andi	$3,$3,0x40
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L318
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$10,$L318
	lui	$8,%hi(attack_saddr+8)
	.set	macro
	.set	reorder

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
	.set	noreorder
	.set	nomacro
	j	$L137
	sw	$8,%lo(LogFlag)($3)
	.set	macro
	.set	reorder

$L275:
	.set	noreorder
	.set	nomacro
	j	$L149
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L272:
	sh	$0,34($5)
	.set	noreorder
	.set	nomacro
	j	$L220
	sh	$0,32($5)
	.set	macro
	.set	reorder

$L251:
	lhu	$3,36($7)
	beq	$3,$0,$L154
	lhu	$9,2($6)
	beq	$3,$9,$L155
	addiu	$8,$8,1
	sw	$8,24($7)
	move	$9,$3
$L155:
	.set	noreorder
	.set	nomacro
	bne	$9,$0,$L319
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

$L154:
	lhu	$3,2($6)
	.set	noreorder
	.set	nomacro
	j	$L153
	sh	$3,36($7)
	.set	macro
	.set	reorder

$L235:
	lhu	$3,36($7)
	bne	$3,$0,$L236
	lw	$8,24($7)
$L151:
	lhu	$3,2($6)
$L284:
	sh	$3,36($7)
	.set	noreorder
	.set	nomacro
	j	$L150
	lbu	$3,9($4)
	.set	macro
	.set	reorder

$L236:
	lhu	$9,2($6)
	beq	$3,$9,$L281
	lw	$8,24($7)
	move	$9,$3
	addiu	$8,$8,1
	.set	noreorder
	.set	nomacro
	beq	$9,$0,$L151
	sw	$8,24($7)
	.set	macro
	.set	reorder

$L282:
	.set	noreorder
	.set	nomacro
	j	$L150
	lbu	$3,9($4)
	.set	macro
	.set	reorder

$L189:
	lhu	$3,6($4)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$3,$3,0x2000
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L187
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L283
	lui	$2,%hi(LogFlag)
	.set	macro
	.set	reorder

$L277:
	lw	$11,%lo(attack_saddr+16)($8)
	lw	$8,12($4)
	bne	$11,$8,$L144
	.set	noreorder
	.set	nomacro
	j	$L143
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L278:
	lw	$11,%lo(attack_saddr+12)($8)
	lw	$8,12($4)
	bne	$11,$8,$L141
	.set	noreorder
	.set	nomacro
	j	$L140
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L279:
	lw	$11,%lo(attack_saddr+20)($8)
	lw	$8,12($4)
	bne	$11,$8,$L148
	.set	noreorder
	.set	nomacro
	j	$L146
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L281:
	.set	noreorder
	.set	nomacro
	bne	$9,$0,$L282
	lw	$8,24($7)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L284
	lhu	$3,2($6)
	.set	macro
	.set	reorder

$L133:
	andi	$3,$3,0x20
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L320
	lw	$3,%lo(dos_item_global)($5)
	.set	macro
	.set	reorder

	blez	$8,$L320
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
$L320:
	.set	noreorder
	.set	nomacro
	j	$L132
	move	$2,$0
	.set	macro
	.set	reorder

$L225:
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
	.set	noreorder
	.set	nomacro
	j	$L220
	sw	$4,%lo(attack_daddr+40)($3)
	.set	macro
	.set	reorder

$L276:
	lw	$11,%lo(attack_saddr+4)($2)
	lw	$8,12($4)
	.set	noreorder
	.set	nomacro
	bne	$11,$8,$L131
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L285
	andi	$8,$3,0x44
	.set	macro
	.set	reorder

$L280:
	lw	$11,%lo(attack_saddr+8)($8)
	lw	$8,12($4)
	bne	$11,$8,$L138
	.set	noreorder
	.set	nomacro
	j	$L137
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	dos_filter_ipv4__action
	.size	dos_filter_ipv4__action, .-dos_filter_ipv4__action
	.section	.rodata.str1.4
	.align	2
$LC18:
	.ascii	"%u.%u.%u.%u\000"
	.text
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
	.text
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
$L325:
	lw	$4,0($16)
	bne	$4,$0,$L336
	addiu	$16,$16,4

	bne	$16,$17,$L325
	nop

	lui	$16,%hi(dos_pkt)
$L341:
	jal	local_bh_disable
	addiu	$16,$16,%lo(dos_pkt)

	lb	$2,0($16)
	lui	$18,%hi(dos_pkt+40920)
	lui	$21,%hi($LC21)
	li	$17,1			# 0x1
	addiu	$18,$18,%lo(dos_pkt+40920)
	addiu	$21,$21,%lo($LC21)
	beq	$2,$17,$L337
	addiu	$20,$fp,%lo(ip_name0)

$L326:
	beq	$16,$18,$L327
	nop

	addiu	$16,$16,40
$L342:
	lb	$2,0($16)
	bne	$2,$17,$L326
	nop

$L337:
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

	bne	$16,$18,$L342
	addiu	$16,$16,40

	addiu	$16,$16,-40
$L327:
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
	j	$L332
	addiu	$22,$2,%lo(ip_name1)

$L329:
	lw	$4,0($17)
	beq	$4,$0,$L331
	nop

$L330:
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

$L331:
	addiu	$16,$16,4
	addiu	$17,$17,4
	beq	$16,$21,$L339
	addiu	$18,$18,4

$L332:
	lw	$2,0($16)
	beq	$2,$0,$L329
	nop

	j	$L330
	lw	$4,0($17)

$L336:
	jal	IP2NAME
	move	$5,$18

	move	$6,$2
	move	$4,$19
	jal	seq_printf
	move	$5,$20

	bne	$16,$17,$L325
	nop

	j	$L341
	lui	$16,%hi(dos_pkt)

$L339:
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
	.rdata
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
	.local	_tcpDosScanBitmap
	.comm	_tcpDosScanBitmap,64,4
	.local	LogFlag
	.comm	LogFlag,4,4
	.local	dos_pkt
	.comm	dos_pkt,40960,4
	.local	cur_p_pkt
	.comm	cur_p_pkt,4,4
	.local	attack_saddr
	.comm	attack_saddr,108,4
	.local	attack_daddr
	.comm	attack_daddr,108,4
	.local	ip_name0
	.comm	ip_name0,16,4
	.local	ip_name1
	.comm	ip_name1,16,4
	.local	icmp_echo_request
	.comm	icmp_echo_request,4,4
	.local	icmp_echo_request_time
	.comm	icmp_echo_request_time,4,4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
