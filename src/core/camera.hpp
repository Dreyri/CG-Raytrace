#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace rt {
struct camera {
  glm::vec3 position;
  glm::quat rotation;

  float fov;

  camera(const glm::vec3& position, const glm::quat& rotation, float fov)
      : position{position}
      , rotation{rotation}
      , fov{fov} {
  }
};
} // namespace rt
