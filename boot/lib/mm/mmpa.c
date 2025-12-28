/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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
BlpMmInitializeConstraints (
    VOID
    )

/*++

Routine Description:

    This routine reads physical memory constraint requirements from the
    application options and saves them for subsequent use by the page allocator.

Arguments:

    None

Return Value:

    NT Status code.

--*/

{

    NTSTATUS Status  = STATUS_SUCCESS;

    // TODO IMPLEMENT
   

    return Status;
}


VOID
BlMmRemoveBadMemory (
    VOID
    )

/*++

Routine Description:

    This routine reads bad memory information from BCD, then attempts to
    remove any such regions from system memory so they will not be
    available to boot applications, or the OS.

Arguments:

    None.

Return Value:

    None.

--*/

{

    // TODO IMPLEMENT

    return;
}

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
        DebugError(L"Invalid parameter\r\n");
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
        DebugError(L"Page allocation not supported\r\n");
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

NTSTATUS
BlMmAllocatePagesInRange (
    IN OUT PVOID          *Address,
    IN     ULONG_PTR      Pages,
    IN     ULONG          MemoryType,
    IN     ULONG          AllocationAttributes,
    IN     PADDRESS_RANGE Range OPTIONAL,
    IN     ULONG          Unknown
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

    Unknown - Ignored.

Return Value:

    STATUS_SUCCESS if successful.

    Any other status value returned by MmPapAllocatePagesInRange.

--*/

{
    (VOID) Unknown;

    if (!(AllocationAttributes & 0x210000)) {
        return MmPapAllocatePagesInRange(Address, Pages, MemoryType, AllocationAttributes, Range);
    }

    //
    // TODO: Finish implementing this routine.
    //

    DebugError(L"Virtual page allocation not implemented\r\n");
    return STATUS_NOT_IMPLEMENTED;
}
