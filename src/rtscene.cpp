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

void RaytraceScene::displayPixmap(QPixmap* pm) {
  m_pixmap->setPixmap(*pm);

  emit displayedPixmapChanged(pm);
}
} // namespace rt
