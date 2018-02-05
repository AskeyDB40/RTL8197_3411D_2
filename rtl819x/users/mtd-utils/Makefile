CROSS_COMPILE ?= msdk-linux-
MTDUTILS_DIR = $(CROSS_COMPILE:-=)
SOURCE=mtd-utils-1.5.0

all:
	make -C $(SOURCE) all
	
romfs:
	$(ROMFSINST) $(SOURCE)/$(MTDUTILS_DIR)/flash_erase /bin/flash_erase		

clean:
	make -C $(SOURCE) clean
