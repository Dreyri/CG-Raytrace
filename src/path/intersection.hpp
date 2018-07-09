#pragma once

#include <glm/glm.hpp>

#include "material.hpp"

namespace rt {
namespace path {
struct Intersection {
  float distance;
  glm::vec3 normal;
  glm::vec4 color;
  // glm::vec2 uv;
  Material material;

  Intersection(float dist, const glm::vec3& norm, const glm::vec4& color,
               const Material& mat = Material())
      : distance{dist}
      , normal{norm}
      , color{color}
      , material{mat} {
  }
};
} // namespace path
} // namespace rt
