#pragma once

#include "objects.hpp"
#include "ray.hpp"

namespace rt {
namespace path {

class Scene {
private:
  std::vector<Object*> m_objects;

public:
  Scene(){};
  void add(Object* object);
  ObjectIntersection intersect(const Ray& ray);
  glm::dvec3 trace_ray(const Ray& ray, int depth, const Rng& rng);
};
} // namespace path
} // namespace rt
