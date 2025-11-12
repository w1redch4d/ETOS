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
        Option = (PBOOT_ENTRY_OPTION)((PUCHAR)Options + NextOptionOffset);
        if (Option->Type == Type && !Option->IsInvalid) {
            return Option;
        }

        //
        // Recursively search additional options.
        //
        if (Option->AdditionalOptionsOffset != 0) {
            Option = BcdUtilGetBootOption((PBOOT_ENTRY_OPTION)((PUCHAR)Option + Option->AdditionalOptionsOffset), Type);
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
        TotalSize += BlGetBootOptionListSize((PBOOT_ENTRY_OPTION)((PUCHAR)Option + Option->AdditionalOptionsOffset));
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
        Option = (PBOOT_ENTRY_OPTION)((PUCHAR)Options + Offset);
        Offset = Option->NextOptionOffset;
        TotalSize += BlGetBootOptionSize(Option);
    } while (Offset != 0);

    return TotalSize;
}

NTSTATUS
BlGetBootOptionBoolean (
    IN  PBOOT_ENTRY_OPTION Options,
    IN  BCDE_DATA_TYPE     Type,
    OUT PBOOLEAN           Value
    )

/*++

Routine Description:

    Retrieves a boot option of type Type as a BOOLEAN.

Arguments:

    Options - Pointer to the boot option list.

    Type - The type of option to search for.

    Value - Pointer to a BOOLEAN that receives the option's value.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Type is not of format BCDE_FORAMT_BOOLEAN.

    STATUS_NOT_FOUND if no matching option could be found.

    Any other status value returned by the callback.

--*/

{
    NTSTATUS Status;
    PBOOT_ENTRY_OPTION Option;
    BOOLEAN Data;

    if ((Type & BCDE_FORMAT_MASK) != BCDE_FORMAT_BOOLEAN) {
        return STATUS_INVALID_PARAMETER;
    }

    Option = BcdUtilGetBootOption(Options, Type);
    if (Option != NULL) {
        Status = STATUS_SUCCESS;
        Data = *(PBOOLEAN)((PUCHAR)Option + Option->DataOffset);
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
            NULL,
            &Data
        );
    }

    if (NT_SUCCESS(Status)) {
        *Value = Data;
    }

    return Status;
}
