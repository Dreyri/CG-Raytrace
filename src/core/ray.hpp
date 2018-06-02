#pragma once

#include <optional>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_query.hpp>

#include "sphere.hpp"

namespace rt {
struct ray {
  glm::vec3 origin;
  glm::vec3 direction;

  ray(const glm::vec3& origin, const glm::vec3& direction);

  void normalize();
};

/**
 * returns depth of the intersection if it occurs, nullopt otherwise
 */
template<bool CheckNormalized = false>
std::optional<float> intersect(const ray& r,
                               const sphere& sph) noexcept(!CheckNormalized) {
  if constexpr (CheckNormalized) {
    if (!glm::isNormalized(r.direction, 0.001f)) {
      throw std::logic_error("ray is not normalized");
    }
  }

  auto relative_sphere_pos = sph.position - r.origin;
  auto sphere_ray_dot = glm::dot(relative_sphere_pos, r.direction);

  // sphere is behind the ray
  if (sphere_ray_dot < 0.0f) {
    return {};
  }

  // distance to sphere center squared
  float d2 = glm::dot(relative_sphere_pos, relative_sphere_pos) -
             sphere_ray_dot * sphere_ray_dot;
  if (d2 > (sph.radius * sph.radius)) {
    return {};
  }

  float thc = sqrt((sph.radius * sph.radius) - d2);

  float t0 = sphere_ray_dot - thc;
  float t1 = sphere_ray_dot - thc;

  if (t0 > t1) {
    std::swap(t0, t1);
  }

  if (t0 < 0) {
    t0 = t1;
    if (t0 < 0) {
      return {};
    }
  }

  return t0;
}
} // namespace rt