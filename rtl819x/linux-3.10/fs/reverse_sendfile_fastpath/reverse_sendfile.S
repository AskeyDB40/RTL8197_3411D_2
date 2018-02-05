	.file	1 "reverse_sendfile.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.rtl_tcp_ctl_init,"ax",@progbits
	.align	2
	.globl	rtl_tcp_ctl_init
	.set	nomips16
	.ent	rtl_tcp_ctl_init
	.type	rtl_tcp_ctl_init, @function
rtl_tcp_ctl_init:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$4,%hi(g2_tcp_ctl)
	addiu	$2,$4,%lo(g2_tcp_ctl)
	li	$5,16			# 0x10
	sw	$5,8($2)
	li	$5,8192			# 0x2000
	sw	$5,12($2)
	li	$5,4096			# 0x1000
	li	$3,1			# 0x1
	sw	$5,16($2)
	li	$5,2			# 0x2
	sw	$5,20($2)
	sw	$3,24($2)
	sw	$0,28($2)
	sw	$3,%lo(g2_tcp_ctl)($4)
	j	$31
	sw	$3,4($2)

	.set	macro
	.set	reorder
	.end	rtl_tcp_ctl_init
	.size	rtl_tcp_ctl_init, .-rtl_tcp_ctl_init
	.section	.text.memcpy_tokerneliovec,"ax",@progbits
	.align	2
	.globl	memcpy_tokerneliovec
	.set	nomips16
	.ent	memcpy_tokerneliovec
	.type	memcpy_tokerneliovec, @function
memcpy_tokerneliovec:
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
	move	$17,$6
	move	$16,$4
	blez	$6,$L4
	move	$19,$5

	lw	$2,4($16)
	bne	$2,$0,$L12
	sltu	$18,$2,$17

$L5:
	blez	$17,$L13
	lw	$31,36($sp)

$L11:
	addiu	$16,$16,8
	lw	$2,4($16)
	beq	$2,$0,$L5
	sltu	$18,$2,$17

$L12:
	lw	$4,0($16)
	movz	$2,$17,$18
	move	$5,$19
	move	$6,$2
	jal	memcpy
	move	$18,$2

	lw	$3,4($16)
	lw	$2,0($16)
	subu	$17,$17,$18
	subu	$3,$3,$18
	addu	$2,$2,$18
	sw	$3,4($16)
	sw	$2,0($16)
	bgtz	$17,$L11
	addu	$19,$19,$18

$L4:
	lw	$31,36($sp)
$L13:
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	memcpy_tokerneliovec
	.size	memcpy_tokerneliovec, .-memcpy_tokerneliovec
	.section	.text.skb_copy_datagram_to_kernel_iovec,"ax",@progbits
	.align	2
	.globl	skb_copy_datagram_to_kernel_iovec
	.set	nomips16
	.ent	skb_copy_datagram_to_kernel_iovec
	.type	skb_copy_datagram_to_kernel_iovec, @function
skb_copy_datagram_to_kernel_iovec:
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
	move	$16,$5
	move	$17,$6
	move	$20,$7
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	move	$21,$4
	.set	macro
	.set	reorder

	lw	$2,8($2)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L15
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	move	$19,$0
	li	$23,-1			# 0xffffffffffffffff
	li	$fp,-2147483648			# 0xffffffff80000000
$L23:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_headlen
	move	$4,$21
	.set	macro
	.set	reorder

	li	$18,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$L20
	move	$3,$19
	.set	macro
	.set	reorder

$L17:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	addiu	$2,$2,44
	sll	$4,$18,3
	addu	$4,$2,$4
	lui	$6,%hi(mem_map)
	lw	$5,0($4)
	lw	$2,%lo(mem_map)($6)
	subu	$20,$20,$22
	addu	$16,$16,$22
	subu	$5,$5,$2
	sra	$5,$5,5
	sll	$5,$5,14
	addu	$5,$5,$fp
	.set	noreorder
	.set	nomacro
	jal	rtl_get_frag_offset
	sw	$5,20($sp)
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	lw	$5,20($sp)
	move	$6,$22
	addu	$2,$2,$3
	addu	$5,$5,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy_tokerneliovec
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L22
	move	$2,$0
	.set	macro
	.set	reorder

$L16:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	move	$4,$21
	.set	macro
	.set	reorder

	lbu	$2,0($2)
	addiu	$18,$18,1
	slt	$2,$18,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L19
	move	$4,$21
	.set	macro
	.set	reorder

	jal	rtl_skb_shared_info
	addiu	$3,$18,5
	sll	$3,$3,3
	addu	$2,$2,$3
	lhu	$2,10($2)
	move	$3,$19
$L20:
	addu	$19,$19,$2
	subu	$2,$19,$16
	slt	$5,$16,$19
	move	$4,$21
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L16
	slt	$22,$20,$2
	.set	macro
	.set	reorder

	movn	$2,$20,$22
	move	$22,$2
	.set	noreorder
	.set	nomacro
	bne	$18,$23,$L17
	subu	$3,$16,$3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_data_common
	sw	$3,24($sp)
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	move	$4,$17
	move	$6,$22
	addu	$5,$2,$3
	.set	noreorder
	.set	nomacro
	jal	memcpy_tokerneliovec
	subu	$20,$20,$22
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$20,$0,$L16
	addu	$16,$16,$22
	.set	macro
	.set	reorder

$L15:
	move	$2,$0
$L22:
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

$L19:
	lw	$2,16($sp)
	beq	$2,$0,$L25
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	lw	$4,16($sp)
	.set	macro
	.set	reorder

	lw	$2,8($2)
 #APP
 # 29 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/bug.h" 1
	tne $0, $2, 12
 # 0 "" 2
 #NO_APP
	lw	$21,16($sp)
	lw	$6,0($21)
	.set	noreorder
	.set	nomacro
	j	$L23
	sw	$6,16($sp)
	.set	macro
	.set	reorder

$L25:
	.set	noreorder
	.set	nomacro
	j	$L22
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	skb_copy_datagram_to_kernel_iovec
	.size	skb_copy_datagram_to_kernel_iovec, .-skb_copy_datagram_to_kernel_iovec
	.section	.text.rtl_splice_from_pipe_feed,"ax",@progbits
	.align	2
	.globl	rtl_splice_from_pipe_feed
	.set	nomips16
	.ent	rtl_splice_from_pipe_feed
	.type	rtl_splice_from_pipe_feed, @function
rtl_splice_from_pipe_feed:
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
	lui	$2,%hi(pipe_to_sendpages)
	addiu	$2,$2,%lo(pipe_to_sendpages)
	move	$20,$6
	move	$16,$5
	move	$18,$4
	beq	$6,$2,$L46
	li	$22,1			# 0x1

$L45:
	lw	$2,32($18)
	beq	$2,$0,$L28
	move	$4,$18

	lw	$2,36($18)
	lw	$17,80($18)
	lw	$3,4($16)
	sll	$5,$2,5
	sll	$2,$2,3
	subu	$2,$5,$2
	addu	$17,$17,$2
	lw	$2,8($17)
	lw	$21,12($17)
	move	$5,$17
	sltu	$6,$3,$2
	beq	$6,$0,$L36
	sw	$2,0($16)

	sw	$3,0($16)
$L36:
	lw	$2,12($17)
	lw	$2,12($2)
	jalr	$2
	nop

	move	$19,$2
	move	$4,$18
	move	$5,$17
	bne	$2,$0,$L47
	move	$6,$16

	jalr	$20
	nop

	blez	$2,$L31
	sra	$6,$2,31

	lw	$3,4($17)
	lw	$4,8($17)
	addu	$3,$3,$2
	subu	$4,$4,$2
	sw	$4,8($17)
	sw	$3,4($17)
	lw	$5,20($16)
	lw	$9,16($16)
	lw	$8,28($16)
	lw	$7,0($16)
	lw	$3,4($16)
	addu	$4,$5,$2
	sltu	$5,$4,$5
	addu	$6,$9,$6
	subu	$3,$3,$2
	addu	$6,$5,$6
	addu	$5,$8,$2
	subu	$2,$7,$2
	sw	$5,28($16)
	sw	$2,0($16)
	sw	$6,16($16)
	sw	$4,20($16)
	sw	$3,4($16)
	lw	$2,8($17)
	beq	$2,$0,$L48
	nop

	bne	$3,$0,$L45
	nop

$L38:
	move	$2,$19
$L31:
	lw	$31,44($sp)
$L53:
$L54:
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,48

$L48:
	sw	$0,12($17)
	lw	$2,16($21)
	move	$5,$17
	jalr	$2
	move	$4,$18

	lw	$4,36($18)
	lw	$5,40($18)
	lw	$3,32($18)
	lw	$2,52($18)
	addiu	$5,$5,-1
	addiu	$4,$4,1
	and	$4,$5,$4
	addiu	$3,$3,-1
	sw	$4,36($18)
	bne	$2,$0,$L40
	sw	$3,32($18)

	lw	$3,4($16)
	bne	$3,$0,$L45
	move	$2,$19

	j	$L54
	lw	$31,44($sp)

$L40:
	lw	$3,4($16)
	bne	$3,$0,$L45
	sb	$22,32($16)

	j	$L31
	move	$2,$19

$L46:
	lw	$2,4($5)
	lw	$3,36($4)
	bne	$2,$0,$L49
	lw	$5,80($4)

$L28:
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,48

$L49:
	sll	$17,$3,5
	sll	$3,$3,3
	subu	$17,$17,$3
	addu	$17,$5,$17
	lw	$2,12($17)
	lw	$2,12($2)
	jalr	$2
	move	$5,$17

	bne	$2,$0,$L50
	move	$4,$18

	move	$5,$17
	jal	pipe_to_sendpages
	move	$6,$16

	blez	$2,$L31
	move	$19,$2

	lw	$3,8($17)
	sltu	$2,$2,$3
	beq	$2,$0,$L43
	nop

	j	$L52
	lw	$2,4($17)

$L51:
	lw	$2,80($18)
	addu	$17,$2,$17
	lw	$3,8($17)
	sltu	$2,$19,$3
	bne	$2,$0,$L32
	nop

$L43:
	lw	$2,12($17)
	sw	$0,12($17)
	move	$5,$17
	lw	$2,16($2)
	move	$4,$18
	jalr	$2
	subu	$19,$19,$3

	lw	$4,36($18)
	lw	$2,40($18)
	lw	$3,32($18)
	addiu	$4,$4,1
	addiu	$2,$2,-1
	and	$2,$2,$4
	sll	$17,$2,5
	sll	$4,$2,3
	addiu	$3,$3,-1
	subu	$17,$17,$4
	sw	$3,32($18)
	bne	$19,$0,$L51
	sw	$2,36($18)

$L33:
	lw	$2,52($18)
	beq	$2,$0,$L35
	nop

	li	$2,1			# 0x1
	sb	$2,32($16)
$L35:
	lw	$2,4($16)
	j	$L31
	sltu	$2,$0,$2

$L32:
	lw	$2,4($17)
$L52:
	subu	$3,$3,$19
	sw	$3,8($17)
	addu	$19,$2,$19
	j	$L33
	sw	$19,4($17)

$L47:
	li	$2,-61			# 0xffffffffffffffc3
	bne	$19,$2,$L38
	move	$2,$0

	j	$L54
	lw	$31,44($sp)

$L50:
	li	$3,-61			# 0xffffffffffffffc3
	bne	$2,$3,$L53
	lw	$31,44($sp)

	j	$L53
	move	$2,$0

	.set	macro
	.set	reorder
	.end	rtl_splice_from_pipe_feed
	.size	rtl_splice_from_pipe_feed, .-rtl_splice_from_pipe_feed
	.section	.text.balance_dirty_pages_ratelimited_nr,"ax",@progbits
	.align	2
	.globl	balance_dirty_pages_ratelimited_nr
	.set	nomips16
	.ent	balance_dirty_pages_ratelimited_nr
	.type	balance_dirty_pages_ratelimited_nr, @function
balance_dirty_pages_ratelimited_nr:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	lw	$2,72($4)
	lui	$3,%hi(ratelimit_pages)
	lw	$6,%lo(ratelimit_pages)($3)
	lw	$7,192($2)
	li	$2,8			# 0x8
	movz	$2,$6,$7
	lw	$7,16($28)
	lui	$6,%hi(__per_cpu_offset)
	addiu	$6,$6,%lo(__per_cpu_offset)
	sll	$7,$7,2
	addu	$6,$7,$6
	lw	$7,0($6)
	lui	$6,%hi(bdp_ratelimits)
	addiu	$6,$6,%lo(bdp_ratelimits)
	addu	$6,$6,$7
	lw	$7,0($6)
	addu	$5,$5,$7
	sltu	$2,$5,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L60
	sw	$5,0($6)
	.set	macro
	.set	reorder

	j	$31
$L60:
	lw	$2,%lo(ratelimit_pages)($3)
	sw	$0,0($6)
	sltu	$3,$5,$2
	movn	$5,$2,$3
	srl	$2,$5,1
	addu	$5,$2,$5
	j	balance_dirty_pages
	.end	balance_dirty_pages_ratelimited_nr
	.size	balance_dirty_pages_ratelimited_nr, .-balance_dirty_pages_ratelimited_nr
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%s: %d: %s:count(%d) exceed maxinum\012\000"
	.align	2
$LC1:
	.ascii	"fs/reverse_sendfile_fastpath/reverse_sendfile.c\000"
	.align	2
$LC2:
	.ascii	"%s: %d: %s: write_end fail,ret = %d\012\000"
	.section	.text.do_splice_from_socket,"ax",@progbits
	.align	2
	.globl	do_splice_from_socket
	.set	nomips16
	.ent	do_splice_from_socket
	.type	do_splice_from_socket, @function
do_splice_from_socket:
	.frame	$sp,680,$31		# vars= 608, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-680
	sw	$31,676($sp)
	sw	$fp,672($sp)
	sw	$23,668($sp)
	sw	$22,664($sp)
	sw	$21,660($sp)
	sw	$20,656($sp)
	sw	$19,652($sp)
	sw	$18,648($sp)
	sw	$17,644($sp)
	sw	$16,640($sp)
	move	$17,$6
	sw	$7,692($sp)
	sw	$5,684($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_file_mapping
	move	$18,$4
	.set	macro
	.set	reorder

	move	$16,$2
	addiu	$2,$17,8
	or	$2,$2,$17
	sw	$2,624($sp)
	lw	$2,24($28)
	lw	$3,624($sp)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L62
	lw	$19,0($16)
	.set	macro
	.set	reorder

	addiu	$4,$sp,40
	move	$5,$17
	li	$6,8			# 0x8
 #APP
 # 571 "fs/reverse_sendfile_fastpath/reverse_sendfile.c" 1
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
	bne	$6,$0,$L62
	lw	$2,692($sp)
	.set	macro
	.set	reorder

	li	$3,262144			# 0x40000
	ori	$3,$3,0x1
	sltu	$3,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L84
	lui	$4,%hi($LC0)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_inode_mutex_lock
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_inode_sb
	move	$4,$19
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_vfs_check_frozen
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	lw	$3,72($16)
	lw	$2,0($28)
	move	$4,$18
	addiu	$5,$sp,40
	sw	$3,720($2)
	addiu	$6,$sp,692
	.set	noreorder
	.set	nomacro
	jal	rtl_generic_write_checks
	move	$7,$19
	.set	macro
	.set	reorder

	beq	$2,$0,$L85
$L65:
	lw	$3,0($28)
	move	$4,$19
	sw	$0,720($3)
	.set	noreorder
	.set	nomacro
	jal	rtl_inode_mutex_unlock
	sw	$2,628($sp)
	.set	macro
	.set	reorder

	lw	$2,628($sp)
$L64:
	lw	$31,676($sp)
	lw	$fp,672($sp)
	lw	$23,668($sp)
	lw	$22,664($sp)
	lw	$21,660($sp)
	lw	$20,656($sp)
	lw	$19,652($sp)
	lw	$18,648($sp)
	lw	$17,644($sp)
	lw	$16,640($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,680
	.set	macro
	.set	reorder

$L85:
	lw	$2,692($sp)
	bne	$2,$0,$L86
$L66:
	lw	$2,0($28)
$L89:
	move	$4,$19
	.set	noreorder
	.set	nomacro
	jal	rtl_inode_mutex_unlock
	sw	$0,720($2)
	.set	macro
	.set	reorder

	lw	$2,692($sp)
	lw	$31,676($sp)
	lw	$fp,672($sp)
	lw	$23,668($sp)
	lw	$22,664($sp)
	lw	$21,660($sp)
	lw	$20,656($sp)
	lw	$19,652($sp)
	lw	$18,648($sp)
	lw	$17,644($sp)
	lw	$16,640($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,680
	.set	macro
	.set	reorder

$L62:
	lw	$31,676($sp)
	lw	$fp,672($sp)
	lw	$23,668($sp)
	lw	$22,664($sp)
	lw	$21,660($sp)
	lw	$20,656($sp)
	lw	$19,652($sp)
	lw	$18,648($sp)
	lw	$17,644($sp)
	lw	$16,640($sp)
	li	$2,-14			# 0xfffffffffffffff2
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,680
	.set	macro
	.set	reorder

$L86:
	.set	noreorder
	.set	nomacro
	jal	file_remove_suid
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	file_update_time
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$23,692($sp)
	lw	$6,40($sp)
	lw	$7,44($sp)
	addiu	$20,$sp,232
	addiu	$3,$sp,76
	move	$21,$20
	move	$fp,$0
	lui	$10,%hi(mem_map)
$L70:
	andi	$8,$7,0x3fff
	li	$4,16384			# 0x4000
	subu	$2,$4,$8
	sltu	$22,$23,$2
	lw	$4,64($16)
	movn	$2,$23,$22
	li	$5,1			# 0x1
	sw	$2,16($sp)
	sw	$5,20($sp)
	move	$22,$2
	addiu	$5,$sp,36
	addiu	$2,$sp,32
	sw	$5,28($sp)
	sw	$2,24($sp)
	lw	$2,20($4)
	move	$5,$16
	sw	$3,628($sp)
	sw	$8,632($sp)
	sw	$10,636($sp)
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$4,$18
	.set	macro
	.set	reorder

	move	$9,$2
	subu	$23,$23,$22
	lw	$3,628($sp)
	lw	$8,632($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L87
	lw	$10,636($sp)
	.set	macro
	.set	reorder

	lw	$4,32($sp)
	lw	$5,%lo(mem_map)($10)
	addiu	$fp,$fp,1
	sw	$4,-16($21)
	lw	$7,44($sp)
	lw	$2,40($sp)
	subu	$4,$4,$5
	sw	$7,-4($21)
	addu	$7,$22,$7
	sw	$2,-8($21)
	sw	$22,0($21)
	sra	$4,$4,5
	sltu	$6,$7,$22
	addu	$6,$6,$2
	sll	$4,$4,14
	lw	$2,36($sp)
	li	$5,-2147483648			# 0xffffffff80000000
	addu	$4,$4,$5
	addu	$8,$4,$8
	sw	$2,4($21)
	sw	$8,0($3)
	sw	$22,4($3)
	addiu	$21,$21,24
	sw	$6,40($sp)
	sw	$7,44($sp)
	.set	noreorder
	.set	nomacro
	bne	$23,$0,$L70
	addiu	$3,$3,8
	.set	macro
	.set	reorder

	lw	$4,684($sp)
	li	$2,65536			# 0x10000
	sw	$2,72($sp)
	addiu	$2,$sp,76
	sw	$0,48($sp)
	sw	$0,52($sp)
	sw	$2,56($sp)
	sw	$fp,60($sp)
	sw	$0,64($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_rcvtimeo
	sw	$0,68($sp)
	.set	macro
	.set	reorder

	lw	$4,684($sp)
	li	$5,300			# 0x12c
	.set	noreorder
	.set	nomacro
	jal	rtl_set_sk_rcvtimeo
	move	$22,$2
	.set	macro
	.set	reorder

	lw	$2,692($sp)
	lw	$4,684($sp)
	addiu	$6,$sp,76
	sw	$2,16($sp)
	li	$2,131072			# 0x20000
	move	$7,$fp
	addiu	$5,$sp,48
	ori	$2,$2,0x100
	.set	noreorder
	.set	nomacro
	jal	kernel_recvmsg
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$4,684($sp)
	move	$5,$22
	move	$21,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_set_sk_rcvtimeo
	move	$22,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bltz	$21,$L72
	lw	$3,44($sp)
	.set	macro
	.set	reorder

	lw	$2,692($sp)
	lw	$4,40($sp)
	sra	$5,$21,31
	subu	$2,$3,$2
	sltu	$3,$3,$2
	subu	$4,$4,$3
	addu	$3,$2,$21
	addu	$4,$4,$5
	sltu	$2,$3,$2
	addu	$2,$2,$4
	sw	$2,40($sp)
	sw	$3,44($sp)
	.set	noreorder
	.set	nomacro
	blez	$fp,$L75
	sw	$21,692($sp)
	.set	macro
	.set	reorder

	lui	$23,%hi($LC1)
	lui	$22,%hi(__func__.55017)
	addiu	$23,$23,%lo($LC1)
	addiu	$22,$22,%lo(__func__.55017)
	move	$21,$0
$L77:
	lw	$2,0($20)
	lw	$5,-16($20)
	lw	$4,4($20)
	lw	$3,64($16)
	sw	$5,24($sp)
	sw	$4,28($sp)
	sw	$2,20($sp)
	sw	$2,16($sp)
	lw	$7,-4($20)
	lw	$6,-8($20)
	lw	$2,24($3)
	move	$4,$18
	move	$5,$16
	.set	noreorder
	.set	nomacro
	jalr	$2
	addiu	$21,$21,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bltz	$2,$L88
	lui	$4,%hi($LC2)
	.set	macro
	.set	reorder

	slt	$2,$21,$fp
$L90:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L77
	addiu	$20,$20,24
	.set	macro
	.set	reorder

$L75:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	balance_dirty_pages_ratelimited_nr
	move	$5,$fp
	.set	macro
	.set	reorder

	lw	$2,24($28)
	lw	$3,624($sp)
	and	$2,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L66
	move	$4,$17
	.set	macro
	.set	reorder

	addiu	$5,$sp,40
	li	$6,8			# 0x8
 #APP
 # 677 "fs/reverse_sendfile_fastpath/reverse_sendfile.c" 1
	jal	__copy_user
	
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L89
	lw	$2,0($28)
	.set	macro
	.set	reorder

$L73:
	lw	$2,0($20)
	lw	$7,-16($20)
	lw	$6,4($20)
	lw	$3,64($16)
	sw	$2,20($sp)
	sw	$7,24($sp)
	sw	$6,28($sp)
	sw	$2,16($sp)
	lw	$7,-4($20)
	lw	$6,-8($20)
	lw	$2,24($3)
	addiu	$22,$22,1
	.set	noreorder
	.set	nomacro
	jalr	$2
	addiu	$20,$20,24
	.set	macro
	.set	reorder

$L72:
	slt	$2,$22,$fp
	move	$4,$18
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L73
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L65
	move	$2,$21
	.set	macro
	.set	reorder

$L88:
	addiu	$4,$4,%lo($LC2)
	move	$5,$23
	li	$6,673			# 0x2a1
	move	$7,$22
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L90
	slt	$2,$21,$fp
	.set	macro
	.set	reorder

$L87:
	.set	noreorder
	.set	nomacro
	beq	$fp,$0,$L65
	move	$17,$0
	.set	macro
	.set	reorder

$L79:
	lw	$2,0($20)
	lw	$5,-16($20)
	lw	$4,4($20)
	lw	$3,64($16)
	sw	$5,24($sp)
	sw	$4,28($sp)
	sw	$2,20($sp)
	sw	$2,16($sp)
	lw	$7,-4($20)
	lw	$6,-8($20)
	lw	$2,24($3)
	addiu	$17,$17,1
	sw	$9,628($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jalr	$2
	move	$5,$16
	.set	macro
	.set	reorder

	slt	$2,$17,$fp
	addiu	$20,$20,24
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L79
	lw	$9,628($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L65
	move	$2,$9
	.set	macro
	.set	reorder

$L84:
	lui	$5,%hi($LC1)
	lui	$7,%hi(__func__.55017)
	addiu	$4,$4,%lo($LC0)
	addiu	$5,$5,%lo($LC1)
	addiu	$7,$7,%lo(__func__.55017)
	li	$6,575			# 0x23f
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L64
	li	$2,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	.end	do_splice_from_socket
	.size	do_splice_from_socket, .-do_splice_from_socket
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"ufsd\000"
	.align	2
$LC4:
	.ascii	"fuseblk\000"
	.align	2
$LC5:
	.ascii	"[%s][%d]: Trap %s filesystem\012\000"
	.section	.text.rtl_splice,"ax",@progbits
	.align	2
	.globl	rtl_splice
	.set	nomips16
	.ent	rtl_splice
	.type	rtl_splice, @function
rtl_splice:
	.frame	$sp,56,$31		# vars= 16, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$19,48($sp)
	sw	$18,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	li	$2,-9			# 0xfffffffffffffff7
	move	$18,$5
	addiu	$5,$sp,20
	sw	$6,24($sp)
	sw	$2,20($sp)
	.set	noreorder
	.set	nomacro
	jal	sockfd_lookup
	move	$19,$7
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L92
	lw	$6,24($sp)
	.set	macro
	.set	reorder

	lw	$2,20($2)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L93
	move	$4,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	fget_light
	addiu	$5,$sp,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L93
	move	$17,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_file_mode
	move	$4,$2
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	bne	$2,$0,$L99
$L94:
	lw	$2,16($sp)
$L101:
	bne	$2,$0,$L100
$L93:
	.set	noreorder
	.set	nomacro
	jal	fput
	lw	$4,16($16)
	.set	macro
	.set	reorder

$L92:
	lw	$2,20($sp)
	lw	$31,52($sp)
	lw	$19,48($sp)
	lw	$18,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L100:
	.set	noreorder
	.set	nomacro
	jal	fput
	move	$4,$17
	.set	macro
	.set	reorder

	j	$L93
$L99:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_file_mapping
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_inode_sb
	lw	$4,0($2)
	.set	macro
	.set	reorder

	lw	$2,32($2)
	lui	$3,%hi($LC3)
	addiu	$3,$3,%lo($LC3)
	lw	$7,0($2)
	move	$2,$7
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L95
	lui	$3,%hi($LC4)
	.set	macro
	.set	reorder

	addiu	$3,$3,%lo($LC4)
	move	$2,$7
 #APP
 # 77 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/string.h" 1
	.set	noreorder
	.set	noat
	lbu	$4,($2)
1:	lbu	$1,($3)
	addiu	$2,1
	bne	$1,$4,2f
	addiu	$3,1
	bnez	$4,1b
	lbu	$4,($2)
	move	$4,$1
2:	subu	$4,$1
3:	.set	at
	.set	reorder
 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L95
	move	$6,$18
	.set	macro
	.set	reorder

	move	$7,$19
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	do_splice_from_socket
	move	$5,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L94
	sw	$2,20($sp)
	.set	macro
	.set	reorder

$L95:
	lui	$4,%hi($LC5)
	lui	$5,%hi(__func__.55155)
	addiu	$4,$4,%lo($LC5)
	addiu	$5,$5,%lo(__func__.55155)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,721			# 0x2d1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L101
	lw	$2,16($sp)
	.set	macro
	.set	reorder

	.end	rtl_splice
	.size	rtl_splice, .-rtl_splice
	.section	.rodata.str1.4
	.align	2
$LC6:
	.ascii	"%s: err=-EPIPE!!!\015\012\000"
	.align	2
$LC7:
	.ascii	"*** psize=%d poffset=%d mss_now=%d size_goal=%d ***\012\000"
	.align	2
$LC8:
	.ascii	"%s: goto wait_for_memory\015\012\000"
	.align	2
$LC9:
	.ascii	"%s: skb->len=%d, page_count=%d\015\012\000"
	.align	2
$LC10:
	.ascii	"%s: sk_stream_wait_memory(), err=%d\015\012\000"
	.align	2
$LC11:
	.ascii	"%s: wait_for_memory(), mss_now=%d, size_goal=%d\015\012\000"
	.section	.text.tcp_sendpages,"ax",@progbits
	.align	2
	.globl	tcp_sendpages
	.set	nomips16
	.ent	tcp_sendpages
	.type	tcp_sendpages, @function
tcp_sendpages:
	.frame	$sp,176,$31		# vars= 112, regs= 10/0, args= 24, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-176
	sw	$31,172($sp)
	sw	$fp,168($sp)
	sw	$23,164($sp)
	sw	$22,160($sp)
	sw	$21,156($sp)
	sw	$20,152($sp)
	sw	$19,148($sp)
	sw	$18,144($sp)
	sw	$17,140($sp)
	sw	$16,136($sp)
	addiu	$20,$sp,32
	move	$22,$5
	move	$18,$4
	move	$5,$0
	move	$4,$20
	move	$17,$6
	li	$6,64			# 0x40
	move	$16,$7
	.set	noreorder
	.set	nomacro
	jal	memset
	lw	$19,192($sp)
	.set	macro
	.set	reorder

	lw	$2,36($22)
	lw	$21,80($22)
	move	$4,$18
	sll	$3,$2,5
	sll	$2,$2,3
	subu	$2,$3,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_route_caps
	addu	$21,$21,$2
	.set	macro
	.set	reorder

	andi	$3,$3,0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L158
	move	$23,$0
	.set	macro
	.set	reorder

$L180:
	addu	$20,$23,$17
	li	$2,16384			# 0x4000
	andi	$20,$20,0x3fff
	subu	$20,$2,$20
	subu	$2,$16,$23
	sltu	$3,$2,$20
	move	$fp,$21
	movn	$20,$2,$3
	lw	$2,0($fp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L107
	move	$4,$18
	.set	macro
	.set	reorder

$L161:
	.set	noreorder
	.set	nomacro
	beq	$23,$16,$L109
	move	$2,$23
	.set	macro
	.set	reorder

	jal	rtl_get_sk_socket
	lw	$5,0($fp)
	move	$4,$2
	move	$6,$17
	move	$7,$20
	.set	noreorder
	.set	nomacro
	jal	sock_no_sendpage
	sw	$19,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bltz	$2,$L159
	lw	$31,172($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$20,$2,$L160
	addiu	$4,$fp,24
	.set	macro
	.set	reorder

	addu	$23,$23,$2
$L162:
	addu	$20,$23,$17
	li	$2,16384			# 0x4000
	andi	$20,$20,0x3fff
	subu	$20,$2,$20
	subu	$2,$16,$23
	sltu	$3,$2,$20
	movn	$20,$2,$3
	lw	$2,0($fp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L161
	move	$4,$18
	.set	macro
	.set	reorder

$L107:
	move	$2,$23
$L109:
	lw	$31,172($sp)
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L160:
	lw	$3,32($22)
	addu	$23,$23,$2
	sll	$5,$3,5
	sll	$3,$3,3
	subu	$3,$5,$3
	addu	$3,$21,$3
	sltu	$3,$fp,$3
	.set	noreorder
	.set	nomacro
	j	$L162
	movn	$fp,$4,$3
	.set	macro
	.set	reorder

$L158:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_route_caps
	move	$4,$18
	.set	macro
	.set	reorder

	andi	$3,$3,0x1a
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L180
	move	$23,$0
	.set	macro
	.set	reorder

	lw	$6,32($22)
	.set	noreorder
	.set	nomacro
	beq	$6,$0,$L105
	move	$4,$20
	.set	macro
	.set	reorder

	move	$3,$0
	move	$2,$0
$L112:
	sll	$5,$3,5
	sll	$3,$3,3
	subu	$3,$5,$3
	addu	$3,$21,$3
	lw	$3,0($3)
	addiu	$2,$2,1
	sltu	$5,$2,$6
	sw	$3,0($4)
	addiu	$4,$4,4
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L112
	move	$3,$2
	.set	macro
	.set	reorder

$L105:
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	lock_sock_nested
	move	$5,$0
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_sk_sndtimeo
	move	$5,$19
	.set	macro
	.set	reorder

	lbu	$3,18($18)
	sw	$2,28($sp)
	li	$2,-259			# 0xfffffffffffffefd
	sra	$3,$2,$3
	andi	$3,$3,0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L113
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	sk_stream_wait_connect
	addiu	$5,$sp,28
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L114
	move	$21,$2
	.set	macro
	.set	reorder

$L113:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_socket
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$2,$2,8
$L115:
 #APP
 # 145 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/bitops.h" 1
		ll	$3, 0($2)		# clear_bit	
	ins	$3, $0, 0, 1			
	sc	$3, 0($2)				

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L115
	addiu	$5,$sp,24
	.set	macro
	.set	reorder

	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	tcp_send_mss
	move	$6,$19
	.set	macro
	.set	reorder

	lui	$3,%hi(g2_tcp_ctl)
	addiu	$3,$3,%lo(g2_tcp_ctl)
	sw	$2,112($sp)
	lw	$2,8($3)
	sltu	$4,$2,2
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L117
	sll	$3,$2,14
	.set	macro
	.set	reorder

	li	$3,196608			# 0x30000
	ori	$3,$3,0xffff
	addu	$2,$2,$3
	sll	$3,$2,14
$L117:
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L118
	sw	$3,24($sp)
$L118:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_err
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L181
	lui	$4,%hi($LC6)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_shutdown
	move	$4,$18
	.set	macro
	.set	reorder

	andi	$2,$2,0x2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L119
	lui	$4,%hi($LC6)
	.set	macro
	.set	reorder

	lui	$4,%hi(g2_tcp_ctl)
	addiu	$4,$4,%lo(g2_tcp_ctl)
	lw	$3,4($4)
	li	$2,3			# 0x3
	beq	$3,$2,$L163
$L121:
	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L164
	lw	$7,112($sp)
	.set	macro
	.set	reorder

$L123:
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L125
	move	$2,$0
	.set	macro
	.set	reorder

	li	$2,-65536			# 0xffffffffffff0000
	ori	$2,$2,0x7fff
	and	$2,$19,$2
	sw	$2,124($sp)
	andi	$3,$19,0x1
	srl	$2,$17,14
	sw	$0,116($sp)
	sw	$0,108($sp)
	sw	$0,100($sp)
	sw	$2,104($sp)
	sw	$3,120($sp)
$L157:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_write_queue_tail
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$4,104($sp)
	move	$21,$2
	sll	$2,$4,2
	addu	$2,$20,$2
	lw	$2,0($2)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_get_tcp_send_head
	sw	$2,96($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L126
	lw	$22,24($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len_common
	move	$4,$21
	.set	macro
	.set	reorder

	subu	$2,$22,$2
	blez	$2,$L126
$L127:
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	move	$4,$21
	.set	macro
	.set	reorder

	lbu	$23,0($2)
	lw	$6,96($sp)
	andi	$fp,$17,0x3fff
	move	$4,$21
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_can_coalesce
	move	$7,$fp
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L131
	move	$3,$2
	.set	macro
	.set	reorder

	sltu	$2,$23,16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L165
	move	$5,$21
	.set	macro
	.set	reorder

$L131:
	li	$4,16384			# 0x4000
	subu	$22,$4,$fp
	sltu	$2,$16,$22
	movn	$22,$16,$2
	move	$4,$18
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	rtl_sk_wmem_schedule
	sw	$3,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L130
	lw	$3,128($sp)
	.set	macro
	.set	reorder

	beq	$3,$0,$L132
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	addiu	$23,$23,4
	.set	macro
	.set	reorder

	sll	$23,$23,3
	addu	$2,$2,$23
	lhu	$3,10($2)
	addu	$3,$22,$3
	sh	$3,10($2)
	move	$4,$21
$L175:
	.set	noreorder
	.set	nomacro
	jal	rtl_add_skb_len_common
	move	$5,$22
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_add_skb_data_len_common
	move	$5,$22
	.set	macro
	.set	reorder

	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_add_skb_truesize_common
	move	$5,$22
	.set	macro
	.set	reorder

	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	rtl_add_skb_sk_wmem_queued
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$2,108($18)
	move	$4,$21
	subu	$2,$2,$22
	.set	noreorder
	.set	nomacro
	jal	rtl_set_tcp_skb_ip_summed
	sw	$2,108($18)
	.set	macro
	.set	reorder

	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	rtl_add_tp_write_seq
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_TCP_SKB_CB
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_add_skb_cb_end_seq
	move	$5,$22
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_skb_shared_info
	move	$4,$21
	.set	macro
	.set	reorder

	sh	$0,4($2)
	lw	$2,100($sp)
	beq	$2,$0,$L166
	lw	$3,100($sp)
$L178:
	lw	$4,108($sp)
	subu	$16,$16,$22
	addu	$3,$3,$22
	addiu	$4,$4,1
	sw	$3,100($sp)
	.set	noreorder
	.set	nomacro
	beq	$16,$0,$L167
	sw	$4,108($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len_common
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$3,24($sp)
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L138
	lw	$3,120($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L138
	lw	$4,116($sp)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L139
	lui	$3,%hi(g2_tcp_ctl)
	.set	macro
	.set	reorder

	addiu	$3,$3,%lo(g2_tcp_ctl)
	lw	$2,8($3)
	lw	$4,108($sp)
	sltu	$2,$4,$2
	bne	$2,$0,$L138
$L139:
	.set	noreorder
	.set	nomacro
	jal	forced_push
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L140
	move	$5,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_mark_push
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$5,112($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	__tcp_push_pending_frames
	li	$6,4			# 0x4
	.set	macro
	.set	reorder

	sw	$0,108($sp)
	sw	$0,116($sp)
$L138:
	addu	$17,$17,$22
	srl	$2,$17,14
	.set	noreorder
	.set	nomacro
	j	$L157
	sw	$2,104($sp)
	.set	macro
	.set	reorder

$L159:
	movn	$2,$23,$23
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,176
	.set	macro
	.set	reorder

$L165:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_mark_push
	move	$4,$18
	.set	macro
	.set	reorder

$L126:
	.set	noreorder
	.set	nomacro
	jal	rtl_sk_stream_memory_free
	move	$4,$18
	.set	macro
	.set	reorder

	beq	$2,$0,$L128
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_allocation
	move	$4,$18
	.set	macro
	.set	reorder

	move	$6,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	sk_stream_alloc_skb
	move	$5,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L168
	move	$21,$2
	.set	macro
	.set	reorder

	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_entail
	move	$4,$18
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L127
	sw	$2,116($sp)
	.set	macro
	.set	reorder

$L168:
	lui	$4,%hi($LC8)
	lui	$5,%hi(__func__.54627)
	addiu	$4,$4,%lo($LC8)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.54627)
	.set	macro
	.set	reorder

$L130:
	lw	$2,100($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L169
	lw	$5,124($sp)
	.set	macro
	.set	reorder

	move	$4,$18
$L176:
	.set	noreorder
	.set	nomacro
	jal	sk_stream_wait_memory
	addiu	$5,$sp,28
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L170
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$5,$sp,24
	.set	noreorder
	.set	nomacro
	jal	tcp_send_mss
	move	$6,$19
	.set	macro
	.set	reorder

	lui	$3,%hi(g2_tcp_ctl)
	addiu	$3,$3,%lo(g2_tcp_ctl)
	sw	$2,112($sp)
	lw	$2,8($3)
	sltu	$4,$2,2
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L147
	sll	$3,$2,14
	.set	macro
	.set	reorder

	li	$4,196608			# 0x30000
	ori	$4,$4,0xffff
	addu	$2,$2,$4
	sll	$3,$2,14
$L147:
	lw	$2,24($sp)
	slt	$2,$3,$2
	beq	$2,$0,$L148
	sw	$3,24($sp)
$L148:
	jal	_cond_resched
	lui	$3,%hi(g2_tcp_ctl)
	addiu	$3,$3,%lo(g2_tcp_ctl)
	lw	$2,4($3)
	li	$4,2			# 0x2
	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L157
	lw	$7,24($sp)
	.set	macro
	.set	reorder

	lw	$6,112($sp)
	lui	$4,%hi($LC11)
	lui	$5,%hi(__func__.54627)
	addiu	$4,$4,%lo($LC11)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.54627)
	.set	macro
	.set	reorder

	j	$L157
$L128:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_sk_socket
	move	$4,$18
	.set	macro
	.set	reorder

	addiu	$2,$2,8
	li	$4,-1			# 0xffffffffffffffff
$L174:
 #APP
 # 93 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips/include/asm/bitops.h" 1
		ll	$3, 0($2)		# set_bit	
	ins	$3, $4, 2, 1			
	sc	$3, 0($2)				

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L130
	li	$4,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	j	$L174
$L132:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_page
	lw	$4,96($sp)
	.set	macro
	.set	reorder

	lw	$6,96($sp)
	move	$5,$23
	move	$7,$fp
	move	$4,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_skb_fill_page_desc
	sw	$22,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L175
	move	$4,$21
	.set	macro
	.set	reorder

$L169:
	lw	$6,112($sp)
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_push
	li	$7,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L176
	move	$4,$18
	.set	macro
	.set	reorder

$L170:
	lui	$4,%hi(g2_tcp_ctl)
	addiu	$4,$4,%lo(g2_tcp_ctl)
	lw	$3,4($4)
	move	$21,$2
	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L171
	lui	$4,%hi($LC10)
	.set	macro
	.set	reorder

$L145 = .
	lw	$4,100($sp)
$L179:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L182
	move	$5,$19
	.set	macro
	.set	reorder

$L150:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_tcp_sock_nonagle
	move	$4,$18
	.set	macro
	.set	reorder

	lw	$6,112($sp)
	move	$7,$2
	move	$4,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_push
	move	$5,$19
	.set	macro
	.set	reorder

	lw	$2,100($sp)
$L125:
	move	$4,$18
$L177:
	.set	noreorder
	.set	nomacro
	jal	release_sock
	sw	$2,128($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L109
	lw	$2,128($sp)
	.set	macro
	.set	reorder

$L119:
$L181:
	lui	$5,%hi(__func__.54627)
	addiu	$4,$4,%lo($LC6)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.54627)
	.set	macro
	.set	reorder

	li	$21,-32			# 0xffffffffffffffe0
$L114:
	move	$5,$19
$L182:
	move	$6,$21
	.set	noreorder
	.set	nomacro
	jal	sk_stream_error
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L177
	move	$4,$18
	.set	macro
	.set	reorder

$L166:
	.set	noreorder
	.set	nomacro
	jal	rtl_TCP_SKB_CB
	move	$4,$21
	.set	macro
	.set	reorder

	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	rtl_and_skb_cb_tcp_flags
	li	$5,-9			# 0xfffffffffffffff7
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L178
	lw	$3,100($sp)
	.set	macro
	.set	reorder

$L163:
	lw	$21,24($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_get_tp_snd_wnd
	move	$4,$18
	.set	macro
	.set	reorder

	sltu	$2,$21,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L122
	lui	$2,%hi(g2_tcp_ctl)
	.set	macro
	.set	reorder

	addiu	$2,$2,%lo(g2_tcp_ctl)
	.set	noreorder
	.set	nomacro
	j	$L121
	lw	$3,4($2)
	.set	macro
	.set	reorder

$L140:
	.set	noreorder
	.set	nomacro
	jal	rtl_tcp_send_head
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$21,$2,$L172
	lw	$5,112($sp)
	.set	macro
	.set	reorder

	sw	$0,108($sp)
	.set	noreorder
	.set	nomacro
	j	$L138
	sw	$0,116($sp)
	.set	macro
	.set	reorder

$L167:
	lui	$2,%hi(g2_tcp_ctl)
	addiu	$2,$2,%lo(g2_tcp_ctl)
	lw	$4,4($2)
	move	$2,$3
	li	$3,2			# 0x2
	beq	$4,$3,$L173
$L137:
	lw	$3,100($sp)
	beq	$3,$0,$L125
	j	$L150
$L122:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_tp_snd_wnd
	move	$4,$18
	.set	macro
	.set	reorder

	lui	$4,%hi(g2_tcp_ctl)
	addiu	$4,$4,%lo(g2_tcp_ctl)
	lw	$3,4($4)
	.set	noreorder
	.set	nomacro
	j	$L121
	sw	$2,24($sp)
	.set	macro
	.set	reorder

$L172:
	.set	noreorder
	.set	nomacro
	jal	tcp_push_one
	move	$4,$18
	.set	macro
	.set	reorder

	sw	$0,108($sp)
	.set	noreorder
	.set	nomacro
	j	$L138
	sw	$0,116($sp)
	.set	macro
	.set	reorder

$L164:
	lw	$2,24($sp)
	lui	$4,%hi($LC7)
	addiu	$4,$4,%lo($LC7)
	move	$5,$16
	move	$6,$17
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	j	$L123
$L171:
	lui	$5,%hi(__func__.54627)
	addiu	$4,$4,%lo($LC10)
	addiu	$5,$5,%lo(__func__.54627)
	.set	noreorder
	.set	nomacro
	jal	printk
	move	$6,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L179
	lw	$4,100($sp)
	.set	macro
	.set	reorder

$L173:
	.set	noreorder
	.set	nomacro
	jal	rtl_get_skb_len_common
	move	$4,$21
	.set	macro
	.set	reorder

	lw	$7,108($sp)
	lui	$4,%hi($LC9)
	lui	$5,%hi(__func__.54627)
	addiu	$4,$4,%lo($LC9)
	addiu	$5,$5,%lo(__func__.54627)
	.set	noreorder
	.set	nomacro
	jal	printk
	move	$6,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L137
	lw	$2,100($sp)
	.set	macro
	.set	reorder

	.end	tcp_sendpages
	.size	tcp_sendpages, .-tcp_sendpages
	.section	.text.inet_sendpages,"ax",@progbits
	.align	2
	.globl	inet_sendpages
	.set	nomips16
	.ent	inet_sendpages
	.type	inet_sendpages, @function
inet_sendpages:
	.frame	$sp,48,$31		# vars= 16, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$17,40($sp)
	sw	$16,36($sp)
	lw	$16,20($4)
	lw	$17,64($sp)
	sw	$5,16($sp)
	move	$4,$16
	sw	$6,20($sp)
	jal	rtl_sock_rps_record_flow
	sw	$7,24($sp)

	jal	rtl_get_sk_inet_num
	move	$4,$16

	lw	$5,16($sp)
	lw	$6,20($sp)
	bne	$2,$0,$L184
	lw	$7,24($sp)

	jal	rtl_get_sk_no_autobind
	move	$4,$16

	lw	$5,16($sp)
	lw	$6,20($sp)
	beq	$2,$0,$L187
	lw	$7,24($sp)

$L184:
	move	$4,$16
	sw	$17,64($sp)
	lw	$31,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	j	tcp_sendpages
	addiu	$sp,$sp,48

$L187:
	jal	inet_autobind
	move	$4,$16

	lw	$5,16($sp)
	lw	$6,20($sp)
	beq	$2,$0,$L184
	lw	$7,24($sp)

	lw	$31,44($sp)
	lw	$17,40($sp)
	lw	$16,36($sp)
	li	$2,-11			# 0xfffffffffffffff5
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	inet_sendpages
	.size	inet_sendpages, .-inet_sendpages
	.section	.text.sock_sendpages,"ax",@progbits
	.align	2
	.globl	sock_sendpages
	.set	nomips16
	.ent	sock_sendpages
	.type	sock_sendpages, @function
sock_sendpages:
	.frame	$sp,48,$31		# vars= 16, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-48
	sw	$31,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$16,$4
	lw	$18,68($sp)
	sw	$5,16($sp)
	sw	$6,20($sp)
	jal	rtl_get_file_private_data
	sw	$7,24($sp)

	move	$4,$16
	jal	rtl_get_file_flags
	move	$17,$2

	andi	$2,$2,0x80
	li	$3,64			# 0x40
	movz	$3,$0,$2
	ori	$2,$3,0x8000
	movn	$3,$2,$18
	move	$4,$17
	lw	$5,16($sp)
	lw	$6,20($sp)
	lw	$7,24($sp)
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	sw	$3,64($sp)
	j	inet_sendpages
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	sock_sendpages
	.size	sock_sendpages, .-sock_sendpages
	.section	.rodata.str1.4
	.align	2
$LC12:
	.ascii	"\0013%s confirm err=%d\012\000"
	.section	.text.pipe_to_sendpages,"ax",@progbits
	.align	2
	.globl	pipe_to_sendpages
	.set	nomips16
	.ent	pipe_to_sendpages
	.type	pipe_to_sendpages, @function
pipe_to_sendpages:
	.frame	$sp,64,$31		# vars= 8, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$22,56($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$19,44($sp)
	sw	$18,40($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	lw	$7,20($6)
	move	$16,$6
	lw	$6,16($6)
	lw	$2,32($4)
	sw	$7,28($sp)
	sw	$6,24($sp)
	move	$20,$4
	move	$21,$5
	beq	$2,$0,$L194
	lw	$22,12($16)

	move	$18,$5
	move	$19,$0
$L197:
	lw	$2,12($18)
	move	$5,$18
	move	$4,$20
	lw	$2,12($2)
	addiu	$19,$19,1
	jalr	$2
	addiu	$18,$18,24

	bne	$2,$0,$L201
	move	$17,$2

	lw	$2,32($20)
	sltu	$2,$19,$2
	bne	$2,$0,$L197
	nop

$L194:
	lw	$7,4($16)
	lw	$2,8($16)
	addiu	$3,$sp,24
	sw	$7,0($16)
	lw	$6,4($21)
	ext	$2,$2,2,1
	move	$4,$22
	move	$5,$20
	sw	$3,16($sp)
	jal	sock_sendpages
	sw	$2,20($sp)

	bltz	$2,$L196
	move	$17,$2

	lw	$3,20($16)
	lw	$8,16($16)
	lw	$5,28($16)
	lw	$4,0($16)
	lw	$6,4($16)
	addu	$2,$3,$2
	sra	$7,$17,31
	addu	$7,$8,$7
	sltu	$3,$2,$3
	addu	$3,$3,$7
	addu	$5,$5,$17
	subu	$4,$4,$17
	subu	$6,$6,$17
	sw	$6,4($16)
	sw	$5,28($16)
	sw	$4,0($16)
	sw	$3,16($16)
	sw	$2,20($16)
$L196:
	move	$2,$17
$L202:
	lw	$31,60($sp)
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,64

$L201:
	lui	$4,%hi($LC12)
	lui	$5,%hi(__func__.54911)
	addiu	$4,$4,%lo($LC12)
	addiu	$5,$5,%lo(__func__.54911)
	jal	printk
	move	$6,$2

	j	$L202
	move	$2,$17

	.set	macro
	.set	reorder
	.end	pipe_to_sendpages
	.size	pipe_to_sendpages, .-pipe_to_sendpages
	.globl	rtl_use_sendfile
	.section	.data.rtl_use_sendfile,"aw",@progbits
	.align	2
	.type	rtl_use_sendfile, @object
	.size	rtl_use_sendfile, 4
rtl_use_sendfile:
	.word	16
	.section	.rodata.__func__.55155,"a",@progbits
	.align	2
	.type	__func__.55155, @object
	.size	__func__.55155, 11
__func__.55155:
	.ascii	"rtl_splice\000"
	.section	.rodata.__func__.55017,"a",@progbits
	.align	2
	.type	__func__.55017, @object
	.size	__func__.55017, 22
__func__.55017:
	.ascii	"do_splice_from_socket\000"
	.section	.rodata.__func__.54911,"a",@progbits
	.align	2
	.type	__func__.54911, @object
	.size	__func__.54911, 18
__func__.54911:
	.ascii	"pipe_to_sendpages\000"
	.section	.rodata.__func__.54627,"a",@progbits
	.align	2
	.type	__func__.54627, @object
	.size	__func__.54627, 19
__func__.54627:
	.ascii	"do_tcp_send_mpages\000"
	.section	.data..percpu,"aw",@progbits
	.align	2
	.type	bdp_ratelimits, @object
	.size	bdp_ratelimits, 4
bdp_ratelimits:
	.space	4
	.globl	g2_tcp_ctl
	.section	.bss.g2_tcp_ctl,"aw",@nobits
	.align	2
	.type	g2_tcp_ctl, @object
	.size	g2_tcp_ctl, 32
g2_tcp_ctl:
	.space	32
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 1459) 4.4.7"
