#pragma once

#include <memory>

#include "defs.hpp"
#include "mesh.hpp"
#include "material.hpp"

namespace rt
{
    class Object
    {
    public:
        Mesh mesh;
        std::shared_ptr<Material> material;
        //Sphere bounding;

        vec3 position;
        vec3 scale;
        quaternion rotation;

        unsigned int poly_first;
        unsigned int poly_last;
    };
}