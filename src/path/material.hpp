#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "ray.hpp"
#include "rendertarget.hpp"

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
  std::shared_ptr<rt::path::Image> m_image{nullptr};
  // todo texture
public:
  Material() = default;
  Material(MaterialType type, const glm::vec3& color,
           const glm::vec3& emission, const std::string& texture = std::string());

  Material(const Material&) = default;
  Material& operator=(const Material&) = default;

  inline MaterialType type() const {
    return m_type;
  }

  inline const glm::vec4& color() const {
    return m_color;
  }

  inline glm::vec4 color_at(float u, float v) const {
    return {1.0f, 0.0f, 0.0f, 1.0f};
  }

  inline const glm::vec4& emission() const {
    return m_emission;
  }

  glm::vec4 color_at(float u, float v);

  rt::path::ray<float> calculateReflectedRay(const rt::path::ray<float>& r, const glm::vec3& origin,
                                const glm::vec3& normal, const Rng& rng);
};
} // namespace path
} // namespace rt
