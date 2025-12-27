/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    ntimage.h

Abstract:

    Provides NT image file definitions.

--*/

#pragma once

#ifndef _NTIMAGE_H
#define _NTIMAGE_H

//
// Machine types.
//
#define IMAGE_FILE_MACHINE_UNKNOWN     0x0000
#define IMAGE_FILE_MACHINE_I386        0x014c
#define IMAGE_FILE_MACHINE_AMD64       0x8664

//
// Subsystem types.
//
#define IMAGE_SUBSYSTEM_UNKNOWN                  0
#define IMAGE_SUBSYSTEM_NATIVE                   1
#define IMAGE_SUBSYSTEM_WINDOWS_GUI              2
#define IMAGE_SUBSYSTEM_WINDOWS_CUI              3
#define IMAGE_SUBSYSTEM_WINDOWS_CE_OLD           4
#define IMAGE_SUBSYSTEM_OS2_CUI                  5
#define IMAGE_SUBSYSTEM_POSIX_CUI                7
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS           8
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI           9
#define IMAGE_SUBSYSTEM_EFI_APPLICATION          10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER       12
#define IMAGE_SUBSYSTEM_EFI_ROM                  13
#define IMAGE_SUBSYSTEM_XBOX                     14
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16
#define IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG        17

//
// File characteristics.
//
#define IMAGE_FILE_RELOCS_STRIPPED         0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE        0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED      0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED     0x0008
#define IMAGE_FILE_AGGRESIVE_WS_TRIM       0x0010
#define IMAGE_FILE_LARGE_ADDRESS_AWARE     0x0020
#define IMAGE_FILE_BYTES_REVERSED_LO       0x0080
#define IMAGE_FILE_32BIT_MACHINE           0x0100
#define IMAGE_FILE_DEBUG_STRIPPED          0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define IMAGE_FILE_NET_RUN_FROM_SWAP       0x0800
#define IMAGE_FILE_SYSTEM                  0x1000
#define IMAGE_FILE_DLL                     0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY          0x4000
#define IMAGE_FILE_BYTES_REVERSED_HI       0x8000

//
// DLL characteristics.
//
#define IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA       0x0020
#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE          0x0040
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY       0x0080
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT             0x0100
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION          0x0200
#define IMAGE_DLLCHARACTERISTICS_NO_SEH                0x0400
#define IMAGE_DLLCHARACTERISTICS_NO_BIND               0x0800
#define IMAGE_DLLCHARACTERISTICS_APPCONTAINER          0x1000
#define IMAGE_DLLCHARACTERISTICS_X86_THUNK             0x1000
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER            0x2000
#define IMAGE_DLLCHARACTERISTICS_GUARD_CF              0x4000
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE 0x8000

//
// Data directory entry indices.
//
#define IMAGE_DIRECTORY_ENTRY_EXPORT         0
#define IMAGE_DIRECTORY_ENTRY_IMPORT         1
#define IMAGE_DIRECTORY_ENTRY_RESOURCE       2
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION      3
#define IMAGE_DIRECTORY_ENTRY_SECURITY       4
#define IMAGE_DIRECTORY_ENTRY_BASERELOC      5
#define IMAGE_DIRECTORY_ENTRY_DEBUG          6
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE   7
#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT      7
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR      8
#define IMAGE_DIRECTORY_ENTRY_TLS            9
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11
#define IMAGE_DIRECTORY_ENTRY_IAT            12
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14

//
// Legacy DOS "MZ" header.
//
typedef struct {
    USHORT e_magic;
    USHORT e_cblp;
    USHORT e_cp;
    USHORT e_crlc;
    USHORT e_cparhdr;
    USHORT e_minalloc;
    USHORT e_maxalloc;
    USHORT e_ss;
    USHORT e_sp;
    USHORT e_csum;
    USHORT e_ip;
    USHORT e_cs;
    USHORT e_lfarlc;
    USHORT e_ovno;
    USHORT e_res[4];
    USHORT e_oemid;
    USHORT e_oeminfo;
    USHORT e_res2[10];
    LONG   e_lfanew;
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

//
// New COFF header.
//
typedef struct {
    USHORT Machine;
    USHORT NumberOfSections;
    ULONG  TimeDateStamp;
    ULONG  PointerToSymbolTable;
    ULONG  NumberOfSymbols;
    USHORT SizeOfOptionalHeader;
    USHORT Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

//
// Data directory.
//

typedef struct {
    ULONG VirtualAddress;
    ULONG Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES 16

//
// Optional header (32-bit).
//
typedef struct {
    //
    // Standard fields.
    //
    USHORT Magic;
    UCHAR  MajorLinkerVersion;
    UCHAR  MinorLinkerVersion;
    ULONG  SizeOfCode;
    ULONG  SizeOfInitializedData;
    ULONG  SizeOfUninitializedData;
    ULONG  AddressOfEntryPoint;
    ULONG  BaseOfCode;
    ULONG  BaseOfData;

    //
    // NT-specific fields.
    //
    ULONG  ImageBase;
    ULONG  SectionAlignment;
    ULONG  FileAlignment;
    USHORT MajorOperatingSystemVersion;
    USHORT MinorOperatingSystemVersion;
    USHORT MajorImageVersion;
    USHORT MinorImageVersion;
    USHORT MajorSubsystemVersion;
    USHORT MinorSubsystemVersion;
    ULONG  Win32VersionValue;
    ULONG  SizeOfImage;
    ULONG  SizeOfHeaders;
    ULONG  CheckSum;
    USHORT Subsystem;
    USHORT DllCharacteristics;
    ULONG  SizeOfStackReserve;
    ULONG  SizeOfStackCommit;
    ULONG  SizeOfHeapReserve;
    ULONG  SizeOfHeapCommit;
    ULONG  LoaderFlags;
    ULONG  NumberOfRvaAndSizes;

    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

//
// Optional header (64-bit).
//
typedef struct {
    //
    // Standard fields.
    //
    USHORT Magic;
    UCHAR  MajorLinkerVersion;
    UCHAR  MinorLinkerVersion;
    ULONG  SizeOfCode;
    ULONG  SizeOfInitializedData;
    ULONG  SizeOfUninitializedData;
    ULONG  AddressOfEntryPoint;
    ULONG  BaseOfCode;

    //
    // NT-specific fields.
    //
    ULONGLONG ImageBase;
    ULONG     SectionAlignment;
    ULONG     FileAlignment;
    USHORT    MajorOperatingSystemVersion;
    USHORT    MinorOperatingSystemVersion;
    USHORT    MajorImageVersion;
    USHORT    MinorImageVersion;
    USHORT    MajorSubsystemVersion;
    USHORT    MinorSubsystemVersion;
    ULONG     Win32VersionValue;
    ULONG     SizeOfImage;
    ULONG     SizeOfHeaders;
    ULONG     CheckSum;
    USHORT    Subsystem;
    USHORT    DllCharacteristics;
    ULONGLONG SizeOfStackReserve;
    ULONGLONG SizeOfStackCommit;
    ULONGLONG SizeOfHeapReserve;
    ULONGLONG SizeOfHeapCommit;
    ULONG     LoaderFlags;
    ULONG     NumberOfRvaAndSizes;

    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

#endif /* !_NTIMAGE_H */
