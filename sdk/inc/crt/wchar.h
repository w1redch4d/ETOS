/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wchar.h

Abstract:

    Wide-character string service definitions.

--*/

#if !defined(_MSC_VER) || _MSC_VER > 1000
#pragma once
#endif

#ifndef __WCHAR_H
#define __WCHAR_H

#ifdef __cplusplus
extern "C" {
#endif

//
// Extensions enabled by default in Windows environments.
//
#if defined(_WIN32) && !defined(__STDC_WANT_LIB_EXT1__)
    #define __STDC_WANT_LIB_EXT1__ 1
#endif

#define __need_size_t
#define __need_wchar_t
#define __need_wint_t
#define __need_NULL
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
    #define __need_rsize_t
#endif
#include <stddef.h>

#define __need_va_list
#include <stdarg.h>

#ifndef WEOF
    #define WEOF (0xffffffffu)
#endif

//
// Memory services.
//
wchar_t *wmemset(wchar_t *wcs, wchar_t wc, size_t n);
wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t n);
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t n);
int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n);

//
// String services.
//
size_t wcslen(const wchar_t *s);
size_t wcsnlen(const wchar_t *s, size_t maxlen);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc);
wchar_t *wcsrchr(const wchar_t *wcs, wchar_t wc);
wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle);

//
// Extensions.
//
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
typedef int errno_t;

size_t wcsnlen_s(const wchar_t *str, size_t strsz);
errno_t wcscpy_s(wchar_t *dest, rsize_t destsz, const wchar_t *src);
errno_t wcscat_s(wchar_t *dest, rsize_t destsz, const wchar_t *src);

int vswprintf_s(wchar_t *buf, size_t bufsz, const wchar_t *format, va_list args);
#endif

#ifdef __cplusplus
}
#endif

#endif // __WCHAR_H
