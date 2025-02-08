_all: all

-include $(TOPDIR)/include/config/auto.conf
include $(TOPDIR)/scripts/toolchain.mk

PHONY := _all all FORCE

define export_macros
$(foreach m,$(1),$(eval export $(m)))
endef

FORCE:

define build_sub_targets
all: $(SUB_TARGET-y) $(SUB_TARGET-m) FORCE

CLEAN_TARGETS := $(addsuffix /clean,$(SUB_TARGETS))
INSTALL_TARGETS := $(addsuffix /install,$(SUB_TARGET-y) $(SUB_TARGET-m))

clean: $$(CLEAN_TARGETS)

install: $$(INSTALL_TARGETS)

$$(CLEAN_TARGETS) $$(INSTALL_TARGETS): FORCE
	$$(SILENT)install -d $$(TARGET_STAG)
	$$(SILENT)$$(MAKE) -C $$(shell echo $$@ | sed 's,/, ,')
endef

.PHONY: $(PHONY)
