# ===========================================================================
# crosstool-NG configuration targets
# These targets are used from top-level makefile

#-----------------------------------------------------------
# The configurators rules
configurators = menuconfig nconfig oldconfig savedefconfig defconfig
PHONY += $(configurators)

# We need CONF for savedefconfig in scripts/saveSample.sh
export CONF  := $(KCONFIG_DIR)/conf
MCONF := $(KCONFIG_DIR)/mconf
NCONF := $(KCONFIG_DIR)/nconf

$(CONF) $(MCONF) $(NCONF):
	@$(MAKE) -C $(KCONFIG_DIR) PATH=$(PATH) $(@F)

menuconfig: $(MCONF)
	@$(ECHO) "  CONF  $(KCONFIG_TOP)"
	$(SILENT)$(MCONF) $(KCONFIG_TOP)

nconfig: $(NCONF)
	@$(ECHO) "  CONF  $(KCONFIG_TOP)"
	$(SILENT)$(NCONF) $(KCONFIG_TOP)

oldconfig: .config $(CONF)
	@$(ECHO) "  CONF  $(KCONFIG_TOP)"
	$(SILENT)$(CONF) --silent$@ $(KCONFIG_TOP)

savedefconfig: .config $(CONF)
	@$(ECHO) '  GEN   $@'
	$(SILENT)$(CONF) --savedefconfig=$${DEFCONFIG-defconfig} $(KCONFIG_TOP)

defconfig: $(CONF)
	@$(ECHO) '  CONF  $@'
	$(SILENT)$(CONF) --defconfig=$${DEFCONFIG-defconfig} $(KCONFIG_TOP)

# Always be silent, the stdout an be >.config
extractconfig:
	@$(awk) 'BEGIN { dump=0; }                                                  \
	         dump==1 && $$0~/^\[.....\][[:space:]]+(# )?CFG_/ {                 \
	             $$1="";                                                        \
	             gsub("^[[:space:]]","");                                       \
	             print;                                                         \
	         }                                                                  \
	         $$0~/Dumping user-supplied Project configuration: done in/ {  \
	             dump=0;                                                        \
	         }                                                                  \
	         $$0~/Dumping user-supplied Project configuration$$/ {         \
	             dump=1;                                                        \
	         }'

#-----------------------------------------------------------
# Help text used by make help

help-config::
	@echo  '  menuconfig         - Update current config using a menu based program'
	@echo  '  nconfig            - Update current config using a menu based program'
	@echo  '  oldconfig          - Update current config using a provided .config as base'
	@echo  '  extractconfig      - Extract to stdout the configuration items from a'
	@echo  '                       build.log file piped to stdin'
	@echo  '  savedefconfig      - Save current config as a mini-defconfig to $${DEFCONFIG}'
	@echo  '  defconfig          - Update config from a mini-defconfig $${DEFCONFIG}'
	@echo  '                       (default: $${DEFCONFIG}=./defconfig)'
