#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->renderboy = new Renderboy();

    connect(renderboy, &Renderboy::setImage, this, &MainWindow::setImage);

    this->renderboy->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete renderboy;
}

void MainWindow::setImage(QImage img)
{
    ui->lDisplay->setPixmap(QPixmap::fromImage(img));
}