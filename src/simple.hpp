#pragma once

#include <QImage>
#include <QColor>
#include <QThread>
#include <QElapsedTimer>

#include "setup.hpp"
#include "core/defs.hpp"
#include "core/raytracer_simple.hpp"

class ImageTarget : public rt::RenderTarget
{
private:
    QImage& image;

public:
    ImageTarget(QImage& image) : image(image) {}
    unsigned int getWidth() override { return this->image.width(); }
    unsigned int getHeight() override { return this->image.height(); }
    void setPixel(unsigned int w, unsigned int h, rt::floating r, rt::floating g, rt::floating b) override
    {
        this->image.setPixelColor(w, h, { (int)(255 * r), (int)(255 * g), (int)(255 * b) });
    }
};

class ImageTexture : public rt::Texture
{
public:
    ImageTexture(QImage img) : img{ img } {}
    int getWidth() override { return img.width(); }
    int getHeight() override { return img.height(); }
    rt::fColor getColor(int u, int v) override
    {
        QColor c = img.pixelColor(v, img.height() - 1 - u);
        return rt::fColor(c.redF(), c.greenF(), c.blueF());
    }

private:
    QImage img;
};


class SimpleScene1 : Setup
{
private:
    rt::RaytracerSimple tracer;
    std::shared_ptr<rt::Scene> scene;
    QElapsedTimer timer;
    unsigned int frameCount;

public:
    void setup() override;
    void transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animation, int slider) override;
    void render(QImage& target) override;
};

class SimpleScene2 : Setup
{
private:
    rt::RaytracerSimple tracer;
    std::shared_ptr<rt::Scene> scene;
    QElapsedTimer timer;
    unsigned int frameCount;

public:
    void setup() override;
    void transform(unsigned int depth, float adaptive, bool smoothing, bool reflection, bool refraction, bool animation, int slider) override;
    void render(QImage& target) override;
};
