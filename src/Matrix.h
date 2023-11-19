#pragma once

#include <array>
#include <cmath>
#include <iostream>

#include "AK.h"
#include "Vector.h"

class Mat4 {
private:
    std::array<std::array<f32, 4>, 4> data;

public:
    Mat4();

    // Access element (for both const and non-const matrices)
    f32& operator()(i32 row, i32 col) { return data[row][col]; }
    const f32& operator()(i32 row, i32 col) const { return data[row][col]; }

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
    static Mat4 rotation_x(f32 angle);
    static Mat4 rotation_y(f32 angle);
    static Mat4 rotation_z(f32 angle);
    static Mat4 world(const Vec3& s, const Vec3& r, const Vec3& t);

    static Mat4 perspective(f32 fov, f32 aspect, f32 znear, f32 zfar);
    static Mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 znear, f32 zfar);
};
