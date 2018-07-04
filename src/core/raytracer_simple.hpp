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
    class RenderTarget
    {
    public:
        virtual unsigned int getWidth() = 0;
        virtual unsigned int getHeight() = 0;
        virtual void setPixel(unsigned int w, unsigned int h, float r, float g, float b) = 0;
    };

    class RaytracerSimple
    {
    public:
        void render(std::shared_ptr<rt::RenderTarget> target);
    };
}