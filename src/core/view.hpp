#pragma once

#include <glm/vec2.hpp>

#include "camera.hpp"
#include "scene.hpp"

namespace rt {
class view {
public:
private:
  scene* m_scene;
  camera m_camera;
  glm::vec2 m_viewport;

public:
};
} // namespace rt