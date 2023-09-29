#include "Algorithms.cpp"
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


int main(int argc, char** argv)
{
    // Read the image file as
    // imread("default.jpg");
    Mat image = imread("../images/ursosEqualized.jpg");
  
    // Error Handling
    if (image.empty()) {
        cout << "Image File "
             << "Not Found" << endl;
  
        // wait for any key press
        cin.get();  
        return -1;
    }
  
    appKernelSobel(image);
    // Show Image inside a window with
    // the name provided
    //Mat textImage = scanographyWrite(image,"escrever qualquer coisa para testar a escanografia no RGB");
    //string p = scanographyRead(textImage);
    //cout << p <<endl;
    //imshow("Window Name", image);
    //showHistogram("../images/frutas.png");
    //Mat equalized = HistogramEqualization(image);
    //imshow("Window Name", equalized);
    //saveImage("../images/indioEqualized.webp",equalized);
    //showHistogram("../images/indioEqualized.webp");
    // Wait for any keystroke
    waitKey(0);
    return 0;
}