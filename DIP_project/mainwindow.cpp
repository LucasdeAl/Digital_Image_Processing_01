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

void MainWindow::ToggleGraphics(int v){
    if(v == 0){
        ui->imagem->hide();
    }
    else{
        ui->imagem->show();
    }
}

void MainWindow::ToggleText(int v){
    if(v == 0){
        ui->inputTextual->hide();
    }
    else{
        ui->inputTextual->show();
    }
}

void MainWindow::setTextualPlaceholder(const char *s){
    ui->inputTextual->setPlaceholderText(s);
}

std::string MainWindow::getText(){
    return ui->inputTextual->toPlainText().toStdString();
}
void MainWindow::clearText(){
    ui->inputTextual->setPlainText("");
}

void MainWindow::on_Quit_clicked()
{
    exit(1);
}

