#include "ray.hpp"

#include <glm/glm.hpp>

namespace rt {
ray::ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin{origin}
    , direction{direction} {
}

std::optional<float> intersect(const ray& r, const sphere& sph) {
  auto l = sph.position - r.origin;
  auto tca = glm::dot(l, r.direction);

  // wrong direction
  if (tca < 0.0f) {
    return {};
  }

  auto d2 = glm::dot(l, l) - tca * tca;

  // doesn't intersect
  if (d2 > sph.radius_pow2()) {
    return {};
  }

  auto thc = sqrt(sph.radius_pow2() - d2);

  // get the depth of the intersection
  auto t0 = tca - thc;
  auto t1 = tca + thc;

  if (t0 > t1) {
    std::swap(t0, t1);
  }

  if (t0 < 0.0f) {
    t0 = t1;
    if (t0 < 0.0f) {
      return {};
    }
  }

  return t0;
}
} // namespace rt