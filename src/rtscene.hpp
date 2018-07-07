#pragma once

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace rt {
class RaytraceScene : public QGraphicsScene {
private:
  QGraphicsPixmapItem* m_pixmap;
public:
  RaytraceScene();
  virtual ~RaytraceScene();
};
} // namespace rt
