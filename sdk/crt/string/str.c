/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    str.c

Abstract:

    String services.

--*/

#include <string.h>

size_t
strlen (
    const char *s
    )

{
    const char *ptr;

    ptr = s;
    while (*ptr != '\0') {
        ptr++;
    }

    return ptr - s;
}

size_t
strnlen (
    const char *s,
    size_t     maxlen
    )

{
    const char *ptr;

    ptr = s;
    while (maxlen-- && *ptr != '\0') {
        ptr++;
    }

    return ptr - s;
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

    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int
strncmp (
    const char *s1,
    const char *s2,
    size_t     n
    )

{
    while (n--) {
        if (*s1 != *s2) {
            return *(unsigned char *)s1 - *(unsigned char *)s2;
        }

        if (*s1 == '\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return 0;
}

char *
strchr (
    const char *s,
    int        c
    )

{
    while (*s != (char)c) {
        if (*s == '\0') {
            return NULL;
        }

        s++;
    }

    return (char *)s;
}

char *
strrchr (
    const char *s,
    int        c
    )

{
    const char *last;

    last = NULL;
    do {
        if (*s == (char)c) {
            last = s;
        }
    } while (*s++ != '\0');

    return (char *)last;
}

char *
strstr (
    const char *haystack,
    const char *needle
    )

{
    const char *pos;

    if (*needle == '\0') {
        return (char *)haystack;
    }

    pos = haystack;
    while ((pos = strchr(pos, *needle)) != NULL) {
        if (strcmp(pos, needle) == 0) {
            return (char *)pos;
        }
    }

    return NULL;
}
