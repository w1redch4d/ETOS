# CRT (C Run-Time) build configuration.
# Copyright (c) 2025, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

BUILDDIR ?= build
CFLAGS += -I../inc/crt
CFILES = stdio/wprintf.c string/mem.c string/str.c string/wmem.c string/wstr.c
LIBFILE = $(BUILDDIR)/crt.lib

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
	@echo Cleaning CRT...
	@rm -f $(OFILES)
