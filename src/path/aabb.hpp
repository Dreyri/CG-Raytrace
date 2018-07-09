#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "ray.hpp"
#include "vertex.hpp"

namespace rt {
namespace path {
class AABB {
  glm::vec3 m_min{std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max()};
  glm::vec3 m_max{std::numeric_limits<float>::min(),
                  std::numeric_limits<float>::min(),
                  std::numeric_limits<float>::min()};

public:
  constexpr AABB() = default;

  template<template<typename...> typename Container>
  explicit AABB(const Container<rt::path::vertex>& vertices) {
    for (const auto& v : vertices) {
      if (v.position.x < m_min.x) {
        m_min.x = v.position.x;
      } else if (v.position.x > m_max.x) {
        m_max.x = v.position.x;
      }

      if (v.position.y < m_min.y) {
        m_min.y = v.position.y;
      } else if (v.position.y > m_max.y) {
        m_max.y = v.position.y;
      }

      if (v.position.z < m_min.z) {
        m_min.z = v.position.z;
      } else if (v.position.z > m_max.z) {
        m_max.z = v.position.z;
      }
    }
  }
  constexpr AABB(const glm::vec3& min, const glm::vec3& max)
      : m_min{min}
      , m_max{max} {
  }

  /**
   * branchless ray box intersect
   * from https://tavianator.com/fast-branchless-raybounding-box-intersections/
   *
   * returns intersection point
   */
  inline std::optional<float> intersects(const rt::path::ray<float>& r) const {
    float tx1 = (m_min.x - r.origin.x) / r.direction.x;
    float tx2 = (m_max.x - r.origin.x) / r.direction.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (m_min.y - r.origin.y) / r.direction.y;
    float ty2 = (m_max.y - r.origin.y) / r.direction.y;

    float tymin = std::min(ty1, ty2);
    float tymax = std::max(ty1, ty2);

    tmin = std::min(tmin, tymin);
    tmax = std::max(tmax, tymax);

    float tz1 = (m_min.z - r.origin.z) / r.direction.z;
    float tz2 = (m_max.z - r.origin.z) / r.direction.z;

    float tzmin = std::min(tz1, tz2);
    float tzmax = std::max(tz1, tz2);

    tmin = std::min(tmin, tzmin);
    tmax = std::max(tmin, tzmax);

    if (tmin > tmax) {
      return {};
    } else {
      return tmin;
    }
  }
};
} // namespace path
} // namespace rt
