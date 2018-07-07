#pragma once

#include <memory>
#include <vector>

#include "defs.hpp"
#include "raytracer.hpp"

namespace rt 
{
    class RaytracerSimple : public Raytracer
    {
    private:
        std::shared_ptr<RenderTarget> target;

        void renderMulti(unsigned int start, unsigned int step);

    public:
        void render(std::shared_ptr<RenderTarget> target) override;
    };
}