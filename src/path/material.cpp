#include "material.hpp"

#include "renderer.hpp"

namespace rt {
namespace path {

Material::Material(MaterialType type, const glm::vec4& color,
                   const glm::vec4& emission)
    : m_type{type}
    , m_color{color}
    , m_emission{emission} {
}

  glm::vec4 Material::color_at(float u, float v) {
    if (m_image) {
      return m_image->at_rel(u, v);
    }
  }

rt::path::ray<float> Material::calculateReflectedRay(
    const rt::path::ray<float>& r, const glm::vec3& origin,
    const glm::vec3& normal, const rt::path::Rng& rng) {
  if (m_type == SPECULAR) {
    float roughness = 0.8f;
    glm::vec3 reflected =
        r.direction - normal * 2.0f * glm::dot(normal, r.direction);

    reflected =
        glm::normalize(glm::vec3(reflected.x + (rng() - 0.5f) * roughness,
                                 reflected.y + (rng() - 0.5f) * roughness,
                                 reflected.z + (rng() - 0.5f) * roughness));

    return rt::path::ray<float>(origin, reflected);
  } else if (m_type == DIFFUSE) {
    glm::vec3 nl =
        glm::dot(normal, r.direction) < 0.0f ? normal : normal * -1.0f;
    float r1 = 2 * pi() * rng();
    float r2 = rng();
    float r2s = std::sqrt(r2);

    glm::vec3 w = nl;
    glm::vec3 u = ((std::abs(w.x) > 0.1f ? glm::vec3(0.0f, 1.0f, 0.0f)
                                         : glm::vec3(1.0f, 0.0f, 0.0f)));
    glm::vec3 v = glm::cross(w, u);

    glm::vec3 d =
        glm::normalize(u * std::cos(r1) * r2s + v * std::sin(r1) * r2s +
                       w * std::sqrt(1.0f - r2));

    return rt::path::ray<float>(origin, d);
  }

  throw std::logic_error("shouldn't be able to reach this");
}
} // namespace path
} // namespace rt
