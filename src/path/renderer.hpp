#pragma once

#include <cstdint>
#include <cstdlib>
#include <limits>

#include "ray.hpp"

namespace rt {
namespace path {
class RenderTarget;

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

public:
  Renderer();

  void render(RenderTarget* targ, uint32_t samples = 1, uint32_t depth = 1);
  glm::vec4 traceRay(const rt::path::ray<float>& r, uint32_t depth,
                     uint32_t min_depth);
};
} // namespace path
} // namespace rt
