/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bootmgr.h

Abstract:

    Provides boot manager definitions.

--*/

#pragma once

#ifndef _BOOTMGR_H
#define _BOOTMGR_H

#include "bootlib.h"

//
// Fiwmare services.
//

PWSTR
BmFwInitializeBootDirectoryPath (
    VOID
    );

NTSTATUS
BmpFwGetFullPath (
    IN  PWSTR PartialPath,
    OUT PWSTR *FullPath
    );

//
// BCD services.
//

NTSTATUS
BmGetDataStorePath (
    OUT PDEVICE_IDENTIFIER *DeviceIdentifier,
    OUT PWSTR              *FilePath,
    OUT PBOOLEAN           FilePathFound
    );

NTSTATUS
BmOpenDataStore (
    PHANDLE DataStoreHandle
    );

NTSTATUS
BmCloseDataStore (
    HANDLE DataStoreHandle
    );

//
// Boot entry services.
//

NTSTATUS
BmTransferExecution (
    IN  PBOOT_APPLICATION_ENTRY BootEntry,
    OUT PULONG                  LaunchCode,
    OUT PBOOLEAN                CanRecover
    );

NTSTATUS
BmpLaunchBootEntry (
    IN  PBOOT_APPLICATION_ENTRY BootEntry,
    OUT PULONG                  EntryIndex,
    IN  ULONG                   LaunchCode,
    IN  BOOLEAN                 AllowRecovery
    );

//
// Entry point.
//
NTSTATUS
BmMain (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters
    );

#endif /* !_BOOTMGR_H */
