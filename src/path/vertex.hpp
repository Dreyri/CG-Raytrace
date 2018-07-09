#pragma once

#include <glm/glm.hpp>

namespace rt {
namespace path {
struct vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;

  vertex(const glm::vec3& pos, const glm::vec3& norm = glm::vec3(),
         const glm::vec4& col = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f})
      : position{pos}
      , normal{norm}
      , color{col} {
      }
};
} // namespace path
} // namespace rt
