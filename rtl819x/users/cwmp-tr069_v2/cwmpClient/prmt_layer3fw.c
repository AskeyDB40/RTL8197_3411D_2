#include "prmt_layer3fw.h"
#include "prmt_wancondevice.h"
#include <sys/socket.h>
//#include <sys/un.h>
#include <linux/sockios.h>
#include <net/if.h>
#ifndef _PRMT_WT107_
#define _PRMT_WT107_
#endif
#ifndef RTF_UP
#define RTF_UP			0x0001          /* route usable                 */
#endif
#ifndef RTF_HOST
#define RTF_HOST		0x0004         
#endif
#ifdef _PRMT_WT107_
#define MAX_DYNAMIC_ROUTE_INSTNUM	1000
int getDynamicForwardingTotalNum(void);
int getDynamicForwardingEntryByInstNum( unsigned int instnum, STATICROUTE_T *pRoute );
#endif //_PRMT_WT107_
//extern unsigned char transfer2IfIndxfromIfName( char *ifname );
extern unsigned int getInstNum( char *name, char *objname );
extern int rtl_transfer_interface_index_to_name(char *name,STATICROUTE_T *pRoute);
int queryRouteStatus( STATICROUTE_T *p );
unsigned int getForwardingInstNum( char *name );
unsigned int findMaxForwardingInstNum(void);
int getForwardingEntryByInstNum( unsigned int instnum, STATICROUTE_T *p, unsigned int change_index );
unsigned int rtl_multiwan_if_name_transfer_index(char *name);
/*******ForwardingEntity****************************************************************************************/

struct CWMP_OP tForwardingEntityLeafOP = { getFwEntity, setFwEntity };
struct CWMP_PRMT tForwardingEntityLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"Status",			eCWMP_tSTRING,	CWMP_READ,		&tForwardingEntityLeafOP},
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#ifdef _PRMT_WT107_
{"StaticRoute",			eCWMP_tBOOLEAN,	CWMP_READ,		&tForwardingEntityLeafOP},
#endif
/*ping_zhang:20081217 END*/
{"Type",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"DestIPAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"DestSubnetMask",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"SourceIPAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"SourceSubnetMask",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"GatewayIPAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"Interface",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
{"ForwardingMetric",		eCWMP_tINT,	CWMP_WRITE|CWMP_READ,	&tForwardingEntityLeafOP},
/*MTU*/
};
enum eForwardingEntityLeaf
{
	eFWEnable,
	eFWStatus,
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1//def _PRMT_WT107_
	eFWStaticRoute,
#endif
/*ping_zhang:20081217 END*/
	eFWType,
	eFWDestIPAddress,
	eFWDestSubnetMask,
	eFWSourceIPAddress,
	eFWSourceSubnetMask,
	eFWGatewayIPAddress,
	eFWInterface,
	eFWForwardingMetric
};
struct CWMP_LEAF tForwardingEntityLeaf[] =
{
{ &tForwardingEntityLeafInfo[eFWEnable] },
{ &tForwardingEntityLeafInfo[eFWStatus] },
/*ping_zhang:20081217 START:patch from telefonica branch to support WT-107*/
#if 1//def _PRMT_WT107_
{ &tForwardingEntityLeafInfo[eFWStaticRoute] },
#endif
/*ping_zhang:20081217 END*/
{ &tForwardingEntityLeafInfo[eFWType] },
{ &tForwardingEntityLeafInfo[eFWDestIPAddress] },
{ &tForwardingEntityLeafInfo[eFWDestSubnetMask] },
{ &tForwardingEntityLeafInfo[eFWSourceIPAddress] },
{ &tForwardingEntityLeafInfo[eFWSourceSubnetMask] },
{ &tForwardingEntityLeafInfo[eFWGatewayIPAddress] },
{ &tForwardingEntityLeafInfo[eFWInterface] },
{ &tForwardingEntityLeafInfo[eFWForwardingMetric] },
{ NULL }
};

/*******Forwarding****************************************************************************************/
struct CWMP_PRMT tForwardingOjbectInfo[] =
{
/*(name,	type,		flag,					op)*/
{"0",		eCWMP_tOBJECT,	CWMP_READ|CWMP_WRITE|CWMP_LNKLIST,	NULL}
};
enum eForwardingOjbect
{
	eFW0
};
struct CWMP_LINKNODE tForwardingObject[] =
{
/*info,  			leaf,			next,		sibling,		instnum)*/
{&tForwardingOjbectInfo[eFW0],	tForwardingEntityLeaf,	NULL,		NULL,			0},
};

/*******Layer3Forwarding****************************************************************************************/
struct CWMP_OP tLayer3ForwardingLeafOP = { getLayer3Fw,	setLayer3Fw };
struct CWMP_PRMT tLayer3ForwardingLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"DefaultConnectionService",	eCWMP_tSTRING,	CWMP_READ,	&tLayer3ForwardingLeafOP},
//{"DefaultConnectionService",	eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tLayer3ForwardingLeafOP},
{"ForwardNumberOfEntries",	eCWMP_tUINT,	CWMP_READ,		&tLayer3ForwardingLeafOP},
};
enum eLayer3ForwardingLeaf
{
	eFWDefaultConnectionService,
	eFWForwardNumberOfEntries
};
struct CWMP_LEAF tLayer3ForwardingLeaf[] =
{
{ &tLayer3ForwardingLeafInfo[eFWDefaultConnectionService] },
{ &tLayer3ForwardingLeafInfo[eFWForwardNumberOfEntries] },
{ NULL }
};
struct CWMP_OP tFW_Forwarding_OP = { NULL, objForwading };
struct CWMP_PRMT tLayer3ForwardingObjectInfo[] =
{
/*(name,			type,		flag,			)*/
{"Forwarding",			eCWMP_tOBJECT,	CWMP_WRITE|CWMP_READ,	&tFW_Forwarding_OP},
};
enum eLayer3ForwardingObject
{
	eFWForwarding
};
struct CWMP_NODE tLayer3ForwardingObject[] =
{
/*info,  					leaf,			node)*/
{&tLayer3ForwardingObjectInfo[eFWForwarding],	NULL,			NULL},
{NULL,						NULL,			NULL}
};
/***********************************************************************************************/
int getFwEntity(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	char	fw_buf[256], buf[64], *tok;
	unsigned int object_num=0;
	struct CWMP_NODE *fw_entity;
	STATICROUTE_T *fw=NULL, route_entity;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	object_num=getForwardingInstNum( name );
	if(object_num==0) return ERR_9005;
	fw = &route_entity;
#ifdef _PRMT_WT107_
	if( object_num<=MAX_DYNAMIC_ROUTE_INSTNUM )
	{
		//dynamic route
		getDynamicForwardingEntryByInstNum( object_num, fw );
	}else
#endif //_PRMT_WT107_
	{
		//static route
		if(getForwardingEntryByInstNum( object_num, fw, 1)<0)
			return ERR_9005;
	}
	
	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
		//fprintf( stderr, "<%s:%d>enable:%d\n",__FUNCTION__,__LINE__,fw->Enable  );fflush(NULL);
		*data = booldup( fw->Enable!=0 );
	}else if( strcmp( lastname, "Status" )==0 )
	{
		int RouteRet;
#ifdef _PRMT_WT107_
		if( object_num<=MAX_DYNAMIC_ROUTE_INSTNUM )
			RouteRet = (fw->Enable)?1:0;
		else
#endif
			RouteRet = queryRouteStatus(fw);
		switch(RouteRet)
		{
		case 1: //found
			*data = strdup( "Enabled" );
			break;
		case 0: //not found
			if( fw->Enable )
				*data = strdup( "Error" );
			else
				*data = strdup( "Disabled" );
			break;
		case -1://error
		default:
			*data = strdup( "Error" );
			break;
		}
	}
#ifdef _PRMT_WT107_
	else if( strcmp( lastname, "StaticRoute" )==0 )
	{
		if( object_num<=MAX_DYNAMIC_ROUTE_INSTNUM )
			*data = booldup(0);
		else
			*data = booldup(1);
	}
#else
	else if( strcmp( lastname, "StaticRoute" )==0 )
	{
			*data = booldup(1);
	}
#endif //_PRMT_WT107_
	else if( strcmp( lastname, "Type" )==0 )
	{
		if( fw->Type==0 )
			*data = strdup( "Network" );
		else if( fw->Type==1 )
			*data = strdup( "Host" );
		else if( fw->Type==2 )
			*data = strdup( "Default" );
		else 
			return ERR_9002;
	}
	else if( strcmp( lastname, "DestIPAddress" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&fw->dstAddr)));
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "DestSubnetMask" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&fw->netmask)));
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "SourceIPAddress" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&fw->SourceIP)));
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "SourceSubnetMask" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&fw->SourceMask)));
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "GatewayIPAddress" )==0 )
	{
		sprintf(buf, "%s", inet_ntoa(*((struct in_addr *)&fw->gateway)));
		*data = strdup( buf );
	}
	else if( strcmp( lastname, "Interface" )==0 )
	{
		char tmp[IFNAMSIZ];
		//change from eth0 to internetgatewaydevice.xxx.xxxx.xxxx.xxxx
		//strcpy( tmp, fw->Interface );
		memset(tmp,0,IFNAMSIZ);
		if(rtl_transfer_interface_index_to_name(tmp,fw)<0)
			*data = strdup( "" );//return ERR_9007;
		else
			*data = strdup(tmp);
	}
	else if( strcmp( lastname, "ForwardingMetric" )==0 )
	{
		*data = intdup( fw->metric );
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int setFwEntity(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	fw_buf[256], *tok;
	char	*buf=data;
	int	object_num=0;
	struct CWMP_NODE *fw_entity;
	STATICROUTE_T *fw=NULL,route_entity;
	STATICROUTE_T target[2];
	struct in_addr in;
	char	*pzeroip="0.0.0.0";
#ifdef _CWMP_APPLY_
	STATICROUTE_T route_old;
#endif
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;

	if( entity->info->type!=type ) return ERR_9006;

	object_num=getForwardingInstNum( name );
	if(object_num==0) return ERR_9005;
#ifdef _PRMT_WT107_
	if( object_num<=MAX_DYNAMIC_ROUTE_INSTNUM ) return ERR_9001; //reject to modify a dynamic route
#endif //_PRMT_WT107_
	fw = &route_entity;
	memset( &target[0], 0, sizeof( STATICROUTE_T ) );
	memset( &target[1], 0, sizeof( STATICROUTE_T ) );

	if(getForwardingEntryByInstNum( object_num, fw,0)<0)
		return ERR_9005;
	memcpy(&target[0], &route_entity, sizeof(STATICROUTE_T));

#ifdef _CWMP_APPLY_
	memcpy( &route_old, fw, sizeof(STATICROUTE_T) );
#endif

	if( strcmp( lastname, "Enable" )==0 )
	{
		int *i = data;

		int entry_Num = 0, value = 0, j = 0;
		STATICROUTE_T tmp_entry;
		fw->Enable = (*i==0)? 0:1;
		
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
		
		//check enable or disable MIB_STATICROUTE_ENABLED
		apmib_get(MIB_STATICROUTE_TBL_NUM, (void *)&entry_Num);
		value = 0;
		for (j=1; j<=entry_Num; j++) {
			memset((void *)&tmp_entry, 0x00, sizeof(tmp_entry));
			*((char *)&tmp_entry) = (char)j;
			apmib_get(MIB_STATICROUTE_TBL, (void *)&tmp_entry);
			if (tmp_entry.Enable){
				value++;
			}
		}
		//printf("[%s:%d]fw->Enable=%d\n", __FUNCTION__, __LINE__, fw->Enable);
		if(value > 0){
			value = 1;
			mib_set( MIB_STATICROUTE_ENABLED, (void *)&value);
		}
		mib_get( MIB_STATICROUTE_ENABLED, (void *)&value);
        	printf("[%s:%d]MIB_STATICROUTE_ENABLED=%d\n", __FUNCTION__, __LINE__, value);
		
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "Type" )==0 )
	{
		if(  strcmp( buf, "Network" )==0 )
			fw->Type = 0;
		else if(  strcmp( buf, "Host" )==0 )
			fw->Type = 1;		
		else if(  strcmp( buf, "Default" )==0 )
			fw->Type = 2;
		else
			return ERR_9007;
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "DestIPAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;//return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy(fw->dstAddr, &in, sizeof(struct in_addr));
		/* "Interface" field maybe not configure for each static route.   
		  * so if not set, default ifIndex will set as wan index here,fix me?  
		  */
		//fw->ifIndex = 1;
		//fw->interface = fw->ifIndex; //for web ui display 
		//printf("%s %d fw->interface=0x%x \n", __FUNCTION__, __LINE__, fw->interface);
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);

#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "DestSubnetMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;//return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy(fw->netmask, &in, sizeof(struct in_addr));

		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "SourceIPAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
#if 1
		if( (strlen(buf)==0) || (strcmp(buf, pzeroip)==0) )
			return 0;
		else
			return ERR_9001;//deny
#else
		if( strlen(buf)==0 ) buf=pzeroip;//return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy( fw->SourceIP, &in, sizeof(struct in_addr) );
		mib_chain_update( MIB_IP_ROUTE_TBL, (unsigned char*)fw, chain_id );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
#endif
	}else if( strcmp( lastname, "SourceSubnetMask" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
#if 1
		if( (strlen(buf)==0) || (strcmp(buf, pzeroip)==0) )
			return 0;
		else
			return ERR_9001;//deny
#else
		if( strlen(buf)==0 ) buf=pzeroip;//return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy( fw->SourceMask, &in, sizeof(struct in_addr) );
		mib_chain_update( MIB_IP_ROUTE_TBL, (unsigned char*)fw, chain_id );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
#endif
	}
	else if( strcmp( lastname, "GatewayIPAddress" )==0 )
	{
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) buf=pzeroip;//return ERR_9007;
		if( inet_aton( buf, &in )==0 ) //the ip address is error.
			return ERR_9007;
		memcpy(fw->gateway, &in, sizeof(struct in_addr));
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "Interface" )==0 )
	{
		//change from internetgatewaydevice.xxx.xx.xxx to eth0
		//strcpy( fw->Interface, "eth0" );
		unsigned int newifindex;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0||strlen(buf)>IFNAMSIZ) 
			fw->ifIndex=DUMMY_IFINDEX;//return ERR_9007;
		else{
			//newifindex = transfer2IfIndex( buf );
			char name[IFNAMSIZ];
			sscanf(buf, "%s",name);
			newifindex=rtl_multiwan_if_name_transfer_index(name);
			if( newifindex==DUMMY_IFINDEX ) 
				return ERR_9007;
			fw->ifIndex = newifindex;
			
			fw->interface = fw->ifIndex; //for web ui display 
			//printf("%s %d fw->ifIndex=0x%x \n", __FUNCTION__, __LINE__, fw->ifIndex);
		}
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else if( strcmp( lastname, "ForwardingMetric" )==0 )
	{
		int *i = data;

		if( *i < -1 ) return ERR_9007;
		fw->metric = *i;
		memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
		mib_set(MIB_STATICROUTE_MOD, (void *)&target);
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_Layer3Forwarding, CWMP_RESTART, chain_id, &route_old, sizeof(MIB_CE_IP_ROUTE_T) );
		return 0;
#else
		return 1;
#endif
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}
extern int rtl_get_default_route(char *name);
int getLayer3Fw(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "DefaultConnectionService" )==0 )
	{
		char name[IFNAMSIZ];
		//if( getDefaultRouteIfaceName(buf)<0 )
		memset(name,0,sizeof(name));
		if(rtl_get_default_route(name)<0)
			*data = strdup( "" );
		else
			*data = strdup(name);
	}else if( strcmp( lastname, "ForwardNumberOfEntries" )==0 )
	{
		unsigned int total=0;
		mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&total);
#ifdef _PRMT_WT107_
		total += getDynamicForwardingTotalNum();
#endif //_PRMT_WT107_
		*data = uintdup( total );
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int setLayer3Fw(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "DefaultConnectionService" )==0 )
	{
		return ERR_9005;//not support now
		char *buf=data;
		if( buf==NULL ) return ERR_9007;
		if( strlen(buf)==0 ) return ERR_9007;
		if( setDefaultRouteIfaceName( buf ) < 0 ) return ERR_9007;
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_DefaultRoute, CWMP_RESTART, 0, NULL, 0 );
		return 0;
#else
		return 1;
#endif //_CWMP_APPLY_
	}
	else{
		return ERR_9005;
	}
	
	return 0;
}

int objForwading(char *name, struct CWMP_LEAF *e, int type, void *data)
{
	struct CWMP_NODE *entity=(struct CWMP_NODE *)e;
	STATICROUTE_T *p,route_entity;
	STATICROUTE_T target[2];
	CWMPDBG( 2, ( stderr, "<%s:%d>name:%s(action:%d)\n", __FUNCTION__, __LINE__, name,type ) );

	switch( type )
	{
	case eCWMP_tINITOBJ:
	     {
		unsigned int num=0,MaxInstNum=0,i;
		struct CWMP_LINKNODE **c = (struct CWMP_LINKNODE **)data;

		
		if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;	
#ifdef _PRMT_WT107_
		{
			//dynamic route
			unsigned int dyn_route_num=0;
			dyn_route_num=getDynamicForwardingTotalNum();
			if(dyn_route_num>0)
			{
				if( create_Object( c, tForwardingObject, sizeof(tForwardingObject), dyn_route_num, 1 ) < 0 )
					return -1;		
			}
			add_objectNum( name, MAX_DYNAMIC_ROUTE_INSTNUM );
		}
#endif //_PRMT_WT107_

		//MaxInstNum=findMaxForwardingInstNum();
		MaxInstNum=0;
#ifdef _PRMT_WT107_
		if(MaxInstNum<=MAX_DYNAMIC_ROUTE_INSTNUM) MaxInstNum=MAX_DYNAMIC_ROUTE_INSTNUM;
#endif //_PRMT_WT107_
		mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
		for( i=1; i<=num;i++ )
		{
			p = &route_entity;
			*((char *)p) = (char)i;
			memset( &target[0], 0, sizeof( STATICROUTE_T ) );
			memset( &target[1], 0, sizeof( STATICROUTE_T ) );
			if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
				continue;

			memcpy(&target[0], &route_entity, sizeof(STATICROUTE_T));
			p->InstanceNum=0;
			if( (p->InstanceNum==0) //maybe createn by web or cli
#ifdef _PRMT_WT107_
			    || (p->InstanceNum<=MAX_DYNAMIC_ROUTE_INSTNUM)
#endif //_PRMT_WT107_
			  )
			{
				MaxInstNum++;
				p->InstanceNum = MaxInstNum;
				memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
				mib_set(MIB_STATICROUTE_MOD, (void *)&target);
			}
			CWMPDBG( 1, ( stderr, "<%s:%d>name:%s(action:%d), 1 eCWMP_tINITOBJ p->InstanceNum=%d\n", __FUNCTION__, __LINE__, name,type,p->InstanceNum ) );
			if( create_Object( c, tForwardingObject, sizeof(tForwardingObject), 1, p->InstanceNum ) < 0 )
				return -1;
			CWMPDBG( 1, ( stderr, "<%s:%d>name:%s(action:%d), 2 eCWMP_tINITOBJ p->InstanceNum=%d\n", __FUNCTION__, __LINE__, name,type,p->InstanceNum ) );
		}
		add_objectNum( name, MaxInstNum );
		return 0;
	     }
	case eCWMP_tADDOBJ:
	     {
	     	int ret;
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;
		ret = add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tForwardingObject, sizeof(tForwardingObject), data );
		if( ret >= 0 )
		{
			STATICROUTE_T fentry;
			memset( &fentry, 0, sizeof( STATICROUTE_T ) );
			fentry.InstanceNum = *(unsigned int*)data;
			fentry.metric = -1;
			fentry.ifIndex = DUMMY_IFINDEX;
			mib_set(MIB_STATICROUTE_ADD, (void *)&fentry);
			ret=1;
		}
		
		return ret;
	     }
	case eCWMP_tDELOBJ:
	     {
	     	int ret;
	     	unsigned int id;
	     	STATICROUTE_T route_old;
	     	
	     	if( (name==NULL) || (entity==NULL) || (data==NULL) ) return -1;

#ifdef _PRMT_WT107_
		if( *(unsigned int*)data<=MAX_DYNAMIC_ROUTE_INSTNUM ) //dynamic route
			return ERR_9001;
#endif //_PRMT_WT107_

		if( getForwardingEntryByInstNum( *(unsigned int*)data, &route_old, &id )<0 )
			return ERR_9005;
#ifdef _CWMP_APPLY_
		apply_Layer3Forwarding( CWMP_STOP, id, &route_old );
#endif
		mib_set(MIB_STATICROUTE_DEL, (void *)&route_old);
		ret = del_Object( name, (struct CWMP_LINKNODE **)&entity->next, *(int*)data );
		
#ifndef _CWMP_APPLY_
		if( ret == 0 ) 
			return 1;
#endif
		return ret;
	     }
	case eCWMP_tUPDATEOBJ:	
	     {
	     	int num=0,i;
	     	struct CWMP_LINKNODE *old_table;
	     	
	     	old_table = (struct CWMP_LINKNODE*)entity->next;
	     	entity->next = NULL;

#ifdef _PRMT_WT107_
		num=getDynamicForwardingTotalNum();
	     	for( i=1; i<=num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;

			remove_entity = remove_SiblingEntity( &old_table, i );
			if( remove_entity!=NULL )
			{
				add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
			}else{ 
				unsigned int MaxInstNum=i;					
				add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tForwardingObject, sizeof(tForwardingObject), &MaxInstNum );
			}
	     	}
#endif //_PRMT_WT107_

		mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
	     	for( i=1; i<=num;i++ )
	     	{
	     		struct CWMP_LINKNODE *remove_entity=NULL;

			p = &route_entity;
			*((char *)p) = (char)i;
			memset( &target[0], 0, sizeof( STATICROUTE_T ) );
			memset( &target[1], 0, sizeof( STATICROUTE_T ) );
			
			if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
				continue;

			memcpy(&target[0], &route_entity, sizeof(STATICROUTE_T));
			
#ifdef _PRMT_WT107_
			if(p->InstanceNum<=MAX_DYNAMIC_ROUTE_INSTNUM) p->InstanceNum=0; //to get an new instnum > MAX_DYNAMIC_ROUTE_INSTNUM
#endif //_PRMT_WT107_			
			remove_entity = remove_SiblingEntity( &old_table, p->InstanceNum );
			if( remove_entity!=NULL )
			{
				add_SiblingEntity( (struct CWMP_LINKNODE **)&entity->next, remove_entity );
			}else{ 
				unsigned int MaxInstNum=p->InstanceNum;
					
				CWMPDBG( 1, ( stderr, "<%s:%d>name:%s(action:%d), 1 eCWMP_tUPDATEOBJ MaxInstNum=%d\n", __FUNCTION__, __LINE__, name,type,MaxInstNum ) );
				add_Object( name, (struct CWMP_LINKNODE **)&entity->next,  tForwardingObject, sizeof(tForwardingObject), &MaxInstNum );
				
				CWMPDBG( 1, ( stderr, "<%s:%d>name:%s(action:%d), 2 eCWMP_tUPDATEOBJ MaxInstNum=%d\n", __FUNCTION__, __LINE__, name,type,MaxInstNum ) );
				if(MaxInstNum!=p->InstanceNum)
				{
					p->InstanceNum = MaxInstNum;
					memcpy(&target[1], &route_entity, sizeof(STATICROUTE_T));
					mib_set(MIB_STATICROUTE_MOD, (void *)&target);
					
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

/**************************************************************************************/
/* utility functions*/
/**************************************************************************************/
int getChainID( struct CWMP_LINKNODE *ctable, int num )
{
	int id=-1;
	char buf[32];

//	sprintf( buf, "%d", num );
	while( ctable )
	{
		id++;
		//if( strcmp(ctable->name, buf)==0 )
		if( ctable->instnum==(unsigned int)num )
			break;
		ctable = ctable->sibling;
	}	
	return id;
}

int rtl_multiwan_if_index_transfer_name(unsigned long int index,char *name)
{
	char *wan_name = "var/wan_interface_";
	char buffer[40];
	char *tmp;
	FILE *fp;
	//char if_name[IFNAMSIZ];
	if (LAN_INDEX(index)){
		return -1;
	}
	memset(buffer,0,sizeof(buffer));
	sprintf( buffer, "%s%u",wan_name,index);
	fp = fopen( buffer, "r" );
	if(fp){
		memset(buffer,0,sizeof(buffer));
		fgets(buffer, sizeof(buffer), fp);
		sscanf(buffer, "%s",name);
		//memcpy(name,if_name,IFNAMSIZ);
		fclose(fp);
		return 0;
	}
	else
	return -1;
}
unsigned int rtl_multiwan_if_name_transfer_index(char *name)
{
	char *wan_name = "var/wan_interface_";
	char multiwan_name[IFNAMSIZ];
	char buffer[40];
	int i=0;
	char *tmp;
	FILE *fp;
	//char if_name[IFNAMSIZ];
	if(memcmp("br0",name,3)==0)
		return (1<<24);
	for(i=1;i<=4;i++)
	{
		memset(buffer,0,sizeof(buffer));
		memset(multiwan_name,0,IFNAMSIZ);
		sprintf( buffer, "%s%u",wan_name,i);
		fp = fopen( buffer, "r" );
		if(fp){
			memset(buffer,0,sizeof(buffer));
			fgets(buffer, sizeof(buffer), fp);
			sscanf(buffer, "%s",multiwan_name);
			//memcpy(name,if_name,IFNAMSIZ);
			fclose(fp);
			if(memcmp(multiwan_name,name,IFNAMSIZ)==0)
				return i;
		}
	}
	return DUMMY_IFINDEX;
}

int rtl_transfer_interface_index_to_name(char *name,STATICROUTE_T *pRoute)
{
	int skfd;
	struct ifreq ifr;
	struct sockaddr_in *addr;
	if((skfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	{
		printf("  ioctl SOCK_DGRAM fail!\n");  
		return -1;
	}
	ifr.ifr_ifindex=pRoute->ifIndex;  
   	if (ioctl(skfd, SIOCGIFNAME, &ifr) < 0){
    		close( skfd );
		printf("  ioctl SIOCGIFNAME fail!\n");  
		return -1;
	}
	close(skfd);
	memcpy(name,ifr.ifr_name,IFNAMSIZ);
	return 0;
}

//return -1:error,  0:not found, 1:found in /proc/net/route
//refer to user/busybox/route.c:displayroutes()
int queryRouteStatus( STATICROUTE_T *p )
{
	int ret=-1;
	//fprintf( stderr, "<%s:%d>Start\n",__FUNCTION__,__LINE__ );
	if(p)
	{
		char *routename="/proc/net/route";
		FILE *fp;
		
		fp=fopen( routename, "r" );
		if(fp)
		{
			char buff[256];
			int  i=0;
			while( fgets(buff, sizeof(buff), fp) != NULL )
			{
				int flgs, ref, use, metric;
				char ifname[IFNAMSIZ], ifname2[IFNAMSIZ]; 
				unsigned long int d,g,m;

				//fprintf( stderr, "<%s:%d>%s\n",__FUNCTION__,__LINE__,buff );
				i++;
				if(i==1) continue; //skip the first line
				 
				//Iface Destination Gateway Flags RefCnt Use Metric Mask MTU Window IRTT
				if(sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
					ifname, &d, &g, &flgs, &ref, &use, &metric, &m)!=8)
					break;//Unsuported kernel route format
				
				//if( ifGetName( p->ifIndex, ifname2, sizeof(ifname2) )==0 ) //any interface
				memset(ifname2,0,sizeof(ifname2));
				rtl_transfer_interface_index_to_name(ifname2,p);
	
				//fprintf( stderr, "<%s:%d>%s==%s(%d)\n",__FUNCTION__,__LINE__,ifname,ifname2, ( (strlen(ifname2)==0) || (strcmp(ifname, ifname2)==0) ) );
				//fprintf( stderr, "<%s:%d>%06x==%06x(%d)\n",__FUNCTION__,__LINE__,*((unsigned long int*)(p->destID)),d,(*((unsigned long int*)(p->destID))==d) );
				//fprintf( stderr, "<%s:%d>%06x==%06x(%d)\n",__FUNCTION__,__LINE__,*((unsigned long int*)(p->netMask)),m,(*((unsigned long int*)(p->netMask))==m) );
				//fprintf( stderr, "<%s:%d>%06x==%06x(%d)\n",__FUNCTION__,__LINE__,*((unsigned long int*)(p->nextHop)),g,(*((unsigned long int*)(p->nextHop))==g) );
				//fprintf( stderr, "<%s:%d>%d==%d(%d)\n",__FUNCTION__,__LINE__, p->FWMetric, metric,( (p->FWMetric==-1 && metric==0) || (p->FWMetric==metric )	) );
				if(  (p->Enable) &&
				     ( (p->ifIndex==DUMMY_IFINDEX) || (strcmp(ifname, ifname2)==0) ) && //interface: any or specific
				     (*((unsigned long int*)(p->dstAddr))==d) &&  //destIPaddress
				     (*((unsigned long int*)(p->netmask))==m) && //netmask
				     (*((unsigned long int*)(p->gateway))==g) && //GatewayIPaddress
				     ( (p->metric==-1 && metric==0) || (p->metric==metric )	) //ForwardingMetric
				  )
				{
					//fprintf( stderr, "<%s:%d>Found\n",__FUNCTION__,__LINE__ );
					ret=1;//found
					break;
				}
			}
			
			if( feof(fp) && (ret!=1) )
			{
					//fprintf( stderr, "<%s:%d>Not Found\n",__FUNCTION__,__LINE__ );
					ret=0; //not found				
			}
			fclose(fp);
		}
	}

	//fprintf( stderr, "<%s:%d>End(ret:%d)\n",__FUNCTION__,__LINE__,ret );
	
	return ret;
}

unsigned int getForwardingInstNum( char *name )
{
	return getInstNum( name, "Forwarding" );
}

unsigned int findMaxForwardingInstNum(void)
{
	unsigned int ret=0, i,num;
	STATICROUTE_T *p,entity;

	mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
	for( i=1; i<=num;i++ )
	{
		p = &entity;
		
		*((char *)p) = (char)i;
		if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
			continue;
		
		if( p->InstanceNum > ret )
			ret = p->InstanceNum;
	}
	return ret;
}

int getForwardingEntryByInstNum( unsigned int instnum, STATICROUTE_T *p, unsigned int change_index )
{
	int ret=-1;
	unsigned int i,num;
	char if_name[IFNAMSIZ];
	int skfd;
	struct ifreq ifr;
	struct sockaddr_in *addr;
	if( (instnum==0) || (p==NULL))
		return ret;
	
	mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
	for( i=1; i<=num;i++ )
	{
		*((char *)p) = (char)i;
		if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
			continue;
		//currently, configure static route from acs, only support wan interface route 
		if (LAN_INDEX(p->ifIndex)){
			printf("%s %d skip lan route set from web ui\n", __FUNCTION__, __LINE__);
			continue;
		}
		
		if( p->InstanceNum==instnum )
		{
			ret = 0;
			break;
		}
	}
	if(change_index==0)
		return ret;
	memset(if_name,0,sizeof(if_name));
	rtl_multiwan_if_index_transfer_name(p->ifIndex,if_name);
	if((skfd = socket(AF_INET, SOCK_DGRAM, 0))<0){
		printf("  ioctl SOCK_DGRAM fail!\n");  
		return ret;
	}
	memcpy(ifr.ifr_name,if_name,IFNAMSIZ);
	if( ioctl(skfd, SIOCGIFINDEX, &ifr ) < 0 ){
    		close(skfd);
		printf("  ioctl SIOCGIFFLAGSl fail111!\n");  
		return ret;
	}
	close(skfd);
	p->ifIndex=ifr.ifr_ifindex;
	return ret;
}
#ifdef _PRMT_WT107_
int getDynamicForwardingTotalNum(void)
{
	int ret=0;
	char *routename="/proc/net/route";
	FILE *fp;
	fp=fopen( routename, "r" );
	if(fp)
	{
		char buff[256];
		
		fgets(buff, sizeof(buff), fp);
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{
			int flgs, ref, use, metric;
			char ifname[IFNAMSIZ], ifname2[IFNAMSIZ]; 
			unsigned long int d,g,m;
			memset(ifname2,0,sizeof(ifname2));
			if(sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
				ifname, &d, &g, &flgs, &ref, &use, &metric, &m)!=8)
				break;//Unsuported kernel route format
			
			{
				unsigned int i,num, is_match_static=0;
				STATICROUTE_T *p,route_entity;
				p = &route_entity;
				#if 0
				num = mib_chain_total( MIB_IP_ROUTE_TBL );
				for( i=0; i<num;i++ )
				{
					p = &entity;
					if( !mib_chain_get( MIB_IP_ROUTE_TBL, i, (void*)p ))
						continue;
						
					if( ifGetName( p->ifIndex, ifname2, sizeof(ifname2) )==0 ) //any interface
						ifname2[0]=0;
		
					if(  (p->Enable) &&
					     ( (p->ifIndex==DUMMY_IFINDEX) || (strcmp(ifname, ifname2)==0) ) && //interface: any or specific
					     (*((unsigned long int*)(p->destID))==d) &&  //destIPaddress
					     (*((unsigned long int*)(p->netMask))==m) && //netmask
					     (*((unsigned long int*)(p->nextHop))==g) && //GatewayIPaddress
					     ( (p->FWMetric==-1 && metric==0) || (p->FWMetric==metric )	) //ForwardingMetric
					  )
					{
						is_match_static=1; //static route
						break;
					}
					
				}
				#else
				mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
				for( i=1; i<=num;i++ )
				{
					*((char *)p) = (char)i;
					if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
						continue;
					//if( ifGetName( p->ifIndex, ifname2, sizeof(ifname2) )==0 ) //any interface
					rtl_multiwan_if_index_transfer_name(p->ifIndex,ifname2);
					if(  (p->Enable) &&
					     ( (p->ifIndex==DUMMY_IFINDEX) || (strcmp(ifname, ifname2)==0) ) && //interface: any or specific
					     (*((unsigned long int*)(p->dstAddr))==d) &&  //destIPaddress
					     (*((unsigned long int*)(p->netmask))==m) && //netmask
					     (*((unsigned long int*)(p->gateway))==g) && //GatewayIPaddress
					     ( (p->metric==-1 && metric==0) || (p->metric==metric )	) //ForwardingMetric
					  )
					{
						is_match_static=1; //static route
						break;
					}
				}
				#endif
				
				if(is_match_static==0) ret++; //dynamic route
			}
		}
		fclose(fp);
	}
	return ret;
}
int rtl_get_default_route(char *name)
{
	int ret=-1;
	char *routename="/proc/net/route";
	FILE *fp;
	fp=fopen( routename, "r" );
	if(fp)
	{
		char buff[256];
		fgets(buff, sizeof(buff), fp);
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{
			int flgs, ref, use, metric;
			unsigned long int d,g,m;
			if(sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
				name, &d, &g, &flgs, &ref, &use, &metric, &m)!=8)
				break;//Unsuported kernel route format
			if((d==0) && (m==0))
			{
				fclose(fp);	
				return 0;
			}
				
	
			
		}
		fclose(fp);
	}	
	
	return ret;
}

int getDynamicForwardingEntryByInstNum( unsigned int instnum, STATICROUTE_T *pRoute )
{
	int ret=-1;
	char *routename="/proc/net/route";
	FILE *fp;
	if( (instnum==0) || (pRoute==NULL) ) return ret;
	memset( pRoute, 0, sizeof(STATICROUTE_T) );
	//pRoute->InstanceNum = instnum;
	pRoute->metric = -1;
	pRoute->ifIndex = DUMMY_IFINDEX;

	fp=fopen( routename, "r" );
	if(fp)
	{
		char buff[256];
		int count=0;
		
		fgets(buff, sizeof(buff), fp);
		while( fgets(buff, sizeof(buff), fp) != NULL )
		{
			int flgs, ref, use, metric;
			char ifname[IFNAMSIZ], ifname2[IFNAMSIZ]; 
			unsigned long int d,g,m;
			memset(ifname2,0,sizeof(ifname2));
			if(sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
				ifname, &d, &g, &flgs, &ref, &use, &metric, &m)!=8)
				break;//Unsuported kernel route format
			
			{
				unsigned int i,num, is_match_static=0;
				#if 0
				MIB_CE_IP_ROUTE_T *p,entity;
				
				num = mib_chain_total( MIB_IP_ROUTE_TBL );
				for( i=0; i<num;i++ )
				{
					p = &entity;
					if( !mib_chain_get( MIB_IP_ROUTE_TBL, i, (void*)p ))
						continue;
						
					if( ifGetName( p->ifIndex, ifname2, sizeof(ifname2) )==0 ) //any interface
						ifname2[0]=0;

					if(  (p->Enable) &&
					     ( (p->ifIndex==DUMMY_IFINDEX) || (strcmp(ifname, ifname2)==0) ) && //interface: any or specific
					     (*((unsigned long int*)(p->destID))==d) &&  //destIPaddress
					     (*((unsigned long int*)(p->netMask))==m) && //netmask
					     (*((unsigned long int*)(p->nextHop))==g) && //GatewayIPaddress
					     ( (p->FWMetric==-1 && metric==0) || (p->FWMetric==metric )	) //ForwardingMetric
					  )
					{
						is_match_static=1; //static route
						break;
					}
				}
				#else
				STATICROUTE_T *p,route_entity;
				p = &route_entity;
				mib_get(MIB_STATICROUTE_TBL_NUM, (void *)&num);
				for( i=1; i<=num;i++ )
				{
					*((char *)p) = (char)i;
					if ( !mib_get(MIB_STATICROUTE_TBL, (void *)p))
						continue;
					//if( ifGetName( p->ifIndex, ifname2, sizeof(ifname2) )==0 ) //any interface
						//ifname2[0]=0;
					rtl_multiwan_if_index_transfer_name(p->ifIndex,ifname2);
					if(  (p->Enable) &&
					     ( (p->ifIndex==DUMMY_IFINDEX) || (strcmp(ifname, ifname2)==0) ) && //interface: any or specific
					     (*((unsigned long int*)(p->dstAddr))==d) &&  //destIPaddress
					     (*((unsigned long int*)(p->netmask))==m) && //netmask
					     (*((unsigned long int*)(p->gateway))==g) && //GatewayIPaddress
					     ( (p->metric==-1 && metric==0) || (p->metric==metric )	) //ForwardingMetric
					  )
					{
						is_match_static=1; //static route
						break;
					}
				}
				
				#endif
				if(is_match_static==0) count++; //dynamic route
			}
			
			if(count==instnum)
			{
				/*if( (strncmp(ifname,"ppp",3)==0) || (strncmp(ifname,"vc",2)==0) )
					pRoute->ifIndex = transfer2IfIndxfromIfName(ifname);
				else
					pRoute->ifIndex = DUMMY_IFINDEX;*/
				*((unsigned long int*)(pRoute->dstAddr))=d;
				*((unsigned long int*)(pRoute->netmask))=m;
				*((unsigned long int*)(pRoute->gateway))=g;
				pRoute->metric=metric;
				pRoute->Enable = (flgs&RTF_UP)?1:0;
				if(flgs&RTF_HOST)
					pRoute->Type = 1; //host
				else if( (d==0) && (m==0) )
					pRoute->Type = 2; //default
				else
					pRoute->Type = 0; //network
				int skfd;
				struct ifreq ifr;
				struct sockaddr_in *addr;
				if((skfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
				{
					printf("  ioctl SOCK_DGRAM fail!\n");  
					break;
				}
				strcpy(ifr.ifr_name, ifname);
				if( ioctl(skfd, SIOCGIFINDEX, &ifr ) < 0 )
				{
    					close(skfd);
					printf("  ioctl SIOCGIFFLAGSl fail!\n");  
					break;
				}
				close(skfd);
				pRoute->ifIndex=ifr.ifr_ifindex;
				break;
			}
		}
		fclose(fp);
	}	
	
	return ret;
}
#endif //_PRMT_WT107_
#undef _PRMT_WT107_

