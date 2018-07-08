#include "material.hpp"

#include "renderer.hpp"

namespace rt {
namespace path {

static constexpr float pi() {
  return std::atan(1.0f) * 4.0f;
}

Material::Material(MaterialType type, const glm::vec4& color,
                   const glm::vec4& emission)
    : m_type{type}
    , m_color{color}
    , m_emission{emission} {
}

rt::ray Material::calculateReflectedRay(const rt::ray& r,
                                        const glm::vec3& origin,
                                        const glm::vec3& normal,
                                        const rt::path::Rng& rng) {
  if (m_type == SPECULAR) {
    float roughness = 0.8f;
    glm::vec3 reflected =
        r.direction - normal * 2.0f * glm::dot(normal, r.direction);

    reflected =
        glm::normalize(glm::vec3(reflected.x + (rng() - 0.5f) * roughness,
                                 reflected.y + (rng() - 0.5f) * roughness,
                                 reflected.z + (rng() - 0.5f) * roughness));

    return rt::ray(origin, reflected);
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

    return rt::ray(origin, d);
  }
}
} // namespace path
} // namespace rt
