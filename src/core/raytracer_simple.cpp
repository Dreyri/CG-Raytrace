#include "raytracer_simple.hpp"

#include <iostream>

void rt::RaytracerSimple::render(std::shared_ptr<rt::RenderTarget> target)
{
    unsigned int height = target->getHeight();
    unsigned int width = target->getWidth();
    this->scene->camera.setViewpane(width, height);
    this->scene->camera.calculateDerived();

    for (unsigned int h = 0; h < height; h++)
    {
        for (unsigned int b = 0; b < width; b++)
        {
            Ray ray = Ray(this->scene->camera.origin, glm::normalize(this->scene->camera.centerOfPixel(h, b) - this->scene->camera.origin));
            fColor color;
            trace(ray, color, 1, 1.0);
            target->setPixel(b, h, color.r, color.g, color.b);         
        }
    }
}