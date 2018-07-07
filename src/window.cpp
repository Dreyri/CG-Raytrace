#include "window.hpp"

namespace rt {
Window::Window(Raytracer* rt)
    : m_raytracer{rt}
    , m_imageLabel{new QLabel} {
  setCentralWidget(m_imageLabel);
}

Window::~Window() {
}
} // namespace rt
