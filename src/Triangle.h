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
    std::vector<Vec2> texcoords;
    float avg_depth = 0.0f;
    Color color;
};

Vec3 vertices_normal(const std::array<Vec3, 3>& points);
