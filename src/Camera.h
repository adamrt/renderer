#pragma once

#include "Matrix.h"
#include "Vector.h"

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

// Camera is an orbital camera
struct Camera {
    Camera(i32 width, i32 height, f32 fov, f32 near, f32 far);

    void orbit(f32 dx, f32 dy);
    void zoom(f32 d);
    Vec3 calculate_eye() const;
    void update();

    Mat4 projection_matrix() const;
    Mat4 view_matrix() const;

    Projection projection {};
    Mat4 perspective {};
    Mat4 orthographic {};
    Mat4 view {};

    i32 width {};
    i32 height {};
    f32 aspect {};
    f32 fov {};
    f32 near {};
    f32 far {};

    f32 distance { 5.0f };
    f32 theta {};
    f32 phi {};
};
