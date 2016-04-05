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
