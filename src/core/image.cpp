#include "image.hpp"

namespace rt {
image::image(image::size_type width, image::size_type height,
             std::unique_ptr<image::value_type[]>&& pixels)
    : m_width{width}
    , m_height{height}
    , m_pixels{std::move(pixels)} {
}

image::size_type image::width() const {
  return m_width;
}

image::size_type image::height() const {
  return m_height;
}

image::value_type* image::pixels() {
  return m_pixels.get();
}

const image::value_type* image::pixels() const {
  return m_pixels.get();
}
} // namespace rt