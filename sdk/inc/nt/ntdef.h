/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    ntdef.h

Abstract:

    Provides basic NT definitions.

--*/

#pragma once

#ifndef _NTDEF_H
#define _NTDEF_H

//
// Parameter decorators.
//

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifndef OPTIONAL
    #define OPTIONAL
#endif

#ifndef CRITICAL
    #define CRITICAL
#endif

//
// Keyword aliases.
//

#ifndef VOID
    #define VOID void
#endif

#ifndef CONST
    #define CONST const
#endif

//
// Kludge for some compilers.
//
#ifndef ANYSIZE_ARRAY
    #define ANYSIZE_ARRAY 1
#endif

//
// Force inlining routine.
//
#ifndef FORCEINLINE
    #if defined(_MSC_EXTENSIONS)
        #if _MSC_VER >= 1200
            #define FORCEINLINE __forceinline
        #else
            #define FORCEINLINE __inline
        #endif
    #elif defined(__clang__) || defined(__GNUC__)
        #define FORCEINLINE __attribute__((always_inline))
    #else
        #define FORCEINLINE static inline
    #endif
#endif

//
// NT calling convention.
//
#ifndef NTAPI
    #if defined(_MSC_EXTENSIONS)
        #define NTAPI __stdcall
    #elif defined(__clang__) || defined(__GNUC__)
        #define NTAPI __attribute__((stdcall))
    #else
        #warning Unable to define NTAPI
        #define NTAPI
    #endif
#endif

//
// Static assertion helper.
//
#ifndef STATIC_ASSERT
    #if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || (defined(__cplusplus) && __cplusplus >= 201103L)
        #define STATIC_ASSERT static_assert
    #elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        #define STATIC_ASSERT _Static_assert
    #else
        #warning Static assertion is not available; compilation problems may not be detected
        #define STATIC_ASSERT(Expr, Msg)
    #endif
#endif /* !STATIC_ASSERT */

//
// Basic types.
//

typedef char           CHAR;
typedef short          SHORT;
typedef long           LONG;
typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
typedef unsigned long  ULONG;

#define MINCHAR   0x80
#define MAXCHAR   0x7f
#define MINSHORT  0x8000
#define MAXSHORT  0x7fff
#define MINLONG   0x80000000
#define MAXLONG   0x7fffffff
#define MAXUCHAR  0xff
#define MAXUSHORT 0xffff
#define MAXULONG  0xffffffff

//
// 64-bit types.
//

#if defined(_MSC_EXTENSIONS)
typedef __int64                LONGLONG;
typedef unsigned __int64       ULONGLONG;
#else
typedef long long          LONGLONG;
typedef unsigned long long ULONGLONG;
#endif

#define MAXLONGLONG  0x7fffffffffffffff
#define MAXULONGLONG 0xffffffffffffffff

//
// Logical/boolean types.
//
typedef ULONG LOGICAL;
#ifndef TRUE
typedef UCHAR BOOLEAN;
#define TRUE  ((BOOLEAN) 1)
#define FALSE ((BOOLEAN) 0)
#endif

//
// Basic pointer types.
//
typedef VOID      *PVOID;
typedef CHAR      *PCHAR;
typedef SHORT     *PSHORT;
typedef LONG      *PLONG;
typedef LONGLONG  *PLONGLONG;
typedef UCHAR     *PUCHAR;
typedef USHORT    *PUSHORT;
typedef ULONG     *PULONG;
typedef ULONGLONG *PULONGLONG;
typedef LOGICAL   *PLOGICAL;
typedef BOOLEAN   *PBOOLEAN;

//
// Character/string types.
//
typedef CHAR *PCH, *LPCH, *PSTR, *LPSTR;
typedef CONST CHAR *PCCH, *LPCCH, *PCSTR, *LPCSTR;

//
// Wide character/string types.
//
typedef USHORT WCHAR;
typedef WCHAR *PWCHAR, *PWCH, *LPWCH, *PWSTR, *LPWSTR;
typedef CONST WCHAR *PCWCHAR, *LPCWCHAR, *PCWSTR, *LPCWSTR;

//
// Numeric pointer types.
//

#if defined(_WIN64)
    typedef LONGLONG  LONG_PTR;
    typedef ULONGLONG ULONG_PTR;
#else
    typedef LONG      LONG_PTR;
    typedef ULONG     ULONG_PTR;
#endif

STATIC_ASSERT(sizeof(LONG_PTR)  == sizeof(PVOID), "LONG_PTR must be the same size as PVOID");
STATIC_ASSERT(sizeof(ULONG_PTR) == sizeof(PVOID), "ULONG_PTR must be the same size as PVOID");

//
// Handle types.
//

typedef PVOID  HANDLE;
typedef HANDLE *PHANDLE;

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR) -1)

//
// Status code types.
//

typedef LONG     NTSTATUS;
typedef NTSTATUS *PNTSTATUS;

#define NT_SUCCESS(Status)     ((NTSTATUS)(Status) >= 0)
#define NT_INFORMATION(Status) ((ULONG)(Status) >> 30 == 1)
#define NT_WARNING(Status)     ((ULONG)(Status) >> 30 == 2)
#define NT_ERROR(Status)       ((ULONG)(Status) >> 30 == 3)

#define APPLICATION_ERROR_MASK       0x20000000
#define ERROR_SEVERITY_SUCCESS       0x00000000
#define ERROR_SEVERITY_INFORMATIONAL 0x40000000
#define ERROR_SEVERITY_WARNING       0x80000000
#define ERROR_SEVERITY_ERROR         0xc0000000

//
// Bit extraction helpers.
//
#define LODWORD(x) ((ULONG)(x))
#define HIDWORD(x) ((ULONG)((x) >> 32))
#define LOWORD(x)  ((USHORT)(x))
#define HIWORD(x)  ((USHORT)((x) >> 16))
#define LOBYTE(x)  ((UCHAR)(x))
#define HIBYTE(x)  ((UCHAR)((x) >> 8))

//
// Large (64-bit) signed integer value.
//
typedef union LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG  HighPart;
    };

    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

//
// Large (64-bit) unsigned integer value.
//
typedef union ULARGE_INTEGER {
    struct {
        ULONG LowPart;
        ULONG HighPart;
    };

    ULONGLONG QuadPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;

//
// Address types.
//
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;
typedef LARGE_INTEGER VIRTUAL_ADDRESS,  *PVIRTUAL_ADDRESS;

//
// Doubly-linked list head/entry.
//
typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

//
// Singly-linked list head/entry.
//
typedef struct _SINGLE_LIST_ENTRY {
    struct _SINGLE_LIST_ENTRY *Next;
} SINGLE_LIST_ENTRY, *PSINGLE_LIST_ENTRY;

//
// ANSI string.
//

typedef struct {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} STRING, *PSTRING;

typedef STRING ANSI_STRING, *PANSI_STRING;
typedef CONST ANSI_STRING *PCANSI_STRING;

#define ANSI_NULL ((CHAR) 0)

#define ANSI_STRING_MAX_BYTES ((USHORT) 65535)
#define ANSI_STRING_MAX_CHARS (65535)

//
// Unicode string.
//

typedef struct {
    USHORT Length;
    USHORT MaximumLength;
    PWCH   Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef CONST UNICODE_STRING *PCUNICODE_STRING;

#define UNICODE_NULL ((WCHAR) 0)

#define UNICODE_STRING_MAX_BYTES ((USHORT) 65534)
#define UNICODE_STRING_MAX_CHARS (32767)

//
// Argument validation helper.
//
#define ARGUMENT_PRESENT(Argument) ((PVOID)(ULONG_PTR)(Argument) != NULL)

//
// Struct helpers.
//
#define FIELD_OFFSET(Type, Field) ((LONG)(LONG_PTR)&(((Type *) 0)->Field))
#define CONTAINING_RECORD(Address, Type, Field) ((Type *)((ULONG_PTR)(Address) - (ULONG_PTR)(&((Type *) 0)->Field)))

//
// Minimum/maximum helpers.
//
#if !defined(NOMINMAX)
    #ifndef min
        #define min(a, b) (((a) < (b)) ? (a) : (b))
    #endif
    #ifndef max
        #define max(a, b) (((a) > (b)) ? (a) : (b))
    #endif
#endif

#include <guiddef.h>

#endif /* !_NTDEF_H */
