/*++

Copyright (c) 2025, Quinn Stephens, w1redch4d
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    ntintsafe.h

Abstract:

    Provides safe integer computation services.

--*/

#pragma once

#ifndef _NTINTSAFE_H
#define _NTINTSAFE_H

#define ULONG_ERROR     0xffffffffUL
#define ULONGLONG_ERROR 0xffffffffffffffffui64

#if defined(_WIN64)
#define SIZE_T_ERROR ULONGLONG_ERROR
#else
#define SIZE_T_ERROR ULONG_ERROR
#endif
#define SIZE_T_MAX SIZE_T_ERROR

NTSTATUS
FORCEINLINE
RtlULongLongToULong (
   IN  ULONGLONG ullOperand,
   OUT PULONG    pulResult
   )

/*++

Routine Description:

   Converts a ULONGLONG to a ULONG.

Arguments:

   ullOperand - The ULONGLONG to convert.

   pulResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    if (ullOperand <= MAXULONG) {
        *pulResult = (ULONG)ullOperand;
        return STATUS_SUCCESS;
    }

    *pulResult = ULONG_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

NTSTATUS
FORCEINLINE
RtlULongAdd (
   IN  ULONG  ulAugend,
   IN  ULONG  ulAddend,
   OUT PULONG pulResult
   )

/*++

Routine Description:

    Calculates the sum of two ULONG values.

Arguments:

    ulAugend - The value to add ulAddend to.

    ulAdddend - The value to add to ulAugend.

    pulResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    if ((ulAugend + ulAddend) >= ulAugend) {
        *pulResult = ulAugend + ulAddend;
        return STATUS_SUCCESS;
    }

    *pulResult = ULONG_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

NTSTATUS
FORCEINLINE
RtlULongSub (
   IN  ULONG  ulMinuend,
   IN  ULONG  ulSubtrahend,
   OUT PULONG pulResult
   )

/*++

Routine Description:

    Calculates the difference of two ULONG values.

Arguments:

    ulMinuend - The value to subtract ulSubtrahend from.

    ulSubtrahend - The value to subtract from ulMinuend.

    pulResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    if (ulMinuend >= ulSubtrahend) {
        *pulResult = ulMinuend - ulSubtrahend;
        return STATUS_SUCCESS;
    }

    *pulResult = ULONG_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

NTSTATUS
FORCEINLINE
RtlULongMult (
   IN  ULONG  ulMultiplicand,
   IN  ULONG  ulMultiplier,
   OUT PULONG pulResult
   )

/*++

Routine Description:

    Calculates the product of two ULONG values.

Arguments:

    ulMultiplicand - The value to multiply by ulMultiplier.

    ulMultiplier - The value to multiply ulMultiplicand by.

    pulResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    ULONGLONG ullProduct;

    ullProduct = (ULONGLONG)(ulMultiplicand * ulMultiplier);

    return RtlULongLongToULong(ullProduct, pulResult);
}

NTSTATUS
FORCEINLINE
RtlSizeTAdd (
   IN  size_t Augend,
   IN  size_t Addend,
   OUT size_t *pResult
   )

/*++

Routine Description:

    Calculates the sum of two size_t values.

Arguments:

    Augend - The value to add Addend to.

    Adddend - The value to add to Augend.

    pResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    if ((Augend + Addend) >= Augend) {
        *pResult = Augend + Addend;
        return STATUS_SUCCESS;
    }

    *pResult = SIZE_T_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

NTSTATUS
FORCEINLINE
RtlSizeTSub (
   IN  size_t Minuend,
   IN  size_t Subtrahend,
   OUT size_t *pResult
   )

/*++

Routine Description:

    Calculates the difference of two size_t values.

Arguments:

    Minuend - The value to subtract Subtrahend from.

    Subtrahend - The value to subtract from Minuend.

    pResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    if (Minuend >= Subtrahend) {
        *pResult = Minuend - Subtrahend;
        return STATUS_SUCCESS;
    }

    *pResult = SIZE_T_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

NTSTATUS
FORCEINLINE
RtlSizeTMult (
   IN  size_t Multiplicand,
   IN  size_t Multiplier,
   OUT size_t *pResult
   )

/*++

Routine Description:

    Calculates the product of two size_t values.

Arguments:

    Multiplicand - The value to multiply by Multiplier.

    Multiplier - The value to multiply Multiplicand by.

    pResult - Pointer to the result.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    __uint128_t Product;

    Product = (__uint128_t)(Multiplicand * Multiplier);
    if (Product <= SIZE_T_MAX) {
        *pResult = (size_t)Product;
        return STATUS_SUCCESS;
    }

    *pResult = SIZE_T_ERROR;
    return STATUS_INTEGER_OVERFLOW;
}

#endif /* !_NTINTSAFE_H */
