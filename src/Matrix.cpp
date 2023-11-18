// This file contains 4x4 Matrix class in row-major order.
// NOTE: All methods use a left handed coordinate system.

#include "Matrix.h"
#include "Vector.h"

Mat4::Mat4()
    : data {}
{
}

Mat4 Mat4::operator+(const Mat4& rhs) const
{
    Mat4 m;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m(i, j) = data[i][j] + rhs(i, j);
        }
    }
    return m;
}

Mat4 Mat4::operator-(const Mat4& rhs) const
{
    Mat4 m;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m(i, j) = data[i][j] - rhs(i, j);
        }
    }
    return m;
}

Mat4 Mat4::operator*(const Mat4& rhs) const
{
    Mat4 m;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m(i, j) = 0;
            for (int k = 0; k < 4; ++k) {
                m(i, j) += data[i][k] * rhs(k, j);
            }
        }
    }
    return m;
}

Vec3 Mat4::operator*(const Vec3& v) const
{
    Vec3 m;
    m.x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3];
    m.y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3];
    m.z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3];
    return m;
}

Vec4 Mat4::operator*(const Vec4& v) const
{
    Vec4 m;
    m.x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3] * v.w;
    m.y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3] * v.w;
    m.z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3] * v.w;
    m.w = data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3] * v.w;
    return m;
}

Mat4 Mat4::identity()
{
    Mat4 m;
    // Initialize to identity matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.data[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return m;
}

Mat4 Mat4::translation(const Vec3& v)
{
    auto m = Mat4::identity();
    m(0, 3) = v.x;
    m(1, 3) = v.y;
    m(2, 3) = v.z;
    return m;
}

Mat4 Mat4::scale(const Vec3& v)
{
    auto m = Mat4::identity();
    m(0, 0) = v.x;
    m(1, 1) = v.y;
    m(2, 2) = v.z;
    return m;
}

Mat4 Mat4::rotation_x(float angle)
{
    auto c = std::cos(angle);
    auto s = std::sin(angle);
    auto m = Mat4::identity();
    m(1, 1) = c;
    m(1, 2) = -s;
    m(2, 1) = s;
    m(2, 2) = c;
    return m;
}

Mat4 Mat4::rotation_y(float angle)
{
    auto c = std::cos(angle);
    auto s = std::sin(angle);
    auto m = Mat4::identity();
    m(0, 0) = c;
    m(0, 2) = s;
    m(2, 0) = -s;
    m(2, 2) = c;
    return m;
}

Mat4 Mat4::rotation_z(float angle)
{
    auto c = std::cos(angle);
    auto s = std::sin(angle);
    auto m = Mat4::identity();
    m(0, 0) = c;
    m(0, 1) = -s;
    m(1, 0) = s;
    m(1, 1) = c;
    return m;
}

Mat4 Mat4::rotation(const Vec3& v)
{
    return Mat4::rotation_x(v.x) * Mat4::rotation_y(v.y) * Mat4::rotation_z(v.z);
}

Mat4 Mat4::world(const Vec3& s, const Vec3& r, const Vec3& t)
{
    Mat4 scale = Mat4::scale(s);
    Mat4 rotation = Mat4::rotation(r);
    Mat4 translation = Mat4::translation(t);
    Mat4 world = translation * rotation * scale;
    return world;
}

Mat4 Mat4::perspective(float fov, float aspect, float znear, float zfar)
{
    float thf = tan(fov / 2.0f);
    float z_range = zfar - znear;

    Mat4 m {};
    m(0, 0) = aspect * (1.0f / thf);
    m(1, 1) = 1.0f / thf;
    m(2, 2) = zfar / z_range;
    m(2, 3) = (-zfar * znear) / z_range;
    m(3, 2) = 1.0;

    return m;
}

Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float znear, float zfar)
{
    float width = right - left;
    float height = top - bottom;
    float depth = zfar - znear;

    auto m = Mat4::identity();
    m(0, 0) = 2.0f / width;
    m(1, 1) = 2.0f / height;
    m(2, 2) = -2.0f / depth;
    m(0, 3) = -(right + left) / width;
    m(1, 3) = -(top + bottom) / height;
    m(2, 3) = -(zfar + znear) / depth;

    return m;
}
