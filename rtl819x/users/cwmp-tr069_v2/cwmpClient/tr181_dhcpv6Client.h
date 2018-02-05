#ifndef _TR181_DHCPV6_CLIENT_H_
#define _TR181_DHCPV6_CLIENT_H_

#include "tr181_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TR181_V6_SUPPORT
extern struct CWMP_NODE tDhcpv6ClientObject[];

int objDhcpv6Client(char *name, struct CWMP_LEAF *e, int type, void *data);

int getDhcpv6ClientEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDhcpv6ClientEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);


int objDhcpv6ClientSent(char *name, struct CWMP_LEAF *e, int type, void *data);
int objDhcpv6ClientReq(char *name, struct CWMP_LEAF *e, int type, void *data);

int getDhcpv6ClientSentEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDhcpv6ClientSentEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getDhcpv6ClientRecvEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDhcpv6ClientRecvEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getDhcpv6ClientServerEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setDhcpv6ClientServerEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
#endif // TR181_V6_SUPPORT

#ifdef __cplusplus
}
#endif

#endif /*_TR181_DHCPV6_CLIENT_H_*/

