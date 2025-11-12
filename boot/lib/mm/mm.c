/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mm.c

Abstract:

    Memory management services.

--*/

#include "mm.h"

ULONG MmDescriptorCallTreeCount;
ULONG MmTranslationType = TRANSLATION_TYPE_MAX;

NTSTATUS
BlpMmDestroy (
    IN ULONG Phase
    )

/*++

Routine Description:

    Cleans up after the memory manager.

    After calling this, the memory management services can no longer be used.

Arguments:

    Phase - Which phase of cleanup to perform.

        Phase 0: Free all application allocations.

        Phase 1: Destroy all MM modules.

Return Value:

    STATUS_SUCCESS if successful.

--*/

{
    if (Phase == 1) {
        DebugInfo(L"Destroying memory manager (phase 1/1)...\r\n");

        //
        // TODO: Implement remaining functionality.
        //

        return STATUS_SUCCESS;
    }

    if (Phase == 0) {
        DebugInfo(L"Destroying memory manager (phase 0/1)...\r\n");

        //
        // TODO: Implement remaining functionality.
        //

        return STATUS_SUCCESS;
    }

    DebugError(L"Invalid phase\r\n");
    return STATUS_INVALID_PARAMETER;
}

NTSTATUS
BlpMmInitialize (
    IN PMEMORY_INFO             MemoryInfo,
    IN ULONG                    TranslationType,
    IN PBOOT_LIBRARY_PARAMETERS LibraryParameters
    )

/*++

Routine Description:

    Initializes the memory manager.

Arguments:

    MemoryInfo - Pointer to the memory info structure.

    TranslationType - The current translation type.

    LibraryParameters - Pointer to the library parameters structure.

Return Value:

    STATUS_SUCCESS if successful,

    STATUS_INVALID_PARAMETER if any translation types are invalid.

--*/

{
    NTSTATUS Status;

    (VOID) MemoryInfo;

    DebugInfo(L"Initializing memory manager...\r\n");

    MmDescriptorCallTreeCount = 1;

    if (TranslationType >= TRANSLATION_TYPE_MAX || LibraryParameters->TranslationType >= TRANSLATION_TYPE_MAX) {
        DebugError(L"Invalid translation type\r\n");
        Status = STATUS_INVALID_PARAMETER;
        goto Phase0Failed;
    }

    //
    // Use the statically-allocated descriptors to bootstrap.
    //
    MmTranslationType = LibraryParameters->TranslationType;
    MmGlobalMemoryDescriptors = MmStaticMemoryDescriptors;
    MmGlobalMemoryDescriptorCount = MAX_STATIC_DESCRIPTOR_COUNT;

    //
    // TODO: Implement remaining functionality.
    //
    Status = STATUS_SUCCESS;

Phase0Failed:
    // MmMdFreeGlobalDescriptors();
    MmDescriptorCallTreeCount--;
    return Status;
}
