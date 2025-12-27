/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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
#include "arch.h"

#define REQUIRED_XSAVE_FEATURES (CPUID_XSAVE_FEATURE_XSAVEOPT | CPUID_XSAVE_FEATURE_XSAVEC | CPUID_XSAVE_FEATURE_XGETBV_ECX1)

#define SET_IDT_ENTRY(_Base, _Index, _Present, _Type, _Selector, _Offset, _Dpl, _IstIndex) \
    { \
        PKIDTENTRY64 _Entry; \
        _Entry = &(((PKIDTENTRY64)(_Base))[(_Index)]); \
        _Entry->OffsetLow = (USHORT)(ULONG_PTR)(_Offset); \
        _Entry->Selector = (_Selector); \
        _Entry->IstIndex = (_IstIndex); \
        _Entry->Reserved0 = 0; \
        _Entry->Type = (_Type); \
        _Entry->Dpl = (_Dpl); \
        _Entry->Present = (_Present); \
        _Entry->OffsetMiddle = (USHORT)((ULONG)(ULONG_PTR)(_Offset) >> 16); \
        _Entry->OffsetHigh = (ULONG)((ULONGLONG)(ULONG_PTR)(_Offset) >> 32); \
        _Entry->Reserved1 = 0; \
    }

#define SET_IDT_ENTRY_TRAP(_Base, _Index, _Present, _Selector, _Offset, _Dpl, _IstIndex) SET_IDT_ENTRY(_Base, _Index, _Present, 0xe, _Selector, _Offset, _Dpl, _IstIndex)

EXECUTION_CONTEXT ApplicationExecutionContext, FirmwareExecutionContext;
PEXECUTION_CONTEXT CurrentExecutionContext = NULL;
PTXT_PRIVATE_SPACE TxtPrivateSpace = NULL;
BOOLEAN ArchForceNx = FALSE;
BOOLEAN ArchDisableNx = FALSE;
ULONG ArchCr4BitsToClear = 0;
ULONG ArchXCr0BitsToClear = 0;

VOID
ArchTrapNoProcess (
    VOID
    );

/*++

Routine Description:

    Receives a trap interrupt without processing it.

    Implemented in archasm.S.

Arguments:

    None.

Return Value:

    None.

--*/

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

VOID
ArchGetIdtRegister (
    OUT PDESCRIPTOR_TABLE_REGISTER Idtr
    );

/*++

Routine Description:

    Gets the current value of IDTR (IDT Register).

    Implemented in archasm.S.

Arguments:

    Idtr - Pointer to a DESCRIPTOR_TABLE_REGISTER that receives the value of IDTR.

Return Value:

    None.

--*/

VOID
ArchSetIdtRegister (
    IN PDESCRIPTOR_TABLE_REGISTER Idtr
    );

/*++

Routine Description:

    Loads a new value into IDTR (IDT Register).

    Implemented in archasm.S.

Arguments:

    Idtr - Pointer to the value to load into IDTR.

Return Value:

    None.

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

        __writecr3(NewContext->Cr3);
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
    IN PEXECUTION_CONTEXT Context
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
    Context->Cr3 = __readcr3();
    BlpArchGetDescriptorTableContext(&Context->DescriptorTableContext);

    //
    // Check if 5-level paging is active.
    //
    if (BlArchIsFiveLevelPagingActive()) {
        //
        // TODO: Finish implementing this routine.
        //
        Status = STATUS_SUCCESS;
        // Status = ArchInitializePagingLevelChangeRoutineVector(Unknown);
        // if (NT_SUCCESS(Status)) {
        //     Context->Attributes |= EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED;
        // }
    } else {
        Context->Attributes &= ~EXECUTION_CONTEXT_5_LEVEL_PAGING_ENABLED;
        Status = STATUS_SUCCESS;
    }

    //
    // Enable FXSAVE and FXRSTOR.
    //
    __writecr4(__readcr4() | CR4_OSFXSR);

    return Status;
}

VOID
ArchEnableProcessorFeatures (
    VOID
    )

/*++

Routine Description:

    Enables processor-specific features.

Arguments:

    None.

Return Value:

    None.

--*/

{
    ULONG_PTR Cr4, XCr0;

    if (!(BlPlatformFlags & PLATFORM_FLAG_XSAVE_SUPPORTED)) {
        return;
    }

    //
    // Enable XSAVE.
    //
    Cr4 = __readcr4();
    if (!(Cr4 & CR4_OSXSAVE)) {
        Cr4 |= CR4_OSXSAVE;
        __writecr4(Cr4);
        ArchCr4BitsToClear = CR4_OSXSAVE;
    }

    //
    // Enable XSAVE for AVX state.
    //
    XCr0 = _xgetbv(0);
    if (!(XCr0 & XCR0_AVX)) {
        XCr0 |= XCR0_AVX;
        _xsetbv(0, XCr0);
        ArchXCr0BitsToClear = XCR0_AVX;
    }
}

VOID
ArchRestoreProcessorFeatures (
    IN BOOLEAN DisableNx
    )

/*++

Routine Description:

    Restores processor-specific features.

Arguments:

    DisableNx - Whether or not to disable NX.

Return Value:

    None.

--*/

{
    //
    // Clear unwanted XCR0 bits.
    //
    if (ArchXCr0BitsToClear != 0) {
        _xsetbv(0, _xgetbv(0) & ~ArchXCr0BitsToClear);
        ArchXCr0BitsToClear = 0;
    }

    //
    // Clear unwanted CR4 bits.
    //
    if (ArchCr4BitsToClear != 0) {
        __writecr4(__readcr4() & ~ArchCr4BitsToClear);
        ArchCr4BitsToClear = 0;
    }

    //
    // Disable NX.
    //
    if (DisableNx) {
        if (ArchDisableNx) {
            __writemsr(IA32_EFER, __readmsr(IA32_EFER) & ~IA32_EFER_NXE);
        }

        if (ArchForceNx) {
            __writemsr(IA32_MISC_ENABLE, __readmsr(IA32_MISC_ENABLE) | 0x400000000);
        }
    }
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
    ULONG CpuVendor;
    CPUID_DATA CpuIdData;
    ULONGLONG Efer;

    //
    // Disable miscellaneous features.
    //
    CpuVendor = BlArchGetCpuVendor();
    if (CpuVendor == CPU_VENDOR_INTEL) {
        __writemsr(IA32_MISC_ENABLE, __readmsr(IA32_MISC_ENABLE) & ~0x003fffff);
    }

    //
    // Enable XSAVE features.
    //
    BlArchCpuId(CPUID_FUNCTION_GET_FEATURES, 0, &CpuIdData);
    if ((CpuIdData.Ecx & CPUID_FEATURE_ECX_XSAVE) && BlArchIsCpuIdFunctionSupported(CPUID_FUNCTION_GET_XSAVE_FEATURES)) {
        BlArchCpuId(CPUID_FUNCTION_GET_XSAVE_FEATURES, 0, &CpuIdData);
        if ((CpuIdData.Eax & REQUIRED_XSAVE_FEATURES) == REQUIRED_XSAVE_FEATURES) {
            BlPlatformFlags |= PLATFORM_FLAG_XSAVE_SUPPORTED;
        }
    }

    if (BlpLibraryParameters.Flags & BOOT_LIBRARY_FLAG_ENABLE_NX) {
        //
        // Force NX if possible.
        //
        BlArchCpuId(CPUID_FUNCTION_GET_EXTENDED_FEATURES, 0, &CpuIdData);
        if (!(CpuIdData.Edx & CPUID_EXTENDED_FEATURE_EDX_NX) && CpuVendor == CPU_VENDOR_INTEL) {
            __writemsr(IA32_MISC_ENABLE, __readmsr(IA32_MISC_ENABLE) & ~0xffffffff);
            ArchForceNx = TRUE;
        }

        //
        // Enable NX in EFER.
        //
        Efer = __readmsr(IA32_EFER);
        if (!(Efer & IA32_EFER_NXE)) {
            __writemsr(IA32_EFER, Efer | IA32_EFER_NXE);
            ArchDisableNx = TRUE;
        }
        BlPlatformFlags |= PLATFORM_FLAG_NX_SUPPORTED;
    }

    //
    // Enable the configured features.
    //
    ArchEnableProcessorFeatures();
}

USHORT
BlpArchGetCodeSegmentSelector (
    VOID
    );

/*++

Routine Description:

    Gets the current value of CS.

    Implemented in archasm.S.

Arguments:

    None.

Return Value:

    The current value of CS.

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
BlArchCpuId (
    IN  ULONG       Eax,
    IN  ULONG       Ecx,
    OUT PCPUID_DATA Result
    );

/*++

Routine Description:

    Fetches processor identification data.

    Implemented in archasm.S.

Arguments:

    Function - The CPUID function to query.

    Ecx - The value of ECX.

    Result - Pointer to a CPUID_DATA that receives the result.

Return Value:

    None.

--*/

BOOLEAN
BlArchIsCpuIdFunctionSupported (
    IN ULONG Function
    )

/*++

Routine Description:

    Determines whether the processor supports a CPUID function.

Arguments:

    Function - The CPUID function.

Return Value:

    TRUE if the function is supported.

    FALSE if the function is not supported.

--*/

{
    CPUID_DATA CpuIdData;

    BlArchCpuId(Function & 0x80000000, 0, &CpuIdData);

    return Function <= CpuIdData.Eax;
}

ULONG
BlArchGetCpuVendor (
    VOID
    )

/*++

Routine Description:

    Identifies the processor's vendor.

Arguments:

    None.

Return Value:

    CPU_VENDOR_*.

--*/

{
    CPUID_DATA CpuIdData;
    PSTR VendorString;

    //
    // Get CPUID data.
    //
    BlArchCpuId(CPUID_FUNCTION_GET_VENDOR, 0, &CpuIdData);

    //
    // Match vendor string in EBX:ECX:EDX.
    //
    VendorString = (PSTR)&CpuIdData.Ebx;
    if (strncmp(VendorString, CPUID_VENDOR_STRING_INTEL, CPUID_VENDOR_STRING_LENGTH) == 0) {
        return CPU_VENDOR_INTEL;
    } else if (strncmp(VendorString, CPUID_VENDOR_STRING_AMD, CPUID_VENDOR_STRING_LENGTH) == 0) {
        return CPU_VENDOR_AMD;
    } else if (strncmp(VendorString, CPUID_VENDOR_STRING_CENTAUR, CPUID_VENDOR_STRING_LENGTH) == 0) {
        return CPU_VENDOR_CENTAUR;
    } else {
        return CPU_VENDOR_UNKNOWN;
    }
}

VOID
BlpArchInstallTrapVectors (
    VOID
    )

/*++

Routine Description:

    Installs trap vectors to handle interrupts.

Arguments:

    None.

Return Value:

    None.

--*/

{
    DESCRIPTOR_TABLE_REGISTER Idtr;
    USHORT Cs;

    //
    // Get current state.
    //
    ArchGetIdtRegister(&Idtr);

    //
    // Install trap handler entries.
    //
    Cs = BlpArchGetCodeSegmentSelector();
    SET_IDT_ENTRY_TRAP(Idtr.Base, 0x03, 1, Cs, ArchTrapNoProcess, 0, 0);
    SET_IDT_ENTRY_TRAP(Idtr.Base, 0x2c, 1, Cs, ArchTrapNoProcess, 0, 0);
    SET_IDT_ENTRY_TRAP(Idtr.Base, 0x2d, 1, Cs, ArchTrapNoProcess, 0, 0);
    if (BlpEnvironmentState & ENVIRONMENT_STATE_HANDLE_DOUBLE_FAULTS) {
        SET_IDT_ENTRY_TRAP(Idtr.Base, 0x08, 1, Cs, BlpTxtUnhandledException, 0, 0);
    }

    //
    // Reload to apply changes.
    //
    ArchSetIdtRegister(&Idtr);
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

    if (Phase == 0) {
        //
        // Reset the TSC to prevent overflow.
        //
        if (__readmsr(IA32_TIME_STAMP_COUNTER) & 0xffc0000000000000) {
            __writemsr(IA32_TIME_STAMP_COUNTER, 0);
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
        if (BlPlatformFlags & PLATFORM_FLAG_FIRMWARE_EXECUTION_CONTEXT_SUPPORTED) {
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
#if !defined(NDEBUG)
    DebugInfo(L"Initializing architecture services (phase 1/1)...\r\n");
#endif
    //
    // Install trap vectors to handle breakpoints and exceptions.
    //
    if (!(BlpApplicationEntry.Attributes & BOOT_ENTRY_DEBUGGER_CONNECTED)) {
        BlpArchInstallTrapVectors();
    }

    return STATUS_SUCCESS;
}

VOID
NORETURN
BlpTxtUnhandledException (
    VOID
    )

/*++

Routine Description:

    Handles an unhandled exception by rebooting the system.

Arguments:

    None.

Return Value:

    Does not return.

--*/

{
    if (TxtPrivateSpace != NULL) {
        TxtPrivateSpace->Status = 0xC0018001;
    }

    //
    // Reboot the system.
    //
    BlFwReboot();
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
