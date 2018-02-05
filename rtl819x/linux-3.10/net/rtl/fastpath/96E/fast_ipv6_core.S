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
	.frame	$sp,32,$31		# vars= 32, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lbu	$2,48($4)
	nop
	beq	$2,$0,$L1
	addiu	$sp,$sp,-32

	lw	$7,4($4)
	lw	$6,8($4)
	lw	$5,12($4)
	lw	$3,16($4)
	lw	$2,20($4)
	xor	$11,$6,$7
	lw	$10,24($4)
	xor	$11,$11,$5
	lw	$9,28($4)
	xor	$11,$11,$3
	lw	$8,32($4)
	xor	$11,$11,$2
	xor	$11,$11,$10
	xor	$11,$11,$9
	srl	$15,$7,8
	xor	$11,$11,$8
	xor	$11,$11,$15
	srl	$15,$6,8
	xor	$11,$11,$15
	srl	$15,$5,8
	xor	$11,$11,$15
	srl	$15,$3,8
	xor	$11,$11,$15
	srl	$25,$2,16
	srl	$14,$10,16
	xor	$11,$11,$25
	xor	$11,$11,$14
	srl	$13,$9,16
	lw	$15,0($4)
	lhu	$24,38($4)
	xor	$11,$11,$13
	lui	$13,%hi(V6_max_udp_frag_entry)
	srl	$12,$8,16
	lw	$13,%lo(V6_max_udp_frag_entry)($13)
	xor	$11,$11,$12
	xor	$15,$24,$15
	lw	$12,80($4)
	srl	$14,$11,16
	xor	$11,$15,$11
	xor	$11,$11,$14
	addiu	$13,$13,-1
	sw	$7,0($sp)
	sw	$6,4($sp)
	sw	$5,8($sp)
	sw	$3,12($sp)
	sw	$2,16($sp)
	sw	$10,20($sp)
	sw	$9,24($sp)
	sw	$8,28($sp)
	and	$11,$11,$13
	beq	$12,$0,$L3
	sb	$0,48($4)

	lw	$3,84($4)
	sll	$2,$11,2
	sll	$11,$11,4
	sw	$3,84($12)
	subu	$11,$11,$2
	lui	$3,%hi(V6_udp_cache_table)
$L4:
	lw	$5,84($4)
	lw	$6,80($4)
	lui	$2,%hi(V6_udp_cache_list_free)
	addiu	$2,$2,%lo(V6_udp_cache_list_free)
	sw	$6,0($5)
	lw	$5,%lo(V6_udp_cache_table)($3)
	lw	$3,4($2)
	addiu	$6,$4,88
	lw	$5,0($5)
	nop
	addu	$5,$5,$11
	lw	$7,8($5)
	nop
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
	addiu	$sp,$sp,32
	j	$31
	nop

$L3:
	lui	$3,%hi(V6_udp_cache_table)
	lw	$2,%lo(V6_udp_cache_table)($3)
	sll	$7,$11,2
	lw	$6,84($4)
	lw	$2,0($2)
	sll	$11,$11,4
	subu	$11,$11,$7
	addu	$2,$2,$11
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

	lw	$31,20($sp)
	move	$2,$0
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
	beq	$2,$3,$L9
	move	$2,$0

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
	lw	$17,40($16)
	lw	$10,4($16)
	lw	$9,8($16)
	lw	$8,12($16)
	xor	$17,$4,$17
	lw	$3,16($16)
	xor	$17,$17,$10
	lw	$7,20($16)
	xor	$17,$17,$9
	lw	$6,24($16)
	xor	$17,$17,$8
	lw	$5,28($16)
	xor	$17,$17,$3
	xor	$17,$17,$7
	xor	$17,$17,$6
	xor	$17,$17,$5
	srl	$18,$4,8
	xor	$17,$17,$18
	srl	$15,$10,8
	xor	$15,$17,$15
	srl	$14,$9,8
	xor	$14,$15,$14
	srl	$2,$8,8
	xor	$14,$14,$2
	srl	$15,$3,16
	xor	$14,$14,$15
	lw	$2,36($16)
	srl	$12,$7,16
	xor	$12,$14,$12
	srl	$11,$6,16
	xor	$12,$12,$11
	srl	$13,$5,16
	lhu	$15,34($16)
	xor	$12,$12,$13
	srl	$14,$2,4
	lui	$23,%hi(v6_path_table_list_max)
	xor	$12,$12,$14
	lui	$20,%hi(v6_table_path)
	lw	$14,%lo(v6_path_table_list_max)($23)
	xor	$12,$12,$15
	lw	$13,%lo(v6_table_path)($20)
	srl	$11,$12,12
	xor	$12,$11,$12
	addiu	$14,$14,-1
	and	$11,$12,$14
	lw	$12,0($13)
	sll	$21,$11,4
	sll	$13,$11,2
	subu	$21,$21,$13
	addu	$11,$12,$21
	lw	$17,0($11)
	sw	$4,64($sp)
	sw	$10,68($sp)
	sw	$9,72($sp)
	sw	$8,76($sp)
	sw	$3,48($sp)
	sw	$7,52($sp)
	sw	$6,56($sp)
	beq	$17,$0,$L12
	sw	$5,60($sp)

	j	$L33
	addiu	$18,$16,16

$L13:
	lw	$17,52($17)
	nop
	beq	$17,$0,$L40
	lui	$22,%hi(v6_path_list_free)

	lw	$2,36($16)
$L33:
	lw	$3,36($17)
	nop
	bne	$3,$2,$L13
	nop

	lw	$3,40($17)
	lw	$2,40($16)
	nop
	bne	$3,$2,$L13
	move	$4,$17

	move	$5,$16
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L13
	addiu	$4,$17,16

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L13
	nop

	lw	$2,32($17)
	lw	$15,32($16)
	nop
	bne	$2,$15,$L13
	nop

	lw	$8,%lo(v6_table_path)($20)
	j	$L14
	nop

$L12:
	lui	$22,%hi(v6_path_list_free)
$L40:
	lw	$18,%lo(v6_path_list_free)($22)
	nop
	beq	$18,$0,$L37
	nop

	jal	rtl_get_skb_protocol
	move	$4,$19

	move	$4,$19
	jal	rtl_ipv6_hdr
	sw	$2,128($sp)

	move	$17,$2
	jal	rtl_get_skb_mark
	move	$4,$19

	lw	$9,20($17)
	lw	$6,32($17)
	sw	$2,112($sp)
	sw	$9,108($sp)
	lhu	$9,40($17)
	lw	$2,36($17)
	lw	$12,8($17)
	lw	$11,12($17)
	lw	$10,16($17)
	lw	$8,24($17)
	lw	$7,28($17)
	sw	$9,116($sp)
	lhu	$9,42($17)
	move	$4,$19
	move	$5,$0
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
	nop
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
	nop
	sh	$2,42($17)
	lw	$3,128($sp)
	li	$2,34525			# 0x86dd
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
	lw	$9,120($sp)
	sw	$2,12($17)
	lw	$2,104($sp)
	sh	$9,42($17)
	sw	$2,16($17)
	lw	$2,108($sp)
	nop
	sw	$2,20($17)
	lw	$2,80($sp)
	nop
	sw	$2,24($17)
	lw	$2,84($sp)
	nop
	sw	$2,28($17)
	lw	$2,88($sp)
	nop
	sw	$2,32($17)
	lw	$2,92($sp)
	nop
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
	nop
	sw	$5,16($18)
	sw	$4,20($18)
	sw	$3,24($18)
	sw	$2,28($18)
	lw	$2,32($16)
	lui	$3,%hi(jiffies)
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
	nop
$L21:
	sw	$3,0($2)
	lui	$2,%hi(v6_path_list_inuse)
	lw	$4,8($22)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	lw	$3,4($2)
	addiu	$4,$4,-1
	sw	$0,60($18)
	sw	$3,64($18)
	sw	$4,8($22)
	sw	$18,0($3)
	lw	$5,8($2)
	lw	$3,%lo(v6_table_path)($20)
	addiu	$4,$18,60
	addiu	$5,$5,1
	sw	$0,52($18)
	sw	$4,4($2)
	sw	$5,8($2)
	lw	$2,0($3)
	addiu	$4,$18,52
	addu	$2,$2,$21
	lw	$2,4($2)
	nop
	sw	$2,56($18)
	lw	$2,0($3)
	nop
	addu	$2,$2,$21
	lw	$2,4($2)
	nop
	sw	$18,0($2)
	lw	$8,%lo(v6_table_path)($20)
	nop
	lw	$2,0($8)
	nop
	addu	$2,$2,$21
	sw	$4,4($2)
	lw	$2,0($8)
	nop
	addu	$21,$2,$21
	lw	$2,8($21)
	nop
	addiu	$2,$2,1
	sw	$2,8($21)
	lw	$15,32($16)
$L14:
	lw	$7,16($16)
	lw	$13,36($16)
	lw	$11,20($16)
	lw	$10,24($16)
	lw	$9,28($16)
	xor	$13,$7,$13
	lw	$6,0($16)
	xor	$13,$13,$11
	lw	$5,4($16)
	xor	$13,$13,$10
	lw	$4,8($16)
	xor	$13,$13,$9
	lw	$3,12($16)
	xor	$13,$13,$6
	xor	$13,$13,$5
	xor	$13,$13,$4
	srl	$17,$7,8
	xor	$13,$13,$3
	xor	$13,$13,$17
	srl	$14,$11,8
	xor	$13,$13,$14
	srl	$12,$10,8
	xor	$13,$13,$12
	srl	$2,$9,8
	xor	$13,$13,$2
	srl	$14,$6,16
	xor	$13,$13,$14
	srl	$12,$5,16
	lw	$2,40($16)
	xor	$13,$13,$12
	srl	$12,$4,16
	xor	$13,$13,$12
	srl	$12,$3,16
	xor	$13,$13,$12
	srl	$12,$2,4
	xor	$13,$13,$12
	andi	$15,$15,0xffff
	lw	$14,%lo(v6_path_table_list_max)($23)
	xor	$13,$13,$15
	srl	$12,$13,12
	xor	$13,$12,$13
	addiu	$14,$14,-1
	and	$12,$13,$14
	lw	$8,0($8)
	sll	$13,$12,2
	sll	$21,$12,4
	subu	$21,$21,$13
	addu	$8,$8,$21
	lw	$17,0($8)
	sw	$7,32($sp)
	sw	$11,36($sp)
	sw	$10,40($sp)
	sw	$9,44($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	beq	$17,$0,$L22
	sw	$3,28($sp)

	j	$L34
	addiu	$18,$16,16

$L23:
	lw	$17,52($17)
	nop
	beq	$17,$0,$L41
	lui	$22,%hi(v6_path_list_free)

	lw	$2,40($16)
$L34:
	lw	$3,36($17)
	nop
	bne	$3,$2,$L23
	nop

	lw	$3,40($17)
	lw	$2,36($16)
	nop
	bne	$3,$2,$L23
	move	$4,$17

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L23
	addiu	$4,$17,16

	move	$5,$16
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L23
	nop

	lw	$3,32($17)
	lw	$2,32($16)
	nop
	bne	$3,$2,$L23
	move	$2,$0

$L9:
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

$L22:
	lui	$22,%hi(v6_path_list_free)
$L41:
	lw	$18,%lo(v6_path_list_free)($22)
	nop
	beq	$18,$0,$L9
	move	$2,$0

	jal	rtl_get_skb_protocol
	move	$4,$19

	move	$4,$19
	jal	rtl_ipv6_hdr
	sw	$2,128($sp)

	move	$17,$2
	jal	rtl_get_skb_mark
	move	$4,$19

	lw	$9,20($17)
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
	move	$4,$19
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
	nop
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
	nop
	sh	$2,42($17)
	lw	$3,128($sp)
	li	$2,34525			# 0x86dd
	beq	$3,$2,$L39
	lui	$3,%hi(nf_hooks)

$L25:
	lw	$5,112($sp)
	jal	rtl_set_skb_mark
	move	$4,$19

	lw	$2,96($sp)
	lw	$9,116($sp)
	sh	$23,42($17)
	sw	$2,8($17)
	lw	$2,100($sp)
	sh	$9,40($17)
	sw	$2,12($17)
	lw	$2,104($sp)
	nop
	sw	$2,16($17)
	lw	$2,108($sp)
	nop
	sw	$2,20($17)
	lw	$2,80($sp)
	nop
	sw	$2,24($17)
	lw	$2,84($sp)
	nop
	sw	$2,28($17)
	lw	$2,88($sp)
	nop
	sw	$2,32($17)
	lw	$2,92($sp)
	nop
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
	sw	$2,28($18)
	lw	$2,32($16)
	lui	$3,%hi(jiffies)
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
	nop
$L29:
	sw	$3,0($2)
	lui	$2,%hi(v6_path_list_inuse)
	lw	$4,8($22)
	addiu	$2,$2,%lo(v6_path_list_inuse)
	lw	$3,4($2)
	addiu	$4,$4,-1
	sw	$4,8($22)
	sw	$3,64($18)
	sw	$0,60($18)
	sw	$18,0($3)
	lw	$5,8($2)
	lw	$3,%lo(v6_table_path)($20)
	addiu	$4,$18,60
	addiu	$5,$5,1
	sw	$5,8($2)
	sw	$0,52($18)
	sw	$4,4($2)
	lw	$4,0($3)
	addiu	$5,$18,52
	li	$2,1			# 0x1
	addu	$4,$4,$21
	lw	$4,4($4)
	nop
	sw	$4,56($18)
	lw	$3,0($3)
	nop
	addu	$3,$3,$21
	lw	$3,4($3)
	nop
	sw	$18,0($3)
	lw	$3,%lo(v6_table_path)($20)
	nop
	lw	$4,0($3)
	nop
	addu	$4,$4,$21
	sw	$5,4($4)
	lw	$3,0($3)
	nop
	addu	$21,$3,$21
	lw	$3,8($21)
	nop
	addiu	$3,$3,1
	j	$L9
	sw	$3,8($21)

$L37:
	lw	$15,32($16)
	lw	$8,%lo(v6_table_path)($20)
	j	$L14
	nop

$L39:
	addiu	$3,$3,%lo(nf_hooks)
	lw	$4,640($3)
	lui	$2,%hi(nf_hooks+640)
	addiu	$2,$2,%lo(nf_hooks+640)
	beq	$4,$2,$L26
	move	$4,$19

	lw	$6,124($sp)
	move	$5,$0
	move	$7,$0
	jal	rtl_ipt6_do_table
	sw	$3,128($sp)

	lw	$3,128($sp)
	nop
$L26:
	lw	$3,672($3)
	lui	$2,%hi(nf_hooks+672)
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
	addiu	$3,$3,%lo(nf_hooks)
	lw	$4,640($3)
	lui	$2,%hi(nf_hooks+640)
	addiu	$2,$2,%lo(nf_hooks+640)
	beq	$4,$2,$L18
	move	$4,$19

	move	$5,$0
	move	$6,$fp
	move	$7,$0
	jal	rtl_ipt6_do_table
	sw	$3,128($sp)

	lw	$3,128($sp)
	nop
$L18:
	lw	$3,672($3)
	lui	$2,%hi(nf_hooks+672)
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
	nop

	j	$L11
	sw	$0,124($sp)

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
	lw	$7,0($4)
	lw	$13,40($4)
	lw	$10,4($4)
	lw	$9,8($4)
	lw	$8,12($4)
	xor	$13,$7,$13
	lw	$6,16($4)
	xor	$13,$13,$10
	lw	$5,20($4)
	xor	$13,$13,$9
	move	$17,$4
	xor	$13,$13,$8
	lw	$4,24($4)
	lw	$3,28($17)
	xor	$13,$13,$6
	xor	$13,$13,$5
	xor	$13,$13,$4
	srl	$14,$7,8
	xor	$13,$13,$3
	xor	$13,$13,$14
	srl	$12,$10,8
	xor	$12,$13,$12
	srl	$11,$9,8
	xor	$12,$12,$11
	srl	$2,$8,8
	xor	$12,$12,$2
	srl	$13,$6,16
	xor	$12,$12,$13
	lw	$2,36($17)
	srl	$13,$5,16
	xor	$12,$12,$13
	srl	$11,$4,16
	xor	$12,$12,$11
	srl	$11,$3,16
	lhu	$13,34($17)
	xor	$12,$12,$11
	srl	$11,$2,4
	xor	$12,$12,$11
	lui	$11,%hi(v6_path_table_list_max)
	lw	$14,%lo(v6_path_table_list_max)($11)
	lui	$19,%hi(v6_table_path)
	xor	$12,$12,$13
	lw	$13,%lo(v6_table_path)($19)
	srl	$11,$12,12
	xor	$12,$11,$12
	addiu	$14,$14,-1
	and	$11,$12,$14
	lw	$12,0($13)
	sll	$18,$11,4
	sll	$13,$11,2
	subu	$18,$18,$13
	addu	$11,$12,$18
	lw	$16,0($11)
	sw	$7,32($sp)
	sw	$10,36($sp)
	sw	$9,40($sp)
	sw	$8,44($sp)
	sw	$6,16($sp)
	sw	$5,20($sp)
	sw	$4,24($sp)
	beq	$16,$0,$L50
	sw	$3,28($sp)

	addiu	$20,$17,16
	lui	$22,%hi(v6_path_list_free)
	j	$L51
	lui	$21,%hi(v6_path_list_inuse)

$L44:
	lw	$16,52($16)
	nop
	beq	$16,$0,$L50
	nop

	lw	$2,36($17)
$L51:
	lw	$3,36($16)
	nop
	bne	$2,$3,$L44
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	nop
	bne	$3,$2,$L44
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L44
	addiu	$4,$16,16

	move	$5,$20
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L44
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	nop
	bne	$3,$2,$L44
	nop

	lw	$2,52($16)
	nop
	beq	$2,$0,$L45
	nop

	lw	$3,56($16)
	nop
	sw	$3,56($2)
$L46:
	lw	$2,56($16)
	lw	$3,52($16)
	nop
	sw	$3,0($2)
	lw	$2,%lo(v6_table_path)($19)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$2,$18
	lw	$3,8($2)
	nop
	addiu	$3,$3,-1
	sw	$3,8($2)
	lw	$2,60($16)
	nop
	beq	$2,$0,$L47
	addiu	$3,$21,%lo(v6_path_list_inuse)

	lw	$4,64($16)
	nop
	sw	$4,64($2)
	lw	$2,64($16)
	lw	$4,60($16)
	nop
$L48:
	sw	$4,0($2)
	lw	$5,8($3)
	addiu	$2,$22,%lo(v6_path_list_free)
	lw	$4,4($2)
	addiu	$5,$5,-1
	sw	$5,8($3)
	sw	$4,64($16)
	sw	$0,60($16)
	sw	$16,0($4)
	lw	$3,8($2)
	addiu	$4,$16,60
	sw	$4,4($2)
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
	nop
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
	lw	$10,%lo(v6_path_table_list_max)($2)
	nop
	beq	$10,$0,$L65
	nop

	move	$7,$0
	move	$9,$0
	lui	$8,%hi(v6_table_path)
	lui	$12,%hi(v6_path_list_inuse)
	lui	$11,%hi(v6_path_list_free)
$L60:
	lw	$2,%lo(v6_table_path)($8)
	nop
	lw	$2,0($2)
	nop
	addu	$2,$2,$7
	lw	$2,0($2)
	nop
	beq	$2,$0,$L54
	addiu	$5,$12,%lo(v6_path_list_inuse)

	j	$L59
	addiu	$3,$11,%lo(v6_path_list_free)

$L64:
	lw	$6,56($2)
	nop
	sw	$6,56($4)
$L56:
	lw	$4,56($2)
	lw	$6,52($2)
	nop
	sw	$6,0($4)
	lw	$4,%lo(v6_table_path)($8)
	nop
	lw	$4,0($4)
	nop
	addu	$4,$4,$7
	lw	$6,8($4)
	nop
	addiu	$6,$6,-1
	sw	$6,8($4)
	lw	$4,60($2)
	nop
	beq	$4,$0,$L57
	move	$6,$0

	lw	$6,64($2)
	nop
	sw	$6,64($4)
	lw	$4,64($2)
	lw	$6,60($2)
	nop
$L58:
	sw	$6,0($4)
	lw	$6,8($5)
	lw	$4,4($3)
	sw	$0,60($2)
	addiu	$6,$6,-1
	sw	$4,64($2)
	sw	$6,8($5)
	sw	$2,0($4)
	lw	$4,8($3)
	addiu	$6,$2,60
	lw	$2,52($2)
	addiu	$4,$4,1
	sw	$6,4($3)
	beq	$2,$0,$L54
	sw	$4,8($3)

$L59:
	lw	$4,52($2)
	nop
	bne	$4,$0,$L64
	nop

	lw	$4,%lo(v6_table_path)($8)
	lw	$6,56($2)
	lw	$4,0($4)
	nop
	addu	$4,$4,$7
	j	$L56
	sw	$6,4($4)

$L57:
	lw	$4,64($2)
	j	$L58
	sw	$4,4($5)

$L54:
	addiu	$9,$9,1
	bne	$10,$9,$L60
	addiu	$7,$7,12

$L65:
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
	bne	$3,$0,$L72
	move	$16,$6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L67
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$3,$5,1
	or	$2,$3,$5
	lw	$3,24($28)
	#nop
	and	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L72
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
	bne	$6,$0,$L67
	li	$2,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	lb	$2,16($sp)
	lui	$3,%hi(fast_ipv6_fw)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L73
	sw	$2,%lo(fast_ipv6_fw)($3)
	.set	macro
	.set	reorder

	move	$2,$16
$L67:
	lw	$31,28($sp)
	lw	$16,24($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,32
	.set	macro
	.set	reorder

$L73:
	jal	rtk_flushV6Connection
	.set	noreorder
	.set	nomacro
	j	$L67
	move	$2,$16
	.set	macro
	.set	reorder

$L72:
	.set	noreorder
	.set	nomacro
	j	$L67
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
	.frame	$sp,112,$31		# vars= 64, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$22,104($sp)
	sw	$21,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
	move	$17,$4
	lw	$3,40($17)
	lw	$4,0($4)
	lw	$9,4($17)
	lw	$8,8($17)
	lw	$7,12($17)
	xor	$19,$4,$3
	lw	$6,16($17)
	xor	$19,$19,$9
	lw	$12,20($17)
	xor	$19,$19,$8
	lw	$11,24($17)
	xor	$19,$19,$7
	lw	$10,28($17)
	xor	$19,$19,$6
	xor	$19,$19,$12
	xor	$19,$19,$11
	srl	$20,$4,8
	xor	$19,$19,$10
	xor	$19,$19,$20
	srl	$18,$9,8
	xor	$18,$19,$18
	srl	$16,$8,8
	xor	$16,$18,$16
	srl	$2,$7,8
	xor	$16,$16,$2
	srl	$18,$6,16
	xor	$16,$16,$18
	lw	$2,36($17)
	srl	$18,$12,16
	xor	$16,$16,$18
	srl	$13,$11,16
	xor	$13,$16,$13
	srl	$14,$10,16
	lhu	$15,34($17)
	xor	$13,$13,$14
	srl	$16,$2,4
	lui	$18,%hi(v6_path_table_list_max)
	xor	$13,$13,$16
	lw	$14,%lo(v6_path_table_list_max)($18)
	lui	$19,%hi(v6_table_path)
	xor	$13,$13,$15
	lw	$20,%lo(v6_table_path)($19)
	srl	$16,$13,12
	xor	$16,$16,$13
	addiu	$14,$14,-1
	and	$16,$16,$14
	lw	$13,0($20)
	sll	$20,$16,2
	sll	$16,$16,4
	subu	$16,$16,$20
	addu	$16,$13,$16
	lw	$16,0($16)
	move	$21,$5
	lui	$5,%hi(jiffies)
	lw	$22,%lo(jiffies)($5)
	sw	$4,64($sp)
	sw	$9,68($sp)
	sw	$8,72($sp)
	sw	$7,76($sp)
	sw	$6,48($sp)
	sw	$12,52($sp)
	sw	$11,56($sp)
	beq	$16,$0,$L75
	sw	$10,60($sp)

	j	$L89
	addiu	$20,$17,16

$L76:
	lw	$16,52($16)
	nop
	beq	$16,$0,$L93
	nop

$L89:
	lw	$4,36($16)
	nop
	bne	$4,$2,$L76
	nop

	lw	$4,40($16)
	nop
	bne	$4,$3,$L76
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	beq	$2,$0,$L77
	addiu	$4,$16,16

$L92:
	lw	$16,52($16)
	lw	$3,40($17)
	lw	$2,36($17)
	bne	$16,$0,$L89
	nop

$L93:
	lw	$4,%lo(v6_table_path)($19)
	lw	$14,%lo(v6_path_table_list_max)($18)
	lhu	$15,34($17)
	lw	$13,0($4)
	lw	$9,4($17)
	lw	$4,0($17)
	lw	$8,8($17)
	lw	$7,12($17)
	lw	$6,16($17)
	lw	$12,20($17)
	lw	$11,24($17)
	lw	$10,28($17)
	addiu	$14,$14,-1
$L75:
	xor	$19,$12,$6
	xor	$19,$19,$11
	xor	$19,$19,$10
	xor	$19,$19,$4
	xor	$19,$19,$9
	xor	$19,$19,$8
	xor	$19,$19,$7
	srl	$20,$6,8
	xor	$19,$19,$20
	srl	$18,$12,8
	xor	$18,$19,$18
	srl	$16,$11,8
	xor	$16,$18,$16
	srl	$5,$10,8
	xor	$5,$16,$5
	srl	$18,$4,16
	srl	$16,$9,16
	xor	$5,$5,$18
	xor	$5,$5,$16
	srl	$16,$8,16
	xor	$5,$5,$16
	srl	$16,$7,16
	xor	$5,$5,$16
	xor	$5,$5,$2
	xor	$15,$5,$15
	srl	$3,$3,4
	xor	$3,$15,$3
	srl	$5,$3,12
	xor	$3,$5,$3
	and	$14,$3,$14
	sll	$3,$14,2
	sll	$14,$14,4
	subu	$14,$14,$3
	addu	$13,$13,$14
	lw	$16,0($13)
	sw	$6,32($sp)
	sw	$12,36($sp)
	sw	$11,40($sp)
	sw	$10,44($sp)
	sw	$4,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	beq	$16,$0,$L88
	sw	$7,28($sp)

	j	$L91
	addiu	$18,$17,16

$L82:
	lw	$16,52($16)
	nop
	beq	$16,$0,$L80
	move	$2,$0

	lw	$2,36($17)
$L91:
	lw	$3,36($16)
	nop
	bne	$3,$2,$L82
	nop

	lw	$3,40($16)
	lw	$2,40($17)
	nop
	bne	$3,$2,$L82
	move	$4,$16

	move	$5,$17
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L82
	addiu	$4,$16,16

	move	$5,$18
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L82
	nop

	lw	$3,32($16)
	lw	$2,32($17)
	nop
	bne	$3,$2,$L82
	subu	$3,$22,$21

	lw	$2,44($16)
	nop
	subu	$3,$3,$2
	slt	$3,$3,0
	li	$2,-1			# 0xffffffffffffffff
	j	$L80
	movz	$2,$0,$3

$L88:
	move	$2,$0
$L80:
	lw	$31,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	j	$31
	addiu	$sp,$sp,112

$L77:
	move	$5,$20
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L92
	nop

	lw	$2,32($17)
	lw	$15,32($16)
	nop
	bne	$15,$2,$L92
	subu	$3,$22,$21

	lw	$2,44($16)
	nop
	subu	$2,$3,$2
	bltz	$2,$L85
	andi	$15,$15,0xffff

	lw	$4,%lo(v6_table_path)($19)
	lw	$14,%lo(v6_path_table_list_max)($18)
	lw	$3,40($17)
	lw	$13,0($4)
	lw	$2,36($17)
	lw	$4,0($17)
	lw	$9,4($17)
	lw	$8,8($17)
	lw	$7,12($17)
	lw	$6,16($17)
	lw	$12,20($17)
	lw	$11,24($17)
	lw	$10,28($17)
	j	$L75
	addiu	$14,$14,-1

$L85:
	j	$L80
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
	beq	$2,$3,$L102
	move	$4,$16

	jal	rtl_get_ct_protonum
	move	$5,$0

	li	$3,6			# 0x6
	bne	$2,$3,$L101
	nop

	jal	rtl_get_ct_tcp_state
	move	$4,$16

	sltu	$2,$2,6
	beq	$2,$0,$L101
	nop

	jal	rtl_tcp_get_timeouts
	move	$4,$16

	move	$17,$2
$L97:
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

	bne	$2,$0,$L99
	addu	$5,$17,$18

$L101:
	lw	$31,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	li	$2,-1			# 0xffffffffffffffff
	j	$31
	addiu	$sp,$sp,80

$L99:
	jal	rtl_check_for_acc
	move	$4,$16

	lw	$31,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,80

$L102:
	jal	rtl_get_ct_udp_status
	nop

	andi	$2,$2,0x2
	beq	$2,$0,$L96
	nop

	li	$4,1			# 0x1
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	j	$L97
	move	$17,$2

$L96:
	move	$4,$0
	move	$5,$16
	jal	udp_get_timeouts_by_state
	li	$6,1			# 0x1

	j	$L97
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
	beq	$2,$0,$L111
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
	nop
	lw	$5,0($3)
	nop
	beq	$5,$0,$L109
	nop

	blez	$17,$L105
	move	$2,$0

	j	$L106
	move	$4,$0

$L113:
	lw	$5,0($3)
	nop
$L106:
	addu	$5,$5,$2
	sw	$0,8($5)
	lw	$3,0($3)
	addiu	$4,$4,1
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(V6_udp_cache_table)($20)
	nop
	lw	$5,0($3)
	nop
	addu	$5,$5,$2
	sw	$5,4($5)
	bne	$4,$17,$L113
	addiu	$2,$2,12

$L105:
	lw	$2,%lo(V6_max_udp_frag_entry)($18)
	nop
	blez	$2,$L110
	lui	$20,%hi(V6_cache_timeout)

	move	$17,$0
	j	$L107
	addiu	$20,$20,%lo(V6_cache_timeout)

$L114:
	jal	init_timer_key
	sw	$2,16($sp)

	lw	$2,16($sp)
	lw	$3,4($16)
	sw	$2,72($2)
	sw	$3,92($2)
	sw	$20,68($2)
	sw	$0,88($2)
	sw	$2,0($3)
	lw	$4,8($16)
	lw	$3,%lo(V6_max_udp_frag_entry)($18)
	addiu	$2,$2,88
	addiu	$4,$4,1
	slt	$3,$17,$3
	sw	$2,4($16)
	beq	$3,$0,$L110
	sw	$4,8($16)

$L107:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	addiu	$4,$2,52
	move	$5,$0
	move	$6,$0
	bne	$2,$0,$L114
	move	$7,$0

$L111:
	move	$2,$0
$L104:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L110:
	j	$L104
	li	$2,1			# 0x1

$L109:
	j	$L104
	li	$2,-1			# 0xffffffffffffffff

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

	bne	$2,$0,$L116
	nop

	lbu	$3,12($16)
	lbu	$2,13($16)
	sll	$3,$3,8
	or	$3,$2,$3
	li	$2,34525			# 0x86dd
	beq	$3,$2,$L116
	nop

$L151:
	move	$2,$0
$L117:
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

$L116:
	jal	__ipv6_addr_type
	addiu	$4,$18,8

	andi	$2,$2,0x20
	bne	$2,$0,$L117
	move	$2,$0

	jal	__ipv6_addr_type
	addiu	$4,$18,24

	andi	$2,$2,0x22
	bne	$2,$0,$L151
	li	$3,6			# 0x6

	lbu	$2,6($18)
	nop
	beq	$2,$3,$L118
	li	$3,17			# 0x11

	beq	$2,$3,$L119
	li	$3,44			# 0x2c

	bne	$2,$3,$L117
	move	$2,$0

$L119:
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
	sw	$8,128($sp)
	sw	$7,132($sp)
	sw	$6,136($sp)
	sw	$5,112($sp)
	sw	$4,116($sp)
	sw	$3,120($sp)
	lbu	$22,6($18)
	li	$2,44			# 0x2c
	lhu	$19,40($18)
	lhu	$20,42($18)
	beq	$22,$2,$L120
	andi	$23,$22,0xffff

	move	$21,$0
	sw	$0,144($sp)
	move	$fp,$0
	sw	$0,148($sp)
$L121:
	lw	$9,128($sp)
	lw	$8,132($sp)
	lw	$6,140($sp)
	lw	$5,112($sp)
	xor	$10,$8,$9
	lw	$4,116($sp)
	lw	$3,120($sp)
	xor	$10,$10,$6
	lw	$2,124($sp)
	xor	$10,$10,$5
	lw	$7,136($sp)
	xor	$10,$10,$4
	xor	$10,$10,$3
	xor	$10,$10,$2
	xor	$10,$10,$7
	srl	$9,$9,8
	xor	$9,$10,$9
	srl	$8,$8,8
	xor	$8,$9,$8
	srl	$7,$7,8
	xor	$7,$8,$7
	srl	$6,$6,8
	xor	$6,$7,$6
	srl	$5,$5,16
	xor	$5,$6,$5
	srl	$4,$4,16
	xor	$4,$5,$4
	srl	$3,$3,16
	xor	$3,$4,$3
	srl	$2,$2,16
	xor	$3,$3,$2
	xor	$3,$3,$20
	srl	$2,$19,4
	lui	$4,%hi(v6_path_table_list_max)
	xor	$3,$3,$23
	xor	$3,$3,$2
	lw	$5,%lo(v6_path_table_list_max)($4)
	lui	$2,%hi(v6_table_path)
	lw	$4,%lo(v6_table_path)($2)
	srl	$2,$3,12
	xor	$3,$2,$3
	addiu	$5,$5,-1
	and	$2,$3,$5
	lw	$3,0($4)
	sll	$4,$2,2
	sll	$2,$2,4
	subu	$2,$2,$4
	addu	$2,$3,$2
	lw	$16,0($2)
	nop
	bne	$16,$0,$L152
	move	$2,$0

	j	$L117
	nop

$L128:
	lw	$16,52($16)
	nop
	beq	$16,$0,$L117
	move	$2,$0

$L152:
	lw	$2,36($16)
	nop
	bne	$2,$19,$L128
	nop

	lw	$2,40($16)
	nop
	bne	$2,$20,$L128
	move	$4,$16

	addiu	$5,$sp,128
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L128
	addiu	$4,$16,16

	addiu	$5,$sp,112
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L128
	nop

	lw	$2,32($16)
	nop
	bne	$2,$22,$L128
	nop

	jal	rtl_ip6_route_input
	move	$4,$17

	li	$5,-1			# 0xffffffffffffffff
	beq	$2,$5,$L117
	move	$2,$0

	jal	rtl_get_skb_ipv6_dst
	move	$4,$17

	beq	$2,$0,$L117
	move	$2,$0

	jal	rtl_skb_ipv6_dst_check
	move	$4,$17

	li	$3,-1			# 0xffffffffffffffff
	beq	$2,$3,$L156
	move	$4,$17

	jal	rtl_check_dst_input
	nop

	li	$5,-1			# 0xffffffffffffffff
	beq	$2,$5,$L156
	move	$4,$17

	lw	$3,148($sp)
	nop
	beq	$3,$0,$L131
	lui	$4,%hi(V6_udp_cache_list_free)

	lw	$5,144($sp)
	lw	$31,132($sp)
	lw	$11,136($sp)
	lw	$10,140($sp)
	lw	$9,116($sp)
	lw	$8,120($sp)
	lw	$6,124($sp)
	lw	$7,128($sp)
	lw	$3,112($sp)
	lw	$2,%lo(V6_udp_cache_list_free)($4)
	lw	$12,4($5)
	lui	$5,%hi(V6_max_udp_frag_entry)
	sw	$7,64($sp)
	sw	$31,68($sp)
	sw	$11,72($sp)
	sw	$10,76($sp)
	sw	$3,48($sp)
	sw	$9,52($sp)
	sw	$8,56($sp)
	sw	$6,60($sp)
	move	$25,$31
	move	$24,$11
	move	$21,$10
	move	$15,$9
	move	$14,$8
	lw	$fp,%lo(V6_max_udp_frag_entry)($5)
	beq	$2,$0,$L132
	move	$13,$6

	lw	$5,88($2)
	andi	$19,$19,0xffff
	andi	$20,$20,0xffff
	sw	$6,32($2)
	li	$6,1			# 0x1
	sb	$6,48($2)
	sw	$12,0($2)
	sw	$7,4($2)
	sw	$31,8($2)
	sw	$11,12($2)
	sw	$10,16($2)
	sw	$3,20($2)
	sw	$9,24($2)
	sw	$8,28($2)
	sw	$22,36($2)
	sw	$19,40($2)
	sw	$20,44($2)
	lw	$6,92($2)
	beq	$5,$0,$L133
	addiu	$4,$4,%lo(V6_udp_cache_list_free)

	sw	$6,92($5)
	lw	$5,88($2)
	lw	$6,92($2)
$L134:
	xor	$8,$25,$7
	xor	$8,$8,$24
	xor	$8,$8,$21
	xor	$8,$8,$3
	xor	$8,$8,$15
	xor	$8,$8,$14
	xor	$8,$8,$13
	srl	$7,$7,8
	xor	$7,$8,$7
	srl	$25,$25,8
	xor	$25,$7,$25
	srl	$24,$24,8
	xor	$24,$25,$24
	srl	$21,$21,8
	xor	$21,$24,$21
	srl	$3,$3,16
	xor	$3,$21,$3
	srl	$15,$15,16
	xor	$15,$3,$15
	srl	$14,$14,16
	xor	$14,$15,$14
	srl	$13,$13,16
	xor	$13,$14,$13
	xor	$12,$13,$12
	srl	$13,$13,16
	xor	$12,$12,$13
	xor	$23,$12,$23
	addiu	$fp,$fp,-1
	li	$3,12			# 0xc
	and	$23,$23,$fp
	mult	$23,$3
	sw	$5,0($6)
	lw	$7,8($4)
	lui	$5,%hi(V6_udp_cache_table)
	lw	$6,%lo(V6_udp_cache_table)($5)
	addiu	$7,$7,-1
	sw	$7,8($4)
	sw	$0,80($2)
	lw	$7,0($6)
	addiu	$8,$2,80
	addiu	$4,$2,52
	mflo	$3
	addu	$7,$7,$3
	lw	$7,4($7)
	nop
	sw	$7,84($2)
	lw	$6,0($6)
	nop
	addu	$6,$6,$3
	lw	$6,4($6)
	nop
	sw	$2,0($6)
	lw	$5,%lo(V6_udp_cache_table)($5)
	lui	$6,%hi(jiffies)
	lw	$6,%lo(jiffies)($6)
	lw	$7,0($5)
	addiu	$6,$6,1000
	addu	$7,$7,$3
	sw	$8,4($7)
	lw	$5,0($5)
	nop
	addu	$3,$5,$3
	lw	$5,8($3)
	nop
	addiu	$5,$5,1
	sw	$5,8($3)
	jal	add_timer
	sw	$6,60($2)

$L135:
	move	$4,$17
$L158:
	jal	rtl_set_skb_dev
	move	$5,$0

	lbu	$2,7($18)
	nop
	sltu	$3,$2,2
	bne	$3,$0,$L138
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
	beq	$2,$0,$L139
	nop

	jal	rtl_skb_is_gso
	move	$4,$17

	beq	$2,$0,$L138
	nop

$L139:
	jal	rtl_dst_allfrag
	move	$4,$17

	beq	$2,$0,$L157
	nop

$L138:
	jal	kfree_skb
	move	$4,$17

	j	$L117
	li	$2,1			# 0x1

$L118:
	lw	$3,52($18)
	li	$2,458752			# 0x70000
	and	$2,$3,$2
	beq	$2,$0,$L119
	move	$2,$0

	j	$L117
	nop

$L120:
	lbu	$3,40($18)
	addiu	$2,$18,40
	sw	$2,144($sp)
	li	$2,17			# 0x11
	beq	$3,$2,$L122
	li	$23,44			# 0x2c

	move	$21,$0
	move	$fp,$0
	j	$L121
	sw	$0,148($sp)

$L122:
	andi	$3,$20,0xfff9
	li	$2,1			# 0x1
	beq	$3,$2,$L123
	andi	$2,$20,0xfff8

	beq	$2,$0,$L124
	move	$fp,$0

	andi	$20,$20,0x1
	xori	$fp,$20,0x1
$L124:
	lw	$4,128($sp)
	lw	$10,132($sp)
	lw	$9,136($sp)
	lw	$8,140($sp)
	lw	$3,112($sp)
	xor	$19,$10,$4
	lw	$7,116($sp)
	xor	$19,$19,$9
	lw	$6,120($sp)
	xor	$19,$19,$8
	lw	$5,124($sp)
	xor	$19,$19,$3
	xor	$19,$19,$7
	xor	$19,$19,$6
	srl	$16,$4,8
	xor	$19,$19,$5
	xor	$16,$19,$16
	srl	$15,$10,8
	xor	$15,$16,$15
	srl	$14,$9,8
	xor	$14,$15,$14
	srl	$13,$8,8
	xor	$13,$14,$13
	srl	$14,$3,16
	xor	$13,$13,$14
	lw	$16,44($18)
	srl	$11,$7,16
	xor	$11,$13,$11
	srl	$12,$6,16
	lui	$13,%hi(V6_max_udp_frag_entry)
	xor	$12,$11,$12
	lui	$2,%hi(V6_udp_cache_table)
	srl	$11,$5,16
	lw	$13,%lo(V6_max_udp_frag_entry)($13)
	xor	$11,$12,$11
	xori	$14,$16,0x11
	lw	$12,%lo(V6_udp_cache_table)($2)
	srl	$2,$11,16
	xor	$11,$14,$11
	sw	$4,96($sp)
	sw	$10,100($sp)
	sw	$9,104($sp)
	sw	$8,108($sp)
	sw	$3,80($sp)
	sw	$7,84($sp)
	sw	$6,88($sp)
	sw	$5,92($sp)
	xor	$11,$11,$2
	addiu	$2,$13,-1
	and	$2,$11,$2
	lw	$3,0($12)
	sll	$4,$2,2
	sll	$2,$2,4
	subu	$2,$2,$4
	addu	$2,$3,$2
	lw	$21,0($2)
	nop
	beq	$21,$0,$L151
	li	$19,17			# 0x11

	j	$L153
	nop

$L125:
	lw	$21,80($21)
	nop
	beq	$21,$0,$L117
	move	$2,$0

$L153:
	lw	$2,0($21)
	nop
	bne	$16,$2,$L125
	addiu	$4,$21,4

	addiu	$5,$sp,96
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L125
	addiu	$4,$21,20

	addiu	$5,$sp,80
	jal	memcmp
	li	$6,16			# 0x10

	bne	$2,$0,$L125
	nop

	lw	$2,36($21)
	nop
	bne	$2,$19,$L125
	nop

	beq	$21,$0,$L151
	li	$23,17			# 0x11

	lw	$19,40($21)
	lw	$20,44($21)
	sw	$0,148($sp)
	j	$L121
	li	$22,17			# 0x11

$L123:
	li	$3,1			# 0x1
	lhu	$19,48($18)
	lhu	$20,50($18)
	li	$23,17			# 0x11
	move	$21,$0
	move	$fp,$0
	sw	$3,148($sp)
	j	$L121
	li	$22,17			# 0x11

$L157:
	lui	$2,%hi(jiffies)
	lw	$2,%lo(jiffies)($2)
	move	$4,$17
	jal	ip_finish_output6
	sw	$2,44($16)

	lui	$3,%hi(statistic_ipv6_fp)
	lw	$4,%lo(statistic_ipv6_fp)($3)
	li	$2,1			# 0x1
	addiu	$4,$4,1
	j	$L117
	sw	$4,%lo(statistic_ipv6_fp)($3)

$L133:
	j	$L134
	sw	$6,4($4)

$L131:
	beq	$fp,$0,$L158
	move	$4,$17

	beq	$21,$0,$L158
	nop

	jal	del_timer
	addiu	$4,$21,52

	lw	$7,4($21)
	lw	$6,8($21)
	lw	$5,12($21)
	lw	$4,16($21)
	lw	$2,20($21)
	xor	$19,$6,$7
	lw	$11,24($21)
	xor	$19,$19,$5
	lw	$10,28($21)
	xor	$19,$19,$4
	lw	$9,32($21)
	xor	$19,$19,$2
	xor	$19,$19,$11
	xor	$19,$19,$10
	srl	$13,$7,8
	xor	$19,$19,$9
	xor	$19,$19,$13
	srl	$13,$6,8
	xor	$19,$19,$13
	srl	$13,$5,8
	xor	$19,$19,$13
	srl	$13,$4,8
	xor	$19,$19,$13
	srl	$13,$2,16
	xor	$13,$19,$13
	srl	$12,$11,16
	xor	$12,$13,$12
	srl	$8,$10,16
	lw	$14,0($21)
	lhu	$15,38($21)
	xor	$12,$12,$8
	lui	$8,%hi(V6_max_udp_frag_entry)
	srl	$3,$9,16
	lw	$13,%lo(V6_max_udp_frag_entry)($8)
	xor	$12,$12,$3
	xor	$14,$15,$14
	lw	$8,80($21)
	srl	$3,$12,16
	xor	$12,$14,$12
	xor	$12,$12,$3
	addiu	$3,$13,-1
	sw	$7,16($sp)
	sw	$6,20($sp)
	sw	$5,24($sp)
	sw	$4,28($sp)
	sw	$2,32($sp)
	sw	$11,36($sp)
	sw	$10,40($sp)
	sw	$9,44($sp)
	and	$3,$12,$3
	beq	$8,$0,$L136
	sb	$0,48($21)

	lw	$4,84($21)
	lui	$2,%hi(V6_udp_cache_table)
	sw	$4,84($8)
$L137:
	li	$4,12			# 0xc
	mult	$3,$4
	lw	$5,84($21)
	lw	$6,80($21)
	lui	$4,%hi(V6_udp_cache_list_free)
	addiu	$4,$4,%lo(V6_udp_cache_list_free)
	sw	$6,0($5)
	lw	$6,%lo(V6_udp_cache_table)($2)
	lw	$2,4($4)
	addiu	$5,$21,88
	lw	$6,0($6)
	mflo	$3
	addu	$3,$6,$3
	lw	$6,8($3)
	nop
	addiu	$6,$6,-1
	sw	$6,8($3)
	sw	$2,92($21)
	sw	$0,88($21)
	sw	$21,0($2)
	lw	$2,8($4)
	sw	$5,4($4)
	addiu	$2,$2,1
	j	$L135
	sw	$2,8($4)

$L132:
	move	$4,$17
$L156:
	jal	rtl_dst_release
	nop

	j	$L117
	move	$2,$0

$L136:
	li	$4,12			# 0xc
	mult	$3,$4
	lui	$2,%hi(V6_udp_cache_table)
	lw	$6,%lo(V6_udp_cache_table)($2)
	lw	$5,84($21)
	lw	$6,0($6)
	mflo	$4
	addu	$4,$6,$4
	j	$L137
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
	jal	kmem_cache_alloc
	li	$5,32800			# 0x8020

	lui	$18,%hi(v6_table_path)
	move	$21,$2
	beq	$2,$0,$L165
	sw	$2,%lo(v6_table_path)($18)

	lui	$6,%hi(v6_path_list_inuse)
	lui	$3,%hi(v6_path_list_free)
	addiu	$2,$6,%lo(v6_path_list_inuse)
	sll	$5,$17,4
	sll	$4,$17,2
	addiu	$16,$3,%lo(v6_path_list_free)
	subu	$4,$5,$4
	sw	$0,8($2)
	li	$5,32			# 0x20
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
	nop
	lw	$5,0($3)
	nop
	beq	$5,$0,$L165
	nop

	blez	$17,$L161
	move	$2,$0

	j	$L162
	move	$4,$0

$L169:
	lw	$5,0($3)
	nop
$L162:
	addu	$5,$5,$2
	sw	$0,8($5)
	lw	$3,0($3)
	addiu	$4,$4,1
	addu	$3,$3,$2
	sw	$0,0($3)
	lw	$3,%lo(v6_table_path)($18)
	nop
	lw	$5,0($3)
	nop
	addu	$5,$5,$2
	sw	$5,4($5)
	bne	$4,$17,$L169
	addiu	$2,$2,12

$L161:
	blez	$20,$L166
	move	$17,$0

	j	$L163
	nop

$L170:
	lw	$3,4($16)
	sw	$0,60($2)
	sw	$3,64($2)
	sw	$2,0($3)
	lw	$2,8($16)
	sw	$4,4($16)
	addiu	$2,$2,1
	beq	$17,$20,$L166
	sw	$2,8($16)

$L163:
	lw	$4,4($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	addiu	$17,$17,1

	bne	$2,$0,$L170
	addiu	$4,$2,60

	j	$L160
	li	$2,-2			# 0xfffffffffffffffe

$L166:
	move	$2,$0
$L160:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L165:
	j	$L160
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

	lw	$31,28($sp)
	lui	$3,%hi(ipv6_res)
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
	nop
	beq	$2,$0,$L173
	lui	$4,%hi($LC1)

	lui	$5,%hi(proc_root)
	addiu	$4,$4,%lo($LC1)
	jal	remove_proc_entry
	addiu	$5,$5,%lo(proc_root)

$L173:
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
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2080) 4.6.4"
