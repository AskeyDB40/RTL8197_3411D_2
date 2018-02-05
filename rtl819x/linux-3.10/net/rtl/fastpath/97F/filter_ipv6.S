	.file	1 "filter_ipv6.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
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
	.text
	.align	2
	.globl	dos_timer_ipv6_handler
	.set	nomips16
	.ent	dos_timer_ipv6_handler
	.type	dos_timer_ipv6_handler, @function
dos_timer_ipv6_handler:
	.frame	$sp,96,$31		# vars= 0, regs= 5/0, args= 72, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$31,92($sp)
	sw	$19,88($sp)
	sw	$18,84($sp)
	sw	$17,80($sp)
	sw	$16,76($sp)
	lui	$12,%hi(dos_item_ipv6)
	lw	$11,%lo(dos_item_ipv6)($12)
	andi	$2,$11,0xe
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L35
	lui	$5,%hi(dos_pkt+32736)
	.set	macro
	.set	reorder

	lui	$18,%hi(dos_pkt)
	addiu	$2,$18,%lo(dos_pkt)
	lb	$3,0($2)
	li	$4,1			# 0x1
	addiu	$5,$5,%lo(dos_pkt+32736)
	move	$6,$0
	move	$7,$0
	andi	$9,$11,0x4
	.set	noreorder
	.set	nomacro
	beq	$3,$4,$L36
	andi	$8,$11,0x8
	.set	macro
	.set	reorder

$L6:
	beq	$2,$5,$L8
$L37:
	addiu	$2,$2,32
	lb	$3,0($2)
	bne	$3,$4,$L6
$L36:
	beq	$9,$0,$L7
	lw	$3,8($2)
	slt	$10,$0,$3
	addu	$3,$7,$3
	movn	$7,$3,$10
$L7:
	beq	$8,$0,$L6
	lw	$3,12($2)
	slt	$10,$0,$3
	addu	$3,$6,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$5,$L37
	movn	$6,$3,$10
	.set	macro
	.set	reorder

$L8:
	andi	$11,$11,0x2
	.set	noreorder
	.set	nomacro
	bne	$11,$0,$L10
	lui	$19,%hi(smurf_pkts)
	.set	macro
	.set	reorder

	move	$3,$0
$L11:
	lui	$2,%hi(Ipv6_smurf_threshold)
	lw	$2,%lo(Ipv6_smurf_threshold)($2)
	slt	$3,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L38
	lui	$17,%hi(LogIpv6Flag)
	.set	macro
	.set	reorder

	bne	$11,$0,$L14
$L29:
	lui	$17,%hi(LogIpv6Flag)
$L38:
	lw	$16,%lo(LogIpv6Flag)($17)
$L13:
	lui	$2,%hi(Ipv6_icmp_too_big_threshold)
	lw	$2,%lo(Ipv6_icmp_too_big_threshold)($2)
	slt	$7,$2,$7
	.set	noreorder
	.set	nomacro
	beq	$7,$0,$L31
	lw	$3,%lo(dos_item_ipv6)($12)
	.set	macro
	.set	reorder

	andi	$3,$3,0x4
	bne	$3,$0,$L18
$L31:
	lw	$16,%lo(LogIpv6Flag)($17)
$L17:
	lui	$2,%hi(Ipv6_ns_threshold)
	lw	$2,%lo(Ipv6_ns_threshold)($2)
	slt	$6,$2,$6
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L33
	lw	$3,%lo(dos_item_ipv6)($12)
	.set	macro
	.set	reorder

	andi	$3,$3,0x8
	beq	$3,$0,$L33
	.set	noreorder
	.set	nomacro
	blez	$2,$L33
	ori	$16,$16,0x8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L5
	sw	$16,%lo(LogIpv6Flag)($17)
	.set	macro
	.set	reorder

$L10:
	addiu	$2,$19,%lo(smurf_pkts)
	lw	$2,4($2)
	slt	$3,$2,0
	movn	$2,$0,$3
	.set	noreorder
	.set	nomacro
	j	$L11
	move	$3,$2
	.set	macro
	.set	reorder

$L33:
	lw	$16,%lo(LogIpv6Flag)($17)
$L5:
	andi	$2,$16,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L39
	andi	$2,$16,0x4
	.set	macro
	.set	reorder

	lui	$3,%hi(attack_v6_saddr)
	lui	$2,%hi(attack_v6_daddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lui	$4,%hi($LC0)
	lhu	$14,30($3)
	lhu	$8,30($2)
	lhu	$5,16($3)
	lhu	$6,18($3)
	lhu	$7,20($3)
	lhu	$31,22($3)
	lhu	$25,24($3)
	lhu	$24,26($3)
	lhu	$15,28($3)
	lhu	$13,16($2)
	lhu	$12,18($2)
	lhu	$11,20($2)
	lhu	$10,22($2)
	lhu	$9,24($2)
	lhu	$3,26($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$14, $14			

 # 0 "" 2
 #NO_APP
	lhu	$2,28($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$14,$14,0xffff
	andi	$8,$8,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$31, $31			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$25, $25			

 # 0 "" 2
 #NO_APP
	andi	$31,$31,0xffff
	andi	$25,$25,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$24, $24			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$15, $15			

 # 0 "" 2
 #NO_APP
	andi	$24,$24,0xffff
	andi	$15,$15,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$13, $13			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$12, $12			

 # 0 "" 2
 #NO_APP
	andi	$13,$13,0xffff
	andi	$12,$12,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$11, $11			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$10, $10			

 # 0 "" 2
 #NO_APP
	andi	$11,$11,0xffff
	andi	$10,$10,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$9, $9			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$9,$9,0xffff
	andi	$3,$3,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	addiu	$4,$4,%lo($LC0)
	andi	$2,$2,0xffff
	andi	$5,$5,0xffff
	andi	$6,$6,0xffff
	andi	$7,$7,0xffff
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
	sw	$3,56($sp)
	sw	$2,60($sp)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	sw	$8,64($sp)
	.set	macro
	.set	reorder

	andi	$2,$16,0x4
$L39:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L24
	lui	$3,%hi(attack_v6_saddr)
	.set	macro
	.set	reorder

	lui	$2,%hi(attack_v6_daddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lui	$4,%hi($LC1)
	lhu	$14,46($3)
	lhu	$8,46($2)
	lhu	$5,32($3)
	lhu	$6,34($3)
	lhu	$7,36($3)
	lhu	$31,38($3)
	lhu	$25,40($3)
	lhu	$24,42($3)
	lhu	$15,44($3)
	lhu	$13,32($2)
	lhu	$12,34($2)
	lhu	$11,36($2)
	lhu	$10,38($2)
	lhu	$9,40($2)
	lhu	$3,42($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$14, $14			

 # 0 "" 2
 #NO_APP
	lhu	$2,44($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$14,$14,0xffff
	andi	$8,$8,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$31, $31			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$25, $25			

 # 0 "" 2
 #NO_APP
	andi	$31,$31,0xffff
	andi	$25,$25,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$24, $24			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$15, $15			

 # 0 "" 2
 #NO_APP
	andi	$24,$24,0xffff
	andi	$15,$15,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$13, $13			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$12, $12			

 # 0 "" 2
 #NO_APP
	andi	$13,$13,0xffff
	andi	$12,$12,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$11, $11			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$10, $10			

 # 0 "" 2
 #NO_APP
	andi	$11,$11,0xffff
	andi	$10,$10,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$9, $9			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$9,$9,0xffff
	andi	$3,$3,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	addiu	$4,$4,%lo($LC1)
	andi	$2,$2,0xffff
	andi	$5,$5,0xffff
	andi	$6,$6,0xffff
	andi	$7,$7,0xffff
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
	sw	$3,56($sp)
	sw	$2,60($sp)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	sw	$8,64($sp)
	.set	macro
	.set	reorder

$L24:
	andi	$16,$16,0x8
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L25
	lui	$3,%hi(attack_v6_saddr)
	.set	macro
	.set	reorder

	lui	$2,%hi(attack_v6_daddr)
	addiu	$3,$3,%lo(attack_v6_saddr)
	addiu	$2,$2,%lo(attack_v6_daddr)
	lui	$4,%hi($LC2)
	lhu	$14,62($3)
	lhu	$8,62($2)
	lhu	$5,48($3)
	lhu	$6,50($3)
	lhu	$7,52($3)
	lhu	$16,54($3)
	lhu	$25,56($3)
	lhu	$24,58($3)
	lhu	$15,60($3)
	lhu	$13,48($2)
	lhu	$12,50($2)
	lhu	$11,52($2)
	lhu	$10,54($2)
	lhu	$9,56($2)
	lhu	$3,58($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$14, $14			

 # 0 "" 2
 #NO_APP
	lhu	$2,60($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$8, $8			

 # 0 "" 2
 #NO_APP
	andi	$14,$14,0xffff
	andi	$8,$8,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$6, $6			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$7, $7			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$16, $16			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$25, $25			

 # 0 "" 2
 #NO_APP
	andi	$16,$16,0xffff
	andi	$25,$25,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$24, $24			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$15, $15			

 # 0 "" 2
 #NO_APP
	andi	$24,$24,0xffff
	andi	$15,$15,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$13, $13			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$12, $12			

 # 0 "" 2
 #NO_APP
	andi	$13,$13,0xffff
	andi	$12,$12,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$11, $11			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$10, $10			

 # 0 "" 2
 #NO_APP
	andi	$11,$11,0xffff
	andi	$10,$10,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$9, $9			

 # 0 "" 2
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	andi	$9,$9,0xffff
	andi	$3,$3,0xffff
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	addiu	$4,$4,%lo($LC2)
	andi	$2,$2,0xffff
	andi	$5,$5,0xffff
	andi	$6,$6,0xffff
	andi	$7,$7,0xffff
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
	sw	$3,56($sp)
	sw	$2,60($sp)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	sw	$8,64($sp)
	.set	macro
	.set	reorder

$L25:
	addiu	$4,$18,%lo(dos_pkt)
	move	$5,$0
	li	$6,32768			# 0x8000
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$0,%lo(LogIpv6Flag)($17)
	.set	macro
	.set	reorder

	addiu	$4,$19,%lo(smurf_pkts)
	lw	$31,92($sp)
	lw	$19,88($sp)
	lw	$18,84($sp)
	lw	$17,80($sp)
	lw	$16,76($sp)
	move	$5,$0
	li	$6,32			# 0x20
	.set	noreorder
	.set	nomacro
	j	memset
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

$L35:
	lui	$17,%hi(LogIpv6Flag)
	lw	$16,%lo(LogIpv6Flag)($17)
	lui	$18,%hi(dos_pkt)
	.set	noreorder
	.set	nomacro
	j	$L5
	lui	$19,%hi(smurf_pkts)
	.set	macro
	.set	reorder

$L18:
	.set	noreorder
	.set	nomacro
	blez	$2,$L31
	ori	$16,$16,0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L17
	sw	$16,%lo(LogIpv6Flag)($17)
	.set	macro
	.set	reorder

$L14:
	.set	noreorder
	.set	nomacro
	blez	$2,$L29
	lui	$17,%hi(LogIpv6Flag)
	.set	macro
	.set	reorder

	lw	$16,%lo(LogIpv6Flag)($17)
	ori	$16,$16,0x2
	.set	noreorder
	.set	nomacro
	j	$L13
	sw	$16,%lo(LogIpv6Flag)($17)
	.set	macro
	.set	reorder

	.end	dos_timer_ipv6_handler
	.size	dos_timer_ipv6_handler, .-dos_timer_ipv6_handler
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
	bne	$4,$0,$L47
	andi	$16,$5,0x00ff

$L41:
	move	$2,$0
$L44:
	lw	$31,36($sp)
$L49:
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

$L47:
	lw	$17,184($4)
	beq	$17,$0,$L44
	move	$2,$0

	lw	$3,0($17)
	li	$2,96			# 0x60
	andi	$3,$3,0xf0
	bne	$3,$2,$L44
	move	$2,$0

	lbu	$2,6($17)
	move	$4,$2
	jal	ipv6_ext_hdr
	sb	$2,16($sp)

	bne	$2,$0,$L48
	lbu	$2,16($sp)

	bne	$2,$16,$L41
	li	$2,40			# 0x28

	j	$L44
	addu	$2,$17,$2

$L48:
	move	$4,$18
	li	$5,40			# 0x28
	addiu	$6,$sp,16
	jal	ipv6_skip_exthdr
	addiu	$7,$sp,18

	bltz	$2,$L41
	lbu	$3,16($sp)

	bne	$3,$16,$L41
	addu	$2,$17,$2

	j	$L49
	lw	$31,36($sp)

	.set	macro
	.set	reorder
	.end	_rtl_get_ipv6_skip_exthdr
	.size	_rtl_get_ipv6_skip_exthdr, .-_rtl_get_ipv6_skip_exthdr
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
	beq	$3,$0,$L54
	move	$16,$0

	andi	$3,$2,0x2
	bne	$3,$0,$L71
	nop

$L56:
	andi	$2,$2,0x8
	bne	$2,$0,$L62
	move	$4,$18

$L70:
	lw	$20,184($18)
$L63:
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

	beq	$2,$0,$L80
	move	$4,$20

	lb	$2,0($19)
	beq	$2,$0,$L72
	move	$5,$20

$L80:
	move	$5,$21
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L79
	move	$2,$16

	lb	$3,0($19)
	li	$2,1			# 0x1
	beq	$3,$2,$L73
	lui	$20,%hi(cur_p_pkt)

$L54:
	move	$2,$16
$L79:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L62:
$L78:
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	beq	$2,$0,$L63
	lw	$20,184($18)

	lbu	$3,0($2)
	li	$2,135			# 0x87
	bne	$3,$2,$L63
	lui	$2,%hi(LogIpv6Flag)

	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x8
	beq	$4,$0,$L81
	lui	$5,%hi(ipv6Ns_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	beq	$5,$4,$L74
	lui	$5,%hi(ipv6Ns_pkts)

$L81:
	addiu	$5,$5,%lo(ipv6Ns_pkts)
	lw	$4,12($5)
	lui	$6,%hi(Ipv6_ns_threshold)
	lw	$6,%lo(Ipv6_ns_threshold)($6)
	addiu	$4,$4,1
	sw	$4,12($5)
	slt	$4,$6,$4
	beq	$4,$0,$L63
	nop

	blez	$6,$L63
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

	j	$L63
	lw	$20,184($18)

$L71:
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	beq	$2,$0,$L69
	lw	$16,184($18)

	lbu	$3,0($2)
	li	$2,129			# 0x81
	beq	$3,$2,$L58
	lui	$2,%hi(LogIpv6Flag)

$L69:
	lw	$2,%lo(dos_item_ipv6)($17)
	move	$16,$0
$L77:
	andi	$2,$2,0x8
	beq	$2,$0,$L70
	move	$4,$18

	j	$L78
	nop

$L73:
	sw	$19,%lo(cur_p_pkt)($20)
$L66:
	lw	$2,%lo(dos_item_ipv6)($17)
	andi	$2,$2,0x4
	beq	$2,$0,$L79
	move	$2,$16

	move	$4,$18
	jal	_rtl_get_ipv6_skip_exthdr
	li	$5,58			# 0x3a

	lw	$18,184($18)
	beq	$2,$0,$L54
	lw	$17,%lo(cur_p_pkt)($20)

	lbu	$3,0($2)
	li	$2,2			# 0x2
	bne	$3,$2,$L79
	move	$2,$16

	lui	$19,%hi(dosFilter_block)
	lw	$3,%lo(dosFilter_block)($19)
	li	$2,1			# 0x1
	beq	$3,$2,$L75
	lui	$4,%hi(attack_v6_saddr+32)

$L67:
	lw	$2,8($17)
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	addiu	$2,$2,1
	sw	$2,8($17)
	lw	$3,%lo(Ipv6_icmp_too_big_threshold)($3)
	slt	$2,$3,$2
	beq	$2,$0,$L79
	move	$2,$16

	blez	$3,$L54
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

	j	$L79
	move	$2,$16

$L72:
	move	$4,$21
	jal	memcpy
	li	$6,16			# 0x10

	lui	$20,%hi(cur_p_pkt)
	li	$2,1			# 0x1
	sb	$2,0($19)
	j	$L66
	sw	$19,%lo(cur_p_pkt)($20)

$L58:
	lw	$3,%lo(LogIpv6Flag)($2)
	andi	$4,$3,0x2
	beq	$4,$0,$L82
	lui	$5,%hi(smurf_pkts)

	lui	$4,%hi(dosFilter_block)
	lw	$5,%lo(dosFilter_block)($4)
	li	$4,1			# 0x1
	beq	$5,$4,$L76
	lui	$5,%hi(smurf_pkts)

$L82:
	addiu	$5,$5,%lo(smurf_pkts)
	lw	$4,4($5)
	lui	$6,%hi(Ipv6_smurf_threshold)
	lw	$6,%lo(Ipv6_smurf_threshold)($6)
	addiu	$4,$4,1
	sw	$4,4($5)
	slt	$4,$6,$4
	beq	$4,$0,$L69
	nop

	blez	$6,$L69
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
	j	$L77
	lw	$2,%lo(dos_item_ipv6)($17)

$L74:
	j	$L63
	li	$16,-1			# 0xffffffffffffffff

$L76:
	lw	$2,%lo(dos_item_ipv6)($17)
	j	$L56
	li	$16,-1			# 0xffffffffffffffff

$L75:
	addiu	$4,$4,%lo(attack_v6_saddr+32)
	addiu	$5,$18,8
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L67
	nop

	j	$L54
	li	$16,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	dos_filter_ipv6__action
	.size	dos_filter_ipv6__action, .-dos_filter_ipv6__action
	.local	dos_pkt
	.comm	dos_pkt,32768,4
	.local	cur_p_pkt
	.comm	cur_p_pkt,4,4
	.local	attack_v6_saddr
	.comm	attack_v6_saddr,64,4
	.local	attack_v6_daddr
	.comm	attack_v6_daddr,64,4
	.local	LogIpv6Flag
	.comm	LogIpv6Flag,4,4
	.local	smurf_pkts
	.comm	smurf_pkts,32,4
	.local	ipv6Ns_pkts
	.comm	ipv6Ns_pkts,32,4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
