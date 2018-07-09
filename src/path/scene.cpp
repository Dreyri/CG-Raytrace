#include "scene.hpp"

#include "renderer.hpp"

namespace rt {
namespace path {
glm::vec4 scene::trace_ray(const rt::path::ray<float>& r, uint32_t depth,
                           const rt::path::Rng& rng) {
  auto intersect = calculateIntersection(r);

  if (!intersect) {
    return m_bg_color;
  }

  if (intersect->material.type() == EMIT) {
    return intersect->material.emission();
  }

  glm::vec4 color = intersect->material.color();

  float p = std::max(std::max(color.x, color.y), color.z);

  float rnd = rng();
  if (++depth > 5) {
    if (rnd < p * 0.9f) {
      color = color * (0.9f / p);
    } else {
      return intersect->material.emission();
    }
  }

  glm::vec3 x = r.origin + r.direction * intersect->distance;
  rt::path::ray<float> reflect =
      intersect->material.calculateReflectedRay(r, x, intersect->normal, rng);

  return color * trace_ray(reflect, depth, rng);
}

std::optional<Intersection>
scene::calculateIntersection(const rt::path::ray<float>& r) const {

  std::optional<Intersection> res = {};
  std::optional<Intersection> tmp = {};

  for (size_t i = 0; i < std::size(m_objects); ++i) {
    tmp = m_objects[i]->calculate_intersection(r);

    if (tmp) {
      if (!res || tmp->distance < res->distance) {
        res = tmp;
      }
    }
  }
  return res;
}
} // namespace path
} // namespace rt
