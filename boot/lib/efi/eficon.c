/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    eficon.c

Abstract:

    EFI console services.

--*/

#include "efilib.h"
#include <wchar.h>

UCHAR BlScratchBuffer[0x4000];

VOID
PrintFormatted (
    IN PCSTR   Source,
    IN PWSTR   Format,
    IN va_list Arguments,
    IN PWSTR   TypeString,
    IN UINTN   TypeAttribute
    )

/*++

Routine Description:

    Internal routine to print a formatted string to the console.

Arguments:

    Format - Pointer to the format string.

    Arguments - Variable argument list.

    TypeString - Pointer to a string containing the type of message.

    TypeAttribute - The console attributes to set when printing TypeString.

Return Value:

    None.

--*/

{
    EXECUTION_CONTEXT_TYPE ContextType;
    int BufferUsed;
    WCHAR SourceBuffer[64];
    ULONG Index;

    //
    // Process format and arguments.
    //
    BufferUsed = vswprintf_s((PWCHAR)BlScratchBuffer, (sizeof(BlScratchBuffer) / sizeof(WCHAR)) - 1, Format, Arguments);

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    //
    // Print styled type string.
    //
    EfiConOut->SetAttribute(EfiConOut, TypeAttribute);
    EfiConOut->OutputString(EfiConOut, TypeString);
    EfiConOut->SetAttribute(EfiConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_BLACK));

    //
    // Print source string, if present.
    //
    if (Source != NULL) {
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

        EfiConOut->OutputString(EfiConOut, SourceBuffer);
    }

    //
    // Print formatted message.
    //
    if (BufferUsed >= 1) {
        EfiConOut->OutputString(EfiConOut, (PWSTR)BlScratchBuffer);
    }

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }
}

VOID
EfiPrintf (
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted string to the console.

Arguments:

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;
    int BufferUsed;
    EXECUTION_CONTEXT_TYPE ContextType;

    //
    // Process format and arguments.
    //
    va_start(Arguments, Format);
    BufferUsed = vswprintf_s((PWCHAR)BlScratchBuffer, (sizeof(BlScratchBuffer) / sizeof(WCHAR)) - 1, Format, Arguments);
    va_end(Arguments);
    if (BufferUsed < 1) {
        return;
    }

    ContextType = CurrentExecutionContext->Type;
    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ExecutionContextFirmware);
    }

    //
    // Print formatted message.
    //
    EfiConOut->OutputString(EfiConOut, (PWSTR)BlScratchBuffer);

    if (ContextType != ExecutionContextFirmware) {
        BlpArchSwitchContext(ContextType);
    }
}

VOID
ConsoleInfo (
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted informational message to the console.

Arguments:

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;

    va_start(Arguments, Format);
    PrintFormatted(NULL, Format, Arguments, L"[Info] ", EFI_TEXT_ATTR(EFI_LIGHTCYAN, EFI_BLACK));
    va_end(Arguments);
}

VOID
ConsoleWarning (
    IN PCSTR Source,
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted warning message to the console.

Arguments:

    Source - Pointer to a string that describes the reporter of the warning.

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;

    va_start(Arguments, Format);
    PrintFormatted(Source, Format, Arguments, L"[Warning] ", EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLACK));
    va_end(Arguments);
}

VOID
ConsoleError (
    IN PCSTR Source,
    IN PWSTR Format,
    ...
    )

/*++

Routine Description:

    Prints a formatted error message to the console.

Arguments:

    Source - Pointer to a string that describes the reporter of the error.

    Format - Pointer to the format string.

    ... - Additional arguments.

Return Value:

    None.

--*/

{
    va_list Arguments;

    va_start(Arguments, Format);
    PrintFormatted(Source, Format, Arguments, L"[Error] ", EFI_TEXT_ATTR(EFI_LIGHTRED, EFI_BLACK));
    va_end(Arguments);
}
