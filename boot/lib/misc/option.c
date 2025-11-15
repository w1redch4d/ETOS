/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    option.c

Abstract:

    Boot option services.

--*/

#include "bootlib.h"
#include <wchar.h>

PBOOT_OPTION_CALLBACKS BlpBootOptionCallbacks = NULL;
ULONGLONG BlpBootOptionCallbackCookie;

PBOOT_ENTRY_OPTION
BcdUtilGetBootOption (
    IN PBOOT_ENTRY_OPTION Options,
    IN BCDE_DATA_TYPE     Type
    )

/*++

Routine Description:

    Retrieves a boot option of type Type.

Arguments:

    Options - Pointer to the boot option list.

    Type - The type of option to search for.

Return Value:

    Pointer to the option if found.

    NULL if not found.

--*/

{
    ULONG NextOptionOffset;
    PBOOT_ENTRY_OPTION Option;

    if (Options == NULL) {
        return NULL;
    }

    NextOptionOffset = 0;
    do {
        Option = (PBOOT_ENTRY_OPTION)((ULONG_PTR)Options + NextOptionOffset);
        if (Option->Type == Type && !Option->IsInvalid) {
            return Option;
        }

        //
        // Recursively search additional options.
        //
        if (Option->AdditionalOptionsOffset != 0) {
            Option = BcdUtilGetBootOption((PBOOT_ENTRY_OPTION)((ULONG_PTR)Option + Option->AdditionalOptionsOffset), Type);
            if (Option != NULL) {
                return Option;
            }
        }

        NextOptionOffset = Option->NextOptionOffset;
    } while (NextOptionOffset != 0);

    return NULL;
}

ULONG
BlGetBootOptionSize (
    IN PBOOT_ENTRY_OPTION Option
    )

/*++

Routine Description:

    Gets the size of a boot option.

Arguments:

    Option - Pointer to the boot option.

Return Value:

    The size of the option.

--*/

{
    ULONG TotalSize;

    if (Option->DataOffset != 0) {
        TotalSize = Option->DataOffset + Option->DataSize;
    } else {
        TotalSize = sizeof(BOOT_ENTRY_OPTION);
    }

    //
    // Recursively add size of additional options.
    //
    if (Option->AdditionalOptionsOffset != 0) {
        TotalSize += BlGetBootOptionListSize((PBOOT_ENTRY_OPTION)((ULONG_PTR)Option + Option->AdditionalOptionsOffset));
    }

    return TotalSize;
}

ULONG
BlGetBootOptionListSize (
    IN PBOOT_ENTRY_OPTION Options
    )

/*++

Routine Description:

    Gets the total size of a list boot options.

Arguments:

    Options - Pointer to the boot option list.

Return Value:

    The size of the option list.

--*/

{
    ULONG TotalSize, Offset;
    PBOOT_ENTRY_OPTION Option;

    TotalSize = 0;
    Offset = 0;
    do {
        Option = (PBOOT_ENTRY_OPTION)((ULONG_PTR)Options + Offset);
        Offset = Option->NextOptionOffset;
        TotalSize += BlGetBootOptionSize(Option);
    } while (Offset != 0);

    return TotalSize;
}

NTSTATUS
BlMergeBootOptionLists (
    IN     PBOOT_ENTRY_OPTION OptionsA,
    IN     PBOOT_ENTRY_OPTION OptionsB,
    IN     PVOID              Buffer,
    IN OUT PULONG             BufferSize
    )

/*++

Routine Description:

    Merges two boot option lists.

Arguments:

    OptionsA - The first list.

    OptionsB - The second list.

    Buffer - Pointer to a buffer to store the new list in.

    BufferSize - Pointer to the size of the buffer (0 to get required size).

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an overflow occurs.

    STATUS_BUFFER_TOO_SMALL if BufferSize is too small.

--*/

{
    ULONG SizeA, SizeB, TotalSize, NextOptionOffset, RealOffset;
    PBOOT_ENTRY_OPTION Option;

    //
    // Calculate the total size of the combined list.
    //
    SizeA = BlGetBootOptionListSize(OptionsA);
    SizeB = BlGetBootOptionListSize(OptionsB);
    TotalSize = SizeA + SizeB;
    if (TotalSize < SizeA) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Require enough space to hold the whole list.
    //
    if (*BufferSize < TotalSize) {
        *BufferSize = TotalSize;
        return STATUS_BUFFER_TOO_SMALL;
    }

    //
    // Copy the option data.
    //
    RtlMoveMemory(Buffer, OptionsA, SizeA);
    RtlMoveMemory((PVOID)((ULONG_PTR)Buffer + SizeA), OptionsB, SizeB);

    //
    // Seek to the end of the first list.
    //
    NextOptionOffset = 0;
    do {
        Option = (PBOOT_ENTRY_OPTION)((ULONG_PTR)Buffer + NextOptionOffset);
        NextOptionOffset = Option->NextOptionOffset;
    } while (NextOptionOffset != 0);

    //
    // Fix the offsets in the second list.
    //
    do {
        RealOffset = NextOptionOffset + SizeA;
        if (RealOffset < NextOptionOffset) {
            return STATUS_INTEGER_OVERFLOW;
        }

        Option->NextOptionOffset = RealOffset;
        Option = (PBOOT_ENTRY_OPTION)((ULONG_PTR)Buffer + RealOffset);
        NextOptionOffset = Option->NextOptionOffset;
    } while (NextOptionOffset != 0);

    return STATUS_SUCCESS;
}

NTSTATUS
BlpBootOptionCallbackString (
    IN  NTSTATUS       Status,
    IN  PGUID          Identifier,
    IN  BCDE_DATA_TYPE Type,
    IN  PWSTR          DefaultString,
    IN  ULONG          DefaultStringLength,
    OUT PWSTR          *FilteredString,
    OUT PULONG         FilteredStringLength
    )

/*++

Routine Description:

    Calls the BCD filter string callback, if registered.

Arguments:

    Status - The current status of the operation.

    Identifier - Pointer to the requestor's unique identifier.

    Type - The requested option type.

    DefaultString - Pointer to a string containing the option's default value.

    DefaultString - The length of DefaultString.

    FilteredString - Pointer to a PWSTR that receives the address of the filtered string.

    FilteredStringLength - Pointer to a ULONG that receives the length of FilteredString.

Return Value:

    Any status code returned by the callback if registered.

    Status if no callback is registered.

--*/

{
    //
    // Run the callback.
    //
    if (BlpBootOptionCallbacks->String != NULL) {
        return BlpBootOptionCallbacks->String(
            BlpBootOptionCallbackCookie,
            Status,
            0,
            Identifier,
            Type,
            DefaultString,
            DefaultStringLength,
            FilteredString,
            FilteredStringLength
        );
    }

    //
    // Return default values if no callback is registered.
    //
    *FilteredString = DefaultString;
    *FilteredStringLength = DefaultStringLength;
    return Status;
}

NTSTATUS
BlGetBootOptionDevice (
    IN  PBOOT_ENTRY_OPTION Options,
    IN  BCDE_DATA_TYPE     Type,
    OUT PDEVICE_IDENTIFIER *IdentifierOut,
    OUT PBOOT_ENTRY_OPTION *AdditionalOptionsOut OPTIONAL
    )

/*++

Routine Description:

    Retrieves a boot option of type Type as a device.

Arguments:

    Options - Pointer to the boot option list.

    Type - The type of option to search for.

    Identifier - Receives a pointer to a buffer containing the device's identifier.

    AdditionalOptions - Receives a pointer to a buffer containing the device's additional options.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Type is not of format BCDE_FORAMT_DEVICE.

    STATUS_NOT_FOUND if no matching option could be found.

    STATUS_NO_MEMORY if memory allocation fails.

    Any other status value returned by the BCD filter callback.

--*/

{
    NTSTATUS Status;
    PBOOT_ENTRY_OPTION Option, OriginalAdditionalOptions, DefaultAdditionalOptions, SelectedAdditionalOptions;
    PDEVICE_IDENTIFIER OriginalIdentifier, DefaultIdentifier, SelectedIdentifier;
    ULONG OriginalAdditionalOptionsSize;

    //
    // Validate the requested option type.
    //
    if ((Type & BCDE_FORMAT_MASK) != BCDE_FORMAT_DEVICE) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Find an option of the requested type.
    //
    Option = BcdUtilGetBootOption(Options, Type);
    if (Option != NULL) {
        Status = STATUS_SUCCESS;
    } else {
        Status = STATUS_NOT_FOUND;
        DefaultIdentifier = NULL;
        DefaultAdditionalOptions = NULL;
        goto RunFilter;
    }

    //
    // Copy the device's identifier.
    //
    OriginalIdentifier = &((PBCDE_DEVICE)((ULONG_PTR)Option + Option->DataOffset))->Identifier;
    DefaultIdentifier = BlMmAllocateHeap(OriginalIdentifier->Size);
    if (DefaultIdentifier == NULL) {
        return STATUS_NO_MEMORY;
    }
    RtlMoveMemory(DefaultIdentifier, OriginalIdentifier, OriginalIdentifier->Size);

    //
    // Continue if additional options were not requested or are not available.
    //
    if (AdditionalOptionsOut == NULL || Option->AdditionalOptionsOffset == 0) {
        DefaultAdditionalOptions = NULL;
        goto RunFilter;
    }

    //
    // Copy the additional options list.
    //
    OriginalAdditionalOptions = (PBOOT_ENTRY_OPTION)((ULONG_PTR)Option + Option->AdditionalOptionsOffset);
    OriginalAdditionalOptionsSize = BlGetBootOptionListSize(OriginalAdditionalOptions);
    DefaultAdditionalOptions = BlMmAllocateHeap(OriginalAdditionalOptionsSize);
    if (DefaultAdditionalOptions == NULL) {
        BlMmFreeHeap(DefaultIdentifier);
        return STATUS_NO_MEMORY;
    }
    RtlMoveMemory(DefaultAdditionalOptions, OriginalAdditionalOptions, OriginalAdditionalOptionsSize);

RunFilter:
    SelectedIdentifier = DefaultIdentifier;
    SelectedAdditionalOptions = DefaultAdditionalOptions;

    //
    // Pass through BCD filter callback if registered.
    //
    if (BlpBootOptionCallbacks != NULL && BlpBootOptionCallbacks->Device != NULL) {
        Status = BlpBootOptionCallbacks->Device(
            BlpBootOptionCallbackCookie,
            Status,
            0,
            BlGetApplicationIdentifier(),
            Type,
            &SelectedIdentifier,
            &SelectedAdditionalOptions
        );
    }

    //
    // Free allocated memory.
    //

    if (SelectedAdditionalOptions != DefaultAdditionalOptions && DefaultAdditionalOptions != NULL) {
        BlMmFreeHeap(DefaultAdditionalOptions);
        DefaultAdditionalOptions = NULL;
    }

    if (SelectedIdentifier != DefaultIdentifier && DefaultIdentifier != NULL) {
        BlMmFreeHeap(DefaultIdentifier);
        DefaultIdentifier = NULL;
    }

    //
    // Return results.
    //
    if (NT_SUCCESS(Status)) {
        if (AdditionalOptionsOut != NULL) {
            *AdditionalOptionsOut = SelectedAdditionalOptions;
        }

        *IdentifierOut = SelectedIdentifier;
    }

    return Status;
}

NTSTATUS
BlGetBootOptionString (
    IN  PBOOT_ENTRY_OPTION Options,
    IN  BCDE_DATA_TYPE     Type,
    OUT PWSTR              *StringOut
    )

/*++

Routine Description:

    Retrieves a boot option of type Type as a string.

Arguments:

    Options - Pointer to the boot option list.

    Type - The type of option to search for.

    String - Receives a pointer to a buffer containing the string.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Type is not of format BCDE_FORAMT_STRING.

    STATUS_NOT_FOUND if no matching option could be found.

    STATUS_NO_MEMORY if memory allocation fails.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

    Any other status value returned by the BCD filter callback.

--*/

{
    NTSTATUS Status;
    PBOOT_ENTRY_OPTION Option;
    PWSTR DefaultString, FilteredString, FinalString;
    ULONG DefaultStringLength, FilteredStringLength, FinalStringSize;
    size_t FilteredStringSize;

    //
    // Validate the requested option type.
    //
    if ((Type & BCDE_FORMAT_MASK) != BCDE_FORMAT_STRING) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Find an option of the requested type.
    //
    Option = BcdUtilGetBootOption(Options, Type);
    if (Option != NULL) {
        Status = STATUS_SUCCESS;
        DefaultString = (PWSTR)((ULONG_PTR)Option + Option->DataOffset);
        DefaultStringLength = Option->DataSize / sizeof(WCHAR);
    } else {
        Status = STATUS_NOT_FOUND;
        DefaultString = NULL;
        DefaultStringLength = 0;
    }

    //
    // Pass through BCD filter callback if registered.
    //
    FilteredString = DefaultString;
    FilteredStringLength = DefaultStringLength;
    if (BlpBootOptionCallbacks != NULL) {
        Status = BlpBootOptionCallbackString(
            Status,
            BlGetApplicationIdentifier(),
            Type,
            DefaultString,
            DefaultStringLength,
            &FilteredString,
            &FilteredStringLength
        );
    }

    //
    // Return if no option was found.
    //
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    //
    // Calculate the size of the filtered string.
    //
    FilteredStringSize = (FilteredStringLength * sizeof(WCHAR)) + sizeof(UNICODE_NULL);
    if (FilteredStringSize > MAXULONG) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Copy the string.
    //
    FinalStringSize = (ULONG)FilteredStringSize;
    FinalString = BlMmAllocateHeap(FinalStringSize);
    if (FinalString == NULL) {
        return STATUS_NO_MEMORY;
    }
    RtlMoveMemory(FinalString, FilteredString, FinalStringSize - sizeof(UNICODE_NULL));
    FinalString[FilteredStringLength] = UNICODE_NULL;

    //
    // Return result.
    //
    *StringOut = FinalString;
    return STATUS_SUCCESS;
}

NTSTATUS
BlGetBootOptionBoolean (
    IN  PBOOT_ENTRY_OPTION Options,
    IN  BCDE_DATA_TYPE     Type,
    OUT PBOOLEAN           ValueOut
    )

/*++

Routine Description:

    Retrieves a boot option of type Type as a BOOLEAN.

Arguments:

    Options - Pointer to the boot option list.

    Type - The type of option to search for.

    Value - Receives the option's value.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Type is not of format BCDE_FORMAT_BOOLEAN.

    STATUS_NOT_FOUND if no matching option could be found.

    Any other status value returned by the BCD filter callback.

--*/

{
    NTSTATUS Status;
    PBOOT_ENTRY_OPTION Option;
    BOOLEAN Value;

    //
    // Validate the requested option type.
    //
    if ((Type & BCDE_FORMAT_MASK) != BCDE_FORMAT_BOOLEAN) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Find an option of the requested type.
    //
    Option = BcdUtilGetBootOption(Options, Type);
    if (Option != NULL) {
        Status = STATUS_SUCCESS;
        Value = *(PBOOLEAN)((ULONG_PTR)Option + Option->DataOffset);
    } else {
        Status = STATUS_NOT_FOUND;
    }

    //
    // Pass through BCD filter callback if registered.
    //
    if (BlpBootOptionCallbacks != NULL && BlpBootOptionCallbacks->Boolean != NULL) {
        Status = BlpBootOptionCallbacks->Boolean(
            BlpBootOptionCallbackCookie,
            Status,
            0,
            BlGetApplicationIdentifier(),
            Type,
            &Value
        );
    }

    //
    // Return result.
    //
    if (NT_SUCCESS(Status)) {
        *ValueOut = Value;
    }

    return Status;
}

NTSTATUS
BlAppendBootOptions (
    IN PBOOT_APPLICATION_ENTRY BootEntry,
    IN PBOOT_ENTRY_OPTION      Options
    )

/*++

Routine Description:

    Appends additional options to a boot entry's option list.

Arguments:

    BootEntry - Pointer to the boot entry.

    Options - Pointer to the options to append.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_UNSUCCESSFUL if buffer size probing fails.

    STATUS_NO_MEMORY if buffer allocation fails.

    Any other error code returned by BlMergeBootOptionLists.

--*/

{
    NTSTATUS Status;
    ULONG BufferSize;
    PVOID Buffer;

    //
    // Get required buffer size.
    //
    BufferSize = 0;
    Status = BlMergeBootOptionLists(BootEntry->Options, Options, NULL, &BufferSize);
    if (NT_SUCCESS(Status)) {
        return STATUS_UNSUCCESSFUL;
    }

    //
    // Abort if a different error occurred.
    //
    if (Status != STATUS_BUFFER_TOO_SMALL) {
        return Status;
    }

    Buffer = BlMmAllocateHeap(BufferSize);
    if (Buffer == NULL) {
        return STATUS_NO_MEMORY;
    }

    Status = BlMergeBootOptionLists(BootEntry->Options, Options, Buffer, &BufferSize);
    if (!NT_SUCCESS(Status)) {
        BlMmFreeHeap(Buffer);
        return Status;
    }

    //
    // Free old options.
    //
    if (BootEntry->Attributes & BOOT_ENTRY_OPTIONS_INTERNAL) {
        BlMmFreeHeap(BootEntry->Options);
    }

    //
    // Use new options.
    //
    BootEntry->Options = Buffer;
    BootEntry->Attributes &= ~BOOT_ENTRY_OPTIONS_EXTERNAL;
    BootEntry->Attributes |= BOOT_ENTRY_OPTIONS_INTERNAL;

    return STATUS_SUCCESS;
}

NTSTATUS
BlAppendBootOptionString (
    IN PBOOT_APPLICATION_ENTRY BootEntry,
    IN BCDE_DATA_TYPE          Type,
    IN PWSTR                   String
    )

/*++

Routine Description:

    Appends a string option to a boot entry's option list.

Arguments:

    BootEntry - Pointer to the boot entry.

    Type - The type to assign the option.

    String - The string to use as the option's data.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_NO_MEMORY if buffer allocation fails.

    Any other error code returned by BlAppendBootOptions.

--*/

{
    NTSTATUS Status;
    size_t StringSize;
    ULONG DataSize, TotalSize;
    PBOOT_ENTRY_OPTION Option;

    //
    // Find value size, in bytes.
    //
    StringSize = wcslen(String) * sizeof(WCHAR);
    if (StringSize > MAXULONG) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Account for NULL terminator and convert to ULONG.
    //
    DataSize = StringSize + sizeof(UNICODE_NULL);
    if (DataSize < StringSize) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Allocate temporary buffer for option.
    //
    TotalSize = sizeof(BOOT_ENTRY_OPTION) + DataSize;
    Option = BlMmAllocateHeap(TotalSize);
    if (Option == NULL) {
        return STATUS_NO_MEMORY;
    }

    //
    // Fill in option structure.
    //
    RtlZeroMemory(Option, TotalSize);
    Option->Type = Type;
    Option->DataOffset = sizeof(BOOT_ENTRY_OPTION);
    Option->DataSize = DataSize;
    wcscpy_s((PWSTR)((ULONG_PTR)Option + Option->DataOffset), Option->DataSize / sizeof(WCHAR), String);

    //
    // Append option to list and free allocated memory.
    //
    Status = BlAppendBootOptions(BootEntry, Option);
    BlMmFreeHeap(Option);
    return Status;
}
