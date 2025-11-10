/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mem.c

Abstract:

    Memory manipulation routines.

--*/

#include <string.h>

void *
memset (
    void *dest,
    int c,
    size_t count
    )

{
    void *ptr = dest;

    while (count--) {
        *(char *)dest = (char)c;
        dest = (char *)dest + 1;
    }

    return ptr;
}

void *
memcpy (
    void *dest,
    const void *src,
    size_t count
    )

{
    void *ptr = dest;

    while (count--) {
        *(char *)dest = *(char *)src;
        dest = (char *)dest + 1;
        src = (char *)src + 1;
    }

    return ptr;
}

void *
memmove (
    void *dest,
    const void *src,
    size_t count
    )

{
    void *ptr = dest;

    /* Check for overlap */
    if (src > dest || ((char *)src + count) < (char *)dest) {
        /* Low-to-high copy */
        return memcpy(dest, src, count);
    }

    /* High-to-low copy */
    dest = (char *)dest + count - 1;
    src = (char *)src + count - 1;
    while (count--) {
        *(char *)dest = *(char *)src;
        dest = (char *)dest - 1;
        src = (char *)src - 1;
    }

    return ptr;
}
