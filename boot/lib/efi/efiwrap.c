/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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

    Wrapper around MmArchTranslateVirtualAddress to make pointer translation easier.

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

    Wrapper around GetMemoryMap.

Arguments:

    MemoryMapSize - Pointer to the size of the buffer.

    MemoryMap - Pointer to the buffer to store the memory map in.

    MapKey - Pointer to the memory map key.

    DescriptorSize - Pointer to a UINTN that receives the size of a single memory descriptor.

    DescriptorVersion - Pointer to a UINT32 that receives the version of memory descriptors being used by the firmware.

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

    Wrapper around AllocatePages.

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

    Wrapper around FreePages.

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

NTSTATUS
EfiSetWatchdogTimer (
    IN UINTN  Timeout,
    IN UINT64 WatchdogCode,
    IN UINTN  DataSize,
    IN CHAR16 *WatchdogData OPTIONAL
    )

/*++

Routine Description:

    Wrapper around SetWatchdogTimer.

Arguments:

    Timeout - The number of seconds to set the timer to (or 0 to disable the timer).

    WatchdogCode - The code to log if the timer times out (0x0000-0xffff are reserved).

    DataSize - The size, in bytes, of WatchdogData.

    WatchdogData - Pointer to a string describing the timeout reason, or NULL.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if WatchdogCode is invalid.

    STATUS_NOT_SUPPORTED if the system does not have a watchdog timer.

    STATUS_IO_DEVICE_ERROR if a hardware error occurs.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->SetWatchdogTimer(
        Timeout,
        WatchdogCode,
        DataSize,
        WatchdogData
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiVmOpenProtocol (
    IN  EFI_HANDLE Handle,
    IN  EFI_GUID   *Protocol,
    OUT VOID       **Interface OPTIONAL
    )

/*++

Routine Description:

    Wrapper around OpenProtocol/HandleProtocol, with address translation support.

Arguments:

    Handle - The handle being queried.

    Protocol - The unique identifier of the protocol.

    Interface - Pointer to a VOID* that receives the address of the interface or NULL.

Return Value:

    STATUS_NOT_IMPLEMENTED.

--*/

{
    (VOID) Handle;
    (VOID) Protocol;
    (VOID) Interface;

    //
    // TODO: Implement this routine.
    //

    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
EfiVmCloseProtocol (
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol
    )

/*++

Routine Description:

    Wrapper around CloseProtocol, with address translation support.

Arguments:

    Handle - The handle for the protocol interface.

    Protocol - The unique identifier of the protocol.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if any argument(s) is/are NULL.

--*/

{
    (VOID) Handle;
    (VOID) Protocol;

    //
    // TODO: Implement this routine.
    //

    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS
EfiOpenProtocol (
    IN  EFI_HANDLE Handle,
    IN  EFI_GUID   *Protocol,
    OUT VOID       **Interface OPTIONAL
    )

/*++

Routine Description:

    Wrapper around OpenProtocol/HandleProtocol.

Arguments:

    Handle - The handle being queried.

    Protocol - The unique identifier of the protocol.

    Interface - Pointer to a VOID* that receives the address of the interface or NULL.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NOT_SUPPORTED if the device does not support the requested protocol.

    STATUS_INVALID_PARAMETER if any argument(s) is/are NULL.

--*/

{
    NTSTATUS Status;
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    if (MmTranslationType != TRANSLATION_TYPE_NONE) {
        Status = EfiVmOpenProtocol(Handle, Protocol, Interface);
    } else {
        ContextType = CurrentExecutionContext->Type;
        if (ContextType != ExecutionContextFirmware) {
            BlpArchSwitchContext(ExecutionContextFirmware);
        }

        if (EfiST->Hdr.Revision == EFI_1_02_SYSTEM_TABLE_REVISION) {
            EfiStatus = EfiBS->HandleProtocol(
                Handle,
                Protocol,
                Interface
            );
        } else {
            EfiStatus = EfiBS->OpenProtocol(
                Handle,
                Protocol,
                Interface,
                EfiImageHandle,
                NULL,
                EFI_OPEN_PROTOCOL_GET_PROTOCOL
            );
        }

        if (ContextType != ExecutionContextFirmware) {
            BlpArchSwitchContext(ContextType);
        }

        Status = EfiGetNtStatusCode(EfiStatus);
    }

    if (!NT_SUCCESS(Status)) {
        *Interface = NULL;
    }

    return Status;
}

NTSTATUS
EfiCloseProtocol (
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol
    )

/*++

Routine Description:

    Wrapper around CloseProtocol.

Arguments:

    Handle - The handle for the protocol interface.

    Protocol - The unique identifier of the protocol.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if any argument(s) is/are NULL.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    if (MmTranslationType != TRANSLATION_TYPE_NONE) {
        return EfiVmCloseProtocol(Handle, Protocol);
    }

    if (EfiST->Hdr.Revision == EFI_1_02_SYSTEM_TABLE_REVISION) {
        return STATUS_SUCCESS;
    }

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->CloseProtocol(
        Handle,
        Protocol,
        EfiImageHandle,
        NULL
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    if (EfiStatus == EFI_NOT_FOUND) {
        return STATUS_SUCCESS;
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiConInExSetState (
    IN EFI_KEY_TOGGLE_STATE PreviousState,
    IN EFI_KEY_TOGGLE_STATE *KeyToggleState
    )

/*++

Routine Description:

    Wrapper around SetState.

Arguments:

    PreviousState - The previous key toggle state (ignored).

    KeyToggleState - Pointer to the desired key toggle state.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_IO_DEVICE_ERROR if the device is not functioning properly.

    STATUS_NOT_SUPPORTED if the device does not support the requested state change.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *ConInEx;
    EFI_STATUS EfiStatus;

    (VOID) PreviousState;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        //
        // Translate addresses for firmware.
        //
        ConInEx = TranslatePointer(EfiConInEx);
        KeyToggleState = TranslatePointer(KeyToggleState);

        //
        // Switch to firmware context.
        //
        BlpArchSwitchContext(ExecutionContextFirmware);
    } else {
        ConInEx = EfiConInEx;
    }

    EfiStatus = ConInEx->SetState(
        ConInEx,
        KeyToggleState
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiAllocatePool (
    IN  EFI_MEMORY_TYPE PoolType,
    IN  UINTN           Size,
    OUT VOID            **Buffer
    )

/*++

Routine Description:

    Wrapper around AllocatePool.

Arguments:

    PoolType - The memory type to allocate.

    Size - The number of bytes to allocate.

    Buffer - Pointer to a VOID* that receives the address of the allocated buffer or NULL.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INSUFFICIENT_NVRAM_RESOURCES if the pool could not be allocated.

    STATUS_INVALID_PARAMETER if PoolType is invalid or Buffer is NULL.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->AllocatePool(
        PoolType,
        Size,
        Buffer
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

NTSTATUS
EfiFreePool (
    IN VOID *Buffer
    )

/*++

Routine Description:

    Wrapper around FreePool.

Arguments:

    Buffer - Pointer to the buffer to free.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Buffer is invalid.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    EFI_STATUS EfiStatus;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiStatus = EfiBS->FreePool(
        Buffer
    );

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }

    return EfiGetNtStatusCode(EfiStatus);
}

VOID
NORETURN
EfiResetSystem (
    IN EFI_RESET_TYPE ResetType
    )

/*++

Routine Description:

    Wrapper around ResetSystem.

Arguments:

    ResetType - The kind of reset to perform.

Return Value:

    Does not return.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware && EfiBS != NULL) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    EfiRT->ResetSystem(
        ResetType,
        EFI_SUCCESS,
        0,
        NULL
    );

    //
    // ResetSystem must never return.
    //
    UNREACHABLE();
}
