_all: prepare all

PHONY := _all all
SHELL := /bin/bash
TOPDIR := $(CURDIR)
DOT_CFG := $(TOPDIR)/.config
STAGING_DIR := $(TOPDIR)/staging
PATH := $(STAGING_DIR)/bin:$(PATH)
SRC_DIR :=

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

all:

$(STAGING_DIR)/.PrePared: prepare
	@touch $(STAGING_DIR)/.PrePared

include scripts/pre-cmds.mk
prepare: $(pre-cmd-list)

clean:
	$(foreach m, $(addsuffix /clean,$(SRC_DIR)), $(MAKE) $(m);)

distclean: clean
	-@rm -rf $(STAGING_DIR)
	-@rm -f .config*

include scripts/tags-gen.mk
CTAGS := exuberant-ctags
IGNORE_LIST += $(TOPDIR)/include/config $(TOPDIR)/scripts $(STAGING_DIR) $(DEST_DIR)

define TARGETS
$(1)/%: FORCE
	$$(MAKE) -C $$(@D) $$(@F)
endef

$(foreach t,$(SRC_DIR),$(eval $(call TARGETS,$(t))))

$(BUILD_TARGETS-y) $(BUILD_TARGETS-m): FORCE
	$(SILENT)$(MAKE) -C $@ all

all: $(BUILD_TARGETS-y) $(BUILD_TARGETS-m)

include scripts/version.mk
include scripts/kconfig.mk

$(configurators): $(STAGING_DIR)/.PrePared

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

