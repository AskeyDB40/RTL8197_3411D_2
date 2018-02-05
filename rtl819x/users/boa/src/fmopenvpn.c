/*
 *      Web server handler routines for Dynamic DNS 
 *
 *      Authors: Shun-Chin  Yang	<sc_yang@realtek.com.tw>
 *
 *      $Id
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "boa.h"
#include "asp_page.h"
#include "apmib.h"
#include "apform.h"
#include "utility.h"

#ifdef CONFIG_APP_OPENVPN
void formOpenvpn(request *wp, char *path, char *query)
{
	char *submitUrl;
	char tmpBuf[100];

	int enabled=0 ,mode=0, port=0;
	int auth_type=0;
	char *tmpStr ;
	       
	submitUrl = req_get_cstream_var(wp, "submit-url", "");   // hidden page
	
	tmpStr = req_get_cstream_var(wp, "openvpnEnabled", "");  
	if(!strcmp(tmpStr, "ON"))
		enabled = 1 ;
	else 
		enabled = 0 ;

	if ( apmib_set( MIB_OPENVPN_ENABLED, (void *)&enabled) == 0) {
		strcpy(tmpBuf, "Set Openvpn enabled flag error!");
		goto setErr_openvpn;
	}
	
	if(enabled){
		tmpStr = req_get_cstream_var(wp, "openvpnMode", "");  
		if(tmpStr[0]){
		mode = tmpStr[0] - '0' ;
	 		if ( apmib_set(MIB_OPENVPN_MODE, (void *)&mode) == 0) {
					strcpy(tmpBuf, "Set Openvpn mode error!");
					goto setErr_openvpn;
			}
		}
		tmpStr = req_get_cstream_var(wp, "authenticationType", "");  
		if(tmpStr[0]){
		auth_type = tmpStr[0] - '0' ;
	 		if ( apmib_set(MIB_OPENVPN_AUTH_TYPE, (void *)&auth_type) == 0) {
					strcpy(tmpBuf, "Set Openvpn authentication type error!");
					goto setErr_openvpn;
			}
		}
		tmpStr = req_get_cstream_var(wp, "openvpnPort", "");  
		if(tmpStr[0]){
			port=atoi(tmpStr);
			//printf("\n%s:%d port=%d\n",__FUNCTION__,__LINE__,port);
			if ( apmib_set(MIB_OPENVPN_PORT, (void *)&port) == 0) {
					strcpy(tmpBuf, "Set Openvpn port error!");
					goto setErr_openvpn;
			}
		}		
	}

	apmib_update_web(CURRENT_SETTING);

#if 0
#ifdef REBOOT_CHECK
	run_init_script_flag = 1;
#endif
#ifndef NO_ACTION
	run_init_script("all");
#endif
	OK_MSG(submitUrl);
#endif

	if (submitUrl[0])
		send_redirect_perm(wp, submitUrl);
	
	int pid;
	pid = fork();	
	
	if (pid == 0) 
	{
		system("sysconf start_openvpn");
		exit(1);
	}	

	return;

setErr_openvpn:
	ERR_MSG(tmpBuf);
}
#endif
