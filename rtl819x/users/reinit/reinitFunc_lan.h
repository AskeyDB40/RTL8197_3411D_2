#ifndef REINITFUNC_LAN_H_
#define REINITFUNC_LAN_H_

#include "reinitSer.h"


#define BR_IFACES_LEN 256
#define BR_COMMAND_LEN 128

#define SDEBUG printf

int reinit_func_bridge(BASE_DATA_Tp data);
int reinit_func_lan_mac(BASE_DATA_Tp data);
int reinit_func_lan_stp(BASE_DATA_Tp data);
int reinit_func_lan_app_dhcp(BASE_DATA_Tp data);
int func_connect_lan(BASE_DATA_Tp data);
#ifdef CONFIG_DOMAIN_NAME_QUERY_SUPPORT
int reinit_func_lan_app_domain_query_dnrd(BASE_DATA_Tp data);
#endif
#ifndef STAND_ALONE_MINIUPNP
int reinit_func_lan_app_miniupnp(BASE_DATA_Tp data);
#endif
#ifdef CONFIG_AUTO_DHCP_CHECK
int reinit_func_lan_app_Auto_DHCP_Check(BASE_DATA_Tp data);
#endif
#ifdef SAMBA_WEB_SUPPORT
int reinit_func_lan_app_samba_web(BASE_DATA_Tp data);
#endif
#if defined(CONFIG_APP_TR069)	
int reinit_func_lan_app_tr069(BASE_DATA_Tp data);
#endif
#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
int reinit_func_lan_app_ETH_802DOT1X(BASE_DATA_Tp data);
#endif
#ifdef RTK_CAPWAP	
int reinit_func_lan_app_capwap_app(BASE_DATA_Tp data);
#endif
int init_func_misc(BASE_DATA_Tp data);

#endif
