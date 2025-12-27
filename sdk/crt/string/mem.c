/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mem.c

Abstract:

    Memory services.

--*/

#include <string.h>

void *
memset (
    void   *s,
    int    c,
    size_t n
    )

{
    void *ptr;

    ptr = s;
    while (n--) {
        *(char *)s = (char)c;
        s = (char *)s + 1;
    }

    return ptr;
}

void *
memcpy (
    void       *dest,
    const void *src,
    size_t     n
    )

{
    void *ptr;

    ptr = dest;
    while (n--) {
        *(char *)dest = *(char *)src;
        dest = (char *)dest + 1;
        src = (char *)src + 1;
    }

    return ptr;
}

void *
memmove (
    void       *dest,
    const void *src,
    size_t     n
    )

{
    void *ptr;

    ptr = dest;

    /* Check for overlap */
    if (src >= dest || ((char *)src + n) <= (char *)dest) {
        /* Low-to-high copy */
        while (n--) {
            *(char *)dest = *(char *)src;
            dest = (char *)dest + 1;
            src = (char *)src + 1;
        }
    } else {
        /* High-to-low copy */
        dest = (char *)dest + n;
        src = (char *)src + n;
        while (n--) {
            dest = (char *)dest - 1;
            src = (char *)src - 1;
            *(char *)dest = *(char *)src;
        }
    }

    return ptr;
}

int
memcmp (
    const void *s1,
    const void *s2,
    size_t     n
    )

{
    while (n--) {
        if (*(char *)s1 != *(char *)s2) {
            return *(unsigned char *)s1 - *(unsigned char *)s2;
        }

        s1 = (char *)s1 + 1;
        s2 = (char *)s2 + 1;
    }

    return 0;
}
