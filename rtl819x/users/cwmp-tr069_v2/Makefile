
.EXPORT_ALL_VARIABLES:

include ./../.config

CWMP_ROOT=$(shell pwd)
ifeq ($(CONFIG_APP_OPENSSL-0.9.8I), y)
OPENSSL_DIR=$(DIR_ROOT)/users/openssl-0.9.8i
endif
ifeq ($(CONFIG_APP_OPENSSL-0.9.8Y), y)
OPENSSL_DIR=$(DIR_ROOT)/users/openssl-0.9.8y
endif
ifeq ($(CONFIG_APP_OPENSSL-1.0.2A), y)
OPENSSL_DIR=$(DIR_ROOT)/users/openssl-1.0.2a
endif
ifeq ($(CONFIG_APP_OPENSSL-1.0.2D), y)
OPENSSL_DIR=$(DIR_ROOT)/users/openssl-1.0.2d
endif
GSOAP_DIR=$(CWMP_ROOT)/gsoaplib
LIBCWMP_DIR=$(CWMP_ROOT)/libcwmp
CWMPCLIENT_DIR=$(CWMP_ROOT)/cwmpClient
MATRIXSSL_DIR=$(CWMP_ROOT)/matrixssl-1-8-3-open
LIB_DIR=$(CWMP_ROOT)/lib
INCLUDE_DIR=$(CWMP_ROOT)/include

LIBOPENSSL_INC_DIR=$(OPENSSL_DIR)/include
LIBMATRIXSSL_INC_DIR=$(MATRIXSSL_DIR)
BOA_DIR=$(DIR_ROOT)/users/boa/src
FW_VERSION_FILE := version.c
export CWMP_ROOT GSOAP_DIR LIBCWMP_DIR CWMPCLIENT_DIR LIB_DIR INCLUDE_DIR BOA_DIR OPENSSL_DIR MATRIXSSL_DIR LIBMATRIXSSL_INC_DIR FW_VERSION_FILE

ifeq ($(CONFIG_APP_GOAHEAD), y)
MIB_INCLUDE=-I$(CWMP_ROOT)/../goahead-2.1.1/LINUX
else
MIB_INCLUDE=-I$(DIR_ROOT)/users/boa/apmib
endif

ifeq ($(CONFIG_USER_CWMP_SSL_NO_SUPPORT), y)
LibPostfix :=
SSLLIB_INCLUDES :=
else
ifeq ($(CONFIG_USER_CWMP_WITH_OPENSSL), y)
LibPostfix := ssl
SSLLIB_INCLUDES = -I$(LIBOPENSSL_INC_DIR)
else
LibPostfix := mssl
SSLLIB_INCLUDES = -I$(LIBMATRIXSSL_INC_DIR)
endif
endif
Libcwmp_LIB_y := libcwmp_$(TOOLCHAIN).a
export SSLLIB_INCLUDES MIB_INCLUDE_DIR LibPostfix Libcwmp_LIB_y

# common cflags
# DLINUX is for matrixSSL
COMMON_CFLAGS += -DWITH_COOKIES -DLINUX=1
COMMON_CFLAGS += -ffunction-sections -fdata-sections
COMMON_CFLAGS += -I$(INCLUDE_DIR)
COMMON_CFLAGS += -I$(LIBCWMP_DIR) -I$(GSOAP_DIR) \
				   -I$(GSOAP_DIR)/soapcpp2 -I$(GSOAP_DIR)/soapcpp2/plugin \
				   $(MIB_INCLUDE) \
				   $(SSLLIB_INCLUDES)

# compile tricks, beautiful output
# use $(MAKE) V=1 to show compile flags
ifeq ("$(origin V)", "command line")
# show compile flags
LIBCWMP_VERBOSE = $(V)
else
LIBCWMP_VERBOSE = 0
endif
ifeq ($(LIBCWMP_VERBOSE), 0)
quiet=quiet_
Q = @
else
quiet=
Q =
endif

all: 
	@echo "***********Start compile TR-069*************"
ifeq ($(CONFIG_USER_CWMP_WITH_MATRIXSSL), y)
	$(Q)echo "compile matrixSSL"
	$(Q)if [ -d $(MATRIXSSL_DIR) ]; then \
		$(MAKE) -C $(MATRIXSSL_DIR)/src || exit "$$?"; \
		if [ ! -d lib ]; then mkdir lib; fi; \
		cp -f $(MATRIXSSL_DIR)/src/libmatrixsslstatic.a $(CWMP_ROOT)/lib/; \
	fi
	$(Q)$(MAKE) -C matrixssl-1-8-3-open/examples || exit "$$?"
endif
	$(Q)echo "compile gSoap"
	$(Q)if [ -d $(GSOAP_DIR) ]; then \
		cd $(GSOAP_DIR); \
		$(MAKE) || exit "$$?"; \
		$(MAKE) install || exit "$$?"; \
	fi
ifeq ($(CONFIG_USER_CWMP_WITH_TR181), y)
	$(Q)echo "compile libcwmp(tr181)"
	$(Q)if [ -d $(LIBCWMP_DIR) ]; then \
		cd $(LIBCWMP_DIR); \
		$(MAKE) || exit "$$?"; \
		$(MAKE) install || exit "$$?"; \
	fi
else
	$(Q)echo "compile libcwmp(tr098)"
	$(Q)if [ -d $(LIBCWMP_DIR) ]; then \
		cd $(LIBCWMP_DIR); \
		$(MAKE) || exit "$$?"; \
		$(MAKE) install || exit "$$?"; \
	fi
endif
ifeq ($(CONFIG_APP_TR104),y)
	@echo "compile voip"
	$(MAKE) -C $(CWMP_ROOT)/../rtk_voip/tr104/
	$(MAKE) -C $(CWMP_ROOT)/../rtk_voip/voip_manager/
endif
	$(Q)echo "compile cwmpClient"
	$(Q)if [ ! -e $(CWMPCLIENT_DIR)/$(FW_VERSION_FILE) ]; then \
		ln -s $(BOA_DIR)/$(FW_VERSION_FILE) $(CWMPCLIENT_DIR)/$(FW_VERSION_FILE); \
	fi
	$(Q)if [ -d $(CWMPCLIENT_DIR) ]; then \
		cd $(CWMPCLIENT_DIR); \
		$(MAKE) || exit "$$?"; \
	fi
	$(Q)@echo "***********End compile TR-069*************"

romfs:
	cp -f $(CWMP_ROOT)/cwmpClient/cwmpClient $(CWMP_ROOT)/../../romfs/bin
	cp -f $(CWMP_ROOT)/cwmpClient/DefaultCwmpNotify.txt $(CWMP_ROOT)/../../romfs/etc
	$(ROMFSINST) -s /var/cwmp_default /etc/cwmp_default
	$(ROMFSINST) -s /var/cwmp_config /etc/cwmp_config 
ifeq ($(CONFIG_USER_CWMP_WITH_TR143),y)
	cp -f $(CWMP_ROOT)/cwmpClient/udpechoserver $(CWMP_ROOT)/../../romfs/bin
endif
ifeq ($(CONFIG_USER_CWMP_WITH_SSL),y)
ifneq ($(CONFIG_USER_CWMP_DYNAMIC_SERV_AUTH), y)
	cp -f $(CWMP_ROOT)/cwmpClient/client.pem $(CWMP_ROOT)/../../romfs/etc
	cp -f $(CWMP_ROOT)/cwmpClient/cacert.pem $(CWMP_ROOT)/../../romfs/etc
endif
endif

clean:
	@if [ -d $(MATRIXSSL_DIR) ]; then \
		$(MAKE) -C $(MATRIXSSL_DIR)/src clean; \
		$(MAKE) -C $(MATRIXSSL_DIR)/examples clean; \
	fi
	@if [ -d $(GSOAP_DIR) ]; then \
		$(MAKE) -C $(GSOAP_DIR) clean; \
	fi
	@if [ -d $(LIBCWMP_DIR) ]; then \
		$(MAKE) -C $(LIBCWMP_DIR) clean; \
	fi
	
	$(MAKE) -C cwmpClient clean	
	-rm -f $(CWMPCLIENT_DIR)/$(FW_VERSION_FILE)

.PHONY: all romfs clean
