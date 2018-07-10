#include "camera.hpp"

#include "ray.hpp"

namespace rt {
namespace path {

Camera::Camera(glm::dvec3 position, glm::dvec3 target, int width, int height) {
  m_width = width;
  m_width_recp = 1. / m_width;
  m_height = height;
  m_height_recp = 1. / m_height;
  m_ratio = (double)m_width / m_height;

  m_position = position;
  m_direction = glm::normalize(target - m_position);
  m_x_direction =
      glm::normalize(glm::cross(glm::dvec3(0.0, 0.0, 1.0), m_direction * -1.0));
  m_y_direction = glm::normalize(glm::cross(m_x_direction, m_direction));

  m_x_spacing = (2.0 * m_ratio) / (double)m_width;
  m_y_spacing = (double)2.0 / (double)m_height;
  m_x_spacing_half = m_x_spacing * 0.5;
  m_y_spacing_half = m_y_spacing * 0.5;
}

int Camera::get_width() {
  return m_width;
}
int Camera::get_height() {
  return m_height;
}

// Returns ray from camera origin through pixel at x,y
Ray Camera::get_ray(int x, int y, bool jitter, unsigned short* Xi) {

  double x_jitter;
  double y_jitter;

  // If jitter == true, jitter point for anti-aliasing
  if (jitter) {
    x_jitter = (erand48(Xi) * m_x_spacing) - m_x_spacing_half;
    y_jitter = (erand48(Xi) * m_y_spacing) - m_y_spacing_half;

  } else {
    x_jitter = 0;
    y_jitter = 0;
  }

  glm::dvec3 pixel = m_position + m_direction * 2.0;
  pixel = pixel - m_x_direction * m_ratio +
          m_x_direction * ((x * 2 * m_ratio) * m_width_recp) + x_jitter;
  pixel = pixel + m_y_direction -
          m_y_direction * ((y * 2.0) * m_height_recp + y_jitter);

  return Ray(m_position, glm::normalize(pixel - m_position));
}
} // namespace path
} // namespace rt
