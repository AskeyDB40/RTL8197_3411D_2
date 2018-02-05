#
# Makefile for FT daemon for the Linux OS
#

include ../.config

#RTL_FT_IFLAGS = -I../wsc/portingssl/crypto/include
#RTL_FT_LIBS = -L../wsc/portingssl -lcrypto
#RTL_FT_LIBS += -L$(ROOTDIR)/lib/librtk $(LIBMD5) -DUSE_RTK_LIB
#RTL_FT_LIBS += ../openssl-0.9.8b/libcrypto.a
CFLAGS += $(RTL_FT_IFLAGS) $(RTL_FT_LIBS) $(RTL_FT_CFLAG)

USE_OPENSSL_DLIB = false

ifeq ($(strip $(CONFIG_APP_OPENSSL-0.9.8B)),y)
openssl_dir = ../openssl-0.9.8b
endif

ifeq ($(strip $(CONFIG_APP_OPENSSL-0.9.8I)),y)
openssl_dir = ../openssl-0.9.8i
endif

ifeq ($(strip $(CONFIG_APP_OPENSSL-0.9.8Y)),y)
openssl_dir = ../openssl-0.9.8y
endif

ifeq ($(openssl_dir),n)
openssl_dir = ../openssl-0.9.8i
endif

ifeq ($(strip $(USE_OPENSSL_DLIB)),true)
RTL_FT_LIBS +=  ${openssl_dir}/libcrypto.so
else
RTL_FT_LIBS +=  ${openssl_dir}/libcrypto.a
endif

all: ftd

ft_main.o : ft_main.c ft_main.h

ftd: ft_main.o
	$(CC) -o ftd ft_main.o $(CFLAGS)


clean:
	rm -f *.o ftd

romfs:
	$(ROMFSINST) ftd /bin/ftd
	$(ROMFSINST) ft.conf /etc/ft.conf

