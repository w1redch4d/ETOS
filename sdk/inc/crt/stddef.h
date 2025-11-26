/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stddef.h

Abstract:

    Standard type and macro definitions.

--*/

#if !defined(__STDDEF_H)        || __has_feature(modules)      || \
    defined(__need_ptrdiff_t)   || defined(__need_size_t)      || \
    defined(__need_rsize_t)     || defined(__need_wchar_t)     || \
    defined(__need_nullptr_t)   || defined(__need_max_align_t) || \
    defined(__need_wint_t)      || defined(__need_NULL)        || \
    defined(__need_unreachable) || defined(__need_offsetof)    || \
    (defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ > 0)

//
// Enable default definitions if none were requested.
//
#if !defined(__need_ptrdiff_t)   && !defined(__need_size_t)      && \
    !defined(__need_rsize_t)     && !defined(__need_wchar_t)     && \
    !defined(__need_nullptr_t)   && !defined(__need_max_align_t) && \
    !defined(__need_wint_t)      && !defined(__need_NULL)        && \
    !defined(__need_unreachable) && !defined(__need_offsetof)

    #define __STDDEF_H
    #define __need_ptrdiff_t
    #define __need_size_t
    #define __need_wchar_t
    #define __need_NULL
    #define __need_offsetof

    #if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ > 0
        #define __need_rsize_t
    #endif

    #if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) || defined(__cplusplus)
        #define __need_nullptr_t
    #endif

    #if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) || (defined(__cplusplus) && __cplusplus >= 201103L)
        #define __need_max_align_t
    #endif

    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
        #define __need_unreachable
    #endif

#endif

//
// Define requested types and macros.
//

#ifdef __need_ptrdiff_t
    #if !defined(_PTRDIFF_T) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define _PTRDIFF_T
        typedef __PTRDIFF_TYPE__ ptrdiff_t;
    #endif
    #undef __need_ptrdiff_t
#endif

#ifdef __need_size_t
    #if !defined(_SIZE_T) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define _SIZE_T
        typedef __SIZE_TYPE__ size_t;
    #endif
    #undef __need_size_t
#endif

#ifdef __need_rsize_t
    #if !defined(_RSIZE_T) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define _RSIZE_T
        typedef __SIZE_TYPE__ rsize_t;
    #endif
    #undef __need_rsize_t
#endif

#ifdef __need_wchar_t
    #if (!defined(__cplusplus) || (defined(_MSC_VER) && !_NATIVE_WCHAR_T_DEFINED)) && \
        (!defined(_WCHAR_T) || (__has_feature(modules) && !__building_module(_Builtin_stddef)))
        #define _WCHAR_T
        typedef __WCHAR_TYPE__ wchar_t;
        #ifdef _MSC_EXTENSIONS
            #define _WCHAR_T_DEFINED
        #endif
    #endif
    #undef __need_wchar_t
#endif

#ifdef __need_nullptr_t
    #if !defined(_NULLPTR_T) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define _NULLPTR_T
        #ifdef __cplusplus
            #if defined(_MSC_EXTENSIONS) && defined(_NATIVE_NULLPTR_SUPPORTED)
                namespace std {
                    typedef decltype(nullptr) nullptr_t;
                }
                using ::std::nullptr_t;
            #endif
        #elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
            typedef typeof(nullptr) nullptr_t;
        #endif
    #endif
    #undef __need_nullptr_t
#endif

#ifdef __need_max_align_t
    #ifndef __LIBC_MAX_ALIGN_T_DEFINED
        #define __LIBC_MAX_ALIGN_T_DEFINED
        #if defined(_MSC_VER)
            typedef double max_align_t;
        #elif defined(__APPLE__)
            typedef long double max_align_t;
        #else
            typedef struct {
                long long   __libc_max_align_ll __attribute__((__aligned__(__alignof__(long long))));
                long double __libc_max_align_ld __attribute__((__aligned__(__alignof__(long double))));
            } max_align_t;
        #endif
    #endif
    #undef __need_max_align_t
#endif

#ifdef __need_wint_t
    #ifndef _WINT_T
        #define _WINT_T
        #ifndef __WINT_TYPE__
            #define __WINT_TYPE__ unsigned int
        #endif
        typedef __WINT_TYPE__ wint_t;
    #endif
    #undef __need_wint_t
#endif

#ifdef __need_NULL
    #if !defined(NULL) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #undef NULL
        #ifdef __cplusplus
            #if !defined(__MINGW32__) && !defined(_MSC_VER)
                #define NULL __null
            #else
                #define NULL 0
            #endif
        #else
            #define NULL ((void *) 0)
        #endif
    #endif
    #undef __need_NULL
#endif

#ifdef __need_unreachable
    #if !defined(unreachable) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define unreachable() __builtin_unreachable()
    #endif
    #undef __need_unreachable
#endif

#ifdef __need_offsetof
    #if !defined(offsetof) || (__has_feature(modules) && !__building_module(_Builtin_stddef))
        #define offsetof(type, member) __builtin_offsetof(type, member)
    #endif
    #undef __need_offsetof
#endif

#endif
