#include "rtscene.hpp"

namespace rt {
RaytraceScene::RaytraceScene()
    : QGraphicsScene()
    , m_pixmap(new QGraphicsPixmapItem(
          QPixmap("/home/dreyri/Pictures/GitHub-Mark.png"))) {
  addItem(m_pixmap);
}

RaytraceScene::~RaytraceScene() {
}

void RaytraceScene::display(uint8_t* pixels, size_t width, size_t height) {
  QImage img((unsigned char*)pixels, width, height, QImage::Format_RGBA8888);
  m_pixmap->setPixmap(QPixmap::fromImage(std::move(img)));
}
} // namespace rt
