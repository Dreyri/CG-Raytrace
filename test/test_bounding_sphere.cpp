#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "core/boundingsphere.hpp"

TEST_CASE("bounding_sphere", "[core]") {
  rt::Model test_model{};
  test_model.vertices = {
      rt::vertex({1.0f, 1.0f, 1.0f}),    rt::vertex({1.0f, -1.0f, 1.0f}),
      rt::vertex({-1.0f, -1.0f, 1.0f}),  rt::vertex({-1.0f, 1.0f, 1.0f}),
      rt::vertex({1.0f, 1.0f, -1.0f}),    rt::vertex({1.0f, -1.0f, -1.0f}),
      rt::vertex({-1.0f, -1.0f, -1.0f}), rt::vertex({-1.0f, 1.0f, -1.0f})};
  // this should make a cube
  test_model.indices = {0, 1, 3, 1, 2, 3, 1, 0, 4, 5, 1, 4, 5, 4, 6, 6, 4, 7,
                        6, 7, 2, 7, 3, 2, 4, 0, 3, 4, 3, 7, 5, 2, 1, 5, 6, 2};

  SECTION("bounding sphere") {
    auto sphere = rt::calculateMinimalBoundingSphere(test_model);

    REQUIRE(sphere.radius ==
            Approx(glm::distance(glm::vec3(1.0f, 1.0f, 1.0f),
                                 glm::vec3(-1.0f, -1.0f, -1.0f)) * 0.5f));
    REQUIRE(sphere.position.x == Approx(0.0f));
    REQUIRE(sphere.position.y == Approx(0.0f));
    REQUIRE(sphere.position.z == Approx(0.0f));
  }
}
