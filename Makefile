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
include scripts/kconfig.mk
include scripts/version.mk

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

