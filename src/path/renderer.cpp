#include "renderer.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "rendertarget.hpp"

#include "../core/camera.hpp"

#include "ray.hpp"

#include <optional>

namespace rt {
namespace path {

static rt::path::ray<float> monteCarloRay(const rt::camera& cam, float x,
                                          float y, const rt::path::Rng& rng) {
  float aspectRatio = cam.aspectRatio();
  float px = (2.0f * ((x + 0.5f) / cam.width) - 1.0f) *
             std::tan(cam.fov / 2 * pi() / 180.0f) * aspectRatio;
  float py = (1.0f - 2.0f * ((y + 0.5f) / cam.height)) *
             std::tan(cam.fov / 2 * pi() / 180.0f);

  glm::vec3 dir(px, py, -1.0f);

  return rt::path::ray<float>(cam.position, glm::normalize(dir));
}

Renderer::Renderer() {
  m_rng.seed(4); // chosen by random diceroll, guaranteed to be random
}

void Renderer::render(RenderTarget* targ, uint32_t samples, uint32_t depth) {
  size_t width = targ->width();
  size_t height = targ->height();

  float sample_strength = 1.0f / static_cast<float>(samples);

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      glm::vec4 col{};

      for (uint32_t sample = 0; sample < samples; ++sample) {
        rt::ray r = monteCarloRay(m_scene->camera(), x, y, m_rng);
        col += traceRay(r, 0, depth);
      }

      col *= sample_strength;
      targ->setColor(x, y, col);
    }

    printf("%zu out of %zu lines calculated\n", y, height);
  }
}

glm::vec4 Renderer::traceRay(const rt::path::ray<float>& r, uint32_t depth,
                             uint32_t min_depth) {
  std::optional<Intersection> intersect = m_scene->calculateIntersection(r);

  if (!intersect) {
    return m_scene->backgroundColor();
  }

  if (intersect->material.getType() == EMIT) {
    return intersect->material.emission();
  }

  glm::vec4 color = intersect->material.getColour();

  // max of all the color elements
  float p = color.r > color.g && color.r > color.b
                ? color.r
                : color.g > color.b ? color.g : color.b;

  float rnd = m_rng();

  // russian roulette termination after the minimum depth
  if (++depth > min_depth) {
    if (rnd < (p * 0.9f)) {
      color *= (0.9f / p);
    } else {
      return intersection->material.emission();
    }
  }

  glm::vec3 newray_orig = r.origin + (r.direction * intersect->distance);
  rt::ray reflected = intersect->material.calculateReflectedRay(
      r, newray_orig, intersection.normal, m_rng);

  return color * traceRay(reflected, depth, min_depth);
}
} // namespace path
} // namespace rt