/*
	Init to handle RA pocket
	should check M(manage) and O(others) field. 
		MO==1x get all config via dhcpv6,should generate dhcpv6 config and start dhcpv6c
		MO==01 get others config(not ip) via dhcpv6, should generate dhcpv6 config and start dhcpv6c
		MO==00 get no config via dhcpv6, should not start dhcpv6c

	also handle dhcp6c get options, when get
		prefix delegation's prefix, set the prefix to lan radvd, 
		dns, set dns
		...
*/
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#include "apmib.h"
#include "mibtbl.h"
#include "sysconf.h"
#include "sys_utility.h"

#include "ipv6_manage_inet.h"

#define BUFSIZE 1024

#ifndef IPV6_ADDR_LINKLOCAL
#define IPV6_ADDR_LINKLOCAL   0x0020U
#endif

RA_INFO_ITEM_T last_recv_data={0};
#ifdef CONFIG_IPV6_DNSV6_MODE_SUPPORT
int get_linklocal_addr(addr6CfgParam_t *addr6_dns, char *ifaname)
{
	FILE *fp;
	char str_addr[40];
	unsigned int plen, scope, dad_status, if_idx;
	char devname[16];
	if ((fp = fopen("/proc/net/if_inet6", "r")) == NULL)
	{
		printf("can't open %s: %s", "/proc/net/if_inet6",
			strerror(errno));
		return (-1);	
	}
	
	while (fscanf(fp, "%32s %x %02x %02x %02x %15s\n",
		      str_addr, &if_idx, &plen, &scope, &dad_status,
		      devname) != EOF)
	{
		if (scope == IPV6_ADDR_LINKLOCAL &&
		    strcmp(devname, ifaname) == 0)
		{
			struct in6_addr addr;
			unsigned int ap;
			int i;
			for (i=0; i<8; i++)
			{
				sscanf(str_addr + i * 4, "%04x", &ap);
				addr6_dns->addrIPv6[i] = ap;
			}
			addr6_dns->prefix_len=plen;
			fclose(fp);
			return 1;	
		}
	}
	printf("no linklocal address configured for %s", ifaname);
	fclose(fp);
	return (-1);	
}

void restart_radvd(RA_INFO_ITEM_Tp rera_data)
	{
	
		radvdCfgParam_t radvdCfgParam;
		int fh;
		char tmpStr[256];
		char tmpBuf[256];
		int dnsMode;
		FILE *fp=NULL;
		unsigned short tmpNum[8];
		int dnsforward;
		if ( !apmib_init()) {
		printf("Initialize AP MIB failed !\n");
		return -1;
		}
		if ( !apmib_get(MIB_IPV6_RADVD_PARAM,(void *)&radvdCfgParam)){
			printf("get MIB_IPV6_RADVD_PARAM failed\n");
			return;  
		}

	/*create config file*/
	fh = open(RADVD_CONF_FILE,  O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
	if (fh < 0) {
		fprintf(stderr, "Create %s file error!\n", RADVD_CONF_FILE);
		return;
	}
	if (flock(fh, LOCK_EX) != 0)
	printf("file lock by others\n");
	printf("------------recreate radvd.conf----------------\n");
	sprintf(tmpStr, "interface %s\n", radvdCfgParam.interface.Name);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "{\n");
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvSendAdvert on;\n");
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "MaxRtrAdvInterval %d;\n", radvdCfgParam.interface.MaxRtrAdvInterval);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "MinRtrAdvInterval %d;\n", radvdCfgParam.interface.MinRtrAdvInterval);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "MinDelayBetweenRAs %d;\n", radvdCfgParam.interface.MinDelayBetweenRAs);
	write(fh, tmpStr, strlen(tmpStr));
	if(radvdCfgParam.interface.AdvManagedFlag > 0) {
		sprintf(tmpStr, "AdvManagedFlag on;\n");
		write(fh, tmpStr, strlen(tmpStr));			
	}
	if(radvdCfgParam.interface.AdvOtherConfigFlag > 0){
		sprintf(tmpStr, "AdvOtherConfigFlag on;\n");
		write(fh, tmpStr, strlen(tmpStr));	
	}
	sprintf(tmpStr, "AdvLinkMTU %d;\n", radvdCfgParam.interface.AdvLinkMTU);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvReachableTime %u;\n", radvdCfgParam.interface.AdvReachableTime);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvRetransTimer %u;\n", radvdCfgParam.interface.AdvRetransTimer);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvCurHopLimit %d;\n", radvdCfgParam.interface.AdvCurHopLimit);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvDefaultLifetime %d;\n", radvdCfgParam.interface.AdvDefaultLifetime);
	write(fh, tmpStr, strlen(tmpStr));
	sprintf(tmpStr, "AdvDefaultPreference %s;\n", radvdCfgParam.interface.AdvDefaultPreference);
	write(fh, tmpStr, strlen(tmpStr));
	if(radvdCfgParam.interface.AdvSourceLLAddress > 0) {
		sprintf(tmpStr, "AdvSourceLLAddress on;\n");
		write(fh, tmpStr, strlen(tmpStr));			
	}		
	if(radvdCfgParam.interface.UnicastOnly > 0){
		sprintf(tmpStr, "UnicastOnly on;\n");
		write(fh, tmpStr, strlen(tmpStr));	
	}
	

	/*prefix 1*/
	if(radvdCfgParam.interface.prefix[0].enabled > 0){
		memcpy(tmpNum,radvdCfgParam.interface.prefix[0].Prefix, sizeof(radvdCfgParam.interface.prefix[0].Prefix));
		sprintf(tmpBuf, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", tmpNum[0], tmpNum[1], 
			tmpNum[2], tmpNum[3], tmpNum[4], tmpNum[5],tmpNum[6],tmpNum[7]);
		strcat(tmpBuf, "\0");
		sprintf(tmpStr, "prefix %s/%d\n", tmpBuf, radvdCfgParam.interface.prefix[0].PrefixLen); 		
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "{\n");
		write(fh, tmpStr, strlen(tmpStr));
		if(radvdCfgParam.interface.prefix[0].AdvOnLinkFlag > 0){
			sprintf(tmpStr, "AdvOnLink on;\n");
			write(fh, tmpStr, strlen(tmpStr));					
		}
		if(radvdCfgParam.interface.prefix[0].AdvAutonomousFlag > 0){
			sprintf(tmpStr, "AdvAutonomous on;\n");
			write(fh, tmpStr, strlen(tmpStr));					
		}
		sprintf(tmpStr, "AdvValidLifetime %u;\n", radvdCfgParam.interface.prefix[0].AdvValidLifetime);
		write(fh, tmpStr, strlen(tmpStr));					
		sprintf(tmpStr, "AdvPreferredLifetime %u;\n", radvdCfgParam.interface.prefix[0].AdvPreferredLifetime);
		write(fh, tmpStr, strlen(tmpStr));	
		if(radvdCfgParam.interface.prefix[0].AdvRouterAddr > 0){
			sprintf(tmpStr, "AdvRouterAddr on;\n");
			write(fh, tmpStr, strlen(tmpStr));						
		}
		if(radvdCfgParam.interface.prefix[0].if6to4[0]){
			sprintf(tmpStr, "Base6to4Interface %s;\n", radvdCfgParam.interface.prefix[0].if6to4);
			write(fh, tmpStr, strlen(tmpStr));						
		}
		sprintf(tmpStr, "};\n");
		write(fh, tmpStr, strlen(tmpStr));						
	}

	/*prefix 2*/
	if(radvdCfgParam.interface.prefix[1].enabled > 0){
		memcpy(tmpNum,radvdCfgParam.interface.prefix[1].Prefix, sizeof(radvdCfgParam.interface.prefix[1].Prefix));
		sprintf(tmpBuf, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", tmpNum[0], tmpNum[1], 
			tmpNum[2], tmpNum[3], tmpNum[4], tmpNum[5],tmpNum[6],tmpNum[7]);
		strcat(tmpBuf, "\0");
		sprintf(tmpStr, "prefix %s/%d\n", tmpBuf, radvdCfgParam.interface.prefix[1].PrefixLen);
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "{\n");
		write(fh, tmpStr, strlen(tmpStr));
		if(radvdCfgParam.interface.prefix[1].AdvOnLinkFlag > 0){
			sprintf(tmpStr, "AdvOnLink on;\n");
			write(fh, tmpStr, strlen(tmpStr));					
		}
		if(radvdCfgParam.interface.prefix[1].AdvAutonomousFlag > 0){
			sprintf(tmpStr, "AdvAutonomous on;\n");
			write(fh, tmpStr, strlen(tmpStr));					
		}
		sprintf(tmpStr, "AdvValidLifetime %u;\n", radvdCfgParam.interface.prefix[1].AdvValidLifetime);
		write(fh, tmpStr, strlen(tmpStr));					
		sprintf(tmpStr, "AdvPreferredLifetime %u;\n", radvdCfgParam.interface.prefix[1].AdvPreferredLifetime);
		write(fh, tmpStr, strlen(tmpStr));	
		if(radvdCfgParam.interface.prefix[1].AdvRouterAddr > 0){
			sprintf(tmpStr, "AdvRouterAddr on;\n");
			write(fh, tmpStr, strlen(tmpStr));						
		}
		if(radvdCfgParam.interface.prefix[1].if6to4[0]){
			sprintf(tmpStr, "Base6to4Interface %s;\n", radvdCfgParam.interface.prefix[1].if6to4);
			write(fh, tmpStr, strlen(tmpStr));						
		}
		sprintf(tmpStr, "};\n");
		write(fh, tmpStr, strlen(tmpStr));						
	}
	if(radvdCfgParam.interface.prefix[0].if6to4[0] ||
	   radvdCfgParam.interface.prefix[1].if6to4[0])
	{
		sprintf(tmpStr, "route 2000::/3\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "{\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "AdvRoutePreference medium;\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "AdvRouteLifetime 1800;\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "};\n");
		write(fh, tmpStr, strlen(tmpStr));
	}
#ifdef CONFIG_IPV6_RA_RDNSS_SUPPORT
	//add RDNSS 
	apmib_get(MIB_IPV6_DNS_AUTO, (void *)&dnsMode); 	
	if(dnsMode==0)	//Set DNS Manually 
	{
		addr6CfgParam_t addr6_dns;
		
		apmib_get(MIB_IPV6_ADDR_DNS_PARAM,	(void *)&addr6_dns);
			
		snprintf(tmpBuf, sizeof(tmpBuf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
		addr6_dns.addrIPv6[0], addr6_dns.addrIPv6[1], addr6_dns.addrIPv6[2], addr6_dns.addrIPv6[3], 
		addr6_dns.addrIPv6[4], addr6_dns.addrIPv6[5], addr6_dns.addrIPv6[6], addr6_dns.addrIPv6[7]);

		if(strstr(tmpBuf, "0000:0000:0000:0000:0000:0000:0000:0000")==NULL) 
		{
			//add RDNSS 
			sprintf(tmpStr, "RDNSS %s\n",tmpBuf);
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "{\n");
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "AdvRDNSSLifetime %d;\n", radvdCfgParam.interface.MaxRtrAdvInterval);
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "};\n");
			write(fh, tmpStr, strlen(tmpStr));
		}			
	}
	else  //Set DNS Auto
	{
		apmib_get(MIB_IPV6_DNS_FORWARD, (void *)&dnsforward);	
		if(dnsforward==1)   // dns forward mode
		{
			if(isFileExist(DNSV6_RDNS_FILE))
			{
				if((fp=fopen("/var/rdnss.conf","r"))!=NULL)
				{	
					memset(tmpStr, 0, sizeof(tmpStr));
					while(fgets(tmpBuf, sizeof(tmpBuf), fp))
					{
						tmpBuf[strlen(tmpBuf)-1]=0;
						write(fh, tmpBuf, strlen(tmpBuf));
						sprintf(tmpBuf, "\n");
						write(fh, tmpBuf, strlen(tmpBuf));	
						memset(tmpBuf, 0, sizeof(tmpBuf));
					}
					fclose(fp); 
				}
			}
		}
		else  //dns relay mode
		{
			addr6CfgParam_t addr6_dns;
			if(get_linklocal_addr(&addr6_dns,radvdCfgParam.interface.Name)>0)
			{
				snprintf(tmpBuf, sizeof(tmpBuf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
				addr6_dns.addrIPv6[0], addr6_dns.addrIPv6[1], addr6_dns.addrIPv6[2], addr6_dns.addrIPv6[3], 
				addr6_dns.addrIPv6[4], addr6_dns.addrIPv6[5], addr6_dns.addrIPv6[6], addr6_dns.addrIPv6[7]);
				if(strstr(tmpBuf, "0000:0000:0000:0000:0000:0000:0000:0000")==NULL) 
				{
					//add RDNSS 
					sprintf(tmpStr, "RDNSS %s\n",tmpBuf);
					write(fh, tmpStr, strlen(tmpStr));
					sprintf(tmpStr, "{\n");
					write(fh, tmpStr, strlen(tmpStr));
					sprintf(tmpStr, "AdvRDNSSLifetime %d;\n", radvdCfgParam.interface.MaxRtrAdvInterval);
					write(fh, tmpStr, strlen(tmpStr));
					sprintf(tmpStr, "};\n");
					write(fh, tmpStr, strlen(tmpStr));
				}
			}
		}				
	}
#ifdef CONFIG_IPV6_RA_DNSSL_SUPPORT
	//add DNSSL
	apmib_get(MIB_IPV6_DNS_AUTO, (void *)&dnsMode); 	
	if(dnsMode==0)	//Set DNSSL Manually 
	{
		memset(tmpBuf, 0, sizeof(tmpBuf));
		apmib_get(MIB_DOMAIN_NAME, (void *)tmpBuf); 
		if(strlen(tmpBuf)>0)
		{
			sprintf(tmpStr, "DNSSL %s.com %s.com.cn\n", tmpBuf, tmpBuf);
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "{\n");
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "AdvDNSSLLifetime %d;\n", radvdCfgParam.interface.MaxRtrAdvInterval);
			write(fh, tmpStr, strlen(tmpStr));
			sprintf(tmpStr, "};\n");
			write(fh, tmpStr, strlen(tmpStr));		
		}
	}
	else //Set DNSSL Auto
	{
		if(isFileExist(DNSV6_SEAR_FILE))
		{
			if((fp=fopen("/var/dnssl.conf","r"))!=NULL)
			{			
				memset(tmpBuf, 0, sizeof(tmpBuf));
				while(fgets(tmpBuf, sizeof(tmpBuf), fp))
				{
					tmpBuf[strlen(tmpBuf)-1]=0;
					write(fh, tmpBuf, strlen(tmpBuf));
					sprintf(tmpBuf, "\n");
					write(fh, tmpBuf, strlen(tmpBuf));
					memset(tmpBuf, 0, sizeof(tmpBuf));			
				}
				fclose(fp);
			}
		}	
	}
#endif
#endif
	sprintf(tmpStr, "};\n");
	write(fh, tmpStr, strlen(tmpStr));
	close(fh);
	flock(fh, LOCK_UN);	
	if(1==rera_data->icmp6_managed)
	{
		system("echo 0 > /proc/sys/net/ipv6/conf/all/accept_ra");
	}
	else
	{
		system("echo 1 > /proc/sys/net/ipv6/conf/all/accept_ra");
	}
	if(isFileExist(RADVD_PID_FILE)){
		if(radvdCfgParam.enabled == 1) {
			system("killall radvd 2> /dev/null");			
			system("rm -f /var/run/radvd.pid 2> /dev/null");		
			unlink(DNRD_PID_FILE);	
			system("echo 1 > /proc/sys/net/ipv6/conf/all/forwarding");
			system("radvd -C /var/radvd.conf");
		} else {	
			system("killall radvd 2> /dev/null");		
			system("rm -f /var/run/radvd.pid 2> /dev/null");			
		}
	} else{
		if(radvdCfgParam.enabled == 1) {
			system("echo 1 > /proc/sys/net/ipv6/conf/all/forwarding");
			system("radvd -C /var/radvd.conf"); 	
		}		
	}
	return;
}

void handler_radns(RA_INFO_ITEM_Tp recv_ra_data)
{
#ifdef CONFIG_IPV6_RA_RDNSS_SUPPORT
	FILE *fd;
	if(recv_ra_data->dnsinfo.lifetime!=0)
	{
		fd = fopen(DNSV6_RDNS_FILE, "w");
	    if (fd == NULL) {
	  		printf("Failed to create %s\n", DNSV6_RDNS_FILE);
	  		return;
	    }
		int i=0;
		fprintf(fd,"%s","RDNSS");
		for(i=0;i<(recv_ra_data->dnsinfo.length-1)/2;i++)
	    {
		   fprintf(fd, " %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
		   recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[0],recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[1],
		   recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[2],recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[3],
		   recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[4],recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[5],
		   recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[6],recv_ra_data->dnsinfo.in6addr_dns[i].s6_addr16[7]);
		}
		fprintf(fd,"\n");
		fprintf(fd,"{\n");
		fprintf(fd,"AdvRDNSSLifetime %d;\n", recv_ra_data->dnsinfo.lifetime);
		fprintf(fd,"};\n");
		if (ferror(fd))
	  	printf("Write failed to %s\n", DNSV6_RDNS_FILE);
	 	fclose(fd);
	}
#ifdef CONFIG_IPV6_RA_DNSSL_SUPPORT
	FILE *fs;
	if(recv_ra_data->dslinfo.lifetime!=0)
	{
		fs = fopen(DNSV6_SEAR_FILE, "w");
	    if (fs == NULL) {
	  	printf("Failed to create %s\n", DNSV6_SEAR_FILE);
	  	return;
	    }
		fprintf(fs,"%s","DNSSL");
		int j=0;
		for(j=0;j<(recv_ra_data->dslinfo.length)/2;j++)
		{
			fprintf(fs," %s",recv_ra_data->dslinfo.name_dsl[j]);
		}
		fprintf(fs,"\n");
		fprintf(fs,"{\n");
		fprintf(fs,"AdvDNSSLLifetime %d;\n", recv_ra_data->dslinfo.lifetime);
		fprintf(fs,"};\n");
		if (ferror(fs))
	  	printf("Write failed to %s\n", DNSV6_SEAR_FILE);
	 	fclose(fs);
	}
#endif
#endif
	restart_radvd(recv_ra_data);
}

int checkDnsAddrIsExist(char *dnsAddr, char * dnsFileName)
{
	char  line_buf[128];		
	FILE *fp=NULL;
	if((fp=fopen(dnsFileName, "r"))==NULL)
	{
//		printf("Open file : %s fails!\n",dnsFileName);
		return 0;
	}
	while(fgets(line_buf, 128, fp)) 
	{			
		if(strstr(line_buf, dnsAddr)!=NULL)
		{
			fclose(fp);
			return 1;
		}			
	}
	fclose(fp);
	return 0;
}

void restart_dhcp6s()
{
	dhcp6sCfgParam_t dhcp6sCfgParam;
	char tmpStr[256];
	char tmpBuf[256];
	int fh;
	int pid=-1;
	int dnsMode;
	int dnsforward=0;
	radvdCfgParam_t radvdCfgParam;
	FILE *fp=NULL;
	if ( !apmib_get(MIB_IPV6_DHCPV6S_PARAM,(void *)&dhcp6sCfgParam)){
		printf("get MIB_IPV6_DHCPV6S_PARAM failed\n");
		return;  
	}
	
	if(!dhcp6sCfgParam.enabled){
		return;
	}
	
		/*create config file*/
		fh = open(DHCP6S_CONF_FILE, O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
		if (fh < 0) {
			fprintf(stderr, "Create %s file error!\n", DHCP6S_CONF_FILE);
			return;
		}
		printf("-------------recreate dhcp6s.conf----------------\n");
		apmib_get(MIB_IPV6_DNS_AUTO, (void *)&dnsMode);		
		if(dnsMode==0)  //Set DNS Manually 
		{
			sprintf(tmpStr, "option domain-name-servers %s;\n", dhcp6sCfgParam.DNSaddr6);
			write(fh, tmpStr, strlen(tmpStr));
			memset(tmpBuf, 0, sizeof(tmpBuf));
			apmib_get(MIB_DOMAIN_NAME, (void *)tmpBuf);	
			sprintf(tmpStr, "option domain-name \"%s.com\";\n", tmpBuf);
			write(fh, tmpStr, strlen(tmpStr));
		}
		else  //Set DNS Auto
		{
			apmib_get(MIB_IPV6_DNS_FORWARD, (void *)&dnsforward);	
			if(dnsforward==0)   // dns relay mode
			{
				addr6CfgParam_t addr6_dns;
				if(get_linklocal_addr(&addr6_dns,dhcp6sCfgParam.interfaceNameds)>0)
				{
					snprintf(tmpBuf, sizeof(tmpBuf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
					addr6_dns.addrIPv6[0], addr6_dns.addrIPv6[1], addr6_dns.addrIPv6[2], addr6_dns.addrIPv6[3], 
					addr6_dns.addrIPv6[4], addr6_dns.addrIPv6[5], addr6_dns.addrIPv6[6], addr6_dns.addrIPv6[7]);
					if(strstr(tmpBuf, "0000:0000:0000:0000:0000:0000:0000:0000")==NULL) 
					{
						sprintf(tmpStr, "option domain-name-servers %s;\n", tmpBuf);
						write(fh, tmpStr, strlen(tmpStr));
					}
				}
			}
			else   // dns forward mode
			{
				if((fp=fopen("/var/dns6.conf","r"))!=NULL)
                {
                    memset(tmpBuf, 0, sizeof(tmpBuf));
                    while(fgets(tmpBuf, sizeof(tmpBuf), fp))
                    {
                        tmpBuf[strlen(tmpBuf)-1]=0;
						if(strstr(tmpBuf,"nameserver")!=NULL)
						{	
							memset(tmpStr, 0, sizeof(tmpStr));
                            strcat(tmpStr, strstr(tmpBuf,"nameserver")+strlen("nameserver")+1);
                            strcat(tmpStr, " ");
							if(strlen(tmpStr)>1)
		                    {
	                            tmpStr[strlen(tmpStr)-1]=0;
	                            sprintf(tmpBuf, "option domain-name-servers %s;\n",tmpStr);
	                            write(fh, tmpBuf, strlen(tmpBuf));
		                    }
						}
                    }
				}
				fclose(fp);
			}
	    	if((fp=fopen("/var/dns6.conf","r"))!=NULL)
	        {
	            memset(tmpBuf, 0, sizeof(tmpBuf));
				while(fgets(tmpBuf, sizeof(tmpBuf), fp))
		        {
	                tmpBuf[strlen(tmpBuf)-1]=0;
					if(strstr(tmpBuf,"search")!=NULL)
					{
						memset(tmpStr, 0, sizeof(tmpStr));
	               		strcat(tmpStr, strstr(tmpBuf,"search")+strlen("search")+1);
	                	strcat(tmpStr, " ");
						if(strlen(tmpStr)>1)
				        {
			                tmpStr[strlen(tmpStr)-1]=0;
			                sprintf(tmpBuf, "option domain-name \"%s\";\n",tmpStr);
			                write(fh, tmpBuf, strlen(tmpBuf));
				        }
					}			
				}
				fclose(fp);
			}
		}
		sprintf(tmpStr, "interface %s {\n", dhcp6sCfgParam.interfaceNameds);
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "  address-pool pool1 3600;\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "};\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "pool pool1 {\n");
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "  range %s to %s ;\n", dhcp6sCfgParam.addr6PoolS, dhcp6sCfgParam.addr6PoolE);
		write(fh, tmpStr, strlen(tmpStr));
		sprintf(tmpStr, "};\n");
		write(fh, tmpStr, strlen(tmpStr));

		close(fh);

	/*start daemon*/
	if(isFileExist(DHCP6S_PID_FILE)) {
		pid=getPid_fromFile(DHCP6S_PID_FILE);
		if(dhcp6sCfgParam.enabled == 1){
			sprintf(tmpStr, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", tmpStr, NULL_STR);
			unlink(DHCP6S_PID_FILE);
			RunSystemCmd(NULL_FILE, "/bin/dhcp6s", dhcp6sCfgParam.interfaceNameds, NULL_STR);
		}
		else 
			RunSystemCmd(NULL_FILE, "kill", "-9", tmpStr, NULL_STR);
			
	}else {
		if(dhcp6sCfgParam.enabled == 1)
			RunSystemCmd(NULL_FILE, "/bin/dhcp6s", dhcp6sCfgParam.interfaceNameds, NULL_STR);
	}
		
	return;
}

void update_dnsmode(RA_INFO_ITEM_Tp reso_data)
{
	FILE *fp;
	int fh;
	int i=0;
	char tmpStr[256];
	char tmpBuf[256];
	int managed=0,others=0;
	int dnsforward;
	managed=reso_data->icmp6_managed;
	others=reso_data->icmp6_other;
	apmib_get(MIB_IPV6_DNS_FORWARD, (void *)&dnsforward);	
	if(0==dnsforward)   // dns relay mode
	{
		if(0==access("/var/dhcp6c_dnss_need_update",0))
			system("rm /var/dhcp6c_dnss_need_update");
		if(0==access("/var/rdnssd_dnss_need_update",0))
			system("rm /var/rdnssd_dnss_need_update");
		if(0==access("/var/dhcp6c_dnsl_need_update",0))
		{
			restart_dhcp6s();  //just forward dnsl by dhcpv6
			system("rm /var/dhcp6c_dnsl_need_update");
		}
		if(0==access("/var/rdnssd_dnsl_need_update",0))
		{
			handler_radns(reso_data); //just forward dnsl by radvd
			system("rm /var/rdnssd_dnsl_need_update");
		}
		fh = open(RESOLV_CONF_FILE, O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
		if (fh < 0) {
			fprintf(stderr, "Create %s file error!\n", RESOLV_CONF_FILE);
			return;
		}
	    if (fh == NULL) 
		{
		  	printf("Failed to create %s\n", RESOLV_CONF_FILE);
		  	return;
		}
		if(1==managed)
		{
			if(isFileExist(DNSV6_ADDR_FILE))
			{
				if((fp=fopen(DNSV6_ADDR_FILE,"r"))!=NULL)
				{			
					memset(tmpBuf, 0, sizeof(tmpBuf));
					while(fgets(tmpBuf, sizeof(tmpBuf), fp))
					{
						tmpBuf[strlen(tmpBuf)-1]=0;
						write(fh, tmpBuf, strlen(tmpBuf));
						memset(tmpBuf, 0, sizeof(tmpBuf));
						sprintf(tmpBuf, "\n");
						write(fh, tmpBuf, strlen(tmpBuf));
					}
					fclose(fp);
				}
			}	
		}
		else if(0==managed && 1==others)
		{
			if(isFileExist(DNSV6_ADDR_FILE))
			{
				if((fp=fopen(DNSV6_ADDR_FILE,"r"))!=NULL)
				{			
					memset(tmpBuf, 0, sizeof(tmpBuf));
					while(fgets(tmpBuf, sizeof(tmpBuf), fp))
					{
						tmpBuf[strlen(tmpBuf)-1]=0;
						write(fh, tmpBuf, strlen(tmpBuf));
						memset(tmpBuf, 0, sizeof(tmpBuf));
						sprintf(tmpBuf, "\n");
						write(fh, tmpBuf, strlen(tmpBuf));
					}
					fclose(fp);
				}
			}
			if(isFileExist(DNSV6_RDNSSD_FILE))
			{
				if((fp=fopen(DNSV6_RDNSSD_FILE,"r"))!=NULL)
				{			
					memset(tmpBuf, 0, sizeof(tmpBuf));
					while(fgets(tmpBuf, sizeof(tmpBuf), fp))
					{
						tmpBuf[strlen(tmpBuf)-1]=0;
						write(fh, tmpBuf, strlen(tmpBuf));
						memset(tmpBuf, 0, sizeof(tmpBuf));
						sprintf(tmpBuf, "\n");
						write(fh, tmpBuf, strlen(tmpBuf));
					}
					fclose(fp);
				}
			}
		}
		else if(0==managed && 0==others)
		{
			 if(isFileExist(DNSV6_RDNSSD_FILE))
			 {
				 if((fp=fopen(DNSV6_RDNSSD_FILE,"r"))!=NULL)
				 {			 
					 memset(tmpBuf, 0, sizeof(tmpBuf));
					 while(fgets(tmpBuf, sizeof(tmpBuf), fp))
					 {
						 tmpBuf[strlen(tmpBuf)-1]=0;
						 write(fh, tmpBuf, strlen(tmpBuf));
						 memset(tmpBuf, 0, sizeof(tmpBuf));
						 sprintf(tmpBuf, "\n");
						 write(fh, tmpBuf, strlen(tmpBuf));
					 }
					 fclose(fp);
				 }
			 }
		}	
	 	close(fh);
	}
	else //dns forward mode
	{
		if(0==access("/var/dhcp6c_dnss_need_update",0) || 0==access("/var/dhcp6c_dnsl_need_update",0))	
		{
			restart_dhcp6s();
			if(0==access("/var/dhcp6c_dnss_need_update",0))
				system("rm /var/dhcp6c_dnss_need_update");
			if(0==access("/var/dhcp6c_dnsl_need_update",0))
				system("rm /var/dhcp6c_dnsl_need_update");
		}
		if(0==access("/var/rdnssd_dnss_need_update",0)|| 0==access("/var/rdnssd_dnsl_need_update",0))
		{
			handler_radns(reso_data);
			if(0==access("/var/rdnssd_dnss_need_update",0))
				system("rm /var/rdnssd_dnss_need_update");
			if(0==access("/var/rdnssd_dnsl_need_update",0))
				system("rm /var/rdnssd_dnsl_need_update");
		}
	}
}
#endif
int handle_ra(struct msghdr * pMsg)
{
	RA_INFO_ITEM_T recv_data={0};
#ifdef CONFIG_IPV6_DNSV6_MODE_SUPPORT
	int dnsforward;
	memcpy(&recv_data,NLMSG_DATA(pMsg->msg_iov->iov_base),sizeof(recv_data));
	if ( !apmib_init()) 
	{
		printf("Initialize AP MIB failed !\n");
		return -1;
	}

		if(last_recv_data.icmp6_managed != recv_data.icmp6_managed
		|| last_recv_data.icmp6_other != recv_data.icmp6_other
		|| 0==access("/var/dhcp6c_dnss_need_update",0)
		|| 0==access("/var/rdnssd_dnss_need_update",0)
		|| 0==access("/var/dhcp6c_dnsl_need_update",0)
		|| 0==access("/var/rdnssd_dnsl_need_update",0))
		{
		//not same as last, do change
			memcpy(&last_recv_data,&recv_data,sizeof(recv_data));
			update_dnsmode(&recv_data);
		}
	//the same as last, do nothing
	return 0;
#endif
#ifdef CONFIG_IPV6_CE_ROUTER_SUPPORT
	if(last_recv_data.icmp6_managed==recv_data.icmp6_managed
		&& last_recv_data.icmp6_other==recv_data.icmp6_other
		&& last_recv_data.slaacFail==recv_data.slaacFail)
		{//the same as last, do nothing
			return 0;
		}
	//It changed, save and apply the change
	memcpy(&last_Ra_data,&recv_data,sizeof(recv_data));
	restart_dhcp6c(&recv_data);
#endif
}

#ifdef CONFIG_IPV6_CE_ROUTER_SUPPORT
int restart_dhcp6c(RA_INFO_ITEM_Tp raData)
{
	FILE *fp = NULL;
	int fh;
	struct duid_t dhcp6c_duid;
	struct sockaddr hwaddr={0};
	uint16 len;
	int opmode,wisp_wan_id,wan_linkType,dhcpPdEnable,val,dhcpRapidCommitEnable,pid;
	char wan_interface[16]={0};
	char filename[64];
	char pidname[64];
	char tmpStr[256];
	int managed=0,others=0;

	managed=raData->icmp6_managed;
	if(raData->slaacFail)//rfc6204 WAA-7 require
		managed=1;
	
	sprintf(pidname,DHCP6C_PID_FILE);
	if(isFileExist(pidname)){
		pid=getPid_fromFile(pidname);
		if(pid>0){
			sprintf(tmpStr, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", tmpStr, NULL_STR);
		}
		unlink(pidname);
	}	

	if(managed==0 && others==0)
		return;
	if ( !apmib_init()) {
		printf("Initialize AP MIB failed !\n");
		return -1;
	}

	apmib_get(MIB_OP_MODE,(void *)&opmode);
	apmib_get(MIB_WISP_WAN_ID,(void *)&wisp_wan_id);
	apmib_get(MIB_IPV6_LINK_TYPE,(void *)&wan_linkType);
	if(wan_linkType==IPV6_LINKTYPE_PPP){
		sprintf(wan_interface,"ppp0");
	}else
	{
		if(opmode==GATEWAY_MODE){
			sprintf(wan_interface, "%s", "eth1");
		}
		else if(opmode==WISP_MODE){				
			sprintf(wan_interface, "wlan%d", wisp_wan_id);
		}
	}
	
#ifdef TR181_V6_SUPPORT
	{
		char value[64]={0};
		if(apmib_get(MIB_IPV6_DHCPC_IFACE,(void*)&value)==0)
		{
			printf("get MIB_IPV6_DHCPC_IFACE fail!\n");
			return -1;
		}
		if(value[0])
			strcpy(wan_interface,value);
	}
#endif
	/*for test use fixed duid of 0003000100e04c8196c9*/
	if(!isFileExist(DHCP6C_DUID_FILE)){
		/*create config file*/
		fp=fopen(DHCP6C_DUID_FILE,"w+");
		if(fp==NULL){
			fprintf(stderr, "Create %s file error!\n", DHCP6C_DUID_FILE);
			return;
		}
		
		dhcp6c_duid.duid_type=3;
		dhcp6c_duid.hw_type=1;
		if ( getInAddr(wan_interface, HW_ADDR_T, (void *)&hwaddr )==0)
		{
				fprintf(stderr, "Read hwaddr Error\n");
				return; 
		}
		memcpy(dhcp6c_duid.mac,hwaddr.sa_data,6);

		len=sizeof(dhcp6c_duid);
		if ((fwrite(&len, sizeof(len), 1, fp)) != 1) {
			fprintf(stderr, "write %s file error!\n", DHCP6C_DUID_FILE);
		}
		else if(fwrite(&dhcp6c_duid,sizeof(dhcp6c_duid),1,fp)!=1)
			fprintf(stderr, "write %s file error!\n", DHCP6C_DUID_FILE);
		
		fclose(fp);
	}

	
	if ( !apmib_get(MIB_IPV6_DHCP_PD_ENABLE,(void *)&dhcpPdEnable)){
				fprintf(stderr, "get mib %d error!\n", MIB_IPV6_DHCP_PD_ENABLE);
				return;
			}	
	
			
			sprintf(filename,DHCP6C_CONF_FILE);
			if(isFileExist(filename) == 0)
			/*create config file*/
				fh = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRWXO|S_IRWXG);	
			else
				fh = open(filename, O_RDWR|O_TRUNC, S_IRWXO|S_IRWXG);	
		
			if (fh < 0){
				fprintf(stderr, "Create %s file error!\n", filename);
				return;
			}
	
		
			sprintf(tmpStr, "interface %s {\n",wan_interface);				
			write(fh, tmpStr, strlen(tmpStr));
			if(dhcpPdEnable){
				sprintf(tmpStr, "	send ia-pd %d;\n",100);
				write(fh, tmpStr, strlen(tmpStr));
			}
#ifdef TR181_V6_SUPPORT
			if(!apmib_get(MIB_IPV6_DHCPC_REQUEST_ADDR,(void *)&val)){	
				fprintf(stderr, "get mib MIB_IPV6_DHCPC_REQUEST_ADDR error!\n");
				return; 		
			}
			if(val)
			{
#endif
				if(managed)
				{
					sprintf(tmpStr, "	send ia-na %d;\n",101);
					write(fh, tmpStr, strlen(tmpStr));
				}
#ifdef TR181_V6_SUPPORT
			}
#endif
	
			if(!apmib_get(MIB_IPV6_DHCP_RAPID_COMMIT_ENABLE,(void *)&dhcpRapidCommitEnable)){	
				fprintf(stderr, "get mib %d error!\n", MIB_IPV6_LINK_TYPE);
				close(fh);
				return; 		
			}
			if(dhcpRapidCommitEnable){
				sprintf(tmpStr, "	send rapid-commit;\n");
				write(fh, tmpStr, strlen(tmpStr));	
			}
			sprintf(tmpStr, "	allow reconfigure-accept;\n");
				write(fh, tmpStr, strlen(tmpStr));
#ifndef TR181_V6_SUPPORT
			/*dns*/
			sprintf(tmpStr, "	request domain-name-servers;\n");
			write(fh, tmpStr, strlen(tmpStr));
#else
			{
				int i=0;
				DHCPV6C_SENDOPT_T entryTmp={0};
				for(i=1;i<=IPV6_DHCPC_SENDOPT_NUM;i++)
				{
					*((char *)&entryTmp) = (char)i;
					if ( !apmib_get(MIB_IPV6_DHCPC_SENDOPT_TBL, (void *)&entryTmp)){
						printf("get MIB_IPV6_DHCPC_SENDOPT_TBL fail!\n");
						return;
					}
					if(entryTmp.enable)
					{
						bzero(tmpStr,sizeof(tmpStr));
						switch(entryTmp.tag)
						{
							case 21:
								sprintf(tmpStr, "	request sip-domain-name;\n");
								break;
							case 22:
								sprintf(tmpStr, "	request sip-server-address;\n");
								break;
							case 23:
								sprintf(tmpStr, "	request domain-name-servers;\n");
								break;
							case 27:
								sprintf(tmpStr, "	request nis-server-address;\n");
								break;
							case 28:
								sprintf(tmpStr, "	request nisp-server-address;\n");
								break;
							case 29:
								sprintf(tmpStr, "	request nis-domain-name;\n");
								break;
							case 30:
								sprintf(tmpStr, "	request nisp-domain-name;\n");
								break;
							case 33:
								sprintf(tmpStr, "	request bcmcs-domain-name;\n");
								break;
							case 34:
								sprintf(tmpStr, "	request bcmcs-server-address;\n");
								break;
							default:
								break;
						}
						if(tmpStr[0])
							write(fh, tmpStr, strlen(tmpStr));
					}
				}
			}
#endif
			sprintf(tmpStr, "	script \"/var/dhcp6cRcv.sh\";\n");
			write(fh, tmpStr, strlen(tmpStr));
			system("cp /bin/dhcp6cRcv.sh /var/dhcp6cRcv.sh");
			sprintf(tmpStr, "};\n\n");
			write(fh, tmpStr, strlen(tmpStr));
	
			if(dhcpPdEnable){
				sprintf(tmpStr, "id-assoc pd %d {\n",100);
				write(fh, tmpStr, strlen(tmpStr));	
#ifdef TR181_V6_SUPPORT
				sprintf(tmpStr, "	suggest-t {\n");
				write(fh, tmpStr, strlen(tmpStr));
				if(!apmib_get(MIB_IPV6_DHCPC_SUGGESTEDT1,(void *)&val)){	
					fprintf(stderr, "get mib MIB_IPV6_DHCPC_SUGGESTEDT1 error!\n");
					return; 		
				}
				if(val >0)
				{
					sprintf(tmpStr, "		t1 %d;\n", val);
					write(fh, tmpStr, strlen(tmpStr));
				}
				if(!apmib_get(MIB_IPV6_DHCPC_SUGGESTEDT2,(void *)&val)){	
					fprintf(stderr, "get mib MIB_IPV6_DHCPC_SUGGESTEDT2 error!\n");
					return; 		
				}
				if(val > 0)
				{
					sprintf(tmpStr, "		t2 %d;\n", val);
					write(fh, tmpStr, strlen(tmpStr));
				}
				sprintf(tmpStr, "	};\n");
				write(fh, tmpStr, strlen(tmpStr));
#endif
				sprintf(tmpStr, "		prefix-interface br0 {\n");
				write(fh, tmpStr, strlen(tmpStr));				
				sprintf(tmpStr, "			sla-id 0;\n");
				write(fh, tmpStr, strlen(tmpStr));
				sprintf(tmpStr, "			sla-len 0;\n");
				write(fh, tmpStr, strlen(tmpStr));
				sprintf(tmpStr, "		};\n");
				write(fh, tmpStr, strlen(tmpStr));
				sprintf(tmpStr, "};\n\n");
				write(fh, tmpStr, strlen(tmpStr));	
			}						

			if(managed)
			{
				/*ia-na*/
				sprintf(tmpStr, "id-assoc na %d {\n",101);
				write(fh, tmpStr, strlen(tmpStr));
#ifdef TR181_V6_SUPPORT
				sprintf(tmpStr, "	suggest-t {\n");
				write(fh, tmpStr, strlen(tmpStr));
		
				if(!apmib_get(MIB_IPV6_DHCPC_SUGGESTEDT1,(void *)&val)){	
					fprintf(stderr, "get mib MIB_IPV6_DHCPC_SUGGESTEDT1 error!\n");
					return; 		
				}
				if(val >0)
				{
					sprintf(tmpStr, "		t1 %d;\n", val);
					write(fh, tmpStr, strlen(tmpStr));
				}
				if(!apmib_get(MIB_IPV6_DHCPC_SUGGESTEDT2,(void *)&val)){	
					fprintf(stderr, "get mib MIB_IPV6_DHCPC_SUGGESTEDT2 error!\n");
					return; 		
				}
				if(val > 0)
				{
					sprintf(tmpStr, "		t2 %d;\n", val);
					write(fh, tmpStr, strlen(tmpStr));
				}
				sprintf(tmpStr, "	};\n");
				write(fh, tmpStr, strlen(tmpStr));
#endif
				sprintf(tmpStr, "};\n\n");
				write(fh, tmpStr, strlen(tmpStr));	
			}

			if(dhcpPdEnable||managed)
			{
				sprintf(tmpStr, "authentication reconfigure {\n\
					protocol reconfig;\n\
					algorithm hmac-md5;\n\
					rdm monocounter;\n\
					};");
				write(fh, tmpStr, strlen(tmpStr));
			}
			close(fh);
		
	
			
			/*start daemon*/
			sprintf(tmpStr, "dhcp6c -c %s -p %s %s ", DHCP6C_CONF_FILE,DHCP6C_PID_FILE,wan_interface);
			/*Use system() instead of RunSystemCmd() to avoid stderr closing, 
			process itself will redirect stderr when it wants to run as deamon() */
			system(tmpStr);
			//printf("%s\n",tmpStr);
		return;
}
#endif
int get_handle_ra(int enable)
{
//use netlink to fit kernel's RA receive
	struct sockaddr_nl src_addr, dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	int sock_fd;
	ssize_t n;
	struct msghdr msg;
	RA_INFO_ITEM_T send_data={0};
	int pid=0;
	char tmpbuf[16]={0};
	
	send_data.enable=enable;	
	send_data.pid=getpid();
	
//init
        sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_RTK_IPV6_RA);
        memset(&msg, 0, sizeof(msg));
        memset(&src_addr, 0, sizeof(src_addr));
        src_addr.nl_family = AF_NETLINK;
        src_addr.nl_pid = getpid(); /* self pid */
        src_addr.nl_groups = 0; /* not in mcast groups */
        bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
        memset(&dest_addr, 0, sizeof(dest_addr));
        dest_addr.nl_family = AF_NETLINK;
        dest_addr.nl_pid = 0; /* For Linux Kernel */
        dest_addr.nl_groups = 0; /* unicast */

//prepare data
        nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(BUFSIZE));
/* Fill the netlink message header */
        nlh->nlmsg_len = NLMSG_SPACE(BUFSIZE);
        nlh->nlmsg_pid = getpid(); /* self pid */
        nlh->nlmsg_flags = 0;
/* Fill in the netlink message payload */
		memcpy(NLMSG_DATA(nlh), &send_data,sizeof(send_data));
		iov.iov_base = (void *)nlh;
		iov.iov_len = nlh->nlmsg_len;
		msg.msg_name = (void *)&dest_addr;
		msg.msg_namelen = sizeof(dest_addr);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;

	sendmsg(sock_fd, &msg, 0);
	
	// kill enable daemon if exist
	if(isFileExist(IPV6_MANG_PID_FILE)) {
		pid=getPid_fromFile(IPV6_MANG_PID_FILE);
		if(pid){
			sprintf(tmpbuf, "%d", pid);
			RunSystemCmd(NULL_FILE, "kill", "-9", tmpbuf, NULL_STR);							
		}
		unlink(IPV6_MANG_PID_FILE);
	}
	if(send_data.enable==0)
	{
		
		usleep(100);
		close(sock_fd);		

		return 1;
	}
	
	setPid_toFile(IPV6_MANG_PID_FILE);
	for(;;)
	{
		//bzero(serverSock.sa_data,sizeof(serverSock.sa_data));
		n=recvmsg(sock_fd,&msg,0);
		if(n<0)
		{
			if(errno==EINTR)
				continue;
			else{
				fprintf(stderr,"recvmsg fail! n=%d\n",n);
				return -1;
			}
		}
		if(handle_ra(&msg)<0)
			fprintf(stderr,"handle_ra fail!\n");
		
	}
	close(sock_fd);
	return 0;
}
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		fprintf(stderr,"invalid input!\n");
		return -1;
	}
	if(strcmp(argv[1],"start")==0)
	{
		get_handle_ra(1);	
	}
	else if(strcmp(argv[1],"stop")==0)
	{
		get_handle_ra(0);	
	}
	
	return 0;
}
