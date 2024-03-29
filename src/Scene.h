#pragma once

#include <vector>

#include "Light.h"
#include "Mesh.h"
#include "Texture.h"
#include "Triangle.h"

class Model {
public:
    Model(Mesh& mesh, const Texture& texture)
        : mesh(mesh)
        , texture(texture)
    {
        // Scale and translate the model initially so there aren't
        // massive models taking up the entire viewport. The UI has
        // functionality to revert to the original scale/translation.
        scale = mesh.normalized_scale();
        translation = mesh.center_translation();
    }

    Mesh& mesh;
    const Texture& texture;

    Vec3 scale = Vec3(1.0f, 1.0f, 1.0f);
    Vec3 rotation = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 translation = Vec3(0.0f, 0.0f, 0.0f);

    std::vector<Triangle> triangles_to_render;
};

struct Scene {
    std::vector<Model> models;
    std::vector<Light> lights;
    AmbientLight ambient_light;
};
