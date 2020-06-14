#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QOpenGLWidget>
#include <QObject>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(
                    ui->checkBox_2,
                    &QCheckBox::toggled,
                    ui->openGLWidget,
                    [=](bool value) {
            ui->openGLWidget->setCamera(value);
            qInfo() << "Camera: " << value;
        });

    QObject::connect(
                    ui->checkBox,
                    &QCheckBox::toggled,
                    ui->openGLWidget,
                    [=](bool value) {
            ui->openGLWidget->setAnimate(value);
            qInfo() << "Animate: " << value;
        });

    QObject::connect(
                    ui->sbFOV,
                    qOverload<int>(&QSpinBox::valueChanged),
                    ui->openGLWidget,
                    [=](int value) {
            ui->openGLWidget->setFOV(value);
            qInfo() << "FOV: " << value;
        });

    QObject::connect(
                    ui->sbAngle,
                    qOverload<int>(&QSpinBox::valueChanged),
                    ui->openGLWidget,
                    [=](int value) {
            ui->openGLWidget->setAngle(value);
            qInfo() << "Angle: " << value;
        });

    QObject::connect(
                    ui->sbNear,
                    qOverload<double>(&QDoubleSpinBox::valueChanged),
                    ui->openGLWidget,
                    [this](double value) {
            ui->openGLWidget->setNear(value);
            qInfo() << "Near: " << value;
            qInfo() << "Near(actual): " << ui->openGLWidget->v_near;
        });

    QObject::connect(
                    ui->openGLWidget,
                    &MyGLWidget::nearChanged,
                    ui->sbNear,
                    &QDoubleSpinBox::setValue
        );

    QObject::connect(
                    ui->sbFar,
                    qOverload<double>(&QDoubleSpinBox::valueChanged),
                    ui->openGLWidget,
                    [this](double value) {
            ui->openGLWidget->setFar(value);
            qInfo() << "Far: " << value;
            qInfo() << "Far(actual): " << ui->openGLWidget->v_far;
        });

    QObject::connect(
                    ui->openGLWidget,
                    &MyGLWidget::farChanged,
                    ui->sbFar,
                    &QDoubleSpinBox::setValue
        );

    QObject::connect(
                    ui->rbOrthogonal,
                    &QRadioButton::clicked,
                    ui->openGLWidget,
                    [this](bool value) {
            ui->openGLWidget->setProjectionMode(!value);
            qInfo() << "Projection: " << ui->openGLWidget->v_perspective;
        });

    QObject::connect(
                    ui->rbPerspective,
                    &QRadioButton::clicked,
                    ui->openGLWidget,
                    [this](bool value) {
            ui->openGLWidget->setProjectionMode(value);
            qInfo() << "Projection: " << ui->openGLWidget->v_perspective;
        });

    QObject::connect(
                    ui->hsRotationA,
                    &QSlider::valueChanged,
                    ui->openGLWidget,
                    [this](int value) {
            ui->openGLWidget->setRotationA(value);
            qInfo() << "A: " << value;
        });

    QObject::connect(
                    ui->hsRotationB,
                    &QSlider::valueChanged,
                    ui->openGLWidget,
                    [this](int value) {
            ui->openGLWidget->setRotationB(value);
            qInfo() << "B: " << value;
        });

    QObject::connect(
                    ui->hsRotationC,
                    &QSlider::valueChanged,
                    ui->openGLWidget,
                    [this](int value) {
            ui->openGLWidget->setRotationC(value);
            qInfo() << "C: " << value;
        });

    QObject::connect(
                    ui->bReset,
                    &QPushButton::clicked,
                    this,
                    [this]() {
            this->reset();
        qInfo() << "Projection: " << ui->openGLWidget->v_perspective;
        });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset(){
       ui->sbFOV->setValue(0);
       ui->sbAngle->setValue(0);
       //ui->rbOrthogonal->setChecked(true);
       ui->rbPerspective->setChecked(false);
       ui->hsRotationA->setValue(0);
       ui->hsRotationB->setValue(0);
       ui->hsRotationC->setValue(0);
       ui->sbFar->setValue(0);
       ui->sbNear->setValue(0);
}

