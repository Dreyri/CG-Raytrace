#pragma once

#include <vector>
#include <optional>

#include "aabb.hpp"
#include "triangle.hpp"

namespace rt {
namespace path {
struct KDNode {

  AABB box{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)};
  KDNode* left{nullptr};
  KDNode* right{nullptr};

  std::vector<triangle*> triangles;
  bool leaf{false};

  KDNode() = default;;

  static KDNode* build(const std::vector<triangle*>& tris, int depth);

  /**
   * if hit occurs optional contains
   * distance, color, normal
   */
  std::optional<std::tuple<float, glm::vec4, glm::vec3> hit(const rt::path::ray<float>& r, float tmin);

};
} // namespace path
} // namespace rt
