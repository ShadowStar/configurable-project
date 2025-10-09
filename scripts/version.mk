PRJ_VERSION := $(shell $(TOPDIR)/scripts/git-vergen -c -d $(TOPDIR))
PRJ_VERSION_FULL := $(shell $(TOPDIR)/scripts/git-vergen -f -n -d $(TOPDIR))
PRJ_GIT_SHA1 := $(shell GIT_WORK_TREE=$(TOPDIR) git show-ref -h -s 2>/dev/null | head -n1 | cut -c 1-16)
PRJ_VERSION_H := $(TOPDIR)/include/version.h

export PRJ_VERSION PRJ_VERSION_FULL

no-dot-cfg-targets += $(PRJ_VERSION_H) version

$(PRJ_VERSION_H): FORCE
	@if [ ! -f $@ ] || [ "$$(cat $@ | grep "\<PRJ_VERSION\>" | cut -d' ' -f3-)" != "\"$(PRJ_VERSION)\"" ]; then \
		echo "#define PRJ_VERSION \"$(PRJ_VERSION)\"" > $@; \
		echo "#define PRJ_VERSION_FULL \"$(PRJ_VERSION_FULL)\"" >> $@; \
		echo "#define PRJ_GIT_SHA1 0x$(if $(PRJ_GIT_SHA1),$(PRJ_GIT_SHA1),0)" >> $@; fi

version: $(PRJ_VERSION_H)
	@cat $^

version.clean:
	-@rm -f $(TOPDIR)/include/version.h

PHONY += FORCE

.PHONY: $(PHONY)
