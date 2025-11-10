/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stddef.h

Abstract:

    Provides standard types and definitions.

--*/

#pragma once

#ifndef _STDDEF_H
#define _STDDEF_H 1

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef NULL
    #if defined(__cplusplus)
        #if __cplusplus >= 201103L
            #define NULL nullptr
        #else
            #define NULL 0
        #endif
    #else
        #define NULL ((void *) 0)
    #endif
#endif

#ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED
    #if defined(_WIN64)
        typedef unsigned __int64 size_t;
    #else
        typedef unsigned int size_t;
    #endif
#endif

#ifndef _PTRDIFF_T_DEFINED
    #define _PTRDIFF_T_DEFINED
    #if defined(_WIN64)
        typedef __int64 ptrdiff_t;
    #else
        typedef long int ptrdiff_t;
    #endif
#endif

#ifndef _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED
    typedef unsigned short wchar_t;
#endif

#ifndef offsetof
    #define offsetof(type, member) __builtin_offsetof(type, member)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* !_STDDEF_H */
