	.file	1 "filter.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.dos_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_read_proc
	.type	dos_read_proc, @function
dos_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(dos_read_proc_func)
	lw	$2,%lo(dos_read_proc_func)($2)
	nop
	beq	$2,$0,$L2
	nop

	jalr	$2
	nop

$L2:
	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	dos_read_proc
	.size	dos_read_proc, .-dos_read_proc
	.section	.text.dos_single_open,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_single_open
	.type	dos_single_open, @function
dos_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(dos_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(dos_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	dos_single_open
	.size	dos_single_open, .-dos_single_open
	.section	.text.filter_table_single_open,"ax",@progbits
	.align	2
	.globl	filter_table_single_open
	.set	nomips16
	.ent	filter_table_single_open
	.type	filter_table_single_open, @function
filter_table_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(filter_table_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(filter_table_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	filter_table_single_open
	.size	filter_table_single_open, .-filter_table_single_open
	.section	.text.rtl_schedule_filter_fastpath,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	rtl_schedule_filter_fastpath
	.type	rtl_schedule_filter_fastpath, @function
rtl_schedule_filter_fastpath:
	.frame	$sp,40,$31		# vars= 16, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	addiu	$4,$sp,16
	jal	do_gettimeofday
	sw	$5,24($sp)

	lw	$2,16($sp)
	li	$3,-1037172736			# 0xffffffffc22e0000
	addiu	$3,$3,17671
	mult	$2,$3
	sra	$4,$2,31
	li	$7,-1840709632			# 0xffffffff92490000
	addiu	$7,$7,9363
	lw	$5,24($sp)
	nop
	lw	$6,8($5)
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,16
	subu	$3,$3,$4
	addiu	$3,$3,4
	mult	$3,$7
	sra	$8,$3,31
	mfhi	$7
	addu	$7,$7,$3
	sra	$7,$7,2
	subu	$7,$7,$8
	sll	$8,$7,3
	subu	$7,$8,$7
	subu	$3,$3,$7
	li	$7,1			# 0x1
	sll	$3,$7,$3
	ori	$3,$3,0x80
	and	$6,$3,$6
	beq	$6,$0,$L12
	li	$3,-1851654144			# 0xffffffff91a20000

	ori	$3,$3,0xb3c5
	mult	$2,$3
	li	$6,-2004353024			# 0xffffffff88880000
	ori	$6,$6,0x8889
	lw	$7,0($5)
	mfhi	$3
	addu	$3,$3,$2
	sra	$3,$3,11
	mult	$2,$6
	subu	$3,$3,$4
	sra	$8,$3,31
	mfhi	$9
	addu	$2,$9,$2
	li	$9,715784192			# 0x2aaa0000
	ori	$9,$9,0xaaab
	mult	$3,$9
	sra	$2,$2,5
	subu	$2,$2,$4
	sra	$4,$2,31
	mfhi	$9
	sra	$9,$9,2
	subu	$8,$9,$8
	mult	$2,$6
	sll	$10,$8,3
	sll	$8,$8,5
	subu	$8,$8,$10
	subu	$3,$3,$8
	sll	$9,$3,2
	sll	$3,$3,6
	subu	$3,$3,$9
	mfhi	$6
	addu	$6,$6,$2
	sra	$6,$6,5
	subu	$4,$6,$4
	sll	$8,$4,2
	sll	$4,$4,6
	subu	$4,$4,$8
	subu	$2,$2,$4
	bne	$7,$0,$L8
	addu	$2,$3,$2

	lw	$3,4($5)
	nop
	beq	$3,$0,$L13
	sltu	$2,$3,$2

	xori	$2,$2,0x1
$L15:
	lw	$31,36($sp)
	nop
	j	$31
	addiu	$sp,$sp,40

$L8:
	sltu	$7,$2,$7
	beq	$7,$0,$L14
	nop

$L12:
	lw	$31,36($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

$L13:
	lw	$31,36($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,40

$L14:
	lw	$3,4($5)
	nop
	sltu	$2,$3,$2
	j	$L15
	xori	$2,$2,0x1

	.set	macro
	.set	reorder
	.end	rtl_schedule_filter_fastpath
	.size	rtl_schedule_filter_fastpath, .-rtl_schedule_filter_fastpath
	.section	.text.rtl_ip_range_filter_fastpath,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	rtl_ip_range_filter_fastpath
	.type	rtl_ip_range_filter_fastpath, @function
rtl_ip_range_filter_fastpath:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	jal	rtl_ip_hdr
	sw	$5,16($sp)

	lw	$5,16($sp)
	lw	$2,12($2)
	lw	$3,0($5)
	nop
	sltu	$3,$2,$3
	bne	$3,$0,$L18
	nop

	lw	$3,4($5)
	lw	$31,28($sp)
	sltu	$2,$3,$2
	xori	$2,$2,0x1
	j	$31
	addiu	$sp,$sp,32

$L18:
	lw	$31,28($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_ip_range_filter_fastpath
	.size	rtl_ip_range_filter_fastpath, .-rtl_ip_range_filter_fastpath
	.section	.text.dos_timer_fn,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_timer_fn
	.type	dos_timer_fn, @function
dos_timer_fn:
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
	lui	$16,%hi(dos_item_global)
	lw	$2,%lo(dos_item_global)($16)
	nop
	bne	$2,$0,$L20
	lui	$18,%hi(dos_item_ipv6)

	lw	$3,%lo(dos_item_ipv6)($18)
	nop
	bne	$3,$0,$L39
	lui	$19,%hi(dosFilter_block)

	lui	$2,%hi(none_dos_drop_pkt_cnt)
$L42:
	lw	$3,%lo(none_dos_drop_pkt_cnt)($2)
	nop
	sltu	$4,$3,3001
	bne	$4,$0,$L40
	lui	$4,%hi(dos_item_backup)

$L36:
	lw	$3,%lo(dos_item_global)($16)
	nop
	bne	$3,$0,$L35
	lui	$4,%hi(dos_item_backup)

$L30:
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
$L29:
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$4,%hi(dos_timer)
	addiu	$4,$4,%lo(dos_timer)
	addiu	$5,$5,100
	j	mod_timer
	addiu	$sp,$sp,40

$L20:
	lui	$19,%hi(dosFilter_block)
$L39:
	lw	$4,%lo(dosFilter_block)($19)
	li	$3,1			# 0x1
	beq	$4,$3,$L34
	lui	$17,%hi(block_count)

	beq	$2,$0,$L23
	lui	$2,%hi(dos_timer_ipv4_func)

$L38:
	lw	$2,%lo(dos_timer_ipv4_func)($2)
	nop
	beq	$2,$0,$L23
	nop

	jalr	$2
	nop

$L23:
	lw	$2,%lo(dos_item_ipv6)($18)
	nop
	beq	$2,$0,$L41
	lui	$2,%hi(block_time)

	lui	$2,%hi(dos_timer_ipv6_func)
	lw	$2,%lo(dos_timer_ipv6_func)($2)
	nop
	beq	$2,$0,$L24
	nop

	jalr	$2
	nop

$L24:
	lui	$2,%hi(block_time)
$L41:
	lw	$3,%lo(block_count)($17)
	lw	$2,%lo(block_time)($2)
	nop
	slt	$2,$3,$2
	beq	$2,$0,$L25
	li	$2,4194304			# 0x400000

	lw	$3,%lo(dos_item_global)($16)
	nop
	and	$2,$3,$2
	bne	$2,$0,$L42
	lui	$2,%hi(none_dos_drop_pkt_cnt)

$L25:
	sw	$0,%lo(dosFilter_block)($19)
	jal	clear_attack_address
	sw	$0,%lo(block_count)($17)

	lui	$2,%hi(none_dos_drop_pkt_cnt)
	lw	$3,%lo(none_dos_drop_pkt_cnt)($2)
	nop
	sltu	$4,$3,3001
	beq	$4,$0,$L36
	lui	$4,%hi(dos_item_backup)

$L40:
	lw	$5,%lo(dos_item_backup)($4)
	nop
	beq	$5,$0,$L28
	nop

	sw	$5,%lo(dos_item_global)($16)
	sw	$0,%lo(dos_item_backup)($4)
$L28:
	beq	$3,$0,$L29
	nop

	j	$L29
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)

$L35:
	sw	$3,%lo(dos_item_backup)($4)
	lui	$3,%hi(dos_item_global)
	j	$L30
	sw	$0,%lo(dos_item_global)($3)

$L34:
	lw	$3,%lo(block_count)($17)
	nop
	addiu	$3,$3,1
	beq	$2,$0,$L23
	sw	$3,%lo(block_count)($17)

	j	$L38
	lui	$2,%hi(dos_timer_ipv4_func)

	.set	macro
	.set	reorder
	.end	dos_timer_fn
	.size	dos_timer_fn, .-dos_timer_fn
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"br0\000"
	.align	2
$LC1:
	.ascii	"dos_filter: alloc skb fail!\012\000"
	.section	.text.GenerateTCPENDACK,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	GenerateTCPENDACK
	.type	GenerateTCPENDACK, @function
GenerateTCPENDACK:
	.frame	$sp,64,$31		# vars= 8, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$23,56($sp)
	sw	$22,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	move	$17,$4
	lui	$4,%hi($LC0)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC0)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	lbu	$3,0($2)
	lui	$4,%hi(endStateClientPort)
	lhu	$4,%lo(endStateClientPort)($4)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$2,$3
	lhu	$5,0($3)
	#nop
	beq	$5,$4,$L67
$L44:
	lw	$31,60($sp)
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L67:
	lw	$4,4($3)
	lui	$3,%hi(endStateClientSeq)
	lw	$3,%lo(endStateClientSeq)($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L44
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L44
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L68
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,96			# 0x60
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L69
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$18,$2
	lbu	$2,0($2)
	move	$4,$19
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
	#nop
	srl	$3,$3,28
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	move	$17,$2
$L65:
	lw	$20,4($17)
	#nop
	andi	$2,$20,0x1
	bne	$2,$0,$L65
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$21,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$21,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($17)
	#nop
	bne	$20,$2,$L65
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$19
	.set	macro
	.set	reorder

 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	beq	$17,$0,$L70
	move	$4,$19
$L72:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lbu	$17,0($18)
	lui	$21,%hi(url_cache)
	lw	$3,12($18)
	lw	$4,16($18)
	addiu	$20,$21,%lo(url_cache)
	lhu	$2,12($20)
	andi	$17,$17,0xf
	sll	$17,$17,2
	sw	$3,16($18)
	sw	$4,12($18)
	addu	$17,$18,$17
	lw	$8,12($17)
	sh	$2,14($17)
	sb	$0,13($17)
	lw	$7,12($17)
	lw	$2,4($17)
	li	$3,-327680			# 0xfffffffffffb0000
	ori	$3,$3,0xffff
	and	$3,$7,$3
	li	$7,65536			# 0x10000
	and	$8,$8,$7
	addiu	$7,$2,1
	lw	$4,8($17)
	lhu	$5,0($17)
	lhu	$6,2($17)
	movn	$2,$7,$8
	li	$9,1048576			# 0x100000
	li	$7,-131072			# 0xfffffffffffe0000
	or	$3,$3,$9
	ori	$7,$7,0xffff
	and	$3,$3,$7
	sw	$4,4($17)
	sw	$3,12($17)
	move	$4,$19
	sh	$6,0($17)
	sh	$5,2($17)
	sw	$2,8($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,18($17)
	.set	macro
	.set	reorder

	lbu	$4,0($18)
	lw	$3,12($22)
	andi	$22,$4,0xf
	subu	$22,$0,$22
	srl	$3,$3,28
	sll	$22,$22,2
	sll	$3,$3,2
	addiu	$22,$22,-14
	sltu	$4,$3,21
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L54
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L58
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L55:
	lbu	$4,1($4)
	beq	$2,$6,$L71
	addu	$16,$16,$4
$L56:
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L73
	move	$5,$22
	.set	macro
	.set	reorder

$L58:
	addu	$4,$17,$16
	lbu	$2,0($4)
	#nop
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L55
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L56
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L68:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L44
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L44
$L71:
	addu	$23,$17,$23
	addiu	$16,$16,6
	addiu	$4,$sp,20
	move	$5,$23
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$16,$17,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$23
	addiu	$5,$sp,16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$16
	addiu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L54:
	move	$5,$22
$L73:
	sh	$0,16($17)
	move	$6,$0
	move	$4,$17
	andi	$22,$22,0xffff
	lw	$23,16($18)
	lw	$16,12($18)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$22,$22,6
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$23, $16		
	sltu	$1, $23, $16	
	addu	$23, $1		
	addu	$23, $22		
	sltu	$1, $23, $22	
	addu	$23, $1		
	addu	$23, $2		
	sltu	$1, $23, $2	
	addu	$23, $1		
	.set	pop
 # 0 "" 2
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $23, 16	
	addu	$23, $1		
	sltu	$1, $23, $1	
	srl	$23, $23, 16	
	addu	$23, $1		
	xori	$23, 0xffff	
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$23,16($17)
	.set	macro
	.set	reorder

	lhu	$4,%lo(url_cache)($21)
	lbu	$3,14($20)
	addiu	$2,$2,-14
	addiu	$4,$4,1
	sh	$2,2($18)
	sh	$4,4($18)
	sh	$0,6($18)
	lw	$5,0($18)
	lw	$6,4($18)
	sb	$3,8($18)
	sh	$0,10($18)
	lw	$3,8($18)
	addu	$2,$6,$5
	lw	$4,12($18)
	sltu	$6,$2,$6
	addu	$2,$2,$3
	addu	$6,$2,$6
	sltu	$3,$6,$3
	addu	$6,$6,$4
	addu	$3,$6,$3
	srl	$5,$5,22
	sltu	$2,$3,$4
	andi	$5,$5,0x3c
	addu	$2,$2,$3
	addu	$5,$18,$5
	addiu	$3,$18,16
$L59:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L59
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$19
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
	sb	$0,16($20)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sh	$2,10($18)
	.set	macro
	.set	reorder

	j	$L44
$L69:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L48
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L48:
	lui	$4,%hi($LC1)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	j	$L44
$L70:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L72
	move	$4,$19
	.set	macro
	.set	reorder

	.end	GenerateTCPENDACK
	.size	GenerateTCPENDACK, .-GenerateTCPENDACK
	.section	.text.GenerateTCPFINACK,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	GenerateTCPFINACK
	.type	GenerateTCPFINACK, @function
GenerateTCPFINACK:
	.frame	$sp,64,$31		# vars= 8, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$23,56($sp)
	sw	$22,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	move	$17,$4
	lui	$4,%hi($LC0)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC0)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	lbu	$3,0($2)
	lui	$4,%hi(finStateClientPort)
	lhu	$4,%lo(finStateClientPort)($4)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$2,$3
	lhu	$5,0($3)
	#nop
	beq	$5,$4,$L97
$L75:
	lw	$31,60($sp)
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L97:
	lw	$4,4($3)
	lui	$3,%hi(finStateClientSeq)
	lw	$3,%lo(finStateClientSeq)($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L75
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L75
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L98
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,96			# 0x60
	move	$6,$0
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L99
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$18,$2
	lbu	$2,0($2)
	move	$4,$19
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
	#nop
	srl	$3,$3,28
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	move	$17,$2
$L95:
	lw	$20,4($17)
	#nop
	andi	$2,$20,0x1
	bne	$2,$0,$L95
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$21,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$21,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($17)
	#nop
	bne	$20,$2,$L95
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$17
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$19
	.set	macro
	.set	reorder

 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	beq	$17,$0,$L100
	move	$4,$19
$L102:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lbu	$17,0($18)
	lui	$21,%hi(url_cache)
	addiu	$20,$21,%lo(url_cache)
	lw	$3,12($18)
	lw	$4,16($18)
	lhu	$2,12($20)
	andi	$17,$17,0xf
	sll	$17,$17,2
	addu	$17,$18,$17
	sw	$4,12($18)
	sw	$3,16($18)
	sh	$2,14($17)
	sb	$0,13($17)
	lw	$7,12($17)
	li	$2,-327680			# 0xfffffffffffb0000
	lw	$3,4($17)
	lw	$4,8($17)
	lhu	$5,0($17)
	lhu	$6,2($17)
	ori	$2,$2,0xffff
	and	$2,$7,$2
	li	$7,1114112			# 0x110000
	or	$2,$2,$7
	sw	$4,4($17)
	sw	$3,8($17)
	move	$4,$19
	sh	$6,0($17)
	sh	$5,2($17)
	sw	$2,12($17)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,18($17)
	.set	macro
	.set	reorder

	lbu	$4,0($18)
	lw	$3,12($22)
	andi	$22,$4,0xf
	subu	$22,$0,$22
	srl	$3,$3,28
	sll	$22,$22,2
	sll	$3,$3,2
	addiu	$22,$22,-14
	sltu	$4,$3,21
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L84
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L88
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L85:
	lbu	$4,1($4)
	beq	$2,$6,$L101
	addu	$16,$16,$4
$L86:
	sltu	$2,$16,$3
	beq	$2,$0,$L84
$L88:
	addu	$4,$17,$16
	lbu	$2,0($4)
	#nop
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L85
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L86
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L98:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L75
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L75
$L101:
	addu	$23,$17,$23
	addiu	$16,$16,6
	addiu	$4,$sp,20
	move	$5,$23
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$16,$17,$16
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$23
	addiu	$5,$sp,16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$16
	addiu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L84:
	lhu	$7,2($17)
	lw	$3,8($17)
	lui	$2,%hi(endStateClientPort)
	move	$5,$22
	sh	$0,16($17)
	move	$6,$0
	sh	$7,%lo(endStateClientPort)($2)
	move	$4,$17
	lui	$2,%hi(endStateClientSeq)
	andi	$22,$22,0xffff
	sw	$3,%lo(endStateClientSeq)($2)
	lw	$23,16($18)
	lw	$16,12($18)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$22,$22,6
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$23, $16		
	sltu	$1, $23, $16	
	addu	$23, $1		
	addu	$23, $22		
	sltu	$1, $23, $22	
	addu	$23, $1		
	addu	$23, $2		
	sltu	$1, $23, $2	
	addu	$23, $1		
	.set	pop
 # 0 "" 2
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $23, 16	
	addu	$23, $1		
	sltu	$1, $23, $1	
	srl	$23, $23, 16	
	addu	$23, $1		
	xori	$23, 0xffff	
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$23,16($17)
	.set	macro
	.set	reorder

	lhu	$4,%lo(url_cache)($21)
	lbu	$3,14($20)
	addiu	$2,$2,-14
	addiu	$4,$4,1
	sh	$2,2($18)
	sh	$4,4($18)
	sh	$0,6($18)
	lw	$5,0($18)
	lw	$6,4($18)
	sb	$3,8($18)
	sh	$0,10($18)
	lw	$3,8($18)
	addu	$2,$6,$5
	lw	$4,12($18)
	sltu	$6,$2,$6
	addu	$2,$2,$3
	addu	$6,$2,$6
	sltu	$3,$6,$3
	addu	$6,$6,$4
	addu	$3,$6,$3
	srl	$5,$5,22
	sltu	$2,$3,$4
	andi	$5,$5,0x3c
	addu	$2,$2,$3
	addu	$5,$18,$5
	addiu	$3,$18,16
$L89:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L89
	addu	$2,$4,$2
	.set	macro
	.set	reorder

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
	sh	$2,10($18)
	li	$2,1			# 0x1
	move	$4,$19
	sb	$0,15($20)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$2,16($20)
	.set	macro
	.set	reorder

	j	$L75
$L99:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L79
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L79:
	lui	$4,%hi($LC1)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	j	$L75
$L100:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L102
	move	$4,$19
	.set	macro
	.set	reorder

	.end	GenerateTCPFINACK
	.size	GenerateTCPFINACK, .-GenerateTCPFINACK
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"HTTP/1.1 401 The web site is blocked by administrator\015"
	.ascii	"\012Server: Embedded HTTP Server 2.00\015\012Connection:"
	.ascii	" close\015\012\015\012<HTML><HEAD><TITLE>401 The web sit"
	.ascii	"e is blocked by administrator</TITLE></HEAD>\012<BODY BG"
	.ascii	"COLOR=\"#ffffff\"><H4>401 The web site is blocked by adm"
	.ascii	"inistrator</H4></BODY></HTML>\012\000"
	.section	.text.GenerateHTTP401,"ax",@progbits
	.align	2
	.globl	GenerateHTTP401
	.set	nomips16
	.ent	GenerateHTTP401
	.type	GenerateHTTP401, @function
GenerateHTTP401:
	.frame	$sp,72,$31		# vars= 16, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$fp,64($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$17,$4
	lui	$4,%hi($LC0)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC0)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L104
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L134
	lui	$18,%hi(block_message)
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	#nop
	beq	$4,$0,$L107
$L132:
	jal	strlen
	move	$6,$2
	move	$4,$17
	li	$5,96			# 0x60
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L135
	move	$21,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$20,$2
	lbu	$2,0($2)
	move	$4,$21
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$20,$22
	lw	$3,12($22)
	#nop
	srl	$3,$3,28
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	#nop
	beq	$4,$0,$L111
$L133:
	jal	strlen
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	move	$19,$2
$L130:
	lw	$23,4($19)
	#nop
	andi	$2,$23,0x1
	bne	$2,$0,$L130
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_get_data_from_hh
	move	$fp,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$fp,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($19)
	#nop
	bne	$23,$2,$L130
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$19
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$4,$21
	.set	macro
	.set	reorder

 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$19,20($16)
	move	$4,$2
	addiu	$19,$19,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$19,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	beq	$19,$0,$L136
	move	$5,$0
$L139:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$21
	.set	macro
	.set	reorder

	lbu	$19,0($20)
	lw	$2,12($20)
	lw	$3,16($20)
	andi	$19,$19,0xf
	sll	$19,$19,2
	addu	$19,$20,$19
	sw	$3,12($20)
	sw	$2,16($20)
	lw	$16,12($19)
	lhu	$2,0($19)
	lhu	$3,2($19)
	lw	$5,%lo(block_message)($18)
	srl	$16,$16,28
	sll	$16,$16,2
	addu	$16,$19,$16
	sh	$3,0($19)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L117
	sh	$2,2($19)
	.set	macro
	.set	reorder

	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,24($sp)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$6,$2,1
	.set	macro
	.set	reorder

$L118:
	lw	$2,8($19)
	lui	$18,%hi(url_cache)
	lw	$16,4($19)
	move	$4,$17
	sw	$2,4($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	addiu	$17,$18,%lo(url_cache)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lbu	$3,0($20)
	lhu	$5,12($17)
	sb	$0,13($19)
	andi	$3,$3,0xf
	sh	$5,14($19)
	srl	$4,$4,28
	lw	$5,12($19)
	addu	$4,$4,$3
	sll	$4,$4,2
	li	$3,-327680			# 0xfffffffffffb0000
	subu	$16,$16,$4
	ori	$3,$3,0xffff
	and	$3,$5,$3
	li	$4,1572864			# 0x180000
	addu	$16,$16,$2
	li	$2,-131072			# 0xfffffffffffe0000
	or	$3,$3,$4
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,12($19)
	move	$4,$21
	sw	$16,8($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$0,18($19)
	.set	macro
	.set	reorder

	lbu	$4,0($20)
	lw	$3,12($22)
	andi	$22,$4,0xf
	subu	$22,$0,$22
	srl	$3,$3,28
	sll	$22,$22,2
	sll	$3,$3,2
	addiu	$22,$22,-14
	sltu	$4,$3,21
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L119
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L123
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L120:
	lbu	$4,1($4)
	beq	$2,$6,$L137
	addu	$16,$16,$4
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L138
	move	$5,$22
	.set	macro
	.set	reorder

$L123:
	addu	$4,$19,$16
$L140:
	lbu	$2,0($4)
	#nop
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L120
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	addiu	$16,$16,1
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L140
	addu	$4,$19,$16
	.set	macro
	.set	reorder

$L119:
	move	$5,$22
$L138:
	sh	$0,16($19)
	move	$6,$0
	move	$4,$19
	andi	$22,$22,0xffff
	lw	$23,16($20)
	lw	$16,12($20)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	addiu	$22,$22,6
	.set	macro
	.set	reorder

	move	$4,$21
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_tcpudp_nofold
	.set	noat		
	addu	$23, $16		
	sltu	$1, $23, $16	
	addu	$23, $1		
	addu	$23, $22		
	sltu	$1, $23, $22	
	addu	$23, $1		
	addu	$23, $2		
	sltu	$1, $23, $2	
	addu	$23, $1		
	.set	pop
 # 0 "" 2
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/checksum.h" 1
		.set	push		# csum_fold
	.set	noat		
	sll	$1, $23, 16	
	addu	$23, $1		
	sltu	$1, $23, $1	
	srl	$23, $23, 16	
	addu	$23, $1		
	xori	$23, 0xffff	
	.set	pop
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sh	$23,16($19)
	.set	macro
	.set	reorder

	lhu	$4,%lo(url_cache)($18)
	lbu	$3,14($17)
	addiu	$2,$2,-14
	addiu	$4,$4,2
	sh	$2,2($20)
	sh	$4,4($20)
	sh	$0,6($20)
	lw	$5,0($20)
	lw	$6,4($20)
	sb	$3,8($20)
	sh	$0,10($20)
	lw	$2,8($20)
	addu	$3,$6,$5
	lw	$4,12($20)
	sltu	$6,$3,$6
	addu	$3,$3,$2
	addu	$6,$3,$6
	sltu	$2,$6,$2
	addu	$6,$6,$4
	addu	$2,$6,$2
	srl	$5,$5,22
	sltu	$3,$2,$4
	andi	$5,$5,0x3c
	addu	$3,$3,$2
	addu	$5,$20,$5
	addiu	$2,$20,16
$L124:
	lw	$4,0($2)
	addiu	$2,$2,4
	addu	$3,$4,$3
	sltu	$4,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L124
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
	sh	$3,10($20)
	lhu	$5,2($19)
	li	$2,1			# 0x1
	lw	$3,8($19)
	sb	$2,15($17)
	lui	$2,%hi(finStateClientPort)
	sh	$5,%lo(finStateClientPort)($2)
	move	$4,$21
	lui	$2,%hi(finStateClientSeq)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sw	$3,%lo(finStateClientSeq)($2)
	.set	macro
	.set	reorder

$L104:
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L135:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L110
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

$L110:
	lui	$4,%hi($LC1)
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	j	$L104
$L137:
	addu	$23,$19,$23
	addiu	$16,$16,6
	addiu	$4,$sp,20
	move	$5,$23
	addu	$16,$19,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	addiu	$4,$sp,16
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$23
	addiu	$5,$sp,16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	move	$4,$16
	addiu	$5,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L138
	move	$5,$22
	.set	macro
	.set	reorder

$L136:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L139
	move	$5,$0
	.set	macro
	.set	reorder

$L134:
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,20($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,20($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	bne	$17,$0,$L104
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L104
$L107:
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	j	$L132
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

$L117:
	lui	$18,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo($LC2)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$18,%lo($LC2)
	.set	macro
	.set	reorder

	j	$L118
$L111:
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	j	$L133
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

	.end	GenerateHTTP401
	.size	GenerateHTTP401, .-GenerateHTTP401
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"type=%d num=%d\012\000"
	.align	2
$LC4:
	.ascii	"index=%d\011\000"
	.align	2
$LC5:
	.ascii	"mac=%02x:%02x:%02x:%02x:%02x:%02x\012\000"
	.align	2
$LC6:
	.ascii	"index=%d\011\011\000"
	.align	2
$LC7:
	.ascii	"url=%s\012\000"
	.align	2
$LC8:
	.ascii	"start=%x\011\000"
	.align	2
$LC9:
	.ascii	"end=%x\011\000"
	.align	2
$LC10:
	.ascii	"day=%x\012\000"
	.align	2
$LC11:
	.ascii	"unknow table type\012\000"
	.align	2
$LC12:
	.ascii	"Mode:%d %s\012\000"
	.section	.text.filter_table_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	filter_table_read_proc
	.type	filter_table_read_proc, @function
filter_table_read_proc:
	.frame	$sp,72,$31		# vars= 0, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$fp,64($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$16,%hi(kmalloc_caches)
	addiu	$16,$16,%lo(kmalloc_caches)
	sw	$4,72($sp)
	lw	$4,36($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L142
	move	$18,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,512			# 0x200

	lw	$4,32($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L159
	move	$16,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,256			# 0x100

	lui	$2,%hi(table_list_head)
	lw	$19,%lo(table_list_head)($2)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L144
	lui	$fp,%hi($LC7)

	lui	$23,%hi($LC8)
	addiu	$fp,$fp,%lo($LC7)
	j	$L153
	addiu	$23,$23,%lo($LC8)

$L161:
	beq	$2,$3,$L148
	li	$3,2			# 0x2

	beq	$2,$3,$L160
	lui	$5,%hi($LC11)

	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC11)

	move	$4,$18
	jal	strcat
	move	$5,$16

$L149:
	lw	$19,0($19)
	lui	$2,%hi(table_list_head)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L162
	lui	$2,%hi(url_filter_mode)

$L153:
	lw	$7,24($19)
	lw	$6,16($19)
	lui	$2,%hi($LC3)
	move	$4,$18
	addiu	$5,$2,%lo($LC3)
	jal	sprintf
	addiu	$7,$7,-1

	lw	$2,16($19)
	li	$3,3			# 0x3
	bne	$2,$3,$L161
	li	$3,4			# 0x4

	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L149
	nop

$L151:
	lw	$6,20($17)
	lui	$5,%hi($LC6)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC6)

	move	$4,$18
	jal	strcat
	move	$5,$16

	addiu	$6,$17,28
	move	$4,$16
	jal	sprintf
	move	$5,$fp

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	nop
	bne	$17,$20,$L151
	lui	$2,%hi(table_list_head)

	lw	$19,0($19)
	addiu	$2,$2,%lo(table_list_head)
	bne	$19,$2,$L153
	nop

$L144:
	lui	$2,%hi(url_filter_mode)
$L162:
	lw	$6,%lo(url_filter_mode)($2)
	lw	$4,72($sp)
	lui	$5,%hi($LC12)
	addiu	$5,$5,%lo($LC12)
	jal	seq_printf
	move	$7,$18

	jal	kfree
	move	$4,$18

	jal	kfree
	move	$4,$16

$L142:
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,72

$L160:
	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L149
	lui	$2,%hi($LC4)

	lui	$21,%hi($LC5)
	addiu	$22,$2,%lo($LC4)
	addiu	$21,$21,%lo($LC5)
$L150:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$22

	move	$4,$18
	jal	strcat
	move	$5,$16

	lbu	$2,30($17)
	lbu	$6,28($17)
	lbu	$7,29($17)
	sw	$2,16($sp)
	lbu	$2,31($17)
	move	$4,$16
	move	$5,$21
	sw	$2,20($sp)
	lbu	$2,32($17)
	nop
	sw	$2,24($sp)
	lbu	$2,33($17)
	jal	sprintf
	sw	$2,28($sp)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	nop
	bne	$17,$20,$L150
	nop

	j	$L149
	nop

$L148:
	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L149
	lui	$2,%hi($LC9)

	lui	$21,%hi($LC10)
	addiu	$22,$2,%lo($LC9)
	addiu	$21,$21,%lo($LC10)
$L152:
	lw	$6,20($17)
	lui	$5,%hi($LC4)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC4)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,28($17)
	move	$4,$16
	jal	sprintf
	move	$5,$23

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,32($17)
	move	$4,$16
	jal	sprintf
	move	$5,$22

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,36($17)
	move	$4,$16
	jal	sprintf
	move	$5,$21

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	nop
	bne	$17,$20,$L152
	nop

	j	$L149
	nop

$L159:
	jal	kfree
	move	$4,$18

	j	$L142
	nop

	.set	macro
	.set	reorder
	.end	filter_table_read_proc
	.size	filter_table_read_proc, .-filter_table_read_proc
	.section	.text.rtl_mac_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_mac_filter_fastpath
	.set	nomips16
	.ent	rtl_mac_filter_fastpath
	.type	rtl_mac_filter_fastpath, @function
rtl_mac_filter_fastpath:
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
	li	$17,6			# 0x6
	move	$16,$4
	move	$19,$5
	li	$20,12			# 0xc
	addu	$2,$19,$17
$L168:
	lbu	$18,-6($2)
	jal	rtl_skb_mac_header
	move	$4,$16

	addu	$2,$2,$17
	lbu	$2,0($2)
	nop
	bne	$18,$2,$L166
	addiu	$17,$17,1

	bne	$17,$20,$L168
	addu	$2,$19,$17

	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,40

$L166:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	rtl_mac_filter_fastpath
	.size	rtl_mac_filter_fastpath, .-rtl_mac_filter_fastpath
	.section	.text.filter_dos,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	filter_dos
	.type	filter_dos, @function
filter_dos:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	jal	rtl_ip_hdr
	move	$16,$4

	move	$4,$16
	jal	filter_wan_check
	move	$17,$2

	beq	$2,$0,$L178
	li	$2,-268435456			# 0xfffffffff0000000

	lw	$3,0($17)
	nop
	and	$3,$3,$2
	li	$2,1610612736			# 0x60000000
	beq	$3,$2,$L181
	lui	$2,%hi(dos_item_global)

	lw	$2,%lo(dos_item_global)($2)
	nop
	beq	$2,$0,$L178
	lui	$2,%hi(filter_dos_ipv4_handler)

	lw	$2,%lo(filter_dos_ipv4_handler)($2)
	nop
	beq	$2,$0,$L178
	nop

$L180:
	jalr	$2
	move	$4,$16

	li	$3,-1			# 0xffffffffffffffff
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	movz	$3,$0,$2
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

$L181:
	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	nop
	beq	$2,$0,$L178
	lui	$2,%hi(filter_dos_ipv6_handler)

	lw	$2,%lo(filter_dos_ipv6_handler)($2)
	nop
	bne	$2,$0,$L180
	nop

$L178:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_dos
	.size	filter_dos, .-filter_dos
	.section	.text.find_pattern.constprop.8,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	find_pattern.constprop.8
	.type	find_pattern.constprop.8, @function
find_pattern.constprop.8:
	.frame	$sp,64,$31		# vars= 8, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$fp,56($sp)
	sw	$23,52($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	lw	$18,80($sp)
	lw	$19,84($sp)
	subu	$3,$5,$7
	move	$16,$4
	sw	$0,0($19)
	move	$17,$5
	move	$21,$7
	sw	$6,72($sp)
	sw	$0,0($18)
	sw	$3,16($sp)
	move	$20,$4
	move	$23,$0
	move	$22,$0
	li	$fp,13			# 0xd
$L188:
	lb	$2,0($20)
	nop
	beq	$2,$fp,$L201
	li	$3,10			# 0xa

	beq	$2,$3,$L202
	andi	$2,$23,0x1

$L194:
	move	$23,$0
$L184:
	lw	$5,72($sp)
	move	$4,$20
	jal	memcmp
	move	$6,$21

	beq	$2,$0,$L187
	nop

	lw	$3,16($sp)
	addiu	$22,$22,1
	sltu	$2,$3,$22
	beq	$2,$0,$L188
	addiu	$20,$20,1

$L198:
	move	$2,$0
$L186:
	lw	$31,60($sp)
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,64

$L187:
	addu	$21,$22,$21
	addu	$2,$16,$21
	sw	$21,0($18)
	lb	$3,0($2)
	li	$2,13			# 0xd
	beq	$3,$2,$L196
	move	$2,$0

	sltu	$2,$17,$21
	bne	$2,$0,$L198
	move	$2,$0

	j	$L190
	li	$5,13			# 0xd

$L191:
	bne	$4,$0,$L198
	nop

$L190:
	addiu	$21,$21,1
	addu	$3,$16,$21
	lb	$3,0($3)
	sltu	$4,$17,$21
	bne	$3,$5,$L191
	addiu	$2,$2,1

	sw	$2,0($19)
	j	$L186
	li	$2,1			# 0x1

$L201:
	andi	$2,$23,0x1
	bne	$2,$0,$L194
	addiu	$23,$23,1

	slt	$2,$23,4
	bne	$2,$0,$L184
	move	$2,$0

	j	$L186
	nop

$L202:
	beq	$2,$0,$L194
	addiu	$23,$23,1

	slt	$2,$23,4
	bne	$2,$0,$L184
	move	$2,$0

	j	$L186
	nop

$L196:
	sw	$2,0($19)
	j	$L186
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	find_pattern.constprop.8
	.size	find_pattern.constprop.8, .-find_pattern.constprop.8
	.section	.rodata.str1.4
	.align	2
$LC13:
	.ascii	"GET \000"
	.align	2
$LC14:
	.ascii	"Host: \000"
	.align	2
$LC15:
	.ascii	"%s\000"
	.section	.text.rtl_url_filter_fastpath,"ax",@progbits
	.align	2
	.globl	rtl_url_filter_fastpath
	.set	nomips16
	.ent	rtl_url_filter_fastpath
	.type	rtl_url_filter_fastpath, @function
rtl_url_filter_fastpath:
	.frame	$sp,64,$31		# vars= 16, regs= 6/0, args= 24, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	move	$18,$4
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$16,$5
	.set	macro
	.set	reorder

	sltu	$2,$2,2
	bne	$2,$0,$L215
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_tcp_header
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L215
	move	$20,$2
	.set	macro
	.set	reorder

	lw	$19,12($2)
	lui	$2,%hi(kmalloc_caches+44)
	lw	$4,%lo(kmalloc_caches+44)($2)
	li	$5,208			# 0xd0
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	srl	$19,$19,28
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L215
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	get_payload_len
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L210
	move	$18,$2
	.set	macro
	.set	reorder

	sll	$19,$19,2
	addu	$19,$20,$19
	lui	$20,%hi($LC13)
	move	$4,$19
	addiu	$5,$20,%lo($LC13)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L210
	addiu	$2,$sp,32
	.set	macro
	.set	reorder

	lui	$6,%hi($LC14)
	sw	$2,16($sp)
	move	$4,$19
	addiu	$2,$sp,28
	move	$5,$18
	addiu	$6,$6,%lo($LC14)
	li	$7,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	find_pattern.constprop.8
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L226
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$4,28($sp)
	lw	$3,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L209
	move	$2,$0
	.set	macro
	.set	reorder

	addu	$3,$19,$3
	move	$2,$17
 #APP
 # 55 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$4,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$4,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lw	$2,28($sp)
	#nop
$L209:
	addu	$2,$17,$2
	sb	$0,0($2)
	addiu	$2,$sp,32
	sw	$2,16($sp)
	move	$4,$19
	addiu	$2,$sp,24
	move	$5,$18
	addiu	$6,$20,%lo($LC13)
	li	$7,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	find_pattern.constprop.8
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L226
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	#nop
	addiu	$2,$2,-9
	.set	noreorder
	.set	nomacro
	blez	$2,$L226
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lw	$3,28($sp)
	lw	$4,32($sp)
	addu	$3,$17,$3
	addu	$19,$19,$4
 #APP
 # 55 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($19)
	subu	$2,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$2,1b
	addiu	$19,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lw	$2,24($sp)
	lw	$3,28($sp)
	move	$4,$17
	addu	$2,$17,$2
	addu	$3,$2,$3
	.set	noreorder
	.set	nomacro
	jal	strlen
	sb	$0,0($3)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$18,$2
	.set	macro
	.set	reorder

	sltu	$18,$18,$2
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L225
	move	$19,$2
	.set	macro
	.set	reorder

$L220:
	move	$16,$0
$L213:
	lui	$6,%hi($LC15)
	lui	$4,%hi(log_info)
	li	$5,32			# 0x20
	addiu	$6,$6,%lo($LC15)
	move	$7,$17
	.set	noreorder
	.set	nomacro
	jal	snprintf
	addiu	$4,$4,%lo(log_info)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
	li	$3,1			# 0x1
	bne	$2,$3,$L204
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L222
	lui	$3,%hi(url_filter_flag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L204
	sw	$2,%lo(url_filter_flag)($3)
	.set	macro
	.set	reorder

$L215:
	move	$16,$0
$L204:
	move	$2,$16
	lw	$31,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L210:
	move	$4,$17
$L226:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$16,$0
	.set	macro
	.set	reorder

	move	$2,$16
	lw	$31,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L225:
	addu	$20,$17,$2
	lb	$2,0($20)
	#nop
	beq	$2,$0,$L220
	move	$4,$0
$L214:
	addu	$4,$17,$4
	move	$5,$16
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$18,$18,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L221
	addu	$3,$20,$18
	.set	macro
	.set	reorder

	lb	$2,0($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L214
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L213
	move	$16,$0
	.set	macro
	.set	reorder

$L221:
	.set	noreorder
	.set	nomacro
	j	$L213
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L222:
	.set	noreorder
	.set	nomacro
	j	$L204
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

	.end	rtl_url_filter_fastpath
	.size	rtl_url_filter_fastpath, .-rtl_url_filter_fastpath
	.section	.rodata.str1.4
	.align	2
$LC16:
	.ascii	"ppp0\000"
	.align	2
$LC17:
	.ascii	"wlan0\000"
	.section	.text.need_filter,"ax",@progbits
	.align	2
	.globl	need_filter
	.set	nomips16
	.ent	need_filter
	.type	need_filter, @function
need_filter:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	lui	$16,%hi($LC0)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo($LC0)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$17,$2
	.set	macro
	.set	reorder

	beq	$2,$0,$L228
	lw	$2,372($2)
	#nop
	beq	$2,$0,$L228
	lw	$4,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L228
	addiu	$16,$16,%lo($LC0)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L257
	j	$L229
$L232:
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L229
$L257:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L232
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L228:
	lui	$16,%hi($LC16)
$L261:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC16)
	.set	macro
	.set	reorder

	beq	$2,$0,$L234
	lw	$2,372($2)
	#nop
	beq	$2,$0,$L254
	lw	$4,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L254
	addiu	$16,$16,%lo($LC16)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L258
	j	$L235
$L238:
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$3,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L235
$L258:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L238
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L254:
	li	$2,1			# 0x1
$L233:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L229:
	lw	$3,16($17)
	lw	$2,28($4)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L261
	lui	$16,%hi($LC16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L233
	move	$2,$0
	.set	macro
	.set	reorder

$L235:
	lw	$3,16($17)
	lw	$2,24($4)
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	xor	$2,$3,$2
	sltu	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L234:
	jal	rtl_get_wan_dev
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L239
	lui	$16,%hi($LC17)
	.set	macro
	.set	reorder

	lw	$2,372($2)
	#nop
	beq	$2,$0,$L254
	lw	$16,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L255
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	j	$L233
$L260:
	lw	$16,8($16)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L233
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L255:
	.set	noreorder
	.set	nomacro
	jal	rtl_strcmp_wan_dev_name
	addiu	$4,$16,43
	.set	macro
	.set	reorder

	bne	$2,$0,$L260
	beq	$16,$0,$L254
	lw	$3,16($17)
	lw	$2,28($16)
	#nop
	xor	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L233
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

$L239:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC17)
	.set	macro
	.set	reorder

	beq	$2,$0,$L254
	lw	$2,372($2)
	#nop
	beq	$2,$0,$L254
	lw	$3,12($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L254
	addiu	$16,$16,%lo($LC17)
	.set	macro
	.set	reorder

	addiu	$4,$3,43
	move	$2,$16
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($4)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$4,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L259
	j	$L242
$L245:
 #APP
 # 80 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$5,($2)
1:	lbu	$1,($4)
	addiu	$2,1
	bne	$1,$5,2f
	addiu	$4,1
	bnez	$5,1b
	lbu	$5,($2)
	nop
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L242
$L259:
	lw	$3,8($3)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L245
	addiu	$4,$3,43
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L233
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L242:
	lw	$4,16($17)
	lw	$2,28($3)
	#nop
	xor	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L233
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

	.end	need_filter
	.size	need_filter, .-need_filter
	.section	.text.str2hexnum,"ax",@progbits
	.align	2
	.globl	str2hexnum
	.set	nomips16
	.ent	str2hexnum
	.type	str2hexnum, @function
str2hexnum:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lbu	$3,0($4)
	nop
	beq	$3,$0,$L267
	nop

	addiu	$4,$4,1
	j	$L266
	move	$2,$0

$L271:
	addiu	$5,$3,-87
$L264:
	addiu	$4,$4,1
	lbu	$3,-1($4)
	sll	$2,$2,4
	beq	$3,$0,$L272
	addu	$2,$5,$2

$L266:
	addiu	$5,$3,-48
	addiu	$7,$3,-97
	sltu	$6,$5,10
	bne	$6,$0,$L264
	sltu	$7,$7,6

	addiu	$5,$3,-65
	bne	$7,$0,$L271
	sltu	$5,$5,6

	beq	$5,$0,$L268
	nop

	addiu	$4,$4,1
	addiu	$5,$3,-55
	lbu	$3,-1($4)
	sll	$2,$2,4
	bne	$3,$0,$L266
	addu	$2,$5,$2

$L272:
	j	$31
	nop

$L268:
	j	$31
	li	$2,-1			# 0xffffffffffffffff

$L267:
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	str2hexnum
	.size	str2hexnum, .-str2hexnum
	.section	.text.filter_table_head_init,"ax",@progbits
	.align	2
	.globl	filter_table_head_init
	.set	nomips16
	.ent	filter_table_head_init
	.type	filter_table_head_init, @function
filter_table_head_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(table_list_head)
	addiu	$4,$16,%lo(table_list_head)
	move	$5,$0
	jal	memset
	li	$6,32			# 0x20

	lui	$3,%hi(table_list_head+8)
	addiu	$2,$16,%lo(table_list_head)
	addiu	$3,$3,%lo(table_list_head+8)
	sw	$3,8($2)
	sw	$3,12($2)
	li	$3,1			# 0x1
	sw	$2,4($2)
	sw	$0,16($2)
	sw	$0,20($2)
	sw	$0,28($2)
	sw	$3,24($2)
	sw	$2,%lo(table_list_head)($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_table_head_init
	.size	filter_table_head_init, .-filter_table_head_init
	.section	.text.filter_table_regist,"ax",@progbits
	.align	2
	.globl	filter_table_regist
	.set	nomips16
	.ent	filter_table_regist
	.type	filter_table_regist, @function
filter_table_regist:
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
	lui	$17,%hi(table_list_head)
	addiu	$16,$17,%lo(table_list_head)
	lw	$2,24($16)
	move	$19,$4
	bne	$2,$0,$L275
	move	$18,$5

	jal	filter_table_head_init
	nop

$L275:
	lui	$2,%hi(kmalloc_caches+20)
	lw	$4,%lo(kmalloc_caches+20)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	sw	$2,0($2)
	lw	$3,%lo(table_list_head)($17)
	addiu	$4,$2,8
	li	$5,1			# 0x1
	sw	$19,16($2)
	sw	$0,20($2)
	sw	$5,24($2)
	sw	$4,8($2)
	sw	$4,12($2)
	sw	$18,28($2)
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$16,4($2)
	lw	$3,24($16)
	sw	$2,%lo(table_list_head)($17)
	lw	$31,36($sp)
	addiu	$3,$3,1
	sw	$3,24($16)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	filter_table_regist
	.size	filter_table_regist, .-filter_table_regist
	.section	.text.filter_table_flush,"ax",@progbits
	.align	2
	.globl	filter_table_flush
	.set	nomips16
	.ent	filter_table_flush
	.type	filter_table_flush, @function
filter_table_flush:
	.frame	$sp,56,$31		# vars= 0, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$23,48($sp)
	sw	$22,44($sp)
	sw	$21,40($sp)
	sw	$20,36($sp)
	sw	$19,32($sp)
	sw	$18,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$20,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($20)
	addiu	$20,$20,%lo(table_list_head)
	lw	$19,0($16)
	beq	$16,$20,$L277
	li	$18,1048576			# 0x100000

	li	$17,2097152			# 0x200000
	addiu	$18,$18,256
	addiu	$17,$17,512
	li	$21,3			# 0x3
$L281:
	lw	$2,8($16)
	addiu	$23,$16,8
	lw	$3,0($2)
	beq	$2,$23,$L278
	nop

$L282:
	lw	$5,4($2)
	move	$4,$2
	move	$22,$3
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$18,0($2)
	jal	kfree
	sw	$17,4($2)

	lw	$3,24($16)
	nop
	addiu	$3,$3,-1
	sw	$3,24($16)
	lw	$3,0($22)
	bne	$22,$23,$L282
	move	$2,$22

$L278:
	lw	$2,16($16)
	nop
	beq	$2,$21,$L284
	li	$2,1			# 0x1

$L280:
	lw	$2,4($16)
	lw	$3,0($16)
	move	$4,$16
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	nop
	addiu	$2,$2,-1
	sw	$2,24($20)
	lw	$2,0($19)
	beq	$19,$20,$L277
	move	$16,$19

	j	$L281
	move	$19,$2

$L284:
	lw	$3,24($16)
	nop
	bne	$3,$2,$L280
	nop

	jal	Del_Pattern_ACL_For_ContentFilter
	nop

	j	$L280
	nop

$L277:
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	sw	$3,%lo(filter_rule_num)($2)
	lw	$31,52($sp)
	lui	$2,%hi(log_enable)
	lw	$23,48($sp)
	lw	$22,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	sw	$0,%lo(log_enable)($2)
	lui	$2,%hi(url_filter_mode)
	sw	$0,%lo(url_filter_mode)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	filter_table_flush
	.size	filter_table_flush, .-filter_table_flush
	.section	.text.filter_item_regist,"ax",@progbits
	.align	2
	.globl	filter_item_regist
	.set	nomips16
	.ent	filter_item_regist
	.type	filter_item_regist, @function
filter_item_regist:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$3,%hi(table_list_head)
	lw	$2,%lo(table_list_head)($3)
	addiu	$3,$3,%lo(table_list_head)
	bne	$2,$3,$L290
	nop

	j	$L286
	li	$2,-1			# 0xffffffffffffffff

$L294:
	lw	$2,0($2)
	nop
	beq	$2,$3,$L291
	nop

$L290:
	lw	$6,16($2)
	nop
	bne	$6,$4,$L294
	addiu	$6,$2,8

	lw	$3,8($2)
	nop
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$6,4($5)
	lw	$3,24($2)
	sw	$5,8($2)
	addiu	$3,$3,1
	sw	$3,24($2)
	li	$2,3			# 0x3
	beq	$4,$2,$L297
	li	$2,2			# 0x2

$L293:
	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L291:
	li	$2,-1			# 0xffffffffffffffff
$L286:
	lw	$31,20($sp)
	nop
	j	$31
	addiu	$sp,$sp,24

$L297:
	bne	$3,$2,$L293
	nop

	jal	Add_Pattern_ACL_For_ContentFilter
	nop

	j	$L286
	move	$2,$0

	.set	macro
	.set	reorder
	.end	filter_item_regist
	.size	filter_item_regist, .-filter_item_regist
	.section	.text.check_esc_end,"ax",@progbits
	.align	2
	.globl	check_esc_end
	.set	nomips16
	.ent	check_esc_end
	.type	check_esc_end, @function
check_esc_end:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	sll	$17,$5,24
	jal	strlen
	move	$16,$4

	addiu	$3,$2,-1
	bltz	$3,$L305
	sra	$17,$17,24

	addu	$3,$16,$3
	lb	$5,0($3)
	nop
	bne	$5,$17,$L305
	addu	$16,$16,$2

	j	$L300
	move	$3,$0

$L302:
	lb	$4,-1($16)
	nop
	bne	$4,$5,$L308
	nop

$L300:
	addiu	$3,$3,1
	bne	$2,$3,$L302
	addiu	$16,$16,-1

	li	$3,-2147483648			# 0xffffffff80000000
	addiu	$3,$3,1
	and	$2,$2,$3
	bltz	$2,$L309
	li	$3,-2			# 0xfffffffffffffffe

$L299:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L308:
	li	$2,-2147483648			# 0xffffffff80000000
	addiu	$2,$2,1
	and	$2,$3,$2
	bgez	$2,$L299
	li	$3,-2			# 0xfffffffffffffffe

$L309:
	addiu	$2,$2,-1
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	or	$2,$2,$3
	addiu	$2,$2,1
	j	$31
	addiu	$sp,$sp,32

$L305:
	j	$L299
	move	$2,$0

	.set	macro
	.set	reorder
	.end	check_esc_end
	.size	check_esc_end, .-check_esc_end
	.section	.rodata.str1.4
	.align	2
$LC18:
	.ascii	"init\000"
	.align	2
$LC19:
	.ascii	" \000"
	.align	2
$LC20:
	.ascii	"flush\000"
	.align	2
$LC21:
	.ascii	"enable_log\000"
	.align	2
$LC22:
	.ascii	"white\000"
	.align	2
$LC23:
	.ascii	"black\000"
	.align	2
$LC24:
	.ascii	"add:\000"
	.align	2
$LC25:
	.ascii	"#\000"
	.align	2
$LC26:
	.ascii	";\000"
	.section	.text.filter_table_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	filter_table_single_write
	.type	filter_table_single_write, @function
filter_table_single_write:
	.frame	$sp,72,$31		# vars= 16, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$fp,64($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	lui	$20,%hi(kmalloc_caches)
	addiu	$20,$20,%lo(kmalloc_caches)
	lw	$4,40($20)
	move	$21,$5
	li	$5,208			# 0xd0
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L337
	move	$17,$2
	.set	macro
	.set	reorder

	move	$5,$0
	li	$6,1024			# 0x400
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memset
	lui	$18,%hi(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$17,16($sp)
	.set	macro
	.set	reorder

	move	$6,$2
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L312
	or	$3,$21,$16
	.set	macro
	.set	reorder

	addu	$2,$21,$16
	or	$2,$3,$2
	lw	$3,24($28)
	#nop
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L338
	move	$19,$16
	.set	macro
	.set	reorder

	addiu	$4,$18,%lo(get_info_from_usr_space)
	move	$5,$21
	move	$6,$16
 #APP
 # 718 "net/rtl/fastpath/filter_v2.c" 1
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L313:
	bne	$6,$0,$L312
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L344
	addiu	$3,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

$L314:
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addu	$2,$17,$2
	sb	$0,-1($2)
	lw	$18,16($sp)
	lui	$16,%hi($LC18)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC18)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L345
	addiu	$16,$sp,16
	.set	macro
	.set	reorder

	lw	$18,16($sp)
	lui	$16,%hi($LC20)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC20)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L318
	lui	$16,%hi($LC21)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

$L317:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

$L311:
	move	$2,$19
	lw	$31,68($sp)
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L345:
	lui	$18,%hi($LC19)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L312
	lui	$20,%hi(filter_tlb)
	.set	macro
	.set	reorder

	addiu	$20,$20,%lo(filter_tlb)
	addiu	$18,$18,%lo($LC19)
	move	$5,$18
$L349:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	sll	$3,$2,3
	addu	$3,$20,$3
	lw	$5,4($3)
	.set	noreorder
	.set	nomacro
	jal	filter_table_regist
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L349
	move	$5,$18
	.set	macro
	.set	reorder

	j	$L317
$L344:
	move	$2,$17
 #APP
 # 55 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($3)
	subu	$16,1
	sb	$1,($2)
	beqz	$1,2f
	addiu	$2,1
	bnez	$16,1b
	addiu	$3,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	j	$L314
$L318:
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC21)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC21)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L319
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	.set	noreorder
	.set	nomacro
	j	$L317
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

$L319:
	lw	$18,16($sp)
	lui	$16,%hi($LC22)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC22)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L320
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	.set	noreorder
	.set	nomacro
	j	$L317
	sw	$3,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L312:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L311
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

$L320:
	lw	$18,16($sp)
	lui	$16,%hi($LC23)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC23)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC23)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L321
	lui	$2,%hi(url_filter_mode)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L317
	sw	$0,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L337:
	.set	noreorder
	.set	nomacro
	j	$L311
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

$L321:
	lw	$18,16($sp)
	lui	$16,%hi($LC24)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC24)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC24)
	.set	macro
	.set	reorder

	bne	$2,$0,$L317
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC24)
	.set	macro
	.set	reorder

	addu	$2,$18,$2
	sw	$2,16($sp)
	lb	$2,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L312
	addiu	$16,$sp,16
	.set	macro
	.set	reorder

	lui	$5,%hi($LC25)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC25)
	.set	macro
	.set	reorder

	lw	$3,16($sp)
	#nop
	beq	$3,$0,$L312
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L312
	lui	$18,%hi($LC19)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lw	$3,16($sp)
	#nop
	beq	$3,$0,$L312
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$21,-1			# 0xffffffffffffffff
	beq	$2,$21,$L312
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lw	$3,16($sp)
	#nop
	beq	$3,$0,$L312
	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L312
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lui	$23,%hi($LC26)
	addiu	$23,$23,%lo($LC26)
	li	$fp,59			# 0x3b
	addiu	$18,$18,%lo($LC19)
	li	$22,32			# 0x20
	move	$4,$16
$L351:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	$L322
	sw	$2,20($sp)
	.set	macro
	.set	reorder

$L346:
	lw	$4,20($sp)
$L322:
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L350
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	lw	$21,20($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	addu	$2,$21,$2
	sb	$fp,0($2)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$23
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L346
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

$L350:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$18
	.set	macro
	.set	reorder

	move	$21,$2
	lw	$2,20($sp)
	#nop
	bne	$2,$0,$L343
	j	$L312
$L326:
	jal	strlen
	addu	$2,$21,$2
	sb	$22,0($2)
	addiu	$4,$sp,20
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$18
	.set	macro
	.set	reorder

$L343:
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L326
	move	$4,$21
	.set	macro
	.set	reorder

	lb	$2,0($21)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L327
	move	$5,$21
	.set	macro
	.set	reorder

	move	$6,$0
	li	$7,92			# 0x5c
$L331:
	beq	$2,$7,$L347
$L328:
	addiu	$5,$5,1
	lb	$2,0($5)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L331
	addiu	$6,$6,1
	.set	macro
	.set	reorder

$L327:
	lw	$4,4($20)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L317
	move	$5,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($2)
	lw	$4,24($sp)
	addiu	$2,$5,8
	sw	$5,0($5)
	sw	$4,16($5)
	sw	$5,4($5)
	sw	$2,8($5)
	sw	$2,12($5)
	sw	$0,24($5)
	sw	$3,20($5)
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,28($sp)
	.set	macro
	.set	reorder

	sltu	$2,$2,41
	lw	$5,28($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L348
	addiu	$4,$5,28
	.set	macro
	.set	reorder

	li	$2,39			# 0x27
	li	$3,39			# 0x27
$L336:
 #APP
 # 55 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($21)
	subu	$2,1
	sb	$1,($4)
	beqz	$1,2f
	addiu	$4,1
	bnez	$2,1b
	addiu	$21,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
$L334:
	addu	$3,$5,$3
	sb	$0,28($3)
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,3			# 0x3
	.set	macro
	.set	reorder

	lui	$2,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($2)
	lw	$2,16($sp)
	lui	$4,%hi(filter_rule_num)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L317
	sw	$3,%lo(filter_rule_num)($4)
	.set	macro
	.set	reorder

	lb	$2,0($2)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L351
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L317
$L347:
	lb	$3,1($5)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L329
	move	$4,$6
	.set	macro
	.set	reorder

	move	$2,$6
$L330:
	addu	$4,$21,$4
	addiu	$2,$2,1
	sb	$3,0($4)
	addu	$3,$21,$2
	lb	$3,1($3)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L330
	move	$4,$2
	.set	macro
	.set	reorder

$L329:
	addu	$4,$21,$4
	.set	noreorder
	.set	nomacro
	j	$L328
	sb	$0,0($4)
	.set	macro
	.set	reorder

$L348:
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$5,28($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L334
	move	$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L336
	addiu	$4,$5,28
	.set	macro
	.set	reorder

$L338:
	.set	noreorder
	.set	nomacro
	j	$L313
	move	$6,$16
	.set	macro
	.set	reorder

	.end	filter_table_single_write
	.size	filter_table_single_write, .-filter_table_single_write
	.section	.text.scan_table,"ax",@progbits
	.align	2
	.globl	scan_table
	.set	nomips16
	.ent	scan_table
	.type	scan_table, @function
scan_table:
	.frame	$sp,48,$31		# vars= 0, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$19,%hi(url_filter_flag)
	move	$17,$6
	move	$22,$4
	move	$21,$5
	move	$20,$7
	beq	$6,$0,$L370
	sw	$0,%lo(url_filter_flag)($19)

	lw	$2,28($6)
	nop
	beq	$2,$0,$L353
	li	$2,-1			# 0xffffffffffffffff

	lw	$2,24($6)
	nop
	sltu	$2,$2,2
	bne	$2,$0,$L354
	addiu	$18,$6,8

	lw	$16,8($6)
	nop
	beq	$18,$16,$L383
	lui	$2,%hi(url_filter_mode)

	lw	$2,16($16)
	nop
	andi	$3,$2,0x1
	bne	$3,$0,$L384
	andi	$3,$2,0x200

$L379:
	lw	$3,20($16)
	nop
	beq	$21,$3,$L384
	andi	$3,$2,0x200

$L356:
	lw	$2,0($16)
	nop
	beq	$18,$2,$L378
	nop

	move	$16,$2
	lw	$2,16($16)
	nop
	andi	$3,$2,0x1
	beq	$3,$0,$L379
	andi	$3,$2,0x200

$L384:
	beq	$3,$0,$L357
	nop

	andi	$2,$2,0x2
	lw	$6,0($20)
	beq	$2,$0,$L356
	nop

	beq	$6,$0,$L370
	move	$4,$22

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L356
	nop

$L373:
	li	$2,1			# 0x1
$L353:
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,48

$L357:
	andi	$3,$2,0x100
	beq	$3,$0,$L380
	andi	$3,$2,0x2

$L382:
	beq	$3,$0,$L381
	andi	$3,$2,0x1

	lw	$6,0($20)
	nop
	beq	$6,$0,$L370
	move	$4,$22

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L356
	nop

	lw	$2,16($16)
	nop
	andi	$3,$2,0x1
	beq	$3,$0,$L360
	lui	$3,%hi(url_filter_mode)

	lw	$3,%lo(url_filter_mode)($3)
	nop
	beq	$3,$0,$L377
	andi	$2,$2,0x8

	j	$L353
	sltu	$2,$0,$2

$L380:
	lw	$2,28($17)
	move	$4,$22
	jalr	$2
	addiu	$5,$16,28

	beq	$2,$0,$L356
	nop

	lw	$2,16($16)
	j	$L382
	andi	$3,$2,0x2

$L360:
	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
$L377:
	j	$L353
	sltu	$2,$2,1

$L354:
	lui	$2,%hi(url_filter_mode)
$L383:
	lw	$4,%lo(url_filter_mode)($2)
	li	$3,-1			# 0xffffffffffffffff
	li	$2,1			# 0x1
	j	$L353
	movz	$2,$3,$4

$L378:
$L363 = .
	beq	$16,$0,$L383
	lui	$2,%hi(url_filter_mode)

	lw	$2,%lo(url_filter_flag)($19)
	nop
	bne	$2,$0,$L353
	li	$2,1			# 0x1

	lw	$2,16($16)
	nop
	andi	$3,$2,0x1
	beq	$3,$0,$L374
	nop

	andi	$2,$2,0x8
	j	$L353
	sltu	$2,$0,$2

$L374:
	j	$L353
	move	$2,$0

$L370:
	j	$L353
	li	$2,-1			# 0xffffffffffffffff

$L381:
	beq	$3,$0,$L373
	andi	$2,$2,0x8

	j	$L353
	sltu	$2,$2,1

	.set	macro
	.set	reorder
	.end	scan_table
	.size	scan_table, .-scan_table
	.section	.rodata.str1.4
	.align	2
$LC27:
	.ascii	"[ip_range_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC28:
	.ascii	"[mac_filter]%d.%d.%d.%d(%02x:%02x:%02x:%02x:%02x:%02x) ="
	.ascii	"==> %d.%d.%d.%d [Blocked]\012\000"
	.align	2
$LC29:
	.ascii	"[url_key_filter]%d.%d.%d.%d ==> %d.%d.%d.%d(%s)  [Blocke"
	.ascii	"d]\012\000"
	.align	2
$LC30:
	.ascii	"[schedule_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC31:
	.ascii	"Unkown Type [Blocked]\012\000"
	.section	.text.log_fastFilter,"ax",@progbits
	.align	2
	.globl	log_fastFilter
	.set	nomips16
	.ent	log_fastFilter
	.type	log_fastFilter, @function
log_fastFilter:
	.frame	$sp,184,$31		# vars= 8, regs= 9/0, args= 136, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-184
	sw	$31,180($sp)
	sw	$23,176($sp)
	sw	$22,172($sp)
	sw	$21,168($sp)
	sw	$20,164($sp)
	sw	$19,160($sp)
	sw	$18,156($sp)
	sw	$17,152($sp)
	sw	$16,148($sp)
	move	$18,$5
	move	$16,$4
	jal	do_gettimeofday
	addiu	$4,$sp,136

	jal	rtl_ip_hdr
	move	$4,$18

	move	$4,$18
	jal	rtl_eth_hdr
	move	$19,$2

	li	$3,2			# 0x2
	beq	$16,$3,$L388
	nop

	slt	$2,$16,3
	beq	$2,$0,$L391
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	beq	$16,$2,$L394
	lui	$4,%hi($LC31)

$L395:
	jal	printk
	addiu	$4,$4,%lo($LC31)

$L392:
	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,184

$L391:
	beq	$16,$2,$L389
	li	$2,4			# 0x4

	bne	$16,$2,$L395
	lui	$4,%hi($LC31)

	beq	$19,$0,$L392
	nop

	lw	$2,16($19)
	lw	$5,12($19)
	lui	$4,%hi($LC30)
	srl	$8,$2,16
	srl	$3,$2,8
	srl	$9,$2,24
	srl	$6,$5,16
	srl	$7,$5,8
	andi	$10,$5,0xff
	andi	$8,$8,0xff
	andi	$3,$3,0xff
	andi	$2,$2,0xff
	addiu	$4,$4,%lo($LC30)
$L393:
	srl	$5,$5,24
	andi	$6,$6,0x00ff
	andi	$7,$7,0x00ff
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	j	$L392
	nop

$L389:
	beq	$19,$0,$L392
	nop

	lw	$2,16($19)
	lw	$5,12($19)
	lui	$4,%hi($LC29)
	srl	$9,$2,24
	srl	$8,$2,16
	srl	$3,$2,8
	andi	$2,$2,0xff
	srl	$6,$5,16
	srl	$7,$5,8
	sw	$2,32($sp)
	lui	$2,%hi(log_info)
	andi	$10,$5,0xff
	andi	$8,$8,0xff
	andi	$3,$3,0xff
	addiu	$4,$4,%lo($LC29)
	srl	$5,$5,24
	andi	$6,$6,0x00ff
	andi	$7,$7,0x00ff
	addiu	$2,$2,%lo(log_info)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,36($sp)

	j	$L392
	nop

$L388:
	beq	$2,$0,$L392
	nop

	beq	$19,$0,$L392
	nop

	lw	$23,12($19)
	jal	rtl_eth_hdr
	move	$4,$18

	move	$4,$18
	jal	rtl_eth_hdr
	move	$22,$2

	move	$4,$18
	jal	rtl_eth_hdr
	move	$21,$2

	move	$4,$18
	jal	rtl_eth_hdr
	move	$20,$2

	move	$4,$18
	jal	rtl_eth_hdr
	move	$17,$2

	move	$4,$18
	jal	rtl_eth_hdr
	move	$16,$2

	andi	$4,$23,0xff
	move	$3,$2
	lw	$2,16($19)
	sw	$4,16($sp)
	lbu	$15,84($22)
	lbu	$12,85($22)
	lbu	$14,88($22)
	lbu	$11,89($22)
	lbu	$13,92($22)
	lbu	$10,93($22)
	lbu	$6,86($22)
	lbu	$5,90($22)
	lbu	$4,94($22)
	lbu	$9,87($22)
	lbu	$8,91($22)
	lbu	$7,95($22)
	sll	$15,$15,24
	sll	$14,$14,24
	sll	$13,$13,24
	sll	$12,$12,16
	sll	$11,$11,16
	sll	$10,$10,16
	or	$12,$12,$15
	or	$11,$11,$14
	or	$10,$10,$13
	sll	$6,$6,8
	sll	$5,$5,8
	sll	$4,$4,8
	or	$6,$6,$12
	or	$5,$5,$11
	or	$4,$4,$10
	or	$6,$9,$6
	or	$5,$8,$5
	or	$4,$7,$4
	sw	$6,20($sp)
	sw	$5,24($sp)
	sw	$4,28($sp)
	lbu	$4,96($22)
	srl	$6,$23,16
	srl	$7,$23,8
	sb	$4,32($sp)
	lbu	$4,97($22)
	srl	$9,$2,16
	srl	$8,$2,8
	sb	$4,33($sp)
	lbu	$24,98($21)
	lbu	$18,99($21)
	lbu	$22,102($21)
	lbu	$15,103($21)
	lbu	$19,106($21)
	lbu	$14,107($21)
	lbu	$10,100($21)
	lbu	$5,104($21)
	lbu	$4,108($21)
	lbu	$13,101($21)
	lbu	$12,105($21)
	lbu	$11,109($21)
	sll	$24,$24,24
	sll	$22,$22,24
	sll	$19,$19,24
	sll	$18,$18,16
	sll	$15,$15,16
	sll	$14,$14,16
	or	$18,$18,$24
	or	$15,$15,$22
	or	$14,$14,$19
	sll	$10,$10,8
	sll	$5,$5,8
	sll	$4,$4,8
	or	$10,$10,$18
	or	$5,$5,$15
	or	$4,$4,$14
	or	$10,$13,$10
	or	$5,$12,$5
	or	$4,$11,$4
	sw	$10,36($sp)
	sw	$5,40($sp)
	sw	$4,44($sp)
	lbu	$4,110($21)
	srl	$10,$2,24
	andi	$9,$9,0xff
	sb	$4,48($sp)
	lbu	$4,111($21)
	andi	$8,$8,0xff
	andi	$2,$2,0xff
	sb	$4,49($sp)
	lbu	$24,112($20)
	lbu	$19,113($20)
	lbu	$22,116($20)
	lbu	$18,117($20)
	lbu	$21,120($20)
	lbu	$15,121($20)
	lbu	$11,114($20)
	lbu	$5,118($20)
	lbu	$4,122($20)
	lbu	$14,115($20)
	lbu	$13,119($20)
	lbu	$12,123($20)
	sll	$24,$24,24
	sll	$22,$22,24
	sll	$21,$21,24
	sll	$19,$19,16
	sll	$18,$18,16
	sll	$15,$15,16
	or	$19,$19,$24
	or	$18,$18,$22
	or	$15,$15,$21
	sll	$11,$11,8
	sll	$5,$5,8
	sll	$4,$4,8
	or	$11,$11,$19
	or	$5,$5,$18
	or	$4,$4,$15
	or	$11,$14,$11
	or	$5,$13,$5
	or	$4,$12,$4
	sw	$4,60($sp)
	sw	$11,52($sp)
	sw	$5,56($sp)
	lbu	$5,124($20)
	lui	$4,%hi($LC28)
	addiu	$4,$4,%lo($LC28)
	sb	$5,64($sp)
	lbu	$11,125($20)
	srl	$5,$23,24
	andi	$6,$6,0x00ff
	sb	$11,65($sp)
	lbu	$24,126($17)
	lbu	$21,127($17)
	lbu	$23,130($17)
	lbu	$20,131($17)
	lbu	$22,134($17)
	lbu	$19,135($17)
	lbu	$13,128($17)
	lbu	$12,132($17)
	lbu	$11,136($17)
	lbu	$18,129($17)
	lbu	$15,133($17)
	lbu	$14,137($17)
	sll	$24,$24,24
	sll	$23,$23,24
	sll	$22,$22,24
	sll	$21,$21,16
	sll	$20,$20,16
	sll	$19,$19,16
	or	$21,$21,$24
	or	$20,$20,$23
	or	$19,$19,$22
	sll	$13,$13,8
	sll	$12,$12,8
	sll	$11,$11,8
	or	$13,$13,$21
	or	$12,$12,$20
	or	$11,$11,$19
	or	$13,$18,$13
	or	$12,$15,$12
	or	$11,$14,$11
	sw	$13,68($sp)
	sw	$12,72($sp)
	sw	$11,76($sp)
	lbu	$11,138($17)
	andi	$7,$7,0x00ff
	sb	$11,80($sp)
	lbu	$11,139($17)
	nop
	sb	$11,81($sp)
	lbu	$23,140($16)
	lbu	$20,141($16)
	lbu	$22,144($16)
	lbu	$19,145($16)
	lbu	$21,148($16)
	lbu	$18,149($16)
	lbu	$13,142($16)
	lbu	$12,146($16)
	lbu	$11,150($16)
	lbu	$17,143($16)
	lbu	$15,147($16)
	lbu	$14,151($16)
	sll	$23,$23,24
	sll	$22,$22,24
	sll	$21,$21,24
	sll	$20,$20,16
	sll	$19,$19,16
	sll	$18,$18,16
	or	$20,$20,$23
	or	$19,$19,$22
	or	$18,$18,$21
	sll	$13,$13,8
	sll	$12,$12,8
	sll	$11,$11,8
	or	$13,$13,$20
	or	$12,$12,$19
	or	$11,$11,$18
	or	$13,$17,$13
	or	$12,$15,$12
	or	$11,$14,$11
	sw	$13,84($sp)
	sw	$12,88($sp)
	sw	$11,92($sp)
	lbu	$11,152($16)
	nop
	sb	$11,96($sp)
	lbu	$11,153($16)
	nop
	sb	$11,97($sp)
	lbu	$22,154($3)
	lbu	$19,155($3)
	lbu	$21,158($3)
	lbu	$18,159($3)
	lbu	$20,162($3)
	lbu	$17,163($3)
	lbu	$13,156($3)
	lbu	$12,160($3)
	lbu	$11,164($3)
	lbu	$16,157($3)
	lbu	$15,161($3)
	lbu	$14,165($3)
	sll	$22,$22,24
	sll	$21,$21,24
	sll	$20,$20,24
	sll	$19,$19,16
	sll	$18,$18,16
	sll	$17,$17,16
	or	$19,$19,$22
	or	$18,$18,$21
	or	$17,$17,$20
	sll	$13,$13,8
	sll	$12,$12,8
	sll	$11,$11,8
	or	$13,$13,$19
	or	$12,$12,$18
	or	$11,$11,$17
	or	$11,$14,$11
	or	$13,$16,$13
	or	$12,$15,$12
	sw	$13,100($sp)
	sw	$12,104($sp)
	sw	$11,108($sp)
	lbu	$11,166($3)
	nop
	sb	$11,112($sp)
	lbu	$3,167($3)
	sw	$10,116($sp)
	sw	$9,120($sp)
	sb	$3,113($sp)
	sw	$8,124($sp)
	jal	scrlog_printk
	sw	$2,128($sp)

	j	$L392
	nop

$L394:
	beq	$19,$0,$L392
	nop

	lw	$2,16($19)
	lw	$5,12($19)
	lui	$4,%hi($LC27)
	srl	$8,$2,16
	srl	$3,$2,8
	srl	$9,$2,24
	srl	$6,$5,16
	srl	$7,$5,8
	andi	$10,$5,0xff
	andi	$8,$8,0xff
	andi	$3,$3,0xff
	andi	$2,$2,0xff
	j	$L393
	addiu	$4,$4,%lo($LC27)

	.set	macro
	.set	reorder
	.end	log_fastFilter
	.size	log_fastFilter, .-log_fastFilter
	.section	.text.do_filter,"ax",@progbits
	.align	2
	.globl	do_filter
	.set	nomips16
	.ent	do_filter
	.type	do_filter, @function
do_filter:
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
	lui	$17,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($17)
	addiu	$17,$17,%lo(table_list_head)
	beq	$16,$17,$L401
	move	$18,$4

	li	$19,-1			# 0xffffffffffffffff
	j	$L403
	li	$20,1			# 0x1

$L406:
	beq	$2,$20,$L405
	lui	$2,%hi(log_enable)

	lw	$16,0($16)
	nop
	beq	$16,$17,$L397
	move	$2,$0

$L403:
	move	$6,$16
	move	$7,$16
	move	$4,$18
	jal	scan_table
	move	$5,$0

	bne	$2,$19,$L406
	nop

$L401:
	move	$2,$0
$L397:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L405:
	lw	$2,%lo(log_enable)($2)
	nop
	bne	$2,$0,$L407
	nop

	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,40

$L407:
	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$18

	j	$L397
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	do_filter
	.size	do_filter, .-do_filter
	.section	.text.FilterWeb_v2,"ax",@progbits
	.align	2
	.globl	FilterWeb_v2
	.set	nomips16
	.ent	FilterWeb_v2
	.type	FilterWeb_v2, @function
FilterWeb_v2:
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

	jal	need_filter
	move	$4,$16

	bne	$2,$0,$L410
	nop

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L410:
	move	$4,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	do_filter
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	FilterWeb_v2
	.size	FilterWeb_v2, .-FilterWeb_v2
	.section	.rodata.str1.4
	.align	2
$LC32:
	.ascii	"filter_table\000"
	.section	.text.filter_table_init,"ax",@progbits
	.align	2
	.globl	filter_table_init
	.set	nomips16
	.ent	filter_table_init
	.type	filter_table_init, @function
filter_table_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	jal	filter_table_head_init
	nop

	lui	$4,%hi($LC32)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(filter_table_proc_fops)
	addiu	$4,$4,%lo($LC32)
	move	$5,$0
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(filter_table_proc_fops)
	jal	proc_create_data
	sw	$0,16($sp)

	lw	$31,28($sp)
	lui	$3,%hi(res12)
	sw	$2,%lo(res12)($3)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_table_init
	.size	filter_table_init, .-filter_table_init
	.section	.text.filter_init_fastpath,"ax",@progbits
	.align	2
	.globl	filter_init_fastpath
	.set	nomips16
	.ent	filter_init_fastpath
	.type	filter_init_fastpath, @function
filter_init_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	jal	filter_table_init
	sw	$3,%lo(filter_rule_num)($2)

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_init_fastpath
	.size	filter_init_fastpath, .-filter_init_fastpath
	.section	.text.filter_exit_fastpath,"ax",@progbits
	.align	2
	.globl	filter_exit_fastpath
	.set	nomips16
	.ent	filter_exit_fastpath
	.type	filter_exit_fastpath, @function
filter_exit_fastpath:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res12)
	lw	$2,%lo(res12)($16)
	nop
	beq	$2,$0,$L414
	lui	$4,%hi($LC32)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC32)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res12)($16)
$L414:
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit_fastpath
	.size	filter_exit_fastpath, .-filter_exit_fastpath
	.section	.text.strtoint,"ax",@progbits
	.align	2
	.globl	strtoint
	.set	nomips16
	.ent	strtoint
	.type	strtoint, @function
strtoint:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lb	$3,0($4)
	nop
	beq	$3,$0,$L419
	nop

	addiu	$2,$3,-48
	sltu	$2,$2,10
	beq	$2,$0,$L420
	addiu	$4,$4,1

	j	$L417
	move	$2,$0

$L418:
	beq	$7,$0,$L422
	addiu	$4,$4,1

$L417:
	lb	$5,0($4)
	sll	$8,$2,1
	sll	$2,$2,3
	addiu	$6,$3,-48
	addiu	$7,$5,-48
	addu	$2,$8,$2
	move	$3,$5
	sltu	$7,$7,10
	bne	$5,$0,$L418
	addu	$2,$2,$6

$L422:
	j	$31
	nop

$L420:
	move	$2,$0
	j	$31
	nop

$L419:
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	strtoint
	.size	strtoint, .-strtoint
	.section	.text.strtok,"ax",@progbits
	.align	2
	.globl	strtok
	.set	nomips16
	.ent	strtok
	.type	strtok, @function
strtok:
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
	beq	$4,$0,$L429
	move	$17,$5

	move	$16,$4
	lui	$18,%hi(___strtok)
	move	$4,$16
$L433:
	jal	strspn
	move	$5,$17

	addu	$16,$16,$2
	lb	$2,0($16)
	nop
	beq	$2,$0,$L430
	move	$4,$16

	jal	strpbrk
	move	$5,$17

	beq	$2,$0,$L427
	nop

	lb	$3,0($2)
	nop
	bne	$3,$0,$L431
	nop

$L427:
	sw	$2,%lo(___strtok)($18)
	move	$2,$16
$L432:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L431:
	sb	$0,0($2)
	j	$L427
	addiu	$2,$2,1

$L430:
	move	$16,$0
	move	$2,$16
	sw	$0,%lo(___strtok)($18)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L429:
	lui	$18,%hi(___strtok)
	lw	$16,%lo(___strtok)($18)
	nop
	bne	$16,$0,$L433
	move	$4,$16

	j	$L432
	move	$2,$16

	.set	macro
	.set	reorder
	.end	strtok
	.size	strtok, .-strtok
	.section	.text.dos_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_single_write
	.type	dos_single_write, @function
dos_single_write:
	.frame	$sp,136,$31		# vars= 80, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$31,132($sp)
	sw	$fp,128($sp)
	sw	$23,124($sp)
	sw	$22,120($sp)
	sw	$21,116($sp)
	sw	$20,112($sp)
	sw	$19,108($sp)
	sw	$18,104($sp)
	sw	$17,100($sp)
	sw	$16,96($sp)
	sltu	$3,$6,2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L461
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L461
	or	$3,$5,$6
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	or	$2,$3,$2
	lw	$3,24($28)
	#nop
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L461
	lui	$2,%hi(dos_flag)
	.set	macro
	.set	reorder

	addiu	$4,$2,%lo(dos_flag)
 #APP
 # 2468 "net/rtl/fastpath/filter.c" 1
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
	bne	$6,$0,$L461
	addiu	$2,$2,%lo(dos_flag)
	.set	macro
	.set	reorder

	addiu	$3,$sp,16
	li	$4,80			# 0x50
 #APP
 # 55 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	subu	$4,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$4,1b
	addiu	$2,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lui	$19,%hi($LC19)
	addiu	$4,$sp,16
	addiu	$5,$19,%lo($LC19)
	.set	noreorder
	.set	nomacro
	jal	strtok
	lui	$20,%hi($L456)
	.set	macro
	.set	reorder

	li	$17,1			# 0x1
	addiu	$20,$20,%lo($L456)
	lui	$fp,%hi(Ipv6_smurf_threshold)
	lui	$23,%hi(dos_item_ipv6)
	lui	$22,%hi(dos_item_global_0)
	lui	$21,%hi(block_time)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L435
	move	$18,$2
	.set	macro
	.set	reorder

$L457:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	move	$6,$0
	.set	macro
	.set	reorder

	sltu	$3,$17,19
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L437
	sll	$3,$17,2
	.set	macro
	.set	reorder

	addu	$3,$20,$3
	lw	$3,0($3)
	#nop
	j	$3
	.section	.rodata.dos_single_write,"a",@progbits
	.align	2
	.align	2
$L456:
	.word	$L437
	.word	$L438
	.word	$L439
	.word	$L440
	.word	$L441
	.word	$L442
	.word	$L443
	.word	$L444
	.word	$L445
	.word	$L446
	.word	$L447
	.word	$L448
	.word	$L449
	.word	$L450
	.word	$L451
	.word	$L452
	.word	$L453
	.word	$L454
	.word	$L455
	.section	.text.dos_single_write
$L438:
	lui	$3,%hi(dos_op_mode)
	sb	$2,%lo(dos_op_mode)($3)
$L437:
	addiu	$17,$17,1
	andi	$17,$17,0x00ff
	move	$4,$0
$L462:
	.set	noreorder
	.set	nomacro
	jal	strtok
	addiu	$5,$19,%lo($LC19)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L457
	move	$18,$2
	.set	macro
	.set	reorder

$L435:
	move	$2,$16
	lw	$31,132($sp)
	lw	$fp,128($sp)
	lw	$23,124($sp)
	lw	$22,120($sp)
	lw	$21,116($sp)
	lw	$20,112($sp)
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

$L455:
	lui	$3,%hi(Ipv6_ns_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(Ipv6_ns_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L454:
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(Ipv6_icmp_too_big_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L453:
	addiu	$17,$17,1
	sw	$2,%lo(Ipv6_smurf_threshold)($fp)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L452:
	addiu	$17,$17,1
	sw	$2,%lo(dos_item_ipv6)($23)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L451:
	addiu	$17,$17,1
	sw	$2,%lo(dos_item_global_0)($22)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L450:
	addiu	$17,$17,1
	sw	$2,%lo(block_time)($21)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L449:
	lui	$3,%hi(per_icmp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_icmp_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L448:
	lui	$3,%hi(per_udp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_udp_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L447:
	lui	$3,%hi(per_fin_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_fin_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L446:
	lui	$3,%hi(per_syn_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_syn_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L445:
	lui	$3,%hi(whole_icmp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_icmp_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L444:
	lui	$3,%hi(whole_udp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_udp_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L443:
	lui	$3,%hi(whole_fin_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_fin_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L442:
	lui	$3,%hi(whole_syn_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_syn_threshold)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L441:
	lui	$3,%hi(dos_item_global)
	sw	$2,%lo(dos_item_global)($3)
	addiu	$17,$17,1
	lui	$2,%hi(dos_item_backup)
	sw	$0,%lo(dos_item_backup)($2)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L440:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	addiu	$17,$17,1
	lui	$3,%hi(dos_lan_mask)
	sw	$2,%lo(dos_lan_mask)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L439:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	addiu	$17,$17,1
	lui	$3,%hi(dos_lan_addr)
	sw	$2,%lo(dos_lan_addr)($3)
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L462
	move	$4,$0
	.set	macro
	.set	reorder

$L461:
	.set	noreorder
	.set	nomacro
	j	$L435
	li	$16,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	dos_single_write
	.size	dos_single_write, .-dos_single_write
	.section	.text.filter_addconnect,"ax",@progbits
	.align	2
	.globl	filter_addconnect
	.set	nomips16
	.ent	filter_addconnect
	.type	filter_addconnect, @function
filter_addconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$2,$3,$2
	j	$31
	sw	$4,0($2)

	.set	macro
	.set	reorder
	.end	filter_addconnect
	.size	filter_addconnect, .-filter_addconnect
	.section	.text.filter_delconnect,"ax",@progbits
	.align	2
	.globl	filter_delconnect
	.set	nomips16
	.ent	filter_delconnect
	.type	filter_delconnect, @function
filter_delconnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	andi	$4,$4,0x00ff
	lui	$2,%hi(ConnectedIp)
	sll	$4,$4,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$4,$4,$2
	j	$31
	sw	$0,0($4)

	.set	macro
	.set	reorder
	.end	filter_delconnect
	.size	filter_delconnect, .-filter_delconnect
	.section	.text.filter_checkConnect,"ax",@progbits
	.align	2
	.globl	filter_checkConnect
	.set	nomips16
	.ent	filter_checkConnect
	.type	filter_checkConnect, @function
filter_checkConnect:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	andi	$3,$4,0xff
	lui	$2,%hi(ConnectedIp)
	sll	$3,$3,2
	addiu	$2,$2,%lo(ConnectedIp)
	addu	$2,$3,$2
	lw	$2,0($2)
	nop
	beq	$2,$0,$L467
	nop

	xor	$2,$4,$2
	j	$31
	sltu	$2,$0,$2

$L467:
	j	$31
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	filter_checkConnect
	.size	filter_checkConnect, .-filter_checkConnect
	.section	.text.unregister_customRspStr,"ax",@progbits
	.align	2
	.globl	unregister_customRspStr
	.set	nomips16
	.ent	unregister_customRspStr
	.type	unregister_customRspStr, @function
unregister_customRspStr:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(block_message)
	lw	$4,%lo(block_message)($16)
	nop
	beq	$4,$0,$L468
	nop

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
$L468:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	unregister_customRspStr
	.size	unregister_customRspStr, .-unregister_customRspStr
	.section	.text.register_customRspStr,"ax",@progbits
	.align	2
	.globl	register_customRspStr
	.set	nomips16
	.ent	register_customRspStr
	.type	register_customRspStr, @function
register_customRspStr:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	jal	unregister_customRspStr
	move	$16,$4

	beq	$16,$0,$L471
	nop

	jal	strlen
	move	$4,$16

	addiu	$4,$2,1
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L470
	sw	$2,%lo(block_message)($3)

	move	$6,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC15)
	move	$4,$2
	addiu	$5,$5,%lo($LC15)
	j	sprintf
	addiu	$sp,$sp,24

$L470:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L471:
	lui	$16,%hi($LC2)
	jal	strlen
	addiu	$4,$16,%lo($LC2)

	move	$4,$2
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L470
	sw	$2,%lo(block_message)($3)

	addiu	$6,$16,%lo($LC2)
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC15)
	move	$4,$2
	addiu	$5,$5,%lo($LC15)
	j	sprintf
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	register_customRspStr
	.size	register_customRspStr, .-register_customRspStr
	.section	.text.register_customRspHook,"ax",@progbits
	.align	2
	.globl	register_customRspHook
	.set	nomips16
	.ent	register_customRspHook
	.type	register_customRspHook, @function
register_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L475
	lui	$2,%hi(fast_path_cusRsp401_func)

	sw	$4,%lo(fast_path_cusRsp401_func)($2)
$L476:
	beq	$5,$0,$L477
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($2)
$L478:
	beq	$6,$0,$L479
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L479:
	lui	$3,%hi(GenerateTCPENDACK)
	addiu	$3,$3,%lo(GenerateTCPENDACK)
	j	$31
	sw	$3,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L477:
	lui	$3,%hi(GenerateTCPFINACK)
	addiu	$3,$3,%lo(GenerateTCPFINACK)
	j	$L478
	sw	$3,%lo(fast_path_cusRspTCPFinAck_func)($2)

$L475:
	lui	$3,%hi(GenerateHTTP401)
	addiu	$3,$3,%lo(GenerateHTTP401)
	j	$L476
	sw	$3,%lo(fast_path_cusRsp401_func)($2)

	.set	macro
	.set	reorder
	.end	register_customRspHook
	.size	register_customRspHook, .-register_customRspHook
	.section	.text.unregister_customRspHook,"ax",@progbits
	.align	2
	.globl	unregister_customRspHook
	.set	nomips16
	.ent	unregister_customRspHook
	.type	unregister_customRspHook, @function
unregister_customRspHook:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(fast_path_cusRsp401_func)
	sw	$0,%lo(fast_path_cusRsp401_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	sw	$0,%lo(fast_path_cusRspTCPFinAck_func)($2)
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	j	$31
	sw	$0,%lo(fast_path_cusRspTCPEndAck_func)($2)

	.set	macro
	.set	reorder
	.end	unregister_customRspHook
	.size	unregister_customRspHook, .-unregister_customRspHook
	.section	.text.filter_enter,"ax",@progbits
	.align	2
	.globl	filter_enter
	.set	nomips16
	.ent	filter_enter
	.type	filter_enter, @function
filter_enter:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$17,%hi(dos_item_global)
	lw	$2,%lo(dos_item_global)($17)
	nop
	bne	$2,$0,$L483
	move	$16,$4

	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	nop
	bne	$2,$0,$L483
	lui	$2,%hi(table_list_head+24)

	lw	$2,%lo(table_list_head+24)($2)
	nop
	sltu	$2,$2,2
	bne	$2,$0,$L504
	lui	$3,%hi(none_dos_drop_pkt_cnt)

$L483:
	jal	rtl_get_skb_type
	move	$4,$16

	li	$3,2			# 0x2
	beq	$2,$3,$L505
	lui	$2,%hi(dos_item_ipv6)

	jal	rtl_get_skb_type
	move	$4,$16

	beq	$2,$0,$L506
	nop

$L501:
	li	$2,4			# 0x4
$L484:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L505:
	lw	$2,%lo(dos_item_ipv6)($2)
	nop
	andi	$2,$2,0x1
	beq	$2,$0,$L501
	nop

	jal	filter_dos
	move	$4,$16

	li	$3,4			# 0x4
	movn	$3,$0,$2
	j	$L484
	move	$2,$3

$L506:
	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,2048			# 0x800
	beq	$2,$3,$L486
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	nop
	beq	$2,$0,$L484
	li	$2,4			# 0x4

	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,34525			# 0x86dd
	bne	$2,$3,$L484
	li	$2,4			# 0x4

$L486:
	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	nop
	sltu	$2,$2,2
	bne	$2,$0,$L487
	nop

	jal	FilterWeb_v2
	move	$4,$16

	beq	$2,$0,$L488
	lui	$2,%hi(fast_path_cusRsp401_func)

	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	nop
	beq	$2,$0,$L500
	nop

$L502:
	jalr	$2
	move	$4,$16

	j	$L484
	move	$2,$0

$L504:
	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	j	$L484
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L487:
	lw	$2,%lo(dos_item_global)($17)
	nop
	andi	$2,$2,0x1
	bne	$2,$0,$L490
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	nop
	andi	$2,$2,0x1
	beq	$2,$0,$L491
	nop

$L490:
	jal	filter_dos
	move	$4,$16

	bne	$2,$0,$L484
	move	$2,$0

$L491:
	jal	rtl_ip_hdr
	move	$4,$16

	lbu	$3,9($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L501
	lui	$3,%hi(none_dos_drop_pkt_cnt)

	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	nop
	addiu	$4,$4,1
	j	$L484
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L488:
	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	nop
	beq	$2,$0,$L487
	lui	$3,%hi(url_cache)

	addiu	$3,$3,%lo(url_cache)
	lbu	$4,15($3)
	li	$2,1			# 0x1
	beq	$4,$2,$L507
	nop

	lbu	$3,16($3)
	nop
	bne	$3,$2,$L487
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
	nop
	bne	$2,$0,$L502
	nop

$L500:
	j	$L484
	move	$2,$0

$L507:
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	nop
	bne	$2,$0,$L502
	nop

	j	$L484
	move	$2,$0

	.set	macro
	.set	reorder
	.end	filter_enter
	.size	filter_enter, .-filter_enter
	.section	.rodata.str1.4
	.align	2
$LC33:
	.ascii	"enable_dos\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	filter_init
	.set	nomips16
	.ent	filter_init
	.type	filter_init, @function
filter_init:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	lui	$2,%hi(dos_item_global)
	sw	$0,%lo(dos_item_global)($2)
	lui	$2,%hi(dos_item_ipv6)
	sw	$0,%lo(dos_item_ipv6)($2)
	lui	$2,%hi(none_dos_drop_pkt_cnt)
	jal	dos_func_init
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)

	lui	$4,%hi($LC33)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(dos_proc_fops)
	addiu	$4,$4,%lo($LC33)
	move	$5,$0
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(dos_proc_fops)
	lui	$16,%hi(dos_timer)
	lui	$2,%hi(url_count)
	sw	$0,16($sp)
	jal	proc_create_data
	sb	$0,%lo(url_count)($2)

	move	$6,$0
	move	$7,$0
	addiu	$4,$16,%lo(dos_timer)
	lui	$3,%hi(res3)
	move	$5,$0
	jal	init_timer_key
	sw	$2,%lo(res3)($3)

	lui	$3,%hi(jiffies)
	lw	$2,%lo(jiffies)($3)
	lw	$5,%lo(jiffies)($3)
	addiu	$16,$16,%lo(dos_timer)
	addiu	$2,$2,100
	sw	$2,8($16)
	lui	$2,%hi(dos_timer_fn)
	move	$4,$16
	addiu	$5,$5,100
	addiu	$2,$2,%lo(dos_timer_fn)
	sw	$0,20($16)
	jal	mod_timer
	sw	$2,16($16)

	jal	filter_init_fastpath
	nop

	jal	igmp_delete_init_netlink
	nop

	lw	$31,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	filter_init
	.size	filter_init, .-filter_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	filter_exit
	.set	nomips16
	.ent	filter_exit
	.type	filter_exit, @function
filter_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(res3)
	lw	$5,%lo(res3)($16)
	nop
	beq	$5,$0,$L510
	nop

	lui	$4,%hi($LC33)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC33)

	sw	$0,%lo(res3)($16)
$L510:
	lui	$4,%hi(dos_timer)
	jal	del_timer
	addiu	$4,$4,%lo(dos_timer)

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	filter_exit_fastpath
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit
	.size	filter_exit, .-filter_exit
	.globl	fast_path_cusRspTCPEndAck_func
	.section	.bss.fast_path_cusRspTCPEndAck_func,"aw",@nobits
	.align	2
	.type	fast_path_cusRspTCPEndAck_func, @object
	.size	fast_path_cusRspTCPEndAck_func, 4
fast_path_cusRspTCPEndAck_func:
	.space	4
	.globl	fast_path_cusRspTCPFinAck_func
	.section	.bss.fast_path_cusRspTCPFinAck_func,"aw",@nobits
	.align	2
	.type	fast_path_cusRspTCPFinAck_func, @object
	.size	fast_path_cusRspTCPFinAck_func, 4
fast_path_cusRspTCPFinAck_func:
	.space	4
	.globl	fast_path_cusRsp401_func
	.section	.bss.fast_path_cusRsp401_func,"aw",@nobits
	.align	2
	.type	fast_path_cusRsp401_func, @object
	.size	fast_path_cusRsp401_func, 4
fast_path_cusRsp401_func:
	.space	4
	.globl	dos_proc_fops
	.section	.data.dos_proc_fops,"aw",@progbits
	.align	2
	.type	dos_proc_fops, @object
	.size	dos_proc_fops, 108
dos_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	dos_single_write
	.space	28
	.word	dos_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	___strtok
	.section	.bss.___strtok,"aw",@nobits
	.align	2
	.type	___strtok, @object
	.size	___strtok, 4
___strtok:
	.space	4
	.globl	dos_flag
	.section	.bss.dos_flag,"aw",@nobits
	.align	2
	.type	dos_flag, @object
	.size	dos_flag, 80
dos_flag:
	.space	80
	.globl	dos_lan_mask
	.section	.bss.dos_lan_mask,"aw",@nobits
	.align	2
	.type	dos_lan_mask, @object
	.size	dos_lan_mask, 4
dos_lan_mask:
	.space	4
	.globl	dos_lan_addr
	.section	.bss.dos_lan_addr,"aw",@nobits
	.align	2
	.type	dos_lan_addr, @object
	.size	dos_lan_addr, 4
dos_lan_addr:
	.space	4
	.globl	dos_item_ipv6
	.section	.dram-l34-fwd,"aw",@progbits
	.align	2
	.type	dos_item_ipv6, @object
	.size	dos_item_ipv6, 4
dos_item_ipv6:
	.space	4
	.globl	dos_item_global_0
	.align	2
	.type	dos_item_global_0, @object
	.size	dos_item_global_0, 4
dos_item_global_0:
	.space	4
	.globl	dos_item_global
	.align	2
	.type	dos_item_global, @object
	.size	dos_item_global, 4
dos_item_global:
	.space	4
	.globl	Ipv6_ns_threshold
	.section	.bss.Ipv6_ns_threshold,"aw",@nobits
	.align	2
	.type	Ipv6_ns_threshold, @object
	.size	Ipv6_ns_threshold, 4
Ipv6_ns_threshold:
	.space	4
	.globl	Ipv6_icmp_too_big_threshold
	.section	.bss.Ipv6_icmp_too_big_threshold,"aw",@nobits
	.align	2
	.type	Ipv6_icmp_too_big_threshold, @object
	.size	Ipv6_icmp_too_big_threshold, 4
Ipv6_icmp_too_big_threshold:
	.space	4
	.globl	Ipv6_smurf_threshold
	.section	.bss.Ipv6_smurf_threshold,"aw",@nobits
	.align	2
	.type	Ipv6_smurf_threshold, @object
	.size	Ipv6_smurf_threshold, 4
Ipv6_smurf_threshold:
	.space	4
	.globl	dosFilter_block
	.section	.bss.dosFilter_block,"aw",@nobits
	.align	2
	.type	dosFilter_block, @object
	.size	dosFilter_block, 4
dosFilter_block:
	.space	4
	.globl	per_ping_threshold
	.section	.bss.per_ping_threshold,"aw",@nobits
	.align	2
	.type	per_ping_threshold, @object
	.size	per_ping_threshold, 4
per_ping_threshold:
	.space	4
	.globl	per_icmp_threshold
	.section	.bss.per_icmp_threshold,"aw",@nobits
	.align	2
	.type	per_icmp_threshold, @object
	.size	per_icmp_threshold, 4
per_icmp_threshold:
	.space	4
	.globl	per_udp_threshold
	.section	.bss.per_udp_threshold,"aw",@nobits
	.align	2
	.type	per_udp_threshold, @object
	.size	per_udp_threshold, 4
per_udp_threshold:
	.space	4
	.globl	per_fin_threshold
	.section	.bss.per_fin_threshold,"aw",@nobits
	.align	2
	.type	per_fin_threshold, @object
	.size	per_fin_threshold, 4
per_fin_threshold:
	.space	4
	.globl	per_syn_threshold
	.section	.bss.per_syn_threshold,"aw",@nobits
	.align	2
	.type	per_syn_threshold, @object
	.size	per_syn_threshold, 4
per_syn_threshold:
	.space	4
	.globl	whole_icmp_threshold
	.section	.bss.whole_icmp_threshold,"aw",@nobits
	.align	2
	.type	whole_icmp_threshold, @object
	.size	whole_icmp_threshold, 4
whole_icmp_threshold:
	.space	4
	.globl	whole_udp_threshold
	.section	.bss.whole_udp_threshold,"aw",@nobits
	.align	2
	.type	whole_udp_threshold, @object
	.size	whole_udp_threshold, 4
whole_udp_threshold:
	.space	4
	.globl	whole_fin_threshold
	.section	.bss.whole_fin_threshold,"aw",@nobits
	.align	2
	.type	whole_fin_threshold, @object
	.size	whole_fin_threshold, 4
whole_fin_threshold:
	.space	4
	.globl	whole_syn_threshold
	.section	.bss.whole_syn_threshold,"aw",@nobits
	.align	2
	.type	whole_syn_threshold, @object
	.size	whole_syn_threshold, 4
whole_syn_threshold:
	.space	4
	.globl	dos_op_mode
	.section	.bss.dos_op_mode,"aw",@nobits
	.type	dos_op_mode, @object
	.size	dos_op_mode, 1
dos_op_mode:
	.space	1
	.globl	ConnectedIp
	.section	.bss.ConnectedIp,"aw",@nobits
	.align	2
	.type	ConnectedIp, @object
	.size	ConnectedIp, 1024
ConnectedIp:
	.space	1024
	.globl	dos_read_proc_func
	.section	.bss.dos_read_proc_func,"aw",@nobits
	.align	2
	.type	dos_read_proc_func, @object
	.size	dos_read_proc_func, 4
dos_read_proc_func:
	.space	4
	.globl	dos_timer_ipv6_func
	.section	.bss.dos_timer_ipv6_func,"aw",@nobits
	.align	2
	.type	dos_timer_ipv6_func, @object
	.size	dos_timer_ipv6_func, 4
dos_timer_ipv6_func:
	.space	4
	.globl	dos_timer_ipv4_func
	.section	.bss.dos_timer_ipv4_func,"aw",@nobits
	.align	2
	.type	dos_timer_ipv4_func, @object
	.size	dos_timer_ipv4_func, 4
dos_timer_ipv4_func:
	.space	4
	.globl	filter_dos_ipv6_handler
	.section	.bss.filter_dos_ipv6_handler,"aw",@nobits
	.align	2
	.type	filter_dos_ipv6_handler, @object
	.size	filter_dos_ipv6_handler, 4
filter_dos_ipv6_handler:
	.space	4
	.globl	filter_dos_ipv4_handler
	.section	.bss.filter_dos_ipv4_handler,"aw",@nobits
	.align	2
	.type	filter_dos_ipv4_handler, @object
	.size	filter_dos_ipv4_handler, 4
filter_dos_ipv4_handler:
	.space	4
	.globl	filter_table_proc_fops
	.section	.data.filter_table_proc_fops,"aw",@progbits
	.align	2
	.type	filter_table_proc_fops, @object
	.size	filter_table_proc_fops, 108
filter_table_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	filter_table_single_write
	.space	28
	.word	filter_table_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	table_list_head
	.section	.bss.table_list_head,"aw",@nobits
	.align	2
	.type	table_list_head, @object
	.size	table_list_head, 32
table_list_head:
	.space	32
	.globl	filter_tlb
	.section	.data.filter_tlb,"aw",@progbits
	.align	2
	.type	filter_tlb, @object
	.size	filter_tlb, 40
filter_tlb:
	.word	0
	.word	0
	.word	1
	.word	rtl_ip_range_filter_fastpath
	.word	2
	.word	rtl_mac_filter_fastpath
	.word	3
	.word	rtl_url_filter_fastpath
	.word	4
	.word	rtl_schedule_filter_fastpath
	.section	.bss.get_info_from_usr_space,"aw",@nobits
	.align	2
	.type	get_info_from_usr_space, @object
	.size	get_info_from_usr_space, 1024
get_info_from_usr_space:
	.space	1024
	.section	.bss.log_enable,"aw",@nobits
	.align	2
	.type	log_enable, @object
	.size	log_enable, 4
log_enable:
	.space	4
	.section	.bss.url_filter_mode,"aw",@nobits
	.align	2
	.type	url_filter_mode, @object
	.size	url_filter_mode, 4
url_filter_mode:
	.space	4
	.section	.bss.filter_rule_num,"aw",@nobits
	.align	2
	.type	filter_rule_num, @object
	.size	filter_rule_num, 4
filter_rule_num:
	.space	4
	.section	.bss.log_info,"aw",@nobits
	.align	2
	.type	log_info, @object
	.size	log_info, 32
log_info:
	.space	32
	.section	.bss.url_filter_flag,"aw",@nobits
	.align	2
	.type	url_filter_flag, @object
	.size	url_filter_flag, 4
url_filter_flag:
	.space	4
	.section	.bss.dos_item_backup,"aw",@nobits
	.align	2
	.type	dos_item_backup, @object
	.size	dos_item_backup, 4
dos_item_backup:
	.space	4
	.section	.bss.block_time,"aw",@nobits
	.align	2
	.type	block_time, @object
	.size	block_time, 4
block_time:
	.space	4
	.section	.bss.res12,"aw",@nobits
	.align	2
	.type	res12, @object
	.size	res12, 4
res12:
	.space	4
	.section	.bss.block_message,"aw",@nobits
	.align	2
	.type	block_message, @object
	.size	block_message, 4
block_message:
	.space	4
	.section	.bss.url_cache,"aw",@nobits
	.align	2
	.type	url_cache, @object
	.size	url_cache, 20
url_cache:
	.space	20
	.section	.bss.finStateClientPort,"aw",@nobits
	.align	1
	.type	finStateClientPort, @object
	.size	finStateClientPort, 2
finStateClientPort:
	.space	2
	.section	.bss.finStateClientSeq,"aw",@nobits
	.align	2
	.type	finStateClientSeq, @object
	.size	finStateClientSeq, 4
finStateClientSeq:
	.space	4
	.section	.bss.endStateClientPort,"aw",@nobits
	.align	1
	.type	endStateClientPort, @object
	.size	endStateClientPort, 2
endStateClientPort:
	.space	2
	.section	.bss.endStateClientSeq,"aw",@nobits
	.align	2
	.type	endStateClientSeq, @object
	.size	endStateClientSeq, 4
endStateClientSeq:
	.space	4
	.section	.dram-l34-fwd
	.align	2
	.type	none_dos_drop_pkt_cnt, @object
	.size	none_dos_drop_pkt_cnt, 4
none_dos_drop_pkt_cnt:
	.space	4
	.type	url_count, @object
	.size	url_count, 1
url_count:
	.space	1
	.section	.bss.res3,"aw",@nobits
	.align	2
	.type	res3, @object
	.size	res3, 4
res3:
	.space	4
	.section	.bss.dos_timer,"aw",@nobits
	.align	2
	.type	dos_timer, @object
	.size	dos_timer, 28
dos_timer:
	.space	28
	.section	.bss.block_count,"aw",@nobits
	.align	2
	.type	block_count, @object
	.size	block_count, 4
block_count:
	.space	4
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2080) 4.6.4"
