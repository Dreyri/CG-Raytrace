#pragma once

#include "camera.hpp"
#include "scene.hpp"

namespace rt {
namespace path {
class Renderer {
private:
  Scene* m_scene;
  Camera* m_camera;
  glm::dvec3* m_pixel_buffer;

public:
  Renderer(Scene* scene, Camera* camera);
  void render(int samples = 4);
  void save_image(const char* file_path);
};
} // namespace path
} // namespace rt
