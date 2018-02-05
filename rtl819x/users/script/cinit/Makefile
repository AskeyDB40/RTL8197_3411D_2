include $(DIR_USERS)/.config
include $(DIR_LINUX)/.config

all :
	echo "script"
romfs :
ifeq ($(CONFIG_RTL_NFJROM_MP),y)
	cp mp*.sh $(DIR_ROMFS)/bin/
	cp set_rx_gain_from_flash.sh $(DIR_ROMFS)/bin/
else
	@cp *.sh $(DIR_ROMFS)/bin/
endif
	@cp ../script_dbg/* $(DIR_ROMFS)/bin/
ifeq ($(CONFIG_APP_WSC),y)
	$(ROMFSINST) simplecfgservice.xml /etc/simplecfgservice.xml
	$(ROMFSINST) -s /var/wps /etc/simplecfg
endif
	-rm $(DIR_ROMFS)/bin/reinit.sh
	-rm $(DIR_ROMFS)/bin/reinitNop.sh
ifeq ($(CONFIG_APP_REINIT),y)
	$(ROMFSINST) reinit.sh /bin/reinit.sh
else
	$(ROMFSINST) reinitNop.sh /bin/reinit.sh
endif

