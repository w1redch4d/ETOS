/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stdint.h

Abstract:

    Integer type definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef __STDINT_H
#define __STDINT_H

//
// Detect processor word size.
//
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    #define __WORDSIZE 64
#else
    #define __WORDSIZE 32
#endif

//
// Fixed-width integer types.
//

#ifdef _MSC_EXTENSIONS
    typedef __int8  int8_t;
    typedef __int16 int16_t;
    typedef __int32 int32_t;
    typedef __int64 int64_t;

    typedef unsigned __int8  uint8_t;
    typedef unsigned __int16 uint16_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;
#else
    typedef signed char      int8_t;
    typedef signed short int int16_t;
    typedef signed int       int32_t;

    typedef unsigned char      uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned int       uint32_t;

    #if __WORDSIZE == 64
        typedef signed long int   int64_t;
        typedef unsigned long int uint64_t;
    #else
        __extension__ typedef signed long long int   int64_t;
        __extension__ typedef unsigned long long int uint64_t;
    #endif
#endif

#define INT8_C(c)  c
#define INT16_C(c) c
#define INT32_C(c) c

#define UINT8_C(c)  c
#define UINT16_C(c) c
#define UINT32_C(c) c ## U

#if __WORDSIZE == 64
    #define INT64_C(c)  c ## L
    #define UINT64_C(c) c ## UL
#else
    #define INT64_C(c)  c ## LL
    #define UINT64_C(c) c ## ULL
#endif

#define INT8_MIN  (-INT8_C(128))
#define INT16_MIN (-INT16_C(32767) - 1)
#define INT32_MIN (-INT32_C(2147483647) - 1)
#define INT64_MIN (-INT64_C(9223372036854775807) - 1)

#define INT8_MAX  (INT8_C(127))
#define INT16_MAX (INT16_C(32767))
#define INT32_MAX (INT32_C(2147483647))
#define INT64_MAX (INT64_C(9223372036854775807))

#define UINT8_MAX  (UINT8_C(255))
#define UINT16_MAX (UINT16_C(65535))
#define UINT32_MAX (UINT32_C(4294967295))
#define UINT64_MAX (UINT64_C(18446744073709551615))

//
// Minimum-width integer types.
//

typedef int8_t  int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX  INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

//
// "Fast" integer types.
//

typedef int8_t  int_fast8_t;
typedef int64_t int_fast64_t;

typedef uint8_t  uint_fast8_t;
typedef uint64_t uint_fast64_t;

#define INT_FAST8_MIN  INT8_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX  INT8_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX  UINT8_MAX
#define UINT_FAST64_MAX UINT64_MAX

#if __WORDSIZE == 64
    typedef int64_t int_fast16_t;
    typedef int64_t int_fast32_t;

    typedef uint64_t uint_fast16_t;
    typedef uint64_t uint_fast32_t;

    #define INT_FAST16_MIN INT64_MIN
    #define INT_FAST32_MIN INT64_MIN

    #define INT_FAST16_MAX INT64_MAX
    #define INT_FAST32_MAX INT64_MAX

    #define UINT_FAST16_MAX UINT64_MAX
    #define UINT_FAST32_MAX UINT64_MAX
#else
    typedef int32_t int_fast16_t;
    typedef int32_t int_fast32_t;

    typedef uint32_t uint_fast16_t;
    typedef uint32_t uint_fast32_t;

    #define INT_FAST16_MIN INT32_MIN
    #define INT_FAST32_MIN INT32_MIN

    #define INT_FAST16_MAX INT32_MAX
    #define INT_FAST32_MAX INT32_MAX

    #define UINT_FAST16_MAX UINT32_MAX
    #define UINT_FAST32_MAX UINT32_MAX
#endif

//
// Maximum-width integer types.
//

typedef int64_t  intmax_t;
typedef uint64_t uintmax_t;

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define INTMAX_C(c)  INT64_C(c)
#define UINTMAX_C(c) UINT64_C(c)

//
// Pointer-width integer types.
//
#if __WORDSIZE == 64
    typedef int64_t  intptr_t;
    typedef uint64_t uintptr_t;

    #define INTPTR_MIN  INT64_MIN
    #define INTPTR_MAX  INT64_MAX
    #define UINTPTR_MAX UINT64_MAX
#else
    typedef int32_t  intptr_t;
    typedef uint32_t uintptr_t;

    #define INTPTR_MIN  INT32_MIN
    #define INTPTR_MAX  INT32_MAX
    #define UINTPTR_MAX UINT32_MAX
#endif

//
// Additional type limits.
//

#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX

#define SIZE_MAX UINTPTR_MAX

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#ifndef WCHAR_MAX
    #ifdef __WCHAR_MAX__
        #define WCHAR_MAX __WCHAR_MAX__
    #elif (L'\0' - 1) > 0
        #define WCHAR_MAX (UINT32_MAX + L'\0')
    #else
        #define WCHAR_MAX (INT32_MAX + L'\0')
    #endif
#endif

#ifndef WCHAR_MIN
    #ifdef __WCHAR_MIN__
        #define WCHAR_MIN __WCHAR_MIN__
    #elif (L'\0' - 1) > 0
        #define WCHAR_MIN (L'\0' + 0)
    #else
        #define WCHAR_MIN (-WCHAR_MAX - 1)
    #endif
#endif

#define WINT_MIN (UINT32_C(0))
#define WINT_MAX UINT32_MAX

//
// Type widths (in bits).
//

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define INT8_WIDTH  8
    #define INT16_WIDTH 16
    #define INT32_WIDTH 32
    #define INT64_WIDTH 64

    #define UINT8_WIDTH  8
    #define UINT16_WIDTH 16
    #define UINT32_WIDTH 32
    #define UINT64_WIDTH 64

    #define INT_LEAST8_WIDTH  INT8_WIDTH
    #define INT_LEAST16_WIDTH INT16_WIDTH
    #define INT_LEAST32_WIDTH INT32_WIDTH
    #define INT_LEAST64_WIDTH INT64_WIDTH

    #define UINT_LEAST8_WIDTH  UINT8_WIDTH
    #define UINT_LEAST16_WIDTH UINT16_WIDTH
    #define UINT_LEAST32_WIDTH UINT32_WIDTH
    #define UINT_LEAST64_WIDTH UINT32_WIDTH

    #define INT_FAST8_WIDTH  INT8_WIDTH
    #define INT_FAST16_WIDTH __WORDSIZE
    #define INT_FAST32_WIDTH __WORDSIZE
    #define INT_FAST64_WIDTH INT64_WIDTH

    #define UINT_FAST8_WIDTH  UINT8_WIDTH
    #define UINT_FAST16_WIDTH __WORDSIZE
    #define UINT_FAST32_WIDTH __WORDSIZE
    #define UINT_FAST64_WIDTH UINT64_WIDTH

    #define INTMAX_WIDTH  INT64_WIDTH
    #define UINTMAX_WIDTH UINT64_WIDTH

    #define INTPTR_WIDTH  __WORDSIZE
    #define UINTPTR_WIDTH __WORDSIZE

    #define PTRDIFF_WIDTH    __WORDSIZE
    #define SIZE_WIDTH       __WORDSIZE
    #define SIG_ATOMIC_WIDTH INT32_WIDTH
    #define WINT_WIDTH       UINT32_WIDTH

    #if WCHAR_MAX == UINT16_MAX
        #define WCHAR_WIDTH UINT16_WIDTH
    #elif WCHAR_MAX == INT16_MAX
        #define WCHAR_WIDTH INT16_WIDTH
    #elif WCHAR_MAX == UINT32_MAX
        #define WCHAR_WIDTH UINT32_WIDTH
    #else
        #define WCHAR_WIDTH INT32_WIDTH
    #endif
#endif

#endif // __STDINT_H
