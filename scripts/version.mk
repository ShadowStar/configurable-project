VERSION := $(shell $(TOPDIR)/scripts/git-vergen -d $(TOPDIR))
VERSION_FULL := $(shell $(TOPDIR)/scripts/git-vergen -f -d $(TOPDIR))

no-dot-cfg-targets += $(TOPDIR)/include/version.h

$(TOPDIR)/include/version.h: FORCE
	@echo "#define VERSION \"$(VERSION)\"" > $@
	@echo "#define VERSION_FULL \"$(VERSION_FULL)\"" >> $@

version.clean:
	-@rm -f $(TOPDIR)/include/version.h
