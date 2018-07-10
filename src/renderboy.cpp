#include "renderboy.hpp"

#include <iostream>

Renderboy::Renderboy() : camH{0}, camV{0}, ri{1.5}, depth{8}, winW{640}, winH{480}, smoothing{true}, reflection{true}, refraction{true}, animate{false}, adaptive{0.05f}
{
}

Renderboy::~Renderboy()
{
}

void Renderboy::run()
{
    SimpleScene2 sc1 = SimpleScene2();
    //SimpleScene1 sc1 = SimpleScene1();

    sc1.setup();
    
    while (true)
    {
        QElapsedTimer timer;
        timer.start();

        sc1.transform(depth, adaptive, smoothing, reflection, refraction, animate, slider, ri, camH, camV);
        
        QImage img = QImage(winW, winH, QImage::Format_RGB888);
        sc1.render(img);
        emit setImage(img);


        emit setFPS(QString::number(1.0 / (timer.elapsed() / 1000.0)));
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

void Renderboy::setCamH(int h)
{
    camH = h;
}

void Renderboy::setCamV(int v)
{
    camV = v;
}

void Renderboy::setRI(double ri)
{
    this->ri = ri;
}

void Renderboy::setSize(int w, int h)
{
    winH = h;
    winW = w;
}
