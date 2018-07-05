#include "camera.hpp"

using namespace rt;

void Camera::calculateDerived()
{
    direction = glm::normalize(direction); // normalize view direction just in case
    vec3 toLeft = glm::cross(up, direction); // vector that points to the left in image plane

                                             // origin point of eye behind image rectangle        
    floating n = -1.0 * ((width / 2) / tan((floating)fov / 2));
    origin = position + (n * direction);

    // top left corner of image rectangle
    topLeft = ((width / 2) * toLeft) + ((height / 2) * up) + position; // Half width to the left, half height up from position

                                                                       // vector to describe a one pixel step to the right
    stepPixelRight = (-1.0f * (width / (floating)imageWidth)) * toLeft;

    // vector to describe a one pixel step downwards
    stepPixelDown = (-1.0f * (height / (floating)imageHeight)) * up;
}

// 0,0 at top left, y points down, x to the right
vec3 Camera::centerOfPixel(unsigned int y, unsigned int x)
{
    return topLeft + ((floating)x * stepPixelRight) + (0.5 * stepPixelRight) // x pixel to the right, starting with 0, add half a step to get to center of pixel
                   + ((floating)y * stepPixelDown) + (0.5 * stepPixelDown);  // y pixel down, starting with 0, add half a step to get to center of pixel
}