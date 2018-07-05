#include "object.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

using namespace rt;

mat4x4 Object::getRotationMat()
{
    return glm::toMat4(this->rotation);
}

mat4x4 Object::getScaleMat()
{
    return glm::scale(mat4x4(1.0), this->scale);
}

mat4x4 Object::getTranslateMat()
{
    return glm::translate(mat4x4(1.0), this->position);
}

void Object::setRotation(floating degree, vec3 axis)
{
    this->rotation = glm::angleAxis(degree, axis);
}

void Object::setScale(vec3 scale)
{
    this->scale = scale;
}

void Object::setPosition(vec3 position)
{
    this->position = position;
}
