#include "path/renderer.hpp"
#include "path/rendertarget.hpp"
#include "path/scene.hpp"

#include <QImage>

int main(int argc, char** argv) {
  rt::path::camera cam{glm::vec3(0.0f, -5.0f, 2.5f),
                       glm::vec3(0.0f, 0.0f, 1.0f), 1280, 720};

  std::vector<rt::path::vertex> my_vertices = {
      rt::path::vertex({-100.0f, -100.0f, 0.0f}),
      rt::path::vertex({100.0f, -100.0f, 0.0f}),
      rt::path::vertex({100.0f, 100.0f, 0.0f}),
      rt::path::vertex({-100.0f, 100.0f, 0.0f})};
  std::vector<uint32_t> my_indices = {0, 1, 2, 0, 2, 3};

  rt::path::mesh<uint32_t> my_mesh{my_vertices, my_indices};

  rt::path::Material* my_mat = new rt::path::Material(
      rt::path::EMIT, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f});

  std::shared_ptr<rt::path::Model> my_model = std::make_shared<rt::path::Model>(
      my_mesh, std::shared_ptr<rt::path::Material>());

  std::vector<std::shared_ptr<rt::path::Model>> my_model_vec{my_model};

  rt::path::scene my_scene{cam, {0.0f, 0.0f, 1.0f, 1.0f}, my_model_vec};

  rt::path::Renderer my_renderer{5, 5};

  auto img = my_renderer.render(&my_scene);

  QImage out_img{img->pixels(), static_cast<int>(img->width()),
                 static_cast<int>(img->height()), QImage::Format_RGBA8888};
  out_img.save("test.png");

  delete img;
}
