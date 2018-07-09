#include "kdtree.hpp"

namespace rt {
namespace path {
KDNode* KDNode::build(const std::vector<triangle*>& tris, int depth) {
  KDNode* node = new KDNode();

  if (std::size(tris) == 0) {
    return node;
  }

  if (depth > 25 || std::size(tris) <= 6) {
    node->triangles = tris;
    node->leaf = true;
    node->box = tris[0]->bounding_box();

    for (size_t i = 1; i < std::size(tris); ++i) {
      node->box.expand(tris[i]->bounding_box());
    }

    node->left = new KDNode();
    node->right = new KDNode();

    return node;
  }

  node->box = tris[0]->bounding_box();
  glm::vec3 midpoint{};
  float tris_recp = 1.0f / std::size(tris);

  for (size_t i = 0; i < std::size(tris); ++i) {
    node->box.expand(tris[i]->bounding_box());
    midpoint = midpoint + (tris[i]->middle() * tris_recp);
  }

  std::vector<triangle*> left_tris;
  std::vector<triangle*> right_tris;

  int axis = node->box.longest_axis();

  for (size_t i = 0; i < std::size(tris); ++i) {
    switch (axis) {
    case 0:
      midpoint.x >= tris[i]->middle().x ? right_tris.push_back(tris[i])
                                        : left_tris.push_back(tris[i]);
      break;
    case 1:
      midpoint.y >= tris[i]->middle().y ? right_tris.push_back(tris[i])
                                        : left_tris.push_back(tris[i]);
      break;
    case 2:
      midpoint.z >= tris[i]->middle().z ? right_tris.push_back(tris[i])
                                        : left_tris.push_back(tris[i]);
    default:
      break;
    }
  }

  if (std::size(tris) == std::size(left_tris) ||
      std::size(tris) == std::size(right_tris)) {
    node->triangles = tris;
    node->leaf = true;
    node->box = tris[0]->bounding_box();

    for (size_t i = 0; i < std::size(tris); ++i) {
      node->box.expand(tris[i]->bounding_box());
    }

    node->left = new KDNode();
    node->right = new KDNode();

    return node;
  }

  node->left = build(left_tris, depth + 1);
  node->right = build(right_tris, depth + 1);

  return node;
}
  std::optional<std::tuple<float, glm::vec4, glm::vec3>> KDNode::hit(const rt::path::ray<float>& r, float tmin) {
  auto dist = box.intersects(r);
  if (dist) {
    if (*dist > tmin) {
      return {};
    }

    std::optional<std::tuple<float, glm::vec4, glm::vec3>> hit_left = {};
    std::optional<std::tuple<float, glm::vec4, glm::vec3>> hit_right = {};
    size_t tri_idx;

    if (!leaf) {
      hit_left = left->hit(r, tmin);

      hit_right = right->hit(r, tmin);

      if (hit_left) {
        return hit_left;
      } else if (hit_right) {
        return hit_right;
      }
    }
    else {
      bool hit_tri{false};
      glm::vec3 normal{};
      for (size_t i = 0; i < std::size(triangles); ++i) {
        auto tri_inter = triangles[i]->intersect(r, tmin);
        if (tri_inter) {
          hit_tri = true;
          tmin = std::get<float>(*tri_inter);
          normal = std::get<glm::vec3>(*tri_inter);
          tri_idx = i;
        }
      }

      if (hit_tri) {
        glm::vec3 pos = r.origin + r.direction * tmin;
        glm::vec4 col = triangles[tri_idx]->color_at(pos);

        return std::make_tuple(tmin, col, normal);
      }
    }
  }

  return {};
}
} // namespace path
} // namespace rt
