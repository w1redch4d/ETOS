/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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
// Memory services.
//
#define RtlEqualMemory(Destination, Source, Length) (memcmp((Destination), (Source), (Length)) == 0)
#define RtlMoveMemory(Destination, Source, Length)  memmove((Destination), (Source), (Length))
#define RtlCopyMemory(Destination, Source, Length)  memcpy((Destination), (Source), (Length))
#define RtlFillMemory(Destination, Length, Fill)    memset((Destination), (Fill), (Length))
#define RtlZeroMemory(Destination, Length)          memset((Destination), 0, (Length))

//
// Doubly-linked list services.
//

#define IsListEmpty(Head) ((Head)->Flink == (Head))

VOID
FORCEINLINE
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

VOID
FORCEINLINE
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
    PLIST_ENTRY Flink;

    Flink = Head->Flink;
    Entry->Flink = Flink;
    Entry->Blink = Head;
    Flink->Blink = Entry;
    Head->Flink = Entry;
}

VOID
FORCEINLINE
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
    PLIST_ENTRY Blink;

    Blink = Head->Blink;
    Entry->Flink = Head;
    Entry->Blink = Blink;
    Blink->Flink = Entry;
    Head->Blink = Entry;
}

BOOLEAN
FORCEINLINE
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
    PLIST_ENTRY Flink, Blink;

    Flink = Entry->Flink;
    Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;

    return (BOOLEAN)(Flink == Blink);
}

PLIST_ENTRY
FORCEINLINE
RemoveHeadList (
    IN PLIST_ENTRY Head
    )

/*++

Routine Description:

    Removes the first entry from a list.

Arguments:

    Head - Pointer to the list's head.

Return Value:

    Pointer to the removed entry.

--*/

{
    PLIST_ENTRY Entry, Flink;

    Entry = Head->Flink;
    Flink = Entry->Flink;
    Head->Flink = Flink;
    Flink->Blink = Head;

    return Entry;
}

PLIST_ENTRY
FORCEINLINE
RemoveTailList (
    IN PLIST_ENTRY Head
    )

/*++

Routine Description:

    Removes the last entry from a list.

Arguments:

    Head - Pointer to the list's head.

Return Value:

    Pointer to the removed entry.

--*/

{
    PLIST_ENTRY Entry, Blink;

    Entry = Head->Blink;
    Blink = Entry->Blink;
    Head->Blink = Blink;
    Blink->Flink = Head;

    return Entry;
}

VOID
NTAPI
RtlInitAnsiString (
    OUT PANSI_STRING Destination,
    IN  PCSTR        Source
    );

VOID
NTAPI
RtlInitUnicodeString (
   OUT PUNICODE_STRING Destination,
   IN  PCWSTR          Source
   );

NTSTATUS
NTAPI
RtlAnsiStringToUnicodeString (
    OUT PUNICODE_STRING DestinationString,
    IN  PCANSI_STRING   SourceString,
    IN  BOOLEAN         AllocateDestinationString
    );

NTSTATUS
NTAPI
RtlGUIDFromString (
   IN  PUNICODE_STRING String,
   OUT GUID            *Guid
   );

#endif /* !_NTRTL_H */
