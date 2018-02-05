############################################################################
#
# Realtek Semiconductor Corp.
#
# Makefile -- Top level dist makefile.
#
# Tony Wu (tonywu@realtek.com)
# Oct. 12, 2008
#

ifneq (.config,$(wildcard .config))
TARGET := config
#
# DIR_ROOT Used when .config not Exist
#
DIR_ROOT = $(shell pwd)
DIR_USERS = $(DIR_ROOT)/users
DIR_BZBOX = $(DIR_ROOT)/users/busybox-1.13
DIR_LINUX = $(DIR_ROOT)/linux-2.6.30
export DIR_ROOT DIR_USERS DIR_BZBOX DIR_LINUX
else

include .config

#
# variables
#
DIR_ROOT = $(shell pwd)
DIR_RSDK = $(DIR_ROOT)/$(CONFIG_RSDKDIR)

ifeq ($(CONFIG_ARCH_CPU_MIPS),y)
VAR_ARCH:= mips
CROSS_TARGET := mips-linux
CROSS_COMPILE:= msdk-linux-
else
VAR_ARCH:= rlx
CROSS_TARGET := rsdk-linux
CROSS_COMPILE := rsdk-linux-
endif
export VAR_ARCH CROSS_COMPILE CROSS_TARGET

ifeq ($(CONFIG_ARCH_CPU_EL),y)
VAR_ENDIAN := EL
else
VAR_ENDIAN := EB
endif

DIR_BOARD = $(DIR_ROOT)/target
DIR_ROMFS = $(DIR_ROOT)/target/romfs
DIR_TMPFS = $(DIR_ROOT)/target/tmpfs
DIR_IMAGE = $(DIR_ROOT)/target/image
DIR_TOOLS = $(DIR_ROOT)/target/tools
DIR_USERS = $(DIR_ROOT)/users
DIR_BZBOX = $(DIR_ROOT)/$(CONFIG_BZBOXDIR)
DIR_LINUX = $(DIR_ROOT)/$(CONFIG_LINUXDIR)

ROMFSINST = $(DIR_ROOT)/config/romfsinst

RCSCONFIG = $(DIR_ROMFS)/etc/init.d/rcS.conf

MAKE = PATH=$(DIR_RSDK)/bin:$(PATH); make
LSTRIP = PATH=$(DIR_RSDK)/bin:$(PATH); rsdk-linux-lstrip
SSTRIP = PATH=$(DIR_RSDK)/bin:$(PATH); rsdk-linux-sstrip

ifeq ($(CONFIG_ARCH_CPU_MIPS),y)
LSTRIP = PATH=$(DIR_RSDK)/bin:$(PATH); msdk-linux-lstrip
SSTRIP = PATH=$(DIR_RSDK)/bin:$(PATH); msdk-linux-sstrip
endif

export DIR_ROOT DIR_RSDK DIR_ROMFS DIR_IMAGE DIR_BOARD DIR_TMPFS
export DIR_LINUX DIR_BZBOX ROMFSINST DIR_TOOLS DIR_USERS 
export RCSCONFIG

ERROR :=
ifneq ($(CONFIG_BZBOXDIR),$(wildcard $(CONFIG_BZBOXDIR)))
ERROR += '$(CONFIG_BZBOXDIR) does not exist '
endif

ifneq ($(CONFIG_LINUXDIR),$(wildcard $(CONFIG_LINUXDIR)))
ERROR += '$(CONFIG_LINUXDIR) does not exist '
endif

ifneq ($(CONFIG_RSDKDIR),$(wildcard $(CONFIG_RSDKDIR)))
ERROR += '$(CONFIG_RSDKDIR) does not exist'
endif

ifeq ($(ERROR),)
# for ramfs or MP build, rootfs must be ready before kernel
TARGET := bins romfs modules external_modules linux image
#TARGET := modules linux bins romfs image
else
TARGET := error
endif

endif

TOOLCHAIN =
ifeq ($(CONFIG_BOARD_rtl8881a),y)
	TOOLCHAIN = 4.6.4-5281
endif
ifeq ($(CONFIG_BOARD_rtl8197F),y)
	TOOLCHAIN = 4.4.7-mips-EL
endif
ifeq ($(CONFIG_BOARD_rtl8198C_8954E),y)
	TOOLCHAIN = 4.4.7-mips-EB
endif
ifeq ($(CONFIG_BOARD_rtl819xD),y)
	TOOLCHAIN = 4.6.4-5281
endif
ifeq ($(CONFIG_BOARD_rtl8196e),y)
	TOOLCHAIN = 4.6.4-4181
endif
TOOLCHAIN =
ifeq ($(CONFIG_BOARD_rtl8881a),y)
	TOOLCHAIN = 4.6.4-5281
endif
ifeq ($(CONFIG_BOARD_rtl8197F),y)
	TOOLCHAIN = 4.4.7-mips-EL
endif
ifeq ($(CONFIG_BOARD_rtl8198C_8954E),y)
	TOOLCHAIN = 4.4.7-mips-EB
endif
ifeq ($(CONFIG_BOARD_rtl819xD),y)
	TOOLCHAIN = 4.6.4-5281
endif
ifeq ($(CONFIG_BOARD_rtl8196e),y)
	TOOLCHAIN = 4.6.4-4181
endif
all: $(TARGET)

error:
	@echo
	@echo "=== NOTICE ===" 
	@echo
	@for X in $(ERROR) ; do \
		echo ERROR: $$X; \
	done
	@echo
	@echo "Please run 'make config' to reconfigure"
	@echo

#
# 0. target selection
#
.PHONY: config menuconfig
config menuconfig:
	@chmod u+x config/genconfig
	@chmod u+x config/setconfig
	@config/genconfig > Kconfig
	@config/mconf Kconfig
#	@if egrep "^CONFIG_BOARD_rtl8196eu=y" .config > /dev/null; then \
#        cp $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig-otg $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig; \
#    else \
#        cp $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig-not-otg $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig; \
#	fi	
	@config/setconfig defaults
	@if egrep "^CONFIG_MCONF_LINUX=y" .config > /dev/null; then \
		if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
			$(MAKE) -C $(DIR_LINUX) menuconfig VAR_ARCH=mips-ori; \
		else \
			$(MAKE) -C $(DIR_LINUX) menuconfig; \
		fi \
	fi
	@if egrep "^CONFIG_MCONF_USERS=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_USERS) menuconfig; \
	fi
	@if egrep "^CONFIG_MCONF_BZBOX=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_BZBOX) menuconfig; \
	fi
	@config/setconfig final
	@config/hdrconfig $(DIR_ROOT)

#
# 1. user lib
#
#.PHONY: libs
#libs:
#	$(MAKE) -C $(DIR_USERS) lib || exit $$?

#
# 2. user app
#
.PHONY: users bins
users bins:
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) dep VAR_ARCH=mips-ori > /dev/null; \
	else \
		$(MAKE) -C $(DIR_LINUX) dep > /dev/null; \
	fi
	$(MAKE) -C $(DIR_USERS) TOOLCHAIN=$(TOOLCHAIN) || exit $$?

#
#2.1 user menuconfig
#
users_menuconfig:
	$(MAKE) -C $(DIR_USERS) menuconfig; 

#
# 3. romfs
#
#	[ -d $(DIR_ROMFS) ] || mkdir $(DIR_ROMFS)
.PHONY: romfs
romfs:
	$(MAKE) -C $(DIR_BOARD) romfs || exit $$?
	cp -R $(DIR_RSDK)/lib/*.so $(DIR_ROMFS)/lib
	cp -R $(DIR_RSDK)/lib/*.so.* $(DIR_ROMFS)/lib
	chmod 755 $(DIR_ROMFS)/lib/*.so
	$(MAKE) -C $(DIR_USERS) TOOLCHAIN=$(TOOLCHAIN) romfs || exit $$?
	$(LSTRIP) $(DIR_ROMFS)

#
# 4.0 kernel modules (if any)
#
.PHONY: modules
modules:
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) modules VAR_ARCH=mips-ori || exit $$?; \
		$(MAKE) -C $(DIR_LINUX) modules_install INSTALL_MOD_PATH=$(DIR_ROMFS) VAR_ARCH=mips-ori || exit $$?; \
	else \
		$(MAKE) -C $(DIR_LINUX) modules || exit $$?; \
		$(MAKE) -C $(DIR_LINUX) modules_install INSTALL_MOD_PATH=$(DIR_ROMFS) || exit $$?; \
	fi

#
# 4.1 kernel image
#
.PHONY: linux
linux:
	. $(DIR_LINUX)/.config; if [ "$$CONFIG_INITRAMFS_SOURCE" != "" ]; then \
        cd $(DIR_LINUX); \
        mkdir -p $(DIR_ROMFS); \
        touch $$CONFIG_INITRAMFS_SOURCE | awk '{print $2}' || exit 1; \
        cd $(DIR_ROOT); \
	fi
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) VAR_ARCH=mips-ori || exit $$?; \
	else \
		$(MAKE) -C $(DIR_LINUX) || exit $$?; \
	fi
	if [ -f $(DIR_LINUX)/vmlinux ]; then \
        ln -f $(DIR_LINUX)/vmlinux $(DIR_LINUX)/linux ; \
	fi

uImage:
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) uImage VAR_ARCH=mips-ori; \
	else \
		$(MAKE) -C $(DIR_LINUX) uImage; \
	fi

#
#4.2 kernel menuconfig
#
linux_menuconfig:
#	@if egrep "^CONFIG_BOARD_rtl8196eu=y" .config > /dev/null; then \
#        cp $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig-otg $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig; \
#    else \
#        cp $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig-not-otg $(shell pwd)/linux-2.6.30/drivers/usb/Kconfig; \
#	fi
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) menuconfig VAR_ARCH=mips-ori; \
	else \
		$(MAKE) -C $(DIR_LINUX) menuconfig; \
	fi

#
#4.3 external modules
#
.PHONY: external_modules
external_modules:
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) dep VAR_ARCH=mips-ori > /dev/null; \
	else \
		$(MAKE) -C $(DIR_LINUX) dep > /dev/null; \
	fi
	$(MAKE) -C $(DIR_USERS) ex_mod || exit $$?

#
# 5. image
#
.PHONY: image
image:
	[ -d $(DIR_IMAGE) ] || mkdir $(DIR_IMAGE)
	$(MAKE) -C $(DIR_BOARD) image


#
# x. short-cut to build and clean one application
#
linux-clean:
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) clean VAR_ARCH=mips-ori; \
	else \
		$(MAKE) -C $(DIR_LINUX) clean; \
	fi

%_build:
	$(MAKE) -C users $(@:_build=)

%-build:
	$(MAKE) -C users $(@:-build=)

%_clean:
	$(MAKE) -C users/$(@:_clean=) clean

%-clean:
	$(MAKE) -C users/$(@:-clean=) clean

#.PHONY: version
#version:
#	$(MAKE) -C $(DIR_USERS)/goahead-2.1.1/LINUX release

.PHONY: clean  distclean
clean:
	$(MAKE) -C $(DIR_USERS) clean
	if egrep "^CONFIG_BOARD_rtl8197F=y" .config > /dev/null; then \
		$(MAKE) -C $(DIR_LINUX) clean VAR_ARCH=mips-ori; \
	else \
		$(MAKE) -C $(DIR_LINUX) clean; \
	fi
	rm -rf $(DIR_ROMFS)
	mkdir -p $(DIR_ROMFS)
	mkdir -p $(DIR_IMAGE)

distclean:
	$(MAKE) -C $(DIR_USERS) distclean
