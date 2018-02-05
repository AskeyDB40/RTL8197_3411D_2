ifeq ($(wildcard $(DIR_LINUX)/.config),)
include ../../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../../.config
else
include $(DIR_USERS)/.config
endif

ifdef CONFIG_RTL_8198_AP_ROOT
CONFIG_RTL_AP = CONFIG_RTL_8198_AP_ROOT
endif

ifdef CONFIG_RTL_8197D_AP
CONFIG_RTL_AP = CONFIG_RTL_8197D_AP
endif

ifdef CONFIG_RTL_AP_PACKAGE 
CONFIG_RTL_AP = CONFIG_RTL_AP_PACKAGE
endif

ifdef CONFIG_RTL_AP
AP=1
SKUTAG=ap
else
GW=1
SKUTAG=gw
endif

#------------------------------------------------------------------------------
#ifndef COMMON_CFLAGS
#$(error COMMON_CFLAGS is not defined)
#endif

#ifndef APMIB_SHARED
#$(error APMIB_SHARED is not defined)
#endif

#ifndef APMIB_LIB
#$(error APMIB_LIB is not defined)
#endif

#------------------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean all depend

LDFLAGS =  -g
CFLAGS = -Os -pipe
DEPEND = ./.depend


#------------------------------------------------------------------------------
APMIB_DIR=../apmib
MTDAPI_DIR=$(DIR_ROOT)/users/lib/lib
#------------------------------------------------------------------------------
# EXTRA_CFLAGS
#------------------------------------------------------------------------------
EXTRA_CFLAGS =

#------------------------------------------------------------------------------
# CFLAGS
#------------------------------------------------------------------------------
DEBUG	= -g -Wall
IFLAGS 	= -I. -I$(APMIB_DIR)
ifeq ($(CONFIG_RTL_8196B),y)
IFLAGS += -I$(DIR_LINUX)/drivers/net/wireless/rtl8190
else
IFLAGS += -I$(DIR_LINUX)/drivers/net/wireless/rtl8192cd
endif
CFLAGS += $(COMMON_CFLAGS) $(EXTRA_CFLAGS)

#include $(DIR_USERS)/.config
ifdef CONFIG_APP_MULTIPPPOE
        CFLAGS += -D MULTI_PPPOE
endif
ifdef CONFIG_SYS_DIAGNOSTIC
	CFLAGS += -D SYS_DIAGNOSTIC
endif
ifeq ($(CONFIG_APP_RTK_VOIP),y)
LDFLAGS += -lpthread
endif

ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
LDFLAGS += -L$(MTDAPI_DIR) -lmtdapi
endif

SOURCES = flash.c
ifeq ($(CONFIG_RTL_COMAPI_CFGFILE),y)
SOURCES += comapi.c
endif
SOURCES += fwd.c

SOURCES += uart.c

SOURCES += fwupg.c ../src/utility.c
#------------------------------------------------------------------------------
# targets
#------------------------------------------------------------------------------
all: depend flash fwd fwupg main_lc5761

#ifeq ($(CONFIG_RTL_11W_SUPPORT),y)
#CFLAGS += -DCONFIG_IEEE80211W
#endif

ifeq ($(CONFIG_RTL_COMAPI_CFGFILE),y)
flash: Makefile flash.o comapi.o $(APMIB_DIR)/$(APMIB_LIB)
	$(CC) -o $@ flash.o comapi.o $(LDFLAGS) -L$(APMIB_DIR) -lapmib
	$(STRIP) $@
else
flash: Makefile flash.o $(APMIB_DIR)/$(APMIB_LIB)
	$(CC) -o $@ flash.o $(LDFLAGS) -L$(APMIB_DIR) -lapmib
	$(STRIP) $@
endif

fwd: Makefile fwd.o
	$(CC) --static -o $@ fwd.o
	$(STRIP) $@
	
main_lc5761: Makefile uart.o
	$(CC) --static -o $@ uart.o
	$(STRIP) $@

ifeq ($(CONFIG_RTK_SMART_ROAMING),y)	
fwupg: Makefile fwupg.o ../src/utility.o $(APMIB_DIR)/$(APMIB_LIB)
	$(CC) -o $@ fwupg.o ../src/utility.o $(LDFLAGS) -L$(APMIB_DIR) -lapmib
else
fwupg:

endif
	
clean:
	rm -f $(DEPEND)
	rm -f *.o flash fwd fwupg

depend: $(SOURCES)
	if [ ! -e $(DEPEND) ]; then \
		$(CPP) $(DEBUG) $(CFLAGS) $(IFLAGS) -MM $^ > $(DEPEND); \
	fi
	
-include $(DEPEND)

.c.o:
	${CC} -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
