#ifndef LTE_NCM_INC
#define LTE_NCM_INC


#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/ctype.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/crc32.h>
#include <linux/usb.h>
#include <linux/version.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/usb/cdc.h>
#include <linux/usb/usbnet.h>
#include "kstype.h"
#include "filter.h"


#define DRV_NAME					"USBNCM"


/* CDC NCM subclass 6.2.11 SetCrcMode */
#define USB_CDC_NCM_CRC_NOT_APPENDED 0x00
#define USB_CDC_NCM_CRC_APPENDED 0x01

#define FLAG_MULTI_PACKET       0x2000

/* CDC NCM subclass 3.2.1 */
#define USB_CDC_NCM_NDP16_LENGTH_MIN		0x10

/* Maximum NTB length */
#define	CDC_NCM_NTB_MAX_SIZE_TX			16384	/* bytes */
#define	CDC_NCM_NTB_MAX_SIZE_RX			16384	/* bytes */

/* Minimum value for MaxDatagramSize, ch. 6.2.9 */
#define	CDC_NCM_MIN_DATAGRAM_SIZE		1514	/* bytes */

#define	CDC_NCM_MIN_TX_PKT			512	/* bytes */

/* Default value for MaxDatagramSize */
#define	CDC_NCM_MAX_DATAGRAM_SIZE		2048	/* bytes */

/*
 * Maximum amount of datagrams in NCM Datagram Pointer Table, not counting
 * the last NULL entry. Any additional datagrams in NTB would be discarded.
 */
#define	CDC_NCM_DPT_DATAGRAMS_MAX		32

/* Maximum amount of IN datagrams in NTB */
#define	CDC_NCM_DPT_DATAGRAMS_IN_MAX		0 /* unlimited */

/* Restart the timer, if amount of datagrams is less than given value */
#define	CDC_NCM_RESTART_TIMER_DATAGRAM_CNT	3

/* The following macro defines the minimum header space */
#define	CDC_NCM_MIN_HDR_SIZE \
	(sizeof(struct usb_cdc_ncm_nth16) + sizeof(struct usb_cdc_ncm_ndp16) + \
	(CDC_NCM_DPT_DATAGRAMS_MAX + 1) * sizeof(struct usb_cdc_ncm_dpe16))


#define CDC_NCM_TIMER_INTERVAL			(400UL * NSEC_PER_USEC)

/*
// vendor request for jungo IP information
struct jungo_net_info
{
	u32 client_ip;
	u32 gateway;
	u32 subnet_mask;
	u32 dns1;
	u32 dns2;
	u8 gateway_mac[6];
} __attribute__((packed));
*/

struct cdc_ncm_data {
	struct usb_cdc_ncm_nth16 nth16;
	struct usb_cdc_ncm_ndp16 ndp16;
	struct usb_cdc_ncm_dpe16 dpe16[CDC_NCM_DPT_DATAGRAMS_MAX + 1];
};

typedef struct cdc_ncm_ctx {
	struct cdc_ncm_data rx_ncm;
	struct cdc_ncm_data tx_ncm;
	struct usb_cdc_ncm_ntb_parameters ncm_parm;
#if 0
	struct timer_list tx_timer;
#else
	struct hrtimer	tx_timer;	
#endif
	struct tasklet_struct bh;

	const struct usb_cdc_ncm_desc *func_desc;
	const struct usb_cdc_header_desc *header_desc;
	const struct usb_cdc_union_desc *union_desc;
	const struct usb_cdc_ether_desc *ether_desc;

	struct usbnet *dev;
	struct net_device *netdev;
	struct usb_device *udev;
	struct usb_host_endpoint *in_ep;
	struct usb_host_endpoint *out_ep;
	struct usb_host_endpoint *status_ep;
	struct usb_interface *intf;
	struct usb_interface *control;
	struct usb_interface *data;

	struct sk_buff *tx_curr_skb;
	struct sk_buff *tx_rem_skb;


	
	spinlock_t mtx;
	atomic_t stop;

	u32 tx_timer_pending;
	u32 tx_curr_offset;
	u32 tx_curr_last_offset;
	u32 tx_curr_frame_num;
	u32 rx_speed;
	u32 tx_speed;
	u32 rx_max;
	u32 tx_max;
	u32 max_datagram_size;
	u16 tx_max_datagrams;
	u16 tx_remainder;
	u16 tx_modulus;
	u16 tx_ndp_modulus;
	u16 tx_seq;
	u16 connected;

	struct urb* connect_vendor_urb;
	struct usb_ctrlrequest connect_req;

//	int net_info_valid;
//	struct jungo_net_info net_info;
	dhcp_info_t dhcp_info;
	dhcpv6_info_t dhcpv6_info;
	void * filter_ctx; //dc_net_filter_ctx_t
	UCHAR phy_nic_mac_address[ETH_LENGTH_OF_ADDRESS];

	struct timer_list test_timer;
}dc_net_ctx_t;


/**
 * missing macros
 */
#define swap(a, b) \
	do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)


#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 22)

/**
 * missing skb functions
 */
static inline void skb_reset_tail_pointer(struct sk_buff *skb)
{
    skb->tail = skb->data;
}

static inline void skb_set_tail_pointer(struct sk_buff *skb, const int offset)
{
    skb_reset_tail_pointer(skb);
    skb->tail += offset;
}

#endif /* LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 22) */

#define IPSTR "%d.%d.%d.%d"
#define IP2STR(x) (x)[3], (x)[2], (x)[1], (x)[0]

#define MACSTR "%02x:%02x:%02x-%02x:%02x:%02x"
#define MAC2STR(x) (x)[0], (x)[1], (x)[2], (x)[3], (x)[4], (x)[5]

void dc_vendor_get_dhcp_info_and_link_up(struct usbnet *dev, dc_net_ctx_t *dc_net_ctx);

#endif // LTE_NCM_INC
// end of file
