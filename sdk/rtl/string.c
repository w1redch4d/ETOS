/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    string.c

Abstract:

    RTL string routines.

--*/

#include <nt.h>
#include <wchar.h>

VOID
NTAPI
RtlInitUnicodeString (
   OUT PUNICODE_STRING Destination,
   IN  PCWSTR          Source
   )

/*++

Routine Description:

    Initializes a unicode string structure with an existing string,
    which may be NULL to initialize an empty unicode string structure.

Arguments:

    Destination - Pointer to the unicode string structure.

    Source - Optionally, a pointer to a NULL-terminated string to initialize
        the unicode string structure with.

Return Value:

    None.

--*/

{
    ULONG Length;

    Destination->Buffer = (PWSTR)Source;
    if (Source == NULL) {
        Destination->Length = 0;
        Destination->MaximumLength = 0;
        return;
    }

    Length = wcslen(Source) * sizeof(WCHAR);
    if (Length >= UNICODE_STRING_MAX_BYTES) {
        Length = UNICODE_STRING_MAX_BYTES - sizeof(UNICODE_NULL);
    }

    Destination->Length = (USHORT)Length;
    Destination->MaximumLength = (USHORT)(Length + sizeof(UNICODE_NULL));
}
