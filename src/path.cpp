#include "path/renderer.hpp"
#include "path/rendertarget.hpp"
#include "path/scene.hpp"

#include <QImage>

int main(int argc, char** argv) {
  rt::path::camera cam{glm::vec3(0.0f, -5.0f, 2.5f),
                       glm::vec3(0.0f, 0.0f, 1.0f), 1280, 720};

  rt::path::scene scene(cam, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

  scene.add(new rt::path::sphere(glm::vec3{0.0f, 0.0f, -1000.0f}, 1000.0f,
                                 rt::path::Material()));
  scene.add(new rt::path::sphere(
      glm::vec3{-1004.0f, 0.0f, 0.0f}, 1000.0,
      rt::path::Material(rt::path::DIFFUSE, glm::vec3(0.85f, 0.4f, 0.4f))));
  scene.add(new rt::path::sphere(
      glm::vec3{1004.0f, 0.0f, 0.0f}, 1000.0f,
      rt::path::Material(rt::path::DIFFUSE, glm::vec3(0.4f, 0.4f, 0.85f))));
  scene.add(new rt::path::sphere(glm::vec3{0.0f, 1006.0f, 0.0f}, 1000.0f,
                                 rt::path::Material()));
  scene.add(new rt::path::sphere(
      glm::vec3{0.0f, 0.0f, 110.0f}, 100.0f,
      rt::path::Material(rt::path::EMIT, glm::vec3{1.0f, 1.0f, 1.0f},
                         glm::vec3{2.2f, 2.2f, 2.2f})));

  scene.add(new rt::path::mesh(
      glm::vec3{0.0f, 0.0f, 0.0f}, "../obj/dragon2.obj",
      rt::path::Material(rt::path::DIFFUSE, glm::vec3(0.9f, 0.9f, 0.9f))));

  rt::path::Renderer my_renderer{1, 1};

  auto img = my_renderer.render(&scene);

  QImage out_img{img->pixels(), static_cast<int>(img->width()),
                 static_cast<int>(img->height()), QImage::Format_RGBA8888};
  out_img.save("test.png");

  delete img;
}
