#pragma once

#include <cstdint>
#include <cstdlib>
#include <limits>

#include "core/ray.hpp"

namespace rt {
class RenderTarget;

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

class PathTracer {
private:
  rt::Rng m_rng;

public:
  PathTracer();

  void render(RenderTarget* targ, uint32_t samples = 1, uint32_t depth = 1);
  glm::vec4 traceRay(const rt::ray& r, uint32_t depth, uint32_t min_depth);
};
} // namespace rt
