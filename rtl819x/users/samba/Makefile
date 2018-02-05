#
# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# Jan. 10, 2011
#

DIR_USERS := ..
include $(DIR_USERS)/.config
include $(DIR_LINUX)/.config

ifeq ($(CONFIG_APP_SAMBA-3.0.24),y)
  DIR_SAMBA := samba-3.0.24
  DIR_SOURCE := samba-3.0.24/source
  CLEAN := clean
else ifeq ($(CONFIG_APP_SAMBA-3.2.15),y)
  DIR_SAMBA := samba-3.2.15
  DIR_SOURCE := samba-3.2.15/source
  CLEAN := clean
  ifeq ($(CONFIG_SAMBA_LOG_SUPPORT),y)
    CFLAGS+= -DUSE_LOGGING
  else
    CFLAGS+= -DAVM_NO_DEBUG
  endif
else ifeq ($(CONFIG_APP_SAMBA-3.6.24),y)
  DIR_SAMBA := samba-3.6.24
  DIR_SOURCE := samba-3.6.24/source3
  CFLAGS += -ffunction-sections -fdata-sections
  CLEAN := distclean
  ifeq ($(CONFIG_SAMBA_LOG_SUPPORT),y)
    CFLAGS+= -DMAX_DEBUG_LEVEL=10
  else
    CFLAGS+= -DMAX_DEBUG_LEVEL=-1
  endif

  ifdef CONFIG_RTL_SENDFILE_PATCH
    CFLAGS += -DCONFIG_RTL_SENDFILE_PATCH
  endif

  #TARGET_LDFLAGS += -Wl,--gc-sections
  #MAKE_FLAGS += DYNEXP= PICFLAG= MODULES=
endif

all: configCheck samba

configCheck:
	if [ -e $(DIR_SAMBA)/.smbConfig ] ; then \
		egrep "CONFIG_SAMBA_LOG_SUPPORT" $(DIR_USERS)/.config >$(DIR_SAMBA)/.smbConfig_new;\
		diff $(DIR_SAMBA)/.smbConfig $(DIR_SAMBA)/.smbConfig_new > $(DIR_SAMBA)/.diff; \
		if [ -s $(DIR_SAMBA)/.diff ] ; then \
			mv $(DIR_SAMBA)/.smbConfig_new $(DIR_SAMBA)/.smbConfig;\
			make clean;\
		fi;\
	else \
		egrep "CONFIG_SAMBA_LOG_SUPPORT" $(DIR_USERS)/.config >$(DIR_SAMBA)/.smbConfig;\
		make clean;\
	fi;

samba:
ifneq (y, $(filter y,$(CONFIG_APP_SAMBA-3.0.24) $(CONFIG_APP_SAMBA-3.2.15)))
	cd $(DIR_SOURCE); \
	[ -f ./Makefile ] || \
	c_cv_lib_attr_getxattr=no \
	ac_cv_search_getxattr=no \
	ac_cv_file__proc_sys_kernel_core_pattern=yes \
	libreplace_cv_HAVE_C99_VSNPRINTF=yes \
	libreplace_cv_HAVE_GETADDRINFO=yes \
	libreplace_cv_HAVE_IFACE_IFCONF=yes \
	LINUX_LFS_SUPPORT=yes \
	samba_cv_CC_NEGATIVE_ENUM_VALUES=yes \
	samba_cv_HAVE_GETTIMEOFDAY_TZ=yes \
	samba_cv_HAVE_IFACE_IFCONF=yes \
	samba_cv_HAVE_KERNEL_OPLOCKS_LINUX=yes \
	samba_cv_HAVE_SECURE_MKSTEMP=yes \
	samba_cv_HAVE_WRFILE_KEYTAB=no \
	samba_cv_USE_SETREUID=yes \
	samba_cv_USE_SETRESUID=yes \
	samba_cv_have_setreuid=yes \
	samba_cv_have_setresuid=yes \
	ac_cv_header_zlib_h=no \
	samba_cv_zlib_1_2_3=no \
	CFLAGS="-Os $(CFLAGS)" \
	./configure \
		--host=$(CROSS_TARGET) \
		--target=$(CROSS_TARGET) \
		--exec-prefix=/bin \
		--prefix=/ \
		--disable-avahi \
		--disable-cups \
		--disable-pie \
		--disable-relro \
		--disable-static \
		--disable-swat \
		--disable-shared-libs \
		--with-codepagedir=/var/lib \
		--with-configdir=/var/samba \
		--with-included-iniparser \
		--with-included-popt \
		--with-lockdir=/var/lock \
		--with-logfilebase=/var/log \
		--with-nmbdsocketdir=/var/nmbd \
		--with-piddir=/var/run \
		--with-privatedir=/var/samba \
		--with-sendfile-support \
		--without-acl-support \
		--without-cluster-support \
		--without-ads \
		--without-krb5 \
		--without-ldap \
		--without-pam \
		--without-winbind \
		--without-libtdb \
		--without-libtalloc \
		--without-libnetapi \
		--without-libsmbclient \
		--without-libsmbsharemodes \
		--without-libtevent \
		--without-libaddns \
		--with-libiconv=$(DIR_RSDK)/lib
endif
	$(MAKE) -C $(DIR_SOURCE)

romfs:
ifeq (y, $(filter y,$(CONFIG_APP_SAMBA-3.0.24) $(CONFIG_APP_SAMBA-3.2.15)))
	$(ROMFSINST) $(DIR_SOURCE)/bin/smbd /bin/smbd
	$(ROMFSINST) $(DIR_SOURCE)/bin/smbpasswd /bin/smbpasswd
	$(ROMFSINST) $(DIR_SOURCE)/bin/nmbd /bin/nmbd
	$(ROMFSINST) $(DIR_SOURCE)/codepages/lowcase.dat /etc/samba/lowcase.dat
	$(ROMFSINST) $(DIR_SOURCE)/codepages/upcase.dat /etc/samba/upcase.dat
	$(ROMFSINST) $(DIR_SOURCE)/codepages/valid.dat /etc/samba/valid.dat
	$(ROMFSINST) smbpasswd /etc/samba/smbpasswd
ifeq ($(CONFIG_APP_SAMBA_WEB),y)
	$(ROMFSINST) smb_StorageManagement.conf /etc/samba/smb.conf
else
	$(ROMFSINST) smb.conf /etc/samba/smb.conf
endif
else
	$(ROMFSINST) $(DIR_SOURCE)/bin/samba_multicall /bin/samba_multicall
	$(ROMFSINST) -s samba_multicall /bin/nmbd
	$(ROMFSINST) -s samba_multicall /bin/smbd
	$(ROMFSINST) -s samba_multicall /bin/smbpasswd
	$(ROMFSINST) $(DIR_SAMBA)/codepages/lowcase.dat /etc/samba/lowcase.dat
	$(ROMFSINST) $(DIR_SAMBA)/codepages/upcase.dat /etc/samba/upcase.dat
	$(ROMFSINST) $(DIR_SAMBA)/codepages/valid.dat /etc/samba/valid.dat
	$(ROMFSINST) smbpasswd /etc/samba/smbpasswd
ifeq ($(CONFIG_APP_SAMBA_WEB),y)
	$(ROMFSINST) smb_3_6_24_StorageManagement.conf /etc/samba/smb.conf
else
	$(ROMFSINST) smb_3_6_24.conf /etc/samba/smb.conf
endif
endif
#	$(ROMFSINST) smbpasswd /etc/samba/smbpasswd
#	$(ROMFSINST) smb_3_6_24.conf /etc/samba/smb.conf
clean:
	if [ -f $(DIR_SOURCE)/Makefile ]; then \
		$(MAKE) -C $(DIR_SOURCE) $(CLEAN); \
	fi
