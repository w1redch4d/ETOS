/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efierr.c

Abstract:

    EFI error code translation routines.

--*/

#include "efilib.h"

EFI_STATUS
EfiGetEfiStatusCode (
    IN NTSTATUS Status
    )

/*++

Routine Description:

    Translates an NT status code into an equivalent EFI status code.

Arguments:

    Status - The NT status code.

Return Value:

    The EFI status code.

--*/

{
    switch (Status) {
    case STATUS_SUCCESS:
        return EFI_SUCCESS;
    case STATUS_DRIVER_UNABLE_TO_LOAD:
        return EFI_LOAD_ERROR;
    case STATUS_INVALID_PARAMETER:
        return EFI_INVALID_PARAMETER;
    case STATUS_NOT_SUPPORTED:
    case STATUS_NOT_IMPLEMENTED:
        return EFI_UNSUPPORTED;
    case STATUS_INVALID_BUFFER_SIZE:
        return EFI_BAD_BUFFER_SIZE;
    case STATUS_BUFFER_TOO_SMALL:
        return EFI_BUFFER_TOO_SMALL;
    case STATUS_IO_DEVICE_ERROR:
        return EFI_DEVICE_ERROR;
    case STATUS_MEDIA_WRITE_PROTECTED:
        return EFI_WRITE_PROTECTED;
    case STATUS_INSUFFICIENT_RESOURCES:
    case STATUS_INSUFFICIENT_NVRAM_RESOURCES:
        return EFI_OUT_OF_RESOURCES;
    case STATUS_DISK_CORRUPT_ERROR:
        return EFI_VOLUME_CORRUPTED;
    case STATUS_DISK_FULL:
        return EFI_VOLUME_FULL;
    case STATUS_NO_MEDIA:
        return EFI_NO_MEDIA;
    case STATUS_MEDIA_CHANGED:
        return EFI_MEDIA_CHANGED;
    case STATUS_NOT_FOUND:
        return EFI_NOT_FOUND;
    case STATUS_ACCESS_DENIED:
        return EFI_ACCESS_DENIED;
    case STATUS_NO_MATCH:
        return EFI_NO_MAPPING;
    case STATUS_TIMEOUT:
        return EFI_TIMEOUT;
    case STATUS_DEVICE_NOT_READY:
        return EFI_NOT_STARTED;
    case STATUS_DEVICE_ALREADY_ATTACHED:
        return EFI_ALREADY_STARTED;
    case STATUS_REQUEST_ABORTED:
        return EFI_ABORTED;
    default:
        return EFI_NO_MAPPING;
    }
}

NTSTATUS
EfiGetNtStatusCode (
    IN EFI_STATUS Status
    )

/*++

Routine Description:

    Translates an EFI status code into an equivalent NT status code.

Arguments:

    Status - The EFI status code.

Return Value:

    The NT status code.

--*/

{
    switch (Status) {
    case EFI_SUCCESS:
        return STATUS_SUCCESS;
    case EFI_LOAD_ERROR:
        return STATUS_DRIVER_UNABLE_TO_LOAD;
    case EFI_INVALID_PARAMETER:
        return STATUS_INVALID_PARAMETER;
    case EFI_UNSUPPORTED:
        return STATUS_NOT_SUPPORTED;
    case EFI_BAD_BUFFER_SIZE:
        return STATUS_INVALID_BUFFER_SIZE;
    case EFI_BUFFER_TOO_SMALL:
        return STATUS_BUFFER_TOO_SMALL;
    case EFI_DEVICE_ERROR:
        return STATUS_IO_DEVICE_ERROR;
    case EFI_WRITE_PROTECTED:
        return STATUS_MEDIA_WRITE_PROTECTED;
    case EFI_OUT_OF_RESOURCES:
        return STATUS_INSUFFICIENT_NVRAM_RESOURCES;
    case EFI_VOLUME_CORRUPTED:
        return STATUS_DISK_CORRUPT_ERROR;
    case EFI_VOLUME_FULL:
        return STATUS_DISK_FULL;
    case EFI_NO_MEDIA:
        return STATUS_NO_MEDIA;
    case EFI_MEDIA_CHANGED:
        return STATUS_MEDIA_CHANGED;
    case EFI_NOT_FOUND:
    case EFI_NOT_READY:
        return STATUS_NOT_FOUND;
    case EFI_ACCESS_DENIED:
    case EFI_SECURITY_VIOLATION:
        return STATUS_ACCESS_DENIED;
    case EFI_NO_MAPPING:
        return STATUS_NO_MATCH;
    case EFI_TIMEOUT:
    case EFI_NO_RESPONSE:
        return STATUS_TIMEOUT;
    case EFI_NOT_STARTED:
        return STATUS_DEVICE_NOT_READY;
    case EFI_ALREADY_STARTED:
        return STATUS_DEVICE_ALREADY_ATTACHED;
    case EFI_ABORTED:
        return STATUS_REQUEST_ABORTED;
    default:
        return STATUS_UNSUCCESSFUL;
    }
}
