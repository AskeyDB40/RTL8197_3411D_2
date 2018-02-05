#ifndef _PRMT_QUEUEMNT_H_
#define _PRMT_QUEUEMNT_H_

#include "prmt_igd.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef IP_QOS

extern struct CWMP_LEAF tQueueMntLeaf[];
extern struct CWMP_NODE tQueueMntObject[];
int getQueueMnt(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setQueueMnt(char *name, struct CWMP_LEAF *entity, int type, void *data);

extern struct CWMP_LEAF tClassEntityLeaf[];
int getClassEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setClassEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

extern struct CWMP_LINKNODE tClassObject[];
int objClass(char *name, struct CWMP_LEAF *entity, int type, void *data);

#if 0
extern struct sCWMP_ENTITY tPolicerEntity[];
int getPolicerEntity(char *name, struct sCWMP_ENTITY *entity, int *type, void **data);
int setPolicerEntity(char *name, struct sCWMP_ENTITY *entity, int type, void *data);

extern struct sCWMP_ENTITY tPolicer[];
int objPolicer(char *name, struct sCWMP_ENTITY *entity, int type, void *data);
#endif

extern struct CWMP_LEAF tQueueEntityLeaf[];
int getQueueEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setQueueEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

extern struct CWMP_LINKNODE tQueueObject[];
int objQueue(char *name, struct CWMP_LEAF *entity, int type, void *data);


#endif /*IP_QOS*/

#ifdef _STD_QOS_

/******************************************************************************/
extern struct CWMP_LEAF tQueueMntLeaf[];
extern struct CWMP_NODE tQueueMntObject[];

int getQueueMnt(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setQueueMnt(char *name, struct CWMP_LEAF *entity, int type, void *data);
/******************************************************************************/

/******************************************************************************/
extern struct CWMP_LINKNODE tClassObject[];
extern struct CWMP_LINKNODE tAppObject[];
extern struct CWMP_LINKNODE tFlowObject[];
extern struct CWMP_LINKNODE tPolicerObject[];
extern struct CWMP_LINKNODE tQueueObject[];
extern struct CWMP_LINKNODE tQueueStatsObject[];

int objClass(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objApp(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objFlow(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objPolicer(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objQueue(char *name, struct CWMP_LEAF *entity, int type, void *data);
int objQueueStats(char *name, struct CWMP_LEAF *entity, int type, void *data);
/******************************************************************************/

/******************************************************************************/
extern struct CWMP_LEAF tClassEntityLeaf[];
extern struct CWMP_LEAF tAppEntityLeaf[];
extern struct CWMP_LEAF tFlowEntityLeaf[];
extern struct CWMP_LEAF tPolicerEntityLeaf[];
extern struct CWMP_LEAF tQueueEntityLeaf[];
extern struct CWMP_LEAF tQueueStatsEntityLeaf[];

int getClassEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setClassEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getAppEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setAppEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getFlowEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setFlowEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getPolicerEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setPolicerEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getQueueEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setQueueEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);

int getQueueStatsEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data);
int setQueueStatsEntity(char *name, struct CWMP_LEAF *entity, int type, void *data);
/******************************************************************************/

#endif /*_STD_QOS_*/

#ifdef __cplusplus
}
#endif
#endif /*_PRMT_QUEUEMNT_H_*/
