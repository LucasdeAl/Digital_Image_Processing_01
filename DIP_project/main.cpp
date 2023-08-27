#include "mainwindow.h"

#include <QApplication>

MainWindow* w;

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
    w->scene->addLine(w->vertexBuffer.back().x(), w->vertexBuffer.back().y(), 400, w->vertexBuffer.back().y(), Qt::SolidLine);
    w->vertexBuffer.emplace_back(400, w->vertexBuffer.back().y());
}

void MainWindow::on_pushButton_2_clicked()
{
    w->vertexBuffer.clear();
    w->scene->clear();
    w->scene->addRect(0, 0, 400, 400, Qt::SolidLine, paint);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    w = &window;
    window.show();
    return a.exec();
}
