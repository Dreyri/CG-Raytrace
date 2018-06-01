#include "sphere.hpp"

namespace rt {
sphere::sphere(const glm::vec3& position, float radius)
    : position{position}
    , m_radius{radius}
    , m_radius_pow2{m_radius * m_radius} {
}

float sphere::radius() const {
  return m_radius;
}
float sphere::radius_pow2() const {
  return m_radius_pow2;
}

void sphere::set_radius(float r) {
  m_radius = r;
  m_radius_pow2 = m_radius * m_radius;
}
} // namespace rt