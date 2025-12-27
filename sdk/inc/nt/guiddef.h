/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
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
} GUID, *PGUID;

FORCEINLINE
BOOLEAN
IsEqualGUID (
    PGUID Guid1,
    PGUID Guid2
    )

{
    return (*((PULONGLONG)Guid1) == *((PULONGLONG)Guid2)) && (*((PULONGLONG)Guid1 + 1) == *((PULONGLONG)Guid2 + 1));
}

#endif /* !_GUIDDEF_H */
