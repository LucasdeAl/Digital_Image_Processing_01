#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <iostream>
#include <vector>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<std::pair<float, float>> vertexBuffer;
    QBrush paint;
    QBrush mark;
    QGraphicsScene *scene;
    QRadialGradient grad;
    QPixmap imG;
    void ToggleGraphics(int v);
    void ToggleText(int v);
    void setTextualPlaceholder(const char* s);
    std::string getText();
    std::string getName();
    void clearText();
    int retrieveBrushSlider();
    void ToggleFourierTools(int v);
    void ToggleBars(int v);
    void resizeView();
    void saveView(QString fileName);
    void zoomImage();
    void hideRGBCheck();
    void ToggleChroma(int v);
    int retrieveDist();
    void ToggleDist(int v);
    std::string retrieveSecondFile();
    std::vector<float> retrieveBars();
    void ToggleHist(int v);
    void ToggleSobel(int v);
    void setPath(std::string path);
    void TogglePond(int v);
    void ToggleBoost(int v);
    double retrieveBoost();
    void ToggleTransform(int v);
    double retrieveTransQ();
    ~MainWindow();

private slots:
    void on_Set_clicked();

    void on_Clear_clicked();

    void on_Limiarizacao_clicked();

    void on_Logaritmo_clicked();

    void on_Negativo_clicked();

    void on_Gamma_clicked();

    void on_Quit_clicked();

    void on_Escanografia_clicked();

    void on_Kernel_clicked();

    void on_Fourier_clicked();

    void on_gaussianBrush_stateChanged(int arg1);

    void on_ShowFourier_clicked();

    void on_Sobel_clicked();

    void on_NegativoHSV_clicked();

    void on_Grayscale_clicked();

    void on_Colors_clicked();

    void on_wantRGB_stateChanged(int arg1);

    void on_adjustCMY_clicked();

    void on_sep_chrom_clicked();

    void on_checkChroma_stateChanged(int arg1);

    void on_Histograma_clicked();

    void on_histogramaEq_stateChanged(int arg1);

    void on_histogramaHSV_stateChanged(int arg1);

    void on_sobelY_stateChanged(int arg1);

    void on_sobelX_stateChanged(int arg1);

    void on_radioButton_8_clicked();

    void on_mediaPond_stateChanged(int arg1);

    void on_radioButton_13_clicked();

    void on_Laplacian_clicked();

    void on_highBoost_stateChanged(int arg1);

    void on_Transforms_clicked();

    void on_rotCheck_stateChanged(int arg1);

    void on_bilinCheck_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
