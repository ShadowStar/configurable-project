_all: all

-include $(TOPDIR)/include/config/auto.conf

PHONY := _all all FORCE

AR := $(CFG_CROSS_TOOLCHAIN)ar
AS := $(CFG_CROSS_TOOLCHAIN)as
CC := $(CFG_CROSS_TOOLCHAIN)gcc
CPP := $(CFG_CROSS_TOOLCHAIN)cpp
GCC := $(CFG_CROSS_TOOLCHAIN)gcc
LD := $(CFG_CROSS_TOOLCHAIN)ld
NM := $(CFG_CROSS_TOOLCHAIN)nm
OBJCOPY := $(CFG_CROSS_TOOLCHAIN)objcopy
OBJDUMP := $(CFG_CROSS_TOOLCHAIN)objdump
RANLIB := $(CFG_CROSS_TOOLCHAIN)ranlib
STRIP := $(CFG_CROSS_TOOLCHAIN)strip

CFLAGS := $(CFG_TARGET_CFLAGS) -MD -Wall -Wextra -Wunused -Wshadow -O2

ifeq ($(CFG_TARGET_ARCH),"")
	CFG_TARGET_ARCH=$(shell $(GCC) -dumpmachine | cut -d'-' -f 1)
endif

define export_macros
$(foreach m,$(1),$(eval export $(m)))
endef

FORCE:

define build_sub_targets
all: $(SUB_TARGET-y) $(SUB_TARGET-m) FORCE

clean: $(addsuffix /clean,$(SUB_DIR))

$(addsuffix /clean,$(SUB_DIR)): FORCE
	$$(MAKE) -C $$(shell echo $$@ | sed 's,/, ,')
endef

.PHONY: $(PHONY)
