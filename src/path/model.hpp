#pragma once

#include <memory>

#include "material.hpp"
#include "mesh.hpp"

namespace rt {
namespace path {
class Model {
public:
  using mesh_type = rt::path::mesh<uint32_t>;

private:
  mesh_type m_mesh;
  std::shared_ptr<Material> m_mat;

public:
  Model(const mesh_type& m, const std::shared_ptr<Material>& mat);

  inline const mesh_type& mesh() const {
    return m_mesh;
  }

  inline const Material* material() const {
    return m_mat.get();
  }
};
} // namespace path
} // namespace rt
