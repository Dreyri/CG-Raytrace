#pragma once

#include "scene.hpp"

namespace rt {
struct trace_data {
  float distance;
  const rt::sphere* sphere;
};

class tracer_sphere {
private:
public:
  glm::vec3 cast(const rt::ray& r, const rt::scene& scene) const;

  std::vector<glm::vec3> render(const rt::scene& scene) const;

private:
  trace_data trace(const rt::ray& r, const rt::scene& scene) const;
};
} // namespace rt
