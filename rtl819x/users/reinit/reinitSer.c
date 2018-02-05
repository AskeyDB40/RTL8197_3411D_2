/*
 * reinit server daemon
 */
#include "reinitUbus.h"
#include "reinitHandle.h"
#include "reinitSer.h"

void reinitSer_init()
{
	if(mibChangelist.count<0)
	{
		bzero(&mibChangelist.changeNodeHead,sizeof(mibChangelist.changeNodeHead));
		INIT_LIST_HEAD(&mibChangelist.changeNodeHead.list);
		mibChangelist.count=0;
	}
	if(reinitFuncList.count<0)
	{
		bzero(&reinitFuncList.funcNodeHead,sizeof(reinitFuncList.funcNodeHead));
		INIT_LIST_HEAD(&reinitFuncList.funcNodeHead.list);
		//reinitFuncList.funcNodeHead.funcItem=get_mibReinitFuncSet_item(REINIT_END_FUNC_ID);
		//reinitFuncList.funcNodeHead.mibChangeList=NULL;
		//INIT_LIST_HEAD(&reinitFuncList.funcNodeHead.parm_list);
		
		reinitFuncList.count=0;
	}
	apmib_init();
}

int main(int argc, char **argv)
{

	reinitSer_init();
	
	reinitUbusMain();

	return 0;
}
