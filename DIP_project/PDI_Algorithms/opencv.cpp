#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs


void negative(Mat image)
{
    Mat negative = image.clone();
    double matrix[image.rows][image.cols];
    uint8_t* pixelImagePtr;
    uint8_t* pixelNegativePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNegativePtr = (uint8_t*)negative.data;
    int cn = image.channels();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            pixelNegativePtr[i*negative.cols*cn + j*cn + 0] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 0]; // B
            pixelNegativePtr[i*negative.cols*cn + j*cn + 1] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 1]; // G
            pixelNegativePtr[i*negative.cols*cn + j*cn + 2] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 2]; // R  
        }
    }

    
    String windowName = "Negative Image"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, negative); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}


void gammaC(Mat image,double c,double gamma)
{
    Mat negative = image.clone();
    double matrix[image.rows][image.cols],b,g,r;
    uint8_t* pixelImagePtr;
    uint8_t* pixelNegativePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNegativePtr = (uint8_t*)negative.data;
    int cn = image.channels();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 0]/255.0); // B
            g =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 1]/255.0); // G
            r =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 2]/255.0); // R

            pixelNegativePtr[i*negative.cols*cn + j*cn + 0] = (uint8_t)ceil((c*pow(b,gamma))*255);
            pixelNegativePtr[i*negative.cols*cn + j*cn + 1] = (uint8_t)ceil((c*pow(g,gamma))*255);
            pixelNegativePtr[i*negative.cols*cn + j*cn + 2] = (uint8_t)ceil((c*pow(r,gamma))*255);

        }
    }

    
    String windowName = "Negative Image"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, negative); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}

int main(int argc, char** argv)
{
 // Read the image file
 Mat image = imread("./einstein.tif");

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }



//negative(image);
gammaC(image,1,0.4);

 String windowName = "Imagem"; 

 namedWindow(windowName); // Create a window

 imshow(windowName, image); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}