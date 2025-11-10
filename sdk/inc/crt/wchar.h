/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wchar.h

Abstract:

    Provides wide string definitions.

--*/

#pragma once

#ifndef _WCHAR_H
#define _WCHAR_H 1

#include <stdarg.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

wchar_t *wmemset(wchar_t *dest, wchar_t c, size_t count);
wchar_t *wmemcpy(wchar_t *dest, const wchar_t *src, size_t count);
wchar_t *wmemmove(wchar_t *dest, const wchar_t *src, size_t count);

size_t wcslen(const wchar_t *str);
size_t wcsnlen(const wchar_t *str, size_t maxlen);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcschr(const wchar_t *wcs, wchar_t wc);
wchar_t *wcsstr(const wchar_t *haystack, const wchar_t *needle);
wchar_t *wcscpy_s(wchar_t *dest, size_t maxlen, const wchar_t *src);
wchar_t *wcscat_s(wchar_t *dest, size_t maxlen, const wchar_t *src);

int vswprintf_s(wchar_t *wcs, size_t maxlen, const wchar_t *format, va_list args);

#if defined(__cplusplus)
}
#endif

#endif /* !_WCHAR_H */
