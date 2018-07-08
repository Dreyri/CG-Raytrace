#include "renderArea.hpp"

#include <iostream>

RenderArea::RenderArea(QWidget* parent) : QWidget(parent)
{

}

void RenderArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), current);
    std::cout << "Repaint" << std::endl;
}

void RenderArea::setImage(QImage img)
{
    current = img;
    this->update();
}