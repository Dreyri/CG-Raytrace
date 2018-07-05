#include "raytracer_simple.hpp"

#include <iostream>

void rt::RaytracerSimple::render(std::shared_ptr<rt::RenderTarget> target)
{
    unsigned int height = target->getHeight();
    unsigned int width = target->getWidth();

    for (unsigned int h = 0; h < height; h++)
    {
        for (unsigned int b = 0; b < width; b++)
        {
            if (h == 650 && b == 600)
                int asdjaid = 111;
            Ray ray = Ray(this->scene->camera.origin, glm::normalize(this->scene->camera.centerOfPixel(h, b) - this->scene->camera.origin));

            fColor color;

            if (trace(ray, color))
            {
                target->setPixel(b, h, color.r, color.g, color.b);
            }
            else
            {
                target->setPixel(b, h, 0.2f, 0.2f, 0.2f);
            }            
        }
    }
}