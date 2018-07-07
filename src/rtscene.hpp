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

signals:
  void displayedPixmapChanged(QPixmap* pm);

public slots:
  void displayPixmap(QPixmap* pm);
};
} // namespace rt
