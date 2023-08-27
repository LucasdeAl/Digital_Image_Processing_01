#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    vertexBuffer.emplace_back(0, 400);
    ui->setupUi(this);
    paint.setColor(QColor(255, 255, 255));
    paint.setStyle(Qt::SolidPattern);
    scene = new QGraphicsScene(this);
    scene->addRect(0, 0, 400, 400, Qt::SolidLine, paint);
    ui->imagem->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}




