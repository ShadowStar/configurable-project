KCONFIG_DIR := $(TOPDIR)/scripts/kconfig
KCONFIG_TOP := $(TOPDIR)/Config.in
NEED_DOT_CFG := yes

no-dot-cfg-targets += menuconfig nconfig defconfig kconfig.clean

ifneq ($(filter $(no-dot-cfg-targets),$(MAKECMDGOALS)),)
  ifeq ($(filter-out $(no-dot-cfg-targets),$(MAKECMDGOALS)),)
    NEED_DOT_CFG := no
  endif
endif

-include $(TOPDIR)/include/config/auto.conf

ifeq ($(NEED_DOT_CFG),yes)
-include $(TOPDIR)/include/config/auto.conf.cmd

$(DOT_CFG) $(TOPDIR)/include/config/auto.conf.cmd: ;

$(TOPDIR)/include/config/auto.conf: $(KCONFIG_TOP) $(DOT_CFG) $(KCONFIG_DIR)/conf

$(TOPDIR)/include/config/auto.conf: $(TOPDIR)/include/config/auto.conf.cmd
	$(SILENT)$(KCONFIG_DIR)/conf --silentoldconfig $(KCONFIG_TOP)

$(TOPDIR)/include/autoconf.h: $(TOPDIR)/include/config/auto.conf
endif

include $(TOPDIR)/scripts/kconfig/kconfig.mk

kconfig.clean:
	-$(SILENT)rm -rf include/config/*
	-$(SILENT)rm -f include/autoconf.h

kconfig.distclean: kconfig.clean
	-$(SILENT)rm -f $(KCONFIG_DIR)/*.o $(KCONFIG_DIR)/*.dep
	-$(SILENT)rm -f $(KCONFIG_DIR)/zconf.*.c
	-$(SILENT)rm -f $(KCONFIG_DIR)/lxdialog/*.o $(KCONFIG_DIR)/lxdialog/*.dep
	-$(SILENT)rm -f $(KCONFIG_DIR)/conf $(KCONFIG_DIR)/mconf $(KCONFIG_DIR)/nconf

clean: kconfig.clean
distclean: kconfig.distclean
