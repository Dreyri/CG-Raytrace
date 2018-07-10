#pragma once

#include <algorithm>
#include <memory>

#include "aabb.hpp"
#include "camera.hpp"
#include "intersection.hpp"
#include "objects.hpp"
#include "ray.hpp"

namespace rt {
namespace path {
class scene {
private:
  rt::path::camera m_camera;
  glm::vec4 m_bg_color;
  std::vector<rt::path::object*> m_objects;

public:
  scene(const rt::path::camera& cam, const glm::vec4& col)
      : m_camera{cam}
      , m_bg_color{col}
      , m_objects{} {
  }
  inline const glm::vec4& background() const {
    return m_bg_color;
  }

  inline const rt::path::camera& camera() const {
    return m_camera;
  }

  inline void add(rt::path::object* obj) {
    m_objects.push_back(obj);
  }

  glm::vec4 trace_ray(const rt::path::ray<float>& r, uint32_t depth,
                      const rt::path::Rng& rng);

  std::optional<Intersection>
  calculateIntersection(const rt::path::ray<float>& r) const;
};
} // namespace path
} // namespace rt
