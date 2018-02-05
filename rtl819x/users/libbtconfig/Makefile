#CC		= mips-uclibc-gcc
COPT		= -Os 
CFLAGS		= ${COPT} 
#STRIP		= mips-uclibc-strip
ifeq ($(wildcard $(DIR_LINUX)/.config),)
include $(DIR_ROOT)/linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif
#include ../../linux-2.6.30/.config

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../.config
else
include $(DIR_USERS)/.config
endif
BLUEZ_DIR = $(DIR_ROOT)/users/bluez-5.24
CFLAGS += -I$(DIR_ROOT)/users/boa/apmib -I$(BLUEZ_DIR)/lib/ -I$(BLUEZ_DIR)/gdbus -I$(DIR_LINUX)/drivers/net/wireless/rtl8192cd
CFLAGS += $(EXTRA_CFLAGS)
ifeq ($(CONFIG_CPU_LITTLE_ENDIAN),y)
CFLAGS += -DL_ENDIAN
else
CFLAGS += -DB_ENDIAN
endif
CFLAGS += $(COMMON_CFLAGS)
LDFLAGS = -lapmib -L$(DIR_ROOT)/users/boa/apmib
all:  libbtconfig_api.so 
#	-ctags *.[ch]

clean:
	make -C common clean
	rm -f libbtconfig_api.so tags *.o 

SOURCES = rtk_btconfig_api.c rtk_btconfig_util.c rtk_btconfig_api.h
OBJS = $(SOURCES:.c=.o)

.c.o:
	$(CC) -c -o $@ -fpic $(DEBUG) $(CFLAGS) $(LDFLAGS) $<

libbtconfig_api.so:	$(OBJS)
	make -C common
	$(CC) -s -shared -o $@ $(OBJS)

romfs:
	$(ROMFSINST) libbtconfig_api.so /lib/libbtconfig_api.so
	$(ROMFSINST) common/libbtconfig_util.so /lib/libbtconfig_util.so
