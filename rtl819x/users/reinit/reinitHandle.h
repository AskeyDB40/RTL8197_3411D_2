#ifndef _REINIT_HANDLE_H
#define _REINIT_HANDLE_H
#include "reinitSer.h"

extern mibChangeList_t mibChangelist;
extern reinit_func_list_t reinitFuncList;
extern int debugLevel;

int reinitSer_printf(int level,char* format,...);
MibChangeFuncItem_tp get_mibReinitFuncSet_item(MIB_CHANGE_FUNC_ID id);
int reinit_add_reinit_func(MIB_CHANGE_FUNC_ID funcId, MibChangeNode_tp pMibChangeNode);
int reinit_event_mibChange(char * data,int dataLen,char *errmsg);
int reinit_event_applyChanges(char * data,int dataLen,char *errmsg);

int reinit_get_desionFuc_orig_mib_value(int id, void* value, BASE_DATA_Tp data);

/*
*  in reinit function test whether mib changed
*  
*/
int reinitFunc_mib_changed(int id,BASE_DATA_Tp data);

/*
* in reinit function get mib changed orig value
*/
int reinitFunc_get_mib_orig_value(int id, void* value, BASE_DATA_Tp data);
#endif