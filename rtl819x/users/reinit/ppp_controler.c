#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "apmib.h"
#include "reinit_utility.h"

#define WAN_NUMBER 4


/* add decision to save cpu time . */
int decision(WANIFACE_T  *entryP,int wan_index)
{		
	unsigned char command[48];
	unsigned char Exefile[48];

	if(!entryP->enable)	//entry disable,skip it
		return 1;

	if(entryP->pppCtype == MANUAL)
		return 1;
		
	sprintf(Exefile,"/var/run/wan_%d.pid",wan_index);
	
	if(isFileExist(Exefile)== 1)
		return 1;
	
	sprintf(command,"pppd -%d",wan_index);
	if(IsExistProcess(command))
		return 1;
	
	return 0;	
}

#if defined(SINGLE_WAN_SUPPORT)
int pptp_decision(WANIFACE_T  *entryP,int wan_index)
{
	unsigned char command[48];
	unsigned char Exefile[48];
	
	if(!entryP->enable)	//entry disable,skip it
		return 1;

	if(entryP->pptpConnectType == MANUAL)
		return 1;
		
	sprintf(Exefile,"/var/run/wan_%d.pid",wan_index);
	
	if(isFileExist(Exefile)== 1)
		return 1;
	
	sprintf(command,"pptp");
	if(IsExistProcess(command))
		return 1;
	
	return 0;		
}
int l2tp_decision(WANIFACE_T  *entryP,int wan_index)
{
	unsigned char command[48];
	unsigned char Exefile[48];
	
	if(!entryP->enable)	//entry disable,skip it
		return 1;

	if(entryP->l2tpConnectType == MANUAL)
		return 1;
		
	sprintf(Exefile,"/var/run/wan_%d.pid",wan_index);
	
	if(isFileExist(Exefile)== 1)
		return 1;

	#if 0
	sprintf(command,"l2tpd");
	if(IsExistProcess(command))
		return 1;
	#endif
	return 0;		
}
#endif
int main(int argc, char** argv)
{	
	int  wan_index;
	WANIFACE_T WanIfaceEntry;
	unsigned char configfile[48];
	unsigned char command[48];
	
	if (apmib_init() == 0) {
		printf("Initialize AP MIB failed!%s:%d\n",__FUNCTION__,__LINE__);
		return;
	}
	if (daemon(0, 1) == -1) {
		perror("ppp_inet fork error");
		return 0;
	}
	for (;;) 
	{
		/*
			1	wan type is pppoe
			2	configure file exist.options file.
			3	/etc/ppp/linkx file not exist!!!
		*/
		int wan_type;
		int wan_index =-1;
		for(wan_index = 1 ; wan_index <= WAN_NUMBER; ++wan_index)
		{	
	#if 0	
			printf("%s%d.swan_index(%d)\n",
				__FUNCTION__,__LINE__,wan_index);
	#endif
			getWanIfaceEntry(wan_index,&WanIfaceEntry);

			/* wan type isn't  pppoe,skip  ********/
			wan_type = WanIfaceEntry.AddressType;
			if(wan_type == PPPOE)
			{
				/*pppoe decision function call ********/
				if(decision(&WanIfaceEntry,wan_index))
					continue;				

				/*pppoe configure isn't exist ,skip ********/	
				sprintf(configfile,"/etc/ppp/options%d",wan_index);
				if(isFileExist(configfile)!= 1)
					continue ;
				
				/*pppoe link file  exist , just skip ********/
				sprintf(configfile,"/etc/ppp/link%d",wan_index);
				if(isFileExist(configfile) == 1)
					continue;
				
				/*start  pppoe dial********/
				sprintf(command,"pppd -%d &",wan_index);
				system(command);
			}
#if defined(SINGLE_WAN_SUPPORT)
			else if(wan_type == L2TP)
			{
				if(l2tp_decision(&WanIfaceEntry,wan_index))
					continue;
				
				sprintf(configfile,"/etc/ppp/options%d",wan_index);
				if(isFileExist(configfile)!= 1)
					continue ;

				sprintf(configfile,"/etc/ppp/link%d",wan_index);
				if(isFileExist(configfile) == 1)
					continue;	
				
				system("echo \"c client\" > /var/run/l2tp-control &");			
			}
			else if(wan_type == PPTP)
			{
				if(pptp_decision(&WanIfaceEntry,wan_index))
					continue;
					
				sprintf(configfile,"/etc/ppp/options%d",wan_index);
				if(isFileExist(configfile)!= 1)
					continue ;

				sprintf(configfile,"/etc/ppp/link%d",wan_index);
				if(isFileExist(configfile) == 1)
					continue;		
				
				system("pppd call rpptp &");
			}			
#endif			
		}
		sleep(5);
	}
}
