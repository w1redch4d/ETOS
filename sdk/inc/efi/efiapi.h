/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efiapi.h

Abstract:

    Provides EFI API definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef _EFIAPI_H
#define _EFIAPI_H

typedef struct _EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

//
// Event, timer, and task priority services.
//

#define EVT_NOTIFY_WAIT                   0x00000100
#define EVT_NOTIFY_SIGNAL                 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES     0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202
#define EVT_RUNTIME                       0x40000000
#define EVT_TIMER                         0x80000000

typedef
VOID
(EFIAPI *EFI_EVENT_NOTIFY) (
    IN EFI_EVENT Event,
    IN VOID      *Context
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CREATE_EVENT) (
    IN  UINT32           Type,
    IN  EFI_TPL          NotifyTpl,
    IN  EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
    IN  VOID             *NotifyContext OPTIONAL,
    OUT EFI_EVENT        *Event
    );

#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES        { 0x27abf055, 0xb1b8, 0x4c26, { 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf } }
#define EFI_EVENT_GROUP_BEFORE_EXIT_BOOT_SERVICES { 0x8be0e274, 0x3970, 0x4b44, { 0x80, 0xc5, 0x1a, 0xb9, 0x50, 0x2f, 0x3b, 0xfc } }
#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE    { 0x13fa7698, 0xc831, 0x49c7, { 0x87, 0xea, 0x8f, 0x43, 0xfc, 0xc2, 0x51, 0x96 } }
#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE         { 0x78bee926, 0x692f, 0x48fd, { 0x9e, 0xdb, 0x01, 0x42, 0x2e, 0xf0, 0xd7, 0xab } }
#define EFI_EVENT_GROUP_READY_TO_BOOT             { 0x7ce88fb3, 0x4bd7, 0x4679, { 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0x0d, 0x2b } }
#define EFI_EVENT_GROUP_AFTER_READY_TO_BOOT       { 0x3a2a00ad, 0x98b9, 0x4cdf, { 0xa4, 0x78, 0x70, 0x27, 0x77, 0xf1, 0xc1, 0x0b } }
#define EFI_EVENT_GROUP_RESET_SYSTEM              { 0x62da6a56, 0x13fb, 0x485a, { 0xa8, 0xda, 0xa3, 0xdd, 0x79, 0x12, 0xcb, 0x6b } }

typedef
EFI_STATUS
(EFIAPI *EFI_CREATE_EVENT_EX) (
    IN  UINT32           Type,
    IN  EFI_TPL          NotifyTpl,
    IN  EFI_EVENT_NOTIFY NotifyFunction OPTIONAL,
    IN  CONST VOID       *NotifyContext OPTIONAL,
    IN  CONST EFI_GUID   *EventGroup    OPTIONAL,
    OUT EFI_EVENT        *Event
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_EVENT) (
    IN EFI_EVENT Event
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SIGNAL_EVENT) (
    IN EFI_EVENT Event
    );

typedef
EFI_STATUS
(EFIAPI *EFI_WAIT_FOR_EVENT) (
    IN  UINTN     NumberOfEvents,
    IN  EFI_EVENT Event,
    OUT UINTN     *Index
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CHECK_EVENT) (
    IN EFI_EVENT Event
    );

typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative,
    TimerTypeMax,
} EFI_TIMER_DELAY;

typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIMER) (
    IN EFI_EVENT       Event,
    IN EFI_TIMER_DELAY Type,
    IN UINT64          TriggerTime
    );

//
// TPLs (Task Priority Levels).
//
#define TPL_APPLICATION 4
#define TPL_CALLBACK    8
#define TPL_NOTIFY      16
#define TPL_HIGH_LEVEL  31

typedef
EFI_TPL
(EFIAPI *EFI_RAISE_TPL) (
    IN EFI_TPL NewTpl
    );

typedef
VOID
(EFIAPI *EFI_RESTORE_TPL) (
    IN EFI_TPL OldTpl
    );

//
// Memory allocation services.
//

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_PAGES) (
    IN     EFI_ALLOCATE_TYPE    Type,
    IN     EFI_MEMORY_TYPE      MemoryType,
    IN     UINTN                Pages,
    IN OUT EFI_PHYSICAL_ADDRESS *Memory
    );

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_PAGES) (
    IN EFI_PHYSICAL_ADDRESS Memory,
    IN UINTN                Pages
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_MEMORY_MAP) (
    IN OUT UINTN                 *MemoryMapSize,
    OUT    EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT    UINTN                 *MapKey,
    OUT    UINTN                 *DescriptorSize,
    OUT    UINT32                *DescriptorVersion
    );

typedef
EFI_STATUS
(EFIAPI *EFI_ALLOCATE_POOL) (
    IN  EFI_MEMORY_TYPE PoolType,
    IN  UINTN           Size,
    OUT VOID            **Buffer
    );

typedef
EFI_STATUS
(EFIAPI *EFI_FREE_POOL) (
    IN VOID *Buffer
    );

//
// Protocol handler services.
//

typedef enum {
    EFI_NATIVE_INTERFACE,
    EFI_PCODE_INTERFACE,
} EFI_INTERFACE_TYPE;

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE) (
    IN OUT EFI_HANDLE         *Handle,
    IN     EFI_GUID           *Protocol,
    IN     EFI_INTERFACE_TYPE InterfaceType,
    IN     VOID               *Interface
    );

typedef
EFI_STATUS
(EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE) (
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol,
    IN VOID       *Interface
    );

typedef
EFI_STATUS
(EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE) (
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol,
    IN VOID       *OldInterface,
    IN VOID       *NewInterface
    );

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY) (
    IN  EFI_GUID  *Protocol,
    IN  EFI_EVENT Event,
    OUT VOID      **Registration
    );

typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol,
} EFI_LOCATE_SEARCH_TYPE;

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_HANDLE) (
    IN     EFI_LOCATE_SEARCH_TYPE SearchType,
    IN     EFI_GUID               *Protocol  OPTIONAL,
    IN     VOID                   *SearchKey OPTIONAL,
    IN OUT UINTN                  *BufferSize,
    OUT    EFI_HANDLE             *Buffer
    );

typedef
EFI_STATUS
(EFIAPI *EFI_HANDLE_PROTOCOL) (
    IN  EFI_HANDLE Handle,
    IN  EFI_GUID   *Protocol,
    OUT VOID       **Interface
    );

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_DEVICE_PATH) (
    IN     EFI_GUID        *Protocol,
    IN OUT EFI_DEVICE_PATH **DevicePath,
    OUT    EFI_HANDLE      *Device
    );

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef
EFI_STATUS
(EFIAPI *EFI_OPEN_PROTOCOL) (
    IN  EFI_HANDLE Handle,
    IN  EFI_GUID   *Protocol,
    OUT VOID       **Interface OPTIONAL,
    IN  EFI_HANDLE AgentHandle,
    IN  EFI_HANDLE ControllerHandle,
    IN  UINT32     Attributes
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_PROTOCOL) (
    IN EFI_HANDLE Handle,
    IN EFI_GUID   *Protocol,
    IN EFI_HANDLE AgentHandle,
    IN EFI_HANDLE ControllerHandle
    );

typedef struct {
    EFI_HANDLE AgentHandle;
    EFI_HANDLE ControllerHandle;
    UINT32     Attributes;
    UINT32     OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef
EFI_STATUS
(EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION) (
    IN  EFI_HANDLE                          Handle,
    IN  EFI_GUID                            *Protocol,
    OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY **EntryBuffer,
    OUT UINTN                               *EntryCount
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CONNECT_CONTROLLER) (
    IN EFI_HANDLE      ControllerHandle,
    IN EFI_HANDLE      *DriverImageHandle   OPTIONAL,
    IN EFI_DEVICE_PATH *RemainingDevicePath OPTIONAL,
    IN BOOLEAN         Recursive
    );

typedef
EFI_STATUS
(EFIAPI *EFI_DISCONNECT_CONTROLLER) (
    IN EFI_HANDLE ControllerHandle,
    IN EFI_HANDLE DriverImageHandle OPTIONAL,
    IN EFI_HANDLE ChildHandle       OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_PROTOCOLS_PER_HANDLE) (
    IN  EFI_HANDLE Handle,
    OUT EFI_GUID   ***ProtocolBuffer,
    OUT UINTN      *ProtocolBufferCount
    );

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
    IN  EFI_LOCATE_SEARCH_TYPE SearchType,
    IN  EFI_GUID               *Protocol  OPTIONAL,
    IN  VOID                   *SearchKey OPTIONAL,
    OUT UINTN                  *NoHandles,
    OUT EFI_HANDLE             **Buffer
    );

typedef
EFI_STATUS
(EFIAPI *EFI_LOCATE_PROTOCOL) (
    IN  EFI_GUID *Protocol,
    IN  VOID     *Registration OPTIONAL,
    OUT VOID     **Interface
    );

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    IN OUT EFI_HANDLE *Handle,
    ...
    );

typedef
EFI_STATUS
(EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    IN OUT EFI_HANDLE *Handle,
    ...
    );

//
// Image services.
//

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_LOAD) (
    IN  BOOLEAN         BootPolicy,
    IN  EFI_HANDLE      ParentImageHandle,
    IN  EFI_DEVICE_PATH *DevicePath   OPTIONAL,
    IN  VOID            *SourceBuffer OPTIONAL,
    IN  UINTN           SourceSize,
    OUT EFI_HANDLE      *ImageHandle
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_START) (
    IN  EFI_HANDLE ImageHandle,
    OUT UINTN      *ExitDataSize,
    OUT CHAR16     **ExitData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_ENTRY_POINT) (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    );

typedef
EFI_STATUS
(EFIAPI *EFI_EXIT) (
    IN EFI_HANDLE ImageHandle,
    IN EFI_STATUS ExitStatus,
    IN UINTN      ExitDataSize,
    IN CHAR16     *ExitData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_EXIT_BOOT_SERVICES) (
    IN EFI_HANDLE ImageHandle,
    IN UINTN      MapKey
    );

//
// Miscellaneous boot services.
//

typedef
EFI_STATUS
(EFIAPI *EFI_SET_WATCHDOG_TIMER) (
    IN UINTN  Timeout,
    IN UINT64 WatchdogCode,
    IN UINTN  DataSize,
    IN CHAR16 *WatchdogData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_STALL) (
    IN UINTN Microseconds
    );

typedef
VOID
(EFIAPI *EFI_COPY_MEM) (
    IN VOID  *Destination,
    IN VOID  *Source,
    IN UINTN Length
    );

typedef
VOID
(EFIAPI *EFI_SET_MEM) (
    IN VOID  *Buffer,
    IN UINTN Size,
    IN UINT8 Value
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT) (
    OUT UINT64 *Count
    );

typedef
EFI_STATUS
(EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE) (
    IN EFI_GUID *Guid,
    IN VOID     *Table
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CALCULATE_CRC32) (
    IN  VOID   *Data,
    IN  UINTN  DataSize,
    OUT UINT32 *Crc32
    );

//
// Variable services.
//

#define EFI_VARIABLE_NON_VOLATILE                          0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                    0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                        0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                 0x00000008
#define EFI_VARIABLE_HR                                    0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE                          0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS         0x00000080

typedef
EFI_STATUS
(EFIAPI *EFI_GET_VARIABLE) (
    IN     CHAR16   *VariableName,
    IN     EFI_GUID *VendorGuid,
    OUT    UINT32   *Attributes OPTIONAL,
    IN OUT UINTN    *DataSize,
    OUT    VOID     *Data       OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME) (
    IN OUT UINTN    *VariableNameSize,
    IN OUT CHAR16   *VariableName,
    IN OUT EFI_GUID *VendorGuid
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_VARIABLE) (
    IN CHAR16   *VariableName,
    IN EFI_GUID *VendorGuid,
    IN UINT32   Attributes,
    IN UINTN    DataSize,
    IN VOID     *Data
    );

typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_VARIABLE_INFO) (
    IN  UINT32 Attributes,
    OUT UINT64 *MaximumVariableStorageSize,
    OUT UINT64 *RemainingVariableStorageSize,
    OUT UINT64 *MaximumVariableSize
    );

//
// Time services.
//

typedef struct {
    UINT32  Resolution;
    UINT32  Accuracy;
    BOOLEAN SetsToZero;
} EFI_TIME_CAPABILITIES;

typedef
EFI_STATUS
(EFIAPI *EFI_GET_TIME) (
    OUT EFI_TIME              *Time,
    OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIME) (
    IN EFI_TIME *Time
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_WAKEUP_TIME) (
    OUT BOOLEAN  *Enabled,
    OUT BOOLEAN  *Pending,
    OUT EFI_TIME *Time
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SET_WAKEUP_TIME) (
    IN BOOLEAN  Enable,
    IN EFI_TIME *Time OPTIONAL
    );

//
// Virtual memory services.
//

typedef
EFI_STATUS
(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP) (
    IN UINTN                 MemoryMapSize,
    IN UINTN                 DescriptorSize,
    IN UINT32                DescriptorVersion,
    IN EFI_MEMORY_DESCRIPTOR *VirtualMap
    );

#define EFI_OPTIONAL_PTR 0x00000001

typedef
EFI_STATUS
(EFIAPI *EFI_CONVERT_POINTER) (
    IN UINTN DebugDisposition,
    IN VOID  **Address
    );

//
// Miscellaneous runtime services.
//

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific,
} EFI_RESET_TYPE;

typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM) (
    IN EFI_RESET_TYPE ResetType,
    IN EFI_STATUS     ResetStatus,
    IN UINTN          DataSize,
    IN VOID           *ResetData OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT) (
    OUT UINT32 *HighCount
    );

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET  0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE 0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET        0x00040000

typedef struct {
    EFI_GUID CapsuleGuid;
    UINT32   HeaderSize;
    UINT32   Flags;
    UINT32   CapsuleImageSize;
} EFI_CAPSULE_HEADER;

typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CAPSULE) (
    IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
    IN UINTN                CapsuleCount,
    IN EFI_PHYSICAL_ADDRESS ScatterGatherList OPTIONAL
    );

typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES) (
    IN  EFI_CAPSULE_HEADER **CapsuleHeaderArray,
    IN  UINTN              CapsuleCount,
    OUT UINT64             *MaximumCapsuleSize,
    OUT EFI_RESET_TYPE     *ResetType
    );

//
// EFI table header.
//
typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

//
// EFI runtime services table.
//

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552 /* "RUNTSERV" */

typedef struct {
    EFI_TABLE_HEADER               Hdr;

    //
    // Time services.
    //
    EFI_GET_TIME                   GetTime;
    EFI_SET_TIME                   SetTime;
    EFI_GET_WAKEUP_TIME            GetWakeupTime;
    EFI_SET_WAKEUP_TIME            SetWakeupTime;

    //
    // Virtual memory services.
    //
    EFI_SET_VIRTUAL_ADDRESS_MAP    SetVirtualAddressMap;
    EFI_CONVERT_POINTER            ConvertPointer;

    //
    // Variable services.
    //
    EFI_GET_VARIABLE               GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME     GetNextVariableName;
    EFI_SET_VARIABLE               SetVariable;

    //
    // Miscellaneous services.
    //
    EFI_GET_NEXT_HIGH_MONO_COUNT   GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM               ResetSystem;

    //
    // All services below require UEFI 2.0+.
    //

    //
    // Capsule services.
    //
    EFI_UPDATE_CAPSULE             UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES QueryCapsuleCapabilities;

    //
    // Miscellaneous services.
    //
    EFI_QUERY_VARIABLE_INFO        QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

//
// EFI boot services table.
//

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42 /* "BOOTSERV" */

typedef struct {
    EFI_TABLE_HEADER                           Hdr;

    //
    // Task priority services.
    //
    EFI_RAISE_TPL                              RaiseTPL;
    EFI_RESTORE_TPL                            RestoreTPL;

    //
    // Memory services.
    //
    EFI_ALLOCATE_PAGES                         AllocatePages;
    EFI_FREE_PAGES                             FreePages;
    EFI_GET_MEMORY_MAP                         GetMemoryMap;
    EFI_ALLOCATE_POOL                          AllocatePool;
    EFI_FREE_POOL                              FreePool;

    //
    // Event & timer services.
    //
    EFI_CREATE_EVENT                           CreateEvent;
    EFI_SET_TIMER                              SetTimer;
    EFI_WAIT_FOR_EVENT                         WaitForEvent;
    EFI_SIGNAL_EVENT                           SignalEvent;
    EFI_CLOSE_EVENT                            CloseEvent;
    EFI_CHECK_EVENT                            CheckEvent;

    //
    // Protocol handler services.
    //
    EFI_INSTALL_PROTOCOL_INTERFACE             InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE           ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE           UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL                        HandleProtocol;
    EFI_HANDLE_PROTOCOL                        PCHandleProtocol;
    EFI_REGISTER_PROTOCOL_NOTIFY               RegisterProtocolNotify;
    EFI_LOCATE_HANDLE                          LocateHandle;
    EFI_LOCATE_DEVICE_PATH                     LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE            InstallConfigurationTable;

    //
    // Image services.
    //
    EFI_IMAGE_LOAD                             LoadImage;
    EFI_IMAGE_START                            StartImage;
    EFI_EXIT                                   Exit;
    EFI_IMAGE_UNLOAD                           UnloadImage;
    EFI_EXIT_BOOT_SERVICES                     ExitBootServices;

    //
    // Miscellaneous services.
    //
    EFI_GET_NEXT_MONOTONIC_COUNT               GetNextMonotonicCount;
    EFI_STALL                                  Stall;
    EFI_SET_WATCHDOG_TIMER                     SetWatchdogTimer;

    //
    // All services below require EFI 1.1+.
    //

    //
    // Driver controller services.
    //
    EFI_CONNECT_CONTROLLER                     ConnectController;
    EFI_DISCONNECT_CONTROLLER                  DisconnectController;

    //
    // Open/close protocol services.
    //
    EFI_OPEN_PROTOCOL                          OpenProtocol;
    EFI_CLOSE_PROTOCOL                         CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION              OpenProtocolInformation;

    //
    // Library services.
    //
    EFI_PROTOCOLS_PER_HANDLE                   ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER                   LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL                        LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES   InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;

    //
    // 32-bit CRC services.
    //
    EFI_CALCULATE_CRC32                        CalculateCrc32;

    //
    // Miscellaneous services.
    //
    EFI_COPY_MEM                               CopyMem;
    EFI_SET_MEM                                SetMem;
    EFI_CREATE_EVENT_EX                        CreateEventEx; // UEFI 2.0+ only.
} EFI_BOOT_SERVICES;

//
// Configuration table.
//

typedef struct {
    EFI_GUID VendorGuid;
    VOID     *VendorTable;
} EFI_CONFIGURATION_TABLE;

//
// EFI system table.
//

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249 /* "IBI SYST" */

#define EFI_SPECIFICATION_REVISION_MAJORMINOR(Major, Minor) (((Major) << 16) | (Minor))
#define EFI_1_02_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(1, 02)
#define EFI_1_10_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(1, 10)
#define EFI_2_00_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 00)
#define EFI_2_10_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 10)
#define EFI_2_20_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 20)
#define EFI_2_30_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 30)
#define EFI_2_31_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 31)
#define EFI_2_40_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 40)
#define EFI_2_50_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 50)
#define EFI_2_60_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 60)
#define EFI_2_70_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 70)
#define EFI_2_80_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 80)
#define EFI_2_90_SYSTEM_TABLE_REVISION  EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 90)
#define EFI_2_100_SYSTEM_TABLE_REVISION EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 100)
#define EFI_2_110_SYSTEM_TABLE_REVISION EFI_SPECIFICATION_REVISION_MAJORMINOR(2, 110)

typedef struct _EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER                Hdr;

    CHAR16                          *FirmwareVendor;
    UINT32                          FiwmareRevision;

    EFI_HANDLE                      ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;

    EFI_HANDLE                      ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;

    EFI_HANDLE                      StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;

    EFI_RUNTIME_SERVICES            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;

    UINTN                           NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE         *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#endif // _EFIAPI_H
