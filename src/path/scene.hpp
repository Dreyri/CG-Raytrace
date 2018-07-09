#pragma once

#include <algorithm>
#include <memory>

#include "../core/camera.hpp"
#include "aabb.hpp"
#include "intersection.hpp"
#include "model.hpp"
#include "ray.hpp"

namespace rt {
namespace path {
class scene {
private:
  rt::camera m_camera;
  glm::vec4 m_bg_color;
  std::vector<std::pair<rt::path::AABB, std::shared_ptr<Model>>> m_objects;

public:
  template<template<typename...> typename Container>
  scene(const rt::camera& cam, const glm::vec4& col,
        const Container<std::shared_ptr<Model>>& models)
      : m_camera{cam}
      , m_bg_color{col}
      , m_objects{std::size(models)} {
    std::transform(std::begin(models), std::end(models), std::begin(m_objects),
                   [](const auto& model) { AABB aabb{model.mesh.vertices}; });
  }
  inline const glm::vec4& background() const {
    return m_bg_color;
  }

  inline std::vector<std::shared_ptr<Model>> objects() const {
    std::vector<std::shared_ptr<Model>> res(std::size(m_objects));

    std::transform(std::begin(m_objects), std::end(m_objects), std::begin(res),
                   [](const auto& aabb_pair) { return aabb_pair.second; });

    return res;
  }

  inline const rt::camera& camera() const {
    return m_camera;
  }

  /**
   * returns the closest intersecting model or nullptr
   */
  inline Model* intersect(const rt::path::ray<float>& r) const {
    float distance = std::numeric_limits<float>::max();
    Model* res = nullptr;

    for (const auto& aabb_pair : m_objects) {
      const auto& aabb = aabb_pair.first;

      std::optional<float> dist = aabb.intersects(r);
      if (dist) {
        if (*dist < distance) {
          distance = *dist;
          res = aabb_pair.second.get();
        }
      }
    }
    return res;
  }

  template<typename T>
  inline std::optional<Intersection>
  calculateIntersection(const rt::path::ray<T>& r) const {
    Model* m = intersect(r);
    if (!m) {
      return {};
    }

    auto res = m->intersect(r);
    assert(res);
    return res;
  }
};
} // namespace path
} // namespace rt
