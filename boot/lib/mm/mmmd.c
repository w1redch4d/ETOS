/*++

Copyright (c) 2025, Quinn Stephens.
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
