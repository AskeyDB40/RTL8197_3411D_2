#include <linux/version.h>

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/inetdevice.h>
#include <net/checksum.h>
#include <net/udp.h>
#include <linux/ctype.h>

#include <net/rtl/rtl_dnstrap.h>
#ifdef CONFIG_IPV6
#include <linux/in6.h>
#include <net/if_inet6.h>
#include <net/addrconf.h>
#include <linux/inetdevice.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#define CONFIG_RTL_PROC_NEW
extern struct proc_dir_entry proc_root;
#endif

struct proc_dir_entry *dnstrap_proc_root = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
static struct proc_dir_entry *proc_domain = NULL;
static struct proc_dir_entry *proc_enable = NULL;
static struct proc_dir_entry *proc_trap_all = NULL;
#endif
#define PROC_ROOT "rtl_dnstrap"
#define PROC_DOMAIN_NAME "domain_name"
#define PROC_ENABLE "enable"

#ifdef SUPPORT_TRAP_ALL
#define PROC_TRAP_ALL "trap_all"
#endif

unsigned char domain_name[80];
//unsigned char dns_answer[] = { 0xC0, 0x0C, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x03, 0x84, 0x00, 0x04 };

unsigned char dns_answer[] = { 0xC0, 0x0C, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
#ifdef CONFIG_IPV6
unsigned char dns_answer_v6[] = { 0xC0, 0x0C, 0x00, 0x1C, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10 };
#endif
int dns_filter_enable = 1;

#ifdef SUPPORT_TRAP_ALL
int trap_all = 1;
#endif

#define DEFAULT_HOSTNAME "wifi.example.com"
#ifdef CONFIG_IPV6
extern struct inet6_ifaddr *ipv6_get_ifaddr(struct net * net,const struct in6_addr * addr,struct net_device * dev,int strict);
#define EXTEND_LEN_V6	28
#endif
#define EXTEND_LEN_V4	16
int br_dns_packet_recap(struct sk_buff *skb,short type)
{
	struct iphdr *iph;
	struct udphdr *udph;
	struct net_device *br0_dev; 
	struct in_device *br0_in_dev;
	dnsheader_t *dns_pkt;
	unsigned char mac[ETH_ALEN];
	unsigned int ip;
	unsigned short port;
	unsigned char *ptr = NULL;
	int extend_len;
	if(type == 0x01)
	{
		extend_len = EXTEND_LEN_V4;
	}
#ifdef CONFIG_IPV6	
	else if(type == 0x1c)
	{
		extend_len = EXTEND_LEN_V6;
	}
#endif
	else
	{
		DBGP_DNS_TRAP("[%s:%d]Invalid type!\n",__FUNCTION__,__LINE__);
		return -1;
	}
#ifdef CONFIG_IPV6
	struct inet6_dev *idev;
	struct inet6_ifaddr *ifa;
#endif
	br0_dev = dev_get_by_name(&init_net,"br0"); 
	br0_in_dev = in_dev_get(br0_dev);
#ifdef CONFIG_IPV6
	if(type == 0x1c)//IPV6 address
	{
		idev = in6_dev_get(br0_dev);
		if(idev != NULL){
			list_for_each_entry(ifa, &idev->addr_list, if_list) {
				in6_dev_put(idev);
				break;
			}
		}
	}
#endif

	if(!br0_dev || !br0_in_dev)	
	{
		if(br0_in_dev)
			in_dev_put(br0_in_dev);
		if(br0_dev)
			dev_put(br0_dev);
		return -1;
	}

	iph = ip_hdr(skb);
	udph = (void *)iph + iph->ihl*4;
	dns_pkt = (void *)udph + sizeof(struct udphdr);
	ptr = (void *)udph + ntohs(udph->len);
	skb_put(skb,extend_len);
	/* swap mac address */
	memcpy(mac, eth_hdr(skb)->h_dest, ETH_ALEN);
	memcpy(eth_hdr(skb)->h_dest, eth_hdr(skb)->h_source, ETH_ALEN);
	memcpy(eth_hdr(skb)->h_source, mac, ETH_ALEN);

	/*swap ip address */
	ip = iph->saddr;
	iph->saddr = iph->daddr;
	iph->daddr = ip;
	iph->tot_len = htons(ntohs(iph->tot_len)+extend_len);
	DBGP_DNS_TRAP("[%s]iph->tot_len:%d\n",__FUNCTION__,iph->tot_len);

	/* swap udp port */
	port = udph->source;
	udph->source = udph->dest;
	udph->dest = port;
	udph->len = htons(ntohs(udph->len)+extend_len);
	dns_pkt->u = htons(0x8180);
	dns_pkt->qdcount = htons(1);
	dns_pkt->ancount = htons(1);
	dns_pkt->nscount = htons(0);
	dns_pkt->arcount = htons(0);
	DBGP_DNS_TRAP("[%s]udph->len:%d\n",__FUNCTION__,ntohs(udph->len));
	DBGP_DNS_TRAP("[%s]dns_pkt->u:%x\n",__FUNCTION__,ntohs(dns_pkt->u));
	DBGP_DNS_TRAP("[%s]dns_pkt->qdcount:%x\n",__FUNCTION__,ntohs(dns_pkt->qdcount));
	DBGP_DNS_TRAP("[%s]dns_pkt->ancount:%x\n",__FUNCTION__,ntohs(dns_pkt->ancount));
	DBGP_DNS_TRAP("[%s]dns_pkt->nscount:%x\n",__FUNCTION__,ntohs(dns_pkt->nscount));
	DBGP_DNS_TRAP("[%s]dns_pkt->arcount:%x\n",__FUNCTION__,ntohs(dns_pkt->arcount));
	/* pad Answers */
	if(type == 0x01)
	{
		memcpy(ptr, dns_answer, 12);
		memcpy(ptr+12, (unsigned char *)&br0_in_dev->ifa_list->ifa_address, 4);
	}
#ifdef CONFIG_IPV6
	else if(type == 0x1c)
	{
		memcpy(ptr,dns_answer_v6,12);
		if(ifa != NULL)
			memcpy(ptr+12,(unsigned char *)&ifa->addr,16);
		else
			memcpy(ptr+12,0,16);
	}
#endif
	/* ip checksum */
	skb->ip_summed = CHECKSUM_NONE;
	skb->dns_trap=1;
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

	/* udp checksum */
	udph->check = 0;
	udph->check = csum_tcpudp_magic(iph->saddr, iph->daddr,
					ntohs(udph->len), IPPROTO_UDP,
					csum_partial((char *)udph,
					             ntohs(udph->len), 0));
    if(br0_in_dev)
        in_dev_put(br0_in_dev);
    if(br0_dev)
        dev_put(br0_dev);

	return 1;
}

static short get_domain_name(unsigned char* dns_body,char* domain_name,int body_len)
{
	int offset = 0,token_len = 0;
	char token[64] = {0};
	char domain[128] = {0};
	unsigned char* tmp;
	short type;
	if(!dns_body || !domain_name || body_len <= 0){
		return -1;
	}
	while(body_len > 0){
		memset(token,0,sizeof(token));
		token_len = dns_body[offset];
		if(token_len > 0){
			strncpy(token,dns_body+offset+1,token_len);
			if(!domain[0]){
				strcpy(domain,token);
			}
			else{
				strcat(domain,".");
				strcat(domain,token);
			}
		}
		else 
		{
			break;
		}
		token_len +=1;
		body_len -= token_len;
		offset += token_len;
	}
	tmp = dns_body + offset + 1;
	type = ntohs((*(unsigned short*)tmp));
	strcpy(domain_name,domain);
	return type;
}
static int is_domain_name_equal(char *domain_name1, char * domain_name2)
{
	char temp1[128];
	char temp2[128];
	if(!domain_name1 || !domain_name2)
	{
		return 0;
	}
	if(!strncmp(domain_name1,"www.",4)){
		strcpy(temp1,domain_name1+4);
	}
	else{
		strcpy(temp1,domain_name1);
	}
	if(!strncmp(domain_name2,"www.",4)){
		strcpy(temp2,domain_name2+4);
	}
	else{
		strcpy(temp2,domain_name2);
	}
	if(strcasecmp(temp1,temp2))
		return 0;
	else
		return 1;
}

static int is_valid_dns_query_header(dnsheader_t *dns_header)
{
	if(dns_header == NULL)
	{
		return 0;
	}
	DBGP_DNS_TRAP("[%s]qdcount:%d\n",__FUNCTION__,dns_header->qdcount);
	if(dns_header->qdcount < 1)
	{
		return 0;
	}
	if(((dns_header->u & 0x8000)>>15)!= 0)/*QR: query should be 0,answer be 1*/
	{
		DBGP_DNS_TRAP("[%s]QR!=0!\n",__FUNCTION__);
		return 0;
	}
	if(((dns_header->u & 0x7100)>>11) != 0)/*opcode: 0:standard,1:reverse,2:server status*/
	{
		DBGP_DNS_TRAP("[%s]opcode!=0!\n",__FUNCTION__);
		return 0;
	}
	if(((dns_header->u & 0x70)>>4) != 0)/*Z: reserved, should be 0*/
	{
		DBGP_DNS_TRAP("[%s]Z!=0!\n",__FUNCTION__);
		return 0;
	}
	return 1;
}
int br_dns_filter_enter(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct udphdr *udph;
	unsigned char *body = NULL;
	dnsheader_t *dns_hdr = NULL;
	int len = 0;
	char domain[512] = {0};
	short type;
	iph = (struct iphdr *)skb_network_header(skb);
	udph = (void *)iph + iph->ihl*4;
	if (iph->protocol==IPPROTO_UDP && ntohs(udph->dest) == 53&& ((iph->frag_off & htons(0x3FFF))==0)&&(ntohs(iph->tot_len-udph->len)>=20)) {
		DBGP_DNS_TRAP("[%s:%d]DNS packet\n",__FUNCTION__,__LINE__);
		len = ntohs(udph->len) - sizeof(struct udphdr) - sizeof(dnsheader_t) - 4;
		if(len <=1 || len > 63)
		{
			return 1;
		}
		dns_hdr = (dnsheader_t*)((void*)udph + sizeof(struct udphdr));
		if(!is_valid_dns_query_header(dns_hdr))
		{
			return 1;
		}
#ifdef SUPPORT_TRAP_ALL
		if(trap_all){
			br_dns_packet_recap(skb);
			return 1;
		}
#endif
		body = (void *)udph + sizeof(struct udphdr) + sizeof(dnsheader_t);
		//DBGP_DNS_TRAP("[%s:%d]DNS packet urlis[%s]\n",__FUNCTION__,__LINE__,body);
		type = get_domain_name(body,domain,len);
		if(type != 0x01 && type != 0x1c)
		{
			DBGP_DNS_TRAP("[%s:%d]Invalid type!\n",__FUNCTION__,__LINE__);
			return -1;
		}
		DBGP_DNS_TRAP("[%s:%d]domain_name is %s,type:%x\n",__FUNCTION__,__LINE__,domain,type);
		if(is_domain_name_equal(domain,domain_name)){
			DBGP_DNS_TRAP("[%s:%d]%s matched!!!\n",__FUNCTION__,__LINE__,domain);
			br_dns_packet_recap(skb,type);
		}
		return 1;
	}

	return 0;
}

int is_recaped_dns_packet(struct sk_buff *skb)
{
	if(skb)
		return skb->dns_trap;
	return 0;
}

int is_dns_packet(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct udphdr *udph;
	iph = (void *)skb->data;
	if (iph) {
		udph = (void *)iph + iph->ihl*4;
		if (iph->protocol==IPPROTO_UDP && (udph->dest == htons(53) || udph->source == htons(53))) {
			skb->is_dns_pkt = 1;
			return 1;
		}
	}
	return 0;
}
#if defined(CONFIG_RTL_PROC_NEW)
static int dnstrap_en_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%d\n",dns_filter_enable);
	return 0;
}
#else
static int dnstrap_en_read(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{

	int len=0;
	len = sprintf(page, "%d\n", dns_filter_enable);
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif
static int dnstrap_en_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char tmpbuf[80];

	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(tmpbuf, buffer, count))  {
		tmpbuf[count] = '\0';
		if (tmpbuf[0] == '0')
			dns_filter_enable = 0;
		else if (tmpbuf[0] == '1')
			dns_filter_enable = 1;
		return count;
	}
	return -EFAULT;
}
#ifdef CONFIG_RTL_PROC_NEW
int dnstrap_en_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, dnstrap_en_read,NULL));
}
int dnstrap_en_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return dnstrap_en_write(file,userbuf,count,off);
}

struct file_operations dnstrap_en_proc_fops= {
        .open           = dnstrap_en_proc_open,
        .write		    = dnstrap_en_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
////////
#ifdef SUPPORT_TRAP_ALL
#if defined(CONFIG_RTL_PROC_NEW)
static int dnstrap_trap_all_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%d\n",trap_all);
	return 0;
}
#else
static int dnstrap_trap_all_read(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{

	int len=0;
	len = sprintf(page, "%d\n", trap_all);
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif
static int dnstrap_trap_all_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	char tmpbuf[80];

	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(tmpbuf, buffer, count))  {
		tmpbuf[count] = '\0';
		if (tmpbuf[0] == '0')
			trap_all = 0;
		else if (tmpbuf[0] == '1')
			trap_all = 1;
		return count;
	}
	return -EFAULT;
}
#ifdef CONFIG_RTL_PROC_NEW
int dnstrap_trap_all_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, dnstrap_en_read,NULL));
}
int dnstrap_trap_all_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return dnstrap_en_write(file,userbuf,count,off);
}

struct file_operations dnstrap_trap_all_proc_fops= {
        .open           = dnstrap_trap_all_proc_open,
        .write		    = dnstrap_trap_all_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
#endif /*SUPPORT_TRAP_ALL*/
////////
#if defined(CONFIG_RTL_PROC_NEW)
static int dnstrap_domain_read(struct seq_file *s, void *v)
{
	seq_printf(s,"%s\n", domain_name);
	return 0;
}
#else
static int dnstrap_domain_read(char *page, char **start, off_t off,
		     int count, int *eof, void *data)
{

	int len=0;
	len = sprintf(page, "%s\n", domain_name);

	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;

	return len;
}
#endif
static int dnstrap_domain_write(struct file *file, const char *buffer,
		      unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(domain_name, buffer, 80)) {
		domain_name[count-1] = 0;
		return count;
	}

	return -EFAULT;
}
#ifdef CONFIG_RTL_PROC_NEW
int dnstrap_domain_proc_open(struct inode *inode, struct file *file)
{
	return(single_open(file, dnstrap_domain_read,NULL));
}
int dnstrap_domain_proc_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	return dnstrap_domain_write(file,userbuf,count,off);
}

struct file_operations dnstrap_domain_proc_fops= {
        .open           = dnstrap_domain_proc_open,
        .write		    = dnstrap_domain_proc_write,
        .read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};
#endif
#if defined(CONFIG_PROC_FS)
static void dnstrap_create_proc(void)
{
#if defined(CONFIG_RTL_PROC_NEW)
	dnstrap_proc_root = proc_mkdir(PROC_ROOT,&proc_root);
	if(dnstrap_proc_root){
		proc_create_data(PROC_DOMAIN_NAME,0,dnstrap_proc_root,&dnstrap_domain_proc_fops,NULL);
		proc_create_data(PROC_ENABLE,0,dnstrap_proc_root,&dnstrap_en_proc_fops,NULL);
		#ifdef SUPPORT_TRAP_ALL
		proc_create_data(PROC_TRAP_ALL,0,dnstrap_proc_root,&dnstrap_trap_all_proc_fops,NULL);
		#endif
	}
#else
	dnstrap_proc_root = proc_mkdir(PROC_ROOT, NULL);
	if (!dnstrap_proc_root){
		printk("create folder fail\n");
		return;
	}
	proc_enable = create_proc_entry(PROC_ENABLE, 0, dnstrap_proc_root);
	if (proc_enable) {
		proc_enable->read_proc = dnstrap_en_read;
		proc_enable->write_proc = dnstrap_en_write;
	}
	proc_domain = create_proc_entry(PROC_DOMAIN_NAME, 0, dnstrap_proc_root);
	if (proc_domain) {
		proc_domain->read_proc = dnstrap_domain_read;
		proc_domain->write_proc = dnstrap_domain_write;
	}
	#ifdef SUPPORT_TRAP_ALL
	proc_trap_all = create_proc_entry(PROC_TRAP_ALL, 0, dnstrap_proc_root);
	if (proc_trap_all) {
		proc_trap_all->read_proc = dnstrap_trap_all_read;
		proc_trap_all->write_proc = dnstrap_trap_all_write;
	}
	#endif
#endif
}
static void dnstrap_destroy_proc(void)
{
#if defined(CONFIG_RTL_PROC_NEW)
	if(dnstrap_proc_root){
		remove_proc_entry(PROC_DOMAIN_NAME, &dnstrap_proc_root);		
		remove_proc_entry(PROC_ENABLE, &dnstrap_proc_root);		
		#ifdef SUPPORT_TRAP_ALL
		remove_proc_entry(PROC_TRAP_ALL, &dnstrap_proc_root);		
		#endif
		remove_proc_entry(PROC_ROOT, &proc_root);		
	}
#else
	if(dnstrap_proc_root){
		if (proc_enable) {
			remove_proc_entry(PROC_ENABLE, dnstrap_proc_root);
			proc_enable = NULL;
		}

		if (proc_domain) {
			remove_proc_entry(PROC_DOMAIN_NAME, dnstrap_proc_root);
			proc_domain = NULL;
		}
		#ifdef SUPPORT_TRAP_ALL
		if (proc_trap_all) {
			remove_proc_entry(PROC_DOMAIN_NAME, dnstrap_proc_root);
			proc_trap_all = NULL;
		}
		#endif
		remove_proc_entry(PROC_ROOT, NULL);
		dnstrap_proc_root = NULL;
	}
#endif
}
#endif
int __init br_dns_filter_init(void)
{
#if defined(CONFIG_PROC_FS)
	dnstrap_create_proc();
#endif
	memset(domain_name,0,sizeof(domain_name));
	strcpy(domain_name,DEFAULT_HOSTNAME);
	return 0;
}

void __exit br_dns_filter_exit(void)
{
#if defined(CONFIG_PROC_FS)
	dnstrap_destroy_proc();
#endif
}

