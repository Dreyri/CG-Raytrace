#pragma once

#include <glm/vec3.hpp>

namespace rt {
struct sphere {
  // TODO: add material
  glm::vec3 color{1.0f, 0.0f, 0.0f};

  glm::vec3 position;
  float radius;

public:
  sphere(const glm::vec3& position, float radius);
};
} // namespace rt
