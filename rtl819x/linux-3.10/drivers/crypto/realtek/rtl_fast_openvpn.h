
#define MAX_OUT_LAYER_HDR_LEN 51  //14+8+20+8+1

#define ETH_HDR_HLEN 14
#define PPP_HDR_LEN 8
#define UDP_HDR_LEN 8
#define IP_HDR_LEN	20

#define FRAGMENT_OUT_LAYER_HDR_LEN  29	//(IP_HDR_LEN+UDP_HDR_LEN+1)

#define MAX_CLIENT_NUM 16

#define KEY_BUF_LEN 256

#define MAX_OPENVPN_PAYLOAD_LEN 1600

#define CRYPT_KEY_LEN	64
#define HMAC_KEY_LEN 	64
#define IV_LEN			16
#define DIGEST_LEN		20
#define MAX_DIGEST_LEN 64
#define PKT_ID_LEN		5

#define PING_STRING_SIZE 16
#define CBC_BLOCK_SIZE	16

#define DEFAULT_PORT_NUM 1194

//#define LIMIT_HW_ENGINE_NUM 163840

#define ADD_CLIENT_FLAG 1
#define DEL_CLIENT_FLAG  2

#define ETH_IFNAME "eth1"
#define PPP_IFNAME "ppp0"


#define CACHED_ADDRESS(x) CKSEG0ADDR(x)

const uint8_t ping_string[] = {
  0x2a, 0x18, 0x7b, 0xf3, 0x64, 0x1e, 0xb4, 0xcb,
  0x07, 0xed, 0x2d, 0x0a, 0x98, 0x1f, 0xc7, 0x48
};

const uint8_t EMPTY_MAC_ADDR[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define P_OPCODE_SHIFT		3
#define P_KEY_ID_MASK          0x07

#define MAX_PKT_ID		4294967295   

/* packet opcodes -- the V1 is intended to allow protocol changes in the future */
#define P_CONTROL_HARD_RESET_CLIENT_V1 1     /* initial key from client, forget previous state */
#define P_CONTROL_HARD_RESET_SERVER_V1 2     /* initial key from server, forget previous state */
#define P_CONTROL_SOFT_RESET_V1        3     /* new key, graceful transition from old to new key */
#define P_CONTROL_V1                   4     /* control channel packet (usually TLS ciphertext) */
#define P_ACK_V1                       5     /* acknowledgement for packets received */
#define P_DATA_V1                      6     /* data channel packet */

/* indicates key_method >= 2 */
#define P_CONTROL_HARD_RESET_CLIENT_V2 7     /* initial key from client, forget previous state */
#define P_CONTROL_HARD_RESET_SERVER_V2 8     /* initial key from server, forget previous state */

/* define the range of legal opcodes */
#define P_FIRST_OPCODE                 1
#define P_LAST_OPCODE                  8


#define KS_PRIMARY    0
#define KS_LAME_DUCK  1
#define KS_SIZE       2

#define RTL_SECONDS_TO_JIFFIES(x) ((x)*HZ)


enum 
{
	CRYPT_KEY_FLAG=0, 
	PEER_IP_FLAG=1, 
	PEER_PORT_FLAG=2, 
	DEC_KEY_FLAG=3, 
	ENC_KEY_FLAG=4, 
	HMAC_DEC_KEY_FLAG=5, 
	HMAC_ENC_KEY_FLAG=6, 
	PORT_NUM_FLAG=7, 
	VIRTUAL_IP_FLAG=8, 
	DEL_IP_PORT_FLAG=9, 
	CONFIG_OPTION_FLAG=10, 
	UPDATE_KEY_TIME_FLAG=11,
	PING_REC_TIME_FLAG=12,
	PING_SEND_TIME_FLAG=13,
	EXITING_FLAG=14,
	AUTH_NONE_FLAG=15
};

typedef struct _openvpn_key_info
{
	u8 *enc_key;	
	u8 *dec_key; 
	u8 *enc_hmac_key;
	u8 *dec_hmac_key;
	u8 *iv;	

	u8 key_id;	
}OpenVPN_Key_Info;

typedef struct _openvpn_info
{
	u8 peer_layer_hdr[MAX_OUT_LAYER_HDR_LEN];  
	//u8 peer_layer_hdr_len;
	u32 peer_ip;
	u32 virtual_ip;
	u16 peer_port;
	//u8 key_id;
	u32	pkt_id;
	
	int enc_key_index;
	//int dec_key_index;

	//u8 *enc_key[2];	
	//u8 *dec_key[2]; 
	//u8 *enc_hmac_key[2];
	//u8 *dec_hmac_key[2];
	//u8 *iv[2];	
	
	OpenVPN_Key_Info key_info[KS_SIZE];	

	u8 crypt_key_len;
	u8 hmac_key_len;
	
	u8 active_flag;
	u8 soft_rest_flag;
	
	struct timer_list key_update_timer;	

	struct timer_list ping_rec_timer;

	struct timer_list ping_send_timer;
	
}OpenVPN_Info;

typedef struct _shared_openvpn_info
{
	void *wan_dev;
	void *tun_dev;
	void *out_dev;	
	u8 *enc_data;
	u8 *dec_data; 
	u8 *digest;
	u8 *CryptResult;
	u8 *DesCryptResult;
	u8 local_layer_hdr_len;
	u16 local_port;
	u32 key_update_time;
	int ping_rec_time;
	int ping_send_time;
	u8 auth_none;
	u8 digest_len;
	u16 ip_id;
}Shared_OpenVPN_Info;

inline void *UNCACHED_MALLOC(int size);

unsigned char *rtl_vpn_get_skb_data(struct sk_buff* skb);
struct iphdr *rtl_vpn_ip_hdr(struct sk_buff *skb);
unsigned int rtl_vpn_get_skb_len(struct sk_buff *skb);
struct net_device *rtl_vpn_get_skb_dev(struct sk_buff* skb);
void rtl_vpn_set_skb_tail_direct(struct sk_buff *skb, int offset);
void rtl_vpn_set_skb_len(struct sk_buff *skb, int len);
void rtl_vpn_set_skb_protocol(struct sk_buff *skb,__be16 protocol);
struct net_device * rtl_vpn_get_dev_by_name(char *name);
void rtl_vpn_set_skb_dev(struct sk_buff *skb, struct net_device *dev);
unsigned char *rtl_vpn_skb_pull(struct sk_buff *skb, unsigned int len);
unsigned char *rtl_vpn_skb_put(struct sk_buff *skb, unsigned int len);
unsigned int rtl_vpn_skb_headroom(struct sk_buff *skb);
int rtl_vpn_skb_cloned(struct sk_buff *skb);
int rtl_vpn_skb_shared(const struct sk_buff *skb);
unsigned char *rtl_vpn_skb_push(struct sk_buff *skb, unsigned int len);
int rtl_vpn_skb_linearize(struct sk_buff *skb);
int rtl_vpn_call_skb_ndo_start_xmit(struct sk_buff *skb);
int rtl_vpn_netif_rx(struct sk_buff *skb);












