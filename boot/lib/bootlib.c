/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bootlib.c

Abstract:

    Boot library services.

--*/

#include "bootlib.h"

#define MIN_APPLICATION_PARAMETERS_SIZE \
    (sizeof(BOOT_APPLICATION_PARAMETERS) + \
    sizeof(MEMORY_INFO) + \
    sizeof(BOOT_APPLICATION_TRANSITION_ENTRY) + \
    sizeof(FIRMWARE_DATA) + \
    sizeof(RETURN_DATA))

ULONG BlpApplicationFlags = 0;
PBOOT_APPLICATION_PARAMETERS BlpApplicationParameters;
BOOT_LIBRARY_PARAMETERS BlpLibraryParameters;
BOOT_APPLICATION_ENTRY BlpApplicationEntry;
PDEVICE_IDENTIFIER BlpBootDevice;
PWSTR BlpApplicationBaseDirectory;
BOOLEAN BlpApplicationIdentifierSet = FALSE;
ULONG BlPlatformFlags = 0x2a0000 | PLATFORM_FLAG_FIRMWARE_EXECUTION_CONTEXT;

NTSTATUS
InitializeLibrary (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters,
    IN PBOOT_LIBRARY_PARAMETERS     LibraryParameters
    )

/*++

Routine Description:

    Initializes the boot library.

Arguments:

    ApplicationParameters - Pointer to the application parameters structure.

    LibraryParameters - Pointer to the library parameters structure.

Return Value:

    STATUS_SUCCESS if successful.

    Error code if unsuccessful.

--*/

{
    NTSTATUS Status;
    PBOOT_APPLICATION_TRANSITION_ENTRY BootEntry;
    PMEMORY_INFO MemoryInfo;
    PFIRMWARE_DATA FirmwareData;
    ULONG Attributes;

    //
    // Validate application parameters.
    //
    if (ApplicationParameters == NULL
        || ApplicationParameters->Signature != BOOT_APPLICATION_PARAMETERS_SIGNATURE
        || ApplicationParameters->TotalSize < MIN_APPLICATION_PARAMETERS_SIZE) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Obtain pointers to auxillary structures.
    //
    BootEntry = (PBOOT_APPLICATION_TRANSITION_ENTRY)((PUCHAR)ApplicationParameters + ApplicationParameters->BootEntryOffset);
    MemoryInfo = (PMEMORY_INFO)((PUCHAR)ApplicationParameters + ApplicationParameters->MemoryInfoOffset);
    FirmwareData = (PFIRMWARE_DATA)((PUCHAR)ApplicationParameters + ApplicationParameters->FirmwareDataOffset);

    //
    // Validate application entry.
    //
    if (BootEntry->Signature != BOOT_APPLICATION_TRANSITION_ENTRY_SIGNATURE) {
        return STATUS_INVALID_PARAMETER_9;
    }

    //
    // Save commonly-accessed pointers.
    //
    BlpApplicationParameters = ApplicationParameters;
    BlpApplicationBaseDirectory = LibraryParameters->ApplicationBaseDirectory;
    BlpBootDevice = (PDEVICE_IDENTIFIER)((ULONG_PTR)ApplicationParameters + ApplicationParameters->BootDeviceOffset);

    //
    // ???
    //
    if (BootEntry->Attributes & BOOT_ENTRY_NO_TRAP_VECTORS) {
        __debugbreak();
    }

    //
    // Begin firmware-specific initialization.
    //
    Status = BlpFwInitialize(0, FirmwareData);
    if (!NT_SUCCESS(Status)) {
        goto Phase0Failed;
    }

    //
    // Save library parameters.
    //
    RtlCopyMemory(&BlpLibraryParameters, LibraryParameters, sizeof(BlpLibraryParameters));

    //
    // Internal options are external to the library.
    //
    Attributes = BootEntry->Attributes;
    if (Attributes & BOOT_ENTRY_OPTIONS_INTERNAL) {
        Attributes &= ~BOOT_ENTRY_OPTIONS_INTERNAL;
        Attributes |= BOOT_ENTRY_OPTIONS_EXTERNAL;
    }
    BlpApplicationEntry.Attributes = Attributes;

    //
    // Use whichever BCD identifier is available.
    //
    if (!(BlpApplicationEntry.Attributes & BOOT_ENTRY_NO_IDENTIFIER)) {
        RtlCopyMemory(&BlpApplicationEntry.Identifier, &BootEntry->Identifier, sizeof(BlpApplicationEntry.Identifier));
    } else if (LibraryParameters->Identifier != NULL) {
        RtlCopyMemory(&BlpApplicationEntry.Identifier, LibraryParameters->Identifier, sizeof(BlpApplicationEntry.Identifier));
        BlpApplicationIdentifierSet = TRUE;
    } else {
        RtlZeroMemory(&BlpApplicationEntry.Identifier, sizeof(BlpApplicationEntry.Identifier));
    }

    //
    // Use inline options.
    //
    BlpApplicationEntry.Options = &BootEntry->InlineOptions;

    //
    // Begin architecture-specific initialization.
    //
    Status = BlpArchInitialize(0);
    if (!NT_SUCCESS(Status)) {
        goto Phase0Failed;
    }

    //
    // Debug routines are available from this point onwards.
    //
    DebugInfo(L"Debug logging enabled\r\n");

    //
    // Initialize the memory manager.
    //
    Status = BlpMmInitialize(MemoryInfo, ApplicationParameters->TranslationType, LibraryParameters);
    if (!NT_SUCCESS(Status)) {
        goto Phase1Failed;
    }

    //
    // Initialize the event notification subsystem.
    //
    BlpEnInitialize();

    //
    // Complete firmware-specific initialization.
    //
    Status = BlpFwInitialize(1, FirmwareData);
    if (!NT_SUCCESS(Status)) {
        goto Phase2Failed;
    }

    //
    // TODO: Implement remaining functionality.
    //

    if (NT_SUCCESS(Status)) {
        DebugInfo(L"Boot library initialization completed successfully\r\n");
        return STATUS_SUCCESS;
    }

Phase2Failed:
    if (EnSubsystemInitialized) {
        BlpEnDestroy();
    }
    BlpMmDestroy(1);
Phase1Failed:
    ArchRestoreProcessorFeatures(TRUE);
Phase0Failed:
    return Status;
}

NTSTATUS
BlDestroyLibrary (
    VOID
    )

/*++

Routine Description:

    Shuts down the boot library.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS if successful.

    Error code if unsuccessful.

--*/

{
    NTSTATUS Status, ReturnStatus;

    DebugInfo(L"Destroying boot library...\r\n");

    ReturnStatus = STATUS_SUCCESS;

    if (EnSubsystemInitialized) {
        BlEnNotifyEvent(EVENT_LIBRARY_DESTRUCTION_BEGIN, NULL);
    }

    //
    // TODO: Implement remaining functionality.
    //

    if (EnSubsystemInitialized) {
        Status = BlpEnDestroy();
        if (!NT_SUCCESS(Status)) {
            ReturnStatus = Status;
        }
    }

    Status = BlpMmDestroy(1);
    if (!NT_SUCCESS(Status)) {
        ReturnStatus = Status;
    }

    ArchRestoreProcessorFeatures(TRUE);
    return ReturnStatus;
}

PGUID
BlGetApplicationIdentifier (
    VOID
    )

/*++

Routine Description:

    Gets the boot application's identifier.

Arguments:

    None.

Return Value:

    Pointer to the identifier if successful.

    NULL if unsuccessful.

--*/

{
    if (!(BlpApplicationEntry.Attributes & BOOT_ENTRY_NO_IDENTIFIER) || BlpApplicationIdentifierSet) {
        return &BlpApplicationEntry.Identifier;
    } else {
        return NULL;
    }
}

NTSTATUS
BlInitializeLibrary (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters,
    IN PBOOT_LIBRARY_PARAMETERS     LibraryParameters
    )

/*++

Routine Description:

    Initializes the boot library.

Arguments:

    ApplicationParameters - Pointer to the application parameters structure.

    LibraryParameters - Pointer to the library parameters structure.

Return Value:

    STATUS_SUCCESS if successful.

    Error code if unsuccessful.

--*/

{
    if (!(LibraryParameters->Flags & BOOT_LIBRARY_FLAG_REINITIALIZE)) {
        return InitializeLibrary(ApplicationParameters, LibraryParameters);
    }

    RtlCopyMemory(&BlpLibraryParameters, LibraryParameters, sizeof(BlpLibraryParameters));
    if (!(LibraryParameters->Flags & BOOT_LIBRARY_FLAG_REINITIALIZE_ALL)) {
        return STATUS_SUCCESS;
    }

    //
    // TODO: Implement remaining functionality.
    //

    return STATUS_SUCCESS;
}
