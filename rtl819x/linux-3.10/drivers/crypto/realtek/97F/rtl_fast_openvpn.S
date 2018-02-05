	.file	1 "rtl_fast_openvpn.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.text
	.align	2
	.globl	get_openvpn_port
	.set	nomips16
	.ent	get_openvpn_port
	.type	get_openvpn_port, @function
get_openvpn_port:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$2,%hi(vpn_shared_info+34)
	j	$31
	lhu	$2,%lo(vpn_shared_info+34)($2)

	.set	macro
	.set	reorder
	.end	get_openvpn_port
	.size	get_openvpn_port, .-get_openvpn_port
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"\012Key Update Timeout!!!\012\000"
	.text
	.align	2
	.globl	key_update_timeout
	.set	nomips16
	.ent	key_update_timeout
	.type	key_update_timeout, @function
key_update_timeout:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	move	$16,$4
	lui	$4,%hi($LC0)
	jal	printk
	addiu	$4,$4,%lo($LC0)

	sll	$3,$16,4
	sll	$2,$16,2
	subu	$2,$3,$2
	addu	$16,$2,$16
	lui	$2,%hi(vpn_client_info)
	addiu	$2,$2,%lo(vpn_client_info)
	sll	$16,$16,4
	addu	$16,$2,$16
	li	$2,1			# 0x1
	sw	$2,64($16)
	sw	$0,68($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	key_update_timeout
	.size	key_update_timeout, .-key_update_timeout
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	" \000"
	.align	2
$LC2:
	.ascii	"%02x\000"
	.align	2
$LC3:
	.ascii	"\012\012\000"
	.text
	.align	2
	.globl	dump_openvpn_info
	.set	nomips16
	.ent	dump_openvpn_info
	.type	dump_openvpn_info, @function
dump_openvpn_info:
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
	move	$16,$5
	bne	$5,$0,$L14
	move	$17,$4

$L6:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	lui	$4,%hi($LC3)
	addiu	$4,$4,%lo($LC3)
	j	printk
	addiu	$sp,$sp,56

$L14:
	lui	$20,%hi($LC2)
	lui	$19,%hi($LC1)
	addiu	$20,$20,%lo($LC2)
	addiu	$19,$19,%lo($LC1)
	move	$21,$0
	move	$18,$0
$L12:
	addu	$21,$17,$21
	lbu	$5,0($21)
	addiu	$18,$18,1
	move	$4,$20
	jal	printk
	sw	$6,16($sp)

	sltu	$2,$18,$16
	move	$21,$18
	beq	$2,$0,$L6
	lw	$6,16($sp)

	beq	$18,$0,$L12
	nop

	divu	$0,$18,$6
	mfhi	$2
	bne	$2,$0,$L12
	move	$4,$19

	jal	printk
	sw	$6,16($sp)

	j	$L12
	lw	$6,16($sp)

	.set	macro
	.set	reorder
	.end	dump_openvpn_info
	.size	dump_openvpn_info, .-dump_openvpn_info
	.section	.rodata.str1.4
	.align	2
$LC4:
	.ascii	"\012%s call netlink_kernel_create()!!!\012\000"
	.align	2
$LC5:
	.ascii	"\0013kernel create openvpn netlink socket fail!\012\000"
	.text
	.align	2
	.globl	rtl_fast_openvpn_init
	.set	nomips16
	.ent	rtl_fast_openvpn_init
	.type	rtl_fast_openvpn_init, @function
rtl_fast_openvpn_init:
	.frame	$sp,64,$31		# vars= 24, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-64
	sw	$31,60($sp)
	sw	$20,56($sp)
	sw	$19,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$4,%hi($LC4)
	lui	$5,%hi(__func__.48870)
	lui	$2,%hi(rtk_openvpn_netlink_receive)
	addiu	$4,$4,%lo($LC4)
	addiu	$5,$5,%lo(__func__.48870)
	addiu	$2,$2,%lo(rtk_openvpn_netlink_receive)
	sw	$2,24($sp)
	sw	$0,16($sp)
	sw	$0,20($sp)
	sw	$0,28($sp)
	jal	printk
	sw	$0,32($sp)

	lui	$4,%hi(init_net)
	addiu	$4,$4,%lo(init_net)
	li	$5,27			# 0x1b
	move	$6,$0
	jal	__netlink_kernel_create
	addiu	$7,$sp,16

	lui	$3,%hi(nl_openvpn_sk)
	beq	$2,$0,$L21
	sw	$2,%lo(nl_openvpn_sk)($3)

	lui	$18,%hi(vpn_client_info)
	addiu	$18,$18,%lo(vpn_client_info)
	move	$16,$0
	move	$20,$18
	li	$19,16			# 0x10
	sll	$2,$16,2
$L22:
	sll	$17,$16,4
	subu	$17,$17,$2
	addu	$17,$17,$16
	sll	$17,$17,4
	addiu	$17,$17,72
	addu	$17,$18,$17
	move	$4,$17
	move	$5,$0
	jal	memset
	li	$6,24			# 0x18

	addiu	$16,$16,1
	addiu	$4,$17,24
	move	$5,$0
	jal	memset
	li	$6,24			# 0x18

	bne	$16,$19,$L22
	sll	$2,$16,2

	move	$4,$20
	move	$5,$0
	li	$6,3328			# 0xd00
	lui	$16,%hi(vpn_shared_info)
	jal	memset
	lui	$18,%hi(kmalloc_caches)

	addiu	$18,$18,%lo(kmalloc_caches)
	li	$6,52			# 0x34
	addiu	$4,$16,%lo(vpn_shared_info)
	jal	memset
	move	$5,$0

	lw	$4,44($18)
	addiu	$16,$16,%lo(vpn_shared_info)
	li	$2,20			# 0x14
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sb	$2,49($16)

	li	$17,536870912			# 0x20000000
	lw	$4,44($18)
	or	$2,$2,$17
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,12($16)

	lw	$4,44($18)
	or	$2,$2,$17
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,16($16)

	lw	$4,44($18)
	or	$2,$2,$17
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,24($16)

	lw	$4,28($18)
	or	$2,$2,$17
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,28($16)

	or	$2,$2,$17
	li	$3,1			# 0x1
	sw	$2,20($16)
	li	$2,-22012			# 0xffffffffffffaa04
	sh	$2,34($16)
	sh	$3,50($16)
	move	$2,$0
$L17:
	lw	$31,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,64

$L21:
	lui	$4,%hi($LC5)
	jal	printk
	addiu	$4,$4,%lo($LC5)

	j	$L17
	li	$2,-1			# 0xffffffffffffffff

	.set	macro
	.set	reorder
	.end	rtl_fast_openvpn_init
	.size	rtl_fast_openvpn_init, .-rtl_fast_openvpn_init
	.section	.rodata.str1.4
	.align	2
$LC6:
	.ascii	"Client Reach to Max Number!\012\000"
	.text
	.align	2
	.globl	lookup_client_by_ip_and_port
	.set	nomips16
	.ent	lookup_client_by_ip_and_port
	.type	lookup_client_by_ip_and_port, @function
lookup_client_by_ip_and_port:
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
	lui	$2,%hi(vpn_client_info+52)
	andi	$5,$5,0xffff
	addiu	$2,$2,%lo(vpn_client_info+52)
	move	$16,$0
	j	$L26
	li	$7,16			# 0x10

$L24:
	addiu	$16,$16,1
$L36:
	beq	$16,$7,$L33
	addiu	$2,$2,208

$L26:
	lw	$3,0($2)
	bne	$3,$4,$L24
	nop

	lhu	$3,8($2)
	bne	$3,$5,$L36
	addiu	$16,$16,1

	addiu	$16,$16,-1
$L25:
	move	$2,$16
$L37:
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

$L33:
	li	$2,1			# 0x1
	bne	$6,$2,$L25
	lui	$2,%hi(vpn_client_info+122)

	addiu	$2,$2,%lo(vpn_client_info+122)
	move	$16,$0
	li	$4,16			# 0x10
$L29:
	lbu	$3,0($2)
	beq	$3,$0,$L34
	addiu	$2,$2,208

	addiu	$16,$16,1
	bne	$16,$4,$L29
	nop

	lui	$4,%hi($LC6)
	jal	printk
	addiu	$4,$4,%lo($LC6)

	j	$L25
	li	$16,-1			# 0xffffffffffffffff

$L34:
	sll	$21,$16,2
	sll	$22,$16,4
	subu	$17,$22,$21
	addu	$17,$17,$16
	lui	$20,%hi(vpn_client_info)
	sll	$17,$17,4
	addiu	$20,$20,%lo(vpn_client_info)
	addu	$17,$20,$17
	lw	$2,72($17)
	beq	$2,$0,$L35
	lui	$19,%hi(kmalloc_caches)

$L28:
	subu	$17,$22,$21
	addu	$17,$17,$16
	sll	$17,$17,4
	addu	$17,$20,$17
	lw	$2,96($17)
	bne	$2,$0,$L37
	move	$2,$16

	lui	$19,%hi(kmalloc_caches)
	addiu	$19,$19,%lo(kmalloc_caches)
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	li	$18,536870912			# 0x20000000

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,96($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,100($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,104($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,108($17)

	or	$2,$2,$18
	sw	$2,112($17)
	j	$L25
	sb	$0,116($17)

$L35:
	addiu	$19,$19,%lo(kmalloc_caches)
	lw	$4,28($19)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	li	$18,536870912			# 0x20000000

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,72($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,76($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,80($17)

	lw	$4,28($19)
	or	$2,$2,$18
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	sw	$2,84($17)

	or	$2,$2,$18
	sw	$2,88($17)
	j	$L28
	sb	$0,92($17)

	.set	macro
	.set	reorder
	.end	lookup_client_by_ip_and_port
	.size	lookup_client_by_ip_and_port, .-lookup_client_by_ip_and_port
	.align	2
	.globl	check_recv_pkt
	.set	nomips16
	.ent	check_recv_pkt
	.type	check_recv_pkt, @function
check_recv_pkt:
	.frame	$sp,40,$31		# vars= 16, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$16,32($sp)
	move	$16,$4
	sw	$5,16($sp)
	sw	$6,20($sp)
	jal	rtl_vpn_get_skb_len
	sw	$7,24($sp)

	sltu	$2,$2,51
	beq	$2,$0,$L62
	nop

$L39:
	move	$2,$0
$L42:
	lw	$31,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,40

$L62:
	jal	rtl_vpn_get_skb_data
	move	$4,$16

	lw	$7,24($sp)
	li	$3,1			# 0x1
	lw	$5,16($sp)
	beq	$7,$3,$L63
	lw	$6,20($sp)

	li	$3,2			# 0x2
	bne	$7,$3,$L39
	lui	$3,%hi(vpn_shared_info+34)

	lhu	$3,%lo(vpn_shared_info+34)($3)
	lhu	$4,44($2)
	bne	$4,$3,$L39
	li	$7,6			# 0x6

	lbu	$4,50($2)
	srl	$3,$4,3
	beq	$3,$7,$L64
	andi	$4,$4,0x7

	addiu	$4,$3,-1
	andi	$4,$4,0x00ff
	sltu	$4,$4,8
	beq	$4,$0,$L39
	li	$4,7			# 0x7

	beq	$3,$4,$L65
	li	$4,3			# 0x3

	bne	$3,$4,$L46
	move	$3,$0

	lhu	$7,42($2)
	lw	$5,34($2)
	lui	$2,%hi(vpn_client_info+52)
	addiu	$2,$2,%lo(vpn_client_info+52)
	j	$L54
	li	$6,16			# 0x10

$L53:
	addiu	$3,$3,1
$L70:
	beq	$3,$6,$L46
	addiu	$2,$2,208

$L54:
	lw	$4,0($2)
	bne	$4,$5,$L53
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L70
	addiu	$3,$3,1

	addiu	$3,$3,-1
	sll	$2,$3,2
$L68:
	sll	$4,$3,4
	subu	$2,$4,$2
	addu	$3,$2,$3
	lui	$2,%hi(vpn_client_info)
	sll	$3,$3,4
	addiu	$2,$2,%lo(vpn_client_info)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sb	$3,123($2)
	j	$L42
	li	$2,2			# 0x2

$L63:
	lui	$3,%hi(vpn_shared_info+34)
	lhu	$3,%lo(vpn_shared_info+34)($3)
	lhu	$4,36($2)
	bne	$4,$3,$L39
	li	$7,6			# 0x6

	lbu	$4,42($2)
	srl	$3,$4,3
	beq	$3,$7,$L66
	andi	$4,$4,0x7

	addiu	$4,$3,-1
	andi	$4,$4,0x00ff
	sltu	$4,$4,8
	beq	$4,$0,$L39
	li	$4,7			# 0x7

	beq	$3,$4,$L67
	li	$4,3			# 0x3

	bne	$3,$4,$L46
	move	$3,$0

	lhu	$7,34($2)
	lw	$6,26($2)
	lui	$2,%hi(vpn_client_info+52)
	addiu	$2,$2,%lo(vpn_client_info+52)
	j	$L48
	li	$5,16			# 0x10

$L47:
	addiu	$3,$3,1
$L71:
	beq	$3,$5,$L46
	addiu	$2,$2,208

$L48:
	lw	$4,0($2)
	bne	$4,$6,$L47
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L71
	addiu	$3,$3,1

	addiu	$3,$3,-1
	j	$L68
	sll	$2,$3,2

$L64:
	li	$2,51			# 0x33
	sw	$2,0($5)
	sw	$4,0($6)
	j	$L42
	li	$2,1			# 0x1

$L46:
	j	$L42
	li	$2,2			# 0x2

$L65:
	lhu	$7,42($2)
	lw	$5,34($2)
	lui	$2,%hi(vpn_client_info+52)
	addiu	$2,$2,%lo(vpn_client_info+52)
	move	$3,$0
	j	$L52
	li	$6,16			# 0x10

$L51:
	addiu	$3,$3,1
$L72:
	beq	$3,$6,$L46
	addiu	$2,$2,208

$L52:
	lw	$4,0($2)
	bne	$4,$5,$L51
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L72
	addiu	$3,$3,1

	addiu	$3,$3,-1
	sll	$2,$3,2
$L69:
	sll	$4,$3,4
	subu	$2,$4,$2
	addu	$3,$2,$3
	lui	$2,%hi(vpn_client_info)
	sll	$3,$3,4
	addiu	$2,$2,%lo(vpn_client_info)
	addu	$2,$3,$2
	sb	$0,122($2)
	sw	$0,64($2)
	j	$L42
	li	$2,2			# 0x2

$L66:
	li	$2,43			# 0x2b
	sw	$2,0($5)
	sw	$4,0($6)
	j	$L42
	li	$2,1			# 0x1

$L67:
	lhu	$7,34($2)
	lw	$5,26($2)
	lui	$2,%hi(vpn_client_info+52)
	addiu	$2,$2,%lo(vpn_client_info+52)
	move	$3,$0
	j	$L45
	li	$6,16			# 0x10

$L44:
	addiu	$3,$3,1
$L73:
	beq	$3,$6,$L46
	addiu	$2,$2,208

$L45:
	lw	$4,0($2)
	bne	$4,$5,$L44
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L73
	addiu	$3,$3,1

	addiu	$3,$3,-1
	j	$L69
	sll	$2,$3,2

	.set	macro
	.set	reorder
	.end	check_recv_pkt
	.size	check_recv_pkt, .-check_recv_pkt
	.section	.rodata.str1.4
	.align	2
$LC7:
	.ascii	"\012%s vpn_shared_info.out_dev==NULL!!!\012\000"
	.text
	.align	2
	.globl	is_openvpn_fragment
	.set	nomips16
	.ent	is_openvpn_fragment
	.type	is_openvpn_fragment, @function
is_openvpn_fragment:
	.frame	$sp,32,$31		# vars= 0, regs= 3/0, args= 16, gp= 0
	.mask	0x80030000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$17,24($sp)
	sw	$16,20($sp)
	lui	$17,%hi(vpn_shared_info)
	addiu	$17,$17,%lo(vpn_shared_info)
	lw	$16,8($17)
	beq	$16,$0,$L79
	lui	$5,%hi(__func__.47810)

	jal	rtl_vpn_get_skb_len
	nop

	lbu	$5,49($17)
	lw	$4,260($16)
	move	$3,$2
	addiu	$6,$5,50
	addu	$6,$3,$6
	sltu	$6,$4,$6
	bne	$6,$0,$L76
	li	$2,1			# 0x1

	addiu	$7,$3,5
	sra	$6,$7,31
	srl	$6,$6,28
	addu	$2,$7,$6
	andi	$2,$2,0xf
	addiu	$5,$5,66
	subu	$6,$2,$6
	addu	$2,$5,$3
	subu	$2,$2,$6
	sltu	$2,$4,$2
$L76:
	lw	$31,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,32

$L79:
	lui	$4,%hi($LC7)
	addiu	$4,$4,%lo($LC7)
	jal	printk
	addiu	$5,$5,%lo(__func__.47810)

	j	$L76
	move	$2,$0

	.set	macro
	.set	reorder
	.end	is_openvpn_fragment
	.size	is_openvpn_fragment, .-is_openvpn_fragment
	.section	.rodata.str1.4
	.align	2
$LC8:
	.ascii	"%s: skb_realloc_headroom failed!\012\000"
	.text
	.align	2
	.globl	openvpn_fast_to_wan
	.set	nomips16
	.ent	openvpn_fast_to_wan
	.type	openvpn_fast_to_wan, @function
openvpn_fast_to_wan:
	.frame	$sp,64,$31		# vars= 8, regs= 9/0, args= 16, gp= 0
	.mask	0x80ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
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
	lui	$18,%hi(vpn_shared_info)
	lw	$2,%lo(vpn_shared_info)($18)
	beq	$2,$0,$L81
	move	$16,$4

	jal	rtl_vpn_skb_linearize
	addiu	$17,$18,%lo(vpn_shared_info)

	jal	rtl_vpn_get_skb_data
	move	$4,$16

	lw	$19,4($17)
	beq	$19,$0,$L97
	move	$20,$2

$L82:
	lw	$6,16($20)
	lui	$2,%hi(vpn_client_info+122)
	addiu	$2,$2,%lo(vpn_client_info+122)
	move	$3,$0
	li	$5,16			# 0x10
$L85:
	lbu	$4,0($2)
	beq	$4,$0,$L83
	nop

	lw	$4,-66($2)
	beq	$4,$6,$L98
	sll	$4,$3,4

$L83:
	addiu	$3,$3,1
	bne	$3,$5,$L85
	addiu	$2,$2,208

$L81:
	move	$2,$0
$L91:
	lw	$31,60($sp)
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,64

$L98:
	sll	$2,$3,2
	subu	$2,$4,$2
	addu	$3,$2,$3
	lui	$23,%hi(vpn_client_info)
	sll	$3,$3,4
	addiu	$23,$23,%lo(vpn_client_info)
	addu	$23,$23,$3
	lui	$5,%hi(EMPTY_MAC_ADDR)
	addiu	$5,$5,%lo(EMPTY_MAC_ADDR)
	move	$4,$23
	jal	memcmp
	li	$6,6			# 0x6

	beq	$2,$0,$L91
	move	$2,$0

	jal	rtl_vpn_get_skb_len
	move	$4,$16

	addiu	$3,$2,5
	move	$22,$2
	sra	$2,$3,31
	srl	$2,$2,28
	addu	$3,$3,$2
	andi	$3,$3,0xf
	subu	$2,$2,$3
	addiu	$21,$2,16
	move	$4,$16
	jal	rtl_vpn_skb_put
	move	$5,$21

	beq	$21,$0,$L86
	andi	$4,$21,0x00ff

	addu	$20,$20,$22
	move	$2,$0
$L87:
	addiu	$2,$2,1
	sltu	$3,$2,$21
	sb	$4,0($20)
	bne	$3,$0,$L87
	addiu	$20,$20,1

$L86:
	lbu	$21,32($17)
	lbu	$2,49($17)
	move	$4,$16
	addu	$21,$21,$2
	jal	rtl_vpn_skb_headroom
	addiu	$21,$21,21

	sltu	$2,$2,$21
	bne	$2,$0,$L102
	move	$4,$16

	jal	rtl_vpn_skb_cloned
	move	$4,$16

	beq	$2,$0,$L99
	move	$4,$16

$L102:
	jal	skb_realloc_headroom
	move	$5,$21

	beq	$2,$0,$L100
	move	$4,$16

	jal	consume_skb
	sw	$2,16($sp)

	lw	$2,16($sp)
	move	$16,$2
	move	$5,$21
$L103:
	jal	rtl_vpn_skb_push
	move	$4,$16

	jal	rtl_vpn_get_skb_data
	move	$4,$16

	lbu	$6,32($17)
	move	$20,$2
	move	$5,$23
	jal	memcpy
	move	$4,$2

	addu	$21,$20,$21
	li	$2,-6			# 0xfffffffffffffffa
	sb	$2,-1($21)
	sw	$0,-5($21)
	lbu	$2,32($17)
	move	$4,$16
	addu	$2,$20,$2
	sb	$0,-1($2)
	jal	rtl_vpn_get_skb_len
	lbu	$21,32($17)

	lbu	$3,32($17)
	addiu	$21,$21,-5
	addu	$21,$20,$21
	subu	$3,$2,$3
	addiu	$3,$3,9
	sh	$3,0($21)
	move	$4,$16
	jal	rtl_vpn_get_skb_len
	lbu	$21,32($17)

	lbu	$3,32($17)
	addiu	$21,$21,-27
	addu	$21,$20,$21
	subu	$3,$2,$3
	addiu	$3,$3,29
	sh	$3,0($21)
	lbu	$3,32($17)
	li	$2,51			# 0x33
	beq	$3,$2,$L101
	nop

$L92:
	lw	$3,140($19)
	lw	$2,132($19)
	move	$4,$16
	addu	$22,$3,$22
	addiu	$2,$2,1
	sw	$22,140($19)
	sw	$2,132($19)
	jal	rtl_vpn_set_skb_dev
	lw	$5,%lo(vpn_shared_info)($18)

	jal	rtl_vpn_call_skb_ndo_start_xmit
	move	$4,$16

	lw	$31,60($sp)
	lw	$23,56($sp)
	lw	$22,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	li	$2,1			# 0x1
	j	$31
	addiu	$sp,$sp,64

$L97:
	jal	rtl_vpn_get_skb_dev
	move	$4,$16

	sw	$2,4($17)
	j	$L82
	move	$19,$2

$L99:
	jal	rtl_vpn_skb_shared
	move	$4,$16

	beq	$2,$0,$L103
	move	$5,$21

	j	$L102
	move	$4,$16

$L101:
	jal	rtl_vpn_get_skb_len
	move	$4,$16

	addiu	$2,$2,-20
	j	$L92
	sh	$2,18($20)

$L100:
	lui	$4,%hi($LC8)
	lui	$5,%hi(__func__.48775)
	addiu	$4,$4,%lo($LC8)
	jal	printk
	addiu	$5,$5,%lo(__func__.48775)

	j	$L91
	move	$2,$0

	.set	macro
	.set	reorder
	.end	openvpn_fast_to_wan
	.size	openvpn_fast_to_wan, .-openvpn_fast_to_wan
	.section	.rodata.str1.4
	.align	2
$LC9:
	.ascii	"%s client_idx=%d\012\012\000"
	.text
	.align	2
	.globl	openvpn_fragment_fast_to_wan
	.set	nomips16
	.ent	openvpn_fragment_fast_to_wan
	.type	openvpn_fragment_fast_to_wan, @function
openvpn_fragment_fast_to_wan:
	.frame	$sp,56,$31		# vars= 8, regs= 7/0, args= 16, gp= 0
	.mask	0x803f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$21,48($sp)
	sw	$20,44($sp)
	sw	$19,40($sp)
	sw	$18,36($sp)
	sw	$17,32($sp)
	sw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	move	$16,$4
	.set	macro
	.set	reorder

	lw	$5,16($2)
	lui	$2,%hi(vpn_client_info+122)
	addiu	$2,$2,%lo(vpn_client_info+122)
	move	$20,$0
	li	$4,16			# 0x10
$L107:
	lbu	$3,0($2)
	beq	$3,$0,$L105
	lw	$3,-70($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$5,$L106
	lui	$18,%hi(vpn_shared_info)
	.set	macro
	.set	reorder

$L105:
	addiu	$20,$20,1
	.set	noreorder
	.set	nomacro
	bne	$20,$4,$L107
	addiu	$2,$2,208
	.set	macro
	.set	reorder

	lui	$4,%hi($LC9)
	lui	$5,%hi(__func__.48237)
	addiu	$4,$4,%lo($LC9)
	addiu	$5,$5,%lo(__func__.48237)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
$L108:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L106:
	addiu	$19,$18,%lo(vpn_shared_info)
	lbu	$3,32($19)
	li	$2,14			# 0xe
	li	$21,22			# 0x16
	xori	$3,$3,0x33
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_headroom
	movn	$21,$2,$3
	.set	macro
	.set	reorder

	sltu	$2,$2,$21
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L117
	move	$4,$16
	.set	macro
	.set	reorder

$L120:
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	move	$5,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L118
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	consume_skb
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,16($sp)
	move	$16,$2
	move	$5,$21
$L121:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_push
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	move	$17,$2
	sll	$3,$20,4
	sll	$2,$20,2
	subu	$2,$3,$2
	addu	$20,$2,$20
	lui	$5,%hi(vpn_client_info)
	sll	$20,$20,4
	addiu	$5,$5,%lo(vpn_client_info)
	addu	$5,$5,$20
	move	$6,$21
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$17
	.set	macro
	.set	reorder

	lbu	$3,32($19)
	li	$2,51			# 0x33
	beq	$3,$2,$L119
$L114:
	lw	$5,%lo(vpn_shared_info)($18)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_dev
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_call_skb_ndo_start_xmit
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$31,52($sp)
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
	addiu	$sp,$sp,56
	.set	macro
	.set	reorder

$L117:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_cloned
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L120
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_shared
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L121
	move	$5,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L120
	move	$4,$16
	.set	macro
	.set	reorder

$L119:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	addiu	$2,$2,-20
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	.set	noreorder
	.set	nomacro
	j	$L114
	sh	$2,18($17)
	.set	macro
	.set	reorder

$L118:
	lui	$4,%hi($LC8)
	lui	$5,%hi(__func__.48237)
	addiu	$4,$4,%lo($LC8)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.48237)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L108
	move	$2,$0
	.set	macro
	.set	reorder

	.end	openvpn_fragment_fast_to_wan
	.size	openvpn_fragment_fast_to_wan, .-openvpn_fragment_fast_to_wan
	.section	.rodata.str1.4
	.align	2
$LC10:
	.ascii	"%s:%d rtl_ipsecEngine failed!!! re-init crypto engine!!!"
	.ascii	"\012\000"
	.text
	.align	2
	.globl	rtk_openvpn_fragment_hw_encrypto
	.set	nomips16
	.ent	rtk_openvpn_fragment_hw_encrypto
	.type	rtk_openvpn_fragment_hw_encrypto, @function
rtk_openvpn_fragment_hw_encrypto:
	.frame	$sp,136,$31		# vars= 40, regs= 10/0, args= 56, gp= 0
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
	lui	$17,%hi(vpn_shared_info)
	lw	$3,%lo(vpn_shared_info)($17)
	move	$19,$4
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L124
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	addiu	$17,$17,%lo(vpn_shared_info)
	lw	$18,4($17)
	beq	$18,$0,$L146
$L125:
	lw	$4,0($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_linearize
	move	$16,$0
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	lw	$4,0($19)
	.set	macro
	.set	reorder

	move	$21,$2
	lw	$5,16($2)
	lui	$2,%hi(vpn_client_info+122)
	addiu	$2,$2,%lo(vpn_client_info+122)
	li	$4,16			# 0x10
$L128:
	lbu	$3,0($2)
	beq	$3,$0,$L126
	lw	$3,-66($2)
	.set	noreorder
	.set	nomacro
	beq	$3,$5,$L127
	sll	$22,$16,2
	.set	macro
	.set	reorder

$L126:
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	bne	$16,$4,$L128
	addiu	$2,$2,208
	.set	macro
	.set	reorder

$L145:
	li	$2,-1			# 0xffffffffffffffff
$L124:
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

$L127:
	sll	$23,$16,4
	subu	$fp,$23,$22
	addu	$fp,$fp,$16
	lui	$20,%hi(vpn_client_info)
	sll	$fp,$fp,4
	addiu	$20,$20,%lo(vpn_client_info)
	addu	$2,$20,$fp
	lui	$5,%hi(EMPTY_MAC_ADDR)
	addiu	$5,$5,%lo(EMPTY_MAC_ADDR)
	move	$4,$2
	li	$6,6			# 0x6
	.set	noreorder
	.set	nomacro
	jal	memcmp
	sw	$2,64($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L145
	lw	$3,64($sp)
	.set	macro
	.set	reorder

	lw	$4,0($19)
	lw	$2,68($3)
	sll	$5,$2,5
	sll	$2,$2,3
	sw	$2,72($sp)
	subu	$2,$5,$2
	addu	$fp,$2,$fp
	sw	$5,76($sp)
	addu	$fp,$fp,$20
	lbu	$fp,92($fp)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	sw	$fp,80($sp)
	.set	macro
	.set	reorder

	addiu	$4,$2,5
	move	$3,$2
	sra	$2,$4,31
	srl	$2,$2,28
	addu	$fp,$4,$2
	andi	$fp,$fp,0xf
	subu	$fp,$2,$fp
	lw	$4,0($19)
	addiu	$fp,$fp,16
	move	$5,$fp
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_put
	sw	$3,88($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$fp,$0,$L130
	lw	$3,88($sp)
	.set	macro
	.set	reorder

	addu	$21,$21,$3
	andi	$5,$fp,0x00ff
	move	$2,$0
$L131:
	addiu	$2,$2,1
	sltu	$4,$2,$fp
	sb	$5,0($21)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L131
	addiu	$21,$21,1
	.set	macro
	.set	reorder

$L130:
	lw	$4,132($18)
	lw	$5,140($18)
	lbu	$2,32($17)
	addiu	$4,$4,1
	addu	$3,$5,$3
	sw	$3,140($18)
	sw	$4,132($18)
	lbu	$21,49($17)
	lw	$4,0($19)
	xori	$2,$2,0x33
	li	$3,8			# 0x8
	movn	$3,$0,$2
	sw	$3,68($sp)
	addiu	$21,$21,50
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_headroom
	lw	$fp,8($17)
	.set	macro
	.set	reorder

	sltu	$2,$2,$21
	beq	$2,$0,$L147
$L134:
	lw	$4,0($19)
	.set	noreorder
	.set	nomacro
	jal	skb_realloc_headroom
	move	$5,$21
	.set	macro
	.set	reorder

	beq	$2,$0,$L148
	lw	$4,0($19)
	.set	noreorder
	.set	nomacro
	jal	consume_skb
	sw	$2,84($sp)
	.set	macro
	.set	reorder

	lw	$2,84($sp)
	sw	$2,0($19)
$L135:
	move	$5,$21
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_push
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	lw	$4,0($19)
	.set	macro
	.set	reorder

	lw	$4,68($sp)
	lw	$6,64($sp)
	move	$18,$2
	addiu	$5,$4,14
	addu	$5,$6,$5
	move	$4,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,29			# 0x1d
	.set	macro
	.set	reorder

	lw	$4,80($sp)
	subu	$3,$23,$22
	addu	$3,$3,$16
	sll	$3,$3,4
	addu	$7,$20,$3
	li	$5,-6			# 0xfffffffffffffffa
	addu	$21,$18,$21
	ori	$2,$4,0x30
	lw	$4,64($7)
	sb	$5,-1($21)
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$4, $4			
	rotr	$4, $4, 16		

 # 0 "" 2
 #NO_APP
	sw	$4,-5($21)
	sb	$2,28($18)
	lw	$4,0($19)
	sw	$7,84($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	sw	$3,88($sp)
	.set	macro
	.set	reorder

	addiu	$2,$2,-20
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,24($18)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	lw	$4,0($19)
	.set	macro
	.set	reorder

 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,2($18)
	lw	$4,44($17)
	lui	$2,%hi(jiffies)
	lw	$3,88($sp)
	sll	$6,$4,4
	lw	$5,%lo(jiffies)($2)
	sll	$2,$4,2
	addu	$2,$2,$6
	sll	$4,$2,2
	addu	$2,$2,$4
	addu	$4,$20,$3
	addu	$5,$2,$5
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	addiu	$4,$4,180
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	lw	$4,0($19)
	.set	macro
	.set	reorder

	lw	$5,76($sp)
	lw	$6,72($sp)
	lw	$3,88($sp)
	subu	$4,$5,$6
	addu	$3,$4,$3
	addu	$3,$20,$3
	lbu	$4,49($17)
	lw	$6,12($17)
	lw	$5,88($3)
	subu	$2,$2,$4
	addu	$4,$6,$4
	li	$6,16			# 0x10
	sw	$3,88($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$21,$2,-45
	.set	macro
	.set	reorder

	lbu	$2,49($17)
	lw	$4,12($17)
	move	$6,$21
	addiu	$5,$2,45
	addiu	$2,$2,16
	addu	$4,$4,$2
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$5,$18,$5
	.set	macro
	.set	reorder

	lbu	$4,48($17)
	li	$2,1			# 0x1
	lw	$3,88($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L149
	lw	$7,84($sp)
	.set	macro
	.set	reorder

	lw	$2,12($17)
	lbu	$13,49($17)
	li	$4,536805376			# 0x1fff0000
	lbu	$9,121($7)
	lw	$8,88($3)
	lbu	$11,120($7)
	lw	$10,72($3)
	lw	$12,24($17)
	lw	$3,80($3)
	addu	$13,$2,$13
	ori	$4,$4,0xffff
	and	$13,$13,$4
	li	$4,-1610612736			# 0xffffffffa0000000
	or	$13,$13,$4
	addiu	$14,$21,16
	sw	$2,44($sp)
	li	$4,36			# 0x24
	li	$2,16			# 0x10
	li	$5,3			# 0x3
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$14,56($sp)
	sw	$13,60($sp)
	sw	$12,16($sp)
	sw	$11,20($sp)
	sw	$10,24($sp)
	sw	$9,28($sp)
	sw	$3,32($sp)
	sw	$8,36($sp)
	sw	$2,48($sp)
	sw	$0,40($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$21,52($sp)
	.set	macro
	.set	reorder

$L138:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L150
	subu	$22,$23,$22
	.set	macro
	.set	reorder

	addu	$16,$22,$16
	sll	$16,$16,4
	addu	$20,$20,$16
	lbu	$6,49($17)
	lw	$2,64($20)
	lw	$5,12($17)
	addiu	$6,$6,16
	addiu	$2,$2,1
	addu	$6,$6,$21
	addiu	$4,$18,29
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,64($20)
	.set	macro
	.set	reorder

	sh	$0,6($18)
	sh	$0,10($18)
	lw	$2,4($18)
	lw	$4,0($18)
	lw	$5,8($18)
	lw	$3,12($18)
	addu	$4,$2,$4
	sltu	$2,$4,$2
	addu	$4,$4,$5
	addu	$2,$4,$2
	lw	$4,16($18)
	sltu	$5,$2,$5
	addu	$2,$2,$3
	addu	$5,$2,$5
	sltu	$3,$5,$3
	addu	$5,$5,$4
	addu	$3,$5,$3
	sltu	$2,$3,$4
	sh	$0,26($18)
	addu	$2,$2,$3
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
	lw	$2,0($19)
	lw	$3,204($2)
	sw	$3,184($2)
	lbu	$3,32($17)
	li	$2,51			# 0x33
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L151
	lui	$2,%hi(vpn_shared_info)
	.set	macro
	.set	reorder

	lw	$4,0($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_dev
	lw	$5,%lo(vpn_shared_info)($2)
	.set	macro
	.set	reorder

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
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

$L147:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_cloned
	lw	$4,0($19)
	.set	macro
	.set	reorder

	bne	$2,$0,$L134
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_shared
	lw	$4,0($19)
	.set	macro
	.set	reorder

	bne	$2,$0,$L134
	.set	noreorder
	.set	nomacro
	j	$L135
	lw	$2,0($19)
	.set	macro
	.set	reorder

$L149:
	lbu	$2,49($17)
	lw	$10,12($17)
	li	$4,536805376			# 0x1fff0000
	addiu	$2,$2,16
	lbu	$8,120($7)
	lw	$9,24($17)
	addu	$10,$10,$2
	ori	$4,$4,0xffff
	lw	$2,88($3)
	lw	$3,72($3)
	and	$10,$10,$4
	li	$4,-1610612736			# 0xffffffffa0000000
	or	$10,$10,$4
	li	$5,-1			# 0xffffffffffffffff
	li	$4,36			# 0x24
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$10,60($sp)
	sw	$9,16($sp)
	sw	$8,20($sp)
	sw	$3,24($sp)
	sw	$2,36($sp)
	sw	$21,56($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$21,52($sp)
	.set	macro
	.set	reorder

	j	$L138
$L151:
	lw	$4,0($19)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_dev
	move	$5,$fp
	.set	macro
	.set	reorder

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
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

$L146:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_dev
	lw	$4,0($4)
	.set	macro
	.set	reorder

	sw	$2,4($17)
	.set	noreorder
	.set	nomacro
	j	$L125
	move	$18,$2
	.set	macro
	.set	reorder

$L148:
	lui	$4,%hi($LC8)
	lui	$5,%hi(__func__.48308)
	addiu	$4,$4,%lo($LC8)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.48308)
	.set	macro
	.set	reorder

	j	$L145
$L150:
	lui	$4,%hi($LC10)
	lui	$5,%hi(__func__.48308)
	addiu	$4,$4,%lo($LC10)
	addiu	$5,$5,%lo(__func__.48308)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,1673			# 0x689
	.set	macro
	.set	reorder

	li	$4,5			# 0x5
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine_init
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L124
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	rtk_openvpn_fragment_hw_encrypto
	.size	rtk_openvpn_fragment_hw_encrypto, .-rtk_openvpn_fragment_hw_encrypto
	.section	.rodata.str1.4
	.align	2
$LC11:
	.ascii	"%s##Invalid Client Index!\012\000"
	.align	2
$LC12:
	.ascii	"ppp0\000"
	.text
	.align	2
	.globl	check_send_pkt
	.set	nomips16
	.ent	check_send_pkt
	.type	check_send_pkt, @function
check_send_pkt:
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
	move	$16,$5
	jal	rtl_vpn_get_skb_len
	move	$19,$4

	sltu	$2,$2,51
	beq	$2,$0,$L171
	nop

$L153:
	move	$2,$0
$L158:
	lw	$31,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,40

$L171:
	jal	rtl_vpn_get_skb_data
	move	$4,$19

	li	$17,1			# 0x1
	beq	$16,$17,$L172
	move	$18,$2

	li	$2,2			# 0x2
	bne	$16,$2,$L158
	move	$2,$0

	lui	$20,%hi(vpn_shared_info)
	addiu	$16,$20,%lo(vpn_shared_info)
	lhu	$3,42($18)
	lhu	$2,34($16)
	bne	$3,$2,$L158
	move	$2,$0

	lbu	$2,50($18)
	srl	$2,$2,3
	beq	$2,$0,$L155
	li	$3,6			# 0x6

	beq	$2,$3,$L153
	addiu	$3,$2,-1

	andi	$3,$3,0x00ff
	sltu	$3,$3,8
	beq	$3,$0,$L153
	li	$3,8			# 0x8

	beq	$2,$3,$L173
	li	$3,3			# 0x3

	beq	$2,$3,$L174
	move	$3,$0

$L159:
	j	$L158
	li	$2,2			# 0x2

$L172:
	lui	$20,%hi(vpn_shared_info)
	addiu	$17,$20,%lo(vpn_shared_info)
	lhu	$3,34($2)
	lhu	$2,34($17)
	bne	$3,$2,$L158
	move	$2,$0

	lbu	$2,42($18)
	srl	$2,$2,3
	beq	$2,$0,$L155
	li	$3,6			# 0x6

	beq	$2,$3,$L153
	addiu	$3,$2,-1

	andi	$3,$3,0x00ff
	sltu	$3,$3,8
	beq	$3,$0,$L153
	li	$3,8			# 0x8

	beq	$2,$3,$L175
	li	$3,3			# 0x3

	bne	$2,$3,$L159
	move	$3,$0

	lui	$2,%hi(vpn_client_info+52)
	lhu	$7,36($18)
	lw	$5,30($18)
	addiu	$2,$2,%lo(vpn_client_info+52)
	j	$L161
	li	$6,16			# 0x10

$L160:
	addiu	$3,$3,1
$L177:
	beq	$3,$6,$L159
	addiu	$2,$2,208

$L161:
	lw	$4,0($2)
	bne	$4,$5,$L160
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L177
	addiu	$3,$3,1

	addiu	$3,$3,-1
	j	$L176
	sll	$2,$3,2

$L155:
	j	$L158
	li	$2,1			# 0x1

$L174:
	lui	$2,%hi(vpn_client_info+52)
	lhu	$7,44($18)
	lw	$6,38($18)
	addiu	$2,$2,%lo(vpn_client_info+52)
	j	$L165
	li	$5,16			# 0x10

$L164:
	addiu	$3,$3,1
$L178:
	beq	$3,$5,$L159
	addiu	$2,$2,208

$L165:
	lw	$4,0($2)
	bne	$4,$6,$L164
	nop

	lhu	$4,8($2)
	bne	$4,$7,$L178
	addiu	$3,$3,1

	addiu	$3,$3,-1
	sll	$2,$3,2
$L176:
	sll	$4,$3,4
	subu	$2,$4,$2
	addu	$3,$2,$3
	lui	$2,%hi(vpn_client_info)
	sll	$3,$3,4
	addiu	$2,$2,%lo(vpn_client_info)
	addu	$2,$3,$2
	li	$3,1			# 0x1
	sb	$3,123($2)
	j	$L158
	li	$2,2			# 0x2

$L173:
	li	$2,51			# 0x33
	move	$4,$19
	jal	rtl_vpn_get_skb_dev
	sb	$2,32($16)

	lui	$4,%hi($LC12)
	addiu	$4,$4,%lo($LC12)
	jal	rtl_vpn_get_dev_by_name
	sw	$2,%lo(vpn_shared_info)($20)

	sw	$2,8($16)
	lw	$4,38($18)
	lhu	$5,44($18)
	jal	lookup_client_by_ip_and_port
	li	$6,1			# 0x1

	sltu	$3,$2,16
	beq	$3,$0,$L169
	sll	$4,$2,4

	sll	$3,$2,2
	subu	$3,$4,$3
	addu	$2,$3,$2
	lui	$16,%hi(vpn_client_info)
	sll	$2,$2,4
	addiu	$16,$16,%lo(vpn_client_info)
	addu	$16,$16,$2
	li	$6,51			# 0x33
	move	$4,$16
	jal	memset
	move	$5,$0

	move	$4,$16
	move	$5,$18
	jal	memcpy
	li	$6,51			# 0x33

	sw	$17,64($16)
	sw	$0,68($16)
	sb	$0,92($16)
	sb	$0,116($16)
	sb	$0,122($16)
	sb	$0,123($16)
	j	$L158
	li	$2,2			# 0x2

$L175:
	li	$2,43			# 0x2b
	move	$4,$19
	jal	rtl_vpn_get_skb_dev
	sb	$2,32($17)

	sw	$2,8($17)
	sw	$2,%lo(vpn_shared_info)($20)
	lw	$4,30($18)
	lhu	$5,36($18)
	jal	lookup_client_by_ip_and_port
	li	$6,1			# 0x1

	sltu	$3,$2,16
	beq	$3,$0,$L169
	sll	$4,$2,4

	sll	$3,$2,2
	subu	$3,$4,$3
	addu	$2,$3,$2
	lui	$17,%hi(vpn_client_info)
	sll	$2,$2,4
	addiu	$17,$17,%lo(vpn_client_info)
	addu	$17,$17,$2
	li	$6,43			# 0x2b
	move	$4,$17
	jal	memset
	move	$5,$0

	move	$4,$17
	move	$5,$18
	jal	memcpy
	li	$6,43			# 0x2b

	sw	$16,64($17)
	sw	$0,68($17)
	sb	$0,92($17)
	sb	$0,116($17)
	sb	$0,122($17)
	sb	$0,123($17)
	j	$L158
	li	$2,2			# 0x2

$L169:
	lui	$4,%hi($LC11)
	lui	$5,%hi(__func__.47845)
	addiu	$4,$4,%lo($LC11)
	jal	printk
	addiu	$5,$5,%lo(__func__.47845)

	j	$L158
	move	$2,$0

	.set	macro
	.set	reorder
	.end	check_send_pkt
	.size	check_send_pkt, .-check_send_pkt
	.align	2
	.globl	rtk_openvpn_hw_encrypto
	.set	nomips16
	.ent	rtk_openvpn_hw_encrypto
	.type	rtk_openvpn_hw_encrypto, @function
rtk_openvpn_hw_encrypto:
	.frame	$sp,120,$31		# vars= 24, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-120
	sw	$31,116($sp)
	sw	$fp,112($sp)
	sw	$23,108($sp)
	sw	$22,104($sp)
	sw	$21,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
	move	$20,$4
	jal	check_send_pkt
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L180
	lui	$17,%hi(vpn_shared_info)
	.set	macro
	.set	reorder

	move	$2,$0
$L181:
	lw	$31,116($sp)
	lw	$fp,112($sp)
	lw	$23,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,120
	.set	macro
	.set	reorder

$L180:
	move	$4,$20
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	addiu	$17,$17,%lo(vpn_shared_info)
	.set	macro
	.set	reorder

	lbu	$3,32($17)
	move	$18,$2
	lui	$2,%hi(vpn_client_info+122)
	addu	$3,$18,$3
	addiu	$2,$2,%lo(vpn_client_info+122)
	lhu	$6,-7($3)
	lw	$5,-13($3)
	move	$16,$0
	.set	noreorder
	.set	nomacro
	j	$L184
	li	$4,16			# 0x10
	.set	macro
	.set	reorder

$L182:
	addiu	$16,$16,1
$L200:
	.set	noreorder
	.set	nomacro
	beq	$16,$4,$L196
	addiu	$2,$2,208
	.set	macro
	.set	reorder

$L184:
	lbu	$3,0($2)
	beq	$3,$0,$L182
	lw	$3,-70($2)
	bne	$3,$5,$L182
	lhu	$3,-62($2)
	.set	noreorder
	.set	nomacro
	bne	$3,$6,$L200
	addiu	$16,$16,1
	.set	macro
	.set	reorder

	addiu	$16,$16,-1
	sll	$22,$16,2
	sll	$23,$16,4
	subu	$4,$23,$22
	addu	$4,$4,$16
	lui	$19,%hi(vpn_client_info)
	sll	$4,$4,4
	addiu	$19,$19,%lo(vpn_client_info)
	addu	$3,$19,$4
	lw	$5,44($17)
	lw	$7,68($3)
	sll	$6,$5,4
	sll	$2,$5,2
	lui	$5,%hi(jiffies)
	sll	$8,$7,5
	addu	$2,$2,$6
	sll	$7,$7,3
	lw	$5,%lo(jiffies)($5)
	subu	$21,$8,$7
	sll	$6,$2,2
	addu	$2,$2,$6
	addu	$21,$21,$4
	addu	$5,$2,$5
	addiu	$4,$3,180
	addu	$21,$19,$21
	lbu	$fp,92($21)
	sw	$3,64($sp)
	sw	$7,68($sp)
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	sw	$8,72($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$20
	.set	macro
	.set	reorder

	lbu	$4,49($17)
	lbu	$9,32($17)
	lw	$6,12($17)
	lw	$5,88($21)
	subu	$21,$0,$4
	subu	$21,$21,$9
	addu	$4,$6,$4
	addiu	$21,$21,-16
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$21,$21,$2
	.set	macro
	.set	reorder

	lbu	$2,49($17)
	lbu	$5,32($17)
	lw	$6,12($17)
	addiu	$4,$2,16
	addu	$5,$2,$5
	addiu	$5,$5,16
	addu	$4,$6,$4
	addu	$5,$18,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$6,$21
	.set	macro
	.set	reorder

	lbu	$13,49($17)
	lw	$2,12($17)
	lw	$3,64($sp)
	addiu	$6,$13,16
	addu	$6,$2,$6
	lw	$4,0($6)
	lw	$7,68($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L185
	lw	$8,72($sp)
	.set	macro
	.set	reorder

	lw	$5,64($3)
	bne	$5,$0,$L186
	li	$2,1			# 0x1
	sw	$2,64($3)
	li	$5,1			# 0x1
$L187:
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$5, $5			
	rotr	$5, $5, 16		

 # 0 "" 2
 #NO_APP
	sw	$5,0($6)
	lw	$2,12($17)
	lbu	$13,49($17)
$L189:
	lbu	$4,48($17)
	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$4,$3,$L197
	subu	$4,$23,$22
	.set	macro
	.set	reorder

	addu	$4,$4,$16
	subu	$7,$8,$7
	sll	$4,$4,4
	addu	$7,$7,$4
	addu	$4,$19,$4
	lbu	$9,121($4)
	lbu	$11,120($4)
	addu	$7,$19,$7
	li	$4,536805376			# 0x1fff0000
	lw	$3,88($7)
	lw	$10,72($7)
	lw	$8,80($7)
	lw	$12,24($17)
	addu	$13,$2,$13
	ori	$4,$4,0xffff
	and	$13,$13,$4
	li	$4,-1610612736			# 0xffffffffa0000000
	or	$13,$13,$4
	addiu	$14,$21,16
	sw	$2,44($sp)
	li	$4,36			# 0x24
	li	$2,16			# 0x10
	li	$5,3			# 0x3
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$14,56($sp)
	sw	$13,60($sp)
	sw	$12,16($sp)
	sw	$11,20($sp)
	sw	$10,24($sp)
	sw	$9,28($sp)
	sw	$8,32($sp)
	sw	$3,36($sp)
	sw	$2,48($sp)
	sw	$0,40($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$21,52($sp)
	.set	macro
	.set	reorder

$L191:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L198
	subu	$22,$23,$22
	.set	macro
	.set	reorder

	addu	$16,$22,$16
	sll	$16,$16,4
	addu	$19,$19,$16
	lw	$3,64($19)
	lbu	$2,32($17)
	ori	$fp,$fp,0x30
	addiu	$3,$3,1
	addu	$2,$18,$2
	sw	$3,64($19)
	sb	$fp,-1($2)
	lbu	$6,49($17)
	lbu	$4,32($17)
	lw	$5,12($17)
	addiu	$6,$6,16
	addu	$6,$6,$21
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$4,$18,$4
	.set	macro
	.set	reorder

	lbu	$2,32($17)
	lhu	$3,50($17)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	addu	$2,$18,$2
	sh	$3,-25($2)
	lhu	$2,50($17)
	addiu	$2,$2,1
	sh	$2,50($17)
	lbu	$2,32($17)
	addu	$2,$18,$2
	sh	$0,-23($2)
	lbu	$2,32($17)
	addiu	$2,$2,-5
	addu	$2,$18,$2
	lhu	$3,0($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	sh	$3,0($2)
	lbu	$2,32($17)
	addiu	$2,$2,-27
	addu	$2,$18,$2
	lhu	$3,0($2)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$3, $3			

 # 0 "" 2
 #NO_APP
	sh	$3,0($2)
	lbu	$2,32($17)
	li	$3,51			# 0x33
	beq	$2,$3,$L199
$L193:
	addu	$2,$18,$2
	sh	$0,-19($2)
	lbu	$5,32($17)
	move	$2,$0
	addiu	$3,$5,-29
	addu	$3,$18,$3
	lw	$8,4($3)
	lw	$6,0($3)
	lw	$7,8($3)
	lw	$4,12($3)
	addu	$6,$8,$6
	sltu	$8,$6,$8
	addu	$6,$6,$7
	addu	$8,$6,$8
	lw	$6,16($3)
	sltu	$7,$8,$7
	addu	$8,$8,$4
	addu	$7,$8,$7
	sltu	$4,$7,$4
	addu	$7,$7,$6
	addu	$4,$7,$4
	addu	$5,$18,$5
	sltu	$3,$4,$6
	addu	$3,$3,$4
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
	sh	$3,-19($5)
	lbu	$3,32($17)
	addu	$18,$18,$3
	sh	$0,-3($18)
	lw	$31,116($sp)
	lw	$fp,112($sp)
	lw	$23,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,120
	.set	macro
	.set	reorder

$L196:
	lui	$4,%hi($LC9)
	lui	$5,%hi(__func__.48036)
	addiu	$4,$4,%lo($LC9)
	addiu	$5,$5,%lo(__func__.48036)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$31,116($sp)
	lw	$fp,112($sp)
	lw	$23,108($sp)
	lw	$22,104($sp)
	lw	$21,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	li	$2,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,120
	.set	macro
	.set	reorder

$L185:
	lw	$5,64($3)
$L186:
	sltu	$3,$4,$5
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L187
	subu	$3,$23,$22
	.set	macro
	.set	reorder

	addu	$3,$3,$16
	sll	$3,$3,4
	addu	$3,$19,$3
	.set	noreorder
	.set	nomacro
	j	$L189
	sw	$4,64($3)
	.set	macro
	.set	reorder

$L197:
	subu	$3,$23,$22
	addu	$3,$3,$16
	subu	$4,$8,$7
	sll	$3,$3,4
	addu	$4,$4,$3
	addu	$4,$19,$4
	addu	$3,$19,$3
	lbu	$9,120($3)
	lw	$8,72($4)
	lw	$3,88($4)
	addiu	$13,$13,16
	li	$4,536805376			# 0x1fff0000
	lw	$10,24($17)
	addu	$2,$2,$13
	ori	$4,$4,0xffff
	and	$2,$2,$4
	li	$4,-1610612736			# 0xffffffffa0000000
	or	$2,$2,$4
	li	$5,-1			# 0xffffffffffffffff
	li	$4,36			# 0x24
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$2,60($sp)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,36($sp)
	sw	$21,56($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	sw	$0,48($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$21,52($sp)
	.set	macro
	.set	reorder

	j	$L191
$L199:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$20
	.set	macro
	.set	reorder

	addiu	$2,$2,-20
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	sh	$2,18($18)
	.set	noreorder
	.set	nomacro
	j	$L193
	lbu	$2,32($17)
	.set	macro
	.set	reorder

$L198:
	lui	$4,%hi($LC10)
	lui	$5,%hi(__func__.48036)
	addiu	$4,$4,%lo($LC10)
	addiu	$5,$5,%lo(__func__.48036)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,1112			# 0x458
	.set	macro
	.set	reorder

	li	$4,5			# 0x5
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine_init
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L181
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	rtk_openvpn_hw_encrypto
	.size	rtk_openvpn_hw_encrypto, .-rtk_openvpn_hw_encrypto
	.section	.rodata.str1.4
	.align	2
$LC13:
	.ascii	"\012%s key_info[KS_PRIMARY].key_id=%d  key_info[KS_LAME_"
	.ascii	"DUCK].key_id=%d  dec_key_id=%d\012\000"
	.align	2
$LC14:
	.ascii	"\012There is no match key to decrypto packet!\012\000"
	.align	2
$LC15:
	.ascii	"\012%s len is not times of CBC_BLOCK_SIZE!!! len=%d\012\000"
	.align	2
$LC16:
	.ascii	"%s:%d rtl_ipsecEngine failed\012\000"
	.align	2
$LC17:
	.ascii	"tun0\000"
	.text
	.align	2
	.globl	rtk_openvpn_fragment_hw_decrypto
	.set	nomips16
	.ent	rtk_openvpn_fragment_hw_decrypto
	.type	rtk_openvpn_fragment_hw_decrypto, @function
rtk_openvpn_fragment_hw_decrypto:
	.frame	$sp,112,$31		# vars= 16, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_linearize
	move	$16,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	move	$19,$2
	lw	$5,12($2)
	lhu	$6,20($2)
	lui	$2,%hi(vpn_client_info+122)
	addiu	$2,$2,%lo(vpn_client_info+122)
	move	$20,$0
	.set	noreorder
	.set	nomacro
	j	$L204
	li	$4,16			# 0x10
	.set	macro
	.set	reorder

$L202:
	addiu	$20,$20,1
$L230:
	.set	noreorder
	.set	nomacro
	beq	$20,$4,$L222
	addiu	$2,$2,208
	.set	macro
	.set	reorder

$L204:
	lbu	$3,0($2)
	beq	$3,$0,$L202
	lw	$3,-70($2)
	bne	$3,$5,$L202
	lhu	$3,-62($2)
	.set	noreorder
	.set	nomacro
	bne	$3,$6,$L230
	addiu	$20,$20,1
	.set	macro
	.set	reorder

	addiu	$20,$20,-1
	lui	$17,%hi(vpn_shared_info)
	addiu	$17,$17,%lo(vpn_shared_info)
	lw	$3,40($17)
	sll	$23,$20,2
	sll	$fp,$20,4
	sll	$2,$3,4
	sll	$3,$3,2
	addu	$2,$3,$2
	subu	$21,$fp,$23
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addu	$21,$21,$20
	sll	$21,$21,4
	sll	$4,$2,2
	lui	$22,%hi(vpn_client_info)
	addu	$2,$2,$4
	addiu	$22,$22,%lo(vpn_client_info)
	addiu	$4,$21,152
	lbu	$18,28($19)
	addu	$5,$2,$3
	addu	$4,$22,$4
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	addu	$21,$22,$21
	.set	macro
	.set	reorder

	lbu	$6,92($21)
	andi	$18,$18,0x7
	beq	$6,$18,$L206
	lbu	$7,116($21)
	.set	noreorder
	.set	nomacro
	bne	$7,$18,$L223
	li	$21,1			# 0x1
	.set	macro
	.set	reorder

$L208:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	lbu	$3,49($17)
	subu	$2,$2,$3
	addiu	$18,$2,-29
	andi	$2,$18,0xf
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L224
	addiu	$3,$3,29
	.set	macro
	.set	reorder

	lw	$4,16($17)
	addu	$5,$19,$3
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$6,$18
	.set	macro
	.set	reorder

	lbu	$3,48($17)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L225
	sll	$3,$21,5
	.set	macro
	.set	reorder

	subu	$23,$fp,$23
	addu	$20,$23,$20
	sll	$21,$21,3
	sll	$20,$20,4
	subu	$3,$3,$21
	lw	$2,16($17)
	addu	$3,$3,$20
	addu	$3,$22,$3
	li	$14,536805376			# 0x1fff0000
	addu	$22,$22,$20
	lw	$9,84($3)
	lw	$11,76($3)
	lbu	$10,121($22)
	lw	$13,28($17)
	lbu	$12,120($22)
	lw	$8,20($17)
	ori	$14,$14,0xffff
	and	$14,$2,$14
	li	$3,-1610612736			# 0xffffffffa0000000
	or	$14,$14,$3
	sw	$2,36($sp)
	addiu	$3,$18,-16
	li	$2,16			# 0x10
	li	$4,32			# 0x20
	li	$5,3			# 0x3
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$14,60($sp)
	sw	$13,16($sp)
	sw	$12,20($sp)
	sw	$11,24($sp)
	sw	$10,28($sp)
	sw	$9,32($sp)
	sw	$8,44($sp)
	sw	$2,48($sp)
	sw	$3,52($sp)
	sw	$18,56($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$0,40($sp)
	.set	macro
	.set	reorder

$L211:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L226
	lui	$4,%hi($LC16)
	.set	macro
	.set	reorder

	lw	$3,16($17)
	li	$4,16			# 0x10
	addu	$2,$3,$18
	lbu	$7,-1($2)
	subu	$2,$4,$7
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L214
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	addu	$4,$3,$2
	addu	$4,$4,$18
	lbu	$5,-16($4)
	.set	noreorder
	.set	nomacro
	bne	$5,$7,$L213
	move	$6,$2
	.set	macro
	.set	reorder

	addu	$4,$18,$2
	addu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	j	$L215
	li	$7,16			# 0x10
	.set	macro
	.set	reorder

$L227:
	lbu	$4,-15($3)
	.set	noreorder
	.set	nomacro
	bne	$5,$4,$L213
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L215:
	addiu	$2,$2,1
	bne	$2,$7,$L227
$L214:
	addiu	$18,$18,-37
	addu	$6,$18,$6
	addiu	$18,$6,29
	move	$5,$18
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_tail_direct
	sw	$6,64($sp)
	.set	macro
	.set	reorder

	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$5,16($17)
	lw	$6,64($sp)
	addiu	$4,$19,29
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$5,$5,21
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_protocol
	li	$5,8			# 0x8
	.set	macro
	.set	reorder

	lw	$18,4($17)
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L228
	lui	$4,%hi($LC17)
	.set	macro
	.set	reorder

$L216:
	li	$5,29			# 0x1d
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_pull
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$2,204($16)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	sw	$2,184($16)
	.set	macro
	.set	reorder

	lw	$5,136($18)
	lw	$3,128($18)
	move	$4,$16
	addu	$2,$5,$2
	addiu	$3,$3,1
	sw	$2,136($18)
	sw	$3,128($18)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_dev
	lw	$5,4($17)
	.set	macro
	.set	reorder

	lw	$2,76($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L217
	andi	$3,$2,0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L229
	li	$4,-2			# 0xfffffffffffffffe
	.set	macro
	.set	reorder

	sw	$0,76($16)
$L217:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_netif_rx
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L223:
	lui	$4,%hi($LC13)
	lui	$5,%hi(__func__.48641)
	addiu	$4,$4,%lo($LC13)
	addiu	$5,$5,%lo(__func__.48641)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$18,16($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC14)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC14)
	.set	macro
	.set	reorder

$L222:
	li	$2,-1			# 0xffffffffffffffff
$L205:
	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L229:
	.set	noreorder
	.set	nomacro
	jal	dst_release
	and	$4,$2,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L217
	sw	$0,76($16)
	.set	macro
	.set	reorder

$L213:
	.set	noreorder
	.set	nomacro
	j	$L214
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

$L225:
	subu	$23,$fp,$23
	addu	$20,$23,$20
	sll	$2,$21,5
	sll	$21,$21,3
	subu	$2,$2,$21
	sll	$20,$20,4
	lw	$3,16($17)
	addu	$2,$2,$20
	addu	$2,$22,$2
	lw	$8,76($2)
	addu	$20,$22,$20
	li	$2,536805376			# 0x1fff0000
	lbu	$9,120($20)
	lw	$10,24($17)
	addiu	$11,$3,16
	ori	$2,$2,0xffff
	and	$11,$11,$2
	li	$2,-1610612736			# 0xffffffffa0000000
	or	$11,$11,$2
	li	$4,32			# 0x20
	addiu	$2,$18,-16
	li	$5,-1			# 0xffffffffffffffff
	li	$6,1			# 0x1
	addiu	$7,$sp,56
	sw	$11,60($sp)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,36($sp)
	sw	$2,52($sp)
	sw	$2,56($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$0,48($sp)
	.set	macro
	.set	reorder

	j	$L211
$L228:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_dev_by_name
	addiu	$4,$4,%lo($LC17)
	.set	macro
	.set	reorder

	sw	$2,4($17)
	.set	noreorder
	.set	nomacro
	j	$L216
	move	$18,$2
	.set	macro
	.set	reorder

$L206:
	.set	noreorder
	.set	nomacro
	j	$L208
	move	$21,$0
	.set	macro
	.set	reorder

$L226:
	lui	$5,%hi(__func__.48641)
	addiu	$4,$4,%lo($LC16)
	addiu	$5,$5,%lo(__func__.48641)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,2088			# 0x828
	.set	macro
	.set	reorder

	li	$4,5			# 0x5
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine_init
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L205
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L224:
	lui	$4,%hi($LC15)
	lui	$5,%hi(__func__.48641)
	addiu	$4,$4,%lo($LC15)
	addiu	$5,$5,%lo(__func__.48641)
	.set	noreorder
	.set	nomacro
	jal	printk
	move	$6,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L205
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	rtk_openvpn_fragment_hw_decrypto
	.size	rtk_openvpn_fragment_hw_decrypto, .-rtk_openvpn_fragment_hw_decrypto
	.section	.rodata.str1.4
	.align	2
$LC18:
	.ascii	"%s:%d rtl_ipsecEngine failed!!! re-init hw crypto engine"
	.ascii	"!\012\000"
	.align	2
$LC19:
	.ascii	"\012%s:%d vpn_shared_info.tun_dev==NULL\012\000"
	.text
	.align	2
	.globl	rtk_openvpn_hw_decrypto
	.set	nomips16
	.ent	rtk_openvpn_hw_decrypto
	.type	rtk_openvpn_hw_decrypto, @function
rtk_openvpn_hw_decrypto:
	.frame	$sp,112,$31		# vars= 16, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	sw	$0,60($sp)
	move	$16,$4
	move	$7,$5
	addiu	$6,$sp,60
	.set	noreorder
	.set	nomacro
	jal	check_recv_pkt
	addiu	$5,$sp,56
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	beq	$2,$3,$L232
	li	$2,1			# 0x1
$L233:
	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L232:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_data
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,56($sp)
	move	$19,$2
	lui	$2,%hi(vpn_client_info+122)
	addu	$3,$19,$3
	addiu	$2,$2,%lo(vpn_client_info+122)
	lhu	$6,-9($3)
	lw	$5,-17($3)
	move	$20,$0
	.set	noreorder
	.set	nomacro
	j	$L236
	li	$4,16			# 0x10
	.set	macro
	.set	reorder

$L234:
	addiu	$20,$20,1
$L265:
	.set	noreorder
	.set	nomacro
	beq	$20,$4,$L256
	addiu	$2,$2,208
	.set	macro
	.set	reorder

$L236:
	lbu	$3,0($2)
	beq	$3,$0,$L234
	lw	$3,-70($2)
	bne	$3,$5,$L234
	lhu	$3,-62($2)
	.set	noreorder
	.set	nomacro
	bne	$3,$6,$L265
	addiu	$20,$20,1
	.set	macro
	.set	reorder

	addiu	$20,$20,-1
	lui	$17,%hi(vpn_shared_info)
	addiu	$17,$17,%lo(vpn_shared_info)
	lw	$3,40($17)
	sll	$23,$20,2
	sll	$fp,$20,4
	sll	$2,$3,4
	sll	$3,$3,2
	addu	$2,$3,$2
	subu	$18,$fp,$23
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addu	$18,$18,$20
	sll	$18,$18,4
	sll	$4,$2,2
	lui	$22,%hi(vpn_client_info)
	addu	$2,$2,$4
	addiu	$22,$22,%lo(vpn_client_info)
	addiu	$4,$18,152
	addu	$5,$2,$3
	addu	$4,$22,$4
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	addu	$18,$22,$18
	.set	macro
	.set	reorder

	lbu	$6,92($18)
	lw	$2,60($sp)
	.set	noreorder
	.set	nomacro
	beq	$6,$2,$L266
	move	$21,$0
	.set	macro
	.set	reorder

	lbu	$7,116($18)
	.set	noreorder
	.set	nomacro
	bne	$2,$7,$L257
	li	$21,1			# 0x1
	.set	macro
	.set	reorder

$L239:
$L266:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,56($sp)
	lbu	$5,49($17)
	subu	$2,$2,$3
	subu	$18,$2,$5
	andi	$2,$18,0xf
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L256
	addu	$5,$5,$3
	.set	macro
	.set	reorder

	lw	$4,16($17)
	addu	$5,$19,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$6,$18
	.set	macro
	.set	reorder

	lbu	$3,48($17)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L258
	sll	$3,$21,5
	.set	macro
	.set	reorder

	subu	$23,$fp,$23
	addu	$20,$23,$20
	sll	$21,$21,3
	sll	$20,$20,4
	subu	$3,$3,$21
	lw	$2,16($17)
	addu	$3,$3,$20
	addu	$3,$22,$3
	li	$14,536805376			# 0x1fff0000
	addu	$22,$22,$20
	lw	$9,84($3)
	lw	$11,76($3)
	lbu	$10,121($22)
	lw	$13,28($17)
	lbu	$12,120($22)
	lw	$8,20($17)
	ori	$14,$14,0xffff
	and	$14,$2,$14
	li	$3,-1610612736			# 0xffffffffa0000000
	or	$14,$14,$3
	sw	$2,36($sp)
	addiu	$3,$18,-16
	li	$2,16			# 0x10
	li	$4,32			# 0x20
	li	$5,3			# 0x3
	li	$6,1			# 0x1
	addiu	$7,$sp,64
	sw	$14,68($sp)
	sw	$13,16($sp)
	sw	$12,20($sp)
	sw	$11,24($sp)
	sw	$10,28($sp)
	sw	$9,32($sp)
	sw	$8,44($sp)
	sw	$2,48($sp)
	sw	$3,52($sp)
	sw	$18,64($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$0,40($sp)
	.set	macro
	.set	reorder

$L242:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L259
	lui	$4,%hi($LC18)
	.set	macro
	.set	reorder

	lw	$4,16($17)
	lui	$5,%hi(ping_string)
	addiu	$5,$5,%lo(ping_string)
	addiu	$4,$4,21
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L256
	li	$4,16			# 0x10
	.set	macro
	.set	reorder

	lw	$3,16($17)
	addu	$2,$3,$18
	lbu	$6,-1($2)
	subu	$2,$4,$6
	.set	noreorder
	.set	nomacro
	beq	$2,$4,$L246
	li	$7,16			# 0x10
	.set	macro
	.set	reorder

	addu	$4,$3,$2
	addu	$4,$4,$18
	lbu	$5,-16($4)
	.set	noreorder
	.set	nomacro
	bne	$5,$6,$L245
	move	$7,$2
	.set	macro
	.set	reorder

	addu	$4,$18,$2
	addu	$3,$3,$4
	.set	noreorder
	.set	nomacro
	j	$L247
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

$L260:
	lbu	$4,-15($3)
	.set	noreorder
	.set	nomacro
	bne	$5,$4,$L245
	addiu	$3,$3,1
	.set	macro
	.set	reorder

$L247:
	addiu	$2,$2,1
	bne	$2,$6,$L260
$L246:
	lw	$5,56($sp)
	addiu	$18,$18,-37
	addu	$18,$18,$7
	addu	$5,$18,$5
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_tail_direct
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$5,56($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_len
	addu	$5,$18,$5
	.set	macro
	.set	reorder

	lw	$5,16($17)
	lw	$4,56($sp)
	move	$6,$18
	addiu	$5,$5,21
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$4,$19,$4
	.set	macro
	.set	reorder

	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_protocol
	li	$5,8			# 0x8
	.set	macro
	.set	reorder

	lw	$18,4($17)
	beq	$18,$0,$L261
$L248:
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_set_skb_dev
	move	$5,$18
	.set	macro
	.set	reorder

	lw	$2,204($16)
	lw	$5,56($sp)
	move	$4,$16
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_skb_pull
	sw	$2,188($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,128($18)
	lw	$4,136($18)
	addiu	$3,$3,1
	addu	$2,$4,$2
	sw	$2,136($18)
	sw	$3,128($18)
	lw	$2,76($16)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L250
	andi	$3,$2,0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L262
	li	$4,-2			# 0xfffffffffffffffe
	.set	macro
	.set	reorder

	sw	$0,76($16)
$L250:
$L264:
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_netif_rx
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	move	$2,$0
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L261:
	lui	$4,%hi($LC17)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_dev_by_name
	addiu	$4,$4,%lo($LC17)
	.set	macro
	.set	reorder

	sw	$2,4($17)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L248
	move	$18,$2
	.set	macro
	.set	reorder

	lui	$4,%hi($LC19)
	lui	$5,%hi(__func__.48495)
	addiu	$4,$4,%lo($LC19)
	addiu	$5,$5,%lo(__func__.48495)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,1938			# 0x792
	.set	macro
	.set	reorder

$L256:
	li	$2,-1			# 0xffffffffffffffff
$L263:
	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

$L262:
	.set	noreorder
	.set	nomacro
	jal	dst_release
	and	$4,$2,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L264
	sw	$0,76($16)
	.set	macro
	.set	reorder

$L245:
	.set	noreorder
	.set	nomacro
	j	$L246
	li	$7,16			# 0x10
	.set	macro
	.set	reorder

$L258:
	subu	$23,$fp,$23
	addu	$20,$23,$20
	sll	$2,$21,5
	sll	$21,$21,3
	subu	$2,$2,$21
	sll	$20,$20,4
	lw	$3,16($17)
	addu	$2,$2,$20
	addu	$2,$22,$2
	lw	$8,76($2)
	addu	$20,$22,$20
	li	$2,536805376			# 0x1fff0000
	lbu	$9,120($20)
	lw	$10,24($17)
	addiu	$11,$3,16
	ori	$2,$2,0xffff
	and	$11,$11,$2
	li	$2,-1610612736			# 0xffffffffa0000000
	or	$11,$11,$2
	li	$4,32			# 0x20
	addiu	$2,$18,-16
	li	$5,-1			# 0xffffffffffffffff
	li	$6,1			# 0x1
	addiu	$7,$sp,64
	sw	$11,68($sp)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,36($sp)
	sw	$2,52($sp)
	sw	$2,64($sp)
	sw	$0,28($sp)
	sw	$0,32($sp)
	sw	$0,40($sp)
	sw	$0,44($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine
	sw	$0,48($sp)
	.set	macro
	.set	reorder

	j	$L242
$L259:
	lui	$5,%hi(__func__.48495)
	addiu	$4,$4,%lo($LC18)
	addiu	$5,$5,%lo(__func__.48495)
	.set	noreorder
	.set	nomacro
	jal	printk
	li	$6,1857			# 0x741
	.set	macro
	.set	reorder

	li	$4,5			# 0x5
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine_init
	li	$5,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L233
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L257:
	lui	$4,%hi($LC13)
	lui	$5,%hi(__func__.48495)
	addiu	$5,$5,%lo(__func__.48495)
	addiu	$4,$4,%lo($LC13)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC14)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC14)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L263
	li	$2,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	.end	rtk_openvpn_hw_decrypto
	.size	rtk_openvpn_hw_decrypto, .-rtk_openvpn_hw_decrypto
	.section	.rodata.str1.4
	.align	2
$LC20:
	.ascii	"\012%s alloc skb fail!!!\012\000"
	.align	2
$LC21:
	.ascii	"\012vpn_shared_info.wan_dev is NULL!!!\012\000"
	.text
	.align	2
	.globl	ping_send_timeout
	.set	nomips16
	.ent	ping_send_timeout
	.type	ping_send_timeout, @function
ping_send_timeout:
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
	sll	$2,$4,2
	sll	$19,$4,4
	subu	$19,$19,$2
	addu	$19,$19,$4
	lui	$2,%hi(vpn_client_info)
	addiu	$2,$2,%lo(vpn_client_info)
	sll	$19,$19,4
	addu	$19,$19,$2
	lbu	$2,122($19)
	bne	$2,$0,$L275
	lw	$31,44($sp)

	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,48

$L275:
	move	$4,$0
	li	$5,512			# 0x200
	jal	__netdev_alloc_skb
	li	$6,32			# 0x20

	beq	$2,$0,$L276
	move	$16,$2

	jal	rtl_vpn_get_skb_data
	move	$4,$2

	lui	$21,%hi(vpn_shared_info)
	move	$4,$2
	move	$5,$0
	li	$6,512			# 0x200
	addiu	$17,$21,%lo(vpn_shared_info)
	jal	memset
	move	$18,$2

	lbu	$2,32($17)
	lbu	$20,49($17)
	move	$5,$19
	move	$6,$2
	addu	$20,$20,$2
	move	$4,$18
	jal	memcpy
	addiu	$19,$20,21

	addu	$19,$18,$19
	lui	$5,%hi(ping_string)
	addiu	$5,$5,%lo(ping_string)
	move	$4,$19
	jal	memcpy
	li	$6,16			# 0x10

	addu	$3,$18,$20
	move	$2,$0
	addiu	$20,$20,37
	li	$5,11			# 0xb
	li	$4,11			# 0xb
$L270:
	addiu	$2,$2,1
	sb	$5,37($3)
	bne	$2,$4,$L270
	addiu	$3,$3,1

	li	$2,-6			# 0xfffffffffffffffa
	sb	$2,-1($19)
	sw	$0,-5($19)
	lbu	$2,32($17)
	move	$5,$0
	move	$4,$16
	addu	$2,$18,$2
	jal	rtl_vpn_set_skb_len
	sb	$0,-1($2)

	addiu	$5,$20,11
	jal	rtl_vpn_skb_put
	move	$4,$16

	move	$4,$16
	jal	rtl_vpn_get_skb_len
	lbu	$19,32($17)

	lbu	$3,32($17)
	addiu	$19,$19,-5
	addu	$19,$18,$19
	subu	$3,$2,$3
	addiu	$3,$3,9
	sh	$3,0($19)
	move	$4,$16
	jal	rtl_vpn_get_skb_len
	lbu	$19,32($17)

	lbu	$3,32($17)
	addiu	$19,$19,-27
	addu	$19,$18,$19
	subu	$3,$2,$3
	addiu	$3,$3,29
	sh	$3,0($19)
	lbu	$3,32($17)
	li	$2,51			# 0x33
	beq	$3,$2,$L277
	nop

$L271:
	lw	$5,%lo(vpn_shared_info)($21)
	beq	$5,$0,$L278
	lw	$31,44($sp)

	jal	rtl_vpn_set_skb_dev
	move	$4,$16

	move	$4,$16
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	rtl_vpn_call_skb_ndo_start_xmit
	addiu	$sp,$sp,48

$L277:
	jal	rtl_vpn_get_skb_len
	move	$4,$16

	addiu	$2,$2,-20
	j	$L271
	sh	$2,18($18)

$L276:
	lw	$31,44($sp)
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$4,%hi($LC20)
	lui	$5,%hi(__func__.47500)
	addiu	$4,$4,%lo($LC20)
	addiu	$5,$5,%lo(__func__.47500)
	j	printk
	addiu	$sp,$sp,48

$L278:
	lw	$21,40($sp)
	lw	$20,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	lui	$4,%hi($LC21)
	addiu	$4,$4,%lo($LC21)
	j	printk
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	ping_send_timeout
	.size	ping_send_timeout, .-ping_send_timeout
	.align	2
	.globl	delete_client_info
	.set	nomips16
	.ent	delete_client_info
	.type	delete_client_info, @function
delete_client_info:
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
	sll	$20,$4,2
	sll	$21,$4,4
	subu	$19,$21,$20
	addu	$19,$19,$4
	lui	$18,%hi(vpn_client_info)
	sll	$19,$19,4
	addiu	$18,$18,%lo(vpn_client_info)
	addu	$16,$18,$19
	lbu	$2,122($16)
	bne	$2,$0,$L284
	move	$17,$4

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

$L284:
	jal	del_timer
	addiu	$4,$16,124

	jal	del_timer
	addiu	$4,$16,180

	addiu	$4,$19,152
	jal	del_timer
	addu	$4,$18,$4

	lw	$4,72($16)
	li	$2,1			# 0x1
	sw	$2,64($16)
	sb	$0,122($16)
	sw	$0,68($16)
	sb	$0,123($16)
	sb	$0,92($16)
	beq	$4,$0,$L281
	sb	$0,116($16)

	li	$22,536805376			# 0x1fff0000
	ori	$22,$22,0xffff
	li	$19,-2147483648			# 0xffffffff80000000
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,76($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,80($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,84($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$2,88($16)
	and	$22,$2,$22
	jal	kfree
	or	$4,$22,$19

	sw	$0,72($16)
$L281:
	subu	$16,$21,$20
	addu	$16,$16,$17
	sll	$16,$16,4
	addu	$16,$18,$16
	lw	$4,96($16)
	beq	$4,$0,$L282
	li	$22,536805376			# 0x1fff0000

	ori	$22,$22,0xffff
	li	$19,-2147483648			# 0xffffffff80000000
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,100($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,104($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$4,108($16)
	and	$4,$4,$22
	jal	kfree
	or	$4,$4,$19

	lw	$2,112($16)
	and	$22,$2,$22
	jal	kfree
	or	$4,$22,$19

	sw	$0,96($16)
$L282:
	subu	$20,$21,$20
	addu	$17,$20,$17
	sll	$4,$17,4
	addu	$4,$18,$4
	lw	$31,44($sp)
	lw	$22,40($sp)
	lw	$21,36($sp)
	lw	$20,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$5,$0
	li	$6,51			# 0x33
	j	memset
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	delete_client_info
	.size	delete_client_info, .-delete_client_info
	.section	.rodata.str1.4
	.align	2
$LC22:
	.ascii	"\012OpenVPN CLIENT IP=%04x\012\012\000"
	.align	2
$LC23:
	.ascii	"\012%s##Invalid Client Index!\012\012\000"
	.align	2
$LC24:
	.ascii	"\012OpenVPN CLIENT %d IP=%04x PORT=%02x\012\012\000"
	.align	2
$LC25:
	.ascii	"%s##Invalid Flag!\012\000"
	.align	2
$LC26:
	.ascii	"\012OpenVPN SERVER PORT=%02x\012\012\000"
	.align	2
$LC27:
	.ascii	"\012OpenVPN SERVER KEY UPDATE TIME: %d\012\000"
	.align	2
$LC28:
	.ascii	"\012OpenVPN PING REC TIME: %d\012\000"
	.align	2
$LC29:
	.ascii	"\012OpenVPN PING SEND TIME: %d\012\000"
	.align	2
$LC30:
	.ascii	"\012OpenVPN AUTH NONE: %d\012\000"
	.align	2
$LC31:
	.ascii	"\012OpenVPN CLIENT %d IP=%04x PORT=%02x #####\012\012\000"
	.align	2
$LC32:
	.ascii	"\012OpenVPN CLIENT VIRTUAL IP=%04x\012\012\000"
	.align	2
$LC33:
	.ascii	"\012OpenVPN CLEAR CLIENT %d IP=%04x PORT=%02x !!!!!\012\000"
	.align	2
$LC34:
	.ascii	"\012%s recv openvpn exit event!!!\012\000"
	.text
	.align	2
	.globl	rtk_openvpn_netlink_receive
	.set	nomips16
	.ent	rtk_openvpn_netlink_receive
	.type	rtk_openvpn_netlink_receive, @function
rtk_openvpn_netlink_receive:
	.frame	$sp,592,$31		# vars= 536, regs= 10/0, args= 16, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-592
	sw	$31,588($sp)
	sw	$fp,584($sp)
	sw	$23,580($sp)
	sw	$22,576($sp)
	sw	$21,572($sp)
	sw	$20,568($sp)
	sw	$19,564($sp)
	sw	$18,560($sp)
	sw	$17,556($sp)
	sw	$16,552($sp)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$16,$4
	.set	macro
	.set	reorder

	sltu	$2,$2,16
	beq	$2,$0,$L338
$L329:
	lw	$31,588($sp)
$L355:
	lw	$fp,584($sp)
	lw	$23,580($sp)
	lw	$22,576($sp)
	lw	$21,572($sp)
	lw	$20,568($sp)
	lw	$19,564($sp)
	lw	$18,560($sp)
	lw	$17,556($sp)
	lw	$16,552($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,592
	.set	macro
	.set	reorder

$L338:
	lw	$23,204($16)
	.set	noreorder
	.set	nomacro
	jal	rtl_vpn_get_skb_len
	move	$4,$16
	.set	macro
	.set	reorder

	lw	$3,12($23)
	addiu	$19,$2,-16
	addiu	$20,$sp,272
	lui	$2,%hi(rtk_openvpn_pid)
	move	$4,$20
	addiu	$5,$23,16
	move	$6,$19
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,%lo(rtk_openvpn_pid)($2)
	.set	macro
	.set	reorder

	lhu	$2,6($23)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L339
	li	$3,10			# 0xa
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$19,$L290
	move	$16,$0
	.set	macro
	.set	reorder

	lui	$fp,%hi($LC25)
	lui	$22,%hi($L298)
	lui	$21,%hi(vpn_client_info)
	addiu	$fp,$fp,%lo($LC25)
	addiu	$22,$22,%lo($L298)
	addiu	$21,$21,%lo(vpn_client_info)
	li	$18,-1			# 0xffffffffffffffff
	addiu	$17,$sp,16
	.set	noreorder
	.set	nomacro
	j	$L303
	sw	$23,536($sp)
	.set	macro
	.set	reorder

$L291:
	lui	$5,%hi(__func__.47633)
	move	$4,$fp
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	addu	$2,$17,$16
	lbu	$2,257($2)
$L299:
	addiu	$2,$2,2
	addu	$16,$16,$2
	slt	$2,$16,$19
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L356
	lw	$23,536($sp)
	.set	macro
	.set	reorder

$L303:
	addu	$2,$17,$16
$L358:
	lbu	$2,256($2)
	sltu	$3,$2,7
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L291
	sll	$2,$2,2
	.set	macro
	.set	reorder

	addu	$2,$22,$2
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
	.align	2
$L298:
	.word	$L291
	.word	$L292
	.word	$L293
	.word	$L294
	.word	$L295
	.word	$L296
	.word	$L297
	.text
$L339:
	move	$16,$0
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L341
	li	$18,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L304:
	li	$3,8			# 0x8
$L359:
	beq	$2,$3,$L342
$L290:
	li	$3,9			# 0x9
	beq	$2,$3,$L343
$L305:
	li	$3,14			# 0xe
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L355
	lw	$31,588($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC34)
	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC34)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	move	$16,$0
	li	$17,16			# 0x10
	move	$4,$16
$L357:
	.set	noreorder
	.set	nomacro
	jal	delete_client_info
	addiu	$16,$16,1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$17,$L357
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(vpn_shared_info)
	addiu	$2,$3,%lo(vpn_shared_info)
	sw	$0,%lo(vpn_shared_info)($3)
	sw	$0,8($2)
	sh	$0,34($2)
	.set	noreorder
	.set	nomacro
	j	$L329
	sw	$0,4($2)
	.set	macro
	.set	reorder

$L297:
	sll	$2,$18,2
	sll	$23,$18,4
	subu	$23,$23,$2
	addu	$23,$23,$18
	addiu	$3,$16,1
	sll	$23,$23,4
	addu	$23,$23,$21
	addu	$3,$17,$3
	lbu	$6,256($3)
	lw	$4,104($23)
	lw	$5,80($23)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,544($sp)
	.set	macro
	.set	reorder

	lw	$3,544($sp)
	lw	$4,80($23)
	addiu	$5,$16,2
	lbu	$6,256($3)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$5,$20,$5
	.set	macro
	.set	reorder

	lw	$3,544($sp)
	lbu	$2,256($3)
	sb	$2,121($23)
	addiu	$2,$2,2
	addu	$16,$16,$2
	slt	$2,$16,$19
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L358
	addu	$2,$17,$16
	.set	macro
	.set	reorder

	lw	$23,536($sp)
$L356:
	li	$3,10			# 0xa
	lhu	$2,6($23)
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L359
	li	$3,8			# 0x8
	.set	macro
	.set	reorder

$L341:
	slt	$3,$16,$19
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L305
	lui	$fp,%hi($L312)
	.set	macro
	.set	reorder

	lui	$22,%hi(vpn_shared_info)
	addiu	$fp,$fp,%lo($L312)
	addiu	$22,$22,%lo(vpn_shared_info)
	.set	noreorder
	.set	nomacro
	j	$L314
	addiu	$17,$sp,16
	.set	macro
	.set	reorder

$L306:
	lui	$4,%hi($LC25)
$L360:
	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC25)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	addiu	$21,$16,1
$L313:
	addu	$21,$17,$21
$L353:
	lbu	$2,256($21)
	addiu	$2,$2,2
	addu	$16,$16,$2
	slt	$2,$16,$19
	beq	$2,$0,$L344
$L314:
	addu	$2,$17,$16
	lbu	$2,256($2)
	addiu	$2,$2,-7
	andi	$2,$2,0x00ff
	sltu	$3,$2,9
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L360
	lui	$4,%hi($LC25)
	.set	macro
	.set	reorder

	sll	$2,$2,2
	addu	$2,$fp,$2
	lw	$2,0($2)
	j	$2
	.rdata
	.align	2
	.align	2
$L312:
	.word	$L307
	.word	$L306
	.word	$L306
	.word	$L306
	.word	$L308
	.word	$L309
	.word	$L310
	.word	$L306
	.word	$L311
	.text
$L296:
	sll	$3,$18,2
	sll	$7,$18,4
	subu	$23,$7,$3
	addiu	$2,$16,1
	addu	$23,$23,$18
	sw	$2,532($sp)
	sll	$23,$23,4
	addu	$2,$17,$2
	addu	$23,$21,$23
	sw	$2,528($sp)
	lw	$4,108($23)
	lw	$5,84($23)
	lbu	$6,256($2)
	sw	$3,544($sp)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$7,540($sp)
	.set	macro
	.set	reorder

	lw	$2,528($sp)
	lw	$4,84($23)
	addiu	$5,$16,2
	lbu	$6,256($2)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$5,$20,$5
	.set	macro
	.set	reorder

	lbu	$2,123($23)
	sw	$0,68($23)
	lw	$3,544($sp)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L301
	lw	$7,540($sp)
	.set	macro
	.set	reorder

	lw	$3,528($sp)
	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($3)
	.set	macro
	.set	reorder

$L295:
	sll	$2,$18,2
	sll	$23,$18,4
	subu	$23,$23,$2
	addu	$23,$23,$18
	sll	$23,$23,4
	addiu	$3,$16,1
	addu	$23,$23,$21
	addu	$3,$17,$3
	lbu	$6,256($3)
	lw	$4,96($23)
	lw	$5,72($23)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,544($sp)
	.set	macro
	.set	reorder

	lw	$3,544($sp)
	lw	$4,72($23)
	addiu	$7,$16,2
	lbu	$6,256($3)
	addu	$7,$20,$7
	move	$5,$7
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$7,540($sp)
	.set	macro
	.set	reorder

	lw	$3,544($sp)
	lw	$4,112($23)
	lw	$5,88($23)
	lbu	$2,256($3)
	li	$6,16			# 0x10
	sb	$2,120($23)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$3,544($sp)
	.set	macro
	.set	reorder

	lw	$7,540($sp)
	lw	$4,88($23)
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$5,$7
	.set	macro
	.set	reorder

	lw	$3,544($sp)
	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($3)
	.set	macro
	.set	reorder

$L293:
	addiu	$23,$16,1
	addu	$23,$17,$23
	lbu	$6,256($23)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lhu	$2,16($sp)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	move	$5,$2
	li	$6,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	lookup_client_by_ip_and_port
	sh	$2,16($sp)
	.set	macro
	.set	reorder

	move	$18,$2
	sltu	$2,$2,16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L345
	sll	$3,$18,4
	.set	macro
	.set	reorder

	sll	$2,$18,2
	subu	$2,$3,$2
	lhu	$8,16($sp)
	lw	$3,20($sp)
	addu	$2,$2,$18
	sll	$2,$2,4
	addu	$2,$21,$2
	lui	$4,%hi($LC24)
	move	$6,$3
	move	$7,$8
	addiu	$4,$4,%lo($LC24)
	move	$5,$18
	sh	$8,60($2)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$3,52($2)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($23)
	.set	macro
	.set	reorder

$L292:
	addiu	$23,$16,1
	addu	$23,$17,$23
	lbu	$6,256($23)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	lw	$5,20($sp)
	lui	$4,%hi($LC22)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($23)
	.set	macro
	.set	reorder

$L294:
	sll	$2,$18,2
	sll	$23,$18,4
	subu	$23,$23,$2
	addu	$23,$23,$18
	addiu	$2,$16,1
	addu	$2,$17,$2
	sll	$23,$23,4
	addu	$23,$21,$23
	sw	$2,528($sp)
	lw	$4,100($23)
	lw	$5,76($23)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	lbu	$6,256($2)
	.set	macro
	.set	reorder

	lw	$2,528($sp)
	lw	$4,76($23)
	addiu	$5,$16,2
	lbu	$6,256($2)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addu	$5,$20,$5
	.set	macro
	.set	reorder

	lw	$3,528($sp)
	lw	$4,76($23)
	addiu	$6,$sp,28
	lbu	$5,256($3)
	.set	noreorder
	.set	nomacro
	jal	AES_set_encrypt_key
	sll	$5,$5,3
	.set	macro
	.set	reorder

	lw	$4,76($23)
	addiu	$5,$sp,252
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	lw	$4,76($23)
	addiu	$5,$sp,236
	li	$6,16			# 0x10
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,16
	.set	macro
	.set	reorder

	lw	$3,528($sp)
	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($3)
	.set	macro
	.set	reorder

$L301:
	lbu	$4,92($23)
	addiu	$2,$4,1
	andi	$2,$2,0x00ff
	sltu	$5,$2,8
	sb	$4,116($23)
	.set	noreorder
	.set	nomacro
	bne	$5,$0,$L302
	sb	$2,92($23)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sb	$2,92($23)
$L302:
	lui	$2,%hi(vpn_shared_info)
	addiu	$2,$2,%lo(vpn_shared_info)
	lw	$4,36($2)
	subu	$23,$7,$3
	addu	$23,$23,$18
	sll	$3,$4,4
	sll	$2,$4,2
	addu	$2,$2,$3
	lui	$3,%hi(jiffies)
	lw	$5,%lo(jiffies)($3)
	sll	$3,$2,2
	addu	$2,$2,$3
	sll	$23,$23,4
	addu	$4,$21,$23
	addu	$5,$2,$5
	addu	$23,$21,$23
	li	$2,1			# 0x1
	addiu	$4,$4,124
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	sw	$2,68($23)
	.set	macro
	.set	reorder

	lw	$3,532($sp)
	sb	$0,123($23)
	addu	$2,$17,$3
	.set	noreorder
	.set	nomacro
	j	$L299
	lbu	$2,256($2)
	.set	macro
	.set	reorder

$L342:
	slt	$2,$16,$19
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L329
	addiu	$17,$sp,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L321
	sw	$23,528($sp)
	.set	macro
	.set	reorder

$L348:
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L318
	li	$3,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L346
	lui	$4,%hi($LC25)
	.set	macro
	.set	reorder

	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC25)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	addiu	$22,$16,1
$L319:
	addu	$22,$17,$22
$L352:
	lbu	$2,256($22)
	addiu	$2,$2,2
	addu	$16,$16,$2
	slt	$2,$16,$19
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L347
	lw	$23,528($sp)
	.set	macro
	.set	reorder

$L321:
	addu	$2,$17,$16
	lbu	$2,256($2)
	li	$3,2			# 0x2
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L348
	li	$3,8			# 0x8
	.set	macro
	.set	reorder

	addiu	$22,$16,1
	addu	$2,$17,$22
	lbu	$6,256($2)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lhu	$5,16($sp)
	.set	noreorder
	.set	nomacro
	jal	lookup_client_by_ip_and_port
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

	move	$18,$2
	sltu	$2,$2,16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L337
	lw	$6,20($sp)
	.set	macro
	.set	reorder

	lhu	$7,16($sp)
	lui	$4,%hi($LC31)
	addiu	$4,$4,%lo($LC31)
	.set	noreorder
	.set	nomacro
	jal	printk
	move	$5,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L352
	addu	$22,$17,$22
	.set	macro
	.set	reorder

$L346:
	addiu	$22,$16,1
	addu	$2,$17,$22
	lbu	$6,256($2)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L352
	addu	$22,$17,$22
	.set	macro
	.set	reorder

$L318:
	addiu	$22,$16,1
	addu	$2,$17,$22
	lbu	$6,256($2)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,24
	.set	macro
	.set	reorder

	sll	$fp,$18,4
	sll	$2,$18,2
	subu	$fp,$fp,$2
	addu	$fp,$fp,$18
	lw	$2,20($sp)
	lui	$3,%hi(vpn_client_info)
	sll	$fp,$fp,4
	addiu	$3,$3,%lo(vpn_client_info)
	addu	$21,$3,$fp
	sw	$2,52($21)
	lhu	$2,16($sp)
	addu	$4,$3,$fp
	addiu	$4,$4,124
	sh	$2,60($21)
	li	$2,1			# 0x1
	sb	$2,122($21)
	lw	$2,24($sp)
	move	$5,$0
 #APP
 # 31 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			
	rotr	$2, $2, 16		

 # 0 "" 2
 #NO_APP
	move	$6,$0
	sw	$2,56($21)
	move	$7,$0
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	sw	$2,24($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(vpn_client_info)
	addiu	$2,$fp,152
	addiu	$3,$3,%lo(vpn_client_info)
	addu	$2,$3,$2
	lui	$3,%hi(key_update_timeout)
	addiu	$3,$3,%lo(key_update_timeout)
	move	$7,$0
	move	$4,$2
	sw	$3,140($21)
	move	$6,$0
	move	$5,$0
	sw	$18,144($21)
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	sw	$2,540($sp)
	.set	macro
	.set	reorder

	lui	$3,%hi(vpn_shared_info)
	addiu	$3,$3,%lo(vpn_shared_info)
	lw	$4,40($3)
	lw	$2,540($sp)
	lui	$23,%hi(jiffies)
	sll	$5,$4,4
	sll	$3,$4,2
	addu	$3,$3,$5
	lw	$6,%lo(jiffies)($23)
	sll	$4,$3,2
	addu	$3,$3,$4
	move	$4,$2
	lui	$2,%hi(ping_rec_timeout)
	addiu	$2,$2,%lo(ping_rec_timeout)
	addu	$5,$3,$6
	addiu	$fp,$21,180
	sw	$18,172($21)
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	sw	$2,168($21)
	.set	macro
	.set	reorder

	move	$7,$0
	move	$6,$0
	move	$4,$fp
	.set	noreorder
	.set	nomacro
	jal	init_timer_key
	move	$5,$0
	.set	macro
	.set	reorder

	lui	$2,%hi(vpn_shared_info)
	addiu	$2,$2,%lo(vpn_shared_info)
	lw	$3,44($2)
	lw	$6,%lo(jiffies)($23)
	move	$4,$fp
	sll	$5,$3,4
	sll	$2,$3,2
	addu	$2,$2,$5
	sll	$3,$2,2
	addu	$2,$2,$3
	lui	$3,%hi(ping_send_timeout)
	addiu	$3,$3,%lo(ping_send_timeout)
	addu	$5,$2,$6
	sw	$3,196($21)
	.set	noreorder
	.set	nomacro
	jal	mod_timer
	sw	$18,200($21)
	.set	macro
	.set	reorder

	lw	$5,24($sp)
	lui	$4,%hi($LC32)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC32)
	.set	macro
	.set	reorder

	lui	$3,%hi(engine_init_flag)
	lbu	$2,%lo(engine_init_flag)($3)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L319
	li	$4,5			# 0x5
	.set	macro
	.set	reorder

	li	$5,2			# 0x2
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecEngine_init
	sb	$2,%lo(engine_init_flag)($3)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	.set	noreorder
	.set	nomacro
	jal	rtl_ipsecSetOption
	li	$5,1			# 0x1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L352
	addu	$22,$17,$22
	.set	macro
	.set	reorder

$L311:
	addu	$2,$17,$16
	lbu	$2,258($2)
	lui	$4,%hi($LC30)
	addiu	$4,$4,%lo($LC30)
	sb	$0,49($22)
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	printk
	sb	$2,48($22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L313
	addiu	$21,$16,1
	.set	macro
	.set	reorder

$L310:
	addiu	$21,$16,1
	addu	$2,$17,$21
	lbu	$6,256($2)
	addiu	$5,$16,2
	lui	$4,%hi(vpn_shared_info+44)
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,%lo(vpn_shared_info+44)
	.set	macro
	.set	reorder

	lw	$5,44($22)
	lui	$4,%hi($LC29)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC29)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L353
	addu	$21,$17,$21
	.set	macro
	.set	reorder

$L309:
	addiu	$21,$16,1
	addu	$2,$17,$21
	lbu	$6,256($2)
	addiu	$5,$16,2
	lui	$4,%hi(vpn_shared_info+40)
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,%lo(vpn_shared_info+40)
	.set	macro
	.set	reorder

	lw	$5,40($22)
	lui	$4,%hi($LC28)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC28)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L353
	addu	$21,$17,$21
	.set	macro
	.set	reorder

$L308:
	addiu	$21,$16,1
	addu	$2,$17,$21
	lbu	$6,256($2)
	addiu	$5,$16,2
	lui	$4,%hi(vpn_shared_info+36)
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,%lo(vpn_shared_info+36)
	.set	macro
	.set	reorder

	lw	$5,36($22)
	lui	$2,%hi($LC27)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$2,%lo($LC27)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L353
	addu	$21,$17,$21
	.set	macro
	.set	reorder

$L307:
	addiu	$21,$16,1
	addu	$2,$17,$21
	lbu	$6,256($2)
	addiu	$5,$16,2
	lui	$4,%hi(vpn_shared_info+34)
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$4,%lo(vpn_shared_info+34)
	.set	macro
	.set	reorder

	lhu	$2,34($22)
	lui	$4,%hi($LC26)
 #APP
 # 20 "/home/jerry_ni/linux_release/release_sdk_3411d/branch-rtl819x-sdk-v3.4.11-1/linux-3.10/arch/mips-ori/include/uapi/asm/swab.h" 1
		wsbh	$2, $2			

 # 0 "" 2
 #NO_APP
	andi	$2,$2,0xffff
	addiu	$4,$4,%lo($LC26)
	move	$5,$2
	.set	noreorder
	.set	nomacro
	jal	printk
	sh	$2,34($22)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L353
	addu	$21,$17,$21
	.set	macro
	.set	reorder

$L343:
	slt	$2,$16,$19
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L329
	addiu	$17,$sp,16
	.set	macro
	.set	reorder

	li	$22,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L327
	li	$fp,2			# 0x2
	.set	macro
	.set	reorder

$L351:
	.set	noreorder
	.set	nomacro
	beq	$2,$fp,$L349
	lui	$4,%hi($LC25)
	.set	macro
	.set	reorder

	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC25)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	addiu	$18,$16,1
	addu	$18,$17,$18
$L354:
	lbu	$2,256($18)
	addiu	$2,$2,2
	addu	$16,$16,$2
	slt	$2,$16,$19
	beq	$2,$0,$L350
$L327:
	addu	$2,$17,$16
	lbu	$2,256($2)
	.set	noreorder
	.set	nomacro
	bne	$2,$22,$L351
	addiu	$18,$16,1
	.set	macro
	.set	reorder

	addu	$2,$17,$18
	lbu	$6,256($2)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	addiu	$4,$sp,20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L354
	addu	$18,$17,$18
	.set	macro
	.set	reorder

$L349:
	addiu	$18,$16,1
	addu	$2,$17,$18
	lbu	$6,256($2)
	addiu	$5,$16,2
	addu	$5,$20,$5
	.set	noreorder
	.set	nomacro
	jal	memcpy
	move	$4,$17
	.set	macro
	.set	reorder

	lw	$4,20($sp)
	lhu	$5,16($sp)
	.set	noreorder
	.set	nomacro
	jal	lookup_client_by_ip_and_port
	li	$6,2			# 0x2
	.set	macro
	.set	reorder

	move	$21,$2
	sltu	$2,$2,16
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L337
	lw	$6,20($sp)
	.set	macro
	.set	reorder

	lhu	$7,16($sp)
	lui	$4,%hi($LC33)
	addiu	$4,$4,%lo($LC33)
	.set	noreorder
	.set	nomacro
	jal	printk
	move	$5,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	delete_client_info
	move	$4,$21
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L354
	addu	$18,$17,$18
	.set	macro
	.set	reorder

$L344:
	.set	noreorder
	.set	nomacro
	j	$L304
	lhu	$2,6($23)
	.set	macro
	.set	reorder

$L347:
	.set	noreorder
	.set	nomacro
	j	$L290
	lhu	$2,6($23)
	.set	macro
	.set	reorder

$L350:
	.set	noreorder
	.set	nomacro
	j	$L305
	lhu	$2,6($23)
	.set	macro
	.set	reorder

$L337:
	lui	$4,%hi($LC11)
	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC11)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L355
	lw	$31,588($sp)
	.set	macro
	.set	reorder

$L345:
	lui	$4,%hi($LC23)
	lui	$5,%hi(__func__.47633)
	addiu	$4,$4,%lo($LC23)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$5,$5,%lo(__func__.47633)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L355
	lw	$31,588($sp)
	.set	macro
	.set	reorder

	.end	rtk_openvpn_netlink_receive
	.size	rtk_openvpn_netlink_receive, .-rtk_openvpn_netlink_receive
	.section	.rodata.str1.4
	.align	2
$LC35:
	.ascii	"%s:%d##date len is too long!\012\000"
	.align	2
$LC36:
	.ascii	"\0013net_link: allocate failed.\012\000"
	.align	2
$LC37:
	.ascii	"data_len=%d len=%d nlh is NULL!\012\012\000"
	.align	2
$LC38:
	.ascii	"\0013net_link: can not unicast skb (%d)\012\000"
	.text
	.align	2
	.globl	rtk_openvpn_netlink_send
	.set	nomips16
	.ent	rtk_openvpn_netlink_send
	.type	rtk_openvpn_netlink_send, @function
rtk_openvpn_netlink_send:
	.frame	$sp,56,$31		# vars= 0, regs= 7/0, args= 24, gp= 0
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
	slt	$2,$7,1601
	move	$17,$7
	move	$18,$4
	move	$19,$5
	beq	$2,$0,$L369
	move	$20,$6

	li	$4,1616			# 0x650
	li	$5,32			# 0x20
	move	$6,$0
	jal	__alloc_skb
	li	$7,-1			# 0xffffffffffffffff

	beq	$2,$0,$L370
	move	$16,$2

	lw	$2,88($2)
	bne	$2,$0,$L372
	lui	$4,%hi($LC37)

	lw	$3,196($16)
	lw	$2,192($16)
	subu	$2,$3,$2
	slt	$2,$2,1632
	bne	$2,$0,$L372
	li	$2,1616			# 0x650

	move	$4,$16
	move	$5,$0
	move	$6,$0
	move	$7,$0
	sw	$2,16($sp)
	jal	__nlmsg_put
	sw	$0,20($sp)

	beq	$2,$0,$L364
	move	$21,$2

	move	$6,$17
	move	$5,$20
	addiu	$4,$2,16
	sw	$0,36($16)
	jal	memcpy
	addiu	$17,$17,16

	sw	$17,0($21)
	move	$4,$19
	move	$5,$16
	move	$6,$18
	jal	netlink_unicast
	li	$7,64			# 0x40

	bltz	$2,$L371
	lw	$31,52($sp)

	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	$31
	addiu	$sp,$sp,56

$L364:
	lui	$4,%hi($LC37)
$L372:
	addiu	$4,$4,%lo($LC37)
	move	$5,$17
	li	$6,1616			# 0x650
$L368:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	j	printk
	addiu	$sp,$sp,56

$L370:
	lw	$31,52($sp)
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	lui	$4,%hi($LC36)
	addiu	$4,$4,%lo($LC36)
	j	printk
	addiu	$sp,$sp,56

$L369:
	lui	$4,%hi($LC35)
	lui	$5,%hi(__func__.47387)
	addiu	$4,$4,%lo($LC35)
	addiu	$5,$5,%lo(__func__.47387)
	j	$L368
	li	$6,126			# 0x7e

$L371:
	lw	$21,48($sp)
	lw	$20,44($sp)
	lw	$19,40($sp)
	lw	$18,36($sp)
	lw	$17,32($sp)
	lw	$16,28($sp)
	lui	$4,%hi($LC38)
	addiu	$4,$4,%lo($LC38)
	move	$5,$2
	j	printk
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	rtk_openvpn_netlink_send
	.size	rtk_openvpn_netlink_send, .-rtk_openvpn_netlink_send
	.section	.rodata.str1.4
	.align	2
$LC39:
	.ascii	"\012Ping Rec Timeout!!!\012\000"
	.text
	.align	2
	.globl	ping_rec_timeout
	.set	nomips16
	.ent	ping_rec_timeout
	.type	ping_rec_timeout, @function
ping_rec_timeout:
	.frame	$sp,104,$31		# vars= 64, regs= 6/0, args= 16, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$20,96($sp)
	sw	$19,92($sp)
	sw	$18,88($sp)
	sw	$17,84($sp)
	sw	$16,80($sp)
	move	$16,$4
	lui	$4,%hi($LC39)
	addiu	$17,$sp,16
	jal	printk
	addiu	$4,$4,%lo($LC39)

	move	$4,$17
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	sll	$20,$16,4
	sll	$2,$16,2
	subu	$20,$20,$2
	addu	$5,$20,$16
	addu	$18,$20,$16
	lui	$19,%hi(vpn_client_info)
	addiu	$19,$19,%lo(vpn_client_info)
	sll	$5,$5,4
	sll	$18,$18,4
	addu	$5,$19,$5
	addiu	$18,$18,56
	addu	$18,$19,$18
	move	$4,$17
	addiu	$5,$5,52
	jal	memcpy
	li	$6,4			# 0x4

	addiu	$4,$sp,20
	move	$5,$18
	jal	memcpy
	li	$6,4			# 0x4

	addiu	$5,$18,4
	addiu	$4,$sp,24
	jal	memcpy
	li	$6,2			# 0x2

	lui	$2,%hi(rtk_openvpn_pid)
	lw	$4,%lo(rtk_openvpn_pid)($2)
	lui	$2,%hi(nl_openvpn_sk)
	lw	$5,%lo(nl_openvpn_sk)($2)
	move	$6,$17
	jal	rtk_openvpn_netlink_send
	li	$7,10			# 0xa

	jal	delete_client_info
	move	$4,$16

	lw	$31,100($sp)
	lw	$20,96($sp)
	lw	$19,92($sp)
	lw	$18,88($sp)
	lw	$17,84($sp)
	lw	$16,80($sp)
	j	$31
	addiu	$sp,$sp,104

	.set	macro
	.set	reorder
	.end	ping_rec_timeout
	.size	ping_rec_timeout, .-ping_rec_timeout
	.align	2
	.globl	GenerateRandomData
	.set	nomips16
	.ent	GenerateRandomData
	.type	GenerateRandomData, @function
GenerateRandomData:
	.frame	$sp,56,$31		# vars= 8, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$18,$5
	beq	$5,$0,$L378
	move	$19,$4

	addiu	$17,$sp,16
	addiu	$22,$sp,17
	addiu	$21,$sp,18
	addiu	$20,$sp,19
	move	$16,$0
$L377:
	move	$4,$17
	jal	get_random_bytes
	li	$5,4			# 0x4

	lbu	$5,0($22)
	lbu	$4,0($21)
	lbu	$3,0($17)
	lbu	$2,0($20)
	xor	$4,$5,$4
	xor	$4,$4,$3
	addu	$3,$19,$16
	addiu	$16,$16,1
	xor	$4,$4,$2
	sltu	$2,$16,$18
	bne	$2,$0,$L377
	sb	$4,0($3)

$L378:
	lw	$31,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	GenerateRandomData
	.size	GenerateRandomData, .-GenerateRandomData
	.align	2
	.globl	increaseVI
	.set	nomips16
	.ent	increaseVI
	.type	increaseVI, @function
increaseVI:
	.frame	$sp,56,$31		# vars= 8, regs= 8/0, args= 16, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$22,48($sp)
	sw	$21,44($sp)
	sw	$20,40($sp)
	sw	$19,36($sp)
	sw	$18,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	move	$2,$0
	move	$18,$4
	move	$3,$4
	li	$7,16			# 0x10
$L383:
	lbu	$4,0($3)
	addu	$4,$5,$4
	slt	$6,$4,256
	bne	$6,$0,$L388
	addiu	$2,$2,1

	bne	$2,$7,$L383
	addiu	$3,$3,1

	addiu	$17,$sp,16
	addiu	$22,$sp,17
	addiu	$21,$sp,18
	addiu	$20,$sp,19
	move	$16,$0
	li	$19,16			# 0x10
$L384:
	move	$4,$17
	jal	get_random_bytes
	li	$5,4			# 0x4

	lbu	$5,0($22)
	lbu	$3,0($21)
	lbu	$4,0($17)
	lbu	$2,0($20)
	xor	$3,$5,$3
	xor	$3,$3,$4
	xor	$3,$3,$2
	addu	$2,$18,$16
	addiu	$16,$16,1
	bne	$16,$19,$L384
	sb	$3,0($2)

	lw	$31,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,56

$L388:
	sb	$4,0($3)
	lw	$31,52($sp)
	lw	$22,48($sp)
	lw	$21,44($sp)
	lw	$20,40($sp)
	lw	$19,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
	.end	increaseVI
	.size	increaseVI, .-increaseVI
	.globl	ping_string
	.rdata
	.align	2
	.type	ping_string, @object
	.size	ping_string, 16
ping_string:
	.byte	42
	.byte	24
	.byte	123
	.byte	-13
	.byte	100
	.byte	30
	.byte	-76
	.byte	-53
	.byte	7
	.byte	-19
	.byte	45
	.byte	10
	.byte	-104
	.byte	31
	.byte	-57
	.byte	72
	.globl	EMPTY_MAC_ADDR
	.align	2
	.type	EMPTY_MAC_ADDR, @object
	.size	EMPTY_MAC_ADDR, 6
EMPTY_MAC_ADDR:
	.space	6
	.globl	nl_openvpn_sk
	.section	.bss,"aw",@nobits
	.align	2
	.type	nl_openvpn_sk, @object
	.size	nl_openvpn_sk, 4
nl_openvpn_sk:
	.space	4
	.section	___ksymtab+openvpn_fast_to_wan,"a",@progbits
	.align	2
	.type	__ksymtab_openvpn_fast_to_wan, @object
	.size	__ksymtab_openvpn_fast_to_wan, 8
__ksymtab_openvpn_fast_to_wan:
	.word	openvpn_fast_to_wan
	.word	__kstrtab_openvpn_fast_to_wan
	.section	___ksymtab+rtk_openvpn_fragment_hw_decrypto,"a",@progbits
	.align	2
	.type	__ksymtab_rtk_openvpn_fragment_hw_decrypto, @object
	.size	__ksymtab_rtk_openvpn_fragment_hw_decrypto, 8
__ksymtab_rtk_openvpn_fragment_hw_decrypto:
	.word	rtk_openvpn_fragment_hw_decrypto
	.word	__kstrtab_rtk_openvpn_fragment_hw_decrypto
	.section	___ksymtab+rtk_openvpn_fragment_hw_encrypto,"a",@progbits
	.align	2
	.type	__ksymtab_rtk_openvpn_fragment_hw_encrypto, @object
	.size	__ksymtab_rtk_openvpn_fragment_hw_encrypto, 8
__ksymtab_rtk_openvpn_fragment_hw_encrypto:
	.word	rtk_openvpn_fragment_hw_encrypto
	.word	__kstrtab_rtk_openvpn_fragment_hw_encrypto
	.section	___ksymtab+openvpn_fragment_fast_to_wan,"a",@progbits
	.align	2
	.type	__ksymtab_openvpn_fragment_fast_to_wan, @object
	.size	__ksymtab_openvpn_fragment_fast_to_wan, 8
__ksymtab_openvpn_fragment_fast_to_wan:
	.word	openvpn_fragment_fast_to_wan
	.word	__kstrtab_openvpn_fragment_fast_to_wan
	.rdata
	.align	2
	.type	__func__.48870, @object
	.size	__func__.48870, 22
__func__.48870:
	.ascii	"rtl_fast_openvpn_init\000"
	.align	2
	.type	__func__.48775, @object
	.size	__func__.48775, 20
__func__.48775:
	.ascii	"openvpn_fast_to_wan\000"
	.align	2
	.type	__func__.48641, @object
	.size	__func__.48641, 33
__func__.48641:
	.ascii	"rtk_openvpn_fragment_hw_decrypto\000"
	.align	2
	.type	__func__.48495, @object
	.size	__func__.48495, 24
__func__.48495:
	.ascii	"rtk_openvpn_hw_decrypto\000"
	.align	2
	.type	__func__.48308, @object
	.size	__func__.48308, 33
__func__.48308:
	.ascii	"rtk_openvpn_fragment_hw_encrypto\000"
	.align	2
	.type	__func__.48237, @object
	.size	__func__.48237, 29
__func__.48237:
	.ascii	"openvpn_fragment_fast_to_wan\000"
	.align	2
	.type	__func__.48036, @object
	.size	__func__.48036, 24
__func__.48036:
	.ascii	"rtk_openvpn_hw_encrypto\000"
	.align	2
	.type	__func__.47845, @object
	.size	__func__.47845, 15
__func__.47845:
	.ascii	"check_send_pkt\000"
	.align	2
	.type	__func__.47810, @object
	.size	__func__.47810, 20
__func__.47810:
	.ascii	"is_openvpn_fragment\000"
	.local	engine_init_flag
	.comm	engine_init_flag,1,1
	.align	2
	.type	__func__.47633, @object
	.size	__func__.47633, 28
__func__.47633:
	.ascii	"rtk_openvpn_netlink_receive\000"
	.align	2
	.type	__func__.47500, @object
	.size	__func__.47500, 18
__func__.47500:
	.ascii	"ping_send_timeout\000"
	.align	2
	.type	__func__.47387, @object
	.size	__func__.47387, 25
__func__.47387:
	.ascii	"rtk_openvpn_netlink_send\000"
	.local	rtk_openvpn_pid
	.comm	rtk_openvpn_pid,4,4
	.globl	vpn_client_info
	.section	.bss
	.align	2
	.type	vpn_client_info, @object
	.size	vpn_client_info, 3328
vpn_client_info:
	.space	3328
	.globl	vpn_shared_info
	.align	2
	.type	vpn_shared_info, @object
	.size	vpn_shared_info, 52
vpn_shared_info:
	.space	52
	.section	__ksymtab_strings,"a",@progbits
	.type	__kstrtab_openvpn_fast_to_wan, @object
	.size	__kstrtab_openvpn_fast_to_wan, 20
__kstrtab_openvpn_fast_to_wan:
	.ascii	"openvpn_fast_to_wan\000"
	.type	__kstrtab_rtk_openvpn_fragment_hw_decrypto, @object
	.size	__kstrtab_rtk_openvpn_fragment_hw_decrypto, 33
__kstrtab_rtk_openvpn_fragment_hw_decrypto:
	.ascii	"rtk_openvpn_fragment_hw_decrypto\000"
	.type	__kstrtab_rtk_openvpn_fragment_hw_encrypto, @object
	.size	__kstrtab_rtk_openvpn_fragment_hw_encrypto, 33
__kstrtab_rtk_openvpn_fragment_hw_encrypto:
	.ascii	"rtk_openvpn_fragment_hw_encrypto\000"
	.type	__kstrtab_openvpn_fragment_fast_to_wan, @object
	.size	__kstrtab_openvpn_fragment_fast_to_wan, 29
__kstrtab_openvpn_fragment_fast_to_wan:
	.ascii	"openvpn_fragment_fast_to_wan\000"
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
