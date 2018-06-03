#pragma once

#include <memory>
#include <vector>

#include <glm\vec3.hpp>

#include "ray.hpp"

namespace rt 
{
    class RaytracerSimple
    {
    public:
        std::unique_ptr<std::vector<std::vector<glm::uvec3>>> render();
    };
}