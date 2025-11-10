/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efibind.h

Abstract:

    Provides processor/compiler-specific EFI definitions for i386 systems.

--*/

#pragma once

#ifndef _EFIBIND_H
#define _EFIBIND_H

//
// EFI requires structure alignment.
//
#if !defined(__GNUC__)
    #pragma pack()
#endif

//
// Compiler-specific calling convention.
//
#ifndef EFIAPI
    #if defined(_MSC_EXTENSIONS)
        #define EFIAPI __cdecl
    #elif defined(__clang__) || defined(__GNUC__)
        #define EFIAPI __attribute__((cdecl))
    #else
        #define EFIAPI
    #endif
#endif

//
// Fixed-width integer types.
//
#if defined(_MSC_EXTENSIONS)
    typedef unsigned __int8    UINT8;
    typedef unsigned __int16   UINT16;
    typedef unsigned __int32   UINT32;
    typedef unsigned __int64   UINT64;
    typedef __int8             INT8;
    typedef __int16            INT16;
    typedef __int32            INT32;
    typedef __int64            INT64;
#else
    typedef unsigned char      UINT8;
    typedef unsigned short     UINT16;
    typedef unsigned int       UINT32;
    typedef unsigned long long UINT64;
    typedef signed char        INT8;
    typedef short              INT16;
    typedef int                INT32;
    typedef long long          INT64;
#endif

//
// Native-width integer types.
//
typedef UINT32 UINTN;
typedef INT32  INTN;

//
// Error masks.
//
#define EFI_ERROR_MASK     0x80000000
#define EFI_ERROR_MASK_OEM 0xc0000000

#endif /* !_EFIBIND_H */
