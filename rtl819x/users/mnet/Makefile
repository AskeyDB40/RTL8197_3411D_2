include $(DIR_LINUX)/.config
include $(DIR_USERS)/.config
CFLAGS += -Os -Wall
CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += -s

ifeq ($(CONFIG_4G_LTE_SUPPORT), y)
CFLAGS += -DCONFIG_4G_LTE_SUPPORT
endif

ifeq ($(CONFIG_LEADCORE_4G_SUPPORT), y)
CFLAGS += -DCONFIG_LEADCORE_4G_SUPPORT
endif

ifeq ($(KERNEL_VER),linux-3.10)
CFLAGS += -DKERNEL_3_10
endif

EXEC = mnet 
OBJS = mnet.o
all: BUILT_TIME $(EXEC)

$(EXEC): $(OBJS)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o built_time

BUILT_TIME:
	@echo  \#define BUILT_TIME \"`TZ=UTC date -u "+%Y.%m.%d-%H:%M%z" `\" > ./built_time
EXEC = mnet 
OBJS = mnet.o
all: BUILT_TIME $(EXEC)

$(EXEC): $(OBJS)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.o built_time

BUILT_TIME:
	@echo  \#define BUILT_TIME \"`TZ=UTC date -u "+%Y.%m.%d-%H:%M%z" `\" > ./built_time
