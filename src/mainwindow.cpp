#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->renderboy = new Renderboy();

    connect(renderboy, &Renderboy::setImage, ui->ra, &RenderArea::setImage);

    this->renderboy->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete renderboy;
}