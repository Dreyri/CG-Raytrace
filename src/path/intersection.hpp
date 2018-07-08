#pragma once

#include <glm/glm.hpp>

namespace rt {
namespace path {
struct Intersection {
  float distance;
  glm::vec3 normal;
  Material material;

  Intersection(float dist, const glm::vec3& norm, const Material& mat)
      : distance{dist}
      , normal{norm}
      , material{mat} {
  }
};
} // namespace path
} // namespace rt
