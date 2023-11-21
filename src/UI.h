#pragma once

#include "imgui.h"

#include "AK.h"

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

struct UI {
    bool rotate = true;
    bool rotate_x = true;
    bool rotate_y = true;
    bool rotate_z = true;

    bool enable_lighting = true;

    bool draw_texture = true;
    bool perspective_correction = true;

    bool draw_filled = false;
    bool draw_empty = false;
    ImVec4 fill_color = { 1.0f, 1.0f, 1.0f, 1.0f };

    bool draw_wireframe = false;
    bool backface_culling = true;

    Projection projection = Projection::Perspective;
};
