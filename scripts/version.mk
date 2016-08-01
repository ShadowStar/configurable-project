-include $(TOPDIR)/include/config/auto.conf

VERSION := $(shell $(TOPDIR)/scripts/git-vergen -r $(CFG_BUILD_LEVEL_SHORT_NAME) -d $(TOPDIR))
VERSION_FULL := $(shell $(TOPDIR)/scripts/git-vergen -r $(CFG_BUILD_LEVEL_SHORT_NAME) -f -d $(TOPDIR))

no-dot-cfg-targets += $(TOPDIR)/include/version.h

$(TOPDIR)/include/version.h: FORCE
	@echo "#define VERSION \"$(VERSION)\"" > $@
	@echo "#define VERSION_FULL \"$(VERSION_FULL)\"" >> $@

version.clean:
	-@rm -f $(TOPDIR)/include/version.h
