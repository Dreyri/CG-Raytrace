#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <glm/glm.hpp>

namespace rt {
namespace path {
class RenderTarget {
public:
  using value_type = uint8_t;
  using color_type = glm::tvec4<value_type>;

protected:
  size_t m_width, m_height;

public:
  RenderTarget(size_t width, size_t height)
      : m_width{width}
      , m_height{height} {
  }

  virtual ~RenderTarget() {
  }

  inline size_t width() const {
    return m_width;
  }

  inline size_t height() const {
    return m_height;
  }

  virtual const value_type* pixels() const = 0;
  virtual void setPixels(const value_type* pix) = 0;

  virtual void setColor(size_t x, size_t y, const color_type& pixel) = 0;
};

class Image final : public RenderTarget {
  using RenderTarget::value_type;

protected:
  value_type* m_pixels;

public:
  Image(const value_type* pixels, size_t width, size_t height)
      : RenderTarget(width, height)
      , m_pixels{} {
    m_pixels = static_cast<value_type*>(
        std::calloc(m_width * m_height, sizeof(value_type) * 4));

    if (pixels) {
      std::memcpy(m_pixels, pixels, m_width * m_height * 4);
    }
  }

  ~Image() {
    std::free(m_pixels);
  }

  virtual const value_type* pixels() const override {
    return m_pixels;
  }

  virtual void setPixels(const value_type* pix) override {
    std::memcpy(m_pixels, pix, m_width * m_height * 4);
  }

  value_type* at(size_t x, size_t y) {
    auto res = x + y * m_width;
    return &m_pixels[res * 4];
  }

  glm::vec4 at_rel(float u, float v) const {
    // convert to x and y
    size_t x = (std::fmod(std::fabs(u), 1.0f)) * (m_width - 1);
    size_t y = (1.0f - std::fmod(std::fabs(v), 1.0f)) * (m_height - 1);

    assert(x < m_width);
    assert(y < m_height);

    float r, g, b;

    r = static_cast<float>(m_pixels[(x + y * m_width) * 4]) / 255.0f;
    g = static_cast<float>(m_pixels[(x + y * m_width) * 4 + 1]) / 255.0f;
    b = static_cast<float>(m_pixels[(x + y * m_width) * 4 + 2]) / 255.0f;

    return glm::vec4(r, g, b, 1.0f);
  }

  virtual void setColor(size_t x, size_t y, const color_type& pixel) override {
    assert(x < m_width);
    assert(y < m_height);

    auto col = at(x, y);
    col[0] = pixel.r;
    col[1] = pixel.g;
    col[2] = pixel.b;
    col[3] = pixel.a;
  }
};
} // namespace path
} // namespace rt