/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efidef.h

Abstract:

    Provides basic EFI definitions.

--*/

#pragma once

#ifndef _EFIDEF_H
#define _EFIDEF_H

//
// Parameter decorators.
//
#define IN
#define OUT
#define OPTIONAL

//
// Keyword aliases.
//
#define CONST const
#define VOID  void

//
// NULL pointer constant.
//
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || (defined(__cplusplus) && __cplusplus >= 201103L)
    #define NULL nullptr
#elif defined(__cplusplus)
    #define NULL 0
#else
    #define NULL ((VOID *) 0)
#endif

//
// Boolean values.
//
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || defined(__cplusplus)
    #define TRUE  true
    #define FALSE false
#else
    #define TRUE  ((BOOLEAN) 1)
    #define FALSE ((BOOLEAN) 0)
#endif

//
// Forces the compiler to inline the routine.
//
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

//
// Static assertion helper.
//
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || (defined(__cplusplus) && __cplusplus >= 202203L)
    #define STATIC_ASSERT static_assert
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define STATIC_ASSERT _Static_assert
#else
    #warning Static assertion not available; compilation problems may not be detected!
    #define STATIC_ASSERT(Expr, Msg)
#endif

//
// Boolean type.
//
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || defined(__cplusplus)
    typedef bool  BOOLEAN;
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    typedef _Bool BOOLEAN;
#else
    typedef UINT8 BOOLEAN;
#endif

//
// Character types.
//
typedef char   CHAR8;
typedef UINT16 CHAR16;

//
// Miscellaneous numeric/pointer types.
//
typedef UINTN  EFI_STATUS;
typedef VOID   *EFI_HANDLE;
typedef VOID   *EFI_EVENT;
typedef UINTN  EFI_TPL;
typedef UINT64 EFI_LBA;

//
// GUID (Globally Unique IDentifier).
//
typedef struct {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];
} EFI_GUID;

//
// Date/time.
// Year:       1900 - 9999.
// Month:      1 - 12.
// Day:        1 - 31.
// Hour:       0 - 23.
// Minute:     0 - 59.
// Second:     0 - 59.
// Nanosecond: 0 - 999999999.
// Timezone:   -1440 - 1440, or 2047.
//
typedef struct {
    UINT16 Year;
    UINT8  Month;
    UINT8  Day;
    UINT8  Hour;
    UINT8  Minute;
    UINT8  Second;
    UINT8  Pad1;
    UINT32 Nanosecond;
    INT16  Timezone;
    UINT8  Daylight;
    UINT8  Pad2;
} EFI_TIME;

//
// For EFI_TIME.Daylight.
//
#define EFI_TIME_ADJUST_DAYLIGHT 0x01
#define EFI_TIME_IN_DAYLIGHT     0x02

//
// For EFI_TIME.TimeZone.
//
#define EFI_UNSPECIFIED_TIMEZONE 0x07ff

//
// Network addresses.
//

typedef struct {
    UINT8 Addr[4];
} EFI_IPv4_ADDRESS;

typedef struct {
    UINT8 Addr[16];
} EFI_IPv6_ADDRESS;

typedef union {
    UINT32           Addr[4];
    EFI_IPv4_ADDRESS v4;
    EFI_IPv6_ADDRESS v6;
} EFI_IP_ADDRESS;

typedef struct {
    UINT8 Addr[32];
} EFI_MAC_ADDRESS;

typedef struct {
    UINT8 Address[6];
} BLUETOOTH_ADDRESS;

typedef struct {
    UINT8 Address[6];
    UINT8 Type;
} BLUETOOTH_LE_ADDRESS;

//
// Memory addresses.
//
typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

//
// Memory allocation methods.
//
typedef enum {
    //
    // Allocate range at any address.
    //
    AllocateAnyPages,

    //
    // Allocate range ending at a specific address.
    //
    AllocateMaxAddress,

    //
    // Allocate range starting at a specific address.
    //
    AllocateAddress,

    //
    // For bounds checking.
    //
    MaxAllocateType,
} EFI_ALLOCATE_TYPE;

//
// Memory types.
//
typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType,

    MEMORY_TYPE_OEM_RESERVED_MIN = 0x70000000,
    MEMORY_TYPE_OEM_RESERVED_MAX = 0x7fffffff,
    MEMORY_TYPE_OS_RESERVED_MIN  = 0x80000000,
    MEMORY_TYPE_OS_RESERVED_MAX  = 0xffffffff,
} EFI_MEMORY_TYPE;

//
// Memory caching attributes.
//
#define EFI_MEMORY_UC            0x0000000000000001
#define EFI_MEMORY_WC            0x0000000000000002
#define EFI_MEMORY_WT            0x0000000000000004
#define EFI_MEMORY_WB            0x0000000000000008
#define EFI_MEMORY_UCE           0x0000000000000010

//
// Memory protection attributes.
//
#define EFI_MEMORY_WP            0x0000000000001000
#define EFI_MEMORY_RP            0x0000000000002000
#define EFI_MEMORY_XP            0x0000000000004000
#define EFI_MEMORY_RO            0x0000000000020000

//
// Miscellaneous memory attributes.
//
#define EFI_MEMORY_NV            0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_SP            0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO    0x0000000000080000
#define EFI_MEMORY_HOT_PLUGGABLE 0x0000000000100000
#define EFI_MEMORY_ISA_VALID     0x4000000000000000
#define EFI_MEMORY_RUNTIME       0x8000000000000000

//
// Memory type/attribute masks.
//
#define EFI_MEMORY_ISA_MASK       0x0ffff00000000000
#define EFI_CACHE_ATTRIBUTE_MASK  (EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB | EFI_MEMORY_UCE | EFI_MEMORY_WP)
#define EFI_MEMORY_ACCESS_MASK    (EFI_MEMORY_RP | EFI_MEMORY_XP | EFI_MEMORY_RO)
#define EFI_MEMORY_ATTRIBUTE_MASK (EFI_MEMORY_ACCESS_MASK | EFI_MEMORY_SP | EFI_MEMORY_CPU_CRYPTO)

//
// Memory descriptor.
//

#define EFI_MEMORY_DESCRIPTOR_VERSION 1

typedef struct {
    //
    // Memory type (EFI_MEMORY_TYPE) of the region.
    //
    UINT32               Type;

    UINT32               Pad;

    //
    // Must be aligned on an EFI_PAGE_SIZE-byte boundary.
    // Must not be above 0xfffffffffffff000.
    //
    EFI_PHYSICAL_ADDRESS PhysicalStart;
    EFI_VIRTUAL_ADDRESS  VirtualStart;

    //
    // Number of EFI_PAGE_SIZE-byte pages.
    // Must not be zero.
    //
    UINT64               NumberOfPages;

    //
    // Capabilities of the memory region.
    //
    UINT64               Attribute;
} EFI_MEMORY_DESCRIPTOR;

//
// International language.
//
typedef CHAR8 ISO_639_2;
#define ISO_639_2_ENTRY_SIZE 3

//
// Page size constants and helpers.
//
#define EFI_PAGE_SIZE  4096
#define EFI_PAGE_MASK  0xfff
#define EFI_PAGE_SHIFT 12
#define EFI_SIZE_TO_PAGES(Size)  (((Size) >> EFI_PAGE_SHIFT) + (((Size) & EFI_PAGE_MASK) ? 1 : 0))
#define EFI_PAGES_TO_SIZE(Pages) ((Pages) << EFI_PAGE_SHIFT)

//
// Maximum values for fixed-width integer types.
//
#define MAX_INT8   ((INT8)   0x7f)
#define MAX_UINT8  ((UINT8)  0xff)
#define MAX_INT16  ((INT16)  0x7fff)
#define MAX_UINT16 ((UINT16) 0xffff)
#define MAX_INT32  ((INT32)  0x7fffffff)
#define MAX_UINT32 ((UINT32) 0xffffffff)
#define MAX_INT64  ((INT64)  0x7fffffffffffffffULL)
#define MAX_UINT64 ((UINT64) 0xffffffffffffffffULL)

//
// Minimum values for signed fixed-width integer types.
//
#define MIN_INT8   (((INT8)  -127) - 1)
#define MIN_INT16  (((INT16) -32767) - 1)
#define MIN_INT32  (((INT32) -2147483647) - 1)
#define MIN_INT64  (((INT64) -9223372036854775807LL) - 1)

//
// Check fixed-width integer type sizes.
//
STATIC_ASSERT(sizeof(UINT8)  == 1, "UINT8 must be 1 byte in size");
STATIC_ASSERT(sizeof(UINT16) == 2, "UINT16 must be 2 bytes in size");
STATIC_ASSERT(sizeof(UINT32) == 4, "UINT32 must be 4 bytes in size");
STATIC_ASSERT(sizeof(UINT64) == 8, "UINT64 must be 8 bytes in size");
STATIC_ASSERT(sizeof(INT8)   == 1, "INT8 must be 1 byte in size");
STATIC_ASSERT(sizeof(INT16)  == 2, "INT16 must be 2 bytes in size");
STATIC_ASSERT(sizeof(INT32)  == 4, "INT32 must be 4 bytes in size");
STATIC_ASSERT(sizeof(INT64)  == 8, "INT64 must be 8 bytes in size");

#endif /* !_EFIDEF_H */
