_all: prepare all

PHONY := _all all $(wildcard *)
SHELL := /bin/bash
TOPDIR := $(CURDIR)
DOT_CFG := $(TOPDIR)/.config
STAGING_DIR := $(TOPDIR)/staging
PATH := $(STAGING_DIR)/bin:$(PATH)
CROSS_PREFIX := $(CFG_CROSS_TOOLCHAIN)
TARGET_CFLAGS := $(CFG_TARGET_CFLAGS)
ifneq ($(GNUPATH),)
  PATH := $(GNUPATH):$(PATH)
endif
export SHELL PATH TOPDIR CROSS_PREFIX

include scripts/verbose.mk
include scripts/version.mk
include scripts/kconfig.mk

no-dot-cfg-targets := %clean tags%

all: $(TOPDIR)/include/autoconf.h $(TOPDIR)/include/version.h

$(configurators): $(STAGING_DIR)/.PrePared

$(STAGING_DIR)/.PrePared: prepare
	@touch $(STAGING_DIR)/.PrePared

include scripts/pre-cmds.mk
prepare: $(pre-cmd-list)

clean: kconfig.clean version.clean

distclean: kconfig.distclean tags.clean clean
	-@rm -rf $(STAGING_DIR)
	-@rm -f .config*

include scripts/tags-gen.mk
CTAGS := exuberant-ctags
IGNORE_LIST += $(TOPDIR)/include/config

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

