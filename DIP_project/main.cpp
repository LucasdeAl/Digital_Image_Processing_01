#include "mainwindow.h"

#include <QApplication>
#include <utility>
#include <Algorithms.hpp>

MainWindow* w;

enum Algorithms{NONE, LIMIARIZACAO, LOGARITMO, NEGATIVO, GAMMA};
enum Visibility{HIDE, SHOW};
const char* templateAlg[] ={"", "", "base num, c num;", "", "c num, gamma num;"};

Algorithms algoritmo = NONE;

std::vector<std::pair<float, float>> equacionaRetas(std::vector<QPointF> buffer);

void LimpaLimiarizacao(){
    w->vertexBuffer.clear();
    w->scene->clear();
    w->scene->addRect(0, 0, 255, 255, Qt::SolidLine, w->paint);
}

void QGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(algoritmo == LIMIARIZACAO){
        QPointF coord = event->scenePos();
        if(w->vertexBuffer.size() == 0){
            w->vertexBuffer.emplace_back(0, coord.y());
        }
        if(coord.x() > w->vertexBuffer.back().first){
            w->scene->addLine(w->vertexBuffer.back().first, w->vertexBuffer.back().second, coord.x(), coord.y(), Qt::SolidLine);
            w->vertexBuffer.emplace_back(coord.x(), coord.y());

        }
    }
}

void MainWindow::on_Set_clicked()
{
    std::string read;
    Mat einstein = imread("../images/einstein.tif", IMREAD_UNCHANGED);
    if (einstein.empty())
    {
        cout << "Could not open or find the image" << endl;
    }
    switch (algoritmo){
    case LIMIARIZACAO:{
        w->scene->addLine(w->vertexBuffer.back().first, w->vertexBuffer.back().second, 255, w->vertexBuffer.back().second, Qt::SolidLine);
        w->vertexBuffer.emplace_back(255, w->vertexBuffer.back().second);
        std::vector<std::pair<float, float>> eqBuffer = equacionaRetas(w->vertexBuffer);
        limiarizacaoPorPartes(w->vertexBuffer, eqBuffer, einstein);
        break;}
    case LOGARITMO:{
        w->getText();
        read = w->getText();
        auto dataLog = parseLog(read);
        logarithm(einstein, dataLog.first, dataLog.second, true);
        break;}
    case NEGATIVO:
        negative(einstein);
        break;
    case GAMMA:{
        read = w->getText();
        auto dataGamma = parseGamma(read);
        gammaC(einstein, dataGamma.first, dataGamma.second);
        break;}
    }
}

void MainWindow::on_Clear_clicked()
{
    switch(algoritmo){
    case LIMIARIZACAO:
        LimpaLimiarizacao();
        break;
    case LOGARITMO:
        w->clearText();
    case GAMMA:
        w->clearText();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    w = &window;
    w->ToggleGraphics(HIDE);
    w->ToggleText(HIDE);
    window.show();
    return a.exec();
}

void MainWindow::on_Limiarizacao_clicked()
{
    algoritmo = LIMIARIZACAO;
    w->ToggleText(HIDE);
    w->ToggleGraphics(SHOW);
}


void MainWindow::on_Logaritmo_clicked()
{
    algoritmo = LOGARITMO;
    w->ToggleGraphics(HIDE);
    w->setTextualPlaceholder(templateAlg[LOGARITMO]);
    w->clearText();
    w->ToggleText(SHOW);
}


void MainWindow::on_Negativo_clicked()
{
    algoritmo = NEGATIVO;
    w->ToggleGraphics(HIDE);
    w->ToggleText(HIDE);
}


void MainWindow::on_Gamma_clicked()
{
    algoritmo = GAMMA;
    w->ToggleGraphics(HIDE);
    w->setTextualPlaceholder(templateAlg[GAMMA]);
    w->clearText();
    w->ToggleText(SHOW);
}
