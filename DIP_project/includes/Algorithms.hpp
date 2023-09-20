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

using namespace cv;
using namespace std;
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs 

bool saveImage(const std::string& path, const cv::Mat& image);

int showHistogram(string path);

cv::Mat HistogramEqualization(Mat image);

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
