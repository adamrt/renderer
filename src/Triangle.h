#pragma once

#include <array>
#include <vector>

#include "Color.h"
#include "Vector.h"

// Triangle is a 2D triangle after scale, rotation, translation and
// projection.
struct Triangle {
    Triangle();
    Triangle(Vec2 a, Vec2 b, Vec2 c, Color color);

    bool should_cull() const;

    std::array<Vec2, 3> points;
    std::array<Vec2, 3> texcoords;
    f32 avg_depth = 0.0f;
    Color color;
};

Vec3 vertices_normal(const std::array<Vec3, 3>& points);
