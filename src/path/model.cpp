#include "model.hpp"

namespace rt {
namespace path {
Model::Model(const mesh_type& m, const std::shared_ptr<Material>& mat)
    : m_mesh{m}
    , m_mat{mat} {
}
} // namespace path
} // namespace rt
