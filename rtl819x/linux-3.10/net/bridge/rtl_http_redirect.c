#include <linux/version.h>
#include <linux/init.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
#include <asm/system.h>
#endif
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/un.h>
#include <linux/in.h>
#include <linux/inet.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#include <generated/autoconf.h>
#else
#include <linux/config.h>
#endif
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/route.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <linux/netfilter/nf_conntrack_common.h>
//#include <linux/netfilter_ipv4/ip_conntrack_core.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/if.h>
#include <linux/spinlock.h>
#include <linux/inetdevice.h>

#include <linux/netfilter_bridge.h>

//#define HTTP_REDIRECT_VERIFICATION
//#define HTTP_REDIRECT_USER_TIMING


//#define HTTP_REDIRECT_DEBUG 


#define HTTP_REDIRECT_PROC_ROOT "http_redirect"
#define HTTP_REDIRECT_PROC_ENABLE "enable"
#ifdef HTTP_REDIRECT_VERIFICATION
#define HTTP_REDIRECT_PROC_REQ_URL "required_url"
#endif
//#define HTTP_REDIRECT_PROC_AUTO_ADD "auto_add"
#ifdef HTTP_REDIRECT_USER_TIMING
#define HTTP_REDIRECT_PROC_MAX_IDLE_TIME "max_idle_time"
#endif
#ifdef CONFIG_RTL_HTTP_REDIRECT_TR098
#define HTTP_REDIRECT_PROC_HOST "CaptivePortalURL"
#define HTTP_REDIRECT_PROC_ALLOWED_LIST "AllowedList"
#else
#define HTTP_REDIRECT_PROC_HOST "url"
#define HTTP_REDIRECT_PROC_CLIENT_LIST "client_list"
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#define CONFIG_RTL_PROC_NEW 
#endif

#ifdef CONFIG_PROC_FS
struct proc_dir_entry *redirect_proc_root = NULL;
#ifdef CONFIG_RTL_PROC_NEW
extern struct proc_dir_entry proc_root;
#else
static struct proc_dir_entry *redirect_proc_enable = NULL;
static struct proc_dir_entry *redirect_proc_host = NULL;
#ifdef CONFIG_RTL_HTTP_REDIRECT_TR098
static struct proc_dir_entry *redirect_proc_allowedList = NULL;
#else
static struct proc_dir_entry *redirect_proc_clientList = NULL;
#endif
#ifdef HTTP_REDIRECT_VERIFICATION
static struct proc_dir_entry *redirect_proc_required_url = NULL;
#endif
//static struct proc_dir_entry *redirect_proc_auto_add = NULL;

#ifdef HTTP_REDIRECT_USER_TIMING
static struct proc_dir_entry *redirect_proc_max_idle_time = NULL;
#endif
#endif/*CONFIG_RTL_PROC_NEW*/
#endif/*CONFIG_PROC_FS*/
/*
	notice: if you want change the redirect or Mac information , 
		you must turn off enableForce, after you set, you 
		can tuen on the enableForce, or race may be happen
*/

#ifdef HTTP_REDIRECT_DEBUG
int debug_on = 1;
#define DEBUGP if(debug_on) \
	printk
#define DUMP_CONTENT(dptr, length) \
	if(debug_on)\
	{\
	  int i;\
	  DEBUGP("\n*************************DUMP***********************\n");\
	  for (i=0;i<length;i++)\
	    DEBUGP("%c",dptr[i]);\
	  DEBUGP("*************************DUMP OVER******************\n");\
	}
#define DUMP_TUPLE_TCP(tp) \
	if(debug_on)\
	DEBUGP("tuple(tcp) %p: %u %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u\n",	\
	       (tp), (tp)->dst.protonum,				\
	       NIPQUAD((tp)->src.ip), ntohs((tp)->src.u.tcp.port),		\
	       NIPQUAD((tp)->dst.ip), ntohs((tp)->dst.u.tcp.port))
#else
#define DEBUGP(format, args...)
#define DUMP_CONTENT(dptr, length) 
#define DUMP_TUPLE_TCP(tp) 
#endif
extern 	int br_handle_frame_finish(struct sk_buff *skb);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
/* IP Hooks */
/* After promisc drops, checksum checks. */
#define NF_IP_PRE_ROUTING	0
/* If the packet is destined for this box. */
#define NF_IP_LOCAL_IN		1
/* If the packet is destined for another interface. */
#define NF_IP_FORWARD		2
/* Packets coming from a local process. */
#define NF_IP_LOCAL_OUT		3
/* Packets about to hit the wire. */
#define NF_IP_POST_ROUTING	4
#define NF_IP_NUMHOOKS		5

#endif

#define HTTP_PORT 80
/* This is slow, but it's simple. --RR */
static char http_buffer[16384];


//static char LanIP[50];

int redirect_on = 1;
//int auto_add = 1;
unsigned char defaultUrl[128];

#ifdef HTTP_REDIRECT_VERIFICATION
unsigned char required_url[512];
#define DEFAULT_REQ_URL "www.realtek.com/home.htm"
#endif

#define DEFAULT_URL "www.realtek.com"

#define MAX_RES_STR_LEN 512
#define MAX_HOST_STR_LEN 512
#define CHECK_ONLINE_INTERVAL 30

#define USE_HTTP_META
//#define USE_HTTP_302
#if defined(USE_HTTP_META)
char* pszHttpRedirectHead = 
	"HTTP/1.1 200 OK\r\n"
	"Server: router-gateway\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: %d\r\n"
	"Connection: Close\r\n"
	"\r\n"
	"%s";
char* pszHttpRedirectContent = 
	"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html\">"
	"<meta http-equiv=\"refresh\" content=\"0; url=http://%s\">"
	"<title>Welcome...</title>"
	"</head></html>";
#elif defined(USE_HTTP_302)
char* pszHttpRedirectHead = 
	"HTTP/1.1 302 Object Moved\r\n"
	"Location: http://%s\r\n"
	"Server: router-gateway\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: %d\r\n"
	"\r\n"
	"%s";
char* pszHttpRedirectContent = 
	"<html><head><title>Object Moved</title></head>"
	"<body><h1>Object Moved</h1>This Object may be found in "
	"<a HREF=\"http://%s\">here</a>.</body><html>";
#endif	
//DEFINE_SPINLOCK(guest_dev_list_lock);
#define MAX_CLIENT_NUM 500
#ifdef HTTP_REDIRECT_USER_TIMING
int max_idle_time = 3600;
#endif
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
struct list_head allowed_ip_list;
unsigned int current_entry_num = 0;
typedef struct allowed_ip_entry{
	uint32 ipaddr;
	int mask;
	//unsigned char ipaddr[20];/*IP address or IP address in VLSM,e.g 192.168.1.1 or 192.168.1.1/24*/
	struct list_head list;
}ipaddr_entry;
#else
struct list_head client_mac_list;
unsigned int client_num = 0;
typedef struct macaddr_entry{
	unsigned char mac[6];/*mac address*/
	struct list_head list;
	#ifdef HTTP_REDIRECT_USER_TIMING
	unsigned long updated;/*last visit time*/
	#endif
}macaddr_entry;
#endif
#ifdef HTTP_REDIRECT_USER_TIMING
struct timer_list		online_chk_timer;	//check online timer
#endif
#if defined(CONFIG_RTL_HTTP_REDIRECT_LOCAL)
extern 	void set_http_redirect_acl(int enable);
#endif
#if !defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
static int c_is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}

static int mac_string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !c_is_hex(tmpBuf[0]) || !c_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) simple_strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}
int is_mac_in_list(unsigned char* mac)
{
	struct macaddr_entry *tmp_entry;
	struct list_head *pos;
	if(mac == NULL)
	{
		return -1;
	}
	list_for_each(pos,&client_mac_list){
		tmp_entry = list_entry(pos,macaddr_entry,list);
		if(memcmp(mac,tmp_entry->mac,6)==0){
			return 1;
		}
	}
	return 0;
}
#endif
#ifdef HTTP_REDIRECT_USER_TIMING
static int update_client_online_time(unsigned char* mac)
{
	struct macaddr_entry *tmp_entry;
	struct list_head *pos;
	if(mac == NULL)
	{
		return -1;
	}
	list_for_each(pos,&client_mac_list){
		tmp_entry = list_entry(pos,macaddr_entry,list);
		if(memcmp(mac,tmp_entry->mac,6)==0){
			tmp_entry->updated = jiffies;
			break;
		}
	}
	return 0;
}
#endif
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
int is_ipaddr_in_list(uint32 ipaddr, int mask)
{
	ipaddr_entry *tmp_entry;
	struct list_head *pos;
	int found = 0;
	list_for_each(pos,&allowed_ip_list){
		tmp_entry = list_entry(pos,ipaddr_entry,list);
		if(mask != 0 && tmp_entry->mask != 0)
		{
			if((ipaddr & (0xffffffff<<(32-mask))) == 
				(tmp_entry->ipaddr & (0xffffffff<<(32-mask))))
			{
				found = 1;
				break;
			}
		}
		else
		{
			if(ipaddr == tmp_entry->ipaddr)
			{
				found = 1;
				break;
			}
		}
	}
	return found;
}
static int add_to_allowed_list(uint32 ipaddr,int mask)
{
	ipaddr_entry *new_entry,*tmp_entry;
	struct list_head *temp;
	if(is_ipaddr_in_list(ipaddr,mask))
	{
		return -1;
	}
	if(current_entry_num >= MAX_CLIENT_NUM)
	{
		return -1;
	}
	new_entry = kmalloc(sizeof(ipaddr_entry), GFP_KERNEL);
	if(new_entry == NULL)
	{
		return -1;
	}
	INIT_LIST_HEAD(&new_entry->list);
	new_entry->ipaddr = ipaddr;
	new_entry->mask = mask;
	list_add_tail(&new_entry->list,&allowed_ip_list);
	current_entry_num ++;
	return 0;
}
static int delete_from_allowed_list(uint32 ipaddr,int mask)
{
	ipaddr_entry *tmp_entry;
	struct list_head *q,*n;
	if(ipaddr == NULL)
	{
		return -1;
	}
	
	if(!list_empty(&allowed_ip_list)){
		list_for_each_safe(q,n,&allowed_ip_list){
			tmp_entry = list_entry(q,ipaddr_entry,list);
			if(ipaddr == tmp_entry->ipaddr && mask == tmp_entry->mask){
				list_del(&tmp_entry->list);
				kfree(tmp_entry);
				current_entry_num --;
			}
		}
	}
	return 0;
}
static int flush_allowed_list(void)
{
	ipaddr_entry *tmp_entry;
	struct list_head *q,*n;
	if(!list_empty(&allowed_ip_list)){
		list_for_each_safe(q,n,&allowed_ip_list){
			tmp_entry = list_entry(q,ipaddr_entry,list);
				list_del(&tmp_entry->list);
				kfree(tmp_entry);
		}
	}
	current_entry_num = 0;
	return 0;
}
#else
static int add_mac_to_list(unsigned char* mac)
{
	macaddr_entry *new_entry,*tmp_entry;
	struct list_head *temp;
	if(mac==NULL)
	{
		return -1;
	}
	if(is_mac_in_list(mac))
	{
		return 0;
	}
	if(client_num >= MAX_CLIENT_NUM)
	{
		temp = client_mac_list.next;
		tmp_entry = list_entry(temp,macaddr_entry,list);
		list_del(&tmp_entry->list);
		kfree(tmp_entry);
		tmp_entry = NULL;
		client_num --;
	}
	new_entry = kmalloc(sizeof(macaddr_entry), GFP_KERNEL);
	if(new_entry == NULL)
	{
		return -1;
	}
	INIT_LIST_HEAD(&new_entry->list);
	memcpy(new_entry->mac,mac,6);
#ifdef HTTP_REDIRECT_USER_TIMING
	new_entry->updated = jiffies;
#endif
	list_add_tail(&new_entry->list,&client_mac_list);
	client_num ++;
	return 0;
}
static int delete_mac_from_list(unsigned char* mac)
{
	macaddr_entry *tmp_entry;
	struct list_head *q,*n;
	if(mac == NULL)
	{
		return -1;
	}
	
	if(!list_empty(&client_mac_list)){
		list_for_each_safe(q,n,&client_mac_list){
			tmp_entry = list_entry(q,macaddr_entry,list);
			if(memcmp(tmp_entry->mac,mac,6)==0){
				list_del(&tmp_entry->list);
				kfree(tmp_entry);
				client_num --;
			}
		}
	}
	return 0;
}
static int flush_mac_list()
{
	macaddr_entry *tmp_entry;
	struct list_head *q,*n;
	if(!list_empty(&client_mac_list)){
		list_for_each_safe(q,n,&client_mac_list){
			tmp_entry = list_entry(q,macaddr_entry,list);
				list_del(&tmp_entry->list);
				kfree(tmp_entry);
		}
	}
	client_num = 0;
	return 0;
}
#endif

//static ushort http_port_array[MAX_LIST_SIZE] = { [0 ... (MAX_LIST_SIZE -1)] = 0 };

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static inline struct rtable *route_reverse(struct sk_buff *skb, int hook)
{
	struct iphdr *iph = ip_hdr(skb);
    struct dst_entry *odst;
	struct flowi4 fl = {};
//        struct rtable *rt;
	struct rtable *rt = skb_rtable(skb);
	struct net *net = dev_net(skb->dev);

    /* We don't require ip forwarding to be enabled to be able to
      * send a RST reply for bridged traffic. */
    if (hook != NF_IP_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
       || (skb->nf_bridge && skb->nf_bridge->mask & BRNF_BRIDGED)
#endif
     ){
       		fl.daddr = iph->saddr;
            if (hook == NF_IP_LOCAL_IN)
				fl.saddr = iph->daddr;
			fl.flowi4_tos = RT_TOS(iph->tos);
            if ((rt=ip_route_output_key(net, &fl)) == NULL){
				DEBUGP("return pl 1\n");
				return NULL;
			}
        } else {
                /* non-local src, find valid iif to satisfy
                 * rp-filter when calling ip_route_input. */
            	 	fl.daddr = iph->daddr;
                 	if ((rt=ip_route_output_key(net, &fl)) == NULL){
						DEBUGP("return pl 2\n");
						return NULL; 
					}

                odst = skb_dst(skb);
                if (ip_route_input(skb, iph->saddr, iph->daddr,
                                   RT_TOS(iph->tos), rt->dst.dev) != 0) {
                        dst_release(&rt->dst);
						DEBUGP("return pl 3\n");
                        return NULL;
                }
                dst_release(&rt->dst);
                rt = (struct rtable *)skb_dst(skb);
				skb_dst_set(skb,odst);
        }
 
      if (rt->dst.error) {
	  		dst_release(&rt->dst);
			DEBUGP("return pl 4\n");
            rt = NULL;
        }

	return rt;
}
#else
static inline struct rtable *route_reverse(struct sk_buff *skb, int hook)
{
	struct iphdr *iph = ip_hdr(skb);
    struct dst_entry *odst;
    struct flowi fl = {};
    struct rtable *rt;
	struct net *net = dev_net(skb->dev);

    /* We don't require ip forwarding to be enabled to be able to
        * send a RST reply for bridged traffic. */
      if (hook != NF_IP_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
          || (skb->nf_bridge && skb->nf_bridge->mask & BRNF_BRIDGED)
#endif
        ) {
               fl.nl_u.ip4_u.daddr = iph->saddr;
                if (hook == NF_IP_LOCAL_IN)
                        fl.nl_u.ip4_u.saddr = iph->daddr;
                fl.nl_u.ip4_u.tos = RT_TOS(iph->tos);
				
                if (ip_route_output_key(net,&rt, &fl) != 0)
                        return NULL;
        } else {
                /* non-local src, find valid iif to satisfy
                 * rp-filter when calling ip_route_input. */
                fl.nl_u.ip4_u.daddr = iph->daddr;
                if (ip_route_output_key(net,&rt, &fl) != 0)
                        return NULL; 

                odst = skb->dst;
                if (ip_route_input(skb, iph->saddr, iph->daddr,
                                   RT_TOS(iph->tos), rt->u.dst.dev) != 0) {
                        dst_release(&rt->u.dst);
                        return NULL;
                }
                dst_release(&rt->u.dst);
                rt = (struct rtable *)skb->dst;
                skb->dst = odst;
        }
 
      if (rt->u.dst.error) {
                dst_release(&rt->u.dst);
                rt = NULL;
        }

        return rt;
}
#endif



static int line_str_len(const char *line, const char *limit)
{
        const char *k = line;
        while ((line <= limit) && (*line == '\r' || *line == '\n'))
                line++;
        while (line <= limit) {
                if (*line == '\r' || *line == '\n')
                        break;
                line++;
        }
        return line - k;
}

static const char* line_str_search(const char *needle, const char *haystack, 
			size_t needle_len, size_t haystack_len) 
{
	const char *limit = haystack + (haystack_len - needle_len);
	while (haystack <= limit) {
		if (strnicmp(haystack, needle, needle_len) == 0)
			return haystack;
		haystack++;
	}
	return NULL;
}

static void send_redirect(struct sk_buff *skb, int hook)
{
	struct tcphdr otcph, *tcph;
	struct sk_buff *nskb;
	struct iphdr *nskb_iph;
	u_int16_t tmp_port;
	u_int32_t tmp_addr;	
	int dataoff;
	int needs_ack;	
	unsigned char mac[ETH_ALEN];
	unsigned int ip;
	unsigned short port;
	char szRedirectPack[512];
	char szRedirectContent[260];	
	char *dptr = NULL;
	struct rtable *rt;
	struct in_device *in_dev;
	struct net_device *dev;
	struct in_ifaddr **ifap = NULL;
	struct in_ifaddr *ifa = NULL;
	int ip_len;
	struct iphdr *iph = ip_hdr(skb);
	struct net *net = dev_net(skb->dev);

	/* IP header checks: fragment. */
	if (iph->frag_off & htons(IP_OFFSET)){
		DEBUGP("send_redirect:error in fragment\n");
		return;
	}

	if ((rt = route_reverse(skb, hook)) == NULL){
		printk("error when find route\n");
		return;
	}
	if (skb_copy_bits(skb, iph->ihl*4,
			  &otcph, sizeof(otcph)) < 0){
		DEBUGP("send_redirect:error in skb_copy_bits\n");
 		return;
	}

	if (otcph.rst)
		return;
	dataoff = iph->ihl*4 + otcph.doff*4;	

	/* No data? */
	if (dataoff >= skb->len) {
		DEBUGP("guest_access_help: skblen = %u\n", skb->len);
		return;
	}
	memset(http_buffer,0,sizeof(http_buffer));
	skb_copy_bits(skb, dataoff, http_buffer, skb->len - dataoff);

	if(line_str_search("HTTP", (const char*)http_buffer, strlen("HTTP"), 
		line_str_len((const char*)http_buffer, 
			(const char*)http_buffer + skb->len - dataoff)) == NULL) {
		DEBUGP("guest_access_tcp_help: is not http head pack\n");
		return;
	}
	DEBUGP("check HTTP ok\n");

	if(line_str_search("GET", (const char*)http_buffer, strlen("GET"), 
		line_str_len((const char*)http_buffer, 
			(const char*)http_buffer + skb->len - dataoff)) == NULL) {
		DEBUGP("guest_access_tcp_help: is not http head pack\n");
		return;
	}
	DEBUGP("check GET ok\n");

#if defined(USE_HTTP_META)
	sprintf(szRedirectContent, pszHttpRedirectContent,defaultUrl);
	sprintf(szRedirectPack, pszHttpRedirectHead ,
			strlen(szRedirectContent), szRedirectContent); 
#elif defined(USE_HTTP_302)
	sprintf(szRedirectContent, pszHttpRedirectContent, defaultUrl);
	sprintf(szRedirectPack, pszHttpRedirectHead, defaultUrl, 
		strlen(szRedirectContent), szRedirectContent); 
#endif
	//DEBUGP("packet content is %s", szRedirectPack);
	nskb = skb_copy_expand(skb, LL_MAX_HEADER, 
		skb_tailroom(skb) + strlen(szRedirectPack), GFP_ATOMIC);

	if (!nskb) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		dst_release(&rt->dst);
#else
		dst_release(&rt->u.dst);
#endif
		return;
	}

	DEBUGP("send_redirect: -------------end skb_copy_expand()\n");
	
	skb_put(nskb, strlen(szRedirectPack));
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	dst_release(skb_dst(nskb));
	skb_dst_set(nskb,&rt->dst);
#else
	dst_release(nskb->dst);
	nskb->dst = &rt->u.dst;
#endif

	DEBUGP("send_redirect: -------------before skb_put()\n");

	/* This packet will not be the same as the other: clear nf fields */
	nf_reset(nskb);
	//nskb->nfmark = 0;
	skb_init_secmark(nskb);
	nskb_iph=ip_hdr(nskb);

	tcph = (struct tcphdr *)((u_int32_t*)nskb_iph + nskb_iph->ihl);




	/* swap mac address */
	memcpy(mac, eth_hdr(nskb)->h_dest, ETH_ALEN);
	memcpy(eth_hdr(nskb)->h_dest, eth_hdr(nskb)->h_source, ETH_ALEN);
	memcpy(eth_hdr(nskb)->h_source, mac, ETH_ALEN);
		

	/* Swap source and dest */
	tmp_addr = nskb_iph->saddr;
	nskb_iph->saddr = nskb_iph->daddr;
	nskb_iph->daddr = tmp_addr;
	tmp_port = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp_port;

	/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, nskb_iph->ihl*4 + sizeof(struct tcphdr) + strlen(szRedirectPack));
	nskb_iph->tot_len = htons(nskb->len);

	if (tcph->ack) {
		tcph->seq = otcph.ack_seq;
	} else {		
		tcph->seq = 0;
	}

	tcph->ack_seq = htonl(ntohl(otcph.seq) + otcph.syn + otcph.fin
	      + skb->len - iph->ihl*4
	      - (otcph.doff<<2));
	needs_ack = 1;

	/* Reset flags */
	((u_int8_t *)tcph)[13] = 0;
	tcph->ack = needs_ack;
	tcph->psh = 1;

	tcph->window = 0;
	tcph->urg_ptr = 0;

	/* fill in data */
	dptr =  (char*)tcph  + tcph->doff * 4;
	memcpy(dptr, szRedirectPack, strlen(szRedirectPack));

	/* Adjust TCP checksum */
	tcph->check = 0;
	tcph->check = tcp_v4_check( sizeof(struct tcphdr) + strlen(szRedirectPack),
				   nskb_iph->saddr,
				   nskb_iph->daddr,
				   csum_partial((char *)tcph,
						sizeof(struct tcphdr) + strlen(szRedirectPack), 0));

	/* Set DF, id = 0 */
	nskb_iph->frag_off = htons(IP_DF);
	nskb_iph->id = 0;

	nskb->ip_summed = CHECKSUM_NONE;
	nskb->http_redirect = 1;

	/* Adjust IP TTL, DF */
	nskb_iph->ttl = MAXTTL;

	/* Adjust IP checksum */
	nskb_iph->check = 0;
	nskb_iph->check = ip_fast_csum((unsigned char *)nskb_iph, 
					   nskb_iph->ihl);

	/* "Never happens" */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, skb);
	//NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, nskb, NULL, nskb->dst->dev,
	//	dst_output);
	DEBUGP("orig src mac=%02x:%02x:%02x:%02x:%02x:%02x \
	dst mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			eth_hdr(skb)->h_source[0],
			eth_hdr(skb)->h_source[1],
			eth_hdr(skb)->h_source[2],
			eth_hdr(skb)->h_source[3],
			eth_hdr(skb)->h_source[4],
			eth_hdr(skb)->h_source[5],
			eth_hdr(skb)->h_dest[0],
			eth_hdr(skb)->h_dest[1],
			eth_hdr(skb)->h_dest[2],
			eth_hdr(skb)->h_dest[3],
			eth_hdr(skb)->h_dest[4],
			eth_hdr(skb)->h_dest[5]
			);
	DEBUGP("src ip=%x\n dst ip=%x\n",ip_hdr(skb)->saddr,ip_hdr(skb)->daddr);
	
		
	DEBUGP("changed src mac=%02x:%02x:%02x:%02x:%02x:%02x \
	dst mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			eth_hdr(nskb)->h_source[0],
			eth_hdr(nskb)->h_source[1],
			eth_hdr(nskb)->h_source[2],
			eth_hdr(nskb)->h_source[3],
			eth_hdr(nskb)->h_source[4],
			eth_hdr(nskb)->h_source[5],
			eth_hdr(nskb)->h_dest[0],
			eth_hdr(nskb)->h_dest[1],
			eth_hdr(nskb)->h_dest[2],
			eth_hdr(nskb)->h_dest[3],
			eth_hdr(nskb)->h_dest[4],
			eth_hdr(nskb)->h_dest[5]
			);
	DEBUGP("src ip=%x:%d\n dst ip=%x:%d\n",nskb_iph->saddr,tcph->source,nskb_iph->daddr,tcph->dest);
	NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, nskb, nskb->dev,
			    NULL, br_handle_frame_finish);
//	dev_queue_xmit(nskb);
//------------------------------------------------------
//contine the oldskb send, modify oldskb as a "reset" tcp pack
//------------------------------------------------------

	tcph = (struct tcphdr *)((u_int32_t*)iph + iph->ihl);
	
	/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(skb, iph->ihl*4 + sizeof(struct tcphdr));
	iph->tot_len = htons(skb->len);

	/* Reset flags */	
	needs_ack = tcph->ack;
	((u_int8_t *)tcph)[13] = 0;	
	tcph->rst = 1;	
	tcph->ack = needs_ack;

	tcph->window = 0;
	tcph->urg_ptr = 0;

	/* Adjust TCP checksum */
	tcph->check = 0;
	tcph->check = tcp_v4_check( sizeof(struct tcphdr),
	   iph->saddr,
	   iph->daddr,
	   csum_partial((char *)tcph,
			sizeof(struct tcphdr), 0));

	/* Adjust IP TTL, DF */
	iph->ttl = MAXTTL;
	/* Set DF, id = 0 */
	iph->frag_off = htons(IP_DF);
	iph->id = 0;

	/* Adjust IP checksum */
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, 
		iph->ihl);


	return;

 free_nskb:
	kfree_skb(nskb);	
}
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
static int ipaddr_convert(char* ipaddr_in,u_int32_t *addr,int *mask)/*Convert ip address from VLSM to uint32*/
{
	char ip[20] = {0};
	*addr = 0;
	*mask = 0;
	char* ptr;
	*addr = *mask = 0;
	if(ipaddr_in == NULL || addr == NULL || mask == NULL)
	{
		return -1;
	}
	DEBUGP("[%s:%d]ipaddr_in:%s\n",__FUNCTION__,__LINE__,ipaddr_in);
	strcpy(ip,ipaddr_in);
	if(ptr=strchr(ipaddr_in,'/'))/*in VLSM format*/
	{
		ip[strlen(ipaddr_in)-strlen(ptr)] = '\0';
		*mask = simple_strtol(++ptr,(char**)NULL,10);
	}
	*addr = ntohl(in_aton(ip));
	if(*mask < 0 || *mask > 31)
	{
		return -1;
	}
	DEBUGP("[%s:%d]ip:%s\n",__FUNCTION__,__LINE__,ip);
	DEBUGP("[%s:%d]*addr:0x%x\n",__FUNCTION__,__LINE__,*addr);
	DEBUGP("[%s:%d]*mask:%d\n",__FUNCTION__,__LINE__,*mask);
	return 0;
}
static int is_allowed_ip(uint32 ip_addr)
{
	ipaddr_entry *tmp_entry;
	struct list_head *pos;
	int found = 0;
	list_for_each(pos,&allowed_ip_list){
		tmp_entry = list_entry(pos,ipaddr_entry,list);
		if(tmp_entry->mask != 0)
		{
			if((tmp_entry->ipaddr & (0xffffffff<<(32-tmp_entry->mask))) ==
			    (ip_addr & (0xffffffff<<(32-tmp_entry->mask))))
			{
				found = 1;
				break;
			}
		}
		else
		{
			if(ip_addr == tmp_entry->ipaddr)
			{
				found = 1;
				break;
			}
		}
	}
	return found;
}
#endif
static int parse_url(char url[],char host[],char resource[])
{
	int i,len;
	int found = 0;
	if(url == NULL||host == NULL||resource ==NULL)
	{
		return -1;
	}
	len = strlen(url);
	for(i=0;i<len;i++)
	{
		if(url[i] == '/')
		{
			found = 1;
			break;
		}
	}
	strncpy(host,url,i);
	if(found)
	{
		strncpy(resource,&url[i],len-i);
	}
	else
	{
		strcpy(resource,"/");
	}
	if(strlen(resource)!=1&&resource[strlen(resource)-1]=='/')
	{
		resource[strlen(resource)-1] = '\0';
	}
}
static int parse_header(char buffer[],char resource[],char host[],char referer[])
{
	char *p,*q;
	int i;
	char* const delim = "\n";
	char* token,*cur = buffer;
	if(buffer == NULL||
	   resource==NULL||
	   host==NULL)
	{
		return -1;
	}
	//DEBUGP("[%s]buffer:\n%s\n",__FUNCTION__,buffer);
	while(token=strsep(&cur,delim))
	{
		if(strncmp(token,"GET",3)==0)
		{
			for(i=3,p=token+i;*p!='/';p++,i++);/*GET /xxx HTTP/1.1*/
			for(q=p;q!=NULL;q++)
			{
				if(*q == ' ')
				{
					*q = '\0';
					break;
				}
			}
			strncpy(resource,token+i,MAX_RES_STR_LEN);
			if(strlen(resource)!=1 && resource[strlen(resource)-1]=='/')
			{
				resource[strlen(resource)-1] = '\0';
			}
		}
		else if(strncmp(token,"Host",4)==0)
		{
			for(i=5,p=token+i;*p==' ';p++,i++);/*Host: xxx.xxx*/
			for(q=p;q!=NULL;q++)
			{
				if(*q=='\r'||*q=='\n')
				{
					*q = '\0';
					break;
				}
			}
			strncpy(host,token+i,MAX_HOST_STR_LEN);
		}
		else if(strncmp(token,"Referer",7)==0)
		{
			for(i=8,p=token+i;*p==' ';p++,i++);/*Referer: http://xxxx.xxx.com*/
			for(q=p;q!=NULL;q++)
			{
				if(*q=='\r'||*q=='\n')
				{
					*q = '\0';
					break;
				}
			}
			strncpy(referer,token+i,MAX_HOST_STR_LEN);
		}
	}
	return 0;
}
static int is_required_url(struct sk_buff *skb,int *is_get_method)
{
	struct tcphdr otcph;
	int dataoff;
	if(skb == NULL || is_get_method == NULL)
		return 0;
	*is_get_method = 0;
	struct iphdr *iph = ip_hdr(skb);
	char resource[MAX_RES_STR_LEN] = {0};
	char host[MAX_HOST_STR_LEN] = {0};
	char referer[MAX_HOST_STR_LEN] = {0};
	
	//char def_resource[MAX_RES_STR_LEN] = {0};
	//char def_host[MAX_HOST_STR_LEN] = {0};
	char req_host[MAX_HOST_STR_LEN] = {0};
	char req_url[MAX_RES_STR_LEN] = {0};

	if (iph->frag_off & htons(IP_OFFSET)){
		DEBUGP("error in fragment\n");
		return 0;
	}
	if (skb_copy_bits(skb, iph->ihl*4,&otcph, sizeof(otcph)) == 0){
		if (otcph.rst == 0){
			dataoff = iph->ihl*4 + otcph.doff*4;
			if (dataoff < skb->len){
				memset(http_buffer,0,sizeof(http_buffer));
				skb_copy_bits(skb, dataoff, http_buffer, skb->len - dataoff);
			}
			if(line_str_search("HTTP", (const char*)http_buffer, strlen("HTTP"), 
				line_str_len((const char*)http_buffer, 
					(const char*)http_buffer + skb->len - dataoff)) == NULL) {
				//DEBUGP("guest_access_tcp_help: is not http head pack\n");
				return 0;
			}
			if(line_str_search("GET", (const char*)http_buffer, strlen("GET"), 
				line_str_len((const char*)http_buffer, 
					(const char*)http_buffer + skb->len - dataoff)) == NULL) {
				//DEBUGP("guest_access_tcp_help: is not http head pack\n");
				return 0;
			}
			*is_get_method = 1;
			DEBUGP("==========\n%s===========\n",http_buffer);
			if(parse_header(http_buffer,resource,host,referer)<0)
			{
				return 0;
			}
			//parse_url(defaultUrl,def_host,def_resource);
#ifdef HTTP_REDIRECT_VERIFICATION
			parse_url(required_url,req_host,req_url);
#else
			parse_url(defaultUrl,req_host,req_url);
#endif
			DEBUGP("resource:%s\n",resource);
			DEBUGP("host:%s\n",host);
			DEBUGP("required_host:%s,required_url:%s\n",req_host,req_url);
			if(strcasecmp(resource,req_url)==0&&strcasecmp(host,req_host)==0)
			{
				return 1;
			}
		}
	}
	return 0;
}
static int is_default_host(struct sk_buff *skb)
{
	struct tcphdr otcph;
	int dataoff;
	struct iphdr *iph = ip_hdr(skb);
	
	char resource[MAX_RES_STR_LEN] = {0};
	char host[MAX_HOST_STR_LEN] = {0};
	char def_resource[MAX_RES_STR_LEN] = {0};
	char def_host[MAX_HOST_STR_LEN] = {0};
	char referer[MAX_HOST_STR_LEN] = {0};
	char req_host[MAX_HOST_STR_LEN] = {0};
	char req_url[MAX_RES_STR_LEN] = {0};


	if (iph->frag_off & htons(IP_OFFSET)){
		DEBUGP("error in fragment\n");
		return 0;
	}

	if (skb_copy_bits(skb, iph->ihl*4,&otcph, sizeof(otcph)) == 0){
		if (otcph.rst == 0){
			dataoff = iph->ihl*4 + otcph.doff*4;
			if (dataoff < skb->len){
				memset(http_buffer,0,sizeof(http_buffer));
				skb_copy_bits(skb, dataoff, http_buffer, skb->len - dataoff);
			}
			//DEBUGP("==========\n%s===========\n",http_buffer);
			if(line_str_search("HTTP", (const char*)http_buffer, strlen("HTTP"), 
				line_str_len((const char*)http_buffer, 
					(const char*)http_buffer + skb->len - dataoff)) == NULL) {
				DEBUGP("guest_access_tcp_help: is not http head pack\n");
				return 0;
			}
			if(line_str_search("GET", (const char*)http_buffer, strlen("GET"), 
				line_str_len((const char*)http_buffer, 
					(const char*)http_buffer + skb->len - dataoff)) == NULL) {
				DEBUGP("guest_access_tcp_help: is not http head pack\n");
				return 0;
			}
			if(parse_header(http_buffer,resource,host,referer)<0)
			{
				return 0;
			}
			parse_url(defaultUrl,def_host,def_resource);
#ifdef HTTP_REDIRECT_VERIFICATION
			parse_url(required_url,req_host,req_url);
#endif
			//DEBUGP("resource:%s\n",resource);
			//DEBUGP("host:%s\n",host);
			//DEBUGP("defaultUrl:%s\n",defaultUrl);
			//DEBUGP("def_host:%s,def_resource:%s\n",def_host,def_resource);
			DEBUGP("referer:%s!!!\n",referer);
			
			if(strstr(host,def_host) || 
#ifdef HTTP_REDIRECT_VERIFICATION
			   strstr(host,req_host) || 
#endif
			   strstr(referer,def_host))
			{
				return 1;
			}
		}
	}
	return 0;
}
int is_http_packet(struct sk_buff *skb)
{
	struct tcphdr tcph;
	struct iphdr *iph = NULL;
	if(skb == NULL)
		return 0;
	iph = (void*)(skb->data);
	if (iph->protocol == IPPROTO_TCP){
		if (skb_copy_bits(skb, iph->ihl*4, &tcph, sizeof(tcph)) == 0){
			if(ntohs(tcph.dest) == 80){
				return 1;
			}
		}
	}
	return 0;
}
int is_http_resp_packet(struct sk_buff *skb)
{
	struct tcphdr tcph;
	struct iphdr *iph = NULL;
	if(skb == NULL)
		return 0;
	return skb->http_redirect;
}
static int get_br0_ip_mask(u_int32_t *ip,u_int32_t *mask)
{
	struct net_device *br0_dev; 
	struct in_device *br0_in_dev;

	if(!ip||!mask){
		return -1;
	}
	
	br0_dev = dev_get_by_name(&init_net,"br0"); 
	br0_in_dev = in_dev_get(br0_dev);
	if(!br0_dev || !br0_in_dev)	
	{
		if(br0_in_dev)
			in_dev_put(br0_in_dev);
		if(br0_dev)
			dev_put(br0_dev);
		return -1;
	}
	
	*ip = *((u_int32_t *)&br0_in_dev->ifa_list->ifa_address);
	*mask = *((u_int32_t *)&br0_in_dev->ifa_list->ifa_mask);

    if(br0_in_dev)
        in_dev_put(br0_in_dev);
    if(br0_dev)
        dev_put(br0_dev);
	return 0;
}
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
int rtl_http_redirect_tr098(struct sk_buff *skb)
{
	struct tcphdr tcph;
	struct tcphdr otcph;
	struct iphdr *iph = (void*)(skb->data);
	int is_http_get = 0;

	u_int32_t dev_ip = 0;
	u_int32_t dev_mask = 0;

	if(!redirect_on ||!strlen(defaultUrl)|| iph->protocol != IPPROTO_TCP)
	{
		return 0;
	}

	if (skb_copy_bits(skb, iph->ihl*4, &tcph, sizeof(tcph)) != 0) {
			DEBUGP("guest_access_tcp_help: skb_copy_bits(tcp) failed\n");
			return 0;
	}
	if ((ntohs(iph->tot_len)  -iph->ihl*4 -tcph.doff*4) == 0 )
	{
		return 0;
	}

	if(ntohs(tcph.dest) != 80)
	{//we only interest in http pockets
		return 0;
	}
	DEBUGP("[%s:%d]iph->daddr:0x%x\n",__FUNCTION__,__LINE__,ntohl(iph->daddr));
	get_br0_ip_mask(&dev_ip,&dev_mask);
	if(iph->daddr == dev_ip)
	{
		DEBUGP("[%s:%d]to br0!!\n",__FUNCTION__,__LINE__);
		return 0;
	}
	if(is_allowed_ip(ntohl(iph->daddr)))
	{
		DEBUGP("[%s:%d]Dest address in allowed list!!!!!\n",__FUNCTION__,__LINE__);
		return 0;
	}
	if(is_required_url(skb,&is_http_get))
	{
		DEBUGP("[%s:%d]CaptivePortalURL!\n");
		return 0;
	}
	if(is_http_get == 0)
	{
		return 0;
	}
redirect:
	DEBUGP("---redirect %x\n", iph->daddr);
	if (skb_copy_bits(skb, iph->ihl*4, &tcph, sizeof(tcph)) != 0) {
		DEBUGP("guest_access_tcp_help: skb_copy_bits(tcp) failed\n");
		return 0;
	}
	send_redirect(skb,NF_IP_PRE_ROUTING);
	return 0;
}
#else
int rtl_http_redirect(struct sk_buff *skb)
{
	struct tcphdr tcph;
	struct tcphdr otcph;
	struct iphdr *iph = (void*)(skb->data);
	u_int32_t dev_ip = 0;
	u_int32_t dev_mask = 0;
	int is_local = 0;
	//unsigned char mac[6];
	unsigned char* mac;
	int is_http_get = 0;

	 if(!redirect_on)
	 {
	 	return 0;
	 }

	if (iph->protocol == IPPROTO_TCP)
	{
		if (skb_copy_bits(skb, iph->ihl*4, &tcph, sizeof(tcph)) != 0) {
				DEBUGP("guest_access_tcp_help: skb_copy_bits(tcp) failed\n");
				return 0;
		}
		if ((ntohs(iph->tot_len)  -iph->ihl*4 -tcph.doff*4) == 0 )
		{
			return 0;
		}

		if(ntohs(tcph.dest) != 80)
		{//we only interest in http pockets
			return 0;
		}
		//memcpy(mac,&skb_mac_header(skb)[6],6);
		mac = skb_mac_header(skb)+6;
		DEBUGP("smac:%02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0],
			mac[1],
			mac[2],
			mac[3],
			mac[4],
			mac[5]);
		if(is_mac_in_list(mac))
		{
#ifdef HTTP_REDIRECT_USER_TIMING
			update_client_online_time(mac);
#endif
			return 0;
		}
		if(is_required_url(skb,&is_http_get))
		{
			DEBUGP("[%s:%d]Get required page!!!!!\n",__FUNCTION__,__LINE__);
			add_mac_to_list(mac);
			return 0;
		}
		if(is_http_get == 0)/*only intrest in http GET packets*/
		{
			DEBUGP("[%s:%d]NOT GET method!\n",__FUNCTION__,__LINE__);
			return 0;
		}
		if(get_br0_ip_mask(&dev_ip,&dev_mask)==-1)
		{
			return -1;
		}
		DEBUGP("dev_ip:%x,dev_mask:%x\n",dev_ip,dev_mask);
		if(dev_ip == iph->daddr)
		{
			return 0;
		}
		else
		{
			if(!is_default_host(skb))
			{
				goto redirect;
			}
			else
			{
				return 0;
			}
		}
redirect:
		DEBUGP("---redirect %x\n", iph->daddr);
		if (skb_copy_bits(skb, iph->ihl*4, &tcph, sizeof(tcph)) != 0) {
			DEBUGP("guest_access_tcp_help: skb_copy_bits(tcp) failed\n");
			return 0;
		}
		send_redirect(skb,NF_IP_PRE_ROUTING);
		return 0;
	}
	return 0;
}
#endif
unsigned int http_redirect_enter(struct sk_buff *skb)
{
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
	return rtl_http_redirect_tr098(skb);
#else
	return rtl_http_redirect(skb);
#endif
}

#ifdef CONFIG_PROC_FS

#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_en_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%d\n",redirect_on);
	return 0;
}
#else
static int rtl_redirect_en_read(char *page, char **start, off_t off,
			int count, int *eof, void *data)
{
	int len;
    len = sprintf(page, "%d\n",redirect_on);
    if (len <= off+count) *eof = 1;
    *start = page + off;
    len -= off;
    if (len>count)
    	len = count;
    if (len<0)
       len = 0;
    return len;
}
#endif
static int rtl_redirect_en_write(struct file *file, const char *buffer,unsigned long count, void *data)
{
	unsigned int tmp=0; 
	char 		tmpbuf[32];
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count-1] = '\0';
		tmp=simple_strtol(tmpbuf, NULL, 0);
		redirect_on = tmp? 1 : 0;
#if defined(CONFIG_RTL_HTTP_REDIRECT_LOCAL)
		set_http_redirect_acl(redirect_on);
#endif
		return count;
	}
	return -EFAULT;
}

#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_en_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_en_read,NULL));
}
int rtl_redirect_en_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_en_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_en_proc_fops= {
        .open           = rtl_redirect_en_proc_open,
        .write		    = rtl_redirect_en_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif

#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_host_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%s\n",defaultUrl);
	return 0;
}
#else
static int rtl_redirect_host_read(char *page, char **start, off_t off,int count, int *eof, void *data)
{

	int len=0;
	len = sprintf(page, "%s\n", defaultUrl);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif

static int rtl_redirect_host_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char 		tmpbuf[128];
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count-1] = '\0';
		if(!strncmp(tmpbuf,"http://",strlen("http://")))
			strcpy(defaultUrl,tmpbuf+strlen("http://"));
		else
			strcpy(defaultUrl,tmpbuf);
		if(defaultUrl[strlen(defaultUrl)-1] == '/')
			defaultUrl[strlen(defaultUrl)-1] = '\0';
		return count;
	}
	return -EFAULT;
}

#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_host_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_host_read,NULL));
}
int rtl_redirect_host_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_host_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_host_proc_fops= {
        .open           = rtl_redirect_host_proc_open,
        .write		    = rtl_redirect_host_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_allowed_list_read(struct seq_file *s, void *v)
{
	ipaddr_entry *tmp_entry;
	struct list_head *pos;

	int len=0;
	seq_printf(s,"IP Address\n");
	list_for_each(pos,&allowed_ip_list){
		tmp_entry = list_entry(pos,ipaddr_entry,list);
		if(tmp_entry->mask != 0)
		{
			seq_printf(s,"%d.%d.%d.%d/%d\n",
									(tmp_entry->ipaddr&0xff000000)>>24,
									(tmp_entry->ipaddr&0x00ff0000)>>16,
									(tmp_entry->ipaddr&0x0000ff00)>>8,
									(tmp_entry->ipaddr&0x000000ff)>>0,
									tmp_entry->mask);
		}
		else
		{
			seq_printf(s,"%d.%d.%d.%d\n",
									(tmp_entry->ipaddr&0xff000000)>>24,
									(tmp_entry->ipaddr&0x00ff0000)>>16,
									(tmp_entry->ipaddr&0x0000ff00)>>8,
									(tmp_entry->ipaddr&0x000000ff)>>0);
		}
	}
	return 0;
}
#else/*CONFIG_RTL_PROC_NEW*/
static int rtl_redirect_allowed_list_read(char *page, char **start, off_t off,int count, int *eof, void *data)
{
	struct ipaddr_entry *tmp_entry;
	struct list_head *pos;

	int len=0;
	len = sprintf(page,"IP Address\n");
	list_for_each(pos,&allowed_ip_list){
		tmp_entry = list_entry(pos,ipaddr_entry,list);
		if(tmp_entry->mask != 0)
		{
			len += sprintf(page+len,"%d.%d.%d.%d/%d",
									(tmp_entry->ipaddr&0xff000000)>>24,
									(tmp_entry->ipaddr&0x00ff0000)>>16,
									(tmp_entry->ipaddr&0x0000ff00)>>8,
									(tmp_entry->ipaddr&0x000000ff)>>0,
									tmp_entry->mask);
		}
		else
		{
			len += sprintf(page+len,"%d.%d.%d.%d",
									(tmp_entry->ipaddr&0xff000000)>>24,
									(tmp_entry->ipaddr&0x00ff0000)>>16,
									(tmp_entry->ipaddr&0x0000ff00)>>8,
									(tmp_entry->ipaddr&0x000000ff)>>0);
		}
	}
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif/*CONFIG_RTL_PROC_NEW*/

static int is_valid_ipaddr(char* str,uint32 *ipaddr,int *mask)
{
	int ret = ipaddr_convert(str,ipaddr,mask);
	if(ret < 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
static int rtl_redirect_allowed_list_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char tmpbuf[32];
	char operation[16];
	char ipstr[32];
	unsigned char mac[6];
	ipaddr_entry *new_entry,*tmp_entry;
	struct list_head *q,*n;
	uint32 ipaddr;
	int mask;
	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count] = '\0';
		sscanf(tmpbuf,"%s %s",operation,ipstr);
		if(strcmp(operation,"flush")==0)//flush the list
		{
			flush_allowed_list();
			return count;
		}
		if(strcmp(operation,"add")==0){
			if(is_valid_ipaddr(ipstr,&ipaddr,&mask))
			{
				add_to_allowed_list(ipaddr,mask);
			}
		}
		else if(strcmp(operation,"delete")==0){
			if(is_valid_ipaddr(ipstr,&ipaddr,&mask))
			{
				delete_from_allowed_list(ipaddr,mask);
			}
		}
		return count;
	}
}

#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_allowed_list_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_allowed_list_read,NULL));
}
int rtl_redirect_allowed_list_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_allowed_list_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_allowed_list_proc_fops= {
        .open           = rtl_redirect_allowed_list_proc_open,
        .write		    = rtl_redirect_allowed_list_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif/*CONFIG_RTL_PROC_NEW*/

#else/*CONFIG_RTL_HTTP_REDIRECT_TR098*/
#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_client_read(struct seq_file *s, void *v)
{	char mac[32];
	struct macaddr_entry *tmp_entry;
	struct list_head *pos;

	int len=0;
#ifdef HTTP_REDIRECT_USER_TIMING
	seq_printf(s, "MAC\t\t\tIdle time(seconds)\n");
#else
	seq_printf(s, "MAC\n");
#endif
	list_for_each(pos,&client_mac_list){
		tmp_entry = list_entry(pos,macaddr_entry,list);
		sprintf(mac,"%02X:%02X:%02X:%02X:%02X:%02X",
			tmp_entry->mac[0],
			tmp_entry->mac[1],
			tmp_entry->mac[2],
			tmp_entry->mac[3],
			tmp_entry->mac[4],
			tmp_entry->mac[5]);
#ifdef HTTP_REDIRECT_USER_TIMING
		seq_printf(s,"%s\t%ld\n",mac,(jiffies-tmp_entry->updated)/HZ);
#else
		seq_printf(s,"%s\n",mac);
#endif
	}
	return 0;
}
#else/*CONFIG_RTL_PROC_NEW*/
static int rtl_redirect_client_read(char *page, char **start, off_t off,int count, int *eof, void *data)
{
	char mac[32];
	struct macaddr_entry *tmp_entry;
	struct list_head *pos;

	int len=0;
#ifdef HTTP_REDIRECT_USER_TIMING
	len = sprintf(page, "MAC\t\t\tIdle time(seconds)\n");
#else
	len = sprintf(page, "MAC\n");
#endif
	list_for_each(pos,&client_mac_list){/*fixed entry*/
		tmp_entry = list_entry(pos,macaddr_entry,list);
		sprintf(mac,"%02X:%02X:%02X:%02X:%02X:%02X",
			tmp_entry->mac[0],
			tmp_entry->mac[1],
			tmp_entry->mac[2],
			tmp_entry->mac[3],
			tmp_entry->mac[4],
			tmp_entry->mac[5]);
#ifdef HTTP_REDIRECT_USER_TIMING
		len += sprintf(page+len,"%s\t%ld\n",mac,(jiffies-tmp_entry->updated)/HZ);
#else
		len += sprintf(page+len,"%s\n",mac);
#endif
	}
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif

static int rtl_redirect_client_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char tmpbuf[32];
	char op[16];
	char mac_str[32];
	unsigned char mac[6];
	macaddr_entry *new_entry,*tmp_entry;
	struct list_head *q,*n;
	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count] = '\0';
		sscanf(tmpbuf,"%s %s",op,mac_str);
		if(strcmp(op,"flush")==0)//flush the list
		{
			flush_mac_list();
			return count;
		}
		if(strlen(mac_str)!=12||!mac_string_to_hex(mac_str,mac,12)){
			return count;
		}
		if(strcmp(op,"add")==0){
			add_mac_to_list(mac);
		}
		else if(strcmp(op,"delete")==0){
			delete_mac_from_list(mac);
		}
		return count;
	}
}

#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_client_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_client_read,NULL));
}
int rtl_redirect_client_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_client_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_client_proc_fops= {
        .open           = rtl_redirect_client_proc_open,
        .write		    = rtl_redirect_client_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
#endif/*CONFIG_RTL_HTTP_REDIRECT_TR098*/
#ifdef HTTP_REDIRECT_VERIFICATION
#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_required_url_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%s\n",required_url);
	return 0;
}
#else
static int rtl_redirect_required_url_read(char *page, char **start, off_t off,int count, int *eof, void *data)
{

	int len=0;
	len = sprintf(page, "%s\n", required_url);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif
static int rtl_redirect_required_url_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char tmpbuf[512];
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count-1] = '\0';
		if(!strncmp(tmpbuf,"http://",strlen("http://")))
			strcpy(required_url,tmpbuf+strlen("http://"));
		else
			strcpy(required_url,tmpbuf);
		return count;
	}
	return -EFAULT;
}
#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_required_url_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_required_url_read,NULL));
}
int rtl_redirect_required_url_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_required_url_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_required_url_proc_fops= {
        .open           = rtl_redirect_required_url_proc_open,
        .write		    = rtl_redirect_required_url_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif

#endif/*HTTP_REDIRECT_VERIFICATION*/

#if 0/***************/
#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_auto_add_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%d\n",auto_add);
	return 0;
}
#else
static int rtl_redirect_auto_add_read(char *page, char **start, off_t off,
			int count, int *eof, void *data)
{
	int len;
    len = sprintf(page, "%d\n",auto_add);
    if (len <= off+count) *eof = 1;
    *start = page + off;
    len -= off;
    if (len>count)
    	len = count;
    if (len<0)
       len = 0;
    return len;
}
#endif
static int rtl_redirect_auto_add_write(struct file *file, const char *buffer,unsigned long count, void *data)
{
	unsigned int tmp=0; 
	char 		tmpbuf[32];
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		tmpbuf[count-1] = '\0';
		tmp=simple_strtol(tmpbuf, NULL, 0);
		auto_add = tmp? 1 : 0;
		return count;
	}
	return -EFAULT;
}

#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_auto_add_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_auto_add_read,NULL));
}
int rtl_redirect_auto_add_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_auto_add_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_auto_add_proc_fops= {
        .open           = rtl_redirect_auto_add_proc_open,
        .write		    = rtl_redirect_auto_add_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
#endif/***************/

#ifdef HTTP_REDIRECT_USER_TIMING
#ifdef CONFIG_RTL_PROC_NEW
static int rtl_redirect_max_idle_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%ld\n",max_idle_time);
	return 0;
}
#else
static int rtl_redirect_max_idle_read(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{
	int len=0;
	len = sprintf(page, "%ld\n", max_idle_time);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif
static int rtl_redirect_max_idle_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char 		tmpbuf[32];
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) 
	{
		max_idle_time = simple_strtol(tmpbuf,(char**)NULL,10);
		return count;
	}
	return -EFAULT;
}
#ifdef CONFIG_RTL_PROC_NEW
int rtl_redirect_max_idle_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, rtl_redirect_max_idle_read,NULL));
}
int rtl_redirect_max_idle_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return rtl_redirect_max_idle_write(file,userbuf,count,off);
}

struct file_operations rtl_redirect_max_idle_proc_fops= {
        .open           = rtl_redirect_max_idle_proc_open,
        .write		    = rtl_redirect_max_idle_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif/*CONFIG_RTL_PROC_NEW*/
#endif/*HTTP_REDIRECT_USER_TIMING*/
#endif/*end of CONFIG_PROC_FS*/
void http_redirect_fini(void);
#ifdef CONFIG_PROC_FS
void http_redirect_create_proc(void)
{
#ifdef CONFIG_RTL_PROC_NEW
	redirect_proc_root = proc_mkdir(HTTP_REDIRECT_PROC_ROOT,&proc_root);
	if(redirect_proc_root){
		proc_create_data(HTTP_REDIRECT_PROC_ENABLE,0,redirect_proc_root,&rtl_redirect_en_proc_fops,NULL);
		proc_create_data(HTTP_REDIRECT_PROC_HOST,0,redirect_proc_root,&rtl_redirect_host_proc_fops,NULL);
		#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
		proc_create_data(HTTP_REDIRECT_PROC_ALLOWED_LIST,0,redirect_proc_root,&rtl_redirect_allowed_list_proc_fops,NULL);
		#else
		proc_create_data(HTTP_REDIRECT_PROC_CLIENT_LIST,0,redirect_proc_root,&rtl_redirect_client_proc_fops,NULL);
		#endif
		#ifdef HTTP_REDIRECT_VERIFICATION		
		proc_create_data(HTTP_REDIRECT_PROC_REQ_URL,0,redirect_proc_root,&rtl_redirect_required_url_proc_fops,NULL);
		#endif
		#ifdef HTTP_REDIRECT_USER_TIMING
		proc_create_data(HTTP_REDIRECT_PROC_MAX_IDLE_TIME,0,redirect_proc_root,&rtl_redirect_max_idle_proc_fops,NULL);
		#endif
	}
#else
	redirect_proc_root = proc_mkdir(HTTP_REDIRECT_PROC_ROOT, NULL);
	if (!redirect_proc_root){
		printk("create folder fail\n");
		return;
	}
	redirect_proc_enable = create_proc_entry(HTTP_REDIRECT_PROC_ENABLE, 0, redirect_proc_root);
	if (redirect_proc_enable) {
		redirect_proc_enable->read_proc = rtl_redirect_en_read;
		redirect_proc_enable->write_proc = rtl_redirect_en_write;
	}
	redirect_proc_host = create_proc_entry(HTTP_REDIRECT_PROC_HOST, 0, redirect_proc_root);
	if(redirect_proc_host){
		redirect_proc_host->read_proc = rtl_redirect_host_read;
		redirect_proc_host->write_proc = rtl_redirect_host_write;
	}
	
	#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
	redirect_proc_allowedList= create_proc_entry(HTTP_REDIRECT_PROC_ALLOWED_LIST, 0, redirect_proc_root);
	if(redirect_proc_allowedList){
		redirect_proc_allowedList->read_proc = rtl_redirect_allowed_list_read;
		redirect_proc_allowedList->write_proc = rtl_redirect_allowed_list_write;
	}
	#else
	redirect_proc_clientList = create_proc_entry(HTTP_REDIRECT_PROC_CLIENT_LIST, 0, redirect_proc_root);
	if(redirect_proc_clientList){
		redirect_proc_clientList->read_proc = rtl_redirect_client_read;
		redirect_proc_clientList->write_proc = rtl_redirect_client_write;
	}
	#endif
	
	#ifdef HTTP_REDIRECT_VERIFICATION		
	redirect_proc_required_url = create_proc_entry(HTTP_REDIRECT_PROC_REQ_URL, 0, redirect_proc_root);
	if(redirect_proc_required_url){
		redirect_proc_required_url->read_proc = rtl_redirect_required_url_read;
		redirect_proc_required_url->write_proc = rtl_redirect_required_url_write;
	}
	#endif
	#ifdef HTTP_REDIRECT_USER_TIMING
	redirect_proc_max_idle_time = create_proc_entry(HTTP_REDIRECT_PROC_MAX_IDLE_TIME, 0, redirect_proc_root);
	if(redirect_proc_max_idle_time){
		redirect_proc_max_idle_time->read_proc = rtl_redirect_max_idle_read;
		redirect_proc_max_idle_time->write_proc = rtl_redirect_max_idle_write;
	}
	#endif
#endif
}
void http_redirect_destroy_proc(void)
{
#ifdef CONFIG_RTL_PROC_NEW
	if(redirect_proc_root){
		remove_proc_entry(HTTP_REDIRECT_PROC_ENABLE, &redirect_proc_root);
		
		#if defined(CONFIG_RTL_HTTP_REDIRECT_TR098)
		remove_proc_entry(HTTP_REDIRECT_PROC_ALLOWED_LIST, &redirect_proc_root);	
		#else
		remove_proc_entry(HTTP_REDIRECT_PROC_CLIENT_LIST, &redirect_proc_root);	
		#endif
				
		#ifdef HTTP_REDIRECT_VERIFICATION		
		remove_proc_entry(HTTP_REDIRECT_PROC_REQ_URL, &redirect_proc_root); 	
		#endif
		
		#ifdef HTTP_REDIRECT_USER_TIMING
		remove_proc_entry(HTTP_REDIRECT_PROC_MAX_IDLE_TIME, &redirect_proc_root);		
		#endif
		
		remove_proc_entry(HTTP_REDIRECT_PROC_ROOT, &proc_root); 	
	}
#else
	if(redirect_proc_root){
		if (redirect_proc_enable) {
			remove_proc_entry(HTTP_REDIRECT_PROC_ENABLE, redirect_proc_root);
			redirect_proc_enable = NULL;
		}
	
		if(redirect_proc_host){
			remove_proc_entry(HTTP_REDIRECT_PROC_HOST, redirect_proc_root);
			redirect_proc_host = NULL;
		}
		
		#ifdef CONFIG_RTL_HTTP_REDIRECT_TR098
		if(redirect_proc_allowedList){
			remove_proc_entry(HTTP_REDIRECT_PROC_ALLOWED_LIST, redirect_proc_root);
			redirect_proc_allowedList = NULL;
		}
		#else
		if(redirect_proc_clientList){
			remove_proc_entry(HTTP_REDIRECT_PROC_CLIENT_LIST, redirect_proc_root);
			redirect_proc_clientList = NULL;
		}
		#endif
		
		#ifdef HTTP_REDIRECT_VERIFICATION		
		if(redirect_proc_required_url){
			remove_proc_entry(HTTP_REDIRECT_PROC_CLIENT_LIST, redirect_proc_root);
			redirect_proc_required_url = NULL;
		}
		#endif
		#ifdef HTTP_REDIRECT_USER_TIMING
		if(redirect_proc_max_idle_time){
			remove_proc_entry(HTTP_REDIRECT_PROC_CLIENT_LIST, redirect_proc_root);
			redirect_proc_max_idle_time = NULL;
		}
		#endif
		remove_proc_entry(HTTP_REDIRECT_PROC_ROOT, NULL);
		redirect_proc_root = NULL;
	}
#endif
}
#endif
#ifdef HTTP_REDIRECT_USER_TIMING		
void http_redirect_chk_timer(unsigned long task_priv)
{
	char mac[32];
	struct macaddr_entry *tmp_entry;
	struct list_head *q,*n;
	DEBUGP("%s\n",__FUNCTION__);
	if(max_idle_time == 0){/*if set to 0, never expires*/
		goto out;
	}
	if(!list_empty(&client_mac_list)){
		list_for_each_safe(q,n,&client_mac_list){
			tmp_entry = list_entry(q,macaddr_entry,list);
			if((jiffies - tmp_entry->updated)/HZ > max_idle_time){
				DEBUGP("%s exceed max idle time, delete client.\n",__FUNCTION__);
				list_del(&tmp_entry->list);
				kfree(tmp_entry);
				client_num --;
			}
		}
	}
out:
	mod_timer(&online_chk_timer,jiffies + CHECK_ONLINE_INTERVAL*HZ);
}
#endif
int __init http_redirect_init(void)
{
	int ret = 0;

	DEBUGP("http redirect init....\n");
#if !defined(CONFIG_RTL_HTTP_REDIRECT_TR098)	
	strcpy(defaultUrl,DEFAULT_URL);
#endif
#ifdef HTTP_REDIRECT_VERIFICATION
	strcpy(required_url,DEFAULT_REQ_URL);
#endif
#ifdef CONFIG_PROC_FS
	http_redirect_create_proc();
#endif
#ifdef CONFIG_RTL_HTTP_REDIRECT_TR098
	INIT_LIST_HEAD(&allowed_ip_list);
#else
	INIT_LIST_HEAD(&client_mac_list);
#endif
#ifdef HTTP_REDIRECT_USER_TIMING		
	init_timer(&online_chk_timer);
	online_chk_timer.function = &http_redirect_chk_timer;
	online_chk_timer.expires = jiffies + CHECK_ONLINE_INTERVAL*HZ;/*check for per 30 seconds*/
	add_timer(&online_chk_timer);
#endif
	DEBUGP("http redirect init success!\n");

	return(0);
}

void __exit http_redirect_fini(void)
{
#ifdef CONFIG_PROC_FS
	http_redirect_destroy_proc();
#endif
#ifndef CONFIG_RTL_HTTP_REDIRECT_TR098
	flush_mac_list();
#endif
#ifdef HTTP_REDIRECT_USER_TIMING		
	if (timer_pending(&online_chk_timer))
		del_timer_sync(&online_chk_timer);
#endif
}

