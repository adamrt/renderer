#pragma once

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

struct UI {
    bool rotate = true;
    bool draw_filled = true;
    bool draw_wireframe = true;
    bool backface_culling = true;

    Projection projection = Projection::Perspective;
};
