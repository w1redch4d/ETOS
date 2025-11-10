/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    guiddef.h

Abstract:

    Provides GUID (Globally Unique IDentifier) definitions.

--*/

#pragma once

#ifndef _GUIDDEF_H
#define _GUIDDEF_H

typedef struct {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#endif /* !_GUIDDEF_H */
