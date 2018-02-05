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
	move	$16,$5
	lw	$2,56($sp)
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
	addiu	$2,$2,1
	mult	$4,$6
	addiu	$3,$3,4
	mflo	$6
	multu	$6,$8
	mfhi	$6
	srl	$6,$6,5
	bne	$2,$7,$L3
	sw	$6,0($5)

	li	$3,-859045888			# 0xffffffffcccc0000
	sll	$5,$4,2
	ori	$3,$3,0xcccd
	lui	$2,%hi(rtl_nf_conntrack_threshold)
	multu	$5,$3
	mfhi	$3
	srl	$3,$3,2
	subu	$5,$4,$3
	sltu	$5,$5,65
	bne	$5,$0,$L4
	sw	$3,%lo(rtl_nf_conntrack_threshold)($2)

	addiu	$3,$4,-64
	sw	$3,%lo(rtl_nf_conntrack_threshold)($2)
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

	li	$3,3			# 0x3
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
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
	move	$16,$5
	lw	$2,56($sp)
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
	addiu	$2,$2,1
	mult	$4,$6
	addiu	$3,$3,4
	mflo	$6
	multu	$6,$8
	mfhi	$6
	srl	$6,$6,5
	bne	$2,$7,$L12
	sw	$6,0($5)

	li	$3,-859045888			# 0xffffffffcccc0000
	sll	$5,$4,2
	ori	$3,$3,0xcccd
	lui	$2,%hi(rtl_nf_conntrack_threshold)
	multu	$5,$3
	mfhi	$3
	srl	$3,$3,2
	subu	$5,$4,$3
	sltu	$5,$5,65
	bne	$5,$0,$L13
	sw	$3,%lo(rtl_nf_conntrack_threshold)($2)

	addiu	$3,$4,-64
	sw	$3,%lo(rtl_nf_conntrack_threshold)($2)
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

	li	$3,3			# 0x3
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
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
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	move	$17,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_hlist_nulls_del_rcu
	move	$5,$0
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_hlist_nulls_del_rcu
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L34
	li	$3,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L35
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L36
	lui	$2,%hi(prot_counters)
	.set	macro
	.set	reorder

	move	$4,$16
$L39:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L23
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_protonum
	move	$5,$0
	.set	macro
	.set	reorder

	li	$3,17			# 0x11
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L38
	move	$5,$17
	.set	macro
	.set	reorder

$L24:
	move	$4,$16
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L23:
	move	$5,$17
$L38:
	.set	noreorder
	.set	nomacro
	jal	clean_from_lists_hooks
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_list_del
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L37
	move	$4,$16
	.set	macro
	.set	reorder

$L40:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$16,20($sp)
	sw	$0,%lo(rtl_newGC_session_status_flags)($2)
	.set	noreorder
	.set	nomacro
	j	nf_ct_remove_expectations
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L34:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L21:
 #APP
 # 108 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 4($2)		# atomic_sub	
	subu	$3, 1				
	sc	$3, 4($2)				

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L39
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L21
$L35:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L22:
 #APP
 # 108 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 8($2)		# atomic_sub	
	subu	$3, 1				
	sc	$3, 8($2)				

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L39
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L22
$L37:
	.set	noreorder
	.set	nomacro
	jal	rtl_gc_threshold_check
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L40
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_newGC_session_status_flags)
	li	$3,3			# 0x3
	lw	$4,%lo(rtl_newGC_session_status_flags)($2)
	beq	$4,$3,$L24
	lui	$4,%hi(jiffies)
	lw	$4,%lo(jiffies)($4)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$4,1
	.set	noreorder
	.set	nomacro
	j	$L24
	sw	$3,%lo(rtl_newGC_session_status_time)($2)
	.set	macro
	.set	reorder

$L36:
$L20:
 #APP
 # 108 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, %lo(prot_counters)($2)		# atomic_sub	
	subu	$3, 1				
	sc	$3, %lo(prot_counters)($2)				

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L39
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L20
	.end	clean_from_lists
	.size	clean_from_lists, .-clean_from_lists
	.section	.text.rtl_death_action,"ax",@progbits
	.align	2
	.globl	rtl_death_action
	.set	nomips16
	.ent	rtl_death_action
	.type	rtl_death_action, @function
rtl_death_action:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	move	$16,$4
	lw	$2,100($4)
	addiu	$2,$4,100
$L42:
 #APP
 # 104 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/bitops.h" 1
		ll	$3, 0($2)		# set_bit	
	or	$3, 512				
	sc	$3, 0($2)				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L42
	.set	noreorder
	.set	nomacro
	jal	nf_ct_delete_from_lists
	move	$4,$16
	.set	macro
	.set	reorder

	beq	$16,$0,$L41
 #APP
 # 176 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
$L44:
 #APP
 # 196 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 0($16)	# atomic_sub_return	
	subu	$2, $3, 1			
	sc	$2, 0($16)				

 # 0 "" 2
 #NO_APP
	beq	$2,$0,$L44
 #APP
 # 215 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L47
$L41:
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,24
	.set	macro
	.set	reorder

$L47:
	move	$4,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	.set	noreorder
	.set	nomacro
	j	nf_conntrack_destroy
	addiu	$sp,$sp,24
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
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	move	$16,$4
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_ip_hdr
	move	$17,$5
	.set	macro
	.set	reorder

	beq	$2,$0,$L49
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_skb_protocol
	move	$4,$16
	.set	macro
	.set	reorder

	li	$3,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L51
	li	$3,17			# 0x11
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L52
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L69
	lui	$2,%hi(prot_counters)
	.set	macro
	.set	reorder

$L49:
	lw	$31,28($sp)
	move	$2,$0
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L52:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$17
	.set	macro
	.set	reorder

	move	$4,$17
	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L55
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L61:
 #APP
 # 69 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 8($2)		# atomic_add	
	addu	$3, 1				
	sc	$3, 8($2)				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L61
	lw	$31,28($sp)
	move	$2,$0
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L69:
$L50:
 #APP
 # 69 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, %lo(prot_counters)($2)		# atomic_add	
	addu	$3, 1				
	sc	$3, %lo(prot_counters)($2)				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L50
	lw	$31,28($sp)
	move	$2,$0
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L51:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_tcp_state
	move	$4,$17
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$2,4($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L66
	move	$4,$17
	.set	macro
	.set	reorder

	li	$5,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	move	$6,$0
	.set	macro
	.set	reorder

$L66:
	lui	$2,%hi(prot_counters)
	addiu	$2,$2,%lo(prot_counters)
$L60:
 #APP
 # 69 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/rlx/include/asm/atomic.h" 1
		ll	$3, 4($2)		# atomic_add	
	addu	$3, 1				
	sc	$3, 4($2)				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L60
	lw	$31,28($sp)
	move	$2,$0
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L55:
	.set	noreorder
	.set	nomacro
	jal	rtl_list_add_tail
	move	$6,$0
	.set	macro
	.set	reorder

	lui	$2,%hi(prot_counters)
	.set	noreorder
	.set	nomacro
	j	$L61
	addiu	$2,$2,%lo(prot_counters)
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
	lbu	$19,79($sp)
	move	$17,$6
	sw	$0,24($sp)
	.set	noreorder
	.set	nomacro
	jal	local_bh_disable
	move	$20,$7
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
	beq	$2,$0,$L78
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
	beq	$2,$3,$L79
	move	$4,$16
	.set	macro
	.set	reorder

	jal	rtl_del_ct_timer
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L80
	lui	$2,%hi(jiffies)
	.set	macro
	.set	reorder

$L74:
	lw	$7,72($sp)
	addiu	$2,$sp,24
	move	$4,$16
	sw	$2,16($sp)
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	__nf_ct_refresh_acct_proto_hooks
	move	$6,$17
	.set	macro
	.set	reorder

$L78:
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

$L79:
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	move	$5,$20
	.set	macro
	.set	reorder

	li	$2,2048			# 0x800
	.set	noreorder
	.set	nomacro
	j	$L74
	sw	$2,24($sp)
	.set	macro
	.set	reorder

$L80:
	move	$4,$16
	lw	$5,%lo(jiffies)($2)
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_set_ct_timeout_expires
	addu	$5,$20,$5
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
	beq	$19,$2,$L75
	li	$2,17			# 0x11
	.set	macro
	.set	reorder

	bne	$19,$2,$L74
	.set	noreorder
	.set	nomacro
	jal	rtl_new_gc_get_ct_udp_status
	move	$4,$16
	.set	macro
	.set	reorder

	move	$4,$16
	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L77
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	j	$L74
$L75:
	lw	$6,84($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	j	$L74
$L77:
	.set	noreorder
	.set	nomacro
	jal	rtl_list_move_tail
	move	$6,$0
	.set	macro
	.set	reorder

	j	$L74
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
	li	$3,-536870912			# 0xffffffffe0000000
	and	$5,$21,$4
	beq	$5,$3,$L93
	and	$4,$2,$4

	beq	$4,$3,$L93
	nop

	beq	$22,$2,$L95
	sltu	$2,$17,1024

$L96:
	bne	$2,$0,$L93
	sltu	$2,$18,1024

	bne	$2,$0,$L93
	sltu	$2,$19,1024

	bne	$2,$0,$L93
	sltu	$2,$20,1024

	bne	$2,$0,$L93
	li	$2,8080			# 0x1f90

	beq	$17,$2,$L93
	nop

	beq	$18,$2,$L93
	nop

	beq	$19,$2,$L93
	xori	$20,$20,0x1f90

	li	$2,-1			# 0xffffffffffffffff
	movn	$2,$0,$20
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

$L95:
	bne	$21,$23,$L96
	nop

$L93:
	lw	$31,52($sp)
	li	$2,-1			# 0xffffffffffffffff
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
	addiu	$23,$23,%lo(prot_counters)
	addiu	$2,$2,%lo(_prot_limit)
	lw	$3,4($2)
	lw	$2,4($23)
	slt	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L168
	lui	$fp,%hi(drop_priority_max_idx)
	.set	macro
	.set	reorder

	lui	$2,%hi(_prot_limit)
$L196:
	addiu	$2,$2,%lo(_prot_limit)
	lw	$3,8($2)
	lw	$2,8($23)
	slt	$2,$3,$2
	beq	$2,$0,$L107
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	blez	$2,$L108
	lui	$21,%hi(drop_priority)
	lui	$23,%hi(Udp_State_Hash_Head)
	addiu	$21,$21,%lo(drop_priority)
	move	$22,$0
	addiu	$23,$23,%lo(Udp_State_Hash_Head)
	li	$20,-1			# 0xffffffffffffffff
$L114:
	lbu	$2,0($21)
	sltu	$3,$2,11
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L109
	addiu	$2,$2,-11
	.set	macro
	.set	reorder

	sll	$2,$2,3
	addu	$2,$23,$2
	lw	$19,4($2)
	lw	$17,0($19)
	beq	$19,$17,$L109
	lb	$2,8($17)
	lw	$16,0($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$20,$L174
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L110:
	beq	$2,$0,$L175
$L111:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L109
	move	$17,$16
	.set	macro
	.set	reorder

	move	$16,$2
	lb	$2,8($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L110
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L174:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L111
	sb	$2,8($17)
	.set	macro
	.set	reorder

$L175:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	beq	$2,$0,$L111
$L170:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$2,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(rtl_newGC_session_status_flags)($2)
	bne	$3,$0,$L162
	lui	$3,%hi(jiffies)
	lw	$4,%lo(jiffies)($3)
	li	$3,3			# 0x3
	lw	$31,60($sp)
	lw	$fp,56($sp)
	addiu	$4,$4,1
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	sw	$3,%lo(rtl_newGC_session_status_flags)($2)
	li	$2,1			# 0x1
	lui	$3,%hi(rtl_newGC_session_status_time)
	sw	$4,%lo(rtl_newGC_session_status_time)($3)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L109:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L114
	addiu	$21,$21,4
	.set	macro
	.set	reorder

$L108:
	jal	local_bh_enable
$L107:
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	.set	noreorder
	.set	nomacro
	blez	$2,$L115
	lui	$20,%hi(drop_priority)
	.set	macro
	.set	reorder

	move	$23,$0
	addiu	$20,$20,%lo(drop_priority)
	lui	$22,%hi(jiffies)
$L132:
	lbu	$2,0($20)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L116
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	lw	$21,0($19)
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L124
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	addiu	$18,$21,-140
	lw	$16,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L176
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L118:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L197
	move	$4,$18
	.set	macro
	.set	reorder

$L119:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L124
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

$L176:
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

	move	$4,$18
$L197:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,7
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L178
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L192:
	bne	$2,$0,$L119
$L124:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L132
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L115:
	jal	local_bh_enable
	jal	local_bh_disable
	lw	$2,%lo(drop_priority_max_idx)($fp)
	.set	noreorder
	.set	nomacro
	blez	$2,$L134
	lui	$20,%hi(drop_priority)
	.set	macro
	.set	reorder

	move	$23,$0
	addiu	$20,$20,%lo(drop_priority)
	lui	$22,%hi(jiffies)
$L152:
	lbu	$2,0($20)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L136
	lui	$3,%hi(Udp_State_Hash_Head)
	.set	macro
	.set	reorder

	lui	$3,%hi(Tcp_State_Hash_Head)
	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	lw	$21,0($19)
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L144
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	addiu	$18,$21,-140
	lw	$16,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L179
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L138:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L198
	move	$4,$18
	.set	macro
	.set	reorder

$L139:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L144
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L138
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L179:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L139
	sb	$2,8($21)
	.set	macro
	.set	reorder

	move	$4,$18
$L198:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,9
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L181
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L194:
	bne	$2,$0,$L139
$L144:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L152
	addiu	$20,$20,4
	.set	macro
	.set	reorder

$L134:
	jal	local_bh_enable
	lui	$3,%hi(rtl_newGC_session_status_flags)
	lw	$2,%lo(rtl_newGC_session_status_flags)($3)
	li	$4,1			# 0x1
	beq	$2,$4,$L182
	lui	$2,%hi(jiffies)
	lw	$5,%lo(jiffies)($2)
	move	$2,$0
	sw	$4,%lo(rtl_newGC_session_status_flags)($3)
	lui	$3,%hi(rtl_newGC_session_status_time)
	addiu	$4,$5,200
	.set	noreorder
	.set	nomacro
	j	$L104
	sw	$4,%lo(rtl_newGC_session_status_time)($3)
	.set	macro
	.set	reorder

$L178:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L192
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,3			# 0x3
	move	$4,$18
	move	$6,$0
	move	$5,$23
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
	beq	$2,$3,$L169
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L135
	move	$6,$0
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$23
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
	j	$L192
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L116:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	lw	$21,0($19)
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L124
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	addiu	$18,$21,-140
	lw	$16,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L183
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L126:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L199
	move	$4,$18
	.set	macro
	.set	reorder

$L127:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L124
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L126
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L183:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L127
	sb	$2,8($21)
	.set	macro
	.set	reorder

	move	$4,$18
$L199:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,7
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L185
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L193:
	bne	$2,$0,$L127
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L132
	addiu	$20,$20,4
	.set	macro
	.set	reorder

	j	$L115
$L185:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L193
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,12			# 0xc
	move	$4,$18
	move	$6,$0
	move	$5,$23
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
	beq	$2,$3,$L169
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L135
	move	$6,$0
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$23
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
	j	$L193
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L169:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	jal	rtl_death_action
	move	$4,$18
	.set	macro
	.set	reorder

$L135:
	lui	$3,%hi(rtl_newGC_session_status_flags)
	lw	$2,%lo(rtl_newGC_session_status_flags)($3)
	beq	$2,$0,$L186
$L162:
	li	$2,1			# 0x1
$L104:
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

$L186:
	lw	$4,%lo(jiffies)($21)
	li	$5,3			# 0x3
	lw	$31,60($sp)
	li	$2,1			# 0x1
	lw	$fp,56($sp)
	addiu	$4,$4,1
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	sw	$5,%lo(rtl_newGC_session_status_flags)($3)
	lui	$3,%hi(rtl_newGC_session_status_time)
	sw	$4,%lo(rtl_newGC_session_status_time)($3)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,64
	.set	macro
	.set	reorder

$L181:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L194
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,3			# 0x3
	move	$4,$18
	li	$6,2			# 0x2
	move	$5,$23
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
	beq	$2,$3,$L169
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L135
	li	$6,2			# 0x2
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$23
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
	j	$L194
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L136:
	addiu	$2,$2,-11
	sll	$2,$2,3
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	lw	$21,0($19)
	.set	noreorder
	.set	nomacro
	beq	$19,$21,$L144
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lb	$2,8($21)
	move	$17,$0
	addiu	$18,$21,-140
	lw	$16,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L187
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L146:
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L200
	move	$4,$18
	.set	macro
	.set	reorder

$L147:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L144
	li	$3,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	move	$21,$16
	move	$16,$2
	lb	$2,8($21)
	addiu	$18,$21,-140
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L146
	addiu	$17,$17,1
	.set	macro
	.set	reorder

$L187:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L147
	sb	$2,8($21)
	.set	macro
	.set	reorder

	move	$4,$18
$L200:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_ct_timer_expires
	lui	$21,%hi(jiffies)
	.set	macro
	.set	reorder

	lw	$4,%lo(jiffies)($22)
	lhu	$3,2($20)
	subu	$4,$2,$4
	srl	$4,$4,9
	sltu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L189
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L195:
	bne	$2,$0,$L147
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L152
	addiu	$20,$20,4
	.set	macro
	.set	reorder

	j	$L134
$L189:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L195
	slt	$2,$17,129
	.set	macro
	.set	reorder

	li	$2,12			# 0xc
	move	$4,$18
	li	$6,2			# 0x2
	move	$5,$23
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
	beq	$2,$3,$L169
	li	$3,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L135
	li	$6,2			# 0x2
	.set	macro
	.set	reorder

	move	$4,$18
	move	$5,$23
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
	j	$L195
	slt	$2,$17,129
	.set	macro
	.set	reorder

$L168:
	jal	local_bh_disable
	lui	$fp,%hi(drop_priority_max_idx)
	lw	$2,%lo(drop_priority_max_idx)($fp)
	blez	$2,$L99
	lui	$21,%hi(drop_priority)
	move	$22,$0
	addiu	$21,$21,%lo(drop_priority)
	li	$20,-1			# 0xffffffffffffffff
$L106:
	lbu	$2,0($21)
	sltu	$3,$2,10
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L100
	lui	$3,%hi(Tcp_State_Hash_Head)
	.set	macro
	.set	reorder

	sll	$2,$2,3
	addiu	$3,$3,%lo(Tcp_State_Hash_Head)
	addu	$2,$3,$2
	lw	$19,4($2)
	lw	$17,0($19)
	beq	$19,$17,$L100
	lb	$2,8($17)
	lw	$16,0($17)
	.set	noreorder
	.set	nomacro
	beq	$2,$20,$L190
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L101:
	beq	$2,$0,$L191
$L102:
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	beq	$19,$16,$L100
	move	$17,$16
	.set	macro
	.set	reorder

$L156:
	move	$16,$2
	lb	$2,8($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$20,$L101
	addiu	$18,$17,-140
	.set	macro
	.set	reorder

$L190:
	.set	noreorder
	.set	nomacro
	jal	__conntrack_drop_check
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L102
	sb	$2,8($17)
	.set	macro
	.set	reorder

$L191:
	.set	noreorder
	.set	nomacro
	jal	rtl_del_ct_timer
	move	$4,$18
	.set	macro
	.set	reorder

	bne	$2,$0,$L170
	lw	$2,0($16)
	.set	noreorder
	.set	nomacro
	bne	$19,$16,$L156
	move	$17,$16
	.set	macro
	.set	reorder

$L100:
	lw	$2,%lo(drop_priority_max_idx)($fp)
	addiu	$22,$22,1
	slt	$2,$22,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L106
	addiu	$21,$21,4
	.set	macro
	.set	reorder

$L99:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	j	$L196
	lui	$2,%hi(_prot_limit)
	.set	macro
	.set	reorder

$L182:
	.set	noreorder
	.set	nomacro
	j	$L104
	move	$2,$0
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
$L203:
	sll	$4,$4,3
	addiu	$3,$3,1
	addu	$2,$6,$4
	lw	$2,4($2)
	beq	$2,$0,$L202
	move	$4,$3

	sw	$2,0($2)
	sw	$2,4($2)
$L202:
	bne	$3,$5,$L203
	nop

	lui	$3,%hi(Udp_State_Hash_Head)
	addiu	$3,$3,%lo(Udp_State_Hash_Head)
	lw	$2,4($3)
	beq	$2,$0,$L204
	nop

	sw	$2,0($2)
	sw	$2,4($2)
$L204:
	lw	$2,12($3)
	beq	$2,$0,$L211
	lui	$16,%hi(prot_limit)

	sw	$2,0($2)
	sw	$2,4($2)
$L211:
	move	$5,$0
	addiu	$4,$16,%lo(prot_limit)
	jal	memset
	li	$6,12			# 0xc

	lui	$2,%hi(nf_conntrack_max)
	lw	$2,%lo(nf_conntrack_max)($2)
	li	$9,90			# 0x5a
	li	$8,60			# 0x3c
	li	$4,-859045888			# 0xffffffffcccc0000
	mult	$2,$9
	sll	$5,$2,2
	ori	$4,$4,0xcccd
	li	$3,1374355456			# 0x51eb0000
	ori	$3,$3,0x851f
	lui	$11,%hi(prot_counters)
	sll	$13,$2,1
	addiu	$10,$11,%lo(prot_counters)
	mflo	$12
	lui	$6,%hi(_prot_limit)
	mult	$2,$8
	sw	$0,%lo(prot_counters)($11)
	sw	$0,4($10)
	sw	$0,8($10)
	lui	$10,%hi(drop_priority_max_idx)
	addiu	$7,$16,%lo(prot_limit)
	lui	$17,%hi(rtl_newGC_session_status_flags)
	mflo	$14
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
	multu	$5,$4
	addiu	$5,$6,%lo(_prot_limit)
	sw	$9,4($7)
	sw	$8,8($7)
	mfhi	$4
	multu	$13,$3
	srl	$4,$4,2
	subu	$15,$2,$4
	sltu	$11,$15,65
	li	$15,10			# 0xa
	sw	$15,%lo(drop_priority_max_idx)($10)
	li	$10,2			# 0x2
	mfhi	$13
	sw	$10,%lo(prot_limit)($16)
	multu	$12,$3
	srl	$13,$13,5
	sw	$13,%lo(_prot_limit)($6)
	mfhi	$12
	multu	$14,$3
	srl	$12,$12,5
	lui	$14,%hi(rtl_nf_conntrack_threshold)
	sw	$12,4($5)
	sw	$4,%lo(rtl_nf_conntrack_threshold)($14)
	mfhi	$3
	srl	$3,$3,5
	bne	$11,$0,$L206
	sw	$3,8($5)

	addiu	$3,$2,-64
	sw	$3,%lo(rtl_nf_conntrack_threshold)($14)
$L206:
	bne	$2,$0,$L210
	nop

$L207:
	sw	$0,%lo(rtl_newGC_session_status_flags)($17)
$L208:
	jal	rtl_nf_conn_GC_init_hooks
	nop

	move	$2,$0
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L210:
	jal	rtl_gc_threshold_check
	move	$4,$0

	bne	$2,$0,$L207
	li	$2,3			# 0x3

	lw	$3,%lo(rtl_newGC_session_status_flags)($17)
	beq	$3,$2,$L208
	lui	$3,%hi(jiffies)

	lui	$4,%hi(rtl_newGC_session_status_flags)
	lw	$3,%lo(jiffies)($3)
	sw	$2,%lo(rtl_newGC_session_status_flags)($4)
	lui	$2,%hi(rtl_newGC_session_status_time)
	addiu	$3,$3,1
	j	$L208
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
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
