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
    if(coord.x() > w->vertexBuffer.back().first){
        w->scene->addLine(w->vertexBuffer.back().first, w->vertexBuffer.back().second, coord.x(), coord.y(), Qt::SolidLine);
        w->vertexBuffer.emplace_back(coord.x(), coord.y());
    }
}

void MainWindow::on_pushButton_clicked()
{
    w->scene->addLine(w->vertexBuffer.back().first, w->vertexBuffer.back().second, 255, w->vertexBuffer.back().second, Qt::SolidLine);
    w->vertexBuffer.emplace_back(255, w->vertexBuffer.back().second);
    std::vector<std::pair<float, float>> eqBuffer = equacionaRetas(w->vertexBuffer);
    Mat einstein = imread("../images/einstein.tif", IMREAD_UNCHANGED);
    if (einstein.empty())
    {
        cout << "Could not open or find the image" << endl;
    }
    limiarizacaoPorPartes(w->vertexBuffer, eqBuffer, einstein);
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
