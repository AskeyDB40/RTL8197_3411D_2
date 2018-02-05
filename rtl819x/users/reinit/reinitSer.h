#ifndef _REINIRSERVER_H
#define _REINIRSERVER_H
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "list.h"
#include "apmib.h"

#define BR_IFACE_FILE "/var/system/br_iface"
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
#define BR_IFACE_FILE2 "/var/system/br_iface2"
#endif
#define MESH_PATHSEL "/bin/pathsel" 
#define BR_INIT_FILE "/tmp/bridge_init"
#define DHCPD_CONF_FILE "/var/udhcpd.conf"
#define PROC_BR_IGMPPROXY "/proc/br_igmpProxy"
#define DHCPD_PID_FILE "/var/run/udhcpd.pid"
#define TR069_PID_FILE "var/run/cwmp.pid"
#define PROC_FASTNAT_FILE "/proc/fast_nat"
#define WEBS_PID_FILE "/var/run/webs.pid"
#define LLTD_PROCESS_FILE "/bin/lld2d"
#define SNMPD_PROCESS_FILE "/bin/snmpd"
#define NMSD_PROCESS_FILE "/bin/nmsd"
#define PROC_BR_IGMPVERSION "/proc/br_igmpVersion"
#define PROC_BR_IGMPSNOOP "/proc/br_igmpsnoop"
#define PROC_BR_IGMPQUERY "/proc/br_igmpquery"
#define RT_CACHE_REBUILD_COUNT "/proc/sys/net/ipv4/rt_cache_rebuild_count"
#define PROC_BR_MLDSNOOP "/proc/br_mldsnoop"
#define PROC_BR_MLDQUERY "/proc/br_mldquery"
#define	PARTITION_FILE "/proc/partitions"
#ifdef CONFIG_CPU_UTILIZATION
#define CPU_LOAD_FILE "/web/cpu_data.dat"
#define CPU_PID_FILE "/var/run/cpu.pid"
#endif

#define HW_NAT_FILE "/proc/hw_nat"
#define SOFTWARE_NAT_FILE "/proc/sw_nat"

#define HW_NAT_LIMIT_NETMASK 0xFFFFFF00 //for arp table 512 limitation,


#define COMMAND_BUFF_LEN 128
/*
* 	degbug level define in syslog.h:
*	#define LOG_EMERG       	0        system is unusable 
*	#define LOG_ALERT       	1       action must be taken immediately 
*	#define LOG_CRIT        	2        critical conditions 
*	#define LOG_ERR         		3        error conditions 
*	#define LOG_WARNING     	4        warning conditions 
*	#define LOG_NOTICE      	5        normal but significant condition 
*	#define LOG_INFO        		6        informational 
*	#define LOG_DEBUG       	7        debug-level messages 
*/
#define REINIT_DEF_LOG_LEVEL LOG_ERR

#define REINIT_FUNC_PRIORITY_MAX 0xffffffff

#define REINIT_FUNC_PRIORITY_SYSTEM 100

#define REINIT_FUNC_PRIORITY_WLAN 3000
#define REINIT_FUNC_PRIORITY_LAN 3001
#define REINIT_FUNC_PRIORITY_LAN_MAC 3010
#define REINIT_FUNC_PRIORITY_LAN_STP 3015
#define REINIT_FUNC_PRIORITY_LAN_APP_DHCP 3020
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
#define REINIT_FUNC_PRIORITY_LAN_APP_DOMAIN_QUERY_DNRD 3030
#endif
#ifndef STAND_ALONE_MINIUPNP
#define REINIT_FUNC_PRIORITY_LAN_APP_MINIUPNP 3040
#endif
#ifdef CONFIG_AUTO_DHCP_CHECK
#define REINIT_FUNC_PRIORITY_LAN_APP_Auto_DHCP_CHECK 3050
#endif
#ifdef SAMBA_WEB_SUPPORT
#define REINIT_FUNC_PRIORITY_LAN_APP_SAMBA_WEB 3060
#endif
#if defined(CONFIG_APP_TR069)	
#define REINIT_FUNC_PRIORITY_LAN_APP_TR069 3070
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
#define REINIT_FUNC_PRIORITY_LAN_APP_ETH_802DOT1X 3080
#endif
#ifdef RTK_CAPWAP	
#define REINIT_FUNC_PRIORITY_LAN_APP_CAPWAP 3090
#endif
#define INIT_FUNC_PRIORITY_MISC 3100

#define REINIT_FUNC_PRIORITY_WAN 5000
#ifdef DOS_SUPPORT
#define REINIT_FUNC_PRIORITY_WAN_DOS 5001
#endif

#define REINIT_FUNC_PRIORITY_WLAN_BLOCK_RELAY 3100
#define REINIT_FUNC_PRIORITY_WLAN_SCHEDULE 3101
#define REINIT_FUNC_PRIORITY_WLAN_APP 3102

#define ERINIT_FUNC_TIMEOUT_USLEEP 100000 //us
#define REINIT_FUNC_TIMEOUT_COUNT_EFFECT 20 //2s
#define REINIT_FUNC_TIMEOUT_COUNT_REINIT 200 //20s

typedef struct mibChangeList_
{
	MibChangeNode_t changeNodeHead;
	int count;
} mibChangeList_t,*mibChangeList_tp;


typedef struct reinit_func_list_node_
{
	BASE_DATA_T base;
	struct list_head *mibChangeList;
	MibChangeFuncItem_tp funcItem;
	struct list_head list;
	//struct list_head parm_list;//chain of MibChangeNode_t
	//MibChangeNode_tp changeNode;//first changeNode
} reinit_func_list_node_t,*reinit_func_list_node_tp;
#if 0
typedef struct reinit_func_parm_list_node_
{
	
	struct list_head parm_list;
	MibChangeNode_tp mibChangeNode;
} reinit_func_parm_list_node_t,*reinit_func_parm_list_node_tp;
#endif
typedef struct reinit_func_list_
{
	reinit_func_list_node_t funcNodeHead;
	int count;
} reinit_func_list_t,*reinit_func_list_tp;

extern MibChangeFuncItem_t mibDecisionFuncSet[],mibReinitFuncSet[];
extern EventHandleFuncItem_t eventHandleFuncSet[];

#endif
