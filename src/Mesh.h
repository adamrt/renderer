#pragma once
#include <cstdint>
#include <vector>

#include "Vector.h"

// Face is a triangle with vertex indices.
struct Face {
    uint32_t a, b, c;
    uint32_t color;
};

struct Mesh {
    Mesh(std::vector<Vec3>, std::vector<Face>);

    std::vector<Vec3> vertices;
    std::vector<Face> faces;
};
