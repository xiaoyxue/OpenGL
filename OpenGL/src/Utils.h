#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <algorithm>
#include "Def.h"
/***************** Type ******************/
using uchar = unsigned char;

using int8 = int8_t;
using uint8 = uint8_t;

using int16 = int16_t;
using uint16 = uint16_t;

using int32 = int32_t;
using uint32 = uint32_t;

using int64 = int64_t;
using uint64 = uint64_t;

using float32 = float;
using float64 = double;

#ifdef USING_DOUBLE
using real = double;
using real_bit = uint64;
#else
using real = float;
using real_bit = uint32;
#endif

#ifdef _WIN64
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline __attribute__((always_inline))
#endif

#if defined(_MSC_VER)
#define ALIGNED(x) __declspec(align(x))
#endif


FORCE_INLINE real Radians(real deg) { return (PI / 180) * deg; }

FORCE_INLINE real Degrees(real rad) { return (180 / PI) * rad; }

template <class T1, class T2, class T3>
T1 Clamp(const T1& tVal, const T2& tMin, const T3& max)
{
	if (tVal < tMin) return tMin;
	if (tVal > max) return max;
	return tVal;
}