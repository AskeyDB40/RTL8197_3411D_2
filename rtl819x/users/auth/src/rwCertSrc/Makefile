# include $(DIR_LINUX)/.config

ifeq ($(wildcard $(DIR_LINUX)/.config),)
include $(DIR_ROOT)/linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

#include ../../../../linux-2.6.30/.config

include ../../../../users/.config
 
ifeq ($(CONFIG_RTL_ETH_802DOT1X_CLIENT_MODE_SUPPORT),y)
	CFLAGS += -DCONFIG_RTL_ETH_802DOT1X_CLIENT_MODE_SUPPORT
endif

MTDAPI_DIR=$(DIR_ROOT)/users/lib/lib

ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
LDFLAGS += -L$(MTDAPI_DIR) -lmtdapi
endif

all: rsCert

rsCert: rsCertCommon.o rsCert.o
	$(CC) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@

rsCertCommon.o: rsCertCommon.h rsCertCommon.c

rsCert.o: rsCertCommon.h rsCert.c

clean:
	rm -f *.o rsCert

romfs:
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
	$(ROMFSINST) $(DIR_ROOT)/users/lib/lib/libmtdapi.so /lib/libmtdapi.so
else
	@echo "Do nothing here."
endif
#	$(ROMFSINST) /bin/rsCert

