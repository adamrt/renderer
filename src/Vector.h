#pragma once

#include <cmath>

struct Vec2 {
    float x, y;
    Vec2()
        : x(0.0f)
        , y(0.0f)
    {
    }
    Vec2(float x, float y)
        : x(x)
        , y(y)
    {
    }

    Vec2 operator-(const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }
};

struct Vec3 {
    float x, y, z;

    Vec3()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    {
    }

    Vec3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    Vec3 rotate_x(float angle) const
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Vec3(x, y * c - z * s, y * s + z * c);
    }

    Vec3 rotate_y(float angle) const
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Vec3(x * c + z * s, y, -x * s + z * c);
    }

    Vec3 rotate_z(float angle) const
    {
        float c = std::cos(angle);
        float s = std::sin(angle);
        return Vec3(x * c - y * s, x * s + y * c, z);
    }
};
