#
# Makefile for DWC_otg Highspeed USB controller driver
#



# Use the BUS_INTERFACE variable to compile the software for either 
# PCI(PCI_INTERFACE) or LM(LM_INTERFACE) bus.
ifeq ($(BUS_INTERFACE),)
#	BUS_INTERFACE = -DPCI_INTERFACE
	BUS_INTERFACE = -DLM_INTERFACE
endif

#EXTRA_CFLAGS	+= -DDEBUG 

# Use one of the following flags to compile the software in host-only or
# device-only mode.
#EXTRA_CFLAGS        += -DDWC_HOST_ONLY
#EXTRA_CFLAGS        += -DDWC_DEVICE_ONLY

EXTRA_CFLAGS	+= -Dlinux -DDWC_HS_ELECT_TST
#EXTRA_CFLAGS	+= -DDWC_EN_ISOC
#EXTRA_CFLAGS   	+= -I$(PWD)/dwc_common_port/
#EXTRA_CFLAGS   	+= -I$(PORTLIB)
EXTRA_CFLAGS   	+= -DDWC_LINUX
EXTRA_CFLAGS   	+= $(CFI)
EXTRA_CFLAGS	+= $(BUS_INTERFACE)
#EXTRA_CFLAGS	+= -DDWC_DEV_SRPCAP

#obj-m	:= dwc_otg.o
obj-$(CONFIG_DWC_OTG) += dwc_otg.o


dwc_otg-objs	:= dwc_otg_driver.o dwc_otg_attr.o 
dwc_otg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o 
dwc_otg-objs	+= dwc_otg_pcd_linux.o dwc_otg_pcd.o dwc_otg_pcd_intr.o 
dwc_otg-objs	+= dwc_otg_hcd.o dwc_otg_hcd_linux.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o dwc_otg_hcd_ddma.o
dwc_otg-objs	+= dwc_otg_adp.o

#from common lib
EXTRA_CFLAGS	+= -DDWC_LINUX
#EXTRA_CFLAGS	+= -DDEBUG
#EXTRA_CFLAGS	+= -DDWC_DEBUG_REGS
#EXTRA_CFLAGS	+= -DDWC_DEBUG_MEMORY

#EXTRA_CFLAGS	+= -DDWC_LIBMODULE
#EXTRA_CFLAGS	+= -DDWC_CCLIB
#EXTRA_CFLAGS	+= -DDWC_CRYPTOLIB
#EXTRA_CFLAGS	+= -DDWC_NOTIFYLIB
#EXTRA_CFLAGS	+= -DDWC_UTFLIB
#dwc_otg-objs	+=  dwc_cc.o dwc_modpow.o dwc_dh.o    dwc_crypto.o dwc_notifier.o     dwc_common_linux.o dwc_mem.o
dwc_otg-objs	+=  dwc_common_linux.o 	    


obj-y	+= lm.o
#obj-y	+= rtk_otg_autodet.o

clean:
	rm -rf   *.o *.ko .*cmd *.mod.c .tmp_versions Module.symvers


