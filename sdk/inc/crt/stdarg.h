/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    stdarg.h

Abstract:

    Provides variable argument list definitions.

--*/

#pragma once

#ifndef _STDARG_H
#define _STDARG_H 1

#if defined(__cplusplus)
extern "C" {
#endif

typedef __builtin_va_list va_list;

#define va_start(ap, v) __builtin_va_start(ap, v)
#define va_arg(ap, t)   __builtin_va_arg(ap, t)
#define va_copy(ap, s)  __builtin_va_copy(ap, s)
#define va_end(ap)      __builtin_va_end(ap)

#if defined(__cplusplus)
}
#endif

#endif /* !_STDARG_H */
