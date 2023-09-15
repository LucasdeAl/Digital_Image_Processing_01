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
    Mat image = imread("../images/einstein.tif",
                       IMREAD_GRAYSCALE);
  
    // Error Handling
    if (image.empty()) {
        cout << "Image File "
             << "Not Found" << endl;
  
        // wait for any key press
        cin.get();
        return -1;
    }
  
    // Show Image inside a window with
    // the name provided
    imshow("Window Name", image);
  
    // Wait for any keystroke
    waitKey(0);
    return 0;
}