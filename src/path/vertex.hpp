#pragma once

#include <glm/glm.hpp>

namespace rt {
namespace path {
struct vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;

  vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& col)
      : position{pos}
      , normal{norm}
      , color{col} {
  }
};
} // namespace path
} // namespace rt
