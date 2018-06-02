#include "sphere.hpp"

namespace rt {
sphere::sphere(const glm::vec3& position, float radius)
    : position{position}
    , radius{radius} {
}
} // namespace rt