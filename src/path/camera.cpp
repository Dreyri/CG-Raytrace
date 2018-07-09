#include "camera.hpp"

#include "renderer.hpp"

namespace rt {
namespace path {
camera::camera(const glm::vec3& pos, const glm::vec3& target, size_t width,
               size_t height)
    : m_width{width}
    , m_height{height}
    , m_width_recp{1.0f / static_cast<float>(m_width)}
    , m_height_recp{1.0f / static_cast<float>(m_height)}
    , m_ratio{static_cast<float>(m_width) / static_cast<float>(m_height)}
    , m_position{pos}
    , m_direction{glm::normalize(target - m_position)}
    , m_x_direction{glm::normalize(
          glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), -1.0f * m_direction))}
    , m_y_direction{glm::normalize(glm::cross(m_x_direction, m_direction))}
    , m_x_spacing{(2.0f * m_ratio) / static_cast<float>(m_width)}
    , m_y_spacing{2.0f / static_cast<float>(m_height)}
    , m_x_spacing_half{0.5f * m_x_spacing}
    , m_y_spacing_half{0.5f * m_y_spacing} {
}

rt::path::ray<float> camera::getRay(size_t x, size_t y,
                                    const rt::path::Rng& rng) const {
  float x_jitter = (rng() * m_x_spacing) - m_x_spacing_half;
  float y_jitter = (rng() * m_y_spacing) - m_y_spacing_half;

  glm::vec3 pixel = m_position + m_direction * 2.0f;
  pixel -= m_x_direction * m_ratio +
           m_x_direction * ((x * 2.0f * m_ratio) * m_width_recp) + x_jitter;
  pixel +=
      m_y_direction - m_y_direction * ((y * 2.0f) * m_height_recp + y_jitter);

  return rt::path::ray<float>(m_position, glm::normalize(pixel - m_position));
}

} // namespace path
} // namespace rt