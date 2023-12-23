#include <cstdio>
#include <cstring>
#include <iostream>
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
