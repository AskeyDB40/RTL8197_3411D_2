/*******************************
* 802.1Q Linux VLAN Support
* Added by Luke Wang, 2015/10
*******************************/

#ifdef CONFIG_8021Q_VLAN_SUPPORTED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>


#include "apmib.h"

#define VLAN_TYPE_NAT 0
#define VLAN_TYPE_BRIDGE 1

#define SOURCE_TAG_MODE (1 << 2)

#define DEFAULT_NAT_LAN_VID 9
#define DEFAULT_NAT_WAN_VID 8

#define DEFAULT_ETH_WAN_PORT_MASK 0x0010
#define CPU_PORT_MASK 0x0040

#define LINUX_VLAN_REINIT_FILE "/var/system/linux_vlan_reinit"

static const char *portDisplayName[] =
{
	"port1", "port2", "port3", "port4", "port5",
	"wlan1", "wlan1-va1", "wlan1-va2", "wlan1-va3", "wlan1-va4", "wlan1-vxd",
	"wlan2", "wlan2-va1", "wlan2-va2", "wlan2-va3", "wlan2-va4", "wlan2-vxd"
};

static const char *portRealName[] =
{
	#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	"eth0", "eth2", "eth3", "eth4", "eth1",
	#else
	"eth0", "eth0", "eth0", "eth0", "eth1",
	#endif
	"wlan0", "wlan0-va0", "wlan0-va1", "wlan0-va2", "wlan0-va3", "wlan0-vxd",
	"wlan1", "wlan1-va0", "wlan1-va1", "wlan1-va2", "wlan1-va3", "wlan1-vxd"
};

#define APMIB_GET(A, B)	 \
	{if(!apmib_get(A, B)) {	\
		printf("%s:%d APMIB_GET %s error!\n", __FUNCTION__, __LINE__, #A);	\
		return -1;	\
	}}

#define APMIB_SET(A, B)	  \
	{if(!apmib_set(A, B)) {	\
		printf("%s:%d APMIB_SET %s error!\n", __FUNCTION__, __LINE__, #A);	\
		return -1;	\
	}}

int RUN_CMD(const char *fmt, ...)
{
	char cmdBuf[256];
	va_list args;
	int ret = 0;

	va_start(args, fmt);
	ret = vsnprintf(cmdBuf, 256, fmt, args);
	va_end(args);

	if(ret < 0)
		return ret;

	system(cmdBuf);

	//printf("%s\n", cmdBuf);

	return 0;
}

static int getWanPortMask(void);
static int setDefaultPVid(void)
{
	int port_idx=0, port_num=0, opmode=0;
	int wan_mask=0, wan_idx=0;
	char pVidArray[MAX_VLAN_PORT_NUM * 2];

	APMIB_GET(MIB_OP_MODE, (void *)&opmode);
	port_num = sizeof(portDisplayName)/sizeof(char *);
	memset((void *)pVidArray, 0, MAX_VLAN_PORT_NUM * 2);

	wan_mask = getWanPortMask();
	for(wan_idx=0; wan_idx<port_num; wan_idx++)
		if(1<<wan_idx == wan_mask) break;

	//LAN/WLAN port.
	for(port_idx=0; port_idx<port_num; port_idx++)
	{
		if(1<<port_idx == wan_mask)
			continue;

		if(port_idx>=0 && port_idx<=4)
			*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
		else
			*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
	}

	//WAN port.
	if(wan_mask != 0)
	{	
		*((short *)pVidArray + wan_idx) = DEFAULT_NAT_WAN_VID;
	}
		
	APMIB_SET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray);
	
	return 0;
} 

//Delete all vlan entries when opMode changed.
static int opModeChangeDelAll(void)
{
	int currentOpMode=0, vlanOpMode=0, hw_nat_lan_vid=0;
	VLAN_CONFIG_T entry;

	APMIB_GET(MIB_OP_MODE, (void *)&currentOpMode);

	APMIB_GET(MIB_VLAN_OPMODE, (void *)&vlanOpMode);

	if(currentOpMode != (vlanOpMode & ~SOURCE_TAG_MODE) ||
	#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
		!(vlanOpMode & SOURCE_TAG_MODE)
	#else
		(vlanOpMode & SOURCE_TAG_MODE)
	#endif
		)
	{
		APMIB_SET(MIB_VLAN_DELALL, (void *)&entry);

		#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
		currentOpMode |= SOURCE_TAG_MODE;
		#endif
		APMIB_SET(MIB_VLAN_OPMODE, (void *)&currentOpMode);

		setDefaultPVid();

		hw_nat_lan_vid = DEFAULT_NAT_LAN_VID;	
		APMIB_SET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);

		apmib_update(CURRENT_SETTING);
	}

	return 0;
}

//Clean existed eth0.xxx and br device, for linux vlan reinit.
static int reinitCleanDevice(void)
{
	char reinitBuf[256];
	char *token=NULL, *saveStr=NULL;
	FILE *fp = NULL;
	int br_created_num=0, idx=0;

	if(!isFileExist(LINUX_VLAN_REINIT_FILE))
	{
		return 0;
	}
		
	fp = fopen(LINUX_VLAN_REINIT_FILE, "r");
	if(!fp)
		return -1;

	memset(reinitBuf, 0, sizeof(reinitBuf));
	fgets(reinitBuf, sizeof(reinitBuf), fp);

	token = strtok_r(reinitBuf, ":", &saveStr);
	do{
		if(!token)
			break;
		
		if(memcmp(token, "eth", 3)==0 || memcmp(token, "wlan", 4)==0)
		{
			RUN_CMD("ifconfig %s down", token);
			RUN_CMD("vconfig rem %s", token);
		}
		else
		{
			br_created_num = strtol(token, NULL, 10);
			if(br_created_num<0 || br_created_num>MAX_VLAN_CONFIG_NUM)
				br_created_num = 0;

			break;
		}

		token = strtok_r(NULL, ":", &saveStr);
	}
	while(token);

	if(!br_created_num)
		br_created_num = MAX_VLAN_CONFIG_NUM+1;
	
	for(idx=1; idx<br_created_num; idx++)
	{
		RUN_CMD("ifconfig br%d down 2>/dev/null", idx);
		RUN_CMD("brctl delbr br%d 2>/dev/null", idx);
	}

	fclose(fp);
	RUN_CMD("rm %s", LINUX_VLAN_REINIT_FILE);
		
	return 0;
}

static int wlanDevUpFlag = 0;
static void getWlanUpFlag(void)
{
	struct ifreq ifr={0};
	int sockfd=0, i=0;	

	wlanDevUpFlag = 0;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		return;
	

	for(i=5; i<=16; i++)
	{
		strcpy(ifr.ifr_name, portRealName[i]);

		if(ioctl(sockfd, SIOCGIFFLAGS, &ifr)>=0 && ifr.ifr_flags&IFF_UP)
			wlanDevUpFlag |= 1<<i;
	}

	close(sockfd);

	return;
}

static VLAN_CONFIG_T entry;
static unsigned char hw_addr[6];
static char pVidArray[MAX_VLAN_PORT_NUM*2], reinitBuf[256];
static int entry_num=0, port_num=0, cpu_port_pvid=0, br_created_num=0;

static int setVlanGatewayMode(void);
static int setVlanBridgeMode(void);

int initLinuxVlan(void)
{
	int	vlan_enabled = 0, opmode=0;

	reinitCleanDevice();

	opModeChangeDelAll();

	APMIB_GET(MIB_OP_MODE, (void *)&opmode);
	if(opmode != GATEWAY_MODE && opmode != BRIDGE_MODE)
	{
		RUN_CMD("echo \"0 0\" > /proc/net/vlan/vlanEnable");
		return 0;
	}

	APMIB_GET(MIB_VLAN_ENABLED, (void *)&vlan_enabled);
	if(!vlan_enabled)
		RUN_CMD("echo \"0 0\" > /proc/net/vlan/vlanEnable");
	else
		RUN_CMD("echo \"1 1\" > /proc/net/vlan/vlanEnable");

	return 0;
}

#if defined (CONFIG_RTL_MULTICAST_PORT_MAPPING)
static int setMCPortMapping(void);
#endif

int setLinuxVlan(void)
{
	int	vlan_enabled=0, port_idx=0, opmode=0;
	char cmdBuf[150], tmpBuf[16]; 

	APMIB_GET(MIB_OP_MODE, (void *)&opmode);
	if(opmode != GATEWAY_MODE && opmode != BRIDGE_MODE)
		return 0;
	
	APMIB_GET(MIB_VLAN_ENABLED, (void *)&vlan_enabled);
	if(!vlan_enabled)
		return 0;

	getWlanUpFlag();

	port_num = sizeof(portDisplayName)/sizeof(char *);
	APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num);
	
	APMIB_GET(MIB_ELAN_MAC_ADDR, (void *)hw_addr);
	if(hw_addr[0]==0 && hw_addr[1]==0 && hw_addr[2]==0 &&
		hw_addr[3]==0 && hw_addr[4]==0 && hw_addr[5]==0)
	{
		APMIB_GET(MIB_HW_NIC0_ADDR, (void *)hw_addr);
	}

	br_created_num = 0;
	cpu_port_pvid = 0;
	memset((void *)&entry, 0, sizeof(VLAN_CONFIG_T));
	memset((void *)reinitBuf, 0, sizeof(reinitBuf));

	//Delete wlan ports already existed in br0.
	for(port_idx=5; port_idx<port_num; port_idx++)
	{			
		if(wlanDevUpFlag & (1<<port_idx))
			RUN_CMD("brctl delif br0 %s 2>/dev/null", portRealName[port_idx]);
	}

	switch(opmode)
	{
		case GATEWAY_MODE:
			setVlanGatewayMode();
			break;
			
		case BRIDGE_MODE:
			setVlanBridgeMode(); 
			break;
	}

	#if defined (CONFIG_RTL_MULTICAST_PORT_MAPPING)
	if(opmode == GATEWAY_MODE)
		setMCPortMapping();
	#endif
	
	//Set linux vlan reinit file.
	sprintf(tmpBuf, "%d:", br_created_num+1);
	strcat(reinitBuf, tmpBuf);
	RUN_CMD("echo %s > %s", reinitBuf, LINUX_VLAN_REINIT_FILE);

	//Set PVID.
	APMIB_GET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray);

	if(! cpu_port_pvid)
		cpu_port_pvid = DEFAULT_NAT_LAN_VID;

	sprintf(cmdBuf, "echo \"");
	for(port_idx=0; port_idx<port_num+4; port_idx++)
	{
		if(port_idx < 5)
			sprintf(tmpBuf, "%d,", *((short *)pVidArray+port_idx));
		else if(port_idx>=5 && port_idx<9)
		{
			if(port_idx == 8)
				sprintf(tmpBuf, "%d,", cpu_port_pvid);
			else
				sprintf(tmpBuf, "%d,", 1);
		}
		else if(port_idx>=9 && port_idx<port_num+4)
		{
			if(wlanDevUpFlag & 1<<(port_idx-4))
				sprintf(tmpBuf, "%d,", *((short *)pVidArray+port_idx-4));
			else
				sprintf(tmpBuf, "%d,", 0);
		}
		
		strcat(cmdBuf, tmpBuf);
	}
	strcat(cmdBuf, "\" > /proc/net/vlan/pvid");
	system(cmdBuf);
	printf("%s\n", cmdBuf);
		
	return 0;
}

static int setVlanGatewayMode(void)
{
	int idx=0, port_idx=0, wan_mask=0, nat_lan_port_mask;
	int port_member_mask=0, port_tagged_mask=0, hw_nat_lan_vid=0, lan_port_remained=0;
	char tmpBuf[20];

	wan_mask = getWanPortMask();
	lan_port_remained = 0x1ffff & (~wan_mask);
	APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);
	
	RUN_CMD("brctl delif br0 eth0");
	#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	RUN_CMD("brctl delif br0 eth2");
	RUN_CMD("brctl delif br0 eth3");
	RUN_CMD("brctl delif br0 eth4");
	#endif
		
	cpu_port_pvid = DEFAULT_NAT_LAN_VID;

	//Set NAT LAN/WAN groups.
	for(idx=1; idx<=entry_num; idx++)
	{
		*(char *)&entry = (char)idx;
		APMIB_GET(MIB_VLAN_TBL, (void *)&entry);
	
		#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
		if(entry.VlanType == VLAN_TYPE_NAT)
		#else
		if(entry.VlanType == VLAN_TYPE_NAT && !(entry.MemberPortMask & wan_mask))
		#endif
		{
			lan_port_remained &= ~entry.MemberPortMask;
			nat_lan_port_mask = entry.MemberPortMask & 0x0000001f;
			
			port_member_mask = entry.MemberPortMask & 0x0000001f;   //LAN ports.
			port_member_mask |= CPU_PORT_MASK;	//CPU port.
			port_member_mask |= (entry.MemberPortMask & 0x0001ffe0) << 4;   //WLAN ports.
			
			port_tagged_mask = entry.TaggedPortMask & 0x0000001f; 
			port_tagged_mask |= CPU_PORT_MASK;
			port_tagged_mask |= (entry.TaggedPortMask & 0x0001ffe0) << 4;

			//printf("\nType = VLAN_TYPE_NAT_LAN\n");

			#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
			RUN_CMD("echo %d > /proc/net/vlan/wanVlanId", entry.VlanId);	
			#endif

			if(entry.VlanId == hw_nat_lan_vid)
				RUN_CMD("echo %d > /proc/net/vlan/hwNatLanVlanId", entry.VlanId);

			RUN_CMD("echo %d > /proc/net/vlan/groupIndex", entry.VlanId);

			RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
				port_member_mask, port_tagged_mask, entry.VlanId, VLAN_TYPE_NAT);

			#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
			RUN_CMD("vconfig add eth0 %d", entry.VlanId);
			RUN_CMD("ifconfig eth0.%d up", entry.VlanId);
			RUN_CMD("brctl addif br0 eth0.%d", entry.VlanId);
			
			sprintf(tmpBuf, "eth0.%d:", entry.VlanId);
			strcat(reinitBuf, tmpBuf);
			#else
			for(port_idx=0; port_idx<4; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx))
					continue;

				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], entry.VlanId);

				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}
			#endif
			
			for(port_idx=5; port_idx<port_num; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx) || !(wlanDevUpFlag & 1<<port_idx))
					continue;

				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], entry.VlanId);

				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}
		}
		else if(entry.VlanType == VLAN_TYPE_NAT && entry.MemberPortMask == wan_mask)
		{
			port_member_mask = wan_mask | CPU_PORT_MASK;   

			port_tagged_mask = entry.TaggedPortMask&wan_mask ? 
				wan_mask|CPU_PORT_MASK : CPU_PORT_MASK;
			
			//printf("\nType = VLAN_TYPE_NAT_WAN\n");

			RUN_CMD("echo %d > /proc/net/vlan/wanVlanId", entry.VlanId);

			RUN_CMD("echo %d > /proc/net/vlan/groupIndex", entry.VlanId);

			RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
				port_member_mask, port_tagged_mask, entry.VlanId, VLAN_TYPE_NAT);
		}
	}

	//Set bridge groups.
	for(idx=1; idx<=entry_num; idx++)
	{
		*(char *)&entry = (char)idx;
		APMIB_GET(MIB_VLAN_TBL, (void *)&entry);
		
		if(entry.VlanType == VLAN_TYPE_BRIDGE)
		{
			lan_port_remained &= ~entry.MemberPortMask;

			port_member_mask = entry.MemberPortMask & 0x0000001f;   //LAN ports.
			port_member_mask |= CPU_PORT_MASK;	//CPU port.
			port_member_mask |= (entry.MemberPortMask & 0x0001ffe0) << 4;   //WLAN ports.
			
			port_tagged_mask = entry.TaggedPortMask & 0x0000001f; 
			port_tagged_mask |= entry.TaggedPortMask&wan_mask ? 
				wan_mask|CPU_PORT_MASK : CPU_PORT_MASK;
			port_tagged_mask |= (entry.TaggedPortMask & 0x0001ffe0) << 4;

			printf("\nType = VLAN_TYPE_LAN_WAN_BRIDGE\n");

			RUN_CMD("echo %d > /proc/net/vlan/groupIndex", entry.VlanId);

			RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
				port_member_mask, port_tagged_mask, entry.VlanId, VLAN_TYPE_BRIDGE);

			br_created_num += 1;

			RUN_CMD("brctl addbr br%d", br_created_num);

			RUN_CMD("brctl setfd br%d 1", br_created_num);

			RUN_CMD("ifconfig br%d hw ether %02x:%02x:%02x:%02x:%02x:%02x", br_created_num,  \
				hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]+br_created_num);

			#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
			RUN_CMD("vconfig add eth1 %d", entry.VlanId);

			RUN_CMD("ifconfig eth1.%d hw ether %02x:%02x:%02x:%02x:%02x:%02x", entry.VlanId,  \
				hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]+br_created_num);
			
			RUN_CMD("ifconfig eth1.%d up", entry.VlanId);

			RUN_CMD("brctl addif br%d eth1.%d", br_created_num, entry.VlanId);

			sprintf(tmpBuf, "eth1.%d:", entry.VlanId);
			strcat(reinitBuf, tmpBuf);
			#else
			for(port_idx=0; port_idx<5; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx))
					continue;
				
				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br%d %s.%d", br_created_num, portRealName[port_idx], entry.VlanId);

				RUN_CMD("ifconfig %s.%d hw ether %02x:%02x:%02x:%02x:%02x:%02x", portRealName[port_idx], entry.VlanId,  \
				hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]+br_created_num);

				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}
			#endif

			for(port_idx=5; port_idx<port_num; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx) || !(wlanDevUpFlag & 1<<port_idx))
					continue;
				
				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br%d %s.%d", br_created_num, portRealName[port_idx], entry.VlanId);

				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}

			RUN_CMD("ifconfig br%d up", br_created_num);
		}	
	}

	port_member_mask = lan_port_remained & 0x1f & (~wan_mask);   //LAN ports.
	port_member_mask |= CPU_PORT_MASK;	//CPU port.
	port_member_mask |= (lan_port_remained & 0x0001ffe0) << 4; 
	port_tagged_mask = CPU_PORT_MASK; 

	#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
	#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
	port_member_mask |=  nat_lan_port_mask & (~wan_mask);
	#else
	port_member_mask |=  0x1f & (~wan_mask);
	#endif
	#endif

	RUN_CMD("echo %d > /proc/net/vlan/groupIndex", DEFAULT_NAT_LAN_VID);

	RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
		port_member_mask, port_tagged_mask, DEFAULT_NAT_LAN_VID, VLAN_TYPE_NAT);
	
	if(lan_port_remained & 0x1f & (~wan_mask))
	{
		#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
		RUN_CMD("vconfig add eth0 %d", DEFAULT_NAT_LAN_VID);
				
		RUN_CMD("ifconfig eth0.%d up", DEFAULT_NAT_LAN_VID);

		RUN_CMD("brctl addif br0 eth0.%d", DEFAULT_NAT_LAN_VID);
	
		sprintf(tmpBuf, "eth0.%d:", DEFAULT_NAT_LAN_VID);
		strcat(reinitBuf, tmpBuf);	
		#else
		for(port_idx=0; port_idx<4; port_idx++)
		{
			if(!(lan_port_remained & 1<<port_idx))
				continue;

			RUN_CMD("vconfig add %s %d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);

			sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			strcat(reinitBuf, tmpBuf);
		}
		#endif
	}

	if(lan_port_remained & 0x1ffe0)
	{
		//lan_port_remained &= 0x1ffe0;
		for(port_idx=5; port_idx<port_num; port_idx++)
		{
			if(!(lan_port_remained & 1<<port_idx) || !(wlanDevUpFlag & 1<<port_idx))
				continue;

			RUN_CMD("vconfig add %s %d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);

			sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			strcat(reinitBuf, tmpBuf);
		}
	}
}

static int setVlanBridgeMode(void)
{
	int idx=0, port_idx=0;
	int port_member_mask=0, port_tagged_mask=0, lan_port_remained=0;
	char tmpBuf[20];
	
	RUN_CMD("brctl delif br0 eth0");
	RUN_CMD("brctl delif br0 eth1");
	#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	RUN_CMD("brctl delif br0 eth2");
	RUN_CMD("brctl delif br0 eth3");
	RUN_CMD("brctl delif br0 eth4");
	#endif
	
	lan_port_remained = 0x1ffff;
	cpu_port_pvid = DEFAULT_NAT_LAN_VID;

	for(idx=1; idx<=entry_num; idx++)
	{
		*(char *)&entry = (char)idx;
		APMIB_GET(MIB_VLAN_TBL, (void *)&entry);
	
		if(entry.VlanType == VLAN_TYPE_BRIDGE)
		{
			lan_port_remained &= ~entry.MemberPortMask;

			port_member_mask = entry.MemberPortMask & 0x0000001f;   //LAN ports.
			port_member_mask |= CPU_PORT_MASK;	//CPU port.
			port_member_mask |= (entry.MemberPortMask & 0x0001ffe0) << 4;   //WLAN ports.
			
			port_tagged_mask = entry.TaggedPortMask & 0x0000001f; 
			port_tagged_mask |= CPU_PORT_MASK;
			port_tagged_mask |= (entry.TaggedPortMask & 0x0001ffe0) << 4;

			printf("\nType = VLAN_TYPE_LAN_BRIDGE\n");

			RUN_CMD("echo %d > /proc/net/vlan/groupIndex", entry.VlanId);

			RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
				port_member_mask, port_tagged_mask, entry.VlanId, VLAN_TYPE_BRIDGE);

			#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
			RUN_CMD("vconfig add eth0 %d", entry.VlanId);

			RUN_CMD("ifconfig eth0.%d hw ether %02x:%02x:%02x:%02x:%02x:%02x", entry.VlanId,  \ 
				hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]);

			RUN_CMD("ifconfig eth0.%d up", entry.VlanId);

			RUN_CMD("brctl addif br0 eth0.%d", entry.VlanId);

			sprintf(tmpBuf, "eth0.%d:", entry.VlanId);
			strcat(reinitBuf, tmpBuf);
			#else
			for(port_idx=0; port_idx<5; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx))
					continue;
				
				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], entry.VlanId);

				RUN_CMD("ifconfig %s.%d hw ether %02x:%02x:%02x:%02x:%02x:%02x", portRealName[port_idx], entry.VlanId,  \ 
				hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]);
				
				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}	
			#endif
			
			for(port_idx=5; port_idx<port_num; port_idx++)
			{
				if(!(entry.MemberPortMask & 1<<port_idx) || !(wlanDevUpFlag & 1<<port_idx))
					continue;
				
				RUN_CMD("vconfig add %s %d", portRealName[port_idx], entry.VlanId);
				RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], entry.VlanId);
				RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], entry.VlanId);

				sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], entry.VlanId);
				strcat(reinitBuf, tmpBuf);
			}	
		}
	}

	port_member_mask = lan_port_remained & 0x1f;   //LAN ports.
	port_member_mask |= CPU_PORT_MASK;	//CPU port.
	port_member_mask |= (lan_port_remained & 0x0001ffe0) << 4; 
	port_tagged_mask = CPU_PORT_MASK; 

	RUN_CMD("echo %d > /proc/net/vlan/groupIndex", DEFAULT_NAT_LAN_VID);

	RUN_CMD("echo \"1,%x,%x,%d,%d\" > /proc/net/vlan/vlanGroup",  \
		port_member_mask, port_tagged_mask, DEFAULT_NAT_LAN_VID, VLAN_TYPE_BRIDGE);
	
	if(lan_port_remained & 0x1f)
	{
		#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
		RUN_CMD("vconfig add eth0 %d", DEFAULT_NAT_LAN_VID);
				
		RUN_CMD("ifconfig eth0.%d up", DEFAULT_NAT_LAN_VID);

		RUN_CMD("brctl addif br0 eth0.%d", DEFAULT_NAT_LAN_VID);
	
		sprintf(tmpBuf, "eth0.%d:", DEFAULT_NAT_LAN_VID);
		strcat(reinitBuf, tmpBuf);
		#else
		for(port_idx=0; port_idx<5; port_idx++)
		{
			if(!(lan_port_remained & 1<<port_idx))
				continue;

			RUN_CMD("vconfig add %s %d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);

			sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			strcat(reinitBuf, tmpBuf);
		}
		#endif
	}

	if(lan_port_remained & 0x1ffe0)
	{
		lan_port_remained &= 0x1ffe0;
		for(port_idx=5; port_idx<port_num; port_idx++)
		{
			if(!(lan_port_remained & 1<<port_idx) || !(wlanDevUpFlag & 1<<port_idx))
				continue;

			RUN_CMD("vconfig add %s %d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("ifconfig %s.%d up", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			RUN_CMD("brctl addif br0 %s.%d", portRealName[port_idx], DEFAULT_NAT_LAN_VID);

			sprintf(tmpBuf, "%s.%d:", portRealName[port_idx], DEFAULT_NAT_LAN_VID);
			strcat(reinitBuf, tmpBuf);
		}
	}
}

#if defined (CONFIG_RTL_MULTICAST_PORT_MAPPING)
static int setMCPortMapping(void)
{
	int idx=0, port_idx=0, wan_mask=0, port_tagged_mask=0, lan_port_remained=0;
	int nat_lan_port_mask=0, untag_nat_wan_vid=DEFAULT_NAT_WAN_VID; 
	int untag_bridge_wan_vid=0, tag_nat_wan_vid=0;
	int mCastVlanLeakyInfo[MAX_VLAN_CONFIG_NUM]={0}, leakyInfoIdx=0;
	char leakyInfoBuf[150]={0};
	char tmpBuf[20];

	wan_mask = getWanPortMask();
	lan_port_remained = 0x1ffff & (~wan_mask);
	
	for(idx=1; idx<=entry_num; idx++)
	{
		*(char *)&entry = (char)idx;
		APMIB_GET(MIB_VLAN_TBL, (void *)&entry);
	
		#if defined(CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG)
		if(entry.VlanType == VLAN_TYPE_NAT)
		{
			lan_port_remained &= ~entry.MemberPortMask;
			nat_lan_port_mask = entry.MemberPortMask & 0x0000001f;
			if(entry.TaggedPortMask & wan_mask)
			{
				untag_nat_wan_vid = 0;
				tag_nat_wan_vid = entry.VlanId;
			}
			else
				untag_nat_wan_vid = entry.VlanId;
		}
		#endif

		#if !defined(CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG)
		if(entry.VlanType == VLAN_TYPE_NAT && entry.MemberPortMask == wan_mask)
		{
			port_tagged_mask = entry.TaggedPortMask&wan_mask ? 
				wan_mask|CPU_PORT_MASK : CPU_PORT_MASK;

			if(port_tagged_mask & wan_mask)
			{
				untag_nat_wan_vid = 0;
				tag_nat_wan_vid = entry.VlanId;
			}
			else
				untag_nat_wan_vid = entry.VlanId;
		}
		
		if(entry.VlanType == VLAN_TYPE_NAT && !(entry.MemberPortMask & wan_mask))
		{
			lan_port_remained &= ~entry.MemberPortMask;
			nat_lan_port_mask = entry.MemberPortMask & 0x0000001f;
			mCastVlanLeakyInfo[leakyInfoIdx++] = entry.VlanId;
		}
		#endif
		
		if(entry.VlanType == VLAN_TYPE_BRIDGE)
		{
			lan_port_remained &= ~entry.MemberPortMask;
			if(!(entry.TaggedPortMask & wan_mask))
				untag_bridge_wan_vid = entry.VlanId;
		}
	}

	RUN_CMD("echo \"%d\" > /proc/mCastVlanLeakyInfo", 0);
	
#ifndef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
	if(lan_port_remained & 0x1f & (~wan_mask)) 
		mCastVlanLeakyInfo[leakyInfoIdx++] = DEFAULT_NAT_LAN_VID;
#endif

	if(untag_nat_wan_vid!=0 && untag_bridge_wan_vid!=0)
	{
		mCastVlanLeakyInfo[leakyInfoIdx++] = untag_nat_wan_vid;
		mCastVlanLeakyInfo[leakyInfoIdx++] = untag_bridge_wan_vid;
	}
#ifndef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
	else 
	{
		if(tag_nat_wan_vid != 0)
			mCastVlanLeakyInfo[leakyInfoIdx++] = tag_nat_wan_vid;
		else
			mCastVlanLeakyInfo[leakyInfoIdx++] = untag_nat_wan_vid;
	}
#endif

	if(mCastVlanLeakyInfo[0] != 0 && mCastVlanLeakyInfo[1]!=0)
	{
		sprintf(leakyInfoBuf, "echo \"");
		for(idx=0; idx<leakyInfoIdx; idx++)
		{
			sprintf(tmpBuf, "%d ", mCastVlanLeakyInfo[idx]);
			strcat(leakyInfoBuf, tmpBuf);
		}
		strcat(leakyInfoBuf, "\" > /proc/mCastVlanLeakyInfo");
			
		RUN_CMD(leakyInfoBuf);

		printf("%s\n", leakyInfoBuf);
	}
}
#endif

static int getWanPortMask(void)
{
	int opMode = 0;

	APMIB_GET(MIB_OP_MODE, (void *)&opMode);

	if(opMode == GATEWAY_MODE)
		return DEFAULT_ETH_WAN_PORT_MASK;
	else if(opMode == BRIDGE_MODE)
		return 0;
	else
		return DEFAULT_ETH_WAN_PORT_MASK;
}

#endif

