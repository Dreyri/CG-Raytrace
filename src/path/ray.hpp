#pragma once

#include <glm/glm.hpp>

namespace rt {
namespace path {

struct Ray {
  glm::dvec3 origin, direction, direction_inv;
  Ray(glm::dvec3 o_, glm::dvec3 d_)
      : origin(o_)
      , direction(d_) {
    direction_inv = glm::dvec3(1. / direction.x, 1. / direction.y, 1. / direction.z);
  }
};
} // namespace path
} // namespace rt
