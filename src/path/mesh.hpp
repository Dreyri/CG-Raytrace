#pragma once

#include <vector>

#include "vertex.hpp"

namespace rt {
namespace path {
template<typename I>
struct mesh {
  using vertex_type = rt::path::vertex;
  using index_type = I;

  std::vector<vertex> vertices;
  std::vector<index_type> indices;

  template<typename ContainerV, typename ContainerI>
  mesh(const ContainerV& vert, const ContainerI& idx)
      : vertices{std::begin(vert), std::end(vert)}
      , indices{std::begin(idx), std::end(idx)} {
  }

  template<typename F>
  void for_each(F func) {
    for (auto i : indices) {
      func(vertices[i]);
    }
  }
};
} // namespace path
} // namespace rt
