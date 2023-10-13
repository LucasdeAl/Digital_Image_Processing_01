#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

#define pi 3.14159
#define max2(x,y) ((x>y)?x:y)
#define min2(x,y) ((x<y)?x:y)

struct RGBcell{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGBcell(int red, int green, int blue);

    RGBcell();
};

struct HSVcell{
    float h;
    float s;
    float v;

    HSVcell(float hue, float saturation, float value);

    HSVcell();

    HSVcell(RGBcell c);

    RGBcell toRGB();
};

class MatHsv{
public:
    std::vector<std::vector<HSVcell>> data;
    bool empty;
    bool isGray;

    MatHsv();

    MatHsv(cv::Mat image);

    cv::Mat toRGB();
};

using namespace cv;
using namespace std;
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs 

bool saveImage(const std::string& path, const cv::Mat& image);

int showHistogram(string path);

cv::Mat HistogramEqualization(Mat image);

std::string parseScan(std::string s);

std::string scanographyRead(Mat image);

cv::Mat scanographyWrite(Mat image,string p);

void logarithm(Mat image, double base, double c, bool isNormalized);

void negative(Mat image);

void gammaC(Mat image,double c,double gamma);


std::vector<std::pair<float, float>> equacionaRetas(std::vector<std::pair<float, float>> buffer);


void limiarizacaoPorPartes(std::vector<std::pair<float, float>> buffer, std::vector<std::pair<float, float>> equacoes, Mat image);

double converteDigito(char c);

double converteString(std::string s);

double parseNumber(int ini, std::string s, int *fim);

std::pair<double, double> parseLog(std::string s);

std::pair<double, double> parseGamma(std::string s);

std::pair<std::vector<double>, std::pair<int, int>> parseKernel(std::string s);

void appKernel(Mat image, std::vector<double> kernel, std::pair<int, int> msize);

//----------------------------------------------------------------------------------------------------------------------
//ALGORITMOS EM HSV

void Vnegative(Mat image);

void grayscale(Mat image);

void colorFilter(Mat image, HSVcell color);
