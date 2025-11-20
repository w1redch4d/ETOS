/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mm.h

Abstract:

    Provides memory manager definitions.

--*/

#pragma once

#ifndef _MM_H
#define _MM_H

#include "bootlib.h"

#define MAX_STATIC_DESCRIPTOR_COUNT 1024

#define ALIGN_UP(Value, Alignment) (((Value) + ((Alignment) - 1)) & ~((Alignment) - 1))

#define MM_HEAP_PTR_BUFFER_FREE    0x1
#define MM_HEAP_PTR_BUFFER_ON_HEAP 0x2
#define MM_HEAP_PTR_ENTRY_NOT_USED 0x4
#define MM_HEAP_PTR_DATA_MASK      ~(MM_HEAP_PTR_BUFFER_FREE | MM_HEAP_PTR_BUFFER_ON_HEAP | MM_HEAP_PTR_ENTRY_NOT_USED)

typedef struct {
    ULONG_PTR BufferNext;
    ULONG_PTR BufferPrevious;
    ULONG_PTR FreeNext;
    ULONG_PTR FreePrevious;
} MM_FREE_HEAP_ENTRY, *PMM_FREE_HEAP_ENTRY;

typedef struct {
    ULONG_PTR BufferNext;
    ULONG_PTR BufferPrevious;
    UCHAR     Buffer[ANYSIZE_ARRAY];
} MM_USED_HEAP_ENTRY, *PMM_USED_HEAP_ENTRY;

typedef struct {
    LIST_ENTRY ListEntry;
    ULONG_PTR  HeapEnd;
    ULONG_PTR  HeapLimit;
    ULONG_PTR  HeapBase;
    ULONG_PTR  HeapStart;
} MM_HEAP_BOUNDARY, *PMM_HEAP_BOUNDARY;

extern ULONG MmDescriptorCallTreeCount;
extern ULONG MmTranslationType;

extern MEMORY_DESCRIPTOR MmStaticMemoryDescriptors[MAX_STATIC_DESCRIPTOR_COUNT];
extern PMEMORY_DESCRIPTOR MmGlobalMemoryDescriptors;
extern ULONG MmGlobalMemoryDescriptorCount, MmGlobalMemoryDescriptorsUsed;
extern LIST_ENTRY MmFreeGlobalMemoryDescriptorsList;

//
// Architecture services.
//

BOOLEAN
MmArchTranslateVirtualAddress (
    IN  PVOID             VirtualAddress,
    OUT PPHYSICAL_ADDRESS PhysicalAddress
    );

//
// Memory descriptor services.
//

VOID
MmMdRemoveDescriptorFromList (
    IN PMEMORY_DESCRIPTOR_LIST Mdl,
    IN PMEMORY_DESCRIPTOR      Descriptor
    );

NTSTATUS
MmMdFreeDescriptor (
    IN PMEMORY_DESCRIPTOR Descriptor
    );

VOID
MmMdFreeList (
    IN PMEMORY_DESCRIPTOR_LIST Mdl
    );

VOID
MmMdFreeGlobalDescriptors (
    VOID
    );

//
// Page allocation services.
//

NTSTATUS
MmPapAllocatePagesInRange (
    IN OUT PVOID          *Address,
    IN     ULONG_PTR      PageCount,
    IN     ULONG          MemoryType,
    IN     ULONG          AllocationAttributes,
    IN     PADDRESS_RANGE Range OPTIONAL
    );

//
// Heap allocation services.
//

NTSTATUS
MmHaInitialize (
    IN ULONG MinimumHeapSize,
    IN ULONG AllocationAttributes
    );

NTSTATUS
MmHapHeapAllocatorExtend (
    IN ULONG_PTR HeapSize
    );

#endif /* !_MM_H */
