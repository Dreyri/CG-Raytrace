#include "ray.hpp"

#include <glm/glm.hpp>

namespace rt {
ray::ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin{origin}
    , direction{direction} {
}

void ray::normalize() {
  direction = glm::normalize(direction);
}
} // namespace rt