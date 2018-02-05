#ifndef INCLUDE_SYSUTILITY_H
#define INCLUDE_SYSUTILITY_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "reinitSer.h"
#include "apmib.h"

#define TRUE 1
#define FALSE 0

#define NULL_FILE 0
#define NULL_STR ""
#define IFACE_FLAG_T 0x01
#define IP_ADDR_T 0x02
#define NET_MASK_T 0x04
#define HW_ADDR_T 0x08

#ifndef _PATH_PROCNET_ROUTE
#define _PATH_PROCNET_ROUTE	"/proc/net/route"
#endif
#ifndef RTF_UP
#define RTF_UP			0x0001          /* route usable                 */
#endif
#ifndef RTF_GATEWAY
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */
#endif

#define NTPTMP_FILE "/tmp/ntp_tmp"
#define READ_BUF_SIZE 64
#define IFACE_NUM_MAX 32
#define IFACE_NAME_MAX 32

typedef enum { IP_ADDR, DST_IP_ADDR, SUBNET_MASK, DEFAULT_GATEWAY, HW_ADDR } ADDR_T;
typedef enum { LAN_NETWORK=0, WAN_NETWORK } DHCPC_NETWORK_TYPE_T;

void get_br_interface_name(char* br_interface_name);
int RunSystemCmd(char *filepath, ...);
int is_wan_connected();
int isFileExist(char *file_name);
int devideStringIntoItems(char * buff,int count_max,char divider, char *item[]);

int getLanPortIfName(char* name,int index);
int write_line_to_file2(char *filename,char *line_data);	//direct write data to file

int write_line_to_file(char *filename, int mode, char *line_data);	//wirte data to file by user control

pid_t find_pid_by_name( char* pidName);
int getPid_fromFile(char *file_name);
int killDaemonByPidFile(char *pidFile);



int daemon_is_running(const char * pid_file);
int getWanLink(char *interface);
int getInAddr( char *interface, int type, void *pAddr );
int IsExistProcess(unsigned char * proName);

int getWanIfaceEntry(int index,WANIFACE_T* pEntry);
int format_wan_data(BASE_DATA_Tp pdata,BASE_DATA_Tp orig_data);
void getWanIface(BASE_DATA_Tp pdata ,unsigned char *wan_iface);
int checkLanPortInvalid(int index);
int getIfNameByIdx(int index,char *name);
int getIfType(char *name);
int getBrList(char* brName,char ifNames[][IFACE_NAME_MAX]);
#endif


