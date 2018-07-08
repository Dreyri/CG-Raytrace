#pragma once

#include <glm/glm.hpp>

namespace rt {
namespace path {
template<typename T>
struct ray {
  using value_type = glm::tvec3<T>;

  value_type origin;
  value_type direction;

  constexpr ray(const value_type& origin, const value_type& direction)
      : origin{origin}
      , direction{direction} {
  }
};
} // namespace path
} // namespace rt
