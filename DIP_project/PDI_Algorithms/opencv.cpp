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




void linear(Mat image, vector<pair<int,int >> vertices)
{
    Mat newImage = image.clone();
    int matrix[image.rows][image.cols],b,g,r;
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();

    pair<int,int> zero (image.rows-1,0);
    pair<int,int> end (0,image.cols-1);




    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 0]/255.0); // B
            g =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 1]/255.0); // G
            r =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 2]/255.0); // R

            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 0] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 0]; // B
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 1] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 1]; // G
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 2] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + 2]; // R  
        }
    }

    
    String windowName = "Correção Linear por partes"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}

void logarithm(Mat image, double base, double c, bool isNormalized)
{
    Mat newImage = image.clone();
    double b,g,r,bmax=1,gmax=1,rmax=1;
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();
    
    if(isNormalized)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 0]/255.0); // B
                g =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 1]/255.0); // G
                r =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 2]/255.0); // R

                if(b>bmax)
                {
                    bmax = b;
                }
                if(g>gmax)
                {
                    gmax = g;
                }
                if(r>rmax)
                {
                    rmax = r;
                }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 0]/255.0); // B
            g =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 1]/255.0); // G
            r =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 2]/255.0); // R

            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 0] = (uint8_t)ceil(((log(b+1)/log(base))/bmax)*255*c);
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 1] = (uint8_t)ceil(((log(g+1)/log(base))/gmax)*255*c);
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 2] = (uint8_t)ceil(((log(r+1)/log(base))/rmax)*255*c);

        }
    }

    
    String windowName = "Correção Logarítimica"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}

void negative(Mat image)
{
    Mat negative = image.clone();
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

    
    String windowName = "Imagem Negativa"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, negative); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}


void gammaC(Mat image,double c,double gamma)
{
    Mat newImage = image.clone();
    double b,g,r;
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 0]/255.0); // B
            g =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 1]/255.0); // G
            r =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + 2]/255.0); // R

            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 0] = (uint8_t)ceil((c*pow(b,gamma))*255);
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 1] = (uint8_t)ceil((c*pow(g,gamma))*255);
            pixelNewImagePtr[i*newImage.cols*cn + j*cn + 2] = (uint8_t)ceil((c*pow(r,gamma))*255);

        }
    }

    
    String windowName = "Correção Gamma"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

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
//gammaC(image,1,1.4);
logarithm(image,2,1,true);

 String windowName = "Imagem"; 

 namedWindow(windowName); // Create a window

 imshow(windowName, image); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}