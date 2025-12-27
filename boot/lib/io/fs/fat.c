/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    fat.c

Abstract:

    FAT filesystem services.

--*/

#include "io.h"

NTSTATUS
FatDestroy (
    VOID
    )

/*++

Routine Description:

    Destroys the FAT filesystem driver.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS.

--*/

{
#if !defined(NDEBUG)
    DebugInfo(L"Destroying FAT filesystem driver...\r\n");
#endif
    //
    // TODO: Implement this routine.
    //

    return STATUS_SUCCESS;
}

NTSTATUS
FatInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the FAT filesystem driver.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS.

--*/

{
#if !defined(NDEBUG)
    DebugInfo(L"Initializing FAT filesystem driver...\r\n");
#endif
    //
    // TODO: Implement this routine.
    //

    return STATUS_SUCCESS;
}

FS_REGISTER_FUNCTION_TABLE FatRegisterFunctionTable = {
    .Initialize = FatInitialize,
    .Destroy    = FatDestroy,
    .Mount      = NULL,
    .Purge      = NULL
};
