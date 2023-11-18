#include "Vector.h"

Vec2::Vec2()
    : x(0.0f)
    , y(0.0f)
{
}

Vec2::Vec2(float x, float y)
    : x(x)
    , y(y)
{
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec3::Vec3()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vec4 Vec3::vec4() const
{
    return Vec4(x, y, z, 1.0f);
};

Vec4::Vec4()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(0.0f)
{
}

Vec4::Vec4(float x, float y, float z, float w)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{
}

Vec4 Vec4::operator-(const Vec4& rhs) const
{
    return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

Vec3 Vec4::vec3() const
{
    return Vec3(x, y, z);
};
