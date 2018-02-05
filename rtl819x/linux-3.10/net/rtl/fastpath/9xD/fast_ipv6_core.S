	.file	1 "fast_ipv6_core.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.V6_cache_timeout,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	V6_cache_timeout
	.type	V6_cache_timeout, @function
V6_cache_timeout:
	.frame	$sp,40,$31		# vars= 32, regs= 2/0, args= 0, gp= 0
	.mask	0x00030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-40
	sw	$17,36($sp)
	sw	$16,32($sp)
	lbu	$2,48($4)
	beq	$2,$0,$L1
	lui	$14,%hi(V6_max_udp_frag_entry)

	lw	$7,4($4)
	lw	$6,8($4)
	lw	$5,12($4)
	srl	$17,$7,8
	lw	$3,16($4)
	xor	$16,$6,$7
	lw	$2,20($4)
	xor	$16,$16,$5
	lw	$13,24($4)
	xor	$16,$16,$3
	lw	$12,28($4)
	xor	$16,$16,$2
	lw	$11,32($4)
	xor	$16,$16,$13
	xor	$16,$16,$12
	sw	$7,0($sp)
	xor	$16,$16,$11
	sw	$6,4($sp)
	xor	$16,$16,$17
	sw	$5,8($sp)
	srl	$7,$6,8
	sw	$3,12($sp)
	xor	$6,$16,$7
	sw	$2,16($sp)
	srl	$5,$5,8
	lw	$24,0($4)
	xor	$5,$6,$5
	lhu	$25,38($4)
	srl	$3,$3,8
	lw	$15,%lo(V6_max_udp_frag_entry)($14)
	xor	$3,$5,$3
	lw	$14,80($4)
	srl	$2,$2,16
	addiu	$15,$15,-1
	xor	$2,$3,$2
	sw	$13,20($sp)
	srl	$10,$13,16
	sw	$12,24($sp)
	xor	$2,$2,$10
	sw	$11,28($sp)
	srl	$9,$12,16
	sb	$0,48($4)
	xor	$2,$2,$9
	srl	$8,$11,16
	xor	$2,$2,$8
	xor	$5,$25,$24
	srl	$3,$2,16
	xor	$2,$5,$2
	xor	$2,$2,$3
	beq	$14,$0,$L3
	and	$2,$2,$15

	lw	$7,84($4)
	sll	$6,$2,2
	sll	$5,$2,4
	lui	$3,%hi(V6_udp_cache_table)
	sw	$7,84($14)
	subu	$5,$5,$6
$L4:
	lw	$7,84($4)
	lui	$2,%hi(V6_udp_cache_list_free)
	lw	$8,80($4)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	addiu	$6,$4,88
	sw	$8,0($7)
	lw	$7,%lo(V6_udp_cache_table)($3)
	lw	$3,4($2)
	lw	$7,0($7)
	addu	$5,$7,$5
	lw	$7,8($5)
	addiu	$7,$7,-1
	sw	$7,8($5)
	sw	$3,92($4)
	sw	$0,88($4)
	sw	$4,0($3)
	lw	$3,8($2)
	sw	$6,4($2)
	addiu	$3,$3,1
	sw	$3,8($2)
$L1:
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,40

$L3:
	lui	$3,%hi(V6_udp_cache_table)
	sll	$7,$2,2
	lw	$6,%lo(V6_udp_cache_table)($3)
	sll	$5,$2,4
	subu	$5,$5,$7
	lw	$2,0($6)
	lw	$6,84($4)
	addu	$2,$2,$5
	j	$L4
	sw	$6,4($2)

	.set	macro
	.set	reorder
	.end	V6_cache_timeout
	.size	V6_cache_timeout, .-V6_cache_timeout
	.section	.text.ipv6_fast_forward_single_open,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_single_open
	.set	nomips16
	.ent	ipv6_fast_forward_single_open
	.type	ipv6_fast_forward_single_open, @function
ipv6_fast_forward_single_open:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	move	$4,$5
	lui	$5,%hi(ipv6_forward_read_proc)
	move	$6,$0
	addiu	$5,$5,%lo(ipv6_forward_read_proc)
	j	single_open
	nop

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_single_open
	.size	ipv6_fast_forward_single_open, .-ipv6_fast_forward_single_open
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%c\012\000"
	.section	.text.ipv6_forward_read_proc,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	ipv6_forward_read_proc
	.type	ipv6_forward_read_proc, @function
ipv6_forward_read_proc:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	lw	$6,%lo(fast_ipv6_fw)($2)
	lui	$5,%hi($LC0)
	addiu	$5,$5,%lo($LC0)
	jal	seq_printf
	addiu	$6,$6,48

	move	$2,$0
	lw	$31,20($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ipv6_forward_read_proc
	.size	ipv6_forward_read_proc, .-ipv6_forward_read_proc
	.section	.text.rtk_addV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_addV6Connection
	.set	nomips16
	.ent	rtk_addV6Connection
	.type	rtk_addV6Connection, @function
rtk_addV6Connection:
	.frame	$sp,176,$31		# vars= 120, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
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
	move	$19,$5
	move	$16,$4
	jal	rtl_store_skb_ipv6_dst
	move	$4,$5

	jal	rtl_get_skb_dev
	move	$4,$19

	move	$4,$19
	jal	rtl_ip6_route_input
	move	$fp,$2

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L32
	nop

	jal	rtl_get_skb_ipv6_dst
	move	$4,$19

	beq	$2,$0,$L36
	move	$4,$19

	jal	rtl_get_skb_ipv6_dst_dev
	nop

	move	$4,$19
	jal	rtl_dst_release
	sw	$2,124($sp)

	jal	rtl_set_skb_ipv6_dst
	move	$4,$19

$L11:
	lw	$4,0($16)
	lui	$23,%hi(v6_path_table_list_max)
	lw	$25,40($16)
	lui	$20,%hi(v6_table_path)
	lw	$17,4($16)
	srl	$24,$4,8
	lw	$15,8($16)
	xor	$25,$4,$25
	lw	$13,12($16)
	xor	$25,$25,$17
	lw	$3,16($16)
	xor	$25,$25,$15
	lw	$11,20($16)
	xor	$25,$25,$13
	lw	$10,24($16)
	xor	$25,$25,$3
	lw	$8,28($16)
	xor	$25,$25,$11
	xor	$25,$25,$10
	sw	$3,48($sp)
	xor	$25,$25,$8
	lw	$2,36($16)
	srl	$14,$17,8
	lhu	$22,34($16)
	xor	$24,$25,$24
	lw	$21,%lo(v6_path_table_list_max)($23)
	xor	$14,$24,$14
	lw	$18,%lo(v6_table_path)($20)
	srl	$12,$15,8
	addiu	$21,$21,-1
	xor	$12,$14,$12
	lw	$18,0($18)
	srl	$9,$13,8
	sw	$17,68($sp)
	xor	$9,$12,$9
	sw	$4,64($sp)
	srl	$3,$3,16
	sw	$15,72($sp)
	xor	$3,$9,$3
	sw	$13,76($sp)
	srl	$7,$11,16
	sw	$11,52($sp)
	xor	$7,$3,$7
	sw	$10,56($sp)
	srl	$6,$10,16
	sw	$8,60($sp)
	xor	$6,$7,$6
	srl	$5,$8,16
	srl	$3,$2,4
	xor	$5,$6,$5
	xor	$5,$5,$3
	xor	$22,$5,$22
	srl	$3,$22,12
	xor	$22,$3,$22
	and	$21,$22,$21
	sll	$3,$21,2
	sll	$21,$21,4
	subu	$21,$21,$3
	addu	$18,$18,$21
	lw	$17,0($18)
	beq	$17,$0,$L12
	addiu	$18,$16,16

	j	$L33
	nop

$L13:
	lw	$17,52($17)
	beq	$17,$0,$L40
	lui	$22,%hi(v6_path_list_free)

	lw	$2,36($16)
$L33:
	lw	$3,36($17)
	bne	$3,$2,$L13
	nop

	lw	$3,40($17)
	lw	$2,40($16)
	bne	$3,$2,$L13
	move	$4,$17

	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$16

	bne	$2,$0,$L13
	addiu	$4,$17,16

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L13
	nop

	lw	$2,32($17)
	lw	$4,32($16)
	bne	$2,$4,$L13
	nop

	lw	$8,%lo(v6_table_path)($20)
	j	$L14
	nop

$L12:
	lui	$22,%hi(v6_path_list_free)
$L40:
	lw	$18,%lo(v6_path_list_free)($22)
	beq	$18,$0,$L37
	nop

	jal	rtl_get_skb_protocol
	move	$4,$19

	move	$4,$19
	jal	rtl_ipv6_hdr
	sw	$2,128($sp)

	move	$4,$19
	jal	rtl_get_skb_mark
	move	$17,$2

	lw	$9,20($17)
	move	$4,$19
	lw	$6,32($17)
	move	$5,$0
	sw	$9,108($sp)
	lhu	$9,40($17)
	sw	$2,112($sp)
	lw	$12,8($17)
	lw	$2,36($17)
	lw	$11,12($17)
	lw	$10,16($17)
	lw	$8,24($17)
	lw	$7,28($17)
	sw	$9,116($sp)
	lhu	$9,42($17)
	sw	$6,88($sp)
	sw	$2,92($sp)
	sw	$12,96($sp)
	sw	$11,100($sp)
	sw	$10,104($sp)
	sw	$8,80($sp)
	sw	$7,84($sp)
	jal	rtl_set_skb_mark
	sw	$9,120($sp)

	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	lw	$2,12($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,36($16)
	sh	$2,40($17)
	lw	$2,28($16)
	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,40($16)
	sh	$2,42($17)
	li	$2,34525			# 0x86dd
	lw	$3,128($sp)
	beq	$3,$2,$L38
	lui	$3,%hi(nf_hooks)

$L17:
	lw	$5,112($sp)
	jal	rtl_set_skb_mark
	move	$4,$19

	lw	$2,96($sp)
	lw	$9,116($sp)
	sw	$2,8($17)
	lw	$2,100($sp)
	sh	$9,40($17)
	sw	$2,12($17)
	lw	$2,104($sp)
	lw	$9,120($sp)
	sw	$2,16($17)
	lw	$2,108($sp)
	sh	$9,42($17)
	sw	$2,20($17)
	lw	$2,80($sp)
	sw	$2,24($17)
	lw	$2,84($sp)
	sw	$2,28($17)
	lw	$2,88($sp)
	sw	$2,32($17)
	lw	$2,92($sp)
	sw	$2,36($17)
	lw	$5,0($16)
	lw	$4,4($16)
	lw	$3,8($16)
	lw	$2,12($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$3,24($16)
	lw	$2,28($16)
	lw	$4,20($16)
	lw	$5,16($16)
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$3,24($18)
	lui	$3,%hi(jiffies)
	sw	$2,28($18)
	lw	$2,32($16)
	lw	$4,%lo(jiffies)($3)
	sw	$2,32($18)
	lw	$3,36($16)
	lw	$2,60($18)
	sw	$3,36($18)
	lw	$3,40($16)
	sw	$4,44($18)
	beq	$2,$0,$L20
	sw	$3,40($18)

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
	lw	$2,64($18)
	lw	$3,60($18)
$L21:
	sw	$3,0($2)
	lui	$2,%hi(v6_path_list_inuse)
	lw	$6,8($22)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	lw	$3,4($2)
	addiu	$5,$18,60
	addiu	$6,$6,-1
	addiu	$4,$18,52
	sw	$3,64($18)
	sw	$0,60($18)
	sw	$6,8($22)
	sw	$18,0($3)
	lw	$6,8($2)
	lw	$3,%lo(v6_table_path)($20)
	addiu	$6,$6,1
	sw	$0,52($18)
	sw	$5,4($2)
	sw	$6,8($2)
	lw	$2,0($3)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$2,56($18)
	lw	$2,0($3)
	addu	$2,$2,$21
	lw	$2,4($2)
	sw	$18,0($2)
	lw	$8,%lo(v6_table_path)($20)
	lw	$2,0($8)
	addu	$2,$2,$21
	sw	$4,4($2)
	lw	$2,0($8)
	addu	$21,$2,$21
	lw	$2,8($21)
	addiu	$2,$2,1
	sw	$2,8($21)
	lw	$4,32($16)
$L14:
	lw	$7,16($16)
	andi	$17,$4,0xffff
	lw	$21,36($16)
	lw	$14,20($16)
	srl	$18,$7,8
	lw	$12,24($16)
	xor	$21,$7,$21
	lw	$11,28($16)
	xor	$21,$21,$14
	lw	$6,0($16)
	xor	$21,$21,$12
	lw	$5,4($16)
	xor	$21,$21,$11
	lw	$4,8($16)
	xor	$21,$21,$6
	lw	$3,12($16)
	xor	$21,$21,$5
	xor	$21,$21,$4
	sw	$7,32($sp)
	xor	$21,$21,$3
	sw	$6,16($sp)
	xor	$7,$21,$18
	sw	$5,20($sp)
	srl	$13,$14,8
	lw	$2,40($16)
	xor	$13,$7,$13
	sw	$4,24($sp)
	srl	$10,$12,8
	sw	$3,28($sp)
	xor	$10,$13,$10
	lw	$15,%lo(v6_path_table_list_max)($23)
	srl	$9,$11,8
	lw	$8,0($8)
	xor	$9,$10,$9
	addiu	$15,$15,-1
	srl	$6,$6,16
	sw	$14,36($sp)
	xor	$6,$9,$6
	sw	$12,40($sp)
	srl	$5,$5,16
	sw	$11,44($sp)
	xor	$5,$6,$5
	srl	$4,$4,16
	xor	$4,$5,$4
	srl	$3,$3,16
	xor	$4,$4,$3
	srl	$5,$2,4
	xor	$4,$4,$5
	xor	$4,$4,$17
	srl	$3,$4,12
	xor	$3,$3,$4
	and	$3,$3,$15
	sll	$4,$3,2
	sll	$21,$3,4
	subu	$21,$21,$4
	addu	$3,$8,$21
	lw	$17,0($3)
	beq	$17,$0,$L22
	addiu	$18,$16,16

	j	$L34
	nop

$L23:
	lw	$17,52($17)
	beq	$17,$0,$L41
	lui	$22,%hi(v6_path_list_free)

	lw	$2,40($16)
$L34:
	lw	$3,36($17)
	bne	$3,$2,$L23
	nop

	lw	$3,40($17)
	lw	$2,36($16)
	bne	$3,$2,$L23
	move	$4,$17

	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$18

	bne	$2,$0,$L23
	addiu	$4,$17,16

	move	$5,$16
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L23
	nop

	lw	$3,32($17)
	lw	$2,32($16)
	bne	$3,$2,$L23
	nop

$L32:
	lw	$31,172($sp)
	move	$2,$0
	lw	$fp,168($sp)
	lw	$23,164($sp)
	lw	$22,160($sp)
	lw	$21,156($sp)
	lw	$20,152($sp)
	lw	$19,148($sp)
	lw	$18,144($sp)
	lw	$17,140($sp)
	lw	$16,136($sp)
	j	$31
	addiu	$sp,$sp,176

$L22:
	lui	$22,%hi(v6_path_list_free)
$L41:
	lw	$18,%lo(v6_path_list_free)($22)
	beq	$18,$0,$L32
	nop

	jal	rtl_get_skb_protocol
	move	$4,$19

	move	$4,$19
	jal	rtl_ipv6_hdr
	sw	$2,128($sp)

	move	$4,$19
	jal	rtl_get_skb_mark
	move	$17,$2

	lw	$9,20($17)
	move	$4,$19
	lw	$6,32($17)
	sw	$2,112($sp)
	lw	$12,8($17)
	lw	$2,36($17)
	lw	$11,12($17)
	lw	$10,16($17)
	lw	$8,24($17)
	lw	$7,28($17)
	sw	$9,108($sp)
	lhu	$9,40($17)
	sw	$6,88($sp)
	sw	$2,92($sp)
	sw	$12,96($sp)
	sw	$11,100($sp)
	sw	$10,104($sp)
	sw	$9,116($sp)
	sw	$8,80($sp)
	sw	$7,84($sp)
	lhu	$23,42($17)
	jal	rtl_set_skb_mark
	move	$5,$0

	lw	$6,16($16)
	lw	$5,20($16)
	lw	$4,24($16)
	lw	$2,28($16)
	sw	$6,8($17)
	sw	$5,12($17)
	sw	$4,16($17)
	sw	$2,20($17)
	lw	$2,40($16)
	sh	$2,40($17)
	lw	$2,12($16)
	lw	$6,0($16)
	lw	$5,4($16)
	lw	$4,8($16)
	sw	$6,24($17)
	sw	$5,28($17)
	sw	$4,32($17)
	sw	$2,36($17)
	lw	$2,36($16)
	sh	$2,42($17)
	li	$2,34525			# 0x86dd
	lw	$3,128($sp)
	beq	$3,$2,$L39
	lui	$3,%hi(nf_hooks)

$L25:
	lw	$5,112($sp)
	jal	rtl_set_skb_mark
	move	$4,$19

	lw	$2,96($sp)
	lw	$9,116($sp)
	sw	$2,8($17)
	lw	$2,100($sp)
	sh	$9,40($17)
	sw	$2,12($17)
	lw	$2,104($sp)
	sh	$23,42($17)
	sw	$2,16($17)
	lw	$2,108($sp)
	sw	$2,20($17)
	lw	$2,80($sp)
	sw	$2,24($17)
	lw	$2,84($sp)
	sw	$2,28($17)
	lw	$2,88($sp)
	sw	$2,32($17)
	lw	$2,92($sp)
	sw	$2,36($17)
	lw	$5,16($16)
	lw	$4,20($16)
	lw	$3,24($16)
	lw	$2,28($16)
	sw	$5,0($18)
	sw	$4,4($18)
	sw	$3,8($18)
	sw	$2,12($18)
	lw	$2,12($16)
	lw	$4,4($16)
	lw	$3,8($16)
	lw	$5,0($16)
	sw	$4,20($18)
	sw	$5,16($18)
	sw	$3,24($18)
	lui	$3,%hi(jiffies)
	sw	$2,28($18)
	lw	$2,32($16)
	lw	$4,%lo(jiffies)($3)
	sw	$2,32($18)
	lw	$3,40($16)
	lw	$2,60($18)
	sw	$3,36($18)
	lw	$3,36($16)
	sw	$4,44($18)
	beq	$2,$0,$L28
	sw	$3,40($18)

	lw	$3,64($18)
	lui	$22,%hi(v6_path_list_free)
	addiu	$22,$22,%lo(v6_path_list_free)
	sw	$3,64($2)
	lw	$2,64($18)
	lw	$3,60($18)
$L29:
	sw	$3,0($2)
	lui	$3,%hi(v6_path_list_inuse)
	lw	$2,8($22)
	addiu	$3,$3,%lo(v6_path_list_inuse)
	lw	$4,4($3)
	addiu	$6,$18,60
	addiu	$2,$2,-1
	addiu	$5,$18,52
	sw	$2,8($22)
	li	$2,1			# 0x1
	sw	$4,64($18)
	sw	$0,60($18)
	sw	$18,0($4)
	lw	$7,8($3)
	lw	$4,%lo(v6_table_path)($20)
	addiu	$7,$7,1
	sw	$0,52($18)
	sw	$7,8($3)
	sw	$6,4($3)
	lw	$3,0($4)
	addu	$3,$3,$21
	lw	$3,4($3)
	sw	$3,56($18)
	lw	$3,0($4)
	addu	$3,$3,$21
	lw	$3,4($3)
	sw	$18,0($3)
	lw	$3,%lo(v6_table_path)($20)
	lw	$4,0($3)
	addu	$4,$4,$21
	sw	$5,4($4)
	lw	$3,0($3)
	addu	$21,$3,$21
	lw	$3,8($21)
	addiu	$3,$3,1
	sw	$3,8($21)
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
	j	$31
	addiu	$sp,$sp,176

$L37:
	lw	$4,32($16)
	lw	$8,%lo(v6_table_path)($20)
	j	$L14
	nop

$L39:
	lui	$2,%hi(nf_hooks+640)
	addiu	$3,$3,%lo(nf_hooks)
	addiu	$2,$2,%lo(nf_hooks+640)
	lw	$4,640($3)
	beq	$4,$2,$L26
	move	$4,$19

	lw	$6,124($sp)
	move	$5,$0
	move	$7,$0
	jal	rtl_ipt6_do_table
	sw	$3,128($sp)

	lw	$3,128($sp)
$L26:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,672($3)
	addiu	$2,$2,%lo(nf_hooks+672)
	beq	$3,$2,$L27
	move	$4,$19

	li	$5,4			# 0x4
	move	$6,$0
	jal	rtl_ipt6_do_table
	move	$7,$fp

$L27:
	jal	rtl_get_skb_mark
	move	$4,$19

	j	$L25
	sw	$2,48($18)

$L38:
	lui	$2,%hi(nf_hooks+640)
	addiu	$3,$3,%lo(nf_hooks)
	addiu	$2,$2,%lo(nf_hooks+640)
	lw	$4,640($3)
	beq	$4,$2,$L18
	move	$4,$19

	move	$5,$0
	move	$6,$fp
	move	$7,$0
	jal	rtl_ipt6_do_table
	sw	$3,128($sp)

	lw	$3,128($sp)
$L18:
	lui	$2,%hi(nf_hooks+672)
	lw	$3,672($3)
	addiu	$2,$2,%lo(nf_hooks+672)
	beq	$3,$2,$L19
	move	$4,$19

	lw	$7,124($sp)
	li	$5,4			# 0x4
	jal	rtl_ipt6_do_table
	move	$6,$0

$L19:
	jal	rtl_get_skb_mark
	move	$4,$19

	j	$L17
	sw	$2,48($18)

$L36:
	jal	rtl_set_skb_ipv6_dst
	sw	$0,124($sp)

	j	$L11
	nop

$L28:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	move	$3,$0
	j	$L29
	sw	$2,4($22)

$L20:
	lw	$2,64($18)
	addiu	$22,$22,%lo(v6_path_list_free)
	move	$3,$0
	j	$L21
	sw	$2,4($22)

	.set	macro
	.set	reorder
	.end	rtk_addV6Connection
	.size	rtk_addV6Connection, .-rtk_addV6Connection
	.section	.text.rtk_delV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_delV6Connection
	.set	nomips16
	.ent	rtk_delV6Connection
	.type	rtk_delV6Connection, @function
rtk_delV6Connection:
	.frame	$sp,80,$31		# vars= 32, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$22,72($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	move	$17,$4
	lw	$7,0($4)
	lui	$14,%hi(v6_path_table_list_max)
	lw	$18,40($4)
	lui	$19,%hi(v6_table_path)
	lw	$12,4($4)
	srl	$20,$7,8
	lw	$10,8($4)
	xor	$18,$7,$18
	lw	$9,12($4)
	xor	$18,$18,$12
	lw	$6,16($4)
	xor	$18,$18,$10
	lw	$5,20($4)
	xor	$18,$18,$9
	lw	$4,24($4)
	xor	$18,$18,$6
	lw	$3,28($17)
	xor	$18,$18,$5
	xor	$18,$18,$4
	sw	$6,16($sp)
	xor	$18,$18,$3
	sw	$5,20($sp)
	xor	$18,$18,$20
	lw	$2,36($17)
	srl	$13,$12,8
	sw	$4,24($sp)
	xor	$13,$18,$13
	sw	$3,28($sp)
	srl	$11,$10,8
	lhu	$16,34($17)
	xor	$11,$13,$11
	lw	$15,%lo(v6_path_table_list_max)($14)
	srl	$8,$9,8
	lw	$14,%lo(v6_table_path)($19)
	xor	$8,$11,$8
	addiu	$15,$15,-1
	srl	$6,$6,16
	lw	$14,0($14)
	xor	$6,$8,$6
	sw	$7,32($sp)
	srl	$5,$5,16
	sw	$12,36($sp)
	xor	$5,$6,$5
	sw	$10,40($sp)
	srl	$4,$4,16
	sw	$9,44($sp)
	srl	$3,$3,16
	xor	$4,$5,$4
	xor	$4,$4,$3
	srl	$6,$2,4
	xor	$4,$4,$6
	xor	$4,$4,$16
	srl	$3,$4,12
	xor	$3,$3,$4
	and	$3,$3,$15
	sll	$4,$3,2
	sll	$18,$3,4
	subu	$18,$18,$4
	addu	$3,$14,$18
	lw	$16,0($3)
	beq	$16,$0,$L50
	addiu	$20,$17,16

	lui	$22,%hi(v6_path_list_free)
	j	$L51
	lui	$21,%hi(v6_path_list_inuse)

$L44:
	lw	$16,52($16)
	beq	$16,$0,$L50
	nop

	lw	$2,36($17)
$L51:
	lw	$3,36($16)
	bne	$2,$3,$L44
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	bne	$3,$2,$L44
	move	$4,$16

	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$17

	bne	$2,$0,$L44
	addiu	$4,$16,16

	move	$5,$20
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L44
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L44
	nop

	lw	$2,52($16)
	beq	$2,$0,$L45
	nop

	lw	$3,56($16)
	sw	$3,56($2)
$L46:
	lw	$2,56($16)
	lw	$3,52($16)
	sw	$3,0($2)
	lw	$2,%lo(v6_table_path)($19)
	lw	$2,0($2)
	addu	$2,$2,$18
	lw	$3,8($2)
	addiu	$3,$3,-1
	sw	$3,8($2)
	lw	$2,60($16)
	beq	$2,$0,$L47
	addiu	$3,$21,%lo(v6_path_list_inuse)

	lw	$4,64($16)
	sw	$4,64($2)
	lw	$2,64($16)
	lw	$4,60($16)
$L48:
	sw	$4,0($2)
	addiu	$2,$22,%lo(v6_path_list_free)
	lw	$6,8($3)
	addiu	$5,$16,60
	lw	$4,4($2)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$0,60($16)
	sw	$4,64($16)
	sw	$16,0($4)
	lw	$3,8($2)
	sw	$5,4($2)
	addiu	$3,$3,1
	j	$L44
	sw	$3,8($2)

$L50:
	lw	$31,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,80

$L45:
	lw	$2,%lo(v6_table_path)($19)
	lw	$3,56($16)
	lw	$2,0($2)
	addu	$2,$2,$18
	j	$L46
	sw	$3,4($2)

$L47:
	lw	$2,64($16)
	move	$4,$0
	j	$L48
	sw	$2,4($3)

	.set	macro
	.set	reorder
	.end	rtk_delV6Connection
	.size	rtk_delV6Connection, .-rtk_delV6Connection
	.section	.text.rtk_flushV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_flushV6Connection
	.set	nomips16
	.ent	rtk_flushV6Connection
	.type	rtk_flushV6Connection, @function
rtk_flushV6Connection:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$2,%hi(v6_path_table_list_max)
	lw	$11,%lo(v6_path_table_list_max)($2)
	beq	$11,$0,$L66
	nop

	move	$8,$0
	move	$10,$0
	lui	$9,%hi(v6_table_path)
	lui	$13,%hi(v6_path_list_inuse)
	lui	$12,%hi(v6_path_list_free)
$L60:
	lw	$2,%lo(v6_table_path)($9)
	lw	$2,0($2)
	addu	$2,$2,$8
	lw	$2,0($2)
	beq	$2,$0,$L54
	nop

	addiu	$5,$13,%lo(v6_path_list_inuse)
	j	$L59
	addiu	$3,$12,%lo(v6_path_list_free)

$L64:
	lw	$6,56($2)
	sw	$6,56($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($9)
	lw	$4,0($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	beq	$4,$0,$L57
	nop

$L65:
	lw	$6,64($2)
	sw	$6,64($4)
	lw	$4,64($2)
	lw	$6,60($2)
	sw	$6,0($4)
	addiu	$6,$2,60
	lw	$7,8($5)
	lw	$4,4($3)
	addiu	$7,$7,-1
	sw	$4,64($2)
	sw	$0,60($2)
	sw	$7,8($5)
	sw	$2,0($4)
	lw	$4,8($3)
	lw	$2,52($2)
	addiu	$4,$4,1
	sw	$6,4($3)
	beq	$2,$0,$L54
	sw	$4,8($3)

$L59:
	lw	$4,52($2)
	bne	$4,$0,$L64
	nop

	lw	$4,%lo(v6_table_path)($9)
	lw	$6,56($2)
	lw	$4,0($4)
	addu	$4,$4,$8
	sw	$6,4($4)
	lw	$4,56($2)
	lw	$6,52($2)
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($9)
	lw	$4,0($4)
	addu	$4,$4,$8
	lw	$6,8($4)
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	bne	$4,$0,$L65
	nop

$L57:
	lw	$4,64($2)
	move	$6,$0
	sw	$4,4($5)
	sw	$6,0($4)
	addiu	$6,$2,60
	lw	$7,8($5)
	lw	$4,4($3)
	addiu	$7,$7,-1
	sw	$4,64($2)
	sw	$0,60($2)
	sw	$7,8($5)
	sw	$2,0($4)
	lw	$4,8($3)
	lw	$2,52($2)
	addiu	$4,$4,1
	sw	$6,4($3)
	bne	$2,$0,$L59
	sw	$4,8($3)

$L54:
	addiu	$10,$10,1
	bne	$11,$10,$L60
	addiu	$8,$8,12

$L66:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	rtk_flushV6Connection
	.size	rtk_flushV6Connection, .-rtk_flushV6Connection
	.section	.text.ipv6_fast_forward_single_write,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	ipv6_fast_forward_single_write
	.type	ipv6_fast_forward_single_write, @function
ipv6_fast_forward_single_write:
	.frame	$sp,32,$31		# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	sltu	$3,$6,2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L73
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L68
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$3,$5,1
	or	$2,$3,$5
	lw	$3,24($28)
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L73
	addiu	$4,$sp,16
	.set	macro
	.set	reorder

	li	$6,1			# 0x1
 #APP
 # 1181 "net/rtl/fastpath/fast_ipv6_core.c" 1
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
	bne	$6,$0,$L68
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	lb	$2,16($sp)
	lui	$3,%hi(fast_ipv6_fw)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L74
	sw	$2,%lo(fast_ipv6_fw)($3)
	.set	macro
	.set	reorder

	move	$2,$16
$L68:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L74:
	jal	rtk_flushV6Connection
	.set	noreorder
	.set	nomacro
	j	$L68
	move	$2,$16
	.set	macro
	.set	reorder

$L73:
	.set	noreorder
	.set	nomacro
	j	$L68
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	.end	ipv6_fast_forward_single_write
	.size	ipv6_fast_forward_single_write, .-ipv6_fast_forward_single_write
	.section	.text.rtk_idleV6Connection,"ax",@progbits
	.align	2
	.globl	rtk_idleV6Connection
	.set	nomips16
	.ent	rtk_idleV6Connection
	.type	rtk_idleV6Connection, @function
rtk_idleV6Connection:
	.frame	$sp,128,$31		# vars= 72, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-128
	sw	$31,124($sp)
	sw	$fp,120($sp)
	sw	$23,116($sp)
	sw	$22,112($sp)
	sw	$21,108($sp)
	sw	$20,104($sp)
	sw	$19,100($sp)
	sw	$18,96($sp)
	sw	$17,92($sp)
	sw	$16,88($sp)
	move	$17,$4
	lw	$3,40($17)
	lui	$18,%hi(v6_path_table_list_max)
	lw	$4,0($4)
	lui	$19,%hi(v6_table_path)
	lw	$9,4($17)
	lui	$23,%hi(jiffies)
	lw	$8,8($17)
	xor	$22,$4,$3
	lw	$7,12($17)
	xor	$22,$22,$9
	lw	$6,16($17)
	xor	$22,$22,$8
	lw	$12,20($17)
	xor	$22,$22,$7
	lw	$11,24($17)
	xor	$22,$22,$6
	lw	$10,28($17)
	xor	$22,$22,$12
	xor	$22,$22,$11
	lhu	$13,34($17)
	srl	$fp,$4,8
	lw	$2,36($17)
	xor	$22,$22,$10
	sw	$13,80($sp)
	xor	$22,$22,$fp
	lw	$14,%lo(v6_path_table_list_max)($18)
	srl	$21,$9,8
	lw	$13,%lo(v6_table_path)($19)
	srl	$20,$8,8
	addiu	$14,$14,-1
	xor	$22,$22,$21
	lw	$13,0($13)
	xor	$21,$22,$20
	lw	$23,%lo(jiffies)($23)
	srl	$25,$7,8
	sw	$4,64($sp)
	xor	$25,$21,$25
	lw	$21,80($sp)
	srl	$fp,$6,16
	sw	$9,68($sp)
	xor	$25,$25,$fp
	sw	$8,72($sp)
	srl	$24,$12,16
	sw	$7,76($sp)
	xor	$24,$25,$24
	sw	$6,48($sp)
	srl	$16,$11,16
	sw	$12,52($sp)
	xor	$16,$24,$16
	sw	$11,56($sp)
	srl	$15,$10,16
	sw	$10,60($sp)
	xor	$15,$16,$15
	srl	$20,$2,4
	xor	$15,$15,$20
	xor	$15,$15,$21
	srl	$16,$15,12
	xor	$15,$16,$15
	and	$15,$15,$14
	sll	$16,$15,2
	sll	$15,$15,4
	subu	$15,$15,$16
	addu	$15,$13,$15
	lw	$16,0($15)
	beq	$16,$0,$L76
	move	$20,$5

	j	$L90
	addiu	$21,$17,16

$L77:
	lw	$16,52($16)
	beq	$16,$0,$L95
	nop

$L90:
	lw	$4,36($16)
	bne	$4,$2,$L77
	nop

	lw	$4,40($16)
	bne	$4,$3,$L77
	move	$4,$16

	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$17

	beq	$2,$0,$L78
	addiu	$4,$16,16

$L93:
	lw	$16,52($16)
	lw	$3,40($17)
	lw	$2,36($17)
	bne	$16,$0,$L90
	nop

$L95:
	lw	$4,%lo(v6_table_path)($19)
	lw	$14,%lo(v6_path_table_list_max)($18)
	lw	$13,0($4)
	lhu	$21,34($17)
	addiu	$14,$14,-1
$L94:
	lw	$4,0($17)
	lw	$9,4($17)
	lw	$8,8($17)
	lw	$7,12($17)
	lw	$6,16($17)
	lw	$12,20($17)
	lw	$11,24($17)
	lw	$10,28($17)
$L76:
	move	$16,$12
	sw	$12,36($sp)
	xor	$12,$12,$6
	sw	$10,44($sp)
	move	$5,$10
	sw	$6,32($sp)
	xor	$10,$12,$11
	sw	$4,16($sp)
	xor	$10,$10,$5
	sw	$9,20($sp)
	xor	$10,$10,$4
	sw	$8,24($sp)
	xor	$10,$10,$9
	sw	$7,28($sp)
	xor	$10,$10,$8
	sw	$11,40($sp)
	xor	$10,$10,$7
	srl	$6,$6,8
	xor	$6,$10,$6
	srl	$16,$16,8
	xor	$16,$6,$16
	srl	$15,$11,8
	xor	$15,$16,$15
	srl	$5,$5,8
	xor	$5,$15,$5
	srl	$4,$4,16
	xor	$4,$5,$4
	srl	$9,$9,16
	xor	$9,$4,$9
	srl	$8,$8,16
	xor	$8,$9,$8
	srl	$7,$7,16
	xor	$7,$8,$7
	xor	$7,$7,$2
	xor	$21,$7,$21
	srl	$3,$3,4
	xor	$3,$21,$3
	srl	$4,$3,12
	xor	$3,$4,$3
	and	$14,$3,$14
	sll	$3,$14,2
	sll	$14,$14,4
	subu	$14,$14,$3
	addu	$13,$13,$14
	lw	$16,0($13)
	beq	$16,$0,$L89
	addiu	$18,$17,16

	j	$L92
	nop

$L83:
	lw	$16,52($16)
	beq	$16,$0,$L81
	move	$2,$0

	lw	$2,36($17)
$L92:
	lw	$3,36($16)
	bne	$3,$2,$L83
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	bne	$3,$2,$L83
	move	$4,$16

	li	$6,16			# 0x10
	jal	memcmp
	move	$5,$17

	bne	$2,$0,$L83
	addiu	$4,$16,16

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L83
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	bne	$3,$2,$L83
	subu	$3,$23,$20

	lw	$2,44($16)
	subu	$3,$3,$2
	li	$2,-1			# 0xffffffffffffffff
	slt	$3,$3,0
	j	$L81
	movz	$2,$0,$3

$L89:
	move	$2,$0
$L81:
	lw	$31,124($sp)
	lw	$fp,120($sp)
	lw	$23,116($sp)
	lw	$22,112($sp)
	lw	$21,108($sp)
	lw	$20,104($sp)
	lw	$19,100($sp)
	lw	$18,96($sp)
	lw	$17,92($sp)
	lw	$16,88($sp)
	j	$31
	addiu	$sp,$sp,128

$L78:
	move	$5,$21
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L93
	nop

	lw	$3,32($17)
	lw	$2,32($16)
	bne	$2,$3,$L93
	subu	$4,$23,$20

	lw	$3,44($16)
	subu	$3,$4,$3
	bltz	$3,$L86
	andi	$21,$2,0xffff

	lw	$4,%lo(v6_table_path)($19)
	lw	$14,%lo(v6_path_table_list_max)($18)
	lw	$3,40($17)
	lw	$2,36($17)
	lw	$13,0($4)
	j	$L94
	addiu	$14,$14,-1

$L86:
	j	$L81
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	rtk_idleV6Connection
	.size	rtk_idleV6Connection, .-rtk_idleV6Connection
	.section	.text.rtl_V6_Cache_Timer_update,"ax",@progbits
	.align	2
	.globl	rtl_V6_Cache_Timer_update
	.set	nomips16
	.ent	rtl_V6_Cache_Timer_update
	.type	rtl_V6_Cache_Timer_update, @function
rtl_V6_Cache_Timer_update:
	.frame	$sp,80,$31		# vars= 48, regs= 4/0, args= 16, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lui	$2,%hi(jiffies)
	move	$5,$0
	move	$16,$4
	lw	$18,%lo(jiffies)($2)
	jal	rtl_get_ct_protonum
	nop

	li	$3,17			# 0x11
	beq	$2,$3,$L104
	move	$4,$16

	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	bne	$2,$3,$L103
	nop

	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L103
	nop

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	move	$17,$2
$L99:
	li	$6,44			# 0x2c
	move	$5,$0
	jal	memset
	addiu	$4,$sp,16

	move	$4,$16
	jal	rtl_get_ct_protonum_ipv6
	move	$5,$0

	move	$5,$0
	move	$6,$0
	addiu	$7,$sp,16
	move	$4,$16
	jal	rtl_get_ct_v6_ip_by_dir
	sw	$2,48($sp)

	addiu	$7,$sp,32
	move	$4,$16
	move	$5,$0
	jal	rtl_get_ct_v6_ip_by_dir
	li	$6,1			# 0x1

	move	$4,$16
	move	$5,$0
	jal	rtl_get_ct_port_by_dir
	move	$6,$0

	move	$4,$16
	move	$5,$0
	li	$6,1			# 0x1
	jal	rtl_get_ct_port_by_dir
	sw	$2,52($sp)

	addiu	$4,$sp,16
	move	$5,$17
	jal	rtk_idleV6Connection
	sw	$2,56($sp)

	bne	$2,$0,$L101
	addu	$5,$17,$18

$L103:
	lw	$31,76($sp)
	li	$2,-1			# 0xffffffffffffffff
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	j	$31
	addiu	$sp,$sp,80

$L101:
	jal	rtl_check_for_acc
	move	$4,$16

	move	$2,$0
	lw	$31,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	j	$31
	addiu	$sp,$sp,80

$L104:
	jal	rtl_get_ct_udp_status
	nop

	andi	$2,$2,0x2
	beq	$2,$0,$L98
	nop

	li	$4,1			# 0x1
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	j	$L99
	move	$17,$2

$L98:
	move	$4,$0
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	j	$L99
	move	$17,$2

	.set	macro
	.set	reorder
	.end	rtl_V6_Cache_Timer_update
	.size	rtl_V6_Cache_Timer_update, .-rtl_V6_Cache_Timer_update
	.section	.text.V6_udp_fragCache_init,"ax",@progbits
	.align	2
	.globl	V6_udp_fragCache_init
	.set	nomips16
	.ent	V6_udp_fragCache_init
	.type	V6_udp_fragCache_init, @function
V6_udp_fragCache_init:
	.frame	$sp,56,$31		# vars= 8, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($19)
	li	$5,32800			# 0x8020
	jal	kmem_cache_alloc
	lui	$20,%hi(V6_udp_cache_table)

	move	$21,$2
	beq	$2,$0,$L113
	sw	$2,%lo(V6_udp_cache_table)($20)

	lui	$2,%hi(V6_udp_cache_list_free)
	sll	$3,$17,2
	sll	$4,$17,4
	addiu	$16,$2,%lo(V6_udp_cache_list_free)
	subu	$4,$4,$3
	li	$5,32			# 0x20
	lui	$18,%hi(V6_max_udp_frag_entry)
	sw	$0,8($16)
	sw	$0,%lo(V6_udp_cache_list_free)($2)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$17,%lo(V6_max_udp_frag_entry)($18)

	sw	$2,0($21)
	lw	$3,%lo(V6_udp_cache_table)($20)
	lw	$5,0($3)
	beq	$5,$0,$L106
	li	$2,-1			# 0xffffffffffffffff

	blez	$17,$L107
	move	$2,$0

	j	$L108
	move	$4,$0

$L115:
	lw	$5,0($3)
$L108:
	addu	$5,$5,$2
	addiu	$4,$4,1
	sw	$0,8($5)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(V6_udp_cache_table)($20)
	lw	$5,0($3)
	addu	$5,$5,$2
	addiu	$2,$2,12
	bne	$4,$17,$L115
	sw	$5,4($5)

$L107:
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	blez	$2,$L112
	lui	$20,%hi(V6_cache_timeout)

	move	$17,$0
	j	$L109
	addiu	$20,$20,%lo(V6_cache_timeout)

$L116:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	lw	$3,4($16)
	sw	$2,72($2)
	addiu	$4,$2,88
	sw	$3,92($2)
	sw	$20,68($2)
	sw	$0,88($2)
	sw	$2,0($3)
	lw	$3,8($16)
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	addiu	$3,$3,1
	slt	$2,$17,$2
	sw	$4,4($16)
	beq	$2,$0,$L112
	sw	$3,8($16)

$L109:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	move	$5,$0
	addiu	$4,$2,52
	move	$6,$0
	bne	$2,$0,$L116
	move	$7,$0

$L113:
	move	$2,$0
$L106:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L112:
	lw	$31,52($sp)
	li	$2,1			# 0x1
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	V6_udp_fragCache_init
	.size	V6_udp_fragCache_init, .-V6_udp_fragCache_init
	.section	.text.ipv6_fast_enter,"ax",@progbits
	.align	2
	.globl	ipv6_fast_enter
	.set	nomips16
	.ent	ipv6_fast_enter
	.type	ipv6_fast_enter, @function
ipv6_fast_enter:
	.frame	$sp,192,$31		# vars= 136, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-192
	sw	$31,188($sp)
	sw	$fp,184($sp)
	sw	$23,180($sp)
	sw	$22,176($sp)
	sw	$21,172($sp)
	sw	$20,168($sp)
	sw	$19,164($sp)
	sw	$18,160($sp)
	sw	$17,156($sp)
	sw	$16,152($sp)
	jal	rtl_skb_mac_header
	move	$17,$4

	move	$4,$17
	jal	rtl_ipv6_hdr
	move	$16,$2

	move	$4,$17
	jal	rtl_get_skb_pppoe_flag
	move	$18,$2

	bne	$2,$0,$L118
	nop

	lbu	$3,12($16)
	lbu	$2,13($16)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,34525			# 0x86dd
	beq	$3,$2,$L118
	nop

$L153:
	move	$2,$0
$L119:
	lw	$31,188($sp)
	lw	$fp,184($sp)
	lw	$23,180($sp)
	lw	$22,176($sp)
	lw	$21,172($sp)
	lw	$20,168($sp)
	lw	$19,164($sp)
	lw	$18,160($sp)
	lw	$17,156($sp)
	lw	$16,152($sp)
	j	$31
	addiu	$sp,$sp,192

$L118:
	jal	__ipv6_addr_type
	addiu	$4,$18,8

	andi	$2,$2,0x20
	bne	$2,$0,$L119
	move	$2,$0

	jal	__ipv6_addr_type
	addiu	$4,$18,24

	andi	$2,$2,0x22
	bne	$2,$0,$L153
	li	$3,6			# 0x6

	lbu	$2,6($18)
	beq	$2,$3,$L120
	li	$3,17			# 0x11

	beq	$2,$3,$L121
	li	$3,44			# 0x2c

	bne	$2,$3,$L119
	move	$2,$0

$L121:
	lw	$2,20($18)
	lw	$8,8($18)
	lw	$7,12($18)
	lw	$6,16($18)
	sw	$2,140($sp)
	lw	$5,24($18)
	lw	$2,36($18)
	lw	$4,28($18)
	lw	$3,32($18)
	sw	$2,124($sp)
	li	$2,44			# 0x2c
	sw	$8,128($sp)
	sw	$7,132($sp)
	sw	$6,136($sp)
	sw	$5,112($sp)
	sw	$4,116($sp)
	sw	$3,120($sp)
	lbu	$22,6($18)
	lhu	$19,40($18)
	lhu	$20,42($18)
	beq	$22,$2,$L122
	andi	$23,$22,0xffff

	move	$21,$0
	sw	$0,144($sp)
	move	$fp,$0
	sw	$0,148($sp)
$L123:
	lw	$4,128($sp)
	lui	$11,%hi(v6_path_table_list_max)
	lw	$10,132($sp)
	srl	$13,$19,4
	lw	$8,140($sp)
	srl	$2,$4,8
	xor	$3,$10,$4
	lw	$7,112($sp)
	lw	$6,116($sp)
	xor	$3,$3,$8
	lw	$5,120($sp)
	xor	$3,$3,$7
	lw	$4,124($sp)
	xor	$3,$3,$6
	lw	$9,136($sp)
	xor	$3,$3,$5
	xor	$3,$3,$4
	lw	$12,%lo(v6_path_table_list_max)($11)
	xor	$3,$3,$9
	xor	$3,$3,$2
	lui	$2,%hi(v6_table_path)
	srl	$10,$10,8
	lw	$11,%lo(v6_table_path)($2)
	xor	$3,$3,$10
	addiu	$12,$12,-1
	srl	$9,$9,8
	lw	$11,0($11)
	xor	$3,$3,$9
	srl	$8,$8,8
	xor	$3,$3,$8
	srl	$7,$7,16
	xor	$3,$3,$7
	srl	$6,$6,16
	xor	$3,$3,$6
	srl	$5,$5,16
	xor	$3,$3,$5
	srl	$4,$4,16
	xor	$3,$3,$4
	xor	$3,$3,$20
	xor	$3,$3,$23
	xor	$3,$3,$13
	srl	$2,$3,12
	xor	$2,$2,$3
	and	$2,$2,$12
	sll	$3,$2,2
	sll	$2,$2,4
	subu	$2,$2,$3
	addu	$2,$11,$2
	lw	$16,0($2)
	bne	$16,$0,$L154
	move	$2,$0

	j	$L119
	nop

$L130:
	lw	$16,52($16)
	beq	$16,$0,$L119
	move	$2,$0

$L154:
	lw	$2,36($16)
	bne	$2,$19,$L130
	nop

	lw	$2,40($16)
	bne	$2,$20,$L130
	move	$4,$16

	li	$6,16			# 0x10
	jal	memcmp
	addiu	$5,$sp,128

	bne	$2,$0,$L130
	addiu	$4,$16,16

	addiu	$5,$sp,112
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L130
	nop

	lw	$2,32($16)
	bne	$2,$22,$L130
	nop

	jal	rtl_ip6_route_input
	move	$4,$17

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L119
	move	$2,$0

	jal	rtl_get_skb_ipv6_dst
	move	$4,$17

	beq	$2,$0,$L119
	move	$2,$0

	jal	rtl_skb_ipv6_dst_check
	move	$4,$17

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L158
	move	$4,$17

	jal	rtl_check_dst_input
	nop

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L158
	move	$4,$17

	lw	$3,148($sp)
	beq	$3,$0,$L133
	lui	$4,%hi(V6_udp_cache_list_free)

	lw	$3,144($sp)
	lw	$31,132($sp)
	lw	$fp,136($sp)
	lw	$25,140($sp)
	move	$24,$31
	lw	$15,116($sp)
	move	$21,$fp
	lw	$7,120($sp)
	move	$14,$25
	lw	$5,124($sp)
	move	$13,$15
	lw	$6,128($sp)
	move	$12,$7
	lw	$9,112($sp)
	lw	$2,%lo(V6_udp_cache_list_free)($4)
	lw	$10,4($3)
	lui	$3,%hi(V6_max_udp_frag_entry)
	sw	$6,64($sp)
	sw	$31,68($sp)
	sw	$fp,72($sp)
	sw	$25,76($sp)
	sw	$9,48($sp)
	sw	$15,52($sp)
	sw	$7,56($sp)
	sw	$5,60($sp)
	lw	$8,%lo(V6_max_udp_frag_entry)($3)
	beq	$2,$0,$L134
	move	$11,$5

	lw	$3,88($2)
	andi	$19,$19,0xffff
	andi	$20,$20,0xffff
	sw	$5,32($2)
	li	$5,1			# 0x1
	sb	$5,48($2)
	sw	$10,0($2)
	sw	$6,4($2)
	sw	$31,8($2)
	sw	$fp,12($2)
	sw	$25,16($2)
	sw	$9,20($2)
	sw	$15,24($2)
	sw	$7,28($2)
	sw	$22,36($2)
	sw	$19,40($2)
	sw	$20,44($2)
	lw	$5,92($2)
	beq	$3,$0,$L135
	addiu	$4,$4,%lo(V6_udp_cache_list_free)

	sw	$5,92($3)
	lw	$3,88($2)
	lw	$5,92($2)
$L136:
	xor	$15,$24,$6
	sw	$3,0($5)
	xor	$15,$15,$21
	addiu	$8,$8,-1
	xor	$15,$15,$14
	lw	$7,8($4)
	xor	$15,$15,$9
	lui	$3,%hi(V6_udp_cache_table)
	xor	$15,$15,$13
	lw	$5,%lo(V6_udp_cache_table)($3)
	xor	$15,$15,$12
	addiu	$7,$7,-1
	xor	$15,$15,$11
	sw	$7,8($4)
	srl	$6,$6,8
	sw	$0,80($2)
	xor	$15,$15,$6
	lw	$7,0($5)
	srl	$24,$24,8
	addiu	$6,$2,80
	xor	$15,$15,$24
	addiu	$4,$2,52
	srl	$21,$21,8
	xor	$15,$15,$21
	srl	$14,$14,8
	xor	$14,$15,$14
	srl	$9,$9,16
	xor	$9,$14,$9
	srl	$13,$13,16
	xor	$9,$9,$13
	srl	$12,$12,16
	xor	$9,$9,$12
	srl	$11,$11,16
	xor	$9,$9,$11
	xor	$10,$9,$10
	srl	$9,$9,16
	xor	$9,$10,$9
	xor	$23,$9,$23
	and	$23,$23,$8
	li	$8,12			# 0xc
	mult	$23,$8
	mflo	$8
	addu	$7,$7,$8
	lw	$7,4($7)
	sw	$7,84($2)
	lw	$5,0($5)
	addu	$5,$5,$8
	lw	$5,4($5)
	sw	$2,0($5)
	lui	$5,%hi(jiffies)
	lw	$3,%lo(V6_udp_cache_table)($3)
	lw	$5,%lo(jiffies)($5)
	lw	$7,0($3)
	addiu	$5,$5,1000
	addu	$7,$7,$8
	sw	$6,4($7)
	lw	$3,0($3)
	addu	$8,$3,$8
	lw	$3,8($8)
	addiu	$3,$3,1
	sw	$3,8($8)
	jal	add_timer
	sw	$5,60($2)

$L137:
	move	$4,$17
$L160:
	jal	rtl_set_skb_dev
	move	$5,$0

	lbu	$2,7($18)
	sltu	$3,$2,2
	bne	$3,$0,$L140
	nop

	addiu	$2,$2,-1
	sb	$2,7($18)
	lw	$5,48($16)
	jal	rtl_set_skb_mark
	move	$4,$17

	jal	rtl_get_skb_len
	move	$4,$17

	move	$4,$17
	jal	rtl_ip6_skb_dst_mtu
	move	$18,$2

	sltu	$2,$2,$18
	beq	$2,$0,$L141
	nop

	jal	rtl_skb_is_gso
	move	$4,$17

	beq	$2,$0,$L140
	nop

$L141:
	jal	rtl_dst_allfrag
	move	$4,$17

	beq	$2,$0,$L159
	move	$4,$17

$L140:
	jal	kfree_skb
	move	$4,$17

	j	$L119
	li	$2,1			# 0x1

$L120:
	lw	$3,52($18)
	li	$2,458752			# 0x70000
	and	$2,$3,$2
	beq	$2,$0,$L121
	move	$2,$0

	j	$L119
	nop

$L122:
	addiu	$2,$18,40
	lbu	$3,40($18)
	sw	$2,144($sp)
	li	$2,17			# 0x11
	beq	$3,$2,$L124
	li	$23,44			# 0x2c

	move	$21,$0
	move	$fp,$0
	j	$L123
	sw	$0,148($sp)

$L124:
	andi	$3,$20,0xfff9
	li	$2,1			# 0x1
	beq	$3,$2,$L125
	andi	$2,$20,0xfff8

	beq	$2,$0,$L126
	move	$fp,$0

	andi	$20,$20,0x1
	xori	$fp,$20,0x1
$L126:
	lw	$19,132($sp)
	lui	$2,%hi(V6_udp_cache_table)
	lw	$4,128($sp)
	lui	$6,%hi(V6_max_udp_frag_entry)
	lw	$15,136($sp)
	srl	$13,$19,8
	lw	$14,140($sp)
	xor	$5,$19,$4
	lw	$3,112($sp)
	xor	$5,$5,$15
	lw	$12,116($sp)
	xor	$5,$5,$14
	lw	$11,120($sp)
	xor	$5,$5,$3
	lw	$9,124($sp)
	srl	$10,$15,8
	lw	$21,%lo(V6_udp_cache_table)($2)
	xor	$2,$5,$12
	xor	$2,$2,$11
	sw	$4,96($sp)
	xor	$2,$2,$9
	sw	$3,80($sp)
	srl	$4,$4,8
	lw	$20,%lo(V6_max_udp_frag_entry)($6)
	xor	$4,$2,$4
	lw	$16,44($18)
	xor	$13,$4,$13
	addiu	$20,$20,-1
	xor	$10,$13,$10
	sw	$19,100($sp)
	srl	$8,$14,8
	sw	$15,104($sp)
	xor	$8,$10,$8
	sw	$14,108($sp)
	srl	$3,$3,16
	sw	$12,84($sp)
	xor	$3,$8,$3
	sw	$11,88($sp)
	srl	$7,$12,16
	sw	$9,92($sp)
	xor	$3,$3,$7
	lw	$4,0($21)
	srl	$6,$11,16
	srl	$5,$9,16
	xor	$3,$3,$6
	xor	$3,$3,$5
	xori	$2,$16,0x11
	srl	$5,$3,16
	xor	$2,$2,$3
	xor	$2,$2,$5
	and	$2,$2,$20
	sll	$3,$2,2
	sll	$2,$2,4
	subu	$2,$2,$3
	addu	$2,$4,$2
	lw	$21,0($2)
	beq	$21,$0,$L153
	li	$19,17			# 0x11

	j	$L155
	nop

$L127:
	lw	$21,80($21)
	beq	$21,$0,$L119
	move	$2,$0

$L155:
	lw	$2,0($21)
	bne	$16,$2,$L127
	addiu	$4,$21,4

	addiu	$5,$sp,96
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L127
	addiu	$4,$21,20

	addiu	$5,$sp,80
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L127
	nop

	lw	$2,36($21)
	bne	$2,$19,$L127
	nop

	beq	$21,$0,$L153
	li	$23,17			# 0x11

	lw	$19,40($21)
	lw	$20,44($21)
	li	$22,17			# 0x11
	j	$L123
	sw	$0,148($sp)

$L125:
	li	$3,1			# 0x1
	li	$23,17			# 0x11
	lhu	$19,48($18)
	move	$21,$0
	lhu	$20,50($18)
	move	$fp,$0
	sw	$3,148($sp)
	j	$L123
	li	$22,17			# 0x11

$L159:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	jal	ip_finish_output6
	sw	$2,44($16)

	lui	$3,%hi(statistic_ipv6_fp)
	lw	$4,%lo(statistic_ipv6_fp)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	j	$L119
	sw	$4,%lo(statistic_ipv6_fp)($3)

$L135:
	j	$L136
	sw	$5,4($4)

$L133:
	beq	$fp,$0,$L160
	move	$4,$17

	beq	$21,$0,$L160
	nop

	jal	del_timer
	addiu	$4,$21,52

	lui	$8,%hi(V6_max_udp_frag_entry)
	lw	$7,4($21)
	lw	$6,8($21)
	lw	$5,12($21)
	srl	$22,$7,8
	lw	$4,16($21)
	xor	$20,$6,$7
	lw	$2,20($21)
	xor	$20,$20,$5
	lw	$14,24($21)
	xor	$20,$20,$4
	lw	$13,28($21)
	xor	$20,$20,$2
	lw	$12,32($21)
	xor	$20,$20,$14
	xor	$20,$20,$13
	sw	$7,16($sp)
	xor	$20,$20,$12
	sw	$6,20($sp)
	xor	$20,$20,$22
	sw	$5,24($sp)
	srl	$7,$6,8
	sw	$4,28($sp)
	xor	$6,$20,$7
	sw	$2,32($sp)
	srl	$5,$5,8
	lw	$19,0($21)
	xor	$5,$6,$5
	lhu	$3,38($21)
	srl	$4,$4,8
	lw	$15,%lo(V6_max_udp_frag_entry)($8)
	xor	$4,$5,$4
	lw	$8,80($21)
	srl	$2,$2,16
	addiu	$15,$15,-1
	xor	$2,$4,$2
	sw	$14,36($sp)
	srl	$11,$14,16
	sw	$13,40($sp)
	xor	$2,$2,$11
	sw	$12,44($sp)
	srl	$10,$13,16
	sb	$0,48($21)
	xor	$2,$2,$10
	srl	$9,$12,16
	xor	$2,$2,$9
	xor	$3,$3,$19
	srl	$4,$2,16
	xor	$3,$3,$2
	xor	$3,$3,$4
	beq	$8,$0,$L138
	and	$3,$3,$15

	lw	$4,84($21)
	lui	$2,%hi(V6_udp_cache_table)
	sw	$4,84($8)
$L139:
	lw	$4,84($21)
	li	$6,12			# 0xc
	lw	$5,80($21)
	mult	$3,$6
	sw	$5,0($4)
	lui	$4,%hi(V6_udp_cache_list_free)
	lw	$6,%lo(V6_udp_cache_table)($2)
	addiu	$4,$4,%lo(V6_udp_cache_list_free)
	mflo	$3
	lw	$2,4($4)
	lw	$6,0($6)
	addiu	$5,$21,88
	addu	$3,$6,$3
	lw	$6,8($3)
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$2,92($21)
	sw	$0,88($21)
	sw	$21,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	j	$L137
	sw	$2,8($4)

$L134:
	move	$4,$17
$L158:
	jal	rtl_dst_release
	nop

	j	$L119
	move	$2,$0

$L138:
	li	$4,12			# 0xc
	lui	$2,%hi(V6_udp_cache_table)
	mult	$3,$4
	lw	$6,%lo(V6_udp_cache_table)($2)
	lw	$5,84($21)
	lw	$6,0($6)
	mflo	$4
	addu	$4,$6,$4
	j	$L139
	sw	$5,4($4)

	.set	macro
	.set	reorder
	.end	ipv6_fast_enter
	.size	ipv6_fast_enter, .-ipv6_fast_enter
	.section	.text.init_V6_table_path,"ax",@progbits
	.align	2
	.globl	init_V6_table_path
	.set	nomips16
	.ent	init_V6_table_path
	.type	init_V6_table_path, @function
init_V6_table_path:
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
	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	move	$17,$4
	lw	$4,20($19)
	move	$20,$5
	li	$5,32800			# 0x8020
	jal	kmem_cache_alloc
	lui	$18,%hi(v6_table_path)

	move	$21,$2
	beq	$2,$0,$L167
	sw	$2,%lo(v6_table_path)($18)

	lui	$6,%hi(v6_path_list_inuse)
	lui	$3,%hi(v6_path_list_free)
	addiu	$2,$6,%lo(v6_path_list_inuse)
	sll	$5,$17,4
	sll	$4,$17,2
	addiu	$16,$3,%lo(v6_path_list_free)
	subu	$4,$5,$4
	li	$5,32			# 0x20
	sw	$0,8($2)
	sw	$2,4($2)
	lui	$2,%hi(v6_path_table_list_max)
	sw	$0,%lo(v6_path_list_free)($3)
	sw	$0,%lo(v6_path_list_inuse)($6)
	sw	$0,8($16)
	sw	$16,4($16)
	jal	__kmalloc
	sw	$17,%lo(v6_path_table_list_max)($2)

	sw	$2,0($21)
	lw	$3,%lo(v6_table_path)($18)
	lw	$5,0($3)
	beq	$5,$0,$L162
	li	$2,-1			# 0xffffffffffffffff

	blez	$17,$L163
	move	$2,$0

	j	$L164
	move	$4,$0

$L171:
	lw	$5,0($3)
$L164:
	addu	$5,$5,$2
	addiu	$4,$4,1
	sw	$0,8($5)
	lw	$3,0($3)
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(v6_table_path)($18)
	lw	$5,0($3)
	addu	$5,$5,$2
	addiu	$2,$2,12
	bne	$4,$17,$L171
	sw	$5,4($5)

$L163:
	blez	$20,$L168
	move	$17,$0

	j	$L165
	nop

$L172:
	lw	$3,4($16)
	sw	$0,60($2)
	sw	$3,64($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$20,$L168
	sw	$2,8($16)

$L165:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	bne	$2,$0,$L172
	addiu	$4,$2,60

	lw	$31,44($sp)
	li	$2,-2			# 0xfffffffffffffffe
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L168:
	move	$2,$0
$L162:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L167:
	j	$L162
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	init_V6_table_path
	.size	init_V6_table_path, .-init_V6_table_path
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"ipv6_fast_forward\000"
	.section	.init.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_init
	.set	nomips16
	.ent	ipv6_fast_forward_init
	.type	ipv6_fast_forward_init, @function
ipv6_fast_forward_init:
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 24, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	lui	$4,%hi($LC1)
	lui	$6,%hi(proc_root)
	lui	$7,%hi(ipv6_fast_forward_proc_fops)
	addiu	$4,$4,%lo($LC1)
	move	$5,$0
	addiu	$6,$6,%lo(proc_root)
	addiu	$7,$7,%lo(ipv6_fast_forward_proc_fops)
	jal	proc_create_data
	sw	$0,16($sp)

	lui	$3,%hi(ipv6_res)
	lw	$31,28($sp)
	sw	$2,%lo(ipv6_res)($3)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_init
	.size	ipv6_fast_forward_init, .-ipv6_fast_forward_init
	.section	.exit.text,"ax",@progbits
	.align	2
	.globl	ipv6_fast_forward_exit
	.set	nomips16
	.ent	ipv6_fast_forward_exit
	.type	ipv6_fast_forward_exit, @function
ipv6_fast_forward_exit:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	lui	$2,%hi(ipv6_res)
	lw	$2,%lo(ipv6_res)($2)
	beq	$2,$0,$L175
	lui	$4,%hi($LC1)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC1)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

$L175:
	lw	$31,20($sp)
	lui	$2,%hi(fast_ipv6_fw)
	sw	$0,%lo(fast_ipv6_fw)($2)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	ipv6_fast_forward_exit
	.size	ipv6_fast_forward_exit, .-ipv6_fast_forward_exit
	.globl	ipv6_fast_forward_proc_fops
	.section	.data.ipv6_fast_forward_proc_fops,"aw",@progbits
	.align	2
	.type	ipv6_fast_forward_proc_fops, @object
	.size	ipv6_fast_forward_proc_fops, 108
ipv6_fast_forward_proc_fops:
	.space	4
	.word	seq_lseek
	.word	seq_read
	.word	ipv6_fast_forward_single_write
	.space	28
	.word	ipv6_fast_forward_single_open
	.space	4
	.word	single_release
	.space	52
	.globl	V6_udp_cache_table
	.section	.bss.V6_udp_cache_table,"aw",@nobits
	.align	2
	.type	V6_udp_cache_table, @object
	.size	V6_udp_cache_table, 4
V6_udp_cache_table:
	.space	4
	.globl	V6_udp_cache_list_free
	.section	.bss.V6_udp_cache_list_free,"aw",@nobits
	.align	2
	.type	V6_udp_cache_list_free, @object
	.size	V6_udp_cache_list_free, 12
V6_udp_cache_list_free:
	.space	12
	.globl	v6_table_path
	.section	.bss.v6_table_path,"aw",@nobits
	.align	2
	.type	v6_table_path, @object
	.size	v6_table_path, 4
v6_table_path:
	.space	4
	.globl	v6_path_list_free
	.section	.bss.v6_path_list_free,"aw",@nobits
	.align	2
	.type	v6_path_list_free, @object
	.size	v6_path_list_free, 12
v6_path_list_free:
	.space	12
	.globl	v6_path_list_inuse
	.section	.bss.v6_path_list_inuse,"aw",@nobits
	.align	2
	.type	v6_path_list_inuse, @object
	.size	v6_path_list_inuse, 12
v6_path_list_inuse:
	.space	12
	.globl	fast_ipv6_fw
	.section	.bss.fast_ipv6_fw,"aw",@nobits
	.align	2
	.type	fast_ipv6_fw, @object
	.size	fast_ipv6_fw, 4
fast_ipv6_fw:
	.space	4
	.section	.bss.v6_path_table_list_max,"aw",@nobits
	.align	2
	.type	v6_path_table_list_max, @object
	.size	v6_path_table_list_max, 4
v6_path_table_list_max:
	.space	4
	.section	.bss.V6_max_udp_frag_entry,"aw",@nobits
	.align	2
	.type	V6_max_udp_frag_entry, @object
	.size	V6_max_udp_frag_entry, 4
V6_max_udp_frag_entry:
	.space	4
	.section	.bss.ipv6_res,"aw",@nobits
	.align	2
	.type	ipv6_res, @object
	.size	ipv6_res, 4
ipv6_res:
	.space	4
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2002) 4.6.4"
