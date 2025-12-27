/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efibind.h

Abstract:

    x64-specific EFI definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef _EFIBIND_H
#define _EFIBIND_H

//
// EFI requires structure alignment.
//
#ifndef __GNUC__
    #pragma pack()
#endif

//
// EFI service calling convention.
//
#ifndef EFIAPI
    #ifdef _MSC_EXTENSIONS
        #define EFIAPI __cdecl
    #elif defined(__clang__) || defined(__GNUC__)
        #define EFIAPI __attribute__((ms_abi))
    #else
        #define EFIAPI
    #endif
#endif

//
// Fixed-width integer types.
//
#ifdef _MSC_EXTENSIONS
    typedef __int8             INT8;
    typedef __int16            INT16;
    typedef __int32            INT32;
    typedef __int64            INT64;

    typedef unsigned __int8    UINT8;
    typedef unsigned __int16   UINT16;
    typedef unsigned __int32   UINT32;
    typedef unsigned __int64   UINT64;
#else
    typedef signed char        INT8;
    typedef signed short int   INT16;
    typedef signed int         INT32;

    typedef unsigned char      UINT8;
    typedef unsigned short int UINT16;
    typedef unsigned int       UINT32;

    typedef signed long int   INT64;
    typedef unsigned long int UINT64;
#endif

//
// Native-width integer types.
//

typedef INT64  INTN;
typedef UINT64 UINTN;

#define MIN_INTN  (((INTN) -9223372036854775807L) - 1)
#define MAX_INTN  ((INTN)  0x7fffffffffffffffUL)
#define MAX_UINTN ((UINTN) 0xffffffffffffffffUL)

//
// EFI_STATUS error masks.
//
#define EFI_ERROR_MASK     0x8000000000000000
#define EFI_ERROR_MASK_OEM 0xc000000000000000

#endif // _EFIBIND_H
