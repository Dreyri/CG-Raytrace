#include "texture.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <stdio.h>

#include <QImage>

namespace rt {
namespace path {
Texture::Texture(const char* filename) {
  QImage img{};
  img.load(filename);
  image = std::vector<unsigned char>(img.width() * img.height() * 4);
  printf("Loading texture: %s\n", filename);

  std::memcpy(std::data(image), img.bits(), std::size(image));

  loaded = true;
}

// Get pixel at texture coordinates x,y
glm::dvec3 Texture::get_pixel(unsigned x, unsigned y) const {

  if (!loaded)
    return (glm::dvec3(1, 0, 1));

  double r, g, b;
  r = (double)image.at(y * width * 4 + x) / 255.;
  g = (double)image.at(y * width * 4 + x + 1) / 255.;
  b = (double)image.at(y * width * 4 + x + 2) / 255.;
  return glm::dvec3(r, g, b);
}

// Get pixel at normalised texture coordinates u,v
glm::dvec3 Texture::get_pixel(double u, double v) const {

  if (!loaded)
    return (glm::dvec3(1, 0, 1));

  int x = (fmod(fabs(u), 1.0)) * (width - 1);
  int y = (1. - fmod(fabs(v), 1.0)) * (height - 1);
  // printf("%f, %f\n", u, v);
  try {
    double r, g, b;
    r = (double)image.at(y * width * 4 + x * 4) / 255.;
    g = (double)image.at(y * width * 4 + x * 4 + 1) / 255.;
    b = (double)image.at(y * width * 4 + x * 4 + 2) / 255.;
    return glm::dvec3(r, g, b);
  } catch (const std::out_of_range& e) {
    printf("error with uv, yx: %lf, %lf - %i, %i (width, height: %i, %i) \n", u,
           v, x, y, width, height);
    return glm::dvec3(0, 1, 0);
  }
}

// Check if texture is loaded
bool Texture::is_loaded() const {
  return loaded;
}
} // namespace path
} // namespace rt
