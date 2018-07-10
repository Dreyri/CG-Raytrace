#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->renderboy = new Renderboy();

    connect(renderboy, &Renderboy::setImage, ui->ra, &RenderArea::setImage);
    connect(renderboy, &Renderboy::setFPS, ui->lFPS, &QLabel::setText);

    connect(ui->rbReset, &QPushButton::clicked, this, &MainWindow::resetControls);

    connect(ui->sbDepth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), renderboy, &Renderboy::setDepth);
    connect(ui->sbAdaptive, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), renderboy, &Renderboy::setAdaptive);
    connect(ui->hsValue, &QSlider::valueChanged, renderboy, &Renderboy::setSlider);
    connect(ui->cbSmoothing, &QCheckBox::toggled, renderboy, &Renderboy::setSmoothing);
    connect(ui->cbReflection, &QCheckBox::toggled, renderboy, &Renderboy::setReflection);
    connect(ui->cbRefraction, &QCheckBox::toggled, renderboy, &Renderboy::setRefraction);
    connect(ui->cbAnimate, &QCheckBox::toggled, renderboy, &Renderboy::setAnimate);
    connect(ui->dsIndex, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), renderboy, &Renderboy::setRI);
    connect(ui->hsVertical, &QSlider::valueChanged, renderboy, &Renderboy::setCamV);
    connect(ui->hsHorizontal, &QSlider::valueChanged, renderboy, &Renderboy::setCamH);
    connect(ui->ra, &RenderArea::setSize, renderboy, &Renderboy::setSize);

    resetControls();
    this->renderboy->start();
    resetControls();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete renderboy;
}

void MainWindow::resetControls()
{
    ui->sbDepth->setValue(8);
    ui->sbAdaptive->setValue(0.05);
    ui->hsValue->setValue(0);
    ui->cbSmoothing->setChecked(true);
    ui->cbReflection->setChecked(true);
    ui->cbRefraction->setChecked(true);
    ui->cbAnimate->setChecked(false);
    ui->dsIndex->setValue(1.5);
    ui->hsVertical->setValue(0);
    ui->hsHorizontal->setValue(0);
}