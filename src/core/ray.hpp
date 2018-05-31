#pragma once

#include <glm/vec3.hpp>

namespace rt {
struct ray {
  glm::vec3 origin;
  glm::vec3 direction;

  ray(const glm::vec3& origin, const glm::vec3& direction);
};
} // namespace rt