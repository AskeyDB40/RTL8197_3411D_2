	.file	1 "filter_ipv4.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.section	.text.dos_pkt_locate,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	dos_pkt_locate
	.type	dos_pkt_locate, @function
dos_pkt_locate:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lw	$2,12($4)
	lui	$3,%hi(ConnectedIp)
	addiu	$3,$3,%lo(ConnectedIp)
	andi	$4,$2,0xff
	sll	$4,$4,2
	addu	$3,$4,$3
	andi	$4,$2,0x3ff
	lw	$5,0($3)
	sll	$3,$4,3
	sll	$4,$4,5
	addu	$4,$3,$4
	lui	$3,%hi(dos_pkt)
	addiu	$3,$3,%lo(dos_pkt)
	addu	$3,$3,$4
	lw	$4,28($3)
	beq	$2,$5,$L9
	nop

	beq	$2,$4,$L5
	nop

$L3:
	lb	$4,0($3)
	nop
	beq	$4,$0,$L10
	nop

	j	$31
	move	$2,$0

$L10:
	sw	$2,28($3)
	li	$2,1			# 0x1
	sb	$2,0($3)
	lui	$2,%hi(cur_p_pkt)
	sw	$3,%lo(cur_p_pkt)($2)
	j	$31
	li	$2,1			# 0x1

$L5:
	lb	$4,0($3)
	li	$2,1			# 0x1
	beq	$4,$2,$L11
	nop

	j	$31
	move	$2,$0

$L9:
	bne	$2,$4,$L3
	nop

	sb	$0,0($3)
	j	$31
	move	$2,$0

$L11:
	lui	$2,%hi(cur_p_pkt)
	sw	$3,%lo(cur_p_pkt)($2)
	j	$31
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
	.end	dos_pkt_locate
	.size	dos_pkt_locate, .-dos_pkt_locate
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"%u.%u.%u.%u\000"
	.section	.text.IP2NAME,"ax",@progbits
	.align	2
	.set	nomips16
	.ent	IP2NAME
	.type	IP2NAME, @function
IP2NAME:
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 24, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$16,24($sp)
	move	$16,$5
	sw	$4,32($sp)
	move	$5,$0
	move	$4,$16
	jal	memset
	li	$6,16			# 0x10

	lbu	$3,34($sp)
	lbu	$2,35($sp)
	lbu	$6,32($sp)
	lbu	$7,33($sp)
	lui	$5,%hi($LC0)
	move	$4,$16
	addiu	$5,$5,%lo($LC0)
	sw	$3,16($sp)
	jal	sprintf
	sw	$2,20($sp)

	move	$2,$16
	lw	$31,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	IP2NAME
	.size	IP2NAME, .-IP2NAME
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"%s\012\000"
	.align	2
$LC2:
	.ascii	"connectip = %s\012\000"
	.align	2
$LC3:
	.ascii	"pkt---IP:%s, syn_cnt:%d, fin_cnt:%d, udp_nct:%d, icmp_cn"
	.ascii	"t:%d, scan_cnt:%d\012\000"
	.align	2
$LC4:
	.ascii	"attack_saddr[%s]=%s,attack_daddr[%s]=%s\012\000"
	.align	2
$LC5:
	.ascii	"op_mode = %d, lan_addr = %s, lan_mask = %s, dos_item_glo"
	.ascii	"bal = 0x%x,dos_item_global_0 = 0x%x\012\000"
	.section	.text.dos_read_proc_handler,"ax",@progbits
	.align	2
	.globl	dos_read_proc_handler
	.set	nomips16
	.ent	dos_read_proc_handler
	.type	dos_read_proc_handler, @function
dos_read_proc_handler:
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
	lui	$5,%hi($LC1)
	lui	$6,%hi(dos_flag)
	lui	$16,%hi(ConnectedIp)
	addiu	$5,$5,%lo($LC1)
	addiu	$6,$6,%lo(dos_flag)
	addiu	$16,$16,%lo(ConnectedIp)
	lui	$fp,%hi(ip_name0)
	lui	$18,%hi($LC2)
	move	$19,$4
	jal	seq_printf
	addiu	$17,$16,1024

	addiu	$20,$fp,%lo(ip_name0)
	addiu	$18,$18,%lo($LC2)
$L15:
	lw	$4,0($16)
	nop
	bne	$4,$0,$L25
	addiu	$16,$16,4

	bne	$16,$17,$L15
	nop

	lui	$16,%hi(dos_pkt)
$L28:
	addiu	$16,$16,%lo(dos_pkt)
	li	$18,40960			# 0xa000
	jal	local_bh_disable
	lui	$20,%hi($LC3)

	addu	$18,$16,$18
	li	$17,1			# 0x1
	addiu	$21,$fp,%lo(ip_name0)
	j	$L17
	addiu	$20,$20,%lo($LC3)

$L16:
	addiu	$16,$16,40
	beq	$16,$18,$L26
	nop

$L17:
	lb	$2,0($16)
	nop
	bne	$2,$17,$L16
	nop

	lw	$4,28($16)
	jal	IP2NAME
	move	$5,$21

	lw	$7,4($16)
	lw	$10,8($16)
	lw	$9,12($16)
	lw	$8,16($16)
	lw	$3,24($16)
	move	$4,$19
	move	$5,$20
	move	$6,$2
	addiu	$16,$16,40
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	jal	seq_printf
	sw	$3,28($sp)

	bne	$16,$18,$L17
	nop

$L26:
	lui	$16,%hi(attack_daddr)
	addiu	$16,$16,%lo(attack_daddr)
	lui	$17,%hi(attack_saddr)
	lui	$18,%hi(rtl_dos_name)
	lui	$2,%hi(ip_name1)
	lui	$22,%hi($LC4)
	addiu	$17,$17,%lo(attack_saddr)
	addiu	$18,$18,%lo(rtl_dos_name)
	addiu	$21,$16,108
	addiu	$23,$2,%lo(ip_name1)
	addiu	$22,$22,%lo($LC4)
$L21:
	lw	$2,0($16)
	lw	$4,0($17)
	bne	$2,$0,$L29
	lui	$5,%hi(ip_name0)

	beq	$4,$0,$L20
	nop

$L29:
	jal	IP2NAME
	addiu	$5,$5,%lo(ip_name0)

	lw	$4,0($16)
	lw	$20,0($18)
	move	$5,$23
	jal	IP2NAME
	sw	$2,36($sp)

	lw	$7,36($sp)
	move	$4,$19
	move	$5,$22
	move	$6,$20
	sw	$20,16($sp)
	jal	seq_printf
	sw	$2,20($sp)

$L20:
	addiu	$16,$16,4
	addiu	$17,$17,4
	bne	$16,$21,$L21
	addiu	$18,$18,4

	jal	local_bh_enable
	nop

	lui	$2,%hi(dos_lan_addr)
	lw	$4,%lo(dos_lan_addr)($2)
	lui	$2,%hi(dos_op_mode)
	lb	$6,%lo(dos_op_mode)($2)
	addiu	$5,$fp,%lo(ip_name0)
	jal	IP2NAME
	sw	$6,32($sp)

	move	$7,$2
	lui	$2,%hi(dos_lan_mask)
	lw	$4,%lo(dos_lan_mask)($2)
	lui	$2,%hi(ip_name1)
	addiu	$5,$2,%lo(ip_name1)
	jal	IP2NAME
	sw	$7,36($sp)

	lui	$3,%hi(dos_item_global)
	lw	$8,%lo(dos_item_global)($3)
	lui	$3,%hi(dos_item_global_0)
	lw	$3,%lo(dos_item_global_0)($3)
	lw	$6,32($sp)
	lw	$7,36($sp)
	lui	$5,%hi($LC5)
	move	$4,$19
	addiu	$5,$5,%lo($LC5)
	sw	$2,16($sp)
	sw	$8,20($sp)
	jal	seq_printf
	sw	$3,24($sp)

	lw	$31,76($sp)
	lw	$fp,72($sp)
	lw	$23,68($sp)
	lw	$22,64($sp)
	lw	$21,60($sp)
	lw	$20,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,80

$L25:
	jal	IP2NAME
	move	$5,$20

	move	$6,$2
	move	$4,$19
	jal	seq_printf
	move	$5,$18

	bne	$16,$17,$L15
	nop

	j	$L28
	lui	$16,%hi(dos_pkt)

	.set	macro
	.set	reorder
	.end	dos_read_proc_handler
	.size	dos_read_proc_handler, .-dos_read_proc_handler
	.section	.text.clear_attack_address_ipv4,"ax",@progbits
	.align	2
	.globl	clear_attack_address_ipv4
	.set	nomips16
	.ent	clear_attack_address_ipv4
	.type	clear_attack_address_ipv4, @function
clear_attack_address_ipv4:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$7,%hi(attack_saddr)
	lui	$6,%hi(attack_daddr)
	move	$2,$0
	addiu	$7,$7,%lo(attack_saddr)
	addiu	$6,$6,%lo(attack_daddr)
	li	$5,108			# 0x6c
$L31:
	addu	$4,$7,$2
	addu	$3,$6,$2
	addiu	$2,$2,4
	sw	$0,0($4)
	bne	$2,$5,$L31
	sw	$0,0($3)

	j	$31
	nop

	.set	macro
	.set	reorder
	.end	clear_attack_address_ipv4
	.size	clear_attack_address_ipv4, .-clear_attack_address_ipv4
	.section	.text.filter_out,"ax",@progbits
	.align	2
	.globl	filter_out
	.set	nomips16
	.ent	filter_out
	.type	filter_out, @function
filter_out:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	rtl_ip_hdr
	nop

	lbu	$4,0($2)
	lbu	$3,9($2)
	andi	$5,$4,0xf
	li	$4,1			# 0x1
	beq	$3,$4,$L35
	nop

$L34:
	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

$L35:
	sll	$5,$5,2
	addu	$2,$2,$5
	lbu	$4,0($2)
	li	$2,8			# 0x8
	bne	$4,$2,$L34
	lui	$2,%hi(jiffies)

	lw	$4,%lo(jiffies)($2)
	lw	$31,20($sp)
	lui	$2,%hi(icmp_echo_request_time)
	sw	$4,%lo(icmp_echo_request_time)($2)
	lui	$2,%hi(icmp_echo_request)
	sw	$3,%lo(icmp_echo_request)($2)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	filter_out
	.size	filter_out, .-filter_out
	.section	.text.dos_filter_ipv4__action,"ax",@progbits
	.align	2
	.globl	dos_filter_ipv4__action
	.set	nomips16
	.ent	dos_filter_ipv4__action
	.type	dos_filter_ipv4__action, @function
dos_filter_ipv4__action:
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
	jal	rtl_ip_hdr
	lui	$18,%hi(dos_item_global)

	lbu	$19,0($2)
	move	$16,$2
	lw	$2,%lo(dos_item_global)($18)
	andi	$19,$19,0xf
	sll	$19,$19,2
	andi	$2,$2,0x3fe
	bne	$2,$0,$L158
	addu	$19,$16,$19

	lui	$2,%hi(dos_item_global_0)
$L206:
	lw	$2,%lo(dos_item_global_0)($2)
	nop
	andi	$3,$2,0x1
	bne	$3,$0,$L159
	move	$17,$0

$L63:
	andi	$2,$2,0x2
$L193:
	beq	$2,$0,$L77
	li	$3,17			# 0x11

	lbu	$2,9($16)
	nop
	beq	$2,$3,$L78
	li	$3,6			# 0x6

	beq	$2,$3,$L78
	nop

$L77:
	lw	$2,%lo(dos_item_global)($18)
	li	$3,536870912			# 0x20000000
	and	$4,$2,$3
	bne	$4,$0,$L160
	li	$5,1			# 0x1

$L81:
	li	$3,1073741824			# 0x40000000
$L210:
	and	$4,$2,$3
	beq	$4,$0,$L86
	nop

	lbu	$4,0($16)
	lbu	$5,9($16)
	andi	$6,$4,0xf
	li	$4,1			# 0x1
	beq	$5,$4,$L161
	li	$4,8			# 0x8

$L86:
	bltz	$2,$L162
	nop

$L88:
	andi	$3,$2,0x8000
$L211:
	beq	$3,$0,$L196
	li	$3,65536			# 0x10000

	lbu	$4,9($16)
	li	$3,6			# 0x6
	beq	$4,$3,$L163
	lui	$3,%hi(_tcpDosScanBitmap)

$L90:
	li	$3,65536			# 0x10000
$L196:
	and	$4,$2,$3
	beq	$4,$0,$L92
	li	$4,6			# 0x6

	lbu	$5,9($16)
	nop
	beq	$5,$4,$L164
	li	$4,2			# 0x2

$L92:
	andi	$3,$2,0x800
	beq	$3,$0,$L197
	li	$3,262144			# 0x40000

	lw	$3,12($16)
	lw	$4,16($16)
	nop
	beq	$3,$4,$L165
	lui	$4,%hi(LogFlag)

$L95:
	li	$3,262144			# 0x40000
$L197:
	and	$3,$2,$3
	beq	$3,$0,$L198
	li	$3,131072			# 0x20000

	lbu	$3,9($16)
	li	$4,17			# 0x11
	beq	$3,$4,$L166
	li	$5,7			# 0x7

$L97:
	li	$3,131072			# 0x20000
$L198:
	and	$4,$2,$3
	beq	$4,$0,$L100
	li	$4,17			# 0x11

	lbu	$5,9($16)
	nop
	beq	$5,$4,$L167
	nop

$L100:
	andi	$3,$2,0x4000
	beq	$3,$0,$L199
	andi	$3,$2,0x400

	lhu	$3,6($16)
	nop
	andi	$4,$3,0x2000
	beq	$4,$0,$L168
	andi	$3,$3,0x1fff

$L102:
	andi	$3,$2,0x400
$L199:
	beq	$3,$0,$L200
	li	$3,524288			# 0x80000

	lbu	$3,0($16)
	lbu	$4,9($16)
	andi	$5,$3,0xf
	li	$3,1			# 0x1
	beq	$4,$3,$L169
	li	$3,8			# 0x8

$L104:
	li	$3,524288			# 0x80000
$L200:
	and	$4,$2,$3
	beq	$4,$0,$L106
	li	$4,17			# 0x11

	lbu	$5,9($16)
	nop
	beq	$5,$4,$L170
	lui	$5,%hi(dos_lan_addr)

$L106:
	li	$3,16777216			# 0x1000000
	and	$3,$2,$3
	beq	$3,$0,$L201
	li	$3,33554432			# 0x2000000

	lbu	$3,0($16)
	lbu	$4,9($16)
	andi	$5,$3,0xf
	li	$3,1			# 0x1
	beq	$4,$3,$L171
	li	$4,8			# 0x8

$L108:
	li	$3,33554432			# 0x2000000
$L201:
	and	$3,$2,$3
	beq	$3,$0,$L202
	li	$3,67108864			# 0x4000000

	lbu	$4,9($16)
	li	$3,17			# 0x11
	beq	$4,$3,$L172
	li	$4,7			# 0x7

$L112:
	li	$3,67108864			# 0x4000000
$L202:
	and	$3,$2,$3
	beq	$3,$0,$L203
	li	$3,134217728			# 0x8000000

	lbu	$4,9($16)
	li	$3,6			# 0x6
	beq	$4,$3,$L173
	nop

$L115:
	li	$3,134217728			# 0x8000000
$L203:
	and	$4,$2,$3
	beq	$4,$0,$L118
	li	$4,6			# 0x6

	lbu	$5,9($16)
	nop
	beq	$5,$4,$L174
	nop

$L118:
	li	$3,268435456			# 0x10000000
	and	$3,$2,$3
	beq	$3,$0,$L204
	andi	$3,$2,0x1000

	lw	$3,12($16)
	li	$5,-268435456			# 0xfffffffff0000000
	li	$4,-536870912			# 0xffffffffe0000000
	and	$5,$3,$5
	beq	$5,$4,$L121
	li	$4,-1			# 0xffffffffffffffff

	beq	$3,$4,$L121
	lui	$4,%hi(dos_lan_mask)

	lw	$4,%lo(dos_lan_mask)($4)
	nop
	nor	$4,$0,$4
	and	$5,$4,$3
	beq	$4,$5,$L205
	lui	$4,%hi(LogFlag)

$L120:
	andi	$3,$2,0x1000
$L204:
	beq	$3,$0,$L123
	lui	$4,%hi(dos_lan_addr)

	lw	$3,12($16)
	lw	$4,%lo(dos_lan_addr)($4)
	lui	$5,%hi(dos_lan_mask)
	lw	$5,%lo(dos_lan_mask)($5)
	xor	$4,$3,$4
	and	$4,$4,$5
	beq	$4,$0,$L175
	li	$4,1			# 0x1

$L123:
	andi	$2,$2,0x2000
	bne	$2,$0,$L176
	nop

$L125:
	move	$2,$17
	lw	$31,36($sp)
	lw	$19,32($sp)
	lw	$18,28($sp)
	lw	$17,24($sp)
	lw	$16,20($sp)
	j	$31
	addiu	$sp,$sp,40

$L158:
	jal	dos_pkt_locate
	move	$4,$16

	beq	$2,$0,$L206
	lui	$2,%hi(dos_item_global_0)

	lw	$2,%lo(dos_item_global)($18)
	nop
	andi	$3,$2,0x22
	beq	$3,$0,$L38
	move	$17,$0

	lbu	$4,9($16)
	lui	$3,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,6			# 0x6
	beq	$4,$3,$L177
	li	$3,2			# 0x2

	move	$17,$0
$L38:
	andi	$3,$2,0x44
$L212:
	beq	$3,$0,$L207
	andi	$3,$2,0x88

	lbu	$4,9($16)
	lui	$3,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,6			# 0x6
	beq	$4,$3,$L178
	li	$4,1			# 0x1

$L145:
	move	$3,$0
$L41:
	or	$17,$17,$3
$L217:
	andi	$3,$2,0x88
$L207:
	beq	$3,$0,$L208
	andi	$3,$2,0x110

	lbu	$4,9($16)
	lui	$3,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,17			# 0x11
	beq	$4,$3,$L179
	lui	$4,%hi(dosFilter_block)

$L43:
	andi	$3,$2,0x110
$L208:
	beq	$3,$0,$L209
	li	$3,3145728			# 0x300000

	lbu	$3,9($16)
	lui	$4,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($4)
	li	$4,1			# 0x1
	beq	$3,$4,$L180
	lui	$4,%hi(dosFilter_block)

$L46:
	li	$3,3145728			# 0x300000
$L209:
	and	$3,$2,$3
	bne	$3,$0,$L181
	nop

$L49:
	andi	$3,$2,0x200
	bne	$3,$0,$L182
	lui	$4,%hi(cur_p_pkt)

$L53:
	li	$3,4194304			# 0x400000
$L194:
	and	$2,$2,$3
$L216:
	movz	$17,$0,$2
	lui	$2,%hi(dos_item_global_0)
	lw	$2,%lo(dos_item_global_0)($2)
	nop
	andi	$3,$2,0x1
	beq	$3,$0,$L63
	nop

$L159:
	lbu	$4,9($16)
	li	$3,6			# 0x6
	bne	$4,$3,$L63
	nop

	lbu	$3,0($16)
	nop
	andi	$3,$3,0xf
	sll	$3,$3,2
	addu	$3,$16,$3
	lbu	$7,12($3)
	nop
	andi	$7,$7,0xf0
	sra	$7,$7,2
	sltu	$4,$7,21
	bne	$4,$0,$L63
	lui	$8,%hi($L71)

	addiu	$3,$3,20
	addiu	$7,$7,-20
	move	$4,$0
	addiu	$8,$8,%lo($L71)
	li	$11,10			# 0xa
	li	$10,3			# 0x3
	li	$9,4			# 0x4
$L76:
	lbu	$5,0($3)
	nop
	sltu	$6,$5,9
	bne	$6,$0,$L183
	nop

$L65:
	lw	$4,12($16)
	lui	$3,%hi(attack_saddr+100)
	li	$17,-1			# 0xffffffffffffffff
	sw	$4,%lo(attack_saddr+100)($3)
	lw	$4,16($16)
	lui	$3,%hi(attack_daddr+100)
	j	$L63
	sw	$4,%lo(attack_daddr+100)($3)

$L175:
	lbu	$5,9($16)
	nop
	beq	$5,$4,$L123
	lui	$4,%hi(LogFlag)

	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+36)
	sw	$3,%lo(attack_saddr+36)($5)
	lw	$5,16($16)
	ori	$3,$6,0x1000
	sw	$3,%lo(LogFlag)($4)
	andi	$2,$2,0x2000
	lui	$3,%hi(attack_daddr+36)
	sw	$5,%lo(attack_daddr+36)($3)
	beq	$2,$0,$L125
	li	$17,-1			# 0xffffffffffffffff

$L176:
	jal	dos_pkt_locate
	move	$4,$16

	beq	$2,$0,$L125
	lui	$2,%hi(cur_p_pkt)

	lw	$2,%lo(cur_p_pkt)($2)
	lhu	$4,4($16)
	lhu	$3,32($2)
	nop
	beq	$4,$3,$L127
	nop

	bne	$3,$0,$L125
	nop

$L127:
	lhu	$3,6($16)
	nop
	andi	$5,$3,0x3fff
	beq	$5,$0,$L125
	nop

	lhu	$5,34($2)
	sll	$6,$3,3
	andi	$6,$6,0xffff
	slt	$6,$6,$5
	bne	$6,$0,$L128
	nop

	andi	$3,$3,0x2000
	bne	$3,$0,$L129
	nop

	sh	$0,32($2)
	j	$L125
	sh	$0,34($2)

$L168:
	beq	$3,$0,$L102
	sll	$3,$3,3

	lbu	$4,0($16)
	lhu	$5,2($16)
	andi	$4,$4,0xf
	sll	$4,$4,2
	addu	$3,$3,$5
	subu	$3,$3,$4
	li	$4,65536			# 0x10000
	sltu	$3,$3,$4
	bne	$3,$0,$L199
	andi	$3,$2,0x400

	lw	$6,12($16)
	lui	$3,%hi(LogFlag)
	lw	$5,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+44)
	sw	$6,%lo(attack_saddr+44)($4)
	lw	$4,16($16)
	ori	$5,$5,0x4000
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+44)
	sw	$4,%lo(attack_daddr+44)($3)
	j	$L102
	li	$17,-1			# 0xffffffffffffffff

$L121:
	lui	$4,%hi(LogFlag)
$L205:
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+84)
	sw	$3,%lo(attack_saddr+84)($5)
	lw	$5,16($16)
	li	$3,268435456			# 0x10000000
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+84)
	sw	$5,%lo(attack_daddr+84)($3)
	j	$L120
	li	$17,-1			# 0xffffffffffffffff

$L78:
	lhu	$2,2($19)
	li	$3,7			# 0x7
	beq	$2,$3,$L80
	li	$4,17			# 0x11

	beq	$2,$4,$L80
	li	$5,19			# 0x13

	beq	$2,$5,$L80
	nop

	lhu	$2,0($19)
	nop
	beq	$2,$3,$L80
	nop

	beq	$2,$4,$L80
	nop

	bne	$2,$5,$L77
	nop

$L80:
	lw	$2,12($16)
	lw	$3,16($16)
	nop
	bne	$2,$3,$L77
	lui	$3,%hi(attack_saddr+104)

	sw	$2,%lo(attack_saddr+104)($3)
	lw	$3,16($16)
	lui	$2,%hi(attack_daddr+104)
	li	$17,-1			# 0xffffffffffffffff
	j	$L77
	sw	$3,%lo(attack_daddr+104)($2)

$L160:
	lbu	$4,9($16)
	nop
	beq	$4,$5,$L184
	nop

	li	$3,17			# 0x11
	beq	$4,$3,$L84
	nop

	li	$3,6			# 0x6
	bne	$4,$3,$L81
	li	$4,20			# 0x14

	lbu	$3,0($16)
	lhu	$5,2($16)
	andi	$3,$3,0xf
	sll	$3,$3,2
	subu	$3,$5,$3
$L130:
	slt	$3,$3,$4
	beq	$3,$0,$L210
	li	$3,1073741824			# 0x40000000

	lw	$5,12($16)
	lui	$3,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+88)
	sw	$5,%lo(attack_saddr+88)($4)
	lw	$4,16($16)
	li	$5,536870912			# 0x20000000
	or	$5,$6,$5
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+88)
	sw	$4,%lo(attack_daddr+88)($3)
	j	$L81
	li	$17,-1			# 0xffffffffffffffff

$L162:
	lbu	$3,0($16)
	lbu	$4,9($16)
	andi	$5,$3,0xf
	li	$3,1			# 0x1
	bne	$4,$3,$L211
	andi	$3,$2,0x8000

	sll	$5,$5,2
	addu	$5,$16,$5
	lbu	$3,0($5)
	nop
	bne	$3,$0,$L211
	andi	$3,$2,0x8000

	lui	$3,%hi(icmp_echo_request)
	lw	$3,%lo(icmp_echo_request)($3)
	nop
	bne	$3,$0,$L211
	andi	$3,$2,0x8000

	lw	$5,12($16)
	lui	$3,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+96)
	sw	$5,%lo(attack_saddr+96)($4)
	lw	$4,16($16)
	li	$5,-2147483648			# 0xffffffff80000000
	or	$5,$6,$5
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+96)
	sw	$4,%lo(attack_daddr+96)($3)
	j	$L88
	li	$17,-1			# 0xffffffffffffffff

$L161:
	sll	$6,$6,2
	addu	$6,$16,$6
	lbu	$5,0($6)
	nop
	bne	$5,$4,$L86
	lui	$4,%hi(LogFlag)

	lw	$7,12($16)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+92)
	sw	$7,%lo(attack_saddr+92)($5)
	lw	$5,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+92)
	sw	$5,%lo(attack_daddr+92)($3)
	j	$L86
	li	$17,-1			# 0xffffffffffffffff

$L164:
	lbu	$5,13($19)
	nop
	andi	$5,$5,0x3f
	bne	$5,$4,$L92
	nop

	lbu	$5,0($16)
	lw	$4,12($19)
	lhu	$6,2($16)
	andi	$5,$5,0xf
	sll	$5,$5,2
	srl	$4,$4,28
	subu	$5,$6,$5
	sll	$4,$4,2
	beq	$5,$4,$L94
	nop

$L157:
	lw	$7,12($16)
	lui	$4,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+52)
	sw	$7,%lo(attack_saddr+52)($5)
	lw	$5,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+52)
	sw	$5,%lo(attack_daddr+52)($3)
	j	$L92
	li	$17,-1			# 0xffffffffffffffff

$L165:
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+32)
	sw	$3,%lo(attack_saddr+32)($5)
	lw	$5,16($16)
	ori	$3,$6,0x800
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+32)
	sw	$5,%lo(attack_daddr+32)($3)
	j	$L95
	li	$17,-1			# 0xffffffffffffffff

$L166:
	lhu	$4,2($19)
	nop
	beq	$4,$5,$L99
	nop

	beq	$4,$3,$L99
	nop

	li	$6,19			# 0x13
	beq	$4,$6,$L99
	nop

	lhu	$4,0($19)
	nop
	beq	$4,$5,$L99
	nop

	beq	$4,$3,$L99
	nop

	bne	$4,$6,$L198
	li	$3,131072			# 0x20000

$L99:
	lw	$5,12($16)
	lui	$3,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+60)
	sw	$5,%lo(attack_saddr+60)($4)
	lw	$4,16($16)
	li	$5,262144			# 0x40000
	or	$5,$6,$5
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+60)
	sw	$4,%lo(attack_daddr+60)($3)
	j	$L97
	li	$17,-1			# 0xffffffffffffffff

$L167:
	lhu	$4,6($16)
	nop
	andi	$4,$4,0x3fff
	bne	$4,$0,$L100
	nop

	lbu	$4,0($16)
	lhu	$6,2($16)
	lhu	$5,4($19)
	andi	$4,$4,0xf
	sll	$4,$4,2
	subu	$4,$6,$4
	slt	$4,$5,$4
	beq	$4,$0,$L100
	lui	$4,%hi(LogFlag)

	lw	$7,12($16)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+56)
	sw	$7,%lo(attack_saddr+56)($5)
	lw	$5,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+56)
	sw	$5,%lo(attack_daddr+56)($3)
	j	$L100
	li	$17,-1			# 0xffffffffffffffff

$L169:
	sll	$5,$5,2
	addu	$5,$16,$5
	lbu	$4,0($5)
	nop
	bne	$4,$3,$L200
	li	$3,524288			# 0x80000

	lui	$4,%hi(dos_lan_addr)
	lw	$3,12($16)
	lw	$4,%lo(dos_lan_addr)($4)
	lui	$5,%hi(dos_lan_mask)
	lw	$5,%lo(dos_lan_mask)($5)
	xor	$4,$3,$4
	and	$4,$4,$5
	bne	$4,$0,$L104
	lui	$4,%hi(LogFlag)

	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+28)
	sw	$3,%lo(attack_saddr+28)($5)
	lw	$5,16($16)
	ori	$3,$6,0x400
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+28)
	sw	$5,%lo(attack_daddr+28)($3)
	j	$L104
	li	$17,-1			# 0xffffffffffffffff

$L174:
	lbu	$4,13($19)
	nop
	andi	$4,$4,0x4
	beq	$4,$0,$L118
	lui	$4,%hi(LogFlag)

	lw	$7,12($16)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+80)
	sw	$7,%lo(attack_saddr+80)($5)
	lw	$5,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+80)
	sw	$5,%lo(attack_daddr+80)($3)
	j	$L118
	li	$17,-1			# 0xffffffffffffffff

$L171:
	sll	$5,$5,2
	addu	$5,$16,$5
	lbu	$3,0($5)
	nop
	beq	$3,$4,$L110
	nop

	bne	$3,$0,$L201
	li	$3,33554432			# 0x2000000

$L110:
	lw	$3,12($16)
	li	$5,-268435456			# 0xfffffffff0000000
	li	$4,-536870912			# 0xffffffffe0000000
	and	$5,$3,$5
	beq	$5,$4,$L111
	li	$4,-1			# 0xffffffffffffffff

	beq	$3,$4,$L111
	lui	$4,%hi(dos_lan_mask)

	lw	$4,%lo(dos_lan_mask)($4)
	nop
	nor	$4,$0,$4
	and	$5,$4,$3
	bne	$4,$5,$L108
	nop

$L111:
	lui	$4,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+68)
	sw	$3,%lo(attack_saddr+68)($5)
	lw	$5,16($16)
	li	$3,16777216			# 0x1000000
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+68)
	sw	$5,%lo(attack_daddr+68)($3)
	j	$L108
	li	$17,-1			# 0xffffffffffffffff

$L172:
	lhu	$3,0($19)
	nop
	beq	$3,$4,$L114
	li	$4,19			# 0x13

	beq	$3,$4,$L114
	li	$4,135			# 0x87

	bne	$3,$4,$L202
	li	$3,67108864			# 0x4000000

$L114:
	lhu	$4,2($19)
	li	$3,135			# 0x87
	bne	$4,$3,$L202
	li	$3,67108864			# 0x4000000

	lw	$5,12($16)
	lui	$3,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+72)
	sw	$5,%lo(attack_saddr+72)($4)
	lw	$4,16($16)
	li	$5,33554432			# 0x2000000
	or	$5,$6,$5
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+72)
	sw	$4,%lo(attack_daddr+72)($3)
	j	$L112
	li	$17,-1			# 0xffffffffffffffff

$L170:
	lw	$4,12($16)
	lw	$5,%lo(dos_lan_addr)($5)
	lui	$6,%hi(dos_lan_mask)
	lw	$6,%lo(dos_lan_mask)($6)
	xor	$5,$4,$5
	and	$5,$5,$6
	bne	$5,$0,$L106
	lui	$5,%hi(LogFlag)

	lw	$6,%lo(LogFlag)($5)
	lui	$7,%hi(attack_saddr+64)
	sw	$4,%lo(attack_saddr+64)($7)
	lw	$4,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($5)
	lui	$3,%hi(attack_daddr+64)
	sw	$4,%lo(attack_daddr+64)($3)
	j	$L106
	li	$17,-1			# 0xffffffffffffffff

$L173:
	lbu	$3,13($19)
	nop
	andi	$4,$3,0x17
	beq	$4,$0,$L117
	li	$4,3			# 0x3

	andi	$5,$3,0x3
	beq	$5,$4,$L117
	andi	$3,$3,0x11

	li	$4,1			# 0x1
	bne	$3,$4,$L203
	li	$3,134217728			# 0x8000000

$L117:
	lw	$5,12($16)
	lui	$3,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+76)
	sw	$5,%lo(attack_saddr+76)($4)
	lw	$4,16($16)
	li	$5,67108864			# 0x4000000
	or	$5,$6,$5
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+76)
	sw	$4,%lo(attack_daddr+76)($3)
	j	$L115
	li	$17,-1			# 0xffffffffffffffff

$L163:
	lbu	$4,13($19)
	addiu	$3,$3,%lo(_tcpDosScanBitmap)
	andi	$4,$4,0x3f
	addu	$3,$4,$3
	lb	$3,0($3)
	nop
	beq	$3,$0,$L196
	li	$3,65536			# 0x10000

	lw	$6,12($16)
	lui	$3,%hi(LogFlag)
	lw	$5,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+48)
	sw	$6,%lo(attack_saddr+48)($4)
	lw	$4,16($16)
	ori	$5,$5,0x8000
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+48)
	sw	$4,%lo(attack_daddr+48)($3)
	j	$L90
	li	$17,-1			# 0xffffffffffffffff

$L84:
	lbu	$3,0($16)
	lhu	$5,2($16)
	li	$4,8			# 0x8
	andi	$3,$3,0xf
	sll	$3,$3,2
	j	$L130
	subu	$3,$5,$3

$L183:
	sll	$5,$5,2
	addu	$5,$8,$5
	lw	$5,0($5)
	nop
	j	$5
	nop

	.section	.rodata.dos_filter_ipv4__action,"a",@progbits
	.align	2
	.align	2
$L71:
	.word	$L67
	.word	$L67
	.word	$L68
	.word	$L69
	.word	$L65
	.word	$L65
	.word	$L65
	.word	$L65
	.word	$L70
	.section	.text.dos_filter_ipv4__action
$L67:
	addiu	$3,$3,1
	addiu	$4,$4,1
$L72:
	sltu	$5,$4,$7
	bne	$5,$0,$L76
	nop

	j	$L193
	andi	$2,$2,0x2

$L70:
	lbu	$5,1($3)
	nop
	bne	$5,$11,$L65
	addiu	$3,$3,10

	j	$L72
	addiu	$4,$4,10

$L69:
	lbu	$5,1($3)
	nop
	bne	$5,$10,$L65
	addiu	$3,$3,3

	j	$L72
	addiu	$4,$4,3

$L68:
	lbu	$5,1($3)
	nop
	bne	$5,$9,$L65
	addiu	$3,$3,4

	j	$L72
	addiu	$4,$4,4

$L184:
	lhu	$4,6($16)
	nop
	andi	$4,$4,0x3fff
	bne	$4,$0,$L81
	nop

	lbu	$4,0($16)
	lhu	$5,2($16)
	andi	$4,$4,0xf
	sll	$4,$4,2
	subu	$4,$5,$4
	slt	$4,$4,4
	beq	$4,$0,$L81
	lui	$4,%hi(LogFlag)

	lw	$7,12($16)
	lw	$6,%lo(LogFlag)($4)
	lui	$5,%hi(attack_saddr+88)
	sw	$7,%lo(attack_saddr+88)($5)
	lw	$5,16($16)
	or	$3,$6,$3
	sw	$3,%lo(LogFlag)($4)
	lui	$3,%hi(attack_daddr+88)
	sw	$5,%lo(attack_daddr+88)($3)
	j	$L81
	li	$17,-1			# 0xffffffffffffffff

$L129:
	sh	$4,32($2)
	lbu	$3,0($16)
	lhu	$4,2($16)
	andi	$3,$3,0xf
	addu	$5,$5,$4
	sll	$3,$3,2
	subu	$5,$5,$3
	j	$L125
	sh	$5,34($2)

$L177:
	lbu	$4,13($19)
	nop
	andi	$4,$4,0x3f
	bne	$4,$3,$L38
	move	$17,$0

	lui	$4,%hi(dosFilter_block)
	lw	$6,%lo(dosFilter_block)($4)
	li	$3,1			# 0x1
	beq	$6,$3,$L185
	lui	$3,%hi(attack_saddr+4)

$L39:
	lw	$3,4($5)
	lui	$6,%hi(per_syn_threshold)
	lw	$6,%lo(per_syn_threshold)($6)
	addiu	$3,$3,1
	sw	$3,4($5)
	slt	$3,$6,$3
	beq	$3,$0,$L38
	move	$17,$0

	andi	$3,$2,0x20
	beq	$3,$0,$L212
	andi	$3,$2,0x44

	blez	$6,$L38
	lui	$6,%hi(attack_saddr+4)

	lw	$7,12($16)
	lui	$3,%hi(LogFlag)
	sw	$7,%lo(attack_saddr+4)($6)
	lw	$5,%lo(LogFlag)($3)
	lw	$7,16($16)
	lui	$6,%hi(attack_daddr+4)
	ori	$5,$5,0x20
	sw	$7,%lo(attack_daddr+4)($6)
	li	$6,1			# 0x1
	sw	$6,%lo(dosFilter_block)($4)
	j	$L38
	sw	$5,%lo(LogFlag)($3)

$L178:
	lbu	$3,13($19)
	nop
	andi	$3,$3,0x3f
	bne	$3,$4,$L145
	lui	$4,%hi(dosFilter_block)

	lw	$6,%lo(dosFilter_block)($4)
	nop
	beq	$6,$3,$L186
	lui	$3,%hi(attack_saddr+8)

$L42:
	lw	$3,8($5)
	lui	$6,%hi(per_fin_threshold)
	lw	$6,%lo(per_fin_threshold)($6)
	addiu	$3,$3,1
	sw	$3,8($5)
	slt	$3,$6,$3
	beq	$3,$0,$L41
	move	$3,$0

	andi	$3,$2,0x40
	beq	$3,$0,$L145
	nop

	blez	$6,$L145
	lui	$6,%hi(attack_saddr+8)

	lw	$7,12($16)
	lui	$3,%hi(LogFlag)
	sw	$7,%lo(attack_saddr+8)($6)
	lw	$5,%lo(LogFlag)($3)
	lw	$7,16($16)
	lui	$6,%hi(attack_daddr+8)
	ori	$5,$5,0x40
	sw	$7,%lo(attack_daddr+8)($6)
	li	$6,1			# 0x1
	sw	$5,%lo(LogFlag)($3)
	sw	$6,%lo(dosFilter_block)($4)
	j	$L41
	move	$3,$0

$L182:
	lbu	$3,9($16)
	li	$5,6			# 0x6
	lw	$4,%lo(cur_p_pkt)($4)
	beq	$3,$5,$L54
	move	$6,$3

	lw	$5,24($4)
$L55:
	li	$6,17			# 0x11
$L195:
	beq	$3,$6,$L187
	nop

$L59:
	li	$3,8388608			# 0x800000
$L215:
	and	$3,$2,$3
	beq	$3,$0,$L62
	nop

	slt	$5,$5,11
	bne	$5,$0,$L194
	li	$3,4194304			# 0x400000

$L156:
	lw	$6,12($16)
	lui	$3,%hi(LogFlag)
	lw	$5,%lo(LogFlag)($3)
	lui	$4,%hi(attack_saddr+24)
	sw	$6,%lo(attack_saddr+24)($4)
	lw	$4,16($16)
	ori	$5,$5,0x200
	sw	$5,%lo(LogFlag)($3)
	lui	$3,%hi(attack_daddr+24)
	j	$L53
	sw	$4,%lo(attack_daddr+24)($3)

$L181:
	lbu	$3,0($16)
	lbu	$4,9($16)
	andi	$6,$3,0xf
	lui	$3,%hi(cur_p_pkt)
	lw	$5,%lo(cur_p_pkt)($3)
	li	$3,1			# 0x1
	beq	$4,$3,$L188
	li	$4,8			# 0x8

$L153:
	move	$3,$0
$L214:
	j	$L49
	or	$17,$17,$3

$L188:
	sll	$6,$6,2
	addu	$6,$16,$6
	lbu	$3,0($6)
	nop
	beq	$3,$4,$L213
	lui	$4,%hi(dosFilter_block)

	bne	$3,$0,$L214
	move	$3,$0

$L213:
	lw	$6,%lo(dosFilter_block)($4)
	li	$3,1			# 0x1
	beq	$6,$3,$L189
	lui	$3,%hi(attack_saddr+20)

$L52:
	lw	$3,20($5)
	lui	$6,%hi(per_ping_threshold)
	lw	$6,%lo(per_ping_threshold)($6)
	addiu	$3,$3,1
	sw	$3,20($5)
	slt	$3,$6,$3
	beq	$3,$0,$L153
	li	$3,2097152			# 0x200000

	and	$5,$2,$3
	beq	$5,$0,$L153
	nop

	blez	$6,$L153
	lui	$7,%hi(attack_saddr+20)

	lw	$8,12($16)
	lui	$5,%hi(LogFlag)
	lw	$6,%lo(LogFlag)($5)
	sw	$8,%lo(attack_saddr+20)($7)
	lw	$7,16($16)
	or	$3,$6,$3
	lui	$6,%hi(attack_daddr+20)
	sw	$7,%lo(attack_daddr+20)($6)
	sw	$3,%lo(LogFlag)($5)
	li	$6,1			# 0x1
	move	$3,$0
	sw	$6,%lo(dosFilter_block)($4)
	j	$L49
	or	$17,$17,$3

$L62:
	slt	$5,$5,201
	beq	$5,$0,$L156
	li	$3,4194304			# 0x400000

	j	$L216
	and	$2,$2,$3

$L179:
	lw	$6,%lo(dosFilter_block)($4)
	li	$3,1			# 0x1
	beq	$6,$3,$L190
	lui	$3,%hi(attack_saddr+12)

$L45:
	lw	$3,12($5)
	lui	$6,%hi(per_udp_threshold)
	lw	$6,%lo(per_udp_threshold)($6)
	addiu	$3,$3,1
	sw	$3,12($5)
	slt	$3,$6,$3
	beq	$3,$0,$L208
	andi	$3,$2,0x110

	andi	$3,$2,0x80
	beq	$3,$0,$L208
	andi	$3,$2,0x110

	blez	$6,$L208
	lui	$6,%hi(attack_saddr+12)

	lw	$7,12($16)
	lui	$3,%hi(LogFlag)
	sw	$7,%lo(attack_saddr+12)($6)
	lw	$5,%lo(LogFlag)($3)
	lw	$7,16($16)
	lui	$6,%hi(attack_daddr+12)
	ori	$5,$5,0x80
	sw	$7,%lo(attack_daddr+12)($6)
	li	$6,1			# 0x1
	sw	$6,%lo(dosFilter_block)($4)
	j	$L43
	sw	$5,%lo(LogFlag)($3)

$L180:
	lw	$6,%lo(dosFilter_block)($4)
	nop
	beq	$6,$3,$L191
	lui	$3,%hi(attack_saddr+16)

$L48:
	lw	$3,16($5)
	lui	$6,%hi(per_icmp_threshold)
	lw	$6,%lo(per_icmp_threshold)($6)
	addiu	$3,$3,1
	sw	$3,16($5)
	slt	$3,$6,$3
	beq	$3,$0,$L209
	li	$3,3145728			# 0x300000

	andi	$3,$2,0x100
	beq	$3,$0,$L209
	li	$3,3145728			# 0x300000

	blez	$6,$L209
	lui	$6,%hi(attack_saddr+16)

	lw	$7,12($16)
	lui	$3,%hi(LogFlag)
	sw	$7,%lo(attack_saddr+16)($6)
	lw	$5,%lo(LogFlag)($3)
	lw	$7,16($16)
	lui	$6,%hi(attack_daddr+16)
	ori	$5,$5,0x100
	sw	$7,%lo(attack_daddr+16)($6)
	li	$6,1			# 0x1
	sw	$6,%lo(dosFilter_block)($4)
	j	$L46
	sw	$5,%lo(LogFlag)($3)

$L187:
	lhu	$3,36($4)
	nop
	beq	$3,$0,$L60
	nop

	lhu	$6,2($19)
	nop
	beq	$6,$3,$L215
	li	$3,8388608			# 0x800000

	addiu	$5,$5,1
	j	$L215
	sw	$5,24($4)

$L54:
	lhu	$3,36($4)
	nop
	beq	$3,$0,$L56
	nop

	lhu	$5,2($19)
	nop
	beq	$5,$3,$L192
	nop

	lw	$5,24($4)
	nop
	addiu	$5,$5,1
	sw	$5,24($4)
	lbu	$6,9($16)
	j	$L55
	move	$3,$6

$L94:
	lhu	$4,6($16)
	nop
	andi	$4,$4,0x2000
	beq	$4,$0,$L92
	nop

	j	$L157
	nop

$L56:
	lhu	$3,2($19)
	lw	$5,24($4)
	sh	$3,36($4)
	lbu	$3,9($16)
	j	$L195
	li	$6,17			# 0x11

$L60:
	lhu	$3,2($19)
	j	$L59
	sh	$3,36($4)

$L191:
	lw	$6,%lo(attack_saddr+16)($3)
	lw	$3,12($16)
	nop
	bne	$6,$3,$L48
	nop

	j	$L46
	li	$17,-1			# 0xffffffffffffffff

$L190:
	lw	$6,%lo(attack_saddr+12)($3)
	lw	$3,12($16)
	nop
	bne	$6,$3,$L45
	nop

	j	$L43
	li	$17,-1			# 0xffffffffffffffff

$L128:
	lw	$5,12($16)
	lui	$2,%hi(LogFlag)
	lw	$4,%lo(LogFlag)($2)
	lui	$3,%hi(attack_saddr+40)
	sw	$5,%lo(attack_saddr+40)($3)
	lw	$3,16($16)
	ori	$4,$4,0x2000
	sw	$4,%lo(LogFlag)($2)
	lui	$2,%hi(attack_daddr+40)
	sw	$3,%lo(attack_daddr+40)($2)
	j	$L125
	li	$17,-1			# 0xffffffffffffffff

$L189:
	lw	$6,%lo(attack_saddr+20)($3)
	lw	$3,12($16)
	nop
	bne	$6,$3,$L52
	li	$3,-1			# 0xffffffffffffffff

	j	$L49
	or	$17,$17,$3

$L192:
	lw	$5,24($4)
	j	$L55
	move	$3,$6

$L185:
	lw	$6,%lo(attack_saddr+4)($3)
	lw	$3,12($16)
	nop
	bne	$6,$3,$L39
	li	$17,-1			# 0xffffffffffffffff

	j	$L212
	andi	$3,$2,0x44

$L186:
	lw	$6,%lo(attack_saddr+8)($3)
	lw	$3,12($16)
	nop
	bne	$6,$3,$L42
	li	$3,-1			# 0xffffffffffffffff

	j	$L217
	or	$17,$17,$3

	.set	macro
	.set	reorder
	.end	dos_filter_ipv4__action
	.size	dos_filter_ipv4__action, .-dos_filter_ipv4__action
	.section	.rodata.str1.4
	.align	2
$LC6:
	.ascii	"DoS: Whole System SYN Flood Attack\012\000"
	.align	2
$LC7:
	.ascii	"DoS: Whole System FIN Flood Attack\012\000"
	.align	2
$LC8:
	.ascii	"DoS: Whole System UDP Flood Attack\012\000"
	.align	2
$LC9:
	.ascii	"DoS: Whole System ICMP Flood Attack\012\000"
	.align	2
$LC10:
	.ascii	"DoS: Per-source SYN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC11:
	.ascii	"DoS: Per-source FIN Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC12:
	.ascii	"DoS: Per-source UDP Flood Attack source=%u.%u.%u.%u dest"
	.ascii	"ination=%u.%u.%u.%u\012\000"
	.align	2
$LC13:
	.ascii	"DoS: Per-source ICMP Flood Attack source=%u.%u.%u.%u des"
	.ascii	"tination=%u.%u.%u.%u\012\000"
	.align	2
$LC14:
	.ascii	"DoS: Port Scan Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC15:
	.ascii	"DoS: Tcp Scan Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.align	2
$LC16:
	.ascii	"DoS: Tcp SYN With Data Attack source=%u.%u.%u.%u destina"
	.ascii	"tion=%u.%u.%u.%u\012\000"
	.align	2
$LC17:
	.ascii	"DoS: IP Land Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC18:
	.ascii	"DoS: UdpEchoChargen Attack source=%u.%u.%u.%u destinatio"
	.ascii	"n=%u.%u.%u.%u\012\000"
	.align	2
$LC19:
	.ascii	"DoS: UdpBomb Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC20:
	.ascii	"DoS: PingOfDeath Attack source=%u.%u.%u.%u destination=%"
	.ascii	"u.%u.%u.%u\012\000"
	.align	2
$LC21:
	.ascii	"DoS: IcmpSmurf Attack source=%u.%u.%u.%u destination=%u."
	.ascii	"%u.%u.%u\012\000"
	.align	2
$LC22:
	.ascii	"DoS: IpSpoof Attack source=%u.%u.%u.%u destination=%u.%u"
	.ascii	".%u.%u\012\000"
	.align	2
$LC23:
	.ascii	"DoS: TearDrop Attack source=%u.%u.%u.%u destination=%u.%"
	.ascii	"u.%u.%u\012\000"
	.section	.text.dos_timer_ipv4_handler,"ax",@progbits
	.align	2
	.globl	dos_timer_ipv4_handler
	.set	nomips16
	.ent	dos_timer_ipv4_handler
	.type	dos_timer_ipv4_handler, @function
dos_timer_ipv4_handler:
	.frame	$sp,56,$31		# vars= 0, regs= 4/0, args= 40, gp= 0
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-56
	sw	$31,52($sp)
	sw	$18,48($sp)
	sw	$17,44($sp)
	sw	$16,40($sp)
	lui	$15,%hi(dos_item_global)
	lw	$16,%lo(dos_item_global)($15)
	nop
	andi	$2,$16,0x1e
	beq	$2,$0,$L256
	lui	$18,%hi(dos_pkt)

	addiu	$2,$18,%lo(dos_pkt)
	li	$5,40960			# 0xa000
	addu	$5,$2,$5
	move	$6,$0
	move	$7,$0
	move	$8,$0
	move	$9,$0
	li	$4,1			# 0x1
	andi	$13,$16,0x2
	andi	$12,$16,0x4
	andi	$11,$16,0x8
	j	$L225
	andi	$10,$16,0x10

$L221:
	addiu	$2,$2,40
	beq	$2,$5,$L257
	nop

$L225:
	lb	$3,0($2)
	nop
	bne	$3,$4,$L221
	nop

	beq	$13,$0,$L222
	nop

	lw	$3,4($2)
	nop
	addu	$14,$9,$3
	slt	$3,$0,$3
	movn	$9,$14,$3
$L222:
	beq	$12,$0,$L223
	nop

	lw	$3,8($2)
	nop
	addu	$14,$8,$3
	slt	$3,$0,$3
	movn	$8,$14,$3
$L223:
	beq	$11,$0,$L224
	nop

	lw	$3,12($2)
	nop
	addu	$14,$7,$3
	slt	$3,$0,$3
	movn	$7,$14,$3
$L224:
	beq	$10,$0,$L221
	nop

	lw	$3,16($2)
	addiu	$2,$2,40
	addu	$14,$6,$3
	slt	$3,$0,$3
	bne	$2,$5,$L225
	movn	$6,$14,$3

$L257:
	lui	$2,%hi(whole_syn_threshold)
	lw	$2,%lo(whole_syn_threshold)($2)
	nop
	slt	$9,$2,$9
	bne	$9,$0,$L226
	andi	$16,$16,0x2

	lui	$17,%hi(LogFlag)
$L293:
	lw	$16,%lo(LogFlag)($17)
	lui	$2,%hi(whole_fin_threshold)
$L294:
	lw	$2,%lo(whole_fin_threshold)($2)
	nop
	slt	$8,$2,$8
	beq	$8,$0,$L230
	nop

	lw	$3,%lo(dos_item_global)($15)
	nop
	andi	$3,$3,0x4
	beq	$3,$0,$L230
	nop

	blez	$2,$L277
	lui	$2,%hi(whole_udp_threshold)

	ori	$16,$16,0x4
	sw	$16,%lo(LogFlag)($17)
$L230:
	lui	$2,%hi(whole_udp_threshold)
$L277:
	lw	$2,%lo(whole_udp_threshold)($2)
	nop
	slt	$7,$2,$7
	beq	$7,$0,$L231
	nop

	lw	$3,%lo(dos_item_global)($15)
	nop
	andi	$3,$3,0x8
	bne	$3,$0,$L258
	nop

$L231:
	lui	$2,%hi(whole_icmp_threshold)
$L310:
	lw	$2,%lo(whole_icmp_threshold)($2)
	nop
	slt	$6,$2,$6
	beq	$6,$0,$L220
	nop

	lw	$3,%lo(dos_item_global)($15)
	nop
	andi	$3,$3,0x10
	beq	$3,$0,$L220
	nop

	blez	$2,$L278
	andi	$2,$16,0x2

	ori	$16,$16,0x10
	sw	$16,%lo(LogFlag)($17)
$L220:
	andi	$2,$16,0x2
$L278:
	bne	$2,$0,$L259
	nop

	andi	$2,$16,0x4
$L311:
	bne	$2,$0,$L260
	andi	$2,$16,0x8

$L295:
	bne	$2,$0,$L261
	andi	$2,$16,0x10

$L296:
	bne	$2,$0,$L262
	andi	$2,$16,0x20

$L297:
	bne	$2,$0,$L279
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x40
$L298:
	bne	$2,$0,$L280
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x80
$L299:
	bne	$2,$0,$L281
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x100
$L300:
	bne	$2,$0,$L282
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x200
$L301:
	bne	$2,$0,$L283
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x8000
$L302:
	bne	$2,$0,$L284
	lui	$3,%hi(attack_saddr)

	li	$2,65536			# 0x10000
$L303:
	and	$2,$16,$2
	bne	$2,$0,$L285
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x800
$L304:
	bne	$2,$0,$L286
	lui	$3,%hi(attack_saddr)

	li	$2,262144			# 0x40000
$L305:
	and	$2,$16,$2
	bne	$2,$0,$L287
	lui	$3,%hi(attack_saddr)

	li	$2,131072			# 0x20000
$L306:
	and	$2,$16,$2
	bne	$2,$0,$L288
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x4000
$L307:
	bne	$2,$0,$L289
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x400
$L308:
	bne	$2,$0,$L290
	lui	$3,%hi(attack_saddr)

	andi	$2,$16,0x1000
$L309:
	bne	$2,$0,$L291
	lui	$3,%hi(attack_saddr)

$L248:
	andi	$16,$16,0x2000
$L276:
	beq	$16,$0,$L292
	addiu	$4,$18,%lo(dos_pkt)

	lui	$3,%hi(attack_saddr)
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,40($3)
	lbu	$6,41($3)
	lbu	$7,42($3)
	lbu	$10,43($3)
	lbu	$9,40($2)
	lbu	$8,41($2)
	lbu	$3,42($2)
	lbu	$2,43($2)
	lui	$4,%hi($LC23)
	addiu	$4,$4,%lo($LC23)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	addiu	$4,$18,%lo(dos_pkt)
$L292:
	move	$5,$0
	li	$6,40960			# 0xa000
	jal	memset
	sw	$0,%lo(LogFlag)($17)

	lui	$2,%hi(icmp_echo_request_time)
	lw	$2,%lo(icmp_echo_request_time)($2)
	lui	$3,%hi(jiffies)
	lw	$3,%lo(jiffies)($3)
	addiu	$2,$2,100
	subu	$2,$2,$3
	bgez	$2,$L218
	lui	$2,%hi(icmp_echo_request)

	sw	$0,%lo(icmp_echo_request)($2)
$L218:
	lw	$31,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,56

$L226:
	beq	$16,$0,$L293
	lui	$17,%hi(LogFlag)

	lw	$16,%lo(LogFlag)($17)
	blez	$2,$L294
	lui	$2,%hi(whole_fin_threshold)

	ori	$16,$16,0x2
	j	$L294
	sw	$16,%lo(LogFlag)($17)

$L256:
	lui	$17,%hi(LogFlag)
	lw	$16,%lo(LogFlag)($17)
	nop
	andi	$2,$16,0x2
	beq	$2,$0,$L311
	andi	$2,$16,0x4

$L259:
	lui	$4,%hi($LC6)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC6)

	andi	$2,$16,0x4
	beq	$2,$0,$L295
	andi	$2,$16,0x8

$L260:
	lui	$4,%hi($LC7)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC7)

	andi	$2,$16,0x8
	beq	$2,$0,$L296
	andi	$2,$16,0x10

$L261:
	lui	$4,%hi($LC8)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC8)

	andi	$2,$16,0x10
	beq	$2,$0,$L297
	andi	$2,$16,0x20

$L262:
	lui	$4,%hi($LC9)
	jal	scrlog_printk
	addiu	$4,$4,%lo($LC9)

	andi	$2,$16,0x20
	beq	$2,$0,$L298
	andi	$2,$16,0x40

	lui	$3,%hi(attack_saddr)
$L279:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,4($3)
	lbu	$6,5($3)
	lbu	$7,6($3)
	lbu	$10,7($3)
	lbu	$9,4($2)
	lbu	$8,5($2)
	lbu	$3,6($2)
	lbu	$2,7($2)
	lui	$4,%hi($LC10)
	addiu	$4,$4,%lo($LC10)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x40
	beq	$2,$0,$L299
	andi	$2,$16,0x80

	lui	$3,%hi(attack_saddr)
$L280:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,8($3)
	lbu	$6,9($3)
	lbu	$7,10($3)
	lbu	$10,11($3)
	lbu	$9,8($2)
	lbu	$8,9($2)
	lbu	$3,10($2)
	lbu	$2,11($2)
	lui	$4,%hi($LC11)
	addiu	$4,$4,%lo($LC11)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x80
	beq	$2,$0,$L300
	andi	$2,$16,0x100

	lui	$3,%hi(attack_saddr)
$L281:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,12($3)
	lbu	$6,13($3)
	lbu	$7,14($3)
	lbu	$10,15($3)
	lbu	$9,12($2)
	lbu	$8,13($2)
	lbu	$3,14($2)
	lbu	$2,15($2)
	lui	$4,%hi($LC12)
	addiu	$4,$4,%lo($LC12)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x100
	beq	$2,$0,$L301
	andi	$2,$16,0x200

	lui	$3,%hi(attack_saddr)
$L282:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,16($3)
	lbu	$6,17($3)
	lbu	$7,18($3)
	lbu	$10,19($3)
	lbu	$9,16($2)
	lbu	$8,17($2)
	lbu	$3,18($2)
	lbu	$2,19($2)
	lui	$4,%hi($LC13)
	addiu	$4,$4,%lo($LC13)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x200
	beq	$2,$0,$L302
	andi	$2,$16,0x8000

	lui	$3,%hi(attack_saddr)
$L283:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,24($3)
	lbu	$6,25($3)
	lbu	$7,26($3)
	lbu	$10,27($3)
	lbu	$9,24($2)
	lbu	$8,25($2)
	lbu	$3,26($2)
	lbu	$2,27($2)
	lui	$4,%hi($LC14)
	addiu	$4,$4,%lo($LC14)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x8000
	beq	$2,$0,$L303
	li	$2,65536			# 0x10000

	lui	$3,%hi(attack_saddr)
$L284:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,48($3)
	lbu	$6,49($3)
	lbu	$7,50($3)
	lbu	$10,51($3)
	lbu	$9,48($2)
	lbu	$8,49($2)
	lbu	$3,50($2)
	lbu	$2,51($2)
	lui	$4,%hi($LC15)
	addiu	$4,$4,%lo($LC15)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,65536			# 0x10000
	and	$2,$16,$2
	beq	$2,$0,$L304
	andi	$2,$16,0x800

	lui	$3,%hi(attack_saddr)
$L285:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,52($3)
	lbu	$6,53($3)
	lbu	$7,54($3)
	lbu	$10,55($3)
	lbu	$9,52($2)
	lbu	$8,53($2)
	lbu	$3,54($2)
	lbu	$2,55($2)
	lui	$4,%hi($LC16)
	addiu	$4,$4,%lo($LC16)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x800
	beq	$2,$0,$L305
	li	$2,262144			# 0x40000

	lui	$3,%hi(attack_saddr)
$L286:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,32($3)
	lbu	$6,33($3)
	lbu	$7,34($3)
	lbu	$10,35($3)
	lbu	$9,32($2)
	lbu	$8,33($2)
	lbu	$3,34($2)
	lbu	$2,35($2)
	lui	$4,%hi($LC17)
	addiu	$4,$4,%lo($LC17)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,262144			# 0x40000
	and	$2,$16,$2
	beq	$2,$0,$L306
	li	$2,131072			# 0x20000

	lui	$3,%hi(attack_saddr)
$L287:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,60($3)
	lbu	$6,61($3)
	lbu	$7,62($3)
	lbu	$10,63($3)
	lbu	$9,60($2)
	lbu	$8,61($2)
	lbu	$3,62($2)
	lbu	$2,63($2)
	lui	$4,%hi($LC18)
	addiu	$4,$4,%lo($LC18)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	li	$2,131072			# 0x20000
	and	$2,$16,$2
	beq	$2,$0,$L307
	andi	$2,$16,0x4000

	lui	$3,%hi(attack_saddr)
$L288:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,56($3)
	lbu	$6,57($3)
	lbu	$7,58($3)
	lbu	$10,59($3)
	lbu	$9,56($2)
	lbu	$8,57($2)
	lbu	$3,58($2)
	lbu	$2,59($2)
	lui	$4,%hi($LC19)
	addiu	$4,$4,%lo($LC19)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x4000
	beq	$2,$0,$L308
	andi	$2,$16,0x400

	lui	$3,%hi(attack_saddr)
$L289:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,44($3)
	lbu	$6,45($3)
	lbu	$7,46($3)
	lbu	$10,47($3)
	lbu	$9,44($2)
	lbu	$8,45($2)
	lbu	$3,46($2)
	lbu	$2,47($2)
	lui	$4,%hi($LC20)
	addiu	$4,$4,%lo($LC20)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x400
	beq	$2,$0,$L309
	andi	$2,$16,0x1000

	lui	$3,%hi(attack_saddr)
$L290:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,28($3)
	lbu	$6,29($3)
	lbu	$7,30($3)
	lbu	$10,31($3)
	lbu	$9,28($2)
	lbu	$8,29($2)
	lbu	$3,30($2)
	lbu	$2,31($2)
	lui	$4,%hi($LC21)
	addiu	$4,$4,%lo($LC21)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	andi	$2,$16,0x1000
	beq	$2,$0,$L248
	lui	$3,%hi(attack_saddr)

$L291:
	lui	$2,%hi(attack_daddr)
	addiu	$3,$3,%lo(attack_saddr)
	addiu	$2,$2,%lo(attack_daddr)
	lbu	$5,36($3)
	lbu	$6,37($3)
	lbu	$7,38($3)
	lbu	$10,39($3)
	lbu	$9,36($2)
	lbu	$8,37($2)
	lbu	$3,38($2)
	lbu	$2,39($2)
	lui	$4,%hi($LC22)
	addiu	$4,$4,%lo($LC22)
	sw	$10,16($sp)
	sw	$9,20($sp)
	sw	$8,24($sp)
	sw	$3,28($sp)
	jal	scrlog_printk
	sw	$2,32($sp)

	j	$L276
	andi	$16,$16,0x2000

$L258:
	blez	$2,$L310
	lui	$2,%hi(whole_icmp_threshold)

	ori	$16,$16,0x8
	j	$L310
	sw	$16,%lo(LogFlag)($17)

	.set	macro
	.set	reorder
	.end	dos_timer_ipv4_handler
	.size	dos_timer_ipv4_handler, .-dos_timer_ipv4_handler
	.section	.text.dos_ipv4_init,"ax",@progbits
	.align	2
	.globl	dos_ipv4_init
	.set	nomips16
	.ent	dos_ipv4_init
	.type	dos_ipv4_init, @function
dos_ipv4_init:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	sw	$16,16($sp)
	lui	$16,%hi(_tcpDosScanBitmap)
	addiu	$4,$16,%lo(_tcpDosScanBitmap)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	li	$2,1			# 0x1
	addiu	$3,$16,%lo(_tcpDosScanBitmap)
	sb	$2,%lo(_tcpDosScanBitmap)($16)
	lw	$31,20($sp)
	lw	$16,16($sp)
	sb	$2,63($3)
	sb	$2,58($3)
	sb	$2,41($3)
	sb	$2,40($3)
	sb	$2,33($3)
	sb	$2,32($3)
	sb	$2,9($3)
	sb	$2,8($3)
	sb	$2,3($3)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	dos_ipv4_init
	.size	dos_ipv4_init, .-dos_ipv4_init
	.section	.bss.ip_name0,"aw",@nobits
	.align	2
	.type	ip_name0, @object
	.size	ip_name0, 16
ip_name0:
	.space	16
	.section	.bss.dos_pkt,"aw",@nobits
	.align	2
	.type	dos_pkt, @object
	.size	dos_pkt, 40960
dos_pkt:
	.space	40960
	.section	.bss.attack_daddr,"aw",@nobits
	.align	2
	.type	attack_daddr, @object
	.size	attack_daddr, 108
attack_daddr:
	.space	108
	.section	.bss.attack_saddr,"aw",@nobits
	.align	2
	.type	attack_saddr, @object
	.size	attack_saddr, 108
attack_saddr:
	.space	108
	.section	.rodata.str1.4
	.align	2
$LC24:
	.ascii	"NULL\000"
	.align	2
$LC25:
	.ascii	"PerSynFlood\000"
	.align	2
$LC26:
	.ascii	"PerFinFlood\000"
	.align	2
$LC27:
	.ascii	"PerUdpFlood\000"
	.align	2
$LC28:
	.ascii	"PerIcmpFlood\000"
	.align	2
$LC29:
	.ascii	"PerPingFlood\000"
	.align	2
$LC30:
	.ascii	"TcpUdpPortScan\000"
	.align	2
$LC31:
	.ascii	"IcmpSmurf\000"
	.align	2
$LC32:
	.ascii	"IpLand\000"
	.align	2
$LC33:
	.ascii	"IpSpoof\000"
	.align	2
$LC34:
	.ascii	"TearDrop\000"
	.align	2
$LC35:
	.ascii	"PingOfDeath\000"
	.align	2
$LC36:
	.ascii	"TcpScan\000"
	.align	2
$LC37:
	.ascii	"TcpSynWithData\000"
	.align	2
$LC38:
	.ascii	"UdpBomb\000"
	.align	2
$LC39:
	.ascii	"UdpEchoChargen\000"
	.align	2
$LC40:
	.ascii	"UdpFraggle\000"
	.align	2
$LC41:
	.ascii	"IcmpEchoStorm\000"
	.align	2
$LC42:
	.ascii	"UdpSnorkAttack\000"
	.align	2
$LC43:
	.ascii	"TcpInvalidFlags\000"
	.align	2
$LC44:
	.ascii	"TcpRstAttack\000"
	.align	2
$LC45:
	.ascii	"BcMcSrcIpAttack\000"
	.align	2
$LC46:
	.ascii	"ShortIpPkt\000"
	.align	2
$LC47:
	.ascii	"IcmpPingSweep\000"
	.align	2
$LC48:
	.ascii	"IcmpEchoWithoutReply\000"
	.align	2
$LC49:
	.ascii	"TcpInvalidOptions\000"
	.align	2
$LC50:
	.ascii	"LoopBackEchoChargen\000"
	.align	2
$LC51:
	.ascii	"MaxDosType\000"
	.section	.rodata.rtl_dos_name,"a",@progbits
	.align	2
	.type	rtl_dos_name, @object
	.size	rtl_dos_name, 112
rtl_dos_name:
	.word	$LC24
	.word	$LC25
	.word	$LC26
	.word	$LC27
	.word	$LC28
	.word	$LC29
	.word	$LC30
	.word	$LC31
	.word	$LC32
	.word	$LC33
	.word	$LC34
	.word	$LC35
	.word	$LC36
	.word	$LC37
	.word	$LC38
	.word	$LC39
	.word	$LC40
	.word	$LC41
	.word	$LC42
	.word	$LC43
	.word	$LC44
	.word	$LC45
	.word	$LC46
	.word	$LC47
	.word	$LC48
	.word	$LC49
	.word	$LC50
	.word	$LC51
	.section	.bss.ip_name1,"aw",@nobits
	.align	2
	.type	ip_name1, @object
	.size	ip_name1, 16
ip_name1:
	.space	16
	.section	.bss.icmp_echo_request_time,"aw",@nobits
	.align	2
	.type	icmp_echo_request_time, @object
	.size	icmp_echo_request_time, 4
icmp_echo_request_time:
	.space	4
	.section	.bss.icmp_echo_request,"aw",@nobits
	.align	2
	.type	icmp_echo_request, @object
	.size	icmp_echo_request, 4
icmp_echo_request:
	.space	4
	.section	.bss.cur_p_pkt,"aw",@nobits
	.align	2
	.type	cur_p_pkt, @object
	.size	cur_p_pkt, 4
cur_p_pkt:
	.space	4
	.section	.bss.LogFlag,"aw",@nobits
	.align	2
	.type	LogFlag, @object
	.size	LogFlag, 4
LogFlag:
	.space	4
	.section	.bss._tcpDosScanBitmap,"aw",@nobits
	.align	2
	.type	_tcpDosScanBitmap, @object
	.size	_tcpDosScanBitmap, 64
_tcpDosScanBitmap:
	.space	64
	.ident	"GCC: (Realtek RSDK-4.6.4 Build 2080) 4.6.4"
