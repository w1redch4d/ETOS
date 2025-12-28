/*++

Copyright (c) 2025, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    resource.c

Abstract:

    Resource Management services.

--*/

#include "bootlib.h"
#include "nt.h"

#define DEFAULT_LOCALE L"en-US"
#define MUI_IMAGE_FLAGS \
    (IMAGE_FLAGS_MAP_ONE_TO_ONE | \
     IMAGE_FLAGS_NO_RELOCATIONS | \
     IMAGE_FLAGS_BOOT_MUI)

// static PVOID ResMuiImageBase = NULL;
// static ULONG ResMuiImageSize = 0;
// static PIMAGE_RESOURCE_DIRECTORY ResRootDirectoryPrimary = NULL;
// static ULONG ResRootDirectoryPrimaryOffset = 0;
// static PVOID ResPeImageBasePrimary = NULL;
// static PVOID ResPeImageEndPrimary = NULL;
// static PWSTR ResLocale = NULL;


// static PIMAGE_RESOURCE_DIRECTORY ResRootDirectoryFallback = NULL;
// static ULONG ResRootDirectoryFallbackOffset = 0;
// static PVOID ResPeImageBaseFallback = NULL;
// static PVOID ResPeImageEndFallback = NULL;

// static PIMAGE_RESOURCE_DIRECTORY ResRootDirectory = NULL;
// static ULONG ResRootDirectoryOffset = 0;
// static PVOID ResPeImageBase = NULL;
// static PVOID ResPeImageEnd = NULL;


// static BOOLEAN ResLoadedFontFiles = FALSE;

NTSTATUS
BlpResourceInitialize (
    VOID
    )

/*++

Routine Description:

    Initializes the resource engine to allow reading of localizable resources
    from in memory.

Arguments:

    None.

Return Value:

    STATUS_SUCCESS always.

--*/

{

    //ULONG HorizontalPixels;
   // PVOID ImageBase;
   // ULONG ImageSize;
   // PIMAGE_SECTION_HEADER SectionHeader;
    NTSTATUS Status = STATUS_SUCCESS;

    // TODO IMPLEMENT
   
    return Status;
}