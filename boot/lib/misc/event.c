/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    event.c

Abstract:

    Event services.

--*/

#include "bootlib.h"

BOOLEAN EnSubsystemInitialized = FALSE;
LIST_ENTRY EnEventNotificationList;

NTSTATUS
BlpEnDestroy (
    VOID
    )

/*++

Routine Description:

    Shuts down the event notification subsystem.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS.

--*/

{
    DebugInfo(L"Destroying event notification subsystem...\r\n");

    //
    // TODO: Finish implementing this routine.
    //

    EnSubsystemInitialized = FALSE;
    return STATUS_SUCCESS;
}

VOID
BlEnNotifyEvent (
    IN ULONG Event,
    IN PVOID Context
    )

/*++

Routine Description:

    Sends an event notification.

Arguments:

    Event - The event identifier.

    Context - Pointer to the event context.

Return Value:

    None.

--*/

{
    (VOID) Context;

    DebugInfo(L"Got notification for event 0x%x\r\n", Event);

    //
    // TODO: Implement this routine.
    //
}

VOID
BlpEnInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the event notification subsystem.

Arguments:

    None.

Return Value:

    None.

--*/

{
    DebugInfo(L"Initializing event notification subsystem...\r\n");

    EnSubsystemInitialized = TRUE;
    InitializeListHead(&EnEventNotificationList);
}
