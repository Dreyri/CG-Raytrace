#include "boundingsphere.hpp"

namespace rt {

rt::sphere calculateMinimalBoundingSphere(const Model& model) {
  float biggestDistance{};

  glm::vec3 point1{};
  glm::vec3 point2{};

  for (auto it = std::begin(model.indices); it != std::end(model.indices);
       ++it) {
    for (auto it2 = it + 1; it2 != std::end(model.indices); ++it2) {
      auto vertex1 = model.vertices[*it];
      auto vertex2 = model.vertices[*it2];

      auto distance = glm::distance(vertex1.position, vertex2.position);

      // if a bigger distance than previously replace the points and distance
      if (distance > biggestDistance) {
        biggestDistance = distance;
        point1 = vertex1.position;
        point2 = vertex2.position;
      }
    }
  }

  // once we're through calculate the middle and generate our bounding sphere
  auto middle = rt::middle(point1, point2);

  return rt::sphere(middle, biggestDistance * 0.5f);
}
} // namespace rt
