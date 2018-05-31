#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <string>

TEST_CASE("example", "[cg]") {
  std::string hello("hello world");

  SECTION("hello world") {
    REQUIRE(hello.compare("hello world") == 0);
  }
}