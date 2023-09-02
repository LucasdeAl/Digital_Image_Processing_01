#include "mainwindow.h"

#include <QApplication>
#include <utility>
#include <Algorithms.hpp>

MainWindow* w;

std::vector<std::pair<float, float>> equacionaRetas(std::vector<QPointF> buffer);

void QGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QPointF coord = event->scenePos();
    if(w->vertexBuffer.size() == 0){
        w->vertexBuffer.emplace_back(0, coord.y());
    }
    if(coord.x() > w->vertexBuffer.back().x()){
        w->scene->addLine(w->vertexBuffer.back().x(), w->vertexBuffer.back().y(), coord.x(), coord.y(), Qt::SolidLine);
        w->vertexBuffer.push_back(coord);
    }
}

void MainWindow::on_pushButton_clicked()
{
    w->scene->addLine(w->vertexBuffer.back().x(), w->vertexBuffer.back().y(), 255, w->vertexBuffer.back().y(), Qt::SolidLine);
    w->vertexBuffer.emplace_back(255, w->vertexBuffer.back().y());
    
    
}

void MainWindow::on_pushButton_2_clicked()
{
    w->vertexBuffer.clear();
    w->scene->clear();
    w->scene->addRect(0, 0, 255, 255, Qt::SolidLine, paint);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    w = &window;
    window.show();
    return a.exec();
}

std::vector<std::pair<float, float>> equacionaRetas(std::vector<QPointF> buffer){
    std::vector<std::pair<float, float>> equacoes;
    for(int i = 1; i < buffer.size(); i++){
        float a = (buffer[i].y() - buffer[i-1].y())/(buffer[i].x() - buffer[i-1].x());
        float b = buffer[i].y() - (a * buffer[i].x());
        equacoes.emplace_back(a, b);
    }
    return equacoes;
}
