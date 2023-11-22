#pragma once

#include "Matrix.h"
#include "UI.h"
#include "Vector.h"

const f32 ZNEAR = 0.01f;
const f32 ZFAR = 100.0f;

// Camera is an orbital camera
struct Camera {
    Camera(UI& ui);

    void orbit(f32 dx, f32 dy);
    void zoom(f32 d);
    Vec3 calculate_eye() const;
    void update();

    float distance = 5.0f;

    Mat4 view_matrix;
    Mat4 proj_matrix;

    Mat4 static_perspective;

    f32 aspect = 800.0f / 800.0f;

    UI& m_ui;
};
