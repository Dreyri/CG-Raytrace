#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace rt {
namespace path {

class Texture {
private:
  unsigned width;
  unsigned height;
  bool loaded = false;
  std::vector<unsigned char> image;

public:
  explicit Texture(const char* filename);
  Texture(){};
  glm::dvec3 get_pixel(unsigned x, unsigned y) const;
  glm::dvec3 get_pixel(double u, double v) const;
  bool is_loaded() const;
};
} // namespace path
} // namespace rt
