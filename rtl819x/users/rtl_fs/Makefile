include $(DIR_LINUX)/.config

ifeq ($(CONFIG_RTL_8198C),y)
  MODULE_TARGET:=r279412
else
  $(error rtl_fs only supports for RTL8198C)
endif

export MODULE_TARGET

all:
	@if [ -e Makefile.build ]; then \
		make -f Makefile.build all ; \
	fi

install:
	$(ROMFSINST) modules/$(MODULE_TARGET)/jnl.ko /lib/modules
	$(ROMFSINST) modules/$(MODULE_TARGET)/ufsd.ko /lib/modules
	$(ROMFSINST) utilities/$(MODULE_TARGET)/mkntfs /bin/
	$(ROMFSINST) utilities/$(MODULE_TARGET)/mkexfat /bin/
	$(ROMFSINST) utilities/$(MODULE_TARGET)/mkhfs /bin/
	$(ROMFSINST) utilities/$(MODULE_TARGET)/chkufsd /bin/
	$(ROMFSINST) -s chkufsd /bin/chkntfs
	$(ROMFSINST) -s chkufsd /bin/chkhfs
	$(ROMFSINST) -s chkufsd /bin/chkexfat
#	$(ROMFSINST) $(MODULE_TARGET)/sysdump /bin/

clean:
	@if [ -e Makefile.build ]; then \
		make -f Makefile.build clean ; \
		rm -rf modules utilities; \
	fi
