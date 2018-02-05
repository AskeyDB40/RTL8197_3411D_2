#ifndef _PRMT_LAYER2BRIDGE_H_
#define _PRMT_LAYER2BRIDGE_H_

#include "prmt_igd.h"
#include "prmt_utility.h"

#ifdef __cplusplus
extern "C" {
#endif


extern struct CWMP_LEAF tPortEntityLeaf[];
extern struct CWMP_LEAF tBridgeEntityLeaf[];
extern struct CWMP_LEAF tLayer2BridgingEntityLeaf[];

extern struct CWMP_LINKNODE tPortObject[];
extern struct CWMP_LINKNODE tBridgeObject[];
extern struct CWMP_NODE tLayer2BridgingObject[];

int getPortEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setPortEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
int getBridgeEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setBridgeEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
int getLayer2BridgingEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setLayer2BridgingEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int objBridge();
int objPort();

#ifdef __cplusplus
}
#endif

#endif /*_PRMT_LAYER2BRIDGE_H_*/

