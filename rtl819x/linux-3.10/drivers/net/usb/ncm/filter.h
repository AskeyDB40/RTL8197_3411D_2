#ifndef filter_h__
#define filter_h__

#include "lte-ncm.h"
#include "kstype.h"


#define CONFIG_DC_NET_ARP_EMULATION
#define CONFIG_DC_NET_DHCP_EMULATION
#define CONFIG_DC_NET_DHCPV6_EMULATION
#define CONFIG_DC_NET_FILTER_TX
#define CONFIG_DC_NET_FILTER_RX
//#define CONFIG_DC_NET_DHCPV6_ADDRESS_EMULATION
//#define CONFIG_DC_NET_DUMP_RX_PACKET
//#define CONFIG_DC_NET_DUMP_TX_PACKET


//-----------------------------------------------------------------------------
// icmpv6

typedef enum
{
	ICMPV6_MSG_TYPE_RS =                133,
	ICMPV6_MSG_TYPE_RA =                134,
	ICMPV6_MSG_TYPE_NS =                135,
	ICMPV6_MSG_TYPE_NA =                136,
	ICMPV6_MSG_TYPE_REDIRECT =          137
} icmpv6_msg_t;

#define ICMPV6_HOP_LIMIT                0xFF

/* Router advertisement flags */
#define ICMPV6_RA_FLAG_MANAGED          0x80
#define ICMPV6_RA_FLAG_OTHER            0x40

/* Neighbor advertisement flags */
#define ICMPV6_NA_FLAG_ROUTER           0x80000000
#define ICMPV6_NA_FLAG_SOLICITED        0x40000000
#define ICMPV6_NA_FLAG_OVERRIDE         0x20000000

/* Default router's maximum lifetime in seconds (18.2 hours) */
#define ICMPV6_ROUTER_LIFTIME           0xFFF0

/* Option type target link-layer address as defined in RFC4861 */
#define ICMPV6_OPTION_TARGET_ADDRESS    2

typedef struct
{
	ua8_t type;
	ua8_t code;
	ua16_t checksum;
} icmpv6_header_t;

typedef struct
{
	ua8_t cur_hop_limit;
	ua8_t flags;
	ua16_t router_lifetime;
	ua32_t reachable_time;
	ua32_t retrans_timer;
} icmpv6_ra_header_t;

typedef struct
{
	ua32_t reserved;
	ua8_t target_addr[16];
} icmpv6_ns_header_t;

typedef struct
{
	ua32_t flags;
	ua8_t target_addr[16];
} icmpv6_na_header_t;

typedef struct {
	eth_header_t eth;
	ipv6_header_t ip;
	icmpv6_header_t icmpv6_hdr;
	union {
		icmpv6_ra_header_t ra;
		icmpv6_na_header_t na;
	} icmp;
	ua8_t icmp_options[128];
} icmpv6_packet_t;

//liubinjun 20130110
typedef struct {
	ua8_t type;
	ua8_t len;
	ua8_t prefix_len;
	ua8_t flags;
	ua32_t route_lifetime;
	ua32_t Prefix[4];
} icmpv6_route_info_option_t;

typedef struct {
	ua8_t type;
	ua8_t len;
	ua8_t mac[ETH_ADDR_LENGTH];
} icmpv6_mac_option_t;

typedef struct {
	ua8_t type;
	ua8_t len;
	ua8_t prefix_len;
	ua8_t flags;
	uint32_t valid_lifetime;
	uint32_t preferred_lifetime;
	uint32_t reserved;
	uint32_t Prefix[4];
} icmpv6_prefix_option_t;

typedef struct {
	icmpv6_packet_t pkt;
	uint32_t pkt_length;
} icmpv6_pkt_ctx_t;


typedef struct{
	ipv4_header_t ip;
	udp_header_t udp;
	bootp_header_t bootp;
	ua32_t magic_cookie;
	ua8_t dhcp_options[128];
} dhcp_packet_ex_t;


/* The emulated "topology". ARP and DHCP processing use this information to
* form their responses */
typedef struct {
    UCHAR client_mac_addr[ETH_ADDR_LENGTH];
    UCHAR gw_mac_addr[ETH_ADDR_LENGTH];
    BOOLEAN emulate_arp;
    BOOLEAN emulate_dhcp;
	
    /* The MAC address of the gateway during the last DHCPACK we sent. Until we
	* send a new DHCPACK we should keep it to be used as source for a future
	* DHCPNAK */
    UCHAR dhcpack_gw_mac_addr[ETH_ADDR_LENGTH];
} emulation_common_info_t;

typedef struct {
    UINT client_ip_addr;
    UINT gw_ip_addr;
    UINT dns_ip_addr_1;
    UINT dns_ip_addr_2;
    UINT subnet;
	
    /* The IP address of the gateway during the last DHCPACK we sent. When
	* sending DHCPNAK we will use it as the server address. */
    UINT dhcpack_gw_ip_addr;
    BOOLEAN is_valid;
} emulation_info_t;

typedef struct {
    UCHAR client_ip_addr[IPV6_ADDR_LEN];
    UCHAR gw_ip_addr[IPV6_ADDR_LEN];
    UCHAR dns_ip_addr_1[IPV6_ADDR_LEN];
    UCHAR dns_ip_addr_2[IPV6_ADDR_LEN];
    UCHAR gw_duid[IPV6_DUID_MAX_LENGTH];
    UCHAR gw_duid_len;
    BOOLEAN is_valid;
} emulation_v6_info_t;


/* The main context of the filtering mechanism */
typedef struct {
    void * dc_net_ctx;//dc_net_ctx_t
    emulation_common_info_t emul_cmn_info;
    emulation_info_t emul_info;
    emulation_v6_info_t emul_v6_info;
} dc_net_filter_ctx_t;


/* Information about a packet that is currently being filtered the fields are
* being updated as the packet progress in the different filtering functions */
typedef struct {
	struct usbnet* dev;
    dc_net_filter_ctx_t *s;
    packet_h packet;
	
    UINT size; /* total size of packet */
    UINT eth_datagram_offset; /* IP/ARP offset */
    UINT udp_datagram_offset;
	
    UINT dhcp_xid;
    UCHAR dhcp_req_type;
    UINT dhcp_client_addr;
    UINT dhcp_requested_ip;
	
    BOOLEAN drop;
} packet_info_t;


typedef struct {
    eth_header_t eth;
    ipv6_header_t ip;
    udp_header_t udp;
    ua32_t msg_type_xid;
    ua8_t dhcp_options[128];
} dhcpv6_packet_t;

typedef struct {
    dhcpv6_packet_t pkt;
    UINT options_size;
    UINT pkt_length;
} dhcpv6_pkt_ctx_t;

void dump_packets(UCHAR * frame, UINT frame_size);

dc_status_t dc_net_get_packet_bytes(packet_h packet_p, uint32_t offset, uint32_t size, void *buff_ptr);
BOOLEAN is_ipv6_packet_ex( unsigned char * frame, unsigned int frame_size );
BOOLEAN is_ipv6_packet(packet_info_t *pi, ipv6_header_t *iphdr);

int dc_net_init_filter_ctx(void * ctx);
void dc_net_uninit_filter_ctx(void * ctx);
void dc_net_filter_invalidate_emulation_info(void *ctx);

void filter_handle_packet(packet_info_t *pi);
int dc_net_self_receive(struct usbnet *dev, UCHAR * data, UINT data_size);

int filter_tx_packet(struct usbnet *dev, unsigned char * frame, unsigned int frame_size);
int filter_rx_packet(struct usbnet *dev, unsigned char * frame, unsigned int frame_size);


BOOLEAN is_ndp_ipv6_packet(packet_info_t *pi, UCHAR *msg_type);
void dc_net_process_icmpv6_packet(packet_info_t *pi, ipv6_header_t *iphdr, UCHAR msg_type);

BOOLEAN dc_net_is_dhcpv6_packet(packet_info_t *pi);
void dc_net_process_dhcpv6_packet(packet_info_t *pi, ipv6_header_t *iphdr);

#endif // filter_h__
