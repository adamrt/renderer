#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "Vector.h"

// Triangle is a 2D triangle after scale, rotation, translation and
// projection.
struct Triangle {
    Triangle();
    Triangle(Vec2 a, Vec2 b, Vec2 c, uint32_t color);

    std::vector<Vec2> points;
    uint32_t color;
};
