#include "mainwindow.h"

#include <QApplication>
#include <utility>
#include <Algorithms.hpp>

MainWindow* w;

enum Algorithms{NONE, LIMIARIZACAO, LOGARITMO, NEGATIVO, GAMMA, ESCANOGRAFIA, KERNEL};
enum Visibility{HIDE, SHOW};
const char* templateAlg[] ={"",
    "",
    "TEMPLATE: base num, c num;\n\nCLEAR - LIMPA TEXTO",
    "",
    "TEMPLATE: c num, gamma num;\n\nCLEAR - LIMPA TEXTO",
    "TEMPLATE: msg;\n\nSET - ESCREVE NA IMAGEM\n\nCLEAR - LIMPA O BLOCO DE TEXTO E LÊ DA IMAGEM",
    "TEMPLATE: tam (lin, col), [l1c1, l1c2, ... l1cn, l2c1, ... lmcn]\n\nSET - APLICA KERNEL NA IMAGEM"};

Algorithms algoritmo = NONE;

std::vector<std::pair<float, float>> equacionaRetas(std::vector<QPointF> buffer);

Mat reference = Mat(cv::Size(1,1), 0);
Mat scanimg = Mat(cv::Size(1,1), 0);

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
    case ESCANOGRAFIA:{
        read = w->getText();
        auto dataScanography = parseScan(read);
        scanimg = scanographyWrite(einstein, dataScanography);
        break;
    }
    case KERNEL:{
        read = w->getText();
        auto dataKernel = parseKernel(read);
        auto matsize = dataKernel.second;
        appKernel(einstein, dataKernel.first, matsize);
        break;
    }
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
        break;
    case GAMMA:
        w->clearText();
        break;
    case ESCANOGRAFIA:{
        if(scanimg.size() != cv::Size(1, 1)){
            w->clearText();
            std::string aux = std::string("Mensagem Lida: ") + scanographyRead(scanimg);
            const char *c = aux.c_str();
            w->setTextualPlaceholder(c);
        }
        else{
            std::cout << "Erro: Não existe imagem com mensagem secreta... ainda." << std::endl;
        }
        break;
    }
    case KERNEL:
        w->clearText();
        break;
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

void MainWindow::on_Escanografia_clicked()
{
    algoritmo = ESCANOGRAFIA;
    w->ToggleGraphics(HIDE);
    w->setTextualPlaceholder(templateAlg[ESCANOGRAFIA]);
    w->clearText();
    w->ToggleText(SHOW);
}

void MainWindow::on_Kernel_clicked()
{
    algoritmo = KERNEL;
    w->ToggleGraphics(HIDE);
    w->setTextualPlaceholder(templateAlg[KERNEL]);
    w->clearText();
    w->ToggleText(SHOW);
}
