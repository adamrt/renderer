#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Color.h"
#include "Triangle.h"
#include "Vector.h"

struct Mesh {
    Mesh(const std::string filename);

    std::vector<Triangle> triangles;
    Vec3 normalized_scale() const;
    Vec3 center_translation() const;
};

// Face has 3 vertices, each with 3 indices (position, normal, uv).
// This is only used to read the file, then convert it to a Triangle.
struct Face {
    std::array<u32, 3> positions;
    std::array<u32, 3> normals;
    std::array<u32, 3> uvs;
};
