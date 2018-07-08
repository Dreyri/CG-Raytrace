#pragma once

#include <QThread>

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

private:
    SimpleScene1 sc1;
};

