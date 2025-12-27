/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    string.c

Abstract:

    String services.

--*/

#include "bootlib.h"

VOID
BlCopyStringToWcharString (
    OUT PWSTR Destination,
    IN  PSTR  Source
    )

/*++

Routine Description:

    Converts an ANSI string to a wide-character Unicode string.

Arguments:

    Destination - Pointer to the destination buffer.

    Source - Pointer to the source buffer.

Return Value:

    Any status code returned by RtlAnsiStringToUnicodeString.

--*/

{
    ANSI_STRING AnsiString;
    UNICODE_STRING UnicodeString;

    //
    // Create structures describing the buffers.
    //
    RtlInitAnsiString(&AnsiString, Source);
    UnicodeString.Buffer = Destination;
    UnicodeString.Length = 0;
    UnicodeString.MaximumLength = AnsiString.MaximumLength * sizeof(WCHAR);

    //
    // Convert the string.
    //
    RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);
}
