#include "material.hpp"

#include <stdexcept>
#include <stdlib.h>

#include "ray.hpp"

namespace rt {
namespace path {

Material::Material(MaterialType t, glm::dvec3 c, glm::dvec3 e, Texture tex) {
  m_type = t, m_colour = c, m_emission = e;
  m_texture = tex;
}

MaterialType Material::get_type() const {
  return m_type;
}
glm::dvec3 Material::get_colour() const {
  return m_colour;
}

// Get colour at UV coordinates u,v
glm::dvec3 Material::get_colour_at(double u, double v) const {
  if (m_texture.is_loaded())
    return m_texture.get_pixel(u, v);

  return m_colour;
}
glm::dvec3 Material::get_emission() const {
  return m_emission;
}

Ray Material::get_reflected_ray(const Ray& r, glm::dvec3& p,
                                const glm::dvec3& n, unsigned short* Xi) const {
  // Ideal specular reflection
  if (m_type == SPEC) {
    double roughness = 0.8;
    glm::dvec3 reflected = r.direction - n * 2.0 * glm::dot(n, r.direction);
    reflected = glm::normalize(
        glm::dvec3(reflected.x + (erand48(Xi) - 0.5) * roughness,
                   reflected.y + (erand48(Xi) - 0.5) * roughness,
                   reflected.z + (erand48(Xi) - 0.5) * roughness));

    return Ray(p, reflected);
  }
  // Ideal diffuse reflection
  if (m_type == DIFF) {
    glm::dvec3 nl = glm::dot(n, r.direction) < 0 ? n : n * -1.0;
    double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
    glm::dvec3 w = nl,
               u = glm::normalize(
                   (fabs(w.x) > .1 ? glm::dvec3(0.0, 1.0, 0.0)
                                   : glm::cross(glm::dvec3(1.0, 0.0, 0.0), w))),
               v = glm::cross(w, u);
    glm::dvec3 d = glm::normalize(u * cos(r1) * r2s + v * sin(r1) * r2s +
                                  w * sqrt(1 - r2));

    return Ray(p, d);
  }

  throw std::logic_error("Can't get here");
}

} // namespace path
} // namespace rt
