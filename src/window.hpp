#include <memory>

#include <QMainWindow>
#include <QPainter>
#include <QLabel>

#include <glm/glm.hpp>

namespace rt {

class Raytracer {
public:
  /**
   * start raytracing our scene
   */
  virtual QImage render() = 0;
  /**
   * cancel our current raytracing process
   */
  virtual void cancelRender() = 0;
  /**
   * resize the output data set, preceeded by a call to cancelRender
   * not used at first
   */
  virtual void resize(int w, int h) = 0;
};

class Window : public QMainWindow {
  Q_OBJECT
private:
  std::unique_ptr<Raytracer> m_raytracer;
  QLabel* m_imageLabel;

public:
  Window(Raytracer* rt);
  virtual ~Window();

  void setPosition(const glm::vec3& pos);

signals:
  void positionChanged(glm::vec3 pos);

};
} // namespace rt
