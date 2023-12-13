TAGS_DIR := $(BUILD_DIR) include
IGNORE_LIST :=
FIND ?= find
CTAGS ?= ctags
CSCOPE ?= cscope

prune_files = $(if $(IGNORE_LIST),\( -path $(firstword $(IGNORE_LIST)) \
		$(addprefix -o -path ,$(wordlist 2,$(words $(IGNORE_LIST)),$(IGNORE_LIST))) \) -prune -o,)

define files-list
	$(FIND) $(TAGS_DIR) $(prune_files) \( -name "*.[chS]" -o -name "*.[ch]pp" \) -type f -print
endef

define tags-gen
	@echo ">>> Building Source Files List" >&2
	@$(files-list) > cscope.files
	@echo ">>> Building Cscope Database" >&2
	@$(CSCOPE) -bqk -i cscope.files
	@echo ">>> Building Ctags Database" >&2
	@$(CTAGS) --c-kinds=+px --fields=+iaS --extra=+q -L cscope.files
endef

tags:
	$(tags-gen)

tags.clean:
	@-rm -f cscope.* tags

.PHONY: tags

