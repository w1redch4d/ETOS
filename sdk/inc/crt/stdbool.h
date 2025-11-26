/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stdbool.h

Abstract:

    Boolean type definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef __STDBOOL_H
#define __STDBOOL_H

#define __bool_true_false_are_defined 1

#ifdef __cplusplus
    //
    // Define GNU extensions.
    //
    #if defined(__GNUC__) && !defined(__STRICT_ANSI__)
        #define _Bool bool
        #if __cplusplus < 201103L
            #define bool  bool
            #define true  true
            #define false false
        #endif
    #endif
#elif !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
    #define bool  char
    #define true  1
    #define false 0
#elif __STDC_VERSION__ < 202311L
    #define bool  _Bool
    #define true  1
    #define false 0
#endif

#endif // __STDBOOL_H
