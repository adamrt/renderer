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
    Vec3 normal() const;

    std::vector<Vec4> points;
    float avg_depth = 0.0f;
    Color color;
};
