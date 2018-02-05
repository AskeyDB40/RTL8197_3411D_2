#ifndef __RTL_GPIOMUXTBL_H__
#define __RTL_GPIOMUXTBL_H__
#include "rtl_gpiocommon.h"

void getPinMuxInitalVal(void);
void setPinMuxInitalVal(void);
int getPinMuxVal(uint32 gpioId,uint32* pData);
int setPinMuxVal(uint32 gpioId);
int checkPinMuxVal(uint32 gpioId,uint32 data);
int checkPinExist(char port,uint32 pin);
/* test */
#ifdef CONFIG_RTL_NEW_GPIO_API_TEST
void getRestWpsPin(uint32 *resetPin,uint32 *wpsPin);
#endif
#endif
