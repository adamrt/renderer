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
    Mat4 world = Mat4::identity() * translation * rotation * scale;
    return world;
}
