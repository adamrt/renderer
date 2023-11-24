#pragma once

#include <array>

#include "Color.h"
#include "Vector.h"

struct Vertex {
    Vec4 position;
    Vec3 normal;
    Vec2 uv;
};

struct Triangle {
    Triangle() {};

    std::array<Vertex, 3> vertices;
    Color light_sum;
};

bool should_cull(std::array<Vertex, 3> vertices);
Vec3 vertices_normal(const std::array<Vec3, 3>& points);
