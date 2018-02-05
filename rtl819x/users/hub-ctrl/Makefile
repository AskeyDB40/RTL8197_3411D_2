CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RANLIB = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip
SSTRIP = $(CROSS_COMPILE)sstrip

SRC         = hub-ctrl-2

PROG        = hub-ctrl
RM          = /bin/rm -f

ifeq ($(KERNEL_VER),linux-2.6.30)
LIBUSB		= libusb
LIBUSB_V	= 0.1.12
LIB_USB		= ../$(LIBUSB)-$(LIBUSB_V)

CFLAGS += -Os
CFLAGS += $(EXTRA_CFLAGS)
CFLAGS += -I $(LIB_USB)
endif
ifeq ($(KERNEL_VER),linux-3.10)
CFLAGS += -Os
CFLAGS += $(EXTRA_CFLAGS)
CFLAGS += -I../usb-k310/usr/local/include
endif

all: $(SRC).o
ifeq ($(KERNEL_VER),linux-2.6.30)
	make -C $(LIB_USB)
	$(CC) $(SRC).o -L$(LIB_USB) -lusb -o $(PROG)
endif
ifeq ($(KERNEL_VER),linux-3.10)
	$(CC) $(SRC).o -L../usb-k310/usr/local/lib -lusb -lusb-1.0 -o $(PROG)
endif

romfs:
	$(ROMFSINST) /bin/$(PROG)

clean:
	$(RM) -f $(EXEC) *.elf *.o *.a
	$(RM) $(PROG)

