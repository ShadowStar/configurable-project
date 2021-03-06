TAGS_DIR ?= $(TOPDIR)
IGNORE_LIST :=
FIND ?= find
CTAGS ?= ctags
CSCOPE ?= cscope

ignore_files = $(subst $(TAGS_DIR)/,,$(IGNORE_LIST))
prune_files = $(if $(ignore_files),-path $(TAGS_DIR)/$(firstword $(ignore_files)) \
		$(addprefix -o -path $(TAGS_DIR)/,\
		  $(wordlist 2,$(words $(ignore_files)),$(ignore_files))),\
		-path x$(TAGS_DIR))

define files-list
	$(FIND) $(TAGS_DIR) \( $(prune_files) \) -prune -o \
		\( -name "*.[chS]" -o -name "*.[ch]pp" \) -type f -print
endef

define tags-gen
	$(SILENT)echo ">>> Building Source Files List" >&2
	$(SILENT)$(files-list) > cscope.files
	$(SILENT)echo ">>> Building Cscope Database" >&2
	$(SILENT)$(CSCOPE) -bqk -i cscope.files
	$(SILENT)echo ">>> Building Ctags Database" >&2
	$(SILENT)$(CTAGS) --c-kinds=+px --fields=+iaS --extra=+q -L cscope.files
endef

tags:
	$(tags-gen)

tags.clean:
	-$(SILENT)rm -f cscope.* tags

clean: tags.clean

.PHONY: tags

