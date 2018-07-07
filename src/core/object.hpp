#pragma once

#include <memory>

#include "defs.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "sphere.hpp"

namespace rt
{
    class Object
    {
    public:
        Mesh mesh;
        std::shared_ptr<Material> material;
        Sphere bounding;

        vec3 position;
        vec3 scale;
        quaternion rotation;

        Object(Mesh mesh, std::shared_ptr<Material> mat);

        mat4x4 getRotationMat();
        mat4x4 getScaleMat();
        mat4x4 getTranslateMat();

        void setRotation(floating degree, vec3 axis);
        void setScale(vec3 scale);
        void setPosition(vec3 position);

        unsigned int poly_first;
        unsigned int poly_last;
    };
}