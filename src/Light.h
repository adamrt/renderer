#pragma once

#include "Color.h"
#include "Vector.h"

struct Light {
    Light(Vec3 position, Color color)
        : position(position)
        , color(color) {};

    Vec3 position;
    Color color;
    bool enabled { true };
};

struct AmbientLight {
    Color color;
};
