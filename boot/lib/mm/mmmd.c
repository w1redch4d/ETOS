/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mmmd.c

Abstract:

    Memory descriptor services.

--*/

#include "mm.h"

MEMORY_DESCRIPTOR MmStaticMemoryDescriptors[MAX_STATIC_DESCRIPTOR_COUNT];
PMEMORY_DESCRIPTOR MmGlobalMemoryDescriptors;
ULONG MmGlobalMemoryDescriptorCount, MmGlobalMemoryDescriptorsUsed;
LIST_ENTRY MmFreeGlobalMemoryDescriptorsList;
PMEMORY_DESCRIPTOR MmDynamicMemoryDescriptors = NULL;
ULONG MmDynamicMemoryDescriptorCount;

VOID
MmMdRemoveDescriptorFromList (
    IN PMEMORY_DESCRIPTOR_LIST Mdl,
    IN PMEMORY_DESCRIPTOR      Descriptor
    )

/*++

Routine Description:

    Removes a descriptor from a MDL.

Arguments:

    Mdl - Pointer to the MDL to remove Descriptor from.

    Descriptor - Pointer to the descriptor to remove from Mdl.

Return Value:

    None.

--*/

{
    PLIST_ENTRY Previous;

    Previous = Descriptor->ListEntry.Blink;
    RemoveEntryList(&Descriptor->ListEntry);

    //
    // Check if the removed descriptor was cached.
    //
    if (Mdl->Current != &Descriptor->ListEntry) {
        return;
    }

    //
    // Cache the previous descriptor if possible.
    //
    if (((ULONG_PTR)Previous < (ULONG_PTR)MmGlobalMemoryDescriptors
         || (ULONG_PTR)Previous >= (ULONG_PTR)&MmGlobalMemoryDescriptors[MmGlobalMemoryDescriptorCount])
        && Previous != Mdl->Head) {
        Mdl->Current = Previous;
    } else {
        Mdl->Current = NULL;
    }
}

NTSTATUS
MmMdFreeDescriptor (
    IN PMEMORY_DESCRIPTOR Descriptor
    )

/*++

Routine Description:

    Frees a memory descriptor.

Arguments:

    Descriptor - Pointer to the descriptor to free.

Return Value:

    STATUS_SUCCESS if successful.

    Any other status value returned by BlMmFreeHeap.

--*/

{
    //
    // Free descriptor from static/dynamic MDL.
    //
    if ((MmDynamicMemoryDescriptors != NULL
         && (ULONG_PTR)Descriptor >= (ULONG_PTR)MmDynamicMemoryDescriptors
         && (ULONG_PTR)Descriptor < (ULONG_PTR)&MmDynamicMemoryDescriptors[MmDynamicMemoryDescriptorCount])
        || ((ULONG_PTR)Descriptor >= (ULONG_PTR)MmStaticMemoryDescriptors
            && (ULONG_PTR)Descriptor < (ULONG_PTR)&MmStaticMemoryDescriptors[MAX_STATIC_DESCRIPTOR_COUNT])) {
        //
        // Clear from static/dynamic MDL.
        //
        RtlZeroMemory(Descriptor, sizeof(*Descriptor));

        //
        // Add to free list.
        //
        InsertHeadList(&MmFreeGlobalMemoryDescriptorsList, &Descriptor->ListEntry);

        return STATUS_SUCCESS;
    }

    //
    // Free descriptor from heap.
    //
    return BlMmFreeHeap(Descriptor);
}

VOID
MmMdFreeList (
    IN PMEMORY_DESCRIPTOR_LIST Mdl
    )

/*++

Routine Description:

    Frees a memory descriptor list.

Arguments:

    Mdl - Pointer to the MDL.

Return Value:

    None.

--*/

{
    PLIST_ENTRY Entry;
    PMEMORY_DESCRIPTOR Descriptor;

    Entry = Mdl->Head->Flink;
    while (Entry != Mdl->Head) {
        Descriptor = CONTAINING_RECORD(Entry, MEMORY_DESCRIPTOR, ListEntry);
        MmMdRemoveDescriptorFromList(Mdl, Descriptor);
        MmMdFreeDescriptor(Descriptor);
        Entry = Entry->Flink;
    }
}

VOID
MmMdFreeGlobalDescriptors (
    VOID
    )

/*++

Routine Description:

    Frees all global memory descriptors.

Arguments:

    None.

Return Value:

    None.

--*/

{
    //
    // Only free descriptors if executing at base level.
    //
    if (MmDescriptorCallTreeCount != 1) {
        return;
    }

    //
    // TODO: Finish implementing this routine.
    //

    MmGlobalMemoryDescriptorsUsed = 0;
    InitializeListHead(&MmFreeGlobalMemoryDescriptorsList);
}
