/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mmpa.c

Abstract:

    Page allocation services.

--*/

#include "mm.h"
#if defined(_EFI)
#include "efilib.h"
#endif

NTSTATUS
MmPapAllocatePagesInRange (
    IN OUT PVOID          *Address,
    IN     ULONG_PTR      PageCount,
    IN     ULONG          MemoryType,
    IN     ULONG          AllocationAttributes,
    IN     PADDRESS_RANGE Range OPTIONAL
    )

/*++

Routine Description:

    Allocates pages in the requested range.

Arguments:

    Address - Pointer to the address to allocate at (receives the allocated address).

    PageCount - The number of pages to allocate.

    MemoryType - The type of memory to allocate.

    AllocationAttributes - Attributes specifying the allocation method.

    Range - Pointer to the range descriptor or NULL.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if any of Address, PageCount, or Range are invalid.

--*/

{
    NTSTATUS Status;
    PVOID RequestedAddress;

    (VOID) MemoryType;

    MmDescriptorCallTreeCount++;

    //
    // Validate arguments.
    //
    if (Address == NULL || PageCount == 0 || (Range != NULL && Range->Minimum >= Range->Maximum)) {
        Status = STATUS_INVALID_PARAMETER;
        goto Exit;
    }

    //
    // Allocate physical pages.
    //
    if (MmTranslationType == TRANSLATION_TYPE_NONE) {
        //
        // Use requested address if specified.
        //
        if (AllocationAttributes & 0x40000) {
            RequestedAddress = *Address;
        } else {
            RequestedAddress = NULL;
        }

        //
        // TODO: Don't use the firmware allocator.
        //
#if defined(_EFI)
        Status = EfiAllocatePages(
            AllocateAnyPages,
            EfiLoaderData,
            PageCount,
            (EFI_PHYSICAL_ADDRESS *)&RequestedAddress
        );
#else
        Status = STATUS_NOT_SUPPORTED;
#endif

        goto Exit;
    }

    //
    // TODO: Finish implementing this routine.
    //
    DebugError(L"Virtual page allocation not implemented\r\n");
    Status = STATUS_NOT_IMPLEMENTED;

Exit:
    MmMdFreeGlobalDescriptors();
    MmDescriptorCallTreeCount--;
    return Status;
}
