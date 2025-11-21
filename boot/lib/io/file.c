/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    file.c

Abstract:

    File management services.

--*/

#include "io.h"

#define MAX_FILE_ENTRIES 16

extern FS_REGISTER_FUNCTION_TABLE FatRegisterFunctionTable;

LIST_ENTRY RegisteredFileSystems;
PVOID *FileTable;
ULONG FileEntries;

PFS_REGISTER_FUNCTION_TABLE FsTable[] = {
    &FatRegisterFunctionTable,
    NULL
};

NTSTATUS
BlpFileDestroy (
    VOID
    )

/*++

Routine Description:

    Destroys the file manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    The most recent error code if unsuccessful.

--*/

{
    NTSTATUS Status, ReturnStatus;
    PFS_REGISTRY_ENTRY RegistryEntry, NextRegistryEntry;

    DebugInfo(L"Destroying file manager...\r\n");

    //
    // TODO: Finish implementing this routine.
    //

    //
    // Freefile table.
    //
    BlMmFreeHeap(FileTable);
    FileTable = NULL;
    FileEntries = 0;

    //
    // Unregister file systems.
    //
    ReturnStatus = STATUS_SUCCESS;
    RegistryEntry = CONTAINING_RECORD(RegisteredFileSystems.Flink, FS_REGISTRY_ENTRY, ListEntry);
    while (&RegistryEntry->ListEntry != &RegisteredFileSystems) {
        //
        // Call  destruction callback.
        //
        Status = RegistryEntry->Destroy();
        if (!NT_SUCCESS(Status)) {
            ReturnStatus = Status;
        }

        //
        // Remove from registry list and free data.
        //
        NextRegistryEntry = CONTAINING_RECORD(RegistryEntry->ListEntry.Flink, FS_REGISTRY_ENTRY, ListEntry);
        RemoveEntryList(&RegistryEntry->ListEntry);
        BlMmFreeHeap(RegistryEntry);
        RegistryEntry = NextRegistryEntry;
    }

    return ReturnStatus;
}

NTSTATUS
BlpFileInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the file manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NO_MEMORY if a memory allocation fails.

--*/

{
    NTSTATUS Status;
    ULONG FileTableSize;
    PFS_REGISTER_FUNCTION_TABLE RegisterFunctionTable;
    PFS_REGISTRY_ENTRY RegistryEntry;

    DebugInfo(L"Initializing file manager...\r\n");

    //
    // Initialize file table.
    //
    FileEntries = MAX_FILE_ENTRIES;
    FileTableSize = sizeof(*FileTable) * FileEntries;
    FileTable = BlMmAllocateHeap(FileTableSize);
    if (FileTable == NULL) {
        return STATUS_NO_MEMORY;
    }
    RtlZeroMemory(FileTable, FileTableSize);

    //
    // Register filesystems.
    //
    InitializeListHead(&RegisteredFileSystems);
    for (ULONG Index = 0; FsTable[Index] != NULL; Index++) {
        RegisterFunctionTable = FsTable[Index];

        //
        // Allocate registry entry.
        //
        RegistryEntry = BlMmAllocateHeap(sizeof(*RegistryEntry));
        if (RegistryEntry == NULL) {
            BlpFileDestroy();
            return STATUS_NO_MEMORY;
        }

        //
        // Call initialization callback.
        //
        Status = RegisterFunctionTable->Initialize();
        if (!NT_SUCCESS(Status)) {
            BlMmFreeHeap(RegistryEntry);
            BlpFileDestroy();
            return Status;
        }

        //
        // Copy callback pointers and add to registry list.
        //
        RegistryEntry->Initialize = RegisterFunctionTable->Initialize;
        RegistryEntry->Destroy = RegisterFunctionTable->Destroy;
        RegistryEntry->Mount = RegisterFunctionTable->Mount;
        RegistryEntry->Purge = RegisterFunctionTable->Purge;
        InsertTailList(&RegisteredFileSystems, &RegistryEntry->ListEntry);
    }

    return STATUS_SUCCESS;
}
