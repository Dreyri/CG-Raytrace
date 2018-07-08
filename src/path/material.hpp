#pragma once

#include <glm/glm.hpp>

#include "../core/ray.hpp"

namespace rt {
namespace path {
class Rng;
enum MaterialType {
  DIFFUSE,
  SPECULAR,
  EMIT,
};
class Material {
private:
  MaterialType m_type{DIFFUSE};
  glm::vec4 m_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_emission{0.0f, 0.0f, 0.0f, 1.0f};
  // todo texture
public:
  Material() = default;
  Material(MaterialType type, const glm::vec4& color,
           const glm::vec4& emission);

  inline MaterialType type() const {
    return m_type;
  }

  inline const glm::vec4& color() const {
    return m_color;
  }

  inline glm::vec4 colorAt(float u, float v) const {
    return {1.0f, 0.0f, 0.0f, 1.0f};
  }

  inline const glm::vec4& emission() const {
    return m_emission;
  }

  rt::ray calculateReflectedRay(const rt::ray& r, const glm::vec3& origin,
                                const glm::vec3& normal, const Rng& rng);
};
} // namespace path
} // namespace rt
