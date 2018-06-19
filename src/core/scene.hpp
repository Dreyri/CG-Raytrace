#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include "ray.hpp"
#include "sphere.hpp"

namespace rt {
class scene {
public:
  glm::vec3 background_color{0.0f, 0.0f, 0.0f};
private:

  std::vector<rt::sphere> m_spheres;
public:

  scene() = default;
  ~scene() = default;

  void add_sphere(const rt::sphere& sph);

  const std::vector<rt::sphere>& spheres() const;
};
} // namespace rt
