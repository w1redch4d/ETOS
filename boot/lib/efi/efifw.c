/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efifw.c

Abstract:

    EFI boot library firmware services.

--*/

#include "efilib.h"

FIRMWARE_DATA EfiFirmwareData;
PFIRMWARE_DATA EfiFirmwareParameters;
EFI_HANDLE EfiImageHandle;
EFI_SYSTEM_TABLE *EfiST;
EFI_BOOT_SERVICES *EfiBS;
EFI_RUNTIME_SERVICES *EfiRT;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *EfiConIn;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EfiConOut;
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *EfiConInEx;
const EFI_GUID EfiSimpleTextInputExProtocol = EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID;

VOID
BlpFwAppLoadCallback (
    IN ULONG Event,
    IN PVOID Context,
    IN ULONG Unknown0,
    IN ULONG Unknown1
    )

/*++

Routine Description:

    Initializes firmware-specific boot library components.

Arguments:

    Event - EVENT_APPLICATION_LOAD.

    Context - Pointer to the event context.

    Unknown0 - Ignored.

    Unknown1 - Ignored.

Return Value:

    None.

--*/

{
    (VOID) Context;
    (VOID) Unknown0;
    (VOID) Unknown1;

    if (Event != EVENT_APPLICATION_LOAD) {
        return;
    }

    //
    // TODO: Finish implementing this routine.
    //
}

VOID
NORETURN
BlFwReboot (
    VOID
    )

/*++

Routine Description:

    Resets the system.

Arguments:

    None.

Return Value:

    Does not return.

--*/

{
    //
    // TODO: Finish implementing this routine.
    //

    //
    // Reset the system.
    //
    EfiResetSystem(EfiResetCold);
}

NTSTATUS
BlpFwInitialize (
    IN ULONG          Phase,
    IN PFIRMWARE_DATA FirmwareData
    )

/*++

Routine Description:

    Initializes firmware-specific boot library components.

Arguments:

    Phase - Which phase of initialization to perform.

        Phase 0: Initialize global firmware-related data and pointers.

        Phase 1: Disable the watchdog timer and open the extended simple text input protocol.

    FirmwareData - Pointer to the firmware data structure.

Return Value:

    STATUS_SUCCESS if successful,

    STATUS_INVALID_PARAMETER if FirmwareData is invalid.

    STATUS_NOT_SUPPORTED if the FirmwareData revision is not supported.

--*/

{
    NTSTATUS Status;
    BOOLEAN EnableNumLock;
    EFI_KEY_TOGGLE_STATE ToggleState;

    //
    // Validate firmware data.
    //
    if (FirmwareData == NULL || FirmwareData->Version == 0) {
        return STATUS_INVALID_PARAMETER;
    }

    if (Phase == 0) {
        //
        // Require latest version of firmware data.
        //
        if (FirmwareData->Version < FIRMWARE_DATA_VERSION) {
            return STATUS_NOT_SUPPORTED;
        }

        //
        // Copy firmware data for later use.
        //
        RtlCopyMemory(&EfiFirmwareData, FirmwareData, sizeof(EfiFirmwareData));
        EfiFirmwareParameters = &EfiFirmwareData;

        //
        // Save commonly used values.
        //
        EfiImageHandle = FirmwareData->ImageHandle;
        EfiST = FirmwareData->SystemTable;
        EfiBS = EfiST->BootServices;
        EfiRT = EfiST->RuntimeServices;
        EfiConIn = EfiST->ConIn;
        EfiConOut = EfiST->ConOut;
        EfiConInEx = NULL;

        return STATUS_SUCCESS;
    }

    DebugInfo(L"Initializing firmware services (phase 1/1)...\r\n");

    //
    // Check if num lock should be enabled.
    //
    Status = BlGetBootOptionBoolean(BlpApplicationEntry.Options, BCDE_LIBRARY_TYPE_ENABLE_NUM_LOCK, &EnableNumLock);
    if (!NT_SUCCESS(Status)) {
        EnableNumLock = TRUE;
    }

    //
    // Perform one-time initialization steps.
    //
    if (!(BlpLibraryParameters.Flags & BOOT_LIBRARY_FLAG_REINITIALIZE)) {
        //
        // Install a hook at application load.
        //
        Status = BlEnRegisterEventHandler(EVENT_APPLICATION_LOAD, BlpFwAppLoadCallback, 0, 0);
        if (!NT_SUCCESS(Status)) {
            return Status;
        }

        //
        // Disable watchdog timer to prevent the system from rebooting itself.
        //
        EfiSetWatchdogTimer(0, 0, 0, NULL);

        //
        // Open the extended simple text input protocol.
        //
        Status = EfiOpenProtocol(EfiST->ConsoleInHandle, (EFI_GUID *)&EfiSimpleTextInputExProtocol, (VOID **)&EfiConInEx);
        if (!NT_SUCCESS(Status)) {
            DebugError(L"Failed to open extended simple text input protocol (Status=0x%x)\r\n", Status);
            return Status;
        }
    } else if (EnableNumLock) {
        //
        // Num lock is already enabled by default.
        //
        return STATUS_SUCCESS;
    }

    //
    // Enable/disable num lock and expose partial/incomplete keystrokes.
    //
    if (EfiConInEx != NULL) {
        ToggleState = EFI_TOGGLE_STATE_VALID | EFI_KEY_STATE_EXPOSED;
        if (EnableNumLock) {
            ToggleState |= EFI_NUM_LOCK_ACTIVE;
        }

        EfiConInExSetState(ToggleState | EFI_NUM_LOCK_ACTIVE, &ToggleState);
    }

    return STATUS_SUCCESS;
}
