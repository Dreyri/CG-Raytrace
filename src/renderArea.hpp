#pragma once

#include <QWidget>
#include <QPainter>
#include <QImage>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

public slots:
    void setImage(QImage img);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage current;
};