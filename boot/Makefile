# Boot application build configuration.
# Copyright (c) 2025, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

APP ?= bootmgr
BUILDDIR ?= build/$(APP)
CFLAGS += -Iinc -I../sdk/inc/nt -I../sdk/inc/crt
CFILES = lib/bootopt.c
ASFILES =
LIBS = $(BUILDDIR)/../crt/crt.lib $(BUILDDIR)/../rtl/rtl.lib

ifeq ($(APP),bootmgr)
CFILES += app/bootmgr/bootmgr.c
endif

ifeq ($(TARGET_ARCH),x64)
ASFILES += lib/x64/archasm.S
endif

ifeq ($(TARGET_FIRMWARE),efi)
CFLAGS += -I../sdk/inc/efi -D_EFI
LDFLAGS += /SUBSYSTEM:EFI_APPLICATION /ENTRY:EfiEntry
CFILES += $(addprefix lib/efi/,eficon.c efidebug.c efierr.c efiwrap.c)
ifeq ($(APP),bootmgr)
CFILES += app/bootmgr/efi/efientry.c
EXEFILE = $(BUILDDIR)/bootmgfw.efi
endif
endif

OFILES = $(patsubst %.c,$(BUILDDIR)/%.obj,$(CFILES)) $(patsubst %.S,$(BUILDDIR)/%.obj,$(ASFILES))

.PHONY: all
all: $(BUILDDIR) $(EXEFILE)

$(EXEFILE): $(OFILES)
	@echo  Linking $@...
	@$(LD) $(LIBS) $^ $(LDFLAGS) /OUT:$@

$(BUILDDIR)/%.obj: %.c
	@echo Compiling $<...
	@mkdir -p "$(BUILDDIR)/$(dir $<)"
	@$(CC) -c "$<" $(CFLAGS) -o "$@"

$(BUILDDIR)/%.obj: %.S
	@echo Assembling $<...
	@mkdir -p "$(BUILDDIR)/$(dir $<)"
	@$(CC) -c "$<" $(CFLAGS) -o "$@"

$(BUILDDIR):
	@mkdir -p "$@"

.PHONY: clean
clean:
	@echo Cleaning $(APP)...
	@rm -f $(OFILES)
