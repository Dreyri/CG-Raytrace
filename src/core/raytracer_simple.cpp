#include "raytracer_simple.hpp"

#include <iostream>
#include <thread>
#include <vector>

void rt::RaytracerSimple::renderMulti(unsigned int start, unsigned int step)
{
    unsigned int height = target->getHeight();
    unsigned int width = target->getWidth();

    for (unsigned int h = 0; h < height; h++)
    {
        for (unsigned int b = start; b < width; b += step)
        {
            Ray ray = Ray(this->scene->camera.origin, glm::normalize(this->scene->camera.centerOfPixel(h, b) - this->scene->camera.origin));
            fColor color;
            trace(ray, color, 1, 1.0);
            target->setPixel(b, h, color.r, color.g, color.b);
        }
    }
}

void rt::RaytracerSimple::render(std::shared_ptr<rt::RenderTarget> target)
{
    this->target = target;
    unsigned int height = target->getHeight();
    unsigned int width = target->getWidth();
    this->scene->camera.setViewpane(width, height);
    this->scene->camera.calculateDerived();

    std::vector<std::thread> t = std::vector<std::thread>(numThreads);
    
    for (int i = 0; i < numThreads; ++i)
    {
        t[i] = std::thread(&RaytracerSimple::renderMulti, this, i, numThreads);
    }

    for (int i = 0; i < numThreads; ++i) {
        t[i].join();
    }
}