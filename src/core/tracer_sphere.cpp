#include "tracer_sphere.hpp"

namespace rt {
glm::vec3 tracer_sphere::cast(const rt::ray& r, const rt::scene& scene) const {
  glm::vec3 color = scene.background_color;

  trace_data trace_result = trace(r, scene);

  if (trace_result.sphere) {
    // TODO more complicated mechanism to determine color
    color = {1.0f, 0.0f, 0.0f};
  }

  return color;
}

std::vector<glm::vec3> render(const rt::scene& scene) const;

trace_data tracer_sphere::trace(const rt::ray& r,
                                const rt::scene& scene) const {

  float distance = std::numeric_limits<float>::max();
  const rt::sphere* hit_sphere = nullptr;

  for (const auto& sphere : scene.spheres()) {
    auto intersection = rt::intersect(r, sphere);
    if (intersection) {
      if (*intersection < distance) {
        distance = *intersection;
        hit_sphere = &sphere;
      }
    }
  }

  return {.distance = distance, .sphere = hit_sphere};
}
} // namespace rt
