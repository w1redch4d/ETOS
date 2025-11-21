/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    io.h

Abstract:

    Provides I/O manager definitions.

--*/

#pragma once

#ifndef _IO_H
#define _IO_H

#include "bootlib.h"

//
// Device I/O information.
//
typedef struct {
    ULONGLONG ReadCount;
    ULONGLONG WriteCount;
} DEVICE_IO_INFORMATION, *PDEVICE_IO_INFORMATION;

//
// Filesystem callbacks.
//

typedef
NTSTATUS
(*PFS_INITIALIZE_CALLBACK) (
    VOID
    );

typedef
NTSTATUS
(*PFS_DESTROY_CALLBACK) (
    VOID
    );

typedef
NTSTATUS
(*PFS_MOUNT_CALLBACK) (
    IN  ULONG DeviceId,
    IN  ULONG Unknown,
    OUT PVOID *FileEntry
    );

typedef
NTSTATUS
(*PFS_PURGE_CALLBACK) (
    VOID
    );

//
// Filesystem table entry.
//

typedef struct {
    PFS_INITIALIZE_CALLBACK Initialize;
    PFS_DESTROY_CALLBACK    Destroy;
    PFS_MOUNT_CALLBACK      Mount;
    PFS_PURGE_CALLBACK      Purge;
} FS_REGISTER_FUNCTION_TABLE, *PFS_REGISTER_FUNCTION_TABLE;

//
// Filesystem registry entry.
//

typedef struct {
    LIST_ENTRY              ListEntry;
    PFS_INITIALIZE_CALLBACK Initialize;
    PFS_DESTROY_CALLBACK    Destroy;
    PFS_MOUNT_CALLBACK      Mount;
    PFS_PURGE_CALLBACK      Purge;
} FS_REGISTRY_ENTRY, *PFS_REGISTRY_ENTRY;

//
// Device management services.
//

NTSTATUS
BlpDeviceInitialize (
    VOID
    );

NTSTATUS
BlpDeviceDestroy (
    VOID
    );

//
// File management services.
//

NTSTATUS
BlpFileInitialize (
    VOID
    );

NTSTATUS
BlpFileDestroy (
    VOID
    );

#endif /* !_IO_H */
