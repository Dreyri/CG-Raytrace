#pragma once

#include <QThread>
#include <QElapsedTimer>
#include <QString>

#include "setup.hpp"
#include "simple.hpp"

class Renderboy : public QThread
{
    Q_OBJECT

public:
    Renderboy();
    ~Renderboy();
    void run() override;

signals:
    void setImage(QImage img);
    void setFPS(QString fps);

public slots:
    void setDepth(int depth);
    void setSlider(int slider);
    void setAdaptive(float adaptive);
    void setSmoothing(bool s);
    void setReflection(bool r);
    void setRefraction(bool r);
    void setAnimate(bool a);

private:
    int depth, slider;
    float adaptive;
    bool smoothing, reflection, refraction, animate;
};

