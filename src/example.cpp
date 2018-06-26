#include <memory>
#include <vector>

#include <QImage>
#include <QWindow>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "core/raytracer_simple.hpp"
#include "core/tracer_sphere.hpp"

int main(int argc, char** argv) {
  rt::RaytracerSimple tracer = rt::RaytracerSimple();
  auto rgbbuffer = tracer.render();

  QImage image = QImage(1920, 1080, QImage::Format::Format_RGB888);

  for (unsigned int h = 0; h < 1080; h++) {
    for (unsigned int w = 0; w < 1920; w++) {
      glm::uvec3 color = (*rgbbuffer)[h][w];
      image.setPixelColor(w, h, QColor(color.r, color.g, color.b));
    }
  }

  image.save("example.png", "PNG");

  rt::scene simple_scene;
  simple_scene.add_sphere({{0.0f, 0.0f, 0.0f}, 15.0f});
  rt::tracer_sphere sphere_raytracer;

  auto image_data = sphere_raytracer.render(simple_scene);
  std::vector<glm::tvec4<uchar>> converted_image_data(std::size(image_data));

  std::transform(std::begin(image_data), std::end(image_data),
                 std::begin(converted_image_data), [](const auto& v) -> glm::tvec4<uchar> {
                   return {v.x * 255.0f, v.y * 255.0f, v.z * 255.0f, 255};
                 });

  QImage sphere_image(reinterpret_cast<uchar*>(std::data(converted_image_data)), 1920, 1080, QImage::Format_RGBA8888, nullptr, nullptr);

  sphere_image.save("example_sphere.png", "PNG");
}
