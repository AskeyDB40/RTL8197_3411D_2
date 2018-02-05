/*
 *      Include file of utility.c
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: utility.h,v 1.8 2009/09/03 05:04:42 keith_huang Exp $
 *
 */

#ifndef INCLUDE_UTILITY_H
#define INCLUDE_UTILITY_H
#include "apmib.h"
#include <unistd.h>

#ifdef CONFIG_RTL_WAPI_SUPPORT
//if SYS_TIME_NOT_SYNC_CA exists, our system hasn't sync time yet
#define SYS_TIME_NOT_SYNC_CA "/var/tmp/notSyncSysTime"
#endif

#define CHECK_PARAM_NUM(input, min, max) if ( (input < min) || (input > max) ) return ERR_9007;
#define CHECK_PARAM_STR(str, min, max)  do { \
	int tmp; \
	if (!str) return ERR_9007; \
	tmp=strlen(str); \
	if ((tmp < min) || (tmp > max)) return ERR_9007; \
}	while (0)
#define CONFIG_SET(key, val) if ( mib_set(key, val)==0)  return ERR_9002
#define CONFIG_GET(key, ret) if ( mib_get(key, ret)==0)  return ERR_9002

#define	TR181_DEBUG
#define	TR181_TRACE
#define	TR181_ERROR
#define	TR181_WARNING

#ifdef TR181_DEBUG
#define tr181_printf(fmt, arg...) CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d] - "fmt"\n",__FUNCTION__,__LINE__,##arg))
#else
#define tr181_printf(a,...) do{}while(0)
#endif

#ifdef TR181_TRACE
#define tr181_trace() CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d]\n",__FUNCTION__,__LINE__))
#else
#define tr181_trace(a,...) do{}while(0)
#endif

#ifdef TR181_ERROR
#define tr181_error(fmt, arg...) CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_ERROR, ("[%s:%d]"fmt"\n",__FUNCTION__,__LINE__,##arg))
#else
#define tr181_error(a,...) do{}while(0)
#endif

#ifdef TR181_WARNING
#define tr181_warning(fmt, arg...) CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_WARNING, ("[%s:%d]"fmt"\n", __FUNCTION__, __LINE__,##arg))
#else
#define tr181_warning(a,...) do{}while(0)
#endif

#ifdef TR069_ANNEX_G
#define	STUN_BINDING_REQ				1
#define STUN_BINDING_MAINTAIN			2
#define STUN_BINDING_TIMEOUT_DISCOVERY	3
#define STUN_BINDING_TIMEOUT_MONITORING	4
#define STUN_BINDING_CHANGE				5
#define STUN_CHANGE_OTHER_PARAM			6

#define CWMP_STUN_CNONCE_LEN			256
#define CWMP_STUN_SIGNATURE_LEN			40
#define CWMP_STUN_SIGNATURE_STRING_LEN	2048
#endif

typedef enum { IP_ADDR, DST_IP_ADDR, SUBNET_MASK, DEFAULT_GATEWAY, HW_ADDR } ADDR_T;
typedef enum {
	SYS_UPTIME,
	SYS_DATE,
	SYS_YEAR,
	SYS_MONTH,
	SYS_DAY,
	SYS_HOUR,
	SYS_MINUTE,
	SYS_SECOND,
	SYS_FWVERSION,
	SYS_LAN_DHCP,
	SYS_DHCP_LAN_IP,
	SYS_DHCP_LAN_SUBNET,
	SYS_DHCPS_IPPOOL_PREFIX,
	SYS_DNS_MODE,
	SYS_WLAN,
	SYS_WLAN_BAND,
	SYS_WLAN_AUTH,
	SYS_WLAN_PREAMBLE,
	SYS_WLAN_BCASTSSID,
	SYS_WLAN_ENCRYPT,
	SYS_WLAN_PSKFMT,
	SYS_WLAN_PSKVAL,
	SYS_WLAN_WEP_KEYLEN,
	SYS_WLAN_WEP_KEYFMT,
	SYS_WLAN_WPA_MODE,
	SYS_WLAN_RSPASSWD,
	SYS_TX_POWER,
	SYS_WLAN_MODE,
	SYS_WLAN_TXRATE,
	SYS_WLAN_BLOCKRELAY,
	SYS_WLAN_AC_ENABLED,
	SYS_WLAN_WDS_ENABLED,
	SYS_WLAN_QoS,
	SYS_DHCP_MODE,
	SYS_IPF_OUT_ACTION,
	SYS_DEFAULT_PORT_FW_ACTION,
	SYS_MP_MODE,
	SYS_IGMP_SNOOPING,
	SYS_PORT_MAPPING,
	SYS_IP_QOS,
	SYS_IPF_IN_ACTION,
	SYS_WLAN_BLOCK_ETH2WIR,
	SYS_DNS_SERVER,
	SYS_LAN_IP2,
	SYS_LAN_DHCP_POOLUSE,
	SYS_DEFAULT_URL_BLK_ACTION,
	SYS_DEFAULT_DOMAIN_BLK_ACTION,
	SYS_DSL_OPSTATE,
	SYS_DHCPV6_MODE,
	SYS_DHCPV6_RELAY_UPPER_ITF,
	SYS_LAN_IP6_LL,
	SYS_LAN_IP6_GLOBAL,
	SYS_WLAN_WPA_CIPHER,
	SYS_WLAN_WPA2_CIPHER
} SYSID_T;

/* type define */
struct user_net_device_stats {
    unsigned long long rx_packets;	/* total packets received       */
    unsigned long long tx_packets;	/* total packets transmitted    */
    unsigned long long rx_bytes;	/* total bytes received         */
    unsigned long long tx_bytes;	/* total bytes transmitted      */
    unsigned long rx_errors;	/* bad packets received         */
    unsigned long tx_errors;	/* packet transmit problems     */
    unsigned long rx_dropped;	/* no space in linux buffers    */
    unsigned long tx_dropped;	/* no space available in linux  */
    unsigned long rx_multicast;	/* multicast packets received   */
    unsigned long rx_compressed;
    unsigned long tx_compressed;
    unsigned long collisions;

    /* detailed rx_errors: */
    unsigned long rx_length_errors;
    unsigned long rx_over_errors;	/* receiver ring buff overflow  */
    unsigned long rx_crc_errors;	/* recved pkt with crc error    */
    unsigned long rx_frame_errors;	/* recv'd frame alignment error */
    unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
    unsigned long rx_missed_errors;	/* receiver missed packet     */
    /* detailed tx_errors */
    unsigned long tx_aborted_errors;
    unsigned long tx_carrier_errors;
    unsigned long tx_fifo_errors;
    unsigned long tx_heartbeat_errors;
    unsigned long tx_window_errors;
};

/* Entry info scanned by site survey */

#define SSID_LEN	32

#if defined(CONFIG_RTK_MESH) ||  defined(CONFIG_RTL_819X)  /*add for RTL819X since wlan driver default include mesh data*/
//by GANTOE for site survey 2008/12/26
#define MESHID_LEN 32 
#endif 

#define	MAX_BSS_DESC	64

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


typedef enum _wlan_mac_state {
    STATE_DISABLED=0, STATE_IDLE, STATE_SCANNING, STATE_STARTED, STATE_CONNECTED, STATE_WAITFORKEY
} wlan_mac_state;


typedef struct _bss_info {
    unsigned char state;
    unsigned char channel;
    unsigned char txRate;
    unsigned char bssid[6];
    unsigned char rssi, sq;	// RSSI  and signal strength
    unsigned char ssid[SSID_LEN+1];
} bss_info;

/**  for wlan stats **/
typedef struct _wlan_stats {
	unsigned int totalBytesSent;
	unsigned int totalBytesReceived;
	unsigned int totalPacketsSent;
	unsigned int totalPacketsReceived;
	unsigned int errorsSent;
	unsigned int errorsReceived;
	unsigned int unicastPacketsSent;
	unsigned int unicastPacketsReceived;
	unsigned int discardPacketsSent;
	unsigned int discardPacketsReceived;
	unsigned int multicastPacketsSent;
	unsigned int multicastPacketsReceived;
	unsigned int broadcastPacketsSent;
	unsigned int broadcastPacketsReceived;
	unsigned int unknownProtoPacketsReceived;
	unsigned int totalPSKFailures;
	unsigned int totalIntegrityFailures;
}wlan_stats;

#if defined(CONFIG_RTL_WAPI_SUPPORT)
#define	SECURITY_INFO_WAPI		0xa5a56789
#endif
typedef struct _BssDscr {
    unsigned char bdBssId[6];
    unsigned char bdSsIdBuf[SSID_LEN];
    OCTET_STRING  bdSsId;

#if defined(CONFIG_RTK_MESH) || defined(CONFIG_RTL_819X) 
	//by GANTOE for site survey 2008/12/26
	unsigned char bdMeshIdBuf[MESHID_LEN]; 
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

} BssDscr, *pBssDscr;
	// P2P_SUPPORT
enum p2p_role_s {
	R_P2P_GO =1	,
	R_P2P_DEVICE = 2,
	R_P2P_CLIENT =3  

};

typedef struct _sitesurvey_status {
    unsigned char number;
    unsigned char pad[3];
    BssDscr bssdb[MAX_BSS_DESC];
} SS_STATUS_T, *SS_STATUS_Tp;

typedef enum _wlan_wds_state {
    STATE_WDS_EMPTY=0, STATE_WDS_DISABLED, STATE_WDS_ACTIVE
} wlan_wds_state;

typedef struct _WDS_INFO {
	unsigned char	state;
	unsigned char	addr[6];
	unsigned long	tx_packets;
	unsigned long	rx_packets;
	unsigned long	tx_errors;
	unsigned char	txOperaRate;
} WDS_INFO_T, *WDS_INFO_Tp;

struct _misc_data_ {
	unsigned char	mimo_tr_hw_support;
	unsigned char	mimo_tr_used;	
	unsigned char	resv[30];
};

#if defined(CONFIG_RTL_P2P_SUPPORT)
/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct __p2p_state_event{
	unsigned char  p2p_status;
	unsigned char  p2p_event;	
	unsigned short p2p_wsc_method;		
	unsigned char  p2p_role;
} P2P_SS_STATUS_T, *P2P_SS_STATUS_Tp;

/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct _p2p_provision_comm
{
	unsigned char dev_address[6];
	unsigned short wsc_config_method;	
	unsigned char channel;  	
} P2P_PROVISION_COMM_T, *P2P_PROVISION_COMM_Tp;

/* Any changed here MUST sync with 8192cd_p2p.h */
typedef struct __p2p_wsc_confirm
{
	unsigned char dev_address[6];		
	unsigned short wsc_config_method;	
	unsigned char pincode[9];  	
}P2P_WSC_CONFIRM_T, *P2P_WSC_CONFIRM_Tp;

// event 
enum {
	P2P_NO_EVENT = 0,
	P2P_EVENT_RX_PROVI_REQ = 1			/* received provision req*/			
};

// need sync with wlan driver 8192cd_p2p.h
enum {
	P2P_S_IDLE = 			0,			/* between state and state */	

	P2P_S_LISTEN ,			/*1 listen state */
	
	P2P_S_SCAN ,			/*2 Scan state */
	
	P2P_S_SEARCH ,			/*3 Search state*/

	// 4~14 ; show status 4 in web page
	P2P_S_PROVI_TX_REQ ,	/*4 send provision req*/	
	P2P_S_PROVI_WAIT_RSP ,	/*5 wait provision rsp*/	
	P2P_S_PROVI_RX_RSP	,	/*6 rx provision rsp*/	

	P2P_S_PROVI_RX_REQ ,	/*7 received provision req*/	
	P2P_S_PROVI_TX_RSP ,	/*8 send provision rsp*/	

	
	P2P_S_NEGO_TX_REQ ,		/*9 send NEGO req*/	
	P2P_S_NEGO_WAIT_RSP ,	/*10 waiting for NEGO rsp*/		
	P2P_S_NEGO_TX_CONF ,	/*11 send NEGO confirm*/			


	P2P_S_NEGO_RX_REQ ,		/*12 rx NEGO req */	
	P2P_S_NEGO_TX_RSP ,		/*13 send NEGO rsp */	
	P2P_S_NEGO_WAIT_CONF ,	/*14 wait NEGO conf */	

	// 15~16 ; show status 5 in web page	
	P2P_S_CLIENT_CONNECTED_DHCPC ,		/*15 p2p client Rdy connected */		
	P2P_S_CLIENT_CONNECTED_DHCPC_done, 	/*16 p2p client Rdy connected */			

	// 17~18 ; show status 6 in web page	
	P2P_S_preGO2GO_DHCPD ,			/*17 p2p client Rdy connected */		
	P2P_S_preGO2GO_DHCPD_done, 		/*18 p2p client Rdy connected */				

	P2P_S_back2dev 		/*exceed 20 seconds p2p client can't connected*/
};



#endif //#if defined(CONFIG_RTL_P2P_SUPPORT)

/*
 * cwmp startup environment check results
 */
enum{
	CWMP_ENV_OK,
	CWMP_ENV_WAN_INTERFACE_ERROR,
	CWMP_ENV_WAN_DISCONN_ERROR,
	CWMP_ENV_MTD_TRANSFER_ERROR,
	CWMP_ENV_MTD_NOTIFY_ERROR,
	CWMP_ENV_ACS_URL_ERROR,
}CWMP_ENV_STATUS;


int getWlStaNum( char *interface, int *num );
int getWlStaInfo( char *interface,  WLAN_STA_INFO_Tp pInfo );
int getInAddr(char *interface, ADDR_T type, void *pAddr);
int getDefaultRoute(char *interface, struct in_addr *route);
int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus );
int getWlSiteSurveyRequest(char *interface, int *pStatus);
int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res);
int getWlJoinResult(char *interface, unsigned char *res);
int getWlBssInfo(char *interface, bss_info *pInfo);
int getWdsInfo(char *interface, char *pInfo);
int getMiscData(char *interface, struct _misc_data_ *pData);
int va_cmd(const char *cmd, int num, int dowait, ...);
int getWlanStatsInfo(char *ifname, wlan_stats *stats_info);
static int getWlanStatsFileValue(char *line, unsigned int *value);
int IS_MCAST(unsigned char *da);
void convert_bin_to_str(unsigned char *bin, int len, char *out);

#ifdef CONFIG_RTK_MESH 
	//GANTOE for site survey 2008/12/26
int setWlJoinMesh (char*, unsigned char*, int, int, int); 
int getWlMeshLink (char*, unsigned char*, int);	// This function might be removed when the mesh peerlink precedure has been completed
int getWlMib (char*, unsigned char*, int);
#endif

int getStats(char *interface, struct user_net_device_stats *pStats);
int getEth0PortLink(unsigned int port_index);
unsigned int getEthernetBytesCount(unsigned int port_index);
int getEthernetEeeState(unsigned int port_index);
int getWanLink(char *interface);
int getWanInfo(char *pWanIP, char *pWanMask, char *pWanDefIP, char *pWanHWAddr);
#ifdef UNIVERSAL_REPEATER
int isVxdInterfaceExist(char *interface);
#endif

int displayPostDate(char *postDate);

int fwChecksumOk(char *data, int len);
void kill_processes(void);
void killDaemon(int wait);
//int updateConfigIntoFlash(unsigned char *data, int total_len, int *pType, int *pStatus);

void swapWlanMibSetting(unsigned char wlanifNumA, unsigned char wlanifNumB);

int getInFlags(char *interface, int *flags);
int cwmpSettingChange(int mibId);


#include <fcntl.h>
int getWlanMib(int wlanRootIndex, int wlanValIndex, int id, void *value);
int setWlanMib(int wlanRootIndex, int wlanValIndex, int id, void *value);
int getWlanBssInfo(int wlanRootIndex, int wlanValIndex, void *value);


#define FW_SIGNATURE_WITH_ROOT	((char *)"cr6b")
#define FW_SIGNATURE			((char *)"cs6b")
#define WEB_SIGNATURE			((char *)"w6bg")
#define ROOT_SIGNATURE			((char *)"r6br")


int SetWlan_idx(char * wlan_iface_name);
short whichWlanIfIs(PHYBAND_TYPE_T phyBand);
unsigned int getWLAN_ChipVersion();

#if defined(CONFIG_REPEATER_WPS_SUPPORT) || defined(POWER_CONSUMPTION_SUPPORT)
typedef enum { WLAN_OFF=0, WLAN_NO_LINK=1, WLAN_LINK=2} WLAN_STATE_T;
WLAN_STATE_T updateWlanifState(char *wlanif_name);
#endif


typedef struct countryIE {
	unsigned int		countryNumber;
	unsigned char 		countryA2[3];
	unsigned char		A_Band_Region;	//if support 5G A band? ;  0 == no support ; aBandRegion == real region domain
	unsigned char		G_Band_Region;	//if support 2.4G G band? ;  0 == no support ; bBandRegion == real region domain
	unsigned char 		countryName[24];
} COUNTRY_IE_ELEMENT;

typedef struct REG_DOMAIN_TABLE {
	unsigned char		region;
	unsigned int 		channel_set;
	unsigned char       area[24];
} REG_DOMAIN_TABLE_ELEMENT_T;


/*
 * File Transfer Utilities
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#ifndef IFNAMSIZ
#define IFNAMSIZ        16
#endif

int http_client( char *url, char *name, char *passwd, char *target, int action, void *data );
int ftp_client(char *url, char *name, char *passwd, char *filename, int action, void *data);

struct TR143_UDPEchoConfig
{
	unsigned char	Enable;
	unsigned char	EchoPlusEnabled;
	unsigned short	UDPPort;
	unsigned char	Interface[16];
	unsigned char	SourceIPAddress[4];
};
extern const char gUDPEchoServerName[];
extern const char gUDPEchoServerPid[];
void UDPEchoConfigSave(struct TR143_UDPEchoConfig *p);
int UDPEchoConfigStart( struct TR143_UDPEchoConfig *p );
int UDPEchoConfigStop( struct TR143_UDPEchoConfig *p );
int apply_UDPEchoConfig( int action_type, int id, void *olddata );

struct net_link_info
{
	unsigned long	supported;	/* Features this interface supports: ports, link modes, auto-negotiation */
	unsigned long	advertising;	/* Features this interface advertises: link modes, pause frame use, auto-negotiation */
	unsigned short	speed;		/* The forced speed, 10Mb, 100Mb, gigabit */
	unsigned char	duplex;		/* Duplex, half or full */
	unsigned char	phy_address;
	unsigned char	transceiver;	/* Which transceiver to use */
	unsigned char	autoneg;	/* Enable or disable autonegotiation */
};


int list_net_device_with_flags(short flags, int nr_names, char (* const names)[IFNAMSIZ]);
int get_net_link_status(const char *ifname);
int get_net_link_info(const char *ifname, struct net_link_info *info);
#if defined(MULTI_WAN_SUPPORT)
#define MULTI_WAN_IFNAME_FILE "/var/run/multi_wan_ifname"
#define NULL_STR ""
#define IFACE_NAME_MAX 32
#endif

#ifdef _CWMP_APPLY_
/*******************************************************************************/
/**** APIs for applying the new values****/
/*******************************************************************************/
#define CWMP_PRI_SH	-2	/*SUPER HIGH*/
#define CWMP_PRI_H	-1	/*HIGH*/
#define CWMP_PRI_N	 0	/*NORMAL*/
#define CWMP_PRI_L	 1	/*LOW*/
#define CWMP_PRI_SL	 2	/*SUPER LOW*/

struct CWMP_APPLY
{
	int	priority;
	int	(*cb_fun)( int action, int i, void *olddata);
	int	action;
	int	id;
	void	*olddata;
	struct CWMP_APPLY	*next;
};

extern int apply_add(
	int			priority,
	void			*cb_fun,
	int			action,
        int	 		id,
        void 			*olddata,
        int			size
);
extern int apply_destroy( void );
extern int apply_takeaction( void );
/*******************************************************************************/
/**** End APIs for applying the new values****/
/*******************************************************************************/
#endif


#endif // INCLUDE_UTILITY_H
