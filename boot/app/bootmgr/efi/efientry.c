/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efientry.c

Abstract:

    Boot manager entry point for EFI systems.

--*/

#include "efilib.h"
#include "bootmgr.h"
#include "ntdef.h"

EFI_STATUS
EFIAPI
EfiEntry (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )

/*++

Routine Description:

    Boot manager entry point from EFI firmware.

Arguments:

    ImageHandle - Firmware handle for the boot manager image.

    SystemTable - Pointer to the firmware system table.

Return Value:

    Does not return if successful.

    Error code if unsuccessful.

--*/

{
    PBOOT_APPLICATION_PARAMETERS InputParameters;
    NTSTATUS status = 0;

#if !defined(NDEBUG)
    //
    // Initialize early debugging services.
    //
    EfiDebugInitialize(SystemTable->ConOut);
#endif
    //
    // Create firmware-independent parameters structure.
    //
    InputParameters = EfiInitCreateInputParameters(ImageHandle, SystemTable);
    if (InputParameters)
        status = BmMain(InputParameters);
    else 
        status = STATUS_INVALID_PARAMETER;

    //
    // Transfer control to firmware-independent code.
    //
    return EfiGetEfiStatusCode(status);
}
