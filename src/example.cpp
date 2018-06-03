#include <memory>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <QWindow>
#include <QImage>

#include "core/raytracer_simple.hpp"

int main(int argc, char** argv)
{
    rt::RaytracerSimple tracer = rt::RaytracerSimple();
    auto rgbbuffer = tracer.render();

    QImage image = QImage(1920, 1080, QImage::Format::Format_RGB888);
    
    for (unsigned int h = 0; h < 1080; h++)
    {
        for (unsigned int w = 0; w < 1920; w++)
        {
            glm::uvec3 color = (*rgbbuffer)[h][w];
            image.setPixelColor(w, h, QColor(color.r, color.g, color.b));
        }
    }

    image.save("example.png", "PNG");
}