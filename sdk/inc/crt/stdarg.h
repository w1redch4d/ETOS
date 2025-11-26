/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stdarg.h

Abstract:

    Variable argument list definitions.

--*/

#if !defined(__STDARG_H)      || __has_feature(modules)    || \
    defined(__need___va_list) || defined(__need_va_list)   || \
    defined(__need_va_arg)    || defined(__need___va_copy) || \
    defined(__need_va_copy)

//
// Enable default definitions if none were requested.
//
#if !defined(__need___va_list) && !defined(__need_va_list)   && \
    !defined(__need_va_arg)    && !defined(__need___va_copy) && \
    !defined(__need_va_copy)

    #define __STDARG_H
    #define __need___va_list
    #define __need_va_list
    #define __need_va_arg
    #define __need___va_copy

    #if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
        (defined(__cplusplus)      && __cplusplus      >= 201103L) || \
        !defined(__STRICT_ANSI__)
        #define __need_va_copy
    #endif

#endif

//
// Define requested types and macros.
//

#ifdef __need___va_list
    #ifndef __GNUC_VA_LIST
        #define __GNUC_VA_LIST
        typedef __builtin_va_list __gnuc_va_list;
    #endif
    #undef __need___va_list
#endif

#ifdef __need_va_list
    #ifndef _VA_LIST
        #define _VA_LIST
        typedef __builtin_va_list va_list;
    #endif
    #undef __need_va_list
#endif

#ifdef __need_va_arg
    #ifndef va_arg
        #define va_arg(ap, type) __builtin_va_arg(ap, type)
        #define va_end(ap)       __builtin_va_end(ap)
        #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
            #define va_start(...)       __builtin_c23_va_start(__VA_ARGS__)
        #else
            #define va_start(ap, param) __builtin_va_start(ap, param)
        #endif
    #endif
    #undef __need_va_arg
#endif

#ifdef __need___va_copy
    #ifndef __va_copy
        #define __va_copy(dest, src) __builtin_va_copy(dest, src)
    #endif
    #undef __need___va_copy
#endif

#ifdef __need_va_copy
    #ifndef va_copy
        #define va_copy(dest, src) __builtin_va_copy(dest, src)
    #endif
    #undef __need_va_copy
#endif

#endif
