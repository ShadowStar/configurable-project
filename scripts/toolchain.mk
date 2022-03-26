include $(TOPDIR)/include/config/auto.conf

TOOLCHAIN_UTILS := ar as gcc cpp ld nm objcopy objdump ranlib strip
TOOLCHAIN_UTILS += llvm-ar llvm-as clang clang-cpp llvm-nm llvm-objcopy llvm-objdump llvm-ranlib llvm-strip

_CROSS := $(shell echo $(CFG_CROSS_TOOLCHAIN))

define _str
$(shell echo $(1))
endef

define _upper
$(shell echo $(1) | tr "[:lower:]" "[:upper:]")
endef

$(foreach t,$(TOOLCHAIN_UTILS),$(eval export $(call _upper,$(t)) := $(_CROSS)$(t)))
export CC := $(GCC)

CFLAGS := $(shell echo $(CFG_TARGET_CFLAGS))

ifneq ($(call _str,$(CFG_SYSROOT_PATH)),)
  CFLAGS += -isysroot $(CFG_SYSROOT_PATH)
endif

ifneq ($(call _str,$(CFG_TOOLCHAIN_PATH)),)
  _TC_PATH := $(patsubst %/bin,%,$(call _str,$(CFG_TOOLCHAIN_PATH)))
  SHELL := /bin/sh -c
  PATH := $(_TC_PATH)/bin:$(PATH)
  export SHELL PATH
endif

export AR AS CC CPP GCC CLANG LD NM OBJCOPY OBJDUMP RANLIB STRIP CFLAGS

ifeq ($(CFG_TARGET_ARCH),"")
  CFG_TARGET_ARCH=$(shell $(GCC) -dumpmachine | cut -d'-' -f 1)
endif

