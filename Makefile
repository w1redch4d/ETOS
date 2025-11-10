# ETOS build configuration.
# Copyright (c) 2025, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export TARGET_FAMILY ?= x86
export TARGET_ARCH ?= x64
export TARGET_FIRMWARE ?= efi

export BUILDDIR ?= $(CURDIR)/build
export CC = clang
export LD = lld-link
export AR = llvm-ar
export CFLAGS = -Wall -Wextra -ffreestanding -fno-stack-protector
export LDFLAGS =

QEMUFLAGS = --enable-kvm 

ifeq ($(TARGET_ARCH),x64)
export CFLAGS += -target x86_64-pc-win32 -mno-red-zone
QEMU = qemu-system-x86_64
ifeq ($(TARGET_FIRMWARE),efi)
QEMUFLAGS += -bios RELEASEX64_OVMF.fd
BOOTMGFW_NAME = bootx64.efi
endif
else ifeq ($(TARGET_ARCH),i386)
export CFLAGS += -target i386-pc-win32
QEMU = qemu-system-i386
ifeq ($(TARGET_FIRMWARE),efi)
QEMUFLAGS += -bios RELEASEIa32_OVMF.fd
BOOTMGFW_NAME = bootia32.efi
endif
endif

ifeq ($(TARGET_FIRMWARE),efi)
QEMUFLAGS += -hda fat:rw:disk
endif

.PHONY: all
all:
	@$(MAKE) -C sdk/crt BUILDDIR=$(BUILDDIR)/crt
	@$(MAKE) -C sdk/rtl BUILDDIR=$(BUILDDIR)/rtl

.PHONY: clean
clean:
	@$(MAKE) -C sdk/rtl clean BUILDDIR=$(BUILDDIR)/rtl
	@$(MAKE) -C sdk/crt clean BUILDDIR=$(BUILDDIR)/crt

.PHONY: run
run:
ifeq ($(TARGET_FIRMWARE),efi)
	@mkdir -p disk/EFI/Boot
	@cp $(BUILDDIR)/bootmgr/bootmgfw.efi disk/EFI/Boot/$(BOOTMGFW_NAME)
endif
	@$(QEMU) $(QEMUFLAGS)
