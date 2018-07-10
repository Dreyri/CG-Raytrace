#include "renderer.hpp"

#include <iostream>
#include <stdio.h>
#include <vector>

#include <QImage>

namespace rt {
namespace path {

// Clamp double to min/max of 0/1
inline double clamp(double x) {
  return x < 0 ? 0 : x > 1 ? 1 : x;
}
// Clamp to between 0-255
inline int toInt(double x) {
  return int(clamp(x) * 255 + .5);
}

Renderer::Renderer(Scene* scene, Camera* camera) {
  m_scene = scene;
  m_camera = camera;
  m_pixel_buffer =
      new glm::dvec3[m_camera->get_width() * m_camera->get_height()];
}

void Renderer::render(int samples) {
  int width = m_camera->get_width();
  int height = m_camera->get_height();
  double samples_recp = 1. / samples;

// Main Loop
#pragma omp parallel for schedule(dynamic, 1) // OpenMP
  for (int y = 0; y < height; y++) {
    //unsigned short Xi[3] = {0, 0, y * y * y}; // Stores seed for erand48
    Rng rng;
    rng.seed(4);

    fprintf(stderr, "\rRendering (%i samples): %.2f%% ", // Prints
            samples, (double)y / height * 100);          // progress

    for (int x = 0; x < width; x++) {
      glm::dvec3 col = glm::dvec3();

      for (int a = 0; a < samples; a++) {
        Ray ray = m_camera->get_ray(x, y, a > 0, rng);
        col = col + m_scene->trace_ray(ray, 0, rng);
      }

      m_pixel_buffer[(y)*width + x] = col * samples_recp;
    }
  }
}

void Renderer::save_image(const char* file_path) {
  int width = m_camera->get_width();
  int height = m_camera->get_height();

  std::vector<unsigned char> pixel_buffer;

  int pixel_count = width * height;

  for (int i = 0; i < pixel_count; i++) {
    pixel_buffer.push_back(toInt(m_pixel_buffer[i].x));
    pixel_buffer.push_back(toInt(m_pixel_buffer[i].y));
    pixel_buffer.push_back(toInt(m_pixel_buffer[i].z));
    pixel_buffer.push_back(255);
  }

  QImage img(std::data(pixel_buffer), width, height, QImage::Format_RGBA8888);
  img.save(file_path);

  pixel_buffer.clear();
}
} // namespace path
} // namespace rt
