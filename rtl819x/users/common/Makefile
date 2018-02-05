include $(DIR_ROOT)/.config
include $(DIR_LINUX)/.config
include $(DIR_USERS)/.config

api_$(CONFIG_MTD_API)	+= mtd-tools

all: 
	for i in $(api_y) ; do \
	[ ! -d $$i ] || $(MAKE) -C $$i || exit $$? ; \
	done	
	@echo "finish compile api"
install:
	for i in $(api_y) ; do \
	[ ! -d $$i ] || $(MAKE) -C $$i install || exit $$? ; \
	done 

clean:
	for i in $(api_y) ; do \
	[ ! -d $$i ] || $(MAKE) -C $$i clean || exit $$? ; \
	done 
