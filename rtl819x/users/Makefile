#
# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# Dec. 07, 2008
#

DIR_ROOT ?= ..
DIR_BOARD ?= $(DIR_ROOT)/target

.EXPORT_ALL_VARIABLES:
.PHONY: all romfs clean prune

ifneq (.config,$(wildcard .config))
all: config
else
all: libbase lib_old app

include .config
include $(DIR_ROOT)/.config
include $(DIR_LINUX)/.config
include COMMON_FLAGS.mk

SELECT_WAPI_LOCAL_AS=0
SELECT_1X_CLIENT=0
SELECT_CWMP=0
SELECT_ETH_1X_CLIENT=0
SELECT_OPENVPN=0
SELECT_FT_DAEMON=0
SELECT_TRANSMISSION=0
openssl_dir= 
# 8198 and POCKET ROUTER support both wapi psk and wapi cert
# 8196C (not include POCKET ROUTER) only support wapi psk

ifdef CONFIG_POCKET_ROUTER_SUPPORT
ifdef CONFIG_RTL_WAPI_SUPPORT
# ############################################
# Because rootfs + 64K wapi-cert is oversize at 2M/16M,
# close WAN apps here just for wapi logo test!!!!!!
CONFIG_APP_PPPD=n
CONFIG_APP_PPTP=n
CONFIG_APP_L2TPD=n
CONFIG_APP_NTPCLIENT=n
CONFIG_APP_DNRD=n
CONFIG_APP_IPROUTE=n
# ############################################
endif
endif

APMIBINC=
APMIBLIB=
ifeq ($(CONFIG_APP_BOA),y)
APMIBINC = $(DIR_USERS)/boa/apmib/
APMIBLIB = $(DIR_USERS)/boa/apmib/libapmib.so
BOA_CGI_SUPPORT := $(CONFIG_APP_BOA_CGI)
else
APMIBINC = $(DIR_USERS)/goahead-2.1.1/LINUX/
APMIBLIB = $(DIR_USERS)/goahead-2.1.1/LINUX/apmib.so
endif
	
ifeq ($(CONFIG_USER_CWMP_OPENSSL_SUPPORT),y)
SELECT_CWMP=1
endif

ifeq ($(CONFIG_RTL_WAPI_SUPPORT),y)
CONFIG_APP_WAPI=y
ifeq ($(CONFIG_RTL_WAPI_LOCAL_AS_SUPPORT),y)
SELECT_WAPI_LOCAL_AS=1
endif
endif

ifeq ($(CONFIG_RTL_802_1X_CLIENT_SUPPORT),y)
SELECT_1X_CLIENT=1
endif

ifeq ($(CONFIG_RTL_ETH_802DOT1X_CLIENT_MODE_SUPPORT),y)
SELECT_ETH_1X_CLIENT=1
endif

ifeq ($(CONFIG_APP_OPENVPN),y)
SELECT_OPENVPN=1
endif

ifeq ($(CONFIG_FT_DAEMON),y)
SELECT_FT_DAEMON=1
endif

ifeq ($(CONFIG_APP_TRANSMISSION),y)
SELECT_TRANSMISSION=1
endif

ifdef CONFIG_APP_IP6TABLES
CONFIG_APP_IP6TABLES_SUPPORT=1
else
CONFIG_APP_IP6TABLES_SUPPORT=0
endif

ifeq ($(CONFIG_APP_WEAVE),y)
CONFIG_APP_OPENSSL-0.9.8Y=y
CONFIG_LIB_OPENSSL_098Y=y

CONFIG_APP_OPENSSL-0.9.8I=n
CONFIG_LIB_OPENSSL_098I=n
openssl_dir=openssl-0.9.8y

else
ifneq ($(SELECT_WAPI_LOCAL_AS)_$(SELECT_1X_CLIENT)_$(SELECT_CWMP)_$(SELECT_ETH_1X_CLIENT)_$(SELECT_OPENVPN)_$(SELECT_FT_DAEMON)_$(SELECT_TRANSMISSION), 0_0_0_0_0_0_0)
CONFIG_APP_OPENSSL-0.9.8I=y
CONFIG_LIB_OPENSSL_098I=y

CONFIG_APP_OPENSSL-0.9.8Y=n
CONFIG_LIB_OPENSSL_098Y=n
openssl_dir=openssl-0.9.8i
endif
endif

export openssl_dir

ifdef CONFIG_RTL_COMAPI_WLTOOLS
WLTOOLS=wireless_tools.29
else
WLTOOLS=wireless_tools.25
endif

ifdef CONFIG_RTL_HS2_SUPPORT
CONFIG_APP_HS2=y
endif

CROSS_TARGET ?= rsdk-linux
CROSS_COMPILE ?= rsdk-linux-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip
SSTRIP = $(CROSS_COMPILE)sstrip

CFLAGS += -I$(DIR_TMPFS)/include

CINIT=1

app_y =
ifeq ($(CONFIG_APP_RTK_VOIP),y)
app_y = script/mdev
endif

app_n =
app_  =

lib_y = 
lib_n =
lib_  =

libbase_y = 
libbase_n =
libbase_  =
ifeq ($(CONFIG_APP_BLUEZ_522),y)
	BLUEZ_DIR = bluez-5.22
else
	ifeq ($(CONFIG_APP_BLUEZ_496),y)
		BLUEZ_DIR = bluez-4.96
	endif
	ifeq ($(CONFIG_APP_BLUEZ_524),y)
		BLUEZ_DIR = bluez-5.24
	endif
endif

ifeq ($(CONFIG_APP_RTK_VOIP),y)
	#ifeq ($(CONFIG_APP_RTK_VOIP_E8C),y)
	#	RTK_VOIP_DIR = rtk_voip-e8c
	#else
		ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxE
			RTK_VOIP_DIR = rtk_voip-89xxE
		else
			RTK_VOIP_DIR = rtk_voip
		endif
	#endif
endif

export CFLAGS

ifeq ($(wildcard $(DIR_LINUX)/.config),)
DIR_LINUX_KERNEL = $(DIR_ROOT)/linux-2.6.30
KERNEL_VER=linux-2.6.30
else
KERNEL_VER=$(patsubst $(DIR_ROOT)/%,%,$(DIR_LINUX))
DIR_LINUX_KERNEL = $(DIR_LINUX)
endif

export KERNEL_VER

# application
app_$(CONFIG_APP_OPENSSL-0.9.8Y)	+= openssl-0.9.8y
app_$(CONFIG_APP_OPENSSL-0.9.8I)	+= openssl-0.9.8i
#app_$(CONFIG_APP_OPENSSL-0.9.8B)	+= openssl-0.9.8b
app_$(CONFIG_APP_OPENSSL-1.0.2D)	+= openssl-1.0.2d
app_$(CONFIG_APP_OPENSSL-1.0.2A)        += openssl-1.0.2a
app_$(CONFIG_APP_BUSYBOX)		+= busybox
app_$(CONFIG_APP_GOAHEAD)		+= goahead-2.1.1
app_$(CONFIG_APP_UBUS)		+= ubus-2014-09-17
app_$(CONFIG_APP_MBEDTLS)		+= mbedtls-2.2.1
app_$(CONFIG_APP_BOA)			+= boa
app_$(CONFIG_APP_ADAPTER_API)           += adapter-api
app_$(CONFIG_APP_REINIT)	+= reinit
ifeq ($(CINIT), 1)
app_$(CONFIG_APP_SCRIPT)		+= script/cinit
else
app_$(CONFIG_APP_SCRIPT)		+= script
endif
app_$(CONFIG_APP_SQUASHFS40)		+= squashfs4.0/squashfs-tools
app_$(CONFIG_APP_SQUASHFS42)		+= squashfs4.2/squashfs-tools
app_$(CONFIG_APP_IPROUTE)		+= iproute2-2.6.29-1
#app_$(CONFIG_APP_IPROUTE)		+= iproute2-3.10.0
app_$(CONFIG_APP_IPTABLES)		+= iptables-1.4.4
app_$(CONFIG_APP_L7FILTER_PAT)		+= l7-protocols
app_$(CONFIG_APP_NTPCLIENT)		+= ntpclient
app_$(CONFIG_APP_CURL)			+= curl-7.36.0
app_$(CONFIG_APP_DHCPV6)		+= wide-dhcpv6
app_$(CONFIG_APP_RADVD)			+= radvd-0.9.1
app_$(CONFIG_APP_NDISC6)		+= ndisc6-1.0.3
app_$(CONFIG_APP_ECMH)			+= ecmh
app_$(CONFIG_APP_GDBSERVER)             += gdb
app_$(CONFIG_APP_USBMOUNT)		+= usbmount
app_$(CONFIG_APP_VSFTPD)		+= vsftpd-2.3.2
app_$(CONFIG_APP_DOSFSTOOLS)		+= dosfstools-4.0
app_$(CONFIG_APP_E2FSPROGS)		+=e2fsprogs-1.42.11
#ifeq ($(CONFIG_APP_SAMBA_WEB), y)
app_$(CONFIG_APP_NTFS3G) 		+=ntfs-3g_ntfsprogs-2014.2.15
#else
#app_$(CONFIG_APP_NTFS3G)		+= ntfs-3g-2010.10.2
#endif
app_$(CONFIG_APP_PARTED)		+= parted-3.0
app_$(CONFIG_APP_MP)			+= mp-daemon
app_$(CONFIG_APP_TRANSMISSION)		+= libevent-2.0.21-stable
app_$(CONFIG_APP_TRANSMISSION)		+= curl-7.34.0
#app_$(CONFIG_APP_TRANSMISSION)		+= openssl-fips-2.0.5
app_$(CONFIG_APP_TRANSMISSION)		+= transmission-2.82
app_$(CONFIG_APP_TRANSMISSION)		+= transmission-web
app_$(CONFIG_APP_TRANSMISSION)		+= http_test
app_$(CONFIG_LIB_Z)			+= zlib-1.2.5
app_$(CONFIG_LIB_CURL)			+= curl-7.34.0
app_$(CONFIG_LIB_LIBDAEMON)		+= libdaemon-0.14
#CONFIG_APP_SAMBA-3.0.37 OR CONFIG_APP_SAMBA-3.0.24 at most one will be chose
#app_$(CONFIG_APP_SAMBA-3.0.37)		+= samba-3.0.37/source
app_$(CONFIG_APP_SAMBA)		+= samba
#app_$(CONFIG_APP_SMABA-3.2.15)		+= samba
#ifeq($(CONFIG_APP_SAMBA-3.0.24),y)
#else
#ifeq($(CONFIG_APP_SAMBA-3.0.37),y)
#else
#ifeq($(CONFIG_APP_SAMBA-3.2.15),y)
#endif

app_$(CONFIG_APP_GPROXY)		+= gproxy
app_$(CONFIG_APP_IGMPPROXY)		+= igmpproxy
app_$(CONFIG_APP_MLDPROXY)      += mldproxy
app_$(CONFIG_APP_PATHSEL)		+= 11s-PathSelection
app_$(CONFIG_APP_WIRELESSTOOLS)	+= $(WLTOOLS)
app_$(CONFIG_APP_DNRD)			+= dnrd-2.12.1/src
app_$(CONFIG_APP_ROUTED)		+= routed
app_$(CONFIG_APP_BIRD)			+= bird-1.4.0
app_$(CONFIG_APP_RTK_CMD)		+= rtk_cmd 
app_$(CONFIG_APP_PPPD)			+= ppp-2.4.4/pppd
app_$(CONFIG_APP_PPTP)			+= pptp-1.7.2
app_$(CONFIG_APP_L2TPD)			+= l2tpd
app_$(CONFIG_APP_DNSMASQ)		+= dnsmasq-2.33
app_$(CONFIG_APP_MINI_UPNP)		+= mini_upnp
app_$(CONFIG_APP_WSC)			+= wsc 
app_$(CONFIG_APP_AUTH)			+= auth/src 
app_$(CONFIG_APP_IWCONTROL)		+= auth/src/dlisten
app_$(CONFIG_APP_AVAHI)			+= avahi-0.6.31
app_$(CONFIG_APP_HOSTAPD)		+= hostapd-2.0/hostapd
app_$(CONFIG_APP_UDHCPD)		+= udhcp-0.9.9-pre
app_$(CONFIG_APP_BRCTL)			+= bridge-utils
app_$(CONFIG_APP_MINIIGD)		+= miniigd
app_$(CONFIG_APP_RELOAD)		+= rc
app_$(CONFIG_APP_LLTDD)			+= lltdd/native-linux
app_$(CONFIG_APP_ORAYDDNS)		+= orayddns
app_$(CONFIG_APP_UPDATEDD)		+=  updatedd-2.5
app_$(CONFIG_APP_DISCOVER)		+=  discover
app_$(CONFIG_APP_IAPP)			+= IAPP 
lib_$(CONFIG_APP_RTK_VOIP)		+= $(RTK_VOIP_DIR)
app_$(CONFIG_APP_RTK_VOIP)		+= $(RTK_VOIP_DIR)
app_$(CONFIG_APP_RTL8686_IPC_UTIL)		+= aipc_util
app_$(CONFIG_APP_TCP2DEV)		+= tcp2dev
app_$(CONFIG_APP_PMC_UTILS)		+= pmc_utils
app_$(CONFIG_APP_NFBI)			+= nfbi
app_$(CONFIG_APP_HOSTAPD_SLAVE)			+= hostapd_slave
app_$(CONFIG_APP_RTK_INBAND_CTL)	+= hcm
app_$(CONFIG_APP_RTK_INBAND_HOST)       += rtk_inbandHost
app_$(CONFIG_APP_CTORRENT)              += ctorrent-dnh3.3.2
app_$(CONFIG_APP_DCTS)			+= dctcs.src/src
app_$(CONFIG_APP_SNMP)			+= net-snmp-5.4.1
app_$(CONFIG_APP_HS2)			+= hs2
app_$(CONFIG_APP_IOH)			+= ioh
app_$(CONFIG_APP_NFBI_HOST)		+= nfbihost/nfbi
app_$(CONFIG_APP_TR069)			+= cwmp-tr069_v2
app_$(CONFIG_APP_UWIFI)		+= uWiFi
app_$(CONFIG_APP_NMBS)		+= nbserver
app_$(CONFIG_APP_ALSA_AUDIO)	+= alsa_related
app_$(CONFIG_APP_AUTORUNIMG)    += autorun_img_tool
app_$(CONFIG_APP_SIMPLE_CONFIG)    += simple_config
app_$(CONFIG_APP_PYTHON2_SUPPORT)    += python2
app_$(CONFIG_APP_PYTHON3_SUPPORT)    += python3

# USB3G
app_$(CONFIG_APP_COMGT)			+= comgt.0.32
app_$(CONFIG_APP_CHAT)			+= ppp-2.4.4/chat
app_$(CONFIG_APP_USBMODESWITCH)		+= usb-modeswitch
app_$(CONFIG_APP_MBPK_EJECT)		+= mbpk_eject
app_$(CONFIG_APP_USBMODESWITCH_DATA)	+= usb-modeswitch-data
app_$(CONFIG_APP_HUBCTRL)		+= hub-ctrl
app_$(CONFIG_APP_MNET)		        += mnet
ifeq ($(KERNEL_VER),linux-2.6.30)
app_$(CONFIG_APP_LIBUSB)		+= libusb-0.1.12
app_$(CONFIG_APP_USBUTILS)		+= usbutils-0.86
endif
ifeq ($(KERNEL_VER),linux-3.10)
app_$(CONFIG_APP_USBUTILS)		+= usb-k310/usbutils-007
endif

#Oprofile
lib_$(CONFIG_LIB_POPT)		+= popt
app_$(CONFIG_APP_BINUTILS)	+= binutils 
app_$(CONFIG_APP_OPROFILE)	+= oprofile

# debug & test
app_$(CONFIG_APP_CLE_SHELL)		+= cle-shell
app_$(CONFIG_APP_UTILS)			+= utils
app_$(CONFIG_APP_IPERF)			+= iperf-2.0.4
app_$(CONFIG_APP_DHRYSTONE)		+= dhrystone
app_$(CONFIG_APP_IOZONE)		+= iozone3_327/src/current
app_$(CONFIG_APP_EXAMPLE)		+= example
app_$(CONFIG_APP_LTP)			+= ltp
app_$(CONFIG_APP_TEST11K)		+= dot11k/test_11k
app_$(CONFIG_APP_TEST11V)		+= dot11v/test_11v

app_$(CONFIG_APP_OPENSSL)		+= 	
app_$(CONFIG_APP_NDP_PROXY)			+= ndppd

app_$(CONFIG_APP_WAPI)			+= wapi_utils/udp_sock wapi_utils/rwWapiFiles \
					  wapi_utils/other_tools wapi_utils/wapi_scripts
app_$(CONFIG_APP_HOSTAPD_HCM)           += hostapd-0.6.10/hostapd inband_lib
app_$(CONFIG_APP_DLNA_DMS)		+= dlna_dms
app_$(CONFIG_APP_ETH_CTRL)      += ethctl
app_$(CONFIG_APP_MT_DAAPD)		+= mt-daapd-0.2.4.2

app_$(CONFIG_APP_FLATFSD) +=flatfsd
app_$(CONFIG_APP_SIGMA_UTILS)		+=  sigma_utils


app_$(CONFIG_APP_MDADM)		+=  mdadm-3.2.5

app_$(CONFIG_LIB_LIBPCAP)			+= libpcap-1.4.0
app_$(CONFIG_APP_PCAP)				+=  pcap
app_$(CONFIG_FT_DAEMON)			    +=  ft_daemon
app_$(CONFIG_DOT11K_DEAMON)			    +=  dot11k/deamon
app_$(CONFIG_RTK_SMART_ROAMING)			+=opencapwap
app_$(CONFIG_APP_OPENVPN)				+= openvpn-2.3.4
app_$(CONFIG_APP_BLUEZ)  +=$(BLUEZ_DIR)
app_$(CONFIG_APP_RTK_BLUETOOTH_FM)  +=rtk_bluetooth_firmware
app_$(CONFIG_LIB_BTCONFIG)		 += libbtconfig
app_$(CONFIG_APP_BT_REPEATER_CONFIG)  += rtk_bt_repeater_config
app_$(CONFIG_APP_BT_REPEATER_CONFIG)  += rtk_ble_repeater_config
app_$(CONFIG_APP_RTK_HCIATTACH)  += rtk_hciattach
app_$(CONFIG_APP_BT_MP)  += bt_mp_tool

ifdef CONFIG_RTL_JFFS2_PARTITION_ENABLE
app_y += mtd-utils
else
app_$(CONFIG_APP_MTD_UTILS) += mtd-utils
endif
app_$(CONFIG_APP_UPIMAGE) += upImage/lrzsz/lrzsz-0.12.20/
app_$(CONFIG_APP_UPIMAGE) += upImage/flash_writer
app_$(CONFIG_MAP_E_SUPPORT) += MAP-master
app_$(CONFIG_APP_NFS_UTILITY) +=portmap_6.0
app_$(CONFIG_APP_NFS_UTILITY) +=unfs3-0.9.22
app_$(CONFIG_APP_CPU_UTILIZATION)       += cpu
app_$(CONFIG_APP_EXFAT_FUSE)		+= fuse-exfat-1.1.0
app_$(CONFIG_RTL_USERSPACE_WTDOG)       += watchdog
app_$(CONFIG_APP_TRACEROUTE)		+= traceroute-2.0.21
ex_mod_$(CONFIG_RTL_HIGH_PERFORMANCE_FILESYSTEM)	+= rtl_fs
app_$(CONFIG_APP_MINIDLNA)  +=minidlna
ifeq ($(CONFIG_APP_PHP), y)
app_$(CONFIG_APP_PHP)                   += php-5.6.19
endif
app_$(CONFIG_REALTEK_CRYPTO_API) += crypto-api
# libraries

lib_$(CONFIG_APP_HOSTAPD_HCM)		+= inband_lib
lib_$(CONFIG_APP_HOSTAPD_SLAVE)		+= inband_lib
lib_$(CONFIG_APP_RTK_INBAND_CTL)	+= rtk_inbandLib
lib_$(CONFIG_APP_RTK_INBAND_HOST)	+= rtk_inbandLib
lib_$(CONFIG_LIB_NL)				+= libnl-1.1
lib_$(CONFIG_APP_FLEX)			+= flex-2.5.33
lib_$(CONFIG_LIB_Z)			+= zlib-1.2.5
lib_$(CONFIG_APP_PPPD)			+= ppp-2.4.4/pppd/plugins/rp-pppoe

lib_$(CONFIG_LZO)                   	+= lzo-2.08
ifeq ($(CONFIG_APP_SNMP),y)
lib_$(CONFIG_APP_GOAHEAD)		+= goahead-2.1.1
lib_$(CONFIG_APP_BOA)		+= boa
endif
ifeq ($(CONFIG_APP_TR069),y)
lib_$(CONFIG_APP_GOAHEAD)		+= goahead-2.1.1
lib_$(CONFIG_APP_BOA)		+= boa
endif
app_$(CONFIG_APP_WEAVE)			+= libweave

#homekit stuff 
app_$(CONFIG_LIB_POLARSSL)    += polarssl-1.3.8
app_$(CONFIG_APP_APPLE_MFI_WAC) += mfi_wac

app_$(CONFIG_APP_ZIGBEE)       += zigbee-jip-daemon

### libbase
libbase_y				+= common
libbase_$(CONFIG_LIB_EXPAT) 		+= expat-2.1.0
libbase_$(CONFIG_LIB_DBUS) 		+= dbus-1.8.0
libbase_$(CONFIG_LIB_LIBDAEMON)		+= libdaemon-0.14
libbase_$(CONFIG_LIB_XZ)		+= xz-5.0.8
libbase_$(CONFIG_LIB_LIBUSB)		+= libusb-1.0.9
libbase_$(CONFIG_LIB_LIBICONV)		+= libiconv-1.14
libbase_$(CONFIG_LIB_FUSE)		+= fuse-2.9.0
ifeq ($(KERNEL_VER),linux-2.6.30)
libbase_$(CONFIG_LIB_USBUTILS)		+= usbutils-007
endif
libbase_$(CONFIG_LIB_READLINE)		+= readline-6.3
libbase_$(CONFIG_LIB_TERMCAP)		+= termcap-1.3.1
libbase_$(CONFIG_LIB_LIBFFI) 		+= libffi-3.0.13
libbase_$(CONFIG_LIB_GETTEXT) 		+= gettext-0.18.3.1
libbase_$(CONFIG_LIB_ZLIB_128) 		+= zlib-1.2.8
libbase_$(CONFIG_LIB_GLIB) 		+= glib-2.36.0
libbase_$(CONFIG_LIB_ALSA) 		+= alsa-1.0.25
libbase_$(CONFIG_LIB_LIBSNDFILE) 	+= libsndfile-1.0.25
libbase_$(CONFIG_LIB_JSON)		+= json-c-0.12
libbase_$(CONFIG_LIB_LIBUBOX)		+= libubox-2014-08-04
libbase_$(CONFIG_LIB_UBUS)		+= ubus-2014-09-17
#libbase_$(CONFIG_LIB_OPENSSL_098B)	+= openssl-0.9.8b
libbase_$(CONFIG_LIB_OPENSSL_098I)	+= openssl-0.9.8i
libbase_$(CONFIG_LIB_OPENSSL_098Y)	+= openssl-0.9.8y
libbase_$(CONFIG_LIB_OPENSSL_102D)	+= openssl-1.0.2d
libbase_$(CONFIG_LIB_LZO)		+= lzo-2.08

CONFIG_LIB_LIBEVENT_2022 = 
ifeq ($(CONFIG_APP_LIBEVENT),y)
	CONFIG_LIB_LIBEVENT_2022 = y
endif
libbase_$(CONFIG_LIB_LIBEVENT_2022)	+= libevent-2.0.22-stable

libbase_$(CONFIG_LIB_ZLIB_125)		+= zlib-1.2.5
libbase_$(CONFIG_LIB_ZLIB_123)		+= zlib-1.2.3
libbase_$(CONFIG_LIB_LIBID3TAG)		+= libid3tag-0.15.1b
libbase_$(CONFIG_LIB_LRZSZ)		+= upImage/lrzsz/lrzsz-0.12.20
libbase_$(CONFIG_LIB_LIBEVENT_2021)	+= libevent-2.0.21-stable


lib_old:
ifneq ($(lib_y),)
	$(MAKE) $(lib_y) || exit $$?
else
	@echo "No libs specified"
endif
app:
	$(MAKE) $(app_y) || exit $$?
ex_mod:
ifneq ($(ex_mod_y),)
	$(MAKE) -C $(ex_mod_y) || exit $$?
	[ -e $(DIR_ROMFS)/lib/modules ] || mkdir -p $(DIR_ROMFS)/lib/modules
	for i in $(ex_mod_y) ; do \
		[ ! -d $$i ] || $(MAKE) -C $$i install || exit $$? ; \
	done
else
	@echo "No external modules specified"
endif
endif
#
# add directory dependancies here
#
.PHONY: ulibs romfs $(lib_y) $(libbase_y) $(app_y) $(ex_mod_y)

# We create the target directory of the symlink if it does
# not exist so the test in check-symlink works and we have a
# directory for generated filesas used by some architectures.
define create-symlink
	if [ ! -L cle-shell ]; then                                      \
			echo '  SYMLINK cle-shell -> ../../../ICTest/users/cle-shell/'; \
			ln -fsn ../../../ICTest/users/cle-shell/ cle-shell;                         \
	fi
endef

prepare:
ifeq ($(CONFIG_LIB_EXPAT),y)
	@echo "build expat";
	@if [ ! -e ./expat-2.1.0/Makefile ]; then \
		cd ./expat-2.1.0; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib"; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_FUSE), y)
	if [ ! -e ./fuse-2.9.0/Makefile ]; then \
		cd ./fuse-2.9.0; \
		./configure --host=mips-linux; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_DBUS),y)
	@echo "libdbus"
	@if [ ! -d ./dbus-1.8.0 ]; then \
		tar -xvzf dbus-1.8.0.tar.gz;\
	fi;
	@if [ ! -e ./dbus-1.8.0/Makefile ]; then \
		cd ./dbus-1.8.0; \
		CC="$(CROSS_COMPILE)gcc" CFLAGS="-fPIC -I$(DIR_ROOT)/users/expat-2.1.0/lib -I$(DIR_ROOT)/users/glib-2.36.0/lib -L$(DIR_ROOT)/users/expat-2.1.0/.libs" PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure \
		--disable-maintainer-mode --enable-inotify \
		--enable-abstract-sockets --disable-systemd --disable-tests --host=mips-linux --prefix="/" \
		--without-x --enable-xml-docs=no --enable-doxygen-docs=no --enable-selinux=no --enable-libaudit=no --enable-kqueue=no --enable-launchd=no --enable-systemd=no --enable-x11-autolaunch=no --enable-tests=no --enable-embedded-tests=no --enable-modular-tests=no \
		--with-dbus-user=root || exit 1 ;\
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include -I$(DIR_ROOT)/users/glib-2.36.0/glib" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib"; \
		make; \
		make install; \
		cp dbus-1.pc "$(DIR_ROOT)/users/lib/lib/pkgconfig"; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_APP_AVAHI), y)
ifeq ($(CONFIG_APP_WEAVE), y)
	@cd ./avahi-0.6.31; \
	if [ ! -e Makefile ]; then \
		AR=$(CROSS_COMPILE)ar \
		AS="$(CROSS_COMPILE)gcc -c -Os -pipe -fno-caller-saves -fpic -DGETTEXT_PACKAGE" \
		LD=$(CROSS_COMPILE)ld \
		NM=$(CROSS_COMPILE)nm \
		CC="$(CROSS_COMPILE)gcc" \
		GCC="$(CROSS_COMPILE)gcc" \
		RANLIB=$(CROSS_COMPILE)ranlib \
		STRIP=$(CROSS_COMPILE)strip \
		OBJCOPY=$(CROSS_COMPILE)objcopy \
		OBJDUMP=$(CROSS_COMPILE)objdump \
		SIZE=$(CROSS_COMPILE)size \
		CFLAGS="-Os -pipe -fno-caller-saves -fpic -DGETTEXT_PACKAGE -DNDEBUG -DDISABLE_SYSTEMD -I$(DIR_ROOT)/users/libdaemon-0.14/ -I$(DIR_ROOT)/users/expat-2.1.0/lib -L$(DIR_ROOT)/users/expat-2.1.0/.libs -I$(DIR_ROOT)/users/dbus-1.8.0/dbus -L$(DIR_ROOT)/users/dbus-1.8.0/dbus/.libs" \
		LIBDAEMON_CFLAGS="-I$(DIR_ROOT)/users/libdaemon-0.14/libdaemon/" \
		LIBDAEMON_LIBS="-L$(DIR_ROOT)/users/libdaemon-0.14/libdaemon/.libs/ -ldaemon" \
		ac_cv_header_sys_capability_h=no \
		./configure --target=mips-linux --host=mips-linux --build=i686-linux-gnu \
		--program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr \
		--bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/var \
		--datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info \
		--disable-nls --enable-shared --enable-static --disable-glib --disable-gobject \
		--disable-qt3 --disable-qt4 --disable-gtk --disable-gtk3 --with-xml=none --disable-dbm --disable-gdbm \
		--enable-libdaemon --disable-python --disable-pygtk --disable-python-dbus --disable-mono --disable-monodoc \
		--disable-doxygen-doc --disable-doxygen-dot --disable-doxygen-man --disable-doxygen-rtf --disable-doxygen-xml \
		--disable-doxygen-chm --disable-doxygen-chi --disable-doxygen-html --disable-doxygen-ps --disable-doxygen-pdf \
		--disable-xmltoman --with-distro=none --with-avahi-user=root --with-avahi-group=root \
		--with-autoipd-user=nobody --with-autoipd-group=nogroup --disable-stack-protector --enable-dbus \
		--with-xml=expat; \
	fi; \
	make; \
	cd ..
else
	@cd ./avahi-0.6.31; \
	if [ ! -e Makefile ]; then \
		AR=$(CROSS_COMPILE)ar \
		AS="$(CROSS_COMPILE)gcc -c -Os -pipe -fno-caller-saves -fpic -DGETTEXT_PACKAGE" \
		LD=$(CROSS_COMPILE)ld \
		NM=$(CROSS_COMPILE)nm \
		CC="$(CROSS_COMPILE)gcc" \
		GCC="$(CROSS_COMPILE)gcc" \
		RANLIB=$(CROSS_COMPILE)ranlib \
		STRIP=$(CROSS_COMPILE)strip \
		OBJCOPY=$(CROSS_COMPILE)objcopy \
		OBJDUMP=$(CROSS_COMPILE)objdump \
		SIZE=$(CROSS_COMPILE)size \
		CFLAGS="-Os -pipe -fno-caller-saves -fpic -DGETTEXT_PACKAGE -DNDEBUG -DDISABLE_SYSTEMD -I$(DIR_ROOT)/users/libdaemon-0.14/" \
		LIBDAEMON_CFLAGS="-I$(DIR_ROOT)/users/libdaemon-0.14/libdaemon/" \
		LIBDAEMON_LIBS="-L$(DIR_ROOT)/users/libdaemon-0.14/libdaemon/.libs/ -ldaemon" \
		ac_cv_header_sys_capability_h=no \
		./configure --target=mips-linux --host=mips-linux --build=i686-linux-gnu \
		--program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr \
		--bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc \
		--datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info \
		--disable-nls --enable-shared --enable-static --disable-glib --disable-gobject \
		--disable-qt3 --disable-qt4 --disable-gtk --disable-gtk3 --with-xml=none --disable-dbm --disable-gdbm \
		--enable-libdaemon --disable-python --disable-pygtk --disable-python-dbus --disable-mono --disable-monodoc \
		--disable-doxygen-doc --disable-doxygen-dot --disable-doxygen-man --disable-doxygen-rtf --disable-doxygen-xml \
		--disable-doxygen-chm --disable-doxygen-chi --disable-doxygen-html --disable-doxygen-ps --disable-doxygen-pdf \
		--disable-xmltoman --with-distro=none --with-avahi-user=nobody --with-avahi-group=nogroup \
		--with-autoipd-user=nobody --with-autoipd-group=nogroup --disable-stack-protector --disable-dbus; \
	fi; \
	make; \
	cd ..
endif
endif

ifeq ($(CONFIG_LIB_LIBDAEMON), y)
	@cd ./libdaemon-0.14; \
	if [ ! -e Makefile ]; then \
		AR=$(CROSS_COMPILE)ar \
		AS="$(CROSS_COMPILE)gcc -c -Os -pipe -fno-caller-saves -fpic" \
		LD=$(CROSS_COMPILE)ld \
		NM=$(CROSS_COMPILE)nm \
		CC="$(CROSS_COMPILE)gcc" \
		GCC="$(CROSS_COMPILE)gcc" \
		RANLIB=$(CROSS_COMPILE)ranlib \
		STRIP=$(CROSS_COMPILE)strip \
		OBJCOPY=$(CROSS_COMPILE)objcopy \
		OBJDUMP=$(CROSS_COMPILE)objdump \
		SIZE=$(CROSS_COMPILE)size \
		CFLAGS="-Os -pipe -fno-caller-saves -fpic " \
		./configure --target=mips-linux --host=mips-linux --build=i686-linux-gnu \
		--program-prefix="" --program-suffix="" --prefix=/usr --exec-prefix=/usr \
		--bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/lib --sysconfdir=/etc \
		--datadir=/usr/share --localstatedir=/var --mandir=/usr/man --infodir=/usr/info \
		--disable-nls --enable-shared --disable-static --disable-lynx \
		ac_cv_func_setpgrp_void=yes; \
	fi; \
	make; \
	cd ..
endif

ifeq ($(CONFIG_LIB_READLINE),y)
	@if [ ! -d ./readline-6.3 ]; then \
		tar -xvzf readline-6.3.tar.gz;\
	fi
	@cd ./readline-6.3; \
	if [ ! -e Makefile ]; then \
		bash_cv_wcwidth_broken=yes ./configure --prefix="$(DIR_ROOT)/users/lib" --target=mips-linux --host=mips-linux --build=i686-linux-gnu --disable-shared --enable-multibyte=no; \
		make; \
		make install; \
	fi; \
	cd ..; 
endif

ifeq ($(CONFIG_LIB_TERMCAP),y)
	@if [ ! -d ./termcap-1.3.1 ]; then \
		tar -xvzf termcap-1.3.1.tar.gz;\
	fi
	@cd ./termcap-1.3.1; \
	if [ ! -e Makefile ]; then \
		./configure --prefix="$(DIR_ROOT)/users/lib" --target=mips-linux --host=mips-linux --build=i686-linux-gnu; \
		make; \
		make install; \
	fi; \
	cd ..;
endif

ifeq ($(CONFIG_APP_RTK_API_TEST_CASE),y)
	@if [ ! -e ./CUnit-2.1-2/Makefile ]; then \
		tar -xvjf CUnit-2.1-2-src.tar.bz2; \
		cd ./CUnit-2.1-2; \
		pwd; \
		./configure --host=mips-linux; \
		make;\
		cd ..; \
	fi
endif
ifeq ($(CONFIG_APP_CLE_SHELL),y)
	@$(create-symlink)
endif
ifeq ($(CONFIG_APP_UTILS),y)
	@if [ ! -L utils ]; then \
			echo '  SYMLINK utils -> ../../../ICTest/users/utils/'; \
			ln -fsn ../../../ICTest/users/utils/ utils; \
	fi
endif
ifeq ($(CONFIG_APP_IPERF),y)
	@if [ ! -e ./iperf-2.0.4/Makefile ]; then \
		cd ./iperf-2.0.4; \
		./configure --host=mips-linux --disable-threads --disable-web100 ac_cv_func_malloc_0_nonnull=yes; \
	fi
endif

ifeq ($(CONFIG_LIB_XZ),y)
	@if [ ! -d ./xz-5.0.8 ]; then \
		tar -xvzf xz-5.0.8.tar.gz;\
	fi;
	#mkdir -p lib;
	#touch ./xz-5.0.8/aclocal.m4;
	#touch ./xz-5.0.8/Makefile.in;
	@if [ ! -e ./xz-5.0.8/Makefile ]; then \
		cd ./xz-5.0.8; \
		./configure CC=gcc --prefix="$(DIR_ROOT)/users/lib" --enable-shared=no; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_APP_DBUS), y)
	@if [ ! -d ./dbus-1.8.0 ]; then \
		tar -xvzf dbus-1.8.0.tar.gz;\
	fi;
	@if [ ! -e ./dbus-1.8.0/Makefile ]; then \
		cd ./dbus-1.8.0; \
		CFLAGS="-fPIC -I$(DIR_ROOT)/users/expat-2.1.0/lib -L$(DIR_ROOT)/users/expat-2.1.0/.libs" PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure --enable-abstract-sockets --disable-systemd --host=mips-linux --prefix="/" --with-dbus-user=root || exit 1 ;\
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib"; \
		make; \
		make install; \
		cp dbus-1.pc "$(DIR_ROOT)/users/lib/lib/pkgconfig"; \
		cd -; \
	fi
endif

ifeq ($(KERNEL_VER),linux-3.10)
ifeq ($(CONFIG_APP_LIBUSB),y)
	@mkdir -p usb-k310;
	@mkdir -p lib;
	@if [  ! -d ./usb-k310/libusb-1.0.20 ]; then \
		tar jxvf libusb-1.0.20.tar.bz2 -C usb-k310; \
	fi
	@if [ ! -e ./usb-k310/libusb-1.0.20/Makefile ]; then \
		cd ./usb-k310/libusb-1.0.20; \
		LD="$(CROSS_COMPILE)ld" ./configure --host=mips-linux --enable-udev=no --enable-debug-log=no; \
		make; \
		make install DESTDIR=$(DIR_ROOT)/users/usb-k310; \
		cd -; \
	fi
	@if [  ! -d ./usb-k310/libusb-compat-0.1.5 ]; then \
		tar jxvf libusb-compat-0.1.5.tar.bz2 -C usb-k310; \
	fi
	@if [ ! -e ./usb-k310/libusb-compat-0.1.5/Makefile ]; then \
		cd ./usb-k310/libusb-compat-0.1.5; \
		LD="$(CROSS_COMPILE)ld" PKG_CONFIG_PATH="$(DIR_ROOT)/users/usb-k310/usr/local/lib/pkgconfig" ./configure --host=mips-linux CPPFLAGS="-I$(DIR_ROOT)/users/usb-k310/usr/local/include/libusb-1.0" LDFLAGS="-L$(DIR_ROOT)/users/usb-k310/usr/local/lib"; \
		make; \
		make install DESTDIR=$(DIR_ROOT)/users/usb-k310; \
		cd -; \
	fi
endif
ifeq ($(CONFIG_APP_USBUTILS),y)
	@mkdir -p usb-k310;
	@if [  ! -d ./usb-k310/usbutils-007 ]; then \
		tar zxvf usbutils-007.tar.gz -C usb-k310; \
		tar zxvf usbutils-007.tar.gz -C usb-k310; \
		tar zxvf usbhid-dump-1.4.tar.gz -C usb-k310; \
		cp -rf usb-k310/usbhid-dump-1.4/* usb-k310/usbutils-007/usbhid-dump; \
	fi
	@if [ ! -e ./usb-k310/usbutils-007/Makefile ]; then \
		cd ./usb-k310/usbutils-007/usbhid-dump; \
		./bootstrap; \
		LD="$(CROSS_COMPILE)ld" PKG_CONFIG_PATH="$(DIR_ROOT)/users/usb-k310/usr/local/lib/pkgconfig" ./configure --host=mips-linux CPPFLAGS="-I$(DIR_ROOT)/users/usb-k310/usr/local/include -I$(DIR_ROOT)/users/usb-k310/usr/local/include/libusb-1.0"  LIBS=" -lusb-1.0" LDFLAGS="-L$(DIR_ROOT)/users/usb-k310/usr/local/lib"; \
		cd -; \
		cd ./usb-k310/usbutils-007; \
		autoreconf --install --symlink; \
		LD="$(CROSS_COMPILE)ld" PKG_CONFIG_PATH="$(DIR_ROOT)/users/usb-k310/usr/local/lib/pkgconfig" ./configure --host=mips-linux CPPFLAGS="-I$(DIR_ROOT)/users/usb-k310/usr/local/include -I$(DIR_ROOT)/users/usb-k310/usr/local/include/libusb-1.0"  LIBS=" -lpthread -lusb-1.0" LDFLAGS="-L$(DIR_ROOT)/users/usb-k310/usr/local/lib" --disable-usbids; \
		echo "romfs:" >> Makefile; \
		echo "	@$(STRIP) lsusb; \\" >> Makefile; \
		echo "	$(ROMFSINST) /bin/lsusb; \\" >> Makefile; \
		echo "	rsync -av $(DIR_ROOT)/toolchain/msdk-4.4.7-mips-EB-3.10-0.9.33-m32t-131227b/lib/librt*.so* $(DIR_ROOT)/romfs/lib/ ; \\" >> Makefile; \
		echo "	rsync -av $(DIR_ROOT)/users/usb-k310/usr/local/lib/libusb*.so* $(DIR_ROOT)/romfs/lib/ ; \\" >> Makefile; \
		echo "	mkdir -p $(DIR_ROOT)/romfs/usr/share ; \\" >> Makefile; \
		echo "	echo '' >$(DIR_ROOT)/romfs/usr/share/usb.ids ; \\" >> Makefile; \
		echo "	cd $(DIR_ROOT)/romfs/lib ; \\" >> Makefile; \
		echo "	ln -sf libusb.so libusb.so.0" >> Makefile; \
		cd -; \
	fi
endif
endif

ifeq ($(CONFIG_LIB_LIBUSB),y)
	@echo "need usblib > 1.0"
	mkdir -p lib;
	@echo "build libusb";
	@if [ ! -e ./libusb-1.0.9/Makefile ]; then \
		cd ./libusb-1.0.9; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib"; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_LIBICONV),y)
	@echo "build libiconv"
	@if [ ! -e ./libiconv-1.14/Makefile ]; then \
		cd ./libiconv-1.14; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib" --disable-nls --enable-static=yes --enable-shared=no --enable-fast-install=no; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_USBUTILS),y)
	@echo "tar usbutils and usb-hexdump"
	@if [  ! -d ./usbutils-007 ]; then \
		tar -xvzf usbutils-007.tar.gz; \
		tar -xvzf usbhid-dump-1.4.tar.gz -C ./usbutils-007/usbhid-dump; \
		cp -rf ./usbutils-007/usbhid-dump/usbhid-dump-1.4/* ./usbutils-007/usbhid-dump; \
		cd ./usbutils-007/usbhid-dump; \
		./bootstrap; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib"; \
		cd -; \
		cd ./usbutils-007; \
		autoreconf --install --symlink; \
		PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" CFLAGS="-fPIC" ./configure --host=mips-linux CPPFLAGS="-I$(DIR_ROOT)/users/lib/include" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib" LIBS=" -lpthread -liconv"; \
		echo "romfs:" >> Makefile; \
		echo "	$(STRIP) lsusb" >> Makefile; \
		echo "	$(ROMFSINST) /bin/lsusb" >> Makefile; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_LIBFFI),y)
	@echo "libffi"
	@if [ ! -e ./libffi-3.0.13/Makefile ]; then \
		cd ./libffi-3.0.13; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib"; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_GETTEXT),y)
	@echo "gettext";
	@if [ ! -d ./gettext-0.18.3.1 ]; then \
		tar -xvzf gettext-0.18.3.1.tar.gz;\
	fi
	@if [ ! -e ./gettext-0.18.3.1/Makefile ]; then \
		echo "configure gettext"; \
		cd ./gettext-0.18.3.1; \
		CFLAGS="-fPIC" PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib" CPPFLAGS="-I$(DIR_ROOT)/users/lib/include" LDFLAGS="-liconv -lrt  -L$(DIR_ROOT)/users/lib/lib" ac_cv_func_posix_spawn=yes; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_ZLIB_128),y)
	@echo "libz"
	@if ! grep "libz.a" -q ./zlib-1.2.8/Makefile; then \
		cd ./zlib-1.2.8; \
		CFLAGS="-fPIC" CC=$(CROSS_COMPILE)gcc ./configure --prefix="$(DIR_ROOT)/users/lib"; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_GLIB),y)
	@echo "glib"
	@if [ ! -e ./glib-2.36.0/Makefile ]; then \
		cd ./glib-2.36.0; \
		echo ac_cv_type_long_long=yes>mips-linux.cache; \
		echo glib_cv_stack_grows=no>>mips-linux.cache; \
		echo glib_cv_uscore=no>>mips-linux.cache; \
		echo c_cv_func_posix_getpwuid_r=yes>>mips-linux.cache; \
		echo ac_cv_func_posix_getpwuid_r=yes>>mips-linux.cache; \
		echo ac_cv_func_posix_getgrgid_r=yes>>mips-linux.cache; \
		PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" CFLAGS="-fPIC" ./configure --enable-iconv=no --with-libiconv=gnu --disable-modular-tests --host=mips-linux --prefix="$(DIR_ROOT)/users/lib" \
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib" \
		--cache-file=mips-linux.cache; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_ALSA),y)
	@echo "libalsa"
	@if [ ! -e ./alsa-lib-1.0.25/Makefile ]; then \
		cd ./alsa-lib-1.0.25; \
		CFLAGS="-fPIC" ./configure --prefix="$(DIR_ROOT)/users/lib" --build=i686-pc-linux-gnu --host=mips-linux --disable-python; \
		touch ./configure; \
		touch ./config.status; \
		touch ./Makefile.in; \
		touch ./Makefile; \
		touch ./include/config.h.in; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_LIBSNDFILE),y)
	@echo "libsndfile"
	@if [ ! -e ./libsndfile-1.0.25/Makefile ]; then \
		cd ./libsndfile-1.0.25; \
		FLAGS="-fPIC" ./configure --prefix="$(DIR_ROOT)/users/lib" --build=i686-pc-linux-gnu --host=mips-linux; \
		echo "romfs:" >> Makefile; \
		cd -; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_APP_BLUEZ),y)
	@echo "build bluez";
	@echo "bluez dir: $(BLUEZ_DIR)";
ifeq ($(CONFIG_APP_BLUEZ_524),y)
	@if [ ! -e ./$(BLUEZ_DIR)/Makefile ]; then \
		cd ./$(BLUEZ_DIR); \
		echo "bluez first"; \
		CFLAGS="-fPIC -I$(DIR_ROOT)/users/dbus-1.8.0" ac_cv_lib_glib_2_0_g_slist_free_full=yes PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure --enable-static=yes --host=mips-linux --disable-udev --disable-obex --disable-systemd --enable-experimental --enable-tools --enable-library \
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include -I$(DIR_ROOT)/users/lib/include/glib-2.0 -I$(DIR_ROOT)/users/lib/include/libusb-1.0 -I$(DIR_ROOT)/users/lib/include/sys -I$(DIR_ROOT)/users/lib/include/alsa -I$(DIR_ROOT)/users/lib/include/dbus-1.0" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib -lglib-2.0 -ldbus-1 -L$(DIR_ROOT)/users/dbus-1.8.0/dbus/.libs"; \
		cd -; \
	fi
else
ifeq ($(CONFIG_APP_BLUEZ_522),y)
	@if [ ! -e ./$(BLUEZ_DIR)/Makefile ]; then \
		cd ./$(BLUEZ_DIR); \
		echo "bluez first"; \
		CFLAGS="-fPIC" ac_cv_lib_glib_2_0_g_slist_free_full=yes PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure --enable-static=yes --host=mips-linux --disable-udev --disable-obex --disable-systemd --disable-client\
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include -I$(DIR_ROOT)/users/lib/include/glib-2.0 -I$(DIR_ROOT)/users/lib/include/libusb-1.0 -I$(DIR_ROOT)/users/lib/include/sys -I$(DIR_ROOT)/users/lib/include/alsa -I$(DIR_ROOT)/users/lib/include/dbus-1.0" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib -lglib-2.0"; \
		cd -; \
	fi
else
	@if [ ! -e ./$(BLUEZ_DIR)/Makefile ]; then \
		cd ./$(BLUEZ_DIR); \
		CFLAGS="-fPIC" ac_cv_lib_glib_2_0_g_slist_free_full=yes PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" ./configure --enable-static=yes --host=mips-linux \
		--disable-audio --disable-usb \
		CPPFLAGS="-I$(DIR_ROOT)/users/lib/include -I$(DIR_ROOT)/users/lib/include/glib-2.0 -I$(DIR_ROOT)/users/lib/include/libusb-1.0 -I$(DIR_ROOT)/users/lib/include/sys -I$(DIR_ROOT)/users/lib/include/alsa -I$(DIR_ROOT)/users/lib/include/dbus-1.0" LDFLAGS="-L$(DIR_ROOT)/users/lib/lib"; \
		cd -; \
	fi
endif
endif
endif

ifeq ($(CONFIG_LIB_JSON),y)
	@echo "Generate lib";
	mkdir -p lib;
	#json-c
	@echo "build json-c"
	@if [ ! -d ./lib/include/json-c ]; then \
		rm -rf json-c-0.12;\
		tar -xvzf json-c-0.12.tar.gz; \
		cd ./json-c-0.12; \
		CFLAGS="-fPIC -DPIC" ac_cv_func_malloc_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes ac_cv_have_decl_isnan=yes ac_cv_have_decl_isinf=yes ./configure --prefix="$(DIR_ROOT)/users/lib" --host=mips-linux; \
		sed 's/^AM_CFLAGS = -Wall -Werror -Wno-error=/AM_CFLAGS = -Wall -Wno-error=/g' -i Makefile; \
		make V=s; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_LIBUBOX),y)
	@echo "build libubox"
	@if [ ! -d ./lib/include/libubox ]; then \
		rm -rf libubox-2014-08-04;\
		tar -xvzf  libubox-2014-08-04-dffbc09baf71b294185a36048166d00066d433b5.tar.gz; \
		cd ./libubox-2014-08-04; \
		sed 's/OPTION(BUILD_LUA \"build Lua plugin\" ON)/OPTION(BUILD_LUA \"build Lua plugin\" OFF)/g' -i CMakeLists.txt; \
		sed 's/PKG_CHECK_MODULES(JSONC json-c json)/PKG_CHECK_MODULES(JSONC json-c)/g' -i CMakeLists.txt; \
		LDFLAGS="-lm" PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" cmake -DCMAKE_INSTALL_PREFIX="$(DIR_ROOT)/users/lib/" -DCMAKE_FIND_ROOT_PATH="$(DIR_ROOT)/users/lib/" .; \
		make; \
		make install; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_UBUS),y)
	@echo "build ubus"
	@if [ ! -e ./ubus-2014-09-17/Makefile ]; then \
		tar -xvzf ubus-2014-09-17-4c4f35cf2230d70b9ddd87638ca911e8a563f2f3.tar.gz; \
		cd ./ubus-2014-09-17; \
		sed 's/OPTION(BUILD_LUA \"build Lua plugin\" ON)/OPTION(BUILD_LUA \"build Lua plugin\" OFF)/g' -i CMakeLists.txt; \
		sed 's/OPTION(ENABLE_SYSTEMD \"systemd support\" ON)/OPTION(ENABLE_SYSTEMD \"systemd support\" OFF)/g' -i CMakeLists.txt; \
		sed '16a'"INCLUDE_DIRECTORIES($(DIR_ROOT)/users/lib/include)" -i CMakeLists.txt; \
		sed '16a'"INCLUDE_DIRECTORIES($(DIR_ROOT)/users/lib/include/json-c)" -i CMakeLists.txt; \
		sed '16a'"LINK_DIRECTORIES($(DIR_ROOT)/users/lib/lib)" -i CMakeLists.txt; \
		sed '16a'"ADD_DEFINITIONS(-DJSONC)" -i CMakeLists.txt; \
		PKG_CONFIG_PATH="$(DIR_ROOT)/users/lib/lib/pkgconfig" cmake -DCMAKE_INSTALL_PREFIX="$(DIR_ROOT)/users/lib/" -DCMAKE_FIND_ROOT_PATH="$(DIR_ROOT)/users/lib" .; \
		sed '526a'"romfs:" -i Makefile; \
		sed '527a\\t$(ROMFSINST) ubusd /bin' -i Makefile; \
		sed '528a\\t$(ROMFSINST) ubus /bin' -i Makefile; \
		sed '529a\\t$(ROMFSINST) examples/client /bin' -i Makefile; \
		sed '530a\\t$(ROMFSINST) examples/server /bin' -i Makefile; \
		sed '531a\\t$(ROMFSINST) \"*.so\" /lib' -i Makefile; \
		sed '532a\\t$(ROMFSINST) ../libubox-2014-08-04/\"*.so\" /lib' -i Makefile; \
		cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_OPENSSL_098I),y)
	@cd ./openssl-0.9.8i; \
	if [ $(openssl_dir) = "openssl-0.9.8i" ]; then \
		if [ ! -f dlib_config_done ]; then \
			./Configure shared $(CROSS_TARGET); make depend; \
			rm -f slib_config_done; touch dlib_config_done; \
			make clean; \
			cp Makefile.DLibForWapi Makefile; \
			cp test/Makefile.wapi test/Makefile; \
		fi;\
	else\
		if [ ! -f slib_config_done ]; then \
			./Configure no-threads no-hw no-shared $(CROSS_TARGET); make depend; \
			rm -f dlib_config_done; touch slib_config_done;\
			make clean; \
			cp Makefile.SLibForWapi Makefile; \
			cp test/Makefile.wapi test/Makefile; \
		fi;\
	fi;\
	cd -;
endif

ifeq ($(CONFIG_LIB_OPENSSL_098Y),y)
	@cd ./openssl-0.9.8y; \
	if [ $(openssl_dir) = "openssl-0.9.8y" ]; then \
		if [ ! -f dlib_config_done ]; then \
			./Configure shared $(CROSS_TARGET); make depend; \
			rm -f slib_config_done; touch dlib_config_done; \
			make clean; \
			cp Makefile.DLibForWapi Makefile; \
			cp test/Makefile.wapi test/Makefile; \
		fi;\
	else\
		if [ ! -f slib_config_done ]; then \
			./Configure no-threads no-hw no-shared $(CROSS_TARGET); make depend; \
			rm -f dlib_config_done; touch slib_config_done;\
			make clean; \
			cp Makefile.SLibForWapi Makefile; \
			cp test/Makefile.wapi test/Makefile; \
		fi;\
	fi;\
	cd -;
endif

ifeq ($(CONFIG_LIB_OPENSSL_102D),y)
	@cd ./openssl-1.0.2d; \
	if [ ! -f Makefile ]; then \
		CROSS_COMPILE= ./Configure no-threads no-hw shared $(CROSS_TARGET) || exit 1;\
		make depend; \
		sed -i "/DO NOT DELETE THIS LINE/i\romfs:\n\tcp ./libssl.so $(DIR_ROMFS)/lib\n\tcp ./libssl.so.1.0.0 $(DIR_ROMFS)/lib\n\tcp ./libcrypto.so $(DIR_ROMFS)/lib\n\tcp ./libcrypto.so.1.0.0 $(DIR_ROMFS)/lib" Makefile; \
		sed -i "/DO NOT DELETE THIS LINE/i\distclean:\n\t-rm -f Makefile" Makefile; \
	fi; \
	make ;\
	cd ..;
endif

ifeq ($(CONFIG_LIB_LZO),y)
	@cd ./lzo-2.08; \
			if [ ! -f lzo_config_done ]; then \
			rm Makefile; \
			./configure --host=mips-linux --prefix=$(DIR_ROOT)/users/lib; \
			make clean; \
			make; \
			touch lzo_config_done; \
			exit;\
			fi; \
	cd -
endif
ifeq ($(CONFIG_APP_OPENVPN),y)
	@if [ ! -e ./openvpn-2.3.4/openvpn_config_done ]; then \
		cd ./openvpn-2.3.4; \
		rm Makefile; \
		autoreconf -ivf; \
		if [ ! -e ./.gitignore ]; then \
			touch ./.gitignore; \
		fi; \
		./configure --host=mips-linux --disable-plugins \
		IFCONFIG=/bin/ifconfig ROUTE=/bin/route \
		LZO_CFLAGS="-I$(DIR_ROOT)/users/lzo-2.08/include/" \
		LZO_LIBS="$(DIR_ROOT)/users/lzo-2.08/src/.libs/liblzo2.a" \
		OPENSSL_SSL_CFLAGS="-I$(DIR_ROOT)/users/$(openssl_dir)/include/" \
  		OPENSSL_SSL_LIBS="-L$(DIR_ROOT)/users/$(openssl_dir)/ -lssl" \
		OPENSSL_CRYPTO_CFLAGS="-I$(DIR_ROOT)/users/$(openssl_dir)/include/" \
		OPENSSL_CRYPTO_LIBS="-L$(DIR_ROOT)/users/$(openssl_dir)/ -lcrypto"; \
		make clean; \
		echo "" >> Makefile; \
  		echo "romfs:" >> Makefile; \
		if [ $(CONFIG_RTK_OPENVPN_HW_CRYPTO) = "y" ]; then \
		echo "#define CONFIG_RTK_OPENVPN_HW_CRYPTO" > ./src/openvpn/rtl_hw_crypto.h; \
		else \
		echo " " > ./src/openvpn/rtl_hw_crypto.h; \
		fi; \
		echo "	$(ROMFSINST) ./src/openvpn/openvpn /bin/openvpn" >> Makefile; \
		echo "	mkdir -p $(DIR_ROMFS)/usr/share/openvpn" >> Makefile; \
		echo "	cp ./openvpn_config/* $(DIR_ROMFS)/usr/share/openvpn" >> Makefile; \
	fi
endif

ifeq ($(CONFIG_APP_DOSFSTOOLS),y)
	@echo 'make dosfstools-4.0';\
	if [ ! -d ./dosfstools-4.0 ]; then \
		tar -xvzf dosfstools-4.0.tar.gz;\
	fi;\
	if [ ! -e ./dosfstools-4.0/Makefile ]; then \
		cd ./dosfstools-4.0; \
		./configure --host=mips-linux --without-udev;\
		make clean;\
		echo "" >> Makefile;\
		echo "romfs:" >> Makefile;\
		echo "	$(ROMFSINST) ./src/mkfs.fat /bin/mkfs.fat" >> Makefile;\
		echo "	$(ROMFSINST) ./src/fsck.fat /bin/fsck.fat" >> Makefile;\
		cd -;\
	fi
endif

ifeq ($(CONFIG_APP_PHP),y)
	@echo 'make php-5.6.19'; \
	cd ./php-5.6.19; \
	if [ ! -f Makefile ]; then \
		./configure --host=mips-linux  --with-config-file-path=/etc --disable-all ac_cv_c_bigendian_php=yes ; \
		make clean; \
		echo "" >> Makefile; \
		echo "romfs:" >> Makefile; \
		echo "	$(ROMFSINST) ./sapi/cgi/php-cgi /bin/php-cgi" >> Makefile; \
		echo "	$(ROMFSINST) ./sapi/cli/php /bin/php" >> Makefile; \
		echo "	cp ./php.ini-development $(DIR_ROMFS)/etc/php.ini" >> Makefile; \
	fi;\
	cd -
endif

ifeq ($(CONFIG_APP_RTK_VOIP),y)
#ifeq ($(CONFIG_APP_RTK_VOIP_E8C),y)
#	ln -fs $(DIR_LINUX)/.config rtk_voip-e8c/.linux_config
#	make -C rtk_voip-e8c prepare
#else
ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxE
	ln -fs $(DIR_LINUX)/.config rtk_voip-89xxE/.linux_config
	make -C rtk_voip-89xxE prepare
else
	ln -fs $(DIR_LINUX)/.config rtk_voip/.linux_config
	make -C rtk_voip prepare
endif
#endif
endif

ifeq ($(CONFIG_APP_IPTABLES),y)
	@cd ./iptables-1.4.4; \
	if [ $(CONFIG_APP_IP6TABLES_SUPPORT) = 1 ]; then \
		if [ ! -f ip6tables_config_done ]; then \
			./configure --host=mips-linux --with-ksource=$(DIR_LINUX_KERNEL) --enable-static --disable-shared CFLAGS=-Os; \
			make depend; \
			make clean; \
			rm -f iptables_config_done; touch ip6tables_config_done; \
		fi; \
	else\
		if [ ! -f iptables_config_done ]; then \
			./configure --host=mips-linux --with-ksource=$(DIR_LINUX_KERNEL) --enable-static --disable-shared --disable-ipv6 CFLAGS=-Os; \
			make depend; \
			make clean; \
			rm -f ip6tables_config_done; touch iptables_config_done; \
		fi; \
	fi; \
	cd -
endif

ifeq ($(CONFIG_LIB_LIBEVENT_2022),y)
	@echo "build libevent";
	cd ./libevent-2.0.22-stable;\
	if [ ! -e Makefile ]; then \
		CC="msdk-linux-gcc" \
		./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib" --disable-thread-support --enable-openssl; \
	fi;\
        make;\
        make install;\
	cd ./testfor98c;\
		make;\
        cd ../..;\
	pwd
endif

ifeq ($(CONFIG_APP_GDBSERVER),y)
	echo 'make gdbserver';
	cd ./gdb; \
	if [ ! -f gdb_config_done ]; then \
		cd gdb-6.8; ./configure --target=mips-linux --host=i686-pc-linux-gnu --prefix=$(shell pwd)/gdb/gdb-host;  \
		cp Makefile.RSDK Makefile; \
		cd gdb/gdbserver; \
		./configure --target=mips-linux --host=mips-linux --prefix=$(shell pwd)/gdb/gdbserver-6.8; \
		cd ../../../; \
		rm -f gdb_config_done; touch gdb_config_done; \
	fi; \
	cd ..
endif
ifeq ($(CONFIG_LIB_ZLIB_125),y)
	@if ! grep "libz.a" -q ./zlib-1.2.5/Makefile; then \
		cd ./zlib-1.2.5/; \
		CC=$(CROSS_COMPILE)gcc ./configure --prefix=$(DIR_USERS)/zlib-1.2.5/install; \
	fi
endif

ifeq ($(CONFIG_LIB_ZLIB_123),y)
	@echo 'make zlib-1.2.3';
	cd ./zlib-1.2.3; \
	if [ ! -f zlib-1.2.3_config_done ]; then \
		./configure; \
		cp Makefile.zlib Makefile; \
		rm -f zlib-1.2.3_config_done; touch zlib-1.2.3_config_done; \
	fi; \
	make; \
	cd ..
endif

ifeq ($(CONFIG_APP_MT_DAAPD),y)
	@echo 'make gdbm-1.8.3'; 
	cd ./gdbm-1.8.3; \
	if [ ! -f gdbm-1.8.3_config_done ]; then \
		./configure --host=mips-linux --build=i686-pc-linux-gnu; \
		cp Makefile.gdbm Makefile; \
		cp libtool.gdbm libtool; \
		rm -f gdbm-1.8.3_config_done; touch gdbm-1.8.3_config_done; \
	fi; \
	make; \
	cd ..
endif

ifeq ($(CONFIG_LIB_LIBID3TAG),y)
	@echo 'make libid3tag';
	cd ./libid3tag-0.15.1b; \
	if [ ! -f libid3tag_config_done ]; then \
		./configure --build=mips-linux --host=i686-pc-linux-gnu \
		--enable-shared \
		--disable-static \
		CPPFLAGS="-I../zlib-1.2.3/" \
		LDFLAGS="-L../zlib-1.2.3/"; \
		cp Makefile.id3tag Makefile; \
		cp libtool.id3tag libtool; \
		cd ./msvc++; \
		cp Makefile.id3tag Makefile; \
		cd ..;\
		rm -f libid3tag_config_done; touch libid3tag_config_done; \
	fi; \
	make; \
	cd ..
endif

ifeq ($(CONFIG_APP_MT_DAAPD),y)
	@echo 'make mt-daapd'; \
	cd ./mt-daapd-0.2.4.2; \
	if [ ! -f mt-daapd_config_done ]; then \
		./configure --host=mips-linux \
		--with-gdbm-includes=../gdbm-1.8.3/ \
		--with-gdbm-libs=../gdbm-1.8.3/ \
		--with-id3tag=../libid3tag-0.15.1b/ \
		--with-id3tag-libs=../libid3tag-0.15.1b/.libs/ \
		LIBS="-lgdbm -L$(DIR_ROOT)/users/gdbm-1.8.3/ -lid3tag -L$(DIR_ROOT)/users/libid3tag-0.15.1b/.libs/ -lz -L$(DIR_ROOT)/users/zlib-1.2.3/" \
		ac_cv_func_setpgrp_void=yes; \
		cp Makefile.daapd Makefile; \
		cd ./src; cp Makefile.daapd Makefile; cd ..; \
		cd ./admin-root; cp Makefile.daapd Makefile; cd ..; \
		cd ./contrib; cp Makefile.daapd Makefile; cd ..; \
		cd ./debian; cp Makefile.daapd Makefile; cd ..; \
		rm -f mt-daapd_config_done; touch mt-daapd_config_done; \
	fi; \
	make; \
	cd ..
endif

ifeq ($(CONFIG_APP_RADVD),y)
	@if [ ! -e ./radvd-0.9.1/Makefile ]; then \
		cd ./radvd-0.9.1; \
		./configure --host=mips-linux; \
		touch Makefile.in; \
	fi
endif

ifeq ($(CONFIG_APP_NDISC6),y)
	@if [ ! -e ./ndisc6-1.0.3/Makefile ]; then \
	cd ./ndisc6-1.0.3; \
	./configure --host=mips-linux; \
	echo "romfs:" >> Makefile; \
	echo "	$(ROMFSINST)  src/ndisc6 /bin/ndisc6" >> Makefile;\
	echo "	$(ROMFSINST)  src/rdisc6 /bin/rdisc6" >> Makefile;\
	echo "	$(ROMFSINST)  src/tcptraceroute6 /bin/tcptraceroute6" >> Makefile;\
	echo "	$(ROMFSINST)  src/tracert6 /bin/tracert6" >> Makefile;\
	echo "	$(ROMFSINST)  rdnssd/rdnssd /bin/rdnssd" >> Makefile;\
	echo "	$(ROMFSINST)  rdnssd/merge-hook /etc/merge-hook" >> Makefile;\
	cd -; \
	fi
endif

ifeq ($(CONFIG_LIB_LRZSZ),y)
	@cd ./upImage/lrzsz/lrzsz-0.12.20; \
	if [ ! -f liblrzsz_config_done ]; then \
		./configure --prefix=/bin/ --host=mips-linux --enable-pubdir=/var;\
		rm -f liblrzsz_config_done; touch liblrzsz_config_done; \
		echo "romfs:" >> Makefile; \
		echo "	$(ROMFSINST)  src/lrz /bin/lrz" >> Makefile;\
		echo "	$(ROMFSINST)  src/lsz /bin/lsz" >> Makefile;\
	fi; \
	cd -;
endif

ifeq ($(CONFIG_LIB_CURL),y)
	@if [ ! -e ./curl-7.34.0/Makefile ]; then \
		cd ./curl-7.34.0; \
		CC=$(CC) \
		CFLAGS="-I$(DIR_ROOT)/users/$(openssl_dir)/include" LDFLAGS="-L$(DIR_ROOT)/users/$(openssl_dir)" \
		./configure --host=mips-linux --disable-shared --enable-static --prefix="/usr" \
		--without-libidn --with-ssl --with-ca-bundle=/etc/ssl/ca-bundle.crt \
		--without-librtmp --without-gnutls --without-nss --without-libssh2 --without-zlib --without-winidn --disable-rtsp --disable-ldap --disable-ldaps --disable-ipv6; \
		touch Makefile.in; \
	fi
endif

ifeq ($(CONFIG_APP_TRANSMISSION),y)
ifeq ($(openssl_dir),)
	$(error openssl shold be selected for transmissionBT)
endif
ifneq ($(CONFIG_LIB_Z),y)
	$(error zlib shold be selected for transmissionBT)
endif
endif

ifeq ($(CONFIG_LIB_LIBEVENT_2021),y)
	@if [ ! -e ./libevent-2.0.21-stable/Makefile ]; then \
		cd ./libevent-2.0.21-stable; \
		./configure --host=mips-linux --disable-nls --enable-static=yes --enable-shared=no --enable-fast-install=no; \
		touch Makefile.in; \
	fi
endif

ifeq ($(CONFIG_APP_TRANSMISSION),y)
	@if [ ! -e ./curl-7.34.0/Makefile ]; then \
		cd ./curl-7.34.0; \
		./configure --host=mips-linux --disable-shared --enable-static --without-libidn \
		--without-ssl --without-librtmp --without-gnutls --without-nss \
		--without-libssh2 --without-zlib --without-winidn --disable-rtsp \
		--disable-ldap --disable-ldaps --disable-ipv6; \
		touch Makefile.in; \
	fi
endif

ifeq ($(CONFIG_APP_TRANSMISSION),y)
	@if [ ! -e ./transmission-2.82/Makefile ]; then \
		cd ./transmission-2.82; \
		./configure --host=mips-linux LIBCURL_CFLAGS="-I$(DIR_ROOT)/users/curl-7.34.0/include/" \
		LIBCURL_LIBS="-L$(DIR_ROOT)/users/curl-7.34.0/lib/.libs/ -lcurl" \
		LIBEVENT_CFLAGS="-I$(DIR_ROOT)/users/libevent-2.0.21-stable/include/" \
		LIBEVENT_LIBS="-L$(DIR_ROOT)/users/libevent-2.0.21-stable/.libs/ -levent" \
		OPENSSL_CFLAGS="-I$(DIR_ROOT)/users/$(openssl_dir)/include/" \
		OPENSSL_LIBS="-L$(DIR_ROOT)/users/$(openssl_dir)/ -lssl -lcrypto" \
		ZLIB_CFLAGS="-DHAVE_ZLIB -I$(DIR_ROOT)/users/zlib-1.2.5/ -L$(DIR_ROOT)/users/zlib-1.2.5/ -lz" \
		--disable-nls --enable-lightweight; \
		touch Makefile.in; \
	fi
endif
ifeq ($(CONFIG_APP_E2FSPROGS),y)
	@cd ./e2fsprogs-1.42.11; \
		if [ ! -f e2fsprogs_config_done ]; then \
			LD=$(CROSS_COMPILE)ld STRIP=$(CROSS_COMPILE)strip AR=$(CROSS_COMPILE)ar ./configure --host=mips-linux --disable-defrag LDFLAGS=-static CFLAGS=-Os; \
			make clean; \
			echo "" >> Makefile; \
			echo "romfs:" >> Makefile; \
			echo "	cp ./misc/mke2fs ../../romfs/bin/mke2fs" >> Makefile; \
			touch e2fsprogs_config_done; \
			exit;\
		fi; \
	cd -
endif

ifeq ($(CONFIG_APP_CURL),y)
	@cd ./curl-7.36.0; \
		if [ ! -f curl_config_done ]; then \
			LD=$(CROSS_COMPILE)ld ./configure --host=mips-linux --with-ksource=$(DIR_LINUX_KERNEL) --enable-shared=yes --disable-static CFLAGS=-Os; \
			make clean; \
			echo "" >> Makefile; \
			echo "romfs:" >> Makefile; \
			echo "	mkdir -p ../../romfs/usr/lib" >> Makefile; \
			echo "	cp ./src/.libs/curl ../../romfs/bin/curl" >> Makefile; \
			echo "	cp -av ./lib/.libs/libcurl.so* ../../romfs/usr/lib" >> Makefile; \
			touch curl_config_done; \
			exit;\
		fi; \
	cd -
endif

ifeq ($(CONFIG_APP_EXFAT_FUSE),y)
ifneq ($(CONFIG_LIB_FUSE), y)
	$(error fuse library should selected by exfat fuse)
endif
endif


prepare_lib:

ifeq ($(CONFIG_APP_LIBICONV),y)
	cd ./libiconv-1.14; \
	if [ ! -f libiconv_config_done ]; then \
		./configure --host=mips-linux --disable-nls --enable-static=yes --enable-shared=no --enable-fast-install=no; \
		cp Makefile.libiconv Makefile; \
		cp ./preload/Makefile.libiconv ./preload/Makefile; \
		rm -f libiconv_config_done; touch libiconv_config_done; \
	fi; \
	make; \
	cd ..
endif

prepare_app:
ifeq ($(CONFIG_APP_NFS_UTILITY),y)
	@echo "Check cmake ...";
	@if [ ! -e ./cmake-2.8.12.1/bin/cmake ]; then \
		cd ./cmake-2.8.12.1; \
		CFLAGS="-fPIC" CC=gcc CXX=g++ ./bootstrap ; \
		make ; \
		cd - ; \
	fi

	@echo "Check librpc.so ...";	
	@if [ ! -e ./librpc-0.9.32-rc2/librpc.so ]; then \
                cd ./librpc-0.9.32-rc2; \
		rm ./Makefile; \
		rm ./uClibc_mutex.h; \
		cp ../../$(CONFIG_RSDKDIR)/config/uclibc/libc/sysdeps/linux/common/bits/uClibc_mutex.h ./;\
                ../cmake-2.8.12.1/bin/cmake \
                        -DCMAKE_SYSTEM_NAME=Linux \
                        -DCMAKE_SYSTEM_VERSION=1 \
                        -DCMAKE_SYSTEM_PROCESSOR=$(ARCH) \
                        -DCMAKE_BUILD_TYPE=Release \
                        -DCMAKE_C_FLAGS_RELEASE="-DNDEBUG" \
                        -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG" \
                        -DCMAKE_C_COMPILER="$(CROSS_COMPILE)gcc" \
                        -DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
                        -DCMAKE_CXX_COMPILER="$(CROSS_COMPILE)g++" \
                        -DCMAKE_CXX_COMPILER_ARG1="$(CMAKE_CXX_COMPILER_ARG1)" \
                        -DCMAKE_EXE_LINKER_FLAGS:STRING="$(CFLAGS)" \
                        -DCMAKE_MODULE_LINKER_FLAGS:STRING="$(CFLAGS)" \
                        -DCMAKE_SHARED_LINKER_FLAGS:STRING="$(CFLAGS)" \
                        -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
                        -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
                        -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
                        -DCMAKE_STRIP=: .; \
                        -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG" \
                        -DCMAKE_C_COMPILER="$(CROSS_COMPILE)gcc" \
                        -DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
                        -DCMAKE_CXX_COMPILER="$(CROSS_COMPILE)g++" \
                        -DCMAKE_CXX_COMPILER_ARG1="$(CMAKE_CXX_COMPILER_ARG1)" \
                        -DCMAKE_EXE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
                        -DCMAKE_MODULE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
                        -DCMAKE_SHARED_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_FIND_ROOT_PATH="$(DIR_RSDK)" \
                        -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
                        -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
                        -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
			-DDL_LIBRARY=$(DIR_RSDK) \
                        -DCMAKE_STRIP=: .; \
		CFLAGS="-fPIC" ./configure --host=mips-linux --prefix="$(DIR_ROOT)/users/lib"; \
		make;\
		cd -; \
        fi
endif
ifeq ($(CONFIG_APP_SNMP),y)
	cd ./net-snmp-5.4.1; \
	./configure.rtk-snmp CROSS_COMPILE=$(CROSS_COMPILE) APMIBINC=$(APMIBINC) APMIBLIB=$(APMIBLIB); \
	cd -
endif



$(lib_y): prepare prepare_lib
	[ ! -d "$@" ] || $(MAKE) -C $@ || exit $$?

libbase:
ifneq ($(libbase_y),)
	$(MAKE) $(libbase_y) || exit $$?
else
	@echo "No base libs specified"
endif
	
$(libbase_y): prepare
	[ ! -d "$@" ] || $(MAKE) -C $@ || exit $$?
#	[ ! -d "$@" ] || $(MAKE) -C $@ install || exit $$?
ifeq ($(CONFIG_APP_WEAVE), y)
	cd ./curl-7.34.0 ; make clean;  cd ../
endif


$(app_y): prepare prepare_app
	[ ! -d "$@" ] || $(MAKE) -C $@ || exit $$?

#%_only:
#	$(MAKE) -C $(@:_only=)
#
#%_clean:
#	$(MAKE) -C $(@:_clean=) clean; true
#
#%_romfs:
#	$(MAKE) -C $(@:_romfs=) romfs

ulibs:
	[ -e $(DIR_ROMFS)/lib ] || mkdir -p $(DIR_ROMFS)/lib
	for i in $(lib_y) ; do \
		[ ! -d $$i ] || $(MAKE) -C $$i romfs || exit $$? ; \
	done

romfs:
	[ -e $(DIR_ROMFS)/bin ] || mkdir -p $(DIR_ROMFS)/bin
	for i in $(app_y) ; do \
		[ ! -d $$i ] || $(MAKE) -C $$i romfs || exit $$? ; \
	done

clean: 
	-for i in $(libbase_y) $(libbase_n) $(libbase_) $(lib_y) $(lib_n) $(lib_) $(ex_mod_y) $(ex_mod_) $(app_y) $(app_n) $(app_); do \
		$(MAKE) -C $$i clean ; \
	done

distclean:
	@rm -rf $(DIR_USERS)/lib
	-for i in $(libbase_y) $(libbase_n) $(libbase_); do \
		if [ -e ./$$i/Makefile ]; then \
		$(MAKE) -C $$i distclean ; \
		fi; \
	done
	if [ -e $(BLUEZ_DIR)/Makefile ]; then \
		make -C $(BLUEZ_DIR) distclean; \
	fi
ifeq ($(CONFIG_APP_UBUS),y)
	if [ -e ./json-c-0.12/Makefile ]; then \
		rm ./json-c-0.12/Makefile; \
	fi
	if [ -e ./libubox-2014-08-04/Makefile ]; then \
		rm ./libubox-2014-08-04/Makefile; \
	fi
	if [ -e ./ubus-2014-09-17/Makefile ]; then \
		rm ./ubus-2014-09-17/Makefile; \
	fi
endif

ifeq ($(CONFIG_APP_SQUASHFS42),y)
	@if [ -e ./xz-5.0.8/Makefile ]; then \
		make -C xz-5.0.8 clean; \
		rm -f xz-5.0.8/Makefile; \
	fi
endif

prune:
	-for i in $(sort $(app_n) $(app_)) ; do \
		found=0; \
		for j in $(sort $(app_y)) ; do \
			if [ $$i = $$j ]; then \
				found=1; \
			fi; \
		done; \
	done

include Kconfig.mk
