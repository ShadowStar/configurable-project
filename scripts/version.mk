PRJ_VERSION := $(shell $(TOPDIR)/scripts/git-vergen -c -d $(TOPDIR))
PRJ_VERSION_FULL := $(shell $(TOPDIR)/scripts/git-vergen -f -n -d $(TOPDIR))
PRJ_GIT_SHA1 := $(shell GIT_WORK_TREE=$(TOPDIR) git show-ref -h -s 2>/dev/null | head -n1 | cut -c 1-16)

export PRJ_VERSION PRJ_VERSION_FULL

no-dot-cfg-targets += $(TOPDIR)/include/version.h version

$(TOPDIR)/include/version.h: FORCE
	@echo "#define PRJ_VERSION \"$(PRJ_VERSION)\"" > $@
	@echo "#define PRJ_VERSION_FULL \"$(PRJ_VERSION_FULL)\"" >> $@
	@echo "#define PRJ_GIT_SHA1 0x$(PRJ_GIT_SHA1)" >> $@

version:
	@echo "PRJ_VERSION:      $(PRJ_VERSION)"
	@echo "PRJ_VERSION_FULL: $(PRJ_VERSION_FULL)"
	@echo "PRJ_GIT_SHA1:     $(PRJ_GIT_SHA1)"

version.clean:
	-@rm -f $(TOPDIR)/include/version.h

PHONY += FORCE

.PHONY: $(PHONY)
