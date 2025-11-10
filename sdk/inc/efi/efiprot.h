/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efiprot.h

Abstract:

    Provides EFI protocol definitions.

--*/

#pragma once

#ifndef _EFIPROT_H
#define _EFIPROT_H

#define EFI_LOADED_IMAGE_PROTOCOL_GUID { 0x5b1b31a1, 0x9562, 0x11d2, { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
#define LOADED_IMAGE_PROTOCOL EFI_LOADED_IMAGE_PROTOCOL_GUID

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000
#define EFI_IMAGE_INFORMATION_REVISION EFI_LOADED_IMAGE_PROTOCOL_REVISION

typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_UNLOAD) (
    IN EFI_HANDLE ImageHandle
    );

typedef struct {
    UINT32                   Revision;
    EFI_HANDLE               ParentHandle;
    struct _EFI_SYSTEM_TABLE *SystemTable;

    EFI_HANDLE               DeviceHandle;
    EFI_DEVICE_PATH          *FilePath;
    VOID                     *Reserved;

    UINT32                   LoadOptionsSize;
    VOID                     *LoadOptions;

    VOID                     *ImageBase;
    UINT64                   ImageSize;
    EFI_MEMORY_TYPE          ImageCodeType;
    EFI_MEMORY_TYPE          ImageDataType;

    EFI_IMAGE_UNLOAD         Unload;
} EFI_LOADED_IMAGE, EFI_LOADED_IMAGE_PROTOCOL;

#endif /* !_EFIPROT_H */
