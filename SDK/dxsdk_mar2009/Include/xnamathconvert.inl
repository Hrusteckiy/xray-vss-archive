/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    xnamathconvert.inl

Abstract:

	XNA math library for Windows and Xbox 360: Conversion, loading, and storing functions.
--*/

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef __XNAMATHCONVERT_INL__
#define __XNAMATHCONVERT_INL__

#define XM_PACK_FACTOR                  (FLOAT)(1 << 22)
#define XM_UNPACK_FACTOR_UNSIGNED       (FLOAT)(1 << 23)
#define XM_UNPACK_FACTOR_SIGNED         XM_PACK_FACTOR

#define XM_UNPACK_UNSIGNEDN_OFFSET(BitsX, BitsY, BitsZ, BitsW) \
                                        {-XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsX)) - 1), \
                                         -XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsY)) - 1), \
                                         -XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsZ)) - 1), \
                                         -XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsW)) - 1)}

#define XM_UNPACK_UNSIGNEDN_SCALE(BitsX, BitsY, BitsZ, BitsW) \
                                        {XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsX)) - 1), \
                                         XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsY)) - 1), \
                                         XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsZ)) - 1), \
                                         XM_UNPACK_FACTOR_UNSIGNED / (FLOAT)((1 << (BitsW)) - 1)}

#define XM_UNPACK_SIGNEDN_SCALE(BitsX, BitsY, BitsZ, BitsW) \
                                        {-XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsX) - 1)) - 1), \
                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsY) - 1)) - 1), \
                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsZ) - 1)) - 1), \
                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsW) - 1)) - 1)}

//#define XM_UNPACK_SIGNEDN_OFFSET(BitsX, BitsY, BitsZ, BitsW) \
//                                        {-XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsX) - 1)) - 1) * 3.0f, \
//                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsY) - 1)) - 1) * 3.0f, \
//                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsZ) - 1)) - 1) * 3.0f, \
//                                         -XM_UNPACK_FACTOR_SIGNED / (FLOAT)((1 << ((BitsW) - 1)) - 1) * 3.0f}

#define XM_PACK_UNSIGNEDN_SCALE(BitsX, BitsY, BitsZ, BitsW) \
                                        {-(FLOAT)((1 << (BitsX)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << (BitsY)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << (BitsZ)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << (BitsW)) - 1) / XM_PACK_FACTOR}

#define XM_PACK_SIGNEDN_SCALE(BitsX, BitsY, BitsZ, BitsW) \
                                        {-(FLOAT)((1 << ((BitsX) - 1)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << ((BitsY) - 1)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << ((BitsZ) - 1)) - 1) / XM_PACK_FACTOR, \
                                         -(FLOAT)((1 << ((BitsW) - 1)) - 1) / XM_PACK_FACTOR}

#define XM_PACK_OFFSET                  XMVectorSplatConstant(3, 0)
//#define XM_UNPACK_OFFSET                XM_PACK_OFFSET

/****************************************************************************
 *
 * Data conversion
 *
 ****************************************************************************/

//------------------------------------------------------------------------------

XMFINLINE FLOAT XMConvertHalfToFloat
(
    HALF Value
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)

    UINT Mantissa;
    UINT Exponent;
    UINT Result;

    Mantissa = (UINT)(Value & 0x03FF);

    if ((Value & 0x7C00) != 0)  // The value is normalized
    {
        Exponent = (UINT)((Value >> 10) & 0x1F);
    }
    else if (Mantissa != 0)     // The value is denormalized
    {
        // Normalize the value in the resulting float
        Exponent = 1;

        do
        {
            Exponent--;
            Mantissa <<= 1;
        } while ((Mantissa & 0x0400) == 0);

        Mantissa &= 0x03FF;
    }
    else                        // The value is zero
    {
        Exponent = (UINT)-112;
    }

    Result = ((Value & 0x8000) << 16) | // Sign
             ((Exponent + 112) << 23) | // Exponent
             (Mantissa << 13);          // Mantissa

    return *(FLOAT*)&Result;

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif
}

//------------------------------------------------------------------------------

XMINLINE FLOAT* XMConvertHalfToFloatStream
(
    FLOAT*      pOutputStream, 
    UINT        OutputStride, 
    CONST HALF* pInputStream, 
    UINT        InputStride, 
    UINT        HalfCount
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)

    UINT  i;
    BYTE* pHalf = (BYTE*)pInputStream;
    BYTE* pFloat = (BYTE*)pOutputStream;

    XMASSERT(pOutputStream);
    XMASSERT(pInputStream);

    for (i = 0; i < HalfCount; i++)
    {
        *(FLOAT*)pFloat = XMConvertHalfToFloat(*(HALF*)pHalf);
        pHalf += InputStride;
        pFloat += OutputStride; 
    }

    return pOutputStream;

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE HALF XMConvertFloatToHalf
(
    FLOAT Value
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)
    UINT Result;

    UINT IValue = ((UINT *)(&Value))[0];
    UINT Sign = (IValue & 0x80000000U) >> 16U;
    IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

    if (IValue > 0x47FFEFFFU)
    {
        // The number is too large to be represented as a half.  Saturate to infinity.
        Result = 0x7FFFU;
    }
    else
    {
        if (IValue < 0x38800000U)
        {
            // The number is too small to be represented as a normalized half.
            // Convert it to a denormalized value.
            UINT Shift = 113U - (IValue >> 23U);
            IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
        }
        else
        {
            // Rebias the exponent to represent the value as a normalized half.
            IValue += 0xC8000000U;
        }

        Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
    }
    return (HALF)(Result|Sign);

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif
}

//------------------------------------------------------------------------------

XMINLINE HALF* XMConvertFloatToHalfStream
(
    HALF*        pOutputStream, 
    UINT         OutputStride, 
    CONST FLOAT* pInputStream, 
    UINT         InputStride, 
    UINT         FloatCount
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)

    UINT  i;
    BYTE* pFloat = (BYTE*)pInputStream;
    BYTE* pHalf = (BYTE*)pOutputStream;

    XMASSERT(pOutputStream);
    XMASSERT(pInputStream);

    for (i = 0; i < FloatCount; i++)
    {
        *(HALF*)pHalf = XMConvertFloatToHalf(*(FLOAT*)pFloat);
        pFloat += InputStride; 
        pHalf += OutputStride;
    }
    return pOutputStream;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)
// For VMX128, these routines are all defines in the main header

#pragma warning(push)
#pragma warning(disable:4701) // Prevent warnings about 'Result' potentially being used without having been initialized

XMINLINE XMVECTOR XMConvertVectorIntToFloat
(
    FXMVECTOR VInt,
    UINT     DivExponent
)
{
#if defined(_XM_NO_INTRINSICS_)
    UINT ElementIndex;
    FLOAT fScale;
    XMVECTOR Result;
    XMASSERT(DivExponent<32);
    fScale = 1.0f / (FLOAT)(1U << DivExponent);
    ElementIndex = 0;
    do {
        INT iTemp = (INT)VInt.u[ElementIndex];
        Result.v[ElementIndex] = ((FLOAT)iTemp) * fScale;
    } while (++ElementIndex<4);
    return Result;
#else // _XM_SSE_INTRINSICS_
    XMASSERT(DivExponent<32);
    // Convert to floats
    XMVECTOR vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&VInt)[0]);
    // Convert DivExponent into 1.0f/(1<<DivExponent)
    UINT uScale = 0x3F800000U - (DivExponent << 23);
    // Splat the scalar value
    __m128i vScale = _mm_set1_epi32(uScale);
    vResult = _mm_mul_ps(vResult,reinterpret_cast<const __m128 *>(&vScale)[0]);
    return vResult;
#endif
}

//------------------------------------------------------------------------------

XMINLINE XMVECTOR XMConvertVectorFloatToInt
(
    FXMVECTOR VFloat,
    UINT     MulExponent
)
{
#if defined(_XM_NO_INTRINSICS_)
    UINT ElementIndex;
    XMVECTOR Result;
    FLOAT fScale;
    XMASSERT(MulExponent<32);
    // Get the scalar factor.
    fScale = (FLOAT)(1U << MulExponent);
    ElementIndex = 0;
    do {
        INT iResult;
        FLOAT fTemp = VFloat.v[ElementIndex]*fScale;
        if (fTemp <= -(65536.0f*32768.0f)) {
            iResult = (-0x7FFFFFFF)-1;
        } else if (fTemp > (65536.0f*32768.0f)-128.0f) {
            iResult = 0x7FFFFFFF;
        } else {
            iResult = (INT)fTemp;
        }
        Result.u[ElementIndex] = (UINT)iResult;
    } while (++ElementIndex<4);
    return Result;
#else // _XM_SSE_INTRINSICS_
    XMASSERT(MulExponent<32);
    static const XMVECTORF32 g_XMMaxInt = {65536.0f*32768.0f-128.0f,65536.0f*32768.0f-128.0f,65536.0f*32768.0f-128.0f,65536.0f*32768.0f-128.0f};
	XMVECTOR vResult = _mm_set_ps1((FLOAT)(1U << MulExponent));
	vResult = _mm_mul_ps(vResult,VFloat);
    // In case of positive overflow, detect it
    XMVECTOR vOverflow = _mm_cmpgt_ps(vResult,g_XMMaxInt);
    // Float to int conversion
	__m128i vResulti = _mm_cvttps_epi32(vResult);
    // If there was positive overflow, set to 0x7FFFFFFF
    vResult = _mm_and_ps(vOverflow,g_XMAbsMask);
    vOverflow = _mm_andnot_ps(vOverflow,reinterpret_cast<const __m128 *>(&vResulti)[0]);
    vOverflow = _mm_or_ps(vOverflow,vResult);
    return vOverflow;
#endif
}

//------------------------------------------------------------------------------

XMINLINE XMVECTOR XMConvertVectorUIntToFloat
(
    FXMVECTOR VUInt,
    UINT      DivExponent
)
{
#if defined(_XM_NO_INTRINSICS_)
    UINT ElementIndex;
    FLOAT fScale;
    XMVECTOR Result;
    XMASSERT(DivExponent<32);
    fScale = 1.0f / (FLOAT)(1U << DivExponent);
    ElementIndex = 0;
    do {
        Result.v[ElementIndex] = (FLOAT)VUInt.u[ElementIndex] * fScale;
    } while (++ElementIndex<4);
    return Result;
#else // _XM_SSE_INTRINSICS_
    XMASSERT(DivExponent<32);
    static const XMVECTORF32 g_XMFixUnsigned = {32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f};
    // For the values that are higher than 0x7FFFFFFF, a fixup is needed
    // Determine which ones need the fix.
    XMVECTOR vMask = _mm_and_ps(VUInt,g_XMNegativeZero);
    // Force all values positive
    XMVECTOR vResult = _mm_xor_ps(VUInt,vMask);
    // Convert to floats
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert 0x80000000 -> 0xFFFFFFFF
    __m128i iMask = _mm_srai_epi32(reinterpret_cast<const __m128i *>(&vMask)[0],31);
    // For only the ones that are too big, add the fixup
    vMask = _mm_and_ps(reinterpret_cast<const __m128 *>(&iMask)[0],g_XMFixUnsigned);
    vResult = _mm_add_ps(vResult,vMask);
    // Convert DivExponent into 1.0f/(1<<DivExponent)
    UINT uScale = 0x3F800000U - (DivExponent << 23);
    // Splat
    iMask = _mm_set1_epi32(uScale);
    vResult = _mm_mul_ps(vResult,reinterpret_cast<const __m128 *>(&iMask)[0]);
    return vResult;
#endif
}

//------------------------------------------------------------------------------

XMINLINE XMVECTOR XMConvertVectorFloatToUInt
(
    FXMVECTOR VFloat,
    UINT      MulExponent
)
{
#if defined(_XM_NO_INTRINSICS_)
    UINT ElementIndex;
    XMVECTOR Result;
    FLOAT fScale;
    XMASSERT(MulExponent<32);
    // Get the scalar factor.
    fScale = (FLOAT)(1U << MulExponent);
    ElementIndex = 0;
    do {
        UINT uResult;
        FLOAT fTemp = VFloat.v[ElementIndex]*fScale;
        if (fTemp <= 0.0f) {
            uResult = 0;
        } else if (fTemp >= (65536.0f*65536.0f)) {
            uResult = 0xFFFFFFFFU;
        } else {
            uResult = (UINT)fTemp;
        }
        Result.u[ElementIndex] = uResult;
    } while (++ElementIndex<4);
    return Result;
#else // _XM_SSE_INTRINSICS_
    XMASSERT(MulExponent<32);
    static const XMVECTORF32 g_XMMaxUInt = {65536.0f*65536.0f-256.0f,65536.0f*65536.0f-256.0f,65536.0f*65536.0f-256.0f,65536.0f*65536.0f-256.0f};
    static const XMVECTORF32 g_XMUnsignedFix = {32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f,32768.0f*65536.0f};
    XMVECTOR vResult = _mm_set_ps1(static_cast<float>(1U << MulExponent));
    vResult = _mm_mul_ps(vResult,VFloat);
    // Clamp to >=0
    vResult = _mm_max_ps(vResult,g_XMZero);
    // Any numbers that are too big, set to 0xFFFFFFFFU
    XMVECTOR vOverflow = _mm_cmpgt_ps(vResult,g_XMMaxUInt);
    XMVECTOR vValue = g_XMUnsignedFix;
    // Too large for a signed integer?
    XMVECTOR vMask = _mm_cmpge_ps(vResult,vValue);
    // Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
    vValue = _mm_and_ps(vValue,vMask);
    // Perform fixup only on numbers too large (Keeps low bit precision)
    vResult = _mm_sub_ps(vResult,vValue);
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Convert from signed to unsigned pnly if greater than 0x80000000
    vMask = _mm_and_ps(vMask,g_XMNegativeZero);
    vResult = _mm_xor_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],vMask);
    // On those that are too large, set to 0xFFFFFFFF
    vResult = _mm_or_ps(vResult,vOverflow);
    return vResult;
#endif
}

#pragma warning(pop)

#endif // _XM_NO_INTRINSICS_ || _XM_SSE_INTRINSICS_

/****************************************************************************
 *
 * Vector and matrix load operations
 *
 ****************************************************************************/

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt(CONST UINT* pSource)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;
    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 3) == 0);

    V.u[0] = *pSource;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 3) == 0);
    __m128i V = _mm_set_epi32( 0, 0, 0, *pSource );
    return reinterpret_cast<__m128 *>(&V)[0];
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat(CONST FLOAT* pSource)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;
    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 3) == 0);

    V.v[0] = *pSource;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 3) == 0);

    return _mm_load_ss( pSource );
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt2
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];

    return V;
#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pSource);
    __m128i V = _mm_set_epi32( 0, 0, *(pSource+1), *pSource );
    return reinterpret_cast<__m128 *>(&V)[0];

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt2A
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];

    return V;

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pSource);
    __m128i V = _mm_loadl_epi64( (const __m128i*)pSource );
    return reinterpret_cast<__m128 *>(&V)[0];

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat2
(
    CONST XMFLOAT2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMVECTOR V;
    XMASSERT(pSource);
    ((UINT *)(&V.x))[0] = ((const UINT *)(&pSource->x))[0];
    ((UINT *)(&V.y))[0] = ((const UINT *)(&pSource->y))[0];
    V.z = 0.0f;
    V.w = 0.0f;
    return V;
#elif defined(_XM_SSE_INTRINSICS_)
    // This reads 2 floats past the memory that should be ignored.
    XMASSERT(pSource);
    return _mm_loadu_ps( &pSource->x );
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat2A
(
    CONST XMFLOAT2A* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.v[0] = pSource->x;
    V.v[1] = pSource->y;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);

    // This reads 2 floats past the memory that should be ignored.

    return _mm_load_ps( &pSource->x );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}
    
//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadHalf2
(
    CONST XMHALF2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMASSERT(pSource);
    {
    XMVECTOR vResult = {
        XMConvertHalfToFloat(pSource->x),
        XMConvertHalfToFloat(pSource->y),
        0.0f,
        0.0f
    };
    return vResult;
    }
#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMVECTOR vResult = {
        XMConvertHalfToFloat(pSource->x),
        XMConvertHalfToFloat(pSource->y),
        0.0f,
        0.0f
    };
    return vResult;

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadShortN2
(
    CONST XMSHORTN2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    {
    XMVECTOR vResult = {
        (FLOAT)pSource->x * (1.0f/32767.0f),
        (FLOAT)pSource->y * (1.0f/32767.0f),
        0.0f,
        0.0f
    };
    return vResult;
    }

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    // Splat the two shorts in all four entries (WORD alignment okay,
    // DWORD alignment preferred)
    __m128 vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0xFFFF, y&0xFFFF0000,z&0,w&0
    vTemp = _mm_and_ps(vTemp,g_XMMaskX16Y16);
    // x needs to be sign extended
    vTemp = _mm_xor_ps(vTemp,g_XMFlipX16Y16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x - 0x8000 to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixX16Y16);
    // Convert 0-32767 to 0.0f-1.0f
    return _mm_mul_ps(vTemp,g_XMNormalizeX16Y16);
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadShort2
(
    CONST XMSHORT2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORF32 g_XMFixupY16 = {1.0f,1.0f/65536.0f,0.0f,0.0f};
    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    // Splat the two shorts in all four entries (WORD alignment okay,
    // DWORD alignment preferred)
    __m128 vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0xFFFF, y&0xFFFF0000,z&0,w&0
    vTemp = _mm_and_ps(vTemp,g_XMMaskX16Y16);
    // x needs to be sign extended
    vTemp = _mm_xor_ps(vTemp,g_XMFlipX16Y16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x - 0x8000 to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixX16Y16);
    // Y is 65536 too large
    return _mm_mul_ps(vTemp,g_XMFixupY16);
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUShortN2
(
    CONST XMUSHORTN2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x / 65535.0f;
    V.v[1] = (FLOAT)pSource->y / 65535.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORF32 g_XMFixupY16 = {1.0f/65535.0f,1.0f/(65535.0f*65536.0f),0.0f,0.0f};
    static const XMVECTORI32 g_XMFlipY = {0,0x80000000,0,0};
    static const XMVECTORF32 g_XMFixaddY16 = {0,32768.0f*65536.0f,0,0};
    XMASSERT(pSource);
    // Splat the two shorts in all four entries (WORD alignment okay,
    // DWORD alignment preferred)
    __m128 vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0xFFFF, y&0xFFFF0000,z&0,w&0
    vTemp = _mm_and_ps(vTemp,g_XMMaskX16Y16);
    // y needs to be sign flipped
    vTemp = _mm_xor_ps(vTemp,g_XMFlipY);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // y + 0x8000 to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixaddY16);
    // Y is 65536 times too large
    vTemp = _mm_mul_ps(vTemp,g_XMFixupY16);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUShort2
(
    CONST XMUSHORT2* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORF32 g_XMFixupY16 = {1.0f,1.0f/65536.0f,0.0f,0.0f};
    static const XMVECTORI32 g_XMFlipY = {0,0x80000000,0,0};
    static const XMVECTORF32 g_XMFixaddY16 = {0,32768.0f,0,0};
    XMASSERT(pSource);
    // Splat the two shorts in all four entries (WORD alignment okay,
    // DWORD alignment preferred)
    __m128 vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0xFFFF, y&0xFFFF0000,z&0,w&0
    vTemp = _mm_and_ps(vTemp,g_XMMaskX16Y16);
    // y needs to be sign flipped
    vTemp = _mm_xor_ps(vTemp,g_XMFlipY);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // Y is 65536 times too large
    vTemp = _mm_mul_ps(vTemp,g_XMFixupY16);
    // y + 0x8000 to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixaddY16);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt3
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];
    V.u[2] = pSource[2];

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    __m128i V = _mm_set_epi32( 0, *(pSource+2), *(pSource+1), *pSource );
    return reinterpret_cast<__m128 *>(&V)[0];
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt3A
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];
    V.u[2] = pSource[2];

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);

    // Reads an extra integer that is 'undefined'

    __m128i V = _mm_load_si128( (const __m128i*)pSource );
    return reinterpret_cast<__m128 *>(&V)[0];
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat3
(
    CONST XMFLOAT3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMVECTOR V;
    XMASSERT(pSource);
    ((UINT *)(&V.x))[0] = ((const UINT *)(&pSource->x))[0];
    ((UINT *)(&V.y))[0] = ((const UINT *)(&pSource->y))[0];
    ((UINT *)(&V.z))[0] = ((const UINT *)(&pSource->z))[0];
    V.w = 0.0f;
    return V;
#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    // This reads 1 floats past the memory that should be ignored.
    return _mm_loadu_ps( &pSource->x );
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat3A
(
    CONST XMFLOAT3A* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.v[0] = pSource->x;
    V.v[1] = pSource->y;
    V.v[2] = pSource->z;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);

	// This reads 1 floats past the memory that should be ignored.

	return _mm_load_ps( &pSource->x );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUHenDN3
(
    CONST XMUHENDN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x7FF;
    V.v[0] = (FLOAT)Element / 2047.0f;
    Element = (pSource->v >> 11) & 0x7FF;
    V.v[1] = (FLOAT)Element / 2047.0f;
    Element = (pSource->v >> 22) & 0x3FF;
    V.v[2] = (FLOAT)Element / 1023.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMUHenDN3And = {0x7FF,0x7ff<<11,0x3FF<<22,0};
    static const XMVECTORI32 g_XMUHenDN3Xor = {0,0,0x80000000,0};
    static const XMVECTORF32 g_XMUHenDN3Add = {0,0,32768.0f*65536.0f,0};
    static const XMVECTORF32 g_XMUHenDN3Mul = {1.0f/2047.0f,1.0f/(2047.0f*2048.0f),1.0f/(1023.0f*2048.0f*2048.0f),0};
    XMASSERT(pSource);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMUHenDN3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMUHenDN3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMUHenDN3Add);
    // Normalize x,y and z to -1.0f-1.0f
    vResult = _mm_mul_ps(vResult,g_XMUHenDN3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUHenD3
(
    CONST XMUHEND3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x7FF;
    V.v[0] = (FLOAT)Element;
    Element = (pSource->v >> 11) & 0x7FF;
    V.v[1] = (FLOAT)Element;
    Element = (pSource->v >> 22) & 0x3FF;
    V.v[2] = (FLOAT)Element;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMUHenD3And = {0x7FF,0x7ff<<11,0x3FF<<22,0};
    static const XMVECTORI32 g_XMUHenD3Xor = {0,0,0x80000000,0};
    static const XMVECTORF32 g_XMUHenD3Add = {0,0,32768.0f*65536.0f,0};
    static const XMVECTORF32 g_XMUHenD3Mul = {1.0f,1.0f/2048.0f,1.0f/(2048.0f*2048.0f),0};
    XMASSERT(pSource);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMUHenD3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMUHenD3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMUHenD3Add);
    // Normalize x and y to -1024-1023.0f and z to -512-511.0f
    vResult = _mm_mul_ps(vResult,g_XMUHenD3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadHenDN3
(
    CONST XMHENDN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtendXY[] = {0x00000000, 0xFFFFF800};
    static CONST UINT SignExtendZ[] = {0x00000000, 0xFFFFFC00};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 11) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 22) & 0x3FF) != 0x200);

    Element = pSource->v & 0x7FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtendXY[Element >> 10]) / 1023.0f;
    Element = (pSource->v >> 11) & 0x7FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtendXY[Element >> 10]) / 1023.0f;
    Element = (pSource->v >> 22) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtendZ[Element >> 9]) / 511.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMHenDN3And = {0x7FF,0x7ff<<11,0x3FF<<22,0};
    static const XMVECTORI32 g_XMHenDN3Xor = {0x400,0x400<<11,0,0};
    static const XMVECTORF32 g_XMHenDN3Add = {-1024.0f,-1024.0f*2048.0f,0,0};
    static const XMVECTORF32 g_XMHenDN3Mul = {1.0f/1023.0f,1.0f/(1023.0f*2048.0f),1.0f/(511.0f*2048.0f*2048.0f),0};
    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 11) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 22) & 0x3FF) != 0x200);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMHenDN3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMHenDN3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMHenDN3Add);
    // Normalize x,y and z to -1.0f-1.0f
    vResult = _mm_mul_ps(vResult,g_XMHenDN3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadHenD3
(
    CONST XMHEND3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtendXY[] = {0x00000000, 0xFFFFF800};
    static CONST UINT SignExtendZ[] = {0x00000000, 0xFFFFFC00};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 11) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 22) & 0x3FF) != 0x200);

    Element = pSource->v & 0x7FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtendXY[Element >> 10]);
    Element = (pSource->v >> 11) & 0x7FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtendXY[Element >> 10]);
    Element = (pSource->v >> 22) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtendZ[Element >> 9]);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMHenD3And = {0x7FF,0x7ff<<11,0x3FF<<22,0};
    static const XMVECTORI32 g_XMHenD3Xor = {0x400,0x400<<11,0,0};
    static const XMVECTORF32 g_XMHenD3Add = {-1024.0f,-1024.0f*2048.0f,0,0};
    static const XMVECTORF32 g_XMHenD3Mul = {1.0f,1.0f/2048.0f,1.0f/(2048.0f*2048.0f),0};
    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 11) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 22) & 0x3FF) != 0x200);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMHenD3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMHenD3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMHenD3Add);
    // Normalize x and y to -1024-1023.0f and z to -512-511.0f
    vResult = _mm_mul_ps(vResult,g_XMHenD3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUDHenN3
(
    CONST XMUDHENN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)Element / 1023.0f;
    Element = (pSource->v >> 10) & 0x7FF;
    V.v[1] = (FLOAT)Element / 2047.0f;
    Element = (pSource->v >> 21) & 0x7FF;
    V.v[2] = (FLOAT)Element / 2047.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMUDHenN3And = {0x3FF,0x7ff<<10,0x7FF<<21,0};
    static const XMVECTORI32 g_XMUDHenN3Xor = {0,0,0x80000000,0};
    static const XMVECTORF32 g_XMUDHenN3Add = {0,0,32768.0f*65536.0f,0};
    static const XMVECTORF32 g_XMUDHenN3Mul = {1.0f/1023.0f,1.0f/(2047.0f*1024.0f),1.0f/(2047.0f*1024.0f*2048.0f),0};
    XMASSERT(pSource);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMUDHenN3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMUDHenN3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMUDHenN3Add);
    // Normalize x,y and z to -1.0f-1.0f
    vResult = _mm_mul_ps(vResult,g_XMUDHenN3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUDHen3
(
    CONST XMUDHEN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)Element;
    Element = (pSource->v >> 10) & 0x7FF;
    V.v[1] = (FLOAT)Element;
    Element = (pSource->v >> 21) & 0x7FF;
    V.v[2] = (FLOAT)Element;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMUDHen3And = {0x3FF,0x7ff<<10,0x7FF<<21,0};
    static const XMVECTORI32 g_XMUDHen3Xor = {0,0,0x80000000,0};
    static const XMVECTORF32 g_XMUDHen3Add = {0,0,32768.0f*65536.0f,0};
    static const XMVECTORF32 g_XMUDHen3Mul = {1.0f,1.0f/1024.0f,1.0f/(1024.0f*2048.0f),0};
    XMASSERT(pSource);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMUDHen3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMUDHen3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMUDHen3Add);
    // Normalize x to 0-1023.0f and y and z to 0-2047.0f
    vResult = _mm_mul_ps(vResult,g_XMUDHen3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadDHenN3
(
    CONST XMDHENN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtendX[] = {0x00000000, 0xFFFFFC00};
    static CONST UINT SignExtendYZ[] = {0x00000000, 0xFFFFF800};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 21) & 0x7FF) != 0x400);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtendX[Element >> 9]) / 511.0f;
    Element = (pSource->v >> 10) & 0x7FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtendYZ[Element >> 10]) / 1023.0f;
    Element = (pSource->v >> 21) & 0x7FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtendYZ[Element >> 10]) / 1023.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMDHenN3And = {0x3FF,0x7ff<<10,0x7FF<<21,0};
    static const XMVECTORI32 g_XMDHenN3Xor = {0x200,0x400<<10,0,0};
    static const XMVECTORF32 g_XMDHenN3Add = {-512.0f,-1024.0f*1024.0f,0,0};
    static const XMVECTORF32 g_XMDHenN3Mul = {1.0f/511.0f,1.0f/(1023.0f*1024.0f),1.0f/(1023.0f*1024.0f*2048.0f),0};
    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 21) & 0x7FF) != 0x400);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMDHenN3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMDHenN3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMDHenN3Add);
    // Normalize x,y and z to -1.0f-1.0f
    vResult = _mm_mul_ps(vResult,g_XMDHenN3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadDHen3
(
    CONST XMDHEN3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtendX[] = {0x00000000, 0xFFFFFC00};
    static CONST UINT SignExtendYZ[] = {0x00000000, 0xFFFFF800};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 21) & 0x7FF) != 0x400);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtendX[Element >> 9]);
    Element = (pSource->v >> 10) & 0x7FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtendYZ[Element >> 10]);
    Element = (pSource->v >> 21) & 0x7FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtendYZ[Element >> 10]);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMDHen3And = {0x3FF,0x7ff<<10,0x7FF<<21,0};
    static const XMVECTORI32 g_XMDHen3Xor = {0x200,0x400<<10,0,0};
    static const XMVECTORF32 g_XMDHen3Add = {-512.0f,-1024.0f*1024.0f,0,0};
    static const XMVECTORF32 g_XMDHen3Mul = {1.0f,1.0f/1024.0f,1.0f/(1024.0f*2048.0f),0};
    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x7FF) != 0x400);
    XMASSERT(((pSource->v >> 21) & 0x7FF) != 0x400);
    // Get the 32 bit value and splat it
    XMVECTOR vResult = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Mask off x, y and z
    vResult = _mm_and_ps(vResult,g_XMDHen3And);
    // Convert x and y to unsigned
    vResult = _mm_xor_ps(vResult,g_XMDHen3Xor);
    // Convert to float
    vResult = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vResult)[0]);
    // Convert x and y back to signed
    vResult = _mm_add_ps(vResult,g_XMDHen3Add);
    // Normalize x to -210-511.0f and y and z to -1024-1023.0f
    vResult = _mm_mul_ps(vResult,g_XMDHen3Mul);
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt4
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];
    V.u[2] = pSource[2];
    V.u[3] = pSource[3];

    return V;

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pSource);
    __m128i V = _mm_loadu_si128( (const __m128i*)pSource );
    return reinterpret_cast<__m128 *>(&V)[0];

#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadInt4A
(
    CONST UINT* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.u[0] = pSource[0];
    V.u[1] = pSource[1];
    V.u[2] = pSource[2];
    V.u[3] = pSource[3];

    return V;

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    __m128i V = _mm_load_si128( (const __m128i*)pSource );
    return reinterpret_cast<__m128 *>(&V)[0];


#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat4
(
    CONST XMFLOAT4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMVECTOR V;
    XMASSERT(pSource);
    ((UINT *)(&V.x))[0] = ((const UINT *)(&pSource->x))[0];
    ((UINT *)(&V.y))[0] = ((const UINT *)(&pSource->y))[0];
    ((UINT *)(&V.z))[0] = ((const UINT *)(&pSource->z))[0];
    ((UINT *)(&V.w))[0] = ((const UINT *)(&pSource->w))[0];
    return V;
#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    return _mm_loadu_ps( &pSource->x );
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadFloat4A
(
    CONST XMFLOAT4A* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    V.v[0] = pSource->x;
    V.v[1] = pSource->y;
    V.v[2] = pSource->z;
    V.v[3] = pSource->w;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    return _mm_load_ps( &pSource->x );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadHalf4
(
    CONST XMHALF4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMASSERT(pSource);
    {
    XMVECTOR vResult = {
        XMConvertHalfToFloat(pSource->x),
        XMConvertHalfToFloat(pSource->y),
        XMConvertHalfToFloat(pSource->z),
        XMConvertHalfToFloat(pSource->w)
    };
    return vResult;
    }
#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    XMVECTOR vResult = {
        XMConvertHalfToFloat(pSource->x),
        XMConvertHalfToFloat(pSource->y),
        XMConvertHalfToFloat(pSource->z),
        XMConvertHalfToFloat(pSource->w)
    };
    return vResult;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadShortN4
(
    CONST XMSHORTN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    XMASSERT(pSource->z != -32768);
    XMASSERT(pSource->w != -32768);
    {
    XMVECTOR vResult = {
        (FLOAT)pSource->x * (1.0f/32767.0f),
        (FLOAT)pSource->y * (1.0f/32767.0f),
        (FLOAT)pSource->z * (1.0f/32767.0f),
        (FLOAT)pSource->w * (1.0f/32767.0f)
    };
    return vResult;
    }
#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    XMASSERT(pSource->z != -32768);
    XMASSERT(pSource->w != -32768);
    // Splat the color in all four entries (x,z,y,w)
    __m128d vIntd = _mm_load1_pd(reinterpret_cast<const double *>(&pSource->x));
    // Shift x&0ffff,z&0xffff,y&0xffff0000,w&0xffff0000
    __m128 vTemp = _mm_and_ps(reinterpret_cast<const __m128 *>(&vIntd)[0],g_XMMaskX16Y16Z16W16);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMFlipX16Y16Z16W16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x8000 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMFixX16Y16Z16W16);
    // Convert -32767-32767 to -1.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMNormalizeX16Y16Z16W16);
    // Very important! The entries are x,z,y,w, flip it to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(3,1,2,0));
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadShort4
(
    CONST XMSHORT4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    XMASSERT(pSource->z != -32768);
    XMASSERT(pSource->w != -32768);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;
    V.v[2] = (FLOAT)pSource->z;
    V.v[3] = (FLOAT)pSource->w;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORF32 g_XMFixupY16W16 = {1.0f,1.0f,1.0f/65536.0f,1.0f/65536.0f};
	XMASSERT(pSource);
    XMASSERT(pSource->x != -32768);
    XMASSERT(pSource->y != -32768);
    XMASSERT(pSource->z != -32768);
    XMASSERT(pSource->w != -32768);
    // Splat the color in all four entries (x,z,y,w)
    __m128d vIntd = _mm_load1_pd(reinterpret_cast<const double *>(&pSource->x));
    // Shift x&0ffff,z&0xffff,y&0xffff0000,w&0xffff0000
    __m128 vTemp = _mm_and_ps(reinterpret_cast<const __m128 *>(&vIntd)[0],g_XMMaskX16Y16Z16W16);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMFlipX16Y16Z16W16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x8000 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMFixX16Y16Z16W16);
    // Fix y and w because they are 65536 too large
    vTemp = _mm_mul_ps(vTemp,g_XMFixupY16W16);
    // Very important! The entries are x,z,y,w, flip it to x,y,z,w
    return _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(3,1,2,0));
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUShortN4
(
    CONST XMUSHORTN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x / 65535.0f;
    V.v[1] = (FLOAT)pSource->y / 65535.0f;
    V.v[2] = (FLOAT)pSource->z / 65535.0f;
    V.v[3] = (FLOAT)pSource->w / 65535.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    static const XMVECTORF32 g_XMFixupY16W16 = {1.0f/65535.0f,1.0f/65535.0f,1.0f/(65535.0f*65536.0f),1.0f/(65535.0f*65536.0f)};
    static const XMVECTORI32 g_XMFlipY16W16  = {0x00000000, 0x00000000, 0x80000000, 0x80000000};
    static const XMVECTORF32 g_XMFixaddY16W16  = {0, 0, 32768.0f*65536.0f, 32768.0f*65536.0f};
	XMASSERT(pSource);
    // Splat the color in all four entries (x,z,y,w)
    __m128d vIntd = _mm_load1_pd(reinterpret_cast<const double *>(&pSource->x));
    // Shift x&0ffff,z&0xffff,y&0xffff0000,w&0xffff0000
    __m128 vTemp = _mm_and_ps(reinterpret_cast<const __m128 *>(&vIntd)[0],g_XMMaskX16Y16Z16W16);
    // y and w are signed! Flip the bits to convert the order to unsigned
    vTemp = _mm_xor_ps(vTemp,g_XMFlipY16W16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // y and w + 0x8000 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMFixaddY16W16);
    // Fix y and w because they are 65536 too large
    vTemp = _mm_mul_ps(vTemp,g_XMFixupY16W16);
    // Very important! The entries are x,z,y,w, flip it to x,y,z,w
    return _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(3,1,2,0));
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUShort4
(
    CONST XMUSHORT4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;
    V.v[2] = (FLOAT)pSource->z;
    V.v[3] = (FLOAT)pSource->w;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    static const XMVECTORF32 g_XMFixupY16W16 = {1.0f,1.0f,1.0f/65536.0f,1.0f/65536.0f};
    static const XMVECTORI32 g_XMFlipY16W16  = {0x00000000, 0x00000000, 0x80000000, 0x80000000};
    static const XMVECTORF32 g_XMFixaddY16W16  = {0, 0, 32768.0f, 32768.0f};
	XMASSERT(pSource);
    // Splat the color in all four entries (x,z,y,w)
    __m128d vIntd = _mm_load1_pd(reinterpret_cast<const double *>(&pSource->x));
    // Shift x&0ffff,z&0xffff,y&0xffff0000,w&0xffff0000
    __m128 vTemp = _mm_and_ps(reinterpret_cast<const __m128 *>(&vIntd)[0],g_XMMaskX16Y16Z16W16);
    // y and w are signed! Flip the bits to convert the order to unsigned
    vTemp = _mm_xor_ps(vTemp,g_XMFlipY16W16);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // Fix y and w because they are 65536 too large
    vTemp = _mm_mul_ps(vTemp,g_XMFixupY16W16);
    // y and w + 0x8000 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMFixaddY16W16);
    // Very important! The entries are x,z,y,w, flip it to x,y,z,w
    return _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(3,1,2,0));
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadXIcoN4
(
    CONST XMXICON4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFF00000};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 20) & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 40) & 0xFFFFFull) != 0x80000ull);

    Element = (UINT)pSource->v & 0xFFFFF;
    V.v[0] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    Element = (UINT)(pSource->v >> 20) & 0xFFFFF;
    V.v[1] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    Element = (UINT)(pSource->v >> 40) & 0xFFFFF;
    V.v[2] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    V.v[3] = (FLOAT)(pSource->v >> 60) / 15.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT((pSource->v & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 20) & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 40) & 0xFFFFFull) != 0x80000ull);
    static const XMVECTORI32 g_XMLoadXIcoN4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadXIcoN4Xor = {0x80000,0x00000000,0x80000,0x80000000};
    static const XMVECTORF32 g_XMLoadXIcoN4Add = {-8.0f*65536.0f,0,-8.0f*65536.0f,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadXIcoN4Mul = {1.0f/524287.0f,1.0f/(524287.0f*4096.0f),1.0f/524287.0f,1.0f/(15.0f*4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadXIcoN4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadXIcoN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadXIcoN4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadXIcoN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadXIco4
(
    CONST XMXICO4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFF00000};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 20) & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 40) & 0xFFFFFull) != 0x80000ull);

    Element = (UINT)pSource->v & 0xFFFFF;
    V.v[0] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    Element = (UINT)(pSource->v >> 20) & 0xFFFFF;
    V.v[1] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    Element = (UINT)(pSource->v >> 40) & 0xFFFFF;
    V.v[2] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    V.v[3] = (FLOAT)(pSource->v >> 60);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT((pSource->v & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 20) & 0xFFFFFull) != 0x80000ull);
    XMASSERT(((pSource->v >> 40) & 0xFFFFFull) != 0x80000ull);
    static const XMVECTORI32 g_XMLoadXIco4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadXIco4Xor = {0x80000,0x00000000,0x80000,0x80000000};
    static const XMVECTORF32 g_XMLoadXIco4Add = {-8.0f*65536.0f,0,-8.0f*65536.0f,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadXIco4Mul = {1.0f,1.0f/4096.0f,1.0f,1.0f/(4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadXIco4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadXIco4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadXIco4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadXIco4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUIcoN4
(
    CONST XMUICON4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)(pSource->v & 0xFFFFF) / 1048575.0f;
    V.v[1] = (FLOAT)((pSource->v >> 20) & 0xFFFFF) / 1048575.0f;
    V.v[2] = (FLOAT)((pSource->v >> 40) & 0xFFFFF) / 1048575.0f;
    V.v[3] = (FLOAT)(pSource->v >> 60) / 15.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadUIcoN4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadUIcoN4Xor = {0x00000,0x80000000,0x00000,0x80000000};
    static const XMVECTORF32 g_XMLoadUIcoN4Add = {0,32768.0f*65536.0f,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadUIcoN4Mul = {1.0f/1048575.0f,1.0f/(1048575.0f*4096.0f),1.0f/1048575.0f,1.0f/(15.0f*4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadUIcoN4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadUIcoN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadUIcoN4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadUIcoN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUIco4
(
    CONST XMUICO4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)(pSource->v & 0xFFFFF);
    V.v[1] = (FLOAT)((pSource->v >> 20) & 0xFFFFF);
    V.v[2] = (FLOAT)((pSource->v >> 40) & 0xFFFFF);
    V.v[3] = (FLOAT)(pSource->v >> 60);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadUIco4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadUIco4Xor = {0x00000,0x80000000,0x00000,0x80000000};
    static const XMVECTORF32 g_XMLoadUIco4Add = {0,32768.0f*65536.0f,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadUIco4Mul = {1.0f,1.0f/4096.0f,1.0f,1.0f/(4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadUIco4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadUIco4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadUIco4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadUIco4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadIcoN4
(
    CONST XMICON4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFF00000};
    static CONST UINT SignExtendW[] = {0x00000000, 0xFFFFFFF0};

    XMASSERT(pSource);

    Element = (UINT)pSource->v & 0xFFFFF;
    V.v[0] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    Element = (UINT)(pSource->v >> 20) & 0xFFFFF;
    V.v[1] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    Element = (UINT)(pSource->v >> 40) & 0xFFFFF;
    V.v[2] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]) / 524287.0f;
    Element = (UINT)(pSource->v >> 60);
    V.v[3] = (FLOAT)(INT)(Element | SignExtendW[Element >> 3]) / 7.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadIcoN4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadIcoN4Xor = {0x80000,0x00000000,0x80000,0x00000000};
    static const XMVECTORF32 g_XMLoadIcoN4Add = {-8.0f*65536.0f,0,-8.0f*65536.0f,0};
    static const XMVECTORF32 g_XMLoadIcoN4Mul = {1.0f/524287.0f,1.0f/(524287.0f*4096.0f),1.0f/524287.0f,1.0f/(7.0f*4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadIcoN4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadIcoN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadIcoN4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadIcoN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadIco4
(
    CONST XMICO4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFF00000};
    static CONST UINT SignExtendW[] = {0x00000000, 0xFFFFFFF0};

    XMASSERT(pSource);

    Element = (UINT)pSource->v & 0xFFFFF;
    V.v[0] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    Element = (UINT)(pSource->v >> 20) & 0xFFFFF;
    V.v[1] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    Element = (UINT)(pSource->v >> 40) & 0xFFFFF;
    V.v[2] = (FLOAT)(INT)(Element | SignExtend[Element >> 19]);
    Element = (UINT)(pSource->v >> 60);
    V.v[3] = (FLOAT)(INT)(Element | SignExtendW[Element >> 3]);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadIco4And = {0xFFFFF,0xFFFFF000,0xFFFFF,0xF0000000};
    static const XMVECTORI32 g_XMLoadIco4Xor = {0x80000,0x00000000,0x80000,0x00000000};
    static const XMVECTORF32 g_XMLoadIco4Add = {-8.0f*65536.0f,0,-8.0f*65536.0f,0};
    static const XMVECTORF32 g_XMLoadIco4Mul = {1.0f,1.0f/4096.0f,1.0f,1.0f/(4096.0f*65536.0f)};
	XMASSERT(pSource);
    // Grab the 64 bit structure
    __m128d vResultd = _mm_load_sd(reinterpret_cast<const double *>(&pSource->v));
    // By shifting down 8 bits, y and z are in seperate 32 bit elements
    __m128i vResulti = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vResultd)[0],8/8);
    // vResultd has x and w, vResulti has y and z, merge into one as x,w,y,z
    XMVECTOR vTemp = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResultd)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(1,0,1,0));
    // Fix the entries to x,y,z,w
    vTemp = _mm_shuffle_ps(vTemp,vTemp,_MM_SHUFFLE(1,3,2,0));
    // Mask x,y,z and w
    vTemp = _mm_and_ps(vTemp,g_XMLoadIco4And);
    // x and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadIco4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadIco4Add);
    // Fix y and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadIco4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}


//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadXDecN4
(
    CONST XMXDECN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMVECTOR V;
    UINT Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFFFFC00};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    V.v[3] = (FLOAT)(pSource->v >> 30) / 3.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    // Splat the color in all four entries
    __m128 vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMMaskA2B10G10R10);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMFlipA2B10G10R10);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixAA2B10G10R10);
    // Convert 0-255 to 0.0f-1.0f
    return _mm_mul_ps(vTemp,g_XMNormalizeA2B10G10R10);
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadXDec4
(
    CONST XMXDEC4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFFFFC00};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    V.v[3] = (FLOAT)(pSource->v >> 30);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);
    static const XMVECTORI32 g_XMXDec4And = {0x3FF, 0x3FF<<10, 0x3FF<<20, 0x3<<30};
    static const XMVECTORI32 g_XMXDec4Xor = {0x200, 0x200<<10, 0x200<<20, 0x80000000};
    static const XMVECTORF32 g_XMXDec4Add = {-512.0f,-512.0f*1024.0f,-512.0f*1024.0f*1024.0f,32768*65536.0f};
    static const XMVECTORF32 g_XMXDec4Mul = {1.0f,1.0f/1024.0f,1.0f/(1024.0f*1024.0f),1.0f/(1024.0f*1024.0f*1024.0f)};
	XMASSERT(pSource);
    // Splat the color in all four entries
    XMVECTOR vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMXDec4And);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMXDec4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMXDec4Add);
    // Convert 0-255 to 0.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMXDec4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUDecN4
(
    CONST XMUDECN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)Element / 1023.0f;
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)Element / 1023.0f;
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)Element / 1023.0f;
    V.v[3] = (FLOAT)(pSource->v >> 30) / 3.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    static const XMVECTORI32 g_XMUDecN4And = {0x3FF, 0x3FF<<10, 0x3FF<<20, 0x3<<30};
    static const XMVECTORI32 g_XMUDecN4Xor = {0, 0, 0, 0x80000000};
    static const XMVECTORF32 g_XMUDecN4Add = {0,0,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMUDecN4Mul = {1.0f/1023.0f,1.0f/(1023.0f*1024.0f),1.0f/(1023.0f*1024.0f*1024.0f),1.0f/(3.0f*1024.0f*1024.0f*1024.0f)};
    // Splat the color in all four entries
    XMVECTOR vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMUDecN4And);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMUDecN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMUDecN4Add);
    // Convert 0-255 to 0.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMUDecN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUDec4
(
    CONST XMUDEC4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;

    XMASSERT(pSource);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)Element;
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)Element;
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)Element;
    V.v[3] = (FLOAT)(pSource->v >> 30);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMUDec4And = {0x3FF, 0x3FF<<10, 0x3FF<<20, 0x3<<30};
    static const XMVECTORI32 g_XMUDec4Xor = {0, 0, 0,0x80000000};
    static const XMVECTORF32 g_XMUDec4Add = {0,0,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMUDec4Mul = {1.0f,1.0f/1024.0f,1.0f/(1024.0f*1024.0f),1.0f/(1024.0f*1024.0f*1024.0f)};
	XMASSERT(pSource);
    // Splat the color in all four entries
    XMVECTOR vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMUDec4And);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMUDec4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMUDec4Add);
    // Convert 0-255 to 0.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMUDec4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadDecN4
(
    CONST XMDECN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFFFFC00};
    static CONST UINT SignExtendW[] = {0x00000000, 0xFFFFFFFC};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 30) & 0x3) != 0x2);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]) / 511.0f;
    Element = pSource->v >> 30;
    V.v[3] = (FLOAT)(SHORT)(Element | SignExtendW[Element >> 1]);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 30) & 0x3) != 0x2);
    static const XMVECTORI32 g_XMDecN4And = {0x3FF, 0x3FF<<10, 0x3FF<<20, 0x3<<30};
    static const XMVECTORI32 g_XMDecN4Xor = {0x200, 0x200<<10, 0x200<<20, 0};
    static const XMVECTORF32 g_XMDecN4Add = {-512.0f,-512.0f*1024.0f,-512.0f*1024.0f*1024.0f,0};
    static const XMVECTORF32 g_XMDecN4Mul = {1.0f/511.0f,1.0f/(511.0f*1024.0f),1.0f/(511.0f*1024.0f*1024.0f),1.0f/(1024.0f*1024.0f*1024.0f)};
    // Splat the color in all four entries
    XMVECTOR vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMDecN4And);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMDecN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMDecN4Add);
    // Convert 0-255 to 0.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMDecN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadDec4
(
    CONST XMDEC4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR          V;
    UINT              Element;
    static CONST UINT SignExtend[] = {0x00000000, 0xFFFFFC00};
    static CONST UINT SignExtendW[] = {0x00000000, 0xFFFFFFFC};

    XMASSERT(pSource);
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 30) & 0x3) != 0x2);

    Element = pSource->v & 0x3FF;
    V.v[0] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    Element = (pSource->v >> 10) & 0x3FF;
    V.v[1] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    Element = (pSource->v >> 20) & 0x3FF;
    V.v[2] = (FLOAT)(SHORT)(Element | SignExtend[Element >> 9]);
    Element = pSource->v >> 30;
    V.v[3] = (FLOAT)(SHORT)(Element | SignExtendW[Element >> 1]);

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT((pSource->v & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 10) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 20) & 0x3FF) != 0x200);
    XMASSERT(((pSource->v >> 30) & 0x3) != 0x2);
    static const XMVECTORI32 g_XMDec4And = {0x3FF, 0x3FF<<10, 0x3FF<<20, 0x3<<30};
    static const XMVECTORI32 g_XMDec4Xor = {0x200, 0x200<<10, 0x200<<20, 0};
    static const XMVECTORF32 g_XMDec4Add = {-512.0f,-512.0f*1024.0f,-512.0f*1024.0f*1024.0f,0};
    static const XMVECTORF32 g_XMDec4Mul = {1.0f,1.0f/1024.0f,1.0f/(1024.0f*1024.0f),1.0f/(1024.0f*1024.0f*1024.0f)};
	XMASSERT(pSource);
    // Splat the color in all four entries
    XMVECTOR vTemp = _mm_load_ps1(reinterpret_cast<const float *>(&pSource->v));
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vTemp = _mm_and_ps(vTemp,g_XMDec4And);
    // a is unsigned! Flip the bit to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMDec4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMDec4Add);
    // Convert 0-255 to 0.0f-1.0f
    vTemp = _mm_mul_ps(vTemp,g_XMDec4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUByteN4
(
    CONST XMUBYTEN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x / 255.0f;
    V.v[1] = (FLOAT)pSource->y / 255.0f;
    V.v[2] = (FLOAT)pSource->z / 255.0f;
    V.v[3] = (FLOAT)pSource->w / 255.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadUByteN4And = {0xFF,0xFF00,0xFF0000,0xFF000000};
    static const XMVECTORI32 g_XMLoadUByteN4Xor = {0,0,0,0x80000000};
    static const XMVECTORF32 g_XMLoadUByteN4Add = {0,0,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadUByteN4Mul = {1.0f/255.0f,1.0f/(255.0f*256.0f),1.0f/(255.0f*65536.0f),1.0f/(255.0f*65536.0f*256.0f)};
	XMASSERT(pSource);
    // Splat the color in all four entries (x,z,y,w)
    XMVECTOR vTemp = _mm_load1_ps(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0ff,y&0xff00,z&0xff0000,w&0xff000000
    vTemp = _mm_and_ps(vTemp,g_XMLoadUByteN4And);
    // w is signed! Flip the bits to convert the order to unsigned
    vTemp = _mm_xor_ps(vTemp,g_XMLoadUByteN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // w + 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadUByteN4Add);
    // Fix y, z and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadUByteN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadUByte4
(
    CONST XMUBYTE4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;
    V.v[2] = (FLOAT)pSource->z;
    V.v[3] = (FLOAT)pSource->w;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadUByte4And = {0xFF,0xFF00,0xFF0000,0xFF000000};
    static const XMVECTORI32 g_XMLoadUByte4Xor = {0,0,0,0x80000000};
    static const XMVECTORF32 g_XMLoadUByte4Add = {0,0,0,32768.0f*65536.0f};
    static const XMVECTORF32 g_XMLoadUByte4Mul = {1.0f,1.0f/256.0f,1.0f/65536.0f,1.0f/(65536.0f*256.0f)};
	XMASSERT(pSource);
    // Splat the color in all four entries (x,z,y,w)
    XMVECTOR vTemp = _mm_load1_ps(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0ff,y&0xff00,z&0xff0000,w&0xff000000
    vTemp = _mm_and_ps(vTemp,g_XMLoadUByte4And);
    // w is signed! Flip the bits to convert the order to unsigned
    vTemp = _mm_xor_ps(vTemp,g_XMLoadUByte4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // w + 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadUByte4Add);
    // Fix y, z and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadUByte4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadByteN4
(
    CONST XMBYTEN4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(pSource->x != -128);
    XMASSERT(pSource->y != -128);
    XMASSERT(pSource->z != -128);
    XMASSERT(pSource->w != -128);

    V.v[0] = (FLOAT)pSource->x / 127.0f;
    V.v[1] = (FLOAT)pSource->y / 127.0f;
    V.v[2] = (FLOAT)pSource->z / 127.0f;
    V.v[3] = (FLOAT)pSource->w / 127.0f;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadByteN4And = {0xFF,0xFF00,0xFF0000,0xFF000000};
    static const XMVECTORI32 g_XMLoadByteN4Xor = {0x80,0x8000,0x800000,0x00000000};
    static const XMVECTORF32 g_XMLoadByteN4Add = {-128.0f,-128.0f*256.0f,-128.0f*65536.0f,0};
    static const XMVECTORF32 g_XMLoadByteN4Mul = {1.0f/127.0f,1.0f/(127.0f*256.0f),1.0f/(127.0f*65536.0f),1.0f/(127.0f*65536.0f*256.0f)};
	XMASSERT(pSource);
    XMASSERT(pSource->x != -128);
    XMASSERT(pSource->y != -128);
    XMASSERT(pSource->z != -128);
    XMASSERT(pSource->w != -128);
    // Splat the color in all four entries (x,z,y,w)
    XMVECTOR vTemp = _mm_load1_ps(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0ff,y&0xff00,z&0xff0000,w&0xff000000
    vTemp = _mm_and_ps(vTemp,g_XMLoadByteN4And);
    // x,y and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadByteN4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x, y and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadByteN4Add);
    // Fix y, z and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadByteN4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadByte4
(
    CONST XMBYTE4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR V;

    XMASSERT(pSource);
    XMASSERT(pSource->x != -128);
    XMASSERT(pSource->y != -128);
    XMASSERT(pSource->z != -128);
    XMASSERT(pSource->w != -128);

    V.v[0] = (FLOAT)pSource->x;
    V.v[1] = (FLOAT)pSource->y;
    V.v[2] = (FLOAT)pSource->z;
    V.v[3] = (FLOAT)pSource->w;

    return V;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORI32 g_XMLoadByte4And = {0xFF,0xFF00,0xFF0000,0xFF000000};
    static const XMVECTORI32 g_XMLoadByte4Xor = {0x80,0x8000,0x800000,0x00000000};
    static const XMVECTORF32 g_XMLoadByte4Add = {-128.0f,-128.0f*256.0f,-128.0f*65536.0f,0};
    static const XMVECTORF32 g_XMLoadByte4Mul = {1.0f,1.0f/256.0f,1.0f/65536.0f,1.0f/(65536.0f*256.0f)};
	XMASSERT(pSource);
    XMASSERT(pSource->x != -128);
    XMASSERT(pSource->y != -128);
    XMASSERT(pSource->z != -128);
    XMASSERT(pSource->w != -128);
    // Splat the color in all four entries (x,z,y,w)
    XMVECTOR vTemp = _mm_load1_ps(reinterpret_cast<const float *>(&pSource->x));
    // Mask x&0ff,y&0xff00,z&0xff0000,w&0xff000000
    vTemp = _mm_and_ps(vTemp,g_XMLoadByte4And);
    // x,y and z are unsigned! Flip the bits to convert the order to signed
    vTemp = _mm_xor_ps(vTemp,g_XMLoadByte4Xor);
    // Convert to floating point numbers
    vTemp = _mm_cvtepi32_ps(reinterpret_cast<const __m128i *>(&vTemp)[0]);
    // x, y and z - 0x80 to complete the conversion
    vTemp = _mm_add_ps(vTemp,g_XMLoadByte4Add);
    // Fix y, z and w because they are too large
    vTemp = _mm_mul_ps(vTemp,g_XMLoadByte4Mul);
    return vTemp;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMVECTOR XMLoadColor
(
    CONST XMCOLOR* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMASSERT(pSource);
    {
    // INT -> Float conversions are done in one instruction.
    // UINT -> Float calls a runtime function. Keep in INT
    INT iColor = (INT)(pSource->c);
    XMVECTOR vColor = {
        (FLOAT)((iColor >> 16) & 0xFF) * (1.0f/255.0f),
        (FLOAT)((iColor >> 8) & 0xFF) * (1.0f/255.0f),
        (FLOAT)(iColor & 0xFF) * (1.0f/255.0f),
        (FLOAT)((iColor >> 24) & 0xFF) * (1.0f/255.0f)
    };
    return vColor;
    }
#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    // Splat the color in all four entries
    __m128i vInt = _mm_set1_epi32(pSource->c);
    // Shift R&0xFF0000, G&0xFF00, B&0xFF, A&0xFF000000
    vInt = _mm_and_si128(vInt,g_XMMaskA8R8G8B8);
    // a is unsigned! Flip the bit to convert the order to signed
    vInt = _mm_xor_si128(vInt,g_XMFlipA8R8G8B8);
    // Convert to floating point numbers
    XMVECTOR vTemp = _mm_cvtepi32_ps(vInt);
    // RGB + 0, A + 0x80000000.f to undo the signed order.
    vTemp = _mm_add_ps(vTemp,g_XMFixAA8R8G8B8);
    // Convert 0-255 to 0.0f-1.0f
    return _mm_mul_ps(vTemp,g_XMNormalizeA8R8G8B8);
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMMATRIX XMLoadFloat3x3
(
    CONST XMFLOAT3X3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMMATRIX M;

    XMASSERT(pSource);

    M.r[0].v[0] = pSource->m[0][0];
    M.r[0].v[1] = pSource->m[0][1];
    M.r[0].v[2] = pSource->m[0][2];
    M.r[0].v[3] = 0.0f;

    M.r[1].v[0] = pSource->m[1][0];
    M.r[1].v[1] = pSource->m[1][1];
    M.r[1].v[2] = pSource->m[1][2];
    M.r[1].v[3] = 0.0f;

    M.r[2].v[0] = pSource->m[2][0];
    M.r[2].v[1] = pSource->m[2][1];
    M.r[2].v[2] = pSource->m[2][2];
    M.r[2].v[3] = 0.0f;

    M.r[3].v[0] = 0.0f;
    M.r[3].v[1] = 0.0f;
    M.r[3].v[2] = 0.0f;
    M.r[3].v[3] = 1.0f;

    return M;

#elif defined(_XM_SSE_INTRINSICS_)
	XMMATRIX M;
	XMVECTOR V1, V2, V3, Z, T1, T2, T3, T4, T5;

	Z = _mm_setzero_ps();

	XMASSERT(pSource);

	V1 = _mm_loadu_ps( &pSource->m[0][0] );
	V2 = _mm_loadu_ps( &pSource->m[1][1] );
	V3 = _mm_load_ss( &pSource->m[2][2] );

	T1 = _mm_unpackhi_ps( V1, Z );
	T2 = _mm_unpacklo_ps( V2, Z );
	T3 = _mm_shuffle_ps( V3, T2, _MM_SHUFFLE( 0, 1, 0, 0 ) );
	T4 = _mm_movehl_ps( T2, T3 );
	T5 = _mm_movehl_ps( Z, T1 );  

	M.r[0] = _mm_movelh_ps( V1, T1 );
	M.r[1] = _mm_add_ps( T4, T5 );
	M.r[2] = _mm_shuffle_ps( V2, V3, _MM_SHUFFLE(1, 0, 3, 2) );
	M.r[3] = g_XMIdentityR3;

	return M;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMMATRIX XMLoadFloat4x3
(
    CONST XMFLOAT4X3* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMMATRIX M;
    XMASSERT(pSource);

    ((UINT *)(&M.r[0].v[0]))[0] = ((const UINT *)(&pSource->m[0][0]))[0];
    ((UINT *)(&M.r[0].v[1]))[0] = ((const UINT *)(&pSource->m[0][1]))[0];
    ((UINT *)(&M.r[0].v[2]))[0] = ((const UINT *)(&pSource->m[0][2]))[0];
    M.r[0].v[3] = 0.0f;

    ((UINT *)(&M.r[1].v[0]))[0] = ((const UINT *)(&pSource->m[1][0]))[0];
    ((UINT *)(&M.r[1].v[1]))[0] = ((const UINT *)(&pSource->m[1][1]))[0];
    ((UINT *)(&M.r[1].v[2]))[0] = ((const UINT *)(&pSource->m[1][2]))[0];
    M.r[1].v[3] = 0.0f;

    ((UINT *)(&M.r[2].v[0]))[0] = ((const UINT *)(&pSource->m[2][0]))[0];
    ((UINT *)(&M.r[2].v[1]))[0] = ((const UINT *)(&pSource->m[2][1]))[0];
    ((UINT *)(&M.r[2].v[2]))[0] = ((const UINT *)(&pSource->m[2][2]))[0];
    M.r[2].v[3] = 0.0f;

    ((UINT *)(&M.r[3].v[0]))[0] = ((const UINT *)(&pSource->m[3][0]))[0];
    ((UINT *)(&M.r[3].v[1]))[0] = ((const UINT *)(&pSource->m[3][1]))[0];
    ((UINT *)(&M.r[3].v[2]))[0] = ((const UINT *)(&pSource->m[3][2]))[0];
    M.r[3].v[3] = 1.0f;

    return M;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    // Use unaligned load instructions to 
    // load the 12 floats
    // vTemp1 = x1,y1,z1,x2
    XMVECTOR vTemp1 = _mm_loadu_ps(&pSource->m[0][0]);
    // vTemp2 = y2,z2,x3,y3
    XMVECTOR vTemp2 = _mm_loadu_ps(&pSource->m[1][1]);
    // vTemp4 = z3,x4,y4,z4
    XMVECTOR vTemp4 = _mm_loadu_ps(&pSource->m[2][2]);
    // vTemp3 = x3,y3,z3,z3
    XMVECTOR vTemp3 = _mm_shuffle_ps(vTemp2,vTemp4,_MM_SHUFFLE(0,0,3,2));
    // vTemp2 = y2,z2,x2,x2
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp1,_MM_SHUFFLE(3,3,1,0));
    // vTemp2 = x2,y2,z2,z2
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp2,_MM_SHUFFLE(1,1,0,2));
    // vTemp1 = x1,y1,z1,0
    vTemp1 = _mm_and_ps(vTemp1,g_XMMask3);
    // vTemp2 = x2,y2,z2,0
    vTemp2 = _mm_and_ps(vTemp2,g_XMMask3);
    // vTemp3 = x3,y3,z3,0
    vTemp3 = _mm_and_ps(vTemp3,g_XMMask3);
    // vTemp4i = x4,y4,z4,0
    __m128i vTemp4i = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vTemp4)[0],32/8);
    // vTemp4i = x4,y4,z4,1.0f
    vTemp4i = _mm_or_si128(vTemp4i,g_XMIdentityR3);
    XMMATRIX M(vTemp1,
            vTemp2,
            vTemp3,
            reinterpret_cast<const __m128 *>(&vTemp4i)[0]);
    return M;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMMATRIX XMLoadFloat4x3A
(
    CONST XMFLOAT4X3A* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMMATRIX M;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    M.r[0].v[0] = pSource->m[0][0];
    M.r[0].v[1] = pSource->m[0][1];
    M.r[0].v[2] = pSource->m[0][2];
    M.r[0].v[3] = 0.0f;

    M.r[1].v[0] = pSource->m[1][0];
    M.r[1].v[1] = pSource->m[1][1];
    M.r[1].v[2] = pSource->m[1][2];
    M.r[1].v[3] = 0.0f;

    M.r[2].v[0] = pSource->m[2][0];
    M.r[2].v[1] = pSource->m[2][1];
    M.r[2].v[2] = pSource->m[2][2];
    M.r[2].v[3] = 0.0f;

    M.r[3].v[0] = pSource->m[3][0];
    M.r[3].v[1] = pSource->m[3][1];
    M.r[3].v[2] = pSource->m[3][2];
    M.r[3].v[3] = 1.0f;

    return M;

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pSource);
    // Use aligned load instructions to 
    // load the 12 floats
    // vTemp1 = x1,y1,z1,x2
    XMVECTOR vTemp1 = _mm_load_ps(&pSource->m[0][0]);
    // vTemp2 = y2,z2,x3,y3
    XMVECTOR vTemp2 = _mm_load_ps(&pSource->m[1][1]);
    // vTemp4 = z3,x4,y4,z4
    XMVECTOR vTemp4 = _mm_load_ps(&pSource->m[2][2]);
    // vTemp3 = x3,y3,z3,z3
    XMVECTOR vTemp3 = _mm_shuffle_ps(vTemp2,vTemp4,_MM_SHUFFLE(0,0,3,2));
    // vTemp2 = y2,z2,x2,x2
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp1,_MM_SHUFFLE(3,3,1,0));
    // vTemp2 = x2,y2,z2,z2
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp2,_MM_SHUFFLE(1,1,0,2));
    // vTemp1 = x1,y1,z1,0
    vTemp1 = _mm_and_ps(vTemp1,g_XMMask3);
    // vTemp2 = x2,y2,z2,0
    vTemp2 = _mm_and_ps(vTemp2,g_XMMask3);
    // vTemp3 = x3,y3,z3,0
    vTemp3 = _mm_and_ps(vTemp3,g_XMMask3);
    // vTemp4i = x4,y4,z4,0
    __m128i vTemp4i = _mm_srli_si128(reinterpret_cast<const __m128i *>(&vTemp4)[0],32/8);
    // vTemp4i = x4,y4,z4,1.0f
    vTemp4i = _mm_or_si128(vTemp4i,g_XMIdentityR3);
    XMMATRIX M(vTemp1,
            vTemp2,
            vTemp3,
            reinterpret_cast<const __m128 *>(&vTemp4i)[0]);
    return M;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMMATRIX XMLoadFloat4x4
(
    CONST XMFLOAT4X4* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMMATRIX M;
    XMASSERT(pSource);

    ((UINT *)(&M.r[0].v[0]))[0] = ((const UINT *)(&pSource->m[0][0]))[0];
    ((UINT *)(&M.r[0].v[1]))[0] = ((const UINT *)(&pSource->m[0][1]))[0];
    ((UINT *)(&M.r[0].v[2]))[0] = ((const UINT *)(&pSource->m[0][2]))[0];
    ((UINT *)(&M.r[0].v[3]))[0] = ((const UINT *)(&pSource->m[0][3]))[0];

    ((UINT *)(&M.r[1].v[0]))[0] = ((const UINT *)(&pSource->m[1][0]))[0];
    ((UINT *)(&M.r[1].v[1]))[0] = ((const UINT *)(&pSource->m[1][1]))[0];
    ((UINT *)(&M.r[1].v[2]))[0] = ((const UINT *)(&pSource->m[1][2]))[0];
    ((UINT *)(&M.r[1].v[3]))[0] = ((const UINT *)(&pSource->m[1][3]))[0];

    ((UINT *)(&M.r[2].v[0]))[0] = ((const UINT *)(&pSource->m[2][0]))[0];
    ((UINT *)(&M.r[2].v[1]))[0] = ((const UINT *)(&pSource->m[2][1]))[0];
    ((UINT *)(&M.r[2].v[2]))[0] = ((const UINT *)(&pSource->m[2][2]))[0];
    ((UINT *)(&M.r[2].v[3]))[0] = ((const UINT *)(&pSource->m[2][3]))[0];

    ((UINT *)(&M.r[3].v[0]))[0] = ((const UINT *)(&pSource->m[3][0]))[0];
    ((UINT *)(&M.r[3].v[1]))[0] = ((const UINT *)(&pSource->m[3][1]))[0];
    ((UINT *)(&M.r[3].v[2]))[0] = ((const UINT *)(&pSource->m[3][2]))[0];
    ((UINT *)(&M.r[3].v[3]))[0] = ((const UINT *)(&pSource->m[3][3]))[0];

    return M;

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pSource);
    XMMATRIX M;

    M.r[0] = _mm_loadu_ps( &pSource->_11 );
    M.r[1] = _mm_loadu_ps( &pSource->_21 );
    M.r[2] = _mm_loadu_ps( &pSource->_31 );
    M.r[3] = _mm_loadu_ps( &pSource->_41 );

    return M;
#elif defined(XM_NO_MISALIGNED_VECTOR_ACCESS)
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE XMMATRIX XMLoadFloat4x4A
(
    CONST XMFLOAT4X4A* pSource
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMMATRIX M;

    XMASSERT(pSource);
    XMASSERT(((UINT_PTR)pSource & 0xF) == 0);

    M.r[0].v[0] = pSource->m[0][0];
    M.r[0].v[1] = pSource->m[0][1];
    M.r[0].v[2] = pSource->m[0][2];
    M.r[0].v[3] = pSource->m[0][3];

    M.r[1].v[0] = pSource->m[1][0];
    M.r[1].v[1] = pSource->m[1][1];
    M.r[1].v[2] = pSource->m[1][2];
    M.r[1].v[3] = pSource->m[1][3];

    M.r[2].v[0] = pSource->m[2][0];
    M.r[2].v[1] = pSource->m[2][1];
    M.r[2].v[2] = pSource->m[2][2];
    M.r[2].v[3] = pSource->m[2][3];

    M.r[3].v[0] = pSource->m[3][0];
    M.r[3].v[1] = pSource->m[3][1];
    M.r[3].v[2] = pSource->m[3][2];
    M.r[3].v[3] = pSource->m[3][3];

    return M;

#elif defined(_XM_SSE_INTRINSICS_)
	XMMATRIX M;

	XMASSERT(pSource);

	M.r[0] = _mm_load_ps( &pSource->_11 );
	M.r[1] = _mm_load_ps( &pSource->_21 );
	M.r[2] = _mm_load_ps( &pSource->_31 );
	M.r[3] = _mm_load_ps( &pSource->_41 );

	return M;
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

/****************************************************************************
 *
 * Vector and matrix store operations
 *
 ****************************************************************************/

XMFINLINE VOID XMStoreInt
(
    UINT*    pDestination,
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    *pDestination = XMVectorGetIntX( V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat
(
    FLOAT*    pDestination,
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    *pDestination = XMVectorGetX( V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt2
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);
    pDestination[0] = XMVectorGetIntX( V );
    pDestination[1] = XMVectorGetIntY( V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt2A
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    _mm_storel_epi64( (__m128i*)pDestination, reinterpret_cast<const __m128i *>(&V)[0] );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat2
(
    XMFLOAT2* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

	XMVECTOR T = _mm_shuffle_ps( V, V, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	_mm_store_ss( &pDestination->x, V );
	_mm_store_ss( &pDestination->y, T );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat2A
(
    XMFLOAT2A*   pDestination, 
    FXMVECTOR     V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

	XMVECTOR T = _mm_shuffle_ps( V, V, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	_mm_store_ss( &pDestination->x, V );
	_mm_store_ss( &pDestination->y, T );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreHalf2
(
    XMHALF2* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->x = XMConvertFloatToHalf(V.v[0]);
    pDestination->y = XMConvertFloatToHalf(V.v[1]);

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    pDestination->x = XMConvertFloatToHalf(XMVectorGetX(V));
    pDestination->y = XMConvertFloatToHalf(XMVectorGetY(V));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreShortN2
(
    XMSHORTN2* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR N;
    static CONST XMVECTORF32  Scale = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static CONST XMVECTORF32 Scale = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

	XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
	vResult = _mm_min_ps(vResult,g_XMOne);
    vResult = _mm_mul_ps(vResult,Scale);
    __m128i vResulti = _mm_cvtps_epi32(vResult);
    vResulti = _mm_packs_epi32(vResulti,vResulti);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->x),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreShort2
(
    XMSHORT2* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-32767.0f, -32767.0f, -32767.0f, -32767.0f};
    static CONST XMVECTOR  Max = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static CONST XMVECTORF32 Min = {-32767.0f, -32767.0f, -32767.0f, -32767.0f};
    static CONST XMVECTORF32 Max = {32767.0f, 32767.0f, 32767.0f, 32767.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,Min);
    vResult = _mm_min_ps(vResult,Max);
     // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Pack the ints into shorts
    vInt = _mm_packs_epi32(vInt,vInt);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->x),reinterpret_cast<const __m128 *>(&vInt)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUShortN2
(
    XMUSHORTN2* pDestination, 
    FXMVECTOR    V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {65535.0f, 65535.0f, 65535.0f, 65535.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiplyAdd(N, Scale.v, g_XMOneHalf.v);
    N = XMVectorTruncate(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static CONST XMVECTORF32 Scale = {65535.0f, 65535.0f, 65535.0f, 65535.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    vResult = _mm_mul_ps(vResult,Scale);
     // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Since the SSE pack instruction clamps using signed rules,
    // manually extract the values to store them to memory
    pDestination->x = static_cast<SHORT>(_mm_extract_epi16(vInt,0));
    pDestination->y = static_cast<SHORT>(_mm_extract_epi16(vInt,2));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUShort2
(
    XMUSHORT2* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {65535.0f, 65535.0f, 65535.0f, 65535.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static CONST XMVECTORF32  Max = {65535.0f, 65535.0f, 65535.0f, 65535.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,Max);
     // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Since the SSE pack instruction clamps using signed rules,
    // manually extract the values to store them to memory
    pDestination->x = static_cast<SHORT>(_mm_extract_epi16(vInt,0));
    pDestination->y = static_cast<SHORT>(_mm_extract_epi16(vInt,2));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt3
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];
    pDestination[2] = V.u[2];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);
    pDestination[0] = XMVectorGetIntX( V );
    pDestination[1] = XMVectorGetIntY( V );
    pDestination[2] = XMVectorGetIntZ( V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt3A
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];
    pDestination[2] = V.u[2];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);
    pDestination[0] = XMVectorGetIntX( V );
    pDestination[1] = XMVectorGetIntY( V );
    pDestination[2] = XMVectorGetIntZ( V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat3
(
    XMFLOAT3* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];
    pDestination->z = V.v[2];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 3) == 0);

	XMVECTOR T1 = _mm_shuffle_ps(V,V,_MM_SHUFFLE(1,1,1,1));
	XMVECTOR T2 = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,2,2,2));
	_mm_store_ss( &pDestination->x, V );
	_mm_store_ss( &pDestination->y, T1 );
	_mm_store_ss( &pDestination->z, T2 );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat3A
(
    XMFLOAT3A*   pDestination, 
    FXMVECTOR     V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];
    pDestination->z = V.v[2];

#elif defined(_XM_SSE_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

	XMVECTOR T1 = _mm_shuffle_ps( V, V, _MM_SHUFFLE( 1, 1, 1, 1 ) );
	XMVECTOR T2 = _mm_unpackhi_ps( V, V );
	_mm_store_ss( &pDestination->x, V );
	_mm_store_ss( &pDestination->y, T1 );
	_mm_store_ss( &pDestination->z, T2 );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUHenDN3
(
    XMUHENDN3* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {2047.0f, 2047.0f, 1023.0f, 0.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = (((UINT)N.v[2] & 0x3FF) << 22) |
                      (((UINT)N.v[1] & 0x7FF) << 11) |
                      (((UINT)N.v[0] & 0x7FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleUHenDN3 = {2047.0f, 2047.0f*2048.0f,1023.0f*(2048.0f*2048.0f)/2.0f,1.0f};
    static const XMVECTORI32 MaskUHenDN3 = {0x7FF,0x7FF<<11,0x3FF<<(22-1),0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUHenDN3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUHenDN3);
    // Do a horizontal or of 3 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(0,3,2,1));
    // i = x|y
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti2,_MM_SHUFFLE(0,3,2,1));
    // Add Z to itself to perform a single bit left shift
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUHenD3
(
    XMUHEND3* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {2047.0f, 2047.0f, 1023.0f, 0.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);

    pDestination->v = (((UINT)N.v[2] & 0x3FF) << 22) |
                      (((UINT)N.v[1] & 0x7FF) << 11) |
                      (((UINT)N.v[0] & 0x7FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MaxUHenD3 = { 2047.0f, 2047.0f, 1023.0f, 1.0f};
    static const XMVECTORF32 ScaleUHenD3 = {1.0f, 2048.0f,(2048.0f*2048.0f)/2.0f,1.0f};
    static const XMVECTORI32 MaskUHenD3 = {0x7FF,0x7FF<<11,0x3FF<<(22-1),0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,MaxUHenD3);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUHenD3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUHenD3);
    // Do a horizontal or of 3 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(0,3,2,1));
    // i = x|y
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti2,_MM_SHUFFLE(0,3,2,1));
    // Add Z to itself to perform a single bit left shift
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreHenDN3
(
    XMHENDN3* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {1023.0f, 1023.0f, 511.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = (((INT)N.v[2] & 0x3FF) << 22) |
                      (((INT)N.v[1] & 0x7FF) << 11) |
                      (((INT)N.v[0] & 0x7FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleHenDN3 = {1023.0f, 1023.0f*2048.0f,511.0f*(2048.0f*2048.0f),1.0f};
    static const XMVECTORI32 MaskHenDN3 = {0x7FF,0x7FF<<11,0x3FF<<22,0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleHenDN3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskHenDN3);
    // Do a horizontal or of all 4 entries
    vResult = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreHenD3
(
    XMHEND3* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-1023.0f, -1023.0f, -511.0f, -1.0f};
    static CONST XMVECTOR  Max = {1023.0f, 1023.0f, 511.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);

    pDestination->v = (((INT)N.v[2] & 0x3FF) << 22) |
                      (((INT)N.v[1] & 0x7FF) << 11) |
                      (((INT)N.v[0] & 0x7FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MinHenD3 = {-1023.0f,-1023.0f,-511.0f,-1.0f};
    static const XMVECTORF32 MaxHenD3 = { 1023.0f, 1023.0f, 511.0f, 1.0f};
    static const XMVECTORF32 ScaleHenD3 = {1.0f, 2048.0f,(2048.0f*2048.0f),1.0f};
    static const XMVECTORI32 MaskHenD3 = {0x7FF,0x7FF<<11,0x3FF<<22,0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,MinHenD3);
    vResult = _mm_min_ps(vResult,MaxHenD3);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleHenD3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskHenD3);
    // Do a horizontal or of all 4 entries
    vResult = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUDHenN3
(
    XMUDHENN3* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {1023.0f, 2047.0f, 2047.0f, 0.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = (((UINT)N.v[2] & 0x7FF) << 21) |
                      (((UINT)N.v[1] & 0x7FF) << 10) |
                      (((UINT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleUDHenN3 = {1023.0f,2047.0f*1024.0f,2047.0f*(1024.0f*2048.0f)/2.0f,1.0f};
    static const XMVECTORI32 MaskUDHenN3 = {0x3FF,0x7FF<<10,0x7FF<<(21-1),0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUDHenN3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUDHenN3);
    // Do a horizontal or of 3 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(0,3,2,1));
    // i = x|y
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti2,_MM_SHUFFLE(0,3,2,1));
    // Add Z to itself to perform a single bit left shift
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUDHen3
(
    XMUDHEN3* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {1023.0f, 2047.0f, 2047.0f, 0.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);

    pDestination->v = (((UINT)N.v[2] & 0x7FF) << 21) |
                      (((UINT)N.v[1] & 0x7FF) << 10) |
                      (((UINT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MaxUDHen3 = { 1023.0f, 2047.0f, 2047.0f, 1.0f};
    static const XMVECTORF32 ScaleUDHen3 = {1.0f, 1024.0f,(1024.0f*2048.0f)/2.0f,1.0f};
    static const XMVECTORI32 MaskUDHen3 = {0x3FF,0x7FF<<10,0x7FF<<(21-1),0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,MaxUDHen3);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUDHen3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUDHen3);
    // Do a horizontal or of 3 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(0,3,2,1));
    // i = x|y
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti2,_MM_SHUFFLE(0,3,2,1));
    // Add Z to itself to perform a single bit left shift
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreDHenN3
(
    XMDHENN3* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {511.0f, 1023.0f, 1023.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = (((INT)N.v[2] & 0x7FF) << 21) |
                      (((INT)N.v[1] & 0x7FF) << 10) |
                      (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleDHenN3 = {511.0f, 1023.0f*1024.0f,1023.0f*(1024.0f*2048.0f),1.0f};
    static const XMVECTORI32 MaskDHenN3 = {0x3FF,0x7FF<<10,0x7FF<<21,0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleDHenN3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskDHenN3);
    // Do a horizontal or of all 4 entries
    vResult = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreDHen3
(
    XMDHEN3* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-511.0f, -1023.0f, -1023.0f, -1.0f};
    static CONST XMVECTOR  Max = {511.0f, 1023.0f, 1023.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);

    pDestination->v = (((INT)N.v[2] & 0x7FF) << 21) |
                      (((INT)N.v[1] & 0x7FF) << 10) |
                      (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MinDHen3 = {-511.0f,-1023.0f,-1023.0f,-1.0f};
    static const XMVECTORF32 MaxDHen3 = { 511.0f, 1023.0f, 1023.0f, 1.0f};
    static const XMVECTORF32 ScaleDHen3 = {1.0f, 1024.0f,(1024.0f*2048.0f),1.0f};
    static const XMVECTORI32 MaskDHen3 = {0x3FF,0x7FF<<10,0x7FF<<21,0};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,MinDHen3);
    vResult = _mm_min_ps(vResult,MaxDHen3);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleDHen3);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskDHen3);
    // Do a horizontal or of all 4 entries
    vResult = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt4
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];
    pDestination[2] = V.u[2];
    pDestination[3] = V.u[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);

    _mm_storeu_si128( (__m128i*)pDestination, reinterpret_cast<const __m128i *>(&V)[0] );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt4A
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];
    pDestination[2] = V.u[2];
    pDestination[3] = V.u[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);

    _mm_store_si128( (__m128i*)pDestination, reinterpret_cast<const __m128i *>(&V)[0] );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreInt4NC
(
    UINT*    pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination[0] = V.u[0];
    pDestination[1] = V.u[1];
    pDestination[2] = V.u[2];
    pDestination[3] = V.u[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);

    _mm_storeu_si128( (__m128i*)pDestination, reinterpret_cast<const __m128i *>(&V)[0] );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4
(
    XMFLOAT4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];
    pDestination->z = V.v[2];
    pDestination->w = V.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);

    _mm_storeu_ps( &pDestination->x, V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4A
(
    XMFLOAT4A*   pDestination, 
    FXMVECTOR     V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];
    pDestination->z = V.v[2];
    pDestination->w = V.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    _mm_store_ps( &pDestination->x, V );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4NC
(
    XMFLOAT4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->x = V.v[0];
    pDestination->y = V.v[1];
    pDestination->z = V.v[2];
    pDestination->w = V.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);

    _mm_storeu_ps( &pDestination->x, V );

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreHalf4
(
    XMHALF4* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_) 

    XMASSERT(pDestination);

    pDestination->x = XMConvertFloatToHalf(V.v[0]);
    pDestination->y = XMConvertFloatToHalf(V.v[1]);
    pDestination->z = XMConvertFloatToHalf(V.v[2]);
    pDestination->w = XMConvertFloatToHalf(V.v[3]);

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    pDestination->x = XMConvertFloatToHalf(XMVectorGetX(V));
    pDestination->y = XMConvertFloatToHalf(XMVectorGetY(V));
    pDestination->z = XMConvertFloatToHalf(XMVectorGetZ(V));
    pDestination->w = XMConvertFloatToHalf(XMVectorGetW(V));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreShortN4
(
    XMSHORTN4* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];
    pDestination->z = (SHORT)N.v[2];
    pDestination->w = (SHORT)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    static CONST XMVECTORF32 Scale = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

    XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    vResult = _mm_mul_ps(vResult,Scale);
    __m128i vResulti = _mm_cvtps_epi32(vResult);
    vResulti = _mm_packs_epi32(vResulti,vResulti);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->x),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreShort4
(
    XMSHORT4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-32767.0f, -32767.0f, -32767.0f, -32767.0f};
    static CONST XMVECTOR  Max = {32767.0f, 32767.0f, 32767.0f, 32767.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];
    pDestination->z = (SHORT)N.v[2];
    pDestination->w = (SHORT)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    static CONST XMVECTORF32 Min = {-32767.0f, -32767.0f, -32767.0f, -32767.0f};
    static CONST XMVECTORF32  Max = {32767.0f, 32767.0f, 32767.0f, 32767.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,Min);
    vResult = _mm_min_ps(vResult,Max);
     // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Pack the ints into shorts
    vInt = _mm_packs_epi32(vInt,vInt);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->x),reinterpret_cast<const __m128d *>(&vInt)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUShortN4
(
    XMUSHORTN4* pDestination, 
    FXMVECTOR    V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {65535.0f, 65535.0f, 65535.0f, 65535.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiplyAdd(N, Scale.v, g_XMOneHalf.v);
    N = XMVectorTruncate(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];
    pDestination->z = (SHORT)N.v[2];
    pDestination->w = (SHORT)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    static CONST XMVECTORF32 Scale = {65535.0f, 65535.0f, 65535.0f, 65535.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    vResult = _mm_mul_ps(vResult,Scale);
    // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Since the SSE pack instruction clamps using signed rules,
    // manually extract the values to store them to memory
    pDestination->x = static_cast<SHORT>(_mm_extract_epi16(vInt,0));
    pDestination->y = static_cast<SHORT>(_mm_extract_epi16(vInt,2));
    pDestination->z = static_cast<SHORT>(_mm_extract_epi16(vInt,4));
    pDestination->w = static_cast<SHORT>(_mm_extract_epi16(vInt,6));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUShort4
(
    XMUSHORT4* pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {65535.0f, 65535.0f, 65535.0f, 65535.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);
    N = XMVectorRound(N);

    pDestination->x = (SHORT)N.v[0];
    pDestination->y = (SHORT)N.v[1];
    pDestination->z = (SHORT)N.v[2];
    pDestination->w = (SHORT)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    static CONST XMVECTORF32  Max = {65535.0f, 65535.0f, 65535.0f, 65535.0f};
    // Bounds check
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,Max);
     // Convert to int with rounding
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Since the SSE pack instruction clamps using signed rules,
    // manually extract the values to store them to memory
    pDestination->x = static_cast<SHORT>(_mm_extract_epi16(vInt,0));
    pDestination->y = static_cast<SHORT>(_mm_extract_epi16(vInt,2));
    pDestination->z = static_cast<SHORT>(_mm_extract_epi16(vInt,4));
    pDestination->w = static_cast<SHORT>(_mm_extract_epi16(vInt,6));
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreXIcoN4
(
    XMXICON4*  pDestination, 
    FXMVECTOR   V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Min = {-1.0f, -1.0f, -1.0f, 0.0f};
    static CONST XMVECTORF32  Scale = {524287.0f, 524287.0f, 524287.0f, 15.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->v = ((UINT64)N.v[3] << 60) |
                       (((INT64)N.v[2] & 0xFFFFF) << 40) |
                       (((INT64)N.v[1] & 0xFFFFF) << 20) |
                       (((INT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 MinXIcoN4 = {-1.0f, 0.0f,-1.0f,-1.0f};
    static const XMVECTORF32 ScaleXIcoN4 = {524287.0f,15.0f*4096.0f*65536.0f*0.5f,524287.0f*4096.0f,524287.0f};
    static const XMVECTORI32 MaskXIcoN4 = {0xFFFFF,0xF<<((60-32)-1),0xFFFFF000,0xFFFFF};

    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,MinXIcoN4);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleXIcoN4);
    // Convert to integer (w is unsigned)
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off unused bits
    vResulti = _mm_and_si128(vResulti,MaskXIcoN4);
    // Isolate Y
    __m128i vResulti2 = _mm_and_si128(vResulti,g_XMMaskY);
    // Double Y (Really W) to fixup for unsigned conversion
    vResulti = _mm_add_epi32(vResulti,vResulti2);
    // Shift y and z to straddle the 32-bit boundary
    vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreXIco4
(
    XMXICO4*  pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR N;
    static CONST XMVECTORF32 Min = {-524287.0f, -524287.0f, -524287.0f, 0.0f};
    static CONST XMVECTORF32 Max = {524287.0f, 524287.0f, 524287.0f, 15.0f};

    XMASSERT(pDestination);
    N = XMVectorClamp(V, Min.v, Max.v);
    pDestination->v = ((UINT64)N.v[3] << 60) |
                       (((INT64)N.v[2] & 0xFFFFF) << 40) |
                       (((INT64)N.v[1] & 0xFFFFF) << 20) |
                       (((INT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 MinXIco4 = {-524287.0f, 0.0f,-524287.0f,-524287.0f};
    static const XMVECTORF32 MaxXIco4 = { 524287.0f,15.0f, 524287.0f, 524287.0f};
    static const XMVECTORF32 ScaleXIco4 = {1.0f,4096.0f*65536.0f*0.5f,4096.0f,1.0f};
    static const XMVECTORI32 MaskXIco4 = {0xFFFFF,0xF<<((60-1)-32),0xFFFFF000,0xFFFFF};
    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,MinXIco4);
    vResult = _mm_min_ps(vResult,MaxXIco4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleXIco4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskXIco4);
    // Isolate Y
    __m128i vResulti2 = _mm_and_si128(vResulti,g_XMMaskY);
    // Double Y (Really W) to fixup for unsigned conversion
    vResulti = _mm_add_epi32(vResulti,vResulti2);
    // Shift y and z to straddle the 32-bit boundary
    vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUIcoN4
(
    XMUICON4*  pDestination, 
    FXMVECTOR   V
)
{
    #define XM_URange       ((FLOAT)(1 << 20))
    #define XM_URangeDiv2   ((FLOAT)(1 << 19))
    #define XM_UMaxXYZ      ((FLOAT)((1 << 20) - 1))
    #define XM_UMaxW        ((FLOAT)((1 << 4) - 1))
    #define XM_ScaleXYZ     (-(FLOAT)((1 << 20) - 1) / XM_PACK_FACTOR)
    #define XM_ScaleW       (-(FLOAT)((1 << 4) - 1) / XM_PACK_FACTOR)
    #define XM_Scale        (-1.0f / XM_PACK_FACTOR)
    #define XM_Offset       (3.0f)

#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32 Scale = {1048575.0f, 1048575.0f, 1048575.0f, 15.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiplyAdd(N, Scale.v, g_XMOneHalf.v);

    pDestination->v = ((UINT64)N.v[3] << 60) |
                       (((UINT64)N.v[2] & 0xFFFFF) << 40) |
                       (((UINT64)N.v[1] & 0xFFFFF) << 20) |
                       (((UINT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 ScaleUIcoN4 = {1048575.0f,15.0f*4096.0f*65536.0f,1048575.0f*4096.0f,1048575.0f};
    static const XMVECTORI32 MaskUIcoN4 = {0xFFFFF,0xF<<(60-32),0xFFFFF000,0xFFFFF};
    static const XMVECTORF32 AddUIcoN4 = {0.0f,-32768.0f*65536.0f,-32768.0f*65536.0f,0.0f};
    static const XMVECTORI32 XorUIcoN4 = {0x00000000,0x80000000,0x80000000,0x00000000};
    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUIcoN4);
    // Adjust for unsigned entries
    vResult = _mm_add_ps(vResult,AddUIcoN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Fix the signs on the unsigned entries
    vResulti = _mm_xor_si128(vResulti,XorUIcoN4);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUIcoN4);
    // Shift y and z to straddle the 32-bit boundary
    __m128i vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_

    #undef XM_URange
    #undef XM_URangeDiv2
    #undef XM_UMaxXYZ
    #undef XM_UMaxW
    #undef XM_ScaleXYZ
    #undef XM_ScaleW
    #undef XM_Scale
    #undef XM_Offset
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUIco4
(
    XMUICO4*  pDestination, 
    FXMVECTOR  V
)
{
    #define XM_Scale        (-1.0f / XM_PACK_FACTOR)
    #define XM_URange       ((FLOAT)(1 << 20))
    #define XM_URangeDiv2   ((FLOAT)(1 << 19))

#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {1048575.0f, 1048575.0f, 1048575.0f, 15.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);
    N = XMVectorRound(N);

    pDestination->v = ((UINT64)N.v[3] << 60) |
                       (((UINT64)N.v[2] & 0xFFFFF) << 40) |
                       (((UINT64)N.v[1] & 0xFFFFF) << 20) |
                       (((UINT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 MaxUIco4 = { 1048575.0f, 15.0f, 1048575.0f, 1048575.0f};
    static const XMVECTORF32 ScaleUIco4 = {1.0f,4096.0f*65536.0f,4096.0f,1.0f};
    static const XMVECTORI32 MaskUIco4 = {0xFFFFF,0xF<<(60-32),0xFFFFF000,0xFFFFF};
    static const XMVECTORF32 AddUIco4 = {0.0f,-32768.0f*65536.0f,-32768.0f*65536.0f,0.0f};
    static const XMVECTORI32 XorUIco4 = {0x00000000,0x80000000,0x80000000,0x00000000};
    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,g_XMZero);
    vResult = _mm_min_ps(vResult,MaxUIco4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUIco4);
    vResult = _mm_add_ps(vResult,AddUIco4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    vResulti = _mm_xor_si128(vResulti,XorUIco4);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUIco4);
    // Shift y and z to straddle the 32-bit boundary
    __m128i vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_

    #undef XM_Scale
    #undef XM_URange
    #undef XM_URangeDiv2
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreIcoN4
(
    XMICON4*  pDestination, 
    FXMVECTOR  V
)
{
    #define XM_Scale    (-1.0f / XM_PACK_FACTOR)
    #define XM_URange   ((FLOAT)(1 << 4))
    #define XM_Offset   (3.0f)
    #define XM_UMaxXYZ  ((FLOAT)((1 << (20 - 1)) - 1))
    #define XM_UMaxW    ((FLOAT)((1 << (4 - 1)) - 1))

#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {524287.0f, 524287.0f, 524287.0f, 7.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiplyAdd(N, Scale.v, g_XMNegativeZero.v);
    N = XMVectorRound(N);

    pDestination->v = ((UINT64)N.v[3] << 60) |
                       (((UINT64)N.v[2] & 0xFFFFF) << 40) |
                       (((UINT64)N.v[1] & 0xFFFFF) << 20) |
                       (((UINT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 ScaleIcoN4 = {524287.0f,7.0f*4096.0f*65536.0f,524287.0f*4096.0f,524287.0f};
    static const XMVECTORI32 MaskIcoN4 = {0xFFFFF,0xF<<(60-32),0xFFFFF000,0xFFFFF};
    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleIcoN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskIcoN4);
    // Shift y and z to straddle the 32-bit boundary
    __m128i vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_

    #undef XM_Scale
    #undef XM_URange
    #undef XM_Offset
    #undef XM_UMaxXYZ
    #undef XM_UMaxW
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreIco4
(
    XMICO4*  pDestination, 
    FXMVECTOR V
)
{
    #define XM_Scale    (-1.0f / XM_PACK_FACTOR)
    #define XM_URange   ((FLOAT)(1 << 4))
    #define XM_Offset   (3.0f)

#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-524287.0f, -524287.0f, -524287.0f, -7.0f};
    static CONST XMVECTOR  Max = {524287.0f, 524287.0f, 524287.0f, 7.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);
    N = XMVectorRound(N);

    pDestination->v = ((INT64)N.v[3] << 60) |
                       (((INT64)N.v[2] & 0xFFFFF) << 40) |
                       (((INT64)N.v[1] & 0xFFFFF) << 20) |
                       (((INT64)N.v[0] & 0xFFFFF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    // Note: Masks are x,w,y and z
    static const XMVECTORF32 MinIco4 = {-524287.0f,-7.0f,-524287.0f,-524287.0f};
    static const XMVECTORF32 MaxIco4 = { 524287.0f, 7.0f, 524287.0f, 524287.0f};
    static const XMVECTORF32 ScaleIco4 = {1.0f,4096.0f*65536.0f,4096.0f,1.0f};
    static const XMVECTORI32 MaskIco4 = {0xFFFFF,0xF<<(60-32),0xFFFFF000,0xFFFFF};
    // Clamp to bounds
    XMVECTOR vResult = _mm_shuffle_ps(V,V,_MM_SHUFFLE(2,1,3,0));
    vResult = _mm_max_ps(vResult,MinIco4);
    vResult = _mm_min_ps(vResult,MaxIco4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleIco4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskIco4);
    // Shift y and z to straddle the 32-bit boundary
    __m128i vResulti2 = _mm_srli_si128(vResulti,(64+12)/8);
    // Shift it into place
    vResulti2 = _mm_slli_si128(vResulti2,20/8);
    // i = x|y<<20|z<<40|w<<60
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_sd(reinterpret_cast<double *>(&pDestination->v),reinterpret_cast<const __m128d *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_

    #undef XM_Scale
    #undef XM_URange
    #undef XM_Offset
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreXDecN4
(
    XMXDECN4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Min = {-1.0f, -1.0f, -1.0f, 0.0f};
    static CONST XMVECTORF32  Scale = {511.0f, 511.0f, 511.0f, 3.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->v = ((UINT)N.v[3] << 30) |
                       (((INT)N.v[2] & 0x3FF) << 20) |
                       (((INT)N.v[1] & 0x3FF) << 10) |
                       (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMVECTORF32 Min = {-1.0f, -1.0f, -1.0f, 0.0f};
    static const XMVECTORF32 Scale = {511.0f, 511.0f*1024.0f, 511.0f*1048576.0f,3.0f*536870912.0f};
    static const XMVECTORI32 ScaleMask = {0x3FF,0x3FF<<10,0x3FF<<20,0x3<<29};
	XMASSERT(pDestination);
    XMVECTOR vResult = _mm_max_ps(V,Min);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,Scale);
    // Convert to int (W is unsigned)
    __m128i vResulti = _mm_cvtps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,ScaleMask);
    // To fix W, add itself to shift it up to <<30 instead of <<29
    __m128i vResultw = _mm_and_si128(vResulti,g_XMMaskW);
    vResulti = _mm_add_epi32(vResulti,vResultw);
    // Do a horizontal or of all 4 entries
    vResult = _mm_shuffle_ps(reinterpret_cast<const __m128 *>(&vResulti)[0],reinterpret_cast<const __m128 *>(&vResulti)[0],_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(0,3,2,1));
    vResulti = _mm_or_si128(vResulti,reinterpret_cast<const __m128i *>(&vResult)[0]);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreXDec4
(
    XMXDEC4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-511.0f, -511.0f, -511.0f, 0.0f};
    static CONST XMVECTOR  Max = {511.0f, 511.0f, 511.0f, 3.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);

    pDestination->v = ((UINT)N.v[3] << 30) |
                       (((INT)N.v[2] & 0x3FF) << 20) |
                       (((INT)N.v[1] & 0x3FF) << 10) |
                       (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MinXDec4 = {-511.0f,-511.0f,-511.0f, 0.0f};
    static const XMVECTORF32 MaxXDec4 = { 511.0f, 511.0f, 511.0f, 3.0f};
    static const XMVECTORF32 ScaleXDec4 = {1.0f,1024.0f/2.0f,1024.0f*1024.0f,1024.0f*1024.0f*1024.0f/2.0f};
    static const XMVECTORI32 MaskXDec4= {0x3FF,0x3FF<<(10-1),0x3FF<<20,0x3<<(30-1)};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,MinXDec4);
    vResult = _mm_min_ps(vResult,MaxXDec4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleXDec4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskXDec4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // Perform a single bit left shift on y|w
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUDecN4
(
    XMUDECN4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {1023.0f, 1023.0f, 1023.0f, 3.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = ((UINT)N.v[3] << 30) |
                       (((UINT)N.v[2] & 0x3FF) << 20) |
                       (((UINT)N.v[1] & 0x3FF) << 10) |
                       (((UINT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleUDecN4 = {1023.0f,1023.0f*1024.0f*0.5f,1023.0f*1024.0f*1024.0f,3.0f*1024.0f*1024.0f*1024.0f*0.5f};
    static const XMVECTORI32 MaskUDecN4= {0x3FF,0x3FF<<(10-1),0x3FF<<20,0x3<<(30-1)};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUDecN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUDecN4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // Perform a left shift by one bit on y|w
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUDec4
(
    XMUDEC4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {1023.0f, 1023.0f, 1023.0f, 3.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);

    pDestination->v = ((UINT)N.v[3] << 30) |
                       (((UINT)N.v[2] & 0x3FF) << 20) |
                       (((UINT)N.v[1] & 0x3FF) << 10) |
                       (((UINT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MaxUDec4 = { 1023.0f, 1023.0f, 1023.0f, 3.0f};
    static const XMVECTORF32 ScaleUDec4 = {1.0f,1024.0f/2.0f,1024.0f*1024.0f,1024.0f*1024.0f*1024.0f/2.0f};
    static const XMVECTORI32 MaskUDec4= {0x3FF,0x3FF<<(10-1),0x3FF<<20,0x3<<(30-1)};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,MaxUDec4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUDec4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUDec4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // Perform a left shift by one bit on y|w
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreDecN4
(
    XMDECN4* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {511.0f, 511.0f, 511.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, g_XMNegativeOne.v, g_XMOne.v);
    N = XMVectorMultiply(N, Scale.v);

    pDestination->v = ((INT)N.v[3] << 30) |
                       (((INT)N.v[2] & 0x3FF) << 20) |
                       (((INT)N.v[1] & 0x3FF) << 10) |
                       (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleDecN4 = {511.0f,511.0f*1024.0f,511.0f*1024.0f*1024.0f,1.0f*1024.0f*1024.0f*1024.0f};
    static const XMVECTORI32 MaskDecN4= {0x3FF,0x3FF<<10,0x3FF<<20,0x3<<30};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleDecN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskDecN4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreDec4
(
    XMDEC4*  pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-511.0f, -511.0f, -511.0f, -1.0f};
    static CONST XMVECTOR  Max = {511.0f, 511.0f, 511.0f, 1.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);

    pDestination->v = ((INT)N.v[3] << 30) |
                       (((INT)N.v[2] & 0x3FF) << 20) |
                       (((INT)N.v[1] & 0x3FF) << 10) |
                       (((INT)N.v[0] & 0x3FF));

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MinDec4 = {-511.0f,-511.0f,-511.0f,-1.0f};
    static const XMVECTORF32 MaxDec4 = { 511.0f, 511.0f, 511.0f, 1.0f};
    static const XMVECTORF32 ScaleDec4 = {1.0f,1024.0f,1024.0f*1024.0f,1024.0f*1024.0f*1024.0f};
    static const XMVECTORI32 MaskDec4= {0x3FF,0x3FF<<10,0x3FF<<20,0x3<<30};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,MinDec4);
    vResult = _mm_min_ps(vResult,MaxDec4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleDec4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskDec4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUByteN4
(
    XMUBYTEN4* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {255.0f, 255.0f, 255.0f, 255.0f};

    XMASSERT(pDestination);

    N = XMVectorSaturate(V);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->x = (BYTE)N.v[0];
    pDestination->y = (BYTE)N.v[1];
    pDestination->z = (BYTE)N.v[2];
    pDestination->w = (BYTE)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleUByteN4 = {255.0f,255.0f*256.0f*0.5f,255.0f*256.0f*256.0f,255.0f*256.0f*256.0f*256.0f*0.5f};
    static const XMVECTORI32 MaskUByteN4 = {0xFF,0xFF<<(8-1),0xFF<<16,0xFF<<(24-1)};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUByteN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUByteN4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // Perform a single bit left shift to fix y|w 
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreUByte4
(
    XMUBYTE4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Max = {255.0f, 255.0f, 255.0f, 255.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, XMVectorZero(), Max);
    N = XMVectorRound(N);

    pDestination->x = (BYTE)N.v[0];
    pDestination->y = (BYTE)N.v[1];
    pDestination->z = (BYTE)N.v[2];
    pDestination->w = (BYTE)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MaxUByte4 = { 255.0f, 255.0f, 255.0f, 255.0f};
    static const XMVECTORF32 ScaleUByte4 = {1.0f,256.0f*0.5f,256.0f*256.0f,256.0f*256.0f*256.0f*0.5f};
    static const XMVECTORI32 MaskUByte4 = {0xFF,0xFF<<(8-1),0xFF<<16,0xFF<<(24-1)};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    vResult = _mm_min_ps(vResult,MaxUByte4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleUByte4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskUByte4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // Perform a single bit left shift to fix y|w 
    vResulti2 = _mm_add_epi32(vResulti2,vResulti2);
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreByteN4
(
    XMBYTEN4* pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {127.0f, 127.0f, 127.0f, 127.0f};

    XMASSERT(pDestination);

    N = XMVectorMultiply(V, Scale.v);
    N = XMVectorRound(N);

    pDestination->x = (CHAR)N.v[0];
    pDestination->y = (CHAR)N.v[1];
    pDestination->z = (CHAR)N.v[2];
    pDestination->w = (CHAR)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 ScaleByteN4 = {127.0f,127.0f*256.0f,127.0f*256.0f*256.0f,127.0f*256.0f*256.0f*256.0f};
    static const XMVECTORI32 MaskByteN4 = {0xFF,0xFF<<8,0xFF<<16,0xFF<<24};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,g_XMNegativeOne);
    vResult = _mm_min_ps(vResult,g_XMOne);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleByteN4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskByteN4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreByte4
(
    XMBYTE4*  pDestination, 
    FXMVECTOR  V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTOR  Min = {-127.0f, -127.0f, -127.0f, -127.0f};
    static CONST XMVECTOR  Max = {127.0f, 127.0f, 127.0f, 127.0f};

    XMASSERT(pDestination);

    N = XMVectorClamp(V, Min, Max);
    N = XMVectorRound(N);

    pDestination->x = (CHAR)N.v[0];
    pDestination->y = (CHAR)N.v[1];
    pDestination->z = (CHAR)N.v[2];
    pDestination->w = (CHAR)N.v[3];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static const XMVECTORF32 MinByte4 = {-127.0f,-127.0f,-127.0f,-127.0f};
    static const XMVECTORF32 MaxByte4 = { 127.0f, 127.0f, 127.0f, 127.0f};
    static const XMVECTORF32 ScaleByte4 = {1.0f,256.0f,256.0f*256.0f,256.0f*256.0f*256.0f};
    static const XMVECTORI32 MaskByte4 = {0xFF,0xFF<<8,0xFF<<16,0xFF<<24};
    // Clamp to bounds
    XMVECTOR vResult = _mm_max_ps(V,MinByte4);
    vResult = _mm_min_ps(vResult,MaxByte4);
    // Scale by multiplication
    vResult = _mm_mul_ps(vResult,ScaleByte4);
    // Convert to int
    __m128i vResulti = _mm_cvttps_epi32(vResult);
    // Mask off any fraction
    vResulti = _mm_and_si128(vResulti,MaskByte4);
    // Do a horizontal or of 4 entries
    __m128i vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(3,2,3,2));
    // x = x|z, y = y|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    // Move Z to the x position
    vResulti2 = _mm_shuffle_epi32(vResulti,_MM_SHUFFLE(1,1,1,1));
    // i = x|y|z|w
    vResulti = _mm_or_si128(vResulti,vResulti2);
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->v),reinterpret_cast<const __m128 *>(&vResulti)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreColor
(
    XMCOLOR* pDestination, 
    FXMVECTOR V
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMVECTOR               N;
    static CONST XMVECTORF32  Scale = {255.0f, 255.0f, 255.0f, 255.0f};

    XMASSERT(pDestination);

    N = XMVectorSaturate(V);
    N = XMVectorMultiply(N, Scale.v);
    N = XMVectorRound(N);

    pDestination->c = ((UINT)N.v[3] << 24) |
                      ((UINT)N.v[0] << 16) |
                      ((UINT)N.v[1] <<  8) |
                      ((UINT)N.v[2]);

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    static CONST XMVECTORF32  Scale = {255.0f,255.0f,255.0f,255.0f};
    // Set <0 to 0
	XMVECTOR vResult = _mm_max_ps(V,g_XMZero);
    // Set>1 to 1
	vResult = _mm_min_ps(vResult,g_XMOne);
    // Convert to 0-255
    vResult = _mm_mul_ps(vResult,Scale);
    // Shuffle RGBA to ARGB
    vResult = _mm_shuffle_ps(vResult,vResult,_MM_SHUFFLE(2,1,0,3));
    // Convert to int 
    __m128i vInt = _mm_cvtps_epi32(vResult);
    // Mash to shorts
    vInt = _mm_packs_epi32(vInt,vInt);
    // Mash to bytes
    vInt = _mm_packs_epi16(vInt,vInt);
    // Store the color
    _mm_store_ss(reinterpret_cast<float *>(&pDestination->c),reinterpret_cast<__m128 *>(&vInt)[0]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat3x3
(
    XMFLOAT3X3*	pDestination, 
    CXMMATRIX	M
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(XM_NO_MISALIGNED_VECTOR_ACCESS) || defined(_XM_SSE_INTRINSICS_)

    XMStoreFloat3x3NC(pDestination, M);

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat3x3NC
(
    XMFLOAT3X3* pDestination, 
    CXMMATRIX M
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->m[0][0] = M.r[0].v[0];
    pDestination->m[0][1] = M.r[0].v[1];
    pDestination->m[0][2] = M.r[0].v[2];

    pDestination->m[1][0] = M.r[1].v[0];
    pDestination->m[1][1] = M.r[1].v[1];
    pDestination->m[1][2] = M.r[1].v[2];

    pDestination->m[2][0] = M.r[2].v[0];
    pDestination->m[2][1] = M.r[2].v[1];
    pDestination->m[2][2] = M.r[2].v[2];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    XMVECTOR vTemp1 = M.r[0];
    XMVECTOR vTemp2 = M.r[1];
    XMVECTOR vTemp3 = M.r[2];
    XMVECTOR vWork = _mm_shuffle_ps(vTemp1,vTemp2,_MM_SHUFFLE(0,0,2,2));
    vTemp1 = _mm_shuffle_ps(vTemp1,vWork,_MM_SHUFFLE(2,0,1,0));
    _mm_storeu_ps(&pDestination->m[0][0],vTemp1);
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp3,_MM_SHUFFLE(1,0,2,1));
    _mm_storeu_ps(&pDestination->m[1][1],vTemp2);
    vTemp3 = _mm_shuffle_ps(vTemp3,vTemp3,_MM_SHUFFLE(2,2,2,2));
    _mm_store_ss(&pDestination->m[2][2],vTemp3);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x3
(
    XMFLOAT4X3* pDestination, 
    CXMMATRIX M
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(XM_NO_MISALIGNED_VECTOR_ACCESS) || defined(_XM_SSE_INTRINSICS_)

    XMStoreFloat4x3NC(pDestination, M);

#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x3A
(
    XMFLOAT4X3A*	pDestination, 
    CXMMATRIX		M
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination->m[0][0] = M.r[0].v[0];
    pDestination->m[0][1] = M.r[0].v[1];
    pDestination->m[0][2] = M.r[0].v[2];

    pDestination->m[1][0] = M.r[1].v[0];
    pDestination->m[1][1] = M.r[1].v[1];
    pDestination->m[1][2] = M.r[1].v[2];

    pDestination->m[2][0] = M.r[2].v[0];
    pDestination->m[2][1] = M.r[2].v[1];
    pDestination->m[2][2] = M.r[2].v[2];

    pDestination->m[3][0] = M.r[3].v[0];
    pDestination->m[3][1] = M.r[3].v[1];
    pDestination->m[3][2] = M.r[3].v[2];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);
    // x1,y1,z1,w1
    XMVECTOR vTemp1 = M.r[0];
    // x2,y2,z2,w2
    XMVECTOR vTemp2 = M.r[1];
    // x3,y3,z3,w3
    XMVECTOR vTemp3 = M.r[2];
    // x4,y4,z4,w4
    XMVECTOR vTemp4 = M.r[3];
    // z1,z1,x2,y2
    XMVECTOR vTemp = _mm_shuffle_ps(vTemp1,vTemp2,_MM_SHUFFLE(1,0,2,2));
    // y2,z2,x3,y3 (Final)
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp3,_MM_SHUFFLE(1,0,2,1));
    // x1,y1,z1,x2 (Final)
    vTemp1 = _mm_shuffle_ps(vTemp1,vTemp,_MM_SHUFFLE(2,0,1,0));
    // z3,z3,x4,x4
    vTemp3 = _mm_shuffle_ps(vTemp3,vTemp4,_MM_SHUFFLE(0,0,2,2));
    // z3,x4,y4,z4 (Final)
    vTemp3 = _mm_shuffle_ps(vTemp3,vTemp4,_MM_SHUFFLE(2,1,2,0));
    // Store in 3 operations
    _mm_store_ps(&pDestination->m[0][0],vTemp1);
    _mm_store_ps(&pDestination->m[1][1],vTemp2);
    _mm_store_ps(&pDestination->m[2][2],vTemp3);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x3NC
(
    XMFLOAT4X3* pDestination, 
    CXMMATRIX M
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->m[0][0] = M.r[0].v[0];
    pDestination->m[0][1] = M.r[0].v[1];
    pDestination->m[0][2] = M.r[0].v[2];

    pDestination->m[1][0] = M.r[1].v[0];
    pDestination->m[1][1] = M.r[1].v[1];
    pDestination->m[1][2] = M.r[1].v[2];

    pDestination->m[2][0] = M.r[2].v[0];
    pDestination->m[2][1] = M.r[2].v[1];
    pDestination->m[2][2] = M.r[2].v[2];

    pDestination->m[3][0] = M.r[3].v[0];
    pDestination->m[3][1] = M.r[3].v[1];
    pDestination->m[3][2] = M.r[3].v[2];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);
    XMVECTOR vTemp1 = M.r[0];
    XMVECTOR vTemp2 = M.r[1];
    XMVECTOR vTemp3 = M.r[2];
    XMVECTOR vTemp4 = M.r[3];
    XMVECTOR vTemp2x = _mm_shuffle_ps(vTemp2,vTemp3,_MM_SHUFFLE(1,0,2,1));
    vTemp2 = _mm_shuffle_ps(vTemp2,vTemp1,_MM_SHUFFLE(2,2,0,0));
    vTemp1 = _mm_shuffle_ps(vTemp1,vTemp2,_MM_SHUFFLE(0,2,1,0));
    vTemp3 = _mm_shuffle_ps(vTemp3,vTemp4,_MM_SHUFFLE(0,0,2,2));
    vTemp3 = _mm_shuffle_ps(vTemp3,vTemp4,_MM_SHUFFLE(2,1,2,0));
    _mm_storeu_ps(&pDestination->m[0][0],vTemp1);
    _mm_storeu_ps(&pDestination->m[1][1],vTemp2x);
    _mm_storeu_ps(&pDestination->m[2][2],vTemp3);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x4
(
    XMFLOAT4X4* pDestination, 
    CXMMATRIX M
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(XM_NO_MISALIGNED_VECTOR_ACCESS)

    XMStoreFloat4x4NC(pDestination, M);

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);

	_mm_storeu_ps( &pDestination->_11, M.r[0] );
	_mm_storeu_ps( &pDestination->_21, M.r[1] );
	_mm_storeu_ps( &pDestination->_31, M.r[2] );
	_mm_storeu_ps( &pDestination->_41, M.r[3] );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x4A
(
    XMFLOAT4X4A*	pDestination, 
    CXMMATRIX		M
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);
    XMASSERT(((UINT_PTR)pDestination & 0xF) == 0);

    pDestination->m[0][0] = M.r[0].v[0];
    pDestination->m[0][1] = M.r[0].v[1];
    pDestination->m[0][2] = M.r[0].v[2];
    pDestination->m[0][3] = M.r[0].v[3];

    pDestination->m[1][0] = M.r[1].v[0];
    pDestination->m[1][1] = M.r[1].v[1];
    pDestination->m[1][2] = M.r[1].v[2];
    pDestination->m[1][3] = M.r[1].v[3];

    pDestination->m[2][0] = M.r[2].v[0];
    pDestination->m[2][1] = M.r[2].v[1];
    pDestination->m[2][2] = M.r[2].v[2];
    pDestination->m[2][3] = M.r[2].v[3];

    pDestination->m[3][0] = M.r[3].v[0];
    pDestination->m[3][1] = M.r[3].v[1];
    pDestination->m[3][2] = M.r[3].v[2];
    pDestination->m[3][3] = M.r[3].v[3];

#elif defined(_XM_SSE_INTRINSICS_)
	XMASSERT(pDestination);

	_mm_store_ps( &pDestination->_11, M.r[0] );
	_mm_store_ps( &pDestination->_21, M.r[1] );
	_mm_store_ps( &pDestination->_31, M.r[2] );
	_mm_store_ps( &pDestination->_41, M.r[3] );
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

//------------------------------------------------------------------------------

XMFINLINE VOID XMStoreFloat4x4NC
(
    XMFLOAT4X4* pDestination, 
    CXMMATRIX M
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMASSERT(pDestination);

    pDestination->m[0][0] = M.r[0].v[0];
    pDestination->m[0][1] = M.r[0].v[1];
    pDestination->m[0][2] = M.r[0].v[2];
    pDestination->m[0][3] = M.r[0].v[3];

    pDestination->m[1][0] = M.r[1].v[0];
    pDestination->m[1][1] = M.r[1].v[1];
    pDestination->m[1][2] = M.r[1].v[2];
    pDestination->m[1][3] = M.r[1].v[3];

    pDestination->m[2][0] = M.r[2].v[0];
    pDestination->m[2][1] = M.r[2].v[1];
    pDestination->m[2][2] = M.r[2].v[2];
    pDestination->m[2][3] = M.r[2].v[3];

    pDestination->m[3][0] = M.r[3].v[0];
    pDestination->m[3][1] = M.r[3].v[1];
    pDestination->m[3][2] = M.r[3].v[2];
    pDestination->m[3][3] = M.r[3].v[3];

#elif defined(_XM_SSE_INTRINSICS_)
    XMASSERT(pDestination);
    _mm_storeu_ps(&pDestination->m[0][0],M.r[0]);
    _mm_storeu_ps(&pDestination->m[1][0],M.r[1]);
    _mm_storeu_ps(&pDestination->m[2][0],M.r[2]);
    _mm_storeu_ps(&pDestination->m[3][0],M.r[3]);
#else // _XM_VMX128_INTRINSICS_
#endif // _XM_VMX128_INTRINSICS_
}

#endif // __XNAMATHCONVERT_INL__

