#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"

namespace rt {
namespace path {
class Rng;
class camera {
private:
  size_t m_width;
  size_t m_height;

  float m_width_recp;
  float m_height_recp;

  float m_ratio;

  float m_x_spacing;
  float m_x_spacing_half;
  float m_y_spacing;
  float m_y_spacing_half;

  glm::vec3 m_position;
  glm::vec3 m_direction;

  glm::vec3 m_x_direction;
  glm::vec3 m_y_direction;

public:
  camera(const glm::vec3& pos, const glm::vec3& target, size_t width,
         size_t height);

  inline size_t width() const {
    return m_width;
  }

  inline size_t height() const {
    return m_height;
  }

  rt::path::ray<float> getRay(size_t x, size_t y,
                              const rt::path::Rng& rng) const;
};
} // namespace path
} // namespace rt
