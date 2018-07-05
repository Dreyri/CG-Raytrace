#include "ray.hpp"

rt::Ray::Ray(const vec3& origin, const vec3& direction)
    : origin{origin}
    , direction{direction} {
}
