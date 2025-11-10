/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    guid.c

Abstract:

    RTL GUID routines.

--*/

#include <nt.h>
#include <stdarg.h>

static int
ScanHexFormat (
    IN PCWSTR Buffer,
    IN ULONG  MaximumLength,
    IN PCWSTR Format,
    ...
    )

/*++

Routine Description:

    Parses a formatted hex string.

Arguments:

    Buffer - The source string to parse.

    MaximumLength - The length of the source string.

    Format - The format of the source string.

    ... - Pointers to data destinations.

Return Value:

    Number of parameters scanned if successful.

    -1 if scanning fails.

--*/

{
    va_list Args;
    int FormatCount = 0;
    int Width, Long;
    ULONG Number;
    PVOID Dest;

    va_start(Args, Format);
    while (*Format != '\0') {
        //
        // Ignore some characters.
        //
        if (*Format != '%' || *(Format + 1) == '%') {
            if (!MaximumLength || *Buffer != *Format) {
                return -1;
            }

            Buffer++;
            MaximumLength--;
            Format++;
            continue;
        }

        //
        // Find format kind.
        //
        Format++;
        Width = 0;
        Long = 0;
        while (*Format != 'X' && *Format != 'x') {
            if (*Format >= '0' && *Format <= '9') {
                Width = Width * 10 + *Format - '0';
            } else if (*Format == 'l') {
                Long++;
            }
            Format++;
        }

        //
        // Calculate number value.
        //
        Format++;
        Number = 0;
        while (Width--) {
            if (!MaximumLength) {
                return -1;
            }

            if (*Buffer >= '0' && *Buffer <= '9') {
                Number += *Buffer - '0';
            } else if (*Buffer >= 'A' && *Buffer <= 'F') {
                Number += *Buffer - 'A' + 0xA;
            } else if (*Buffer >= 'a' && *Buffer <= 'f') {
                Number += *Buffer - 'a' + 0xa;
            } else {
                return -1;
            }

            Buffer++;
            MaximumLength--;
        }

        //
        // Store number in destination.
        //
        Dest = va_arg(Args, PVOID);
        if (Long) {
            *(PULONG)Dest = Number;
        } else {
            *(PUSHORT)Dest = (USHORT)Number;
        }

        FormatCount++;
    }
    va_end(Args);

    return (MaximumLength && *Buffer) ? -1:FormatCount;
}

NTSTATUS
NTAPI
RtlGUIDFromString (
   IN  PUNICODE_STRING String,
   OUT GUID            *Guid
   )

/*++

Routine Description:

    Turns a text representation of a GUID into the binary format.

Arguments:

    String - A GUID string in the format {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}.

    Guid - The GUID structure which is to recieve the data.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if the string is invalid.

--*/

{
    USHORT Data4[8];

    //
    // Convert string to GUID data.
    //
    if (
        ScanHexFormat(
            String->Buffer, String->Length / sizeof(WCHAR),
            L"{%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
            &Guid->Data1, &Guid->Data2, &Guid->Data3, &Data4[0], &Data4[1], &Data4[2], &Data4[3], &Data4[4], &Data4[5], &Data4[6], &Data4[7]
            ) == -1
        ) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Convert USHORTs to UCHARs.
    //
    for (int Index = 0; Index < 8; Index++) {
        Guid->Data4[Index] = (UCHAR)Data4[Index];
    }

    return STATUS_SUCCESS;
}
