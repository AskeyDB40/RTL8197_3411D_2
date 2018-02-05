#ifndef _PRMT_LANDEVICE_WLAN_H_
#define _PRMT_LANDEVICE_WLAN_H_

#ifndef __ECOS
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "prmt_igd.h"

#include "prmt_utility.h" //keith add.

#ifdef WLAN_SUPPORT
#ifdef __cplusplus
extern "C" {
#endif

#define UUID_LEN					16

extern struct CWMP_LEAF tPreSharedKeyEntityLeaf[];
extern struct CWMP_NODE tPreSharedKeyObject[];
extern struct CWMP_LEAF tWEPKeyEntityLeaf[];
extern struct CWMP_NODE tWEPKeyObject[];
extern struct CWMP_LEAF tAscDeviceEntityLeaf[];
extern struct CWMP_LINKNODE tAscDeviceObject[];
extern struct CWMP_LEAF tWLANConfEntityLeaf[];
extern struct CWMP_NODE tWLANConfEntityObject[];
extern struct CWMP_NODE tWLANConfigObject[];

extern struct CWMP_LEAF tWPSEntityLeaf[];
extern struct CWMP_NODE tWPSObject[];
extern struct CWMP_LEAF tWPSRegistrarEntityLeaf[];
extern struct CWMP_LINKNODE tWPSRegistarObject[];
extern struct CWMP_PRMT tStatsEntityLeafInfo[];

int getPreSharedKeyEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setPreSharedKeyEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getWEPKeyEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setWEPKeyEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getWPSEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setWPSEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getStatsEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);

int getWPSRegistrarEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setWPSRegistrarEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getAscDeviceEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int objAscDevice(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objWPSRegistrar(char *name, struct CWMP_LEAF *e, int type, void *data);


int getWLANConf(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setWLANConf(char *name, struct CWMP_LEAF *entity, int type, void *data);

#ifdef WLAN_SUPPORT//#if 1//def CTCOM_WLAN_REQ
int objWLANConfiguration(char *name, struct CWMP_LEAF *e, int type, void *data);
#endif

#define WSC_ROMFS_CFG_CILE		 "/etc/wscd.conf"
#define MAX_EXTERNAL_REGISTRAR_NUM	3

enum {	CONFIG_ERR_NO_ERR=0, CONFIG_ERR_OOB_INTERFACE_READ_ERR=1,
		CONFIG_ERR_DECRYPTION_CRC_ERR=2, CONFIG_ERR_2_4_CH_NOT_SUPPORTED=3,
		CONFIG_ERR_5_0_CH_NOT_SUPPORTED=4, CONFIG_ERR_SIGNAL_TOO_WEAK=5,
		CONFIG_ERR_NET_AUTH_FAIL=6, CONFIG_ERR_NET_ASSOC_FAIL=7,
		CONFIG_ERR_NO_DHCP_RESPONSE=8, CONFIG_ERR_FAIL_DHCP_CONFIG=9,
		CONFIG_ERR_IP_ADDR_CONFLICT=10, CONFIG_ERR_CANNOT_CONNECT_TO_REG=11,
		CONFIG_ERR_MUL_PBC_DETECTED=12, CONFIG_ERR_ROGUE_ACT_SUSPECTED=13,
		CONFIG_ERR_DEV_BUSY=14, CONFIG_ERR_SETUP_LOCKED=15,
		CONFIG_ERR_MESSAGE_TIMEOUT=16, CONFIG_ERR_REG_SESSION_TIMEOUT=17,
		CONFIG_ERR_DEV_PASS_AUTH_FAIL=18};

#ifdef __cplusplus
}
#endif
#endif /*#ifdef WLAN_SUPPORT*/
#endif /*_PRMT_LANDEVICE_WLAN_H_*/
