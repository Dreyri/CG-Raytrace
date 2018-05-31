#pragma once

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

namespace rt {
class image {
public:
  using value_type = uint8_t;
  using size_type = size_t;

private:
  size_type m_width;
  size_type m_height;

  std::unique_ptr<value_type[]> m_pixels;

public:
  image(size_t width, size_t height, std::unique_ptr<value_type[]>&& pixels);
  ~image();

  size_type width() const;
  size_type height() const;

  const value_type* pixels() const;
  value_type* pixels();
};
} // namespace rt