SRC_DIR +=

$(foreach t,$(SRC_DIR),$(eval BUILD_TARGETS-$(CFG_$(t)) += $(t)))

