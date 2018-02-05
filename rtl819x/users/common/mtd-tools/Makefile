ifeq ($(wildcard $(DIR_LINUX)/.config),)
include ../../../linux-2.6.30/.config
else
include $(DIR_LINUX)/.config
endif

ifeq ($(CONFIG_PROC_FS),y)
CFLAGS += -DCONFIG_PROC_FS
endif
ifeq ($(CONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE),y)
CFLAGS += -DCONFIG_RTL_FLASH_DUAL_IMAGE_ENABLE
endif
ifeq ($(CONFIG_MTD_NAND),y)
CFLAGS += -DCONFIG_MTD_NAND
endif

RTK_MTDLIB = libmtdapi.so
SOURCES = rtl_mtd.o 
OBJS = $(SOURCES:.c=.o)

all:$(RTK_MTDLIB)
	make install

$(RTK_MTDLIB): $(OBJS)
	$(CC) $(CFLAGS) -s -shared -o $@ $(OBJS)

install:
	mkdir -p $(DIR_ROOT)/users/lib/lib
	cp libmtdapi.so $(DIR_ROOT)/users/lib/lib

.c.o:
	$(CC) -c -o $@ -fpic $(DEBUG) $(CFLAGS) $(IFLAGS) $<

clean:
	-rm -f  *.elf *.o libmtdapi.so
