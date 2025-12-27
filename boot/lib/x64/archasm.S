/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    archasm.S

Abstract:

    Boot library architecture services for x64 processors.

--*/

.text

#define CR0_PG (1 << 31)

#define CR4_LA57 (1 << 12)

#define IA32_EFER_MSR 0xc0000080
#define IA32_EFER_LME (1 << 10)

SetSegmentRegisters:
    movw 0x18(%rcx), %ds
    movw 0x1a(%rcx), %es
    movw 0x1e(%rcx), %gs
    movw 0x1c(%rcx), %fs
    movw 0x20(%rcx), %ss
    retq

.globl ArchTrapNoProcess
ArchTrapNoProcess:
    iretq

.globl Archpx64EnableInterruptsAsm
Archpx64EnableInterruptsAsm:
    sti
    retq

.globl Archpx64DisableInterruptsAsm
Archpx64DisableInterruptsAsm:
    cli
    retq

.globl ArchGetIdtRegister
ArchGetIdtRegister:
    sidtq (%rcx)
    retq

.globl ArchSetIdtRegister
ArchSetIdtRegister:
    lidtq (%rcx)
    retq

.globl ArchSetDescriptorTableContext
ArchSetDescriptorTableContext:
    lgdtq 0x00(%rcx)
    lidtq 0x0a(%rcx)
    lldtw 0x14(%rcx)
    pushq 0x16(%rcx)
    lea SetSegmentRegisters(%rip), %rax
    pushq %rax
    lretq

.globl BlpArchGetCodeSegmentSelector
BlpArchGetCodeSegmentSelector:
    movw %cs, %ax
    retq

.globl BlpArchGetDescriptorTableContext
BlpArchGetDescriptorTableContext:
    sgdtq 0x00(%rcx)
    sidtq 0x0a(%rcx)
    sldtw 0x14(%rcx)
    movw %cs, 0x16(%rcx)
    movw %ds, 0x18(%rcx)
    movw %es, 0x1a(%rcx)
    movw %fs, 0x1c(%rcx)
    movw %gs, 0x1e(%rcx)
    movw %ss, 0x20(%rcx)
    retq

.globl BlArchIsFiveLevelPagingActive
BlArchIsFiveLevelPagingActive:
    /* Paging must be enabled */
    movq %cr0, %rax
    testl $CR0_PG, %eax
    jz 1f

    /* Long mode must be enabled */
    movl $IA32_EFER_MSR, %ecx
    rdmsr
    testl $IA32_EFER_LME, %eax
    jz 1f

    /* 57-bit linear addressing must be enabled */
    movq %cr4, %rax
    testl $CR4_LA57, %eax
    jz 1f

    movb $1, %al
    retq
1:
    xorb %al, %al
    retq

.global BlArchCpuId
BlArchCpuId:
    pushq %rbx
    movl %ecx, %eax
    movl %edx, %ecx
    movq %r8, %r9
    cpuid
    movl %eax, 0x00(%r9)
    movl %ebx, 0x04(%r9)
    movl %ecx, 0x08(%r9)
    movl %edx, 0x0c(%r9)
    popq %rbx
    retq
