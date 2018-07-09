#include "model.hpp"

namespace rt {
namespace path {
Model::Model(const mesh_type& m, const std::shared_ptr<Material>& mat,
             const glm::vec3& pos)
    : m_mesh{m}
    , m_mat{mat}
    , m_position{pos} {
}
} // namespace path
} // namespace rt
