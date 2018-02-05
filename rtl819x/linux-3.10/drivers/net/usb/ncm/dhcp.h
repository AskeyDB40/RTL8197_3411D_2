#ifndef _DHCP_INC_
#define _DHCP_INC_

#include "lte-ncm.h"

int
dhcp_state_machine(
	struct usbnet* dev,
	struct cdc_ncm_ctx* ctx,
	unsigned char* buf,
	unsigned int len
	);

#endif // _DHCP_INC_
// end of file
