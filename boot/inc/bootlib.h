/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bootlib.h

Abstract:

    Provides boot library definitions.

--*/

#pragma once

#ifndef _BOOTLIB_H
#define _BOOTLIB_H

#if defined(_EFI)
#include "efi.h"

#define PAGE_SIZE  EFI_PAGE_SIZE
#define PAGE_MASK  EFI_PAGE_MASK
#define PAGE_SHIFT EFI_PAGE_SHIFT
#endif

#include <nt.h>
#include <ntrtl.h>

//
// Set machine type.
//
#if defined(__x86_64__)
#define BOOT_MACHINE_TYPE IMAGE_FILE_MACHINE_AMD64
#elif defined(__i386__)
#define BOOT_MACHINE_TYPE IMAGE_FILE_MACHINE_I386
#else
#warning Unable to set machine type
#define BOOT_MACHINE_TYPE IMAGE_FILE_MACHINE_UNKNOWN
#endif

//
// Descriptor table context.
//
#if defined(__x86_64__) || defined(__i386__)
#pragma pack(push,1)

typedef struct {
    USHORT    Limit;
    ULONG_PTR Base;
} DESCRIPTOR_TABLE_REGISTER, *PDESCRIPTOR_TABLE_REGISTER;

typedef struct {
    DESCRIPTOR_TABLE_REGISTER Gdtr;
    DESCRIPTOR_TABLE_REGISTER Idtr;
    USHORT                    LdtSelector;
    USHORT                    Cs, Ds, Es, Fs, Gs, Ss;
} DESCRIPTOR_TABLE_CONTEXT, *PDESCRIPTOR_TABLE_CONTEXT;

#pragma pack(pop)

VOID
BlpArchGetDescriptorTableContext (
    PDESCRIPTOR_TABLE_CONTEXT Context
    );
#endif

//
// Address translation types.
//

#define TRANSLATION_TYPE_NONE    0
#define TRANSLATION_TYPE_VIRTUAL 1
#define TRANSLATION_TYPE_MAX     2

//
// Memory type.
//
typedef ULONG MEMORY_TYPE;

//
// Memory type classes.
//
#define MEMORY_CLASS_APPLICATION 0xd
#define MEMORY_CLASS_LIBRARY     0xe
#define MEMORY_CLASS_SYSTEM      0xf

//
// Memory types.
//
#define MEMORY_TYPE_BOOT_APPLICATION      0xd0000002
#define MEMORY_TYPE_UNKNOWN_D0000013      0xd0000013
#define MEMORY_TYPE_FREE                  0xf0000001
#define MEMORY_TYPE_UNUSABLE              0xf0000002
#define MEMORY_TYPE_RESERVED              0xf0000003
#define MEMORY_TYPE_BOOT_SERVICES         0xf0000004
#define MEMORY_TYPE_FREE_ZEROED           0xf0000005
#define MEMORY_TYPE_RUNTIME_SERVICES_CODE 0xf0000006
#define MEMORY_TYPE_PERSISTENT            0xf0000007
#define MEMORY_TYPE_ACPI_RECLAIM          0xf0000008
#define MEMORY_TYPE_ACPI_NVS              0xf0000009
#define MEMORY_TYPE_MMIO                  0xf000000a
#define MEMORY_TYPE_MMIO_PORT_SPACE       0xf000000b
#define MEMORY_TYPE_PAL_CODE              0xf000000c
#define MEMORY_TYPE_RUNTIME_SERVICES_DATA 0xf000000e

//
// Memory caching attributes.
//
#define MEMORY_ATTRIBUTE_UC  0x00000001
#define MEMORY_ATTRIBUTE_WC  0x00000002
#define MEMORY_ATTRIBUTE_WT  0x00000004
#define MEMORY_ATTRIBUTE_WB  0x00000008
#define MEMORY_ATTRIBUTE_UCE 0x00000010  

//
// Memory protection attributes.
//
#define MEMORY_ATTRIBUTE_WP  0x00000100
#define MEMORY_ATTRIBUTE_RP  0x00000200
#define MEMORY_ATTRIBUTE_XP  0x00000400

//
// Memory descriptor.
//
typedef struct {
    LIST_ENTRY  ListEntry;

    ULONG_PTR   FirstPage;
    ULONG_PTR   VirtualFirstPage;
    ULONG_PTR   PageCount;

    ULONG       Attributes;
    MEMORY_TYPE MemoryType;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

//
// Memory descriptor list.
//

typedef enum {
    MDL_TYPE_PHYSICAL,
    MDL_TYPE_VIRTUAL
} MEMORY_DESCRIPTOR_LIST_TYPE;

typedef struct {
    LIST_ENTRY                  ListEntry;

    PLIST_ENTRY                 Head;
    PLIST_ENTRY                 Current;
    MEMORY_DESCRIPTOR_LIST_TYPE Type;
} MEMORY_DESCRIPTOR_LIST, *PMEMORY_DESCRIPTOR_LIST;

//
// Hard disk identifier.
//

#define HARD_DISK_PARTITION_TYPE_GPT  0
#define HARD_DISK_PARTITION_TYPE_MBR  1
#define HARD_DISK_PARTITION_TYPE_NONE 2

typedef struct {
    ULONG PartitionType;

    union {
        struct {
            ULONG Signature;
        } Mbr;

        struct {
            GUID Guid;
        } Gpt;

        struct {
            ULONG DriveNumber;
        } Raw;
    };
} HARD_DISK_IDENTIFIER, *PHARD_DISK_IDENTIFIER;

//
// Removable disk identifier.
//
typedef struct {
    ULONG DriveNumber;
} REMOVABLE_DISK_IDENTIFIER, *PREMOVABLE_DISK_IDENTIFIER;

//
// CD-ROM identifier.
//
typedef struct {
    ULONG DriveNumber;
} CDROM_IDENTIFIER, *PCDROM_IDENTIFIER;

//
// RAM disk identifier.
//
typedef struct {
    LARGE_INTEGER ImageBase;
    ULONGLONG     ImageSize;
    ULONG         ImageOffset;
} RAM_DISK_IDENTIFIER, *PRAM_DISK_IDENTIFIER;

//
// Block device identifier.
//

#define BLOCK_DEVICE_TYPE_HARD_DISK      0
#define BLOCK_DEVICE_TYPE_REMOVABLE_DISK 1
#define BLOCK_DEVICE_TYPE_CDROM          2
#define BLOCK_DEVICE_TYPE_RAM_DISK       3

typedef struct {
    ULONG Type;

    union {
        HARD_DISK_IDENTIFIER      HardDisk;
        REMOVABLE_DISK_IDENTIFIER RemovableDisk;
        CDROM_IDENTIFIER          Cdrom;
        RAM_DISK_IDENTIFIER       RamDisk;
    };
} BLOCK_DEVICE_IDENTIFIER, *PBLOCK_DEVICE_IDENTIFIER;

//
// Partition identifier.
//
typedef struct {
    union {
        struct {
            ULONG PartitionNumber;
        } Mbr;

        struct {
            GUID Guid;
        } Gpt;

        struct {
            ULONG BootEntryNumber;
        } ElTorito;
    };

    BLOCK_DEVICE_IDENTIFIER Parent;
} PARTITION_IDENTIFIER, *PPARTITION_IDENTIFIER;

//
// Extended partition identifier.
//
typedef struct {
    union {
        struct {
            ULONG_PTR PartitionOffset;
        } Mbr;

        struct {
            GUID Guid;
        } Gpt;

        struct {
            ULONG BootEntryNumber;
        } ElTorito;
    };

    BLOCK_DEVICE_IDENTIFIER Parent;
} PARTITION_IDENTIFIER_EX, *PPARTITION_IDENTIFIER_EX;

//
// Network device identifier.
//

#define NETWORK_DEVICE_TYPE_UDP 0x00000100

typedef struct {
    ULONG Type;
    UCHAR Address[16];
} NETWORK_DEVICE_IDENTIFIER, *PNETWORK_DEVICE_IDENTIFIER;

//
// VMBus (Virtual Machine Bus) channel identifier.
//
typedef struct {
    GUID InterfaceType;
    GUID InterfaceInstance;
} VMBUS_CHANNEL_IDENTIFIER, *PVMBUS_CHANNEL_IDENTIFIER;

//
// URI (Uniform Resource Identifier) identifier.
//
typedef struct {
    ULONG UriLength;
    CHAR  Uri[ANYSIZE_ARRAY];
} URI_IDENTIFIER, *PURI_IDENTIFIER;

//
// Device identifier.
//

#define DEVICE_TYPE_BLOCK         0
#define DEVICE_TYPE_PARTITION     2
#define DEVICE_TYPE_NETWORK       4
#define DEVICE_TYPE_PARTITION_EX  6
#define DEVICE_TYPE_VMBUS_CHANNEL 7
#define DEVICE_TYPE_URI           9

#define DEVICE_ATTRIBUTE_NO_PARENT_SIGNATURE 0x00000004

typedef struct {
    ULONG Type;
    ULONG Attributes;
    ULONG Size;
    ULONG Reserved;

    union {
        BLOCK_DEVICE_IDENTIFIER   BlockDevice;
        PARTITION_IDENTIFIER      Partition;
        NETWORK_DEVICE_IDENTIFIER NetworkDevice;
        PARTITION_IDENTIFIER_EX   PartitionEx;
        VMBUS_CHANNEL_IDENTIFIER  VmbusChannel;
        URI_IDENTIFIER            Uri;
    };
} DEVICE_IDENTIFIER, *PDEVICE_IDENTIFIER;

//
// BCD element.
//

typedef ULONG BCDE_DATA_TYPE;
typedef BCDE_DATA_TYPE *PBCDE_DATA_TYPE;

#define BCDE_CLASS_MASK          0xf0000000
#define BCDE_CLASS_LIBRARY       0x10000000
#define BCDE_CLASS_APPLICATION   0x20000000
#define BCDE_CLASS_DEVICE        0x30000000
#define BCDE_CLASS_TEMPLATE      0x40000000

#define BCDE_FORMAT_MASK         0x0f000000
#define BCDE_FORMAT_DEVICE       0x01000000
#define BCDE_FORMAT_STRING       0x02000000
#define BCDE_FORMAT_GUID         0x03000000
#define BCDE_FORMAT_GUID_LIST    0x04000000
#define BCDE_FORMAT_INTEGER      0x05000000
#define BCDE_FORMAT_BOOLEAN      0x06000000
#define BCDE_FORMAT_INTEGER_LIST 0x07000000

#define BCDE_LIBRARY_TYPE_APPLICATION_DEVICE           0x11000001
#define BCDE_LIBRARY_TYPE_APPLICATION_PATH             0x12000002
#define BCDE_LIBRARY_TYPE_DESCRIPTION                  0x12000004
#define BCDE_LIBRARY_TYPE_PREFERRED_LOCALE             0x12000005
#define BCDE_LIBRARY_TYPE_INHERITED_OBJECTS            0x14000006
#define BCDE_LIBRARY_TYPE_TRUNCATE_PHYSICAL_MEMORY     0x15000007
#define BCDE_LIBRARY_TYPE_RECOVERY_SEQUENCE            0x14000008
#define BCDE_LIBRARY_TYPE_AUTO_RECOVERY_ENABLED        0x16000009
#define BCDE_LIBRARY_TYPE_BAD_MEMORY_LIST              0x1700000a
#define BCDE_LIBRARY_TYPE_ALLOW_BAD_MEMORY_ACCESS      0x1600000b
#define BCDE_LIBRARY_TYPE_FIRST_MEGABYTE_POLICY        0x1500000c
#define BCDE_LIBRARY_TYPE_RELOCATE_PHYSICAL_MEMORY     0x1500000d
#define BCDE_LIBRARY_TYPE_AVOID_LOW_PHYSICAL_MEMORY    0x1500000e
#define BCDE_LIBRARY_TYPE_DEBUGGER_ENABLED             0x16000010
#define BCDE_LIBRARY_TYPE_DEBUGGER_TYPE                0x15000011
#define BCDE_LIBRARY_TYPE_SERIAL_DEBUGGER_PORT_ADDRESS 0x15000012
#define BCDE_LIBRARY_TYPE_SERIAL_DEBUGGER_PORT         0x15000013
#define BCDE_LIBRARY_TYPE_SERIAL_DEBUGGER_BAUD_RATE    0x15000014
#define BCDE_LIBRARY_TYPE_1394_DEBUGGER_CHANNEL        0x15000015
#define BCDE_LIBRARY_TYPE_USB_DEBUGGER_TARGET_NAME     0x12000016
#define BCDE_LIBRARY_TYPE_LOAD_OPTIONS_STRING          0x12000030
#define BCDE_LIBRARY_TYPE_DISPLAY_ADVANCED_OPTIONS     0x16000040
#define BCDE_LIBRARY_TYPE_DISPLAY_OPTIONS_EDIT         0x16000041
#define BCDE_LIBRARY_TYPE_BSD_LOG_DEVICE               0x11000043
#define BCDE_LIBRARY_TYPE_BSD_LOG_PATH                 0x12000044
#define BCDE_LIBRARY_TYPE_GRAPHICS_MODE_DISABLED       0x16000046
#define BCDE_LIBRARY_TYPE_SI_POLICY                    0x1500004b
#define BCDE_LIBRARY_TYPE_CONSOLE_EXTENDED_INPUT       0x16000050
#define BCDE_LIBRARY_TYPE_GRAPHICS_RESOLUTION          0x15000052
#define BCDE_LIBRARY_TYPE_RESTART_ON_FAILURE           0x16000053
#define BCDE_LIBRARY_TYPE_GRAPHICS_FORCE_HIGHEST_MODE  0x16000054
#define BCDE_LIBRARY_TYPE_ISOLATED_EXECUTION_CONTEXT   0x16000060
#define BCDE_LIBRARY_TYPE_BOOT_SHUTDOWN_DISABLED       0x16000074
#define BCDE_LIBRARY_TYPE_ALLOWED_IN_MEMORY_SETTINGS   0x17000077
#define BCDE_LIBRARY_TYPE_DEVICE_000083                0x11000083
#define BCDE_LIBRARY_TYPE_ENABLE_NUM_LOCK              0x16000087

#define BCDE_BOOTMGR_TYPE_DISPLAY_ORDER                0x24000001
#define BCDE_BOOTMGR_TYPE_BOOT_SEQUENCE                0x24000002
#define BCDE_BOOTMGR_TYPE_DEFAULT_OBJECT               0x23000003
#define BCDE_BOOTMGR_TYPE_TIMEOUT                      0x25000004
#define BCDE_BOOTMGR_TYPE_ATTEMPT_RESUME               0x26000005
#define BCDE_BOOTMGR_TYPE_RESUME_OBJECT                0x23000006
#define BCDE_BOOTMGR_TYPE_TOOLS_DISPLAY_ORDER          0x24000010
#define BCDE_BOOTMGR_TYPE_DISPLAY_BOOT_MENU            0x26000020
#define BCDE_BOOTMGR_TYPE_NO_ERROR_DISPLAY             0x26000021
#define BCDE_BOOTMGR_TYPE_BCD_DEVICE                   0x21000022
#define BCDE_BOOTMGR_TYPE_BCD_FILE_PATH                0x22000023
#define BCDE_BOOTMGR_TYPE_PROCESS_CUSTOM_ACTIONS_FIRST 0x26000028
#define BCDE_BOOTMGR_TYPE_CUSTOM_ACTIONS_LIST          0x27000030
#define BCDE_BOOTMGR_TYPE_PERSIST_BOOT_SEQUENCE        0x26000031

#define BCDE_OS_LOADER_TYPE_OS_DEVICE                 0x21000001
#define BCDE_OS_LOADER_TYPE_SYSTEM_ROOT               0x22000002
#define BCDE_OS_LOADER_TYPE_KERNEL_PATH               0x22000011
#define BCDE_OS_LOADER_TYPE_HAL_PATH                  0x22000012
#define BCDE_OS_LOADER_TYPE_NX_POLICY                 0x25000020
#define BCDE_OS_LOADER_TYPE_PAE_POLICY                0x25000021
#define BCDE_OS_LOADER_TYPE_ADVANCED_OPTIONS_ONE_TIME 0x260000c3
#define BCDE_OS_LOADER_TYPE_BOOLEAN_0000C4            0x260000c4

typedef struct {
    GUID              Options;
    DEVICE_IDENTIFIER Identifier;
} BCDE_DEVICE, *PBCDE_DEVICE;

//
// Memory information.
//

#define MEMORY_INFO_VERSION 1

typedef struct {
    ULONG Version;
    ULONG MdlOffset;
    ULONG DescriptorCount;
    ULONG DescriptorSize;
    ULONG DescriptorOffset;
} MEMORY_INFO, *PMEMORY_INFO;

//
// Firmware data.
//

#define FIRMWARE_DATA_VERSION 2

typedef struct {
    ULONG                    Version;
    ULONG                    Reserved;
#if defined(_EFI)
    EFI_HANDLE               ImageHandle;
    EFI_SYSTEM_TABLE         *SystemTable;
#if defined(__x86_64__) || defined(__i386__)
    ULONG_PTR                Cr3;
    DESCRIPTOR_TABLE_CONTEXT DescriptorTableContext;
#endif
#endif
} FIRMWARE_DATA, *PFIRMWARE_DATA;

//
// Return data.
//

#define RETURN_DATA_VERSION 1

typedef struct {
    ULONG    Version;
    NTSTATUS Status;
    ULONG    Flags;
} RETURN_DATA, *PRETURN_DATA;

//
// Firmware-independent boot application parameters.
//

#define BOOT_APPLICATION_PARAMETERS_SIGNATURE 0x50504120544f4f42 /* "BOOT APP" */
#define BOOT_APPLICATION_PARAMETERS_VERSION   2

typedef struct {
    //
    // Structure metadata.
    //
    ULONGLONG Signature;
    ULONG     Version;
    ULONG     TotalSize;

    //
    // Machine information.
    //
    ULONG     MachineType;
    ULONG     TranslationType;

    //
    // Image information.
    //
    PVOID     ImageBase;
    ULONG     ImageSize;

    //
    // Offsets to auxillary structures.
    //
    ULONG     MemoryInfoOffset;
    ULONG     BootEntryOffset;
    ULONG     BootDeviceOffset;
    ULONG     FirmwareDataOffset;
    ULONG     ReturnDataOffset;
} BOOT_APPLICATION_PARAMETERS, *PBOOT_APPLICATION_PARAMETERS;

//
// Boot entry option.
//
typedef struct {
    ULONG   Type;
    ULONG   DataOffset;
    ULONG   DataSize;
    ULONG   AdditionalOptionsOffset;
    ULONG   NextOptionOffset;
    BOOLEAN IsInvalid;
    UCHAR   Reserved[3];
} BOOT_ENTRY_OPTION, *PBOOT_ENTRY_OPTION;

//
// Boot application entry.
//

#define BOOT_ENTRY_NO_IDENTIFIER    0x00000001
#define BOOT_ENTRY_OPTIONS_INTERNAL 0x00000002
#define BOOT_ENTRY_OS_LOADER        0x00000004
#define BOOT_ENTRY_OPTIONS_EXTERNAL 0x00000080
#define BOOT_ENTRY_NO_TRAP_VECTORS  0x00004000
#define BOOT_ENTRY_UNKNOWN_8000     0x00008000

typedef struct {
    ULONG              Attributes;
    GUID               Identifier;
    PBOOT_ENTRY_OPTION Options;
} BOOT_APPLICATION_ENTRY, *PBOOT_APPLICATION_ENTRY;

//
// Boot application transition entry.
//

#define BOOT_APPLICATION_TRANSITION_ENTRY_SIGNATURE 0x544e4550415442 /* "BTAPENT" */

typedef struct {
    ULONGLONG         Signature;
    ULONG             Attributes;
    GUID              Identifier;
    UCHAR             Reserved[16];
    BOOT_ENTRY_OPTION InlineOptions;
} BOOT_APPLICATION_TRANSITION_ENTRY, *PBOOT_APPLICATION_TRANSITION_ENTRY;

//
// Boot library parameters.
//

#define BOOT_LIBRARY_FLAG_REINITIALIZE            0x00000002
#define BOOT_LIBRARY_FLAG_REINITIALIZE_ALL        0x00000004
#define BOOT_LIBRARY_FLAG_INITIALIZATION_COMPLETE 0x00000020

typedef struct {
    ULONG Flags;
    ULONG TranslationType;
    ULONG MinimumAllocationCount;
    ULONG MinimumHeapSize;
    ULONG Reserved0;
    PWSTR ApplicationBaseDirectory;
    ULONG Reserved1;
    PWSTR FontBaseDirectory;
    ULONG Reserved2[2];
    PGUID Identifier;
} BOOT_LIBRARY_PARAMETERS, *PBOOT_LIBRARY_PARAMETERS;

//
// Boot option callbacks.
//

typedef
NTSTATUS
(*PBOOT_OPTION_CALLBACK_BOOLEAN) (
    IN  ULONGLONG      Cookie,
    IN  NTSTATUS       Status,
    IN  ULONG          Unknown,
    IN  PGUID          Identifier,
    IN  BCDE_DATA_TYPE Type,
    IN  PVOID          SecurityDescriptor,
    OUT PBOOLEAN       Value
    );

typedef struct {
    PBOOT_OPTION_CALLBACK_BOOLEAN Boolean;
    PVOID                         Integer;
    PVOID                         String;
    PVOID                         Device;
} BOOT_OPTION_CALLBACKS, *PBOOT_OPTION_CALLBACKS;

//
// Execution context.
//

typedef enum {
    ExecutionContextApplication,
    ExecutionContextFirmware,
    ExecutionContextMax
} EXECUTION_CONTEXT_TYPE;

#if defined(__x86_64__) || defined(__i386__)
#define EXECUTION_CONTEXT_INTERRUPTS_DISABLED    0x00000001
#define EXECUTION_CONTEXT_INTERRUPTS_ENABLED     0x00000002
#define EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED 0x00000004
#endif

typedef struct {
    EXECUTION_CONTEXT_TYPE   Type;
    ULONG                    TranslationType;
    ULONG                    Attributes;
#if defined(__x86_64__) || defined(__i386__)
    ULONG_PTR                Cr3;
    DESCRIPTOR_TABLE_CONTEXT DescriptorTableContext;
#endif
} EXECUTION_CONTEXT, *PEXECUTION_CONTEXT;

//
// Windows boot options.
//

#define WINDOWS_OS_OPTIONS_SIGNATURE 0x0053574f444e4957 /* "WINDOWS\0" */

#define WINDOWS_OS_PATH_TYPE_INTERNAL 3
#define WINDOWS_OS_PATH_TYPE_EFI      4

#pragma pack(push,1)

typedef struct {
    ULONG Unknown;
    ULONG Length;
    ULONG Type;
    UCHAR Data[ANYSIZE_ARRAY];
} WINDOWS_OS_PATH, *PWINDOWS_OS_PATH;

typedef struct {
    ULONGLONG Signature;
    ULONG     Version;
    ULONG     Size;
    ULONG     OsPathOffset;
    WCHAR     Options[2];
} WINDOWS_OS_OPTIONS, *PWINDOWS_OS_OPTIONS;

#pragma pack(pop)

#define PLATFORM_FLAG_FIRMWARE_EXECUTION_CONTEXT 0x00100000

extern ULONG BlPlatformFlags;

#define BOOT_APPLICATION_FLAG_LOADED_BY_FIRMWARE 0x00000001

extern ULONG BlpApplicationFlags;

extern PBOOT_APPLICATION_PARAMETERS BlpApplicationParameters;
extern BOOT_LIBRARY_PARAMETERS BlpLibraryParameters;
extern BOOT_APPLICATION_ENTRY BlpApplicationEntry;
extern PWSTR BlpApplicationBaseDirectory;
extern BOOLEAN BlpApplicationIdentifierSet;

extern BOOLEAN EnSubsystemInitialized;

extern PEXECUTION_CONTEXT CurrentExecutionContext;

//
// String services.
//

VOID
BlCopyStringToWcharString (
    OUT PWSTR Destination,
    IN  PSTR  Source
    );

//
// Boot option services.
//

PBOOT_ENTRY_OPTION
BcdUtilGetBootOption (
    IN PBOOT_ENTRY_OPTION Options,
    IN BCDE_DATA_TYPE     Type
    );

ULONG
BlGetBootOptionSize (
    IN PBOOT_ENTRY_OPTION Option
    );

ULONG
BlGetBootOptionListSize (
    IN PBOOT_ENTRY_OPTION Options
    );

NTSTATUS
BlMergeBootOptionLists (
    IN     PBOOT_ENTRY_OPTION OptionsA,
    IN     PBOOT_ENTRY_OPTION OptionsB,
    IN     PVOID              Buffer,
    IN OUT PULONG             BufferSize
    );

NTSTATUS
BlGetBootOptionBoolean (
    IN  PBOOT_ENTRY_OPTION Options,
    IN  BCDE_DATA_TYPE     Type,
    OUT PBOOLEAN           Value
    );

NTSTATUS
BlAppendBootOptions (
    IN PBOOT_APPLICATION_ENTRY BootEntry,
    IN PBOOT_ENTRY_OPTION      Options
    );

NTSTATUS
BlAppendBootOptionString (
    IN PBOOT_APPLICATION_ENTRY BootEntry,
    IN BCDE_DATA_TYPE          Type,
    IN PWSTR                   String
    );

//
// Firmware services.
//

NTSTATUS
BlpFwInitialize (
    IN ULONG          Phase,
    IN PFIRMWARE_DATA FirmwareData
    );

//
// Architecture services.
//

NTSTATUS
BlpArchInitialize (
    IN ULONG Phase
    );

VOID
BlpArchSwitchContext (
    IN EXECUTION_CONTEXT_TYPE Type
    );

VOID
ArchRestoreProcessorFeatures (
    IN BOOLEAN Unknown
    );

//
// Memory management services.
//

NTSTATUS
BlpMmInitialize (
    IN PMEMORY_INFO             MemoryInfo,
    IN ULONG                    TranslationType,
    IN PBOOT_LIBRARY_PARAMETERS LibraryParameters
    );

PVOID
BlMmAllocateHeap (
    IN ULONG_PTR Size
    );

NTSTATUS
BlMmFreeHeap (
    IN PVOID Pointer
    );

NTSTATUS
BlpMmDestroy (
    IN ULONG Phase
    );

//
// Event notification services.
//

#define EVENT_10000002                  0x10000002
#define EVENT_APPLICATION_LOAD          0x10000003
#define EVENT_10000004                  0x10000004
#define EVENT_APPLICATION_EXIT          0x10000005
#define EVENT_LIBRARY_DESTRUCTION_BEGIN 0x10000009

typedef
VOID
(*PEVENT_HANDLER) (
    IN ULONG Event,
    IN PVOID Context,
    IN ULONG Unknown0,
    IN ULONG Unknown1
    );

VOID
BlpEnInitialize (
    VOID
    );

NTSTATUS
BlEnRegisterEventHandler (
    IN ULONG          Event,
    IN PEVENT_HANDLER Handler,
    IN ULONG          Unknown0,
    IN ULONG          Unknown1
    );

VOID
BlEnNotifyEvent (
    IN ULONG Event,
    IN PVOID Context
    );

NTSTATUS
BlpEnDestroy (
    VOID
    );

//
// Library services.
//

NTSTATUS
BlInitializeLibrary (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters,
    IN PBOOT_LIBRARY_PARAMETERS     LibraryParameters
    );

PGUID
BlGetApplicationIdentifier (
    VOID
    );

NTSTATUS
BlDestroyLibrary (
    VOID
    );

//
// Console services.
//

#if defined(_EFI)
VOID
EfiPrintf (
    IN PWSTR Format,
    ...
    );

#define ConsolePrint EfiPrintf
#endif

VOID
ConsoleInfo (
    IN PWSTR Format,
    ...
    );

VOID
ConsoleWarning (
    IN PCSTR Source,
    IN PWSTR Format,
    ...
    );

VOID
ConsoleError (
    IN PCSTR Source,
    IN PWSTR Format,
    ...
    );

//
// Debugging services.
//
#if !defined(NDEBUG)
#define DebugPrint        ConsolePrint
#define DebugInfo         ConsoleInfo
#define DebugWarning(...) ConsoleWarning(__func__, __VA_ARGS__)
#define DebugError(...)   ConsoleError(__func__, __VA_ARGS__)
#else
#define DebugPrint(Format, ...)
#define DebugInfo(Format, ...)
#define DebugWarning(Format, ...)
#define DebugError(Format, ...)
#endif

#endif /* !_BOOTLIB_H */
