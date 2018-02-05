#ifndef ARP_INC
#define ARP_INC

int
arp_state_machine(
	struct usbnet* dev,
	struct cdc_ncm_ctx* ctx,
	unsigned char* buf,
	unsigned int len
	);


#endif // ARP_INC
// end of file
