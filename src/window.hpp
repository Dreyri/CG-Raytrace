#include <memory>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include <glm/glm.hpp>

namespace rt {

class Raytracer {
public:
  /**
   * start raytracing our scene
   */
  virtual void render() = 0;
  /**
   * cancel our current raytracing process
   */
  virtual void cancelRender() = 0;
  /**
   * the vector with colors which is being updated from the render call
   * this is used to continuously update the texture whilst raytracing is being done
   * read access to this value should be thread safe
   */
  virtual const std::vector<glm::vec4>& currentColors() const = 0;
  /**
   * resize the output data set, preceeded by a call to cancelRender
   */
  virtual void resize(int w, int h) = 0;
  /**
   * indicates rendering is done and this image can be used until the next
   * rendering has finished
   */
  virtual bool renderingDone() const = 0;
};

class Widget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core {
  Q_OBJECT
private:
  std::unique_ptr<Raytracer> m_raytracer;

  const std::vector<glm::vec4>& m_currentRenderImage;

  QOpenGLTexture m_texture;

public:
  Widget(Raytracer* rt);

  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;
};
} // namespace rt
