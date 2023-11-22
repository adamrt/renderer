#pragma once

#include "Color.h"
#include "Vector.h"

struct Light {
    Vec3 position;
    Color color;
};

struct AmbientLight {
    Color color;
};
