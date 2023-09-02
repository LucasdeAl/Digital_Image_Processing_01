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
void scanographyRead(Mat image);
void scanographyWrite(Mat image);

void linear(Mat image, vector<pair<int,int >> vertices);

void logarithm(Mat image, double base, double c, bool isNormalized);

void negative(Mat image);

void gammaC(Mat image,double c,double gamma);
