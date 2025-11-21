/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    device.c

Abstract:

    Device management services.

--*/

#include "io.h"

#define MAX_DEVICE_ENTRIES 8

DEVICE_IO_INFORMATION DmDeviceIoInformation;
ULONG DmTableEntries;
LIST_ENTRY DmRegisteredDevices;
PVOID *DmDeviceTable;

NTSTATUS
BlpDeviceDestroy (
    VOID
    )

/*++

Routine Description:

    Destroys the device manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS.

--*/

{
    DebugInfo(L"Destroying device manager...\r\n");

    //
    // TODO: Finish implementing this routine.
    //

    //
    // Free device table.
    //
    BlMmFreeHeap(DmDeviceTable);

    return STATUS_SUCCESS;
}

NTSTATUS
BlpDeviceInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the device manager.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NO_MEMORY if a memory allocation fails.

--*/

{
    ULONG DeviceTableSize;

    DebugInfo(L"Initializing device manager...\r\n");

    //
    // Initialize device table entry count and device list.
    //
    DmTableEntries = MAX_DEVICE_ENTRIES;
    InitializeListHead(&DmRegisteredDevices);

    //
    // Initialize device I/O information.
    //
    DmDeviceIoInformation.ReadCount = 0;
    DmDeviceIoInformation.WriteCount = 0;

    //
    // Initialize device table.
    //
    DeviceTableSize = sizeof(*DmDeviceTable) * DmTableEntries;
    DmDeviceTable = BlMmAllocateHeap(DeviceTableSize);
    if (DmDeviceTable == NULL) {
        return STATUS_NO_MEMORY;
    }
    RtlZeroMemory(DmDeviceTable, DeviceTableSize);

    //
    // Fix boot device.
    //
    // Status = DeviceFixBootDevice();
    // if (!NT_SUCCESS(Status)) {
    //     return Status;
    // }

    //
    // Initialize BitLocker.
    //
    // Status = FvebInitialize();
    // if (!NT_SUCCESS(Status)) {
    //     return Status;
    // }

    //
    // Initialize UWF.
    //
    // Status = UwfInitialize();
    // if (!NT_SUCCESS(Status)) {
    //     return Status;
    // }

    //
    // TODO: Finish implementing this routine.
    //

    return STATUS_SUCCESS;
}
