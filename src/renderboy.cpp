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
        sc1.transform();
        QImage img = QImage(640, 480, QImage::Format_RGBA8888);
        sc1.render(img);
        emit setImage(img);
    }
}
