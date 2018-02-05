# $Id: Makefile.in,v 1.1.2.6 2002/12/12 21:29:04 jnelson Exp $


ifeq ($(wildcard $(DIR_LINUX)/.config),)
include ../../linux-2.6.30/.config
DIR_LINUX_KERNEL = ../../linux-2.6.30
else
KERNEL_VER=$(patsubst $(DIR_ROOT)/%,%,$(DIR_LINUX))
include $(DIR_LINUX)/.config
DIR_LINUX_KERNEL = $(DIR_LINUX)
endif

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../.config
else
include $(DIR_USERS)/.config
endif

ifndef DIR_RSDK
include ../../.config
ifeq ($(x86),1)
#
# x86 simulation for boa & flash
#
# usage: 
# 1. cd boa
# 2. make x86=1 (it will create x86 files, and copy to x86 dir)
# 3. cd x86 
# 
# * if APMIB_SHARED = 0 
# 1. ./flash default (it will create setting.bin to simulate flash)
# 2. ./boa -c . -r . (it will start boa, you may need run 'service httpd stop' first)
#
# * if APMIB_SHARED = 1
# 1. LD_LIBRARY_PATH=. ./flash default 
# 2. LD_LIBRARY_PATH=. ./boa -c . -r .
#
# Use browser to view html now!
#
export CC = gcc
export CPP = gcc -E
export AR = ar
export STRIP = strip
# uncomment it to disable strip behavior for gdb debug
#export STRIP = :
export DIR_USERS := $(shell pwd)/../
export DIR_LINUX := $(shell pwd)/../../linux-2.6.30
else
export PATH := $(shell pwd)/../../$(CONFIG_RSDKDIR)/bin:$(PATH)
CROSS_COMPILE ?= rsdk-linux-
export CC = $(CROSS_COMPILE)gcc
export CPP = $(CROSS_COMPILE)gcc -E
export AR = $(CROSS_COMPILE)ar
export STRIP = $(CROSS_COMPILE)strip
endif
export x86 = 1
endif

ifdef CONFIG_RTL_AP_PACKAGE 
CONFIG_RTL_AP = CONFIG_RTL_AP_PACKAGE
endif
#------------------------------------------------------------------------------

MAKE=make

.PHONY: clean mrclean distclean configcheck boa docs apmib utils system defconfig html tools cgi


ifeq ($(CONFIG_RTL8192CD), y)
COMMON_CFLAGS += -DCONFIG_WLAN_UI_SUPPORT
endif

ifeq ($(CONFIG_WLAN_HAL_8814AE),y)
COMMON_CFLAGS += -DCONFIG_WLAN_HAL_8814AE
ifeq ($(CONFIG_SLOT_0_8194AE),y)
COMMON_CFLAGS += -DCONFIG_SLOT_0_8194AE
endif
COMMON_CFLAGS += -DCONFIG_RTL_8812_SUPPORT
COMMON_CFLAGS += -DCONFIG_RTL_11AC_SUPPORT

ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
ifeq ($(CONFIG_USE_PCIE_SLOT_1),y)
COMMON_CFLAGS += -DCONFIG_RTL_8881A_SELECTIVE
COMMON_CFLAGS += -DCONFIG_RTL_92D_SUPPORT
COMMON_CFLAGS += -DCONFIG_RTL_DUAL_PCIESLOT_BIWLAN
COMMON_CFLAGS += -DCONFIG_RTL_DUAL_PCIESLOT_BIWLAN_D
COMMON_CFLAGS += -DFOR_DUAL_BAND
endif
endif
endif

ifeq ($(CONFIG_WLAN_HAL_8197F),y)
COMMON_CFLAGS += -DCONFIG_WLAN_HAL_8197F
ifeq ($(CONFIG_USE_PCIE_SLOT_0),y)
COMMON_CFLAGS += -DCONFIG_RTL_92D_SUPPORT
COMMON_CFLAGS += -DFOR_DUAL_BAND
COMMON_CFLAGS += -DCONFIG_RTL_DUAL_PCIESLOT_BIWLAN_D
endif
endif

#zj: add for QoS from upvel
GATEWAY += -DENABLE_QOS
GATEWAY += -DQOS_BY_BANDWIDTH
COMMON_CFLAGS += -DADVANCED_IPQOS
ifeq ($(CONFIG_RTL_EXT_PORT_SUPPORT),y)
	COMMON_CFLAGS += -DCONFIG_RTL_EXT_PORT_SUPPORT
endif
ifeq ($(CONFIG_RTL_HW_QOS_SUPPORT),y)
	COMMON_CFLAGS += -DCONFIG_RTL_HW_QOS_SUPPORT
endif
ifeq ($(CONFIG_RTL_8367_QOS_SUPPORT),y)
    COMMON_CFLAGS += -DCONFIG_RTL_8367_QOS_SUPPORT
endif

ifeq ($(CONFIG_USER_CWMP_WHTH_TR098_QOS),y)
COMMON_CFLAGS += -DQOS_OF_TR069
endif

ifeq ($(CONFIG_IPV6),y)
        COMMON_CFLAGS += -DCONFIG_IPV6
endif
ifeq ($(CONFIG_RTL_8367R_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_RTL_8367R_SUPPORT
endif

ifeq ($(CONFIG_WLAN_HAL_8822BE),y)
COMMON_CFLAGS += -DCONFIG_RTL_8812_SUPPORT
COMMON_CFLAGS += -DCONFIG_RTL_11AC_SUPPORT
ifeq ($(CONFIG_RTL_8196E), y)
else
COMMON_CFLAGS += -DCONFIG_RTL_92D_SUPPORT
endif
COMMON_CFLAGS += -DCONFIG_WLAN_HAL_8822BE
endif

ifeq ($(CONFIG_RTL_DUAL_BAND_SELECTIVE),y)
COMMON_CFLAGS += -DCONFIG_RTL_8881A_SELECTIVE
endif

ifdef CONFIG_RTL_WEBPAGES_IN_ROOTFS
COMMON_CFLAGS += -DCONFIG_RTL_WEBPAGES_IN_ROOTFS
endif

ifeq ($(CONFIG_RTL_8814_8194_2T2R_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_RTL_8814_8194_2T2R_SUPPORT
endif

ifeq ($(CONFIG_4G_LTE_SUPPORT), y)
COMMON_CFLAGS += -DCONFIG_4G_LTE_SUPPORT
endif
ifeq ($(CONFIG_BAND_2G_ON_WLAN0), y)
COMMON_CFLAGS += -DCONFIG_BAND_2G_ON_WLAN0
endif

ifeq ($(CONFIG_BAND_5G_ON_WLAN0), y)
COMMON_CFLAGS += -DCONFIG_BAND_5G_ON_WLAN0
endif

ifeq ($(CONFIG_RTK_OPENVPN_HW_CRYPTO),y)
COMMON_CFLAGS += -DCONFIG_RTK_OPENVPN_HW_CRYPTO
endif

ifeq ($(CONFIG_APP_OPENVPN),y)
COMMON_CFLAGS += -DCONFIG_APP_OPENVPN
endif

ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
COMMON_CFLAGS += -DMTD_NAME_MAPPING
endif

ifeq ($(CONFIG_RTL_AP_PACKAGE),y)
COMMON_CFLAGS += -DCONFIG_RTL_AP_PACKAGE
endif

ifeq ($(CONFIG_APP_RTK_EVENTD_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_RTK_EVENTD_SUPPORT
endif

ifeq ($(CONFIG_RTL_TDLS_SUPPORT),y)
COMMON_CFLAGS += -DTDLS_SUPPORT
endif

ifeq ($(CONFIG_RTL_11V_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_IEEE80211V
endif

ifeq ($(CONFIG_RTK_SMART_ROAMING),y)
COMMON_CFLAGS += -DRTK_CAPWAP
COMMON_CFLAGS += -DRTK_SMART_ROAMING
endif

ifeq ($(CONFIG_RF_DPK_SETTING_SUPPORT),y)
COMMON_CFLAGS += -DCONFIG_RF_DPK_SETTING_SUPPORT
endif

#------------------------------------------------------------------------------
# Targets
#------------------------------------------------------------------------------
ifeq ($(CONFIG_RTL_8196C_iNIC),y)
#iNIC no boa ,only need tools to build code
TARGET = tools
else
ifeq ($(CONFIG_RTL_8198_INBAND_AP),y)
TARGET = tools
else
TARGET = tools defconfig apmib utils system boa cgi html
endif
endif

all: configcheck generate_mk $(TARGET)
ifeq ($(x86),1)
	rm -rf x86
	mkdir x86
	cp -av html x86/web
	cp src/boa x86/
	cp utils/flash x86/
	cp src/boa.conf.x86 x86/boa.conf
	cp src/mime.types x86/
ifeq ($(APMIB_SHARED),1)
	cp apmib/libapmib.so x86/
endif
endif

generate_mk:
	@echo "SYS_COMMON_CFLAGS=$(COMMON_CFLAGS)" > ./wfaudio.include.mk
	@echo "export SYS_COMMON_CFLAGS" >> ./wfaudio.include.mk

configcheck:
	@echo "" > ./.diff_config;
	@if [ -e .kernel_config ]; then \
		diff .kernel_config $(DIR_LINUX_KERNEL)/.config > .diff_config; \
		if [ -s .diff_config ]; then \
		        cp $(DIR_LINUX_KERNEL)/.config .kernel_config; \
			make clean; \
                fi; \
        else \
                cp $(DIR_LINUX_KERNEL)/.config ./.kernel_config; \
	fi;

	@echo "" > ./.diff_users_config;
	@if [ -e .users_config ]; then \
		diff .users_config $(DIR_USERS)/.config > .diff_users_config; \
		if [ -s .diff_users_config ]; then \
			cp $(DIR_USERS)/.config .users_config; \
			make	clean; \
		fi; \
	else \
		cp $(DIR_USERS)/.config ./.users_config; \
	fi;	
#boa:
#	(cd src && $(MAKE) $(MFLAGS))

boa:
	make -C src

boa-clean:
	make -C src clean

apmib:
	make -C apmib

apmib-clean:
	make -C apmib clean

utils:
	make -C utils

utils-clean:
	make -C utils clean
	
system:
	make -C system

system-clean:
	make -C system clean

defconfig:
	make -C defconfig 
	
defconfig-clean:
	make -C defconfig clean
	
tools:
	make -C tools

tools-clean:
	make -C tools clean
	
cgi:
ifeq ($(BOA_CGI_SUPPORT),y)
	make -C cgi
endif
cgi-clean:
ifeq ($(BOA_CGI_SUPPORT),y)
	make -C cgi clean
endif
#html: tools
html:
	make -C html

html-clean:
	make -C html clean

docs:
	(cd docs && $(MAKE) $(MFLAGS))

clean:
	-(cd src && $(MAKE) $(MFLAGS) clean)
	-(cd docs && $(MAKE) clean)
	make apmib-clean
	make utils-clean
	make system-clean
	make boa-clean
	make defconfig-clean
	make html-clean
	make tools-clean
	make cgi-clean

distclean:	mrclean

mrclean:	clean
	-(cd src && $(MAKE) $(MFLAGS) mrclean)
	-(cd docs && $(MAKE)$(MFLAGS) mrclean)
	rm -f config.status config.cache config.h config.log 
	rm -f Makefile *~

# dummy target for install
install:
	@echo Please read the documentation then install manually.

romfs:
ifeq ($(APMIB_SHARED),1)
	$(ROMFSINST) apmib/$(APMIB_LIB) /lib/libapmib.so
endif
ifeq ($(CONFIG_MTD_NAME_MAPPING),y)
	$(ROMFSINST) $(DIR_ROOT)/users/lib/lib/libmtdapi.so /lib/libmtdapi.so
endif
	$(ROMFSINST) -s /var/resolv.conf /etc/resolv.conf
ifneq ($(CONFIG_RTL_8196C_iNIC),y)
ifneq ($(CONFIG_RTL_8198_INBAND_AP),y)
ifneq ($(CONFIG_RTL_NFJROM_MP),y)
ifneq ($(CONFIG_RTL_8198_AP_ROOT),y)
ifneq ($(CONFIG_RTL_8197D_AP),y)
ifneq ($(CONFIG_RTL_AP_PACKAGE),y)
	$(ROMFSINST) system/ppp_inet /bin/ppp_inet
	$(ROMFSINST) system/ntp_inet /bin/ntp_inet
	$(ROMFSINST) system/ddns_inet /bin/ddns_inet
ifeq  ($(CONFIG_IPV6_DNSV6_MODE_SUPPORT),y)
	$(ROMFSINST) system/ipv6_manage_inet /bin/ipv6_manage_inet
endif
ifeq  ($(CONFIG_IPV6_CE_ROUTER_SUPPORT),y)
        $(ROMFSINST) system/ipv6_manage_inet /bin/ipv6_manage_inet
endif
endif	#CONFIG_RTL_AP_PACKAGE
endif  #CONFIG_RTL_8197D_AP
endif	#CONFIG_RTL_8198_AP_ROOT
	$(ROMFSINST) src/boa /bin/boa

ifeq ($(CONFIG_APP_RTK_INBAND_CTL), y)	
	$(ROMFSINST) src/showwanstatus /bin/showwanstatus
endif

	$(ROMFSINST) -d src/boa.conf /etc/boa/boa.conf
	$(ROMFSINST) -d src/mime.types /etc/boa/mime.types
ifdef CONFIG_RTL_WEBPAGES_IN_ROOTFS
	rm -rf tmp-html
	mkdir tmp-html
	cd tmp-html && tar jxvf ../html/www_files.tar.bz2 && cd -
	mv tmp-html $(DIR_ROMFS)/web
	$(ROMFSINST) -s /var/config.dat /web/config.dat
ifeq ($(CONFIG_APP_OPENVPN), y)
	$(ROMFSINST) -s /var/openvpn/client.ovpn /web/client.ovpn
endif
	$(ROMFSINST) -s /var/ca.cer /web/ca.cer
	$(ROMFSINST) -s /var/user.cer /web/user.cer
else
	$(ROMFSINST) -s /var/web /web
endif
endif	#CONFIG_RTL_NFJROM_MP
	$(ROMFSINST) utils/flash /bin/flash
ifeq ($(CONFIG_APP_FWD),y)	
	$(ROMFSINST) utils/fwd /bin/fwd
endif
	$(ROMFSINST) utils/main_lc5761 /bin/main_lc5761
ifeq ($(CONFIG_RTK_SMART_ROAMING_),y)	
	$(ROMFSINST) utils/fwupg /bin/fwupg
endif

ifneq ($(CONFIG_RTL_NFJROM_MP),y)
	$(ROMFSINST) system/sysconf /bin/sysconf
endif
	$(ROMFSINST) system/timelycheck /bin/timelycheck
ifeq ($(CONFIG_APP_RTK_EVENTD_SUPPORT),y)
	$(ROMFSINST) system/rtk_eventd /bin/rtk_eventd
endif
ifeq ($(CONFIG_AUTO_DHCP_CHECK),y)
	$(ROMFSINST) system/Auto_DHCP_Check /bin/Auto_DHCP_Check
endif
endif	#CONFIG_RTL_8198_INBAND_AP
endif	#CONFIG_RTL_8196C_iNIC
ifeq ($(CONFIG_RTL_DNS_TRAP),y)
	$(ROMFSINST) -s /var/hosts /etc/hosts
endif
ifeq ($(CONFIG_RTL_COMAPI_CFGFILE),y)
	rm -rf Wireless
	mkdir Wireless
	mv Wireless $(DIR_ROMFS)/etc
	$(ROMFSINST) -s /var/RTL8192CD.dat  /etc/Wireless/RTL8192CD.dat
endif
	cp ./src/certificate.crt ./../../romfs/etc
	cp ./src/privateKey.key ./../../romfs/etc
ifeq ($(SHOW_LAN_DEV_INFO),1)
	$(ROMFSINST) src/device_mac_brand.txt /etc/device_mac_brand.txt
endif
ifeq ($(CONFIG_SYS_DIAGNOSTIC),y)
ifdef CONFIG_RTL_WEBPAGES_IN_ROOTFS
	$(ROMFSINST) -s /var/sys_diagnostic.txt /web/sys_diagnostic.txt
endif
endif
