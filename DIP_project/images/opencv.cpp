

int main(int argc, char** argv)
{
 // Read the image file
 Mat image = imread("./images/einstein.tif",IMREAD_UNCHANGED);

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }



//negative(image);
//gammaC(image,1,1.4);
//logarithm(image,10,1,false);
 scanographyWrite(image);
 Mat image2 = imread("./einsteinText.tif",IMREAD_UNCHANGED);
 scanographyRead(image2);

 String windowName = "Imagem"; 

 namedWindow(windowName); // Create a window

 imshow(windowName, image); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}
