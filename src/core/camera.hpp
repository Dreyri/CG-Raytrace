#pragma once

#include "defs.hpp"

namespace rt {
    struct Camera
    {
        vec3 position;
        vec3 direction;
        vec3 up{ 0.0, 1.0, 0.0 };
        vec3 camera_up;
        floating height, width;
        unsigned int imageHeight, imageWidth;
        unsigned int fov;

        int vOff, hOff;

        vec3 origin;
        vec3 topLeft;
        vec3 stepPixelRight;
        vec3 stepPixelDown;

        Camera();
        void setViewpane(unsigned int width, unsigned int height);
        void calculateDerived();
        vec3 centerOfPixel(unsigned int y, unsigned int x);
    };
} // namespace rt
