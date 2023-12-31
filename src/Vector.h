#pragma once

#include <cmath>

#include "AK.h"

struct Vec4;

struct Vec2 {
    f32 x {};
    f32 y {};

    Vec2();
    Vec2(f32 x, f32 y);

    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator*(f32 scalar) const;

    static f32 dot(const Vec2& a, const Vec2& b);
    static f32 edge_cross(const Vec2& a, const Vec2& b, const Vec2& p);
    static bool is_top_left(const Vec2& start, const Vec2& end);
};

struct Vec3 {
    f32 x {};
    f32 y {};
    f32 z {};

    Vec3();
    Vec3(f32 x, f32 y, f32 z);

    Vec3 operator+(const Vec3& rhs) const;
    Vec3 operator-(const Vec3& rhs) const;
    Vec3 operator*(f32 scalar) const;
    Vec3 operator/(f32 scalar) const;

    void normalize();
    Vec3 normalized() const;

    static Vec3 cross(const Vec3& a, const Vec3& b);
    static f32 dot(const Vec3& a, const Vec3& b);

    Vec2 xy() const;
    Vec4 xyzw() const;
};

struct Vec4 {
    f32 x {};
    f32 y {};
    f32 z {};
    f32 w {};

    Vec4();
    Vec4(f32 x, f32 y, f32 z, f32 w);

    Vec4 operator-(const Vec4& rhs) const;

    Vec2 xy() const;
    Vec3 xyz() const;
};
