#include "mainwindow.h"

#include <QApplication>
#include <utility>
#include <Algorithms.hpp>

MainWindow* w;

enum Algorithms{NONE, LIMIARIZACAO, LOGARITMO, NEGATIVO, GAMMA, ESCANOGRAFIA, KERNEL, FOURIER, SOBEL, GRAYSCALE, NEGATIVOHSV, COLORS};
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
bool can_draw = false;
bool gaussianB = false;
QImage trans;

void LimpaLimiarizacao(){
    w->vertexBuffer.clear();
    w->scene->clear();
    w->scene->addRect(0, 0, 255, 255, Qt::SolidLine, w->paint);
    w->scene->setSceneRect(0, 0, 255, 255);
    w->resizeView();
}

void QGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QPointF coord = event->scenePos();
    switch(algoritmo){
    case LIMIARIZACAO:{
        if(w->vertexBuffer.size() == 0){
            w->vertexBuffer.emplace_back(0, coord.y());
        }
        if(coord.x() > w->vertexBuffer.back().first){
            w->scene->addLine(w->vertexBuffer.back().first, w->vertexBuffer.back().second, coord.x(), coord.y(), Qt::SolidLine);
            w->vertexBuffer.emplace_back(coord.x(), coord.y());

        }
        break;
    }
    case FOURIER:{
        can_draw = false;
    }
    }
}

void QGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QPointF coord = event->scenePos();
    switch(algoritmo){
    case FOURIER:{
        can_draw = true;
    }
    }
}

void QGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QPointF coord = event->scenePos();
    switch(algoritmo){
    case FOURIER:{
        if(can_draw && (coord.x() >= 0 && coord.x() < w->scene->sceneRect().width()) && (coord.y() >= 0 && coord.y() < w->scene->sceneRect().height())){
            if(gaussianB){
                int bsize = w->retrieveBrushSlider();
                w->grad = QRadialGradient(QPointF(coord.x() + bsize/2, coord.y() + bsize/2), bsize);
                w->grad.setColorAt(0, QColor(0, 0, 0, 70));
                w->grad.setColorAt(0.6, QColor(0, 0, 0, 10));
                w->grad.setColorAt(1, QColor(0, 0, 0, 0));
                w->scene->addEllipse(coord.x(), coord.y(), w->retrieveBrushSlider(), w->retrieveBrushSlider(), QPen(QColor(0, 0, 0, 0)), QBrush(w->grad));
            }
            else{
                w->scene->addEllipse(coord.x(), coord.y(), w->retrieveBrushSlider(), w->retrieveBrushSlider(), QPen(QColor(0, 0, 0, 0)), w->mark);
            }
        }
    }
    }
}

void MainWindow::on_Set_clicked()
{
    std::string read;
    std::string imgpath = "../images/";
    imgpath = imgpath + w->getName();
    Mat einstein = imread(imgpath);
    if (einstein.empty())
    {
        cout << "Could not open or find the image" << endl;
        return;
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
    case FOURIER:{
        Mat ajuste;
        int m = getOptimalDFTSize(einstein.rows);
        int n = getOptimalDFTSize(einstein.cols);
        copyMakeBorder(einstein, ajuste, 0, m - einstein.rows, 0, n - einstein.cols, BORDER_CONSTANT, Scalar::all(0));
        Mat planos[] = {Mat_<float>(ajuste), Mat::zeros(ajuste.size(), CV_32F)};
        Mat complexo;
        merge(planos, 2, complexo);
        dft(complexo, complexo);
        split(complexo, planos);
        magnitude(planos[0], planos[1], planos[0]);
        Mat fourier = planos[0];
        fourier += Scalar::all(1);
        log(fourier, fourier);
        fourier = fourier(Rect(0, 0, fourier.cols & -2, fourier.rows & -2));
        int cx = fourier.cols/2;
        int cy = fourier.rows/2;
        Mat q0(fourier, Rect(0, 0, cx, cy));
        Mat q1(fourier, Rect(cx, 0, cx, cy));
        Mat q2(fourier, Rect(0, cy, cx, cy));
        Mat q3(fourier, Rect(cx, cy, cx, cy));

        Mat tmp;
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);

        q1.copyTo(tmp);
        q2.copyTo(q1);
        tmp.copyTo(q2);
        Mat exibicao;
        normalize(fourier, exibicao, 0, 255, NORM_MINMAX, CV_8UC4);
        QImage imgIn= QImage((uchar*) exibicao.data, exibicao.cols, exibicao.rows, exibicao.step, QImage::Format_Grayscale8);
        w->imG = QPixmap::fromImage(imgIn);
        w->zoomImage();
        w->scene->setSceneRect(0, 0, (w->imG).width(), (w->imG).height());
        w->scene->addPixmap(w->imG);
        break;
    }
    case SOBEL:{
        MatHsv teste(einstein);
        Mat testeb = teste.toRGB();
        std::cout << testeb.channels() << std::endl;
        imshow("teste", testeb);
        waitKey();
        break;
    }
    case NEGATIVOHSV:
        Vnegative(einstein);
        break;
    case GRAYSCALE:
        grayscale(einstein);
        break;
    case COLORS:
    {
        std::vector<float> values = w->retrieveBars();
        HSVcell cor(values[0], values[1], values[2]);
        std::cout << values[0] << " - " << values[1] << " - " << values[2] << " - " << w->retrievePerc() << std::endl;
        colorFilter(einstein, cor, w->retrievePerc());
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
    case FOURIER:
        LimpaLimiarizacao();
        break;
    }
}

void MainWindow::on_ShowFourier_clicked()
{
    w->saveView("../images/ufourier.jpg");
    Mat fourier = imread("../images/ufourier.jpg", IMREAD_GRAYSCALE);

    int cx = fourier.cols/2;
    int cy = fourier.rows/2;
    Mat q0(fourier, Rect(0, 0, cx, cy));
    Mat q1(fourier, Rect(cx, 0, cx, cy));
    Mat q2(fourier, Rect(0, cy, cx, cy));
    Mat q3(fourier, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    int w = getOptimalDFTSize(fourier.cols);
    int h = getOptimalDFTSize(fourier.rows);

    Mat operando;
    fourier.convertTo(operando, CV_32FC1);

    Mat inversa;
    dft(operando, inversa, DFT_INVERSE|DFT_REAL_OUTPUT);

    inversa = inversa(Rect(0, 0, inversa.cols & -2, inversa.rows & -2));

    Mat final;
    inversa.convertTo(final, CV_8UC1);

    imshow("bananas", final);
    waitKey();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    w = &window;
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleText(HIDE);
    w->ToggleBars(HIDE);
    window.show();
    return a.exec();
}

void MainWindow::on_gaussianBrush_stateChanged(int arg1)
{
    if(arg1 == 2){
        gaussianB = true;
    }
    else{
        gaussianB = false;
    }
}

void MainWindow::on_Limiarizacao_clicked()
{
    algoritmo = LIMIARIZACAO;
    w->ToggleText(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleBars(HIDE);
    LimpaLimiarizacao();
    w->ToggleGraphics(SHOW);
}


void MainWindow::on_Logaritmo_clicked()
{
    algoritmo = LOGARITMO;
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleBars(HIDE);
    w->setTextualPlaceholder(templateAlg[LOGARITMO]);
    w->clearText();
    w->ToggleText(SHOW);
}


void MainWindow::on_Negativo_clicked()
{
    algoritmo = NEGATIVO;
    w->ToggleGraphics(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleText(HIDE);
}


void MainWindow::on_Gamma_clicked()
{
    algoritmo = GAMMA;
    w->ToggleGraphics(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleFourierTools(HIDE);
    w->setTextualPlaceholder(templateAlg[GAMMA]);
    w->clearText();
    w->ToggleText(SHOW);
}

void MainWindow::on_Escanografia_clicked()
{
    algoritmo = ESCANOGRAFIA;
    w->ToggleGraphics(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleFourierTools(HIDE);
    w->setTextualPlaceholder(templateAlg[ESCANOGRAFIA]);
    w->clearText();
    w->ToggleText(SHOW);
}

void MainWindow::on_Kernel_clicked()
{
    algoritmo = KERNEL;
    w->ToggleGraphics(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleFourierTools(HIDE);
    w->setTextualPlaceholder(templateAlg[KERNEL]);
    w->clearText();
    w->ToggleText(SHOW);
}

void MainWindow::on_Fourier_clicked()
{
    algoritmo = FOURIER;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleFourierTools(SHOW);
    w->ToggleGraphics(SHOW);
}

void MainWindow::on_Sobel_clicked()
{
    algoritmo = SOBEL;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_NegativoHSV_clicked()
{
    algoritmo = NEGATIVOHSV;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Grayscale_clicked()
{
    algoritmo = GRAYSCALE;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Colors_clicked()
{
    algoritmo = COLORS;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleBars(SHOW);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}


