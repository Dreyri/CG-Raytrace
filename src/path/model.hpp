#pragma once

#include <memory>
#include <optional>

#include <glm/gtc/matrix_transform.hpp>

#include "intersection.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "vertex.hpp"

namespace rt {
namespace path {
class Model {
public:
  using mesh_type = rt::path::mesh<uint32_t>;

private:
  mesh_type m_mesh;
  std::shared_ptr<Material> m_mat;
  glm::vec3 m_position;

public:
  Model(const mesh_type& m, const std::shared_ptr<Material>& mat,
        const glm::vec3& pos = glm::vec3());

  inline const mesh_type& mesh() const {
    return m_mesh;
  }

  inline const Material* material() const {
    return m_mat.get();
  }

  /**
   * returns intersection without material
   * checks if ray intersects the triangle formed by the 3 vertices and returns
   * interpolated normal and calculated distance
   */
  static std::optional<Intersection>
  intersectTriangle(rt::path::vertex* triangle, const rt::path::ray<float>& r,
                    const glm::mat4& transform) {
    glm::vec3 pos0 = glm::vec4(triangle[0].position, 1.0f) * transform;
    glm::vec3 pos1 = glm::vec4(triangle[1].position, 1.0f) * transform;
    glm::vec3 pos2 = glm::vec4(triangle[2].position, 1.0f) * transform;
    glm::vec3 edge1 = pos1 - pos0;
    glm::vec3 edge2 = pos2 - pos0;

    glm::vec3 tnorm = glm::normalize(glm::cross(edge1, edge2));

    float u, v, t_temp = 0.0f;

    glm::vec3 pvec = glm::cross(r.direction, edge2);
    float det = glm::dot(edge1, pvec);

    if (det == 0.0f) {
      return {};
    }
    float inv_det = 1.0f / det;
    glm::vec3 tvec = r.origin - pos0;
    u = glm::dot(tvec, pvec) * inv_det;

    if (u < 0.0f || u > 1.0f) {
      return {};
    }
    glm::vec3 qvec = glm::cross(tvec, edge1);
    v = glm::dot(r.direction, qvec) * inv_det;

    if (v < 0.0f || ((u + v) > 1.0f)) {
      return {};
    }

    t_temp = glm::dot(edge2, qvec) * inv_det;

    if (t_temp > 0.0001f) {
      return Intersection(t_temp, tnorm, triangle[0].color);
    }

    return {};
  }

  template<typename T>
  std::optional<Intersection> intersect(const rt::path::ray<T>& r) {
    glm::mat4 transformation;
    transformation = glm::translate(transformation, m_position);

    std::optional<Intersection> intersect;
    for (size_t i = 0; i < std::size(m_mesh.indices); i += 3) {
      std::optional<Intersection> tmp_intersect =
          intersectTriangle(&m_mesh.vertices[i], r, transformation);
      if (!intersect ||
          (tmp_intersect && tmp_intersect->distance < intersect->distance)) {
        if (m_mat) {
          tmp_intersect->material = *m_mat;
        }
        intersect = tmp_intersect;
      }
    }
    return intersect;
  }
};
} // namespace path
} // namespace rt
