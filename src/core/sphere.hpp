#pragma once

#include <glm/vec3.hpp>

namespace rt {
struct sphere {
  // TODO: add material
  glm::vec3 position;

private:
  float m_radius;

  // cached for intersect call
  float m_radius_pow2;

public:
  sphere(const glm::vec3& position, float radius);

  float radius() const;
  float radius_pow2() const;

  void set_radius(float r);
};
} // namespace rt