/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mmha.c

Abstract:

    Heap allocation services.

--*/

#include "efilib.h"
#include "mm.h"

#define FREE_LIST_BUCKET_COUNT 7
#define FREE_LIST_SIZE (sizeof(PMM_FREE_HEAP_ENTRY) * FREE_LIST_BUCKET_COUNT)

ULONG HapInitializationStatus = 0;
PMM_FREE_HEAP_ENTRY *MmFreeList;
ULONG HapMinimumHeapSize;
ULONG HapAllocationAttributes;
LIST_ENTRY MmHeapBoundaries;

FORCEINLINE
ULONG
MmHapGetBufferSize (
    IN PMM_FREE_HEAP_ENTRY HeapEntry
    )

/*++

Routine Description:

    Calculates the size of a heap entry's buffer.

Arguments:

    HeapEntry - Pointer to the heap entry to check.

Return Value:

    The buffer size.

--*/

{
    return MM_HEAP_LINK_DECODE(HeapEntry->BufferNext) - (ULONG_PTR)HeapEntry;
}

ULONG
MmHapGetBucketIndex (
    IN ULONG_PTR BufferSize
    )

/*++

Routine Description:

    Calculates the bucket index for the specified buffer size.

Arguments:

    BufferSize - The buffer size.

Return Value:

    The bucket index if successful.

    FREE_LIST_BUCKET_COUNT if unsuccessful.

--*/

{
    ULONG Shift;

    //
    // Large allocations use the last bucket.
    //
    if (BufferSize >= PAGE_SIZE) {
        return FREE_LIST_BUCKET_COUNT - 1;
    }

    //
    // Calculate the bucket index.
    //
    Shift = 0;
    do {
        Shift++;
    } while (BufferSize >> Shift);

    return Shift - 6;
}

PMM_USED_HEAP_ENTRY
MmHapFindFreeHeapEntry (
    IN ULONG_PTR BufferSize
    )

/*++

Routine Description:

    Finds a suitable entry in the freelist.

Arguments:

    BufferSize - The minimum buffer size.

Return Value:

    Pointer to the entry if successful.

    NULL if unsuccessful.

--*/

{
    ULONG BucketIndex;
    PMM_FREE_HEAP_ENTRY FreeEntry;
    ULONG_PTR FreeBufferSize;

    //
    // Find the bucket index for the buffer size.
    //
    BucketIndex = MmHapGetBucketIndex(BufferSize);
    if (BucketIndex >= FREE_LIST_BUCKET_COUNT) {
        DebugError(L"Invalid bucket index\r\n");
        return NULL;
    }

    //
    // Search the freelist.
    //
    do {
        FreeEntry = MmFreeList != NULL ? MmFreeList[BucketIndex] : NULL;

        //
        // Find a large enough free entry in the bucket.
        //
        while (FreeEntry != NULL) {
            FreeBufferSize = MmHapGetBufferSize(FreeEntry);
            if (FreeBufferSize >= BufferSize) {
                break;
            }

            FreeEntry = (PMM_FREE_HEAP_ENTRY)(MM_HEAP_LINK_DECODE(FreeEntry->FreeNext));
        }

        //
        // Done if an entry was found.
        //
        if (FreeEntry != NULL) {
            break;
        }

        //
        // Move on to the next bucket.
        //
        BucketIndex++;
        if (BucketIndex >= FREE_LIST_BUCKET_COUNT) {
            return NULL;
        }
    } while (FreeEntry == NULL);

    //
    // Remove it from the list.
    //
    // FreeEntry = MmHapRemoveBufferFromFreeList(FreeEntry);
    // if (FreeEntry == NULL) {
    //     return NULL;
    // }

    //
    // Check for corruption.
    //
    // FreeEntry = MmHapCheckBufferLinks(FreeEntry);
    // if (FreeEntry == NULL) {
    //     return NULL;
    // }

    //
    // TODO: Finish implementing this routine.
    //
    return NULL;
}

NTSTATUS
BlMmFreeHeap (
    IN PVOID Pointer
    )

/*++

Routine Description:

    Frees allocated heap memory.

Arguments:

    Pointer - Pointer to the memory to free.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_UNSUCCESSFUL if the heap allocator is not initialized.

    Error code if unsuccessful.

--*/

{
    NTSTATUS Status;

    //
    // The heap allocator must be initialized.
    //
    if (HapInitializationStatus != 1) {
        DebugError(L"Heap allocator not initialized\r\n");
        return STATUS_UNSUCCESSFUL;
    }

    //
    // TODO: Don't use the firmware allocator.
    //
    Status = EfiFreePool(Pointer);
    if (!NT_SUCCESS(Status)) {
        DebugError(L"EfiFreePool failed (Status=0x%x)\r\n", Status);
        return Status;
    }

    return STATUS_SUCCESS;
}

PVOID
BlMmAllocateHeap (
    IN ULONG_PTR Size
    )

/*++

Routine Description:

    Allocates memory on the heap.

Arguments:

    Size - The size, in bytes, to allocate.

Return Value:

    Pointer to the allocated memory, if successful.

    NULL if unsuccessful.

--*/

{
    NTSTATUS Status;
    ULONG_PTR RealSize;
    PVOID Buffer;

    //
    // The heap allocator must be initialized.
    //
    if (HapInitializationStatus != 1) {
        DebugError(L"Heap allocator not initialized\r\n");
        return NULL;
    }

    //
    // Align to size of used entry.
    //
    RealSize = ALIGN_UP(Size + FIELD_OFFSET(MM_USED_HEAP_ENTRY, Buffer), FIELD_OFFSET(MM_USED_HEAP_ENTRY, Buffer));
    if (RealSize <= Size) {
        DebugError(L"Integer overflow\r\n");
        return NULL;
    }

    //
    // Must be large enough to hold a free entry.
    //
    if (RealSize < sizeof(MM_FREE_HEAP_ENTRY)) {
        RealSize = sizeof(MM_FREE_HEAP_ENTRY);
    }

    //
    // TODO: Don't use the firmware allocator.
    //

    Status = EfiAllocatePool(EfiLoaderData, Size, &Buffer);
    if (!NT_SUCCESS(Status)) {
        DebugError(L"EfiAllocatePool failed (Status=0x%x)\r\n", Status);
        return NULL;
    }

    return Buffer;
}

VOID
MmHaDestroy (
    VOID
    )

/*++

Routine Description:

    Destroys the heap allocator.

Arguments:

    None.

Return Value:

    None.

--*/

{
    DebugInfo(L"Destroying heap allocator...\r\n");
    HapInitializationStatus = 0;
}

NTSTATUS
MmHapHeapAllocatorExtend (
    IN ULONG_PTR HeapSize
    )

/*++

Routine Description:

    Extends the heap to the desired size.

Arguments:

    HeapSize - The amount to extend the heap by.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

    Any other status value returned by BlMmAllocatePagesInRange.

--*/

{
    NTSTATUS Status;
    PMM_HEAP_BOUNDARY HeapBoundary;
    PVOID HeapBase;
    PMM_USED_HEAP_ENTRY FirstHeapEntry;
    PMM_FREE_HEAP_ENTRY SecondHeapEntry;

    //
    // Add space for extra data.
    //
    Status = RtlSizeTAdd(HeapSize, 2 * PAGE_SIZE, &HeapSize);
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    //
    // Apply the minimum heap size restriction.
    //
    if (HeapSize < HapMinimumHeapSize) {
        HeapSize = HapMinimumHeapSize;
    }

    //
    // Round to the page size.
    //
    HeapSize = ALIGN_UP(HeapSize, PAGE_SIZE);
    if (HeapSize == 0) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Check if a heap already exists.
    //
    if (!IsListEmpty(&MmHeapBoundaries)) {
        HeapBoundary = CONTAINING_RECORD(MmHeapBoundaries.Flink, MM_HEAP_BOUNDARY, ListEntry);
        if ((HeapBoundary->HeapLimit + PAGE_SIZE) <= HeapBoundary->HeapEnd) {
            DebugError(L"Existing heap expansion not implemented\r\n");
            return STATUS_NOT_IMPLEMENTED;
        }
    }

    //
    // Allocate memory for the heap.
    //
    HeapBase = NULL;
    Status = BlMmAllocatePagesInRange(
        &HeapBase,
        HeapSize >> PAGE_SHIFT,
        MEMORY_TYPE_HEAP,
        HapAllocationAttributes,
        NULL,
        0
    );
    if (!NT_SUCCESS(Status)) {
        DebugError(L"Heap memory allocation failed\r\n");
        return Status;
    }

    //
    // Define initial heap layout.
    //
    FirstHeapEntry = (PMM_USED_HEAP_ENTRY)HeapBase;
    HeapBoundary = (PMM_HEAP_BOUNDARY)FirstHeapEntry->Buffer;
    SecondHeapEntry = (PMM_FREE_HEAP_ENTRY)(HeapBoundary + 1);

    //
    // The first buffer contains the heap boundary structure.
    //
    FirstHeapEntry->BufferNext = MM_HEAP_LINK_DECODE(SecondHeapEntry);
    FirstHeapEntry->BufferPrevious = 0;
    HeapBoundary->HeapBase = (ULONG_PTR)HeapBase;
    HeapBoundary->HeapLimit = (ULONG_PTR)HeapBase + HeapSize;
    HeapBoundary->HeapStart = (ULONG_PTR)SecondHeapEntry;

    //
    // The second buffer is free.
    //
    SecondHeapEntry->BufferNext = MM_HEAP_LINK_ENCODE(SecondHeapEntry, MM_HEAP_LINK_BUFFER_FREE | MM_HEAP_LINK_BUFFER_ON_HEAP);
    SecondHeapEntry->BufferPrevious = (ULONG_PTR)FirstHeapEntry;

    //
    // Host the free list if needed.
    //
    if (IsListEmpty(&MmHeapBoundaries)) {
        MmFreeList = (PMM_FREE_HEAP_ENTRY *)(HeapBoundary->HeapLimit - FREE_LIST_SIZE);
        HeapBoundary->HeapLimit = (ULONG_PTR)MmFreeList;
        RtlZeroMemory(MmFreeList, FREE_LIST_SIZE);
    }

    //
    // Reserve the top page.
    //
    HeapBoundary->HeapEnd = HeapBoundary->HeapLimit;
    HeapBoundary->HeapLimit -= PAGE_SIZE;

    InsertTailList(&MmHeapBoundaries, &HeapBoundary->ListEntry);
    return STATUS_SUCCESS;
}

NTSTATUS
MmHaInitialize (
    IN ULONG MinimumHeapSize,
    IN ULONG AllocationAttributes
    )

/*++

Routine Description:

    Initializes the heap allocator.

Arguments:

    MinimumHeapSize - The minimum size of the heap.

    AllocationAttributes - Flags to refer to when allocating heap memory.

Return Value:

    STATUS_SUCCESS if successful.

    Any other status value returned by MmHapHeapAllocatorExtend.

--*/

{
    NTSTATUS Status;

    DebugInfo(L"Initializing heap allocator...\r\n");

    //
    // Initialize heap state.
    //
    MmFreeList = NULL;
    HapMinimumHeapSize = ALIGN_UP(MinimumHeapSize, PAGE_SIZE);
    HapAllocationAttributes = AllocationAttributes & 0x20000;
    InitializeListHead(&MmHeapBoundaries);

    //
    // Expand the heap.
    //
    Status = MmHapHeapAllocatorExtend(sizeof(PVOID));
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    HapInitializationStatus = 1;
    return STATUS_SUCCESS;
}
