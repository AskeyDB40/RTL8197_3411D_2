#ifndef IAPP_H
#define IAPP_H

#define IAPP_PORT_D3 2313
#define IAPP_PORT_D5 3517
#define IAPP_MULTICAST_ADDR	"224.0.1.178"
#define SIOCGIWRTLSTAINFO   		0x8B30	// get station table information
#define MAX_STA_NUM			64	// max support sta number
/* flag of sta info */
#define STA_INFO_FLAG_AUTH_OPEN     	0x01
#define STA_INFO_FLAG_AUTH_WEP      	0x02
#define STA_INFO_FLAG_ASOC          	0x04
#define STA_INFO_FLAG_ASLEEP        	0x08


struct iapp_hdr {
	u8 version;
	u8 command;
	u16 identifier;
	u16 length;
	/* followed by length-6 octets of data */
} __attribute__ ((packed));

#define IAPP_VERSION 0

enum IAPP_COMMAND {
	IAPP_CMD_ADD_notify = 0,
	IAPP_CMD_MOVE_notify = 1,
	IAPP_CMD_MOVE_response = 2,
	IAPP_CMD_Send_Security_Block = 3,
	IAPP_CMD_ACK_Security_Block = 4
};

struct iapp_add_notify {
	u8 addr_len;
	u8 reserved;
	u8 mac_addr[6];
	u16 seq_num;
} __attribute__ ((packed));


/* Layer 2 Update frame (802.2 Type 1 LLC XID Update response) */
struct iapp_layer2_update {
	u8 da[6]; /* broadcast */
	u8 sa[6]; /* STA addr */
	u16 len; /* 8 */
	u8 dsap; /* 0 */
	u8 ssap; /* 0 */
	u8 control;
	u8 xid_info[3];
	u8 pad[10];		// Fix TKIP MIC error issue. david+2006-11-09
};

/* WLAN sta info structure */
typedef struct wlan_sta_info {
	unsigned short	aid;
	unsigned char	addr[6];
	unsigned long	tx_packets;
	unsigned long	rx_packets;
	unsigned long	expired_time;	// 10 msec unit
	unsigned short	flag;
	unsigned char	txOperaRates;
	unsigned char	rssi;
	unsigned long	link_time;		// 1 sec unit
	unsigned long	tx_fail;
	unsigned long tx_bytes;
	unsigned long rx_bytes;
	unsigned char network;
	unsigned char ht_info;	// bit0: 0=20M mode, 1=40M mode; bit1: 0=longGI, 1=shortGI
	unsigned char	RxOperaRate;
	unsigned char 	resv[5];
} WLAN_STA_INFO_T, *WLAN_STA_INFO_Tp;

#define MAX_MSG_SIZE	1600
#define APPLY_D3		0x01
#define APPLY_D5		0x02

#define DRIVER_8180		0x01
#define DRIVER_8185		0x02

// context
struct iapp_context {
	int				driver_ver;
	u16				iapp_identifier; /* next IAPP identifier */
	struct in_addr	iapp_own, iapp_broadcast;
	int				iapp_udp_sock;
	int				iapp_mltcst_sock;
	int				iapp_packet_sock;

	char			iapp_iface[16];
	int				readfifo;
	u8				spcApply;
	u8				RecvBuf[MAX_MSG_SIZE];

	char			wlan_iface[10][16];
	int				wlan_sock[10];

	int				debug; /* debug verbosity level */
};

#define FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#endif /* IAPP_H */
