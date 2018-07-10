#include "camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

using namespace rt;

Camera::Camera() : up{vec3(0.0, 1.0, 0.0)}, height{1.0}
{
}

void rt::Camera::setViewpane(unsigned int width, unsigned int height)
{
    this->width = this->height * ((rt::floating)width / (rt::floating)height);
    this->imageHeight = height;
    this->imageWidth = width;
}

void Camera::calculateDerived()
{
    direction = glm::normalize(direction); // normalize view direction just in case
    vec3 toLeft = glm::normalize(glm::cross(up, direction)); // vector that points to the left in image plane - up refers to global scene upwards
    camera_up = glm::normalize(glm::cross(direction, toLeft)); // vector that points up for camera

    quaternion mV = glm::angleAxis(glm::radians((floating)vOff), toLeft);
    quaternion mH = glm::angleAxis(glm::radians((floating)hOff), camera_up);
    vec3 d = glm::toMat4(mV + mH) * vec4(direction, 1.0);
    vec3 left = glm::toMat4(mV + mH) * vec4(toLeft, 1.0);
    vec3 u = glm::toMat4(mV + mH) * vec4(camera_up, 1.0);

    // origin point of eye behind image rectangle        
    floating n = -1.0 * ((width / 2) / tan((floating)fov / 2));
    origin = position + (n * d);

    // top left corner of image rectangle
    topLeft = ((width / 2) * left) + ((height / 2) * u) + position; // Half width to the left, half height up from position

    // vector to describe a one pixel step to the right
    stepPixelRight = (-1.0f * (width / (floating)imageWidth)) * left;

    // vector to describe a one pixel step downwards
    stepPixelDown = (-1.0f * (height / (floating)imageHeight)) * u;
}

// 0,0 at top left, y points down, x to the right
vec3 Camera::centerOfPixel(unsigned int y, unsigned int x)
{
    return topLeft + ((floating)x * stepPixelRight) + (0.5 * stepPixelRight) // x pixel to the right, starting with 0, add half a step to get to center of pixel
                   + ((floating)y * stepPixelDown) + (0.5 * stepPixelDown);  // y pixel down, starting with 0, add half a step to get to center of pixel
}