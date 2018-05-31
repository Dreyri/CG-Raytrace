#include "ray.hpp"

namespace rt {
ray::ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin{origin}
    , direction{direction} {
}
} // namespace rt