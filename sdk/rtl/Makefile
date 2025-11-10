# RTL (Run-Time Library) build configuration.
# Copyright (c) 2025, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

BUILDDIR ?= build
CFLAGS += -I../inc/crt -I../inc/nt -I../inc/rtl
CFILES = guid.c string.c
LIBFILE = $(BUILDDIR)/rtl.lib

OFILES = $(patsubst %.c,$(BUILDDIR)/%.obj,$(CFILES))

.PHONY: all
all: $(BUILDDIR) $(LIBFILE)

$(LIBFILE): $(OFILES)
	@echo Linking $@...
	@$(AR) -rcs "$@" $^

$(BUILDDIR)/%.obj: %.c
	@echo Compiling $<...
	@mkdir -p "$(BUILDDIR)/$(dir $<)"
	@$(CC) -c "$<" $(CFLAGS) -o "$@"

$(BUILDDIR):
	@mkdir -p "$@"

.PHONY: clean
clean:
	@echo Cleaning RTL...
	@rm -f $(OFILES)
