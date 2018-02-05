#ifndef IPV6_MANAGE_INET_H
#define IPV6_MANAGE_INET_H

#define IPV6_MANG_PID_FILE "/var/run/ipv6_manage_inet.pid"
#ifdef	CONFIG_IPV6_RA_RDNSS_SUPPORT 
#define IPV6_RDNSS_MAX  3
struct rdnss_info {
uint8_t 	type;
uint8_t 	length;
uint16_t	reserved;
uint32_t	lifetime;
struct in6_addr in6addr_dns[IPV6_RDNSS_MAX];
};
#ifdef  CONFIG_IPV6_RA_DNSSL_SUPPORT 
#define IPV6_DNSSL_MAX  3
struct dnssl_info {
 uint8_t    type;
 uint8_t    length;
 uint16_t  reserved;
 uint32_t  lifetime;
 char name_dsl[IPV6_DNSSL_MAX][128];
};
#endif
#endif
typedef struct RA_INFO_ITEM_
{
	int enable;
	int pid;                      //input
	int slaacFail;
	int icmp6_managed;	  //output
	int icmp6_other;	  //output
#ifdef	CONFIG_IPV6_RA_RDNSS_SUPPORT 
	struct rdnss_info dnsinfo;
#ifdef CONFIG_IPV6_RA_DNSSL_SUPPORT
	struct dnssl_info dslinfo;
#endif
#endif
} RA_INFO_ITEM_T, *RA_INFO_ITEM_Tp;
#endif
