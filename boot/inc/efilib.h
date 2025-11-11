/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efilib.h

Abstract:

    Provides EFI-specific boot library definitions.

--*/

#pragma once

#ifndef _EFILIB_H
#define _EFILIB_H

#include "bootlib.h"

extern PFIRMWARE_DATA EfiFirmwareParameters;
extern EFI_HANDLE EfiImageHandle;
extern EFI_SYSTEM_TABLE *EfiST;
extern EFI_BOOT_SERVICES *EfiBS;
extern EFI_RUNTIME_SERVICES *EfiRT;
extern EFI_SIMPLE_TEXT_INPUT_PROTOCOL *EfiConIn;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EfiConOut;
extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *EfiConInEx;

//
// Debugging services.
//
#if !defined(NDEBUG)
VOID
EfiDebugInitialize (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Interface
    );

VOID
EfiDebugPrint (
    IN PWSTR String
    );

VOID
EfiDebugPrintf (
    IN PWSTR Format,
    ...
    );
#else
#define EfiDebugInitialize(Interface)
#define EfiDebugPrint(String)
#define EfiDebugPrintf(Format, ...)
#endif

//
// Status code translation services.
//

EFI_STATUS
EfiGetEfiStatusCode (
    IN NTSTATUS Status
    );

NTSTATUS
EfiGetNtStatusCode (
    IN EFI_STATUS Status
    );

//
// Wrapper/context services.
//

NTSTATUS
EfiGetMemoryMap (
    IN OUT UINTN                 *MemoryMapSize,
    OUT    EFI_MEMORY_DESCRIPTOR *MemoryMap,
    OUT    UINTN                 *MapKey,
    OUT    UINTN                 *DescriptorSize,
    OUT    UINT32                *DescriptorVersion
    );

NTSTATUS
EfiAllocatePages (
    IN     EFI_ALLOCATE_TYPE    Type,
    IN     EFI_MEMORY_TYPE      MemoryType,
    IN     UINTN                Pages,
    IN OUT EFI_PHYSICAL_ADDRESS *Memory
    );

NTSTATUS
EfiFreePages (
    IN EFI_PHYSICAL_ADDRESS Memory,
    IN UINTN                Pages
    );

//
// File path services.
//

NTSTATUS
EfiInitpAppendPathString (
    IN  PWCHAR Destination,
    IN  ULONG  BufferSize,
    IN  PWCHAR Source,
    IN  ULONG  SourceSize,
    OUT PULONG BufferUsed
    );

NTSTATUS
EfiInitpConvertEfiFilePath (
    IN  EFI_DEVICE_PATH    *EfiFilePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    );

NTSTATUS
EfiInitConvertEfiPxeFilePath (
    IN  EFI_SYSTEM_TABLE   *SystemTable,
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    );

//
// Device path services.
//

EFI_DEVICE_PATH *
EfiInitpGetDeviceNode (
    IN EFI_DEVICE_PATH *DevicePath
    );

NTSTATUS
EfiInitTranslateDevicePath (
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    OUT PDEVICE_IDENTIFIER BootDevice,
    IN  ULONG              BufferSize
    );

NTSTATUS
EfiInitpConvertEfiDevicePath (
    IN  EFI_DEVICE_PATH    *EfiDevicePath,
    IN  BCDE_DATA_TYPE     OptionType,
    OUT PBOOT_ENTRY_OPTION Option,
    IN  ULONG              BufferSize
    );

//
// Application services.
//

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
    );

PBOOT_APPLICATION_PARAMETERS
EfiInitCreateInputParameters (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    );

PBOOT_APPLICATION_PARAMETERS
EfiInitCreateInputParametersEx (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable,
    IN ULONG            Flags
    );

#endif /* !_EFILIB_H */
