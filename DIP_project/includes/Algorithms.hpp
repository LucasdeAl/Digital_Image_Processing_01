#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

using namespace cv;
using namespace std;
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs
void HistogramEqualization(Mat image);

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
