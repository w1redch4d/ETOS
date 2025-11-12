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

extern ULONG MmDescriptorCallTreeCount;
extern ULONG MmTranslationType;

extern MEMORY_DESCRIPTOR MmStaticMemoryDescriptors[MAX_STATIC_DESCRIPTOR_COUNT];
extern PMEMORY_DESCRIPTOR MmGlobalMemoryDescriptors;
extern ULONG MmGlobalMemoryDescriptorCount, MmGlobalMemoryDescriptorsUsed;

BOOLEAN
MmArchTranslateVirtualAddress (
    IN  PVOID             VirtualAddress,
    OUT PPHYSICAL_ADDRESS PhysicalAddress
    );

#endif /* !_MM_H */
