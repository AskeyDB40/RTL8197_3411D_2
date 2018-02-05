
DESTDIR=/usr/local
PREFIX=mbedtls_

CC=mips-linux-gcc -fPIC
LD=mips-linux-ld
STRIP=mips-linux-strip
AR=mips-linux-ar

.SILENT:

.PHONY: all programs lib tests romfs clean

all: programs tests

programs: lib
	$(MAKE) -C programs

lib:
	$(MAKE) -C library

tests: lib
	$(MAKE) -C tests

romfs:
	$(ROMFSINST) library/libmbedtls.so.10  		/lib/libmbedtls.so.10
	$(ROMFSINST) library/libmbedcrypto.so.0  	/lib/libmbedcrypto.so.0
	$(ROMFSINST) library/libmbedx509.so.0  		/lib/libmbedx509.so.0

clean:
	$(MAKE) -C library clean
	$(MAKE) -C programs clean
	$(MAKE) -C tests clean
	find . \( -name \*.gcno -o -name \*.gcda -o -name *.info \) -exec rm {} +

