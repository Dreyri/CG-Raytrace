#include <memory>
#include <vector>
#include <iostream>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <QWindow>
#include <QImage>
#include <QTime>

#include "core/raytracer_simple.hpp"

class ImageTarget : public rt::RenderTarget
{
private:
    std::shared_ptr<QImage> image;

public:
    ImageTarget(std::shared_ptr<QImage> image) : image(image) {}
    unsigned int getWidth() override { return this->image->width(); }
    unsigned int getHeight() override { return this->image->height(); }
    void setPixel(unsigned int w, unsigned int h, float r, float g, float b) override
    {
        this->image->setPixelColor(w, h, { (int)(255 * r), (int)(255 * g), (int)(255 * b) });
    }
};

int main(int argc, char** argv)
{
    rt::RaytracerSimple tracer = rt::RaytracerSimple();

    std::shared_ptr<QImage> image = std::make_shared<QImage>(QImage(1920, 1080, QImage::Format::Format_RGB888));
    std::shared_ptr<ImageTarget> target = std::make_shared<ImageTarget>(ImageTarget(image));

    QTime timer = QTime();
    timer.start();

    tracer.render(target);

    unsigned long elapsed = timer.elapsed();

    image->save("example.png", "PNG");

    std::cout << elapsed << std::endl;
    system("PAUSE");
}