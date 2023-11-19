#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "Color.h"
#include "Vector.h"

// Triangle is a 2D triangle after scale, rotation, translation and
// projection.
struct Triangle {
    Triangle();
    Triangle(Vec4 a, Vec4 b, Vec4 c, Color color);

    bool should_cull() const;

    std::vector<Vec4> points;
    Color color;
};
