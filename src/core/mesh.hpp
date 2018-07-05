#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "defs.hpp"
#include "material.hpp"

namespace rt
{
    struct Polygon
    {
        vec3 v1, v2, v3;
        vec3 n1, n2, n3;

        std::shared_ptr<Material> material;
    };

    class Mesh
    {
    private:
        static std::optional<Mesh> unityCube;
        static std::optional<Mesh> unitySphere;

    public:
        std::vector<Polygon> polygons;

        static Mesh getUnityCube();
        static Mesh getUnitySphere();
    };
}