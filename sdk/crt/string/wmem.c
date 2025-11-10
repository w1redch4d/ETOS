/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wmem.c

Abstract:

    Wide memory manipulation routines.

--*/

#include <wchar.h>

wchar_t *
wmemset (
    wchar_t *dest,
    wchar_t c,
    size_t count
    )

{
    wchar_t *ptr = dest;

    while (count--) {
        *dest++ = c;
    }

    return ptr;
}

wchar_t *
wmemcpy (
    wchar_t *dest,
    const wchar_t *src,
    size_t count
    )

{
    wchar_t *ptr = dest;

    while (count--) {
        *dest++ = *src++;
    }

    return ptr;
}

wchar_t *
wmemmove (
    wchar_t *dest,
    const wchar_t *src,
    size_t count
    )

{
    wchar_t *ptr = dest;

    /* Check for overlap */
    if (src > dest || ((wchar_t *)src + count) < (wchar_t *)dest) {
        /* Low-to-high copy */
        return wmemcpy(dest, src, count);
    }

    /* High-to-low copy */
    dest = (wchar_t *)dest + count - 1;
    src = (wchar_t *)src + count - 1;
    while (count--) {
        *(wchar_t *)dest = *(wchar_t *)src;
        dest = (wchar_t *)dest - 1;
        src = (wchar_t *)src - 1;
    }

    return ptr;
}
