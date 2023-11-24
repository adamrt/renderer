#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Color.h"
#include "Vector.h"

// Face is a triangle in a mesh with indices to the vertex, normal and
// texture coordinate arrays.
struct Face {
    std::array<u32, 3> positions;
    std::array<u32, 3> normals;
    std::array<u32, 3> texcoords;

    Color color = Color::White;
};

struct Mesh {
    Mesh(const std::string filename);

    std::vector<Face> faces;

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
};
