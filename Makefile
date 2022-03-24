_all: all

PHONY := _all all
SHELL := /bin/sh -c
TOPDIR := $(CURDIR)
DOT_CFG := $(TOPDIR)/.config
STAGING_DIR := $(TOPDIR)/staging
PATH := $(STAGING_DIR)/bin:$(PATH)
DEST_DIR := $(CFG_DEST_DIR)
BUILD_DIR := $(patsubst %/,%,$(sort $(dir $(wildcard */Makefile))))

ifeq (x$(DEST_DIR),x)
  DEST_DIR := $(TOPDIR)/bin
endif
ifneq ($(GNUPATH),)
  PATH := $(GNUPATH):$(PATH)
endif

export SHELL PATH TOPDIR STAGING_DIR DEST_DIR

include scripts/verbose.mk
include scripts/version.mk
include scripts/kconfig.mk

no-dot-cfg-targets := %clean tags%

all:

$(configurators): $(STAGING_DIR)/.PrePared

$(STAGING_DIR)/.PrePared: prepare
	@touch $(STAGING_DIR)/.PrePared
	@install -d $(DEST_DIR)

include scripts/pre-cmds.mk
prepare: $(pre-cmd-list)

clean: kconfig.clean version.clean
	$(foreach m, $(addsuffix /clean,$(BUILD_DIR)), $(MAKE) $(m);)

distclean: kconfig.distclean tags.clean clean
	-@rm -rf $(STAGING_DIR)
	-@rm -f .config*

include scripts/tags-gen.mk
CTAGS := exuberant-ctags
IGNORE_LIST += include/config

define TARGETS
$(1)/%: FORCE
	$$(SILENT)PATH=$$(PATH) $$(MAKE) -C $$(firstword $$(subst /, ,$$@)) $$(subst $$(firstword $$(subst /, ,$$@))/,,$$@)
endef

$(foreach t,$(BUILD_DIR),$(eval $(call TARGETS,$(t))))
$(foreach t,$(BUILD_DIR),$(eval BUILD_TARGETS-$(CFG_$(t)) += $(t)))

$(BUILD_TARGETS-y) $(BUILD_TARGETS-m): FORCE
	$(SILENT)PATH=$(PATH) $(MAKE) -C $@

all: $(BUILD_TARGETS-y) $(BUILD_TARGETS-m)

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

