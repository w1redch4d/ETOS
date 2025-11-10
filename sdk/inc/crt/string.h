/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    string.h

Abstract:

    Provides string definitions.

--*/

#pragma once

#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

void *memset(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);

size_t strlen(const char *str);
size_t strnlen(const char *str, size_t maxlen);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);

#if defined(__cplusplus)
}
#endif

#endif /* !_STRING_H */
