/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efiwrap.c

Abstract:

    Wrappers around firmware interfaces.

--*/

#include "efilib.h"
#include "mm.h"

FORCEINLINE
PVOID
TranslatePointer (
    PVOID Pointer
    )

/*++

Routine Description:

    Wrapper around MmArchTranslateVirtualAddress() to make pointer translation easier.

Arguments:

    Pointer - The pointer to be translated.

Return Value:

    The translated pointer.

--*/

{
    PHYSICAL_ADDRESS PhysicalAddress;

    if (Pointer == NULL) {
        return NULL;
    }

    MmArchTranslateVirtualAddress(Pointer, &PhysicalAddress);
    return (PVOID)PhysicalAddress.QuadPart;
}

NTSTATUS
EfiGetMemoryMap (
    IN OUT UINTN                 *MemoryMapSize,
    OUT    EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT    UINTN                 *MapKey,
    OUT    UINTN                 *DescriptorSize,
    OUT    UINT32                *DescriptorVersion
    )

/*++

Routine Description:

    Wrapper around GetMemoryMap().

Arguments:

    MemoryMapSize - Pointer to the size of the buffer.

    MemoryMap - Pointer to the buffer to store the memory map in.

    MapKey - Pointer to the memory map key.

    DescriptorSize - Pointer to a UINTN that receives the size of
        a single memory descriptor.

    DescriptorVersion - Pointer to a UINT32 that receives the version
        of memory descriptors being used by the firmware.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if MemoryMapSize and/or MemoryMap are invalid.

    STATUS_BUFFER_TOO_SMALL if MemoryMapSize is too small.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        //
        // Translate addresses for firmware.
        //
        MemoryMapSize = TranslatePointer(MemoryMapSize);
        MemoryMap = TranslatePointer(MemoryMap);
        MapKey = TranslatePointer(MapKey);
        DescriptorSize = TranslatePointer(DescriptorSize);
        DescriptorVersion = TranslatePointer(DescriptorVersion);

        //
        // Switch to firmware context.
        //
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->GetMemoryMap(
        MemoryMapSize,
        MemoryMap,
        MapKey,
        DescriptorSize,
        DescriptorVersion
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiAllocatePages (
    IN     EFI_ALLOCATE_TYPE    Type,
    IN     EFI_MEMORY_TYPE      MemoryType,
    IN     UINTN                Pages,
    IN OUT EFI_PHYSICAL_ADDRESS *Memory
    )

/*++

Routine Description:

    Wrapper for AllocatePages().

Arguments:

    Type - The allocation method.

    MemoryType - The type of memory to allocate.

    Pages - The number of pages to allocate.

    Memory - Pointer to a physical address of the allocation.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Type, MemoryType, and/or Memory are invalid.

    STATUS_INSUFFICIENT_NVRAM_RESOURCES if the pages could not be allocated.

    STATUS_NOT_FOUND if the pages could not be found.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    if (EfiBS == NULL && Type == AllocateAddress) {
        return STATUS_SUCCESS;
    }

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->AllocatePages(
        Type,
        MemoryType,
        Pages,
        Memory
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiFreePages (
    IN EFI_PHYSICAL_ADDRESS Memory,
    IN UINTN                Pages
    )

/*++

Routine Description:

    Wrapper for FreePages().

Arguments:

    Memory - Physical address of the pages to be freed.

    Pages - The number of pages to free.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NOT_FOUND if the allocation is not found.

    STATUS_INVALID_PARAMETER Memory and/or Pages are invalid.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    if (EfiBS == NULL) {
        return STATUS_SUCCESS;
    }

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->FreePages(
        Memory,
        Pages
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}
