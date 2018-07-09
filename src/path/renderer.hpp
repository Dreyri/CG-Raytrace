#pragma once

#include <cstdint>
#include <cstdlib>
#include <limits>

#include "ray.hpp"
#include "scene.hpp"
#include "rendertarget.hpp"

namespace rt {
namespace path {
static constexpr float pi() {
  return std::atan(1.0f) * 4.0f;
}

class Rng {
public:
  Rng() {
  }

  inline void seed(int s) const {
    std::srand(s);
  }

  /**
   * generates a random number between 0 and 1
   */
  inline float operator()() const {
    int r = std::rand();
    float f = static_cast<float>(r) /
              static_cast<float>(std::numeric_limits<decltype(r)>::max());
    return f;
  }
};

class Renderer {
private:
  rt::path::Rng m_rng;

  uint32_t m_samples;
  uint32_t m_depth;

public:
  Renderer(uint32_t samples, uint32_t depth);

  rt::path::Image* render(rt::path::scene* scene);
  glm::vec4 traceRay(rt::path::scene* scene, const rt::path::ray<float>& r,
                     uint32_t curr_depth);
};
} // namespace path
} // namespace rt
