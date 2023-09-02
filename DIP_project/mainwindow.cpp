#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paint.setColor(QColor(255, 255, 255));
    paint.setStyle(Qt::SolidPattern);
    scene = new QGraphicsScene(this);
    scene->addRect(0, 0, 255, 255, Qt::SolidLine, paint);
    ui->imagem->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}




