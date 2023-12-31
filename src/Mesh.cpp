#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>

#include "Mesh.h"

Mesh::Mesh(std::string filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<Face> faces;

    char line[1024];

    while (fgets(line, 1024, file)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 position {};
            if (sscanf(line, "v %f %f %f", &position.x, &position.y, &position.z) != 3) {
                std::cerr << "Error reading position" << std::endl;
            }
            positions.push_back(position);
        }
        if (strncmp(line, "vn ", 2) == 0) {
            Vec3 normal {};
            if (sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z) != 3) {
                std::cerr << "Error reading normal" << std::endl;
            }
            normals.push_back(normal);
        }
        if (strncmp(line, "vt ", 3) == 0) {
            Vec2 texcoord;
            if (sscanf(line, "vt %f %f", &texcoord.x, &texcoord.y) != 2) {
                std::cerr << "Error reading texcoord" << std::endl;
            }
            texcoords.push_back(texcoord);
        }
        // Texture coordinate information
        if (strncmp(line, "f ", 2) == 0) {
            Face f = {};

            if (std::strchr(line, '/')) {
                if (sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                        &f.positions[0], &f.uvs[0], &f.normals[0],
                        &f.positions[1], &f.uvs[1], &f.normals[1],
                        &f.positions[2], &f.uvs[2], &f.normals[2])
                    != 9) {
                    std::cerr << "Error reading face" << std::endl;
                }
            } else {
                if (sscanf(line, "f %u %u %u", &f.positions[0], &f.positions[1], &f.positions[2])
                    != 3) {
                    std::cerr << "Error reading face" << std::endl;
                }
            }

            faces.push_back(f);
        }
    }

    for (auto const& face : faces) {
        Triangle t = {};
        for (u32 i = 0; i < 3; i++) {
            t.vertices[i].position = positions[face.positions[i] - 1].xyzw();
            t.vertices[i].normal = normals[face.normals[i] - 1];
            t.vertices[i].uv = texcoords[face.uvs[i] - 1];
        }
        triangles.push_back(t);
    }

    fclose(file);
}

// normalized_scale returns a Vec3 that can be used to scale the mesh
// to (-1, 1) on each coordinate.
Vec3 Mesh::normalized_scale() const
{
    if (triangles.empty()) {
        return Vec3(1.0f, 1.0f, 1.0f);
    }

    f32 min_x = std::numeric_limits<f32>::max();
    f32 max_x = std::numeric_limits<f32>::min();
    f32 min_y = std::numeric_limits<f32>::max();
    f32 max_y = std::numeric_limits<f32>::min();
    f32 min_z = std::numeric_limits<f32>::max();
    f32 max_z = std::numeric_limits<f32>::min();

    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle.vertices) {
            min_x = std::min(min_x, vertex.position.x);
            max_x = std::max(max_x, vertex.position.x);
            min_y = std::min(min_y, vertex.position.y);
            max_y = std::max(max_y, vertex.position.y);
            min_z = std::min(min_z, vertex.position.z);
            max_z = std::max(max_z, vertex.position.z);
        }
    }

    f32 size_x = max_x - min_x;
    f32 size_y = max_y - min_y;
    f32 size_z = max_z - min_z;

    f32 largest_dimension = std::max({ size_x, size_y, size_z });
    f32 scaling_factor = 2.0f / largest_dimension;

    return Vec3(scaling_factor, scaling_factor, scaling_factor);
}

// center_translation returns a Vec3 that can be used to translate the
// mesh to the center of (0, 0).
Vec3 Mesh::center_translation() const
{
    f32 min_x = std::numeric_limits<f32>::max();
    f32 max_x = std::numeric_limits<f32>::min();
    f32 min_y = std::numeric_limits<f32>::max();
    f32 max_y = std::numeric_limits<f32>::min();
    f32 min_z = std::numeric_limits<f32>::max();
    f32 max_z = std::numeric_limits<f32>::min();

    for (auto& t : triangles) {
        for (int i = 0; i < 3; i++) {
            // Min
            min_x = std::min(t.vertices[i].position.x, min_x);
            min_y = std::min(t.vertices[i].position.y, min_y);
            min_z = std::min(t.vertices[i].position.z, min_z);
            // max
            max_x = std::max(t.vertices[i].position.x, max_x);
            max_y = std::max(t.vertices[i].position.y, max_y);
            max_z = std::max(t.vertices[i].position.z, max_z);
        }
    }

    f32 x = -(max_x + min_x) / 2.0;
    f32 y = -(max_y + min_y) / 2.0;
    f32 z = -(max_z + min_z) / 2.0;

    return Vec3(x, y, z);
};
