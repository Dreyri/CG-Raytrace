#pragma once

#include <QMainWindow>
#include <QtDebug>
#include <QThread>
#include <QImage>

#include "renderboy.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void resetControls();

private:
    Ui::MainWindow *ui;
    Renderboy* renderboy;
};
