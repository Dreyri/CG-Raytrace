#pragma once

#include <vector>

#include "ray.hpp"
#include "triangle.hpp"

namespace rt {
namespace path {

class KDNode {
public:
  AABBox box;
  KDNode* left;
  KDNode* right;
  std::vector<Triangle*> triangles;
  bool leaf;

  KDNode(){};
  KDNode* build(std::vector<Triangle*>& tris, int depth);
  bool hit(KDNode* node, const Ray& ray, double& t, double& tmin,
           glm::dvec3& normal, glm::dvec3& c);
};
} // namespace path
} // namespace rt
