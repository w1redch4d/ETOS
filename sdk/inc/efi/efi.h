/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    efi.h

Abstract:

    Provides EFI header files.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef _EFI_H
#define _EFI_H

#if defined(_M_X64) || defined(__x86_64__)
#include "x64/efibind.h"
#elif defined(_M_IX86) || defined(__i386__)
#include "i386/efibind.h"
#else
#error Unsupported processor architecture
#endif

#include "efidef.h"
#include "efidevp.h"
#include "efiprot.h"
#include "eficon.h"
#include "efiapi.h"
#include "efierr.h"

#endif // _EFI_H
