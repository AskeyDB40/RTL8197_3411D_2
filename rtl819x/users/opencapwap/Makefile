WTP_NAME = WTP
AACWTP_NAME = AACWTP
AC_NAME = AC
WUM_NAME = wum
all:
	@echo "builded"
romfs:
	$(ROMFSINST) $(WTP_NAME)_$(TOOLCHAIN) /bin/$(WTP_NAME)
	$(ROMFSINST) $(AACWTP_NAME)_$(TOOLCHAIN) /bin/$(AACWTP_NAME)
	$(ROMFSINST) $(AC_NAME)_$(TOOLCHAIN) /bin/$(AC_NAME)
	$(ROMFSINST) $(WUM_NAME)_$(TOOLCHAIN) /bin/$(WUM_NAME)
	mkdir $(DIR_ROMFS)/etc/capwap
	./write_setting_$(TOOLCHAIN)
	$(ROMFSINST) ./config.wtp /etc/capwap/config.wtp
	$(ROMFSINST) ./config.ac /etc/capwap/config.ac
	$(ROMFSINST) ./settings.wtp.txt /etc/capwap/settings.wtp.txt
	$(ROMFSINST) ./settings.ac.txt /etc/capwap/settings.ac.txt
	$(ROMFSINST) ./root.pem /etc/capwap/root.pem
	$(ROMFSINST) ./client.pem /etc/capwap/client.pem
	$(ROMFSINST) ./server.pem /etc/capwap/server.pem
clean:
	@echo "It's cleaned"
