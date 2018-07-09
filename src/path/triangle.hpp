#pragma once

#include <glm/glm.hpp>

#include "aabb.hpp"
#include "material.hpp"

namespace rt {
namespace path {
struct triangle {
  glm::vec3 v0, v1, v2;
  glm::vec3 e1, e2;
  glm::vec3 n;
  glm::vec2 t0, t1, t2;

  rt::path::Material* material;

  triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
           const glm::vec2& t0, const glm::vec2& t1, const glm::vec2& t2,
           rt::path::Material* mat)
      : v0{v0}
      , v1{v1}
      , v2{v2}
      , e1{v1 - v0}
      , e2{v2 - v0}
      , n{glm::normalize(glm::cross(e1, e2))}
      , t0{t0}
      , t1{t1}
      , t2{t2}
      , material{mat} {
  }

  rt::path::AABB bounding_box() const {
    glm::vec3 min{std::min(std::min(v0.x, v1.x), v2.x),
                  std::min(std::min(v0.y, v1.y), v2.y),
                  std::min(std::min(v0.z, v1.z), v2.z)};

    glm::vec3 max{std::max(std::max(v0.x, v1.x), v2.x),
                  std::max(std::max(v0.y, v1.y), v2.y),
                  std::max(std::max(v0.z, v1.z), v2.z)};

    return rt::path::AABB(min, max);
  }

  glm::vec3 middle() const {
    return (v0 + v1 + v2) / 3.0f;
  }

  std::optional<std::pair<float, glm::vec3>> intersect(rt::path::ray<float>& r,
                                                       float tmin) const {
    glm::vec3 pvec = glm::cross(r.direction, e2);
    float det = glm::dot(e1, pvec);

    if (det == 0.0f) {
      return {};
    }

    float inv_det = 1.0f / det;
    auto tvec = r.origin - v0;

    float u = glm::dot(tvec, pvec) * inv_det;

    if (u < 0.0f || u > 1.0f) {
      return {};
    }

    glm::vec3 qvec = glm::cross(tvec, e1);

    float v = glm::dot(r.direction, qvec) * inv_det;

    if (v < 0.0f || u + v > 1.0f) {
      return {};
    }

    float t_tmp = glm::dot(e2, qvec) * inv_det;

    if (t_tmp < tmin) {
      if (t_tmp > 0.0001f) {
        return std::make_pair(t_tmp, n);
      }
    }

    return {};
  }

  /**
   * coordinates of point p on the triangle
   */
  glm::vec3 barycentric(const glm::vec3& p) const {
    glm::vec3 v2_ = p - v0;

    float f00 = glm::dot(e1, e1);
    float f01 = glm::dot(e1, e2);
    float f11 = glm::dot(e2, e2);
    float f20 = glm::dot(v2_, e1);
    float f21 = glm::dot(v2_, e2);

    float f = f00 * f11 - f01 * f01;
    float v = (f11 * f20 - f01 * f21) / f;
    float w = (f00 * f21 - f01 * f20) / f;
    float u = 1 - v - w;

    return glm::vec3(u, v, w);
  }

  glm::vec4 color_at(const glm::vec3& p) const {
    if (!material) {
      return glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    }

    glm::vec3 b = barycentric(p);
    glm::vec3 c{};

    c += (glm::vec3(t0, 0.0f) * b.x);
    c += (glm::vec3(t1, 0.0f) * b.y);
    c += (glm::vec3(t2, 0.0f) * b.z);

    return material->color_at(c.x, c.y);
  }
};
} // namespace path
} // namespace rt
