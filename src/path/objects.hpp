#pragma once

#include <optional>

#include <glm/glm.hpp>

#include "intersection.hpp"
#include "kdtree.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "triangle.hpp"

#include "third_party/tiny_obj_loader.hpp"

namespace rt {
namespace path {
class object {
protected:
  glm::vec3 m_position;

public:
  constexpr object(const glm::vec3& pos)
      : m_position{pos} {
  }

  virtual std::optional<Intersection>
  calculate_intersection(const rt::path::ray<float>& r) = 0;
};

class sphere final : public object {
private:
  float m_radius;
  rt::path::Material m_material;

public:
  sphere(const glm::vec3& pos, float radius, rt::path::Material mat);

  inline float radius() const {
    return m_radius;
  }

  inline rt::path::Material material() const {
    return m_material;
  }

  virtual std::optional<Intersection>
  calculate_intersection(const rt::path::ray<float>& r) override;
};

class mesh final : public object {
private:
  std::vector<tinyobj::shape_t> m_tshapes;
  std::vector<tinyobj::material_t> m_tmaterials;
  std::vector<rt::path::Material> m_materials;
  std::vector<rt::path::triangle*> m_triangles;

  rt::path::Material m_material;
  KDNode* m_node;

public:
  mesh(const glm::vec3& position, const std::string& file_path,
       rt::path::Material mat);

  virtual std::optional<Intersection>
  calculate_intersection(const rt::path::ray<float>& r) override;
};
} // namespace path
} // namespace rt
