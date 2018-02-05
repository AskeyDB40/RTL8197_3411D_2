#ifndef _RTK_WLAN_H__
#define _RTK_WLAN_H__
#define SSID_LEN 32
#define RTK_BTCONFIG_MAX_BSS_NUM 64

typedef enum _wlan_mac_state {
    STATE_DISABLED=0, STATE_IDLE, STATE_SCANNING, STATE_STARTED, STATE_CONNECTED, STATE_WAITFORKEY
} wlan_mac_state;

//typedef enum { AUTH_OPEN=0, AUTH_SHARED, AUTH_BOTH } AUTH_TYPE_T;
//typedef enum { ENCRYPT_DISABLED=0, ENCRYPT_WEP=1, ENCRYPT_WPA=2, ENCRYPT_WPA2=4, ENCRYPT_WPA2_MIXED=6 ,ENCRYPT_WAPI=7} ENCRYPT_T;

typedef enum _Capability {
    cESS 		= 0x01,
    cIBSS		= 0x02,
    cPollable		= 0x04,
    cPollReq		= 0x01,
    cPrivacy		= 0x10,
    cShortPreamble	= 0x20,
} Capability;

typedef enum _Synchronization_Sta_State{
    STATE_Min		= 0,
    STATE_No_Bss	= 1,
    STATE_Bss		= 2,
    STATE_Ibss_Active	= 3,
    STATE_Ibss_Idle	= 4,
    STATE_Act_Receive	= 5,
    STATE_Pas_Listen	= 6,
    STATE_Act_Listen	= 7,
    STATE_Join_Wait_Beacon = 8,
    STATE_Max		= 9
} Synchronization_Sta_State;

typedef struct bss_info
{
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;	// RSSI  and signal strength
    unsigned char ssid[SSID_LEN+1];
} bss_info,RTK_BSS_INFO, *RTK_BSS_INFOp;

typedef struct _OCTET_STRING {
    unsigned char *Octet;
    unsigned short Length;
} OCTET_STRING;

typedef enum _BssType {
    infrastructure = 1,
    independent = 2,
} BssType;

typedef	struct _IbssParms {
    unsigned short	atimWin;
} IbssParms;

typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[32];
    OCTET_STRING  bdSsId;

#if 1//defined(CONFIG_RTK_MESH) || defined(CONFIG_RTL_819X) 
	//by GANTOE for site survey 2008/12/26
	unsigned char bdMeshIdBuf[32]; 
	OCTET_STRING bdMeshId; 
#endif 
    BssType bdType;
    unsigned short bdBcnPer;			// beacon period in Time Units
    unsigned char bdDtimPer;			// DTIM period in beacon periods
    unsigned long bdTstamp[2];			// 8 Octets from ProbeRsp/Beacon
    IbssParms bdIbssParms;			// empty if infrastructure BSS
    unsigned short bdCap;				// capability information
    unsigned char ChannelNumber;			// channel number
    unsigned long bdBrates;
    unsigned long bdSupportRates;		
    unsigned char bdsa[6];			// SA address
    unsigned char rssi, sq;			// RSSI and signal strength
    unsigned char network;			// 1: 11B, 2: 11G, 4:11G
	// P2P_SUPPORT
	unsigned char	p2pdevname[33];		
	unsigned char	p2prole;	
	unsigned short	p2pwscconfig;		
	unsigned char	p2paddress[6];	
	unsigned char	stage;	    
} BssDscr, *pBssDscr;

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    BssDscr bssdb[64];
} SS_STATUS_T, *SS_STATUS_Tp;

typedef struct rtk_wlan_sta {
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
} RTK_WLAN_STA_INFO_T, *RTK_WLAN_STA_INFO_Tp;

#if 1
struct rtk_wlan_sta_info
{
	unsigned char bssid[6];
	unsigned char rssi;
}__attribute__((packed));
struct rtk_btconfig_sta_info
{
	unsigned char band;/*0:2.4g,1:5g*/
	unsigned int number;/*total number of clients*/
	struct rtk_wlan_sta_info sta_info[64];
}__attribute__((packed));

struct rtk_btconfig_bss_info
{
	unsigned char authAlg;/*0:open,1:WPA,2:WEP*/
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[32];
    unsigned char ChannelNumber;
	unsigned char rssi;
} __attribute__((packed));
struct rtk_btconfig_ss_result
{
	unsigned char band;/*0 for 2.4G,1 for 5G*/
	unsigned int number;/*total number of results*/
	struct rtk_btconfig_bss_info bss_info[RTK_BTCONFIG_MAX_BSS_NUM];
}__attribute__((packed));

struct rtk_btconfig_status
{
	unsigned char band;/*0 for 2.4G,1 for 5G*/
	unsigned char status;
	unsigned char encrypt;/*0:open,1 WPA,2:WEP*/
    unsigned char ssid[32];
    unsigned char bssid[6];
	unsigned char rssi;
	unsigned char config_status;/*0:unconfigured,1:configured*/
}__attribute__((packed));

struct rtk_btconfig_band_info
{
	unsigned char support_2_4g;
	unsigned char support_5g;
}__attribute__((packed));
struct rtk_connect_AP_info
{	
	unsigned char band; //0 for 2.4G,1 for 5G
	unsigned char encrypt; //0 for open ,1 for encrypt
    unsigned char bdSSID[32];
    unsigned char bdMAC[6];
	unsigned char password[32];
}__attribute__((packed));
struct rtk_wlan_config_info
{
	char wlan_interface[32];
	char ssid[32];
	int auth_type;
	int enc_mode;
	char psk[64];
	char wepkey[32];
};
#endif

#endif/*_RTK_WLAN_H__*/
