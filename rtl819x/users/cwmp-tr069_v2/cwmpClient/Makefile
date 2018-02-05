KERNEL_VER=$(patsubst $(DIR_ROOT)/%,%,$(DIR_LINUX))
ifeq ($(wildcard $(DIR_LINUX)/.config),)
ifeq ($(KERNEL_VER),linux-2.6.30)
include ./../../../linux-2.6.30/.config
else
include ./../../../linux-3.10/.config
endif
else
include $(DIR_LINUX)/.config
endif
ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../../.config
else
include $(DIR_USERS)/.config
endif
# Directory Definition
VPATH:=$(GSOAP_DIR)/soapcpp2/plugin
APMIB_DIR=$(DIR_ROOT)/users/boa/apmib
MTDAPI_DIR=$(DIR_ROOT)/users/lib/lib

# Input Declarance
# Digest Authentication files
HTTPDA_SRC_y  := httpda.c \
			   	 md5evp.c \
			   	 threads.c 
HTTPDA_OBJ_y  := $(HTTPDA_SRC_y:.c=.o)
# Data models
TR098_SOURCES := prmt_igd.c prmt_deviceinfo.c prmt_mngmtserver.c \
				 prmt_time.c prmt_ddns.c  prmt_services.c prmt_userif.c prmt_captiveportal.c  \
				 prmt_layer3fw.c prmt_layer2bridge.c \
				 prmt_landevice.c prmt_landevice_wlan.c prmt_landevice_usb.c prmt_landevice_eth.c \
				 prmt_wandevice.c prmt_wancondevice.c \
				 prmt_traceroute.c \
				 prmt_ippingdiag.c prmt_queuemnt.c \
				 prmt_utility.c cwmp_ssl.c arping.c debug.c porting.c main.c

TR098_HEADERS := prmt_utility.h prmt_igd.h prmt_deviceinfo.h prmt_mngmtserver.h \
				 prmt_time.h prmt_ddns.h prmt_services.h prmt_userif.h prmt_captiveportal.h \
				 prmt_layer3fw.h prmt_layer2bridge.h \
				 prmt_landevice.h prmt_landevice_eth.h prmt_landevice_usb.h prmt_landevice_wlan.h \
				 prmt_wandevice.h prmt_wancondevice.h \
				 prmt_ippingdiag.h \
				 prmt_traceroute.h \
				 prmt_queuemnt.h cwmp_ssl.h prmt_apply.h

TR181_SOURCES := tr181_device.c tr181_deviceInfo.c tr181_gatewayInfo.c tr181_userInterface.c \
				 tr181_mgmtServer.c tr181_mgableDev.c tr181_eth.c tr181_ethIF.c tr181_MoCA.c \
				 tr181_dns.c tr181_dnsClient.c \
				 tr181_MoCA_IF.c tr181_wifi.c tr181_wifiRadio.c tr181_bridging.c tr181_bridge.c \
				 tr181_ip.c tr181_ipIF.c tr181_dhcpv4.c tr181_dhcpv4Client.c \
				 tr181_lanConfigSecurity.c tr181_deviceInfoVendorLogFile.c \
				 prmt_deviceinfo.c prmt_mngmtserver.c  \
				 prmt_landevice_wlan.c prmt_landevice.c prmt_landevice_eth.c \
				 prmt_wancondevice.c \
				 prmt_ippingdiag.c \
				 prmt_utility.c cwmp_ssl.c arping.c debug.c porting.c main.c

TR181_HEADERS := prmt_utility.h tr181_device.h tr181_deviceInfo.h tr181_gatewayInfo.h tr181_userInterface.h \
				 tr181_mgmtServer.h tr181_mgableDev.h tr181_eth.h tr181_ethIF.h tr181_MoCA.h \
				 tr181_MoCA_IF.h tr181_wifi.h tr181_wifiRadio.h tr181_bridging.h tr181_bridge.h \
				 tr181_ip.h tr181_ipIF.h tr181_dhcpv4.h tr181_dhcpv4Client.h  \
				 cwmp_ssl.h prmt_landevice_wlan.h prmt_ippingdiag.h prmt_wancondevice.h prmt_landevice.h prmt_landevice_eth.h \
				 prmt_deviceinfo.h prmt_mngmtserver.h tr181_dns.h tr181_dnsClient.h tr181_lanConfigSecurity.h \
				 tr181_deviceInfoVendorLogFile.h

ifeq ($(CONFIG_USER_CWMP_WITH_TR181_V6), y)
TR181_SOURCES += tr181_dhcpv6.c tr181_dhcpv6Client.c 

TR181_HEADERS += tr181_dhcpv6.h tr181_dhcpv6Client.h
endif

TR143_SOURCES := udpechoserver.c

TR143_HEADERS := prmt_tr143.h

ifeq ($(CONFIG_USER_CWMP_WITH_TR143), y)
TR098_SOURCES += prmt_tr143.c
TR098_HEADERS += prmt_tr143.h
TR181_SOURCES += prmt_tr143.c
TR181_HEADERS += prmt_tr143.h
endif

TR111_SOURCES := stun.c udp.c

TR111_HEADERS := stun.h udp.h

ifeq ($(CONFIG_USER_CWMP_WITH_TR069_ANNEX_G), y)
TR098_SOURCES += $(TR111_SOURCES)
TR098_HEADERS += $(TR111_HEADERS)
TR181_SOURCES += $(TR111_SOURCES)
TR181_HEADERS += $(TR111_HEADERS)
endif

# Output Declarance
DEPEND = ./.depend

EXEC = cwmpClient

ifeq ($(CONFIG_USER_CWMP_WITH_TR143), y)
TR143_EXEC = udpechoserver
else
TR143_EXEC = 
endif

FW_VERSION_OBJ = $(FW_VERSION_FILE:.c=.o)
TR098_OBJS = $(TR098_SOURCES:.c=.o)
TR181_OBJS = $(TR181_SOURCES:.c=.o)
TR143_OBJS = $(TR143_SOURCES:.c=.o)

ifeq ($(CONFIG_USER_CWMP_WITH_TR098), y)
ROOT_DATAMODEL = TR098
endif
ifeq ($(CONFIG_USER_CWMP_WITH_TR181), y)
ROOT_DATAMODEL = TR181
endif

OBJS := $($(ROOT_DATAMODEL)_OBJS)

SOURCES := $($(ROOT_DATAMODEL)_SOURCES)

DEPH := $($(ROOT_DATAMODEL)_HEADERS)

# Flags Definition
CWMPCLIENT_CFLAGS :=
CWMPCLIENT_LDFLAGS :=

# Support auto mtd partition
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
CWMPCLIENT_CFLAGS += -DMTD_NAME_MAPPING
CWMPCLIENT_LDFLAGS += -L$(MTDAPI_DIR) -lmtdapi
endif

ifeq ($(CONFIG_USER_CWMP_WITH_TRACEROUTE), y)
# support traceroute 
CWMPCLIENT_CFLAGS += -D_SUPPORT_TRACEROUTE_PROFILE_
endif

# support .LANDevice.{i}.LANHostConfigManagement.AllowedMACAddresses
CWMPCLIENT_CFLAGS += -D_CWMP_MAC_FILTER_

ifeq ($(CONFIG_USER_CWMP_WITH_TR069_ANNEX_G), y)
ifeq ($(CONFIG_USER_CWMP_WITH_OPENSSL),y)
CWMPCLIENT_CFLAGS += -DHAS_MESSAGE_INTEGRITY
endif
endif

CWMPCLIENT_INCLUDE := -I$(CWMP_ROOT)/include  -I$(CWMP_ROOT)/cwmpClient

CWMPCLIENT_LDFLAGS += -L$(APMIB_DIR) -lapmib \
					  -lpthread

ifeq ($(CONFIG_USER_CWMP_WITH_OPENSSL), y)
CWMPCLIENT_LDFLAGS += -L$(OPENSSL_DIR) -lssl -lcrypto -ldl
endif

ifeq ($(CONFIG_USER_CWMP_WITH_MATRIXSSL),y)
CWMPCLIENT_LDFLAGS += -L$(LIB_DIR) -lmatrixsslstatic
endif

ifeq ($(CONFIG_USER_CWMP_WITH_TR181), y)
CWMPCLIENT_INCLUDE += -I$(BOA_DIR)
# for log2()
CWMPCLIENT_LDFLAGS += -lm
endif

CFLAGS += $(COMMON_CFLAGS) $(CWMPCLIENT_CFLAGS) $(CWMPCLIENT_INCLUDE)
LDFLAGS += $(CWMPCLIENT_LDFLAGS)
LDFLAGS += $(CWMPCLIENT_LDFLAGS) -Wl,--gc-sections

# Beauty Output
quiet_CWMPCLIENT_MAKE = $(CC)     $@

all: depend $(HTTPDA_OBJ_y) $(EXEC) $(TR143_EXEC)

%.o: %.c $(DEPH) 
	$(Q)echo "$($(quiet)CWMPCLIENT_MAKE)"
	$(Q)$(CC) -Os $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJS) $(FW_VERSION_OBJ)
	$(Q)echo "$($(quiet)CWMPCLIENT_MAKE)"
	$(Q)$(CC) -Os -o $@ $(OBJS) $(CWMP_ROOT)/lib/$(Libcwmp_LIB_y) $(HTTPDA_OBJ_y) $(CWMP_ROOT)/lib/libgsoap$(LibPostfix).a $(FW_VERSION_OBJ) $(LDFLAGS)

$(TR143_EXEC): $(TR143_OBJS) $(TR143_HEADERS)
	$(Q)echo "$($(quiet)CWMPCLIENT_MAKE)"
	$(Q)$(CC) -Os -o $@ $(TR143_OBJS) $(LDFLAGS)

depend: $(SOURCES)
	$(Q)if [ ! -e $(DEPEND) ]; then \
		$(CPP) $(DEBUG) $(CFLAGS) $(IFLAGS) -MM $^ > $(DEPEND); \
	fi
	
-include $(DEPEND)

clean:
	-rm -f $(DEPEND)
	-rm -f $(EXEC) *.o udpechoserver

.PHONY: all depend clean
