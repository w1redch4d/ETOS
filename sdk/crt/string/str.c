/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    str.c

Abstract:

    String manipulation routines.

--*/

#include <string.h>

size_t
strlen (
    const char *str
    )

{
    const char *ptr;

    ptr = str;
    while (*ptr) {
        ptr++;
    }

    return ptr - str;
}

size_t
strnlen (
    const char *str,
    size_t maxlen
    )

{
    const char *ptr;

    ptr = str;
    while (maxlen-- && *ptr) {
        ptr++;
    }

    return ptr - str;
}

int
strcmp (
    const char *s1,
    const char *s2
    )

{
    while (*s1 == *s2) {
        if (*s1 == '\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return *(unsigned char *)s1 - *(unsigned char*)s2;
}

int
strncmp (
    const char *s1,
    const char *s2,
    size_t n
    )

{
    while (n > 0) {
        if (*s1 != *s2) {
            return *(unsigned char *)s1 - *(unsigned char*)s2;
        }

        if (*s1 == '\0') {
            return 0;
        }

        n--;
        s1++;
        s2++;
    }

    return 0;
}

char *
strchr (
    const char *s,
    int c
    )

{
    while (*s != (char)c) {
        if (!*s) {
            return NULL;
        }

        s++;
    }

    return (char *)s;
}

char *
strstr (
    const char *haystack,
    const char *needle
    )

{
    const char *ptr = haystack;

    if (!*needle) {
        return (char *)haystack;
    }

    while ((ptr = strchr(ptr, *needle)) != NULL) {
        if (strcmp(ptr, needle) == 0) {
            return (char *)ptr;
        }
    }

    return NULL;
}
