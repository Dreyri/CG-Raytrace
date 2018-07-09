#include "renderboy.hpp"

#include <iostream>

Renderboy::Renderboy()
{

}

Renderboy::~Renderboy()
{
}

void Renderboy::run()
{
    sc1 = SimpleScene1();

    sc1.setup();
    
    while (true)
    {
        sc1.transform(depth, adaptive, smoothing, reflection, refraction, animate, slider);
        QImage img = QImage(640, 480, QImage::Format_RGB888);
        sc1.render(img);
        emit setImage(img);
    }
}

void Renderboy::setDepth(int depth)
{
    this->depth = depth;
}

void Renderboy::setSlider(int slider)
{
    this->slider = slider;
}

void Renderboy::setAdaptive(float adaptive)
{
    this->adaptive = adaptive;
}

void Renderboy::setSmoothing(bool s)
{
    smoothing = s;
}

void Renderboy::setReflection(bool r)
{
    reflection = r;
}

void Renderboy::setRefraction(bool r)
{
    refraction = r;
}

void Renderboy::setAnimate(bool a)
{
    animate = a;
}
