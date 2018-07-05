#pragma once

#include <memory>
#include <vector>

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