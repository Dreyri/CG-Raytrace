#include <QApplication>

//#include "window.hpp"
#include "rtscene.hpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  rt::RaytraceScene scene;
  QGraphicsView view(&scene);
  view.show();

  return app.exec();
}
