#pragma once

#include "camera.hpp"
#include "scene.hpp"

namespace rt {
namespace path {
struct Rng {
  Rng() {
  }

  void seed(int seed) {
    srand(seed);
  }

  /**
   * random number between 0.0 and 1.0
   */
  inline double operator()() const {
    return static_cast<double>(rand()) /
           static_cast<double>(std::numeric_limits<int>::max());
  }
};
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
