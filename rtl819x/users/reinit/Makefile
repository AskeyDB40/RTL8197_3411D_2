ifeq ($(wildcard $(DIR_LINUX)/.config),)
ifeq ($(KERNEL_VER),linux-2.6.30)
include ../../linux-2.6.30/.config
else
include ../../linux-3.10/.config
endif
else
include $(DIR_LINUX)/.config
endif

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../.config
else
include $(DIR_USERS)/.config
endif

#------------------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .o .c
.PHONY: clean all depend

LDFLAGS =  -g
CFLAGS = -Os -pipe
DEPEND = ./.depend

#------------------------------------------------------------------------------
APMIB_DIR=$(DIR_USERS)/boa/apmib

LIB_DIR=$(DIR_USERS)/lib/include
UBUS_DIR=$(DIR_USERS)/ubus-2014-09-17
JSONC_DIR=$(DIR_USERS)/lib/include/json-c
#------------------------------------------------------------------------------
# EXTRA_CFLAGS
#------------------------------------------------------------------------------
EXTRA_CFLAGS =

#------------------------------------------------------------------------------
# CFLAGS
#------------------------------------------------------------------------------
DEBUG	= -g -Wall
IFLAGS 	= -I. -I$(APMIB_DIR) -I$(JSONC_DIR) -I$(LIB_DIR)  -I$(UBUS_DIR) 
CFLAGS += $(COMMON_CFLAGS) $(EXTRA_CFLAGS)

ifeq ($(CONFIG_ISP_IGMPPROXY_MULTIWAN),y)
CFLAGS += -DCONFIG_ISP_IGMPPROXY_MULTIWAN
endif

SOURCES_SER = reinitSer.c reinitUbus.c reinitFunc_wan.c reinitFunc_lan.c reinitFunc_wlan.c reinitFunc.c reinitEvent.c reinitHandle.c  reinit_utility.c reinitFunc_firewall.c decisonFunc.c reinitWan_confliction.c

SOURCES_CLI = reinitCli.c
OBJS_SER = $(SOURCES_SER:.c=.o)
#------------------------------------------------------------------------------
# targets
#------------------------------------------------------------------------------
all: depend reinitSer reinitCli ppp_controler


reinitSer: Makefile $(OBJS_SER)
	$(CC) -o $@  $(OBJS_SER) $(LDFLAGS)  -L$(DIR_USERS)/lib/lib -lubox -lblobmsg_json -L$(UBUS_DIR) -lubus -L$(APMIB_DIR) -lapmib 
	$(STRIP) $@

reinitCli: Makefile reinitCli.o
	$(CC) -o $@  reinitCli.o  $(LDFLAGS)  -L$(DIR_USERS)/lib/lib -lubox -lblobmsg_json -L$(UBUS_DIR) -lubus -L$(APMIB_DIR) -lapmib
	$(STRIP) $@

ppp_controler: Makefile ppp_controler.o reinit_utility.o
	$(CC) -o $@ ppp_controler.o  reinit_utility.o $(LDFLAGS) -L$(APMIB_DIR) -lapmib
	$(STRIP) $@

clean:
	rm -f $(DEPEND)
	rm -f *.o reinitSer reinitCli
	rm -rf ppp_controler

depend: $(SOURCES_SER) $(SOURCES_CLI)
	if [ ! -e $(DEPEND) ]; then \
		$(CPP) $(DEBUG) $(CFLAGS) $(IFLAGS) -MM $^ > $(DEPEND); \
	fi
	
-include $(DEPEND)

.c.o:
	${CC} -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<

romfs:
	$(ROMFSINST) reinitSer /bin/reinitSer
	$(ROMFSINST) reinitCli /bin/reinitCli
	$(ROMFSINST) ppp_controler  /bin/ppp_controler
