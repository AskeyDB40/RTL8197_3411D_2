	.file	1 "filter.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
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
	beq	$3,$0,$L13
	move	$2,$0

$L9:
	addiu	$6,$3,-97
	addiu	$5,$3,-48
	sltu	$7,$6,6
	sltu	$6,$5,10
	addiu	$4,$4,1
	bne	$6,$0,$L4
	sll	$2,$2,4

	addiu	$5,$3,-65
	sltu	$6,$5,6
	bne	$7,$0,$L4
	addiu	$5,$3,-87

	beq	$6,$0,$L12
	addiu	$5,$3,-55

$L4:
	lbu	$3,0($4)
	bne	$3,$0,$L9
	addu	$2,$5,$2

$L13:
	j	$31
	nop

$L12:
	j	$31
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	str2hexnum
	.size	str2hexnum, .-str2hexnum
	.align	2
	.globl	scan_table
	.set	nomips16
	.ent	scan_table
	.type	scan_table, @function
scan_table:
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
	lui	$18,%hi(url_filter_flag)
	move	$16,$6
	move	$21,$4
	move	$20,$5
	move	$19,$7
	bne	$6,$0,$L34
	sw	$0,%lo(url_filter_flag)($18)

$L15:
	li	$2,-1			# 0xffffffffffffffff
$L23:
	lw	$31,52($sp)
$L39:
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

$L34:
	lw	$2,28($6)
	beq	$2,$0,$L23
	li	$2,-1			# 0xffffffffffffffff

	lw	$2,24($6)
	sltu	$2,$2,2
	bne	$2,$0,$L16
	addiu	$22,$6,8

	lw	$17,8($6)
	beq	$22,$17,$L37
	lui	$2,%hi(url_filter_mode)

$L31:
	lw	$2,16($17)
	andi	$3,$2,0x1
	bne	$3,$0,$L17
	move	$23,$17

	lw	$3,20($17)
	beq	$20,$3,$L38
	andi	$3,$2,0x200

$L18:
	lw	$17,0($17)
	bne	$22,$17,$L31
	nop

$L27 = .
	beq	$23,$0,$L16
	lw	$2,%lo(url_filter_flag)($18)

	bne	$2,$0,$L20
	nop

	lw	$3,16($23)
	andi	$2,$3,0x1
	ext	$4,$3,3,1
	movz	$4,$0,$2
	j	$L23
	move	$2,$4

$L17:
	andi	$3,$2,0x200
$L38:
	beq	$3,$0,$L19
	nop

	andi	$2,$2,0x2
	beq	$2,$0,$L18
	lw	$6,0($19)

	beq	$6,$0,$L15
	move	$4,$21

	lw	$5,20($17)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L18
	nop

$L20:
	j	$L23
	li	$2,1			# 0x1

$L19:
	andi	$3,$2,0x100
	beq	$3,$0,$L35
	move	$4,$21

$L21:
	andi	$3,$2,0x2
	beq	$3,$0,$L36
	andi	$3,$2,0x1

	lw	$6,0($19)
	beq	$6,$0,$L15
	move	$4,$21

	lw	$5,20($17)
	jal	scan_table
	move	$7,$6

	beq	$2,$0,$L18
	nop

	lw	$3,16($17)
	andi	$2,$3,0x1
	beq	$2,$0,$L24
	nop

	lui	$2,%hi(url_filter_mode)
	lw	$4,%lo(url_filter_mode)($2)
	bne	$4,$0,$L23
	ext	$2,$3,3,1

	xori	$2,$3,0x8
	j	$L23
	ext	$2,$2,3,1

$L35:
	lw	$2,28($16)
	jalr	$2
	addiu	$5,$17,28

	beq	$2,$0,$L18
	nop

	j	$L21
	lw	$2,16($17)

$L24:
	lui	$2,%hi(url_filter_mode)
	lw	$2,%lo(url_filter_mode)($2)
	j	$L23
	sltu	$2,$2,1

$L16:
	lui	$2,%hi(url_filter_mode)
$L37:
	lw	$2,%lo(url_filter_mode)($2)
	beq	$2,$0,$L15
	li	$2,1			# 0x1

	j	$L39
	lw	$31,52($sp)

$L36:
	beq	$3,$0,$L20
	nop

	xori	$2,$2,0x8
	j	$L23
	ext	$2,$2,3,1

	.set	macro
	.set	reorder
	.end	scan_table
	.size	scan_table, .-scan_table
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
	beq	$3,$0,$L41
	addiu	$5,$3,-48

	sltu	$5,$5,10
	bne	$5,$0,$L42
	move	$2,$0

	j	$L46
	nop

$L44:
	beq	$7,$0,$L46
	nop

$L42:
	addiu	$4,$4,1
	lb	$5,0($4)
	sll	$6,$2,3
	sll	$2,$2,1
	addu	$2,$2,$6
	addiu	$7,$5,-48
	addiu	$6,$3,-48
	sltu	$7,$7,10
	move	$3,$5
	bne	$5,$0,$L44
	addu	$2,$6,$2

	j	$31
	nop

$L41:
	move	$2,$0
$L46:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	strtoint
	.size	strtoint, .-strtoint
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
	beq	$2,$0,$L51
	lw	$31,20($sp)

	jalr	$2
	nop

	lw	$31,20($sp)
$L51:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	dos_read_proc
	.size	dos_read_proc, .-dos_read_proc
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
	lw	$3,0($2)
	beq	$3,$0,$L60
	li	$2,1			# 0x1

	xor	$2,$4,$3
	sltu	$2,$0,$2
$L60:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	filter_checkConnect
	.size	filter_checkConnect, .-filter_checkConnect
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
	
	beq	$4,$0,$L62
	lui	$2,%hi(fast_path_cusRsp401_func)

	sw	$4,%lo(fast_path_cusRsp401_func)($2)
$L63:
	beq	$5,$0,$L64
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)

	sw	$5,%lo(fast_path_cusRspTCPFinAck_func)($2)
$L65:
	beq	$6,$0,$L66
	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)

	j	$31
	sw	$6,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L66:
	lui	$3,%hi(GenerateTCPENDACK)
	addiu	$3,$3,%lo(GenerateTCPENDACK)
	j	$31
	sw	$3,%lo(fast_path_cusRspTCPEndAck_func)($2)

$L64:
	lui	$3,%hi(GenerateTCPFINACK)
	addiu	$3,$3,%lo(GenerateTCPFINACK)
	j	$L65
	sw	$3,%lo(fast_path_cusRspTCPFinAck_func)($2)

$L62:
	lui	$3,%hi(GenerateHTTP401)
	addiu	$3,$3,%lo(GenerateHTTP401)
	j	$L63
	sw	$3,%lo(fast_path_cusRsp401_func)($2)

	.set	macro
	.set	reorder
	.end	register_customRspHook
	.size	register_customRspHook, .-register_customRspHook
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
	addiu	$5,$5,%lo(dos_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	dos_single_open
	.size	dos_single_open, .-dos_single_open
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
	addiu	$5,$5,%lo(filter_table_read_proc)
	move	$6,$0
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	filter_table_single_open
	.size	filter_table_single_open, .-filter_table_single_open
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
	li	$4,1			# 0x1
	sw	$4,24($2)
	sw	$3,12($2)
	sw	$2,4($2)
	sw	$3,8($2)
	sw	$0,16($2)
	sw	$0,20($2)
	sw	$0,28($2)
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
	seb	$17,$5
	jal	strlen
	move	$16,$4

	addiu	$3,$2,-1
	bltz	$3,$L78
	addu	$4,$16,$3

	lb	$5,0($4)
	bne	$5,$17,$L78
	addu	$16,$16,$2

	j	$L79
	move	$4,$0

$L81:
	lb	$2,-2($16)
	bne	$2,$5,$L84
	addiu	$16,$16,-1

$L79:
	addiu	$3,$3,-1
	bgez	$3,$L81
	addiu	$4,$4,1

$L84:
	srl	$3,$4,31
	addu	$2,$4,$3
	andi	$2,$2,0x1
	subu	$2,$2,$3
$L80:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L78:
	j	$L80
	move	$2,$0

	.set	macro
	.set	reorder
	.end	check_esc_end
	.size	check_esc_end, .-check_esc_end
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
	beq	$4,$0,$L88
	lw	$31,20($sp)

	jal	kfree
	nop

	sw	$0,%lo(block_message)($16)
	lw	$31,20($sp)
$L88:
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	unregister_customRspStr
	.size	unregister_customRspStr, .-unregister_customRspStr
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
	bne	$2,$0,$L90
	move	$18,$5

	jal	filter_table_head_init
	nop

$L90:
	lui	$2,%hi(kmalloc_caches+28)
	lw	$4,%lo(kmalloc_caches+28)($2)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	move	$3,$2
	li	$4,1			# 0x1
	addiu	$2,$2,8
	sw	$2,12($3)
	sw	$3,0($3)
	sw	$3,4($3)
	sw	$2,8($3)
	sw	$19,16($3)
	sw	$4,24($3)
	sw	$18,28($3)
	sw	$0,20($3)
	lw	$4,%lo(table_list_head)($17)
	move	$2,$0
	sw	$3,4($4)
	sw	$4,0($3)
	sw	$16,4($3)
	lw	$4,24($16)
	sw	$3,%lo(table_list_head)($17)
	lw	$31,36($sp)
	addiu	$3,$4,1
	sw	$3,24($16)
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
	ori	$3,$3,0x4507
	mult	$2,$3
	sra	$4,$2,31
	li	$7,-1840709632			# 0xffffffff92490000
	ori	$7,$7,0x2493
	lw	$5,24($sp)
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
	bne	$6,$0,$L98
	li	$3,-1851654144			# 0xffffffff91a20000

$L93:
	move	$2,$0
$L96:
	lw	$31,36($sp)
	j	$31
	addiu	$sp,$sp,40

$L98:
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
	sll	$9,$8,5
	sll	$8,$8,3
	subu	$8,$9,$8
	subu	$3,$3,$8
	sll	$8,$3,6
	sll	$3,$3,2
	subu	$3,$8,$3
	mfhi	$6
	addu	$6,$6,$2
	sra	$6,$6,5
	subu	$4,$6,$4
	sll	$6,$4,6
	sll	$4,$4,2
	subu	$4,$6,$4
	subu	$2,$2,$4
	bne	$7,$0,$L94
	addu	$3,$3,$2

	lw	$4,4($5)
	beq	$4,$0,$L96
	li	$2,1			# 0x1

	sltu	$2,$4,$3
	xori	$2,$2,0x1
$L99:
	lw	$31,36($sp)
	j	$31
	addiu	$sp,$sp,40

$L94:
	sltu	$7,$3,$7
	bne	$7,$0,$L93
	nop

	lw	$4,4($5)
	sltu	$2,$4,$3
	j	$L99
	xori	$2,$2,0x1

	.set	macro
	.set	reorder
	.end	rtl_schedule_filter_fastpath
	.size	rtl_schedule_filter_fastpath, .-rtl_schedule_filter_fastpath
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
	lw	$3,12($2)
	lw	$4,0($5)
	sltu	$4,$3,$4
	bne	$4,$0,$L102
	move	$2,$0

	lw	$2,4($5)
	sltu	$2,$2,$3
	xori	$2,$2,0x1
$L102:
	lw	$31,28($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	rtl_ip_range_filter_fastpath
	.size	rtl_ip_range_filter_fastpath, .-rtl_ip_range_filter_fastpath
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"filter_table\000"
	.text
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
	beq	$2,$0,$L105
	lui	$4,%hi($LC0)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC0)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

	sw	$0,%lo(res12)($16)
$L105:
	lw	$31,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_exit_fastpath
	.size	filter_exit_fastpath, .-filter_exit_fastpath
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"enable_dos\000"
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
	beq	$5,$0,$L108
	nop

	lui	$4,%hi($LC1)
	jal	remove_proc_entry
	addiu	$4,$4,%lo($LC1)

	sw	$0,%lo(res3)($16)
$L108:
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
	.text
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

	lui	$4,%hi($LC0)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(filter_table_proc_fops)
	addiu	$4,$4,%lo($LC0)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(filter_table_proc_fops)
	move	$5,$0
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

	lui	$4,%hi($LC1)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(dos_proc_fops)
	addiu	$4,$4,%lo($LC1)
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(dos_proc_fops)
	move	$5,$0
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
	sw	$2,16($16)
	jal	mod_timer
	sw	$0,20($16)

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
	.text
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
	beq	$2,$0,$L130
	lui	$18,%hi(dos_item_ipv6)

	lui	$19,%hi(dosFilter_block)
	lw	$4,%lo(dosFilter_block)($19)
	li	$3,1			# 0x1
	beq	$4,$3,$L139
	lui	$17,%hi(block_count)

$L134:
	beq	$2,$0,$L120
	lui	$17,%hi(block_count)

	lui	$2,%hi(dos_timer_ipv4_func)
$L136:
	lw	$2,%lo(dos_timer_ipv4_func)($2)
	beq	$2,$0,$L120
	nop

	jalr	$2
	nop

$L120:
	lw	$2,%lo(dos_item_ipv6)($18)
	beq	$2,$0,$L138
	lui	$2,%hi(block_time)

	lui	$2,%hi(dos_timer_ipv6_func)
	lw	$2,%lo(dos_timer_ipv6_func)($2)
	beq	$2,$0,$L121
	nop

	jalr	$2
	nop

$L121:
	lui	$2,%hi(block_time)
$L138:
	lw	$2,%lo(block_time)($2)
	lw	$3,%lo(block_count)($17)
	slt	$2,$3,$2
	beq	$2,$0,$L122
	nop

	lw	$2,%lo(dos_item_global)($16)
	ext	$2,$2,22,1
	beq	$2,$0,$L122
	nop

$L118:
	lui	$2,%hi(none_dos_drop_pkt_cnt)
$L137:
	lw	$3,%lo(none_dos_drop_pkt_cnt)($2)
	sltu	$4,$3,3001
	bne	$4,$0,$L123
	lui	$4,%hi(dos_item_backup)

	lw	$3,%lo(dos_item_global)($16)
	bne	$3,$0,$L133
	nop

$L124:
	sw	$0,%lo(none_dos_drop_pkt_cnt)($2)
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

$L123:
	lw	$5,%lo(dos_item_backup)($4)
	beq	$5,$0,$L125
	nop

	sw	$5,%lo(dos_item_global)($16)
	sw	$0,%lo(dos_item_backup)($4)
$L125:
	bne	$3,$0,$L124
	nop

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

$L130:
	lw	$3,%lo(dos_item_ipv6)($18)
	beq	$3,$0,$L118
	li	$3,1			# 0x1

	lui	$19,%hi(dosFilter_block)
	lw	$4,%lo(dosFilter_block)($19)
	bne	$4,$3,$L134
	nop

	lui	$17,%hi(block_count)
$L139:
	lw	$3,%lo(block_count)($17)
	addiu	$3,$3,1
	beq	$2,$0,$L120
	sw	$3,%lo(block_count)($17)

	j	$L136
	lui	$2,%hi(dos_timer_ipv4_func)

$L122:
	sw	$0,%lo(dosFilter_block)($19)
	jal	clear_attack_address
	sw	$0,%lo(block_count)($17)

	j	$L137
	lui	$2,%hi(none_dos_drop_pkt_cnt)

$L133:
	sw	$3,%lo(dos_item_backup)($4)
	lui	$3,%hi(dos_item_global)
	j	$L124
	sw	$0,%lo(dos_item_global)($3)

	.set	macro
	.set	reorder
	.end	dos_timer_fn
	.size	dos_timer_fn, .-dos_timer_fn
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

	beq	$2,$0,$L141
	li	$2,96			# 0x60

	lw	$3,0($17)
	andi	$3,$3,0xf0
	beq	$3,$2,$L147
	lui	$2,%hi(dos_item_global)

	lw	$2,%lo(dos_item_global)($2)
	beq	$2,$0,$L149
	lw	$31,28($sp)

	lui	$2,%hi(filter_dos_ipv4_handler)
	lw	$2,%lo(filter_dos_ipv4_handler)($2)
	beq	$2,$0,$L150
	lw	$17,24($sp)

$L146:
	jalr	$2
	move	$4,$16

	beq	$2,$0,$L141
	lw	$31,28($sp)

	lw	$17,24($sp)
	lw	$16,20($sp)
	li	$2,-1			# 0xffffffffffffffff
	j	$31
	addiu	$sp,$sp,32

$L147:
	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	bne	$2,$0,$L148
	lui	$2,%hi(filter_dos_ipv6_handler)

$L141:
	lw	$31,28($sp)
$L149:
	lw	$17,24($sp)
$L150:
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

$L148:
	lw	$2,%lo(filter_dos_ipv6_handler)($2)
	bne	$2,$0,$L146
	lw	$31,28($sp)

	j	$L150
	lw	$17,24($sp)

	.set	macro
	.set	reorder
	.end	filter_dos
	.size	filter_dos, .-filter_dos
	.section	.rodata.str1.4
	.align	2
$LC2:
	.ascii	"br0\000"
	.align	2
$LC3:
	.ascii	"dos_filter: alloc skb fail!\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	GenerateTCPENDACK
	.type	GenerateTCPENDACK, @function
GenerateTCPENDACK:
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
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	lw	$3,0($2)
	lui	$4,%hi(endStateClientPort)
	lhu	$4,%lo(endStateClientPort)($4)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$2,$3
	lhu	$5,0($3)
	beq	$5,$4,$L180
$L152:
	lw	$31,68($sp)
$L187:
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

$L180:
	lw	$4,4($3)
	lui	$3,%hi(endStateClientSeq)
	lw	$3,%lo(endStateClientSeq)($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L187
	lw	$31,68($sp)
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L152
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
	beq	$2,$0,$L181
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
	beq	$2,$0,$L182
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
	lw	$2,0($2)
	move	$4,$19
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
	ext	$3,$3,4,4
	addu	$2,$3,$2
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
	addiu	$21,$2,4
$L178:
	lw	$20,0($21)
	andi	$2,$20,0x1
	bne	$2,$0,$L183
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
	move	$23,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$23,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($17)
	bne	$20,$2,$L178
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
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L161:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L161
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L184
	move	$4,$19
$L188:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$17,0($18)
	lw	$2,12($18)
	lw	$3,16($18)
	andi	$17,$17,0xf
	sll	$17,$17,2
	sw	$3,12($18)
	sw	$2,16($18)
	addu	$17,$18,$17
	lw	$3,12($17)
	lhu	$2,0($17)
	lhu	$5,2($17)
	lw	$4,8($17)
	andi	$3,$3,0x100
	sh	$2,2($17)
	sh	$5,0($17)
	lw	$2,4($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L163
	sw	$4,4($17)
	.set	macro
	.set	reorder

 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	addiu	$2,$2,1
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
$L163:
	lui	$21,%hi(url_cache)
	sw	$2,8($17)
	addiu	$20,$21,%lo(url_cache)
	lhu	$2,12($20)
	sb	$0,13($17)
	move	$4,$19
	sh	$2,14($17)
	lw	$2,12($17)
	sh	$0,18($17)
	ins	$2,$0,10,1
	ori	$2,$2,0x1000
	ins	$2,$0,8,1
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,12($17)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$22,0($18)
	ext	$4,$4,4,4
	andi	$22,$22,0xf
	sll	$22,$22,2
	sll	$4,$4,2
	subu	$22,$2,$22
	sltu	$3,$4,21
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L164
	addiu	$22,$22,-14
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L168
	li	$7,8			# 0x8
	.set	macro
	.set	reorder

$L165:
	.set	noreorder
	.set	nomacro
	beq	$2,$7,$L185
	lbu	$5,1($6)
	.set	macro
	.set	reorder

	addu	$16,$16,$5
$L166:
	sltu	$2,$16,$4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L190
	move	$5,$22
	.set	macro
	.set	reorder

$L168:
	addu	$2,$17,$16
	lbu	$2,0($2)
	addu	$6,$17,$16
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L165
	addiu	$3,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L166
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L181:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L155:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L155
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L152
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L187
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L185:
	addu	$3,$17,$3
	addiu	$23,$sp,16
	addiu	$16,$16,6
	move	$5,$3
	move	$4,$23
	li	$6,4			# 0x4
	addu	$16,$17,$16
	addiu	$fp,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	move	$4,$fp
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	move	$5,$fp
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$3
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L164:
	move	$5,$22
$L190:
	andi	$22,$22,0xffff
	sh	$0,16($17)
	move	$6,$0
	move	$4,$17
	addiu	$22,$22,6
	lw	$23,16($18)
	lw	$16,12($18)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	sll	$22,$22,8
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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

	addiu	$2,$2,-14
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,2($18)
	lbu	$3,14($20)
	sh	$0,6($18)
	lw	$2,0($18)
	sb	$3,8($18)
	lhu	$4,%lo(url_cache)($21)
	sh	$0,10($18)
	lw	$3,8($18)
	addiu	$4,$4,1
	sh	$4,4($18)
	lw	$6,4($18)
	lw	$4,12($18)
	andi	$5,$2,0xf
	addu	$2,$6,$2
	sltu	$6,$2,$6
	addu	$2,$2,$3
	addu	$6,$2,$6
	sltu	$3,$6,$3
	addu	$6,$6,$4
	addu	$3,$6,$3
	sltu	$2,$3,$4
	sll	$5,$5,2
	addu	$2,$2,$3
	addu	$5,$18,$5
	addiu	$3,$18,16
$L169:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L169
	addu	$2,$4,$2
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,16($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L187
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L182:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L157:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$4, 0($2)	# atomic_sub_return	
	subu	$3, $4, 1			
	sc	$3, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L157
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$4,$2,$L186
	lui	$4,%hi($LC3)
$L189:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L187
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L184:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L188
	move	$4,$19
	.set	macro
	.set	reorder

$L186:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L189
	lui	$4,%hi($LC3)
	.set	macro
	.set	reorder

$L183:
	j	$L178
	.end	GenerateTCPENDACK
	.size	GenerateTCPENDACK, .-GenerateTCPENDACK
	.align	2
	.set	nomips16
	.ent	GenerateTCPFINACK
	.type	GenerateTCPFINACK, @function
GenerateTCPFINACK:
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
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC2)
	.set	macro
	.set	reorder

	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	rtl_ip_hdr
	move	$16,$2
	.set	macro
	.set	reorder

	lw	$3,0($2)
	lui	$4,%hi(finStateClientPort)
	lhu	$4,%lo(finStateClientPort)($4)
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$2,$3
	lhu	$5,0($3)
	beq	$5,$4,$L219
$L192:
	lw	$31,68($sp)
$L226:
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

$L219:
	lw	$4,4($3)
	lui	$3,%hi(finStateClientSeq)
	lw	$3,%lo(finStateClientSeq)($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L226
	lw	$31,68($sp)
	.set	macro
	.set	reorder

	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_neigh_lookup
	addiu	$4,$2,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L192
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
	beq	$2,$0,$L220
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
	beq	$2,$0,$L221
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
	lw	$2,0($2)
	move	$4,$19
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$18,$22
	lw	$3,12($22)
	ext	$3,$3,4,4
	addu	$2,$3,$2
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
	addiu	$21,$2,4
$L217:
	lw	$20,0($21)
	andi	$2,$20,0x1
	bne	$2,$0,$L222
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
	move	$23,$2
	.set	macro
	.set	reorder

	move	$5,$2
	addiu	$4,$23,-16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$2,4($17)
	bne	$20,$2,$L217
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
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L201:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L201
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L223
	move	$4,$19
$L227:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$5,$0
	.set	macro
	.set	reorder

	lw	$17,0($18)
	lw	$2,12($18)
	lw	$3,16($18)
	andi	$17,$17,0xf
	sll	$17,$17,2
	sw	$3,12($18)
	sw	$2,16($18)
	addu	$17,$18,$17
	lhu	$4,0($17)
	lw	$2,4($17)
	lw	$3,8($17)
	lhu	$5,2($17)
	lui	$21,%hi(url_cache)
	sh	$4,2($17)
	sw	$3,4($17)
	sh	$5,0($17)
	sw	$2,8($17)
	addiu	$20,$21,%lo(url_cache)
	lhu	$2,12($20)
	sb	$0,13($17)
	li	$3,1			# 0x1
	sh	$2,14($17)
	lw	$2,12($17)
	move	$4,$19
	sh	$0,18($17)
	ins	$2,$0,10,1
	ori	$2,$2,0x1000
	ins	$2,$3,8,1
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,12($17)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$22,0($18)
	ext	$4,$4,4,4
	andi	$22,$22,0xf
	sll	$22,$22,2
	sll	$4,$4,2
	subu	$22,$2,$22
	sltu	$3,$4,21
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L203
	addiu	$22,$22,-14
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L207
	li	$7,8			# 0x8
	.set	macro
	.set	reorder

$L204:
	.set	noreorder
	.set	nomacro
	beq	$2,$7,$L224
	lbu	$5,1($6)
	.set	macro
	.set	reorder

	addu	$16,$16,$5
$L205:
	sltu	$2,$16,$4
	beq	$2,$0,$L203
$L207:
	addu	$2,$17,$16
	lbu	$2,0($2)
	addu	$6,$17,$16
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L204
	addiu	$3,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L205
	addiu	$16,$16,1
	.set	macro
	.set	reorder

$L220:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L195:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L195
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L192
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L226
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L224:
	addu	$3,$17,$3
	addiu	$23,$sp,16
	addiu	$16,$16,6
	move	$5,$3
	move	$4,$23
	li	$6,4			# 0x4
	addu	$16,$17,$16
	addiu	$fp,$sp,20
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	move	$4,$fp
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	move	$5,$fp
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$3
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

$L203:
	lhu	$3,2($17)
	lui	$2,%hi(endStateClientPort)
	move	$5,$22
	sh	$3,%lo(endStateClientPort)($2)
	lw	$3,8($17)
	lui	$2,%hi(endStateClientSeq)
	andi	$22,$22,0xffff
	sw	$3,%lo(endStateClientSeq)($2)
	move	$6,$0
	sh	$0,16($17)
	move	$4,$17
	addiu	$22,$22,6
	lw	$23,16($18)
	lw	$16,12($18)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	sll	$22,$22,8
	.set	macro
	.set	reorder

	move	$4,$19
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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

	addiu	$2,$2,-14
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,2($18)
	lbu	$3,14($20)
	sh	$0,6($18)
	lw	$2,0($18)
	sb	$3,8($18)
	lhu	$4,%lo(url_cache)($21)
	sh	$0,10($18)
	lw	$3,8($18)
	addiu	$4,$4,1
	sh	$4,4($18)
	lw	$6,4($18)
	lw	$4,12($18)
	andi	$5,$2,0xf
	addu	$2,$6,$2
	sltu	$6,$2,$6
	addu	$2,$2,$3
	addu	$6,$2,$6
	sltu	$3,$6,$3
	addu	$6,$6,$4
	addu	$3,$6,$3
	sltu	$2,$3,$4
	sll	$5,$5,2
	addu	$2,$2,$3
	addu	$5,$18,$5
	addiu	$3,$18,16
$L208:
	lw	$4,0($3)
	addiu	$3,$3,4
	addu	$2,$2,$4
	sltu	$4,$2,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$3,$L208
	addu	$2,$4,$2
	.set	macro
	.set	reorder

 #APP
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
	sb	$2,16($20)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sb	$0,15($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L226
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L221:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L197:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$4, 0($2)	# atomic_sub_return	
	subu	$3, $4, 1			
	sc	$3, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$3,$0,$L197
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$4,$2,$L225
	lui	$4,%hi($LC3)
$L228:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L226
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L223:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L227
	move	$4,$19
	.set	macro
	.set	reorder

$L225:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L228
	lui	$4,%hi($LC3)
	.set	macro
	.set	reorder

$L222:
	j	$L217
	.end	GenerateTCPFINACK
	.size	GenerateTCPFINACK, .-GenerateTCPFINACK
	.section	.rodata.str1.4
	.align	2
$LC4:
	.ascii	"HTTP/1.1 401 The web site is blocked by administrator\015"
	.ascii	"\012Server: Embedded HTTP Server 2.00\015\012Connection:"
	.ascii	" close\015\012\015\012<HTML><HEAD><TITLE>401 The web sit"
	.ascii	"e is blocked by administrator</TITLE></HEAD>\012<BODY BG"
	.ascii	"COLOR=\"#ffffff\"><H4>401 The web site is blocked by adm"
	.ascii	"inistrator</H4></BODY></HTML>\012\000"
	.text
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
	lui	$4,%hi($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$4,%lo($LC2)
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
	beq	$2,$0,$L230
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
	beq	$2,$0,$L265
	lui	$18,%hi(block_message)
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L234
$L263:
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
	beq	$2,$0,$L266
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
	lw	$2,0($2)
	move	$4,$21
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$22,$20,$22
	lw	$3,12($22)
	ext	$3,$3,4,4
	addu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	jal	skb_trim
	sll	$5,$2,2
	.set	macro
	.set	reorder

	lw	$4,%lo(block_message)($18)
	beq	$4,$0,$L239
$L264:
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
	addiu	$fp,$2,4
$L261:
	lw	$23,0($fp)
	andi	$2,$23,0x1
	bne	$2,$0,$L267
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
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	move	$5,$2
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$3,-16
	.set	macro
	.set	reorder

	lw	$2,4($19)
	bne	$23,$2,$L261
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
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L243:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L243
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L268
	move	$5,$0
$L274:
	.set	noreorder
	.set	nomacro
	jal	rtl_set_skb_nfct
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$19,0($20)
	lw	$2,12($20)
	lw	$3,16($20)
	andi	$19,$19,0xf
	sll	$19,$19,2
	sw	$3,12($20)
	sw	$2,16($20)
	addu	$19,$20,$19
	lhu	$3,0($19)
	lhu	$2,2($19)
	lw	$16,12($19)
	sh	$3,2($19)
	sh	$2,0($19)
	lw	$5,%lo(block_message)($18)
	ext	$16,$16,4,4
	sll	$16,$16,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L245
	addu	$16,$19,$16
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

	lw	$2,8($19)
$L273:
	move	$4,$17
	lw	$16,4($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,4($19)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$3,0($20)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$16, $16			
	rotr	$16, $16, 16		

 # 0 "" 2
 #NO_APP
	ext	$4,$4,4,4
	andi	$3,$3,0xf
	addu	$3,$4,$3
	sll	$3,$3,2
	addu	$16,$2,$16
	lui	$18,%hi(url_cache)
	subu	$16,$16,$3
	addiu	$17,$18,%lo(url_cache)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$16, $16			
	rotr	$16, $16, 16		

 # 0 "" 2
 #NO_APP
	sw	$16,8($19)
	lhu	$2,12($17)
	sb	$0,13($19)
	move	$4,$21
	sh	$2,14($19)
	lw	$2,12($19)
	sh	$0,18($19)
	ins	$2,$0,10,1
	ori	$2,$2,0x1800
	ins	$2,$0,8,1
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len
	sw	$2,12($19)
	.set	macro
	.set	reorder

	lw	$4,12($22)
	lw	$22,0($20)
	ext	$4,$4,4,4
	andi	$22,$22,0xf
	sll	$22,$22,2
	sll	$4,$4,2
	subu	$22,$2,$22
	sltu	$3,$4,21
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L247
	addiu	$22,$22,-14
	.set	macro
	.set	reorder

	li	$16,20			# 0x14
	.set	noreorder
	.set	nomacro
	j	$L251
	li	$7,8			# 0x8
	.set	macro
	.set	reorder

$L248:
	.set	noreorder
	.set	nomacro
	beq	$2,$7,$L269
	lbu	$5,1($6)
	.set	macro
	.set	reorder

	addu	$16,$16,$5
	sltu	$2,$16,$4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L271
	move	$5,$22
	.set	macro
	.set	reorder

$L251:
	addu	$2,$19,$16
$L276:
	lbu	$2,0($2)
	addu	$6,$19,$16
	sltu	$5,$2,2
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L248
	addiu	$3,$16,2
	.set	macro
	.set	reorder

	addiu	$16,$16,1
	sltu	$2,$16,$4
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L276
	addu	$2,$19,$16
	.set	macro
	.set	reorder

$L247:
	move	$5,$22
$L271:
	andi	$22,$22,0xffff
	sh	$0,16($19)
	move	$6,$0
	move	$4,$19
	addiu	$22,$22,6
	lw	$23,16($20)
	lw	$16,12($20)
	.set	noreorder
	.set	nomacro
	jal	csum_partial
	sll	$22,$22,8
	.set	macro
	.set	reorder

	move	$4,$21
 #APP
 # 136 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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

	addiu	$2,$2,-14
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,2($20)
	lbu	$2,14($17)
	sh	$0,6($20)
	lw	$3,0($20)
	sb	$2,8($20)
	lhu	$4,%lo(url_cache)($18)
	sh	$0,10($20)
	lw	$2,8($20)
	addiu	$4,$4,2
	sh	$4,4($20)
	lw	$6,4($20)
	lw	$4,12($20)
	andi	$5,$3,0xf
	addu	$3,$6,$3
	sltu	$6,$3,$6
	addu	$3,$3,$2
	addu	$6,$3,$6
	sltu	$2,$6,$2
	addu	$6,$6,$4
	addu	$2,$6,$2
	sltu	$3,$2,$4
	sll	$5,$5,2
	addu	$3,$3,$2
	addu	$5,$20,$5
	addiu	$2,$20,16
$L252:
	lw	$4,0($2)
	addiu	$2,$2,4
	addu	$3,$3,$4
	sltu	$4,$3,$4
	.set	noreorder
	.set	nomacro
	bne	$5,$2,$L252
	addu	$3,$4,$3
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
 #APP
 # 78 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/checksum.h" 1
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
	sb	$2,15($17)
	lhu	$3,2($19)
	lui	$2,%hi(finStateClientPort)
	move	$4,$21
	sh	$3,%lo(finStateClientPort)($2)
	lw	$3,8($19)
	lui	$2,%hi(finStateClientSeq)
	.set	noreorder
	.set	nomacro
	jal	dev_queue_xmit
	sw	$3,%lo(finStateClientSeq)($2)
	.set	macro
	.set	reorder

$L230:
	lw	$31,68($sp)
$L272:
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

$L266:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L237:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L237
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	beq	$3,$2,$L270
	lui	$4,%hi($LC3)
$L275:
	.set	noreorder
	.set	nomacro
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC3)
	.set	macro
	.set	reorder

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

$L269:
	addu	$3,$19,$3
	addiu	$23,$sp,16
	addiu	$16,$16,6
	move	$5,$3
	move	$4,$23
	addu	$16,$19,$16
	addiu	$fp,$sp,20
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	move	$4,$fp
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	move	$5,$fp
	li	$6,4			# 0x4
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$3
	.set	macro
	.set	reorder

	move	$4,$16
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L271
	move	$5,$22
	.set	macro
	.set	reorder

$L265:
 #APP
 # 185 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	addiu	$2,$16,20
$L233:
 #APP
 # 207 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
		.set	mips3				
	ll	$3, 0($2)	# atomic_sub_return	
	subu	$4, $3, 1			
	sc	$4, 0($2)				
	.set	mips0				

 # 0 "" 2
 #NO_APP
	beq	$4,$0,$L233
 #APP
 # 228 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/atomic.h" 1
			

 # 0 "" 2
 #NO_APP
	li	$2,1			# 0x1
	bne	$3,$2,$L230
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L272
	lw	$31,68($sp)
	.set	macro
	.set	reorder

$L234:
	lui	$4,%hi($LC4)
	.set	noreorder
	.set	nomacro
	j	$L263
	addiu	$4,$4,%lo($LC4)
	.set	macro
	.set	reorder

$L245:
	lui	$18,%hi($LC4)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo($LC4)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$18,%lo($LC4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L273
	lw	$2,8($19)
	.set	macro
	.set	reorder

$L239:
	lui	$4,%hi($LC4)
	.set	noreorder
	.set	nomacro
	j	$L264
	addiu	$4,$4,%lo($LC4)
	.set	macro
	.set	reorder

$L268:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L274
	move	$5,$0
	.set	macro
	.set	reorder

$L270:
	.set	noreorder
	.set	nomacro
	jal	neigh_destroy
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L275
	lui	$4,%hi($LC3)
	.set	macro
	.set	reorder

$L267:
	j	$L261
	.end	GenerateHTTP401
	.size	GenerateHTTP401, .-GenerateHTTP401
	.section	.rodata.str1.4
	.align	2
$LC5:
	.ascii	"%s\000"
	.text
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

	beq	$16,$0,$L278
	nop

	jal	strlen
	move	$4,$16

	addiu	$4,$2,1
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L280
	sw	$2,%lo(block_message)($3)

	move	$6,$16
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC5)
	move	$4,$2
	addiu	$5,$5,%lo($LC5)
	j	sprintf
	addiu	$sp,$sp,24

$L280:
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L278:
	lui	$16,%hi($LC4)
	jal	strlen
	addiu	$4,$16,%lo($LC4)

	move	$4,$2
	jal	__kmalloc
	li	$5,208			# 0xd0

	lui	$3,%hi(block_message)
	beq	$2,$0,$L280
	sw	$2,%lo(block_message)($3)

	addiu	$6,$16,%lo($LC4)
	lw	$31,20($sp)
	lw	$16,16($sp)
	lui	$5,%hi($LC5)
	move	$4,$2
	addiu	$5,$5,%lo($LC5)
	j	sprintf
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	register_customRspStr
	.size	register_customRspStr, .-register_customRspStr
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
	beq	$4,$0,$L283
	move	$17,$5

	move	$16,$4
	lui	$18,%hi(___strtok)
$L284:
	move	$4,$16
	jal	strspn
	move	$5,$17

	addu	$16,$16,$2
	lb	$2,0($16)
	beq	$2,$0,$L290
	move	$5,$17

	jal	strpbrk
	move	$4,$16

	beq	$2,$0,$L287
	nop

	lb	$3,0($2)
	bne	$3,$0,$L291
	nop

$L287:
	sw	$2,%lo(___strtok)($18)
	lw	$31,28($sp)
	move	$2,$16
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L291:
	sb	$0,0($2)
	j	$L287
	addiu	$2,$2,1

$L290:
	move	$16,$0
	move	$2,$16
	sw	$0,%lo(___strtok)($18)
	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

$L283:
	lui	$18,%hi(___strtok)
	lw	$16,%lo(___strtok)($18)
	bne	$16,$0,$L284
	move	$2,$16

	lw	$31,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	strtok
	.size	strtok, .-strtok
	.section	.rodata.str1.4
	.align	2
$LC6:
	.ascii	" \000"
	.text
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
	sltu	$2,$6,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L318
	move	$16,$6
	.set	macro
	.set	reorder

$L293:
	li	$16,-14			# 0xfffffffffffffff2
$L316:
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

$L318:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L293
	addu	$2,$5,$6
	.set	macro
	.set	reorder

	or	$3,$5,$6
	or	$3,$3,$2
	lw	$2,24($28)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L293
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
	bne	$6,$0,$L293
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	addiu	$2,$2,%lo(dos_flag)
	li	$5,80			# 0x50
	move	$3,$4
 #APP
 # 52 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
	.set	noreorder
	.set	noat
1:	lbu	$1,($2)
	subu	$5,1
	sb	$1,($3)
	beqz	$1,2f
	addiu	$3,1
	bnez	$5,1b
	addiu	$2,1
2:
	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	lui	$20,%hi($L314)
	addiu	$20,$20,%lo($L314)
	li	$17,1			# 0x1
	lui	$19,%hi($LC6)
	lui	$fp,%hi(block_time)
	lui	$23,%hi(per_icmp_threshold)
	lui	$22,%hi(per_udp_threshold)
	lui	$21,%hi(per_fin_threshold)
$L294:
	.set	noreorder
	.set	nomacro
	jal	strtok
	addiu	$5,$19,%lo($LC6)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L316
	move	$18,$2
	.set	macro
	.set	reorder

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
	beq	$3,$0,$L295
	sll	$3,$17,2
	.set	macro
	.set	reorder

	addu	$3,$20,$3
	lw	$3,0($3)
	j	$3
	.rdata
	.align	2
	.align	2
$L314:
	.word	$L295
	.word	$L296
	.word	$L297
	.word	$L298
	.word	$L299
	.word	$L300
	.word	$L301
	.word	$L302
	.word	$L303
	.word	$L304
	.word	$L305
	.word	$L306
	.word	$L307
	.word	$L308
	.word	$L309
	.word	$L310
	.word	$L311
	.word	$L312
	.word	$L313
	.text
$L296:
	lui	$3,%hi(dos_op_mode)
	sb	$2,%lo(dos_op_mode)($3)
$L295:
	addiu	$17,$17,1
	andi	$17,$17,0x00ff
	.set	noreorder
	.set	nomacro
	j	$L294
	move	$4,$0
	.set	macro
	.set	reorder

$L313:
	lui	$3,%hi(Ipv6_ns_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(Ipv6_ns_threshold)($3)
	.set	macro
	.set	reorder

$L312:
	lui	$3,%hi(Ipv6_icmp_too_big_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(Ipv6_icmp_too_big_threshold)($3)
	.set	macro
	.set	reorder

$L311:
	lui	$3,%hi(Ipv6_smurf_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(Ipv6_smurf_threshold)($3)
	.set	macro
	.set	reorder

$L310:
	lui	$3,%hi(dos_item_ipv6)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(dos_item_ipv6)($3)
	.set	macro
	.set	reorder

$L309:
	lui	$3,%hi(dos_item_global_0)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(dos_item_global_0)($3)
	.set	macro
	.set	reorder

$L308:
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(block_time)($fp)
	.set	macro
	.set	reorder

$L307:
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(per_icmp_threshold)($23)
	.set	macro
	.set	reorder

$L306:
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(per_udp_threshold)($22)
	.set	macro
	.set	reorder

$L305:
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(per_fin_threshold)($21)
	.set	macro
	.set	reorder

$L304:
	lui	$3,%hi(per_syn_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(per_syn_threshold)($3)
	.set	macro
	.set	reorder

$L303:
	lui	$3,%hi(whole_icmp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(whole_icmp_threshold)($3)
	.set	macro
	.set	reorder

$L302:
	lui	$3,%hi(whole_udp_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(whole_udp_threshold)($3)
	.set	macro
	.set	reorder

$L301:
	lui	$3,%hi(whole_fin_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(whole_fin_threshold)($3)
	.set	macro
	.set	reorder

$L300:
	lui	$3,%hi(whole_syn_threshold)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(whole_syn_threshold)($3)
	.set	macro
	.set	reorder

$L299:
	lui	$3,%hi(dos_item_global)
	sw	$2,%lo(dos_item_global)($3)
	lui	$2,%hi(dos_item_backup)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$0,%lo(dos_item_backup)($2)
	.set	macro
	.set	reorder

$L298:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_mask)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(dos_lan_mask)($3)
	.set	macro
	.set	reorder

$L297:
	move	$4,$18
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	simple_strtol
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lui	$3,%hi(dos_lan_addr)
	.set	noreorder
	.set	nomacro
	j	$L295
	sw	$2,%lo(dos_lan_addr)($3)
	.set	macro
	.set	reorder

	.end	dos_single_write
	.size	dos_single_write, .-dos_single_write
	.section	.rodata.str1.4
	.align	2
$LC7:
	.ascii	"[ip_range_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC8:
	.ascii	"[mac_filter]%d.%d.%d.%d(%02x:%02x:%02x:%02x:%02x:%02x) ="
	.ascii	"==> %d.%d.%d.%d [Blocked]\012\000"
	.align	2
$LC9:
	.ascii	"[url_key_filter]%d.%d.%d.%d ==> %d.%d.%d.%d(%s)  [Blocke"
	.ascii	"d]\012\000"
	.align	2
$LC10:
	.ascii	"[schedule_filter]%d.%d.%d.%d ===> %d.%d.%d.%d  [Blocked]"
	.ascii	"\012\000"
	.align	2
$LC11:
	.ascii	"Unkown Type [Blocked]\012\000"
	.text
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
	beq	$16,$3,$L322
	nop

	slt	$2,$16,3
	beq	$2,$0,$L325
	li	$2,3			# 0x3

	li	$2,1			# 0x1
	beq	$16,$2,$L329
	lui	$4,%hi($LC11)

$L330:
	jal	printk
	addiu	$4,$4,%lo($LC11)

	lw	$31,180($sp)
$L331:
	lw	$23,176($sp)
$L332:
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

$L325:
	beq	$16,$2,$L323
	li	$2,4			# 0x4

	bne	$16,$2,$L330
	lui	$4,%hi($LC11)

	beq	$22,$0,$L331
	lw	$31,180($sp)

	lw	$3,16($22)
	lw	$6,12($22)
	lui	$4,%hi($LC10)
	andi	$2,$3,0xff
	addiu	$4,$4,%lo($LC10)
$L328:
	andi	$10,$6,0xff
	srl	$9,$3,24
	ext	$8,$3,16,8
	ext	$7,$6,8,8
	ext	$3,$3,8,8
	srl	$5,$6,24
	ext	$6,$6,16,8
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

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

$L323:
	beq	$22,$0,$L331
	lw	$31,180($sp)

	lw	$3,16($22)
	lw	$6,12($22)
	lui	$4,%hi($LC9)
	andi	$2,$3,0xff
	sw	$2,32($sp)
	lui	$2,%hi(log_info)
	andi	$10,$6,0xff
	srl	$9,$3,24
	ext	$8,$3,16,8
	ext	$7,$6,8,8
	ext	$3,$3,8,8
	srl	$5,$6,24
	addiu	$4,$4,%lo($LC9)
	ext	$6,$6,16,8
	addiu	$2,$2,%lo(log_info)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,36($sp)

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

$L322:
	beq	$2,$0,$L331
	lw	$31,180($sp)

	beq	$22,$0,$L332
	lw	$23,176($sp)

	move	$4,$21
	jal	rtl_eth_hdr
	lw	$19,12($22)

	move	$4,$21
	jal	rtl_eth_hdr
	move	$23,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$20,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$18,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$17,$2

	move	$4,$21
	jal	rtl_eth_hdr
	move	$16,$2

	andi	$3,$19,0xff
	lw	$11,16($22)
	sw	$3,16($sp)
	lwl	$5,87($23)
	lwl	$4,91($23)
	lwl	$3,95($23)
	lwr	$5,84($23)
	lwr	$4,88($23)
	lwr	$3,92($23)
	sw	$5,20($sp)
	sw	$4,24($sp)
	sw	$3,28($sp)
	lbu	$3,96($23)
	lui	$4,%hi($LC8)
	andi	$10,$11,0xff
	sb	$3,32($sp)
	lbu	$3,97($23)
	srl	$13,$11,24
	ext	$12,$11,16,8
	sb	$3,33($sp)
	lwl	$6,101($20)
	lwl	$5,105($20)
	lwl	$3,109($20)
	lwr	$6,98($20)
	lwr	$5,102($20)
	lwr	$3,106($20)
	sw	$6,36($sp)
	sw	$5,40($sp)
	sw	$3,44($sp)
	lbu	$3,110($20)
	ext	$7,$19,8,8
	ext	$11,$11,8,8
	sb	$3,48($sp)
	lbu	$3,111($20)
	srl	$5,$19,24
	addiu	$4,$4,%lo($LC8)
	sb	$3,49($sp)
	lwl	$8,115($18)
	lwl	$6,119($18)
	lwl	$3,123($18)
	lwr	$8,112($18)
	lwr	$6,116($18)
	lwr	$3,120($18)
	sw	$8,52($sp)
	sw	$6,56($sp)
	sw	$3,60($sp)
	lbu	$3,124($18)
	ext	$6,$19,16,8
	sb	$3,64($sp)
	lbu	$3,125($18)
	sb	$3,65($sp)
	lwl	$9,129($17)
	lwl	$8,133($17)
	lwl	$3,137($17)
	lwr	$9,126($17)
	lwr	$8,130($17)
	lwr	$3,134($17)
	sw	$9,68($sp)
	sw	$8,72($sp)
	sw	$3,76($sp)
	lbu	$3,138($17)
	sb	$3,80($sp)
	lbu	$3,139($17)
	sb	$3,81($sp)
	lwl	$9,143($16)
	lwl	$8,147($16)
	lwl	$3,151($16)
	lwr	$9,140($16)
	lwr	$8,144($16)
	lwr	$3,148($16)
	sw	$9,84($sp)
	sw	$8,88($sp)
	sw	$3,92($sp)
	lbu	$3,152($16)
	sb	$3,96($sp)
	lbu	$3,153($16)
	sb	$3,97($sp)
	lwl	$9,157($2)
	lwl	$8,161($2)
	lwl	$3,165($2)
	lwr	$9,154($2)
	lwr	$8,158($2)
	lwr	$3,162($2)
	sw	$9,100($sp)
	sw	$8,104($sp)
	sw	$3,108($sp)
	lbu	$3,166($2)
	sb	$3,112($sp)
	lbu	$2,167($2)
	sw	$13,116($sp)
	sw	$12,120($sp)
	sb	$2,113($sp)
	sw	$11,124($sp)
	jal	scrlog_printk
	sw	$10,128($sp)

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

$L329:
	beq	$22,$0,$L331
	lw	$31,180($sp)

	lw	$3,16($22)
	lui	$4,%hi($LC7)
	lw	$6,12($22)
	andi	$2,$3,0xff
	j	$L328
	addiu	$4,$4,%lo($LC7)

	.set	macro
	.set	reorder
	.end	log_fastFilter
	.size	log_fastFilter, .-log_fastFilter
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
	beq	$16,$17,$L334
	move	$18,$4

	li	$19,-1			# 0xffffffffffffffff
	j	$L340
	li	$20,1			# 0x1

$L343:
	beq	$2,$20,$L342
	lui	$2,%hi(log_enable)

	lw	$16,0($16)
	beq	$16,$17,$L337
	move	$2,$0

$L340:
	move	$6,$16
	move	$7,$16
	move	$4,$18
	jal	scan_table
	move	$5,$0

	bne	$2,$19,$L343
	nop

$L334:
	move	$2,$0
$L337:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L342:
	lw	$3,%lo(log_enable)($2)
	beq	$3,$0,$L337
	li	$2,1			# 0x1

	lw	$4,16($16)
	jal	log_fastFilter
	move	$5,$18

	j	$L337
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	do_filter
	.size	do_filter, .-do_filter
	.section	.rodata.str1.4
	.align	2
$LC12:
	.ascii	"type=%d num=%d\012\000"
	.align	2
$LC13:
	.ascii	"index=%d\011\000"
	.align	2
$LC14:
	.ascii	"mac=%02x:%02x:%02x:%02x:%02x:%02x\012\000"
	.align	2
$LC15:
	.ascii	"index=%d\011\011\000"
	.align	2
$LC16:
	.ascii	"url=%s\012\000"
	.align	2
$LC17:
	.ascii	"start=%x\011\000"
	.align	2
$LC18:
	.ascii	"end=%x\011\000"
	.align	2
$LC19:
	.ascii	"day=%x\012\000"
	.align	2
$LC20:
	.ascii	"unknow table type\012\000"
	.align	2
$LC21:
	.ascii	"Mode:%d %s\012\000"
	.text
	.align	2
	.set	nomips16
	.ent	filter_table_read_proc
	.type	filter_table_read_proc, @function
filter_table_read_proc:
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
	lui	$16,%hi(kmalloc_caches)
	addiu	$16,$16,%lo(kmalloc_caches)
	sw	$4,80($sp)
	lw	$4,36($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L345
	move	$18,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,512			# 0x200

	lw	$4,32($16)
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0

	beq	$2,$0,$L363
	move	$16,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,256			# 0x100

	lui	$2,%hi(table_list_head)
	lw	$19,%lo(table_list_head)($2)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L347
	lui	$20,%hi($LC13)

	lui	$fp,%hi($LC18)
	lui	$23,%hi($LC19)
	lui	$22,%hi($LC14)
	addiu	$20,$20,%lo($LC13)
	addiu	$fp,$fp,%lo($LC18)
	addiu	$23,$23,%lo($LC19)
	j	$L356
	addiu	$22,$22,%lo($LC14)

$L365:
	beq	$2,$3,$L351
	li	$3,2			# 0x2

	beq	$2,$3,$L364
	lui	$5,%hi($LC20)

	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC20)

	move	$4,$18
	jal	strcat
	move	$5,$16

$L352:
	lw	$19,0($19)
$L366:
	lui	$2,%hi(table_list_head)
	addiu	$2,$2,%lo(table_list_head)
	beq	$19,$2,$L368
	lui	$2,%hi(url_filter_mode)

$L356:
	lw	$7,24($19)
	lw	$6,16($19)
	lui	$5,%hi($LC12)
	move	$4,$18
	addiu	$7,$7,-1
	jal	sprintf
	addiu	$5,$5,%lo($LC12)

	lw	$2,16($19)
	li	$3,3			# 0x3
	bne	$2,$3,$L365
	li	$3,4			# 0x4

	lw	$17,8($19)
	addiu	$21,$19,8
	beq	$17,$21,$L352
	nop

$L354:
	lw	$6,20($17)
	lui	$5,%hi($LC15)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC15)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lui	$5,%hi($LC16)
	addiu	$6,$17,28
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC16)

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	bne	$17,$21,$L354
	lui	$2,%hi(table_list_head)

	lw	$19,0($19)
	addiu	$2,$2,%lo(table_list_head)
	bne	$19,$2,$L356
	nop

$L347:
	lui	$2,%hi(url_filter_mode)
$L368:
	lw	$6,%lo(url_filter_mode)($2)
	lw	$4,80($sp)
	lui	$5,%hi($LC21)
	addiu	$5,$5,%lo($LC21)
	jal	seq_printf
	move	$7,$18

	jal	kfree
	move	$4,$18

	jal	kfree
	move	$4,$16

$L345:
	lw	$31,76($sp)
$L367:
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,80

$L364:
	lw	$17,8($19)
	addiu	$21,$19,8
	beq	$17,$21,$L352
	nop

$L353:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$20

	move	$4,$18
	jal	strcat
	move	$5,$16

	lbu	$2,30($17)
	lbu	$6,28($17)
	lbu	$7,29($17)
	sw	$2,16($sp)
	lbu	$2,31($17)
	move	$4,$16
	move	$5,$22
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
	bne	$17,$21,$L353
	nop

	j	$L366
	lw	$19,0($19)

$L351:
	lw	$17,8($19)
	addiu	$2,$19,8
	beq	$17,$2,$L352
	sw	$2,32($sp)

$L355:
	lw	$6,20($17)
	move	$4,$16
	jal	sprintf
	move	$5,$20

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,28($17)
	lui	$5,%hi($LC17)
	move	$4,$16
	jal	sprintf
	addiu	$5,$5,%lo($LC17)

	addiu	$21,$17,28
	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,4($21)
	move	$4,$16
	jal	sprintf
	move	$5,$fp

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$6,8($21)
	move	$4,$16
	jal	sprintf
	move	$5,$23

	move	$4,$18
	jal	strcat
	move	$5,$16

	lw	$17,0($17)
	lw	$3,32($sp)
	bne	$17,$3,$L355
	nop

	j	$L366
	lw	$19,0($19)

$L363:
	jal	kfree
	move	$4,$18

	j	$L367
	lw	$31,76($sp)

	.set	macro
	.set	reorder
	.end	filter_table_read_proc
	.size	filter_table_read_proc, .-filter_table_read_proc
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
	bne	$2,$3,$L375
	nop

	j	$L374
	li	$2,-1			# 0xffffffffffffffff

$L377:
	lw	$2,0($2)
	beq	$2,$3,$L370
	nop

$L375:
	lw	$6,16($2)
	bne	$6,$4,$L377
	addiu	$6,$2,8

	lw	$3,8($2)
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$6,4($5)
	lw	$3,24($2)
	sw	$5,8($2)
	addiu	$3,$3,1
	sw	$3,24($2)
	li	$2,3			# 0x3
	beq	$4,$2,$L380
	li	$2,2			# 0x2

	lw	$31,20($sp)
$L381:
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L370:
	li	$2,-1			# 0xffffffffffffffff
$L374:
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

$L380:
	bne	$3,$2,$L381
	lw	$31,20($sp)

	jal	Add_Pattern_ACL_For_ContentFilter
	nop

	j	$L374
	move	$2,$0

	.set	macro
	.set	reorder
	.end	filter_item_regist
	.size	filter_item_regist, .-filter_item_regist
	.align	2
	.globl	filter_table_flush
	.set	nomips16
	.ent	filter_table_flush
	.type	filter_table_flush, @function
filter_table_flush:
	.frame	$sp,56,$31		# vars= 0, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$fp,48($sp)
	sw	$23,44($sp)
	sw	$22,40($sp)
	sw	$21,36($sp)
	sw	$20,32($sp)
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	lui	$20,%hi(table_list_head)
	lw	$16,%lo(table_list_head)($20)
	addiu	$20,$20,%lo(table_list_head)
	beq	$16,$20,$L383
	lw	$19,0($16)

	li	$18,1048576			# 0x100000
	li	$17,2097152			# 0x200000
	ori	$18,$18,0x100
	ori	$17,$17,0x200
	li	$21,3			# 0x3
	li	$22,1			# 0x1
$L387:
	lw	$2,8($16)
	addiu	$fp,$16,8
	beq	$2,$fp,$L384
	lw	$23,0($2)

	j	$L385
	move	$3,$23

$L389:
	move	$23,$3
$L385:
	lw	$5,4($2)
	move	$4,$2
	sw	$5,4($3)
	sw	$3,0($5)
	sw	$18,0($2)
	jal	kfree
	sw	$17,4($2)

	lw	$3,24($16)
	move	$2,$23
	addiu	$3,$3,-1
	sw	$3,24($16)
	bne	$23,$fp,$L389
	lw	$3,0($23)

$L384:
	lw	$2,16($16)
	beq	$2,$21,$L390
	nop

$L386:
	lw	$2,4($16)
	lw	$3,0($16)
	move	$4,$16
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	beq	$19,$20,$L383
	lw	$2,0($19)

$L391:
	move	$16,$19
	j	$L387
	move	$19,$2

$L390:
	lw	$2,24($16)
	bne	$2,$22,$L386
	nop

	jal	Del_Pattern_ACL_For_ContentFilter
	nop

	lw	$2,4($16)
	lw	$3,0($16)
	move	$4,$16
	sw	$2,4($3)
	sw	$3,0($2)
	sw	$18,0($16)
	jal	kfree
	sw	$17,4($16)

	lw	$2,24($20)
	addiu	$2,$2,-1
	sw	$2,24($20)
	bne	$19,$20,$L391
	lw	$2,0($19)

$L383:
	li	$3,1			# 0x1
	lui	$2,%hi(filter_rule_num)
	sw	$3,%lo(filter_rule_num)($2)
	lw	$31,52($sp)
	lui	$2,%hi(log_enable)
	lw	$fp,48($sp)
	lw	$23,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
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
	.section	.rodata.str1.4
	.align	2
$LC22:
	.ascii	"init\000"
	.align	2
$LC23:
	.ascii	"flush\000"
	.align	2
$LC24:
	.ascii	"enable_log\000"
	.align	2
$LC25:
	.ascii	"white\000"
	.align	2
$LC26:
	.ascii	"black\000"
	.align	2
$LC27:
	.ascii	"add:\000"
	.align	2
$LC28:
	.ascii	"#\000"
	.align	2
$LC29:
	.ascii	";\000"
	.text
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

	move	$17,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L394
	li	$19,-14			# 0xfffffffffffffff2
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
	sw	$17,20($sp)
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
	beq	$21,$0,$L395
	or	$3,$21,$16
	.set	macro
	.set	reorder

	addu	$2,$21,$16
	or	$2,$3,$2
	lw	$3,24($28)
	move	$19,$16
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L397
	move	$6,$16
	.set	macro
	.set	reorder

	move	$5,$21
	addiu	$4,$18,%lo(get_info_from_usr_space)
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
$L397:
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L433
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L427
	addiu	$3,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

$L398:
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$18,%lo(get_info_from_usr_space)
	.set	macro
	.set	reorder

	addu	$2,$17,$2
	sb	$0,-1($2)
	lw	$18,20($sp)
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
	bne	$2,$0,$L399
	lw	$18,20($sp)
	.set	macro
	.set	reorder

	addiu	$16,$sp,20
	lui	$18,%hi($LC6)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC6)
	.set	macro
	.set	reorder

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L395
	lui	$20,%hi(filter_tlb)
	.set	macro
	.set	reorder

	addiu	$20,$20,%lo(filter_tlb)
	addiu	$18,$18,%lo($LC6)
	move	$5,$18
$L434:
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

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L434
	move	$5,$18
	.set	macro
	.set	reorder

$L401:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

$L394:
	move	$2,$19
$L432:
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

$L399:
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
	beq	$2,$0,$L428
	lw	$18,20($sp)
	.set	macro
	.set	reorder

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

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L403
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(log_enable)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	kfree
	sw	$3,%lo(log_enable)($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L432
	move	$2,$19
	.set	macro
	.set	reorder

$L427:
	move	$2,$17
 #APP
 # 52 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	j	$L398
$L428:
	.set	noreorder
	.set	nomacro
	jal	filter_table_flush
	move	$4,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L432
	move	$2,$19
	.set	macro
	.set	reorder

$L395:
	move	$4,$17
$L433:
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

$L403:
	lw	$18,20($sp)
	lui	$16,%hi($LC25)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC25)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC25)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L404
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	lui	$2,%hi(url_filter_mode)
	.set	noreorder
	.set	nomacro
	j	$L401
	sw	$3,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L404:
	lw	$18,20($sp)
	lui	$16,%hi($LC26)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC26)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC26)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L405
	lui	$2,%hi(url_filter_mode)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L401
	sw	$0,%lo(url_filter_mode)($2)
	.set	macro
	.set	reorder

$L405:
	lw	$18,20($sp)
	lui	$16,%hi($LC27)
	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC27)
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	memcmp
	addiu	$5,$16,%lo($LC27)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L401
	lw	$18,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strlen
	addiu	$4,$16,%lo($LC27)
	.set	macro
	.set	reorder

	addu	$2,$18,$2
	sw	$2,20($sp)
	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L395
	addiu	$16,$sp,20
	.set	macro
	.set	reorder

	lui	$5,%hi($LC28)
	addiu	$5,$5,%lo($LC28)
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L433
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
	beq	$2,$3,$L395
	lui	$18,%hi($LC6)
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC6)
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L433
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	str2hexnum
	move	$4,$2
	.set	macro
	.set	reorder

	li	$21,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$2,$21,$L395
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$18,%lo($LC6)
	.set	macro
	.set	reorder

	lw	$3,20($sp)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L433
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
	beq	$2,$21,$L395
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	addiu	$22,$18,%lo($LC6)
	li	$fp,92			# 0x5c
	addiu	$18,$sp,16
	li	$23,32			# 0x20
	lui	$5,%hi($LC29)
$L435:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC29)
	.set	macro
	.set	reorder

	move	$4,$2
	sw	$2,16($sp)
$L429:
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L407
	lw	$21,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	addu	$2,$21,$2
	li	$3,59			# 0x3b
	lui	$5,%hi($LC29)
	sb	$3,0($2)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$5,%lo($LC29)
	.set	macro
	.set	reorder

	lw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L429
	lw	$4,16($sp)
	.set	macro
	.set	reorder

$L407:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$22
	.set	macro
	.set	reorder

	move	$21,$2
	lw	$2,16($sp)
	bne	$2,$0,$L426
	.set	noreorder
	.set	nomacro
	j	$L433
	move	$4,$17
	.set	macro
	.set	reorder

$L410:
	jal	strlen
	addu	$2,$21,$2
	sb	$23,0($2)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	strsep
	move	$5,$22
	.set	macro
	.set	reorder

$L426:
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	check_esc_end
	li	$5,92			# 0x5c
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L410
	move	$4,$21
	.set	macro
	.set	reorder

	lb	$2,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L411
	move	$6,$21
	.set	macro
	.set	reorder

	move	$7,$0
$L415:
	beq	$2,$fp,$L430
$L412:
	lb	$2,1($6)
	addiu	$7,$7,1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L415
	addiu	$6,$6,1
	.set	macro
	.set	reorder

$L411:
	lw	$4,28($20)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L401
	move	$5,$2
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	sw	$2,12($5)
	sw	$5,0($5)
	sw	$5,4($5)
	sw	$2,8($5)
	sw	$0,24($5)
	lui	$4,%hi(filter_rule_num)
	lw	$2,%lo(filter_rule_num)($4)
	lw	$3,24($sp)
	move	$4,$21
	sw	$2,20($5)
	sw	$3,16($5)
	.set	noreorder
	.set	nomacro
	jal	strlen
	sw	$5,28($sp)
	.set	macro
	.set	reorder

	sltu	$2,$2,41
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L431
	lw	$5,28($sp)
	.set	macro
	.set	reorder

	addiu	$4,$5,28
	li	$2,39			# 0x27
	li	$3,39			# 0x27
$L420:
 #APP
 # 52 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
$L418:
	addu	$3,$5,$3
	sb	$0,28($3)
	.set	noreorder
	.set	nomacro
	jal	filter_item_regist
	li	$4,3			# 0x3
	.set	macro
	.set	reorder

	lui	$4,%hi(filter_rule_num)
	lw	$3,%lo(filter_rule_num)($4)
	lw	$2,20($sp)
	addiu	$3,$3,1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L401
	sw	$3,%lo(filter_rule_num)($4)
	.set	macro
	.set	reorder

	lb	$2,0($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L435
	lui	$5,%hi($LC29)
	.set	macro
	.set	reorder

	j	$L401
$L430:
	lb	$2,1($6)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L413
	move	$5,$7
	.set	macro
	.set	reorder

	move	$4,$6
	move	$3,$7
$L414:
	addu	$5,$21,$5
	sb	$2,0($5)
	lb	$2,2($4)
	addiu	$3,$3,1
	addiu	$4,$4,1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L414
	move	$5,$3
	.set	macro
	.set	reorder

$L413:
	addu	$5,$21,$5
	.set	noreorder
	.set	nomacro
	j	$L412
	sb	$0,0($5)
	.set	macro
	.set	reorder

$L431:
	.set	noreorder
	.set	nomacro
	jal	strlen
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$5,28($sp)
	move	$3,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L418
	addiu	$4,$5,28
	.set	macro
	.set	reorder

	j	$L420
	.end	filter_table_single_write
	.size	filter_table_single_write, .-filter_table_single_write
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
	move	$17,$0
	move	$16,$4
	move	$19,$5
	li	$20,6			# 0x6
	addu	$2,$19,$17
$L443:
	move	$4,$16
	jal	rtl_skb_mac_header
	lbu	$18,0($2)

	addu	$2,$2,$17
	lbu	$2,6($2)
	bne	$18,$2,$L442
	addiu	$17,$17,1

	bne	$17,$20,$L443
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

$L442:
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
	.section	.rodata.str1.4
	.align	2
$LC30:
	.ascii	"ppp0\000"
	.align	2
$LC31:
	.ascii	"wlan0\000"
	.text
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
	lui	$16,%hi($LC2)
	.set	macro
	.set	reorder

	addiu	$4,$16,%lo($LC2)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	move	$17,$2
	.set	macro
	.set	reorder

	beq	$2,$0,$L445
	lw	$2,372($2)
	beq	$2,$0,$L445
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L445
	addiu	$16,$16,%lo($LC2)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	bne	$5,$0,$L464
	.set	noreorder
	.set	nomacro
	j	$L468
	lw	$3,28($4)
	.set	macro
	.set	reorder

$L448:
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	beq	$5,$0,$L446
$L464:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L448
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L445:
	lui	$16,%hi($LC30)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC30)
	.set	macro
	.set	reorder

	beq	$2,$0,$L450
	lw	$2,372($2)
	beq	$2,$0,$L451
	lw	$4,12($2)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L451
	addiu	$16,$16,%lo($LC30)
	.set	macro
	.set	reorder

	addiu	$3,$4,43
	move	$2,$16
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	bne	$5,$0,$L465
	.set	noreorder
	.set	nomacro
	j	$L469
	lw	$3,16($17)
	.set	macro
	.set	reorder

$L454:
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	beq	$5,$0,$L452
$L465:
	lw	$4,8($4)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L454
	addiu	$3,$4,43
	.set	macro
	.set	reorder

$L451:
	li	$2,1			# 0x1
$L449:
	lw	$31,28($sp)
$L470:
	lw	$17,24($sp)
	lw	$16,20($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L446:
	lw	$3,28($4)
$L468:
	lw	$4,16($17)
	.set	noreorder
	.set	nomacro
	bne	$4,$3,$L445
	move	$2,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L470
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L452:
	lw	$3,16($17)
$L469:
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

$L450:
	jal	rtl_get_wan_dev
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L455
	lui	$16,%hi($LC31)
	.set	macro
	.set	reorder

	lw	$2,372($2)
	beq	$2,$0,$L451
	lw	$16,12($2)
	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L462
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L470
	lw	$31,28($sp)
	.set	macro
	.set	reorder

$L467:
	lw	$16,8($16)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L449
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L462:
	.set	noreorder
	.set	nomacro
	jal	rtl_strcmp_wan_dev_name
	addiu	$4,$16,43
	.set	macro
	.set	reorder

	bne	$2,$0,$L467
	beq	$16,$0,$L451
	lw	$3,16($17)
	lw	$2,28($16)
	xor	$2,$3,$2
	.set	noreorder
	.set	nomacro
	j	$L449
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

$L455:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_dev_by_name
	addiu	$4,$16,%lo($LC31)
	.set	macro
	.set	reorder

	beq	$2,$0,$L451
	lw	$2,372($2)
	beq	$2,$0,$L451
	lw	$3,12($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L451
	addiu	$16,$16,%lo($LC31)
	.set	macro
	.set	reorder

	addiu	$4,$3,43
	move	$2,$16
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	bne	$5,$0,$L466
	.set	noreorder
	.set	nomacro
	j	$L471
	lw	$4,16($17)
	.set	macro
	.set	reorder

$L460:
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	beq	$5,$0,$L458
$L466:
	lw	$3,8($3)
	move	$2,$16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L460
	addiu	$4,$3,43
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L449
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

$L458:
	lw	$4,16($17)
$L471:
	lw	$2,28($3)
	xor	$2,$4,$2
	.set	noreorder
	.set	nomacro
	j	$L449
	sltu	$2,$0,$2
	.set	macro
	.set	reorder

	.end	need_filter
	.size	need_filter, .-need_filter
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

	bne	$2,$0,$L473
	lw	$31,20($sp)

	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

$L473:
	move	$4,$16
	lw	$16,16($sp)
	j	do_filter
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	FilterWeb_v2
	.size	FilterWeb_v2, .-FilterWeb_v2
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
	bne	$2,$0,$L477
	move	$16,$4

	lui	$2,%hi(dos_item_ipv6)
	lw	$2,%lo(dos_item_ipv6)($2)
	beq	$2,$0,$L494
	lui	$2,%hi(table_list_head+24)

$L477:
	jal	rtl_get_skb_type
	move	$4,$16

	li	$3,2			# 0x2
	beq	$2,$3,$L495
	lui	$2,%hi(dos_item_ipv6)

	jal	rtl_get_skb_type
	move	$4,$16

	beq	$2,$0,$L496
	nop

$L480:
	li	$2,4			# 0x4
$L478:
	lw	$31,28($sp)
$L499:
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L494:
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	beq	$2,$0,$L477
	lui	$3,%hi(none_dos_drop_pkt_cnt)

	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	j	$L478
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L496:
	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,8			# 0x8
	beq	$2,$3,$L482
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	beq	$2,$0,$L478
	li	$2,4			# 0x4

	jal	rtl_get_skb_protocol
	move	$4,$16

	li	$3,56710			# 0xdd86
	bne	$2,$3,$L478
	li	$2,4			# 0x4

$L482:
	lui	$2,%hi(table_list_head+24)
	lw	$2,%lo(table_list_head+24)($2)
	sltu	$2,$2,2
	bne	$2,$0,$L498
	lw	$2,%lo(dos_item_global)($17)

	jal	FilterWeb_v2
	move	$4,$16

	beq	$2,$0,$L484
	lui	$2,%hi(fast_path_cusRsp401_func)

	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	beq	$2,$0,$L481
	nop

$L490:
	jalr	$2
	move	$4,$16

	j	$L478
	move	$2,$0

$L495:
	lw	$2,%lo(dos_item_ipv6)($2)
	andi	$2,$2,0x1
	beq	$2,$0,$L478
	li	$2,4			# 0x4

	jal	filter_dos
	move	$4,$16

	beq	$2,$0,$L480
	nop

$L481:
	j	$L478
	move	$2,$0

$L483:
	lw	$2,%lo(dos_item_global)($17)
$L498:
	andi	$2,$2,0x1
	bne	$2,$0,$L486
	lui	$2,%hi(dos_item_ipv6)

	lw	$2,%lo(dos_item_ipv6)($2)
	andi	$2,$2,0x1
	bne	$2,$0,$L486
	nop

$L487:
	jal	rtl_ip_hdr
	move	$4,$16

	lbu	$3,9($2)
	li	$2,1			# 0x1
	beq	$3,$2,$L480
	lui	$3,%hi(none_dos_drop_pkt_cnt)

	lw	$4,%lo(none_dos_drop_pkt_cnt)($3)
	addiu	$4,$4,1
	j	$L478
	sw	$4,%lo(none_dos_drop_pkt_cnt)($3)

$L486:
	jal	filter_dos
	move	$4,$16

	beq	$2,$0,$L487
	move	$2,$0

	j	$L499
	lw	$31,28($sp)

$L484:
	lw	$2,%lo(fast_path_cusRsp401_func)($2)
	beq	$2,$0,$L483
	lui	$3,%hi(url_cache)

	addiu	$3,$3,%lo(url_cache)
	lbu	$4,15($3)
	li	$2,1			# 0x1
	beq	$4,$2,$L497
	nop

	lbu	$3,16($3)
	bne	$3,$2,$L498
	lw	$2,%lo(dos_item_global)($17)

	lui	$2,%hi(fast_path_cusRspTCPEndAck_func)
	lw	$2,%lo(fast_path_cusRspTCPEndAck_func)($2)
	bne	$2,$0,$L490
	nop

	j	$L478
	move	$2,$0

$L497:
	lui	$2,%hi(fast_path_cusRspTCPFinAck_func)
	lw	$2,%lo(fast_path_cusRspTCPFinAck_func)($2)
	bne	$2,$0,$L490
	nop

	j	$L478
	move	$2,$0

	.set	macro
	.set	reorder
	.end	filter_enter
	.size	filter_enter, .-filter_enter
	.align	2
	.set	nomips16
	.ent	T.1434
	.type	T.1434, @function
T.1434:
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
	subu	$2,$5,$7
	move	$17,$4
	sw	$0,0($19)
	move	$16,$5
	sw	$6,72($sp)
	move	$22,$7
	sw	$0,0($18)
	move	$20,$4
	sw	$2,16($sp)
	move	$21,$0
	move	$23,$0
	li	$fp,13			# 0xd
$L507:
	lb	$2,0($20)
	beq	$2,$fp,$L515
	li	$3,10			# 0xa

	beq	$2,$3,$L516
	andi	$2,$21,0x1

$L502:
	move	$21,$0
	lw	$5,72($sp)
$L517:
	move	$4,$20
	jal	memcmp
	move	$6,$22

	beq	$2,$0,$L506
	lw	$3,16($sp)

	addiu	$23,$23,1
	sltu	$2,$3,$23
	beq	$2,$0,$L507
	addiu	$20,$20,1

$L504:
	lw	$31,60($sp)
$L518:
	lw	$fp,56($sp)
	lw	$23,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,64

$L506:
	addu	$22,$23,$22
	sw	$22,0($18)
	addu	$17,$17,$22
	lb	$4,0($17)
	li	$3,13			# 0xd
	beq	$4,$3,$L509
	move	$2,$0

	sltu	$2,$16,$22
	bne	$2,$0,$L518
	lw	$31,60($sp)

	move	$2,$0
	j	$L510
	li	$5,13			# 0xd

$L511:
	bne	$4,$0,$L504
	addiu	$17,$17,1

$L510:
	lb	$3,1($17)
	addiu	$2,$2,1
	addu	$4,$2,$22
	bne	$3,$5,$L511
	sltu	$4,$16,$4

$L509:
	sw	$2,0($19)
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
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,64

$L515:
	andi	$2,$21,0x1
	bne	$2,$0,$L502
	addiu	$21,$21,1

	slt	$2,$21,4
	beq	$2,$0,$L518
	lw	$31,60($sp)

	j	$L517
	lw	$5,72($sp)

$L516:
	beq	$2,$0,$L502
	addiu	$21,$21,1

	slt	$2,$21,4
	beq	$2,$0,$L518
	lw	$31,60($sp)

	j	$L517
	lw	$5,72($sp)

	.set	macro
	.set	reorder
	.end	T.1434
	.size	T.1434, .-T.1434
	.section	.rodata.str1.4
	.align	2
$LC32:
	.ascii	"GET \000"
	.align	2
$LC33:
	.ascii	"Host: \000"
	.text
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
	bne	$2,$0,$L520
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
	beq	$2,$0,$L520
	move	$20,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(kmalloc_caches+44)
	lw	$4,%lo(kmalloc_caches+44)($2)
	lw	$19,12($20)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,208			# 0xd0
	.set	macro
	.set	reorder

	ext	$19,$19,4,4
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L520
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
	beq	$2,$0,$L527
	move	$18,$2
	.set	macro
	.set	reorder

	sll	$19,$19,2
	addu	$19,$20,$19
	lui	$20,%hi($LC32)
	move	$4,$19
	addiu	$5,$20,%lo($LC32)
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L527
	lui	$6,%hi($LC33)
	.set	macro
	.set	reorder

	addiu	$21,$sp,24
	addiu	$2,$sp,28
	addiu	$6,$6,%lo($LC33)
	move	$4,$19
	move	$5,$18
	li	$7,6			# 0x6
	sw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	jal	T.1434
	sw	$21,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L527
	lw	$4,28($sp)
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L526
	move	$2,$0
	.set	macro
	.set	reorder

	addu	$3,$19,$3
	move	$2,$17
 #APP
 # 52 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
$L526:
	addu	$2,$17,$2
	sb	$0,0($2)
	move	$5,$18
	addiu	$2,$sp,32
	addiu	$6,$20,%lo($LC32)
	move	$4,$19
	li	$7,4			# 0x4
	sw	$21,16($sp)
	.set	noreorder
	.set	nomacro
	jal	T.1434
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L527
	lw	$2,32($sp)
	.set	macro
	.set	reorder

	addiu	$2,$2,-9
	.set	noreorder
	.set	nomacro
	blez	$2,$L527
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$3,28($sp)
	lw	$4,24($sp)
	addu	$3,$17,$3
	addu	$19,$19,$4
 #APP
 # 52 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/asm/string.h" 1
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
	lw	$2,32($sp)
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
	move	$19,$2
	.set	macro
	.set	reorder

	sltu	$19,$19,$2
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L537
	move	$18,$2
	.set	macro
	.set	reorder

$L530:
	move	$16,$0
$L532:
	lui	$6,%hi($LC5)
	lui	$4,%hi(log_info)
	addiu	$6,$6,%lo($LC5)
	addiu	$4,$4,%lo(log_info)
	li	$5,32			# 0x20
	.set	noreorder
	.set	nomacro
	jal	snprintf
	move	$7,$17
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
	bne	$2,$3,$L522
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L534
	lui	$3,%hi(url_filter_flag)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L522
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L520:
	move	$16,$0
$L522:
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

$L527:
	move	$4,$17
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

$L537:
	addu	$20,$17,$2
	lb	$2,0($20)
	beq	$2,$0,$L530
	move	$4,$0
$L533:
	addu	$4,$17,$4
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jal	memcmp
	move	$6,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L531
	addiu	$19,$19,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L532
	li	$16,1			# 0x1
	.set	macro
	.set	reorder

$L531:
	lb	$2,1($20)
	move	$4,$19
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L533
	addiu	$20,$20,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L532
	move	$16,$0
	.set	macro
	.set	reorder

$L534:
	.set	noreorder
	.set	nomacro
	j	$L522
	sw	$2,%lo(url_filter_flag)($3)
	.set	macro
	.set	reorder

	.end	rtl_url_filter_fastpath
	.size	rtl_url_filter_fastpath, .-rtl_url_filter_fastpath
	.globl	filter_tlb
	.data
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
	.globl	filter_table_proc_fops
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
	.globl	filter_dos_ipv4_handler
	.section	.bss,"aw",@nobits
	.align	2
	.type	filter_dos_ipv4_handler, @object
	.size	filter_dos_ipv4_handler, 4
filter_dos_ipv4_handler:
	.space	4
	.globl	filter_dos_ipv6_handler
	.align	2
	.type	filter_dos_ipv6_handler, @object
	.size	filter_dos_ipv6_handler, 4
filter_dos_ipv6_handler:
	.space	4
	.globl	dos_timer_ipv4_func
	.align	2
	.type	dos_timer_ipv4_func, @object
	.size	dos_timer_ipv4_func, 4
dos_timer_ipv4_func:
	.space	4
	.globl	dos_timer_ipv6_func
	.align	2
	.type	dos_timer_ipv6_func, @object
	.size	dos_timer_ipv6_func, 4
dos_timer_ipv6_func:
	.space	4
	.globl	dos_read_proc_func
	.align	2
	.type	dos_read_proc_func, @object
	.size	dos_read_proc_func, 4
dos_read_proc_func:
	.space	4
	.globl	dosFilter_block
	.align	2
	.type	dosFilter_block, @object
	.size	dosFilter_block, 4
dosFilter_block:
	.space	4
	.globl	dos_item_global
	.align	2
	.type	dos_item_global, @object
	.size	dos_item_global, 4
dos_item_global:
	.space	4
	.globl	dos_item_global_0
	.align	2
	.type	dos_item_global_0, @object
	.size	dos_item_global_0, 4
dos_item_global_0:
	.space	4
	.globl	dos_item_ipv6
	.align	2
	.type	dos_item_ipv6, @object
	.size	dos_item_ipv6, 4
dos_item_ipv6:
	.space	4
	.globl	dos_proc_fops
	.data
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
	.globl	fast_path_cusRsp401_func
	.section	.bss
	.align	2
	.type	fast_path_cusRsp401_func, @object
	.size	fast_path_cusRsp401_func, 4
fast_path_cusRsp401_func:
	.space	4
	.globl	fast_path_cusRspTCPFinAck_func
	.align	2
	.type	fast_path_cusRspTCPFinAck_func, @object
	.size	fast_path_cusRspTCPFinAck_func, 4
fast_path_cusRspTCPFinAck_func:
	.space	4
	.globl	fast_path_cusRspTCPEndAck_func
	.align	2
	.type	fast_path_cusRspTCPEndAck_func, @object
	.size	fast_path_cusRspTCPEndAck_func, 4
fast_path_cusRspTCPEndAck_func:
	.space	4
	.local	dos_item_backup
	.comm	dos_item_backup,4,4
	.local	block_time
	.comm	block_time,4,4
	.local	log_enable
	.comm	log_enable,4,4
	.local	url_filter_mode
	.comm	url_filter_mode,4,4
	.local	res3
	.comm	res3,4,4
	.local	none_dos_drop_pkt_cnt
	.comm	none_dos_drop_pkt_cnt,4,4
	.local	url_count
	.comm	url_count,1,1
	.local	block_count
	.comm	block_count,4,4
	.local	url_cache
	.comm	url_cache,20,4
	.local	endStateClientPort
	.comm	endStateClientPort,2,2
	.local	endStateClientSeq
	.comm	endStateClientSeq,4,4
	.local	finStateClientPort
	.comm	finStateClientPort,2,2
	.local	finStateClientSeq
	.comm	finStateClientSeq,4,4
	.local	block_message
	.comm	block_message,4,4
	.local	res12
	.comm	res12,4,4
	.local	url_filter_flag
	.comm	url_filter_flag,4,4
	.local	get_info_from_usr_space
	.comm	get_info_from_usr_space,1024,4
	.local	log_info
	.comm	log_info,32,4
	.local	filter_rule_num
	.comm	filter_rule_num,4,4
	.local	dos_timer
	.comm	dos_timer,28,4
	.globl	table_list_head
	.align	2
	.type	table_list_head, @object
	.size	table_list_head, 32
table_list_head:
	.space	32
	.globl	ConnectedIp
	.align	2
	.type	ConnectedIp, @object
	.size	ConnectedIp, 1024
ConnectedIp:
	.space	1024
	.globl	dos_op_mode
	.type	dos_op_mode, @object
	.size	dos_op_mode, 1
dos_op_mode:
	.space	1
	.globl	whole_syn_threshold
	.align	2
	.type	whole_syn_threshold, @object
	.size	whole_syn_threshold, 4
whole_syn_threshold:
	.space	4
	.globl	whole_fin_threshold
	.align	2
	.type	whole_fin_threshold, @object
	.size	whole_fin_threshold, 4
whole_fin_threshold:
	.space	4
	.globl	whole_udp_threshold
	.align	2
	.type	whole_udp_threshold, @object
	.size	whole_udp_threshold, 4
whole_udp_threshold:
	.space	4
	.globl	whole_icmp_threshold
	.align	2
	.type	whole_icmp_threshold, @object
	.size	whole_icmp_threshold, 4
whole_icmp_threshold:
	.space	4
	.globl	per_syn_threshold
	.align	2
	.type	per_syn_threshold, @object
	.size	per_syn_threshold, 4
per_syn_threshold:
	.space	4
	.globl	per_fin_threshold
	.align	2
	.type	per_fin_threshold, @object
	.size	per_fin_threshold, 4
per_fin_threshold:
	.space	4
	.globl	per_udp_threshold
	.align	2
	.type	per_udp_threshold, @object
	.size	per_udp_threshold, 4
per_udp_threshold:
	.space	4
	.globl	per_icmp_threshold
	.align	2
	.type	per_icmp_threshold, @object
	.size	per_icmp_threshold, 4
per_icmp_threshold:
	.space	4
	.globl	per_ping_threshold
	.align	2
	.type	per_ping_threshold, @object
	.size	per_ping_threshold, 4
per_ping_threshold:
	.space	4
	.globl	Ipv6_smurf_threshold
	.align	2
	.type	Ipv6_smurf_threshold, @object
	.size	Ipv6_smurf_threshold, 4
Ipv6_smurf_threshold:
	.space	4
	.globl	Ipv6_icmp_too_big_threshold
	.align	2
	.type	Ipv6_icmp_too_big_threshold, @object
	.size	Ipv6_icmp_too_big_threshold, 4
Ipv6_icmp_too_big_threshold:
	.space	4
	.globl	Ipv6_ns_threshold
	.align	2
	.type	Ipv6_ns_threshold, @object
	.size	Ipv6_ns_threshold, 4
Ipv6_ns_threshold:
	.space	4
	.globl	dos_lan_addr
	.align	2
	.type	dos_lan_addr, @object
	.size	dos_lan_addr, 4
dos_lan_addr:
	.space	4
	.globl	dos_lan_mask
	.align	2
	.type	dos_lan_mask, @object
	.size	dos_lan_mask, 4
dos_lan_mask:
	.space	4
	.globl	dos_flag
	.align	2
	.type	dos_flag, @object
	.size	dos_flag, 80
dos_flag:
	.space	80
	.globl	___strtok
	.align	2
	.type	___strtok, @object
	.size	___strtok, 4
___strtok:
	.space	4
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
