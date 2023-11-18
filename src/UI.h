#pragma once

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

struct UI {
    bool rotate = false;
    bool draw_filled = false;
    bool draw_wireframe = true;
    bool backface_culling = false;

    Projection projection = Projection::Perspective;
};
