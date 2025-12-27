/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    io.c

Abstract:

    I/O management services.

--*/

#include "io.h"

#define MAX_ROUTINE_ENTRIES 4

ULONG IoMgrRoutineEntries;
PVOID *IoMgrDestroyRoutineTable;

NTSTATUS
BlpIoDestroy (
    VOID
    )

/*++

Routine Description:

    Destroys the I/O manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    The most recent error code if unsuccessful.

--*/

{
    NTSTATUS Status, ReturnStatus;

#if !defined(NDEBUG)
    DebugInfo(L"Destroying I/O manager...\r\n");
#endif
    ReturnStatus = STATUS_SUCCESS;

    //
    // Destroy file manager.
    //
    Status = BlpFileDestroy();
    if (!NT_SUCCESS(Status)) {
        ReturnStatus = Status;
    }

    //
    // Destroy device manager.
    //
    Status = BlpDeviceDestroy();
    if (!NT_SUCCESS(Status)) {
        ReturnStatus = Status;
    }

    //
    // TODO: Finish implementing this routine.
    //

    //
    // Free destroy routine table.
    //
    Status = BlMmFreeHeap(IoMgrDestroyRoutineTable);
    if (!NT_SUCCESS(Status)) {
        ReturnStatus = Status;
    }

    return ReturnStatus;
}

NTSTATUS
BlpIoInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the I/O manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NO_MEMORY if a memory allocation fails.

    Any other status value returned by BlpDeviceInitialize or BlpFileInitialize.

--*/

{
    NTSTATUS Status;
    ULONG DestroyRoutineTableSize;

#if !defined(NDEBUG)
    DebugInfo(L"Initializing I/O manager...\r\n");
#endif
    //
    // Initialize destroy routine table.
    //
    IoMgrRoutineEntries = MAX_ROUTINE_ENTRIES;
    DestroyRoutineTableSize = sizeof(*IoMgrDestroyRoutineTable) * IoMgrRoutineEntries;
    IoMgrDestroyRoutineTable = BlMmAllocateHeap(DestroyRoutineTableSize);
    if (IoMgrDestroyRoutineTable == NULL) {
        return STATUS_NO_MEMORY;
    }
    RtlZeroMemory(IoMgrDestroyRoutineTable, DestroyRoutineTableSize);

    //
    // Initialize device manager.
    //
    Status = BlpDeviceInitialize();
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    //
    // Initialize file manager.
    //
    Status = BlpFileInitialize();
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    return STATUS_SUCCESS;
}
