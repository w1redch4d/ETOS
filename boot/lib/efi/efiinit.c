/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efiinit.c

Abstract:

    EFI initialization routines.

--*/

#include "efilib.h"
#include <wchar.h>

UCHAR EfiInitScratch[2048];
const EFI_GUID EfiLoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
const EFI_GUID EfiDevicePathProtocol = EFI_DEVICE_PATH_PROTOCOL_GUID;
const EFI_GUID EfiPxeBaseCodeProtocol = EFI_PXE_BASE_CODE_PROTOCOL_GUID;
const EFI_GUID EfiVmbusChannelDevicePath = VMBUS_CHANNEL_DEVICE_GUID;

NTSTATUS
AhCreateLoadOptionsList (
    IN  PWSTR              OptionsString,
    IN  PBOOT_ENTRY_OPTION Options,
    IN  ULONG              BufferSize,
    OUT PULONG             OptionSize,
    IN  PBOOT_ENTRY_OPTION PreviousOption,
    IN  PULONG             PreviousOptionSize
    )

/*++

Routine Description:

    Parses a command-line option string and converts it into boot option structures.

Arguments:

    OptionsString - Pointer to the options string.

    Options - Pointer to the option buffer.

    BufferSize - The size of the option buffer.

    OptionSize - Pointer to a ULONG that recieves the size of the added option.

    PreviousOption - Pointer to the previous option.

    PreviousOptionSize - The size of the previous option.

Return Value:

    STATUS_NOT_IMPLEMENTED.

--*/

{
    (VOID) OptionsString;
    (VOID) Options;
    (VOID) BufferSize;
    (VOID) OptionSize;
    (VOID) PreviousOption;
    (VOID) PreviousOptionSize;

    //
    // TODO: Implement this routine.
    //

    EfiDebugPrint(L"Warning: AhCreateLoadOptionsList not implemented\r\n");
    return STATUS_NOT_IMPLEMENTED;
}

BOOLEAN
EfiVmbusGetChannelInterfaceGuids (
    IN  EFI_DEVICE_PATH *DevicePath,
    OUT PGUID           InterfaceType,
    OUT PGUID           InterfaceInstance
    )

/*++

Routine Description:

    Parses a command-line option string and converts it into boot option structures.

Arguments:

    DevicePath - Pointer to the VMBus device path.

    InterfaceType - Pointer to a GUID that recieves the interface type identifier.

    InterfaceInterface - Pointer to a GUID that recieves the interface instance identifier.

Return Value:

    TRUE if successful.

    FALSE if unsuccessful.

--*/

{
    EFI_DEVICE_PATH *Node;
    VMBUS_CHANNEL_DEVICE_PATH *VmbusChannelNode;

    Node = DevicePath;
    while (!IsDevicePathEndType(Node) && DevicePathSubType(Node) != END_ENTIRE_DEVICE_PATH_SUBTYPE) {
        if (DevicePathType(Node) == HARDWARE_DEVICE_PATH &&
            DevicePathSubType(Node) == HW_VENDOR_DP      &&
            IsEqualGUID((PGUID)&(((VENDOR_DEVICE_PATH *)Node)->Guid), (PGUID)&EfiVmbusChannelDevicePath)
            ) {

            VmbusChannelNode = (VMBUS_CHANNEL_DEVICE_PATH *)Node;
            RtlCopyMemory(InterfaceType, &VmbusChannelNode->InterfaceType, sizeof(*InterfaceType));
            RtlCopyMemory(InterfaceInstance, &VmbusChannelNode->InterfaceInstance, sizeof(*InterfaceInstance));

            return TRUE;
        }

        Node = NextDevicePathNode(Node);
    }

    return FALSE;
}

EFI_DEVICE_PATH *
EfiInitpGetDeviceNode (
    IN EFI_DEVICE_PATH *DevicePath
    )

/*++

Routine Description:

    Searches an EFI device path for the last device path node
    before a file path node.

Arguments:

    DevicePath - Pointer to the device path to search.

Return Value:

    The last non-filepath node.

--*/

{
    EFI_DEVICE_PATH *Node;

    if (IsDevicePathEndType(DevicePath)) {
        return DevicePath;
    }

    Node = NextDevicePathNode(DevicePath);
    while (!IsDevicePathEndType(Node)) {
        //
        // Stop early if we find a file path.
        //
        if (DevicePathType(Node) == MEDIA_DEVICE_PATH && DevicePathSubType(Node) == MEDIA_FILEPATH_DP) {
            return DevicePath;
        }

        DevicePath = Node;
        Node = NextDevicePathNode(Node);
    }

    return DevicePath;
}

NTSTATUS
EfiInitpAppendPathString (
    IN  PWCHAR Destination,
    IN  ULONG  BufferSize,
    IN  PWCHAR Source,
    IN  ULONG  SourceSize,
    OUT PULONG BufferUsed
    )

/*++

Routine Description:

    Appends a soure path to a destination path.

Arguments:

    Destination - Pointer to the path to append Source to.

    BufferSize - Maximum number of bytes to append to Destination.

    Source - Pointer to the path to append to Destination.

    SourceSize - Size of Source, in bytes.

    BufferUsed - Pointer to a ULONG that receives the number of bytes appended.

Return Value:

    STATUS_SUCCESS if successful,

    STATUS_INVALID_PARAMETER if SourceSize is invalid.

    STATUS_BUFFER_TOO_SMALL if BufferSize is too small.

--*/

{
    ULONG Position;

    //
    // Verify that Source uses wide characters.
    //
    if (SourceSize % sizeof(WCHAR) != 0) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Remove NULL terminator.
    //
    if (SourceSize >= sizeof(WCHAR)) {
        Position = (SourceSize / sizeof(WCHAR)) - 1;
        if (Source[Position] == UNICODE_NULL) {
            SourceSize -= sizeof(UNICODE_NULL);
        }
    }

    //
    // Remove leading separator.
    //
    if (SourceSize >= sizeof(WCHAR)) {
        if (Source[0] == L'\\') {
            Source++;
            SourceSize -= sizeof(WCHAR);
        }
    }

    //
    // Remove trailing separator.
    //
    if (SourceSize >= sizeof(WCHAR)) {
        Position = (SourceSize / sizeof(WCHAR)) - 1;
        if (Source[Position] == L'\\') {
            SourceSize -= sizeof(WCHAR);
        }
    }

    //
    // Check if Source is empty.
    //
    if (SourceSize == 0) {
        *BufferUsed = 0;
        return STATUS_SUCCESS;
    }

    //
    // Make sure the buffer is large enough.
    //
    if (BufferSize < SourceSize + sizeof(WCHAR)) {
        return STATUS_BUFFER_TOO_SMALL;
    }

    //
    // Append separator and Source to Destination.
    //
    Destination[0] = L'\\';
    RtlCopyMemory(Destination + 1, Source, SourceSize);

    *BufferUsed = SourceSize + sizeof(WCHAR);
    return STATUS_SUCCESS;
}

NTSTATUS
EfiInitpConvertEfiDevicePath (
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    )

/*++

Routine Description:

    Converts an EFI device path into a device option.

Arguments:

    EfiDevicePath - Pointer to the device path to be converted.

    OptionType - The type to assign to the option.

    Option - Pointer to a buffer that receives the option data.

    BufferSize - The number of bytes in the buffer.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if the buffer is too small.

    Any status code returned by EfiInitTranslateDevicePath.

--*/

{
    NTSTATUS Status;
    PBCDE_DEVICE DeviceElement;

    if (BufferSize < (sizeof(BOOT_ENTRY_OPTION) + FIELD_OFFSET(BCDE_DEVICE, Identifier))) {
        return STATUS_INVALID_PARAMETER;
    }

    RtlZeroMemory(Option, sizeof(*Option));
    DeviceElement = (PBCDE_DEVICE)((PUCHAR)Option + sizeof(BOOT_ENTRY_OPTION));
    Status = EfiInitTranslateDevicePath(
        EfiDevicePath,
        &DeviceElement->Identifier,
        BufferSize - (sizeof(BOOT_ENTRY_OPTION) + FIELD_OFFSET(BCDE_DEVICE, Identifier))
    );
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    Option->Type = OptionType;
    Option->DataOffset = sizeof(BOOT_ENTRY_OPTION);
    Option->DataSize = FIELD_OFFSET(BCDE_DEVICE, Identifier) + DeviceElement->Identifier.Size;

    return STATUS_SUCCESS;
}

NTSTATUS
EfiInitpConvertEfiFilePath (
    IN  EFI_DEVICE_PATH    *EfiFilePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    )

/*++

Routine Description:

    Converts an EFI file path into option format.

Arguments:

    EfiFilePath - Pointer to the file path to be converted.

    OptionType - The type to assign to the option.

    Option - Pointer to a buffer that receives the option data.

    BufferSize - The number of bytes in the buffer.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if the buffer is too small.

--*/

{
    NTSTATUS Status;
    EFI_DEVICE_PATH *Node;
    PWCHAR PathStart, Position;
    ULONG BufferRemaining, Length, Appended;

    if (BufferSize < sizeof(BOOT_ENTRY_OPTION)) {
        return STATUS_INVALID_PARAMETER;
    }

    RtlZeroMemory(Option, sizeof(*Option));
    Option->Type = OptionType;
    Option->DataOffset = sizeof(BOOT_ENTRY_OPTION);

    //
    // Loop through nodes and append one at a time.
    //
    Option->DataSize = 0;
    BufferRemaining = BufferSize - sizeof(BOOT_ENTRY_OPTION);
    Node = EfiFilePath;
    PathStart = (PWCHAR)((PUCHAR)Option + Option->DataOffset);
    Position = PathStart;
    while (!IsDevicePathEndType(Node)) {
        //
        // Ignore non-filepath nodes.
        //
        if (DevicePathType(Node) != MEDIA_DEVICE_PATH || DevicePathSubType(Node) != MEDIA_FILEPATH_DP) {
            Node = NextDevicePathNode(Node);
            continue;
        }

        //
        // Find the length of this path.
        //
        Status = RtlULongSub(DevicePathNodeLength(Node), FIELD_OFFSET(FILEPATH_DEVICE_PATH, PathName), &Length);
        if (!NT_SUCCESS(Status)) {
            return Status;
        }

        //
        // Append this path to the path string.
        //
        Status = EfiInitpAppendPathString(Position, BufferRemaining, &((FILEPATH_DEVICE_PATH *)Node)->PathName[0], Length, &Appended);
        if (!NT_SUCCESS(Status)) {
            return Status;
        }

        //
        // Update counters & position.
        //
        Option->DataSize += Appended;
        BufferRemaining -= Appended;
        Position = (PWCHAR)((PUCHAR)Position + Appended);
        Node = NextDevicePathNode(Node);
    }

    //
    // NULL-terminate path string.
    //
    if (BufferRemaining < sizeof(UNICODE_NULL)) {
        return STATUS_INVALID_PARAMETER;
    }
    *Position = UNICODE_NULL;
    Option->DataSize += sizeof(UNICODE_NULL);

    //
    // The option is invalid if the path is empty.
    //
    if (Position == PathStart) {
        Option->IsInvalid = TRUE;
        Option->DataSize = 0;
    }

    return STATUS_SUCCESS;
}

VOID
EfiInitpCreateApplicationEntry (
    IN  EFI_SYSTEM_TABLE                   *SystemTable,
    OUT PBOOT_APPLICATION_TRANSITION_ENTRY Entry,
    IN  ULONG                              BufferSize,
    IN  EFI_DEVICE_PATH                    *EfiDevicePath,
    IN  EFI_DEVICE_PATH                    *EfiFilePath,
    IN  PVOID                              LoadOptions,
    IN  ULONG                              LoadOptionsSize,
    IN  ULONG                              Flags,
    OUT PULONG                             BufferUsed,
    OUT PDEVICE_IDENTIFIER                 *BootDevice
    )

/*++

Routine Description:

    Creates an application entry for the boot application.

Arguments:

    SystemTable - Pointer to the firmware system table.

    Entry - Pointer to a buffer that receives the entry data.

    BufferSize - The number of bytes in the buffer.

    EfiDevicePath - Pointer to the application's device path.

    EfiFilePath - Pointer to the application's file path.

    LoadOptions - Pointer to the firmware load options string.

    LoadOptionsSize - The size in bytes of the string pointed to by LoadOptions.

    Flags - Ignored.

    BufferUsed - Pointer to a ULONG that receives the amount of buffer space used by this routine.

    BootDevice - Pointer to a PDEVICE_IDENTIFIER that receives the device the application was loaded from.

Return Value:

    None.

--*/

{
    NTSTATUS Status;
    PWINDOWS_OS_OPTIONS WindowsOptions;
    BOOLEAN UsingWindowsOptions, BcdIdentifierSet;
    PWSTR OptionsString;
    ULONG OptionsStringLength, OptionsSize, Size, BufferRemaining;
    PWSTR BcdIdentifierOption;
    PBOOT_ENTRY_OPTION Option, PreviousOption;
    UNICODE_STRING UnicodeString;
    PBCDE_DEVICE BootDeviceElement;
    PWINDOWS_OS_PATH OsPath;
    EFI_DEVICE_PATH *OsDevicePath;

    (VOID) Flags;

    //
    // Clear outputs in case an error occurs.
    //
    *BufferUsed = 0;
    *BootDevice = NULL;

    //
    // Check if buffer is large enough.
    //
    if (BufferSize < sizeof(BOOT_APPLICATION_TRANSITION_ENTRY)) {
        return;
    }

    //
    // Check for Windows-specific options.
    //
    WindowsOptions = (PWINDOWS_OS_OPTIONS)LoadOptions;
    if (
        WindowsOptions != NULL                             &&
        LoadOptionsSize >= sizeof(WINDOWS_OS_OPTIONS)      &&
        WindowsOptions->Size >= sizeof(WINDOWS_OS_OPTIONS) &&
        WindowsOptions->Signature == WINDOWS_OS_OPTIONS_SIGNATURE
        ) {
        EfiDebugPrint(L"Detected Windows boot options format\r\n");
        UsingWindowsOptions = TRUE;
        OptionsString = (PWSTR)WindowsOptions->Options;
        OptionsStringLength = LoadOptionsSize - FIELD_OFFSET(WINDOWS_OS_OPTIONS, Options);
    } else {
        UsingWindowsOptions = FALSE;
        OptionsString = (PWSTR)LoadOptions;
        OptionsStringLength = LoadOptionsSize;
    }

    //
    // Process options string.
    //
    OptionsStringLength /= sizeof(WCHAR);
    if (OptionsStringLength > 0 && wcsnlen(OptionsString, OptionsStringLength) == OptionsStringLength) {
        OptionsString[OptionsStringLength - 1] = UNICODE_NULL;
    }
    EfiDebugPrintf(L"Options: \"%s\"\r\n", OptionsString);

    //
    // Initialize entry structure.
    //
    RtlZeroMemory(Entry, sizeof(*Entry));
    Entry->Signature = BOOT_APPLICATION_TRANSITION_ENTRY_SIGNATURE;
    Entry->Attributes |= BOOT_APPLICATION_ENTRY_ATTRIBUTE_UNKNOWN_8000;

    //
    // Parse BCD identifier option.
    //
    BcdIdentifierSet = FALSE;
    if (LoadOptions != NULL && (BcdIdentifierOption = wcsstr(OptionsString, L"BCDOBJECT=")) != NULL) {
        EfiDebugPrint(L"Found BCDOBJECT option\r\n");
        RtlInitUnicodeString(&UnicodeString, (PWSTR)((PUCHAR)BcdIdentifierOption + sizeof(L"BCDOBJECT=") - sizeof(UNICODE_NULL)));
        Status = RtlGUIDFromString(&UnicodeString, &Entry->BcdIdentifier);
        if (NT_SUCCESS(Status)) {
            BcdIdentifierSet = TRUE;
        }
    }

    if (!BcdIdentifierSet) {
        Entry->Attributes |= BOOT_APPLICATION_ENTRY_ATTRIBUTE_NO_BCD_IDENTIFIER;
    }

    OptionsSize = 0;
    BufferRemaining = BufferSize - FIELD_OFFSET(BOOT_APPLICATION_TRANSITION_ENTRY, InlineOptions);

    //
    // Convert the EFI device path into a boot device option.
    //
    Option = &Entry->InlineOptions;
    Status = EfiInitpConvertEfiDevicePath(
        EfiDevicePath,
        BCDE_LIBRARY_TYPE_APPLICATION_DEVICE,
        Option,
        BufferRemaining
    );
    if (!NT_SUCCESS(Status)) {
        EfiDebugPrint(L"Failed to convert boot application device path\r\n");
        Option->IsInvalid = TRUE;
        goto Quit;
    }
    BootDeviceElement = (PBCDE_DEVICE)((PUCHAR)Option + Option->DataOffset);
    *BootDevice = &BootDeviceElement->Identifier;
    Size = BlGetBootOptionSize(Option);
    OptionsSize += Size;
    BufferRemaining -= Size;

    //
    // Convert the EFI file path into a boot file path option.
    //
    PreviousOption = Option;
    Option = (PBOOT_ENTRY_OPTION)((PUCHAR)&Entry->InlineOptions + OptionsSize);
    if (BootDeviceElement->Identifier.Type == DEVICE_TYPE_NETWORK) {
        Status = EfiInitConvertEfiPxeFilePath(
            SystemTable,
            EfiDevicePath,
            BCDE_LIBRARY_TYPE_APPLICATION_PATH,
            Option,
            BufferRemaining
        );
    } else {
        Status = EfiInitpConvertEfiFilePath(
            EfiFilePath,
            BCDE_LIBRARY_TYPE_APPLICATION_PATH,
            Option,
            BufferRemaining
        );
    }
    if (!NT_SUCCESS(Status)) {
        EfiDebugPrint(L"Failed to convert boot application file path\r\n");
        goto Quit;
    }
    PreviousOption->NextOptionOffset = OptionsSize;
    Size = BlGetBootOptionSize(Option);
    OptionsSize += Size;
    BufferRemaining -= Size;

    //
    // Try to find BCD identifier.
    //
    if (UsingWindowsOptions && !BcdIdentifierSet) {
        OsPath = (PWINDOWS_OS_PATH)((PUCHAR)WindowsOptions + WindowsOptions->OsPathOffset);
        if (OsPath->Length > FIELD_OFFSET(WINDOWS_OS_PATH, Data) && OsPath->Type == WINDOWS_OS_PATH_TYPE_EFI) {
            OsDevicePath = (EFI_DEVICE_PATH *)OsPath->Data;

            //
            // Convert OS loader device path.
            //
            PreviousOption = Option;
            Option = (PBOOT_ENTRY_OPTION)((PUCHAR)&Entry->InlineOptions + OptionsSize);
            Status = EfiInitpConvertEfiDevicePath(
                OsDevicePath,
                BCDE_APPLICATION_TYPE_APPLICATION_DEVICE,
                Option,
                BufferRemaining
            );
            if (!NT_SUCCESS(Status)) {
                EfiDebugPrint(L"Failed to convert OS loader device path\r\n");
                goto Quit;
            }
            PreviousOption->NextOptionOffset = OptionsSize;
            Size = BlGetBootOptionSize(Option);
            OptionsSize += Size;
            BufferRemaining -= Size;

            //
            // Convert OS loader file path.
            //
            PreviousOption = Option;
            Option = (PBOOT_ENTRY_OPTION)((PUCHAR)&Entry->InlineOptions + OptionsSize);
            Status = EfiInitpConvertEfiFilePath(
                OsDevicePath,
                BCDE_APPLICATION_TYPE_APPLICATION_PATH,
                Option,
                BufferRemaining
            );
            if (!NT_SUCCESS(Status)) {
                EfiDebugPrint(L"Failed to convert OS loader file path\r\n");
                goto Quit;
            }
            PreviousOption->NextOptionOffset = OptionsSize;
            Size = BlGetBootOptionSize(Option);
            OptionsSize += Size;
            BufferRemaining -= Size;
        }
    }

    if (OptionsString != NULL) {
        PreviousOption = Option;
        AhCreateLoadOptionsList(
            OptionsString,
            &Entry->InlineOptions,
            BufferRemaining,
            &OptionsSize,
            PreviousOption,
            &Size
        );
    }

Quit:
    *BufferUsed = BufferSize - BufferRemaining;
}

NTSTATUS
EfiInitTranslateDevicePath (
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    OUT PDEVICE_IDENTIFIER BootDevice,
    IN  ULONG              BufferSize
    )

/*++

Routine Description:

    Translates an EFI_DEVICE_PATH into a DEVICE_IDENTIFIER.

Arguments:

    EfiDevicePath - Pointer to the device path to be translated.

    BootDevice - Pointer to a buffer that receives the device data.

    BufferSize - The number of bytes in the buffer.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if the buffer is too small.

    STATUS_UNSUCCESSFUL if the path could not be translated.

--*/

{
    EFI_DEVICE_PATH *DeviceNode;
    MEMMAP_DEVICE_PATH *MemmapNode;
    ACPI_HID_DEVICE_PATH *AcpiHidNode;
    HARDDRIVE_DEVICE_PATH *HarddriveNode;
    PBLOCK_DEVICE_IDENTIFIER BlockDevice;

    if (BufferSize < sizeof(DEVICE_IDENTIFIER)) {
        return STATUS_INVALID_PARAMETER;
    }
    BootDevice->Size = sizeof(DEVICE_IDENTIFIER);

    //
    // Memory map devices are treated as ram disks.
    //
    if (DevicePathType(EfiDevicePath) == HARDWARE_DEVICE_PATH && DevicePathSubType(EfiDevicePath) == HW_MEMMAP_DP) {
        MemmapNode = (MEMMAP_DEVICE_PATH *)EfiDevicePath;
        BootDevice->Type = DEVICE_TYPE_BLOCK;
        BlockDevice = &BootDevice->BlockDevice;
        BlockDevice->Type = BLOCK_DEVICE_TYPE_RAM_DISK;
        BlockDevice->RamDisk.ImageBase.QuadPart = MemmapNode->StartingAddress;
        BlockDevice->RamDisk.ImageSize = MemmapNode->EndingAddress - MemmapNode->StartingAddress;
        BlockDevice->RamDisk.ImageOffset = 0;
        return STATUS_SUCCESS;
    }

    //
    // Now, we only care about the last node on the path.
    //
    DeviceNode = EfiInitpGetDeviceNode(EfiDevicePath);

    //
    // Certain ACPI devices are treated as removable disks.
    //
    if (DevicePathType(DeviceNode) == ACPI_DEVICE_PATH) {
        AcpiHidNode = (ACPI_HID_DEVICE_PATH *)DeviceNode;
        if (AcpiHidNode->HID != EISA_PNP_ID(0x604) && AcpiHidNode->HID != EISA_PNP_ID(0x700)) {
            EfiDebugPrintf(L"Unrecognized ACPI device (HID %x)\r\n", AcpiHidNode->HID);
            return STATUS_UNSUCCESSFUL;
        }

        BootDevice->Type = DEVICE_TYPE_BLOCK;
        BlockDevice = &BootDevice->BlockDevice;
        BlockDevice->Type = BLOCK_DEVICE_TYPE_REMOVABLE_DISK;
        BlockDevice->RemovableDisk.DriveNumber = AcpiHidNode->UID;
        return STATUS_SUCCESS;
    }

    //
    // Messaging devices are used for network booting.
    //
    if (DevicePathType(DeviceNode) == MESSAGING_DEVICE_PATH) {
        switch (DevicePathSubType(DeviceNode)) {
        case MSG_MAC_ADDR_DP:
        case MSG_IPv4_DP:
        case MSG_IPv6_DP:
            BootDevice->Type = DEVICE_TYPE_NETWORK;
            BootDevice->NetworkDevice.Type = NETWORK_DEVICE_TYPE_UDP;
            break;
        case MSG_URI_DP:
            BootDevice->Type = DEVICE_TYPE_URI;
            BootDevice->Uri.UriLength = DevicePathNodeLength(DeviceNode) - FIELD_OFFSET(URI_DEVICE_PATH, Uri);
            BootDevice->Size = FIELD_OFFSET(DEVICE_IDENTIFIER, Uri.Uri) + BootDevice->Uri.UriLength;
            if (BufferSize >= BootDevice->Size) {
                RtlMoveMemory(BootDevice->Uri.Uri, ((URI_DEVICE_PATH *)DeviceNode)->Uri, BootDevice->Uri.UriLength);
            }

            break;
        default:
            BootDevice->Type = DEVICE_TYPE_BLOCK;
            BlockDevice = &BootDevice->BlockDevice;
            BlockDevice->Type = BLOCK_DEVICE_TYPE_REMOVABLE_DISK;
            BlockDevice->RemovableDisk.DriveNumber = 0;
            break;
        }

        return STATUS_SUCCESS;
    }

    //
    // VMBus devices are used in virtual machines.
    //
    if (DevicePathType(DeviceNode) == HARDWARE_DEVICE_PATH) {
        if (!EfiVmbusGetChannelInterfaceGuids(DeviceNode, &BootDevice->VmbusChannel.InterfaceType, &BootDevice->VmbusChannel.InterfaceInstance)) {
            return STATUS_UNSUCCESSFUL;
        }

        BootDevice->Type = DEVICE_TYPE_VMBUS_CHANNEL;
        return STATUS_SUCCESS;
    }

    //
    // Only media devices should be left now.
    //
    if (DevicePathType(DeviceNode) != MEDIA_DEVICE_PATH) {
        EfiDebugPrintf(L"Unsupported boot device (type %x)\r\n", DevicePathType(DeviceNode));
        return STATUS_UNSUCCESSFUL;
    }

    switch (DevicePathSubType(DeviceNode)) {
    case MEDIA_HARDDRIVE_DP:
        HarddriveNode = (HARDDRIVE_DEVICE_PATH *)DeviceNode;

        //
        // MBR disks still use the old partition structure.
        //
        if (HarddriveNode->SignatureType != SIGNATURE_TYPE_MBR) {
            BootDevice->Type = DEVICE_TYPE_PARTITION_EX;
            BlockDevice = &BootDevice->PartitionEx.Parent;
        } else {
            BootDevice->Type = DEVICE_TYPE_PARTITION;
            BlockDevice = &BootDevice->Partition.Parent;
        }
        BlockDevice->Type = BLOCK_DEVICE_TYPE_HARD_DISK;

        //
        // Add information identifying the partition.
        //
        switch (HarddriveNode->SignatureType) {
        case SIGNATURE_TYPE_GUID:
            BlockDevice->HardDisk.PartitionType = HARD_DISK_PARTITION_TYPE_GPT;
            RtlCopyMemory(&BootDevice->PartitionEx.Gpt.Guid, &HarddriveNode->Signature, sizeof(GUID));
            BootDevice->Attributes |= DEVICE_ATTRIBUTE_NO_PARENT_SIGNATURE;
            break;
        case SIGNATURE_TYPE_MBR:
            BlockDevice->HardDisk.PartitionType = HARD_DISK_PARTITION_TYPE_MBR;
            BlockDevice->HardDisk.Mbr.Signature = *((ULONG *)&HarddriveNode->Signature);
            BootDevice->Partition.Mbr.PartitionNumber = HarddriveNode->PartitionNumber;
            break;
        default:
            BlockDevice->HardDisk.PartitionType = HARD_DISK_PARTITION_TYPE_NONE;
            BlockDevice->HardDisk.Raw.DriveNumber = 0;
            break;
        }

        break;
    case MEDIA_CDROM_DP:
        BootDevice->Type = DEVICE_TYPE_BLOCK;
        BootDevice->BlockDevice.Type = BLOCK_DEVICE_TYPE_CDROM;
        BootDevice->BlockDevice.Cdrom.DriveNumber = 0;
        break;
    default:
        EfiDebugPrintf(L"Unsupported boot device (media subtype %x)\r\n", DevicePathSubType(DeviceNode));
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}

NTSTATUS
EfiInitConvertEfiPxeFilePath (
    IN  EFI_SYSTEM_TABLE   *SystemTable,
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    )

/*++

Routine Description:

    Converts a network device path to a file path.

Arguments:

    SystemTable - Pointer to the firmware system table.

    EfiDevicePath - Pointer to the application's device path.

    OptionType - The type to assign to the option.

    Option - Pointer to a buffer that receives the option data.

    BufferSize - The number of bytes in the buffer.

Return Value:

    STATUS_NOT_IMPLEMENTED.

--*/

{
    EFI_STATUS Status;
    EFI_DEVICE_PATH *DevicePath;
    EFI_HANDLE DeviceHandle;
    EFI_PXE_BASE_CODE *PxeBaseCode;
    EFI_PXE_BASE_CODE_MODE *Mode;
    PCHAR BootFile;
    ULONG BootFileLength, TotalSize;

    (VOID) OptionType;

    if (BufferSize < sizeof(BOOT_ENTRY_OPTION)) {
        return STATUS_INVALID_PARAMETER;
    }

    RtlZeroMemory(Option, sizeof(*Option));
    Option->Type = BCDE_LIBRARY_TYPE_APPLICATION_PATH;
    Option->DataOffset = sizeof(BOOT_ENTRY_OPTION);

    //
    // Get the device handle for the base code protocol.
    //
    DevicePath = EfiDevicePath;
    Status = SystemTable->BootServices->LocateDevicePath(
        (EFI_GUID *)&EfiPxeBaseCodeProtocol,
        &DevicePath,
        &DeviceHandle
    );
    if (Status != EFI_SUCCESS) {
        EfiDebugPrintf(L"Failed to locate PXE base code device path (Status=%x)\r\n", Status);
        return STATUS_INVALID_PARAMETER;
    }

    //
    // TODO: Check for EFI 1.1+ before using OpenProtocol().
    //
    EfiDebugPrint(L"Warning: not checking for OpenProtocol support\r\n");

    //
    // Open the protocol using the handle.
    //
    Status = SystemTable->BootServices->OpenProtocol(
        DeviceHandle,
        (EFI_GUID *)&EfiPxeBaseCodeProtocol,
        (VOID **)&PxeBaseCode,
        EfiImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if (Status != EFI_SUCCESS) {
        EfiDebugPrintf(L"Failed to open PXE base code protocol (Status=%x)\r\n", Status);
        return STATUS_INVALID_PARAMETER;
    }

    Mode = PxeBaseCode->Mode;
    if (Mode->UsingIpv6 || (Mode->ProxyOffer.Dhcpv4.BootpBootFile[0] == '\0' && Mode->DhcpAck.Dhcpv4.BootpBootFile[0] == '\0')) {
        EfiDebugPrint(L"Invalid or unsupported PXE base code mode\r\n");
        Option->IsInvalid = TRUE;
        return STATUS_SUCCESS;
    }

    BootFile = (PCHAR)Mode->DhcpAck.Dhcpv4.BootpBootFile;
    BootFileLength = (ULONG)strlen(BootFile);
    TotalSize = sizeof(BOOT_ENTRY_OPTION) + ((BootFileLength * sizeof(WCHAR)) + sizeof(UNICODE_NULL));

    if (BootFile[0] != '\\') {
        TotalSize += sizeof(L'\\');
        // *Destination++ = L'\\';
    }

    if (BufferSize < TotalSize) {
        return STATUS_INVALID_PARAMETER;
    }

    // BlCopyStringToWcharString(Destination);
    Option->DataSize = TotalSize;

    //
    // TODO: Finish implementing this routine.
    //

    return STATUS_NOT_IMPLEMENTED;
}

PBOOT_APPLICATION_PARAMETERS
EfiInitCreateInputParametersEx (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable,
    IN ULONG            Flags
    )

/*++

Routine Description:

    Creates the input parameters structure for the boot application.

Arguments:

    ImageHandle - Firmware handle for the boot application image.

    SystemTable - Pointer to the firmware system table.

    Flags - Passed to EfiInitpCreateApplicationEntry.

Return Value:

    Pointer to parameter structure if successful.

    NULL if unsuccessful.

--*/

{
    EFI_STATUS Status;
    EFI_PHYSICAL_ADDRESS BadPageAddress;
    EFI_LOADED_IMAGE *LoadedImage;
    EFI_DEVICE_PATH *DevicePath;
    PBOOT_APPLICATION_PARAMETERS InputParameters;
    ULONG ScratchUsed, BootEntrySize;
    PMEMORY_INFO MemoryInfo;
    PMEMORY_DESCRIPTOR MemoryDescriptor;
    PDEVICE_IDENTIFIER BootDevice;
    PFIRMWARE_DATA FirmwareData;
    PRETURN_DATA ReturnData;

    BlpApplicationFlags |= BOOT_APPLICATION_FLAG_LOADED_BY_FIRMWARE;

    //
    // Page 0x102 may be broken on some machines.
    // It is mapped early on so that it does not get used.
    //
    BadPageAddress = 0x102 << EFI_PAGE_SHIFT;
    SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, 1, &BadPageAddress);

    //
    // Get image information from firmware.
    //
    Status = SystemTable->BootServices->HandleProtocol(
        ImageHandle,
        (EFI_GUID *)&EfiLoadedImageProtocol,
        (VOID **)&LoadedImage
    );
    if (Status != EFI_SUCCESS) {
        EfiDebugPrintf(L"Failed to get boot application image information (Status=0x%x)\r\n", Status);
        return NULL;
    }
#if defined(_WIN64)
    EfiDebugPrintf(L"Image base: %x %x\r\n", HIDWORD((EFI_VIRTUAL_ADDRESS)LoadedImage->ImageBase), LODWORD((EFI_VIRTUAL_ADDRESS)LoadedImage->ImageBase));
#else
    EfiDebugPrintf(L"Image base: %x\r\n", (EFI_VIRTUAL_ADDRESS)LoadedImage->ImageBase);
#endif
    EfiDebugPrintf(L"Image size: %x %x\r\n", HIDWORD(LoadedImage->ImageSize), LODWORD(LoadedImage->ImageSize));

    //
    // Get boot device information from firmware.
    //
    Status = SystemTable->BootServices->HandleProtocol(
        LoadedImage->DeviceHandle,
        (EFI_GUID *)&EfiDevicePathProtocol,
        (VOID **)&DevicePath
    );
    if (Status != EFI_SUCCESS) {
        EfiDebugPrintf(L"Failed to get boot application device path (Status=0x%x)\r\n", Status);
        return NULL;
    }

    //
    // Create application parameters structure.
    //
    InputParameters = (PBOOT_APPLICATION_PARAMETERS)&EfiInitScratch[0];
    ScratchUsed = sizeof(BOOT_APPLICATION_PARAMETERS);
    InputParameters->Signature = BOOT_APPLICATION_PARAMETERS_SIGNATURE;
    InputParameters->Version = BOOT_APPLICATION_PARAMETERS_VERSION;
    InputParameters->MachineType = BOOT_MACHINE_TYPE;
    InputParameters->TranslationType = TRANSLATION_TYPE_NONE;
    InputParameters->ImageBase = LoadedImage->ImageBase;
    InputParameters->ImageSize = LoadedImage->ImageSize;

    //
    // Add memory info structure.
    //
    InputParameters->MemoryInfoOffset = ScratchUsed;
    MemoryInfo = (PMEMORY_INFO)&EfiInitScratch[ScratchUsed];
    ScratchUsed += sizeof(MEMORY_INFO);
    MemoryInfo->Version = MEMORY_INFO_VERSION;
    MemoryInfo->MdlOffset = sizeof(MEMORY_INFO);
    MemoryInfo->DescriptorCount = 1;
    MemoryInfo->DescriptorSize = sizeof(MEMORY_DESCRIPTOR);
    MemoryInfo->DescriptorOffset = FIELD_OFFSET(MEMORY_DESCRIPTOR, FirstPage);

    //
    // Add a memory descriptor for the boot application image.
    //
    MemoryDescriptor = (PMEMORY_DESCRIPTOR)&EfiInitScratch[ScratchUsed];
    ScratchUsed += sizeof(MEMORY_DESCRIPTOR);
    MemoryDescriptor->FirstPage = (UINTN)InputParameters->ImageBase >> EFI_PAGE_SHIFT;
    MemoryDescriptor->PageCount = EFI_SIZE_TO_PAGES(InputParameters->ImageSize);
    MemoryDescriptor->Attributes = MEMORY_ATTRIBUTE_WB;
    MemoryDescriptor->MemoryType = MEMORY_TYPE_BOOT_APPLICATION;

    //
    // Create a transition entry for the boot application.
    //
    InputParameters->BootEntryOffset = ScratchUsed;
    EfiInitpCreateApplicationEntry(
        SystemTable,
        (PBOOT_APPLICATION_TRANSITION_ENTRY)&EfiInitScratch[ScratchUsed],
        sizeof(EfiInitScratch) - ScratchUsed,
        DevicePath,
        LoadedImage->FilePath,
        LoadedImage->LoadOptions,
        LoadedImage->LoadOptionsSize,
        Flags,
        &BootEntrySize,
        &BootDevice
    );
    ScratchUsed += BootEntrySize;

    //
    // Copy the boot device, if present.
    //
    InputParameters->BootDeviceOffset = ScratchUsed;
    if (BootDevice != NULL) {
        RtlCopyMemory(&EfiInitScratch[ScratchUsed], BootDevice, BootDevice->Size);
        ScratchUsed += BootDevice->Size;
    } else {
        RtlZeroMemory(&EfiInitScratch[ScratchUsed], sizeof(DEVICE_IDENTIFIER));
        ScratchUsed += sizeof(DEVICE_IDENTIFIER);
    }

    //
    // Add firmware data structure.
    //
    InputParameters->FirmwareDataOffset = ScratchUsed;
    FirmwareData = (PFIRMWARE_DATA)&EfiInitScratch[ScratchUsed];
    ScratchUsed += sizeof(FIRMWARE_DATA);
    RtlZeroMemory(FirmwareData, sizeof(*FirmwareData));
    FirmwareData->Version = FIRMWARE_DATA_VERSION;
    FirmwareData->ImageHandle = ImageHandle;
    FirmwareData->SystemTable = SystemTable;
#if defined(__x86_64__) || defined(__i386__)
    asm volatile("mov %%cr3, %0" :"=r"(FirmwareData->Cr3));
    BlpArchGetDescriptorTableContext(&FirmwareData->DescriptorTableContext);
#endif

    //
    // Add return data structure.
    //
    InputParameters->ReturnDataOffset = ScratchUsed;
    ReturnData = (PRETURN_DATA)&EfiInitScratch[ScratchUsed];
    ScratchUsed += sizeof(RETURN_DATA);
    ReturnData->Version = RETURN_DATA_VERSION;

    //
    // Detect buffer overflow.
    //
    if (ScratchUsed > sizeof(EfiInitScratch)) {
        EfiDebugPrintf(L"EfiInitScratch buffer overflow (%x/%x bytes used)\r\n", ScratchUsed, sizeof(EfiInitScratch));
        return NULL;
    }

    InputParameters->TotalSize = ScratchUsed;
    return InputParameters;
}

PBOOT_APPLICATION_PARAMETERS
EfiInitCreateInputParameters (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )

/*++

Routine Description:

    Creates the input parameters structure for the boot application.

Arguments:

    ImageHandle - Firmware handle for the boot application image.

    SystemTable - Pointer to the firmware system table.

Return Value:

    Pointer to parameter structure if successful.

    NULL if unsuccessful.

--*/

{
    return EfiInitCreateInputParametersEx(ImageHandle, SystemTable, 0);
}
