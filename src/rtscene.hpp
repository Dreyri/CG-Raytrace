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

public slots:
  void display(uint8_t* pixels, size_t width, size_t height);
};
} // namespace rt
