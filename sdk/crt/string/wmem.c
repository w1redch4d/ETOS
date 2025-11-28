/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wmem.c

Abstract:

    Wide-character memory services.

--*/

#include <wchar.h>

wchar_t *
wmemset (
    wchar_t *wcs,
    wchar_t wc,
    size_t  n
    )

{
    wchar_t *ptr;

    ptr = wcs;
    while (n--) {
        *wcs++ = wc;
    }

    return ptr;
}

wchar_t *
wmemcpy (
    wchar_t       *dest,
    const wchar_t *src,
    size_t        n
    )

{
    wchar_t *ptr;

    ptr = dest;
    while (n--) {
        *dest++ = *src++;
    }

    return ptr;
}

wchar_t *
wmemmove (
    wchar_t       *dest,
    const wchar_t *src,
    size_t        n
    )

{
    wchar_t *ptr;

    ptr = dest;

    /* Check for overlap */
    if (src >= dest || (src + n) <= dest) {
        /* Low-to-high copy */
        while (n--) {
            *dest++ = *src++;
        }
    } else {
        /* High-to-low copy */
        dest += n;
        src += n;
        while (n--) {
            *--dest = *--src;
        }
    }

    return ptr;
}

int
wmemcmp (
    const wchar_t *s1,
    const wchar_t *s2,
    size_t        n
    )

{
    while (n--) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        s1++;
        s2++;
    }

    return 0;
}
