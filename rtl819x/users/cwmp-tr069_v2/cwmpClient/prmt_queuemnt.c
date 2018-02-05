#include "prmt_queuemnt.h"
#include "prmt_utility.h"
#ifdef IP_QOS

extern unsigned int getInstNum( char *name, char *objname );
unsigned int getQueueInstNum( char *name );
unsigned int getClassInstNum( char *name );
unsigned int findClassInstNum(void);
int getClassEntryByInstNum(unsigned int instnum, MIB_CE_IP_QOS_T *p, unsigned int *id);


struct CWMP_OP tQueueEntityLeafOP = { getQueueEntity, setQueueEntity };
struct CWMP_PRMT tQueueEntityLeafInfo[] =
{
/*(name,		type,		flag,			op)*/
{"QueueKey",		eCWMP_tUINT,	CWMP_READ,		&tQueueEntityLeafOP},
{"QueueEnable",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
{"QueueStatus",		eCWMP_tSTRING,	CWMP_READ,		&tQueueEntityLeafOP},
{"QueueInterface",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
{"QueueBufferLength",	eCWMP_tUINT,	CWMP_READ,		&tQueueEntityLeafOP},
//{"QueueWeight",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
{"QueuePrecedence",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
//{"REDThreshold",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
//{"REDPercentage",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
{"DropAlgorithm",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
//{"SchedulerAlgorithm",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
//{"ShapingRate",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP},
//{"ShapingBurstSize",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueEntityLeafOP}
};
enum eQueueEntityLeaf
{
	eQE_QueueKey,
	eQE_QueueEnable,
	eQE_QueueStatus,
	eQE_QueueInterface,
	eQE_QueueBufferLength,
	eQE_QueuePrecedence,
	eQE_DropAlgorithm
};
struct CWMP_LEAF tQueueEntityLeaf[] =
{
{ &tQueueEntityLeafInfo[eQE_QueueKey] },
{ &tQueueEntityLeafInfo[eQE_QueueEnable] },
{ &tQueueEntityLeafInfo[eQE_QueueStatus] },
{ &tQueueEntityLeafInfo[eQE_QueueInterface] },
{ &tQueueEntityLeafInfo[eQE_QueueBufferLength] },
{ &tQueueEntityLeafInfo[eQE_QueuePrecedence] },
{ &tQueueEntityLeafInfo[eQE_DropAlgorithm] },
{ NULL }
};

struct CWMP_PRMT tQueueObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eQueueObject
{
	eQueueObject0
};
struct CWMP_LINKNODE tQueueObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tQueueObjectInfo[eQueueObject0],	tQueueEntityLeaf,	NULL,		NULL,			0},
};


#if 0
struct sCWMP_ENTITY tPolicerEntity[] =
{
/*(name,		type,		flag,			accesslist,	getvalue,	setvalue,	next_table,	sibling)*/
{"PolicerKey",		eCWMP_tUINT,	CWMP_READ,		NULL,		getPolicerEntity,NULL,		NULL,		NULL},
{"PolicerEnable",	eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
{"PolicerStatus",	eCWMP_tSTRING,	CWMP_READ,		NULL,		getPolicerEntity,NULL,		NULL,		NULL},
{"CommittedRate",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
{"CommittedBurstSize",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
/*ExcessBurstSize*/
/*PeakRate*/
/*PeakBurstSize*/
{"MeterType",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
{"PossibleMeterTypes",	eCWMP_tSTRING,	CWMP_READ,		NULL,		getPolicerEntity,NULL,		NULL,		NULL},
{"ConformingAction",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
/*PartialConformingAction*/
{"NonConformingAction",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	NULL,		getPolicerEntity,setPolicerEntity,NULL,		NULL},
{"CountedPackets",	eCWMP_tUINT,	CWMP_READ,		NULL,		getPolicerEntity,NULL,		NULL,		NULL},
{"CountedBytes",	eCWMP_tUINT,	CWMP_READ,		NULL,		getPolicerEntity,NULL,		NULL,		NULL},
{"",			eCWMP_tNONE,	0,			NULL,		NULL,		NULL,		NULL,		NULL}
};

struct sCWMP_ENTITY tPolicer[] =
{
/*(name,			type,		flag,					accesslist,	getvalue,	setvalue,	next_table,	sibling)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL,		NULL,		NULL,		tPolicerEntity,	NULL}
//{"1",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL,		NULL,		NULL,		tPolicerEntity,	NULL}
};
#endif


struct CWMP_OP tClassEntityLeafOP = { getClassEntity, setClassEntity };
struct CWMP_PRMT tClassEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"ClassificationKey",		eCWMP_tUINT,	CWMP_READ,		&tClassEntityLeafOP},
{"ClassificationEnable",	eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"ClassificationStatus",	eCWMP_tSTRING,	CWMP_READ,		&tClassEntityLeafOP},
{"ClassificationOrder",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"ClassInterface",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"DestIP",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"DestMask",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DestIPExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"SourceIP",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"SourceMask",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"SourceIPExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"Protocol",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"ProtocolExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"DestPort",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DestPortRangeMax",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DestPortExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"SourcePort",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"SourcePortRangeMax",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"SourcePortExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"SourceMACAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
/*SourceMACMask*/
//{"SourceMACExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DestMACAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
/*DestMACMask*/
//{"DestMACExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
/*Ethertype, EthertypeExclude*/
/*SSAP, SSAPExclude*/
/*DSAP, DSAPExclude*/
/*LLCControl, LLCControlExclude*/
/*SNAPOUI, SNAPOUIExclude*/
/*SourceVendorClassID, SourceVendorClassIDExclude*/
/*DestVendorClassID, DestVendorClassIDExclude*/
/*SourceClientID, SourceClientIDExclude*/
/*DestClientID, DestClientIDExclude*/
/*SourceUserClassID, SourceUserClassIDExclude*/
/*DestUserClassID, DestUserClassIDExclude*/
/*TCPACK, TCPACKExclude*/
/*IPLengthMin, IPLengthMax, IPLengthExclude*/
//{"DSCPCheck",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DSCPExclude",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"DSCPMark",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"EthernetPriorityCheck",	eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"EthernetPriorityExclude",	eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"EthernetPriorityMark",	eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"VLANIDCheck",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"VLANIDExclude",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"ForwardingPolicy",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
//{"ClassPolicer",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP},
{"ClassQueue",			eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tClassEntityLeafOP}
/*ClassApp*/
};
enum eClassEntityLeaf
{
	eQC_ClassificationKey,
	eQC_ClassificationEnable,
	eQC_ClassificationStatus,
	eQC_ClassificationOrder,
	eQC_ClassInterface,
	eQC_DestIP,
	eQC_DestMask,
	eQC_SourceIP,
	eQC_SourceMask,
	eQC_Protocol,
	eQC_ProtocolExclude,
	eQC_DestPort,
	eQC_SourcePort,
	eQC_EthernetPriorityCheck,
	eQC_EthernetPriorityMark,
	eQC_ClassQueue
};
struct CWMP_LEAF tClassEntityLeaf[] =
{
{ &tClassEntityLeafInfo[eQC_ClassificationKey] },
{ &tClassEntityLeafInfo[eQC_ClassificationEnable] },
{ &tClassEntityLeafInfo[eQC_ClassificationStatus] },
{ &tClassEntityLeafInfo[eQC_ClassificationOrder] },
{ &tClassEntityLeafInfo[eQC_ClassInterface] },
{ &tClassEntityLeafInfo[eQC_DestIP] },
{ &tClassEntityLeafInfo[eQC_DestMask] },
{ &tClassEntityLeafInfo[eQC_SourceIP] },
{ &tClassEntityLeafInfo[eQC_SourceMask] },
{ &tClassEntityLeafInfo[eQC_Protocol] },
{ &tClassEntityLeafInfo[eQC_ProtocolExclude] },
{ &tClassEntityLeafInfo[eQC_DestPort] },
{ &tClassEntityLeafInfo[eQC_SourcePort] },
{ &tClassEntityLeafInfo[eQC_EthernetPriorityCheck] },
{ &tClassEntityLeafInfo[eQC_EthernetPriorityMark] },
{ &tClassEntityLeafInfo[eQC_ClassQueue] },
{ NULL }
};

struct CWMP_PRMT tClassObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eClassObject
{
	eClassObject0
};
struct CWMP_LINKNODE tClassObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tClassObjectInfo[eClassObject0],	tClassEntityLeaf,	NULL,		NULL,			0},
};



struct CWMP_OP tQueueMntLeafOP = { getQueueMnt,	setQueueMnt };
struct CWMP_PRMT tQueueMntLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"MaxQueues",			eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"MaxClassificationEntries",	eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"ClassificationNumberOfEntries",eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"MaxAppEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"AppNumberOfEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"MaxFlowEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"FlowNumberOfEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"MaxPolicerEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"PolicerNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"MaxQueueEntries",		eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
{"QueueNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tQueueMntLeafOP},
//{"DefaultForwardingPolicy",	eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
//{"DefaultPolicer",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
//{"DefaultQueue",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
//{"DefaultDSCPMark",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"DefaultEthernetPriorityMark",	eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"AvailableAppList",		eCWMP_tSTRING,	CWMP_READ,		&tQueueMntLeafOP}
};
enum eQueueMntLeaf
{
	eQ_Enable,
	eQ_MaxQueues,
	eQ_MaxClassificationEntries,
	eQ_ClassificationNumberOfEntries,
	eQ_MaxAppEntries,
	eQ_AppNumberOfEntries,
	eQ_MaxFlowEntries,
	eQ_FlowNumberOfEntries,
	eQ_MaxPolicerEntries,
	eQ_PolicerNumberOfEntries,
	eQ_MaxQueueEntries,
	eQ_QueueNumberOfEntries,
	eQ_DefaultEthernetPriorityMark,
	eQ_AvailableAppList
};
struct CWMP_LEAF tQueueMntLeaf[] =
{
{ &tQueueMntLeafInfo[eQ_Enable] },
{ &tQueueMntLeafInfo[eQ_MaxQueues] },
{ &tQueueMntLeafInfo[eQ_MaxClassificationEntries] },
{ &tQueueMntLeafInfo[eQ_ClassificationNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxAppEntries] },
{ &tQueueMntLeafInfo[eQ_AppNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxFlowEntries] },
{ &tQueueMntLeafInfo[eQ_FlowNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxPolicerEntries] },
{ &tQueueMntLeafInfo[eQ_PolicerNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxQueueEntries] },
{ &tQueueMntLeafInfo[eQ_QueueNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_DefaultEthernetPriorityMark] },
{ &tQueueMntLeafInfo[eQ_AvailableAppList] },
{ NULL }
};


struct CWMP_OP tQM_Class_OP = { NULL, objClass };
//struct CWMP_OP tQM_Policer_OP = { NULL, objPolicer };
struct CWMP_OP tQM_Queue_OP = { NULL, objQueue };
struct CWMP_PRMT tQueueMntObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Classification",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Class_OP},
//{"Policer",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Policer_OP},
{"Queue",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Queue_OP}
};
enum tQueueMntObject
{
	eQ_Classification,
	//eQ_Policer,
	eQ_Queue
};
struct CWMP_NODE tQueueMntObject[] =
{
/*info,  					leaf,			node)*/
{&tQueueMntObjectInfo[eQ_Classification],	NULL,			NULL},
{&tQueueMntObjectInfo[eQ_Queue],		NULL,			NULL},
{NULL,						NULL,			NULL}
};


int getQueueEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char 	buff[256]={0};
	unsigned char vChar=0;
	unsigned int  qinst=0;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	qinst = getQueueInstNum( name );
	if(qinst==0) return ERR_9005;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "QueueKey" )==0 )
	{
		*data=uintdup( qinst );
	}else if( strcmp( lastname, "QueueEnable" )==0 )
	{
		 *data=booldup( 1 );
	}else if( strcmp( lastname, "QueueStatus" )==0 )
	{
		 *data=strdup( "Enabled" );
	}else if( strcmp( lastname, "QueueInterface" )==0 )
	{
		 *data=strdup( "" );
	}else if( strcmp( lastname, "QueueBufferLength" )==0 )
	{
		 *data=uintdup( 8*2048 );
//	}else if( strcmp( lastname, "QueueWeight" )==0 )
//	{
//		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "QueuePrecedence" )==0 )
	{
		 *data=uintdup( qinst );
//	}else if( strcmp( lastname, "REDThreshold" )==0 )
//	{
//		 *data=uintdup( 0 );
//	}else if( strcmp( lastname, "REDPercentage" )==0 )
//	{
//		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "DropAlgorithm" )==0 )
	{
		 *data=strdup( "DT" );
//	}else if( strcmp( lastname, "SchedulerAlgorithm" )==0 )
//	{
//		 *data=strdup( "SP" );
//	}else if( strcmp( lastname, "ShapingRate" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "ShapingBurstSize" )==0 )
//	{
//		 *data=uintdup( 0 );
	}else{
		return ERR_9005;
	}
	
	return 0;
}


int setQueueEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int  qinst=0;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;



	qinst = getQueueInstNum( name );
	if(qinst==0) return ERR_9005;

	if( strcmp( lastname, "QueueEnable" )==0 )
	{

		int *i = data;

		if(i==NULL) return ERR_9007;
		if(*i==0) return ERR_9001;
		return 0;
	}else if( strcmp( lastname, "QueueInterface" )==0 )
	{
		if(buf==NULL) return ERR_9007;
		if( strlen(buf)!=0 ) return ERR_9001;
		return 0;
//	}else if( strcmp( lastname, "QueueWeight" )==0 )
//	{
	}else if( strcmp( lastname, "QueuePrecedence" )==0 )
	{

		unsigned int *i = data;

		if(i==NULL) return ERR_9007;
		if(*i!=qinst) return ERR_9001;
		return 0;
//	}else if( strcmp( lastname, "REDThreshold" )==0 )
//	{
//	}else if( strcmp( lastname, "REDPercentage" )==0 )
//	{
	}else if( strcmp( lastname, "DropAlgorithm" )==0 )
	{
		if(buf==NULL) return ERR_9007;
		if( strcmp( buf, "DT" )!=0 ) return ERR_9001;
		return 0;
//	}else if( strcmp( lastname, "SchedulerAlgorithm" )==0 )
//	{
//		if(buf==NULL) return ERR_9007;
//		if( strcmp( buf, "SP" )!=0 ) return ERR_9001;
//		return 0;
//	}else if( strcmp( lastname, "ShapingRate" )==0 )
//	{
//	}else if( strcmp( lastname, "ShapingBurstSize" )==0 )
//	{
	}else{
		return ERR_9005;
	}
	
	return 0;
}


int objQueue(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
	     	int num=0,MaxInstNum=0,i;
	     	struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
	     	
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
	     	
	     	num=IPQOS_NUM_PRIOQ;
		for( i=0; i<num;i++ )
		{
			MaxInstNum = i+1;
			if( create_Object( c, tQueueObject, sizeof(tQueueObject), 1, MaxInstNum ) < 0 )
				return -1;
		}
		add_objectNum( name, MaxInstNum );
		return 0;
	     }
	case eCWMP_tADDOBJ:
	     {
		return ERR_9001;
	     }
	case eCWMP_tDELOBJ:
	     {
		return ERR_9001;
	     }
	case eCWMP_tUPDATEOBJ:	
	     {
	     	return 0;
	     }
	}
	return -1;
}

#if 0
int getPolicerEntity(char *name, struct sCWMP_ENTITY *entity, int *type, void **data)
{
	char	*lastname = entity->name;
	unsigned char vChar=0;
	struct in_addr ipAddr;
	char buff[256]={0};
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->type;
	*data = NULL;
	if( strcmp( lastname, "PolicerKey" )==0 )
	{
		*data=uintdup( 0 );
	}else if( strcmp( lastname, "PolicerEnable" )==0 )
	{
		 *data=booldup( 0 );
	}else if( strcmp( lastname, "PolicerStatus" )==0 )
	{
		 *data=strdup( "Disabled" );
	}else if( strcmp( lastname, "CommittedRate" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "CommittedBurstSize" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "MeterType" )==0 )
	{
		 *data=strdup( "SimpleTokenBucket" );
	}else if( strcmp( lastname, "PossibleMeterTypes" )==0 )
	{
		 *data=strdup( "SimpleTokenBucket,SingleRateThreeColor,TwoRateThreeColor" );
	}else if( strcmp( lastname, "ConformingAction" )==0 )
	{
		 *data=strdup( "Null" );
	}else if( strcmp( lastname, "NonConformingAction" )==0 )
	{
		 *data=strdup( "Drop" );
	}else if( strcmp( lastname, "CountedPackets" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "CountedBytes" )==0 )
	{
		 *data=uintdup( 0 );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setPolicerEntity(char *name, struct sCWMP_ENTITY *entity, int type, void *data)
{
	char	*lastname = entity->name;
	char	*buf=data;
	unsigned char vChar=0;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->type!=type ) return ERR_9006;

	if( strcmp( lastname, "PolicerEnable" )==0 )
	{
	}else if( strcmp( lastname, "CommittedRate" )==0 )
	{
	}else if( strcmp( lastname, "CommittedBurstSize" )==0 )
	{
	}else if( strcmp( lastname, "MeterType" )==0 )
	{
	}else if( strcmp( lastname, "ConformingAction" )==0 )
	{
	}else if( strcmp( lastname, "NonConformingAction" )==0 )
	{
	}else{
		return ERR_9005;
	}
	
	return 0;
}




int objPolicer(char *name, struct sCWMP_ENTITY *entity, int type, void *data)
{
	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		return 0;
	     }
	case eCWMP_tADDOBJ:
	     {
		return 0;
	     }
	case eCWMP_tDELOBJ:
	     {
		return 0;
	     }
	case eCWMP_tUPDATEOBJ:	
	     {
	     	return 0;
	     }
	}
	return -1;
}
#endif

int getClassEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned char vChar=0;
	unsigned int  instnum=0,chainid=0;
	char buf[256]={0};
	MIB_CE_IP_QOS_T *p, qos_entity;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	instnum=getClassInstNum(name);
	if(instnum==0) return ERR_9005;
	p = &qos_entity;
	if(getClassEntryByInstNum( instnum, p, &chainid )<0) return ERR_9002;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "ClassificationKey" )==0 )
	{
		*data = uintdup( p->InstanceNum );
	}else if( strcmp( lastname, "ClassificationEnable" )==0 )
	{
		 *data=booldup( p->enable );
	}else if( strcmp( lastname, "ClassificationStatus" )==0 )
	{
		if( p->enable==0 )
		 	*data=strdup( "Disabled" );
		else
			*data=strdup( "Enabled" );
	}else if( strcmp( lastname, "ClassificationOrder" )==0 )
	{
		 *data=uintdup( chainid+1 );
	}else if( strcmp( lastname, "ClassInterface" )==0 )
	{
		if( p->phyPort==0xff )
			*data=strdup( "LAN" );
#if (defined(CONFIG_EXT_SWITCH)  && defined(IP_QOS_VPORT))
		else if( p->phyPort==0 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4");
		else if( p->phyPort==1 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3");
		else if( p->phyPort==2 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2");
		else if( p->phyPort==3 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1");
#else
		else if( p->phyPort==0 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1");
#endif
#ifdef WLAN_SUPPORT
		else if( p->phyPort==5 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.WLANConfiguration.1");
#ifdef WLAN_MBSSID
		else if( p->phyPort==6 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.WLANConfiguration.2");
		else if( p->phyPort==7 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.WLANConfiguration.3");
		else if( p->phyPort==8 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.WLANConfiguration.4");
		else if( p->phyPort==9 )
			*data=strdup("InternetGatewayDevice.LANDevice.1.WLANConfiguration.5");
#endif //WLAN_MBSSID
#endif //WLAN_SUPPORT
		else 
			return ERR_9002;
	}else if( strcmp( lastname, "DestIP" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&p->dip)));
		*data = strdup( buf );
	}else if( strcmp( lastname, "DestMask" )==0 )
	{
		unsigned long mask=0;
		int i,mbit;
		mbit = p->dmaskbit;
		for(i=0;i<32;i++)
		{
			mask = mask << 1;
			if(mbit)
			{ 
				mask = mask | 0x1;
				mbit--;
			}
		}
		mask = ntohl(mask);
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&mask)));
		*data = strdup( buf );
//	}else if( strcmp( lastname, "DestIPExclude" )==0 )
//	{
//		 *data=booldup( 0 );
	}else if( strcmp( lastname, "SourceIP" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&p->sip)));
		*data = strdup( buf );
	}else if( strcmp( lastname, "SourceMask" )==0 )
	{
		unsigned long mask=0;
		int i,mbit;
		mbit = p->smaskbit;
		for(i=0;i<32;i++)
		{
			mask = mask << 1;
			if(mbit)
			{ 
				mask = mask | 0x1;
				mbit--;
			}
		}
		mask = ntohl(mask);
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&mask)));
		*data = strdup( buf );
//	}else if( strcmp( lastname, "SourceIPExclude" )==0 )
//	{
//		 *data=booldup( 0 );
	}else if( strcmp( lastname, "Protocol" )==0 )
	{
		if( p->protoType==PROTO_NONE )
			*data=intdup( -1 );
		else if( p->protoType==PROTO_TCP )
			*data=intdup( 6 );
		else if( p->protoType==PROTO_UDP )
			*data=intdup( 17 );
		else if( p->protoType==PROTO_ICMP )
			*data=intdup( 1 );
		else
			return ERR_9002;			
//	}else if( strcmp( lastname, "ProtocolExclude" )==0 )
//	{
//		 *data=booldup( 0 );
	}else if( strcmp( lastname, "DestPort" )==0 )
	{
		if( p->dPort==0 )
			*data=intdup( -1 );
		else
			*data=intdup( p->dPort );
//	}else if( strcmp( lastname, "DestPortRangeMax" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "DestPortExclude" )==0 )
//	{
//		 *data=booldup( 0 );
	}else if( strcmp( lastname, "SourcePort" )==0 )
	{
		if( p->sPort==0 )
			*data=intdup( -1 );
		else
			*data=intdup( p->sPort );
//	}else if( strcmp( lastname, "SourcePortRangeMax" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "SourcePortExclude" )==0 )
//	{
//		 *data=booldup( 0 );
//	}else if( strcmp( lastname, "SourceMACAddress" )==0 )
//	{
//		 *data=strdup( "" );
//	}else if( strcmp( lastname, "SourceMACExclude" )==0 )
//	{
//		 *data=booldup( 0 );
//	}else if( strcmp( lastname, "DestMACAddress" )==0 )
//	{
//		 *data=strdup( "" );
//	}else if( strcmp( lastname, "DestMACExclude" )==0 )
//	{
//		 *data=booldup( 0 );
//	}else if( strcmp( lastname, "DSCPCheck" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "DSCPExclude" )==0 )
//	{
//		 *data=booldup( 0 );
//	}else if( strcmp( lastname, "DSCPMark" )==0 )
//	{
//		 *data=intdup( -1 );
	}else if( strcmp( lastname, "EthernetPriorityCheck" )==0 )
	{
		if( p->m_1p==0 )
		 	*data=intdup( -1 );
		else
			*data=intdup( (int)p->m_1p-1 );
//	}else if( strcmp( lastname, "EthernetPriorityExclude" )==0 )
//	{
//		 *data=booldup( 0 );
	}else if( strcmp( lastname, "EthernetPriorityMark" )==0 )
	{
		if( p->m_1p==0 )
		 	*data=intdup( -1 );
		else
			*data=intdup( (int)p->m_1p-1 );
//	}else if( strcmp( lastname, "VLANIDCheck" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "VLANIDExclude" )==0 )
//	{
//		 *data=booldup( 0 );
//	}else if( strcmp( lastname, "ForwardingPolicy" )==0 )
//	{
//		 *data=uintdup( 0 );
//	}else if( strcmp( lastname, "ClassPolicer" )==0 )
//	{
//		 *data=intdup( -1 );
	}else if( strcmp( lastname, "ClassQueue" )==0 )
	{
		 *data=intdup( p->prior+1 );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setClassEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned char vChar=0;
	unsigned int  instnum=0,chainid=0;
	MIB_CE_IP_QOS_T *p, qos_entity, old_qos_entity;
	struct in_addr in;
	char	*pzeroip="0.0.0.0";
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;


	instnum=getClassInstNum(name);
	if(instnum==0) return ERR_9005;
	p = &qos_entity;
	if(getClassEntryByInstNum( instnum, p, &chainid )<0) return ERR_9002;
	memcpy( &old_qos_entity, &qos_entity, sizeof(MIB_CE_IP_QOS_T) );

	if( strcmp( lastname, "ClassificationEnable" )==0 )
	{

		int *i = data;

		if(i==NULL) return ERR_9007;
		p->enable = (*i==0) ? 0:1;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "ClassificationOrder" )==0 )
	{

		unsigned int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i==0 ) return ERR_9007;
		if( *i!=(chainid+1) ) return ERR_9001;
		return 0;
	}else if( strcmp( lastname, "ClassInterface" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strcmp( buf, "LAN" )==0 )
			p->phyPort=0xff;
#if (defined(CONFIG_EXT_SWITCH)  && defined(IP_QOS_VPORT))
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4" )==0 )
			p->phyPort=0;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3" )==0 )
			p->phyPort=1;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2" )==0 )
			p->phyPort=2;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1" )==0 )
			p->phyPort=3;
#else
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1" )==0 )
			p->phyPort=0;
#endif
#ifdef WLAN_SUPPORT
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1" )==0 )
			p->phyPort=5;
#ifdef WLAN_MBSSID
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.2" )==0 )
			p->phyPort=6;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.3" )==0 )
			p->phyPort=7;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.4" )==0 )
			p->phyPort=8;
		else if( strcmp( buf, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.5" )==0 )
			p->phyPort=9;
#endif //WLAN_MBSSID
#endif //WLAN_SUPPORT
		else
			return ERR_9007;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "DestIP" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy( p->dip, &in, sizeof(struct in_addr) );
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "DestMask" )==0 )
	{
		unsigned long mask;
		int intVal, i, mbit;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;
		if( inet_aton( buf, (struct in_addr *)&mask )==0 ) //the ip address is error.
			return ERR_9007;
		mask = htonl(mask);
		mbit=0; intVal=0;
		for (i=0; i<32; i++)
		{
			if (mask&0x80000000)
			{
				if (intVal) return ERR_9007;
				mbit++;
			}
			else
				intVal=1;
			mask <<= 1;
		}
		p->dmaskbit = mbit;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "DestIPExclude" )==0 )
//	{
	}else if( strcmp( lastname, "SourceIP" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy( p->sip, &in, sizeof(struct in_addr) );
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
	}else if( strcmp( lastname, "SourceMask" )==0 )
	{
		unsigned long mask;
		int intVal, i, mbit;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;
		if( inet_aton( buf, (struct in_addr *)&mask )==0 ) //the ip address is error.
			return ERR_9007;
		mask = htonl(mask);
		mbit=0; intVal=0;
		for (i=0; i<32; i++)
		{
			if (mask&0x80000000)
			{
				if (intVal) return ERR_9007;
				mbit++;
			}
			else
				intVal=1;
			mask <<= 1;
		}
		p->smaskbit = mbit;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "SourceIPExclude" )==0 )
//	{
	}else if( strcmp( lastname, "Protocol" )==0 )
	{


		int *pro = data;

		if(pro==NULL) return ERR_9007;
		switch(*pro)
		{
		case -1:
			p->protoType=PROTO_NONE;
			break;
		case 1:
			p->protoType=PROTO_ICMP;
			break;
		case 6:
			p->protoType=PROTO_TCP;
			break;
		case 17:
			p->protoType=PROTO_UDP;
			break;
		default:
			return ERR_9001;
		}
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "ProtocolExclude" )==0 )
//	{
	}else if( strcmp( lastname, "DestPort" )==0 )
	{

		int *dport = data;

		if( dport==NULL ) return ERR_9007;
		if( *dport==-1 )
			p->dPort = 0;
		else if( (*dport>=1) && (*dport<=65535) )
			p->dPort = *dport;
		else
			return ERR_9007;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "DestPortRangeMax" )==0 )
//	{
//	}else if( strcmp( lastname, "DestPortExclude" )==0 )
//	{
	}else if( strcmp( lastname, "SourcePort" )==0 )
	{

		int *sport = data;

		if( sport==NULL ) return ERR_9007;
		if( *sport==-1 )
			p->sPort = 0;
		else if( (*sport>=1) && (*sport<=65535) )
			p->sPort = *sport;
		else
			return ERR_9007;
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "SourcePortRangeMax" )==0 )
//	{
//	}else if( strcmp( lastname, "SourcePortExclude" )==0 )
//	{
//	}else if( strcmp( lastname, "SourceMACAddress" )==0 )
//	{
//	}else if( strcmp( lastname, "SourceMACExclude" )==0 )
//	{
//	}else if( strcmp( lastname, "DestMACAddress" )==0 )
//	{
//	}else if( strcmp( lastname, "DestMACExclude" )==0 )
//	{
//	}else if( strcmp( lastname, "DSCPCheck" )==0 )
//	{
//	}else if( strcmp( lastname, "DSCPExclude" )==0 )
//	{
//	}else if( strcmp( lastname, "DSCPMark" )==0 )
//	{
	}else if( strcmp( lastname, "EthernetPriorityCheck" )==0 )
	{


		int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i<-1 || *i>7 ) return ERR_9007;
		p->m_1p = (unsigned char)(*i+1);
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "EthernetPriorityExclude" )==0 )
//	{
	}else if( strcmp( lastname, "EthernetPriorityMark" )==0 )
	{


		int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i<-1 || *i>7 ) return ERR_9007;
		p->m_1p = (unsigned char)(*i+1);
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "VLANIDCheck" )==0 )
//	{
//	}else if( strcmp( lastname, "VLANIDExclude" )==0 )
//	{
//	}else if( strcmp( lastname, "ForwardingPolicy" )==0 )
//	{
//	}else if( strcmp( lastname, "ClassPolicer" )==0 )
//	{
	}else if( strcmp( lastname, "ClassQueue" )==0 )
	{


		int *i = data;

		if(i==NULL) return ERR_9007;
		if( *i<1 || *i>IPQOS_NUM_PRIOQ ) return ERR_9007;
		p->prior = (unsigned char)(*i-1);
		mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, chainid );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_IPQoSRule, CWMP_RESTART, chainid, &old_qos_entity, sizeof(MIB_CE_IP_QOS_T) );
		return 0;
#else
		return 1;
#endif
	}else{
		return ERR_9005;
	}
	
	return 0;
}


int objClass(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	//fprintf( stderr, "%s:action:%d: %s\n", __FUNCTION__, type, name);

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		unsigned int num=0,MaxInstNum=0,i;
		struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;
		MIB_CE_IP_QOS_T *p, qos_entity;

		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

		MaxInstNum = findClassInstNum();
		num = mib_chain_total( MIB_IP_QOS_TBL );
		for( i=0; i<num;i++ )
		{
			p = &qos_entity;
			if( !mib_chain_get( MIB_IP_QOS_TBL, i, (void*)p ) )
				continue;
			
			if( p->InstanceNum==0 ) //maybe createn by web or cli
			{
				MaxInstNum++;
				p->InstanceNum = MaxInstNum;
				mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, i );
			}
			if( create_Object( c, tClassObject, sizeof(tClassObject), 1, p->InstanceNum ) < 0 )
				return -1;
			//c = & (*c)->sibling;
		}
		add_objectNum( name, MaxInstNum );
		return 0;	     		     	
	     }
	case eCWMP_tADDOBJ:
	     {
	     	int ret;
	     	unsigned int num=0;
	     	
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
		num = mib_chain_total( MIB_IP_QOS_TBL );
		if(num>=MAX_QOS_RULE) return ERR_9004;
		
		ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tClassObject, sizeof(tClassObject), data );
		if( ret >= 0 )
		{
			MIB_CE_IP_QOS_T qos_entry;
			memset( &qos_entry, 0, sizeof( MIB_CE_IP_QOS_T ) );
			qos_entry.InstanceNum = *(unsigned int*)data;
			qos_entry.phyPort=0xff;
			qos_entry.prior=3;
			qos_entry.m_iptos=0xff;
			mib_chain_add( MIB_IP_QOS_TBL, (unsigned char*)&qos_entry );
		}
		return ret;
	     }
	case eCWMP_tDELOBJ:
	     {
		unsigned int i,num;
		int ret=0;
		MIB_CE_IP_QOS_T *p,qos_entity;
	     	
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
	
		num = mib_chain_total( MIB_IP_QOS_TBL );
		for( i=0; i<num;i++ )
		{
			p = &qos_entity;
			if( !mib_chain_get( MIB_IP_QOS_TBL, i, (void*)p ))
				continue;
			if( p->InstanceNum == *(unsigned int*)data )
				break;
		}	     	
		if(i==num) return ERR_9005;
		
#ifdef _CWMP_APPLY_
		//why don't use CWMP_RESTART?
		//for IPQoS, use the chain index to count the mark number, get_classification_mark()
		//after deleting one rule, the mark numbers will change
		//hence, stop all rules=>delete one rule=>start the rest rules;
		apply_IPQoSRule( CWMP_STOP, -1, NULL );
#endif
	     	mib_chain_delete( MIB_IP_QOS_TBL, i );
		ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
#ifdef _CWMP_APPLY_
		apply_IPQoSRule( CWMP_START, -1, NULL );
#else
		if( ret == 0 ) return 1;
#endif
		return ret;
	     }
	case eCWMP_tUPDATEOBJ:	
	     {
	     	int num=0,i;
	     	struct CWMP_LINKNODE *old_table;
	     	
	     	num = mib_chain_total( MIB_IP_QOS_TBL );
	     	old_table = (struct CWMP_LINKNODE *)entity->next;
	     	entity->next = NULL;
	     	for( i=0; i<num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;
			MIB_CE_IP_QOS_T *p,qos_entity;

			p = &qos_entity;
			if( !mib_chain_get( MIB_IP_QOS_TBL, i, (void*)p ) )
				continue;
			
			remove_entity = remove_SiblingEntity( &old_table, p->InstanceNum );
			if( remove_entity!=NULL )
			{
				add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
			}else{ 
				unsigned int MaxInstNum=p->InstanceNum;
					
				add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tClassObject, sizeof(tClassObject), &MaxInstNum );
				if(MaxInstNum!=p->InstanceNum)
				{
					p->InstanceNum = MaxInstNum;
					mib_chain_update( MIB_IP_QOS_TBL, (unsigned char*)p, i );
				}
			}	
	     	}
	     	
	     	if( old_table )
	     		destroy_ParameterTable( (struct CWMP_NODE *)old_table );	     	

	     	return 0;
	     }
	}
	return -1;
}




int getQueueMnt(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char 	buff[256]={0};
	unsigned char vChar=0;
	unsigned int num=0;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
		mib_get(MIB_MPMODE, (void *)&vChar);
		vChar = vChar&0x02;
		*data = booldup( vChar!=0 );
	}else if( strcmp( lastname, "MaxQueues" )==0 )
	{
		*data=uintdup( IPQOS_NUM_PRIOQ );
	}else if( strcmp( lastname, "MaxClassificationEntries" )==0 )
	{
		*data=uintdup( MAX_QOS_RULE );
	}else if( strcmp( lastname, "ClassificationNumberOfEntries" )==0 )
	{
		num = mib_chain_total(MIB_IP_QOS_TBL);
		*data=uintdup( num );
	}else if( strcmp( lastname, "MaxAppEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "AppNumberOfEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "MaxFlowEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "FlowNumberOfEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "MaxPolicerEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "PolicerNumberOfEntries" )==0 )
	{
		 *data=uintdup( 0 );
	}else if( strcmp( lastname, "MaxQueueEntries" )==0 )
	{
		 *data=uintdup( IPQOS_NUM_PRIOQ );
	}else if( strcmp( lastname, "QueueNumberOfEntries" )==0 )
	{
		 *data=uintdup( IPQOS_NUM_PRIOQ );
//	}else if( strcmp( lastname, "DefaultForwardingPolicy" )==0 )
//	{
//		 *data=uintdup( 0 );
//	}else if( strcmp( lastname, "DefaultPolicer" )==0 )
//	{
//		 *data=intdup( -1 );
//	}else if( strcmp( lastname, "DefaultQueue" )==0 )
//	{
//		 *data=uintdup( 0 );
//	}else if( strcmp( lastname, "DefaultDSCPMark" )==0 )
//	{
//		 *data=intdup( -1 );
	}else if( strcmp( lastname, "DefaultEthernetPriorityMark" )==0 )
	{
		 *data=intdup( -1 );
	}else if( strcmp( lastname, "AvailableAppList" )==0 )
	{
		 *data=strdup( "" );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setQueueMnt(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned char vChar=0;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;


	if( strcmp( lastname, "Enable" )==0 )
	{

		int *i = data;

		if(i==NULL) return ERR_9007;
		mib_get(MIB_MPMODE, (void *)&vChar);
		if(*i==0)
			vChar &= 0xfd;
		else
			vChar |= 0x02;
		mib_set(MIB_MPMODE, (void *)&vChar);
#ifdef _CWMP_APPLY_
		//apply_IPQoS has higher priority than apply_IPQoSRule
		apply_add( CWMP_PRI_H, apply_IPQoS, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif
//	}else if( strcmp( lastname, "DefaultForwardingPolicy" )==0 )
//	{
//	}else if( strcmp( lastname, "DefaultPolicer" )==0 )
//	{
//	}else if( strcmp( lastname, "DefaultQueue" )==0 )
//	{
//	}else if( strcmp( lastname, "DefaultDSCPMark" )==0 )
//	{
	}else if( strcmp( lastname, "DefaultEthernetPriorityMark" )==0 )
	{

		int *i = data;

		if(i==NULL) return ERR_9007;
		if(*i!=-1) return ERR_9001;
		return 0;
	}else{
		return ERR_9005;
	}
	
	return 0;
}


/**************************************************************************************/
/* utility functions*/
/**************************************************************************************/

unsigned int getQueueInstNum( char *name )
{
	return getInstNum( name, "Queue" );
}

unsigned int getClassInstNum( char *name )
{
	return getInstNum( name, "Classification" );
}

unsigned int findClassInstNum(void)
{
	unsigned int ret=0, i,num;
	MIB_CE_IP_QOS_T *p,qos_entity;
	
	num = mib_chain_total( MIB_IP_QOS_TBL );
	for( i=0; i<num;i++ )
	{
		p = &qos_entity;
		if( !mib_chain_get( MIB_IP_QOS_TBL, i, (void*)p ))
			continue;
		if( p->InstanceNum > ret )
			ret = p->InstanceNum;
	}
	
	return ret;
}

int getClassEntryByInstNum(unsigned int instnum, MIB_CE_IP_QOS_T *p, unsigned int *id)
{
	int ret=-1;
	unsigned int i,num;
	
	if( (instnum==0) || (p==NULL) || (id==NULL) ) return ret;
	
	num = mib_chain_total( MIB_IP_QOS_TBL );
	for( i=0; i<num;i++ )
	{
		if( !mib_chain_get( MIB_IP_QOS_TBL, i, (void*)p ))
			continue;
		if( p->InstanceNum == instnum )
		{
			*id = i;
			ret=0;
			break;
		}
	}
	
	return ret;
}

#endif /*IP_QOS*/


#ifdef _STD_QOS_

/******************************************************************************/
struct CWMP_OP tClassEntityLeafOP = { getClassEntity, setClassEntity };

struct CWMP_PRMT tClassEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"ClassificationEnable",               eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ClassificationStatus",               eCWMP_tSTRING,	CWMP_READ,				&tClassEntityLeafOP},
{"ClassificationOrder",                eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ClassInterface",					   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestIP",							   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceIP",						   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"Protocol",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestPort",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestPortRangeMax",				   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourcePort",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourcePortRangeMax",				   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceMACAddress",				   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestMACAddress",					   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DSCPCheck",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DSCPMark",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"EthernetPriorityCheck",			   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"EthernetPriorityMark",			   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"OutOfBandInfo",					   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ClassQueue",						   eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},

#if 0
{"ClassificationKey",                  eCWMP_tUINT,		CWMP_READ,	&tClassEntityLeafOP},
{"Alias",                              eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestMask",                           eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestIPExclude",                      eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceMask",                         eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceIPExclude",                    eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ProtocolExclude",                    eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestPortExclude",                    eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourcePortExclude",                  eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceMACMask",                      eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceMACExclude",                   eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestMACMask",                        eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestMACExclude",                     eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"Ethertype",                          eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"EthertypeExclude",                   eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SSAP",                               eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SSAPExclude",                        eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DSAP",                               eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DSAPExclude",                        eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"LLCControl",                         eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"LLCControlExclude",                  eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SNAPOUI",                            eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SNAPOUIExclude",                     eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorClassID",                eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorClassIDExclude",         eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorClassIDMode",            eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorClassID",                  eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorClassIDExclude",           eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorClassIDMode",              eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceClientID",                     eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceClientIDExclude",              eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestClientID",                       eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestClientIDExclude",                eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceUserClassID",                  eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceUserClassIDExclude",           eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestUserClassID",                    eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestUserClassIDExclude",             eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorSpecificInfo",           eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorSpecificInfoExclude",    eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorSpecificInfoEnterprise", eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorSpecificInfoSubOption",  eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"SourceVendorSpecificInfoMode",       eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorSpecificInfo",             eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorSpecificInfoExclude",      eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorSpecificInfoEnterprise",   eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorSpecificInfoSubOption",    eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DestVendorSpecificInfoMode",         eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"TCPACK",                             eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"TCPACKExclude",                      eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"IPLengthMin",                        eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"IPLengthMax",                        eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"IPLengthExclude",                    eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"DSCPExclude",                        eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"EthernetPriorityExclude",            eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"VLANIDCheck",                        eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"VLANIDExclude",                      eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ForwardingPolicy",                   eCWMP_tUINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"TrafficClass",                       eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ClassPolicer",                       eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
{"ClassApp",                           eCWMP_tINT,		CWMP_READ|CWMP_WRITE,	&tClassEntityLeafOP},
#endif
};

enum eClassEntityLeaf
{
	eQC_ClassificationEnable,
	eQC_ClassificationStatus,
	eQC_ClassificationOrder,
	eQC_ClassInterface,
	eQC_DestIP,
	eQC_SourceIP,
	eQC_Protocol,
	eQC_DestPort,
	eQC_DestPortRangeMax,
	eQC_SourcePort,
	eQC_SourcePortRangeMax,
	eQC_SourceMACAddress,
	eQC_DestMACAddress,
	eQC_DSCPCheck,
	eQC_DSCPMark,
	eQC_EthernetPriorityCheck,
	eQC_EthernetPriorityMark,
	eQC_OutOfBandInfo,
	eQC_ClassQueue
#if 0	
	, eQC_ClassificationKey,
	eQC_Alias,
	eQC_DestMask,
	eQC_DestIPExclude,
	eQC_SourceMask,
	eQC_SourceIPExclude,
	eQC_ProtocolExclude,
	eQC_DestPortExclude,
	eQC_SourcePortExclude,
	eQC_SourceMACMask,
	eQC_SourceMACExclude,
	eQC_DestMACMask,
	eQC_DestMACExclude,
	eQC_Ethertype,
	eQC_EthertypeExclude,
	eQC_SSAP,
	eQC_SSAPExclude,
	eQC_DSAP,
	eQC_DSAPExclude,
	eQC_LLCControl,
	eQC_LLCControlExclude,
	eQC_SNAPOUI,
	eQC_SNAPOUIExclude,
	eQC_SourceVendorClassID,
	eQC_SourceVendorClassIDExclude,
	eQC_SourceVendorClassIDMode,
	eQC_DestVendorClassID,
	eQC_DestVendorClassIDExclude,
	eQC_DestVendorClassIDMode,
	eQC_SourceClientID,
	eQC_SourceClientIDExclude,
	eQC_DestClientID,
	eQC_DestClientIDExclude,
	eQC_SourceUserClassID,
	eQC_SourceUserClassIDExclude,
	eQC_DestUserClassID,
	eQC_DestUserClassIDExclude,
	eQC_SourceVendorSpecificInfo,
	eQC_SourceVendorSpecificInfoExclude,
	eQC_SourceVendorSpecificInfoEnterprise,
	eQC_SourceVendorSpecificInfoSubOption,
	eQC_SourceVendorSpecificInfoMode,
	eQC_DestVendorSpecificInfo,
	eQC_DestVendorSpecificInfoExclude,
	eQC_DestVendorSpecificInfoEnterprise,
	eQC_DestVendorSpecificInfoSubOption,
	eQC_DestVendorSpecificInfoMode,
	eQC_TCPACK,
	eQC_TCPACKExclude,
	eQC_IPLengthMin,
	eQC_IPLengthMax,
	eQC_IPLengthExclude,
	eQC_DSCPExclude,
	eQC_EthernetPriorityExclude,
	eQC_VLANIDCheck,
	eQC_VLANIDExclude,
	eQC_ForwardingPolicy,
	eQC_TrafficClass,
	eQC_ClassPolicer,
	eQC_ClassApp
#endif
};

struct CWMP_LEAF tClassEntityLeaf[] =
{
{ &tClassEntityLeafInfo[eQC_ClassificationEnable]               },
{ &tClassEntityLeafInfo[eQC_ClassificationStatus]               },
{ &tClassEntityLeafInfo[eQC_ClassificationOrder]				},
{ &tClassEntityLeafInfo[eQC_ClassInterface]                     },
{ &tClassEntityLeafInfo[eQC_DestIP] 							},
{ &tClassEntityLeafInfo[eQC_SourceIP]							},
{ &tClassEntityLeafInfo[eQC_Protocol]							},
{ &tClassEntityLeafInfo[eQC_DestPort]							},
{ &tClassEntityLeafInfo[eQC_DestPortRangeMax]					},
{ &tClassEntityLeafInfo[eQC_SourcePort] 						},
{ &tClassEntityLeafInfo[eQC_SourcePortRangeMax] 				},
{ &tClassEntityLeafInfo[eQC_SourceMACAddress]					},
{ &tClassEntityLeafInfo[eQC_DestMACAddress] 					},
{ &tClassEntityLeafInfo[eQC_DSCPCheck]							},
{ &tClassEntityLeafInfo[eQC_DSCPMark]							},
{ &tClassEntityLeafInfo[eQC_EthernetPriorityCheck]				},
{ &tClassEntityLeafInfo[eQC_EthernetPriorityMark]				},
{ &tClassEntityLeafInfo[eQC_OutOfBandInfo]						},
{ &tClassEntityLeafInfo[eQC_ClassQueue] 						},

#if 0
{ &tClassEntityLeafInfo[eQC_ClassificationKey]                  },
{ &tClassEntityLeafInfo[eQC_Alias]                              },
{ &tClassEntityLeafInfo[eQC_DestMask]                           },
{ &tClassEntityLeafInfo[eQC_DestIPExclude]                      },
{ &tClassEntityLeafInfo[eQC_SourceMask]                         },
{ &tClassEntityLeafInfo[eQC_SourceIPExclude]                    },
{ &tClassEntityLeafInfo[eQC_ProtocolExclude]                    },
{ &tClassEntityLeafInfo[eQC_DestPortExclude]                    },
{ &tClassEntityLeafInfo[eQC_SourcePortExclude]                  },
{ &tClassEntityLeafInfo[eQC_SourceMACMask]                      },
{ &tClassEntityLeafInfo[eQC_SourceMACExclude]                   },
{ &tClassEntityLeafInfo[eQC_DestMACMask]                        },
{ &tClassEntityLeafInfo[eQC_DestMACExclude]                     },
{ &tClassEntityLeafInfo[eQC_Ethertype]                          },
{ &tClassEntityLeafInfo[eQC_EthertypeExclude]                   },
{ &tClassEntityLeafInfo[eQC_SSAP]                               },
{ &tClassEntityLeafInfo[eQC_SSAPExclude]                        },
{ &tClassEntityLeafInfo[eQC_DSAP]                               },
{ &tClassEntityLeafInfo[eQC_DSAPExclude]                        },
{ &tClassEntityLeafInfo[eQC_LLCControl]                         },
{ &tClassEntityLeafInfo[eQC_LLCControlExclude]                  },
{ &tClassEntityLeafInfo[eQC_SNAPOUI]                            },
{ &tClassEntityLeafInfo[eQC_SNAPOUIExclude]                     },
{ &tClassEntityLeafInfo[eQC_SourceVendorClassID]                },
{ &tClassEntityLeafInfo[eQC_SourceVendorClassIDExclude]         },
{ &tClassEntityLeafInfo[eQC_SourceVendorClassIDMode]            },
{ &tClassEntityLeafInfo[eQC_DestVendorClassID]                  },
{ &tClassEntityLeafInfo[eQC_DestVendorClassIDExclude]           },
{ &tClassEntityLeafInfo[eQC_DestVendorClassIDMode]              },
{ &tClassEntityLeafInfo[eQC_SourceClientID]                     },
{ &tClassEntityLeafInfo[eQC_SourceClientIDExclude]              },
{ &tClassEntityLeafInfo[eQC_DestClientID]                       },
{ &tClassEntityLeafInfo[eQC_DestClientIDExclude]                },
{ &tClassEntityLeafInfo[eQC_SourceUserClassID]                  },
{ &tClassEntityLeafInfo[eQC_SourceUserClassIDExclude]           },
{ &tClassEntityLeafInfo[eQC_DestUserClassID]                    },
{ &tClassEntityLeafInfo[eQC_DestUserClassIDExclude]             },
{ &tClassEntityLeafInfo[eQC_SourceVendorSpecificInfo]           },
{ &tClassEntityLeafInfo[eQC_SourceVendorSpecificInfoExclude]    },
{ &tClassEntityLeafInfo[eQC_SourceVendorSpecificInfoEnterprise] },
{ &tClassEntityLeafInfo[eQC_SourceVendorSpecificInfoSubOption]  },
{ &tClassEntityLeafInfo[eQC_SourceVendorSpecificInfoMode]       },
{ &tClassEntityLeafInfo[eQC_DestVendorSpecificInfo]             },
{ &tClassEntityLeafInfo[eQC_DestVendorSpecificInfoExclude]      },
{ &tClassEntityLeafInfo[eQC_DestVendorSpecificInfoEnterprise]   },
{ &tClassEntityLeafInfo[eQC_DestVendorSpecificInfoSubOption]    },
{ &tClassEntityLeafInfo[eQC_DestVendorSpecificInfoMode]         },
{ &tClassEntityLeafInfo[eQC_TCPACK]                             },
{ &tClassEntityLeafInfo[eQC_TCPACKExclude]                      },
{ &tClassEntityLeafInfo[eQC_IPLengthMin]                        },
{ &tClassEntityLeafInfo[eQC_IPLengthMax]                        },
{ &tClassEntityLeafInfo[eQC_IPLengthExclude]                    },
{ &tClassEntityLeafInfo[eQC_DSCPExclude]                        },
{ &tClassEntityLeafInfo[eQC_EthernetPriorityExclude]            },
{ &tClassEntityLeafInfo[eQC_VLANIDCheck]                        },
{ &tClassEntityLeafInfo[eQC_VLANIDExclude]                      },
{ &tClassEntityLeafInfo[eQC_ForwardingPolicy]                   },
{ &tClassEntityLeafInfo[eQC_TrafficClass]                       },
{ &tClassEntityLeafInfo[eQC_ClassPolicer]                       },
{ &tClassEntityLeafInfo[eQC_ClassApp]							},
#endif
{ NULL }
};
/******************************************************************************/

/******************************************************************************/
#if 0
struct CWMP_OP tAppEntityLeafOP = { getAppEntity, setAppEntity };

struct CWMP_PRMT tAppEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"AppKey",                          eCWMP_tUINT,	CWMP_READ,	&tAppEntityLeafOP},
{"AppEnable",                       eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppStatus",                       eCWMP_tSTRING,	CWMP_READ,	&tAppEntityLeafOP},
{"Alias",                           eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"ProtocolIdentifier",              eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppName",                         eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultForwardingPolicy",      eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultTrafficClass",          eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultPolicer",               eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultQueue",                 eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultDSCPMark",              eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
{"AppDefaultEthernetPriorityMark",	eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tAppEntityLeafOP},
};

enum eAppEntityLeaf
{
	eQC_AppKey,
	eQC_AppEnable,
	eQC_AppStatus,
	eQC_AppAlias,
	eQC_ProtocolIdentifier,
	eQC_AppName,
	eQC_AppDefaultForwardingPolicy,
	eQC_AppDefaultTrafficClass,
	eQC_AppDefaultPolicer,
	eQC_AppDefaultQueue,
	eQC_AppDefaultDSCPMark,
	eQC_AppDefaultEthernetPriorityMark
};

struct CWMP_LEAF tAppEntityLeaf[] =
{
{ &tAppEntityLeafInfo[eQC_AppKey]                         },
{ &tAppEntityLeafInfo[eQC_AppEnable]                      },
{ &tAppEntityLeafInfo[eQC_AppStatus]                      },
{ &tAppEntityLeafInfo[eQC_AppAlias]                       },
{ &tAppEntityLeafInfo[eQC_ProtocolIdentifier]             },
{ &tAppEntityLeafInfo[eQC_AppName]                        },
{ &tAppEntityLeafInfo[eQC_AppDefaultForwardingPolicy]     },
{ &tAppEntityLeafInfo[eQC_AppDefaultTrafficClass]         },
{ &tAppEntityLeafInfo[eQC_AppDefaultPolicer]              },
{ &tAppEntityLeafInfo[eQC_AppDefaultQueue]                },
{ &tAppEntityLeafInfo[eQC_AppDefaultDSCPMark]             },
{ &tAppEntityLeafInfo[eQC_AppDefaultEthernetPriorityMark] },
{ NULL }
};
/******************************************************************************/

/******************************************************************************/
struct CWMP_OP tFlowEntityLeafOP = { getFlowEntity, setFlowEntity };

struct CWMP_PRMT tFlowEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"FlowKey",                     eCWMP_tUINT,	CWMP_READ,	&tFlowEntityLeafOP},
{"FlowEnable",                  eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowStatus",                  eCWMP_tSTRING,	CWMP_READ,	&tFlowEntityLeafOP},
{"Alias",                       eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowType",                    eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowTypeParameters",          eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowName",                    eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"AppIdentifier",               eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowForwardingPolicy",        eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowTrafficClass",            eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowPolicer",                 eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowQueue",                   eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowDSCPMark",                eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
{"FlowEthernetPriorityMark",	eCWMP_tINT,	CWMP_READ|CWMP_WRITE,	&tFlowEntityLeafOP},
};

enum eFlowEntityLeaf
{
	eQC_FlowKey,
	eQC_FlowEnable,
	eQC_FlowStatus,
	eQC_FlowAlias,
	eQC_FlowType,
	eQC_FlowTypeParameters,
	eQC_FlowName,
	eQC_AppIdentifier,
	eQC_FlowForwardingPolicy,
	eQC_FlowTrafficClass,
	eQC_FlowPolicer,
	eQC_FlowQueue,
	eQC_FlowDSCPMark,
	eQC_FlowEthernetPriorityMark
};

struct CWMP_LEAF tFlowEntityLeaf[] =
{
{ &tFlowEntityLeafInfo[eQC_FlowKey]                  },
{ &tFlowEntityLeafInfo[eQC_FlowEnable]               },
{ &tFlowEntityLeafInfo[eQC_FlowStatus]               },
{ &tFlowEntityLeafInfo[eQC_FlowAlias]                },
{ &tFlowEntityLeafInfo[eQC_FlowType]                 },
{ &tFlowEntityLeafInfo[eQC_FlowTypeParameters]       },
{ &tFlowEntityLeafInfo[eQC_FlowName]                 },
{ &tFlowEntityLeafInfo[eQC_AppIdentifier]            },
{ &tFlowEntityLeafInfo[eQC_FlowForwardingPolicy]     },
{ &tFlowEntityLeafInfo[eQC_FlowTrafficClass]         },
{ &tFlowEntityLeafInfo[eQC_FlowPolicer]              },
{ &tFlowEntityLeafInfo[eQC_FlowQueue]                },
{ &tFlowEntityLeafInfo[eQC_FlowDSCPMark]             },
{ &tFlowEntityLeafInfo[eQC_FlowEthernetPriorityMark] },
{ NULL }
};
/******************************************************************************/

/******************************************************************************/
struct CWMP_OP tPolicerEntityLeafOP = { getPolicerEntity, setPolicerEntity };

struct CWMP_PRMT tPolicerEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"PolicerKey",                          eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"PolicerEnable",                       eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"PolicerStatus",                       eCWMP_tSTRING,	CWMP_READ,	&tPolicerEntityLeafOP},
{"Alias",                               eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"CommittedRate",                       eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"CommittedBurstSize",                  eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"ExcessBurstSize",                     eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"PeakRate",                            eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"PeakBurstSize",                       eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"MeterType",                           eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"PossibleMeterTypes",                  eCWMP_tSTRING,	CWMP_READ,	&tPolicerEntityLeafOP},
{"ConformingAction",                    eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"PartialConformingAction",             eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"NonConformingAction",                 eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tPolicerEntityLeafOP},
{"CountedPackets",                      eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"CountedBytes",                        eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"TotalCountedPackets",                 eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"TotalCountedBytes",                   eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"ConformingCountedPackets",            eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"ConformingCountedBytes",              eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"PartiallyConformingCountedPackets",   eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"PartiallyConformingCountedBytes",     eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"NonConformingCountedPackets",         eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
{"NonConformingCountedBytes",			eCWMP_tUINT,	CWMP_READ,	&tPolicerEntityLeafOP},
};

enum ePolicerEntityLeaf
{
	eQC_PolicerKey,
	eQC_PolicerEnable,
	eQC_PolicerStatus,
	eQC_PolicerAlias,
	eQC_CommittedRate,
	eQC_CommittedBurstSize,
	eQC_ExcessBurstSize,
	eQC_PeakRate,
	eQC_PeakBurstSize,
	eQC_MeterType,
	eQC_PossibleMeterTypes,
	eQC_ConformingAction,
	eQC_PartialConformingAction,
	eQC_NonConformingAction,
	eQC_CountedPackets,
	eQC_CountedBytes,
	eQC_TotalCountedPackets,
	eQC_TotalCountedBytes,
	eQC_ConformingCountedPackets,
	eQC_ConformingCountedBytes,
	eQC_PartiallyConformingCountedPackets,
	eQC_PartiallyConformingCountedBytes,
	eQC_NonConformingCountedPackets,
	eQC_NonConformingCountedBytes
};

struct CWMP_LEAF tPolicerEntityLeaf[] =
{
{ &tPolicerEntityLeafInfo[eQC_PolicerKey]                        },
{ &tPolicerEntityLeafInfo[eQC_PolicerEnable]                     },
{ &tPolicerEntityLeafInfo[eQC_PolicerStatus]                     },
{ &tPolicerEntityLeafInfo[eQC_PolicerAlias]                      },
{ &tPolicerEntityLeafInfo[eQC_CommittedRate]                     },
{ &tPolicerEntityLeafInfo[eQC_CommittedBurstSize]                },
{ &tPolicerEntityLeafInfo[eQC_ExcessBurstSize]                   },
{ &tPolicerEntityLeafInfo[eQC_PeakRate]                          },
{ &tPolicerEntityLeafInfo[eQC_PeakBurstSize]                     },
{ &tPolicerEntityLeafInfo[eQC_MeterType]                         },
{ &tPolicerEntityLeafInfo[eQC_PossibleMeterTypes]                },
{ &tPolicerEntityLeafInfo[eQC_ConformingAction]                  },
{ &tPolicerEntityLeafInfo[eQC_PartialConformingAction]           },
{ &tPolicerEntityLeafInfo[eQC_NonConformingAction]               },
{ &tPolicerEntityLeafInfo[eQC_CountedPackets]                    },
{ &tPolicerEntityLeafInfo[eQC_CountedBytes]                      },
{ &tPolicerEntityLeafInfo[eQC_TotalCountedPackets]               },
{ &tPolicerEntityLeafInfo[eQC_TotalCountedBytes]                 },
{ &tPolicerEntityLeafInfo[eQC_ConformingCountedPackets]          },
{ &tPolicerEntityLeafInfo[eQC_ConformingCountedBytes]            },
{ &tPolicerEntityLeafInfo[eQC_PartiallyConformingCountedPackets] },
{ &tPolicerEntityLeafInfo[eQC_PartiallyConformingCountedBytes]   },
{ &tPolicerEntityLeafInfo[eQC_NonConformingCountedPackets]       },
{ &tPolicerEntityLeafInfo[eQC_NonConformingCountedBytes]         },
{ NULL }
};
#endif
/******************************************************************************/

/******************************************************************************/
struct CWMP_OP tQueueEntityLeafOP = { getQueueEntity, setQueueEntity };

struct CWMP_PRMT tQueueEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"QueueEnable",         eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"QueueStatus",         eCWMP_tSTRING,	CWMP_READ,				&tQueueEntityLeafOP},
{"QueueInterface",		eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"QueueWeight",         eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"QueuePrecedence",     eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"SchedulerAlgorithm",	eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"ShapingRate", 		eCWMP_tINT, CWMP_READ|CWMP_WRITE,		&tQueueEntityLeafOP},

#if 0
{"QueueKey",            eCWMP_tUINT,	CWMP_READ,				&tQueueEntityLeafOP},
{"Alias",               eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"TrafficClasses",      eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"QueueBufferLength",   eCWMP_tUINT,	CWMP_READ,				&tQueueEntityLeafOP},
{"REDThreshold",        eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"REDPercentage",       eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"DropAlgorithm",       eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
{"ShapingBurstSize",	eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueEntityLeafOP},
#endif
};

enum eQueueEntityLeaf
{
	eQC_QueueEnable,
	eQC_QueueStatus,
	eQC_QueueInterface,
	eQC_QueueWeight,
	eQC_QueuePrecedence,
	eQC_SchedulerAlgorithm,
	eQC_ShapingRate
	
#if 0
	, QC_QueueKey,
	eQC_QueueAlias,
	eQC_TrafficClasses,
	eQC_QueueBufferLength,
	eQC_REDThreshold,
	eQC_REDPercentage,
	eQC_DropAlgorithm,
	eQC_ShapingBurstSize
#endif
};

struct CWMP_LEAF tQueueEntityLeaf[] =
{
{ &tQueueEntityLeafInfo[eQC_QueueEnable]        },
{ &tQueueEntityLeafInfo[eQC_QueueStatus]        },
{ &tQueueEntityLeafInfo[eQC_QueueInterface]     },
{ &tQueueEntityLeafInfo[eQC_QueueWeight]		},
{ &tQueueEntityLeafInfo[eQC_QueuePrecedence]	},
{ &tQueueEntityLeafInfo[eQC_SchedulerAlgorithm] },
{ &tQueueEntityLeafInfo[eQC_ShapingRate]		},

#if 0
{ &tQueueEntityLeafInfo[eQC_QueueKey]			},
{ &tQueueEntityLeafInfo[eQC_QueueAlias]         },
{ &tQueueEntityLeafInfo[eQC_TrafficClasses]     },
{ &tQueueEntityLeafInfo[eQC_QueueBufferLength]  },
{ &tQueueEntityLeafInfo[eQC_REDThreshold]       },
{ &tQueueEntityLeafInfo[eQC_REDPercentage]      },
{ &tQueueEntityLeafInfo[eQC_DropAlgorithm]      },
{ &tQueueEntityLeafInfo[eQC_ShapingBurstSize]   },
#endif
{ NULL }
};
/******************************************************************************/

/******************************************************************************/
#if 0
struct CWMP_OP tQueueStatsEntityLeafOP = { getQueueStatsEntity, setQueueStatsEntity };

struct CWMP_PRMT tQueueStatsEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",                      eCWMP_tBOOLEAN,	CWMP_READ|CWMP_WRITE,	&tQueueStatsEntityLeafOP},
{"Status",                      eCWMP_tSTRING,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"Alias",                       eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueStatsEntityLeafOP},
{"Queue",                       eCWMP_tUINT,	CWMP_READ|CWMP_WRITE,	&tQueueStatsEntityLeafOP},
{"Interface",                   eCWMP_tSTRING,	CWMP_READ|CWMP_WRITE,	&tQueueStatsEntityLeafOP},
{"OutputPackets",               eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"OutputBytes",                 eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"DroppedPackets",              eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"DroppedBytes",                eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"QueueOccupancyPackets",       eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
{"QueueOccupancyPercentage",	eCWMP_tUINT,	CWMP_READ,	&tQueueStatsEntityLeafOP},
};

enum eQueueStatsEntityLeaf
{
	eQC_Enable,
	eQC_Status,
	eQC_QueueStatsAlias,
	eQC_Queue,
	eQC_Interface,
	eQC_OutputPackets,
	eQC_OutputBytes,
	eQC_DroppedPackets,
	eQC_DroppedBytes,
	eQC_QueueOccupancyPackets,
	eQC_QueueOccupancyPercentage
};

struct CWMP_LEAF tQueueStatsEntityLeaf[] =
{
{ &tQueueStatsEntityLeafInfo[eQC_Enable]                   },
{ &tQueueStatsEntityLeafInfo[eQC_Status]                   },
{ &tQueueStatsEntityLeafInfo[eQC_QueueStatsAlias]          },
{ &tQueueStatsEntityLeafInfo[eQC_Queue]                    },
{ &tQueueStatsEntityLeafInfo[eQC_Interface]                },
{ &tQueueStatsEntityLeafInfo[eQC_OutputPackets]            },
{ &tQueueStatsEntityLeafInfo[eQC_OutputBytes]              },
{ &tQueueStatsEntityLeafInfo[eQC_DroppedPackets]           },
{ &tQueueStatsEntityLeafInfo[eQC_DroppedBytes]             },
{ &tQueueStatsEntityLeafInfo[eQC_QueueOccupancyPackets]    },
{ &tQueueStatsEntityLeafInfo[eQC_QueueOccupancyPercentage] },
{ NULL }
};
#endif
/******************************************************************************/

/******************************************************************************/
struct CWMP_PRMT tClassObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum eClassObject
{
	eClassObject0
};

struct CWMP_LINKNODE tClassObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tClassObjectInfo[eClassObject0],	tClassEntityLeaf,	NULL,		NULL,			0},
};
/******************************************************************************/

/******************************************************************************/
#if 0
struct CWMP_PRMT tAppObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum eAppObject
{
	eAppObject0
};

struct CWMP_LINKNODE tAppObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tAppObjectInfo[eAppObject0],	tAppEntityLeaf,	NULL,		NULL,			0},
};
/******************************************************************************/

/******************************************************************************/
struct CWMP_PRMT tFlowObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum eFlowObject
{
	eFlowObject0
};

struct CWMP_LINKNODE tFlowObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tFlowObjectInfo[eFlowObject0],	tFlowEntityLeaf,	NULL,		NULL,			0},
};
/******************************************************************************/

/******************************************************************************/
struct CWMP_PRMT tPolicerObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum ePolicerObject
{
	ePolicerObject0
};

struct CWMP_LINKNODE tPolicerObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tPolicerObjectInfo[ePolicerObject0],	tPolicerEntityLeaf,	NULL,		NULL,			0},
};
#endif
/******************************************************************************/

/******************************************************************************/
struct CWMP_PRMT tQueueObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum eQueueObject
{
	eQueueObject0
};

struct CWMP_LINKNODE tQueueObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tQueueObjectInfo[eQueueObject0],	tQueueEntityLeaf,	NULL,		NULL,			0},
};
/******************************************************************************/

/******************************************************************************/
#if 0
struct CWMP_PRMT tQueueStatsObjectInfo[] =
{
/*(name,			type,		flag,					op)*/
{"0",				eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};

enum eQueueStatsObject
{
	eQueueStatsObject0
};

struct CWMP_LINKNODE tQueueStatsObject[] =
{
/*info,  				leaf,			next,		sibling,		instnum)*/
{&tQueueStatsObjectInfo[eQueueStatsObject0],	tQueueStatsEntityLeaf,	NULL,		NULL,			0},
};
#endif
/******************************************************************************/

/******************************************************************************/
struct CWMP_OP tQM_Class_OP 		= { NULL, objClass };
struct CWMP_OP tQM_Queue_OP 		= { NULL, objQueue };

#if 0
struct CWMP_OP tQM_App_OP 			= { NULL, objApp };
struct CWMP_OP tQM_Flow_OP 			= { NULL, objFlow };
struct CWMP_OP tQM_Policer_OP 		= { NULL, objPolicer };
struct CWMP_OP tQM_QueueStats_OP 	= { NULL, objQueueStats };
#endif

struct CWMP_PRMT tQueueMntObjectInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Classification",	eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Class_OP},
{"Queue",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Queue_OP}

#if 0
, {"App",				eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_App_OP},
{"Flow",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Flow_OP},
{"Policer",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_Policer_OP},
{"QueueStats",		eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tQM_QueueStats_OP}
#endif
};
enum tQueueMntObject
{
	eQ_Classification,
	eQ_Queue
#if 0
	, eQ_App,
	eQ_Flow,
	eQ_Policer,
	eQ_QueueStats
#endif
};
struct CWMP_NODE tQueueMntObject[] =
{
/*info,  					leaf,			node)*/
{&tQueueMntObjectInfo[eQ_Classification],	NULL,			NULL},
{&tQueueMntObjectInfo[eQ_Queue],			NULL,			NULL},
#if 0
{&tQueueMntObjectInfo[eQ_App],				NULL,			NULL},
{&tQueueMntObjectInfo[eQ_Flow],				NULL,			NULL},
{&tQueueMntObjectInfo[eQ_Policer],			NULL,			NULL},
{&tQueueMntObjectInfo[eQ_QueueStats],		NULL,			NULL},
#endif
{NULL,						NULL,			NULL}
};
/******************************************************************************/

/******************************************************************************/
struct CWMP_OP tQueueMntLeafOP = { getQueueMnt,	setQueueMnt };

struct CWMP_PRMT tQueueMntLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",							eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"MaxQueues",						eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"MaxClassificationEntries",		eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"ClassificationNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"MaxQueueEntries", 				eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"QueueNumberOfEntries",			eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"DefaultQueue",					eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
#if 0
{"DefaultDSCPMark", 				eCWMP_tINT, 	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"DefaultEthernetPriorityMark", 	eCWMP_tINT, 	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP}, 
{"MaxAppEntries",					eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"AppNumberOfEntries",				eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"MaxFlowEntries",					eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"FlowNumberOfEntries",				eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"MaxPolicerEntries",				eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"PolicerNumberOfEntries",			eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"QueueStatsNumberOfEntries",		eCWMP_tUINT,	CWMP_READ,				&tQueueMntLeafOP},
{"DefaultForwardingPolicy",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"DefaultTrafficClass",				eCWMP_tINT,		CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"DefaultPolicer",					eCWMP_tINT,		CWMP_WRITE|CWMP_READ,	&tQueueMntLeafOP},
{"AvailableAppList",				eCWMP_tSTRING,	CWMP_READ,				&tQueueMntLeafOP}
#endif
};
enum eQueueMntLeaf
{
	eQ_Enable,
	eQ_MaxQueues,
	eQ_MaxClassificationEntries,
	eQ_ClassificationNumberOfEntries,
	eQ_MaxQueueEntries,
	eQ_QueueNumberOfEntries,
	eQ_DefaultQueue,
#if 0
	,eQ_DefaultDSCPMark,
	eQ_DefaultEthernetPriorityMark, 
	eQ_MaxAppEntries,
	eQ_AppNumberOfEntries,
	eQ_MaxFlowEntries,
	eQ_FlowNumberOfEntries,
	eQ_MaxPolicerEntries,
	eQ_PolicerNumberOfEntries,
	eQ_QueueStatsNumberOfEntries,
	eQ_DefaultForwardingPolicy,
	eQ_DefaultTrafficClass,
	eQ_DefaultPolicer,
	eQ_AvailableAppList
#endif
};
struct CWMP_LEAF tQueueMntLeaf[] =
{
{ &tQueueMntLeafInfo[eQ_Enable] },
{ &tQueueMntLeafInfo[eQ_MaxQueues] },
{ &tQueueMntLeafInfo[eQ_MaxClassificationEntries] },
{ &tQueueMntLeafInfo[eQ_ClassificationNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxQueueEntries] },
{ &tQueueMntLeafInfo[eQ_QueueNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_DefaultQueue] },
#if 0
{ &tQueueMntLeafInfo[eQ_DefaultDSCPMark] },
{ &tQueueMntLeafInfo[eQ_DefaultEthernetPriorityMark] },
{ &tQueueMntLeafInfo[eQ_MaxAppEntries] },
{ &tQueueMntLeafInfo[eQ_AppNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxFlowEntries] },
{ &tQueueMntLeafInfo[eQ_FlowNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_MaxPolicerEntries] },
{ &tQueueMntLeafInfo[eQ_PolicerNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_QueueStatsNumberOfEntries] },
{ &tQueueMntLeafInfo[eQ_DefaultForwardingPolicy] },
{ &tQueueMntLeafInfo[eQ_DefaultTrafficClass] },
{ &tQueueMntLeafInfo[eQ_DefaultPolicer] },
{ &tQueueMntLeafInfo[eQ_AvailableAppList] },
#endif
{ NULL }
};
/******************************************************************************/

unsigned int getQosClassInstNum(char *name)
{
	return getInstNum(name, "Classification");
}

unsigned int getQosAppInstNum(char *name)
{
	return getInstNum(name, "App");
}

unsigned int getQosFlowInstNum(char *name)
{
	return getInstNum(name, "Flow");
}

unsigned int getQosPolicerInstNum(char *name)
{
	return getInstNum(name, "Policer");
}

unsigned int getQosQueueInstNum(char *name)
{
	return getInstNum(name, "Queue");
}

unsigned int getQosQueueStatsInstNum(char *name)
{
	return getInstNum(name, "QueueStats");
}

int getQosClassEntryByInstNum(unsigned int instNum, QOSCLASS_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_QOS_CLASS_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_QOS_CLASS_TBL, (void *)entry))
			continue;

		if (entry->ClassInstanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQosAppEntryByInstNum(unsigned int instNum, TR098_APPCONF_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_TR098_QOS_APP_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_TR098_QOS_APP_TBL, (void *)entry))
			continue;

		if (entry->instanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQosFlowEntryByInstNum(unsigned int instNum, TR098_FLOWCONF_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_TR098_QOS_FLOW_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_TR098_QOS_FLOW_TBL, (void *)entry))
			continue;

		if (entry->instanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQosPolicerEntryByInstNum(unsigned int instNum, QOSPOLICER_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_QOS_POLICER_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_QOS_POLICER_TBL, (void *)entry))
			continue;

		if (entry->InstanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQosQueueEntryByInstNum(unsigned int instNum, QOSQUEUE_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_QOS_QUEUE_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_QOS_QUEUE_TBL, (void *)entry))
			continue;

		if (entry->QueueInstanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQosQueueStatsEntryByInstNum(unsigned int instNum, QOSQUEUESTATS_T *entry, unsigned int *id)
{
	int ret = -1;
	unsigned int i, tblNum;

	if ((instNum==0) || (entry==NULL) || (id==NULL))
		return ret;

	mib_get(MIB_QOS_QUEUESTATS_TBL_NUM, (void *)&tblNum);
	
	for (i=1; i<=tblNum; i++)
	{
		*((char *)entry) = (char)i;
		
		if (!mib_get(MIB_QOS_QUEUESTATS_TBL, (void *)entry))
			continue;

		if (entry->InstanceNum == instNum)
		{
			*id = i;
			ret = 0;
			break;
		}
	}

	return ret;
}

int getQueueMnt(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char 	buff[1025]={0};
	unsigned char vChar=0;
	unsigned int num=0;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
		int tmp;
		
		mib_get(MIB_QOS_ENABLE, (void *)&tmp);
		
		*data = booldup( tmp!=0 );
	}
	else if( strcmp( lastname, "MaxQueues" )==0 )
	{
		*data = uintdup(MAX_QOS_QUEUE_NUM);
	}
	else if( strcmp( lastname, "QueueNumberOfEntries" )==0 )
	{
		 mib_get(MIB_QOS_QUEUE_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}
	else if( strcmp( lastname, "MaxQueueEntries" )==0 )
	{
		 mib_get(MIB_QOS_QUEUE_TBL_NUM, (void *)&num);
		
		*data = uintdup(MAX_QOS_QUEUE_NUM - num);
	}
	else if( strcmp( lastname, "ClassificationNumberOfEntries" )==0 )
	{
		 mib_get(MIB_QOS_CLASS_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}
	else if( strcmp( lastname, "MaxClassificationEntries" )==0 )
	{
		mib_get(MIB_QOS_CLASS_TBL_NUM, (void *)&num);
		
		*data = uintdup(MAX_QOS_CLASS_NUM - num);
	}else if( strcmp( lastname, "MaxAppEntries" )==0 )
	{
		 *data = uintdup(MAX_TR098_APP_TBL_NUM);
	}else if( strcmp( lastname, "AppNumberOfEntries" )==0 )
	{
		 mib_get(MIB_TR098_QOS_APP_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "MaxFlowEntries" )==0 )
	{
		 *data = uintdup(MAX_TR098_FLOW_TBL_NUM);
	}else if( strcmp( lastname, "FlowNumberOfEntries" )==0 )
	{
		 mib_get(MIB_TR098_QOS_FLOW_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "MaxPolicerEntries" )==0 )
	{
		 *data = uintdup(MAX_QOS_POLICER_NUM);
	}else if( strcmp( lastname, "PolicerNumberOfEntries" )==0 )
	{
		 mib_get(MIB_QOS_POLICER_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "QueueStatsNumberOfEntries" )==0 )
	{
		 mib_get(MIB_QOS_QUEUESTATS_TBL_NUM, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "DefaultForwardingPolicy" )==0 )
	{
		 mib_get(MIB_QOS_DEF_FORWORDING_POLICY, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "DefaultTrafficClass" )==0 )
	{
		int defTrafClass;

		mib_get(MIB_QOS_DEF_TRAFFIC_CLASS, (void *)&defTrafClass);
		
		*data = intdup(defTrafClass);
	}else if( strcmp( lastname, "DefaultPolicer" )==0 )
	{
		int defPolicer;

		mib_get(MIB_QOS_DEF_POLICER, (void *)&defPolicer);
		
		*data = intdup(defPolicer);
	}else if( strcmp( lastname, "DefaultQueue" )==0 )
	{
		 mib_get(MIB_QOS_DEF_QUEUE, (void *)&num);
		
		*data = uintdup(num);
	}else if( strcmp( lastname, "DefaultDSCPMark" )==0 )
	{
		int dscpMark;

		mib_get(MIB_QOS_DEF_DSCP_MARK, (void *)&dscpMark);
		
		*data = intdup(dscpMark);
	}else if( strcmp( lastname, "DefaultEthernetPriorityMark" )==0 )
	{
		int ethPrioMark;

		mib_get(MIB_QOS_DEF_ETHERNET_PRIO_MARK, (void *)&ethPrioMark);
		
		*data = intdup(ethPrioMark);
	}else if( strcmp( lastname, "AvailableAppList" )==0 )
	{
		mib_get(MIB_QOS_AVAIL_APP_LIST, (void *)buff);
		
		*data = strdup(buff);
	}else{
		return ERR_9005;
	}
	
	return 0;
}

int setQueueMnt(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastname = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	
	if ((name==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	if (entity->info->type != type)
		return ERR_9006;

	if( strcmp( lastname, "Enable" )==0 )
	{
		int *pEnable = data;
		int enable;

		if (pEnable == NULL)
			return ERR_9007;

		if (*pEnable == 0)
			enable = 0;
		else
			enable = 1;
		
		mib_set(MIB_QOS_ENABLE, (void *)&enable);

		//system("sysconf setQos");
	}else if( strcmp( lastname, "DefaultForwardingPolicy" )==0 )
	{
		unsigned int *pId = data;
		unsigned int id;

		if (!pId)
			return ERR_9007;

		id = *pId;

		mib_set(MIB_QOS_DEF_FORWORDING_POLICY, (void *)&id);
	}else if( strcmp( lastname, "DefaultTrafficClass" )==0 )
	{
		int *pId = data;
		int id;

		if (!pId || *pId < -1)
			return ERR_9007;

		id = *pId;

		mib_set(MIB_QOS_DEF_TRAFFIC_CLASS, (void *)&id);
	}else if( strcmp( lastname, "DefaultPolicer" )==0 )
	{
		int *pInstNum = data;
		int instNum;

		if (!pInstNum || *pInstNum < -1)
			return ERR_9007;

		instNum = *pInstNum;

		mib_set(MIB_QOS_DEF_POLICER, (void *)&instNum);
	}else if( strcmp( lastname, "DefaultQueue" )==0 )
	{
		unsigned int *pInstNum = data;
		unsigned int instNum;

		if (!pInstNum || *pInstNum < 1)
			return ERR_9007;

		instNum = *pInstNum;

		mib_set(MIB_QOS_DEF_QUEUE, (void *)&instNum);
	}else if( strcmp( lastname, "DefaultDSCPMark" )==0 )
	{
		int *pDscpMark = data;
		int dscpMark;

		if (!pDscpMark || *pDscpMark < -2)
			return ERR_9007;

		dscpMark = *pDscpMark;

		mib_set(MIB_QOS_DEF_DSCP_MARK, (void *)&dscpMark);
	}else if( strcmp( lastname, "DefaultEthernetPriorityMark" )==0 )
	{
		int *pEthPrioMark = data;
		int ethPrioMark = *pEthPrioMark;

		if (!pEthPrioMark || *pEthPrioMark < -2)
			return ERR_9007;

		ethPrioMark = *pEthPrioMark;

		mib_set(MIB_QOS_DEF_ETHERNET_PRIO_MARK, (void *)&ethPrioMark);
	}else{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int getClassEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;
	unsigned int objNum = 0;
	unsigned int id;

	QOSCLASS_T *pQosClass, qosClass;

	//tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosClassInstNum(name);
	if (objNum == 0)
		return ERR_9005;

	pQosClass = &qosClass;
	if (getQosClassEntryByInstNum(objNum, pQosClass, &id) < 0)
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;


	if(strcmp(lastname, "ClassificationKey")                  == 0)
	{
		*data = uintdup(pQosClass->ClassInstanceNum);
	}
	else if (strcmp(lastname, "ClassificationEnable")               == 0)
	{
		*data = booldup(pQosClass->ClassificationEnable != 0);
	}
	else if (strcmp(lastname, "ClassificationStatus")               == 0)
	{
		if( pQosClass->ClassificationEnable != 0 )
		 	*data=strdup( "Enabled" );
		else
			*data=strdup( "Disabled" );
		
	}
	else if (strcmp(lastname, "Alias")                              == 0)
	{
		*data = strdup(pQosClass->Alias);
	}
	else if (strcmp(lastname, "ClassificationOrder")                == 0)
	{
		*data = uintdup(pQosClass->ClassificationOrder);
	}
	else if (strcmp(lastname, "ClassInterface")                     == 0)
	{
		*data = strdup(pQosClass->ClassInterface);
	}
	else if (strcmp(lastname, "DestIP")                             == 0)
	{
		sprintf(buff, "%s", inet_ntoa(*((struct in_addr *)&pQosClass->DestIP)));
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "DestMask")                           == 0)
	{
		sprintf(buff, "%s", inet_ntoa(*((struct in_addr *)&pQosClass->DestMask)));
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "DestIPExclude")                      == 0)
	{
		*data = booldup(pQosClass->DestIPExclude != 0);
	}
	else if (strcmp(lastname, "SourceIP")                           == 0)
	{
		sprintf(buff, "%s", inet_ntoa(*((struct in_addr *)&pQosClass->SourceIP)));
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "SourceMask")                         == 0)
	{
		sprintf(buff, "%s", inet_ntoa(*((struct in_addr *)&pQosClass->SourceMask)));
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "SourceIPExclude")                    == 0)
	{
		*data = booldup(pQosClass->SourceIPExclude != 0);
	}
	else if (strcmp(lastname, "Protocol")                           == 0)
	{
		*data = intdup(pQosClass->Protocol);
	}
	else if (strcmp(lastname, "ProtocolExclude")                    == 0)
	{
		*data = booldup(pQosClass->ProtocolExclude != 0);
	}
	else if (strcmp(lastname, "DestPort")                           == 0)
	{
		*data = intdup(pQosClass->DestPort);
	}
	else if (strcmp(lastname, "DestPortRangeMax")                   == 0)
	{
		*data = intdup(pQosClass->DestPortRangeMax);
	}
	else if (strcmp(lastname, "DestPortExclude")                    == 0)
	{
		*data = booldup(pQosClass->DestPortExclude != 0);
	}
	else if (strcmp(lastname, "SourcePort")                         == 0)
	{
		*data = intdup(pQosClass->SourcePort);
	}
	else if (strcmp(lastname, "SourcePortRangeMax")                 == 0)
	{
		*data = intdup(pQosClass->SourcePortRangeMax);
	}
	else if (strcmp(lastname, "SourcePortExclude")                  == 0)
	{
		*data = booldup(pQosClass->SourcePortExclude != 0);
	}
	else if (strcmp(lastname, "SourceMACAddress")                   == 0)
	{
		sprintf(buff,"%02x:%02x:%02x:%02x:%02x:%02x", 
			pQosClass->SourceMACAddress[0], pQosClass->SourceMACAddress[1], 
			pQosClass->SourceMACAddress[2], pQosClass->SourceMACAddress[3], 
			pQosClass->SourceMACAddress[4], pQosClass->SourceMACAddress[5]);
		
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "SourceMACMask")                      == 0)
	{
		sprintf(buff,"%02x:%02x:%02x:%02x:%02x:%02x", 
			pQosClass->SourceMACMask[0], pQosClass->SourceMACMask[1], 
			pQosClass->SourceMACMask[2], pQosClass->SourceMACMask[3], 
			pQosClass->SourceMACMask[4], pQosClass->SourceMACMask[5]);
		
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "SourceMACExclude")                   == 0)
	{
		*data = booldup(pQosClass->SourceMACExclude != 0);
	}
	else if (strcmp(lastname, "DestMACAddress")                     == 0)
	{
		sprintf(buff,"%02x:%02x:%02x:%02x:%02x:%02x", 
			pQosClass->DestMACAddress[0], pQosClass->DestMACAddress[1], 
			pQosClass->DestMACAddress[2], pQosClass->DestMACAddress[3], 
			pQosClass->DestMACAddress[4], pQosClass->DestMACAddress[5]);
		
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "DestMACMask")                        == 0)
	{
		sprintf(buff,"%02x:%02x:%02x:%02x:%02x:%02x", 
			pQosClass->DestMACMask[0], pQosClass->DestMACMask[1], 
			pQosClass->DestMACMask[2], pQosClass->DestMACMask[3], 
			pQosClass->DestMACMask[4], pQosClass->DestMACMask[5]);
		
		*data = strdup(buff);
	}
	else if (strcmp(lastname, "DestMACExclude")                     == 0)
	{
		*data = booldup(pQosClass->DestMACExclude != 0);
	}
	else if (strcmp(lastname, "Ethertype")                          == 0)
	{
		*data = intdup(pQosClass->Ethertype);
	}
	else if (strcmp(lastname, "EthertypeExclude")                   == 0)
	{
		*data = booldup(pQosClass->EthertypeExclude != 0);
	}
	else if (strcmp(lastname, "SSAP")                               == 0)
	{
		*data = intdup(pQosClass->SSAP);
	}
	else if (strcmp(lastname, "SSAPExclude")                        == 0)
	{
		*data = booldup(pQosClass->SSAPExclude != 0);
	}
	else if (strcmp(lastname, "DSAP")                               == 0)
	{
		*data = intdup(pQosClass->DSAP);
	}
	else if (strcmp(lastname, "DSAPExclude")                        == 0)
	{
		*data = booldup(pQosClass->DSAPExclude != 0);
	}
	else if (strcmp(lastname, "LLCControl")                         == 0)
	{
		*data = intdup(pQosClass->LLCControl);
	}
	else if (strcmp(lastname, "LLCControlExclude")                  == 0)
	{
		*data = booldup(pQosClass->LLCControlExclude != 0);
	}
	else if (strcmp(lastname, "SNAPOUI")                            == 0)
	{
		*data = intdup(pQosClass->SNAPOUI);
	}
	else if (strcmp(lastname, "SNAPOUIExclude")                     == 0)
	{
		*data = booldup(pQosClass->SNAPOUIExclude != 0);
	}
	else if (strcmp(lastname, "SourceVendorClassID")                == 0)
	{
		*data = strdup(pQosClass->SourceVendorClassID);
	}
	else if (strcmp(lastname, "SourceVendorClassIDExclude")         == 0)
	{
		*data = booldup(pQosClass->SourceVendorClassIDExclude != 0);
	}
	else if (strcmp(lastname, "SourceVendorClassIDMode")            == 0)
	{
		*data = strdup(pQosClass->SourceVendorClassIDMode);
	}
	else if (strcmp(lastname, "DestVendorClassID")                  == 0)
	{
		*data = strdup(pQosClass->DestVendorClassID);
	}
	else if (strcmp(lastname, "DestVendorClassIDExclude")           == 0)
	{
		*data = booldup(pQosClass->DestVendorClassIDExclude != 0);
	}
	else if (strcmp(lastname, "DestVendorClassIDMode")              == 0)
	{
		*data = strdup(pQosClass->DestVendorClassIDMode);
	}
	else if (strcmp(lastname, "SourceClientID")                     == 0)
	{
		*data = strdup(pQosClass->SourceClientID);
	}
	else if (strcmp(lastname, "SourceClientIDExclude")              == 0)
	{
		*data = booldup(pQosClass->SourceClientIDExclude != 0);
	}
	else if (strcmp(lastname, "DestClientID")                       == 0)
	{
		*data = strdup(pQosClass->DestClientID);
	}
	else if (strcmp(lastname, "DestClientIDExclude")                == 0)
	{
		*data = booldup(pQosClass->DestClientIDExclude != 0);
	}
	else if (strcmp(lastname, "SourceUserClassID")                  == 0)
	{
		*data = strdup(pQosClass->SourceUserClassID);
	}
	else if (strcmp(lastname, "SourceUserClassIDExclude")           == 0)
	{
		*data = booldup(pQosClass->SourceUserClassIDExclude != 0);
	}
	else if (strcmp(lastname, "DestUserClassID")                    == 0)
	{
		*data = strdup(pQosClass->DestUserClassID);
	}
	else if (strcmp(lastname, "DestUserClassIDExclude")             == 0)
	{
		*data = booldup(pQosClass->DestUserClassIDExclude != 0);
	}
	else if (strcmp(lastname, "SourceVendorSpecificInfo")           == 0)
	{
		*data = strdup(pQosClass->SourceVendorSpecificInfo);
	}
	else if (strcmp(lastname, "SourceVendorSpecificInfoExclude")    == 0)
	{
		*data = booldup(pQosClass->SourceVendorSpecificInfoExclude != 0);
	}
	else if (strcmp(lastname, "SourceVendorSpecificInfoEnterprise") == 0)
	{
		*data = uintdup(pQosClass->SourceVendorSpecificInfoEnterprise);
	}
	else if (strcmp(lastname, "SourceVendorSpecificInfoSubOption")  == 0)
	{
		*data = intdup(pQosClass->SourceVendorSpecificInfoSubOption);
	}
	else if (strcmp(lastname, "SourceVendorSpecificInfoMode")       == 0)
	{
		*data = strdup(pQosClass->SourceVendorSpecificInfoMode);
	}
	else if (strcmp(lastname, "DestVendorSpecificInfo")             == 0)
	{
		*data = strdup(pQosClass->DestVendorSpecificInfo);
	}
	else if (strcmp(lastname, "DestVendorSpecificInfoExclude")      == 0)
	{
		*data = booldup(pQosClass->DestVendorSpecificInfoExclude != 0);
	}
	else if (strcmp(lastname, "DestVendorSpecificInfoEnterprise")   == 0)
	{
		*data = uintdup(pQosClass->DestVendorSpecificInfoEnterprise);
	}
	else if (strcmp(lastname, "DestVendorSpecificInfoSubOption")    == 0)
	{
		*data = intdup(pQosClass->DestVendorSpecificInfoSubOption);
	}
	else if (strcmp(lastname, "DestVendorSpecificInfoMode")         == 0)
	{
		*data = strdup(pQosClass->DestVendorSpecificInfoMode);
	}
	else if (strcmp(lastname, "TCPACK")                             == 0)
	{
		*data = booldup(pQosClass->TCPACK != 0);
	}
	else if (strcmp(lastname, "TCPACKExclude")                      == 0)
	{
		*data = booldup(pQosClass->TCPACKExclude != 0);
	}
	else if (strcmp(lastname, "IPLengthMin")                        == 0)
	{
		*data = uintdup(pQosClass->IPLengthMin);
	}
	else if (strcmp(lastname, "IPLengthMax")                        == 0)
	{
		*data = uintdup(pQosClass->IPLengthMax);
	}
	else if (strcmp(lastname, "IPLengthExclude")                    == 0)
	{
		*data = booldup(pQosClass->IPLengthExclude != 0);
	}
	else if (strcmp(lastname, "DSCPCheck")                          == 0)
	{
		*data = intdup(pQosClass->DSCPCheck);
	}
	else if (strcmp(lastname, "DSCPExclude")                        == 0)
	{
		*data = booldup(pQosClass->DSCPExclude != 0);
	}
	else if (strcmp(lastname, "DSCPMark")                           == 0)
	{
		*data = intdup(pQosClass->DSCPMark);
	}
	else if (strcmp(lastname, "EthernetPriorityCheck")              == 0)
	{
		*data = intdup(pQosClass->EthernetPriorityCheck);
	}
	else if (strcmp(lastname, "EthernetPriorityExclude")            == 0)
	{
		*data = booldup(pQosClass->EthernetPriorityExclude != 0);
	}
	else if (strcmp(lastname, "EthernetPriorityMark")               == 0)
	{
		*data = intdup(pQosClass->EthernetPriorityMark);
	}
	else if (strcmp(lastname, "VLANIDCheck")                        == 0)
	{
		*data = intdup(pQosClass->VLANIDCheck);
	}
	else if (strcmp(lastname, "VLANIDExclude")                      == 0)
	{
		*data = booldup(pQosClass->VLANIDExclude != 0);
	}
	else if (strcmp(lastname, "OutOfBandInfo")                      == 0)
	{
		*data = intdup(pQosClass->OutOfBandInfo);
	}
	else if (strcmp(lastname, "ForwardingPolicy")                   == 0)
	{
		*data = uintdup(pQosClass->ForwardingPolicy);
	}
	else if (strcmp(lastname, "TrafficClass")                       == 0)
	{
		*data = intdup(pQosClass->TrafficClass);
	}
	else if (strcmp(lastname, "ClassPolicer")                       == 0)
	{
		*data = intdup(pQosClass->ClassPolicer);
	}
	else if (strcmp(lastname, "ClassQueue")                         == 0)
	{
		*data = intdup(pQosClass->ClassQueue);
	}
	else if (strcmp(lastname, "ClassApp")                           == 0)
	{
		*data = intdup(pQosClass->ClassApp);
	}
	else
	{
		return ERR_9005;
	}

	return 0;
}

int setClassEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastName = entity->info->name;
	char *buf = data;
	char *pZeroIp = "0.0.0.0";
	int needReinit = 1;
	unsigned int objNum = 0, id;

	struct in_addr inAddr;

	QOSCLASS_T *pQosClass, qosClass;
	QOSCLASS_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosClassInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosClass = &qosClass;
	if (getQosClassEntryByInstNum(objNum, pQosClass, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(QOSCLASS_T));
	memset(&qosArray[1], 0, sizeof(QOSCLASS_T));
	memcpy(&qosArray[0], &qosClass, sizeof(QOSCLASS_T));

//============================================================================//
	if (strcmp(lastName, "ClassificationEnable") == 0)
	{
		int *bEn = data;

		pQosClass->ClassificationEnable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "Alias") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_ALIAS_LEN+1))
			strcpy(pQosClass->Alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "ClassificationOrder") == 0)
	{
		unsigned int *pOrder = data;

		if (*pOrder < 1)
			return ERR_9007;

		pQosClass->ClassificationOrder = *pOrder;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "ClassInterface") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->ClassInterface[0] = '\0';
		else if (len < MAX_CLASS_INTERFACE_LEN)
			strcpy(pQosClass->ClassInterface, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestIP") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strlen(buf) == 0)
			buf = pZeroIp;

		if(inet_aton(buf, &inAddr) == 0) // the ip address is error.
			return ERR_9007;

		memcpy(pQosClass->DestIP, &inAddr, sizeof(struct in_addr));
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestMask") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strlen(buf) == 0)
			buf = pZeroIp;

		if(inet_aton(buf, &inAddr) == 0) // the ip address is error.
			return ERR_9007;

		memcpy(pQosClass->DestMask, &inAddr, sizeof(struct in_addr));
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestIPExclude") == 0)
	{
		int *bEx = data;

		pQosClass->DestIPExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceIP") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strlen(buf) == 0)
			buf = pZeroIp;

		if(inet_aton(buf, &inAddr) == 0) // the ip address is error.
			return ERR_9007;

		memcpy(pQosClass->SourceIP, &inAddr, sizeof(struct in_addr));
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceMask") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strlen(buf) == 0)
			buf = pZeroIp;

		if(inet_aton(buf, &inAddr) == 0) // the ip address is error.
			return ERR_9007;

		memcpy(pQosClass->SourceMask, &inAddr, sizeof(struct in_addr));
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceIPExclude") == 0)
	{
		int *bEx = data;

		pQosClass->SourceIPExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "Protocol") == 0)
	{
		int *prot = data;

		if (*prot < -1)
			return ERR_9007;

		pQosClass->Protocol = *prot;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "ProtocolExclude") == 0)
	{
		int *bEx = data;

		pQosClass->ProtocolExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestPort") == 0)
	{
		int *dstPort = data;

		if (*dstPort < -1)
			return ERR_9007;

		pQosClass->DestPort = *dstPort;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestPortRangeMax") == 0)
	{
		int *dstPortMax = data;

		if (*dstPortMax < -1)
			return ERR_9007;

		if(*dstPortMax != -1 && (*dstPortMax < pQosClass->DestPort))
			return ERR_9007;

		pQosClass->DestPortRangeMax = *dstPortMax;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestPortExclude") == 0)
	{
		int *bEx = data;

		pQosClass->DestPortExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourcePort") == 0)
	{
		int *srcPort = data;

		if (*srcPort < -1)
			return ERR_9007;

		pQosClass->SourcePort = *srcPort;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourcePortRangeMax") == 0)
	{
		int *srcPortMax = data;

		if (*srcPortMax < -1)
			return ERR_9007;

		if(*srcPortMax != -1 && (*srcPortMax < pQosClass->SourcePort))
			return ERR_9007;

		pQosClass->SourcePortRangeMax = *srcPortMax;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourcePortExclude") == 0)
	{
		int *bEx = data;

		pQosClass->SourcePortExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceMACAddress") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
		{
			pQosClass->SourceMACAddress[0] = 0;
			pQosClass->SourceMACAddress[1] = 0;
			pQosClass->SourceMACAddress[2] = 0;
			pQosClass->SourceMACAddress[3] = 0;
			pQosClass->SourceMACAddress[4] = 0;
			pQosClass->SourceMACAddress[5] = 0;
		}
		else if (len <= 17) // AA:BB:CC:DD:EE:FF
		{
			char m0, m1, m2, m3, m4, m5;
			
			if (sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
			{
				pQosClass->SourceMACAddress[0] = (unsigned char) m0;
				pQosClass->SourceMACAddress[1] = (unsigned char) m1;
				pQosClass->SourceMACAddress[2] = (unsigned char) m2;
				pQosClass->SourceMACAddress[3] = (unsigned char) m3;
				pQosClass->SourceMACAddress[4] = (unsigned char) m4;
				pQosClass->SourceMACAddress[5] = (unsigned char) m5;
			}
			else
			{
				return ERR_9007;
			}
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceMACMask") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
		{
			pQosClass->SourceMACMask[0] = 0;
			pQosClass->SourceMACMask[1] = 0;
			pQosClass->SourceMACMask[2] = 0;
			pQosClass->SourceMACMask[3] = 0;
			pQosClass->SourceMACMask[4] = 0;
			pQosClass->SourceMACMask[5] = 0;
		}
		else if (len <= 17) // AA:BB:CC:DD:EE:FF
		{
			char m0, m1, m2, m3, m4, m5;
			
			if (sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
			{
				pQosClass->SourceMACMask[0] = (unsigned char) m0;
				pQosClass->SourceMACMask[1] = (unsigned char) m1;
				pQosClass->SourceMACMask[2] = (unsigned char) m2;
				pQosClass->SourceMACMask[3] = (unsigned char) m3;
				pQosClass->SourceMACMask[4] = (unsigned char) m4;
				pQosClass->SourceMACMask[5] = (unsigned char) m5;
			}
			else
			{
				return ERR_9007;
			}
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceMACExclude") == 0)
	{
		int *bEx = data;

		pQosClass->SourceMACExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "DestMACAddress") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
		{
			pQosClass->DestMACAddress[0] = 0;
			pQosClass->DestMACAddress[1] = 0;
			pQosClass->DestMACAddress[2] = 0;
			pQosClass->DestMACAddress[3] = 0;
			pQosClass->DestMACAddress[4] = 0;
			pQosClass->DestMACAddress[5] = 0;
		}
		else if (len <= 17) // AA:BB:CC:DD:EE:FF
		{
			char m0, m1, m2, m3, m4, m5;
			
			if (sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
			{
				pQosClass->DestMACAddress[0] = (unsigned char) m0;
				pQosClass->DestMACAddress[1] = (unsigned char) m1;
				pQosClass->DestMACAddress[2] = (unsigned char) m2;
				pQosClass->DestMACAddress[3] = (unsigned char) m3;
				pQosClass->DestMACAddress[4] = (unsigned char) m4;
				pQosClass->DestMACAddress[5] = (unsigned char) m5;
			}
			else
			{
				return ERR_9007;
			}
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestMACMask") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
		{
			pQosClass->DestMACMask[0] = 0;
			pQosClass->DestMACMask[1] = 0;
			pQosClass->DestMACMask[2] = 0;
			pQosClass->DestMACMask[3] = 0;
			pQosClass->DestMACMask[4] = 0;
			pQosClass->DestMACMask[5] = 0;
		}
		else if (len <= 17) // AA:BB:CC:DD:EE:FF
		{
			char m0, m1, m2, m3, m4, m5;
			
			if (sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &m0, &m1, &m2, &m3, &m4, &m5 ) == 6)
			{
				pQosClass->DestMACMask[0] = (unsigned char) m0;
				pQosClass->DestMACMask[1] = (unsigned char) m1;
				pQosClass->DestMACMask[2] = (unsigned char) m2;
				pQosClass->DestMACMask[3] = (unsigned char) m3;
				pQosClass->DestMACMask[4] = (unsigned char) m4;
				pQosClass->DestMACMask[5] = (unsigned char) m5;
			}
			else
			{
				return ERR_9007;
			}
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestMACExclude") == 0)
	{
		int *bEx = data;

		pQosClass->DestMACExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "Ethertype")                          == 0)
	{
		int *ethType = data;

		if (*ethType < -1)
			return ERR_9007;

		pQosClass->Ethertype = *ethType;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "EthertypeExclude")                   == 0)
	{
		int *bEx = data;

		pQosClass->EthertypeExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SSAP")                               == 0)
	{
		int *ssap = data;

		if (*ssap < -1)
			return ERR_9007;

		pQosClass->SSAP = *ssap;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SSAPExclude")                        == 0)
	{
		int *bEx = data;

		pQosClass->SSAPExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DSAP")                               == 0)
	{
		int *dsap = data;

		if (*dsap < -1)
			return ERR_9007;

		pQosClass->DSAP = *dsap;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DSAPExclude")                        == 0)
	{
		int *bEx = data;

		pQosClass->DSAPExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "LLCControl")                         == 0)
	{
		int *llcCtl = data;

		if (*llcCtl < -1)
			return ERR_9007;

		pQosClass->LLCControl = *llcCtl;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "LLCControlExclude")                  == 0)
	{
		int *bEx = data;

		pQosClass->LLCControlExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SNAPOUI")                            == 0)
	{
		int *snapOui = data;

		if (*snapOui < -1)
			return ERR_9007;

		pQosClass->SNAPOUI = *snapOui;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SNAPOUIExclude")                     == 0)
	{
		int *bEx = data;

		pQosClass->SNAPOUIExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorClassID")                == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->SourceVendorClassID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->SourceVendorClassID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorClassIDExclude")         == 0)
	{
		int *bEx = data;

		pQosClass->SourceVendorClassIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorClassIDMode")            == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "Exact") == 0 ||
			strcmp(buf, "Prefix") == 0 ||
			strcmp(buf, "Suffix") == 0 ||
			strcmp(buf, "Substring") == 0)
		{
			strcpy(pQosClass->SourceVendorClassIDMode, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorClassID")                  == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->DestVendorClassID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->DestVendorClassID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorClassIDExclude")           == 0)
	{
		int *bEx = data;

		pQosClass->DestVendorClassIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorClassIDMode")              == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "Exact") == 0 ||
			strcmp(buf, "Prefix") == 0 ||
			strcmp(buf, "Suffix") == 0 ||
			strcmp(buf, "Substring") == 0)
		{
			strcpy(pQosClass->DestVendorClassIDMode, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceClientID")                     == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->SourceClientID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->SourceClientID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceClientIDExclude")              == 0)
	{
		int *bEx = data;

		pQosClass->SourceClientIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestClientID")                       == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->DestClientID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->DestClientID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestClientIDExclude")                == 0)
	{
		int *bEx = data;

		pQosClass->DestClientIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceUserClassID")                  == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->SourceUserClassID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->SourceUserClassID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceUserClassIDExclude")           == 0)
	{
		int *bEx = data;

		pQosClass->SourceUserClassIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestUserClassID")                    == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->DestUserClassID[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->DestUserClassID, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestUserClassIDExclude")             == 0)
	{
		int *bEx = data;

		pQosClass->DestUserClassIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorSpecificInfo")           == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->SourceVendorSpecificInfo[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->SourceVendorSpecificInfo, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorSpecificInfoExclude")    == 0)
	{
		int *bEx = data;

		pQosClass->SourceVendorSpecificInfoExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorSpecificInfoEnterprise") == 0)
	{
		unsigned int *pEnterprise = data;

		printf("[%s:%d] *pEnterprise: %d\n", __FUNCTION__, __LINE__, *pEnterprise);
		
		if (*pEnterprise < 0)
			return ERR_9007;

		pQosClass->SourceVendorSpecificInfoEnterprise = *pEnterprise;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorSpecificInfoSubOption")  == 0)
	{
		int *subOpt = data;

		CHECK_PARAM_NUM(*subOpt, 0, 255)

		pQosClass->SourceVendorSpecificInfoSubOption = *subOpt;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "SourceVendorSpecificInfoMode")       == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "Exact") == 0 ||
			strcmp(buf, "Prefix") == 0 ||
			strcmp(buf, "Suffix") == 0 ||
			strcmp(buf, "Substring") == 0)
		{
			strcpy(pQosClass->SourceVendorSpecificInfoMode, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorSpecificInfo")             == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosClass->DestVendorSpecificInfo[0] = '\0';
		else if (len < 256)
			strcpy(pQosClass->DestVendorSpecificInfo, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorSpecificInfoExclude")      == 0)
	{
		int *bEx = data;

		pQosClass->DestVendorSpecificInfoExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorSpecificInfoEnterprise")   == 0)
	{
		unsigned int *pEnterprise = data;

		if (*pEnterprise < 0)
			return ERR_9007;

		pQosClass->DestVendorSpecificInfoEnterprise = *pEnterprise;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorSpecificInfoSubOption")    == 0)
	{
		int *subOpt = data;

		CHECK_PARAM_NUM(*subOpt, 0, 255)

		pQosClass->DestVendorSpecificInfoSubOption = *subOpt;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DestVendorSpecificInfoMode")         == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "Exact") == 0 ||
			strcmp(buf, "Prefix") == 0 ||
			strcmp(buf, "Suffix") == 0 ||
			strcmp(buf, "Substring") == 0)
		{
			strcpy(pQosClass->DestVendorSpecificInfoMode, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "TCPACK")                             == 0)
	{
		int *bEx = data;

		pQosClass->TCPACK = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "TCPACKExclude")                      == 0)
	{
		int *bEx = data;

		pQosClass->TCPACKExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "IPLengthMin")                        == 0)
	{
		unsigned int *pIpLenMin = data;

		if (*pIpLenMin < 0)
			return ERR_9007;

		pQosClass->IPLengthMin = *pIpLenMin;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "IPLengthMax")                        == 0)
	{
		unsigned int *pIpLenMax = data;

		if (*pIpLenMax < 0)
			return ERR_9007;

		pQosClass->IPLengthMax = *pIpLenMax;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "IPLengthExclude")                    == 0)
	{
		int *bEx = data;

		pQosClass->IPLengthExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DSCPCheck") == 0)
	{
		int *dscpChk = data;

		if (*dscpChk < -1)
			return ERR_9007;

		pQosClass->DSCPCheck = *dscpChk;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "DSCPExclude")                    == 0)
	{
		int *bEx = data;

		pQosClass->DSCPExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "DSCPMark") == 0)
	{
		int *dscpMark = data;

		if (*dscpMark < -2)
			return ERR_9007;

		pQosClass->DSCPMark = *dscpMark;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "EthernetPriorityCheck") == 0)
	{
		int *ethPrioChk = data;

		if (*ethPrioChk < -1)
			return ERR_9007;

		pQosClass->EthernetPriorityCheck = *ethPrioChk;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "EthernetPriorityExclude")                    == 0)
	{
		int *bEx = data;

		pQosClass->EthernetPriorityExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "EthernetPriorityMark") == 0)
	{
		int *ethPrioMark = data;

		if (*ethPrioMark < -2)
			return ERR_9007;

		pQosClass->EthernetPriorityMark = *ethPrioMark;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "VLANIDCheck") == 0)
	{
		int *vidChk = data;

		if (*vidChk < -1)
			return ERR_9007;

		pQosClass->VLANIDCheck = *vidChk;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "VLANIDExclude")                    == 0)
	{
		int *bEx = data;

		pQosClass->VLANIDExclude = (*bEx == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "OutOfBandInfo") == 0)
	{
		int *outOfBandInfo = data;

		if (*outOfBandInfo < -1)
			return ERR_9007;

		pQosClass->OutOfBandInfo = *outOfBandInfo;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "ForwardingPolicy")                        == 0)
	{
		unsigned int *pForwardPolicy = data;

		if (*pForwardPolicy < 0)
			return ERR_9007;

		pQosClass->ForwardingPolicy = *pForwardPolicy;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
	}
//============================================================================//
	else if (strcmp(lastName, "TrafficClass") == 0)
	{
		int *trafficClass = data;

		if (*trafficClass < -1)
			return ERR_9007;

		pQosClass->TrafficClass = *trafficClass;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "ClassPolicer") == 0)
	{
		int *classPolicer = data;

		if (*classPolicer < -1)
			return ERR_9007;

		pQosClass->ClassPolicer = *classPolicer;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "ClassQueue") == 0)
	{
		int *classQ = data;

		if (*classQ < -1)
			return ERR_9007;

		pQosClass->ClassQueue = *classQ;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else if (strcmp(lastName, "ClassApp") == 0)
	{
		int *classApp = data;

		if (*classApp < -1)
			return ERR_9007;

		pQosClass->ClassApp = *classApp;
		memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
		mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);

		
	}
//============================================================================//
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int getAppEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;
	unsigned int objNum = 0;
	unsigned int id;

	TR098_APPCONF_T *pQosApp, qosApp;

	//tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosAppInstNum(name);
	if (objNum == 0)
		return ERR_9005;

	pQosApp = &qosApp;
	if (getQosAppEntryByInstNum(objNum, pQosApp, &id) < 0)
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if      (strcmp(lastname, "AppKey"                         ) == 0 )
	{
		*data = uintdup(pQosApp->app_key);
	}
	else if (strcmp(lastname, "AppEnable"                      ) == 0 )
	{
		*data = booldup(pQosApp->app_enable != 0);
	}
	else if (strcmp(lastname, "AppStatus"                      ) == 0 )
	{
		*data = strdup(pQosApp->app_status);
	}
	else if (strcmp(lastname, "Alias"                          ) == 0 )
	{
		*data = strdup(pQosApp->app_alias);
	}
	else if (strcmp(lastname, "ProtocolIdentifier"             ) == 0 )
	{
		*data = strdup(pQosApp->protocol_identify);
	}
	else if (strcmp(lastname, "AppName"                        ) == 0 )
	{
		*data = strdup(pQosApp->app_name);
	}
	else if (strcmp(lastname, "AppDefaultForwardingPolicy"     ) == 0 )
	{
		*data = uintdup(pQosApp->default_policy);
	}
	else if (strcmp(lastname, "AppDefaultTrafficClass"         ) == 0 )
	{
		*data = intdup(pQosApp->default_class);
	}
	else if (strcmp(lastname, "AppDefaultPolicer"              ) == 0 )
	{
		*data = intdup(pQosApp->default_policer);
	}
	else if (strcmp(lastname, "AppDefaultQueue"                ) == 0 )
	{
		*data = intdup(pQosApp->default_queue);
	}
	else if (strcmp(lastname, "AppDefaultDSCPMark"             ) == 0 )
	{
		*data = intdup(pQosApp->default_dscp_mark);
	}
	else if (strcmp(lastname, "AppDefaultEthernetPriorityMark" ) == 0 )
	{
		*data = intdup(pQosApp->default_8021p_mark);
	}
	else
	{
		return ERR_9005;
	}

	return 0;
}

int setAppEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastName = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	unsigned int objNum = 0, id;

	TR098_APPCONF_T *pQosApp, qosApp;
	TR098_APPCONF_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosAppInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosApp = &qosApp;
	if (getQosAppEntryByInstNum(objNum, pQosApp, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(TR098_APPCONF_T));
	memset(&qosArray[1], 0, sizeof(TR098_APPCONF_T));
	memcpy(&qosArray[0], &qosApp, sizeof(TR098_APPCONF_T));

	if (strcmp(lastName, "AppEnable") == 0)
	{
		int *bEn = data;

		pQosApp->app_enable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "Alias") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_TR098_NAME_LEN+1))
			strcpy(pQosApp->app_alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "ProtocolIdentifier") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_IDENTIFY_LEN))
			strcpy(pQosApp->protocol_identify, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppName") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_TR098_NAME_LEN+1))
			strcpy(pQosApp->app_name, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultForwardingPolicy") == 0)
	{
		unsigned int *pDefFrdPolicy = data;

		pQosApp->default_policy = *pDefFrdPolicy;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultTrafficClass") == 0)
	{
		int *pDefTrafCls = data;

		if (*pDefTrafCls < -1)
			return ERR_9007;

		pQosApp->default_class = *pDefTrafCls;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultPolicer") == 0)
	{
		int *pDefPolicer = data;

		if (*pDefPolicer < -1)
			return ERR_9007;

		pQosApp->default_policer = *pDefPolicer;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultQueue") == 0)
	{
		int *pDefQ = data;

		if (*pDefQ < -1)
			return ERR_9007;

		pQosApp->default_queue = *pDefQ;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultDSCPMark") == 0)
	{
		int *pDefDscpMark = data;

		if (*pDefDscpMark < -2)
			return ERR_9007;

		pQosApp->default_dscp_mark = *pDefDscpMark;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppDefaultEthernetPriorityMark") == 0)
	{
		int *pDefEthPrioMark = data;

		if (*pDefEthPrioMark < -2)
			return ERR_9007;

		pQosApp->default_8021p_mark = *pDefEthPrioMark;
		memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
		mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
	}
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int getFlowEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;
	unsigned int objNum = 0;
	unsigned int id;

	TR098_FLOWCONF_T *pQosFlow, qosFlow;

	//tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosFlowInstNum(name);
	if (objNum == 0)
		return ERR_9005;

	pQosFlow = &qosFlow;
	if (getQosFlowEntryByInstNum(objNum, pQosFlow, &id) < 0)
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if      (strcmp(lastname, "FlowKey"                  ) == 0 )
	{
		*data = uintdup(pQosFlow->flow_key);
	}
	else if (strcmp(lastname, "FlowEnable"               ) == 0 )
	{
		*data = booldup(pQosFlow->flow_enable != 0);
	}
	else if (strcmp(lastname, "FlowStatus"               ) == 0 )
	{
		*data = strdup(pQosFlow->flow_status);
	}
	else if (strcmp(lastname, "Alias"                    ) == 0 )
	{
		*data = strdup(pQosFlow->flow_alias);
	}
	else if (strcmp(lastname, "FlowType"                 ) == 0 )
	{
		*data = strdup(pQosFlow->flow_type);
	}
	else if (strcmp(lastname, "FlowTypeParameters"       ) == 0 )
	{
		*data = strdup(pQosFlow->flow_type_para);
	}
	else if (strcmp(lastname, "FlowName"                 ) == 0 )
	{
		*data = strdup(pQosFlow->flow_name);
	}
	else if (strcmp(lastname, "AppIdentifier"            ) == 0 )
	{
		*data = intdup(pQosFlow->app_identify);
	}
	else if (strcmp(lastname, "FlowForwardingPolicy"     ) == 0 )
	{
		*data = uintdup(pQosFlow->qos_policy);
	}
	else if (strcmp(lastname, "FlowTrafficClass"         ) == 0 )
	{
		*data = intdup(pQosFlow->flow_class);
	}
	else if (strcmp(lastname, "FlowPolicer"              ) == 0 )
	{
		*data = intdup(pQosFlow->flow_policer);
	}
	else if (strcmp(lastname, "FlowQueue"                ) == 0 )
	{
		*data = intdup(pQosFlow->flow_queue);
	}
	else if (strcmp(lastname, "FlowDSCPMark"             ) == 0 )
	{
		*data = intdup(pQosFlow->flow_dscp_mark);
	}
	else if (strcmp(lastname, "FlowEthernetPriorityMark" ) == 0 )
	{
		*data = intdup(pQosFlow->flow_8021p_mark);
	}
	else
	{
		return ERR_9005;
	}

	return 0;
}

int setFlowEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastName = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	unsigned int objNum = 0, id;

	TR098_FLOWCONF_T *pQosFlow, qosFlow;
	TR098_FLOWCONF_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosFlowInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosFlow = &qosFlow;
	if (getQosFlowEntryByInstNum(objNum, pQosFlow, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(TR098_FLOWCONF_T));
	memset(&qosArray[1], 0, sizeof(TR098_FLOWCONF_T));
	memcpy(&qosArray[0], &qosFlow, sizeof(TR098_FLOWCONF_T));

	if (strcmp(lastName, "FlowEnable") == 0)
	{
		int *bEn = data;

		pQosFlow->flow_enable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "Alias") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_TR098_NAME_LEN+1))
			strcpy(pQosFlow->flow_alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowType") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_IDENTIFY_LEN+1))
			strcpy(pQosFlow->flow_type, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowTypeParameters") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_IDENTIFY_LEN+1))
			strcpy(pQosFlow->flow_type_para, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowName") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_TR098_NAME_LEN+1))
			strcpy(pQosFlow->flow_name, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "AppIdentifier") == 0)
	{
		int *pAppId = data;

		if (*pAppId < -1)
			return ERR_9007;

		pQosFlow->app_identify = *pAppId;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowForwardingPolicy") == 0)
	{
		unsigned int *pFlowFwdPolicy = data;

		pQosFlow->qos_policy = *pFlowFwdPolicy;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowTrafficClass") == 0)
	{
		int *pTrafCls = data;

		if (*pTrafCls < -1)
			return ERR_9007;

		pQosFlow->flow_class = *pTrafCls;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowPolicer") == 0)
	{
		int *pPolicer = data;

		if (*pPolicer < -1)
			return ERR_9007;

		pQosFlow->flow_policer = *pPolicer;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowQueue") == 0)
	{
		int *pQueue = data;

		if (*pQueue < -1)
			return ERR_9007;

		pQosFlow->flow_queue = *pQueue;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowDSCPMark") == 0)
	{
		int *pDscpMark = data;

		if (*pDscpMark < -2)
			return ERR_9007;

		pQosFlow->flow_dscp_mark = *pDscpMark;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "FlowEthernetPriorityMark") == 0)
	{
		int *pEthPrioMark = data;

		if (*pEthPrioMark < -2)
			return ERR_9007;

		pQosFlow->flow_8021p_mark = *pEthPrioMark;
		memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
		mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
	}
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int getPolicerEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;
	unsigned int objNum = 0;
	unsigned int id;

	QOSPOLICER_T *pQosPolicer, qosPolicer;

	//tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosPolicerInstNum(name);
	if (objNum == 0)
		return ERR_9005;

	pQosPolicer = &qosPolicer;
	if (getQosPolicerEntryByInstNum(objNum, pQosPolicer, &id) < 0)
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if      (strcmp(lastname, "PolicerKey"                        ) == 0 )
	{
		*data = uintdup(pQosPolicer->PolicerKey);
	}
	else if (strcmp(lastname, "PolicerEnable"                     ) == 0 )
	{
		*data = booldup(pQosPolicer->PolicerEnable != 0);
	}
#if 0
	else if (strcmp(lastname, "PolicerStatus"                     ) == 0 )
	{
		*data = uintdup(0);
	}
#endif
	else if (strcmp(lastname, "Alias"                             ) == 0 )
	{
		*data = strdup(pQosPolicer->Alias);
	}
	else if (strcmp(lastname, "CommittedRate"                     ) == 0 )
	{
		*data = uintdup(pQosPolicer->CommittedRate);
	}
	else if (strcmp(lastname, "CommittedBurstSize"                ) == 0 )
	{
		*data = uintdup(pQosPolicer->CommittedBurstSize);
	}
	else if (strcmp(lastname, "ExcessBurstSize"                   ) == 0 )
	{
		*data = uintdup(pQosPolicer->ExcessBurstSize);
	}
	else if (strcmp(lastname, "PeakRate"                          ) == 0 )
	{
		*data = uintdup(pQosPolicer->PeakRate);
	}
	else if (strcmp(lastname, "PeakBurstSize"                     ) == 0 )
	{
		*data = uintdup(pQosPolicer->PeakBurstSize);
	}
	else if (strcmp(lastname, "MeterType"                         ) == 0 )
	{
		*data = strdup(pQosPolicer->MeterType);
	}
	else if (strcmp(lastname, "PossibleMeterTypes"                ) == 0 )
	{
		*data = strdup(pQosPolicer->PossibleMeterTypes);
	}
	else if (strcmp(lastname, "ConformingAction"                  ) == 0 )
	{
		*data = strdup(pQosPolicer->ConformingAction);
	}
	else if (strcmp(lastname, "PartialConformingAction"           ) == 0 )
	{
		*data = strdup(pQosPolicer->PartialConformingAction);
	}
	else if (strcmp(lastname, "NonConformingAction"               ) == 0 )
	{
		*data = strdup(pQosPolicer->NonConformingAction);
	}
#if 0
	else if (strcmp(lastname, "CountedPackets"                    ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "CountedBytes"                      ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "TotalCountedPackets"               ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "TotalCountedBytes"                 ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "ConformingCountedPackets"          ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "ConformingCountedBytes"            ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "PartiallyConformingCountedPackets" ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "PartiallyConformingCountedBytes"   ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "NonConformingCountedPackets"       ) == 0 )
	{
		*data = uintdup(0);
	}
	else if (strcmp(lastname, "NonConformingCountedBytes"         ) == 0 )
	{
		*data = uintdup(0);
	}
#endif
	else
	{
		return ERR_9005;
	}
	
	return 0;
}

int setPolicerEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastName = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	unsigned int objNum = 0, id;

	QOSPOLICER_T *pQosPolicer, qosPolicer;
	QOSPOLICER_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosPolicerInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosPolicer = &qosPolicer;
	if (getQosPolicerEntryByInstNum(objNum, pQosPolicer, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(QOSPOLICER_T));
	memset(&qosArray[1], 0, sizeof(QOSPOLICER_T));
	memcpy(&qosArray[0], &qosPolicer, sizeof(QOSPOLICER_T));

	if (strcmp(lastName, "PolicerEnable") == 0)
	{
		int *bEn = data;

		pQosPolicer->PolicerEnable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "Alias") == 0)
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_PLOICER_ALIAS_LEN+1))
			strcpy(pQosPolicer->Alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "CommittedRate") == 0)
	{
		unsigned int *pCmtRate = data;

		pQosPolicer->CommittedRate = *pCmtRate;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "CommittedBurstSize") == 0)
	{
		unsigned int *pCmtBstSize = data;

		pQosPolicer->CommittedBurstSize = *pCmtBstSize;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "ExcessBurstSize") == 0)
	{
		unsigned int *pExBstSize = data;

		pQosPolicer->ExcessBurstSize = *pExBstSize;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "PeakRate") == 0)
	{
		unsigned int *pPeakRate = data;

		pQosPolicer->PeakRate = *pPeakRate;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "PeakBurstSize") == 0)
	{
		unsigned int *pPeakBstSize = data;

		pQosPolicer->PeakBurstSize = *pPeakBstSize;
		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "MeterType") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "SimpleTokenBucket") == 0 ||
			strcmp(buf, "SingleRateThreeColor") == 0 ||
			strcmp(buf, "TwoRateThreeColor") == 0)
		{
			strcpy(pQosPolicer->MeterType, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
#if 0
	else if (strcmp(lastName, "ConformingAction") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "SimpleTokenBucket") == 0 ||
			strcmp(buf, "SingleRateThreeColor") == 0 ||
			strcmp(buf, "TwoRateThreeColor") == 0)
		{
			strcpy(pQosPolicer->ConformingAction, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "PartialConformingAction") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "SimpleTokenBucket") == 0 ||
			strcmp(buf, "SingleRateThreeColor") == 0 ||
			strcmp(buf, "TwoRateThreeColor") == 0)
		{
			strcpy(pQosPolicer->PartialConformingAction, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastName, "NonConformingAction") == 0)
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "SimpleTokenBucket") == 0 ||
			strcmp(buf, "SingleRateThreeColor") == 0 ||
			strcmp(buf, "TwoRateThreeColor") == 0)
		{
			strcpy(pQosPolicer->NonConformingAction, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
		mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
	}
#endif
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int getQueueEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;	
	unsigned int objNum = 0;	
	unsigned int id;

	QOSQUEUE_T *pQosQueue, qosQueue;

	tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosQueueInstNum(name);	
	if (objNum == 0)
		return ERR_9005;

	pQosQueue = &qosQueue;
	if (getQosQueueEntryByInstNum(objNum, pQosQueue, &id) < 0)		
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if      (strcmp(lastname, "QueueKey"          ) == 0 )
	{
		*data = uintdup(pQosQueue->QueueKey);
	}
	else if (strcmp(lastname, "QueueEnable"       ) == 0 )
	{
		*data = booldup(pQosQueue->QueueEnable != 0);
	}
	else if (strcmp(lastname, "QueueStatus"       ) == 0 )
	{
		if( pQosQueue->QueueEnable==0 )
		 	*data=strdup( "Disabled" );
		else
			*data=strdup( "Enabled" );
	}
	else if (strcmp(lastname, "Alias"             ) == 0 )
	{
		*data = strdup(pQosQueue->Alias);
	}
	else if (strcmp(lastname, "TrafficClasses"    ) == 0 )
	{
		*data = strdup(pQosQueue->TrafficClasses);
	}
	else if (strcmp(lastname, "QueueInterface"    ) == 0 )
	{
		*data = strdup(pQosQueue->QueueInterface);
	}
	else if (strcmp(lastname, "QueueBufferLength" ) == 0 )
	{
		*data = uintdup(pQosQueue->QueueBufferLength);
	}
	else if (strcmp(lastname, "QueueWeight"       ) == 0 )
	{
		*data = uintdup(pQosQueue->QueueWeight);
	}
	else if (strcmp(lastname, "QueuePrecedence"   ) == 0 )
	{
		*data = uintdup(pQosQueue->QueuePrecedence);
	}
	else if (strcmp(lastname, "REDThreshold"      ) == 0 )
	{
		*data = uintdup(pQosQueue->REDThreshold);
	}
	else if (strcmp(lastname, "REDPercentage"     ) == 0 )
	{
		*data = uintdup(pQosQueue->REDPercentage);
	}
	else if (strcmp(lastname, "DropAlgorithm"     ) == 0 )
	{
		*data = strdup(pQosQueue->DropAlgorithm);
	}
	else if (strcmp(lastname, "SchedulerAlgorithm") == 0 )
	{
		*data = strdup(pQosQueue->SchedulerAlgorithm);
	}
	else if (strcmp(lastname, "ShapingRate"       ) == 0 )
	{
		*data = intdup(pQosQueue->ShapingRate);
	}
	else if (strcmp(lastname, "ShapingBurstSize"  ) == 0 )
	{
		*data = uintdup(pQosQueue->ShapingBurstSize);
	}
	else
	{
		return ERR_9005;
	}
	
	return 0;
}

int setQueueEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastname = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	unsigned int objNum = 0, id;

	QOSQUEUE_T *pQosQueue, qosQueue;
	QOSQUEUE_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosQueueInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosQueue = &qosQueue;
	if (getQosQueueEntryByInstNum(objNum, pQosQueue, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(QOSQUEUE_T));
	memset(&qosArray[1], 0, sizeof(QOSQUEUE_T));
	memcpy(&qosArray[0], &qosQueue, sizeof(QOSQUEUE_T));
	
	if (strcmp(lastname, "QueueEnable"       ) == 0 )
	{
		int *bEn = data;

		pQosQueue->QueueEnable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "Alias"             ) == 0 )
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_ALIAS_LEN+1))
			strcpy(pQosQueue->Alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "TrafficClasses"    ) == 0 )
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosQueue->TrafficClasses[0] = '\0';
		else if (len < 256)
			strcpy(pQosQueue->TrafficClasses, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "QueueInterface"    ) == 0 )
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosQueue->QueueInterface[0] = '\0';
		else if (len < 256)
			strcpy(pQosQueue->QueueInterface, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "QueueWeight"       ) == 0 )
	{
		unsigned int *pQueueWeight = data;

		pQosQueue->QueueWeight = *pQueueWeight;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "QueuePrecedence"   ) == 0 )
	{
		unsigned int *pQueuePrecedence = data;

		if (*pQueuePrecedence < 1)
			return ERR_9007;

		pQosQueue->QueuePrecedence = *pQueuePrecedence;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "REDThreshold"      ) == 0 )
	{
		unsigned int *pREDThreshold = data;

		if (*pREDThreshold > 100)
			return ERR_9007;

		pQosQueue->REDThreshold = *pREDThreshold;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "REDPercentage"     ) == 0 )
	{
		unsigned int *pREDPercentage = data;

		if (*pREDPercentage > 100)
			return ERR_9007;

		pQosQueue->REDPercentage = *pREDPercentage;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "DropAlgorithm"     ) == 0 )
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "RED") == 0 ||
			strcmp(buf, "DT") == 0 ||
			strcmp(buf, "WRED") == 0 ||
			strcmp(buf, "BLUE") == 0)
		{
			strcpy(pQosQueue->DropAlgorithm, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "SchedulerAlgorithm") == 0 )
	{
		if (buf == NULL)
			return ERR_9007;

		if (strcmp(buf, "WFQ") == 0 ||
			strcmp(buf, "WRR") == 0 ||
			strcmp(buf, "SP") == 0)
		{
			strcpy(pQosQueue->SchedulerAlgorithm, buf);
		}
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "ShapingRate"       ) == 0 )
	{
		int *pShapingRate = data;

		if (*pShapingRate < -1)
			return ERR_9007;

		pQosQueue->ShapingRate = *pShapingRate;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "ShapingBurstSize"  ) == 0 )
	{
		unsigned int *pShapingBurstSize = data;

		pQosQueue->ShapingBurstSize = *pShapingBurstSize;
		memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
		mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
	}
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

#define QUEUE_STATS_START_FLAG	0x1
#define QUEUE_STATS_END_FLAG	0x2
#define LAN_FLAG	0x1
#define WAN_FLAG	0x2

typedef struct {
	unsigned int 	qId;
	unsigned int	outputPkt;
	unsigned int	outputByte;
	unsigned int	dropPkt;
	unsigned int	OccupancyPkt;
	
}rtl_queStats_info;

static int getQueueStatistics(unsigned int id, char* lastname, char* intf)
{
	int downlink=0,uplink=0;
	int queueIndex=0;	
	int qId;
	int i = -1,j=0;
	int flag=0;
	char tmp[64];
	unsigned int tmpValue;
	rtl_queStats_info stats_info[MAX_QOS_QUEUE_NUM+1];
	char *ptr;
	char *strptr;
	FILE * stream;
	char command[64]={0};
	char buffer[512]={0};
	int retval=-1;
	int mode =0;
	char line_buffer[128]={0};
	QOSQUEUESTATS_T entry;
	if(lastname==NULL||intf==NULL)
		return retval;
	//tc -s class ls dev
	sprintf(command,"tc -s qdisc show");
	
	memset(&(stats_info[0]),0,sizeof(rtl_queStats_info)*(MAX_QOS_QUEUE_NUM+1));
	stream = popen (command,"r");
	if ( stream == NULL ) 
		return retval;
	else
	{ 	
		while(fgets(buffer, sizeof(buffer), stream))
		{
			strptr=buffer;
			ptr= strsep(&strptr," ");
			
			if(ptr) 
			{
				if(strcmp(ptr,"qdisc")==0)
				{	
					i++;
					flag =QUEUE_STATS_START_FLAG;
					ptr= strsep(&strptr," ");
					
					if(ptr)
					{
						ptr= strsep(&strptr,":");
						if(ptr){
						
							sscanf(ptr,"%d",&tmpValue);
							
							stats_info[i].qId=tmpValue;
						}
						
					}

					
					
				}
				else
				{
					ptr= strsep(&strptr," ");
					if(ptr){
					if(strcmp(ptr,"rate")==0)
					{	
						flag =QUEUE_STATS_END_FLAG;
					}
					
					if(flag ==QUEUE_STATS_START_FLAG)
					{
						if(strcmp(ptr,"Sent")==0)
						{
							
							ptr= strsep(&strptr," ");
							
							if(ptr)
							{
								sscanf(ptr,"%d",&tmpValue);
								stats_info[i].outputByte=tmpValue;
								
							}
							ptr= strstr(strptr,"bytes");
							
							if(ptr)
							{
								ptr= ptr+sizeof("bytes");
								if(ptr)
								{
									sscanf(ptr,"%d",&tmpValue);
									stats_info[i].outputPkt=tmpValue;
									
								}
							}
							ptr= strstr(strptr,"dropped");
							
							if(ptr)
							{
								ptr= ptr+sizeof("dropped");
								if(ptr)
								{
									sscanf(ptr,"%d",&tmpValue);
									
									stats_info[i].dropPkt=tmpValue;
									
								}
							}
							ptr= strstr(strptr,"overlimits");
							
							if(ptr)
							{
								ptr= ptr+sizeof("overlimits");
								if(ptr)
								{
									sscanf(ptr,"%d",&tmpValue);
									
									stats_info[i].OccupancyPkt=tmpValue;
								}
							}
						}
						
					}
					}
				}
				
			}
		}
	}
	pclose(stream);
	if(strlen(intf))
	{
		if(strstr(intf, "WAN")){
			queueIndex = WAN_FLAG *100+id+2;
		
		}	
		else if(strstr(intf, "LAN")){
			queueIndex = LAN_FLAG *100+id+2;
		}
		else
		{
		
		}
		
	}
	for(j=0;j<=i;j++)
	{
		if(stats_info[j].qId == queueIndex)
		{
			if(!strcmp(lastname,"OutputPackets"))
			{
				retval = stats_info[j].outputPkt;
				break;
			}
			else if(!strcmp(lastname,"OutputBytes"))
			{
				retval = stats_info[j].outputByte;
				break;
			}
			else if(!strcmp(lastname,"DroppedPackets"))
			{
				retval = stats_info[j].dropPkt;
				break;
			}
			else if(!strcmp(lastname,"QueueOccupancyPackets"))
			{
				retval = stats_info[i].OccupancyPkt;
				break;
			}
		}
	}
	return retval;
}
int getQueueStatsEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char *lastname = entity->info->name;
	char buff[256] = {0};
	unsigned char vChar = 0;	
	unsigned int objNum = 0;	
	unsigned int id;
	int res = -1;

	QOSQUEUESTATS_T *pQosQueueStats, qosQueueStats;

	tr098_printf("name: %s, lastname: %s", name, lastname);
	
	if ((name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	objNum = getQosQueueStatsInstNum(name);	
	if (objNum == 0)
		return ERR_9005;

	pQosQueueStats = &qosQueueStats;
	if (getQosQueueStatsEntryByInstNum(objNum, pQosQueueStats, &id) < 0)		
		return ERR_9005;

	*type = entity->info->type;
	*data = NULL;

	if      (strcmp(lastname, "Enable"                  ) == 0 )
	{
		*data = booldup(pQosQueueStats->Enable != 0);
	}
#if 0
	else if (strcmp(lastname, "Status"                  ) == 0 )
	{
		*data = uintdup(0);
	}
#endif
	else if (strcmp(lastname, "Alias"                   ) == 0 )
	{
		*data = strdup(pQosQueueStats->Alias);
	}
	else if (strcmp(lastname, "Queue"                   ) == 0 )
	{
		*data = uintdup(pQosQueueStats->Queue);
	}
	else if (strcmp(lastname, "Interface"               ) == 0 )
	{
		*data = strdup(pQosQueueStats->Interface);
	}
	else if(strcmp(lastname,"OutputPackets")==0||
		    strcmp(lastname,"OutputBytes")==0||
		    strcmp(lastname,"DroppedPackets")==0||
		    strcmp(lastname,"QueueOccupancyPackets")==0)
	{
		res = getQueueStatistics(pQosQueueStats->Queue,lastname,pQosQueueStats->Interface);
		if(res < 0)
			return ERR_9007;
		*data = uintdup(res);
	}
	else
	{
		return ERR_9005;
	}
	
	return 0;
}

int setQueueStatsEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char *lastname = entity->info->name;
	char *buf = data;
	int needReinit = 1;
	unsigned int objNum = 0, id;

	QOSQUEUESTATS_T *pQosQueueStats, qosQueueStats;
	QOSQUEUESTATS_T qosArray[2];

	if ((name == NULL) || (data == NULL) || (entity == NULL))
		return -1;

	if (entity->info->type!=type)
		return ERR_9006;

	objNum = getQosQueueStatsInstNum(name);

	tr098_printf("objNum %d", objNum);
	
	if (objNum == 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	pQosQueueStats = &qosQueueStats;
	if (getQosQueueStatsEntryByInstNum(objNum, pQosQueueStats, &id) < 0)
	{
		tr098_trace();
		return ERR_9005;
	}

	memset(&qosArray[0], 0, sizeof(QOSQUEUESTATS_T));
	memset(&qosArray[1], 0, sizeof(QOSQUEUESTATS_T));
	memcpy(&qosArray[0], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
	
	if (strcmp(lastname, "Enable"       ) == 0 )
	{
		int *bEn = data;

		pQosQueueStats->Enable = (*bEn == 0) ? 0 : 1;
		memcpy(&qosArray[1], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
		mib_set(MIB_QOS_QUEUESTATS_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "Alias"             ) == 0 )
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		len = strlen(buf);

		if (len > 0 && len < (MAX_PLOICER_ALIAS_LEN+1))
			strcpy(pQosQueueStats->Alias, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
		mib_set(MIB_QOS_QUEUESTATS_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "Queue"       ) == 0 )
	{
		unsigned int *pQueue = data;

		pQosQueueStats->Queue = *pQueue;
		memcpy(&qosArray[1], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
		mib_set(MIB_QOS_QUEUESTATS_MOD, (void *)&qosArray);
	}
	else if (strcmp(lastname, "Interface"    ) == 0 )
	{
		int len = 0;
		
		if(buf == NULL)
			return ERR_9007;

		if ((len = strlen(buf)) == 0)
			pQosQueueStats->Interface[0] = '\0';
		else if (len < (MAX_QUEUE_INTERFACE_LEN+1))
			strcpy(pQosQueueStats->Interface, buf);
		else
			return ERR_9007;

		memcpy(&qosArray[1], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
		mib_set(MIB_QOS_QUEUESTATS_MOD, (void *)&qosArray);
	}
	else
	{
		return ERR_9005;
	}
	if (needReinit)
		return 1;
	return 0;
}

int objClass(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	QOSCLASS_T *pQosClass, qosClass;
	QOSCLASS_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pQosClass = &qosClass;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_QOS_CLASS_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pQosClass) = (char)i;
				
				if (!mib_get(MIB_QOS_CLASS_TBL, (void *)pQosClass))
					continue;

				tr098_printf("ClassInstanceNum %d", pQosClass->ClassInstanceNum);

				if (pQosClass->ClassInstanceNum != 0)
				{
					maxInstNum = (maxInstNum>pQosClass->ClassInstanceNum)?maxInstNum:pQosClass->ClassInstanceNum;
				
					if (create_Object(c, tClassObject, sizeof(tClassObject), 1, pQosClass->ClassInstanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			tr098_printf("ClassInstanceNum %d", *(unsigned int*)data);
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tClassObject, sizeof(tClassObject), data);

			tr098_printf("ClassInstanceNum %d", *(unsigned int*)data);

			if (ret >= 0)
			{
				QOSCLASS_T newEntry;
				
				memset(&newEntry, 0, sizeof(QOSCLASS_T));
				
				newEntry.ClassInstanceNum = *(unsigned int*)data;

				/* inital value */
				strcpy(newEntry.ClassificationStatus, "Disabled");
				newEntry.Protocol = -1;
				newEntry.DestPort = -1;
				newEntry.DestPortRangeMax = -1;
				newEntry.SourcePort = -1;
				newEntry.SourcePortRangeMax = -1;
				newEntry.Ethertype = -1;
				newEntry.SSAP = -1;
				newEntry.DSAP = -1;
				newEntry.LLCControl = -1;
				newEntry.SNAPOUI = -1;
				strcpy(newEntry.SourceVendorClassIDMode, "Exact");
				strcpy(newEntry.DestVendorClassIDMode, "Exact");
				newEntry.SourceVendorSpecificInfoEnterprise = 0;
				newEntry.SourceVendorSpecificInfoSubOption = 0;
				strcpy(newEntry.SourceVendorSpecificInfoMode, "Exact");
				newEntry.DestVendorSpecificInfoEnterprise = 0;
				newEntry.DestVendorSpecificInfoSubOption = 0;
				strcpy(newEntry.DestVendorSpecificInfoMode, "Exact");
				newEntry.IPLengthMin = 0;
				newEntry.IPLengthMax = 0;
				newEntry.DSCPCheck = -1;
				newEntry.DSCPMark = -1;
				newEntry.EthernetPriorityCheck = -1;
				newEntry.EthernetPriorityMark = -1;
				newEntry.VLANIDCheck = -1;
				newEntry.OutOfBandInfo = -1;
				newEntry.ForwardingPolicy = 0;
				newEntry.TrafficClass = -1;
				newEntry.ClassPolicer = -1;
				newEntry.ClassQueue = -1;
				newEntry.ClassApp = -1;
				
				mib_set(MIB_QOS_CLASS_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			QOSCLASS_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosClassEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_QOS_CLASS_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_QOS_CLASS_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pQosClass) = (char)i;
				
				if (!mib_get(MIB_QOS_CLASS_TBL, (void *)pQosClass))
					continue;

				memset(&qosArray[0], 0, sizeof(QOSCLASS_T));
				memset(&qosArray[1], 0, sizeof(QOSCLASS_T));
				memcpy(&qosArray[0], &qosClass, sizeof(QOSCLASS_T));

				remove_entity = remove_SiblingEntity(&old_table, pQosClass->ClassInstanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pQosClass->ClassInstanceNum;

					tr098_printf("ClassInstanceNum %d", pQosClass->ClassInstanceNum);
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tClassObject, sizeof(tClassObject), &maxInstNum);
					
					if(maxInstNum != pQosClass->ClassInstanceNum)
					{
						pQosClass->ClassInstanceNum = maxInstNum;

						tr098_printf("ClassInstanceNum %d", pQosClass->ClassInstanceNum);
						
						memcpy(&qosArray[1], &qosClass, sizeof(QOSCLASS_T));
						mib_set(MIB_QOS_CLASS_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}

int objQueue(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	QOSQUEUE_T *pQosQueue, qosQueue;
	QOSQUEUE_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pQosQueue = &qosQueue;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_QOS_QUEUE_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pQosQueue) = (char)i;
				
				if (!mib_get(MIB_QOS_QUEUE_TBL, (void *)pQosQueue))
					continue;

				if (pQosQueue->QueueInstanceNum != 0)
				{
                    maxInstNum = (maxInstNum>pQosQueue->QueueInstanceNum)?maxInstNum:pQosQueue->QueueInstanceNum;
				
					if (create_Object(c, tQueueObject, sizeof(tQueueObject), 1, pQosQueue->QueueInstanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tQueueObject, sizeof(tQueueObject), data);

			if (ret >= 0)
			{
				QOSQUEUE_T newEntry;
				
				memset(&newEntry, 0, sizeof(QOSQUEUE_T));
				
				newEntry.QueueInstanceNum = *(unsigned int*)data;

				/* initial value */
				//strcpy(newEntry.QueueStatus, "Disabled");
				newEntry.QueueWeight = 0;
				newEntry.QueuePrecedence = 1;
				newEntry.REDThreshold = 0;
				newEntry.REDPercentage = 0;
				//strcpy(newEntry.DropAlgorithm, "DT");
				//strcpy(newEntry.SchedulerAlgorithm, "SP");
				newEntry.ShapingRate = -1;
				
				mib_set(MIB_QOS_QUEUE_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			QOSQUEUE_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosQueueEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_QOS_QUEUE_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_QOS_QUEUE_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pQosQueue) = (char)i;
				
				if (!mib_get(MIB_QOS_QUEUE_TBL, (void *)pQosQueue))
					continue;

				memset(&qosArray[0], 0, sizeof(QOSQUEUE_T));
				memset(&qosArray[1], 0, sizeof(QOSQUEUE_T));
				memcpy(&qosArray[0], &qosQueue, sizeof(QOSQUEUE_T));

				remove_entity = remove_SiblingEntity(&old_table, pQosQueue->QueueInstanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pQosQueue->QueueInstanceNum;
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tQueueObject, sizeof(tQueueObject), &maxInstNum);
					
					if(maxInstNum != pQosQueue->QueueInstanceNum)
					{
						pQosQueue->QueueInstanceNum = maxInstNum;
						memcpy(&qosArray[1], &qosQueue, sizeof(QOSQUEUE_T));
						mib_set(MIB_QOS_QUEUE_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}

#if 0
int objApp(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	TR098_APPCONF_T *pQosApp, qosApp;
	TR098_APPCONF_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pQosApp = &qosApp;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_TR098_QOS_APP_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pQosApp) = (char)i;
				
				if (!mib_get(MIB_TR098_QOS_APP_TBL, (void *)pQosApp))
					continue;

				tr098_printf("instanceNum %d", pQosApp->instanceNum);

				if (pQosApp->instanceNum != 0)
				{
                    maxInstNum = (maxInstNum>pQosApp->instanceNum)?maxInstNum:pQosApp->instanceNum;
				
					if (create_Object(c, tAppObject, sizeof(tAppObject), 1, pQosApp->instanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			tr098_printf("instanceNum %d", *(unsigned int*)data);
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tAppObject, sizeof(tAppObject), data);

			tr098_printf("instanceNum %d", *(unsigned int*)data);

			if (ret >= 0)
			{
				TR098_APPCONF_T newEntry;
				
				memset(&newEntry, 0, sizeof(TR098_APPCONF_T));
				
				newEntry.instanceNum = *(unsigned int*)data;

				/* inital value */
				strcpy(newEntry.app_status, "Disabled");
				newEntry.default_policy = 0;
				newEntry.default_class = -1;
				newEntry.default_policer = -1;
				newEntry.default_queue = -1;
				newEntry.default_dscp_mark = -1;
				newEntry.default_8021p_mark = -1;
				
				mib_set(MIB_TR098_QOS_APP_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			TR098_APPCONF_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosAppEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_TR098_QOS_APP_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_TR098_QOS_APP_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pQosApp) = (char)i;
				
				if (!mib_get(MIB_TR098_QOS_APP_TBL, (void *)pQosApp))
					continue;

				memset(&qosArray[0], 0, sizeof(TR098_APPCONF_T));
				memset(&qosArray[1], 0, sizeof(TR098_APPCONF_T));
				memcpy(&qosArray[0], &qosApp, sizeof(TR098_APPCONF_T));

				remove_entity = remove_SiblingEntity(&old_table, pQosApp->instanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pQosApp->instanceNum;

					tr098_printf("instanceNum %d", pQosApp->instanceNum);
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tAppObject, sizeof(tAppObject), &maxInstNum);
					
					if(maxInstNum != pQosApp->instanceNum)
					{
						pQosApp->instanceNum = maxInstNum;

						tr098_printf("instanceNum %d", pQosApp->instanceNum);
						
						memcpy(&qosArray[1], &qosApp, sizeof(TR098_APPCONF_T));
						mib_set(MIB_TR098_QOS_APP_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}

int objFlow(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	TR098_FLOWCONF_T *pQosFlow, qosFlow;
	TR098_FLOWCONF_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pQosFlow = &qosFlow;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_TR098_QOS_FLOW_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pQosFlow) = (char)i;
				
				if (!mib_get(MIB_TR098_QOS_FLOW_TBL, (void *)pQosFlow))
					continue;

				tr098_printf("instanceNum %d", pQosFlow->instanceNum);

				if (pQosFlow->instanceNum != 0)
				{
                    maxInstNum = (maxInstNum>pQosFlow->instanceNum)?maxInstNum:pQosFlow->instanceNum;
				
					if (create_Object(c, tFlowObject, sizeof(tFlowObject), 1, pQosFlow->instanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			tr098_printf("instanceNum %d", *(unsigned int*)data);
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tFlowObject, sizeof(tFlowObject), data);

			tr098_printf("instanceNum %d", *(unsigned int*)data);

			if (ret >= 0)
			{
				TR098_FLOWCONF_T newEntry;
				
				memset(&newEntry, 0, sizeof(TR098_FLOWCONF_T));
				
				newEntry.instanceNum = *(unsigned int*)data;

				/* inital value */
				strcpy(newEntry.flow_status, "Disabled");
				newEntry.app_identify = -1;
				newEntry.qos_policy = 0;
				newEntry.flow_class = -1;
				newEntry.flow_policer = -1;
				newEntry.flow_queue = -1;
				newEntry.flow_dscp_mark = -1;
				newEntry.flow_8021p_mark = -1;
				
				mib_set(MIB_TR098_QOS_FLOW_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			TR098_FLOWCONF_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosFlowEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_TR098_QOS_FLOW_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_TR098_QOS_FLOW_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pQosFlow) = (char)i;
				
				if (!mib_get(MIB_TR098_QOS_FLOW_TBL, (void *)pQosFlow))
					continue;

				memset(&qosArray[0], 0, sizeof(TR098_FLOWCONF_T));
				memset(&qosArray[1], 0, sizeof(TR098_FLOWCONF_T));
				memcpy(&qosArray[0], &qosFlow, sizeof(TR098_FLOWCONF_T));

				remove_entity = remove_SiblingEntity(&old_table, pQosFlow->instanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pQosFlow->instanceNum;

					tr098_printf("instanceNum %d", pQosFlow->instanceNum);
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tFlowObject, sizeof(tFlowObject), &maxInstNum);
					
					if(maxInstNum != pQosFlow->instanceNum)
					{
						pQosFlow->instanceNum = maxInstNum;

						tr098_printf("instanceNum %d", pQosFlow->instanceNum);
						
						memcpy(&qosArray[1], &qosFlow, sizeof(TR098_FLOWCONF_T));
						mib_set(MIB_TR098_QOS_FLOW_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}

int objPolicer(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	QOSPOLICER_T *pQosPolicer, qosPolicer;
	QOSPOLICER_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pQosPolicer = &qosPolicer;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_QOS_POLICER_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pQosPolicer) = (char)i;
				
				if (!mib_get(MIB_QOS_POLICER_TBL, (void *)pQosPolicer))
					continue;

				tr098_printf("InstanceNum %d", pQosPolicer->InstanceNum);

				if (pQosPolicer->InstanceNum != 0)
				{
                    maxInstNum = (maxInstNum>pQosPolicer->InstanceNum)?maxInstNum:pQosPolicer->InstanceNum;
				
					if (create_Object(c, tPolicerObject, sizeof(tPolicerObject), 1, pQosPolicer->InstanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			tr098_printf("InstanceNum %d", *(unsigned int*)data);
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tPolicerObject, sizeof(tPolicerObject), data);

			tr098_printf("InstanceNum %d", *(unsigned int*)data);

			if (ret >= 0)
			{
				QOSPOLICER_T newEntry;
				
				memset(&newEntry, 0, sizeof(QOSPOLICER_T));
				
				newEntry.InstanceNum = *(unsigned int*)data;

				/* inital value */
				newEntry.CommittedRate = 0;
				newEntry.CommittedBurstSize = 0;
				newEntry.ExcessBurstSize = 0;
				newEntry.PeakRate = 0;
				newEntry.PeakBurstSize = 0;
				strcpy(newEntry.MeterType, "Simple-Token-Bucket");
				strcpy(newEntry.ConformingAction, "Null");
				strcpy(newEntry.PartialConformingAction, "Drop");
				strcpy(newEntry.NonConformingAction, "Drop");
				
				mib_set(MIB_QOS_POLICER_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			QOSPOLICER_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosPolicerEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_QOS_POLICER_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_QOS_POLICER_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pQosPolicer) = (char)i;
				
				if (!mib_get(MIB_QOS_POLICER_TBL, (void *)pQosPolicer))
					continue;

				memset(&qosArray[0], 0, sizeof(QOSPOLICER_T));
				memset(&qosArray[1], 0, sizeof(QOSPOLICER_T));
				memcpy(&qosArray[0], &qosPolicer, sizeof(QOSPOLICER_T));

				remove_entity = remove_SiblingEntity(&old_table, pQosPolicer->InstanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pQosPolicer->InstanceNum;

					tr098_printf("InstanceNum %d", pQosPolicer->InstanceNum);
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tPolicerObject, sizeof(tPolicerObject), &maxInstNum);
					
					if(maxInstNum != pQosPolicer->InstanceNum)
					{
						pQosPolicer->InstanceNum = maxInstNum;

						tr098_printf("InstanceNum %d", pQosPolicer->InstanceNum);
						
						memcpy(&qosArray[1], &qosPolicer, sizeof(QOSPOLICER_T));
						mib_set(MIB_QOS_POLICER_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}

int objQueueStats(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity = (struct CWMP_NODE *)e;

	QOSQUEUESTATS_T *pqosQueueStatsStats, qosQueueStats;
	QOSQUEUESTATS_T qosArray[2];

	int i, ret, tblNum;
	
	tr098_printf("action: %d, name: %s", type, name);

	pqosQueueStatsStats = &qosQueueStats;

	switch (type)
	{
		case eCWMP_tINITOBJ:
		{
			struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

			unsigned int maxInstNum = 0;

			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			mib_get(MIB_QOS_QUEUESTATS_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				*((char *)pqosQueueStatsStats) = (char)i;
				
				if (!mib_get(MIB_QOS_QUEUESTATS_TBL, (void *)pqosQueueStatsStats))
					continue;

				if (pqosQueueStatsStats->InstanceNum != 0)
				{
                    maxInstNum = (maxInstNum>pqosQueueStatsStats->InstanceNum)?maxInstNum:pqosQueueStatsStats->InstanceNum;
				
					if (create_Object(c, tQueueStatsObject, sizeof(tQueueStatsObject), 1, pqosQueueStatsStats->InstanceNum) < 0)
						return -1;
				}
			}

			add_objectNum(name, maxInstNum);

			return 0;	     		     	
		}
		case eCWMP_tADDOBJ:
		{
			if ((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;
			
			ret = add_Object(name, (struct CWMP_LINKNODE **)&entity->next, tQueueStatsObject, sizeof(tQueueStatsObject), data);

			if (ret >= 0)
			{
				QOSQUEUESTATS_T newEntry;
				
				memset(&newEntry, 0, sizeof(QOSQUEUESTATS_T));
				
				newEntry.InstanceNum = *(unsigned int*)data;

				/* initial value */
				
				mib_set(MIB_QOS_QUEUESTATS_ADD, (void *)&newEntry);
				
				ret = 1;
			}

			return ret;
		}
		case eCWMP_tDELOBJ:
		{
			QOSQUEUESTATS_T delEntry;
			
			int ret = 0;
			unsigned int id;

			if((name == NULL) || (entity == NULL) || (data == NULL))
				return -1;

			if (getQosQueueStatsEntryByInstNum(*(unsigned int*)data, &delEntry, &id))
				return ERR_9005;

			mib_set(MIB_QOS_QUEUESTATS_DEL, (void *)&delEntry);
			
			ret = del_Object(name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data);

			return ret;
		}
		case eCWMP_tUPDATEOBJ:
		{
			int i, tblNum;
			struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

			mib_get(MIB_QOS_QUEUESTATS_TBL_NUM, (void *)&tblNum);

			for (i=1; i<=tblNum; i++)
			{
				struct CWMP_LINKNODE *remove_entity = NULL;

				*((char *)pqosQueueStatsStats) = (char)i;
				
				if (!mib_get(MIB_QOS_QUEUESTATS_TBL, (void *)pqosQueueStatsStats))
					continue;

				memset(&qosArray[0], 0, sizeof(QOSQUEUESTATS_T));
				memset(&qosArray[1], 0, sizeof(QOSQUEUESTATS_T));
				memcpy(&qosArray[0], &qosQueueStats, sizeof(QOSQUEUESTATS_T));

				remove_entity = remove_SiblingEntity(&old_table, pqosQueueStatsStats->InstanceNum);

				if (remove_entity != NULL)
				{
					add_SiblingEntity((struct CWMP_LINKNODE **)&entity->next, remove_entity);
				}
				else
				{
					unsigned int maxInstNum = pqosQueueStatsStats->InstanceNum;
					
					add_Object(name, (struct CWMP_LINKNODE **)&entity->next,  tQueueStatsObject, sizeof(tQueueStatsObject), &maxInstNum);
					
					if(maxInstNum != pqosQueueStatsStats->InstanceNum)
					{
						pqosQueueStatsStats->InstanceNum = maxInstNum;
						memcpy(&qosArray[1], &qosQueueStats, sizeof(QOSQUEUESTATS_T));
						mib_set(MIB_QOS_QUEUESTATS_MOD, (void *)&qosArray);
					}
				}
			}

			if (old_table)
	     		destroy_ParameterTable((struct CWMP_NODE *)old_table);

			return 0;
		}
	}
	
	return -1;
}
#endif
#endif /*_STD_QOS_*/
