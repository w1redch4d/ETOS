/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wstr.c

Abstract:

    Wide-character string services.

--*/

#define __STDC_WANT_LIB_EXT1__ 1
#include <errno.h>
#include <stdint.h>
#include <wchar.h>

size_t
wcslen (
    const wchar_t *s
    )

{
    const wchar_t *ptr;

    ptr = s;
    while (*ptr != L'\0') {
        ptr++;
    }

    return ptr - s;
}

size_t
wcsnlen (
    const wchar_t *s,
    size_t        maxlen
    )

{
    const wchar_t *ptr;

    ptr = s;
    while (maxlen-- && *ptr != L'\0') {
        ptr++;
    }

    return ptr - s;
}

int
wcscmp (
    const wchar_t *s1,
    const wchar_t *s2
    )

{
    while (*s1 == *s2) {
        if (*s1 == L'\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int
wcsncmp (
    const wchar_t *s1,
    const wchar_t *s2,
    size_t        n
    )

{
    while (n--) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        if (*s1 == L'\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return 0;
}

wchar_t *
wcschr (
    const wchar_t *wcs,
    wchar_t       wc
    )

{
    while (*wcs != wc) {
        if (*wcs == L'\0') {
            return NULL;
        }

        wcs++;
    }

    return (wchar_t *)wcs;
}

wchar_t *
wcsrchr (
    const wchar_t *wcs,
    wchar_t       wc
    )

{
    const wchar_t *last;

    last = NULL;
    do {
        if (*wcs == wc) {
            last = wcs;
        }
    } while (*wcs++ != L'\0');

    return (wchar_t *)last;
}

wchar_t *
wcsstr (
    const wchar_t *haystack,
    const wchar_t *needle
    )

{
    const wchar_t *pos;

    if (*needle == L'\0') {
        return (wchar_t *)haystack;
    }

    pos = haystack;
    while ((pos = wcschr(pos, *needle)) != NULL) {
        if (wcscmp(pos, needle) == 0) {
            return (wchar_t *)pos;
        }
    }

    return NULL;
}

size_t
wcsnlen_s (
    const wchar_t *str,
    size_t        strsz
    )

{
    const wchar_t *ptr;
    size_t maxlen;

    if (str == NULL) {
        return 0;
    }

    ptr = str;
    maxlen = strsz;
    while (strsz--) {
        if (*ptr == '\0') {
            return ptr - str;
        }

        ptr++;
    }

    return maxlen;
}

errno_t
wcscpy_s (
    wchar_t       *dest,
    rsize_t       destsz,
    const wchar_t *src
    )

{
    size_t srcsz;

    //
    // TODO: Report security failures.
    //

    if (dest == NULL) {
        return EINVAL;
    }

    if (destsz == 0 || destsz > (RSIZE_MAX / sizeof(wchar_t))) {
        return ERANGE;
    }

    if (src == NULL) {
        dest[0] = L'\0';
        return EINVAL;
    }

    srcsz = wcsnlen_s(src, destsz) + 1;
    if (destsz < srcsz) {
        dest[0] = L'\0';
        return ERANGE;
    }

    while (srcsz--) {
        *dest++ = *src++;
    }

    return 0;
}

errno_t
wcscat_s (
    wchar_t       *dest,
    rsize_t       destsz,
    const wchar_t *src
    )

{
    size_t destlen, srcsz;

    //
    // TODO: Report security failures.
    //

    if (dest == NULL) {
        return EINVAL;
    }

    if (destsz == 0 || destsz > (RSIZE_MAX / sizeof(wchar_t))) {
        return ERANGE;
    }

    destlen = wcsnlen_s(dest, destsz);
    if (destlen == destsz) {
        return EINVAL;
    }

    if (src == NULL) {
        dest[0] = L'\0';
        return EINVAL;
    }

    srcsz = wcsnlen_s(src, destsz) + 1;
    if (destsz < (destlen + srcsz)) {
        dest[0] = L'\0';
        return ERANGE;
    }

    dest += destlen;
    while (srcsz--) {
        *dest++ = *src++;
    }

    return 0;
}
