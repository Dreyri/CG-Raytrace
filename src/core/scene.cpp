#include "scene.hpp"

namespace rt {

void scene::add_sphere(const rt::sphere& sph) {
  m_spheres.push_back(sph);
}

const std::vector<rt::sphere>& scene::spheres() const {
  return m_spheres;
}
} // namespace rt
