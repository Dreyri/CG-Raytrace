#include <QApplication>

#include "window.hpp"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  rt::Window window(nullptr);
  window.show();

  return app.exec();
}
