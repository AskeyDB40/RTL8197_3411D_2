ifeq ($(DIR_LINUX),)
include ../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

RSDK_LIB_DIR=$(shell  echo $(PATH) | cut -f1 -d:)

SUBDIR := parted-3.0
E2FSPROGS_DIR := $(shell pwd)/../e2fsprogs-1.42.11


all:
	if [ ! -e ./$(SUBDIR)/Makefile ]; then \
		cd ./$(SUBDIR); \
		LD=$(CROSS_COMPILE)ld CC="$(CROSS_COMPILE)gcc -O3" ./configure LDFLAGS="-L$(E2FSPROGS_DIR)/lib" CPPFLAGS="-I$(E2FSPROGS_DIR)/lib" --build=i686-pc-linux-gnu --host=mips-linux --disable-shared --disable-device-mapper --without-readline; \
	fi
	make -C $(SUBDIR)

	
romfs:
	$(ROMFSINST) $(SUBDIR)/partprobe/partprobe /bin/partprobe
	$(ROMFSINST) $(SUBDIR)/parted/parted /bin/parted


clean:
	make -C $(SUBDIR) distclean
	
distclean:
	make -C $(SUBDIR) distclean
