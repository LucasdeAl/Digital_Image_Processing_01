#include <Algorithms.hpp>
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs
void scanographyWrite(Mat image)
{
    Mat newImage = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();
    //applying the mask
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for(int k = 0; k< cn; k++)
            {
                pixelNewImagePtr[i*newImage.cols*cn + j*cn + k] = (pixelImagePtr[i*image.cols*cn + j*cn + k] & 0b11111110);
            } 
        }
    }
    string teste = "Deus não joga dados";
    teste.push_back('\0');
    bool bits[8*teste.length()];
    int k = 0;
    for(int i = 0; i < teste.length(); i++)
    {
        for( int j = 0 ; j < 8; j++)
        {
            bits[k] = (teste[i]>>j)&1;
            k++;
        }
    }
    int c = 0;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for(int k = 0; k< cn; k++)
            {
                if(c<8*teste.length())
                {
                    pixelNewImagePtr[i*newImage.cols*cn + j*cn + k] = (pixelNewImagePtr[i*image.cols*cn + j*cn + k] | bits[c]);
                    c++;
                }
                else
                    break;
            } 
        }
    }
  bool check = imwrite("./einsteinText.tif", newImage);
  if (check == false) {
    cout << "Saving the image, FAILED" << endl;
    cin.get();  
  }
  cout << " Image saved successfully " << endl;
  //Image has been saved to the desired location
    //scanographyRead(newImage);


    int count = 0;
    char letter = 0;
    string extractedText;

    for (int i = 0; i < image.rows; i++) 
    {
        for (int j = 0; j < image.cols; j++) 
        {
            for (int k = 0; k < cn; k++) 
            {
                if (count == 8)
                {
                    if (letter == '\0')
                    {
                        goto done_extraction; // Exit the loop when null character is encountered
                    }
                    extractedText.push_back(letter);
                    count = 0;
                    letter = 0;
                }
                letter = letter|((pixelNewImagePtr[i * image.cols * cn + j * cn + k] & 1)<<count);
                count++;
            }
        }
    }

    done_extraction:
    cout << "Texto extraido: " << extractedText << endl;
    cout<<"deu pau";

    String windowName = "Contém texto escondido"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}


void scanographyRead(Mat image)
{
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    int cn = image.channels();
    int count = 0;
    char letter;
    string text;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for(int k = 0; k< cn; k++)
            {
                if(count == 8)
                {
                    text = text + letter;
                    count = 0;
                    letter = 0;
                }
                letter = (letter<<count)|(pixelImagePtr[i*image.cols*cn + j*cn + k] & 1);
            } 
        }
    }

    text = text + '\0';
    cout<<'\n'+text+'\n';
   
}

//not done yet
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
            for( int k = 0 ; k < cn; k++)
                pixelNegativePtr[i*negative.cols*cn + j*cn + k] = 255 - pixelImagePtr[i*image.cols*cn + j*cn + k];
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
    double b;
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for(int k = 0 ; k < cn; k++)
            {
                b =  ((double)pixelImagePtr[i*image.cols*cn + j*cn + k]/255.0); 
                pixelNewImagePtr[i*newImage.cols*cn + j*cn + k] = (uint8_t)ceil((c*pow(b,gamma))*255);
            }

        }
    }

    
    String windowName = "Correção Gamma"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}

std::vector<std::pair<float, float>> equacionaRetas(std::vector<std::pair<float, float>> buffer){
    std::vector<std::pair<float, float>> equacoes;
    for(int i = 1; i < buffer.size(); i++){
        float a = (buffer[i].second - buffer[i-1].second)/(buffer[i].first - buffer[i-1].first);
        float b = buffer[i].second - (a * buffer[i].first);
        equacoes.emplace_back(a, b);
    }
    return equacoes;
}

void limiarizacaoPorPartes(std::vector<std::pair<float, float>> buffer, std::vector<std::pair<float, float>> equacoes, Mat image){
    Mat limiar = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelLimiarPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelLimiarPtr = (uint8_t*)limiar.data;
    int cn = image.channels();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++)
                for(int it = 1; it < buffer.size(); it++){
                    int pixelIndex = i*image.cols*cn + j*cn + k;
                    if(pixelImagePtr[pixelIndex] < round(buffer[it].first)){
                        pixelLimiarPtr[i*limiar.cols*cn + j*cn + k] = round(abs((((float)pixelImagePtr[pixelIndex]) * equacoes[it-1].first) + equacoes[it-1].second - 255));
                        break;
                    }
                }
        }
    }


    String windowName = "Imagem Limiarizada";

    namedWindow(windowName); // Create a window

    imshow(windowName, limiar); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
}
