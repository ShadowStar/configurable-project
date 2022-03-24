_all: all

include $(TOPDIR)/include/config/auto.conf
include $(TOPDIR)/scripts/toolchain.mk

PHONY := _all all FORCE

define export_macros
$(foreach m,$(1),$(eval export $(m)))
endef

FORCE:

define build_sub_targets
all: $(SUB_TARGET-y) $(SUB_TARGET-m) FORCE

clean: $(addsuffix /clean,$(SUB_DIR))

install: $(addsuffix /install,$(SUB_DIR))

$(addsuffix /clean,$(SUB_DIR)): FORCE
	$$(SILENT)$$(MAKE) -C $$(shell echo $$@ | sed 's,/, ,')

$(addsuffix /install,$(SUB_DIR)): FORCE
	$$(SILENT)$$(MAKE) -C $$(shell echo $$@ | sed 's,/, ,')
endef

.PHONY: $(PHONY)
