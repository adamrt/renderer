// This is a header file for Adam's Kit.
#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

constexpr f32 K_PI = 3.141592653589793;
constexpr f32 DEG2RAD_MUL(K_PI / 180.0f);

inline f32 kclamp(f32 n, f32 lower, f32 upper)
{
    return std::max(lower, std::min(n, upper));
}

inline f32 kradians(f32 degrees)
{
    return degrees * DEG2RAD_MUL;
}
