#include "window.hpp"

namespace rt {
Widget::Widget(Raytracer* rt)
    : m_raytracer{rt}
    , m_currentRenderImage{m_raytracer->currentColors()} {
  rt->resize(width(), height());
}
} // namespace rt
