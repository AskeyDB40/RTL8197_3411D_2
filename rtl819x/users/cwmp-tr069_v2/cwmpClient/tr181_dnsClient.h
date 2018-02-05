#ifndef _TR181_DNS_CLIENT_H_
#define _TR181_DNS_CLIENT_H_

#include "tr181_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TR181_V6_SUPPORT
extern struct CWMP_NODE tDNSClientObject[];

int getDNSClientEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDNSClientEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int objDNSClientServer(char *name, struct CWMP_LEAF *e, int type, void *data);

int getDNSClientServerEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDNSClientServerEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
#endif // TR181_V6_SUPPORT

#ifdef __cplusplus
}
#endif

#endif /*_TR181_DNS_CLIENT_H_*/


