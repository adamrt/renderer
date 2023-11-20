#include "Vector.h"

Vec2::Vec2()
    : x(0.0f)
    , y(0.0f)
{
}

Vec2::Vec2(f32 x, f32 y)
    : x(x)
    , y(y)
{
}

Vec2 Vec2::operator*(f32 scalar) const
{
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

f32 Vec2::dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

f32 Vec2::edge_cross(const Vec2& a, const Vec2& b, const Vec2& c)
{
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

Vec2 Vec3::xy() const
{
    return Vec2(x, y);
};

Vec3::Vec3()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
{
}

Vec3::Vec3(f32 x, f32 y, f32 z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vec3 Vec3::operator+(const Vec3& rhs) const
{
    return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3 Vec3::operator-(const Vec3& rhs) const
{
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3 Vec3::operator*(f32 scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(f32 scalar) const
{
    return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b)
{
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

f32 Vec3::dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void Vec3::normalize()
{
    f32 length = std::sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}

Vec3 Vec3::normalized() const
{
    Vec3 v = *this;
    v.normalize();
    return v;
}

Vec4 Vec3::xyzw() const
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

Vec4::Vec4(f32 x, f32 y, f32 z, f32 w)
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

Vec3 Vec4::xyz() const
{
    return Vec3(x, y, z);
};

Vec2 Vec4::xy() const
{
    return Vec2(x, y);
};
