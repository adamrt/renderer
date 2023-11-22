#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "Mesh.h"

Mesh::Mesh(std::vector<Vec3> _vertices, std::vector<Face> _faces)
    : vertices(_vertices)
    , faces(_faces)
{
}

Mesh::Mesh(std::string filename)
{
    FILE* file;
    file = fopen(filename.c_str(), "r");
    char line[1024];

    while (fgets(line, 1024, file)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            Vec3 vertex {};
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }
        if (strncmp(line, "vt ", 3) == 0) {
            Vec2 texcoord;
            sscanf(line, "vt %f %f", &texcoord.x, &texcoord.y);
            texcoords.push_back(texcoord);
        }
        if (strncmp(line, "n ", 2) == 0) {
            Vec3 vertex {};
            sscanf(line, "n %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            normals.push_back(vertex);
        }
        // Texture coordinate information
        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];

            if (std::strchr(line, '/')) {
                sscanf(
                    line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                    &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                    &vertex_indices[2], &texture_indices[2], &normal_indices[2]);
            } else {
                sscanf(
                    line, "f %d %d %d",
                    &vertex_indices[0],
                    &vertex_indices[1],
                    &vertex_indices[2]);
            }

            Face face = {};
            face.a = (uint32_t)vertex_indices[0];
            face.b = (uint32_t)vertex_indices[1];
            face.c = (uint32_t)vertex_indices[2];
            face.ta = (uint32_t)texture_indices[0];
            face.tb = (uint32_t)texture_indices[1];
            face.tc = (uint32_t)texture_indices[2];
            face.color = Color::White;
            faces.push_back(face);
        }
    }

    fclose(file);
}
