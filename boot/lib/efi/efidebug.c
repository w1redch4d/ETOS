/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efidebug.c

Abstract:

    EFI early debugging services.

--*/

#include "efilib.h"
#include <wchar.h>

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;

VOID
EfiDebugPrint (
    IN PWSTR String
    )

/*++

Routine Description:

    Prints a string to the debugging console.

Arguments:

    String - Pointer to the string to print.

Return Value:

    None.

--*/

{
    ConOut->OutputString(ConOut, String);
}

VOID
EfiDebugPrintf (
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted string to the debugging console.

Arguments:

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;
    WCHAR Buffer[256];

    va_start(Arguments, Format);
    if (vswprintf_s(Buffer, (sizeof(Buffer) / sizeof(WCHAR)) - 1, Format, Arguments) >= 1) {
        ConOut->OutputString(ConOut, Buffer);
    }
    va_end(Arguments);
}

VOID
EfiDebugInitialize (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Interface
    )

/*++

Routine Description:

    Initializes the debugging interface.

Arguments:

    Interface - Pointer to the console interface.

Return Value:

    None.

--*/

{
    ConOut = Interface;
    EfiDebugPrint(L"[ETOS] EFI early debug logging enabled\r\n");
}
