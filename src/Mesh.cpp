#include "Mesh.h"

#include <vector>

Mesh::Mesh(std::vector<Vec3> _vertices, std::vector<Face> _faces)
    : vertices(_vertices)
    , faces(_faces)
{
}
