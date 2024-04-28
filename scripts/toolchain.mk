include $(TOPDIR)/include/config/auto.conf

TOOLCHAIN_UTILS := ar as gcc cpp ld nm objcopy objdump ranlib strip
LLVM_UTILS += llc llvm-ar llvm-as clang clang-cpp llvm-nm llvm-objcopy llvm-objdump llvm-ranlib llvm-strip

_CROSS := $(shell echo $(CFG_CROSS_TOOLCHAIN))

define _str
$(shell echo $(1))
endef

define _upper
$(shell echo $(1) | tr "[:lower:]" "[:upper:]")
endef

$(foreach t,$(TOOLCHAIN_UTILS),$(eval export $(call _upper,$(t)) := $(_CROSS)$(t)))

$(foreach t,$(LLVM_UTILS),$(eval export $(call _upper,$(t)) := $(t)))

ifneq ($(call _str,$(CFG_SYSROOT_PATH)),)
  GCC += --sysroot=$(CFG_SYSROOT_PATH)
  CLANG += -isysroot $(CFG_SYSROOT_PATH)
  export GCC CLANG
endif

ifneq ($(call _str,$(CFG_TOOLCHAIN_PATH)),)
  _TC_PATH := $(patsubst %/bin,%,$(call _str,$(CFG_TOOLCHAIN_PATH)))
  SHELL := /bin/sh -c
  PATH := $(_TC_PATH)/bin:$(PATH)
  export SHELL PATH
endif

ifeq ($(CFG_TARGET_ARCH),"")
  CFG_TARGET_ARCH=$(shell $(GCC) -dumpmachine | cut -d'-' -f 1)
endif

CFG_TARGET=$(shell $(GCC) -dumpmachine)

export CC := $(GCC)
export TARGET_STAG = "$(STAGING_DIR)/$(CFG_TARGET)/"
export CFG_TARGET_ARCH
export CFLAGS := $(shell echo $(CFG_TARGET_CFLAGS)) -I$(TOPDIR)/include -I$(TARGET_STAG)/include -L$(TARGET_STAG)/lib -L$(TARGET_STAG)/lib64 -L$(TARGET_STAG)/lib32
