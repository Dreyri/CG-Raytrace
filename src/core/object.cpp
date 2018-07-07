#include "object.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

using namespace rt;

rt::Object::Object(Mesh mesh, std::shared_ptr<Material> mat) : position{vec3(0.0)}, scale{vec3(1.0)}, rotation{quaternion()}, mesh{mesh}, material{mat}, poly_first{0}, poly_last{0}, bounding{Sphere()}
{
}

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