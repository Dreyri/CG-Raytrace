#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace rt {
struct camera {
  glm::vec3 position;
  glm::quat rotation;

  float fov;

  uint32_t width;
  uint32_t height;

private:
  float m_aspectRatio;

public:
  constexpr camera(const glm::vec3& position, const glm::quat& rotation,
                   float fov, uint32_t width = 800, uint32_t height = 600)
      : position{position}
      , rotation{rotation}
      , fov{fov}
      , width{width}
      , height{height}
      , m_aspectRatio{(float)width / (float)height} {
  }

  inline float aspectRatio() const {
    return m_aspectRatio;
  }
};
} // namespace rt
