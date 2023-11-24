#pragma once

#include "Matrix.h"
#include "Vector.h"

const f32 ZNEAR = 0.01f;
const f32 ZFAR = 100.0f;

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

// Camera is an orbital camera
struct Camera {
    Camera(f32 aspect);

    void orbit(f32 dx, f32 dy);
    void zoom(f32 d);
    Vec3 calculate_eye() const;
    void update();

    Mat4 projection_matrix() const;
    Mat4 view_matrix() const;

    Projection projection;
    Mat4 perspective;
    Mat4 orthographic;

    Mat4 view;

    f32 distance = 5.0f;
    f32 fov = K_PI / 3.0f;

    f32 aspect;
    f32 theta;
    f32 phi;
};
