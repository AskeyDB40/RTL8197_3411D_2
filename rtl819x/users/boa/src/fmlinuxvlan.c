/*******************************
* 802.1Q Linux VLAN UI Support
* Added by Luke Wang, 2015/10
*******************************/

#if defined(CONFIG_8021Q_VLAN_SUPPORTED)

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "boa.h"
#include "globals.h"
#include "apform.h"
#include "apmib.h"
#include "utility.h"
#include "asp_page.h"

#define VLAN_TYPE_NAT 0
#define VLAN_TYPE_BRIDGE 1

#define GATEWAY_MODE 0
#define BRIDGE_MODE 1
#define WISP_MODE 2
#define SOURCE_TAG_MODE (1 << 2)

#define DEFAULT_NAT_LAN_VID 9
#define DEFAULT_NAT_WAN_VID 8

#define DEFAULT_ETH_WAN_PORT_MASK 0x10

static const char *portDisplayName[] =
{
	"port1", "port2", "port3", "port4", "port5",
	"wlan1", "wlan1-va1", "wlan1-va2", "wlan1-va3", "wlan1-va4", "wlan1-vxd",
	"wlan2", "wlan2-va1", "wlan2-va2", "wlan2-va3", "wlan2-va4", "wlan2-vxd"
};

#define APMIB_GET(A, B, C)	 \
	{if(!apmib_get(A, B)) { strcpy(errBuf, (C)); goto setErr; }}
#define APMIB_SET(A, B, C)	  \
	{if(!apmib_set(A, B)) { strcpy(errBuf, (C)); goto setErr; }}

static int getWanPortMask(void);

int getVlanInfo(request *wp, int argc, char **argv)
{
	int intVal=0, wan_mask=0;
	char strBuf[30];
	char errBuf[50];

	if (argv[0] == NULL) {
   		fprintf(stderr, "Insufficient args\n");
   		return -1;
   	}

	if(!strcmp(argv[0], "vlanEnabled"))
	{
		APMIB_GET(MIB_VLAN_ENABLED, (void *)&intVal, "Get VLAN enable flag error!");
		sprintf(strBuf, "%d", intVal);
		return req_format_write(wp, strBuf);
	}
	else if(!strcmp(argv[0], "portNum"))
	{
		intVal = sizeof(portDisplayName)/sizeof(char *);
		sprintf(strBuf, "%d", intVal);
		return req_format_write(wp, strBuf);
	}
	else if(!strcmp(argv[0], "vlanNum"))
	{
		APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&intVal, "Get VLAN entry number error!");
		sprintf(strBuf, "%d", intVal);
		return req_format_write(wp, strBuf);
	}
	else if(!strcmp(argv[0], "isSrcTagMode"))
	{
		#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
		sprintf(strBuf, "%d", 1);
		#else
		sprintf(strBuf, "%d", 0);
		#endif
		return req_format_write(wp, strBuf);
	}
	else if(!strcmp(argv[0], "isHwNatEnabled"))
	{
		#ifdef CONFIG_RTL_HW_NAPT
		sprintf(strBuf, "%d", 1);
		#else
		sprintf(strBuf, "%d", 0);
		#endif
		return req_format_write(wp, strBuf);
	}
	else if(!strcmp(argv[0], "wanPortId"))
	{
		wan_mask = getWanPortMask();
		for(intVal=0; intVal<32; intVal++)
		{
			if(1<<intVal == wan_mask)
				break;
		}

		if(intVal < 32)
			intVal += 1;
		else
			intVal = 0;
		
		sprintf(strBuf, "%d", intVal);
		return req_format_write(wp, strBuf);
	}

	return -1;

setErr:
	ERR_MSG(errBuf);
	return -1;
}

extern int getWlStaNum(char *interface, int *num);
int getWlanValid(request *wp, int argc, char **argv)
{
	int idx, num;
	char strBuf[50], devName[10];

	for (idx=0; idx<2; idx++) {
		sprintf(devName, "wlan%d", idx);
		if (getWlStaNum(devName, &num) < 0)		
			sprintf(strBuf, "wlanValid[%d] = %d;\n", idx, 0);
		else
			sprintf(strBuf, "wlanValid[%d] = %d;\n", idx, 1);
		req_format_write(wp, strBuf);
	}

	return 0;
}

int getPortList(request *wp, int argc, char **argv)
{
	int idx=0, port_num=0;
	char strBuf[50];

	port_num = sizeof(portDisplayName)/sizeof(char *);

	for(idx=0; idx<port_num; idx++)
	{
		sprintf(strBuf, "portDisplayName[%d] = \'%s\';\n", idx+1, portDisplayName[idx]);
		req_format_write(wp, strBuf);
	}

	return 0;
}

static int hwNatVidValidCheck(void);
int getVlanTable(request *wp, int argc, char **argv)
{
	int idx=0, entry_num=0, tmp_flag=0, opmode=0, hw_nat_lan_vid=0;
	int port_idx=0, port_num=0, port_mask=0, wan_idx=0, wan_mask=0;
	VLAN_CONFIG_T entry;
	char strBuf[200], strBuf1[20], errBuf[50];
	
	hwNatVidValidCheck();

	port_num = sizeof(portDisplayName)/sizeof(char *);
	APMIB_GET(MIB_OP_MODE, (void *)&opmode, "Get operation mode error!");
	APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num, "Get VLAN entry number error!");
	APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Get hw nat lan vid error!");
	memset((void *)&entry, 0, sizeof(VLAN_CONFIG_T));

	wan_mask = getWanPortMask();
	for(wan_idx=0; wan_idx<port_num; wan_idx++)
		if(1<<wan_idx == wan_mask) break;
	
	for(idx=1; idx<=entry_num; idx++)
	{
		*(char *)&entry = (char)idx;
		APMIB_GET(MIB_VLAN_TBL, (void *)&entry, "Get VLAN table entry error!");

		if(opmode == GATEWAY_MODE)
		{
			if(entry.VlanType == VLAN_TYPE_NAT)
			{
				if(entry.VlanId==hw_nat_lan_vid)
					strcpy(strBuf1, "NAT(hw)");
				else
					strcpy(strBuf1, "NAT");
			}
			else if(entry.VlanType == VLAN_TYPE_BRIDGE)
				strcpy(strBuf1, "Bridge");
			else
				strcpy(strBuf1, "Unknown");
		}
		else if(opmode == BRIDGE_MODE)
		{
			strcpy(strBuf1, "Bridge");
		}
			
		sprintf(strBuf, "vlanTable[%d] = \'%d|%d|%s|", 
			idx, entry.VlanId, entry.VlanPriority, strBuf1);

		tmp_flag = 0;
		port_mask = entry.TaggedPortMask;
		for(port_idx=0; port_idx<port_num; port_idx++)
		{
			if(port_mask & (1<<port_idx))
			{
				if(tmp_flag)
					strcat(strBuf, ", ");
				strcat(strBuf, portDisplayName[port_idx]);
				if(port_idx == wan_idx)
					strcat(strBuf, "(WAN)");
				tmp_flag = 1;
			}
		}
		strcat(strBuf, "|");
		
		tmp_flag = 0;
		port_mask = entry.MemberPortMask & (~entry.TaggedPortMask);
		for(port_idx=0; port_idx<port_num; port_idx++)
		{
			if(port_mask & (1<<port_idx))
			{
				if(tmp_flag)
					strcat(strBuf, ", ");
				strcat(strBuf, portDisplayName[port_idx]);
				if(port_idx == wan_idx)
					strcat(strBuf, "(WAN)");
				tmp_flag = 1;
			}
		}
		strcat(strBuf, "\';\n");
		req_format_write(wp, strBuf);
	}

	return 0;
	
setErr:
	ERR_MSG(errBuf);
	return -1;	
}

static int setDefaultPVid(void);
int getPVidArray(request *wp, int argc, char **argv)
{
	int idx=0, port_num=0, pvid_val=0;
	char pVidArray[MAX_VLAN_PORT_NUM * 2];
	char strBuf[50], errBuf[50];

	port_num = sizeof(portDisplayName)/sizeof(char *);
	memset((void *)pVidArray, 0, MAX_VLAN_PORT_NUM * 2);
	APMIB_GET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray, "Get PVID array error!");
	
	for(idx=0; idx<=6; idx++)
	{
		pvid_val = *((short *)pVidArray + idx);
		if(pvid_val>0 && pvid_val<4096)
			continue;

		setDefaultPVid();
		apmib_update_web(CURRENT_SETTING);
		
		APMIB_GET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray, "Get PVID array error!");
		break;
	}
	
	for(idx=1; idx<=port_num; idx++)
	{
		sprintf(strBuf, "PVidArray[%d] = \'%d\';\n", idx, *((short *)pVidArray + idx - 1));

		req_format_write(wp, strBuf);
	}

	return 0;
	
setErr:
	ERR_MSG(errBuf);
	return -1;	
}

void formVlan(request *wp, char *path, char *query)
{
	VLAN_CONFIG_T new_entry, entry;
	char *submitUrl=NULL, *strTmp=NULL;
	int	vlan_enabled=0, vlan_enabled_last=0, idx=0, vlan_id=0, priority=0, entry_num=0, opmode=0;
	int port_idx=0, port_num=0, port_flag=0, port_member_mask=0, port_tagged_mask=0, wan_mask=0;
	int hw_nat_flag=0, hw_nat_lan_vid=0; 
	char strBuf[50], errBuf[100];
	char pVidArray[MAX_VLAN_PORT_NUM * 2];

	APMIB_GET(MIB_OP_MODE, (void *)&opmode, "Get operation mode error!");
	wan_mask = getWanPortMask();

	strTmp = req_get_cstream_var(wp, "addVlan", "");
#if defined(APPLY_CHANGE_DIRECT_SUPPORT)
	if(strTmp[0]==0){
		strTmp = req_get_cstream_var(wp, "addVlanFlag", "");
	}
#endif

	if(strTmp[0])
	{
		strTmp = req_get_cstream_var(wp, "vlanEnabledFlag", "");
		if(strTmp[0])
		{
			vlan_enabled = atoi(strTmp);
			APMIB_GET(MIB_VLAN_ENABLED, (void *)&vlan_enabled_last, "Get VLAN enable error!");
			APMIB_SET(MIB_VLAN_ENABLED, (void *)&vlan_enabled, "Set VLAN enable error!");
		}
		if(! vlan_enabled)
			goto ApmibUpdate;
	
		//Set VLAN type.
		memset(&new_entry, 0, sizeof(VLAN_CONFIG_T));
		strTmp = req_get_cstream_var(wp, "vlanType", "");
		switch(strTmp[0])
		{
			case '0':
				new_entry.VlanType = VLAN_TYPE_NAT;
				break;
			case '1':
				new_entry.VlanType = VLAN_TYPE_BRIDGE;
				break;
			default:
				strcpy(errBuf, "Error! Invalid forwarding rule!");
				goto setErr;
		}
	
		//Set VLAN id/priority.
		strTmp = req_get_cstream_var(wp, "vlanId", "");
		vlan_id = strtol(strTmp, NULL, 10);
		if(vlan_id<0 || vlan_id>4095)
		{
			strcpy(errBuf, "Invalid VLAN id!");
			goto setErr;
		}	
		if(vlan_id==100 || vlan_id==DEFAULT_NAT_LAN_VID || vlan_id==DEFAULT_NAT_WAN_VID)
		{
			sprintf(errBuf, "VLAN id %d and %d are for internal use!",
				DEFAULT_NAT_WAN_VID, DEFAULT_NAT_LAN_VID);
			goto setErr;
		}		
		new_entry.VlanId = vlan_id;
	
		strTmp = req_get_cstream_var(wp, "priority", "");
		priority = strtol(strTmp, NULL, 10);
		if(priority<0 || priority>7)
		{
			strcpy(errBuf, "Error! Invalid priority!");
			goto setErr;
		}
		new_entry.VlanPriority = priority;

		//Set port member/tagged port.
		port_num = sizeof(portDisplayName)/sizeof(char *);
		for(idx=1; idx<=port_num; idx++)
		{
			sprintf(strBuf, "port_member_%d_val", idx);
			strTmp = req_get_cstream_var(wp, strBuf, "");
			if(strTmp[0])
			{
				port_flag = atoi(strTmp);
				if(port_flag)
					port_member_mask |= 1<<(idx-1);
			}

			sprintf(strBuf, "port_tagged_%d_val", idx);
			strTmp = req_get_cstream_var(wp, strBuf, "");
			if(strTmp[0])
			{
				port_flag = atoi(strTmp);
				if(port_flag)
					port_tagged_mask |= 1<<(idx-1);
			}
		}
		port_tagged_mask &= port_member_mask;
		new_entry.MemberPortMask = port_member_mask;
		new_entry.TaggedPortMask = port_tagged_mask;

		//Nothing is configured, then just update enabled/disabled.
		if(vlan_id==0 && port_member_mask==0 && port_tagged_mask==0)
		{
			if(vlan_enabled != vlan_enabled_last)
				goto ApmibUpdate;
			else
				goto ApmibNoUpdate;
		}
		if(vlan_id == 0)
		{
			strcpy(errBuf, "Invalid VLAN id!");
			goto setErr;
		}
			
		//Maximum vlan table entry num check.
		APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num, "Get VLAN table entry number error!");
		if(entry_num >= MAX_VLAN_CONFIG_NUM)
		{
			sprintf(errBuf, "Maximum VLAN table entry number is %d!", MAX_VLAN_CONFIG_NUM); 
			goto setErr;	
		}
		
		//Port member check.
		if(! port_member_mask)
		{
			strcpy(errBuf, "No ports are selected!"); 
			goto setErr;	
		}
		if(opmode == GATEWAY_MODE)
		{
			#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
			if(new_entry.VlanType == VLAN_TYPE_NAT ||
				new_entry.VlanType == VLAN_TYPE_BRIDGE)
			{
				if(!(port_member_mask & wan_mask) ||
					!(port_member_mask & ~wan_mask))
				{
					strcpy(errBuf, "WAN and at least one LAN/WLAN port should be included!"); 
					goto setErr;	
				}
			}
			#else
			if(new_entry.VlanType == VLAN_TYPE_NAT)
			{
				if((port_member_mask & wan_mask)
					&& (port_member_mask & ~wan_mask))
				{
					strcpy(errBuf, "When forwarding rule is NAT, "
						"WAN and LAN/WLAN ports should be in seperated VLAN groups!"); 
					goto setErr;	
				}
			}
			#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
			if(new_entry.VlanType == VLAN_TYPE_BRIDGE)
			{
				if(!(port_member_mask & wan_mask) ||
					!(port_member_mask & ~wan_mask))
				{
					strcpy(errBuf, "When forwarding rule is Bridge, "
						"WAN and at least one LAN/WLAN port should be included!"); 
					goto setErr;	
				}
			}
			#endif
			#endif
			
			#if defined(CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG)
			if(new_entry.VlanType == VLAN_TYPE_BRIDGE ||
				new_entry.VlanType == VLAN_TYPE_NAT)
			{
				if((port_tagged_mask & port_member_mask) != 0 &&
					(port_tagged_mask & port_member_mask) != port_member_mask)
				{
					strcpy(errBuf, "Ports should be all tagged or all untagged!"); 
					goto setErr;	
				}	
			}
			#endif
		}
		
		//Repeated VID check/repeated NAT wan check/repeated untagged wan group check.
		memset(&entry, 0, sizeof(VLAN_CONFIG_T));
		for(idx=1; idx<=entry_num; idx++)
		{
			*(char *)&entry = (char)idx;
			APMIB_GET(MIB_VLAN_TBL, (void *)&entry, "Get VLAN table entry error!");

			if(entry.VlanId == new_entry.VlanId)
			{
				strcpy(errBuf, "Repeated VLAN id is set!"); 
				goto setErr;	
			}

			#if defined(CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG)
			if(opmode==GATEWAY_MODE && 
				entry.VlanType == VLAN_TYPE_NAT &&
				new_entry.VlanType == VLAN_TYPE_NAT)
			{
				strcpy(errBuf, "only one NAT group can exist!"); 
				goto setErr;
			}
			#else
			if(opmode==GATEWAY_MODE && 
				entry.MemberPortMask == wan_mask &&
				new_entry.MemberPortMask == wan_mask)
			{
				strcpy(errBuf, "When forwarding rule is NAT, "
					"only one VLAN group that includes WAN port can exist!"); 
				goto setErr;
			}
			#endif

			//Repeated untagged bridge group check.
			//NAT WAN can be tagged/untagged freely.
			if(opmode == GATEWAY_MODE)
			{
				if(entry.VlanType==VLAN_TYPE_BRIDGE && new_entry.VlanType==VLAN_TYPE_BRIDGE &&
					 (entry.MemberPortMask&wan_mask) && !(entry.TaggedPortMask&wan_mask) &&
					 (new_entry.MemberPortMask&wan_mask) && !(new_entry.TaggedPortMask&wan_mask))
				{
					strcpy(errBuf, "Only support 1 untagged bridge-wan at most!"); 
					goto setErr;
				}
			}
		}

		//Duplicated LAN/WLAN port check.(A LAN/WLAN port can only be included in one group.)
		if(opmode == GATEWAY_MODE &&
			(new_entry.MemberPortMask & ~wan_mask))
		{
			for(port_idx=0; port_idx<port_num; port_idx++)
			{
				if(!(new_entry.MemberPortMask & 1<<port_idx) 
					|| wan_mask==(1<<port_idx))
					continue;

				for(idx=1; idx<=entry_num; idx++)
				{
					*(char *)&entry = (char)idx;
					APMIB_GET(MIB_VLAN_TBL, (void *)&entry, "Get VLAN table entry error!");

					if(!(entry.MemberPortMask & 1<<port_idx))
						continue;

					strcpy(errBuf, "A LAN/WLAN port can't be included in more than one group!"); 
					goto setErr;
				}
			}
		}

		//Hardware NAT LAN vid check.
		if(opmode == GATEWAY_MODE)
		{
			APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Get hw nat lan vid error!");
			strTmp = req_get_cstream_var(wp, "hwNatFlag", "");
			hw_nat_flag = strtol(strTmp, NULL, 10);
			if(hw_nat_flag)
			{
				if(new_entry.VlanType==VLAN_TYPE_BRIDGE)
				{
					strcpy(errBuf, "Hardware NAT is only for NAT-LAN group!");
					goto setErr;
				}

				if(hw_nat_lan_vid>0 && hw_nat_lan_vid!=DEFAULT_NAT_LAN_VID)
				{
					strcpy(errBuf, "Only 1 NAT-LAN group can be hardware processed!");
					goto setErr;
				}

				hw_nat_lan_vid = new_entry.VlanId;	
				APMIB_SET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Set hw nat lan vid error!");
			}
		}
		
		APMIB_SET(MIB_VLAN_ADD, (void *)&new_entry, "Add VLAN table entry error!");

		goto SetPVid;
	}

	strTmp = req_get_cstream_var(wp, "deleteSelVlan", "");
	if(strTmp[0])
	{
		int deleted_num=0;

		vlan_enabled = 1;
		APMIB_SET(MIB_VLAN_ENABLED, (void *)&vlan_enabled, "Set VLAN enable error!");
		
		APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num, "Get VLAN table entry number error!");

		memset(&entry, 0, sizeof(VLAN_CONFIG_T));
		for(idx=1; idx<=entry_num; idx++)
		{
			sprintf(strBuf, "vlan_select_%d", idx);
			strTmp = req_get_cstream_var(wp, strBuf, "");
			if(! strTmp[0])
				continue;

			*(char *)&entry = (char)(idx-deleted_num);
			deleted_num++;
			APMIB_GET(MIB_VLAN_TBL, (void *)&entry, "Get VLAN table entry error!");
			APMIB_SET(MIB_VLAN_DEL, (void *)&entry, "Del VLAN table entry error!");

			if(opmode == GATEWAY_MODE)
			{
				APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Get hw nat lan vid error!");
				if(entry.VlanId == hw_nat_lan_vid)
				{
					hw_nat_lan_vid = DEFAULT_NAT_LAN_VID;
					APMIB_SET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Set hw nat lan vid error!");
				}
			}
		}
		
		goto SetPVid;
	}

	strTmp = req_get_cstream_var(wp, "deleteAllVlan", "");
	if(strTmp[0])
	{
		vlan_enabled = 1;
		APMIB_SET(MIB_VLAN_ENABLED, (void *)&vlan_enabled, "Set VLAN enable error!");

		APMIB_SET(MIB_VLAN_DELALL, (void *)&entry, "Delete all VLAN table entry error!");

		if(opmode == GATEWAY_MODE)
		{
			hw_nat_lan_vid = DEFAULT_NAT_LAN_VID;
			APMIB_SET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid, "Set hw nat lan vid error!");
		}
		
		goto SetPVid;
	}

	strTmp = req_get_cstream_var(wp, "changePVid", "");
	if(strTmp[0])
	{
		vlan_enabled = 1;
		APMIB_SET(MIB_VLAN_ENABLED, (void *)&vlan_enabled, "Set VLAN enable error!");

		port_num = sizeof(portDisplayName)/sizeof(char *);
		memset((void *)pVidArray, 0, MAX_VLAN_PORT_NUM * 2);
		APMIB_GET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray, "Get VLAN PVID array error!");
		
		for(idx=1; idx<=port_num; idx++)
		{
			sprintf(strBuf, "vlan_pvid_%d", idx);
			strTmp = req_get_cstream_var(wp, strBuf, "");
			if(strTmp[0])
			{
				vlan_id = atoi(strTmp);
				if(vlan_id<0 || vlan_id>4095)
				{
					strcpy(errBuf, "Error! Invalid PVID value!");
					goto setErr;
				}
				*((short *)pVidArray + idx - 1) = vlan_id;
			}
		}
		
		APMIB_SET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray, "Set VLAN PVID array error!");
		
		goto ApmibUpdate;
	}

SetPVid:
	setDefaultPVid();

ApmibUpdate:
	#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
	opmode |= SOURCE_TAG_MODE;
	#endif
	APMIB_SET(MIB_VLAN_OPMODE, (void *)&opmode, "Set VLAN OPMODE error!");
	apmib_update_web(CURRENT_SETTING);

ApmibNoUpdate:
#ifndef NO_ACTION
	run_init_script("all");
#endif

	submitUrl = req_get_cstream_var(wp, "submit-url", "");   // hidden page
	if (submitUrl[0])
	{
		OK_MSG(submitUrl);
	}
  	return;

setErr:
	ERR_MSG(errBuf);
	return;
}

#undef APMIB_GET
#undef APMIB_SET

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

static int setDefaultPVid(void)
{
	int port_idx=0, port_num=0, vlan_idx=0, entry_num=0, opmode=0;
	int default_vid_find=0, wan_mask=0, wan_idx=0, hw_nat_lan_vid=0;
	char pVidArray[MAX_VLAN_PORT_NUM * 2];
	VLAN_CONFIG_T entry;

	APMIB_GET(MIB_OP_MODE, (void *)&opmode);
	port_num = sizeof(portDisplayName)/sizeof(char *);
	APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num);
	memset((void *)pVidArray, 0, MAX_VLAN_PORT_NUM * 2);
	memset((void *)&entry, 0, sizeof(VLAN_CONFIG_T));

	APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);

	wan_mask = getWanPortMask();
	for(wan_idx=0; wan_idx<port_num; wan_idx++)
		if(1<<wan_idx == wan_mask) break;

	//LAN/WLAN port.
	for(port_idx=0; port_idx<port_num; port_idx++)
	{
		if(1<<port_idx == wan_mask)
			continue;

		default_vid_find = 0;

		if(opmode == GATEWAY_MODE)
		{
			//As long as the vlan entry has this lan port, no matter tagged/untagged.
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

				if(!(entry.MemberPortMask & (1<<port_idx)))
					continue;

				default_vid_find = entry.VlanId;
			}
		}
		else
		{
			//Search vlan group which contains this port untagged.
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

				if(!(entry.MemberPortMask & (1<<port_idx)))
					continue;

				if(!(entry.TaggedPortMask & (1<<port_idx)))
				{
					default_vid_find = entry.VlanId;
						break;
				}
			}

			//Then search vlan group which contains this port tagged.
			if(default_vid_find == 0)
			{
				for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
				{
					*(char *)&entry = (char)vlan_idx;
					APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

					if(!(entry.MemberPortMask & (1<<port_idx)))
						continue;

					default_vid_find = entry.VlanId;
						break;
				}
			}
		}

		if(default_vid_find)
			*((short *)pVidArray + port_idx) = default_vid_find;
		else
		{
			if(port_idx>=0 && port_idx<=4)
				*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
			else
				*((short *)pVidArray + port_idx) = DEFAULT_NAT_LAN_VID;
		}
	}

	//WAN port.
	if(wan_mask != 0)
	{	
		default_vid_find = 0;

		for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
		{
			*(char *)&entry = (char)vlan_idx;
			APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

			//Untagged bridge-wan.
			if(entry.VlanType == VLAN_TYPE_BRIDGE &&
				(entry.MemberPortMask & wan_mask) &&
				!(entry.TaggedPortMask & wan_mask))
			{
				default_vid_find = entry.VlanId;
				break;
			}
		}

		if(default_vid_find == 0)
		{
			for(vlan_idx=1; vlan_idx<=entry_num; vlan_idx++)
			{
				*(char *)&entry = (char)vlan_idx;
				APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

				//Untagged nat group.
				if(entry.VlanType == VLAN_TYPE_NAT &&
					(entry.MemberPortMask & wan_mask) &&
					!(entry.TaggedPortMask & wan_mask))
				{
					default_vid_find = entry.VlanId;
					break;
				}
			}
		}

		if(default_vid_find)
			*((short *)pVidArray + wan_idx) = default_vid_find;
		else
			*((short *)pVidArray + wan_idx) = DEFAULT_NAT_WAN_VID;
	}
		
	APMIB_SET(MIB_VLAN_PVID_ARRAY, (void *)pVidArray);
	
	return 0;
} 

//HW NAT lan vid valid check.
static int hwNatVidValidCheck(void)
{	
	int idx=0, opmode=0, entry_num=0, hw_nat_lan_vid=0, wan_mask=0;
	VLAN_CONFIG_T entry;
	
	APMIB_GET(MIB_OP_MODE, (void *)&opmode);
	if(opmode == GATEWAY_MODE)
	{
		APMIB_GET(MIB_VLAN_TBL_NUM, (void *)&entry_num);
		APMIB_GET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);
		wan_mask = getWanPortMask();
		
		for(idx=1; idx<=entry_num; idx++)
		{
			*(char *)&entry = (char)idx;
			APMIB_GET(MIB_VLAN_TBL, (void *)&entry);

			#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
			if(entry.VlanId==hw_nat_lan_vid && entry.VlanType==VLAN_TYPE_NAT)
				break;
			#else
			if(entry.VlanId==hw_nat_lan_vid && entry.VlanType==VLAN_TYPE_NAT &&
				!(entry.MemberPortMask & wan_mask))
				break;
			#endif
		}

		if(idx > entry_num)
		{
			hw_nat_lan_vid = DEFAULT_NAT_LAN_VID;
			APMIB_SET(MIB_VLAN_HW_NAT_LAN_VID, (void *)&hw_nat_lan_vid);
		}
	}	
}

#endif

