#pragma once

#include <memory>
#include <vector>

/*#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
*/

#include "defs.hpp"
#include "raytracer.hpp"

namespace rt 
{
    class RaytracerSimple : public Raytracer
    {
    public:
        void render(std::shared_ptr<RenderTarget> target) override;
    };
}