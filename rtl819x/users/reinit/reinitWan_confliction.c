#include <stdlib.h>
#include <time.h>
#include "reinit_utility.h"
#include "reinitWan_confliction.h"

/*avoid lan and wan confliction*/
int avoid_confliction_ip(char *wanIp, char *wanMask)
{
	char line_buffer[100]={0};
	char *strtmp=NULL;
	char tmp1[64]={0};
	unsigned int tmp1Val;
	struct in_addr inIp, inMask, inGateway;
	struct in_addr myIp, myMask, mask;
	unsigned int inIpVal, inMaskVal, myIpVal, myMaskVal, maskVal;
	char tmpBufIP[64]={0}, tmpBufMask[64]={0};
	DHCP_T dhcp;
	
	apmib_get( MIB_DHCP, (void *)&dhcp);
	
	if(isFileExist(DHCPD_PID_FILE) == 0 || dhcp == DHCP_SERVER){

	}else{
		return 0; //no dhcpd or dhcp server is disable
	}
	
	if ( !inet_aton(wanIp, &inIp) ) {
		printf("\r\n Invalid IP-address value!__[%s-%u]\r\n",__FILE__,__LINE__);
		return 0;
	}
	
	if ( !inet_aton(wanMask, &inMask) ) {
		printf("\r\n Invalid IP-address value!__[%s-%u]\r\n",__FILE__,__LINE__);
		return 0;
	}
	
	memcpy(&inIpVal, &inIp, 4);
	memcpy(&inMaskVal, &inMask, 4);


	getInAddr("br0", IP_ADDR_T, (void *)&myIp );	
	getInAddr("br0", NET_MASK_T, (void *)&myMask );
		
	
	memcpy(&myIpVal, &myIp, 4);
	memcpy(&myMaskVal, &myMask, 4);

//printf("\r\n inIpVal=[0x%x],__[%s-%u]\r\n",inIpVal,__FILE__,__LINE__);
//printf("\r\n inMaskVal=[0x%x],__[%s-%u]\r\n",inMaskVal,__FILE__,__LINE__);
//printf("\r\n myIpVal=[0x%x],__[%s-%u]\r\n",myIpVal,__FILE__,__LINE__);
//printf("\r\n myMaskVal=[0x%x],__[%s-%u]\r\n",myMaskVal,__FILE__,__LINE__);

	memcpy(&maskVal,myMaskVal>inMaskVal?&inMaskVal:&myMaskVal,4);
	
//printf("\r\n maskVal=[0x%x],__[%s-%u]\r\n",maskVal,__FILE__,__LINE__);
	
	if((inIpVal & maskVal) == (myIpVal & maskVal)) //wan ip conflict lan ip 
	{
		int i=0, j=0;
		printf("\r\n wan ip conflict lan ip!,__[%s-%u]\r\n\n",__FILE__,__LINE__);

		for(i=0; i<32; i++)
		{
			if((maskVal & (1<<i)) != 0)
				break;
		}
		
		if((myIpVal & (1<<i)) == 0)
		{
			myIpVal = myIpVal+(1<<i);
		}
		else
		{
			myIpVal = myIpVal-(1<<i);
		}
		
		memcpy(&myIp, &myIpVal, 4);
				
						
		for(j=0; j<32; j++)
		{
			if((maskVal & (1<<j)) != 0)
				break;
		}
		
	//	j=(32-j)/8;

		system("killall -9 udhcpd 2> /dev/null");
		system("rm -f /var/run/udhcpd.pid 2> /dev/null");
		system("rm -f /var/udhcpd.conf");
		
		sprintf(line_buffer,"interface %s\n","br0");
		write_line_to_file(DHCPD_CONF_FILE, 1, line_buffer);
		
		apmib_get(MIB_DHCP_CLIENT_START,  (void *)tmp1);		
	//	memcpy(tmp1, &myIpVal,  j);
		*(unsigned int*)tmp1 ^= (1<<(j));
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
//printf("\r\n start ip=[%s],__[%s-%u]\r\n",strtmp,__FILE__,__LINE__);		
		sprintf(line_buffer,"start %s\n",strtmp);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
		apmib_get(MIB_DHCP_CLIENT_END,  (void *)tmp1);		
		//memcpy(tmp1, &myIpVal,  j);
		*(unsigned int*)tmp1 ^= (1<<(j));
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
//printf("\r\n end ip=[%s],__[%s-%u]\r\n",strtmp,__FILE__,__LINE__);		
		sprintf(line_buffer,"end %s\n",strtmp);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
	
//printf("\r\n subnet mask=[%s],__[%s-%u]\r\n",inet_ntoa(myMask),__FILE__,__LINE__);			
		sprintf(line_buffer,"opt subnet %s\n",inet_ntoa(myMask));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
		apmib_get(MIB_DHCP_LEASE_TIME, (void *)&tmp1Val);
		if( (tmp1Val==0) || (tmp1Val<0) || (tmp1Val>10080))
		{
			tmp1Val = 480; //8 hours
			if(!apmib_set(MIB_DHCP_LEASE_TIME, (void *)&tmp1Val))
			{
				printf("set MIB_DHCP_LEASE_TIME error\n");
			}
		
			apmib_update(CURRENT_SETTING);
		}
		tmp1Val *= 60;

		sprintf(line_buffer,"opt lease %ld\n",tmp1Val);
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

//printf("\r\n gateway ip=[%s],__[%s-%u]\r\n",inet_ntoa(myIp),__FILE__,__LINE__);					
		sprintf(line_buffer,"opt router %s\n",inet_ntoa(myIp));
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);

//printf("\r\n dns ip=[%s],__[%s-%u]\r\n",inet_ntoa(myIp),__FILE__,__LINE__);							
		sprintf(line_buffer,"opt dns %s\n",inet_ntoa(myIp)); /*now strtmp is ip address value */
		write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		
		memset(tmp1,0x00,sizeof(tmp1));
		apmib_get( MIB_DOMAIN_NAME, (void *)&tmp1);
		if(tmp1[0]){
			sprintf(line_buffer,"opt domain %s\n",tmp1);
			write_line_to_file(DHCPD_CONF_FILE, 2, line_buffer);
		}
		
		memset(tmp1,0x00,sizeof(tmp1));
		memcpy(tmp1, &myIpVal,  4);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(tmpBufIP,"%s",strtmp);
//printf("\r\n tmpBufIP=[%s],__[%s-%u]\r\n",tmpBufIP,__FILE__,__LINE__);

		memset(tmp1,0x00,sizeof(tmp1));
		memcpy(tmp1, &myMaskVal,  4);
		strtmp= inet_ntoa(*((struct in_addr *)tmp1));
		sprintf(tmpBufMask,"%s",strtmp);
//printf("\r\n tmpBufMask=[%s],__[%s-%u]\r\n",tmpBufMask,__FILE__,__LINE__);

		memset(line_buffer,0x00,sizeof(line_buffer));
		sprintf(line_buffer, "ifconfig br0 %s netmask %s", tmpBufIP, tmpBufMask);
//printf("\r\n line_buffer=[%s],__[%s-%u]\r\n",line_buffer,__FILE__,__LINE__);									
		system(line_buffer);

		sprintf(line_buffer, "udhcpd %s", DHCPD_CONF_FILE);
		system(line_buffer);
		//start_dnrd();
		return 1;
	}

	return 0;
}

static int check_wlSchedule()
{
	int wlschEnable=0;
	int entryNum=0;
	SCHEDULE_T wlan_sched={0};
	int i=0;
	int current=0;
	time_t tm;
	struct tm tm_time;
	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	current = tm_time.tm_hour * 60 + tm_time.tm_min;
	if(apmib_get(MIB_WLAN_SCHEDULE_ENABLED,(void*)&wlschEnable)==0)
		return -1;
	if(apmib_get(MIB_WLAN_SCHEDULE_TBL_NUM, (void *)&entryNum)==0)
		return -1;
	if(wlschEnable==0)
	 return 1;
	for (i=1; i<=entryNum; i++) 
	{
		*((char *)&wlan_sched) = (char)i;
		apmib_get(MIB_WLAN_SCHEDULE_TBL, (void *)&wlan_sched);
		if(!wlan_sched.eco)
			continue;
		if(wlan_sched.day==7 || wlan_sched.day==tm_time.tm_wday)
		{//day hit
			if(wlan_sched.fTime<=wlan_sched.tTime)
			{//from<current<to
				if(current>=wlan_sched.fTime && current<=wlan_sched.tTime)
					return 1;
			}else
			{//current<to || current >from
				if(current>=wlan_sched.fTime || current<=wlan_sched.tTime)
					return 1;
			}
		}
	}
	return 0;
}

static int stop_wlan_interfaces(void)
{
	int i;
	char wlan_name[16] = {0};
	char virtual_interface[64] = {0};
	char *token=NULL, *savestr1=NULL;
	char tmpBuff[64];
	for(i=0;i<NUM_WLAN_INTERFACE;i++)
	{
		/*down vxd interface*/
		sprintf(wlan_name,"wlan%d-vxd",i);
		RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
		/*down virtual interface*/
		sprintf(wlan_name,"wlan%d-va",i);
		 bzero(virtual_interface,sizeof(virtual_interface));
		if_readlist_proc(virtual_interface, wlan_name, 0);
		if(virtual_interface[0]){
			token=NULL;
			savestr1=NULL;
			sprintf(tmpBuff, "%s", virtual_interface);
			token = strtok_r(tmpBuff," ", &savestr1);
			do{
				if (token == NULL){
					break;
				}else{
					RunSystemCmd(NULL_FILE, "ifconfig", token, "down", NULL_STR);
				}
				token = strtok_r(NULL, " ", &savestr1);
			}while(token !=NULL);
		}
		/*down root interface*/
		sprintf(wlan_name,"wlan%d",i);
		RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "down", NULL_STR);
	 }
	return 0;
}

static int start_wlan_interfaces(void)
	{
		int i;
		char wlan_name[16] = {0};
		char virtual_interface[64] = {0};
		char *token=NULL, *savestr1=NULL;
		char tmpBuff[64];
		int wlan_disabled;
		int wlanBand2G5GSelect;
		apmib_get(MIB_WLAN_BAND2G5G_SELECT,(void *)&wlanBand2G5GSelect);	
		for(i=0;i<NUM_WLAN_INTERFACE;i++)
		{
			if((wlanBand2G5GSelect != BANDMODEBOTH)&&(i==1))
			{
				break;
			}
			/*up root interface*/
			sprintf(wlan_name,"wlan%d",i);
			SetWlan_idx(wlan_name);
			apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disabled);
			if(wlan_disabled)/*root interface is disabled*/
			{
				continue;
			}
			if(check_wlSchedule()!=1)
				continue;
			RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "up", NULL_STR);
	
			/*up vxd interface*/
			sprintf(wlan_name,"wlan%d-vxd",i);
			SetWlan_idx(wlan_name);
			apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disabled);
			if(!wlan_disabled)
			{
				RunSystemCmd(NULL_FILE, "ifconfig", wlan_name, "up", NULL_STR);
			}
			
			/*up virtual interface*/
			sprintf(wlan_name,"wlan%d-va",i);
			 bzero(virtual_interface,sizeof(virtual_interface));
			if_readlist_proc(virtual_interface, wlan_name, 0);
			if(virtual_interface[0]){
				token=NULL;
				savestr1=NULL;
				sprintf(tmpBuff, "%s", virtual_interface);
				token = strtok_r(tmpBuff," ", &savestr1);
				do{
					if (token == NULL){
						break;
					}else{
						SetWlan_idx(token);
						apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&wlan_disabled);
						if(!wlan_disabled)
						{
							RunSystemCmd(NULL_FILE, "ifconfig", token, "up", NULL_STR);
						}
					}
					token = strtok_r(NULL, " ", &savestr1);
				}while(token !=NULL);
			}
		}
		return 0;
	}



void check_wan_confliction(char *wanIp, char *wanMask){
	int ret = 0, op_mode = 0, intValue = 0;
	
	/*avoid lan and wan confliction*/
	ret = avoid_confliction_ip(wanIp,wanMask);
	apmib_get(MIB_OP_MODE,(void *)&op_mode);
	
#if defined(CONFIG_POCKET_ROUTER_SUPPORT)
		if(ret == 1 && opmode == GATEWAY_MODE)
#else		
		if(ret == 1)
#endif
		{		
			if(op_mode != WISP_MODE)
			{
				stop_wlan_interfaces();
			}
#if !defined(CONFIG_POCKET_ROUTER_SUPPORT)
		//when op_mode == GATEWAY_MODE for pocket AP, there isn't interface eth0
			system("ifconfig eth0 down");
#endif
			sleep(10);
	
#if !defined(CONFIG_POCKET_ROUTER_SUPPORT)
			system("ifconfig eth0 up");
#endif
	
			if(op_mode != WISP_MODE)
			{
				start_wlan_interfaces();
				
			}
		}
	
}

