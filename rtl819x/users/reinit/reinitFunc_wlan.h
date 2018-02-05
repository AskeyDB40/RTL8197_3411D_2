#ifndef __REINITFUNC_WLAN_H
#define __REINITFUNC_WLAN_H
#define NTP_INRT_PID_FILE "/var/run/ntp_inet.pid"
extern int reinit_func_wlan(BASE_DATA_Tp data);
extern int reinit_func_wlan_app(BASE_DATA_Tp data);
extern int reinit_func_wlan_block_relay(BASE_DATA_Tp data);
extern int reinit_func_wlan_schedule(BASE_DATA_Tp data);
extern int decision_func_wlan_disable(BASE_DATA_Tp data);
extern int decision_func_wlan_property(BASE_DATA_Tp data);
extern int decision_func_wlan_repeater(BASE_DATA_Tp data);
extern int decision_func_wlan_schedule(BASE_DATA_Tp data);
extern int decision_func_wlan_app(BASE_DATA_Tp data);
#ifdef WLAN_PROFILE
extern int decision_func_wlan_profile(BASE_DATA_Tp data);
#endif
extern int decision_func_wlan_block_relay(BASE_DATA_Tp data);

#endif

