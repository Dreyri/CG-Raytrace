#pragma once

#include <memory>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#include "ray.hpp"

namespace rt 
{
    class RaytracerSimple
    {
    public:
        std::unique_ptr<std::vector<std::vector<glm::uvec3>>> render();
    };
}