# Digital_Image_Processing_01

Work we did for the Digital Image Processing subject in Federal University of Fortaleza. It's an application made with Qt, in C++, and there you can enter an image (it will be searched in the images folder), and select the method of processing you would like to use with the radio button menu. Sometimes, an option of this menu can reveal more options, and sometimes it has an textual input with a template, but it's all in Brazilian Portuguese. Trust me, it is very intuitive, and the template should be enough.

Warning: Fourier is not working, and scale operations can be very slow due to linear algebra operations.

## Things we did:
Negative (HSV/RGB);
Limiarization;
Histogram Equalization (HSV/RGB);
Scanography;
General filters (Logarithm, Gamma, Kernel application, Kernel with mean, Kernel with weighted mean, Kernel with median, Sobel (X, Y, Magnitude), Laplacian, High boost);
Change scale and rotate (both Bilinear (SLOW!) and Nearest);
Grayscale;
Color filter (HSV, RGB and CMY);
Chroma Key;
Sepia;
