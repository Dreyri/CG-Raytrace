#include "renderArea.hpp"

#include <iostream>

RenderArea::RenderArea(QWidget* parent) : QWidget(parent)
{

}

void RenderArea::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), current);
}

void RenderArea::resizeEvent(QResizeEvent * event)
{
    emit setSize(this->width(), this->height());
}

void RenderArea::setImage(QImage img)
{
    current = img;
    this->update();
}