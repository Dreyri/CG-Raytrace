#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "sphere.hpp"

namespace rt {

struct vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;

  vertex(const glm::vec3& position, const glm::vec3& normal = glm::vec3(),
         const glm::vec4& color = glm::vec4())
      : position{position}
      , normal{normal}
      , color{color} {
  }
};

struct Model {
  std::vector<vertex> vertices;
  std::vector<uint16_t> indices;
};

template<typename T>
glm::tvec3<T> middle(const glm::tvec3<T>& vec1, const glm::tvec3<T>& vec2) {
  glm::tvec3<T> res;
  res.x = (vec1.x + vec2.x) * T(0.5f);
  res.y = (vec1.y + vec2.y) * T(0.5f);
  res.z = (vec1.z + vec2.z) * T(0.5f);

  return res;
}

/**
 * calculate distance between each vertex and the longest distance will be used
 * for the bounding sphere very inefficient but works for our purpose as it only
 * has to be done once
 */
rt::sphere calculateMinimalBoundingSphere(const Model& model);
} // namespace rt
