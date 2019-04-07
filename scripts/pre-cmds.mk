pre-cmds := find xargs md5sum cp stat grep egrep fgrep lzma sed awk

define PreCmd
$(STAGING_DIR)/bin/$(1):
	$(SILENT)mkdir -p "$$(dir $$@)"; rm -f "$$@"
	$(SILENT)export FILE="$$$$(which $(2) 2>/dev/null | grep -v 'not found' | head -n1)"; \
		[ -n "$$$$FILE" ] || { echo "Command $(1) not found."; false; }; \
		ln -s "$$$$FILE" "$$@"

endef

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

pre-cmd-list := $(patsubst %,$(STAGING_DIR)/bin/%,$(pre-cmds))
