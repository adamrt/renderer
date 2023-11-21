#pragma once

#include "imgui.h"

#include "AK.h"
#include "Vector.h"

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

struct UI {
    bool rotate = true;
    bool rotate_x = false;
    bool rotate_y = true;
    bool rotate_z = false;

    bool enable_fill_convention = true;
    bool enable_lighting = true;

    bool draw_texture = true;
    bool perspective_correction = true;
    bool draw_filled = false;
    ImVec4 fill_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    bool draw_empty = false;

    bool draw_wireframe = false;
    bool backface_culling = true;

    Projection projection = Projection::Perspective;
    Vec3 camera_position = Vec3(0.0f, 3.0f, -5.0f);

    int total_triangles = 0;
    int visible_triangles = 0;
};
