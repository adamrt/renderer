#pragma once

#include <cmath>

struct Vec4;

struct Vec2 {
    float x, y;
    Vec2();
    Vec2(float x, float y);
    Vec2 operator-(const Vec2& rhs) const;
};

struct Vec3 {
    float x, y, z;
    Vec3();
    Vec3(float x, float y, float z);

    Vec4 vec4() const;
};

struct Vec4 {
    float x, y, z, w;
    Vec4();
    Vec4(float x, float y, float z, float w);

    Vec3 vec3() const;
};
