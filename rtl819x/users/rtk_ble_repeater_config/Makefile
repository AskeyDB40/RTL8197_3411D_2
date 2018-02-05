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
CFLAGS += -I$(DIR_ROOT)/users/boa/apmib -I$(DIR_ROOT)/users/libbtconfig/ -I$(BLUEZ_DIR)/lib/ -I$(BLUEZ_DIR)/gdbus -I$(DIR_LINUX)/drivers/net/wireless/rtl8192cd
#CFLAGS += -I$(DIR_ROOT)/users/dbus-1.8.0
#CFLAGS +=-I$(DIR_ROOT)/users/glib-2.36.0 -I$(DIR_ROOT)/users/glib-2.36.0/glib
CFLAGS += $(EXTRA_CFLAGS)
ifeq ($(CONFIG_CPU_LITTLE_ENDIAN),y)
CFLAGS += -DL_ENDIAN
else
CFLAGS += -DB_ENDIAN
endif
ifeq ($(CONFIG_RTK_BLUETOOTH_HW_RTL8822B_S),y)
COMMON_CFLAGS += -DCONFIG_RTK_BLUETOOTH_HW_RTL8822B_S
endif
LIBS = -lpthread
#LDFLAGS	= -lbluetooth -L$(BLUEZ_DIR)/lib/.libs/ -lglib-2.0 -L$(DIR_ROOT)/users/lib/lib -ldbus-1 -L$(DIR_ROOT)/users/dbus-1.8.0/dbus/.libs
CFLAGS += $(COMMON_CFLAGS)
LDFLAGS	= -lapmib -L$(DIR_ROOT)/users/boa/apmib -lbtconfig_api -L$(DIR_ROOT)/users/libbtconfig -lbtconfig_util -L$(DIR_ROOT)/users/libbtconfig/common
all:   rtk_ble_repeater_config
#	-ctags *.[ch]

clean:
	rm -f rtk_ble_repeater_config tags *.o 

btconfig_sources =  rtk_bleconfig.c rtk_bleconfig.h

rtk_ble_repeater_config:	$(btconfig_sources:.c=.o)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@	
	$(STRIP) $@

romfs:
	$(ROMFSINST) rtk_ble_repeater_config /bin/rtk_ble_repeater_config
