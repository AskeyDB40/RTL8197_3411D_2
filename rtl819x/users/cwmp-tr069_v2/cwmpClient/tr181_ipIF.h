#ifndef _TR181_IPIF_H_
#define _TR181_IPIF_H_

#include "tr181_device.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct CWMP_NODE tIpIFObject[];


int getIpIFEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setIpIFEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getIpIFStats(char *name, struct CWMP_LEAF *entity, int *type, void **data);

int getIpIFIpv4AddrEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setIpIFIpv4AddrEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

#ifdef TR181_V6_SUPPORT
int objIpIFIpv6Addr(char *name, struct CWMP_LEAF *e, int type, void *data);
int objIpIFIpv6Prefix(char *name, struct CWMP_LEAF *e, int type, void *data);

int getIpIFIpv6AddrEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setIpIFIpv6AddrEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getIpIFIpv6PreEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setIpIFIpv6PreEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
#endif // TR181_V6_SUPPORT

#ifdef __cplusplus
}
#endif

#endif /*_TR181_IPIF_H_*/
