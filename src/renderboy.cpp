#include "renderboy.hpp"

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
        QImage img = QImage(400, 300, QImage::Format_RGB888);
        sc1.render(img);
        emit setImage(img);
    }
}
