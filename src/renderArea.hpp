#pragma once

#include <QWidget>
#include <QPainter>
#include <QImage>

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

signals:
    void setSize(int w, int h);

public slots:
    void setImage(QImage img);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QImage current;
};