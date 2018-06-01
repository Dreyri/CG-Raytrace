#pragma once

#include <optional>

#include <glm/vec3.hpp>

#include "sphere.hpp"

namespace rt {
struct ray {
  glm::vec3 origin;
  glm::vec3 direction;

  ray(const glm::vec3& origin, const glm::vec3& direction);
};

/**
 * returns depth of the intersection if it occurs, nullopt otherwise
 */
std::optional<float> intersect(const ray& r, const sphere& sph);
} // namespace rt