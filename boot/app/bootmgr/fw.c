/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    fw.c

Abstract:

    Boot manager firmware services.

--*/

#include "bootmgr.h"
#include <wchar.h>

PWSTR BootDirectory;

PWSTR
BmFwInitializeBootDirectoryPath (
    VOID
    )

/*++

Routine Description:

    Initializes the boot directory path.

Arguments:

    None.

Return Value:

    Pointer to the boot directory path.

--*/

{
    //
    // TODO: Finish implementing this routine.
    //

#if defined(_EFI)
    BootDirectory = L"\\EFI\\Microsoft\\Boot";
#else
    BootDirectory = L"\\Boot";
#endif

    return BootDirectory;
}

NTSTATUS
BmpFwGetFullPath (
    IN  PWSTR PartialPath,
    OUT PWSTR *FullPathOut
    )

/*++

Routine Description:

    Transforms a partial file path into a full file path.

Arguments:

    PartialPath - Pointer to the partial path.

    FullPath - Pointer to a PWSTR that receives the address of the full path.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

    STATUS_NO_MEMORY if memory allocation fails.

--*/

{
    ULONG BootDirectoryLength, PartialPathLength, TotalLength;
    size_t TotalSize;
    PWSTR FullPath;

    //
    // Find the length of the boot directory path.
    //
    BootDirectoryLength = wcslen(BootDirectory);
    if ((BootDirectoryLength + 1) < BootDirectoryLength) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Find the length of the partial path.
    //
    PartialPathLength = wcslen(PartialPath);
    TotalLength = BootDirectoryLength + PartialPathLength + 1;
    if (TotalLength < PartialPathLength) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Calculate the total length.
    //
    TotalSize = TotalLength * sizeof(WCHAR);
    if (TotalSize > MAXULONG) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Concatenate the paths.
    //
    FullPath = BlMmAllocateHeap(TotalSize);
    *FullPathOut = FullPath;
    if (FullPath == NULL) {
        return STATUS_NO_MEMORY;
    }
    wcscpy_s(FullPath, TotalLength, BootDirectory);
    wcscat_s(FullPath, TotalLength, PartialPath);

    return STATUS_SUCCESS;
}
