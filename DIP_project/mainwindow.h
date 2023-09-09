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
    QGraphicsScene *scene;
    void ToggleGraphics(int v);
    void ToggleText(int v);
    void setTextualPlaceholder(const char* s);
    std::string getText();
    void clearText();
    ~MainWindow();

private slots:
    void on_Set_clicked();

    void on_Clear_clicked();

    void on_Limiarizacao_clicked();

    void on_Linear_clicked();

    void on_Logaritmo_clicked();

    void on_Negativo_clicked();

    void on_Gamma_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
