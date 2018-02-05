#include "lte-ncm.h"
#include "partialbuf.h"

#define HARDWARE_TYPE_ETHERNET 0x0001

#define PROTOCOL_ETHERNET_SIZE 6
#define PROTOCOL_IP_SIZE 4

#define OP_CODE_REQUEST 0x0001
#define OP_CODE_REPLY 0x0002

#define ARP_PACKET_MIN_LENGTH (14 + 28)


/* ARP packet format */
struct arp_packet
{
	u16 hardware_type;
	u16 protocol_type;
	u8 hardware_size;
	u8 protocol_size;
	u16 op_code;
	u8* sender_mac;
	u8* sender_ip;
	u8* target_mac;
	u8* target_ip;
};

int
parse_arp_packet(
	unsigned char* buf,
	unsigned int len,
	struct ethhdr* eth,
	struct arp_packet* arp
	)
{
	struct partial_buf pb;

	pb_init(&pb, buf, len);

	do {
		if (pb_length(&pb) < ARP_PACKET_MIN_LENGTH)
			break;

		// ethernet header
		pb_get_bytes(&pb, &eth->h_dest[0], ETH_ALEN);
		pb_get_bytes(&pb, &eth->h_source[0], ETH_ALEN);
		eth->h_proto = __be16_to_cpu(pb_get_word(&pb));

		// printk(KERN_INFO "parsing packet length(%d), h_proto(%04x)\n", len, eth->h_proto);

		if (eth->h_proto != ETH_P_ARP)
			break;

		// ARP packet
		arp->hardware_type = __be16_to_cpu(pb_get_word(&pb));
		arp->protocol_type = __be16_to_cpu(pb_get_word(&pb));
		arp->hardware_size = pb_get_byte(&pb);
		arp->protocol_size = pb_get_byte(&pb);
		arp->op_code = __be16_to_cpu(pb_get_word(&pb));
		
		//printk(KERN_INFO "hw_type(%04x), proto_type(%04x), hw_size(%02x), proto_size(%02x), op_code(%04x)\n",
		//	arp->hardware_type, arp->protocol_type,
		//	arp->hardware_size, arp->protocol_size,
		//	arp->op_code);

		if (arp->hardware_type != HARDWARE_TYPE_ETHERNET)
			break;

		if (arp->protocol_type != ETH_P_IP)
			break;

		if (arp->hardware_size != ETH_ALEN)
			break;

		if (arp->protocol_size != 4)
			break;

		if (arp->op_code != OP_CODE_REQUEST && arp->op_code != OP_CODE_REPLY)
			break;


		arp->sender_mac = pb_get_current_address(&pb);
		pb_skip(&pb, ETH_ALEN);

		arp->sender_ip = pb_get_current_address(&pb);
		pb_skip(&pb, 4);

		arp->target_mac = pb_get_current_address(&pb);
		pb_skip(&pb, ETH_ALEN);

		arp->target_ip = pb_get_current_address(&pb);
		pb_skip(&pb, 4);

		return 1;

	} while (0);

	return 0;
}


struct sk_buff*
arp_build_packet(
	struct cdc_ncm_ctx* ctx,
	struct arp_packet* arp,
	u16 opcode,
	u32 sender_ip
	)
{
	struct sk_buff* skb;
	struct partial_buf pb;
	int len;

	len =
		ETH_HLEN + // ethernet header
		sizeof(u16) + // hardware type
		sizeof(u16) + // protocol type
		sizeof(u8) + // hardware size
		sizeof(u8) + // protocol size
		sizeof(u16) + // opcode
		arp->hardware_size + // sender mac
		arp->protocol_size + // sender ip
		arp->hardware_size + // target mac
		arp->protocol_size; // target ip

	skb = dev_alloc_skb(len);
	if (!skb)
		return NULL;

	skb_put(skb, len);

	pb_init(&pb, skb->data, len);

	// ethernet header
	pb_put_bytes(&pb, ctx->netdev->dev_addr, ETH_ALEN);
	pb_put_bytes(&pb, ((dc_net_filter_ctx_t*)((dc_net_filter_ctx_t*)ctx->filter_ctx))->emul_cmn_info.gw_mac_addr, ETH_ALEN);
	pb_put_word(&pb, __cpu_to_be16(ETH_P_ARP));

	// arp
	pb_put_word(&pb, __cpu_to_be16(arp->hardware_type));
	pb_put_word(&pb, __cpu_to_be16(arp->protocol_type));
	pb_put_byte(&pb, arp->hardware_size);
	pb_put_byte(&pb, arp->protocol_size);
	pb_put_word(&pb, __cpu_to_be16(opcode));

	// sender mac/ip
	pb_put_bytes(&pb, ((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_cmn_info.gw_mac_addr, ETH_ALEN);
	pb_put_dword(&pb, sender_ip);

	// target mac/ip
	pb_put_bytes(&pb, ctx->netdev->dev_addr, ETH_ALEN);
	pb_put_dword(&pb, cpu_to_be32(((dc_net_filter_ctx_t*)ctx->filter_ctx)->emul_info.client_ip_addr));

	return skb;
}


int
arp_state_machine(
	struct usbnet* dev,
	struct cdc_ncm_ctx* ctx,
	unsigned char* buf,
	unsigned int len
	)
{
	struct arp_packet arp;
	struct ethhdr eth;
	struct sk_buff* skb = NULL;
	int ret;

	memset(&arp, 0, sizeof(struct arp_packet));


	ret = parse_arp_packet(buf, len, &eth, &arp);
	if (!ret)
		return ret;

	if (arp.op_code != OP_CODE_REQUEST)
		return 0;

#if 0
	if (*(u32*)arp.target_ip != ctx->net_info.gateway)
		return 0;
#endif

	skb = arp_build_packet(ctx, &arp, OP_CODE_REPLY, *(u32*)arp.target_ip);
	if (skb) {
		usbnet_skb_return(dev, skb);
	}
	return 1;
}
