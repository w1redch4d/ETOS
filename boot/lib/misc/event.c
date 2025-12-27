/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    event.c

Abstract:

    Event services.

--*/

#include "bootlib.h"

ULONG EnNotificationInProgress = 0;
BOOLEAN EnSubsystemInitialized = FALSE;
BOOLEAN EnDeregistrationPending = FALSE;
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

    if (Event == 0 || (Event >> 28) > 2) {
        return;
    }

    EnNotificationInProgress++;

    //
    // TODO: Finish implementing this routine.
    //

    EnNotificationInProgress--;

    if (!EnDeregistrationPending) {
        EnDeregistrationPending = FALSE;
    }
}

NTSTATUS
BlEnRegisterEventHandler (
    IN ULONG          Event,
    IN PEVENT_HANDLER Handler,
    IN ULONG          Unknown0,
    IN ULONG          Unknown1
    )

/*++

Routine Description:

    Registers a handler to receive event notifications.

Arguments:

    Event - The event identifier.

    Handler - Pointer to the event handler.

    Unknown0 - Unknown.

    Unknown1 - Unknown.

Return Value:

    STATUS_SUCCESS.

--*/

{
    (VOID) Event;
    (VOID) Handler;
    (VOID) Unknown0;
    (VOID) Unknown1;

    //
    // TODO: Implement this routine.
    //

    return STATUS_SUCCESS;
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

    InitializeListHead(&EnEventNotificationList);
    EnSubsystemInitialized = TRUE;
}
