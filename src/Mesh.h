#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "Color.h"
#include "Vector.h"

// Face is a triangle with vertex indices.
struct Face {
    uint32_t a, b, c;
    uint32_t ta, tb, tc;
    Color color = Color::Black;
};

struct Mesh {
    Mesh(std::vector<Vec3>, std::vector<Face>);
    Mesh(const std::string filename);

    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<Face> faces;
};
