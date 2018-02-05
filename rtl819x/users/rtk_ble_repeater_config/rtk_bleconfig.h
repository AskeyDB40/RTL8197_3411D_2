#ifndef __RTK_BLE_CONFIG_H__
#define __RTK_BLE_CONFIG_H__
typedef struct _Device_info {
    unsigned char  status;                  //BIT(0):connected BIT(1):profile saved
    unsigned short dev_type;
    unsigned char  mac[6];
    unsigned int   ip;
    unsigned char  extra_info[64];//name
    unsigned char  require_pin;             //1-require PIN, 0-no need for PIN
} dev_info, *dev_info_Tp;

#endif/*__RTK_BLE_CONFIG_H__*/
