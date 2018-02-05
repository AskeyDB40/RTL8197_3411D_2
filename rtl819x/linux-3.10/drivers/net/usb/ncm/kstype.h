#ifndef kstype_h__
#define kstype_h__

typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef unsigned char BOOLEAN;
typedef unsigned int UINT;
typedef int dc_status_t;
typedef unsigned char UINT8;

#define DC_STATUS_SUCCESS                       0
#define DC_STATUS_OPERATION_FAILED              1
#define DC_STATUS_TIMEOUT                       2
#define DC_STATUS_BUFFER_TOO_SMALL              3
#define DC_STATUS_INVALID_PARAMETER             4
#define DC_STATUS_INVALID_DEVICE_REQUEST        5
#define DC_STATUS_INVALID_DEVICE_HANDLE         6
#define DC_STATUS_ACCESS_VIOLATION              7
#define DC_STATUS_DEVICE_BUSY                   8
#define DC_STATUS_DEVICE_NOT_CONNECTED          9
#define DC_STATUS_INSUFFICIENT_RESOURCES        10
#define DC_STATUS_NOT_SUPPORTED                 11
#define DC_STATUS_ALREADY                       12
#define DC_STATUS_BAD_MESSAGE                   13
#define DC_STATUS_MESSAGE_SIZE                  14
#define DC_STATUS_PROTOCOL                      15
#define DC_STATUS_INTERRUPTED                   16
#define DC_STATUS_STATE                         17
#define DC_STATUS_NOENT                         18


#define BREQUEST_GET_DHCP_INFO 1
#define BREQUEST_GET_DHCPV6_INFO 2


#define ETH_ADDR_LENGTH         6
#define ETH_HEADER_SIZE         14

#define ETH_LENGTH_OF_ADDRESS 6

#define ETH_PROTOCOL_TYPE_IPV4  0x0800
#define ETH_PROTOCOL_TYPE_IPV6  0x86dd

#define ETHER_TYPE_ARP          dc_hton16(0x0806)
#define ETHER_TYPE_IPV4         dc_hton16(ETH_PROTOCOL_TYPE_IPV4)
#define ETHER_TYPE_IPV6         dc_hton16(ETH_PROTOCOL_TYPE_IPV6)

#define BOOTP_UDP_CLIENT_PORT   dc_hton16(0x0044)
#define BOOTP_UDP_SERVER_PORT   dc_hton16(0x0043)
#define UDP_IPV6_CLIENT_PORT   dc_hton16(0x0222)
#define UDP_IPV6_SERVER_PORT   dc_hton16(0x0223)

#define IPV6_ADDR_LEN 16
#define IPV6_IP_HEADER_SIZE 40
#define IPV6_MCAST_ADDR_PREFIX 0xff
#define IPV6_DUID_MAX_LENGTH 32

#define IP4_ADDR_SIZE 4
#define IP4_VER_AND_DEFAULT_LENGTH 0x45
#define IP4_BROADCAST_ADDR 0xFFFFFFFF
#define IP_VERSION_IPV4 4
#define IP_VERSION_IPV6 6

#define IP_PROTOCOL_ICMPV4 1
#define IP_PROTOCOL_TCP 6
#define IP_PROTOCOL_UDP 17
#define IP_PROTOCOL_ICMPV6 58


#define BOOTP_OP_BOOTREQUEST 1
#define BOOTP_OP_BOOTREPLY   2

#define DHCP_MAGIC_COOKIE dc_hton32(0x63825363)
#define DHCP_REPLY_TTL 0x40

#define DHCP_MSG_DHCPDISCOVER 1
#define DHCP_MSG_DHCPOFFER 2
#define DHCP_MSG_DHCPREQUEST 3
#define DHCP_MSG_DHCPDECLINE 4
#define DHCP_MSG_DHCPACK 5
#define DHCP_MSG_DHCPNAK 6
#define DHCP_MSG_DHCPRELEASE 7
#define DHCP_MSG_DHCPINFORM 8
#define DHCP_LAST_MSG DHCP_MSG_DHCPINFORM


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/* Unaligned types */
//typedef uint8_t         ua8_t;
//typedef uint8_t         ua16_t[2];
//typedef uint8_t         ua32_t[4];

typedef unsigned char         ua8_t;
typedef unsigned char         ua16_t[2];
typedef unsigned char         ua32_t[4];


typedef void *packet_h;

#define DBG_I_IPV4_ADDR(_s_, _a_) \
	printk(KERN_ERR DRV_NAME " %s: %s = %d.%d.%d.%d\n", __func__, (_s_), (_a_)[3],  (_a_)[2],  (_a_)[1],  (_a_)[0])


#define DBG_I_MAC_ADDR(_s_, _a_) \
	printk(KERN_ERR DRV_NAME " %s: %s = %02X-%02X-%02X-%02X-%02X-%02X\n", __func__, (_s_), (_a_)[0],  (_a_)[1],  (_a_)[2],  (_a_)[3], (_a_)[4], (_a_)[5])

#define DBG_I_IPV6_ADDR(_s_, _a_) \
	printk(KERN_ERR DRV_NAME " %s: %s = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", __func__, (_s_), \
	(_a_)[0],  (_a_)[1],  (_a_)[2],  (_a_)[3],  (_a_)[4],  (_a_)[5],  (_a_)[6],  (_a_)[7],  \
	(_a_)[8],  (_a_)[9],  (_a_)[10],  (_a_)[11],  (_a_)[12],  (_a_)[13],  (_a_)[14],  (_a_)[15])

#define DBG_I_DUID_H(_s_, _a_) \
	printk(KERN_ERR DRV_NAME " %s: %s = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", __func__, (_s_), \
	(_a_)[0],  (_a_)[1],  (_a_)[2],  (_a_)[3],  (_a_)[4],  (_a_)[5],  (_a_)[6],  (_a_)[7],  \
	(_a_)[8],  (_a_)[9],  (_a_)[10],  (_a_)[11],  (_a_)[12],  (_a_)[13],  (_a_)[14],  (_a_)[15])

#define DBG_I_DUID_L(_s_, _a_) \
	printk(KERN_ERR DRV_NAME " %s: %s = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", __func__, (_s_), \
	(_a_)[16],  (_a_)[17],  (_a_)[18],  (_a_)[19],  (_a_)[20],  (_a_)[21],  (_a_)[22],  (_a_)[23],  \
	(_a_)[24],  (_a_)[25],  (_a_)[26],  (_a_)[27],  (_a_)[28],  (_a_)[29],  (_a_)[30],  (_a_)[31])

#define NdisMoveMemory(Destination, Source, Length) \
memcpy(Destination, Source, Length)


#define RtlEqualMemory(Destination,Source,Length) \
(!memcmp((Destination),(Source),(Length)))


#define NdisZeroMemory(Destination, Length) \
memset(Destination, 0, Length)



//
// This macro is used to copy from one network address to
// another.
//
#define ETH_COPY_NETWORK_ADDRESS(_D, _S) \
{ \
    *((UINT *)(_D)) = *((UINT *)(_S)); \
    *((USHORT *)((UCHAR *)(_D)+4)) = *((USHORT *)((UCHAR *)(_S)+4)); \
}


typedef struct
{
    ua8_t dst_addr[ETH_ADDR_LENGTH];
    ua8_t src_addr[ETH_ADDR_LENGTH];
    ua16_t eth_type;
} eth_header_t;

typedef struct
{
    ua8_t version_and_length;
    ua8_t diff_services;
    ua16_t total_length;
    ua16_t identification;
    ua16_t flags_and_frag_offset;
    ua8_t ttl;
    ua8_t protocol;
    ua16_t checksum;
    ua32_t src_ip_addr;
    ua32_t dst_ip_addr;
} ipv4_header_t;

typedef struct
{
    ua32_t version_traffic_flow;
    ua16_t payload_len;
    ua8_t next_header;
    ua8_t hop_limit;
    ua8_t src_ip_addr[16];
    ua8_t dst_ip_addr[16];
} ipv6_header_t;

typedef struct
{
    ua16_t src_port;
    ua16_t dst_port;
    ua16_t length;
    ua16_t checksum;
} udp_header_t;

typedef struct 
{
	ua16_t	sourcePort;			//发送端端口号
	ua16_t	destinationPort;	//接收端端口号
	ua32_t	sequenceNumber;		//标示消息端的数据位于全体数据块的某一字节的数字
	ua32_t	acknowledgeNumber;	//确认号,标示接收端对于发送端接收到数据块数值
	ua8_t	dataoffset;			//数据偏移
	ua8_t	flags;				//标志
	ua16_t	windows;			//窗口
	ua16_t	checksum;			//校验码
	ua16_t	urgentPointer;		//紧急数据指针
	ua32_t	options;			//选项和填充
}tcp_header_t;


/* ARP packet is generic and the address fields are of variable size. But we
* only care about Ethernet/IPv4 ARP requests that use MAC and IPv4 addresses,
* so we can set a fixed structure for that purpose and validate that the
* incoming ARP match these parameters */
typedef struct
{
    ua16_t hw_type;
    ua16_t prot_type;
    ua8_t hw_addr_len;
    ua8_t prot_addr_len;
    ua16_t opcode;
    ua8_t src_mac[ETH_ADDR_LENGTH];
    ua32_t src_ip;
    ua8_t dst_mac[ETH_ADDR_LENGTH];
    ua32_t dst_ip;
} arp_eth_ipv4_t;

typedef struct
{
    ua8_t op;
    ua8_t htype;
    ua8_t hlen;
    ua8_t hops;
    ua32_t xid;
    ua16_t secs;
    ua16_t flags;
    ua32_t ciaddr;
    ua32_t yiaddr;
    ua32_t siaddr;
    ua32_t giaddr;
    ua8_t chaddr[16];
    ua8_t sname[64];
    ua8_t file[128];
} bootp_header_t;

typedef struct
{
    ua32_t client_ip;
    ua32_t gateway_ip;
    ua32_t subnet;
    ua32_t dns_1;
    ua32_t dns_2;
    ua8_t  gateway_mac[ETH_ADDR_LENGTH];
} dhcp_info_t;

typedef struct
{
    ua8_t client_ip[IPV6_ADDR_LEN];
    ua8_t gateway_ip[IPV6_ADDR_LEN];
    ua8_t dns_1[IPV6_ADDR_LEN];
    ua8_t dns_2[IPV6_ADDR_LEN];
    ua8_t gateway_duid[IPV6_DUID_MAX_LENGTH];
    ua8_t gateway_duid_len;
} dhcpv6_info_t;


#endif // kstype_h__
