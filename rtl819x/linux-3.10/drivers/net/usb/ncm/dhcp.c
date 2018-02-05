#include "lte-ncm.h"
#include "dhcp.h"
#include "partialbuf.h"
#include "kstype.h"
#include "filter.h"

enum {
	BOOTREQUEST =1,
	BOOTREPLY = 2,
};

enum {
	HTYPE_ETHERNET = 1,
};

#define FLAG_BROADCAST 0x0001

#define DHCP_MAGIC_CODE 0x63825363

struct dhcp_option
{
	__u8 code;
	__u8 len;
	__u8* data;
};


struct dhcp_context
{
	__u8 DA[6];
	__u8 SA[6];
	__u8 opcode;
	__u8 htype;
	__u8 hlen;
	__u32 xid;
	__u16 secs;
	__u16 flags;
	__u32 ciaddr;
	__u32 yiaddr;
	__u32 siaddr;
	__u32 giaddr;
	__u8 chaddr[16];
	__u8 sname[64];
	__u8 file [128];
	struct dhcp_option* options;
};


#define MAX_KNOWN_OPTION 20

// 342 octets
#define DHCP_PACKET_LENGTH (14 + 20 + 8 + 300)



#define DHCP_OPTION_SUBNET_MASK 1
#define DHCP_OPTION_ROUTER 3
#define DHCP_OPTION_DOMAIN_SERVER 6
#define DHCP_OPTION_REQUESTED_IP 50
#define DHCP_OPTION_ADDRESS_TIME 51
#define DHCP_OPTION_MESSAGE_TYPE 53
#define DHCP_OPTION_SERVER_ID 54
#define DHCP_OPTION_END 0xFF



// for DHCP_OPTION_MESSAGE_TYPE
enum {
	DHCPDISCOVER = 1,
	DHCPOFFER,
	DHCPREQUEST,
	DHCPDECLINE,
	DHCPACK,
	DHCPNAK,
	DHCPRELEASE,
	DHCPINFORM,
	DHCPFORCERENEW,
	DHCPLEASEQUERY,
	DHCPLEASEUNASSIGNED,
	DHCPLEASEUNKNOWN,
	DHCPLEASEACTIVE,
};


int
ParseDhcpPacket(
	unsigned char* buffer,
	unsigned int length,
	struct dhcp_context* context,
	struct dhcp_option* options,
	int option_count
	)
{
	struct partial_buf pb;
	__u16 typelen;
	__u16 port;
	__u16 len;
	__u32 magic_code;

	int option_index;
	int valid;

	pb_init(&pb, buffer, length);

	do {
		if (pb_length(&pb) < DHCP_PACKET_LENGTH)
			break;

		// DA
		pb_get_bytes(&pb, &context->DA[0], 6);

		// SA
		pb_get_bytes(&pb, &context->SA[0], 6);

		// TL
		typelen = __be16_to_cpu(pb_get_word(&pb));
		if (typelen != 0x0800)
			break;

		// IP header

		// ignore Version(1), Service(1), Length(2), Identifier(2),
		// Flags(1), Fragment(1), TOL(1)
		pb_skip(&pb, 9);

		// protocol, UDP
		if (pb_get_byte(&pb) != 0x11)
			break;

		// ignore checksum(2), source IP(4), destination IP(4)
		pb_skip(&pb, 10);

		// UDP header

		// source port
		port = __be16_to_cpu(pb_get_word(&pb));
		if (port != 0x0044)
			break;

		// destination port
		port = __be16_to_cpu(pb_get_word(&pb));
		if (port != 0x0043)
			break;

		// length
		len = __be16_to_cpu(pb_get_word(&pb));
		if (len - 8 > pb_length(&pb))
			break;

		// checksum
		pb_skip(&pb, 2);

		// bootstrap protocol

		context->opcode = pb_get_byte(&pb);
		if (context->opcode != BOOTREQUEST)
			break;

		context->htype = pb_get_byte(&pb);
		if (context->htype != HTYPE_ETHERNET)
			break;

		context->hlen = pb_get_byte(&pb);
		if (context->hlen != 6)
			break;

		// hops
		pb_skip(&pb, 1);

		context->xid = __be32_to_cpu(pb_get_dword(&pb));
		context->secs = __be16_to_cpu(pb_get_word(&pb));
		context->flags = __be16_to_cpu(pb_get_word(&pb));

		context->ciaddr = __be32_to_cpu(pb_get_dword(&pb));
		context->yiaddr = __be32_to_cpu(pb_get_dword(&pb));
		context->siaddr = __be32_to_cpu(pb_get_dword(&pb));
		context->giaddr = __be32_to_cpu(pb_get_dword(&pb));

		pb_get_bytes(&pb, context->chaddr, 16);
		pb_get_bytes(&pb, context->sname, 64);
		pb_get_bytes(&pb, context->file, 128);

		magic_code = __be32_to_cpu(pb_get_dword(&pb));
		if (magic_code != DHCP_MAGIC_CODE)
			break;

		// options
		option_index = 0;
		valid = 1;

		while (pb_length(&pb)) {
			options[option_index].code = pb_get_byte(&pb);

			if (options[option_index].code == DHCP_OPTION_END) {
				break;
			}
			else {
				options[option_index].len = pb_get_byte(&pb);

				if (options[option_index].len > pb_length(&pb)) {
					valid = 0;
					break;
				}

				options[option_index].data = (__u8*)pb_get_current_address(&pb);
				pb_skip(&pb, options[option_index].len);

				if (option_index >= option_count) {
					valid = 0;
					break;
				}

				++option_index;
			}
		}

		if (!valid)
			break;

		return 1;

	} while (0);

	return 0;
}

struct dhcp_option*
DhcpFindOption(
	struct dhcp_option* option,
	__u8 code
	)
{
	while (option->code != code && option->code != DHCP_OPTION_END) {
		++option;
	}

	if (option->code == code)
		return option;

	return NULL;
}



__u16 ip_sum_calc(__u16 len, __u16* ip)
{
	long sum = 0;  /* assume 32 bit long, 16 bit short */

	while(len > 1){
		sum += *ip++;
		if(sum & 0x80000000)   /* if high order bit set, fold */
			sum = (sum & 0xFFFF) + (sum >> 16);
		len -= 2;
	}

	if(len)       /* take care of left over byte */
		sum += (unsigned short) *(unsigned char *)ip;

	while(sum>>16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}


#define MAKE_IP(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))


struct sk_buff*
BuildDhcpPacket(
	struct cdc_ncm_ctx* ctx,
	struct dhcp_context* dhcpctx,
	struct dhcp_option* options,
	__u32 destip
	)
{
	struct sk_buff* skb;
	struct partial_buf pb;
	__u32 sourceip = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.gw_ip_addr);
	__u32 yiaddr = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr);
	u16* p;
	__u16* checksum;

	unsigned int Length =
		14 + // Ethernet header
		20 + // IP header
		8 + // UDP header
		1 + // op
		1 + // htype
		1 + // hlen
		1 + // hops
		4 + // xid
		2 + // secs
		2 + // flags
		4 + // ciaddr
		4 + // yiaddr
		4 + // siaddr
		4 + // giaddr
		16 + // chaddr
		64 + // sname
		128 // file
		;
	unsigned int OptionLength = 1 + 4; // DHCP_OPTION_END + magic code
	int i;

	for (i = 0; options[i].code != DHCP_OPTION_END; ++i) {
		OptionLength += (1 + 1 + options[i].len);
	}

	Length += OptionLength;

	skb = dev_alloc_skb(Length);
	if (!skb)
		return NULL;

	skb_put(skb, Length);

	pb_init(&pb, skb->data, Length);

	// Ethernet header
	pb_put_bytes(&pb, ctx->netdev->dev_addr, 6);
	pb_put_bytes(&pb, ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_cmn_info.gw_mac_addr, 6);
	pb_put_word(&pb, __cpu_to_be16(0x0800));

	p = (__u16*)pb_get_current_address(&pb);

	// IP header
	pb_put_byte(&pb, 0x45); // version and length
	pb_put_byte(&pb, 0x00); // TOS
	pb_put_word(&pb, __cpu_to_be16(Length - 14)); // length
	pb_put_word(&pb, 0); // identification
	pb_put_word(&pb, 0); // flags and fragment offset
	pb_put_byte(&pb, 0x80); // TTL
	pb_put_byte(&pb, 0x11); // protocol

	checksum = (__u16*)pb_get_current_address(&pb);
	pb_put_word(&pb, 0); // checksum, calculate later

	pb_put_dword(&pb, sourceip);
	pb_put_dword(&pb, __cpu_to_be32(destip));

	*checksum = ip_sum_calc(20, p);

	// UDP header
	pb_put_word(&pb, __cpu_to_be16(0x43));
	pb_put_word(&pb, __cpu_to_be16(0x44));
	pb_put_word(&pb, __cpu_to_be16(Length - 14 - 20));
	pb_put_word(&pb, 0);

	// BOOTP protocol

	pb_put_byte(&pb, BOOTREPLY); // op
	pb_put_byte(&pb, HTYPE_ETHERNET); // htype
	pb_put_byte(&pb, 6); // hlen
	pb_put_byte(&pb, 0); // hops

	pb_put_dword(&pb, __cpu_to_be32(dhcpctx->xid)); // xid
	pb_put_word(&pb, 0); // secs
	pb_put_word(&pb, 0); // flags
	pb_put_dword(&pb, 0); // ciaddr
	pb_put_dword(&pb, yiaddr); // yiaddr
	pb_put_dword(&pb, 0); // siaddr
	pb_put_dword(&pb, 0); // giaddr
	pb_put_bytes(&pb, ctx->netdev->dev_addr, 6); // chaddr(16)
	pb_fill_bytes(&pb, 0, 10);
	pb_fill_bytes(&pb, 0, 64); // sname(64)
	pb_fill_bytes(&pb, 0, 128); // file(128)

	pb_put_dword(&pb, __cpu_to_be32(DHCP_MAGIC_CODE)); // magic code

	// options
	for (i = 0; options[i].code != DHCP_OPTION_END; ++i) {
		pb_put_byte(&pb, options[i].code);
		pb_put_byte(&pb, options[i].len);
		pb_put_bytes(&pb, options[i].data, options[i].len);
	}

	pb_put_byte(&pb, DHCP_OPTION_END);

	return skb;
}

void
dhcp_option_set(
	struct dhcp_option* opt,
	__u8 code,
	__u8 len,
	__u8* data
	)
{
	opt->code = code;
	opt->len = len;
	opt->data = data;
}


struct sk_buff*
BuildDhcpOffer(
	struct cdc_ncm_ctx* ctx,
	struct dhcp_context* dhcpctx
	)
{
	__u8 MessageType[] = { DHCPOFFER };
	__u8 AddressTime[] = { 0x00, 0x0d, 0x2f, 0x00 };
	__u32 destip = __cpu_to_be32(0xffffffff);
	__u8 dns[8] = {0};
	u32 gateway = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.gw_ip_addr);
	u32 subnet_mask = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.subnet);

	struct dhcp_option options[MAX_KNOWN_OPTION];
	int i = 0;

	memset(&options[0], 0, sizeof(options));

	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_MESSAGE_TYPE, sizeof(MessageType), &MessageType[0]
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_ADDRESS_TIME, sizeof(AddressTime), &AddressTime[0]
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_SERVER_ID, 4, (__u8*)&gateway
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_SUBNET_MASK, 4, (__u8*)&subnet_mask
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_ROUTER, 4, (__u8*)&gateway
		);

	if (((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_1 != 0 || ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_2 != 0) {

		if (((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_1 != 0 && ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_2 != 0) {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_1);
			*(unsigned int*)&dns[4] = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_2);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 8, dns
				);
		}
		else if (((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_1 != 0) {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_1);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 4, dns
				);
		} else {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_2);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 4, dns
				);
		}
	}

	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_END, 0, NULL
		);

	return BuildDhcpPacket(ctx, dhcpctx, options, destip);
}


struct sk_buff*
BuildDhcpAck(
	struct cdc_ncm_ctx* ctx,
	struct dhcp_context* dhcpctx,
	__u8 Type
	)
{
	__u8 MessageType[] = { Type };
	__u8 AddressTime[] = { 0x00, 0x0d, 0x2f, 0x00 };
	__u32 destip = __cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr);
	__u8 dns[8] = {0};
	u32 gateway = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.gw_ip_addr);
	u32 subnet_mask = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.subnet);

	struct dhcp_option options[MAX_KNOWN_OPTION];
	int i = 0;

	memset(&options[0], 0, sizeof(options));

	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_MESSAGE_TYPE, sizeof(MessageType), &MessageType[0]
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_ADDRESS_TIME, sizeof(AddressTime), &AddressTime[0]
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_SERVER_ID, 4, (__u8*)&gateway
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_SUBNET_MASK, 4, (__u8*)&subnet_mask
		);
	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_ROUTER, 4, (__u8*)&gateway
		);

	if (((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_1 != 0 || ((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_2 != 0) {

		if (((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_1 != 0 && ((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_2 != 0) {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_1);
			*(unsigned int*)&dns[4] = cpu_to_be32(((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_2);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 8, dns
				);
		}
		else if (((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_1 != 0) {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_info.dns_ip_addr_1);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 4, dns
				);
		} else {
			*(unsigned int*)&dns[0] = cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.dns_ip_addr_2);
			dhcp_option_set(
				&options[i++],
				DHCP_OPTION_DOMAIN_SERVER, 4, dns
				);
		}
	}

	dhcp_option_set(
		&options[i++],
		DHCP_OPTION_END, 0, NULL
		);

	return BuildDhcpPacket(ctx, dhcpctx, options, destip);
}


int
dhcp_state_machine(
	struct usbnet* dev,
	struct cdc_ncm_ctx* ctx,
	unsigned char* buf,
	unsigned int len
	)
{
	struct dhcp_context dhcpctx;
	struct dhcp_option options[MAX_KNOWN_OPTION];
	struct dhcp_option* option;
	struct dhcp_option* requested_ip_op;
	struct sk_buff* skb = NULL;
	int bret;

	memset(&dhcpctx, 0, sizeof(dhcpctx));
	memset(&options[0], 0, sizeof(options));

	bret = ParseDhcpPacket(
		buf, len, &dhcpctx, options, MAX_KNOWN_OPTION
		);
	if (!bret)
		return 0;

	dhcpctx.options = &options[0];

	option = DhcpFindOption(options, DHCP_OPTION_MESSAGE_TYPE);
	if (!option || option->len != 1)
		return 0;

	requested_ip_op = DhcpFindOption(options, DHCP_OPTION_REQUESTED_IP);
	if (requested_ip_op) {
		printk(KERN_INFO "Found requested_ip option, IP:" IPSTR "\n",
			   IP2STR(requested_ip_op->data));
	}

	printk(KERN_INFO DRV_NAME" %s: Handle DHCP message type: %02d\n",__func__,
		   *(__u8*)option->data);

	printk(KERN_INFO DRV_NAME" %s: Client IP: " IPSTR "\n",__func__,
		   IP2STR((__u8*)&((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr));

	switch (*(__u8*)option->data) {
	case DHCPDISCOVER:
		printk(KERN_INFO "indicate DHCP discover packet\n");
		skb = BuildDhcpOffer(ctx, &dhcpctx);
		break;

	case DHCPREQUEST:
	case DHCPINFORM:
		if (dhcpctx.ciaddr != 0 &&
			dhcpctx.ciaddr != ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr)
		{
			printk(KERN_INFO "indicate DHCP NAK packet\n");
			skb = BuildDhcpAck(ctx, &dhcpctx, DHCPNAK);
			break;
		}

		if (requested_ip_op) {
			u32 request_ip = be32_to_cpu(*(u32*)requested_ip_op->data);
			printk(KERN_INFO "requested ip: " IPSTR "\n", IP2STR((u8*)&request_ip));

			if (request_ip != ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr) {
				printk(KERN_INFO "indicate DHCP NAK packet\n");
				skb = BuildDhcpAck(ctx, &dhcpctx, DHCPNAK);
				break;
			}
		}

		printk(KERN_INFO "indicate DHCP ACK packet\n");
		skb = BuildDhcpAck(ctx, &dhcpctx, DHCPACK);
		// ctx->net_info_valid = 0;
		break;

	default:
		break;
	}

	if (skb) {
		usbnet_skb_return(dev, skb);
		return 1;
	}

	return 0;
}

