SHELL = /bin/sh

include $(CWMP_ROOT)/../.config

INSTALL = /usr/bin/install
INSTALL_DATA = ${INSTALL} -m 644

OBJEXT = o

# source files
SOAP_SOURCES = stdsoap2.cpp

SOAP_HEADERS = stdsoap2.h

# output files
SOAP_LIB	 = libgsoap$(LibPostfix).a

# CFLAGS
CFLAGS += $(SOAP_CFLAGS) $(COMMON_CFLAGS) $(SSLLIB_INCLUDES)

# beauty output 
quiet_COMPILE = $(CC)    $@
quiet_AR = $(AR)     $@ 
quiet_INSTALL = INSTALL    $(SOAP_LIB)


# here we compile
all: clean stdsoap2.o
	$(Q)echo $($(quiet)AR)
	$(Q)$(AR) cru $(SOAP_LIB) stdsoap2.o || exit $?

stdsoap2.o:stdsoap2.c
	$(Q)echo "$($(quiet)COMPILE)"
	$(Q)$(CC) -Os $(CFLAGS) -c -o $@ $< || exit $?

stdsoap2.c: stdsoap2.cpp
	$(Q)ln -s stdsoap2.cpp stdsoap2.c

install:
	$(Q)echo "$($(quiet)INSTALL)"
	$(Q)if [ ! -d $(INCLUDE_DIR) ]; then\
		$(INSTALL) -d $(INCLUDE_DIR); \
	fi
	$(Q)$(INSTALL) -d $(LIB_DIR)
	$(Q)$(INSTALL_DATA) $(SOAP_HEADERS) $(INCLUDE_DIR)
	$(Q)$(INSTALL_DATA) $(SOAP_LIB) $(LIB_DIR)

clean:
	-rm -f *.a *.o stdsoap2.c
	-rm -f $(INCLUDE_DIR)/$(SOAP_HEADERS)
	-rm -f $(LIB_DIR)/libgsoap*.a

.PHONY: all clean install
