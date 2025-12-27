/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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

#ifndef NDEBUG
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
EfiDebugSource (
    IN PCSTR Source,
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted string with source to the debugging console.

Arguments:

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;
    WCHAR Buffer[256], SourceBuffer[64];
    ULONG Index;

    //
    // Convert Source to a wide-character string.
    //
    Index = 0;
    while (*Source != '\0' && Index < ((sizeof(SourceBuffer) / sizeof(WCHAR)) - 3)) {
        SourceBuffer[Index++] = (WCHAR)*Source++;
    }
    SourceBuffer[Index++] = L':';
    SourceBuffer[Index++] = L' ';
    SourceBuffer[Index] = L'\0';

    //
    // Print source and message.
    //
    ConOut->OutputString(ConOut, SourceBuffer);
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
    EfiDebugPrintf(L"[ETOS] EFI early debug logging enabled\r\n");
}

#endif