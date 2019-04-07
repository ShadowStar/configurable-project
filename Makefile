_all: all

PHONY := _all all
SHELL := /bin/bash
TOPDIR := $(CURDIR)
DOT_CFG := $(TOPDIR)/.config
STAGING_DIR := $(TOPDIR)/staging
PATH := $(STAGING_DIR)/bin:$(PATH)
SRC_DIR :=

-include $(TOPDIR)/include/config/auto.conf

ifneq ($(GNUPATH),)
  PATH := $(GNUPATH):$(PATH)
endif
CT_PATH := $(strip $(subst ",,$(CFG_CROSS_TOOLCHAIN_PATH)))
ifneq (x$(CT_PATH),x)
  PATH := $(CT_PATH):$(PATH)
endif

DEST_DIR := $(strip $(subst ",,$(CFG_DEST_DIR)))
ifeq (x$(DEST_DIR),x)
  DEST_DIR := $(TOPDIR)/bin
endif

export SHELL PATH TOPDIR STAGING_DIR DEST_DIR

include scripts/verbose.mk

no-dot-cfg-targets := %clean tags%

$(STAGING_DIR)/.PrePared: prepare
	$(SILENT)touch $(STAGING_DIR)/.PrePared

include scripts/pre-cmds.mk
prepare: $(pre-cmd-list)

include Project.mk

define TARGETS
$(1)/%: FORCE
	$$(SILENT)$$(MAKE) -C $$(@D) $$(@F)
endef

$(foreach t,$(SRC_DIR),$(eval $(call TARGETS,$(t))))

$(BUILD_TARGETS-y) $(BUILD_TARGETS-m): FORCE
	$(SILENT)$(MAKE) -C $@ all

clean: $(addsuffix /clean,$(SRC_DIR))

distclean: clean
	-$(SILENT)rm -rf $(STAGING_DIR)
	-$(SILENT)rm -f .config*

include scripts/version.mk
include scripts/kconfig.mk

$(configurators): $(STAGING_DIR)/.PrePared

include scripts/tags-gen.mk
CTAGS := exuberant-ctags
IGNORE_LIST += $(TOPDIR)/include/config $(TOPDIR)/scripts $(STAGING_DIR) $(DEST_DIR)

all: $(BUILD_TARGETS-y) $(BUILD_TARGETS-m)

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

