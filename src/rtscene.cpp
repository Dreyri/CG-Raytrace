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
} // namespace rt
