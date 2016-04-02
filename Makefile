_all: prepare all

PHONY := _all all $(wildcard *)
SHELL := /bin/bash
TOPDIR := $(CURDIR)
KCONFIG_DIR := $(TOPDIR)/scripts/kconfig
KCONFIG_TOP := $(TOPDIR)/Config.in
DOT_CFG := $(TOPDIR)/.config
STAGING_DIR := $(TOPDIR)/staging
PATH := $(STAGING_DIR)/bin:$(PATH)
ifneq ($(GNUPATH),)
PATH := $(GNUPATH):$(PATH)
endif
export SHELL PATH TOPDIR

SILENT=@
ECHO=echo
ifeq ($(strip $(origin V)),command line)
  ifeq ($(strip $(V)),0)
    SILENT=@
    ECHO=:
  else
    ifeq ($(strip $(V)),1)
      SILENT=
      ECHO=:
    else
      ifeq ($(strip $(V)),2)
        SILENT=
        ECHO=echo
      endif # V == 2
    endif # V== 1
  endif # V == 0
endif # origin V
export V SILENT ECHO

VERSION := $(shell $(TOPDIR)/scripts/git-vergen -d $(TOPDIR))
VERSION_FULL := $(shell $(TOPDIR)/scripts/git-vergen -f -d $(TOPDIR))

include/version.h: FORCE
	@echo "#define VERSION \"$(VERSION)\"" > $@
	@echo "#define VERSION_FULL \"$(VERSION_FULL)\"" >> $@

NEED_DOT_CFG := yes
no-dot-cfg-targets := %clean tags% include/version.h info menuconfig

ifneq ($(filter $(no-dot-cfg-targets),$(MAKECMDGOALS)),)
ifeq ($(filter-out $(no-dot-cfg-targets),$(MAKECMDGOALS)),)
NEED_DOT_CFG := no
endif
endif

-include include/config/auto.conf

ifeq ($(NEED_DOT_CFG),yes)
-include include/config/auto.conf.cmd

$(DOT_CFG) include/config/auto.conf.cmd: ;

include/config/auto.conf: $(KCONFIG_TOP) $(DOT_CFG) include/config/auto.conf.cmd $(KCONFIG_DIR)/conf
	@$(KCONFIG_DIR)/conf --silentoldconfig $(KCONFIG_TOP)

include/autoconf.h: include/config/auto.conf
endif

all: include/autoconf.h include/version.h

include scripts/kconfig/kconfig.mk
$(configurators): $(STAGING_DIR)/.PrePared

$(STAGING_DIR)/.PrePared: prepare
	@touch $(STAGING_DIR)/.PrePared

define PreCmd
$(STAGING_DIR)/bin/$(1):
	@mkdir -p "$$(dir $$@)"; rm -f "$$@"
	@export FILE="$$$$(which $(2) 2>/dev/null | grep -v 'not found' | head -n1)"; [ -n "$$$$FILE" ] || { \
		echo "Command $(1) not found."; false; \
	}; ln -s "$$$$FILE" "$$@"

endef

PreCmds := find xargs md5sum cp stat grep egrep fgrep lzma sed awk

$(eval $(call PreCmd,find,gfind find))
$(eval $(call PreCmd,xargs,gxargs xargs))
$(eval $(call PreCmd,md5sum,gmd5sum md5sum))
$(eval $(call PreCmd,cp,gcp cp))
$(eval $(call PreCmd,stat,gstat stat))
$(eval $(call PreCmd,grep,ggrep grep))
$(eval $(call PreCmd,egrep,gegrep egrep))
$(eval $(call PreCmd,fgrep,gfgrep fgrep))
$(eval $(call PreCmd,lzma,lzma-old lzma))
$(eval $(call PreCmd,sed,gsed sed))
$(eval $(call PreCmd,awk,gawk awk))

prepare: $(patsubst %,$(STAGING_DIR)/bin/%,$(PreCmds))

CTAGS := exuberant-ctags
TAGS_DIR := $(TOPDIR)
IGNORE_LIST := $(TOPDIR)/scripts

tags:

tags.all:

-include scripts/tags-gen.mk

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)

