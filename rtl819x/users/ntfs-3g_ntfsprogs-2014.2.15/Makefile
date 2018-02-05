ifeq ($(DIR_LINUX),)
include ../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

RSDK_LIB_DIR=$(shell  echo $(PATH) | cut -f1 -d:)

SUBDIR_NTFS_3G := ntfs-3g_ntfsprogs-2014.2.15


all:
	if [ ! -e ./$(SUBDIR_NTFS_3G)/Makefile ]; then \
		cd ./$(SUBDIR_NTFS_3G); \
		LD=$(CROSS_COMPILE)ld CC="$(CROSS_COMPILE)gcc -fpic -O3" ./configure --build=i686-pc-linux-gnu --host=mips-linux; \
	fi
	make -C $(SUBDIR_NTFS_3G)
	rm -f $(RSDK_LIB_DIR)/../lib/libntfs-3g.so
	rm -f $(RSDK_LIB_DIR)/../lib/libntfs-3g.so.85
	rm -f $(RSDK_LIB_DIR)/../lib/libntfs-3g.so.85.0.0
	cp -p ./$(SUBDIR_NTFS_3G)/libntfs-3g/.libs/libntfs-3g.so.85.0.0 $(RSDK_LIB_DIR)/../lib/libntfs-3g.so.85.0.0
	ln -s libntfs-3g.so.85.0.0 $(RSDK_LIB_DIR)/../lib/libntfs-3g.so
	ln -s libntfs-3g.so.85.0.0 $(RSDK_LIB_DIR)/../lib/libntfs-3g.so.85

	
romfs:
	$(ROMFSINST) $(SUBDIR_NTFS_3G)/src/.libs/ntfs-3g /bin/ntfs-3g
	$(ROMFSINST) $(SUBDIR_NTFS_3G)/ntfsprogs/.libs/mkntfs /bin/mkntfs


clean:
	make -C $(SUBDIR_NTFS_3G) distclean
	
distclean:
	make -C $(SUBDIR_NTFS_3G) distclean
