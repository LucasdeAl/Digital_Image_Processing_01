#include "mainwindow.h"

#include <QApplication>
#include <utility>
#include <Algorithms.hpp>

MainWindow* w;

enum Algorithms{NONE, LIMIARIZACAO, LOGARITMO, NEGATIVO, GAMMA, ESCANOGRAFIA, KERNEL, FOURIER, SOBEL, MEDIA, MEDIANA, TRANSFORMS, LAPLACIAN, GRAYSCALE, NEGATIVOHSV, COLORS, CMY, SEPIA_CHROMA, HISTOGRAMA};
enum Visibility{HIDE, SHOW};
const char* templateAlg[] ={"",
    "",
    "TEMPLATE: base num, c num;\n\nCLEAR - LIMPA TEXTO",
    "",
    "TEMPLATE: c num, gamma num;\n\nCLEAR - LIMPA TEXTO",
    "TEMPLATE: msg;\n\nSET - ESCREVE NA IMAGEM\n\nCLEAR - LIMPA O BLOCO DE TEXTO E LÊ DA IMAGEM",
    "TEMPLATE: tam (lin, col), [l1c1, l1c2, ... l1cn, l2c1, ... lmcn]\n\nSET - APLICA KERNEL NA IMAGEM",
    "",
    "",
    "TEMPLATE: tam (lin, col), [l1c1, l1c2, ... l1cn, l2c1, ... lmcn]\n\nSET - APLICA A MEDIA\n\nSE FOR USAR MEDIA ARITMETICA, PONHA APENAS OS TAMANHOS SEGUIDOS DE ';'",
    "TEMPLATE: tam (lin, col);\n\nSET - APLICA A MEDIANA"};

Algorithms algoritmo = NONE;

std::vector<std::pair<float, float>> equacionaRetas(std::vector<QPointF> buffer);

Mat reference = Mat(cv::Size(1,1), 0);
Mat scanimg = Mat(cv::Size(1,1), 0);
bool can_draw = false;
bool gaussianB = false;
bool adjustRGB = false;
bool setChroma = false;
bool histHSV = false;
bool histEq = false;
bool sobX = false, sobY = false;
bool pond = false;
bool isHB = false;
bool isRot = false, isBilin = false;
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
    std::string imgname = "../images/";
    std::string imgpath = imgname + w->getName();
    Mat einstein = imread(imgpath, IMREAD_UNCHANGED);
    Mat saved = Mat(einstein);
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
        saved = limiarizacaoPorPartes(w->vertexBuffer, eqBuffer, einstein);
        imgpath = "../images/limiar.jpg";
        imgname = "limiar.jpg";
        break;}
    case LOGARITMO:{
        read = w->getText();
        auto dataLog = parseLog(read);
        saved = logarithm(einstein, dataLog.first, dataLog.second, true);
        imgpath = "../images/log.jpg";
        imgname = "log.jpg";
        break;}
    case NEGATIVO:
        saved = negative(einstein);
        imgpath = "../images/neg.jpg";
        imgname = "neg.jpg";
        break;
    case GAMMA:{
        read = w->getText();
        auto dataGamma = parseGamma(read);
        saved = gammaC(einstein, dataGamma.first, dataGamma.second);
        imgpath = "../images/gamma.jpg";
        imgname = "gamma.jpg";
        break;}
    case ESCANOGRAFIA:{
        read = w->getText();
        auto dataScanography = parseScan(read);
        saved = scanographyWrite(einstein, dataScanography);
        imgpath = "../images/scan.jpg";
        imgname = "scan.jpg";
        break;
    }
    case KERNEL:{
        read = w->getText();
        auto dataKernel = parseKernel(read);
        auto matsize = dataKernel.second;
        saved = appKernel(einstein, dataKernel.first, matsize);
        imgpath = "../images/kernel.jpg";
        imgname = "kernel.jpg";
        break;
    }
    case FOURIER:{
        Mat ajuste;
        MatHsv original(einstein);
        if(einstein.channels() == 3){
            einstein = returnGray(einstein);
        }
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
        if(sobX && sobY){
            saved = appKernelSobelMagnitude(einstein);
            imgpath = "../images/sobelmag.jpg";
            imgname = "sobelmag.jpg";
        } else if(sobX){
            saved = appKernelSobelX(einstein);
            imgpath = "../images/sobelx.jpg";
            imgname = "sobelx.jpg";
        } else{
            saved = appKernelSobelY(einstein);
            imgpath = "../images/sobely.jpg";
            imgname = "sobely.jpg";
        }
        break;
    }
    case MEDIA:{
        if(pond){
            auto info = parseKernel(w->getText());
            saved = appKernelWeightedMean(einstein, info.first, info.second);
            imgpath = "../images/ponderada.jpg";
            imgname = "ponderada.jpg";
        }else{
            auto info = parseKernelSize(w->getText());
            saved = appKernelSimpleMean(einstein, info);
            imgpath = "../images/media.jpg";
            imgname = "media.jpg";
        }
        break;
    }
    case MEDIANA:{
        auto info = parseKernelSize(w->getText());
        saved = appKernelMedian(einstein, info);
        imgpath = "../images/mediana.jpg";
        imgname = "mediana.jpg";
        break;
    }
    case TRANSFORMS:{
        if(isRot){
            if(isBilin){
                saved = rotateBilinear(einstein, (w->retrieveTransQ()/5)*360);
                imgpath = "../images/rotbilin.jpg";
                imgname = "rotbilin.jpg";
            } else{
                saved = rotate(einstein, (w->retrieveTransQ()/5)*360);
                imgpath = "../images/rot.jpg";
                imgname = "rot.jpg";
            }
        } else{
            if(isBilin){
                saved = changeScaleBilinear(einstein, w->retrieveTransQ());
                imgpath = "../images/scalebilin.jpg";
                imgname = "scalebilin.jpg";
            } else{
                saved = changeScale(einstein, w->retrieveTransQ());
                imgpath = "../images/scale.jpg";
                imgname = "scale.jpg";
            }
        }
        break;
    }
    case LAPLACIAN:{
        if(isHB){
            saved = appKernelHighBoost(einstein, w->retrieveBoost());
            imgpath = "../images/highboost.jpg";
            imgname = "highboost.jpg";
        } else{
            saved = appKernelLaplacian(einstein);
            imgpath = "../images/laplacian.jpg";
            imgname = "laplacian.jpg";
        }
        break;
    }
    case NEGATIVOHSV:
        saved = Vnegative(einstein);
        imgpath = "../images/hsvneg.jpg";
        imgname = "hsvneg.jpg";
        break;
    case GRAYSCALE:
        saved = grayscale(einstein);
        imgpath = "../images/grayscale.jpg";
        imgname = "grayscale.jpg";
        break;
    case COLORS:
    {
        std::vector<float> values = w->retrieveBars();
        HSVcell cor(values[0], values[1], values[2]);
        saved = colorFilterHSV(einstein, cor);
        imgpath = "../images/hsv.jpg";
        imgname = "hsv.jpg";
        break;
    }
    case CMY:{
        std::vector<float> values = w->retrieveBars();
        if(adjustRGB){
            saved = colorFilterRGB(einstein, values);
            imgpath = "../images/rgb.jpg";
            imgname = "rgb.jpg";
        }
        else{
            saved = colorFilterCMY(einstein, values);
            imgpath = "../images/cmy.jpg";
            imgname = "cmy.jpg";
        }
        break;
    }
    case SEPIA_CHROMA:{
        if(setChroma){
            std::string secondPath = "../images/";
            secondPath = secondPath + w->retrieveSecondFile();
            Mat secondImg = imread(secondPath);
            saved = applyChromaKey(einstein, w->retrieveDist(), secondImg);
            imgpath = "../images/chromakey.jpg";
            imgname = "chromakey.jpg";
        }
        else{
            saved = applySepia(einstein);
            imgpath = "../images/sepia.jpg";
            imgname = "sepia.jpg";
        }
        break;
    }
    case HISTOGRAMA:{
        if(histEq){
            Mat equalized(HistogramEqualization(einstein));
            saveImage("../images/equalized.jpg", equalized);
            showHistogram("../images/equalized.jpg");
            imgpath = "../images/equalized.jpg";
            imgname = "equalized.jpg";
        }
        else{
            showHistogram(imgpath);
        }
        break;
    }
    }
    if(algoritmo != FOURIER){
        saveImage(imgpath, saved);
        w->setPath(imgname);
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
            Mat scanimg = imread("../images/scan.jpg", IMREAD_UNCHANGED);
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

    Mat final;
    inversa.convertTo(final, CV_8UC1);

    imshow("Transformada Inversa", final);
    waitKey();
    destroyWindow("Transformada Inversa");
    saveImage("../images/ift.jpg", final);
    setPath("ift.jpg");
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
    w->ToggleChroma(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleSobel(HIDE);
    w->TogglePond(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleTransform(HIDE);
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

void MainWindow::on_wantRGB_stateChanged(int arg1)
{
    if(arg1 == 2){
        adjustRGB = true;
    }
    else{
        adjustRGB = false;
    }
}

void MainWindow::on_checkChroma_stateChanged(int arg1)
{
    if(arg1 == 2){
        setChroma = true;
        w->ToggleDist(SHOW);
    }
    else{
        setChroma = false;
        w->ToggleDist(HIDE);
    }
}

void MainWindow::on_histogramaEq_stateChanged(int arg1)
{
    if(arg1 == 2){
        histEq = true;
    }
    else{
        histEq = false;
    }
}

void MainWindow::on_histogramaHSV_stateChanged(int arg1)
{
    if(arg1 == 2){
        histHSV = true;
    }
    else{
        histHSV = false;
    }
}

void MainWindow::on_sobelY_stateChanged(int arg1)
{
    if(arg1 == 2){
        sobY = true;
    }
    else{
        sobY = false;
    }
}

void MainWindow::on_sobelX_stateChanged(int arg1)
{
    if(arg1 == 2){
        sobX = true;
    }
    else{
        sobX = false;
    }
}

void MainWindow::on_mediaPond_stateChanged(int arg1)
{
    if(arg1 == 2){
        pond = true;
    }
    else{
        pond = false;
    }
}

void MainWindow::on_highBoost_stateChanged(int arg1)
{
    if(arg1 == 2){
        isHB = true;
    }
    else{
        isHB = false;
    }
}

void MainWindow::on_rotCheck_stateChanged(int arg1)
{
    if(arg1 == 2){
        isRot = true;
    }
    else{
        isRot = false;
    }
}

void MainWindow::on_bilinCheck_stateChanged(int arg1)
{
    if(arg1 == 2){
        isBilin = true;
    }
    else{
        isBilin = false;
    }
}

void MainWindow::on_Limiarizacao_clicked()
{
    algoritmo = LIMIARIZACAO;
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleSobel(HIDE);
    w->TogglePond(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleChroma(HIDE);
    LimpaLimiarizacao();
    w->ToggleGraphics(SHOW);
}


void MainWindow::on_Logaritmo_clicked()
{
    algoritmo = LOGARITMO;
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleHist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleChroma(HIDE);
    w->setTextualPlaceholder(templateAlg[LOGARITMO]);
    w->clearText();
    w->ToggleText(SHOW);
}


void MainWindow::on_Negativo_clicked()
{
    algoritmo = NEGATIVO;
    w->ToggleGraphics(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleFourierTools(HIDE);
    w->ToggleText(HIDE);
}


void MainWindow::on_Gamma_clicked()
{
    algoritmo = GAMMA;
    w->ToggleGraphics(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleHist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
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
    w->ToggleSobel(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleChroma(HIDE);
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
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleChroma(HIDE);
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
    w->ToggleSobel(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleFourierTools(SHOW);
    w->ToggleGraphics(SHOW);
}

void MainWindow::on_Sobel_clicked()
{
    algoritmo = SOBEL;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleSobel(SHOW);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_NegativoHSV_clicked()
{
    algoritmo = NEGATIVOHSV;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->TogglePond(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Grayscale_clicked()
{
    algoritmo = GRAYSCALE;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleHist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Colors_clicked()
{
    algoritmo = COLORS;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleBars(SHOW);
    w->ToggleDist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleHist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
    w->hideRGBCheck();
}

void MainWindow::on_adjustCMY_clicked()
{
    algoritmo = CMY;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleBars(SHOW);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_sep_chrom_clicked()
{
    algoritmo = SEPIA_CHROMA;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(SHOW);
    w->ToggleTransform(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Histograma_clicked()
{
    algoritmo = HISTOGRAMA;
    LimpaLimiarizacao();
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(SHOW);
    w->ToggleBoost(HIDE);
    w->ToggleTransform(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_radioButton_8_clicked()
{
    algoritmo = MEDIA;
    LimpaLimiarizacao();
    w->ToggleText(SHOW);
    w->setTextualPlaceholder(templateAlg[MEDIA]);
    w->ToggleTransform(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(SHOW);
    w->ToggleHist(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_radioButton_13_clicked()
{
    algoritmo = MEDIANA;
    LimpaLimiarizacao();
    w->ToggleTransform(HIDE);
    w->ToggleText(SHOW);
    w->setTextualPlaceholder(templateAlg[MEDIANA]);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleBoost(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Laplacian_clicked()
{
    algoritmo = LAPLACIAN;
    LimpaLimiarizacao();
    w->ToggleBoost(SHOW);
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->ToggleTransform(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

void MainWindow::on_Transforms_clicked()
{
    algoritmo = TRANSFORMS;
    LimpaLimiarizacao();
    w->ToggleTransform(SHOW);
    w->ToggleBoost(HIDE);
    w->ToggleText(HIDE);
    w->ToggleDist(HIDE);
    w->TogglePond(HIDE);
    w->ToggleHist(HIDE);
    w->ToggleSobel(HIDE);
    w->ToggleChroma(HIDE);
    w->ToggleBars(HIDE);
    w->ToggleGraphics(HIDE);
    w->ToggleFourierTools(HIDE);
}

