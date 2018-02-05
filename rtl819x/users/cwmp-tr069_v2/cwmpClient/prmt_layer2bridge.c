/******* prmt_layer2bridge.h ********/
/******* by luke_wang 2017-01-11 ********/
#include "prmt_layer2bridge.h"

/*****************************     Port     *****************************/
struct CWMP_OP tPortEntityLeafOP = { getPortEntity, setPortEntity };
struct CWMP_PRMT tPortEntityLeafInfo[] =
{
/*(name,					type,			flag,						op)*/
{"PortEnable",				eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
{"Alias",					eCWMP_tSTRING, 	CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
{"PortInterface",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
{"PortState",				eCWMP_tSTRING,	CWMP_READ,				&tPortEntityLeafOP},
{"PVID",					eCWMP_tUINT, 	CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
{"AcceptableFrameTypes",	eCWMP_tSTRING, 	CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
{"IngressFiltering",		eCWMP_tBOOLEAN, CWMP_WRITE|CWMP_READ,	&tPortEntityLeafOP},
};
enum ePortEntityLeaf
{
	ePortEnable,
	ePortAlias,
	ePortInterface,
	ePortState,
	ePVID,
	eAcceptableFrameTypes,
	eIngressFiltering
};
struct CWMP_LEAF tPortEntityLeaf[] =
{
{ &tPortEntityLeafInfo[ePortEnable] },
{ &tPortEntityLeafInfo[ePortAlias] },
{ &tPortEntityLeafInfo[ePortInterface] },
{ &tPortEntityLeafInfo[ePortState] },
{ &tPortEntityLeafInfo[ePVID] },
{ &tPortEntityLeafInfo[eAcceptableFrameTypes] },
{ &tPortEntityLeafInfo[eIngressFiltering] },
{ NULL }
};

struct CWMP_PRMT tPortObjectInfo[] =
{
/*(name,				type,		flag,			op)*/
{"1",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum ePortObject
{
	ePort1
};
struct CWMP_LINKNODE tPortObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tPortObjectInfo[ePort1],	tPortEntityLeaf,	NULL,		NULL,			0}
};

/*****************************     Bridge     *****************************/
struct CWMP_OP tBridgeEntityLeafOP = { getBridgeEntity, setBridgeEntity };
struct CWMP_PRMT tBridgeEntityLeafInfo[] =
{
/*(name,					type,			flag,						op)*/
{"Alias",				eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tBridgeEntityLeafOP},
{"BridgeKey",			eCWMP_tUINT, 	CWMP_READ,				&tBridgeEntityLeafOP},
{"BridgeStandard",		eCWMP_tSTRING, 	CWMP_WRITE|CWMP_READ,	&tBridgeEntityLeafOP},
{"BridgeEnable",		eCWMP_tBOOLEAN, CWMP_WRITE|CWMP_READ,	&tBridgeEntityLeafOP},
{"BridgeStatus",		eCWMP_tSTRING, 	CWMP_READ,				&tBridgeEntityLeafOP},
{"BridgeName",			eCWMP_tSTRING, 	CWMP_WRITE|CWMP_READ,	&tBridgeEntityLeafOP},
{"VLANID",				eCWMP_tUINT, 	CWMP_WRITE|CWMP_READ,	&tBridgeEntityLeafOP},
{"PortNumberOfEntries",	eCWMP_tUINT, 	CWMP_READ,				&tBridgeEntityLeafOP},
{"VLANNumberOfEntries",	eCWMP_tUINT, 	CWMP_READ,				&tBridgeEntityLeafOP},
};
enum eBridgeEntityLeaf
{
	eAlias,
	eBridgeKey,
	eBridgeStandard,
	eBridgeEnable,
	eBridgeStatus,
	eBridgeName,
	eVLANID,
	ePortNumberOfEntries,
	eVLANNumberOfEntries
};
struct CWMP_LEAF tBridgeEntityLeaf[] =
{
{ &tBridgeEntityLeafInfo[eAlias] },
{ &tBridgeEntityLeafInfo[eBridgeKey] },
{ &tBridgeEntityLeafInfo[eBridgeStandard] },
{ &tBridgeEntityLeafInfo[eBridgeEnable] },
{ &tBridgeEntityLeafInfo[eBridgeStatus] },
{ &tBridgeEntityLeafInfo[eBridgeName] },
{ &tBridgeEntityLeafInfo[eVLANID] },
{ &tBridgeEntityLeafInfo[ePortNumberOfEntries] },
{ &tBridgeEntityLeafInfo[eVLANNumberOfEntries] },
{ NULL }
};

struct CWMP_OP tPort_OP = { NULL, objPort};
struct CWMP_PRMT tBridgeEntityObjectInfo[] =
{
/*(name,			type,		flag,			)*/
{"Port",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tPort_OP},
};
enum eBridgeEntityObject
{
	ePort
};
struct CWMP_NODE tBridgeEntityObject[] =
{
/*info,  					leaf,			node)*/
{&tBridgeEntityObjectInfo[ePort],	NULL,			NULL},
{NULL,						NULL,			NULL}
};

struct CWMP_PRMT tBridgeObjectInfo[] =
{
/*(name,				type,			flag,									op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eBridgeObject
{
	ebridge1
};
struct CWMP_LINKNODE tBridgeObject[] =
{
/*info,  				leaf,			next,				sibling,		instnum)*/
{&tBridgeObjectInfo[ebridge1],	tBridgeEntityLeaf,	tBridgeEntityObject,		NULL,			0}
};

/*****************************     Layer2Bridging     *****************************/
struct CWMP_OP tLayer2BridgingEntityLeafOP = { getLayer2BridgingEntity,	NULL};
struct CWMP_PRMT tLayer2BridgingEntityLeafInfo[] =
{
/*(name,								  type,				flag,					op)*/
{"MaxBridgeEntries",				 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"MaxDBridgeEntries",				 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"MaxQBridgeEntries",				 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"MaxVLANEntries",					 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"MaxFilterEntries",				 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"BridgeNumberOfEntries",			 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"FilterNumberOfEntries",			 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"MarkingNumberOfEntries",			 eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
{"AvailableInterfaceNumberOfEntries",eCWMP_tUINT,	CWMP_READ,	&tLayer2BridgingEntityLeafOP},
};
enum eLayer2BridgingEntityLeaf
{
	eMaxBridgeEntries,
	eMaxDBridgeEntries,
	eMaxQBridgeEntries,
	eMaxVLANEntries,
	eMaxFilterEntries,
	eBridgeNumberOfEntries,
	eFilterNumberOfEntries,
	eMarkingNumberOfEntries,
	AvailableInterfaceNumberOfEntries
};
struct CWMP_LEAF tLayer2BridgingEntityLeaf[] =
{
{ &tLayer2BridgingEntityLeafInfo[eMaxBridgeEntries] },
{ &tLayer2BridgingEntityLeafInfo[eMaxDBridgeEntries] },
{ &tLayer2BridgingEntityLeafInfo[eMaxQBridgeEntries] },
{ &tLayer2BridgingEntityLeafInfo[eMaxVLANEntries] },
{ &tLayer2BridgingEntityLeafInfo[eMaxFilterEntries] },
{ &tLayer2BridgingEntityLeafInfo[eBridgeNumberOfEntries] },
{ &tLayer2BridgingEntityLeafInfo[eFilterNumberOfEntries] },
{ &tLayer2BridgingEntityLeafInfo[eMarkingNumberOfEntries] },
{ &tLayer2BridgingEntityLeafInfo[AvailableInterfaceNumberOfEntries] },
{ NULL }
};
struct CWMP_OP tBridge_OP = { NULL, objBridge};
struct CWMP_PRMT tLayer2BridgingEntityObjectInfo[] =
{
/*(name,				type,				flag,					op)*/
{"Bridge",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tBridge_OP},
};
enum eLayer2BridgingEntityObject
{
	eBridge
};

struct CWMP_NODE tLayer2BridgingObject[] =
{
/*info,  					leaf,			node)*/
{&tLayer2BridgingEntityObjectInfo[eBridge],	NULL, NULL},
{NULL,						NULL,			NULL}
};

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

#define UNTAGGED_VID 0xFFFF
#define WAN_IDX 14
#define VALID_PORT(x) ((x)>=0 && (x)<=WAN_IDX)
static const char *portIfaceDispName[] = {
	"lan1", "lan2", "lan3", "lan4", 
	"wlan1", "wlan1-va1", "wlan1-va2", "wlan1-va3", "wlan1-va4",
	"wlan2", "wlan2-va1", "wlan2-va2", "wlan2-va3", "wlan2-va4",
	"wan" 
	};

static int get_bridge_by_inst_num(CWMP_BRIDGE_T *bridgeEntry, int inst_num)
{
	int br_i=0, br_num=0;

	if((!bridgeEntry) || inst_num<=0)
		return -1;
	
	APMIB_GET(MIB_CWMP_BRIDGE_TBL_NUM, (void *)&br_num);

	for(br_i=1; br_i<=br_num; br_i++)
	{
		memset((void *)bridgeEntry, 0, sizeof(CWMP_BRIDGE_T));
		*((char *)bridgeEntry) = br_i;
		APMIB_GET(MIB_CWMP_BRIDGE_TBL, (void *)bridgeEntry);

		if(bridgeEntry->bridgeInstNum == inst_num)
			return 0;
	}

	//Bridge.{i}. not found in the MIB.
	return -1;
}

static int get_port_by_inst_num(CWMP_BRIDGE_T *bridgeEntry, int inst_num)
{
	int port_i=0;

	if((!bridgeEntry) || inst_num<=0)
		return -1;

	for(port_i=0; port_i<TOTAL_INTERFACE_PORT_NUM; port_i++)
	{
		if(bridgeEntry->portInstNum[port_i] == inst_num)
			return port_i;	
	}

	return -1;
}

static int add_port_to_vlan(CWMP_BRIDGE_T *bridgeEntry, int port_idx)
{
	int port_i=0;

	if(!bridgeEntry || !VALID_PORT(port_idx) ||
		bridgeEntry->bridgePortNum >= TOTAL_INTERFACE_PORT_NUM)
		return -1;

	for(port_i=0; port_i<TOTAL_INTERFACE_PORT_NUM; port_i++)
	{
		//already exist
		if(bridgeEntry->portInstNum[port_i] != 0 && 
			bridgeEntry->portIdx[port_i] == port_idx)
			return -1;
	}

	for(port_i=0; port_i<TOTAL_INTERFACE_PORT_NUM; port_i++)
	{
		if(bridgeEntry->portInstNum[port_i] == 0)
		{
			//Assign new max instance num for newly added port.
			bridgeEntry->bridgePortNum++;
			bridgeEntry->portMaxInstNum++;
			if(bridgeEntry->portMaxInstNum == 0)
				bridgeEntry->portMaxInstNum++;
			
			bridgeEntry->portInstNum[port_i] = bridgeEntry->portMaxInstNum;
			bridgeEntry->portIdx[port_i] = port_idx;

			return 0;
		}	
	}

	return -1;
}

static int delete_port_by_inst_num(CWMP_BRIDGE_T *bridgeEntry, int inst_num)
{
	int port_i=0, ret_deleted_port_idx=-1;

	if((!bridgeEntry) || inst_num<=0)
		return -1;
	
	for(port_i=0; port_i<TOTAL_INTERFACE_PORT_NUM; port_i++)
	{
		if(bridgeEntry->portInstNum[port_i] == inst_num)
		{
			if(VALID_PORT(bridgeEntry->portIdx[port_i]))
				ret_deleted_port_idx = bridgeEntry->portIdx[port_i];
			else
				ret_deleted_port_idx = -1;

			bridgeEntry->portIdx[port_i] = 0;
			bridgeEntry->portInstNum[port_i] = 0;

			if(bridgeEntry->bridgePortNum > 0)
				bridgeEntry->bridgePortNum--;	

			return ret_deleted_port_idx;
		}
	}

	return -1;
}

/*****************************     Port     *****************************/
int objPort(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	struct CWMP_LINKNODE **c;
	CWMP_BRIDGE_T bridgeEntry[2] = {0};
	int ret=0, vlan_i=0, port_i=0, br_inst_num=0, cwmp_vlan_num=0, update_and_reinit = 0; 
	unsigned int port_max_inst_num=0;
	
	br_inst_num = getInstNum( name, "Bridge" );
	ret = get_bridge_by_inst_num(&bridgeEntry[0], br_inst_num);
	if(ret < 0)
		return ERR_9007;
	
	switch(type)
	{
		case eCWMP_tINITOBJ:
			c = (struct CWMP_LINKNODE **)data;
			if(name==NULL || entity==NULL || data==NULL) 
				return -1;

			for(port_i = 0; port_i < TOTAL_INTERFACE_PORT_NUM; port_i++)
			{
				if(bridgeEntry[0].portInstNum[port_i] == 0)
					continue;
				
				if(create_Object(c, tPortObject, sizeof(tPortObject), 1, bridgeEntry[0].portInstNum[port_i]) < 0)
					return -1;
			}
			break;
			
		case eCWMP_tADDOBJ:
			port_max_inst_num = bridgeEntry[0].portMaxInstNum;
			port_max_inst_num++;
			if(port_max_inst_num == 0)
				port_max_inst_num;
			
			*((unsigned int *)data) = port_max_inst_num;
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tPortObject, sizeof(tPortObject), data);
			if(ret >= 0){
				memcpy((char *)&bridgeEntry[1], (char *)&bridgeEntry[0], sizeof(CWMP_BRIDGE_T));
				
				ret = add_port_to_vlan(&bridgeEntry[1], -1);
				if(ret < 0)
					return ERR_9004;
				
				APMIB_SET(MIB_CWMP_BRIDGE_MOD, (void *)&bridgeEntry[0]);			
			}
			break;
			
		case eCWMP_tDELOBJ:
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);
			if(ret >= 0)
			{
				memcpy((char *)&bridgeEntry[1], (char *)&bridgeEntry[0], sizeof(CWMP_BRIDGE_T)); 

				ret = delete_port_by_inst_num(&bridgeEntry[1], *(int*)data);
					
				APMIB_SET(MIB_CWMP_BRIDGE_MOD, (void *)&bridgeEntry[0]);

				//Only when delete a valid port.
				if(ret >= 0)
					update_and_reinit = 1;
			}
			break;
			
		case eCWMP_tUPDATEOBJ:
		{
			struct CWMP_LINKNODE *old_table;
			struct CWMP_LINKNODE *remove_entity = NULL;
			unsigned int port_inst_num=0;
			
			old_table = (struct CWMP_LINKNODE*)entity->next;
			entity->next = NULL;
			
			for(port_i=0; port_i<TOTAL_INTERFACE_PORT_NUM; port_i++)
			{
				if(bridgeEntry[0].portInstNum[port_i] == 0)
					continue;
				
				remove_entity = remove_SiblingEntity(&old_table, bridgeEntry[0].portInstNum[port_i]);
				if(remove_entity != NULL)
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				else
				{
					port_inst_num = (int)bridgeEntry[0].portInstNum[port_i];
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tPortObject, sizeof(tPortObject), &port_inst_num);
				}
			}		

			if( old_table )
				destroy_ParameterTable((struct CWMP_NODE *)old_table);
			break;
		}
	}

	if(update_and_reinit)
	{
		//to-do: need to sync to multi-wan table here
		//apply_cwmp_vlan_config();

		return 1;
	}
	else
		return 0;	
}

int getPortEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	CWMP_BRIDGE_T bridgeEntry = {0};
	int instb, instp, int_value, port_i=0, idx, para_num;
	char tmp_data[32];

	const char *para[] = {
		"PortEnable", "Alias", "PortInterface", "PortState", 
		"PVID", "AcceptableFrameTypes", "IngressFiltering"
	};
	
	*type = entity->info->type;
	*data = NULL;
		
	instb = getInstNum( name, "Bridge" );//.Bridge.instb.xx
	instp = getInstNum( name, "Port" );//.Bridge.instp.xx

	if(instb == 0 || instp == 0)
		return ERR_9007;

	if(get_bridge_by_inst_num(&bridgeEntry, instb) < 0)
		return ERR_9007;

	port_i = get_port_by_inst_num(&bridgeEntry, instp);
	if(port_i < 0)
		return ERR_9007;

	para_num = sizeof(para)/sizeof(char *);
	for(idx = 0; idx < para_num; idx++)
		if(strcmp(lastname, para[idx]) == 0)
			break;
	if(idx >= para_num)
		return ERR_9005;
	
	switch(idx)
	{
		case 0:
			*data = intdup(1); 
			break;

		case 1:
			*data = strdup(""); 
			break;

		case 2:
			if(VALID_PORT(bridgeEntry.portIdx[port_i]))
				*data = strdup(portIfaceDispName[bridgeEntry.portIdx[port_i]]);
			else
				*data = strdup("");
			break;

		case 3:
			*data = strdup("Forwarding"); 
			break;

		case 4:
			*data = intdup(bridgeEntry.bridgeVid == UNTAGGED_VID ? 0 : bridgeEntry.bridgeVid); 
			break;

		case 5:
			if(bridgeEntry.portIdx[port_i] == WAN_IDX 
				&& bridgeEntry.bridgeVid != UNTAGGED_VID)
				*data = strdup("AdmitOnlyVLANTagged");
			else	
				*data = strdup("AdmitAll");	
			break;
	
		default:
			*data = booldup(0); 
			break;
	}

	return 0;
}

int setPortEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	CWMP_BRIDGE_T bridgeEntry = {0};
	char *lastname = entity->info->name;
	int instb,instp, int_value, ret, port_i, update_and_reinit = 0;
	int idx, para_num;

	const char *para[] = {
		"PortEnable", "Alias", "PortInterface", "PortState", 
		"PVID", "AcceptableFrameTypes", "IngressFiltering"
	};
	
	instb = getInstNum( name, "Bridge" );
	instp = getInstNum( name, "Port" );

	if(instb==0 || instp==0)
		return ERR_9007;

	if(get_bridge_by_inst_num(&bridgeEntry, instb) < 0)
		return ERR_9007;

	port_i = get_port_by_inst_num(&bridgeEntry, instp);
	if(port_i < 0)
		return ERR_9007;

	para_num = sizeof(para)/sizeof(char *);
	for(idx = 0; idx < para_num; idx++)
		if(strcmp(lastname, para[idx]) == 0)
			break;
	if(idx >= para_num)
		return ERR_9005;

	switch(idx)
	{
		case 2:
			/*char *buf=data;
			*((char *)cwmp_vlan_entry) = (char)instb;
			apmib_get(MIB_CWMPVLANCONFIG_TBL, (void *)&cwmp_vlan_entry[0]);
			cwmp_vlan_entry[1] = cwmp_vlan_entry[0];
			strcpy(&cwmp_vlan_entry[1].portInterface[(instp-1)*MAX_CWMP_PORT_INTERFACE_LEN],buf);
			apmib_set(MIB_CWMPVLANCONFIG_MOD, (void *)&cwmp_vlan_entry);			
			mib_update_flag = 1;*/ 
			break;

		case 5:
			/*char *buf=data;
			if(!strcmp(buf,"AdmitAll"))
				int_value = 1;
			else if(!strcmp(buf,"AdmitOnlyVLANTagged"))
				int_value = 2;
			else if(!strcmp(buf,"AdmitOnlyPrioUntagged"))
				int_value = 3;
			else
				return ERR_9007;
		
			*((char *)cwmp_vlan_entry) = (char)instb;
			apmib_get(MIB_CWMPVLANCONFIG_TBL, (void *)&cwmp_vlan_entry[0]);
			cwmp_vlan_entry[1] = cwmp_vlan_entry[0];	
			cwmp_vlan_entry[1].portTagged[instp-1] = int_value;
			apmib_set(MIB_CWMPVLANCONFIG_MOD, (void *)&cwmp_vlan_entry);			
			mib_update_flag = 1;*/
			break;
	
		default:
			break;
	}
	
	
	if(update_and_reinit)
	{
		//to-do: need to sync to multi-wan table here
		//apply_cwmp_vlan_config();

		return 1;
	}
	else
		return 0;
}

/*****************************     Bridge     *****************************/
int objBridge(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	struct CWMP_LINKNODE **c;
	CWMP_BRIDGE_T bridgeEntry = {0};
	int br_i=0, br_new_i=0, br_num=0, ret=0, update_and_reinit=0;
	unsigned int max_br_inst_num=0;
	
	switch(type)
	{
		case eCWMP_tINITOBJ:
			c = (struct CWMP_LINKNODE **)data;
			if( (name==NULL) || (entity==NULL) || (data==NULL) ) 
				return -1;

			APMIB_GET(MIB_CWMP_BRIDGE_TBL_NUM, (void *)&br_num);
			
			for(br_i=1; br_i<=br_num; br_i++)
			{
				memset((void *)&bridgeEntry, 0, sizeof(CWMP_BRIDGE_T));
				*((char *)&bridgeEntry) = br_i;
				
				APMIB_GET(MIB_CWMP_BRIDGE_TBL, (void *)&bridgeEntry);
				
				if(create_Object(c, tBridgeObject, sizeof(tBridgeObject), 1, bridgeEntry.bridgeInstNum)<0)
					return -1;		
			}	

			while((*c) != NULL)
			{
				printf("Bridge object created! instnum=%d\n", (*c)->instnum);
				c = &(*c)->sibling;
			}
			break;
			
		case eCWMP_tADDOBJ:		
			APMIB_GET(MIB_CWMP_BRIDGE_TBL_NUM, (void *)&br_num);
			//Bridge should not be over the max entry limit.
			if(br_num >= LAYER2BRIDGING_MAX_BRIDGE_NUM)
				return ERR_9004;

			//get currently used max instance num
			APMIB_GET(MIB_CWMP_BRIDGE_MAX_INST_NUM, (void *)&max_br_inst_num);
			
			//Add a new object.
			max_br_inst_num++;
			if(max_br_inst_num == 0)
				max_br_inst_num++;
			
			*((unsigned int *)data) = max_br_inst_num;
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tBridgeObject, sizeof(tBridgeObject), data);
			if(ret >= 0)
			{		
				//Add a new empty bridge entry
				memset((void *)&bridgeEntry, 0, sizeof(CWMP_BRIDGE_T));
				bridgeEntry.bridgeInstNum = max_br_inst_num;
				bridgeEntry.bridgeEnabled = 1;
				bridgeEntry.bridgeValid = 1;
				APMIB_SET(MIB_CWMP_BRIDGE_ADD, (void *)&bridgeEntry);
				APMIB_SET(MIB_CWMP_BRIDGE_MAX_INST_NUM, (void *)&max_br_inst_num);
				
				//not reinit
			}
			break;
			
		case eCWMP_tDELOBJ:
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);
			if(ret >= 0 &&
				get_bridge_by_inst_num(&bridgeEntry, *(int*)data) == 0)
			{
				APMIB_SET(MIB_CWMP_BRIDGE_DEL, (void *)&bridgeEntry);

				if(bridgeEntry.bridgeEnabled &&
					bridgeEntry.bridgeVid != 0 && 
					bridgeEntry.bridgePortNum >= 1)
					update_and_reinit = 1;
			}
			break;

		case eCWMP_tUPDATEOBJ:
		{
	     	struct CWMP_LINKNODE *old_table;
			struct CWMP_LINKNODE *remove_entity = NULL;
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;
	
			APMIB_GET(MIB_CWMP_BRIDGE_TBL_NUM, (void *)&br_num);
			
			for(br_i=1; br_i<=br_num; br_i++)
			{
				memset((void *)&bridgeEntry, 0, sizeof(CWMP_BRIDGE_T));
				*((char *)&bridgeEntry) = br_i;
				APMIB_GET(MIB_CWMP_BRIDGE_TBL, (void *)&bridgeEntry);
				
				remove_entity = remove_SiblingEntity(&old_table, bridgeEntry.bridgeInstNum);
				if(remove_entity != NULL)
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				else
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tBridgeObject, sizeof(tBridgeObject), &bridgeEntry.bridgeInstNum);
			}
			
			if( old_table )
	     		destroy_ParameterTable( (struct CWMP_NODE *)old_table );

			break;
		}
	}

	if(update_and_reinit)
	{
		//to-do: need to sync to multi-wan table here
		//apply_cwmp_vlan_config();
		return 1;
	}
	else
		return 0;
}

int getBridgeEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	CWMP_BRIDGE_T bridgeEntry;
	char *lastname=entity->info->name;
	int br_inst_num=0, idx, para_num;

	const char *para[] = {
		"Alias", "BridgeKey", "BridgeStandard", "BridgeEnable", 
		"BridgeStatus", "BridgeName", "VLANID", "PortNumberOfEntries",
		"VLANNumberOfEntries"
	};

	*type = entity->info->type;
	*data = NULL;
	
	br_inst_num = getInstNum(name, "Bridge");//Bridge.{i}.xx

	if(get_bridge_by_inst_num(&bridgeEntry, br_inst_num) < 0)
		return ERR_9007;

	para_num = sizeof(para)/sizeof(char *);
	for(idx = 0; idx < para_num; idx++)
		if(strcmp(lastname, para[idx]) == 0)
			break;
	if(idx >= para_num)
		return ERR_9005;

	switch(idx)
	{	
		case 0:
			*data = strdup(""); 
			break;

		case 1:
			*data = intdup(bridgeEntry.bridgeKey); 
			break;

		case 2:
			*data = strdup("802.1Q"); 
			break;

		case 3:
			*data = intdup(bridgeEntry.bridgeEnabled ? 1 : 0); 
			break;

		case 4:
			*data = strdup(bridgeEntry.bridgeEnabled ? "Enabled" : "Disabled"); 
			break;

		case 5:	
			*data = strdup(bridgeEntry.bridgeName); 
			break;

		case 6:	
			*data = intdup(bridgeEntry.bridgeVid == UNTAGGED_VID ? 0 : bridgeEntry.bridgeVid); 
			break;

		case 7:	
			*data = intdup(bridgeEntry.bridgePortNum); 
			break;

		default:
			*data = intdup(0); 
			break;
	}

	return 0;
}

int setBridgeEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	CWMP_BRIDGE_T bridgeEntry[2];
	char *lastname = entity->info->name;
	int inst_num=0, update_and_reinit=0, str_len=0, idx, para_num;

	const char *para[] = {
		"Alias", "BridgeKey", "BridgeStandard", "BridgeEnable", 
		"BridgeStatus", "BridgeName", "VLANID", "PortNumberOfEntries",
		"VLANNumberOfEntries"
	};

	inst_num = getInstNum(name, "Bridge");//.Bridge.{i}.xx

	if(get_bridge_by_inst_num(&bridgeEntry[0], inst_num) < 0)
		return ERR_9007;

	para_num = sizeof(para)/sizeof(char *);
	for(idx = 0; idx < para_num; idx++)
		if(strcmp(lastname, para[idx]) == 0)
			break;
	if(idx >= para_num)
		return ERR_9005;

	memcpy((void *)&bridgeEntry[1], (void *)&bridgeEntry[0], sizeof(CWMP_BRIDGE_T));

	switch(idx)
	{	
		case 3:
			bridgeEntry[1].bridgeEnabled = *(int*)data;
			APMIB_SET(MIB_CWMP_BRIDGE_MOD, (void *)bridgeEntry);

			if(bridgeEntry[1].bridgeVid!=0 && bridgeEntry[1].bridgePortNum>=1
				&& bridgeEntry[1].bridgeEnabled!=bridgeEntry[0].bridgeEnabled)
				update_and_reinit = 1;
			break;

		case 5:	
			str_len = strlen((char *)data);
			if(str_len > LAYER2BRIDGING_NAMSIZE)
				return ERR_9007;
		
			memcpy((void *)bridgeEntry[1].bridgeName, (void *)data, str_len+1);
			APMIB_SET(MIB_CWMP_BRIDGE_MOD, (void *)bridgeEntry);
			break;

		case 6:	
			bridgeEntry[1].bridgeVid = *(int*)data;
			APMIB_SET(MIB_CWMP_BRIDGE_MOD, (void *)bridgeEntry);

			if(bridgeEntry[1].bridgeEnabled && bridgeEntry[1].bridgePortNum>=1
				&& bridgeEntry[1].bridgeVid!=bridgeEntry[0].bridgeVid)
				update_and_reinit = 1;
			break;

		default:
			break;
	}
	
	if(update_and_reinit)
	{
		//to-do: need to sync to multi-wan table here
		//apply_cwmp_vlan_config();
		return 1;
	}
	else
		return 0;
}

/*****************************     Layer2Bridging     *****************************/

int getLayer2BridgingEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	CWMP_BRIDGE_T bridgeEntry;
	int int_value=0, idx, para_num;

	const char *para[] = {
		"MaxBridgeEntries", "MaxDBridgeEntries", "MaxQBridgeEntries", 
		"MaxVLANEntries", "MaxFilterEntries", "BridgeNumberOfEntries", 
		"FilterNumberOfEntries", "MarkingNumberOfEntries", "AvailableInterfaceNumberOfEntries"  
	};
	
	*type = entity->info->type;
	*data = NULL;

	para_num = sizeof(para)/sizeof(char *);
	CWMPDBG_FUNC( MODULE_DATA_MODEL, LEVEL_INFO, ("[%s:%d]Para num = %d\n", __FUNCTION__, __LINE__, para_num));
	for(idx = 0; idx < para_num; idx++)
		if(strcmp(lastname, para[idx]) == 0)
			break;
	if(idx >= para_num)
		return ERR_9005;

	switch(idx)
	{	
		case 0:
			*data = intdup(LAYER2BRIDGING_MAX_BRIDGE_NUM); 
			break;

		case 2:
			*data = intdup(LAYER2BRIDGING_MAX_BRIDGE_NUM); 
			break;

		case 5:	
			APMIB_GET(MIB_CWMP_BRIDGE_TBL_NUM, (void *)&int_value);
			*data = intdup(int_value);
			break;

		default:
			*data = intdup(0); 
			break;
	}

	return 0;
}



