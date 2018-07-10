#include "objects.hpp"

#include <iostream>

#include "triangle.hpp"

namespace rt {
namespace path {
sphere::sphere(const glm::vec3& pos, float radius, rt::path::Material mat)
    : object(pos)
    , m_radius{radius}
    , m_material{mat} {
}

std::optional<Intersection>
sphere::calculate_intersection(const rt::path::ray<float>& r) {
  float dist = 0.0f;

  glm::vec3 n{};

  glm::vec3 op = m_position - r.origin;
  float t;
  static constexpr float eps = 0.0001f;

  float b = glm::dot(op, r.direction);
  float det = b * b - glm::dot(op, op) + m_radius * m_radius;

  if (det < 0.0f) {
    return {};
  } else {
    det = std::sqrt(det);
  }

  dist = (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0.0f);
  if (dist != 0.0f) {
    n = glm::normalize((r.origin + r.direction * dist) - m_position);
    return rt::path::Intersection(dist, n, m_material);
  }

  return {};
}

mesh::mesh(const glm::vec3& position, const std::string& file_path,
           rt::path::Material mat)
    : object(position)
    , m_tshapes{}
    , m_tmaterials{}
    , m_materials{}
    , m_triangles{}
    , m_material{mat} {
  std::cout << "Loading '" << file_path << "'\n";

  size_t pos = file_path.find_last_of("/");
  std::string base_path = file_path.substr(0, pos + 1);

  std::string err = tinyobj::LoadObj(m_tshapes, m_tmaterials, file_path.c_str(),
                                     base_path.c_str());

  if (!err.empty()) {
    std::cerr << err << "\n";
    exit(1);
  }

  std::cout << "Generating KD Tree\n";

  for (const auto& mat : m_tmaterials) {
    std::string texture_path("");

    if (!mat.diffuse_texname.empty()) {
      if (mat.diffuse_texname[0] == '/') {
        texture_path = mat.diffuse_texname;
      }
      texture_path = base_path + mat.diffuse_texname;
      QImage img{};
      auto qtexpath = QString::fromStdString(texture_path);
      img.load(qtexpath);
      auto path_img = std::make_shared<rt::path::Image>(img);
      m_materials.push_back(Material(DIFFUSE, glm::vec3(1.0f, 1.0f, 1.0f),
                                     glm::vec3(), path_img));
    }

    else {
      m_materials.push_back(
          Material(DIFFUSE, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3()));
    }
  }

  // load triangles

  for (size_t i = 0; i < std::size(m_tshapes); ++i) {
    for (size_t j = 0; j < (std::size(m_tshapes[i].mesh.indices) / 3); ++j) {
      glm::vec3 v0{
          m_tshapes[i].mesh.positions[m_tshapes[i].mesh.indices[3 * j] * 3],
          m_tshapes[i].mesh.positions[m_tshapes[i].mesh.indices[3 * j] * 3 + 1],
          m_tshapes[i]
              .mesh.positions[m_tshapes[i].mesh.indices[3 * j] * 3 + 2]};

      glm::vec3 v1{
          m_tshapes[i].mesh.positions[m_tshapes[i].mesh.indices[3 * j + 1] * 3],
          m_tshapes[i]
              .mesh.positions[m_tshapes[i].mesh.indices[3 * j + 1] * 3 + 1],
          m_tshapes[i]
              .mesh.positions[m_tshapes[i].mesh.indices[3 * j + 1] * 3 + 2]};
      glm::vec3 v2{
          m_tshapes[i].mesh.positions[m_tshapes[i].mesh.indices[3 * j + 2] * 3],
          m_tshapes[i]
              .mesh.positions[m_tshapes[i].mesh.indices[3 * j + 2] * 3 + 1],
          m_tshapes[i]
              .mesh.positions[m_tshapes[i].mesh.indices[3 * j + 2] * 3 + 2]};

      glm::vec2 t0{};
      glm::vec2 t1{};
      glm::vec2 t2{};

      if (m_tshapes[i].mesh.indices[3 * j + 2] * 2 + 1 <
          std::size(m_tshapes[i].mesh.texcoords)) {
        t0 = glm::vec2(
            m_tshapes[i].mesh.texcoords[m_tshapes[i].mesh.indices[3 * j] * 2],
            m_tshapes[i]
                .mesh.texcoords[m_tshapes[i].mesh.indices[3 * j] * 2 + 1]);
        t1 = glm::vec2(
            m_tshapes[i]
                .mesh.texcoords[m_tshapes[i].mesh.indices[3 * j + 1] * 2],
            m_tshapes[i]
                .mesh.texcoords[m_tshapes[i].mesh.indices[3 * j + 1] * 2 + 1]);
        t2 = glm::vec2(
            m_tshapes[i]
                .mesh.texcoords[m_tshapes[i].mesh.indices[3 * j + 2] * 2],
            m_tshapes[i]
                .mesh.texcoords[m_tshapes[i].mesh.indices[3 * j + 2] * 2 + 1]);
      }

      if (m_tshapes[i].mesh.material_ids[j] >= 0 &&
          m_tshapes[i].mesh.material_ids[j] <
              static_cast<int>(std::size(m_materials))) {
        m_triangles.push_back(
            new triangle(v0, v1, v2, t0, t1, t2,
                         &m_materials[m_tshapes[i].mesh.material_ids[j]]));
      } else {
        m_triangles.push_back(
            new triangle(v0, v1, v2, t0, t1, t2, &m_material));
      }
    }
  }

  m_tshapes.clear();
  m_tshapes.shrink_to_fit();
  m_tmaterials.clear();
  m_tmaterials.shrink_to_fit();

  m_node = KDNode::build(m_triangles, 0);
}

std::optional<Intersection>
mesh::calculate_intersection(const rt::path::ray<float>& r) {
  float tmin = std::numeric_limits<float>::max();

  auto intersect = m_node->hit(r, tmin);

  if (intersect) {
    float dist = std::get<float>(*intersect);
    glm::vec4 col = std::get<glm::vec4>(*intersect);
    glm::vec3 normal = std::get<glm::vec3>(*intersect);
    return Intersection(dist, normal, Material(DIFFUSE, col));
  }

  return {};
}
} // namespace path
} // namespace rt
