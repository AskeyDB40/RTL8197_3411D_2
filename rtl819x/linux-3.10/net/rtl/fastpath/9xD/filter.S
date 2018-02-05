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

	li	$3,-1037172736			# 0xffffffffc22e0000
	lw	$2,16($sp)
	addiu	$3,$3,17671
	li	$7,-1840709632			# 0xffffffff92490000
	mult	$2,$3
	sra	$4,$2,31
	addiu	$7,$7,9363
	lw	$5,24($sp)
	mfhi	$3
	lw	$6,8($5)
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

	li	$6,-2004353024			# 0xffffffff88880000
	ori	$3,$3,0xb3c5
	lw	$7,0($5)
	mult	$2,$3
	ori	$6,$6,0x8889
	mfhi	$3
	mult	$2,$6
	addu	$3,$3,$2
	sra	$3,$3,11
	subu	$3,$3,$4
	mfhi	$8
	addu	$2,$8,$2
	li	$8,715784192			# 0x2aaa0000
	ori	$8,$8,0xaaab
	mult	$3,$8
	sra	$2,$2,5
	subu	$2,$2,$4
	sra	$4,$3,31
	sra	$9,$2,31
	mfhi	$8
	mult	$2,$6
	sra	$8,$8,2
	subu	$4,$8,$4
	sll	$10,$4,3
	sll	$4,$4,5
	mfhi	$6
	subu	$4,$4,$10
	addu	$6,$6,$2
	subu	$3,$3,$4
	sra	$6,$6,5
	subu	$6,$6,$9
	sll	$8,$3,2
	sll	$4,$6,2
	sll	$6,$6,6
	subu	$6,$6,$4
	sll	$3,$3,6
	subu	$2,$2,$6
	subu	$3,$3,$8
	bne	$7,$0,$L8
	addu	$2,$3,$2

	lw	$3,4($5)
	beq	$3,$0,$L13
	sltu	$2,$3,$2

	xori	$2,$2,0x1
$L15:
	lw	$31,36($sp)
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
	bne	$2,$0,$L20
	lui	$18,%hi(dos_item_ipv6)

	lw	$3,%lo(dos_item_ipv6)($18)
	bne	$3,$0,$L40
	lui	$19,%hi(dosFilter_block)

	lui	$2,%hi(none_dos_drop_pkt_cnt)
$L43:
	lw	$3,%lo(none_dos_drop_pkt_cnt)($2)
	sltu	$4,$3,3001
	bne	$4,$0,$L41
	lui	$4,%hi(dos_item_backup)

$L37:
	lw	$3,%lo(dos_item_global)($16)
	bne	$3,$0,$L36
	lui	$4,%hi(dos_item_backup)

$L30:
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	lui	$4,%hi(dos_timer)
	lw	$31,36($sp)
	addiu	$4,$4,%lo(dos_timer)
	lw	$19,32($sp)
	addiu	$5,$5,100
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	mod_timer
	addiu	$sp,$sp,40

$L20:
	lui	$19,%hi(dosFilter_block)
$L40:
	li	$3,1			# 0x1
	lw	$4,%lo(dosFilter_block)($19)
	beq	$4,$3,$L34
	lui	$17,%hi(block_count)

	beq	$2,$0,$L23
	lui	$2,%hi(dos_timer_ipv4_func)

$L39:
	lw	$2,%lo(dos_timer_ipv4_func)($2)
	beq	$2,$0,$L23
	nop

	jalr	$2
	nop

$L23:
	lw	$2,%lo(dos_item_ipv6)($18)
	beq	$2,$0,$L42
	lui	$2,%hi(block_time)

	lui	$2,%hi(dos_timer_ipv6_func)
	lw	$2,%lo(dos_timer_ipv6_func)($2)
	beq	$2,$0,$L24
	nop

	jalr	$2
	nop

$L24:
	lui	$2,%hi(block_time)
$L42:
	lw	$3,%lo(block_count)($17)
	lw	$2,%lo(block_time)($2)
	slt	$2,$3,$2
	beq	$2,$0,$L25
	li	$2,4194304			# 0x400000

	lw	$3,%lo(dos_item_global)($16)
	and	$2,$3,$2
	bne	$2,$0,$L43
	lui	$2,%hi(none_dos_drop_pkt_cnt)

$L25:
	sw	$0,%lo(dosFilter_block)($19)
	jal	clear_attack_address
	sw	$0,%lo(block_count)($17)

	lui	$2,%hi(none_dos_drop_pkt_cnt)
	lw	$3,%lo(none_dos_drop_pkt_cnt)($2)
	sltu	$4,$3,3001
	beq	$4,$0,$L37
	lui	$4,%hi(dos_item_backup)

$L41:
	lw	$5,%lo(dos_item_backup)($4)
	beq	$5,$0,$L28
	nop

	sw	$5,%lo(dos_item_global)($16)
	sw	$0,%lo(dos_item_backup)($4)
$L28:
	bne	$3,$0,$L30
	lui	$4,%hi(dos_timer)

	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	addiu	$4,$4,%lo(dos_timer)
	lw	$31,36($sp)
	lw	$19,32($sp)
	addiu	$5,$5,100
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	mod_timer
	addiu	$sp,$sp,40

$L36:
	sw	$3,%lo(dos_item_backup)($4)
	lui	$3,%hi(dos_item_global)
	j	$L30
	sw	$0,%lo(dos_item_global)($3)

$L34:
	lw	$3,%lo(block_count)($17)
	addiu	$3,$3,1
	beq	$2,$0,$L23
	sw	$3,%lo(block_count)($17)

	j	$L39
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
	beq	$5,$4,$L73
$L45:
	lw	$31,60($sp)
	move	$2,$0
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L73:
	lw	$4,4($3)
	lui	$3,%hi(endStateClientSeq)
	lw	$3,%lo(endStateClientSeq)($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L45
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
	beq	$2,$0,$L45
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
	beq	$2,$0,$L74
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
	beq	$2,$0,$L75
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$19
	move	$18,$2
	lbu	$2,0($2)
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
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
$L71:
	lw	$20,4($17)
	andi	$2,$20,0x1
	bne	$2,$0,$L71
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
	bne	$20,$2,$L71
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L55:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L55
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L76
	move	$4,$19
$L79:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lui	$21,%hi(url_cache)
	lbu	$17,0($18)
	addiu	$20,$21,%lo(url_cache)
	lw	$3,12($18)
	li	$9,1048576			# 0x100000
	lw	$4,16($18)
	andi	$17,$17,0xf
	sll	$17,$17,2
	sw	$3,16($18)
	sw	$4,12($18)
	addu	$17,$18,$17
	lhu	$2,12($20)
	li	$3,-327680			# 0xfffffffffffb0000
	lw	$8,12($17)
	ori	$3,$3,0xffff
	sh	$2,14($17)
	li	$2,65536			# 0x10000
	sb	$0,13($17)
	and	$8,$8,$2
	lw	$2,4($17)
	lw	$4,12($17)
	addiu	$7,$2,1
	movn	$2,$7,$8
	and	$3,$4,$3
	li	$7,-131072			# 0xfffffffffffe0000
	or	$3,$3,$9
	lhu	$4,2($17)
	ori	$7,$7,0xffff
	lhu	$6,0($17)
	and	$3,$3,$7
	lw	$5,8($17)
	sh	$4,0($17)
	move	$4,$19
	sw	$3,12($17)
	sh	$6,2($17)
	sw	$5,4($17)
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
	bne	$4,$0,$L58
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L62
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L59:
	lbu	$4,1($4)
	beq	$2,$6,$L77
	addu	$16,$16,$4
$L60:
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L81
	move	$5,$22
	.set	macro
	.set	reorder

$L62:
	addu	$4,$17,$16
	lbu	$2,0($4)
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L59
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L60
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L74:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L48:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L48
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L45
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L45
$L77:
	addu	$23,$17,$23
	addiu	$16,$16,6
	addiu	$4,$sp,20
	li	$6,4			# 0x4
	move	$5,$23
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

$L58:
	move	$5,$22
$L81:
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

	addiu	$3,$18,16
	lhu	$4,%lo(url_cache)($21)
	addiu	$2,$2,-14
	lbu	$5,14($20)
	addiu	$4,$4,1
	sh	$4,4($18)
	sh	$2,2($18)
	sh	$0,6($18)
	lw	$2,0($18)
	lw	$7,4($18)
	sb	$5,8($18)
	srl	$5,$2,22
	sh	$0,10($18)
	addu	$2,$7,$2
	lw	$4,8($18)
	sltu	$7,$2,$7
	lw	$6,12($18)
	andi	$5,$5,0x3c
	addu	$2,$2,$4
	addu	$5,$18,$5
	addu	$7,$2,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	sltu	$2,$4,$6
	addu	$2,$2,$4
$L63:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L63
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	sb	$0,16($20)
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
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sh	$2,10($18)
	.set	macro
	.set	reorder

	j	$L45
$L75:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L50:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L50
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L78
	lui	$4,%hi($LC1)
$L80:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	j	$L45
$L76:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L79
	move	$4,$19
	.set	macro
	.set	reorder

$L78:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L80
	lui	$4,%hi($LC1)
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
	beq	$5,$4,$L110
$L83:
	lw	$31,60($sp)
	move	$2,$0
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L110:
	lw	$4,4($3)
	lui	$3,%hi(finStateClientSeq)
	lw	$3,%lo(finStateClientSeq)($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L83
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
	beq	$2,$0,$L83
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
	beq	$2,$0,$L111
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
	beq	$2,$0,$L112
	move	$19,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$19
	move	$18,$2
	lbu	$2,0($2)
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
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
$L108:
	lw	$20,4($17)
	andi	$2,$20,0x1
	bne	$2,$0,$L108
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
	bne	$20,$2,$L108
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L93:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L93
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L113
	move	$4,$19
$L116:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lui	$21,%hi(url_cache)
	lbu	$17,0($18)
	addiu	$20,$21,%lo(url_cache)
	lw	$3,12($18)
	li	$5,1114112			# 0x110000
	andi	$17,$17,0xf
	lw	$4,16($18)
	lhu	$2,12($20)
	sll	$17,$17,2
	addu	$17,$18,$17
	sw	$4,12($18)
	move	$4,$19
	sw	$3,16($18)
	sh	$2,14($17)
	li	$2,-327680			# 0xfffffffffffb0000
	sb	$0,13($17)
	ori	$2,$2,0xffff
	lw	$3,12($17)
	lhu	$6,0($17)
	and	$2,$3,$2
	lhu	$7,2($17)
	lw	$3,4($17)
	or	$2,$2,$5
	lw	$5,8($17)
	sh	$7,0($17)
	sw	$3,8($17)
	sh	$6,2($17)
	sw	$5,4($17)
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
	bne	$4,$0,$L95
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L99
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L96:
	lbu	$4,1($4)
	beq	$2,$6,$L114
	addu	$16,$16,$4
$L97:
	sltu	$2,$16,$3
	beq	$2,$0,$L95
$L99:
	addu	$4,$17,$16
	lbu	$2,0($4)
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L96
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L97
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L111:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L86:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L86
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L83
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L83
$L114:
	addu	$23,$17,$23
	addiu	$16,$16,6
	addiu	$4,$sp,20
	li	$6,4			# 0x4
	move	$5,$23
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

$L95:
	lhu	$7,2($17)
	lui	$2,%hi(endStateClientPort)
	lw	$3,8($17)
	move	$5,$22
	sh	$7,%lo(endStateClientPort)($2)
	move	$6,$0
	sh	$0,16($17)
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

	addiu	$3,$18,16
	lhu	$4,%lo(url_cache)($21)
	addiu	$2,$2,-14
	lbu	$5,14($20)
	addiu	$4,$4,1
	sh	$4,4($18)
	sh	$2,2($18)
	sh	$0,6($18)
	lw	$2,0($18)
	lw	$7,4($18)
	sb	$5,8($18)
	srl	$5,$2,22
	sh	$0,10($18)
	addu	$2,$7,$2
	lw	$4,8($18)
	sltu	$7,$2,$7
	lw	$6,12($18)
	andi	$5,$5,0x3c
	addu	$2,$2,$4
	addu	$5,$18,$5
	addu	$7,$2,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	sltu	$2,$4,$6
	addu	$2,$2,$4
$L100:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L100
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

	j	$L83
$L112:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L88:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L88
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L115
	lui	$4,%hi($LC1)
$L117:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	j	$L83
$L113:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L116
	move	$4,$19
	.set	macro
	.set	reorder

$L115:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L117
	lui	$4,%hi($LC1)
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
	beq	$2,$0,$L119
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
	beq	$2,$0,$L154
	lui	$18,%hi(block_message)
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L123
$L152:
	jal	strlen
	move	$4,$17
	li	$5,96			# 0x60
	move	$6,$2
	.set	noreorder
	.set	nomacro
	jal	skb_copy_expand
	li	$7,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L155
	move	$21,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$4,$2
	.set	macro
	.set	reorder

	move	$4,$21
	move	$20,$2
	lbu	$2,0($2)
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$20,$22
	lw	$3,12($22)
	srl	$3,$3,28
	addu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L128
$L153:
	jal	strlen
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	skb_put
	move	$5,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_hh_from_neigh
	move	$4,$16
	.set	macro
	.set	reorder

	move	$19,$2
$L150:
	lw	$23,4($19)
	andi	$2,$23,0x1
	bne	$2,$0,$L150
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
	bne	$23,$2,$L150
	.set	noreorder
	.set	nomacro
	jal	rtl_get_len_from_hh
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	skb_push
	move	$5,$2
	.set	macro
	.set	reorder

	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L133:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L133
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L156
	move	$5,$0
$L160:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$21
	.set	macro
	.set	reorder

	lbu	$19,0($20)
	lw	$3,12($20)
	lw	$2,16($20)
	andi	$19,$19,0xf
	sll	$19,$19,2
	sw	$3,16($20)
	addu	$19,$20,$19
	sw	$2,12($20)
	lw	$16,12($19)
	lhu	$2,0($19)
	lhu	$3,2($19)
	srl	$16,$16,28
	lw	$5,%lo(block_message)($18)
	sll	$16,$16,2
	addu	$16,$19,$16
	sh	$3,0($19)
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L135
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

	move	$4,$16
	lw	$5,24($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$6,$2,1
	.set	macro
	.set	reorder

$L136:
	lw	$2,8($19)
	move	$4,$17
	lw	$16,4($19)
	lui	$18,%hi(url_cache)
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
	srl	$4,$4,28
	andi	$3,$3,0xf
	sb	$0,13($19)
	sh	$5,14($19)
	addu	$4,$4,$3
	lw	$5,12($19)
	sll	$4,$4,2
	li	$3,-327680			# 0xfffffffffffb0000
	subu	$16,$16,$4
	ori	$3,$3,0xffff
	li	$4,1572864			# 0x180000
	and	$3,$5,$3
	addu	$16,$16,$2
	li	$2,-131072			# 0xfffffffffffe0000
	or	$3,$3,$4
	ori	$2,$2,0xffff
	sh	$0,18($19)
	and	$3,$3,$2
	sw	$16,8($19)
	sw	$3,12($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	move	$4,$21
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
	bne	$4,$0,$L137
	addu	$22,$22,$2
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L141
	li	$6,8			# 0x8
	.set	macro
	.set	reorder

$L138:
	lbu	$4,1($4)
	beq	$2,$6,$L157
	addu	$16,$16,$4
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L159
	move	$5,$22
	.set	macro
	.set	reorder

$L141:
	addu	$4,$19,$16
$L162:
	lbu	$2,0($4)
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L138
	addiu	$23,$16,2
	.set	macro
	.set	reorder

	addiu	$16,$16,1
	sltu	$2,$16,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L162
	addu	$4,$19,$16
	.set	macro
	.set	reorder

$L137:
	move	$5,$22
$L159:
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

	addiu	$3,$20,16
	lhu	$4,%lo(url_cache)($18)
	addiu	$2,$2,-14
	lbu	$5,14($17)
	addiu	$4,$4,2
	sh	$4,4($20)
	sh	$2,2($20)
	sh	$0,6($20)
	lw	$2,0($20)
	lw	$7,4($20)
	sb	$5,8($20)
	srl	$5,$2,22
	sh	$0,10($20)
	addu	$2,$7,$2
	lw	$4,8($20)
	sltu	$7,$2,$7
	lw	$6,12($20)
	andi	$5,$5,0x3c
	addu	$2,$2,$4
	addu	$5,$20,$5
	addu	$7,$2,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	sltu	$2,$4,$6
	addu	$2,$2,$4
$L142:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$4,$2
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L142
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
	sh	$2,10($20)
	li	$2,1			# 0x1
	lhu	$5,2($19)
	move	$4,$21
	lw	$3,8($19)
	sb	$2,15($17)
	lui	$2,%hi(finStateClientPort)
	sh	$5,%lo(finStateClientPort)($2)
	lui	$2,%hi(finStateClientSeq)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sw	$3,%lo(finStateClientSeq)($2)
	.set	macro
	.set	reorder

$L119:
	lw	$31,68($sp)
	move	$2,$0
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

$L155:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L126:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L126
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L158
	lui	$4,%hi($LC1)
$L161:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC1)
	.set	macro
	.set	reorder

	move	$2,$0
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

$L157:
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
	j	$L159
	move	$5,$22
	.set	macro
	.set	reorder

$L154:
	addiu	$2,$16,20
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L122:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L122
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L119
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L119
$L123:
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	j	$L152
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

$L135:
	lui	$18,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo($LC2)
	.set	macro
	.set	reorder

	move	$4,$16
	addiu	$5,$18,%lo($LC2)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$6,$2
	.set	macro
	.set	reorder

	j	$L136
$L128:
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	j	$L153
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

$L156:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L160
	move	$5,$0
	.set	macro
	.set	reorder

$L158:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L161
	lui	$4,%hi($LC1)
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

	beq	$2,$0,$L164
	move	$18,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,512			# 0x200

	lw	$4,32($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L181
	move	$16,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,256			# 0x100

	lui	$2,%hi(table_list_head)
	lw	$19,%lo(table_list_head)($2)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L166
	lui	$fp,%hi($LC7)

	lui	$23,%hi($LC8)
	addiu	$fp,$fp,%lo($LC7)
	j	$L175
	addiu	$23,$23,%lo($LC8)

$L183:
	beq	$2,$3,$L170
	li	$3,2			# 0x2

	beq	$2,$3,$L182
	lui	$5,%hi($LC11)

	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC11)

	move	$4,$18
	jal	strcat
	move	$5,$16

$L171:
	lui	$2,%hi(table_list_head)
$L184:
	lw	$19,0($19)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L185
	lui	$2,%hi(url_filter_mode)

$L175:
	lw	$7,24($19)
	lui	$2,%hi($LC3)
	lw	$6,16($19)
	move	$4,$18
	addiu	$5,$2,%lo($LC3)
	jal	sprintf
	addiu	$7,$7,-1

	li	$3,3			# 0x3
	lw	$2,16($19)
	bne	$2,$3,$L183
	li	$3,4			# 0x4

	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L184
	lui	$2,%hi(table_list_head)

$L173:
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
	bne	$17,$20,$L173
	lui	$2,%hi(table_list_head)

	lw	$19,0($19)
	addiu	$2,$2,%lo(table_list_head)
	bne	$19,$2,$L175
	nop

$L166:
	lui	$2,%hi(url_filter_mode)
$L185:
	lui	$5,%hi($LC12)
	lw	$6,%lo(url_filter_mode)($2)
	addiu	$5,$5,%lo($LC12)
	lw	$4,72($sp)
	jal	seq_printf
	move	$7,$18

	jal	kfree
	move	$4,$18

	jal	kfree
	move	$4,$16

$L164:
	lw	$31,68($sp)
	move	$2,$0
	lw	$fp,64($sp)
	lw	$23,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,72

$L182:
	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L171
	lui	$2,%hi($LC4)

	lui	$21,%hi($LC5)
	addiu	$22,$2,%lo($LC4)
	addiu	$21,$21,%lo($LC5)
$L172:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$22

	move	$4,$18
	jal	strcat
	move	$5,$16

	move	$4,$16
	lbu	$2,30($17)
	move	$5,$21
	lbu	$6,28($17)
	lbu	$7,29($17)
	sw	$2,16($sp)
	lbu	$2,31($17)
	sw	$2,20($sp)
	lbu	$2,32($17)
	sw	$2,24($sp)
	lbu	$2,33($17)
	jal	sprintf
	sw	$2,28($sp)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	bne	$17,$20,$L172
	lui	$2,%hi(table_list_head)

	j	$L184
	nop

$L170:
	lw	$17,8($19)
	addiu	$20,$19,8
	beq	$17,$20,$L171
	lui	$2,%hi($LC9)

	lui	$21,%hi($LC10)
	addiu	$22,$2,%lo($LC9)
	addiu	$21,$21,%lo($LC10)
$L174:
	lw	$6,20($17)
	lui	$5,%hi($LC4)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC4)

	move	$4,$18
	jal	strcat
	move	$5,$16

	move	$4,$16
	lw	$6,28($17)
	jal	sprintf
	move	$5,$23

	move	$4,$18
	jal	strcat
	move	$5,$16

	move	$4,$16
	lw	$6,32($17)
	jal	sprintf
	move	$5,$22

	move	$4,$18
	jal	strcat
	move	$5,$16

	move	$4,$16
	lw	$6,36($17)
	jal	sprintf
	move	$5,$21

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	bne	$17,$20,$L174
	lui	$2,%hi(table_list_head)

	j	$L184
	nop

$L181:
	jal	kfree
	move	$4,$18

	j	$L164
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
	li	$16,6			# 0x6
	move	$17,$4
	li	$20,12			# 0xc
	move	$19,$5
	addu	$2,$19,$16
$L191:
	lbu	$18,-6($2)
	jal	rtl_skb_mac_header
	move	$4,$17

	addu	$2,$2,$16
	lbu	$2,0($2)
	bne	$18,$2,$L189
	addiu	$16,$16,1

	bne	$16,$20,$L191
	addu	$2,$19,$16

	lw	$31,36($sp)
	li	$2,1			# 0x1
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L189:
	lw	$31,36($sp)
	move	$2,$0
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
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

	beq	$2,$0,$L201
	li	$2,-268435456			# 0xfffffffff0000000

	lw	$3,0($17)
	and	$3,$3,$2
	li	$2,1610612736			# 0x60000000
	beq	$3,$2,$L204
	lui	$2,%hi(dos_item_global)

	lw	$2,%lo(dos_item_global)($2)
	beq	$2,$0,$L201
	lui	$2,%hi(filter_dos_ipv4_handler)

	lw	$2,%lo(filter_dos_ipv4_handler)($2)
	beq	$2,$0,$L201
	nop

$L203:
	jalr	$2
	move	$4,$16

	li	$3,-1			# 0xffffffffffffffff
	movz	$3,$0,$2
	lw	$31,28($sp)
	move	$2,$3
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L204:
	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	beq	$2,$0,$L201
	lui	$2,%hi(filter_dos_ipv6_handler)

	lw	$2,%lo(filter_dos_ipv6_handler)($2)
	bne	$2,$0,$L203
	nop

$L201:
	lw	$31,28($sp)
	move	$2,$0
	lw	$17,24($sp)
	lw	$16,20($sp)
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
	subu	$3,$5,$7
	lw	$18,80($sp)
	move	$16,$4
	lw	$19,84($sp)
	move	$17,$5
	move	$21,$7
	move	$23,$0
	sw	$0,0($19)
	move	$20,$4
	sw	$6,72($sp)
	move	$22,$0
	sw	$0,0($18)
	li	$fp,13			# 0xd
	sw	$3,16($sp)
$L211:
	lb	$2,0($20)
	beq	$2,$fp,$L224
	li	$3,10			# 0xa

	beq	$2,$3,$L225
	andi	$2,$23,0x1

$L217:
	move	$23,$0
$L207:
	lw	$5,72($sp)
	move	$4,$20
	jal	memcmp
	move	$6,$21

	beq	$2,$0,$L210
	nop

	lw	$3,16($sp)
	addiu	$22,$22,1
	sltu	$2,$3,$22
	beq	$2,$0,$L211
	addiu	$20,$20,1

$L221:
	lw	$31,60($sp)
	move	$2,$0
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

$L210:
	addu	$21,$22,$21
	addu	$2,$16,$21
	sw	$21,0($18)
	lb	$3,0($2)
	li	$2,13			# 0xd
	beq	$3,$2,$L219
	move	$2,$0

	sltu	$2,$17,$21
	bne	$2,$0,$L221
	move	$2,$0

	j	$L213
	li	$5,13			# 0xd

$L214:
	bne	$4,$0,$L221
	nop

$L213:
	addiu	$21,$21,1
	addiu	$2,$2,1
	addu	$3,$16,$21
	lb	$3,0($3)
	bne	$3,$5,$L214
	sltu	$4,$17,$21

	sw	$2,0($19)
	li	$2,1			# 0x1
$L226:
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

$L224:
	andi	$2,$23,0x1
	bne	$2,$0,$L217
	addiu	$23,$23,1

	slt	$2,$23,4
	bne	$2,$0,$L207
	nop

	j	$L221
	nop

$L225:
	beq	$2,$0,$L217
	addiu	$23,$23,1

	slt	$2,$23,4
	bne	$2,$0,$L207
	nop

	j	$L221
	nop

$L219:
	sw	$2,0($19)
	j	$L226
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
	.frame	$sp,72,$31		# vars= 16, regs= 7/0, args= 24, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$21,64($sp)
	sw	$20,60($sp)
	sw	$19,56($sp)
	sw	$18,52($sp)
	sw	$17,48($sp)
	sw	$16,44($sp)
	move	$18,$4
	move	$4,$5
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$16,$5
	.set	macro
	.set	reorder

	sltu	$2,$2,2
	bne	$2,$0,$L239
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
	beq	$2,$0,$L239
	move	$21,$2
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
	beq	$2,$0,$L239
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
	beq	$2,$0,$L234
	move	$18,$2
	.set	macro
	.set	reorder

	sll	$19,$19,2
	lui	$20,%hi($LC13)
	addu	$19,$21,$19
	addiu	$5,$20,%lo($LC13)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L234
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
	beq	$2,$0,$L250
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$4,28($sp)
	lw	$3,32($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L233
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
$L233:
	addu	$2,$17,$2
	move	$4,$19
	sb	$0,0($2)
	addiu	$2,$sp,32
	sw	$2,16($sp)
	move	$5,$18
	addiu	$2,$sp,24
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
	beq	$2,$0,$L250
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$2,24($sp)
	addiu	$2,$2,-9
	.set	noreorder
	.set	nomacro
	blez	$2,$L250
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
	move	$4,$17
	lw	$3,28($sp)
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
	beq	$18,$0,$L249
	move	$19,$2
	.set	macro
	.set	reorder

$L244:
	move	$16,$0
$L237:
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
	bne	$2,$3,$L228
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L246
	lui	$3,%hi(url_filter_flag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L228
	sw	$2,%lo(url_filter_flag)($3)
	.set	macro
	.set	reorder

$L239:
	move	$16,$0
$L228:
	move	$2,$16
	lw	$31,68($sp)
	lw	$21,64($sp)
	lw	$20,60($sp)
	lw	$19,56($sp)
	lw	$18,52($sp)
	lw	$17,48($sp)
	lw	$16,44($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L234:
	move	$4,$17
$L250:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$16,$0
	.set	macro
	.set	reorder

	move	$2,$16
	lw	$31,68($sp)
	lw	$21,64($sp)
	lw	$20,60($sp)
	lw	$19,56($sp)
	lw	$18,52($sp)
	lw	$17,48($sp)
	lw	$16,44($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,72
	.set	macro
	.set	reorder

$L249:
	addu	$20,$17,$2
	lb	$2,0($20)
	beq	$2,$0,$L244
	move	$4,$0
$L238:
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
	beq	$2,$0,$L245
	addu	$3,$20,$18
	.set	macro
	.set	reorder

	lb	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L238
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L237
	move	$16,$0
	.set	macro
	.set	reorder

$L245:
	.set	noreorder
	.set	nomacro
	j	$L237
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L246:
	.set	noreorder
	.set	nomacro
	j	$L228
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

	beq	$2,$0,$L252
	lw	$2,372($2)
	beq	$2,$0,$L252
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L252
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L281
	j	$L253
$L256:
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L253
$L281:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L256
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L252:
	lui	$16,%hi($LC16)
$L285:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC16)
	.set	macro
	.set	reorder

	beq	$2,$0,$L258
	lw	$2,372($2)
	beq	$2,$0,$L278
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L278
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L282
	j	$L259
$L262:
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L259
$L282:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L262
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L278:
	li	$2,1			# 0x1
$L257:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L253:
	lw	$3,16($17)
	lw	$2,28($4)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L285
	lui	$16,%hi($LC16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L257
	move	$2,$0
	.set	macro
	.set	reorder

$L259:
	lw	$3,16($17)
	lw	$2,24($4)
	lw	$31,28($sp)
	xor	$2,$3,$2
	lw	$17,24($sp)
	lw	$16,20($sp)
	sltu	$2,$0,$2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L258:
	jal	rtl_get_wan_dev
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L263
	lui	$16,%hi($LC17)
	.set	macro
	.set	reorder

	lw	$2,372($2)
	beq	$2,$0,$L278
	lw	$16,12($2)
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L279
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	j	$L257
$L284:
	lw	$16,8($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L257
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L279:
	.set	noreorder
	.set	nomacro
	jal	rtl_strcmp_wan_dev_name
	addiu	$4,$16,43
	.set	macro
	.set	reorder

	bne	$2,$0,$L284
	beq	$16,$0,$L278
	lw	$3,16($17)
	lw	$2,28($16)
	xor	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L257
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

$L263:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC17)
	.set	macro
	.set	reorder

	beq	$2,$0,$L278
	lw	$2,372($2)
	beq	$2,$0,$L278
	lw	$3,12($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L278
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	bne	$5,$0,$L283
	j	$L266
$L269:
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
	move	$5,$1
2:	subu	$5,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	beq	$5,$0,$L266
$L283:
	lw	$3,8($3)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L269
	addiu	$4,$3,43
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L257
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L266:
	lw	$4,16($17)
	lw	$2,28($3)
	xor	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L257
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
	beq	$3,$0,$L291
	nop

	addiu	$4,$4,1
	move	$2,$0
$L290:
	addiu	$5,$3,-48
	addiu	$7,$3,-97
	sltu	$6,$5,10
	bne	$6,$0,$L288
	sltu	$7,$7,6

	addiu	$6,$3,-65
	addiu	$5,$3,-87
	bne	$7,$0,$L288
	sltu	$6,$6,6

	beq	$6,$0,$L292
	nop

	addiu	$5,$3,-55
$L288:
	addiu	$4,$4,1
	sll	$2,$2,4
	lbu	$3,-1($4)
	bne	$3,$0,$L290
	addu	$2,$5,$2

	j	$31
	nop

$L292:
	j	$31
	li	$2,-1			# 0xffffffffffffffff

$L291:
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

	addiu	$3,$16,%lo(table_list_head)
	lui	$4,%hi(table_list_head+8)
	move	$2,$0
	addiu	$4,$4,%lo(table_list_head+8)
	sw	$3,%lo(table_list_head)($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	sw	$4,8($3)
	sw	$4,12($3)
	li	$4,1			# 0x1
	sw	$3,4($3)
	sw	$0,16($3)
	sw	$0,20($3)
	sw	$0,28($3)
	sw	$4,24($3)
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
	move	$19,$4
	lw	$2,24($16)
	bne	$2,$0,$L296
	move	$18,$5

	jal	filter_table_head_init
	nop

$L296:
	lui	$2,%hi(kmalloc_caches+20)
	lw	$4,%lo(kmalloc_caches+20)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	li	$6,1			# 0x1
	move	$3,$2
	addiu	$5,$2,8
	sw	$2,0($3)
	lw	$4,%lo(table_list_head)($17)
	sw	$19,16($2)
	move	$2,$0
	sw	$0,20($3)
	sw	$6,24($3)
	sw	$5,8($3)
	sw	$5,12($3)
	sw	$18,28($3)
	sw	$3,4($4)
	sw	$4,0($3)
	sw	$16,4($3)
	lw	$4,24($16)
	sw	$3,%lo(table_list_head)($17)
	addiu	$3,$4,1
	sw	$3,24($16)
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
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
	beq	$16,$20,$L298
	li	$18,1048576			# 0x100000

	li	$17,2097152			# 0x200000
	addiu	$18,$18,256
	addiu	$17,$17,512
	li	$21,3			# 0x3
$L302:
	lw	$2,8($16)
	addiu	$23,$16,8
	lw	$3,0($2)
	beq	$2,$23,$L299
	nop

$L303:
	lw	$5,4($2)
	move	$4,$2
	move	$22,$3
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$18,0($2)
	jal	kfree
	sw	$17,4($2)

	lw	$3,24($16)
	addiu	$3,$3,-1
	sw	$3,24($16)
	lw	$3,0($22)
	bne	$22,$23,$L303
	move	$2,$22

$L299:
	lw	$2,16($16)
	beq	$2,$21,$L305
	li	$2,1			# 0x1

$L301:
	lw	$2,4($16)
	move	$4,$16
	lw	$3,0($16)
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	lw	$2,0($19)
	beq	$19,$20,$L306
	li	$3,1			# 0x1

	move	$16,$19
$L307:
	j	$L302
	move	$19,$2

$L305:
	lw	$3,24($16)
	bne	$3,$2,$L301
	nop

	jal	Del_Pattern_ACL_For_ContentFilter
	nop

	move	$4,$16
	lw	$2,4($16)
	lw	$3,0($16)
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	lw	$2,0($19)
	bne	$19,$20,$L307
	move	$16,$19

$L298:
	li	$3,1			# 0x1
$L306:
	lui	$2,%hi(filter_rule_num)
	sw	$3,%lo(filter_rule_num)($2)
	lui	$3,%hi(log_enable)
	lw	$31,52($sp)
	move	$2,$0
	lw	$23,48($sp)
	lw	$22,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	sw	$0,%lo(log_enable)($3)
	lui	$3,%hi(url_filter_mode)
	sw	$0,%lo(url_filter_mode)($3)
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
	lui	$2,%hi(table_list_head)
	lw	$3,%lo(table_list_head)($2)
	addiu	$2,$2,%lo(table_list_head)
	bne	$3,$2,$L313
	nop

	j	$L309
	li	$2,-1			# 0xffffffffffffffff

$L317:
	lw	$3,0($3)
	beq	$3,$2,$L314
	nop

$L313:
	lw	$6,16($3)
	bne	$6,$4,$L317
	addiu	$6,$3,8

	lw	$2,8($3)
	sw	$5,4($2)
	sw	$2,0($5)
	sw	$6,4($5)
	lw	$2,24($3)
	sw	$5,8($3)
	addiu	$2,$2,1
	sw	$2,24($3)
	li	$3,3			# 0x3
	beq	$4,$3,$L320
	li	$3,2			# 0x2

$L316:
	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L314:
	li	$2,-1			# 0xffffffffffffffff
$L309:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

$L320:
	bne	$2,$3,$L316
	nop

	jal	Add_Pattern_ACL_For_ContentFilter
	nop

	j	$L309
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
	bltz	$3,$L328
	sra	$17,$17,24

	addu	$3,$16,$3
	lb	$5,0($3)
	bne	$5,$17,$L328
	addu	$16,$16,$2

	j	$L323
	move	$3,$0

$L325:
	lb	$4,-1($16)
	bne	$4,$5,$L331
	nop

$L323:
	addiu	$3,$3,1
	bne	$2,$3,$L325
	addiu	$16,$16,-1

	li	$3,-2147483648			# 0xffffffff80000000
	addiu	$3,$3,1
	and	$2,$2,$3
	bltz	$2,$L332
	li	$3,-2			# 0xfffffffffffffffe

$L322:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L331:
	li	$2,-2147483648			# 0xffffffff80000000
	addiu	$2,$2,1
	and	$2,$3,$2
	bgez	$2,$L322
	li	$3,-2			# 0xfffffffffffffffe

$L332:
	addiu	$2,$2,-1
	or	$2,$2,$3
	lw	$31,28($sp)
	lw	$17,24($sp)
	addiu	$2,$2,1
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L328:
	j	$L322
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
	move	$21,$5
	lw	$4,40($20)
	li	$5,208			# 0xd0
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L360
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

	addiu	$4,$18,%lo(get_info_from_usr_space)
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L335
	or	$3,$21,$16
	.set	macro
	.set	reorder

	addu	$2,$21,$16
	or	$2,$3,$2
	lw	$3,24($28)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L361
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
$L336:
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L373
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L367
	addiu	$3,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addiu	$4,$18,%lo(get_info_from_usr_space)
$L372:
	.set	noreorder
	.set	nomacro
	jal	strlen
	lui	$16,%hi($LC18)
	.set	macro
	.set	reorder

	addu	$2,$17,$2
	sb	$0,-1($2)
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC18)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC18)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L368
	addiu	$16,$sp,16
	.set	macro
	.set	reorder

	lui	$16,%hi($LC20)
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC20)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC20)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L341
	lui	$16,%hi($LC21)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

$L340:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

$L334:
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

$L368:
	lui	$18,%hi($LC19)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L335
	addiu	$20,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lui	$18,%hi(filter_tlb)
	addiu	$18,$18,%lo(filter_tlb)
	move	$5,$20
$L374:
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
	addu	$3,$18,$3
	lw	$5,4($3)
	.set	noreorder
	.set	nomacro
	jal	filter_table_regist
	move	$4,$2
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L374
	move	$5,$20
	.set	macro
	.set	reorder

	j	$L340
$L367:
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
	.set	noreorder
	.set	nomacro
	j	$L372
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

$L341:
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC21)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC21)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L342
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	.set	noreorder
	.set	nomacro
	j	$L340
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

$L342:
	lui	$16,%hi($LC22)
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC22)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC22)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L343
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	.set	noreorder
	.set	nomacro
	j	$L340
	sw	$3,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L335:
	move	$4,$17
$L373:
	.set	noreorder
	.set	nomacro
	jal	kfree
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

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

$L343:
	lui	$16,%hi($LC23)
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC23)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC23)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L344
	lui	$2,%hi(url_filter_mode)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L340
	sw	$0,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L360:
	.set	noreorder
	.set	nomacro
	j	$L334
	li	$19,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

$L344:
	lui	$16,%hi($LC24)
	lw	$18,16($sp)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC24)
	.set	macro
	.set	reorder

	addiu	$5,$16,%lo($LC24)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$2
	.set	macro
	.set	reorder

	bne	$2,$0,$L340
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
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L335
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
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L373
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L335
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
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L335
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	li	$21,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L335
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC19)
	.set	macro
	.set	reorder

	lw	$3,16($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L373
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L335
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lui	$23,%hi($LC26)
	li	$fp,59			# 0x3b
	addiu	$23,$23,%lo($LC26)
	addiu	$18,$18,%lo($LC19)
	li	$22,32			# 0x20
	move	$4,$16
$L376:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	j	$L345
	sw	$2,20($sp)
	.set	macro
	.set	reorder

$L369:
	lw	$4,20($sp)
$L345:
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L375
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

	move	$4,$16
	addu	$2,$21,$2
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	strsep
	sb	$fp,0($2)
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L369
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

$L375:
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$18
	.set	macro
	.set	reorder

	move	$21,$2
	lw	$2,20($sp)
	bne	$2,$0,$L366
	.set	noreorder
	.set	nomacro
	j	$L373
	move	$4,$17
	.set	macro
	.set	reorder

$L349:
	jal	strlen
	addiu	$4,$sp,20
	addu	$2,$21,$2
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	strsep
	sb	$22,0($2)
	.set	macro
	.set	reorder

$L366:
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L349
	move	$4,$21
	.set	macro
	.set	reorder

	lb	$2,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L350
	move	$5,$21
	.set	macro
	.set	reorder

	move	$6,$0
	li	$7,92			# 0x5c
$L354:
	beq	$2,$7,$L370
$L351:
	addiu	$5,$5,1
	lb	$2,0($5)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L354
	addiu	$6,$6,1
	.set	macro
	.set	reorder

$L350:
	lw	$4,4($20)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L340
	move	$5,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(filter_rule_num)
	move	$4,$21
	lw	$3,%lo(filter_rule_num)($2)
	addiu	$2,$5,8
	sw	$2,8($5)
	sw	$2,12($5)
	lw	$2,24($sp)
	sw	$5,0($5)
	sw	$5,4($5)
	sw	$0,24($5)
	sw	$3,20($5)
	sw	$2,16($5)
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
	bne	$2,$0,$L371
	addiu	$4,$5,28
	.set	macro
	.set	reorder

	li	$2,39			# 0x27
	li	$3,39			# 0x27
$L359:
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
$L357:
	addu	$2,$5,$3
	li	$4,3			# 0x3
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	sb	$0,28($2)
	.set	macro
	.set	reorder

	lui	$4,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($4)
	lw	$2,16($sp)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L340
	sw	$3,%lo(filter_rule_num)($4)
	.set	macro
	.set	reorder

	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L376
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L340
$L370:
	lb	$3,1($5)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L352
	move	$4,$6
	.set	macro
	.set	reorder

	move	$2,$6
$L353:
	addu	$4,$21,$4
	addiu	$2,$2,1
	sb	$3,0($4)
	addu	$3,$21,$2
	lb	$3,1($3)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L353
	move	$4,$2
	.set	macro
	.set	reorder

$L352:
	addu	$4,$21,$4
	.set	noreorder
	.set	nomacro
	j	$L351
	sb	$0,0($4)
	.set	macro
	.set	reorder

$L371:
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$5,28($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L357
	move	$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L359
	addiu	$4,$5,28
	.set	macro
	.set	reorder

$L361:
	.set	noreorder
	.set	nomacro
	j	$L336
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
	sw	$0,%lo(url_filter_flag)($19)
	move	$22,$4
	move	$21,$5
	beq	$6,$0,$L395
	move	$20,$7

	lw	$2,28($6)
	beq	$2,$0,$L378
	li	$2,-1			# 0xffffffffffffffff

	lw	$2,24($6)
	sltu	$2,$2,2
	bne	$2,$0,$L379
	addiu	$18,$6,8

	lw	$16,8($6)
	beq	$18,$16,$L409
	lui	$2,%hi(url_filter_mode)

	lw	$2,16($16)
	andi	$3,$2,0x1
	bne	$3,$0,$L410
	andi	$3,$2,0x200

$L404:
	lw	$3,20($16)
	beq	$21,$3,$L410
	andi	$3,$2,0x200

$L381:
	lw	$2,0($16)
	beq	$18,$2,$L403
	nop

	move	$16,$2
	lw	$2,16($16)
	andi	$3,$2,0x1
	beq	$3,$0,$L404
	andi	$3,$2,0x200

$L410:
	beq	$3,$0,$L382
	nop

	andi	$2,$2,0x2
	lw	$6,0($20)
	beq	$2,$0,$L381
	nop

	beq	$6,$0,$L395
	move	$4,$22

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L381
	nop

$L398:
	li	$2,1			# 0x1
$L378:
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

$L382:
	andi	$3,$2,0x100
	beq	$3,$0,$L405
	andi	$3,$2,0x2

$L408:
	beq	$3,$0,$L406
	andi	$3,$2,0x1

	lw	$6,0($20)
	beq	$6,$0,$L395
	move	$4,$22

	lw	$5,20($16)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L381
	nop

	lw	$2,16($16)
	andi	$3,$2,0x1
	beq	$3,$0,$L385
	lui	$3,%hi(url_filter_mode)

	lw	$3,%lo(url_filter_mode)($3)
	beq	$3,$0,$L402
	andi	$2,$2,0x8

	sltu	$2,$0,$2
$L407:
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

$L405:
	lw	$2,28($17)
	move	$4,$22
	jalr	$2
	addiu	$5,$16,28

	beq	$2,$0,$L381
	nop

	lw	$2,16($16)
	j	$L408
	andi	$3,$2,0x2

$L385:
	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
$L402:
	lw	$31,44($sp)
	sltu	$2,$2,1
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,48

$L379:
	lui	$2,%hi(url_filter_mode)
$L409:
	li	$3,-1			# 0xffffffffffffffff
	lw	$4,%lo(url_filter_mode)($2)
	li	$2,1			# 0x1
	j	$L378
	movz	$2,$3,$4

$L403:
$L388 = .
	beq	$16,$0,$L409
	lui	$2,%hi(url_filter_mode)

	lw	$2,%lo(url_filter_flag)($19)
	bne	$2,$0,$L378
	li	$2,1			# 0x1

	lw	$2,16($16)
	andi	$3,$2,0x1
	beq	$3,$0,$L399
	nop

	andi	$2,$2,0x8
	j	$L407
	sltu	$2,$0,$2

$L399:
	j	$L378
	move	$2,$0

$L395:
	j	$L378
	li	$2,-1			# 0xffffffffffffffff

$L406:
	beq	$3,$0,$L398
	andi	$2,$2,0x8

	j	$L378
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
	move	$21,$5
	move	$16,$4
	jal	do_gettimeofday
	addiu	$4,$sp,136

	jal	rtl_ip_hdr
	move	$4,$21

	move	$4,$21
	jal	rtl_eth_hdr
	move	$22,$2

	li	$3,2			# 0x2
	beq	$16,$3,$L414
	nop

	slt	$2,$16,3
	beq	$2,$0,$L417
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	beq	$16,$2,$L420
	lui	$4,%hi($LC31)

$L421:
	jal	printk
	addiu	$4,$4,%lo($LC31)

$L418:
	lw	$31,180($sp)
	move	$2,$0
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	j	$31
	addiu	$sp,$sp,184

$L417:
	beq	$16,$2,$L415
	li	$2,4			# 0x4

	bne	$16,$2,$L421
	lui	$4,%hi($LC31)

	beq	$22,$0,$L418
	nop

	lw	$5,12($22)
	lui	$4,%hi($LC30)
	lw	$2,16($22)
	addiu	$4,$4,%lo($LC30)
	srl	$6,$5,16
	srl	$7,$5,8
	j	$L419
	srl	$8,$2,16

$L415:
	beq	$22,$0,$L418
	lui	$3,%hi(log_info)

	lw	$5,12($22)
	lw	$2,16($22)
	lui	$4,%hi($LC29)
	addiu	$3,$3,%lo(log_info)
	srl	$6,$5,16
	srl	$7,$5,8
	addiu	$4,$4,%lo($LC29)
	srl	$8,$2,16
	sw	$3,36($sp)
$L419:
	srl	$3,$2,8
	andi	$10,$5,0xff
	srl	$9,$2,24
	sw	$10,16($sp)
	andi	$8,$8,0xff
	sw	$9,20($sp)
	andi	$3,$3,0xff
	sw	$8,24($sp)
	andi	$2,$2,0xff
	sw	$3,28($sp)
	srl	$5,$5,24
	sw	$2,32($sp)
	andi	$6,$6,0x00ff
	jal	scrlog_printk
	andi	$7,$7,0x00ff

	move	$2,$0
	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	j	$31
	addiu	$sp,$sp,184

$L414:
	beq	$2,$0,$L418
	nop

	beq	$22,$0,$L418
	nop

	lw	$23,12($22)
	jal	rtl_eth_hdr
	move	$4,$21

	move	$4,$21
	jal	rtl_eth_hdr
	move	$20,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$19,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$18,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$17,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$16,$2

	andi	$3,$23,0xff
	lw	$10,16($22)
	sw	$3,16($sp)
	srl	$6,$23,16
	lwl	$5,84($20)
	srl	$7,$23,8
	lwl	$4,88($20)
	srl	$12,$10,16
	lwl	$3,92($20)
	srl	$11,$10,8
	lwr	$5,87($20)
	srl	$13,$10,24
	lwr	$4,91($20)
	andi	$12,$12,0xff
	lwr	$3,95($20)
	andi	$11,$11,0xff
	sw	$5,20($sp)
	andi	$10,$10,0xff
	sw	$4,24($sp)
	lui	$4,%hi($LC28)
	sw	$3,28($sp)
	srl	$5,$23,24
	lbu	$3,96($20)
	addiu	$4,$4,%lo($LC28)
	andi	$6,$6,0x00ff
	sb	$3,32($sp)
	andi	$7,$7,0x00ff
	lbu	$3,97($20)
	sb	$3,33($sp)
	lwl	$9,98($19)
	lwl	$8,102($19)
	lwl	$3,106($19)
	lwr	$8,105($19)
	lwr	$3,109($19)
	lwr	$9,101($19)
	sw	$9,36($sp)
	sw	$8,40($sp)
	sw	$3,44($sp)
	lbu	$3,110($19)
	sb	$3,48($sp)
	lbu	$3,111($19)
	sb	$3,49($sp)
	lwl	$9,112($18)
	lwl	$8,116($18)
	lwl	$3,120($18)
	lwr	$9,115($18)
	lwr	$8,119($18)
	lwr	$3,123($18)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$3,60($sp)
	lbu	$3,124($18)
	sb	$3,64($sp)
	lbu	$3,125($18)
	sb	$3,65($sp)
	lwl	$9,126($17)
	lwl	$8,130($17)
	lwl	$3,134($17)
	lwr	$8,133($17)
	lwr	$3,137($17)
	lwr	$9,129($17)
	sw	$9,68($sp)
	sw	$8,72($sp)
	sw	$3,76($sp)
	lbu	$3,138($17)
	sb	$3,80($sp)
	lbu	$3,139($17)
	sb	$3,81($sp)
	lwl	$9,140($16)
	lwl	$8,144($16)
	lwl	$3,148($16)
	lwr	$9,143($16)
	lwr	$8,147($16)
	lwr	$3,151($16)
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$3,92($sp)
	lbu	$3,152($16)
	sb	$3,96($sp)
	lbu	$3,153($16)
	sb	$3,97($sp)
	lwl	$9,154($2)
	lwl	$8,158($2)
	lwl	$3,162($2)
	lwr	$8,161($2)
	lwr	$3,165($2)
	lwr	$9,157($2)
	sw	$9,100($sp)
	sw	$8,104($sp)
	sw	$3,108($sp)
	lbu	$3,166($2)
	sb	$3,112($sp)
	lbu	$2,167($2)
	sw	$13,116($sp)
	sb	$2,113($sp)
	sw	$12,120($sp)
	sw	$11,124($sp)
	jal	scrlog_printk
	sw	$10,128($sp)

	move	$2,$0
	lw	$31,180($sp)
	lw	$23,176($sp)
	lw	$22,172($sp)
	lw	$21,168($sp)
	lw	$20,164($sp)
	lw	$19,160($sp)
	lw	$18,156($sp)
	lw	$17,152($sp)
	lw	$16,148($sp)
	j	$31
	addiu	$sp,$sp,184

$L420:
	beq	$22,$0,$L418
	nop

	lw	$5,12($22)
	lui	$4,%hi($LC27)
	lw	$2,16($22)
	addiu	$4,$4,%lo($LC27)
	srl	$6,$5,16
	srl	$7,$5,8
	j	$L419
	srl	$8,$2,16

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
	beq	$16,$17,$L427
	move	$18,$4

	li	$19,-1			# 0xffffffffffffffff
	j	$L429
	li	$20,1			# 0x1

$L432:
	beq	$2,$20,$L431
	lui	$2,%hi(log_enable)

	lw	$16,0($16)
	beq	$16,$17,$L423
	move	$2,$0

$L429:
	move	$6,$16
	move	$5,$0
	move	$7,$16
	jal	scan_table
	move	$4,$18

	bne	$2,$19,$L432
	nop

$L427:
	move	$2,$0
$L423:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L431:
	lw	$2,%lo(log_enable)($2)
	bne	$2,$0,$L433
	nop

	lw	$31,36($sp)
	li	$2,1			# 0x1
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L433:
	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$18

	j	$L423
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

	bne	$2,$0,$L436
	nop

	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L436:
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

	lui	$3,%hi(res12)
	lw	$31,28($sp)
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

	move	$2,$0
	lw	$31,20($sp)
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
	beq	$2,$0,$L440
	lui	$4,%hi($LC32)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC32)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res12)($16)
$L440:
	lw	$31,20($sp)
	move	$2,$0
	lw	$16,16($sp)
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
	beq	$3,$0,$L445
	nop

	addiu	$2,$3,-48
	sltu	$2,$2,10
	beq	$2,$0,$L446
	addiu	$4,$4,1

	j	$L443
	move	$2,$0

$L444:
	beq	$6,$0,$L448
	addiu	$4,$4,1

$L443:
	lb	$5,0($4)
	sll	$6,$2,1
	sll	$2,$2,3
	addiu	$7,$3,-48
	addu	$2,$6,$2
	addiu	$6,$5,-48
	move	$3,$5
	addu	$2,$2,$7
	bne	$5,$0,$L444
	sltu	$6,$6,10

$L448:
	j	$31
	nop

$L446:
	move	$2,$0
	j	$31
	nop

$L445:
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
	beq	$4,$0,$L455
	move	$17,$5

	move	$16,$4
	lui	$18,%hi(___strtok)
	move	$4,$16
$L459:
	jal	strspn
	move	$5,$17

	addu	$16,$16,$2
	lb	$2,0($16)
	beq	$2,$0,$L456
	move	$4,$16

	jal	strpbrk
	move	$5,$17

	beq	$2,$0,$L453
	nop

	lb	$3,0($2)
	bne	$3,$0,$L457
	nop

$L453:
	sw	$2,%lo(___strtok)($18)
	move	$2,$16
$L458:
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L457:
	sb	$0,0($2)
	j	$L453
	addiu	$2,$2,1

$L456:
	move	$16,$0
	move	$2,$16
	sw	$0,%lo(___strtok)($18)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L455:
	lui	$18,%hi(___strtok)
	lw	$16,%lo(___strtok)($18)
	bne	$16,$0,$L459
	move	$4,$16

	j	$L458
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
	bne	$3,$0,$L487
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L487
	or	$3,$5,$6
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	or	$2,$3,$2
	lw	$3,24($28)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L487
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
	bne	$6,$0,$L487
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
	lui	$20,%hi($L482)
	.set	macro
	.set	reorder

	li	$17,1			# 0x1
	addiu	$20,$20,%lo($L482)
	lui	$fp,%hi(Ipv6_smurf_threshold)
	lui	$23,%hi(dos_item_ipv6)
	lui	$22,%hi(dos_item_global_0)
	lui	$21,%hi(block_time)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L461
	move	$18,$2
	.set	macro
	.set	reorder

$L483:
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
	beq	$3,$0,$L463
	sll	$3,$17,2
	.set	macro
	.set	reorder

	addu	$3,$20,$3
	lw	$3,0($3)
	j	$3
	.section	.rodata.dos_single_write,"a",@progbits
	.align	2
	.align	2
$L482:
	.word	$L463
	.word	$L464
	.word	$L465
	.word	$L466
	.word	$L467
	.word	$L468
	.word	$L469
	.word	$L470
	.word	$L471
	.word	$L472
	.word	$L473
	.word	$L474
	.word	$L475
	.word	$L476
	.word	$L477
	.word	$L478
	.word	$L479
	.word	$L480
	.word	$L481
	.section	.text.dos_single_write
$L464:
	lui	$3,%hi(dos_op_mode)
	sb	$2,%lo(dos_op_mode)($3)
$L463:
	addiu	$17,$17,1
	move	$4,$0
	andi	$17,$17,0x00ff
$L488:
	.set	noreorder
	.set	nomacro
	jal	strtok
	addiu	$5,$19,%lo($LC19)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L483
	move	$18,$2
	.set	macro
	.set	reorder

$L461:
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

$L481:
	lui	$3,%hi(Ipv6_ns_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(Ipv6_ns_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L480:
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(Ipv6_icmp_too_big_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L479:
	addiu	$17,$17,1
	move	$4,$0
	sw	$2,%lo(Ipv6_smurf_threshold)($fp)
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L478:
	addiu	$17,$17,1
	move	$4,$0
	sw	$2,%lo(dos_item_ipv6)($23)
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L477:
	addiu	$17,$17,1
	move	$4,$0
	sw	$2,%lo(dos_item_global_0)($22)
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L476:
	addiu	$17,$17,1
	move	$4,$0
	sw	$2,%lo(block_time)($21)
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L475:
	lui	$3,%hi(per_icmp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_icmp_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L474:
	lui	$3,%hi(per_udp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_udp_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L473:
	lui	$3,%hi(per_fin_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_fin_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L472:
	lui	$3,%hi(per_syn_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(per_syn_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L471:
	lui	$3,%hi(whole_icmp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_icmp_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L470:
	lui	$3,%hi(whole_udp_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_udp_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L469:
	lui	$3,%hi(whole_fin_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_fin_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L468:
	lui	$3,%hi(whole_syn_threshold)
	addiu	$17,$17,1
	sw	$2,%lo(whole_syn_threshold)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L467:
	lui	$3,%hi(dos_item_global)
	addiu	$17,$17,1
	sw	$2,%lo(dos_item_global)($3)
	lui	$2,%hi(dos_item_backup)
	sw	$0,%lo(dos_item_backup)($2)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L466:
	move	$4,$18
	move	$5,$0
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	addiu	$17,$17,1
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_mask)
	sw	$2,%lo(dos_lan_mask)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L465:
	move	$4,$18
	move	$5,$0
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	addiu	$17,$17,1
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_addr)
	sw	$2,%lo(dos_lan_addr)($3)
	move	$4,$0
	.set	noreorder
	.set	nomacro
	j	$L488
	andi	$17,$17,0x00ff
	.set	macro
	.set	reorder

$L487:
	.set	noreorder
	.set	nomacro
	j	$L461
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
	beq	$2,$0,$L493
	nop

	xor	$2,$4,$2
	j	$31
	sltu	$2,$0,$2

$L493:
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
	beq	$4,$0,$L494
	nop

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
$L494:
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

	beq	$16,$0,$L497
	nop

	jal	strlen
	move	$4,$16

	li	$5,208			# 0xd0
	jal	__kmalloc
	addiu	$4,$2,1

	lui	$3,%hi(block_message)
	beq	$2,$0,$L496
	sw	$2,%lo(block_message)($3)

	move	$6,$16
	lui	$5,%hi($LC15)
	lw	$31,20($sp)
	move	$4,$2
	lw	$16,16($sp)
	addiu	$5,$5,%lo($LC15)
	j	sprintf
	addiu	$sp,$sp,24

$L496:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L497:
	lui	$16,%hi($LC2)
	jal	strlen
	addiu	$4,$16,%lo($LC2)

	li	$5,208			# 0xd0
	jal	__kmalloc
	move	$4,$2

	lui	$3,%hi(block_message)
	beq	$2,$0,$L496
	sw	$2,%lo(block_message)($3)

	addiu	$6,$16,%lo($LC2)
	lui	$5,%hi($LC15)
	lw	$31,20($sp)
	move	$4,$2
	lw	$16,16($sp)
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
	beq	$4,$0,$L501
	lui	$2,%hi(fast_path_cusRsp401_func)

	sw	$4,%lo(fast_path_cusRsp401_func)($2)
$L502:
	beq	$5,$0,$L503
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($2)
$L504:
	beq	$6,$0,$L505
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L505:
	lui	$3,%hi(GenerateTCPENDACK)
	addiu	$3,$3,%lo(GenerateTCPENDACK)
	j	$31
	sw	$3,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L503:
	lui	$3,%hi(GenerateTCPFINACK)
	addiu	$3,$3,%lo(GenerateTCPFINACK)
	j	$L504
	sw	$3,%lo(fast_path_cusRspTCPFinAck_func)($2)

$L501:
	lui	$3,%hi(GenerateHTTP401)
	addiu	$3,$3,%lo(GenerateHTTP401)
	j	$L502
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
	bne	$2,$0,$L509
	move	$16,$4

	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	bne	$2,$0,$L509
	lui	$2,%hi(table_list_head+24)

	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L530
	lui	$3,%hi(none_dos_drop_pkt_cnt)

$L509:
	jal	rtl_get_skb_type
	move	$4,$16

	li	$3,2			# 0x2
	beq	$2,$3,$L531
	lui	$2,%hi(dos_item_ipv6)

	jal	rtl_get_skb_type
	move	$4,$16

	beq	$2,$0,$L532
	nop

$L527:
	li	$2,4			# 0x4
$L510:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L531:
	lw	$2,%lo(dos_item_ipv6)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L527
	nop

	jal	filter_dos
	move	$4,$16

	li	$3,4			# 0x4
	movn	$3,$0,$2
	j	$L510
	move	$2,$3

$L532:
	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,2048			# 0x800
	beq	$2,$3,$L512
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	beq	$2,$0,$L510
	li	$2,4			# 0x4

	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,34525			# 0x86dd
	bne	$2,$3,$L510
	li	$2,4			# 0x4

$L512:
	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L513
	nop

	jal	FilterWeb_v2
	move	$4,$16

	beq	$2,$0,$L514
	lui	$2,%hi(fast_path_cusRsp401_func)

	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	beq	$2,$0,$L526
	nop

$L528:
	jalr	$2
	move	$4,$16

	j	$L510
	move	$2,$0

$L530:
	li	$2,1			# 0x1
	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$4,$4,1
	j	$L510
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L513:
	lw	$2,%lo(dos_item_global)($17)
	andi	$2,$2,0x1
	bne	$2,$0,$L516
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L517
	nop

$L516:
	jal	filter_dos
	move	$4,$16

	bne	$2,$0,$L510
	move	$2,$0

$L517:
	jal	rtl_ip_hdr
	move	$4,$16

	lbu	$3,9($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L527
	lui	$3,%hi(none_dos_drop_pkt_cnt)

	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$4,$4,1
	j	$L510
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L514:
	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	beq	$2,$0,$L513
	lui	$3,%hi(url_cache)

	li	$2,1			# 0x1
	addiu	$3,$3,%lo(url_cache)
	lbu	$4,15($3)
	beq	$4,$2,$L533
	nop

	lbu	$3,16($3)
	bne	$3,$2,$L513
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
	bne	$2,$0,$L528
	nop

$L526:
	j	$L510
	move	$2,$0

$L533:
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	bne	$2,$0,$L528
	nop

	j	$L510
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
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
	jal	dos_func_init
	lui	$16,%hi(dos_timer)

	lui	$4,%hi($LC33)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(dos_proc_fops)
	addiu	$4,$4,%lo($LC33)
	move	$5,$0
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(dos_proc_fops)
	lui	$2,%hi(url_count)
	sw	$0,16($sp)
	jal	proc_create_data
	sb	$0,%lo(url_count)($2)

	move	$6,$0
	move	$7,$0
	addiu	$4,$16,%lo(dos_timer)
	lui	$3,%hi(res3)
	move	$5,$0
	sw	$2,%lo(res3)($3)
	jal	init_timer_key
	addiu	$16,$16,%lo(dos_timer)

	lui	$3,%hi(jiffies)
	lw	$2,%lo(jiffies)($3)
	move	$4,$16
	lw	$5,%lo(jiffies)($3)
	addiu	$2,$2,100
	sw	$2,8($16)
	lui	$2,%hi(dos_timer_fn)
	addiu	$5,$5,100
	addiu	$2,$2,%lo(dos_timer_fn)
	sw	$0,20($16)
	jal	mod_timer
	sw	$2,16($16)

	jal	filter_init_fastpath
	nop

	jal	igmp_delete_init_netlink
	nop

	move	$2,$0
	lw	$31,28($sp)
	lw	$16,24($sp)
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
	beq	$5,$0,$L536
	nop

	lui	$4,%hi($LC33)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC33)

	sw	$0,%lo(res3)($16)
$L536:
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
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
