
EXEC = ethctl
OBJS = ethctl.o

ifeq ($(CONFIG_ISP_IGMPPROXY_MULTIWAN),y)
CFLAGS += -DCONFIG_ISP_IGMPPROXY_MULTIWAN
endif

all: $(EXEC)

$(EXEC): $(OBJS)
ifndef CONFIG_USER_BOA_SRC_BOA
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
else  #CONFIG_USER_BOA_SRC_BOA
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) $(LIBMIB) $(LIBVOIP)
endif #CONFIG_USER_BOA_SRC_BOA

romfs:
	$(ROMFSINST) /bin/$(EXEC)
	#$(ROMFSINST) ./ethctl /bin/ethctl
clean:
	-rm -f $(EXEC) *.elf *.o

