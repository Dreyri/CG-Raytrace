#pragma once

#include <glm/glm.hpp>

namespace rt {
struct sphere {
  // TODO: add material
  glm::vec3 position;
  float radius;

  sphere(const glm::vec3& position, float radius);
};
} // namespace rt