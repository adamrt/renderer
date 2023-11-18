#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "Vector.h"

// Face is a triangle with vertex indices.
struct Face {
    uint32_t a, b, c;
    uint32_t color;
};

struct Mesh {
    Mesh(std::vector<Vec3>, std::vector<Face>);
    Mesh(const std::string filename);

    std::vector<Vec3> vertices;
    std::vector<Face> faces;

    Vec3 scale = Vec3(1.0f, 1.0f, 1.0f);
    Vec3 rotation = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 translation = Vec3(0.0f, 0.0f, 0.0f);
};
