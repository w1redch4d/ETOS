/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    ntrtl.h

Abstract:

    Provides RTL (Run-Time Library) definitions.

--*/

#pragma once

#ifndef _NTRTL_H
#define _NTRTL_H

#include <string.h>

//
// Memory operations.
//
#define RtlMoveMemory(Destination, Source, Length) memmove((Destination), (Source), (Length))
#define RtlCopyMemory(Destination, Source, Length) memcpy((Destination), (Source), (Length))
#define RtlFillMemory(Destination, Length, Fill)   memset((Destination), (Fill), (Length))
#define RtlZeroMemory(Destination, Length)         memset((Destination), 0, (Length))

FORCEINLINE
VOID
InitializeListHead (
    IN PLIST_ENTRY Head
    )

/*++

Routine Description:

    Initializes a list head.

Arguments:

    Head - Pointer to the list's head.

Return Value:

    None.

--*/

{
    Head->Blink = Head;
    Head->Flink = Head;
}

FORCEINLINE
VOID
InsertHeadList (
    IN PLIST_ENTRY Head,
    IN PLIST_ENTRY Entry
    )

/*++

Routine Description:

    Inserts an entry at the head of a list.

Arguments:

    Entry - Pointer to the list entry to insert.

Return Value:

    None.

--*/

{
    Entry->Flink = Head->Flink;
    Entry->Blink = Head;
    Head->Flink->Blink = Entry;
    Head->Flink = Entry;
}

FORCEINLINE
VOID
InsertTailList (
    IN PLIST_ENTRY Head,
    IN PLIST_ENTRY Entry
    )

/*++

Routine Description:

    Inserts an entry at the tail of a list.

Arguments:

    Entry - Pointer to the list entry to insert.

Return Value:

    None.

--*/

{
    Entry->Blink = Head->Blink;
    Entry->Flink = Head;
    Head->Blink->Flink = Entry;
    Head->Blink = Entry;
}

FORCEINLINE
BOOLEAN
RemoveEntryList (
    IN PLIST_ENTRY Entry
    )

/*++

Routine Description:

    Removes an entry from a list.

Arguments:

    Entry - Pointer to the entry to remove.

Return Value:

    TRUE if the list is now empty,
    FALSE if the list still has at least one entry.

--*/

{
    PLIST_ENTRY Blink, Flink;

    Blink = Entry->Blink;
    Flink = Entry->Flink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;

    return (BOOLEAN)(Flink == Blink);
}

#define ULONG_ERROR 0xFFFFFFFFUL

FORCEINLINE
NTSTATUS
RtlULongSub (
   IN     ULONG  ulMinuend,
   IN     ULONG  ulSubtrahend,
   IN OUT PULONG pulResult
   )

/*++

Routine Description:

    Calculates the difference of two ULONG values.

Arguments:

    ulMinuend - The value to subtract ulSubtrahend from.

    ulSubtrahend - The value to subtract from ulMinuend.

    pulResult - Pointer to a ULONG to store the difference in.

Return Value:

    STATUS_SUCCESS if successful.
    STATUS_INTEGER_OVERFLOW if unsuccessful.

--*/

{
    if (ulMinuend >= ulSubtrahend) {
        *pulResult = ulMinuend - ulSubtrahend;
        return STATUS_SUCCESS;
    }

    *pulResult = ULONG_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

VOID
NTAPI
RtlInitUnicodeString (
   OUT PUNICODE_STRING Destination,
   IN  PCWSTR          Source
   );

NTSTATUS
NTAPI
RtlGUIDFromString (
   IN  PUNICODE_STRING String,
   OUT GUID            *Guid
   );

#endif /* !_NTRTL_H */
