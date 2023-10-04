#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paint.setColor(QColor(255, 255, 255));
    paint.setStyle(Qt::SolidPattern);
    mark.setColor(QColor(0, 0, 0));
    mark.setStyle(Qt::SolidPattern);
    scene = new QGraphicsScene(this);
    scene->addRect(0, 0, 255, 255, Qt::SolidLine, paint);
    ui->imagem->setScene(scene);
    ui->imagem->setFixedSize(255, 255);
    ui->imagem->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->imagem->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

void MainWindow::resizeView(){
    ui->imagem->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->imagem->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->imagem->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

int MainWindow::retrieveBrushSlider(){
    return ui->brushSize->sliderPosition();
}

void MainWindow::ToggleFourierTools(int v){
    if(v==0){
        ui->gaussianBrush->hide();
        ui->brushSize->hide();
        ui->ShowFourier->hide();
    }
    else{
        ui->gaussianBrush->show();
        ui->brushSize->show();
        ui->ShowFourier->show();
    }
}

void MainWindow::saveView(QString fileName){
    QPixmap pix = QPixmap(scene->sceneRect().width(), scene->sceneRect().height());
    QPainter temp(&pix);
    scene->render(&temp, pix.rect(), scene->sceneRect(), Qt::KeepAspectRatio);
    temp.end();
    pix.save(fileName);
}

void MainWindow::zoomImage(){
    scene->setSceneRect(0, 0, 255, 255);
    resizeView();
    ui->imagem->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->imagem->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

