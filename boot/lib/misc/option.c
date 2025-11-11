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
