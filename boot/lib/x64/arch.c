/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    arch.c

Abstract:

    Boot library architecture services for x64 processors.

--*/

#if defined(_EFI)
#include "efilib.h"
#else
#include "bootlib.h"
#endif
#include "mm.h"

#define CR4_OSFXSR (1 << 9)

#define IA32_TIME_STAMP_COUNTER 0x00000010

EXECUTION_CONTEXT ApplicationExecutionContext, FirmwareExecutionContext;
PEXECUTION_CONTEXT CurrentExecutionContext;

VOID
Archpx64EnableInterruptsAsm (
    VOID
    );

/*++

Routine Description:

    Enables interrupts.

    Implemented in archasm.S.

Arguments:

    None.

Return Value:

    None.

--*/

VOID
Archpx64DisableInterruptsAsm (
    VOID
    );

/*++

Routine Description:

    Disables interrupts.

    Implemented in archasm.S.

Arguments:

    None.

Return Value:

    None.

--*/

UCHAR
BlArchIsFiveLevelPagingActive (
    VOID
    );

/*++

Routine Description:

    Determines if five-level paging is activated.

    Implemented in archasm.S.

Arguments:

    None.

Return Value:

    Nonzero if active.

    Zero if inactive.

--*/

VOID
ArchSetDescriptorTableContext (
    IN PDESCRIPTOR_TABLE_CONTEXT Context
    );

/*++

Routine Description:

    Loads current descriptor values from a descriptor table context structure.

    Implemented in archasm.S.

Arguments:

    Context - Pointer to the context structure.

Return Value:

    None.

--*/

VOID
ArchSetPagingContext (
    IN PEXECUTION_CONTEXT NewContext,
    IN PEXECUTION_CONTEXT CurrentContext
    )

/*++

Routine Description:

    Loads the current paging context.

Arguments:

    NewContext - The context to switch to.

    CurrentContext - The currently loaded context.

Return Value:

    None.

--*/

{
    BOOLEAN Have5LevelPaging, Need5LevelPaging;

    //
    // Check if 5-level paging is enabled.
    //
    if (CurrentContext != NULL) {
        Have5LevelPaging = (CurrentContext->Attributes & EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED) ? TRUE : FALSE;
    } else {
        Have5LevelPaging = BlArchIsFiveLevelPagingActive() ? TRUE : FALSE;
    }

    //
    // Check if 5-level paging is needed.
    //
    Need5LevelPaging = (NewContext->Attributes & EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED) ? TRUE : FALSE;

    //
    // If 5-level paging is not being enabled or disabled, just set CR3.
    //
    if (Have5LevelPaging == Need5LevelPaging) {
        //
        // Don't reload CR3 if both contexts use the same value.
        // Otherwise we would be flushing the TLB and wasting time.
        //
        if (CurrentContext != NULL && CurrentContext->Cr3 == NewContext->Cr3) {
            return;
        }

        asm volatile("mov %0, %%cr3" ::"r"(NewContext->Cr3));
        return;
    }

    //
    // TODO: Implement 5-level paging.
    //
    DebugError(L"5-level paging not implemented\r\n");
}

VOID
ArchSwitchContext (
    IN PEXECUTION_CONTEXT NewContext,
    IN PEXECUTION_CONTEXT CurrentContext
    )

/*++

Routine Description:

    Switches to a specified execution context.

Arguments:

    NewContext - The context to switch to.

    CurrentContext - The currently loaded context.

Return Value:

    None.

--*/

{
    if (CurrentContext != NULL && CurrentContext->Attributes & EXECUTION_CONTEXT_INTERRUPTS_ENABLED) {
        Archpx64DisableInterruptsAsm();
    }

    //
    // Set descriptor table and paging contexts, in the correct order.
    //
    if (NewContext->Type == ExecutionContextFirmware) {
        ArchSetPagingContext(NewContext, CurrentContext);
        ArchSetDescriptorTableContext(&NewContext->DescriptorTableContext);
    } else {
        ArchSetDescriptorTableContext(&NewContext->DescriptorTableContext);
        ArchSetPagingContext(NewContext, CurrentContext);
    }

    if (NewContext->Attributes & EXECUTION_CONTEXT_INTERRUPTS_ENABLED) {
        Archpx64EnableInterruptsAsm();
    }
}

NTSTATUS
ArchInitializeContext (
    IN OUT PEXECUTION_CONTEXT Context
    )

/*++

Routine Description:

    Initializes an execution context.

Arguments:

    Context - Pointer to the context structure.

Return Value:

    STATUS_SUCCESS if successful.

--*/

{
    NTSTATUS Status;
    ULONG_PTR Cr4;

    if (Context->Type == ExecutionContextFirmware) {
        Context->Attributes &= ~(EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED | EXECUTION_CONTEXT_INTERRUPTS_DISABLED);
        Context->Attributes |= EXECUTION_CONTEXT_INTERRUPTS_ENABLED;

        //
        // Use context data from firmware.
        //
#if defined(_EFI)
        Context->Cr3 = EfiFirmwareParameters->Cr3;
        RtlCopyMemory(&Context->DescriptorTableContext, &EfiFirmwareParameters->DescriptorTableContext, sizeof(Context->DescriptorTableContext));
#endif

        return STATUS_SUCCESS;
    }

    //
    // Application execution contexts must use address translation.
    //
    if (BlpLibraryParameters.TranslationType == TRANSLATION_TYPE_NONE) {
        return STATUS_NOT_SUPPORTED;
    }

    //
    // Use current state.
    //
    Context->Attributes &= ~EXECUTION_CONTEXT_INTERRUPTS_ENABLED;
    Context->Attributes |= EXECUTION_CONTEXT_INTERRUPTS_DISABLED;
    asm volatile("mov %%cr3, %0" :"=r"(Context->Cr3));
    BlpArchGetDescriptorTableContext(&Context->DescriptorTableContext);

    //
    // Check if 5-level paging is active.
    //
    if (!BlArchIsFiveLevelPagingActive()) {
        Context->Attributes &= ~EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED;
        Status = STATUS_SUCCESS;
        goto Finish;
    }

    //
    // TODO: Finish implementing this routine.
    //
    Status = STATUS_SUCCESS;
    // Status = ArchInitializePagingLevelChangeRoutineVector(Unknown);
    // if (NT_SUCCESS(Status)) {
    //     Context->Attributes |= EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED;
    // }

Finish:
    //
    // Enable FXSAVE and FXRSTOR.
    //
    asm volatile("mov %%cr4, %0" :"=r"(Cr4));
    Cr4 |= CR4_OSFXSR;
    asm volatile("mov %0, %%cr4" ::"r"(Cr4));

    return Status;
}

VOID
ArchInitializeProcessorFeatures (
    VOID
    )

/*++

Routine Description:

    Initializes processor-specific features.

Arguments:

    None.

Return Value:

    None.

--*/

{
    //
    // TODO: Implement this routine.
    //
}

VOID
ArchRestoreProcessorFeatures (
    IN BOOLEAN Unknown
    )

/*++

Routine Description:

    Restores processor-specific features.

Arguments:

    Unknown - Ignored.

Return Value:

    None.

--*/

{
    (VOID) Unknown;

    //
    // TODO: Implement this routine.
    //
}

VOID
BlpArchInstallTrapVectors (
    VOID
    )

/*++

Routine Description:

    Installs trap vectors to handle exceptions.

Arguments:

    None.

Return Value:

    None.

--*/

{
    //
    // TODO: Implement this routine.
    //
}

VOID
BlpArchSwitchContext (
    IN EXECUTION_CONTEXT_TYPE Type
    )

/*++

Routine Description:

    Switches to an execution context of the requested type.

Arguments:

    Type - The desired context type.

Return Value:

    None.

--*/

{
    PEXECUTION_CONTEXT NewContext;

    if (Type == ExecutionContextFirmware) {
        NewContext = &FirmwareExecutionContext;
    } else {
        NewContext = &ApplicationExecutionContext;
    }

    if (CurrentExecutionContext->Type == NewContext->Type) {
        return;
    }

#if defined(_EFI)
    if (EfiBS == NULL) {
        __fastfail(0x30);
    }
#endif

    ArchSwitchContext(NewContext, CurrentExecutionContext);
    CurrentExecutionContext = NewContext;
}

NTSTATUS
BlpArchInitialize (
    IN ULONG Phase
    )

/*++

Routine Description:

    Performs architecture-specific initialization.

Arguments:

    Phase - Which phase of initialization to perform.

Return Value:

    STATUS_SUCCESS if successful.

    Any error code returned by ArchInitializeContext.

--*/

{
    NTSTATUS Status;
    ULONGLONG Tsc;

    if (Phase == 0) {
        //
        // Reset the TSC to prevent overflow.
        //
        asm volatile("rdmsr" :"=A"(Tsc) :"c"(IA32_TIME_STAMP_COUNTER));
        if (Tsc & 0xffc0000000000000) {
            asm volatile("wrmsr": :"c"(IA32_TIME_STAMP_COUNTER), "a"(0), "d"(0));
        }

        //
        // Initialize and use application context.
        //
        ApplicationExecutionContext.Type = ExecutionContextApplication;
        ApplicationExecutionContext.Attributes = 0;
        Status = ArchInitializeContext(&ApplicationExecutionContext);
        if (NT_SUCCESS(Status)) {
            CurrentExecutionContext = &ApplicationExecutionContext;
        } else {
            CurrentExecutionContext = NULL;
        }

        //
        // Initialize firmware context if supported.
        //
        FirmwareExecutionContext.Type = ExecutionContextFirmware;
        FirmwareExecutionContext.Attributes = 0;
        if (BlPlatformFlags & PLATFORM_FLAG_FIRMWARE_EXECUTION_CONTEXT) {
            Status = ArchInitializeContext(&FirmwareExecutionContext);
            if (!NT_SUCCESS(Status)) {
                ArchInitializeProcessorFeatures();
                return Status;
            }

            //
            // Use firmware execution context if the application context is not supported.
            //
            if (CurrentExecutionContext == NULL) {
                CurrentExecutionContext = &FirmwareExecutionContext;
            }
        }

        //
        // Switch to the correct context.
        //
        ArchSwitchContext(CurrentExecutionContext, NULL);

        //
        // Initialize processor features.
        //
        ArchInitializeProcessorFeatures();

        return STATUS_SUCCESS;
    }

    //
    // Install trap vectors to handle exceptions.
    //
    if (!(BlpApplicationEntry.Attributes & BOOT_ENTRY_NO_TRAP_VECTORS)) {
        BlpArchInstallTrapVectors();
    }

    return STATUS_SUCCESS;
}

BOOLEAN
MmArchTranslateVirtualAddress (
    IN  PVOID             VirtualAddress,
    OUT PPHYSICAL_ADDRESS PhysicalAddress
    )

/*++

Routine Description:

    Translates a virtual address to its equivalent physical address.

Arguments:

    VirtualAddress - The virtual address to translate.

    PhysicalAddress - Pointer to a PHYSICAL_ADDRESS tha receives the translated address.

Return Value:

    TRUE if successful.

    FALSE if unsuccessful.

--*/

{
    //
    // TODO: Implement this routine.
    //

    if (PhysicalAddress != NULL) {
        PhysicalAddress->QuadPart = (ULONGLONG)VirtualAddress;
    }

    return TRUE;
}
