/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    arch.h

Abstract:

    Provides x64-specific definitions.

--*/

#pragma once

#ifndef _ARCH_H
#define _ARCH_H

#include <immintrin.h>

//
// CR0 (Control Register 0) bits.
//
#define CR0_PE (1 << 0)
#define CR0_MP (1 << 1)
#define CR0_EM (1 << 2)
#define CR0_TS (1 << 3)
#define CR0_ET (1 << 4)
#define CR0_NE (1 << 5)
#define CR0_WP (1 << 16)
#define CR0_AM (1 << 18)
#define CR0_NW (1 << 29)
#define CR0_CD (1 << 30)
#define CR0_PG (1 << 31)

//
// CR3 (Control Register 3) bits.
//
#define CR3_PWT (1 << 3)
#define CR3_PCD (1 << 5)

//
// CR4 (Control Register 4) bits.
//
#define CR4_VME        (1 << 0)
#define CR4_PVI        (1 << 1)
#define CR4_TSD        (1 << 2)
#define CR4_DE         (1 << 3)
#define CR4_PSE        (1 << 4)
#define CR4_PAE        (1 << 5)
#define CR4_MCE        (1 << 6)
#define CR4_PGE        (1 << 7)
#define CR4_PCE        (1 << 8)
#define CR4_OSFXSR     (1 << 9)
#define CR4_OSXMMEXCPT (1 << 10)
#define CR4_UMIP       (1 << 11)
#define CR4_LA57       (1 << 12)
#define CR4_VMXE       (1 << 13)
#define CR4_SMXE       (1 << 14)
#define CR4_FSGSBASE   (1 << 16)
#define CR4_PCIDE      (1 << 17)
#define CR4_OSXSAVE    (1 << 18)
#define CR4_SMEP       (1 << 20)
#define CR4_SMAP       (1 << 21)
#define CR4_PKE        (1 << 22)
#define CR4_CET        (1 << 23)
#define CR4_PKS        (1 << 24)

//
// XCR0 (Extended Control Register 0) bits.
//
#define XCR0_X87       (1 << 0)
#define XCR0_SSE       (1 << 1)
#define XCR0_AVX       (1 << 2)
#define XCR0_BNDREG    (1 << 3)
#define XCR0_BNDCSR    (1 << 4)
#define XCR0_OPMASK    (1 << 5)
#define XCR0_ZMM_HI256 (1 << 6)
#define XCR0_HI16_ZMM  (1 << 7)
#define XCR0_PKRU      (1 << 9)

//
// MSR (Model-Specific Register) IDs.
//
#define IA32_P5_MC_ADDR           0x00000000
#define IA32_PC_MC_TYPE           0x00000001
#define IA32_MONITOR_FILTER_SIZE  0x00000006
#define IA32_TIME_STAMP_COUNTER   0x00000010
#define IA32_PLATFORM_ID          0x00000017
#define IA32_APIC_BASE            0x0000001b
#define IA32_BARRIER              0x0000002f
#define IA32_FEATURE_CONTROL      0x0000003a
#define IA32_TSC_ADJUST           0x0000003b
#define IA32_SPEC_CTRL            0x00000048
#define IA32_PRED_CMD             0x00000049
#define IA32_PPIN_CTL             0x0000004e
#define IA32_PPIN                 0x0000004f
#define IA32_BIOS_UPDT_TRIG       0x00000079
#define IA32_FEATURE_ACTIVATION   0x0000007a
#define IA32_MCU_ENUMERATION      0x0000007b
#define IA32_MCU_STATUS           0x0000007c
#define IA32_FZM_RANGE_INDEX      0x00000082
#define IA32_FZM_DOMAIN_CONFIG    0x00000083
#define IA32_FZM_RANGE_STARTADDR  0x00000084
#define IA32_FZM_RANGE_ENDADDR    0x00000085
#define IA32_SMBASE               0x0000009e
#define IA32_XAPIC_DISABLE_STATUS 0x000000bd
#define IA32_SYSENTER_CS          0x00000174
#define IA32_SYSENTER_ESP         0x00000175
#define IA32_SYSENTER_EIP         0x00000176
#define IA32_MCG_CAP              0x00000179
#define IA32_OVERCLOCKING_STATUS  0x00000195
#define IA32_MISC_ENABLE          0x000001a0
#define IA32_EFER                 0xc0000080

//
// EFER (Extended Feature Enable Register) bits.
//
#define IA32_EFER_NXE (1 << 11)

//
// CPUID function IDs.
//
#define CPUID_FUNCTION_GET_VENDOR            0x00000000
#define CPUID_FUNCTION_GET_FEATURES          0x00000001
#define CPUID_FUNCTION_GET_XSAVE_FEATURES    0x0000000d
#define CPUID_FUNCTION_GET_EXTENDED_FEATURES 0x80000001

//
// CPUID vendor strings.
//
#define CPUID_VENDOR_STRING_LENGTH 12
#define CPUID_VENDOR_STRING_INTEL   "GenuineIntel"
#define CPUID_VENDOR_STRING_AMD     "AuthenticAMD"
#define CPUID_VENDOR_STRING_CENTAUR "CentaurHauls"

//
// CPUID feature bits.
//

#define CPUID_FEATURE_ECX_XSAVE (1 << 26)

#define CPUID_EXTENDED_FEATURE_EDX_NX (1 << 20)

#define CPUID_XSAVE_FEATURE_XSAVEOPT    (1 << 0)
#define CPUID_XSAVE_FEATURE_XSAVEC      (1 << 1)
#define CPUID_XSAVE_FEATURE_XGETBV_ECX1 (1 << 2)
#define CPUID_XSAVE_FEATURE_XSS         (1 << 3)
#define CPUID_XSAVE_FEATURE_XFD         (1 << 4)

//
// CPUID return data.
//
typedef struct {
    ULONG Eax;
    ULONG Ebx;
    ULONG Ecx;
    ULONG Edx;
} CPUID_DATA, *PCPUID_DATA;

//
// Boot library CPU vendor IDs.
//
#define CPU_VENDOR_UNKNOWN 0
#define CPU_VENDOR_AMD     1
#define CPU_VENDOR_INTEL   2
#define CPU_VENDOR_CENTAUR 3

//
// IDT (Interrupt Descriptor Table) entry.
//
typedef struct _KIDTENTRY64 {
    union {
        struct {
            USHORT OffsetLow;
            USHORT Selector;
            USHORT IstIndex : 3;
            USHORT Reserved0 : 5;
            USHORT Type : 5;
            USHORT Dpl : 2;
            USHORT Present : 1;
            USHORT OffsetMiddle;
            ULONG  OffsetHigh;
            ULONG  Reserved1;
        };

        ULONGLONG Alignment;
    };
} KIDTENTRY64, *PKIDTENTRY64;

//
// TXT private space.
//
typedef struct {
    UCHAR    Unknown[48];
    NTSTATUS Status;
} TXT_PRIVATE_SPACE, *PTXT_PRIVATE_SPACE;

//
// Control register services.
//

ULONG_PTR
FORCEINLINE
__readcr3 (
    VOID
    )

/*++

Routine Description:

    Gets the current value of CR3.

Arguments:

    None.

Return Value:

    The current value of CR3.

--*/

{
    ULONG_PTR Cr3;

    asm volatile("mov %%cr3, %0" :"=r"(Cr3));

    return Cr3;
}

ULONG_PTR
FORCEINLINE
__readcr4 (
    VOID
    )

/*++

Routine Description:

    Gets the current value of CR4.

Arguments:

    None.

Return Value:

    The current value of CR4.

--*/

{
    ULONG_PTR Cr4;

    asm volatile("mov %%cr4, %0" :"=r"(Cr4));

    return Cr4;
}

#define __writecr3(Data) asm volatile("mov %0, %%cr3" ::"r"(Data))
#define __writecr4(Data) asm volatile("mov %0, %%cr4" ::"r"(Data))

//
// MSR (Model-Specific Register) services.
//

ULONGLONG
FORCEINLINE
__readmsr (
    IN ULONG Register
    )

/*++

Routine Description:

    Gets the current value of an MSR.

Arguments:

    Register - The MSR's identifier.

Return Value:

    The current value of the MSR.

--*/

{
    ULONGLONG Value;

    asm volatile("rdmsr" :"=A"(Value) :"c"(Register));

    return Value;
}

#define __writemsr(Register, Value) asm volatile("wrmsr": :"c"(Register), "A"(Value))

USHORT
BlpArchGetCodeSegmentSelector (
    VOID
    );

UCHAR
BlArchIsFiveLevelPagingActive (
    VOID
    );

VOID
BlArchCpuId (
    IN  ULONG       Function,
    IN  ULONG       Ecx,
    OUT PCPUID_DATA Result
    );

BOOLEAN
BlArchIsCpuIdFunctionSupported (
    IN ULONG Function
    );

ULONG
BlArchGetCpuVendor (
    VOID
    );

VOID
BlpArchInstallTrapVectors (
    VOID
    );

VOID
NORETURN
BlpTxtUnhandledException (
    VOID
    );

#endif /* !_ARCH_H */
