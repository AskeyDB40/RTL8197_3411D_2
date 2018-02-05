#ifndef __REINITFUNC_WAN_H
#define __REINITFUNC_WAN_H
#define NTP_INRT_PID_FILE "/var/run/ntp_inet.pid"
extern int reinit_func_wan_app_ntp(BASE_DATA_Tp data);
#ifdef DOS_SUPPORT
extern int reinit_func_wan_dos(BASE_DATA_Tp data);
#endif
extern int reinit_func_start_wan(BASE_DATA_Tp data);
extern void reinit_fun_firewall_dmz();
extern void reinit_fun_firewall_urlfilter();
extern void reinit_fun_firewall_macfilter();
extern void reinit_fun_firewall_ipfilter();
extern void reinit_fun_firewall_portfilter();
extern void reinit_fun_firewall_portforward();
extern void reinit_fun_firewall_pingAccess();
extern void reinit_fun_firewall_web_access();
extern void reinit_fun_static_route();

#if defined (CONFIG_ISP_IGMPPROXY_MULTIWAN)
#define MYSIG 33  //SIGRTMIN+1
#define ADD_WAN_IF 0x1000000
#define DEL_WAN_IF 0x2000000
#define WAN_PIF_1  0x1    //eth1.1
#define WAN_PIF_2  0x2    //eth1.2
#define WAN_PIF_3  0x4    //eth1.3
#define WAN_PIF_4  0x8    //eth1.4
#define WAN_VIF_1  0x100  //ppp1
#define WAN_VIF_2  0x200  //ppp2
#define WAN_VIF_3  0x400  //ppp3
#define WAN_VIF_4  0x800  //ppp4
#define STOP_BEFORE_DISCONNECTION 	0x1
#define STOP_AFTER_DISCONNECTION	0x2
#define START_BEFORE_CONNECTION 0x1
#define START_AFTER_CONNECTION  0x2
#define NEED_DEL_VIF 0x10000
#endif

#endif
