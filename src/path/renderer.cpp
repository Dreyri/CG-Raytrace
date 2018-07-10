#include "renderer.hpp"
#include "intersection.hpp"
#include "material.hpp"
#include "rendertarget.hpp"

#include "../core/camera.hpp"
#include "../core/scene.hpp"

#include "ray.hpp"

#include <iostream>
#include <optional>
#include <thread>

namespace rt {
namespace path {

/*
static rt::path::ray<float> monteCarloRay(const rt::camera& cam, float x,
                                          float y, const rt::path::Rng& rng) {
  // TODO use variance
  float aspectRatio = cam.aspectRatio();
  float px = (2.0f * ((x + 0.5f) / cam.width) - 1.0f) *
             std::tan(cam.fov / 2 * pi() / 180.0f) * aspectRatio;
  float py = (1.0f - 2.0f * ((y + 0.5f) / cam.height)) *
             std::tan(cam.fov / 2 * pi() / 180.0f);

  glm::vec3 dir(px, py, -1.0f);

  return rt::path::ray<float>(cam.position, glm::normalize(dir));
}
*/

Renderer::Renderer(uint32_t samples, uint32_t depth)
    : m_samples{samples}
    , m_depth{depth} {
  m_rng.seed(4); // chosen by random diceroll, guaranteed to be random
}

rt::path::Image* Renderer::render(rt::path::scene* scene) {
  size_t width = scene->camera().width();
  size_t height = scene->camera().height();

  rt::path::Image* target = new rt::path::Image(nullptr, width, height);

  float sample_strength = 1.0f / static_cast<float>(m_samples);

#pragma omp parallel for
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      glm::vec4 col{};

      for (uint32_t sample = 0; sample < m_samples; ++sample) {
        rt::path::ray<float> r = scene->camera().getRay(x, y, m_rng);
        col += scene->trace_ray(r, 0, m_rng);
      }

      col *= sample_strength;
      glm::tvec4<uint8_t> final_color;
      final_color.r = col.r * 255.0f;
      final_color.g = col.g * 255.0f;
      final_color.b = col.b * 255.0f;
      final_color.a = col.a * 255.0f;
      target->setColor(x, y, final_color);
    }

    printf("%zu out of %zu lines calculated\n", y, height);
  }
  return target;
}

glm::vec4 Renderer::traceRay(rt::path::scene* scene,
                             const rt::path::ray<float>& r, uint32_t depth) {
  std::optional<Intersection> intersect = scene->calculateIntersection(r);

  if (!intersect) {
    return scene->background();
  }

  if (intersect->material.type() == EMIT) {
    return intersect->material.emission();
  }

  glm::vec4 color = intersect->material.color();

  // max of all the color elements
  float p = color.r > color.g && color.r > color.b
                ? color.r
                : color.g > color.b ? color.g : color.b;

  float rnd = m_rng();

  // russian roulette termination after the minimum depth
  if (++depth > m_depth) {
    if (rnd < (p * 0.9f)) {
      color *= (0.9f / p);
    } else {
      return intersect->material.emission();
    }
  }

  glm::vec3 newray_orig = r.origin + (r.direction * intersect->distance);
  rt::path::ray<float> reflected = intersect->material.calculateReflectedRay(
      r, newray_orig, intersect->normal, m_rng);

  return color + traceRay(scene, reflected, depth);
}
} // namespace path
} // namespace rt
