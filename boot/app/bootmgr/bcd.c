/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bcd.c

Abstract:

    Boot manager BCD services.

--*/

#include "bootmgr.h"
#include <wchar.h>

NTSTATUS
BcdOpenStoreFromFile (
    IN  PUNICODE_STRING Path,
    OUT PHANDLE         DataStoreHandle
    )

/*++

Routine Description:

    Opens the boot data store (aka BCD).

Arguments:

    DataStoreHandle - Pointer to a HANDLE that receives the data store handle.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NO_MEMORY if memory allocation fails.

--*/

{
    PFILE_IDENTIFIER FileIdentifier;
    ULONG TotalSize;

    (VOID) DataStoreHandle;

    //
    // Allocate buffer for file identifier.
    //
    TotalSize = sizeof(*FileIdentifier) + Path->Length;
    FileIdentifier = BlMmAllocateHeap(TotalSize);
    if (FileIdentifier == NULL) {
        return STATUS_NO_MEMORY;
    }

    //
    // Set up file identifier with path.
    //
    FileIdentifier->Version = FILE_IDENTIFIER_VERSION;
    FileIdentifier->Length = TotalSize;
    FileIdentifier->PathType = FILE_PATH_TYPE_INTERNAL;
    RtlMoveMemory(FileIdentifier->Path, Path->Buffer, Path->Length);
    FileIdentifier->Path[Path->Length / sizeof(WCHAR)] = UNICODE_NULL;

    //
    // TODO: Finish implementing this routine.
    //

    BlMmFreeHeap(FileIdentifier);
    return STATUS_SUCCESS;
}

NTSTATUS
BmGetDataStorePath (
    OUT PDEVICE_IDENTIFIER *DeviceIdentifierOut,
    OUT PWSTR              *FilePathOut,
    OUT PBOOLEAN           FilePathFoundOut
    )

/*++

Routine Description:

    Finds the containing device and file path of the BCD.

    If the device is found but the file path is not, BmGetDataStorePath will set
    DeviceIdentifier, but return an error code and set FilePathFound to FALSE.

Arguments:

    DeviceIdentifier - Receives a pointer to the device identifier.

    FilePath - Receives a pointer to the file path.

    FilePathFound - Receives TRUE if the file path was found, FALSE otherwise.

Return Value:

    STATUS_SUCCESS if both the device and file path were found.

    STATUS_NO_MEMORY if memory allocation fails.

    STATUS_NOT_IMPLEMENTED if the BCD resides on a network device.

    Any other error code returned by subroutines.

--*/

{
    NTSTATUS Status;
    PDEVICE_IDENTIFIER DeviceIdentifier, BootDeviceIdentifier;
    PWSTR FilePath;
    BOOLEAN FilePathFound;

    //
    // Initialize state.
    //
    FilePathFound = FALSE;

    //
    // Use the specified device or the boot device.
    //
    Status = BlGetBootOptionDevice(BlpApplicationEntry.Options, BCDE_BOOTMGR_TYPE_BCD_DEVICE, &DeviceIdentifier, NULL);
    if (!NT_SUCCESS(Status)) {
        BootDeviceIdentifier = BlpBootDevice;
        DeviceIdentifier = BlMmAllocateHeap(BootDeviceIdentifier->Size);
        if (DeviceIdentifier == NULL) {
            return STATUS_NO_MEMORY;
        }
        RtlMoveMemory(DeviceIdentifier, BootDeviceIdentifier, BootDeviceIdentifier->Size);
    }

    //
    // Use the specified path if possible.
    //
    Status = BlGetBootOptionString(BlpApplicationEntry.Options, BCDE_BOOTMGR_TYPE_BCD_FILE_PATH, &FilePath);
    if (NT_SUCCESS(Status)) {
        FilePathFound = TRUE;
        goto Success;
    }

    //
    // Check what kind of device the BCD is on.
    //
    if (DeviceIdentifier->Type == DEVICE_TYPE_NETWORK) {
        //
        // TODO: Implement network device support.
        //

        BlMmFreeHeap(DeviceIdentifier);
        return STATUS_NOT_IMPLEMENTED;
    } else {
        //
        // Get the full default path of the BCD.
        //
        Status = BmpFwGetFullPath(L"\\BCD", &FilePath);
        if (NT_SUCCESS(Status)) {
            FilePathFound = TRUE;
            goto Success;
        }

        BlMmFreeHeap(DeviceIdentifier);
        return Status;
    }

Success:
    //
    // Return results.
    //
    *DeviceIdentifierOut = DeviceIdentifier;
    *FilePathOut = FilePath;
    *FilePathFoundOut = FilePathFound;
    return STATUS_SUCCESS;
}

NTSTATUS
BmOpenDataStore (
    PHANDLE DataStoreHandle
    )

/*++

Routine Description:

    Opens the boot data store (aka BCD).

Arguments:

    DataStoreHandle - Pointer to a HANDLE that receives the data store handle.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

    STATUS_NO_MEMORY if memory allocation fails.

    Any other error code returned by BcdOpenStoreFromFile.

--*/

{
    NTSTATUS Status;
    PDEVICE_IDENTIFIER DeviceIdentifier;
    PWSTR FilePath;
    BOOLEAN FilePathFound;
    ULONG FilePathLength, TotalSize;
    size_t FilePathSize;
    PVOID Buffer;
    UNICODE_STRING Path;

    //
    // Get the BCD path.
    //
    DeviceIdentifier = NULL;
    FilePath = NULL;
    FilePathFound = FALSE;
    Status = BmGetDataStorePath(&DeviceIdentifier, &FilePath, &FilePathFound);
    if (!NT_SUCCESS(Status)) {
        goto Exit;
    }

    //
    // Find length of file path.
    //
    FilePathLength = wcslen(FilePath);
    if ((FilePathLength + 1) < FilePathLength) {
        Status = STATUS_INTEGER_OVERFLOW;
        goto Exit;
    }

    //
    // Calculate size of file path.
    //
    FilePathSize = (FilePathLength * sizeof(WCHAR)) + sizeof(UNICODE_NULL);
    if (FilePathSize > MAXULONG) {
        Status = STATUS_INTEGER_OVERFLOW;
        goto Exit;
    }

    //
    // Calculate total size of device identifier and file path.
    //
    TotalSize = DeviceIdentifier->Size + (ULONG)FilePathSize;
    if (TotalSize < DeviceIdentifier->Size) {
        Status = STATUS_INTEGER_OVERFLOW;
        goto Exit;
    }

    //
    // Copy the device identifier and file path.
    //
    Buffer = BlMmAllocateHeap(TotalSize);
    if (Buffer == NULL) {
        Status = STATUS_NO_MEMORY;
        goto Exit;
    }
    RtlMoveMemory(Buffer, DeviceIdentifier, DeviceIdentifier->Size);
    RtlMoveMemory((PVOID)((ULONG_PTR)Buffer + DeviceIdentifier->Size), FilePath, FilePathSize);

    //
    // Open the BCD.
    //
    Path.Length = TotalSize;
    Path.MaximumLength = TotalSize;
    Path.Buffer = Buffer;
    Status = BcdOpenStoreFromFile(&Path, DataStoreHandle);
    BlMmFreeHeap(Buffer);

Exit:
    //
    // Free allocated memory.
    //

    if (FilePath != NULL && FilePathFound) {
        BlMmFreeHeap(FilePath);
    }

    if (DeviceIdentifier != NULL) {
        BlMmFreeHeap(DeviceIdentifier);
    }

    return Status;
}
