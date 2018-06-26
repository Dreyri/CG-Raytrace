#include "tracer_sphere.hpp"

#include "camera.hpp"

namespace rt {
glm::vec3 tracer_sphere::cast(const rt::ray& r, const rt::scene& scene) const {
  glm::vec3 color = scene.background_color;

  trace_data trace_result = trace(r, scene);

  if (trace_result.sphere) {
    // TODO more complicated mechanism to determine color
    color = {1.0f, 0.0f, 0.0f};
  }

  return color;
}

std::vector<glm::vec3> tracer_sphere::render(const rt::scene& scene) const {
  int width{1920};
  int height{1080};
  int halfwidth = width / 2;
  int halfheight = height / 2;

  std::vector<glm::vec3> res;
  res.reserve(width * height);
  std::vector<rt::ray> rays;
  rays.reserve(width * height);

  rt::camera cam{{0.0f, 0.0f, -20.0f}, glm::quat(), 60.0f};

  // generate rays, probably put this in a separate function
  for (size_t i = 0; i < width * height; ++i) {
    int xoffset = i % width;
    int yoffset = i / width;

    glm::vec3 direction = {0.0f, 0.0f, 1.0f};
    glm::vec3 position = {xoffset - halfwidth - cam.position.x,
                          yoffset - halfheight - cam.position.y,
                          cam.position.z};
    direction = cam.rotation * direction;

    rt::ray r{position, direction};
    rays.push_back(r);
  }

  for (const auto& ray : rays) {
    res.push_back(cast(ray, scene));
  }

  return res;
}

trace_data tracer_sphere::trace(const rt::ray& r,
                                const rt::scene& scene) const {

  float distance = std::numeric_limits<float>::max();
  const rt::sphere* hit_sphere = nullptr;

  for (const auto& sphere : scene.spheres()) {
    auto intersection = rt::intersect(r, sphere);
    if (intersection) {
      if (*intersection < distance) {
        distance = *intersection;
        hit_sphere = &sphere;
      }
    }
  }

  return {distance, hit_sphere};
}
} // namespace rt
