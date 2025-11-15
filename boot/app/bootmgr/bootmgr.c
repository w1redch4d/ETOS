/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bootmgr.c

Abstract:

    Firmware-independent boot manager main routine.

--*/

#include "bootmgr.h"

GUID GUID_WINDOWS_BOOTMGR = { 0x9dea862c, 0x5cdd, 0x4e70, { 0xac, 0xc1, 0xf3, 0x2b, 0x34, 0x4d, 0x47, 0x95 } };
GUID GUID_WINDOWS_LEGACY_NTLDR = { 0x466f5a88, 0x0af2, 0x4f76, { 0x90, 0x38, 0x09, 0x5b, 0x17, 0x0d, 0xc2, 0x1c } };

ULONGLONG ApplicationStartTime, PostTime;

NTSTATUS
BmMain (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters
    )

/*++

Routine Description:

    Firmware-independent boot manager main routine.

Arguments:

    ApplicationParameters - Pointer to the application parameters structure.

Return Value:

    Does not return if successful (execution is transferred to the OS loader).

    Error code if unsuccessful.

--*/

{
    NTSTATUS Status;
    BOOT_LIBRARY_PARAMETERS LibraryParameters;
    HANDLE DataStoreHandle; 
    PRETURN_DATA ReturnData;

#if defined(__x86_64__) || defined(__i386__)
    ApplicationStartTime = __rdtsc();
    PostTime = ApplicationStartTime;
#endif

    //
    // Initialize the boot library.
    //
    LibraryParameters.Flags = 0x0002f198;
    LibraryParameters.TranslationType = TRANSLATION_TYPE_NONE;
    LibraryParameters.MinimumAllocationCount = 16;
    LibraryParameters.MinimumHeapSize = 512 * 1024;
#if defined(_EFI)
    LibraryParameters.ApplicationBaseDirectory = L"\\EFI\\Microsoft\\Boot";
#else
    LibraryParameters.ApplicationBaseDirectory = L"\\Boot";
#endif
    LibraryParameters.FontBaseDirectory = NULL;
    LibraryParameters.Identifier = &GUID_WINDOWS_BOOTMGR;
    Status = BlInitializeLibrary(ApplicationParameters, &LibraryParameters);
    if (!NT_SUCCESS(Status)) {
        if (Status != STATUS_INVALID_PARAMETER_9) {
            ConsolePrint(L"BlInitializeLibrary failed 0x%x\r\n", Status);
        }

        goto Exit;
    }

    //
    // Initialize the boot directory.
    //
    BmFwInitializeBootDirectoryPath();

    //
    // Open the BCD.
    //
    DebugInfo(L"Opening BCD...\r\n");
    Status = BmOpenDataStore(&DataStoreHandle);
    if (!NT_SUCCESS(Status)) {
        DebugError(L"Failed to open BCD\r\n");
        goto Exit;
    }

    //
    // Nothing else to do right now.
    //
    DebugInfo(L"Halting...\r\n");
    while (TRUE) {
#if defined(__x86_64__) || defined(__i386__)
        asm volatile("cli; hlt");
#endif
    }

Exit:
    ReturnData = (PRETURN_DATA)((PUCHAR)ApplicationParameters + ApplicationParameters->ReturnDataOffset);
    ReturnData->Version = RETURN_DATA_VERSION;
    ReturnData->Status = Status;
    BlDestroyLibrary();
    return Status;
}
