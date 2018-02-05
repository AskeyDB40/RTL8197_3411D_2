
include $(DIR_LINUX)/.config
DIR_ROMFS = ../../target/romfs
ROMFSINST = ../../config/romfsinst

CFLAGS := -s -Wall -Os $(CFLAGS)
LDFLAGS = -s -Wall
CFLAGS += -I../libpcap-1.4.0
LIBS = ../libpcap-1.4.0/libpcap.a

ifeq ($(CONFIG_PACP_SUPPORT), y)
CFLAGS += -DCONFIG_PACP_SUPPORT 
endif

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -s -Wl,--gc-sections
endif

all:	pcap

pcap:	packet_capture.c 
	$(CC) -o pcap packet_capture.c $(LIBS) $(CFLAGS)

clean:
	rm -f *.o
	rm -f pcap

romfs:
	cp pcap ../../romfs/bin
	
