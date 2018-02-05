#include "reinitSer.h"
#define FUNC_NODE_BUFF_LEN 128

#define FUNC_NODE_BUFF_FORMAT "wlanIdx=%d vwlanIdx=%d wanIdx=%d funcId=%d\n"
mibChangeList_t mibChangelist={.count=-1,.changeNodeHead={0}};
reinit_func_list_t reinitFuncList={.count=-1,.funcNodeHead={0}};
int debugLevel=REINIT_DEF_LOG_LEVEL;
/*
* 	degbug level define in syslog.h:
*	#define LOG_EMERG       	0        system is unusable 
*	#define LOG_ALERT       	1       action must be taken immediately 
*	#define LOG_CRIT        	2        critical conditions 
*	#define LOG_ERR         		3        error conditions 
*	#define LOG_WARNING     	4        warning conditions 
*	#define LOG_NOTICE      	5        normal but significant condition 
*	#define LOG_INFO        		6        informational 
*	#define LOG_DEBUG       	7        debug-level messages 
*/
int reinitSer_printf(int level,char* format,...)
{
	va_list argptr; 
	int cnt;

	//fprintf(stderr,"\n reinitSer_printf %d\n",debugLevel);

	if(level>debugLevel) return 0;
	va_start(argptr, format); 
	cnt = vfprintf(stderr, format, argptr); 
	va_end(argptr);

	return(cnt); 
}

MibChangeFuncItem_tp get_mibDecisionFuncSet_item(MIB_CHANGE_FUNC_ID id)
{
	int i=0;
	if(id>=DECISION_END_FUNC_ID||id<=DECISION_NULL_FUNC_ID)
	{		
		reinitSer_printf(LOG_ERR,"%s:%d Invalid input func id %d!\n",__FUNCTION__,__LINE__,id);
		return NULL;
	}
	for(i=0;mibDecisionFuncSet[i].change_func_id!=0;i++)
	{
		if(mibDecisionFuncSet[i].change_func_id==id)
			return &mibDecisionFuncSet[i];
	}
	reinitSer_printf(LOG_ERR,"%s:%d Invalid input func id %d! Not find the function map to the function id!\n",__FUNCTION__,__LINE__,id);
	return NULL;
}
MibChangeFuncItem_tp get_mibReinitFuncSet_item(MIB_CHANGE_FUNC_ID id)
{
	int i=0;
	if(id>=REINIT_END_FUNC_ID||id<=REINIT_NULL_FUNC_ID)
	{		
		reinitSer_printf(LOG_ERR,"%s:%d Invalid input func id %d!\n",__FUNCTION__,__LINE__,id);
		return NULL;
	}
	for(i=0;mibReinitFuncSet[i].change_func_id!=0;i++)
	{
		if(mibReinitFuncSet[i].change_func_id==id)
			return &mibReinitFuncSet[i];
	}
	reinitSer_printf(LOG_ERR,"%s:%d Invalid input func id %d! Not find the function map to the function id!\n",__FUNCTION__,__LINE__,id);
	return NULL;
}


/*void dumpMibChangeNodeParmList(struct list_head *head)
{
	MibChangeNode_tp pMibChangeNode=NULL;
	reinit_func_parm_list_node_tp parmListNode=NULL;
	reinit_func_parm_list_node_tp tmp=NULL;
	//reinitSer_printf(LOG_DEBUG,"%s:%d head=%p next=%p prev=%p\n",__FUNCTION__,__LINE__,head,head->next,head->prev);

	list_for_each_entry_safe(parmListNode,tmp,head,parm_list)
	{
		//reinitSer_printf(LOG_DEBUG,"%s:%d head=%p next=%p prev=%p\n",__FUNCTION__,__LINE__,&parmListNode->parm_list,parmListNode->parm_list.next,parmListNode->parm_list.prev);
		pMibChangeNode=parmListNode->mibChangeNode;
		reinitSer_printf(LOG_DEBUG,"%s:%d dumpMibChangeNodeParmList----------\n",__FUNCTION__,__LINE__);
		reinitSer_printf(LOG_DEBUG,"%s:%d mibid=%d\n",__FUNCTION__,__LINE__,pMibChangeNode->id);
		reinitSer_printf(LOG_DEBUG,"%s:%d index=%d,%d,%d\n",__FUNCTION__,__LINE__,pMibChangeNode->mibChangeNode_wlan_idx,
		pMibChangeNode->mibChangeNode_vwlan_idx,pMibChangeNode->mibChangeNode_wan_idx);
		reinitSer_printf(LOG_DEBUG,"%s:%d func_id=%d\n",__FUNCTION__,__LINE__,pMibChangeNode->change_func_id);
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
}*/
int get_reinit_event_mibchange(MibChangeNode_tp pMibChangeNode,char * data)
{
	char buff[512]={0};
	char tmpbuf[8]={0};
	FILE *fp=NULL;
	char * idx=NULL;
	char *token=NULL;
	char *savestr=NULL;
	int i=0;
	bzero(pMibChangeNode,sizeof(MibChangeNode_t));
	reinitSer_printf(LOG_DEBUG,"%s:%d data=%s\n",__FUNCTION__,__LINE__,data);

	token=strtok_r(data,",",&savestr);
	while(token!=NULL)
	{		
		reinitSer_printf(LOG_DEBUG,"%s:%d ###%s\n",__FUNCTION__,__LINE__,token);
		if(strncmp("id=",token,strlen("id="))==0)
		{
			pMibChangeNode->id=strtol(token+strlen("id="),NULL,16);
		}
		else if(strncmp("length=",token,strlen("length="))==0)
		{
			pMibChangeNode->length=strtol(token+strlen("length="),NULL,16);
		}
		else if(strncmp("value=",token,strlen("value="))==0)
		{
			pMibChangeNode->value=(char*)malloc(pMibChangeNode->length);
			if(!pMibChangeNode->value) {reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);exit(0);}
			
			bzero(pMibChangeNode->value,pMibChangeNode->length);

			idx=token+strlen("value=");
			for(i=0;i<pMibChangeNode->length;i++)
			{
				bzero(tmpbuf,sizeof(tmpbuf));
				memcpy(tmpbuf,idx,2);
				reinitSer_printf(LOG_DEBUG,"%s:%d hex=%s\n",__FUNCTION__,__LINE__,tmpbuf);
				pMibChangeNode->value[i]=(char)strtol(tmpbuf,NULL,16);
				//sscanf(tmpbuf,"%x",&pMibChangeNode->value[i]);
				reinitSer_printf(LOG_DEBUG,"%s:%d value=%02x\n",__FUNCTION__,__LINE__,pMibChangeNode->value[i]);
				idx+=3;
			}
		}
		else if(strncmp("mibChangeNode_wlan_idx=",token,strlen("mibChangeNode_wlan_idx="))==0)
		{
			pMibChangeNode->mibChangeNode_wlan_idx=strtol(token+strlen("mibChangeNode_wlan_idx="),NULL,16);
		}
		else if(strncmp("mibChangeNode_vwlan_idx=",token,strlen("mibChangeNode_vwlan_idx="))==0)
		{
			pMibChangeNode->mibChangeNode_vwlan_idx=strtol(token+strlen("mibChangeNode_vwlan_idx="),NULL,16);
		}
		else if(strncmp("mibChangeNode_wan_idx=",token,strlen("mibChangeNode_wan_idx="))==0)
		{
			pMibChangeNode->mibChangeNode_wan_idx=strtol(token+strlen("mibChangeNode_wan_idx="),NULL,16);
		}
		else if(strncmp("change_func_id=",token,strlen("change_func_id="))==0)
		{
			pMibChangeNode->change_func_id=strtol(token+strlen("change_func_id="),NULL,16);
			if(pMibChangeNode->change_func_id<=DECISION_NULL_FUNC_ID || 
				pMibChangeNode->change_func_id>=REINIT_END_FUNC_ID ||
				pMibChangeNode->change_func_id==DECISION_END_FUNC_ID||
				pMibChangeNode->change_func_id==REINIT_BEGIN_FUNC_ID)
				return -1;
		}
		token=strtok_r(NULL,",",&savestr);
	}
	
	return 0;

}

int mibChangeNode_isTheSame(MibChangeNode_tp pnode1,MibChangeNode_tp pnode2)
{
	if(pnode1->id==pnode2->id &&
			pnode1->mibChangeNode_wlan_idx==pnode2->mibChangeNode_wlan_idx &&
			//pnode1->mibChangeNode_vwlan_idx==pnode2->mibChangeNode_vwlan_idx &&
			pnode1->mibChangeNode_wan_idx==pnode2->mibChangeNode_wan_idx)
			return 1;
	return 0;
}
int reinit_mibChange_addToList(MibChangeNode_tp pMibChangeNode)
{
	MibChangeNode_tp pnode=NULL;
	if(!pMibChangeNode)
	{		
		return -1;
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	list_for_each_entry(pnode,(&(mibChangelist.changeNodeHead.list)),list)
	{
		reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
		if(mibChangeNode_isTheSame(pnode,pMibChangeNode))
		{
			//the same mib, not need to add, free the node and return;
			if(pMibChangeNode->value) free(pMibChangeNode->value);
			free(pMibChangeNode);
			reinitSer_printf(LOG_DEBUG,"Needn't to add to mibchange list\n");
			return 1;
		}
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	list_add_tail(&(pMibChangeNode->list),&(mibChangelist.changeNodeHead.list));
	
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	mibChangelist.count++;	
	return 0;
}
#if 0
MibChangeNode_tp reinit_clone_changeNode(MibChangeNode_tp origChangeNode)
{
	MibChangeNode_tp cloneChangeNode=NULL;
	reinitSer_printf(LOG_DEBUG,"%s:%d origChangeNode:\n",__FUNCTION__,__LINE__);
	if(!origChangeNode)
		return NULL;
	//dumpMibChangeNodeParmList(origChangeNode);
	cloneChangeNode=(MibChangeNode_tp)malloc(sizeof(MibChangeNode_t));
	if(!cloneChangeNode) 
	{
		reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	memcpy(cloneChangeNode,origChangeNode,sizeof(MibChangeNode_t));
	cloneChangeNode->value=(unsigned char *)malloc(cloneChangeNode->length);
	if(!cloneChangeNode->value && cloneChangeNode->length>0) 
	{
		reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	memcpy(cloneChangeNode->value,origChangeNode->value,cloneChangeNode->length);
	reinitSer_printf(LOG_DEBUG,"%s:%d cloneChangeNode:\n",__FUNCTION__,__LINE__);
	//dumpMibChangeNodeParmList(cloneChangeNode);

	return cloneChangeNode;
}

reinit_func_parm_list_node_tp generate_reinit_func_parm_list_node(MibChangeNode_tp mibChangeNode)
{
	reinit_func_parm_list_node_tp reinit_func_parm_list_node=NULL;
	if(!mibChangeNode)
	{
		return NULL;
	}
	reinit_func_parm_list_node=(reinit_func_parm_list_node_tp)malloc(sizeof(reinit_func_parm_list_node_t));
	if(!reinit_func_parm_list_node)
	{
		reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	bzero(reinit_func_parm_list_node,sizeof(reinit_func_parm_list_node_t));

	reinit_func_parm_list_node->mibChangeNode=mibChangeNode;
	return reinit_func_parm_list_node;
}
#endif

reinit_func_list_node_tp generate_reinit_func_list_node(MibChangeNode_tp mibChangeNode,int reinit_funcId,BASE_DATA_T* base)
{//generate reinit function list node from mibChange node, 
	reinit_func_list_node_tp reinit_func_node=NULL;
	//reinit_func_parm_list_node_tp reinit_func_parm_list_node=NULL;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(!mibChangeNode)
	{
		reinitSer_printf(LOG_ERR,"%s:%d Invalid input!\n",__FUNCTION__,__LINE__);
		return NULL;
	}
	reinit_func_node=(reinit_func_list_node_tp)malloc(sizeof(reinit_func_list_node_t));
	if(!reinit_func_node)
	{
		reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	bzero(reinit_func_node,sizeof(reinit_func_list_node_t));

	if(reinit_funcId>0)
	{		
		reinit_func_node->funcItem=get_mibReinitFuncSet_item(reinit_funcId);
	}else
	{		
		reinit_func_node->funcItem=get_mibReinitFuncSet_item(mibChangeNode->change_func_id);
	}
	if(!reinit_func_node->funcItem)
		return NULL;
	
	if(base)
	{
		memcpy(&(reinit_func_node->base),base,sizeof(BASE_DATA_T));
	}else
	{
		memcpy(&(reinit_func_node->base),&(mibChangeNode->base),sizeof(BASE_DATA_T));
	}
	//reinit_func_parm_list_node=generate_reinit_func_parm_list_node(mibChangeNode);
	//if(!reinit_func_parm_list_node||!reinit_func_node->funcItem)
	//	return NULL;
	//INIT_LIST_HEAD(&(reinit_func_node->parm_list));
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	//dumpMibChangeNodeParmList(&(reinit_func_node->parm_list));
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	//list_add_tail(&(reinit_func_parm_list_node->parm_list),&(reinit_func_node->parm_list));
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	//dumpMibChangeNodeParmList(&(reinit_func_node->parm_list));
	//reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_func_node->mibChangeList=&(mibChangelist.changeNodeHead.list);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return reinit_func_node;
}



MibChangeNode_tp generate_MibChangeNode()
{
	MibChangeNode_tp mibChangeNode=(MibChangeNode_tp)malloc(sizeof(MibChangeNode_t));
	if(!mibChangeNode)
	{
		reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	return mibChangeNode;
}

void free_MibChangeNode(MibChangeNode_tp pnode)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(!pnode) return;
	if(pnode->value)
		free(pnode->value);
	if(pnode)
		free(pnode);	
}

/*void free_reinit_func_parm_list_node(reinit_func_parm_list_node_tp pnode)
{
	if(pnode)
		free(pnode);
}*/
void free_reinit_func_list_node(reinit_func_list_node_tp pFuncListNode)
{
	//reinit_func_parm_list_node_tp ptmp=NULL;
	//reinit_func_parm_list_node_tp pnode=NULL;
	//reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(!pFuncListNode) return;
	//free parm_changeNode_list, only reinit_func_list_node have the list
	
	/*
	list_for_each_entry_safe(pnode,ptmp,(&pFuncListNode->parm_list),parm_list)
	{
		//dumpMibChangeNodeParmList(&pFuncListNode->parm_list);
		reinitSer_printf(LOG_DEBUG,"%s:%d free pram %d from %s\n",__FUNCTION__,__LINE__,pnode->mibChangeNode->id,pFuncListNode->funcItem->name);
		
		list_del(&pnode->parm_list);
		reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
		//dumpMibChangeNodeParmList(&pnode->parm_list);
		free_reinit_func_parm_list_node(pnode);
	}*/
	//dumpMibChangeNodeParmList(&pFuncListNode->parm_list);

	free(pFuncListNode);
}


int del_mibChangeListNode(MibChangeNode_tp pnode)
{
	if(!pnode) return 0;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	list_del(&pnode->list);
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	free_MibChangeNode(pnode);
	mibChangelist.count--;
	return 0;
}
int reinit_clear_mibChangeList()
{
	MibChangeNode_tp ptmp=NULL;
	MibChangeNode_tp pnode=NULL;
	reinitSer_printf(LOG_DEBUG,"%s:%d count=%d\n",__FUNCTION__,__LINE__,mibChangelist.count);

	list_for_each_entry_safe(pnode,ptmp,(&mibChangelist.changeNodeHead.list),list)
	{
		reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
		del_mibChangeListNode(pnode);
	}
	return 0;
}

/***************************************************
** 	check whether mibchenge list node really have chenged
****************************************************/
int reinit_check_mibChangeList_change()
{
	MibChangeNode_tp ptmp=NULL;
	MibChangeNode_tp pnode=NULL;
	char * new_value=NULL;
	unsigned char ch=0;
	unsigned short wd=0;
	int mibDataSize=sizeof(int);
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	list_for_each_entry_safe(pnode,ptmp,&(mibChangelist.changeNodeHead.list),list)
	{
		if(pnode->id & MIB_TABLE_LIST)
		{//for table arrary, treat  it always change
			continue;
		}
		mibDataSize=mibDataSize > (pnode->length)?mibDataSize:(pnode->length);
		//at least sizeof(int) for apmib_get value size is at least sizeof(int)
		new_value=(char*)malloc(mibDataSize);
		if(!new_value) {reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);exit(0);}
		bzero(new_value,mibDataSize);
		apmib_rtkReinit_get(pnode->id,new_value,pnode->mibChangeNode_wlan_idx,pnode->mibChangeNode_vwlan_idx,pnode->mibChangeNode_wan_idx);
		if(pnode->length==1)//byte
		{
			ch= *((unsigned int*)new_value);
			*((unsigned char*)new_value)=ch;
		}
		else if(pnode->length==2)//word
		{
			wd=*((unsigned int*)new_value);
			*((unsigned short*)new_value)=wd;
		}

		if(memcmp(new_value,pnode->value,pnode->length)==0)
		{//not change, should delete it 
			del_mibChangeListNode(pnode);
		}
		free(new_value);
	}
	return 0;
}


int effctFuncNode_priorityHighThan(reinit_func_list_node_tp pnode1,reinit_func_list_node_tp pnode2)
{
#if 0
	if(pnode1->funcItem == get_mibReinitFuncSet_item(REINIT_END_FUNC_ID))
		return 0;
	else if(pnode2->funcItem==get_mibReinitFuncSet_item(REINIT_END_FUNC_ID))
		return 1;
#endif
	
	if(pnode1->funcItem->priority < pnode2->funcItem->priority)
		return 1;
	else if(pnode1->funcItem->priority > pnode2->funcItem->priority)
		return 0;

	else if(pnode1->base.wlan_idx!= pnode2->base.wlan_idx)
	{
		if(pnode1->base.wlan_idx==0)
			return 1;
		else if(pnode2->base.wlan_idx==0)
			return 0;
	}
	else if(pnode1->base.wan_idx!= pnode2->base.wan_idx)
	{
		if(pnode1->base.wan_idx==0)
			return 1;
		else if(pnode2->base.wan_idx==0)
			return 0;
	}

	return 0;
}

int reinitFuncNode_isTheSame(reinit_func_list_node_tp pnode1,reinit_func_list_node_tp pnode2)
{
	if(pnode1->funcItem==pnode2->funcItem &&
		pnode1->base.wlan_idx==pnode2->base.wlan_idx &&
//		pnode1->base.vwlan_idx==pnode2->base.vwlan_idx &&
		pnode1->base.wan_idx==pnode2->base.wan_idx)
		return 1;
	return 0;
}


int reinit_add_reinitFunc_to_list(reinit_func_list_node_tp pFuncListNode)
{
	reinit_func_list_node_tp pnode=NULL;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(!pFuncListNode)
	{		
		reinitSer_printf(LOG_ERR,"invalid input! \n");
		return -1;
	}
	list_for_each_entry(pnode,&(reinitFuncList.funcNodeHead.list),list)
	{
		if(reinitFuncNode_isTheSame(pnode,pFuncListNode))
		{//the same node,not need to add to reinit func list
			
			//reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
			//dumpMibChangeNodeParmList(&pnode->parm_list);
			//reinitSer_printf(LOG_DEBUG,"%s:%d add %d to %s\n",__FUNCTION__,__LINE__,pFuncListNode->changeNode->id,pnode->funcItem->name);
			//list_splice_tail_init(&(pFuncListNode->parm_list),&(pnode->parm_list));
			
			//reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
			//dumpMibChangeNodeParmList(&pnode->parm_list);
			reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
			free_reinit_func_list_node(pFuncListNode);
			reinitSer_printf(LOG_INFO,"Needn't to add to reinit func list\n");
			return 1;
		}

		if(effctFuncNode_priorityHighThan(pFuncListNode,pnode))
		{
			break;
		}
	}
	list_add_tail(&(pFuncListNode->list),&(pnode->list));	
	reinitFuncList.count++;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	reinitSer_printf(LOG_DEBUG,"%s:%d add %d to %s\n",__FUNCTION__,__LINE__,pFuncListNode->funcItem->change_func_id,pFuncListNode->funcItem->name);
	//dumpMibChangeNodeParmList(&pFuncListNode->parm_list);
	return 0;
}


int reinit_run_decision_func(MibChangeNode_tp pMibChangeNode)
{
	int fd[2]={0};
	int pid=0,status=0;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(pipe(fd)<0)
	{
		reinitSer_printf(LOG_ERR,"%s:%d reinit_run_decision_func fail\n",__FUNCTION__,__LINE__);
		return -1;
	}

	switch(pid=fork())
	{
		case -1:
			reinitSer_printf(LOG_ERR,"fork fail!!!\n");
            return -1;
		case 0://child
			{

				MibChangeFuncItem_tp funcItem=get_mibDecisionFuncSet_item(pMibChangeNode->change_func_id);
				if(!funcItem||!funcItem->mib_change_func)
					exit(0);
				close(fd[0]);//child process write pipe
				pMibChangeNode->parentPipeFd=fd[1];

				reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
				funcItem->mib_change_func((BASE_DATA_Tp)pMibChangeNode);
				close(fd[1]);
				exit(0);
			}
		default://parent
			{
				reinit_func_list_node_tp pFuncListNode=NULL;
				BASE_DATA_T base={0};
				int funcId=0;
				FILE * fp=NULL;
				char readBuf[FUNC_NODE_BUFF_LEN]={0};
				FILE* fstream=NULL;
				int count=0;
				MibChangeFuncItem_tp decFuncItem=get_mibDecisionFuncSet_item(pMibChangeNode->change_func_id);
				fd_set rset;
				struct timeval timeOut={0};
				int selRet=0,rdRet=0;
				int flag=fcntl(fd[0],F_GETFL,0);
				char *tmpStr=NULL,*rdIdx=NULL;
				
				close(fd[1]);//parent read only
				
				reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
				if(fcntl(fd[0],F_SETFL,flag|O_NONBLOCK) < 0){
				   reinitSer_printf(LOG_ALERT,"%s:%d fcntl fail!\n",__FUNCTION__,__LINE__);
				   return -1;
				}			
				fp=fdopen(fd[0],"r");
				if(!fp)
				{					
					reinitSer_printf(LOG_ALERT,"%s:%d open pipe fail!\n",__FUNCTION__,__LINE__);
					return -1;
				}
				reinitSer_printf(LOG_DEBUG,"%s:%d parent wait for client data or timeout\n",__FUNCTION__,__LINE__);

				do
				{
					FD_ZERO(&rset);
					FD_SET(fd[0],&rset);
					timeOut.tv_sec=0;
					timeOut.tv_usec=100000;//0.1s
					reinitSer_printf(LOG_DEBUG,"%s:%d count=%d\n",__FUNCTION__,__LINE__,count);
					count++;

					selRet=select(fd[0]+1,&rset,NULL,NULL,&timeOut);
					
					if(selRet==0)// time out 
						continue;

					usleep(ERINIT_FUNC_TIMEOUT_USLEEP);
					if(FD_ISSET(fd[0],&rset))
					{//get value	
						reinitSer_printf(LOG_DEBUG,"%s:%d parent(pipe read) get value\n",__FUNCTION__,__LINE__);
						bzero(readBuf,sizeof(readBuf));
						while(fgets(readBuf,sizeof(readBuf),fp))
						{
							reinitSer_printf(LOG_DEBUG,"%s:%d read reinit funcId value:%s\n",__FUNCTION__,__LINE__,readBuf);
							sscanf(readBuf,FUNC_NODE_BUFF_FORMAT,&(base.wlan_idx),&(base.vwlan_idx),&(base.wan_idx),&funcId);
							reinitSer_printf(LOG_DEBUG,"%s:%d wlanIdx=%d vwlanIdx=%d wanIdx=%d funcId=%d\n",__FUNCTION__,__LINE__,base.wlan_idx,base.vwlan_idx,base.wan_idx,funcId);
							
							pFuncListNode=generate_reinit_func_list_node(pMibChangeNode,funcId,&base);
							if(pFuncListNode)
								reinit_add_reinitFunc_to_list(pFuncListNode);
						}
					#if 0
						reinitSer_printf(LOG_DEBUG,"%s:%d parent(pipe read) get value\n",__FUNCTION__,__LINE__);
						rdRet=read(fd[0],readBuf,sizeof(readBuf));
						if(rdRet<=0) continue;
						if(rdRet>=sizeof(readBuf))
						{							
							reinitSer_printf(LOG_ALERT,"%s:%d FUNC_NODE_BUFF_LEN(%d) too short! now read %d bytes:%s\n",__FUNCTION__,__LINE__,FUNC_NODE_BUFF_LEN,rdRet,readBuf);
							exit(0);
						}
						
						
						rdIdx=readBuf;
						while(tmpStr=strstr(rdIdx,"wlanIdx="))
						{
							tmpStr+=strlen("wlanIdx=");
							base.wlan_idx=atoi(tmpStr);
							
							tmpStr=strstr(rdIdx,"vwlanIdx=");
							tmpStr+=strlen("vwlanIdx=");
							base.vwlan_idx=atoi(tmpStr);

							tmpStr=strstr(rdIdx,"wanIdx=");
							tmpStr+=strlen("wanIdx=");
							base.wan_idx=atoi(tmpStr);

							tmpStr=strstr(rdIdx,"funcId=");
							tmpStr+=strlen("funcId=");
							funcId=atoi(tmpStr);
							
							rdIdx=tmpStr;							
						}
					#endif
					}
				}while(waitpid(pid,&status,WNOHANG)<=0 && count<REINIT_FUNC_TIMEOUT_COUNT_EFFECT);
				reinitSer_printf(LOG_DEBUG,"%s:%d count=%d\n",__FUNCTION__,__LINE__,count);

				fclose(fp);

				if(count==REINIT_FUNC_TIMEOUT_COUNT_EFFECT)
				{
					kill(pid,SIGTERM);
					reinitSer_printf(LOG_ERR,"run decision func %s exit time out(%dS)!kill pid(%d)\n",decFuncItem->name,REINIT_FUNC_TIMEOUT_COUNT_EFFECT*ERINIT_FUNC_TIMEOUT_USLEEP/1000000,pid);
				}
	            else if(WIFEXITED(status))
	            {
	                reinitSer_printf(LOG_INFO,"run decision func %s exited\n",decFuncItem->name);
	            }
	            else if(WIFSIGNALED(status))
	            {
	                reinitSer_printf(LOG_CRIT,"run decision func %s crash!!!\n",decFuncItem->name);
				}				 
				else
				{
					 reinitSer_printf(LOG_ERR,"run decision func %s exit abnormal!\n",decFuncItem->name);
				}
	            break;
			}
	}
	
	return 0;
}
/*
void reinit_init_baseData_parm(BASE_DATA_Tp parm,reinit_func_list_node_tp pFuncListNode)
{
	reinit_func_parm_list_node_tp parmListNode=NULL;
	
	memcpy(parm,&pFuncListNode->changeNode->base,sizeof(BASE_DATA_T));
	parm->parm=&pFuncListNode->parm_list;
}
*/
int reinit_run_reinit_func(reinit_func_list_node_tp pFuncListNode)
{
    int pid=0,status=0;
	
	reinitSer_printf(LOG_DEBUG,"%s:%d funcid=%d\n",__FUNCTION__,__LINE__,pFuncListNode->funcItem->change_func_id);
    switch(pid=fork())
    {
        case -1:
            reinitSer_printf(LOG_ERR,"fork fail!!!\n");
            return -1;
        case 0://child
        {
			//BASE_DATA_T parm={0};

			//reinitSer_printf(LOG_DEBUG,"%s:%d funcId=%d\n",__FUNCTION__,__LINE__,pFuncListNode->funcItem->change_func_id);
			if(!pFuncListNode||!pFuncListNode->funcItem||!pFuncListNode->funcItem->mib_change_func)
				exit(0);
			reinitSer_printf(LOG_DEBUG,"%s:%d wlanIdx=%d vwlanIdx=%d wanIdx=%d funcId=%d\n",__FUNCTION__,__LINE__,pFuncListNode->base.wlan_idx,pFuncListNode->base.vwlan_idx,pFuncListNode->base.wan_idx,pFuncListNode->funcItem->change_func_id);

			//reinit_init_baseData_parm(&parm,pFuncListNode);
            pFuncListNode->funcItem->mib_change_func(pFuncListNode);
	        reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
            exit(0);
        }
        default://parent
        {
			int count=0;
	        reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
			while(waitpid(pid,&status,WNOHANG)<=0 && count<REINIT_FUNC_TIMEOUT_COUNT_REINIT)
			{
				usleep(ERINIT_FUNC_TIMEOUT_USLEEP);
				count++;
			}
			if(count>=REINIT_FUNC_TIMEOUT_COUNT_REINIT)
			{
				kill(pid,SIGTERM);
				reinitSer_printf(LOG_ERR,"run init func %s exit time out(%dS)! kill pid(%d)\n",pFuncListNode->funcItem->name,REINIT_FUNC_TIMEOUT_COUNT_REINIT*ERINIT_FUNC_TIMEOUT_USLEEP/1000000,pid);
			}
            else if(WIFEXITED(status))
            {
                reinitSer_printf(LOG_INFO,"run init func %s exited\n",pFuncListNode->funcItem->name);
            }
            else if(WIFSIGNALED(status))
            {
                reinitSer_printf(LOG_CRIT,"run init func %s crash!!!\n",pFuncListNode->funcItem->name);
			}			 
			else
			{
				 reinitSer_printf(LOG_ERR,"run init func %s exit abnormal!\n",pFuncListNode->funcItem->name);
			}
            break;
        }
    }
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}

int reinit_generate_reinitFuncList()
{
	MibChangeNode_tp pnode=NULL;
	reinit_func_list_node_tp pFuncNode=NULL;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);

	list_for_each_entry(pnode,&(mibChangelist.changeNodeHead.list),list)
	{
		
		if(pnode->change_func_id<DECISION_END_FUNC_ID)
		{//decision function need to run
			reinit_run_decision_func(pnode);
		}else
		{
			pFuncNode=generate_reinit_func_list_node(pnode,0,NULL);
			if(pFuncNode)
				reinit_add_reinitFunc_to_list(pFuncNode);
		}
	}
	return 0;
}

int reinit_run_reinitFuncList()
{
	struct list_head * plist=NULL;
	reinit_func_list_node_tp pnode=NULL;
	list_for_each(plist,&(reinitFuncList.funcNodeHead.list))
	{
		reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
		pnode=list_entry(plist,reinit_func_list_node_t,list);
		reinit_run_reinit_func(pnode);
	}
	return 0;
}


int del_reinitFuncListNode(reinit_func_list_node_tp pnode)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	list_del(&pnode->list);	
	free_reinit_func_list_node(pnode);
	reinitFuncList.count--;
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}

int reinit_clear_reinitFuncList()
{
	reinit_func_list_node_tp ptmp=NULL;
	reinit_func_list_node_tp pnode=NULL;
	
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	list_for_each_entry_safe(pnode,ptmp,(&reinitFuncList.funcNodeHead.list),list)
	{
		reinitSer_printf(LOG_DEBUG,"%s:%d count=%d id=%d\n",__FUNCTION__,__LINE__,reinitFuncList.count,pnode->funcItem->change_func_id);
		del_reinitFuncListNode(pnode);
	}
	return 0;
}

int reinit_event_mibChange(char * data,int dataLen,char *errmsg)
{
	MibChangeNode_tp pMibChangeNode=NULL;
	int retval=0;
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	pMibChangeNode=generate_MibChangeNode();
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(get_reinit_event_mibchange(pMibChangeNode,data)<0)
	{
		sprintf(errmsg,"%s:%d get_reinit_event_mibchange fail!\n",__FUNCTION__,__LINE__);
		retval=-1;
		goto reinit_event_mibChange_fail;
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	//dumpMibChangeListInfo(pMibChangeNode);
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	if(reinit_mibChange_addToList(pMibChangeNode)<0)
	{
		sprintf(errmsg,"%s:%d reinit_mibChange_addToList fail!\n",__FUNCTION__,__LINE__);
		retval=-1;
		goto reinit_event_mibChange_fail;
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d \n",__FUNCTION__,__LINE__);
	return retval;
reinit_event_mibChange_fail:
	if(pMibChangeNode)
	{
		if(pMibChangeNode->value)
			free(pMibChangeNode->value);
		free(pMibChangeNode);
	}
	return retval;
}

int reinit_event_applyChanges(char * data,int dataLen,char *errmsg)
{
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_check_mibChangeList_change();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_generate_reinitFuncList();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_run_reinitFuncList();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_clear_mibChangeList();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	reinit_clear_reinitFuncList();
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return 0;
}


int reinit_get_desionFuc_orig_mib_value(int id, void* value, BASE_DATA_Tp data)
{
	MibChangeNode_tp pMibChangeNode = (MibChangeNode_tp)data;
	if(id!=pMibChangeNode->id)
	{
		reinitSer_printf(LOG_ERR,"%s:%d input invalid! Please check input mib id!\n",__FUNCTION__,__LINE__);
		exit(0);
	}
	if(!value)
	{
		reinitSer_printf(LOG_ERR,"%s:%d input invalid! Please check input value!\n",__FUNCTION__,__LINE__);
		exit(0);
	}

	memcpy(value,pMibChangeNode->value,pMibChangeNode->length);
	return 0;
}

int reinit_add_reinit_func(MIB_CHANGE_FUNC_ID funcId, BASE_DATA_Tp data)
{
	MibChangeNode_tp pMibChangeNode = (MibChangeNode_tp)data;
	int parentPipeFd=pMibChangeNode->parentPipeFd;
	char writeBuf[FUNC_NODE_BUFF_LEN]={0};
	if(funcId>=REINIT_END_FUNC_ID || funcId<=REINIT_BEGIN_FUNC_ID)
	{
		printf("%s:%d Invalid funcId %d\n",__FUNCTION__,__LINE__,funcId);
		return -1;
	}

	sprintf(writeBuf,FUNC_NODE_BUFF_FORMAT,pMibChangeNode->mibChangeNode_wlan_idx,pMibChangeNode->mibChangeNode_vwlan_idx,pMibChangeNode->mibChangeNode_wan_idx,funcId);
	reinitSer_printf(LOG_DEBUG,"%s:%d add %s\n",__FUNCTION__,__LINE__,writeBuf);
	write(parentPipeFd,writeBuf,strnlen(writeBuf));	
	return 0;
}

int reinitFunc_mib_changed(int id,BASE_DATA_Tp data)
{
	reinit_func_list_node_tp input_func_list_node=data;
	MibChangeNode_tp pnode=NULL;
	struct list_head* pchangeListNodeHead=NULL;
	int find_id=0;
	if(!data)
	{
		reinitSer_printf(LOG_ERR,"%s:%d Invalid input!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	pchangeListNodeHead=&mibChangelist.changeNodeHead.list;
	reinitSer_printf(LOG_NOTICE,"%s:%d \n",__FUNCTION__,__LINE__);
	list_for_each_entry(pnode,(pchangeListNodeHead),list)
	{		
		if(pnode->id==id)
		{
			find_id=1;
		}
	}
	return find_id;
}
int reinitFunc_get_mib_orig_value(int id, void* value, BASE_DATA_Tp data)
{
	reinit_func_list_node_tp input_func_list_node=data;
	MibChangeNode_tp pnode=NULL;
	struct list_head* pchangeListNodeHead=NULL;
	int find_id=0;
	if(!data)
	{
		reinitSer_printf(LOG_ERR,"%s:%d Invalid input!\n",__FUNCTION__,__LINE__);
		return -1;
	}
	pchangeListNodeHead=&mibChangelist.changeNodeHead.list;
	reinitSer_printf(LOG_NOTICE,"%s:%d pchangeListNodeHead=%p\n",__FUNCTION__,__LINE__,pchangeListNodeHead);
	list_for_each_entry(pnode,(pchangeListNodeHead),list)
	{		
		reinitSer_printf(LOG_DEBUG,"%s:%d pnode->id=%d\n",__FUNCTION__,__LINE__,pnode->id);
		if(pnode->id==id)
		{
			find_id=1;
			if(pnode->length==1)
			{
				*((unsigned int*)value)=*((unsigned char*)(pnode->value));
			}else
			if(pnode->length==2)
			{
				*((unsigned int*)value)=*((unsigned short*)(pnode->value));
			}else
			if(pnode->length>0)
				memcpy(value,pnode->value,pnode->length);
			else
				find_id=2;

			break;
		}
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	if(find_id==0)//not find, get orig value from mib
	{
		reinitSer_printf(LOG_DEBUG,"%s:%d wan_idx=%d\n",__FUNCTION__,__LINE__,data->wan_idx);
		if(apmib_rtkReinit_get(id,value,data->wlan_idx,data->vwlan_idx,data->wan_idx)<=0)
		{			
			reinitSer_printf(LOG_ERR,"%s:%d apmib_rtkReinit_get %d fail\n",__FUNCTION__,__LINE__,id);
			return -1;
		}
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d\n",__FUNCTION__,__LINE__);
	return find_id;
}

