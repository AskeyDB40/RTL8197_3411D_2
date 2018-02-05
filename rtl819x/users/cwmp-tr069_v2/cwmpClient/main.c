#include <signal.h>
#include <sys/wait.h>
#include "prmt_igd.h"
#include "prmt_utility.h"
#ifdef CONFIG_USER_CWMP_WITH_TR181
#include "tr181_device.h"
#include "tr181_mgmtServer.h"
#endif
#ifdef CONFIG_APP_TR104
#include "cwmp_main_tr104.h"
#endif
#ifdef MULTI_WAN_SUPPORT
#include "mibdef.h"
#include "apmib.h"
#endif

void cwmp_show_help( void )
{
#ifdef MULTI_WAN_SUPPORT
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "cwmpClient [-i InterfaceName]:\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-I/-i\t\tSpecify the interface for TR-069. Default interface will be that with TR069 WAN Type. If no such WAN interface, TR-069 won't start\n" ));
#else
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "cwmpClient:\n" ));
#endif
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-flush transfer|notification:	flush transfer or notification partition\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-SendGetRPC:	send GetRPCMethods to ACS\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-SSLAuth:	ACS need certificate the CPE\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-SkipMReboot:	do not send 'M Reboot' event code\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-Delay: 	delay some seconds to start\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	-h or -help: 	show help\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "	if no arguments, read the setting from mib\n" ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "\n" ));
}

/*refer to climenu.c*/
#define CWMP_RUNFILE	"/var/run/cwmp.pid"
static void log_pid()
{
	FILE *f;
	pid_t pid;
	char *pidfile = CWMP_RUNFILE;

	pid = getpid();
	if((f = fopen(pidfile, "w")) == NULL)
		return;
	fprintf(f, "%d\n", pid);
	fclose(f);
}

static void clr_pid()
{
		unlink(CWMP_RUNFILE);
}

void clear_child(int i)
{
	int status;
	pid_t chidpid = 0;

	//chidpid=wait( &status );
	if((chidpid = waitpid(-1, &status, WNOHANG))<=0)/* see wait(2) manpage */
		return;
#ifdef _PRMT_TR143_
#ifdef CONFIG_USER_FTP_FTP_FTP
	//if(chidpid!=-1)
		checkPidforFTPDiag( chidpid );
#endif //CONFIG_USER_FTP_FTP_FTP
#endif //_PRMT_TR143_
#ifdef _SUPPORT_TRACEROUTE_PROFILE_
//	if(chidpid!=-1)
		checkPidforTraceRouteDiag( chidpid );
#endif //_SUPPORT_TRACEROUTE_PROFILE_
	return;
}

void handle_term()
{
	clr_pid();
#ifdef CONFIG_APP_TR104	
	tr104_main_exit();
#endif
	exit(0);
}

#ifdef MULTI_WAN_SUPPORT
int specifyWanInterface()
{
    int ret=0, wan_index, found_tr069if=0;
    char wanIfName[32]= {0};
    WANIFACE_T WanIfaceEntry, *pEntry;
    for(wan_index = 1; wan_index <= WANIFACE_NUM; ++wan_index)
    {
        memset(&WanIfaceEntry,0,sizeof(WANIFACE_T));
        getWanIfaceEntry(wan_index,&WanIfaceEntry);
        if( WanIfaceEntry.enable!=0  && (WanIfaceEntry.applicationtype==APPTYPE_TR069 || WanIfaceEntry.applicationtype==APPTYPE_TR069_INTERNET))
        {
            found_tr069if=1;
            break;
        }
    }
    if(found_tr069if){
        //if(WanIfaceEntry.AddressType == PPPOE)
        //    sprintf(wanIfName, "ppp%d", wan_index);
        //else
        //    sprintf(wanIfName, "eth1.%d", wan_index);
        pEntry = &WanIfaceEntry;
		*((char *)&WanIfaceEntry) = (char)wan_index;
		mib_get(MIB_WANIFACE_TBL, (void *)&WanIfaceEntry);
		if(pEntry->cmode==IP_PPP)
		{
#if 0
			/* TR069 wan is PPP 
			 *  1     2      3     4
             	 * ppp1, ppp8, ppp9, ppp10
			 */
			sprintf(wanIfName, "ppp%d", wan_index==1?wan_index:(wan_index+6));
#else
			getPPPIfName(wan_index,wanIfName,sizeof(wanIfName));
#endif
		}else
		{
#if 0
			/* TR069 wan is static IP/DHCP 
			 *  1     2      3     4
             	 * eth1, eth8, eth9, eth10
			 */
			int tr069_vlan_id = pEntry->vlan==0?0:pEntry->vlanid;
			if(tr069_vlan_id)
				sprintf(wanIfName, "eth%d.%d", wan_index==1?wan_index:(wan_index+6), tr069_vlan_id);
			else
				sprintf(wanIfName, "eth%d",  wan_index==1?wan_index:(wan_index+6));
#else
			getWanIfName(wan_index,wanIfName,sizeof(wanIfName));
#endif
		}
		
        //getWanIfName(wan_index, wanIfName);
        CWMPDBG_FUNC( MODULE_CORE, LEVEL_INFO, ("[%s:%d] wanIfName:%s\n", __FUNCTION__, __LINE__, wanIfName));
        cwmpinit_WanIf(wanIfName);
        ret = 1;
    }

    return ret;
}
#endif

int main(int argc, char **argv)
{
	int result = 0;
	log_pid();
	fprintf(stderr, "TR-069 cwmpClient startup.\n");

	apmib_init();
	cwmp_get_debug_module();
	cwmp_get_debug_level();
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "[%s:%d] set debug module:0x%x\n", __FUNCTION__, __LINE__, gDebugModule ));
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "[%s:%d] set debug level:0x%x\n", __FUNCTION__, __LINE__, gDebugLevel ));

	signal(SIGCHLD, clear_child);	//set this signal process function according to CWMP_TR069_ENABLE below if MIDDLEWARE is defined
	signal(SIGTERM,handle_term);

	if( argc >= 2 )
	{
		int i;
#ifdef MULTI_WAN_SUPPORT
        int specifyWanIf = 0;
		for(i=1;i<argc;i++)
        {
			if( strcmp( argv[i], "-I" )==0 || strcmp( argv[i], "-i" )==0 ){
                specifyWanIf=1;
                break;
            }
        }
        if(!specifyWanIf)
            if(!specifyWanInterface()){
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "ERROR: No WAN interface is specified for TR069!\n" ));
                return 0;
            }
#endif
		for(i=1;i<argc;i++)
		{
			if( strcmp( argv[i], "-SendGetRPC" )==0 )
			{
				cwmpinit_SendGetRPC(1);
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Send GetPRCMethods to ACS\n",__FILE__ ));
			}else if( strcmp( argv[i], "-I" )==0 ||  strcmp( argv[i], "-i" )==0 )
            {
#ifdef MULTI_WAN_SUPPORT
                if((i+1)>(argc-1)){ //exceed
               		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("Error: argument invalid\n"));
                    cwmp_show_help();
				
                    return 0;
                }

                i++;
                if( strstr( argv[i], "eth") || strstr( argv[i], "ppp") ){
                    CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("<%s>Bind TR-069 to interface %s\n",__FILE__, argv[i]));
                    cwmpinit_WanIf(argv[i]);
                }else{
                    CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("ERROR: %s is a wrong TR-069 interface. Must be eth1/eth1.1/...\n", argv[i]) );
                    cwmp_show_help();
                    
                    return 0;
                }
#endif
            }else if( strcmp( argv[i], "-SSLAuth" )==0 )
			{
				cwmpinit_SSLAuth(1);
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set using certificate auth.\n",__FILE__ ));
			}else if( strcmp( argv[i], "-SkipMReboot" )==0 )
			{
				cwmpinit_SkipMReboot(1);
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set skipping MReboot event code\n",__FILE__ ));
			}else if( strcmp( argv[i], "-Delay" )==0 )
			{
				cwmpinit_DelayStart(30);
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set Delay!\n", __FILE__ ));
			}else if( strcmp( argv[i], "-h" )==0 || strcmp( argv[i], "-help" )==0 )
			{
				cwmp_show_help();
				exit(0);
			}else if( strcmp( argv[i], "-reboot" )==0 )
			{
				cwmp_reboot();
				sleep(1);
				return 0;
			}else if( strcmp( argv[i], "-flush" )==0 )
			{
				if((i+1) > (argc-1)){ //exceed
               		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("Error: argument invalid\n"));
                    cwmp_show_help();
				
                    return 0;
                }
				
				i++;				
				if(cwmp_flush_partition(argv[i]) < 0)
					cwmp_show_help();
				
				return 0;
			}else
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "<%s>Error argument: %s\n", __FILE__,argv[i] ));
				cwmp_show_help();
				
				return 0;
			}
		}
	}else{
		unsigned int cwmp_flag=0;
		//read the flag, CWMP_FLAG, from mib
		if ( mib_get( MIB_CWMP_FLAG, (void *)&cwmp_flag)!=0 )
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("[%s:%d] cwmp_flag=0x%x\n", __FUNCTION__, __LINE__, cwmp_flag));				
			if( cwmp_flag&CWMP_FLAG_CERT_AUTH )
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set using certificate auth.\n",__FILE__ ));
				cwmpinit_SSLAuth(1);
			}
				
			if( cwmp_flag&CWMP_FLAG_SENDGETRPC )
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Send GetPRCMethods to ACS\n",__FILE__ ));
				cwmpinit_SendGetRPC(1);
			}
			
			if( cwmp_flag&CWMP_FLAG_SKIPMREBOOT )
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set skipping MReboot event code\n",__FILE__ ));
				cwmpinit_SkipMReboot(1);
			}
				
			if( cwmp_flag&CWMP_FLAG_DELAY )
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set Delay!\n", __FILE__ ));
				cwmpinit_DelayStart(30);
			}

			if( cwmp_flag&CWMP_FLAG_SELFREBOOT)
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set SelfReboot!\n", __FILE__ ));
				cwmpinit_SelfReboot(1);
			}
				
		}

		if ( mib_get( MIB_CWMP_FLAG2, (void *)&cwmp_flag)!=0 )
		{
#if defined(CONFIG_BOA_WEB_E8B_CH) || defined(_TR069_CONREQ_AUTH_SELECT_)
			if( cwmp_flag&CWMP_FLAG2_DIS_CONREQ_AUTH)
			{
				CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ( "<%s>Set DisConReqAuth!\n", __FILE__ ));
				cwmpinit_DisConReqAuth(1);
			}
#endif

			if( cwmp_flag&CWMP_FLAG2_DEFAULT_WANIP_IN_INFORM)
				cwmpinit_OnlyDefaultWanIPinInform(1);
			else
				cwmpinit_OnlyDefaultWanIPinInform(0);

		}		
#ifdef MULTI_WAN_SUPPORT
        if(!specifyWanInterface()){
            CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("ERROR: No WAN interface is specified for TR069!\n") );
            return 0;
        }
#endif
	}

#ifdef TR069_ANNEX_G
	/*
	 * 1. There're two possible locations to startup STUN thread:
	 * a. cwmpClient startsup
	 *		cwmp_process will call port_misc_thread_startup() before enter main loop.
	 *		Start STUN thread so late until notification has been initilized. This is for some ISP STUN Active Notification requirements.
	 * b. session closed
	 *		At port_session_close(), global argument gRestartStun will be checked.
	 *		This global argument is ONLY set to 1 by setPV to .ManagementServer.STUNEnable.
	 * 2. CWMP_STUN_EN mib is used to decide whether start STUN thread.
	 * 3. STUN result is reset here, to meet some ISP STUN Active Notification requirements:
	 *		Some ISP require "4 VALUE CHANGE" event for UDPConnectionRequestAddress and NATDetected if they're set to Active Notification, even when CPE boot up.
	 */
	{
		unsigned char zerobuf[] = "0.0.0.0";
		unsigned int vInt = 2;	// MIB_CWMP_NAT_DETECTED can only be 1 or 0, initial to 2 here so that it must be contained in Inform 4 VALUE CHANGE.
		apmib_set(MIB_CWMP_UDP_CONN_REQ_ADDR, (void *)zerobuf);
		apmib_set(MIB_CWMP_NAT_DETECTED, (void *)&vInt);
	}
#endif
#ifdef CONFIG_USER_CWMP_WITH_TR143
	{
		struct TR143_UDPEchoConfig tr143_data;
		UDPEchoConfigSave( &tr143_data );
		UDPEchoConfigStart(&tr143_data);
	}
#endif

/*star:20100105 START when there is already a session, if return 503 after receive connection request*/
	cwmpinit_Return503(0);
/*star:20100105 END*/
#ifdef CONFIG_APP_TR104
	//cwmp_solarOpen();
	tr104_main_init();
#endif

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("cwmp startup check...\n"));
	result = port_cwmp_startup_check();
	switch (result)
	{
	case CWMP_ENV_WAN_INTERFACE_ERROR:
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("wan interface is not specified for cwmp!!Exit cwmp!!\n"));
		return 0;
	
	case CWMP_ENV_ACS_URL_ERROR:
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ("ACS url is invalid!!Exit cwmp!!\n"));
		return 0;
	
	case CWMP_ENV_WAN_DISCONN_ERROR:
		CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ("WAN may still disconnected now!!\n"));
		break;
	
	case CWMP_ENV_OK:
	default:
		break;
	}
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("cwmp startup check done, result:%s.\n", (result==CWMP_ENV_OK)?"OK":"Not OK"));
	
	CWMPDBG_FUNC(MODULE_CORE, LEVEL_INFO, ("enter cwmp_main!\n"));

#ifdef CONFIG_USER_CWMP_WITH_TR181
	fprintf(stderr, "Using TR-181 root data model\n");
	cwmp_main( tDevROOT );
#else
	fprintf(stderr, "Using TR-098 root data model\n");
	cwmp_main( tROOT );
#endif

	return 0;
}
