#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "Vector.h"

class Mat4 {
private:
    std::array<std::array<float, 4>, 4> data;

public:
    Mat4();

    // Access element (for both const and non-const matrices)
    float& operator()(int row, int col) { return data[row][col]; }
    const float& operator()(int row, int col) const { return data[row][col]; }

    Mat4 operator+(const Mat4& rhs) const;
    Mat4 operator-(const Mat4& rhs) const;
    Mat4 operator*(const Mat4& rhs) const;

    Vec3 operator*(const Vec3& v) const;
    Vec4 operator*(const Vec4& v) const;

    // Static methods for creating common matrices
    static Mat4 identity();
    static Mat4 translation(const Vec3& scale);
    static Mat4 scale(const Vec3& scale);
    static Mat4 rotation(const Vec3& scale);
    static Mat4 rotation_x(float angle);
    static Mat4 rotation_y(float angle);
    static Mat4 rotation_z(float angle);
    static Mat4 world(const Vec3& s, const Vec3& r, const Vec3& t);

    static Mat4 perspective(float fov, float aspect, float znear, float zfar);
    static Mat4 orthographic(float left, float right, float bottom, float top, float znear, float zfar);
};
