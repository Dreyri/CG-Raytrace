#include "scene.hpp"
#include "objects.hpp"

#include "renderer.hpp"

namespace rt {
namespace path {

void Scene::add(Object* object) {
  m_objects.push_back(object);
}

ObjectIntersection Scene::intersect(const Ray& ray) {
  ObjectIntersection isct = ObjectIntersection();
  ObjectIntersection temp;
  long size = m_objects.size();

  for (int i = 0; i < size; i++) {
    temp = m_objects.at((unsigned)i)->get_intersection(ray);

    if (temp.hit) {
      if (isct.u == 0 || temp.u < isct.u)
        isct = temp;
    }
  }
  return isct;
}

glm::dvec3 Scene::trace_ray(const Ray& ray, int depth, const Rng& rng) {

  ObjectIntersection isct = intersect(ray);

  // If no hit, return world colour
  if (!isct.hit)
    return glm::dvec3();

  if (isct.m.get_type() == EMIT)
    return isct.m.get_emission();

  glm::dvec3 colour = isct.m.get_colour();

  // Calculate max reflection
  double p = colour.x > colour.y && colour.x > colour.z
                 ? colour.x
                 : colour.y > colour.z ? colour.y : colour.z;

  // Russian roulette termination.
  // If random number between 0 and 1 is > p, terminate and return hit object's
  // emmission
  double rnd = rng();
  if (++depth > 5) {
    if (rnd <
        p * 0.9) { // Multiply by 0.9 to avoid infinite loop with colours of 1.0
      colour = colour * (0.9 / p);
    } else {
      return isct.m.get_emission();
    }
  }

  glm::dvec3 x = ray.origin + ray.direction * isct.u;
  Ray reflected = isct.m.get_reflected_ray(ray, x, isct.n, rng);

  return colour * (trace_ray(reflected, depth, rng));
}
} // namespace path
} // namespace rt
