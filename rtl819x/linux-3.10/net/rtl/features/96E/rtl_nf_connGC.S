	.file	1 "rtl_nf_connGC.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.conntrack_dointvec,"ax",@progbits
	.align	2
	.globl	conntrack_dointvec
	.set	nomips16
	.ent	conntrack_dointvec
	.type	conntrack_dointvec, @function
conntrack_dointvec:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lw	$2,56($sp)
	move	$16,$5
	jal	proc_dointvec
	sw	$2,16($sp)

	bne	$2,$0,$L2
	move	$17,$2

	beq	$16,$0,$L2
	lui	$2,%hi(nf_conntrack_max)

	lui	$3,%hi(prot_limit)
	lui	$9,%hi(_prot_limit)
	li	$8,1374355456			# 0x51eb0000
	lw	$4,%lo(nf_conntrack_max)($2)
	addiu	$3,$3,%lo(prot_limit)
	move	$2,$0
	addiu	$9,$9,%lo(_prot_limit)
	ori	$8,$8,0x851f
	li	$7,3			# 0x3
$L3:
	lw	$6,0($3)
	sll	$5,$2,2
	addu	$5,$9,$5
	mult	$4,$6
	addiu	$2,$2,1
	addiu	$3,$3,4
	mflo	$6
	multu	$6,$8
	mfhi	$6
	srl	$6,$6,5
	bne	$2,$7,$L3
	sw	$6,0($5)

	li	$2,-859045888			# 0xffffffffcccc0000
	sll	$3,$4,2
	ori	$2,$2,0xcccd
	multu	$3,$2
	lui	$3,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$5,$4,$2
	sltu	$5,$5,65
	bne	$5,$0,$L4
	sw	$2,%lo(rtl_nf_conntrack_threshold)($3)

	addiu	$2,$4,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($3)
$L4:
	bne	$4,$0,$L8
	lui	$2,%hi(rtl_newGC_session_status_flags)

$L9:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L2:
	move	$2,$17
	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,40

$L8:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L9
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L2
	lui	$4,%hi(jiffies)

	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	j	$L2
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	conntrack_dointvec
	.size	conntrack_dointvec, .-conntrack_dointvec
	.section	.text.conntrack_dointvec_minmax,"ax",@progbits
	.align	2
	.globl	conntrack_dointvec_minmax
	.set	nomips16
	.ent	conntrack_dointvec_minmax
	.type	conntrack_dointvec_minmax, @function
conntrack_dointvec_minmax:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lw	$2,56($sp)
	move	$16,$5
	jal	proc_dointvec_minmax
	sw	$2,16($sp)

	bne	$2,$0,$L11
	move	$17,$2

	beq	$16,$0,$L11
	lui	$2,%hi(nf_conntrack_max)

	lui	$3,%hi(prot_limit)
	lui	$9,%hi(_prot_limit)
	li	$8,1374355456			# 0x51eb0000
	lw	$4,%lo(nf_conntrack_max)($2)
	addiu	$3,$3,%lo(prot_limit)
	move	$2,$0
	addiu	$9,$9,%lo(_prot_limit)
	ori	$8,$8,0x851f
	li	$7,3			# 0x3
$L12:
	lw	$6,0($3)
	sll	$5,$2,2
	addu	$5,$9,$5
	mult	$4,$6
	addiu	$2,$2,1
	addiu	$3,$3,4
	mflo	$6
	multu	$6,$8
	mfhi	$6
	srl	$6,$6,5
	bne	$2,$7,$L12
	sw	$6,0($5)

	li	$2,-859045888			# 0xffffffffcccc0000
	sll	$3,$4,2
	ori	$2,$2,0xcccd
	multu	$3,$2
	lui	$3,%hi(rtl_nf_conntrack_threshold)
	mfhi	$2
	srl	$2,$2,2
	subu	$5,$4,$2
	sltu	$5,$5,65
	bne	$5,$0,$L13
	sw	$2,%lo(rtl_nf_conntrack_threshold)($3)

	addiu	$2,$4,-64
	sw	$2,%lo(rtl_nf_conntrack_threshold)($3)
$L13:
	bne	$4,$0,$L16
	lui	$2,%hi(rtl_newGC_session_status_flags)

$L17:
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
$L11:
	move	$2,$17
	lw	$31,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,40

$L16:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L17
	lui	$2,%hi(rtl_newGC_session_status_flags)

	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L11
	lui	$4,%hi(jiffies)

	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	j	$L11
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	conntrack_dointvec_minmax
	.size	conntrack_dointvec_minmax, .-conntrack_dointvec_minmax
	.section	.text.clean_from_lists,"ax",@progbits
	.align	2
	.globl	clean_from_lists
	.set	nomips16
	.ent	clean_from_lists
	.type	clean_from_lists, @function
clean_from_lists:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	move	$17,$5
	jal	rtl_hlist_nulls_del_rcu
	move	$5,$0

	move	$4,$16
	jal	rtl_hlist_nulls_del_rcu
	li	$5,1			# 0x1

	move	$4,$16
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L21
	li	$3,17			# 0x11

	beq	$2,$3,$L22
	li	$3,1			# 0x1

	beq	$2,$3,$L26
	nop

	move	$4,$16
$L28:
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	beq	$2,$3,$L23
	move	$4,$16

	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0

	li	$3,17			# 0x11
	beq	$2,$3,$L29
	move	$5,$17

$L24:
	move	$4,$16
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32

$L23:
	move	$5,$17
$L29:
	jal	clean_from_lists_hooks
	move	$4,$16

	jal	rtl_list_del
	move	$4,$16

	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	nop
	bne	$2,$0,$L27
	move	$4,$16

$L30:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$2,%hi(rtl_newGC_session_status_flags)
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32

$L26:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	lw	$5,%lo(prot_counters)($3)
	move	$4,$2
	addiu	$5,$5,-1
	jal	arch_local_irq_restore
	sw	$5,%lo(prot_counters)($3)

	j	$L28
	move	$4,$16

$L22:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	addiu	$3,$3,%lo(prot_counters)
	lw	$5,8($3)
	move	$4,$2
	addiu	$5,$5,-1
	jal	arch_local_irq_restore
	sw	$5,8($3)

	j	$L28
	move	$4,$16

$L21:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	addiu	$3,$3,%lo(prot_counters)
	lw	$5,4($3)
	move	$4,$2
	addiu	$5,$5,-1
	jal	arch_local_irq_restore
	sw	$5,4($3)

	j	$L28
	move	$4,$16

$L27:
	jal	rtl_gc_threshold_check
	move	$4,$17

	bne	$2,$0,$L30
	move	$4,$16

	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,3			# 0x3
	beq	$4,$3,$L24
	nop

	lui	$4,%hi(jiffies)
	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	j	$L24
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	clean_from_lists
	.size	clean_from_lists, .-clean_from_lists
	.section	.text.rtl_death_action,"ax",@progbits
	.align	2
	.globl	rtl_death_action
	.set	nomips16
	.ent	rtl_death_action
	.type	rtl_death_action, @function
rtl_death_action:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	addiu	$5,$4,100
	li	$4,9			# 0x9
	lw	$2,100($16)
	jal	__mips_set_bit
	.set	noreorder
	.set	nomacro
	jal	nf_ct_delete_from_lists
	move	$4,$16
	.set	macro
	.set	reorder

	beq	$16,$0,$L31
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	jal	arch_local_irq_save
	lw	$17,0($16)
	move	$4,$2
	addiu	$17,$17,-1
	.set	noreorder
	.set	nomacro
	jal	arch_local_irq_restore
	sw	$17,0($16)
	.set	macro
	.set	reorder

 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L33
	move	$4,$16
	.set	macro
	.set	reorder

$L31:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L33:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	nf_conntrack_destroy
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

	.end	rtl_death_action
	.size	rtl_death_action, .-rtl_death_action
	.section	.text.rtl_connGC_addList,"ax",@progbits
	.align	2
	.globl	rtl_connGC_addList
	.set	nomips16
	.ent	rtl_connGC_addList
	.type	rtl_connGC_addList, @function
rtl_connGC_addList:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	jal	rtl_new_gc_ip_hdr
	move	$17,$5

	beq	$2,$0,$L35
	nop

	jal	rtl_new_gc_get_skb_protocol
	move	$4,$16

	li	$3,6			# 0x6
	beq	$2,$3,$L37
	li	$3,17			# 0x11

	beq	$2,$3,$L38
	li	$3,1			# 0x1

	beq	$2,$3,$L42
	nop

$L35:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L38:
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$17

	andi	$2,$2,0x2
	move	$4,$17
	beq	$2,$0,$L40
	li	$5,2			# 0x2

	jal	rtl_list_add_tail
	li	$6,1			# 0x1

$L41:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	addiu	$3,$3,%lo(prot_counters)
	lw	$5,8($3)
	move	$4,$2
	addiu	$5,$5,1
	jal	arch_local_irq_restore
	sw	$5,8($3)

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L42:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	lw	$5,%lo(prot_counters)($3)
	move	$4,$2
	addiu	$5,$5,1
	jal	arch_local_irq_restore
	sw	$5,%lo(prot_counters)($3)

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L37:
	jal	rtl_new_gc_get_ct_tcp_state
	move	$4,$17

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	nop
	beq	$2,$0,$L39
	move	$4,$17

	li	$5,1			# 0x1
	jal	rtl_list_add_tail
	move	$6,$0

$L39:
	jal	arch_local_irq_save
	nop

	lui	$3,%hi(prot_counters)
	addiu	$3,$3,%lo(prot_counters)
	lw	$5,4($3)
	move	$4,$2
	addiu	$5,$5,1
	jal	arch_local_irq_restore
	sw	$5,4($3)

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L40:
	jal	rtl_list_add_tail
	move	$6,$0

	j	$L41
	nop

	.set	macro
	.set	reorder
	.end	rtl_connGC_addList
	.size	rtl_connGC_addList, .-rtl_connGC_addList
	.section	.text.__nf_ct_refresh_acct_proto,"ax",@progbits
	.align	2
	.globl	__nf_ct_refresh_acct_proto
	.set	nomips16
	.ent	__nf_ct_refresh_acct_proto
	.type	__nf_ct_refresh_acct_proto, @function
__nf_ct_refresh_acct_proto:
	.frame	$sp,56,$31		# vars= 8, regs= 6/0, args= 24, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$16,$4
	move	$18,$5
	move	$17,$6
	move	$19,$7
	lbu	$20,79($sp)
	.set	noreorder
	.set	nomacro
	jal	local_bh_disable
	sw	$0,24($sp)
	.set	macro
	.set	reorder

	li	$5,10			# 0xa
	.set	noreorder
	.set	nomacro
	jal	rtl_test_bit
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L51
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_test_bit
	li	$5,3			# 0x3
	.set	macro
	.set	reorder

	li	$3,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L52
	move	$4,$16
	.set	macro
	.set	reorder

	jal	rtl_del_ct_timer
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L53
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

$L47:
	lw	$7,72($sp)
	addiu	$2,$sp,24
	move	$4,$16
	move	$5,$18
	move	$6,$17
	.set	noreorder
	.set	nomacro
	jal	__nf_ct_refresh_acct_proto_hooks
	sw	$2,16($sp)
	.set	macro
	.set	reorder

$L51:
	jal	local_bh_enable
	lw	$31,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L52:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	move	$5,$19
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	j	$L47
	sw	$2,24($sp)
	.set	macro
	.set	reorder

$L53:
	lw	$5,%lo(jiffies)($2)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	addu	$5,$19,$5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_add_ct_timer
	move	$4,$16
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	sw	$2,24($sp)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L48
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$20,$2,$L47
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$16
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	move	$4,$16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L50
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	j	$L47
$L48:
	lw	$6,84($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	j	$L47
$L50:
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	move	$6,$0
	.set	macro
	.set	reorder

	j	$L47
	.end	__nf_ct_refresh_acct_proto
	.size	__nf_ct_refresh_acct_proto, .-__nf_ct_refresh_acct_proto
	.section	.text.__conntrack_drop_check,"ax",@progbits
	.align	2
	.globl	__conntrack_drop_check
	.set	nomips16
	.ent	__conntrack_drop_check
	.type	__conntrack_drop_check, @function
__conntrack_drop_check:
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
	move	$5,$0
	jal	rtl_new_gc_get_ct_protonum
	move	$16,$4

	move	$4,$16
	move	$5,$0
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$6,$0

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$17,$2

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$18,$2

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_new_gc_get_ct_port_by_dir
	move	$19,$2

	move	$4,$16
	move	$5,$0
	move	$6,$0
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$20,$2

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$22,$2

	move	$4,$16
	li	$5,1			# 0x1
	move	$6,$0
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$21,$2

	move	$4,$16
	li	$5,1			# 0x1
	li	$6,1			# 0x1
	jal	rtl_new_gc_get_ct_ip_by_dir
	move	$23,$2

	li	$4,-268435456			# 0xfffffffff0000000
	and	$5,$21,$4
	li	$3,-536870912			# 0xffffffffe0000000
	beq	$5,$3,$L66
	and	$4,$2,$4

	beq	$4,$3,$L66
	nop

	beq	$22,$2,$L68
	nop

$L56:
	sltu	$2,$17,1024
	bne	$2,$0,$L55
	li	$2,-1			# 0xffffffffffffffff

	sltu	$2,$18,1024
	bne	$2,$0,$L55
	li	$2,-1			# 0xffffffffffffffff

	sltu	$2,$19,1024
	bne	$2,$0,$L55
	li	$2,-1			# 0xffffffffffffffff

	sltu	$2,$20,1024
	bne	$2,$0,$L55
	li	$2,-1			# 0xffffffffffffffff

	li	$2,8080			# 0x1f90
	beq	$17,$2,$L66
	nop

	beq	$18,$2,$L66
	nop

	beq	$19,$2,$L66
	xori	$20,$20,0x1f90

	li	$2,-1			# 0xffffffffffffffff
	movn	$2,$0,$20
$L55:
	lw	$31,52($sp)
	lw	$23,48($sp)
	lw	$22,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,56

$L68:
	bne	$21,$23,$L56
	nop

$L66:
	j	$L55
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	__conntrack_drop_check
	.size	__conntrack_drop_check, .-__conntrack_drop_check
	.section	.text.drop_one_conntrack,"ax",@progbits
	.align	2
	.globl	drop_one_conntrack
	.set	nomips16
	.ent	drop_one_conntrack
	.type	drop_one_conntrack, @function
drop_one_conntrack:
	.frame	$sp,64,$31		# vars= 0, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
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
	lui	$23,%hi(prot_counters)
	lui	$2,%hi(_prot_limit)
	addiu	$2,$2,%lo(_prot_limit)
	addiu	$23,$23,%lo(prot_counters)
	lw	$3,4($2)
	lw	$2,4($23)
	#nop
	slt	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L140
	lui	$fp,%hi(drop_priority_max_idx)
	.set	macro
	.set	reorder

	lui	$2,%hi(_prot_limit)
$L169:
	addiu	$2,$2,%lo(_prot_limit)
	lw	$3,8($2)
	lw	$2,8($23)
	#nop
	slt	$2,$3,$2
	beq	$2,$0,$L79
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	#nop
	blez	$2,$L80
	lui	$21,%hi(drop_priority)
	lui	$23,%hi(Udp_State_Hash_Head)
	addiu	$21,$21,%lo(drop_priority)
	move	$22,$0
	addiu	$23,$23,%lo(Udp_State_Hash_Head)
	li	$20,-1			# 0xffffffffffffffff
$L86:
	lbu	$2,0($21)
	#nop
	sltu	$3,$2,11
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L81
	addiu	$2,$2,-11
	.set	macro
	.set	reorder

	sll	$2,$2,3
	addu	$2,$23,$2
	lw	$19,4($2)
	#nop
	lw	$17,0($19)
	#nop
	beq	$19,$17,$L81
	lb	$2,8($17)
	lw	$16,0($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$20,$L148
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L82:
	beq	$2,$0,$L149
$L83:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L81
	move	$17,$16
	.set	macro
	.set	reorder

	move	$16,$2
	lb	$2,8($17)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L82
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L148:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L83
	sb	$2,8($17)
	.set	macro
	.set	reorder

$L149:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	beq	$2,$0,$L83
$L144:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(rtl_newGC_session_status_flags)($2)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L134
	lui	$3,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$3,%lo(jiffies)($3)
	.set	noreorder
	.set	nomacro
	j	$L164
	li	$4,3			# 0x3
	.set	macro
	.set	reorder

$L81:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L86
	addiu	$21,$21,4
	.set	macro
	.set	reorder

$L80:
	jal	local_bh_enable
$L79:
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	#nop
	blez	$2,$L87
	lui	$20,%hi(drop_priority)
	addiu	$20,$20,%lo(drop_priority)
	move	$23,$0
	lui	$22,%hi(jiffies)
$L104:
	lbu	$2,0($20)
	#nop
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L88
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	#nop
	lw	$21,0($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L96
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	lw	$16,0($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L150
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L90:
	beq	$2,$0,$L151
$L91:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L96
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L90
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L150:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L91
	sb	$2,8($21)
	.set	macro
	.set	reorder

$L151:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,7
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L152
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	slt	$2,$17,129
$L165:
	bne	$2,$0,$L91
$L96:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L104
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L87:
	jal	local_bh_enable
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	#nop
	.set	noreorder
	.set	nomacro
	blez	$2,$L106
	lui	$20,%hi(drop_priority)
	.set	macro
	.set	reorder

	addiu	$20,$20,%lo(drop_priority)
	move	$23,$0
	lui	$22,%hi(jiffies)
$L124:
	lbu	$2,0($20)
	#nop
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L108
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	#nop
	lw	$21,0($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L116
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	lw	$16,0($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L153
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L110:
	beq	$2,$0,$L154
$L111:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L116
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L110
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L153:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L111
	sb	$2,8($21)
	.set	macro
	.set	reorder

$L154:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,9
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L155
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	slt	$2,$17,129
$L167:
	bne	$2,$0,$L111
$L116:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L124
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L106:
	jal	local_bh_enable
	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L142
	lui	$4,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,200
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
$L142:
	.set	noreorder
	.set	nomacro
	j	$L76
	move	$2,$0
	.set	macro
	.set	reorder

$L152:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L165
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,3			# 0x3
	move	$4,$18
	move	$5,$23
	move	$6,$0
	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L141
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L107
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$18
	move	$6,$0
	li	$7,1			# 0x1
	li	$2,3			# 0x3
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L165
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L88:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	#nop
	lw	$21,0($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L96
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	lw	$16,0($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L156
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L98:
	beq	$2,$0,$L157
$L99:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L96
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L98
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L156:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L99
	sb	$2,8($21)
	.set	macro
	.set	reorder

$L157:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,7
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L158
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	slt	$2,$17,129
$L166:
	bne	$2,$0,$L99
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L104
	addiu	$20,$20,4
	.set	macro
	.set	reorder

	j	$L87
$L158:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L166
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,12			# 0xc
	move	$4,$18
	move	$5,$23
	move	$6,$0
	li	$7,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L141
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L107
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$18
	move	$6,$0
	li	$7,1			# 0x1
	li	$2,12			# 0xc
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L141:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$18
	.set	macro
	.set	reorder

$L107:
	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(rtl_newGC_session_status_flags)($2)
	#nop
	bne	$3,$0,$L134
	lw	$3,%lo(jiffies)($21)
	li	$4,3			# 0x3
$L164:
	addiu	$3,$3,1
	sw	$4,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
	.set	noreorder
	.set	nomacro
	j	$L76
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L134:
	li	$2,1			# 0x1
$L76:
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
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L155:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L167
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,3			# 0x3
	move	$4,$18
	move	$5,$23
	li	$6,2			# 0x2
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L141
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L107
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$18
	li	$6,2			# 0x2
	move	$7,$0
	li	$2,3			# 0x3
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L167
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L108:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	#nop
	lw	$21,0($19)
	#nop
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L116
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	lw	$16,0($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L159
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L118:
	beq	$2,$0,$L160
$L119:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L116
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L118
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L159:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L119
	sb	$2,8($21)
	.set	macro
	.set	reorder

$L160:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,9
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L161
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	slt	$2,$17,129
$L168:
	bne	$2,$0,$L119
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L124
	addiu	$20,$20,4
	.set	macro
	.set	reorder

	j	$L106
$L161:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L168
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,12			# 0xc
	move	$4,$18
	move	$5,$23
	li	$6,2			# 0x2
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks1
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L141
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L107
	move	$5,$23
	.set	macro
	.set	reorder

	move	$4,$18
	li	$6,2			# 0x2
	move	$7,$0
	li	$2,12			# 0xc
	.set	noreorder
	.set	nomacro
	jal	__drop_one_conntrack_process_hooks2
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L168
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L140:
	jal	local_bh_disable
	lui	$fp,%hi(drop_priority_max_idx)
	lw	$2,%lo(drop_priority_max_idx)($fp)
	#nop
	blez	$2,$L71
	lui	$21,%hi(drop_priority)
	addiu	$21,$21,%lo(drop_priority)
	move	$22,$0
	li	$20,-1			# 0xffffffffffffffff
$L78:
	lbu	$2,0($21)
	#nop
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L72
	lui	$3,%hi(Tcp_State_Hash_Head)
	.set	macro
	.set	reorder

	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	#nop
	lw	$17,0($19)
	#nop
	beq	$19,$17,$L72
	lb	$2,8($17)
	lw	$16,0($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$20,$L162
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L73:
	beq	$2,$0,$L163
$L74:
	lw	$2,0($16)
	beq	$19,$16,$L72
$L128:
	move	$17,$16
	move	$16,$2
	lb	$2,8($17)
	#nop
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L73
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L162:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L74
	sb	$2,8($17)
	.set	macro
	.set	reorder

$L163:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	bne	$2,$0,$L144
	lw	$2,0($16)
	bne	$19,$16,$L128
$L72:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L78
	addiu	$21,$21,4
	.set	macro
	.set	reorder

$L71:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	j	$L169
	lui	$2,%hi(_prot_limit)
	.set	macro
	.set	reorder

	.end	drop_one_conntrack
	.size	drop_one_conntrack, .-drop_one_conntrack
	.section	.text.rtl_nf_conn_GC_init,"ax",@progbits
	.align	2
	.globl	rtl_nf_conn_GC_init
	.set	nomips16
	.ent	rtl_nf_conn_GC_init
	.type	rtl_nf_conn_GC_init, @function
rtl_nf_conn_GC_init:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$6,%hi(Tcp_State_Hash_Head)
	move	$4,$0
	move	$3,$0
	addiu	$6,$6,%lo(Tcp_State_Hash_Head)
	li	$5,11			# 0xb
$L172:
	sll	$4,$4,3
	addu	$4,$6,$4
	lw	$2,4($4)
	addiu	$3,$3,1
	beq	$2,$0,$L171
	move	$4,$3

	sw	$2,0($2)
	sw	$2,4($2)
$L171:
	bne	$3,$5,$L172
	nop

	lui	$3,%hi(Udp_State_Hash_Head)
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	lw	$2,4($3)
	nop
	beq	$2,$0,$L173
	nop

	sw	$2,0($2)
	sw	$2,4($2)
$L173:
	lw	$2,12($3)
	nop
	beq	$2,$0,$L180
	lui	$16,%hi(prot_limit)

	sw	$2,0($2)
	sw	$2,4($2)
$L180:
	addiu	$4,$16,%lo(prot_limit)
	move	$5,$0
	jal	memset
	li	$6,12			# 0xc

	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	li	$8,90			# 0x5a
	li	$7,60			# 0x3c
	mult	$2,$8
	addiu	$6,$16,%lo(prot_limit)
	sw	$8,4($6)
	sw	$7,8($6)
	li	$6,-859045888			# 0xffffffffcccc0000
	ori	$6,$6,0xcccd
	sll	$15,$2,2
	li	$3,1374355456			# 0x51eb0000
	ori	$3,$3,0x851f
	sll	$14,$2,1
	lui	$11,%hi(rtl_nf_conntrack_threshold)
	lui	$10,%hi(prot_counters)
	addiu	$9,$10,%lo(prot_counters)
	lui	$5,%hi(_prot_limit)
	sw	$0,%lo(prot_counters)($10)
	sw	$0,4($9)
	sw	$0,8($9)
	mflo	$13
	lui	$9,%hi(drop_priority_max_idx)
	li	$10,10			# 0xa
	mult	$2,$7
	addiu	$4,$5,%lo(_prot_limit)
	lui	$17,%hi(rtl_newGC_session_status_flags)
	sw	$10,%lo(drop_priority_max_idx)($9)
	li	$9,2			# 0x2
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
	sw	$9,%lo(prot_limit)($16)
	mflo	$12
	multu	$15,$6
	mfhi	$6
	srl	$7,$6,2
	subu	$6,$2,$7
	multu	$14,$3
	sw	$7,%lo(rtl_nf_conntrack_threshold)($11)
	sltu	$6,$6,65
	mfhi	$8
	srl	$8,$8,5
	sw	$8,%lo(_prot_limit)($5)
	multu	$13,$3
	mfhi	$7
	srl	$7,$7,5
	sw	$7,4($4)
	multu	$12,$3
	mfhi	$3
	srl	$3,$3,5
	bne	$6,$0,$L175
	sw	$3,8($4)

	addiu	$3,$2,-64
	sw	$3,%lo(rtl_nf_conntrack_threshold)($11)
$L175:
	bne	$2,$0,$L179
	nop

$L176:
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
$L177:
	jal	rtl_nf_conn_GC_init_hooks
	nop

	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L179:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L176
	li	$2,3			# 0x3

	lw	$3,%lo(rtl_newGC_session_status_flags)($17)
	nop
	beq	$3,$2,$L177
	lui	$3,%hi(jiffies)

	lw	$3,%lo(jiffies)($3)
	lui	$4,%hi(rtl_newGC_session_status_flags)
	sw	$2,%lo(rtl_newGC_session_status_flags)($4)
	addiu	$3,$3,1
	lui	$2,%hi(rtl_newGC_session_status_time)
	j	$L177
	sw	$3,%lo(rtl_newGC_session_status_time)($2)

	.set	macro
	.set	reorder
	.end	rtl_nf_conn_GC_init
	.size	rtl_nf_conn_GC_init, .-rtl_nf_conn_GC_init
	.globl	prot_limit
	.section	.bss.prot_limit,"aw",@nobits
	.align	2
	.type	prot_limit, @object
	.size	prot_limit, 12
prot_limit:
	.space	12
	.globl	drop_priority_max_idx
	.section	.bss.drop_priority_max_idx,"aw",@nobits
	.align	2
	.type	drop_priority_max_idx, @object
	.size	drop_priority_max_idx, 4
drop_priority_max_idx:
	.space	4
	.globl	drop_priority
	.section	.data.drop_priority,"aw",@progbits
	.align	2
	.type	drop_priority, @object
	.size	drop_priority, 40
drop_priority:
	.byte	8
	.space	1
	.half	60
	.byte	6
	.space	1
	.half	30
	.byte	5
	.space	1
	.half	60
	.byte	7
	.space	1
	.half	120
	.byte	4
	.space	1
	.half	120
	.byte	11
	.space	1
	.half	85
	.byte	1
	.space	1
	.half	110
	.byte	2
	.space	1
	.half	30
	.byte	12
	.space	1
	.half	10
	.byte	3
	.space	1
	.half	120
	.globl	Udp_State_Hash_Head
	.section	.data.Udp_State_Hash_Head,"aw",@progbits
	.align	2
	.type	Udp_State_Hash_Head, @object
	.size	Udp_State_Hash_Head, 16
Udp_State_Hash_Head:
	.byte	11
	.space	3
	.word	udp_unreply_list
	.byte	12
	.space	3
	.word	udp_assured_list
	.globl	Tcp_State_Hash_Head
	.section	.data.Tcp_State_Hash_Head,"aw",@progbits
	.align	2
	.type	Tcp_State_Hash_Head, @object
	.size	Tcp_State_Hash_Head, 88
Tcp_State_Hash_Head:
	.word	0
	.word	0
	.word	1
	.word	syn_sent_list
	.word	2
	.word	syn_recv_list
	.word	3
	.word	established_list
	.word	4
	.word	fin_wait_list
	.word	5
	.word	close_wait_list
	.word	6
	.word	last_ack_list
	.word	7
	.word	time_wait_list
	.word	8
	.word	close_list
	.word	9
	.word	listen_list
	.word	10
	.word	0
	.globl	rtl_newGC_session_status_time
	.section	.dram-fwd,"aw",@progbits
	.align	2
	.type	rtl_newGC_session_status_time, @object
	.size	rtl_newGC_session_status_time, 4
rtl_newGC_session_status_time:
	.space	4
	.globl	rtl_newGC_session_status_flags
	.align	2
	.type	rtl_newGC_session_status_flags, @object
	.size	rtl_newGC_session_status_flags, 4
rtl_newGC_session_status_flags:
	.space	4
	.globl	conntrack_max
	.section	.data.conntrack_max,"aw",@progbits
	.align	2
	.type	conntrack_max, @object
	.size	conntrack_max, 4
conntrack_max:
	.word	100
	.globl	conntrack_min
	.section	.bss.conntrack_min,"aw",@nobits
	.align	2
	.type	conntrack_min, @object
	.size	conntrack_min, 4
conntrack_min:
	.space	4
	.section	.bss._prot_limit,"aw",@nobits
	.align	2
	.type	_prot_limit, @object
	.size	_prot_limit, 12
_prot_limit:
	.space	12
	.section	.bss.prot_counters,"aw",@nobits
	.align	2
	.type	prot_counters, @object
	.size	prot_counters, 12
prot_counters:
	.space	12
	.section	.data.udp_unreply_list,"aw",@progbits
	.align	2
	.type	udp_unreply_list, @object
	.size	udp_unreply_list, 8
udp_unreply_list:
	.word	udp_unreply_list
	.word	udp_unreply_list
	.section	.data.udp_assured_list,"aw",@progbits
	.align	2
	.type	udp_assured_list, @object
	.size	udp_assured_list, 8
udp_assured_list:
	.word	udp_assured_list
	.word	udp_assured_list
	.section	.data.syn_sent_list,"aw",@progbits
	.align	2
	.type	syn_sent_list, @object
	.size	syn_sent_list, 8
syn_sent_list:
	.word	syn_sent_list
	.word	syn_sent_list
	.section	.data.syn_recv_list,"aw",@progbits
	.align	2
	.type	syn_recv_list, @object
	.size	syn_recv_list, 8
syn_recv_list:
	.word	syn_recv_list
	.word	syn_recv_list
	.section	.data.established_list,"aw",@progbits
	.align	2
	.type	established_list, @object
	.size	established_list, 8
established_list:
	.word	established_list
	.word	established_list
	.section	.data.fin_wait_list,"aw",@progbits
	.align	2
	.type	fin_wait_list, @object
	.size	fin_wait_list, 8
fin_wait_list:
	.word	fin_wait_list
	.word	fin_wait_list
	.section	.data.close_wait_list,"aw",@progbits
	.align	2
	.type	close_wait_list, @object
	.size	close_wait_list, 8
close_wait_list:
	.word	close_wait_list
	.word	close_wait_list
	.section	.data.last_ack_list,"aw",@progbits
	.align	2
	.type	last_ack_list, @object
	.size	last_ack_list, 8
last_ack_list:
	.word	last_ack_list
	.word	last_ack_list
	.section	.data.time_wait_list,"aw",@progbits
	.align	2
	.type	time_wait_list, @object
	.size	time_wait_list, 8
time_wait_list:
	.word	time_wait_list
	.word	time_wait_list
	.section	.data.close_list,"aw",@progbits
	.align	2
	.type	close_list, @object
	.size	close_list, 8
close_list:
	.word	close_list
	.word	close_list
	.section	.data.listen_list,"aw",@progbits
	.align	2
	.type	listen_list, @object
	.size	listen_list, 8
listen_list:
	.word	listen_list
	.word	listen_list
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2080) 4.6.4"
