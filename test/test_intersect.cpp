#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <glm/glm.hpp>

#include "core/ray.hpp"
#include "core/sphere.hpp"

TEST_CASE("intersect", "[core]") {
  rt::ray test_ray({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
  test_ray.normalize();
  rt::ray wrong_dir_ray({0.0f, 0.0f, 0.0f},
                        {0.0f, 0.0f, -1.0f}); // provokes negative dot
  wrong_dir_ray.normalize();
  rt::ray missing_ray({0.0f, 0.0f, 0.0f},
                      {0.3f, 0.0f, 1.0f}); // provokes radius missing calc
  missing_ray.normalize();
  rt::sphere test_sphere({0.0f, 0.0f, 500.0f}, 10.0f);

  SECTION("sphere") {
    REQUIRE(rt::intersect(test_ray, test_sphere));
    REQUIRE(!rt::intersect(wrong_dir_ray, test_sphere));
    REQUIRE(!rt::intersect(missing_ray, test_sphere));

    rt::ray narrow_missing_ray({0.0f, 0.0f, 0.0f}, {0.021f, 0.0f, 1.0f});
    narrow_missing_ray.normalize();
    REQUIRE(!rt::intersect(narrow_missing_ray, test_sphere));

    rt::ray barely_hitting_ray({0.0f, 0.0f, 0.0f}, {0.01f, 0.0f, 1.0f});
    barely_hitting_ray.normalize();
    REQUIRE(*rt::intersect(barely_hitting_ray, test_sphere) < 500.0f);
  }

  SECTION("intersect check normalized") {
    rt::ray not_normalized({100.0f, 100.0f, 100.0f}, {1.0f, 1.0f, 1.0f});

    REQUIRE_THROWS(rt::intersect<true>(not_normalized, test_sphere));
  }
}