#include "../includes/Algorithms.hpp"
#include <iterator>
#include <stdlib.h>
#include <eigen3/Eigen/Dense>
//g++ $(pkg-config --cflags eigen3) mainTest.cpp -o mainTest -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc
RGBcell::RGBcell(int red, int green, int blue){
    r = red;
    g = green;
    b = blue;
}

RGBcell::RGBcell(){
    r = 0; g = 0; b = 0;
}

HSVcell::HSVcell(float hue, float saturation, float value){
    h = hue; s = saturation; v = value;
}

HSVcell::HSVcell(){
    h = 0; s = 0; v = 0;
}

HSVcell::HSVcell(RGBcell c){
    float r = ((float)(c.r))/255;
    float g = ((float)(c.g))/255;
    float b = ((float)(c.b))/255;
    float cmax = max2(r, max2(g, b));
    float cmin = min2(r, min2(g, b));
    float maxdiff = cmax - cmin;
    if(maxdiff == 0){
        h = 0;
    }
    else if(cmax == r){
        h = ((2*Pi/6)*((g-b)/maxdiff) + (2*Pi));
        if(h > 2*Pi){
            h -= 2*Pi;
        }
    } else if(cmax == g){
        h = ((2*Pi/6)*((b-r)/maxdiff) + (2*Pi/3));
        if(h > 2*Pi){
            h -= 2*Pi;
        }
    } else if(cmax == b){
        h = ((2*Pi/6)*((r-g)/maxdiff) + (4*Pi/3));
        if(h > 2*Pi){
            h -= 2*Pi;
        }
    } else {h = 0; s = 0; v = 0;}

    if(cmax == 0){
        s = 0;
    } else{
        s = (maxdiff/cmax);
    }
    v = cmax;
}

RGBcell HSVcell::toRGB(){
    float red = 0, green = 0, blue = 0;
    if(h >= 0 && h < Pi/3){
        red = 255;
        green = h*255/(Pi/3);
    } else if(h >= Pi/3 && h < 2*Pi/3){
        green = 255;
        red = 255 - ((h - Pi/3)*255/(Pi/3));
    }
    else if(h >= 2*Pi/3 && h < Pi){
        green = 255;
        blue = (h - 2*Pi/3)*255/(Pi/3);
    }
    else if(h >= Pi && h < 4*Pi/3){
        blue = 255;
        green = 255 - ((h - Pi)*255/(Pi/3));
    }
    else if(h >= 4*Pi/3 && h < 5*Pi/3){
        blue = 255;
        red = (h - 4*Pi/3)*255/(Pi/3);
    }
    else if(h >= 5*Pi/3 && h <= 2*Pi){
        red = 255;
        blue = 255 - ((h - 5*Pi/3)*255/(Pi/3));
    }
    else{
        red = 0; green = 0; blue = 0;
    }
    red += (255 - red) * (1 - s);
    green += (255 - green) * (1 - s);
    blue += (255 - blue) * (1 - s);

    red *= v;
    green *= v;
    blue *= v;

    return RGBcell(round(red), round(green), round(blue));
}

MatHsv::MatHsv(cv::Mat image){
    {
        try{
            uint8_t* pixelImagePtr;
            pixelImagePtr = (uint8_t*)image.data;
            int cn = image.channels();
            std::vector<HSVcell> linha;
            for(int i = 0; i < image.rows; i++)
            {
                linha.clear();
                for(int j = 0; j < image.cols; j++)
                {
                    int initial = i*image.cols*cn + j*cn;
                    if(cn == 3){
                        int red = pixelImagePtr[initial];
                        int green = pixelImagePtr[initial + 1];
                        int blue = pixelImagePtr[initial + 2];
                        linha.emplace_back(RGBcell(red, green, blue));
                    } else if(cn == 1){
                        int gray = pixelImagePtr[initial];
                        linha.emplace_back(RGBcell(gray, gray, gray));
                    }
                    else{
                        throw "Erro, imagem em outro formato de cores que não é RGB ou GRAYSCALE";
                    }
                }
                data.push_back(linha);
            }
            empty = false;
            isGray = cn == 1;
        }
        catch(const char* e){
            std::cout << e << std::endl;
            data.clear();
            empty = true;
        }
    }
}

MatHsv::MatHsv(){
    empty = true;
    isGray = false;
}

cv::Mat MatHsv::toRGB(){
    if(!isGray){
        cv::Mat image(data.size(), data[0].size(), CV_8UC3);
        uint8_t* pixelImagePtr;
        pixelImagePtr = (uint8_t*)image.data;
        int cn = image.channels();
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                RGBcell eq = data[i][j].toRGB();
                pixelImagePtr[i*image.cols*cn + j*cn] = eq.r;
                pixelImagePtr[i*image.cols*cn + j*cn + 1] = eq.g;
                pixelImagePtr[i*image.cols*cn + j*cn + 2] = eq.b;
            }
        }
        return image;
    }
    else{
        cv::Mat image(data.size(), data[0].size(), CV_8UC1);
        uint8_t* pixelImagePtr;
        pixelImagePtr = (uint8_t*)image.data;
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                RGBcell eq = data[i][j].toRGB();
                pixelImagePtr[i*image.cols + j] = eq.r;
            }
        }
        return image;
    }
}

bool saveImage(const std::string& path, const cv::Mat& image) 
{
    if (image.empty()) 
    {
        std::cerr << "Error: Input image is empty." << std::endl;
        return false;
    }

    bool success = cv::imwrite(path, image);

    if (success) 
    {
        std::cout << "Image saved successfully to " << path << std::endl;
    } else 
    {
        std::cerr << "Error: Unable to save image to " << path << std::endl;
    }

    return success;
}

int showHistogram(string path)
{
    Mat src = imread(path);
    if( src.empty() )
    {
    return EXIT_FAILURE;
    }
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for( int i = 1; i < histSize; i++ )
    {
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
    Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
    Scalar( 255, 0, 0), 2, 8, 0 );
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
    Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
    Scalar( 0, 255, 0), 2, 8, 0 );
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
    Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
    Scalar( 0, 0, 255), 2, 8, 0 );
    }
    imshow("Source image", src );
    imshow("calcHist Demo", histImage );
    waitKey();
    return EXIT_SUCCESS;
}

int showHistogramHSV(string path)
{
    Mat src = imread(path);
    if (src.empty())
    {
        return EXIT_FAILURE;
    }

    MatHsv image(src);

    std::vector<std::vector<HSVcell>> hsvData = image.data;
    std::vector<float> vValues;

    for (int i = 0; i < hsvData.size(); i++)
    {
        for (int j = 0; j < hsvData[i].size(); j++)
        {
            vValues.push_back(hsvData[i][j].v);
        }
    }

    int histSize = 256;
    float range[] = { 0, 256 }; 
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    Mat v_hist;
    float max =0;
    for(float value: vValues)
    {
        if(value>max) max = value;
    }
    std::vector<uint8_t> vValuesUint8;
    for (size_t i = 0; i < vValues.size(); i++) {
        uint8_t normalizedValue = static_cast<uint8_t>((vValues[i] / max) * 255.0);
        vValuesUint8.push_back(normalizedValue);
    }

    cv::Mat vValuesMat(vValuesUint8); 

    calcHist(&vValuesMat, 1, 0, Mat(), v_hist, 1, &histSize, histRange, uniform, accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(v_hist, v_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(v_hist.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(v_hist.at<float>(i))),
             Scalar(255, 255, 255), 2, 8, 0); 
    }

    imshow("Source image", src);
    imshow("Histogram of V Component", histImage);
    waitKey();
    return EXIT_SUCCESS;
}

Mat HistogramEqualization(Mat image)
{
    Mat newImage = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelNewImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelNewImagePtr = (uint8_t*)newImage.data;
    int cn = image.channels();
    int histogram[3][256],equalizedHistogram[3][256];
    double probability[3][256];
    double dimensions = (double)(image.rows*image.cols);
    double aux;
    // initiation of histogram
    for(int k = 0 ; k < cn; k++)
    {    
        for(int i = 0; i < 256; i++)
        {  
            histogram[k][i] = 0;
        }
    }
    // creating histograms for each channel
    for(int k = 0 ; k < cn; k++)
    {    
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                histogram[k][pixelImagePtr[i*image.cols*cn + j*cn + k]]++;
            }
        }
    }
    
    // calculating probabilities
    for(int k = 0 ; k < cn; k++)
    {    
        for(int i = 0; i < 256; i++)
        {  
            probability[k][i] = (double)(histogram[k][i])/dimensions;
        }
    }

    // calculating cumulative probabilities 
    for(int k = 0 ; k < cn; k++)
    {   
        aux = 0; 
        for(int i = 0; i < 256; i++)
        {
            probability[k][i] += aux;
            aux = probability[k][i];
        }
    }
    //round cumulative probabilities multiplied by image max intensity
    for(int k = 0 ; k < cn; k++)
    {   
        for(int i = 0; i < 256; i++)
        {
            equalizedHistogram[k][i] = ceil(probability[k][i]*255);  
            //cout << equalizedHistogram[k][i];       
        }
    }

    // generating new image
    for(int k = 0 ; k < cn; k++)
    {    
        for(int i = 0; i < image.rows; i++)
        {
            for(int j = 0; j < image.cols; j++)
            {
                pixelNewImagePtr[i*image.cols*cn + j*cn + k] = (uint8_t)equalizedHistogram[k][pixelImagePtr[i*image.cols*cn + j*cn + k]];
            }
        }
    }    
    return newImage;
}


std::string parseScan(std::string s){
    try{
        int indice = 0;
        for(int i = 0; i < s.size(); i++){
            if( s[i] == ';'){
                indice = i;
                break;
            }
        }
        if(indice == 0){
            throw "Erro: Você digitou algo?";
        }
        else{
            return s.substr(0, indice);
        }
    }
    catch(const char* c){
        std::cout << c << std::endl;
        return std::string(c);
    }
}


cv::Mat scanographyWrite(Mat image,string p)
{
    if(8*p.size() - 1 > image.cols*image.rows*image.channels())
    {
        cout << "Texto muito longo!"<<endl;
        return image;
    }
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
   
    p.push_back('\0');
    bool bits[8*p.length()];
    int k = 0;
    for(int i = 0; i < p.length(); i++)
    {
        for( int j = 0 ; j < 8; j++)
        {
            bits[k] = (p[i]>>j)&1;
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
                if(c<8*p.length())
                {
                    pixelNewImagePtr[i*newImage.cols*cn + j*cn + k] = (pixelNewImagePtr[i*image.cols*cn + j*cn + k] | bits[c]);
                    c++;
                }
                else
                    break;
            } 
        }
    }
   return newImage;
}


string scanographyRead(Mat image)
{
    uint8_t* pixelImagePtr;
    pixelImagePtr = (uint8_t*)image.data;
    int cn = image.channels();
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
                letter = letter|((pixelImagePtr[i * image.cols * cn + j * cn + k] & 1)<<count);
                count++;
            }
        }
    }

    done_extraction:
    //cout << "Texto extraido: " << extractedText << endl;
    return extractedText;
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

    namedWindow(windowName);

    imshow(windowName, limiar);

    waitKey(0);

    destroyWindow(windowName);
}

double converteDigito(char c){
    if (c > '9' || c < '0'){
        throw "Número inválido";
    }
    return (double)(c-'0');
}

double converteString(std::string s){
    int indice = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == '.'){
            indice = i;
            break;
        }
        else{
            indice = s.size() - 1;
        }
    }
    if(indice == 0 && s[indice] == '.'){
        throw "Número estranho";
    }
    int i = indice, j = indice; if(i != s.size() - 1) i -= 1;
    double result = converteDigito(s[i]);
    int cont = 1;
    while(i >= 0 or j <= s.size() - 1){
        i--; j++;
        if(j < s.size()){
            result += converteDigito(s[j]) * std::pow(10, (double)(-cont));
        }
        if(i >=0 ){
            result += converteDigito(s[i]) * std::pow(10, (double)cont);
        }
        cont++;
    }
    return result;
}

double parseNumber(int ini, std::string s, int *fim){
    int num_of_dots = 0; bool isNegative = false; double d = 0;
    while(s[ini] == ' '){
        ini++;
    }
    if(s[ini] == '-'){
    	isNegative = true; ini++;
    }
    for(int i = ini; i < s.size(); i++){
        if (s[i] > '9' || (s[i] < '0' && s[i] != '.')){
            *fim = i;
            if(isNegative){
            	d = -converteString(s.substr(ini, i - ini).c_str());
            }
            else{
            	d = converteString(s.substr(ini, i - ini).c_str());
            }
            return d;
        }
        else{
            if(s[i] == '.'){
                num_of_dots++;
                if(num_of_dots > 1){
                    throw "Número tem 2 ou mais pontos!";
                }
            }
        }
    }
    throw "Algo não está no formato da template!";
}
std::pair<double, double> parseLog(std::string s){
    double base = 0, c = 0;
    int* p = new int(0);
    try{
        if(s.substr(0, 5).compare("base ") == 0){
            base = parseNumber(5, s, p);
            if(s.substr(*p, 4).compare(", c ") == 0){
                c = parseNumber(*p + 4, s, p);
                delete p;
                return std::pair(base, c);
            }
            else{
                throw "Faltou o valor de c!";
            }
        }
        else{
            throw "Formatação errada do input, por favor seguir a template!";
        }
    }
    catch(const char* err){
        delete p;
        std::cout << err << std::endl;
        return std::pair(2.0, 1.0);
    }
    catch(std::invalid_argument e){
        std::cout << "Não foi encontrado um número para um dos parâmetros!" << std::endl;
            return std::pair(2.0, 1.0);
    }
}

std::pair<double, double> parseGamma(std::string s){
    double c = 0, gamma = 0;
    int* p = new int(0);
    try{
            if(s.substr(0, 2).compare("c ") == 0){
            c = parseNumber(2, s, p);
            if(s.substr(*p, 8).compare(", gamma ") == 0){
                gamma = parseNumber(*p + 8, s, p);
                delete p;
                return std::pair(c, gamma);
            }
            else{
                throw "Faltou o valor de gamma!";
            }
            }
            else{
            throw "Formatação errada do input, por favor seguir a template!";
            }
    }
    catch(const char* err){
            delete p;
            std::cout << err << std::endl;
            return std::pair(1.0, 1.0);
    }
    catch(std::invalid_argument e){
            std::cout << "Não foi encontrado um número para um dos parâmetros!" << std::endl;
                return std::pair(2.0, 1.0);
    }
}

std::pair<std::vector<double>, std::pair<int, int>> parseKernel(std::string s){
    int l = 0, c = 0, *p = new int(0);
    std::vector<double> r;
    try{
        if(s.substr(0, 5).compare("tam (") == 0){
            l = parseNumber(5, s, p);
            if(s[*p] == ','){
                c = parseNumber(*p + 1, s, p);
                if(s.substr(*p, 4).compare("), [") == 0){
                    *p = *p+3;
                    do{
                        r.push_back(parseNumber(*p + 1, s, p));
                    } while(s[*p] == ',');
                    if(s[*p] != ']') throw "Kernel disposto de forma incorreta! (Não coloque espaços antes da vírgula, só depois)";
                    if(r.size() != l*c) throw "Número de elementos não bate com o tamanho do kernel!";
                    delete p;
                    return std::pair(r, std::pair(l, c));
                }
                else{
                    throw "Input errado entre o tamanho e o kernel!";
                }
            }
            else{
                throw "Faltou o valor do segundo tamanho! (Não coloque espaços antes da vírgula, só depois)";
            }
        }
        else{
            throw "Formatação errada do input, por favor seguir a template!";
        }
    }
    catch(const char* err){
        delete p;
        std::vector<double> temp;
        std::cout << err << std::endl;
        return std::pair(temp, std::pair(l, c));
    }
    catch(std::invalid_argument e){
        std::vector<double> temp;
        std::cout << "Não foi encontrado um número para um dos parâmetros!" << std::endl;
        return std::pair(temp, std::pair(l, c));
    }
}

void appKernel(Mat image, std::vector<double> kernel, std::pair<int, int> msize){
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = msize.first, col = msize.second;
    int reflin = lin/2, refcol = col/2;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * kernel[l*col + c];
                            }
                        }
                    }
                }
                pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round(result * 255.0);
            }
        }
    }


    String windowName = "Imagem com kernel customizado";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}




void appKernelSimpleMean(Mat image, std::pair<int, int> msize){
    int lin = msize.first, col = msize.second;
    double size = lin*col;
    std::vector<double> kernel(size,1); 
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    
    int reflin = lin/2, refcol = col/2;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * kernel[l*col + c];
                            }
                        }
                    }
                }
                result = result/size;
                pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round(result * 255.0);
            }
        }
    }


    String windowName = "Imagem com filtro média Aritmética simples";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}


void appKernelWeightedMean(Mat image, std::vector<double> kernel, std::pair<int, int> msize){
    int lin = msize.first, col = msize.second;
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int reflin = lin/2, refcol = col/2;
    double sum;
    for(int i : kernel) sum+=i;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * kernel[l*col + c];
                            }
                        }
                    }
                }
                result = result/sum;
                pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round(result * 255.0);
            }
        }
    }


    String windowName = "Imagem com filtro média Aritmética Ponderada";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}

void appKernelMedian(Mat image, std::pair<int, int> msize){
    std::vector<double> kernel;
    int tam = msize.first*msize.second;
    kernel.assign(tam,1);
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = msize.first, col = msize.second;
    int reflin = lin/2, refcol = col/2;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                std::vector<double> array;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                array.push_back((pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * kernel[l*col + c]);
                            }
                        }
                    }
                }
                std::sort(array.begin(), array.end(), std::greater<double>());
                pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round(array[(int)tam/2] * 255.0);
            }
        }
    }


    String windowName = "Imagem com filtro da mediana aplicado";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}
void appKernelSobelMagnitude(Mat image){
    std::vector<double> gx = {-1,0,1,-2,0,2,-1,0,1};
    std::vector<double> gy = {1,2,1,0,0,0,-1,-2,-1};
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = 3, col = 3;
    int reflin = lin/2, refcol = col/2;
    double maxNeg = 0;
    double maxAbs = 0;
    

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               if( sqrt(pow(result1,2)+pow(result2,2))> maxAbs)
               {
                maxAbs = sqrt(pow(result1,2)+pow(result2,2));
               }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round(sqrt(pow(result1,2)+pow(result2,2))/maxAbs * 255.0);               
            }
        }
    }


    String windowName = "Imagem com filtro de Sobel e x e y aplicado";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}



void appKernelSobelX(Mat image){
    std::vector<double> gx = {-1,0,1,-2,0,2,-1,0,1};
    //std::vector<double> gy = {1,2,1,0,0,0,-1,-2,-1};
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = 3, col = 3;
    int reflin = lin/2, refcol = col/2;
    double maxNeg = 0;
    double maxAbs = 0;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                //result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               if(maxNeg > result1)
               {
                maxNeg = result1;
               }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                //result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               if(abs(maxNeg) + result1 > maxAbs)
               {
                maxAbs = abs(maxNeg) + result1;
               }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                //result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               //cout << round((abs(result1)+maxNeg)/maxAbs * 255.0) << " "<< endl; 
               pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round((result1+abs(maxNeg))/maxAbs * 255.0);
               //pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round((result2+abs(maxNeg))/maxAbs * 255.0);

            }
        }
    }


    String windowName = "Imagem com filtro de Sobel em x Aplicado";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}

void appKernelSobelY(Mat image){
    //std::vector<double> gx = {-1,0,1,-2,0,2,-1,0,1};
    std::vector<double> gy = {1,2,1,0,0,0,-1,-2,-1};
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = 3, col = 3;
    int reflin = lin/2, refcol = col/2;
    double maxNeg = 0;
    double maxAbs = 0;
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                //result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               if(maxNeg > result2)
               {
                maxNeg = result2;
               }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                //result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               if(abs(maxNeg) + result2 > maxAbs)
               {
                maxAbs = abs(maxNeg) + result2;
               }
            }
        }
    }

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result1 = 0;
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                //result1 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gx[l*col + c];
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];

                            }
                        }
                    }
                }
               //cout << round((abs(result1)+maxNeg)/maxAbs * 255.0) << " "<< endl; 
               //pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round((result1+abs(maxNeg))/maxAbs * 255.0);
               pixelAppliedPtr[i*image.cols*cn + j*cn + k] = round((result2+abs(maxNeg))/maxAbs * 255.0);

            }
        }
    }


    String windowName = "Imagem com filtro de Sobel em y aplicado";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}

void appKernelLaplacian(Mat image){
    std::vector<double> gy = {1,1,1,1,-8,1,1,1,1};
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = 3, col = 3;
    int reflin = lin/2, refcol = col/2;
    
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result2 += (pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c];
                            }
                        }
                    }
                }
               double value = (((double)pixelImagePtr[i*image.cols*cn + j*cn + k]/255.0) - result2); 
               pixelAppliedPtr[i*image.cols*cn + j*cn + k] = ceil(value*255)>0? ceil(value*255)>255?255 : ceil(value*255): 0;

            }
        }
    }


    String windowName = "Aguçamento por lapalaciano";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}


void appKernelHighBoost(Mat image,double f){
    std::vector<double> gy = {1,2,1,2,4,2,1,2,1};
    Mat applied = image.clone();
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    double mascara;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    int cn = image.channels();
    int lin = 3, col = 3;
    int reflin = lin/2, refcol = col/2;
    
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            for( int k = 0 ; k < cn; k++){
                double result2 = 0;
                for(int l = 0; l < lin; l++){
                    for(int c = 0; c < col; c++){
                        int thisline = i + (l - reflin), thiscol = j + (c - refcol);
                        if(thisline >= 0 and thisline < image.rows){
                            if(thiscol >= 0 and thiscol < image.cols){
                                result2 += ((pixelImagePtr[thisline*image.cols*cn + thiscol*cn + k]/255.0) * gy[l*col + c]);
                            }
                        }
                    }
                }
               result2/=16;
               mascara = (pixelImagePtr[i*image.cols*cn + j*cn + k]/255.0) - result2;
               double value = round(255*mascara)*f+pixelImagePtr[i*image.cols*cn + j*cn + k];
               pixelAppliedPtr[i*image.cols*cn + j*cn + k] = value>255?255:value;
            }
        }
    }


    String windowName = "Aguçamento por High Boost";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}

void changeScale(Mat image,double scale){
    int newX = image.rows*scale;
    int newY = image.cols*scale;
    int cn = image.channels();
    Mat applied;
    if(cn==1) applied = Mat(newX,newY,CV_8UC1,Scalar::all(0));
    else applied = Mat(newX,newY,CV_8UC3,Scalar::all(0));
    uint8_t* pixelImagePtr;
    uint8_t* pixelAppliedPtr;
    pixelImagePtr = (uint8_t*)image.data;
    pixelAppliedPtr = (uint8_t*)applied.data;
    for(int k = 0; k < cn; k++)
    {
        for(int i = 0; i < newX; i++)
        {
            int x = (int)ceil(i/scale);
            for(int j = 0; j < newY; j++)
            {
                int y = (int)ceil(j/scale);
                pixelAppliedPtr[i*newY*cn + j*cn + k] = pixelImagePtr[x*image.cols*cn+y*cn+k];
            }
        }
    }

    String windowName = "Imagem com escala modificada por Interpolção por proximidade";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
}


void changeScaleBilinear(Mat image, double scale) {
    int newX = static_cast<int>(image.rows * scale);
    int newY = static_cast<int>(image.cols * scale);
    int cn = image.channels();
    Mat applied;
    if (cn == 1)
        applied = Mat(newX, newY, CV_8UC1, Scalar::all(0));
    else
        applied = Mat(newX, newY, CV_8UC3, Scalar::all(0));

    Eigen::MatrixXd A(4, 4);
    Eigen::VectorXd B(4);

    for (int i = 0; i < newX; i++) {
        for (int j = 0; j < newY; j++) {
            double x = i / scale;
            double y = j / scale;
            int x1 = static_cast<int>(x);
            int x2 = std::min(x1 + 1,image.cols-1);
            int y1 = static_cast<int>(y);
            int y2 = std::min(y1 + 1,image.cols-1);

            if(cn>1){
                for (int ch = 0; ch < 3; ch++) {
                    A << x1, y1, x1 * y1, 1,
                        x1, y2, x1 * y2, 1,
                        x2, y1, x2 * y1, 1,
                        x2, y2, x2 * y2, 1;
                
                    B << image.at<Vec3b>(x1, y1)[ch],
                        image.at<Vec3b>(x1, y2)[ch],
                        image.at<Vec3b>(x2, y1)[ch],
                        image.at<Vec3b>(x2, y2)[ch];

                    Eigen::VectorXd coefficients = A.colPivHouseholderQr().solve(B);
                    double a = coefficients(0);
                    double b = coefficients(1);
                    double c = coefficients(2);
                    double d = coefficients(3);
                    
                    applied.at<Vec3b>(i, j)[ch] = static_cast<uint8_t>(a * x + b * y + c * x * y + d);
                }
            }
                else
                {
                     A << x1, y1, x1 * y1, 1,
                        x1, y2, x1 * y2, 1,
                        x2, y1, x2 * y1, 1,
                        x2, y2, x2 * y2, 1;
                
                    B << image.at<uint8_t>(x1, y1),
                        image.at<uint8_t>(x1, y2),
                        image.at<uint8_t>(x2, y1),
                        image.at<uint8_t>(x2, y2);

                    Eigen::VectorXd coefficients = A.colPivHouseholderQr().solve(B);
                    double a = coefficients(0);
                    double b = coefficients(1);
                    double c = coefficients(2);
                    double d = coefficients(3);
                    
                    applied.at<uint8_t>(i, j) = static_cast<uint8_t>(a * x + b * y + c * x * y + d);
                }
            }
        }
    

    String windowName = "Imagem com escala modificada por interpolação Bilinear";

    namedWindow(windowName);

    imshow(windowName, applied);

    waitKey(0);

    destroyWindow(windowName);
    }

void rotate(cv::Mat image, double degrees) {
    double rads = (-Pi / 180) * degrees;
    double diag = sqrt(image.rows * image.rows + image.cols * image.cols);
    int newX = static_cast<int>(ceil(diag));
    int newY = newX;
    int cn = image.channels();

    cv::Mat applied;
    if (cn == 1)
        applied = cv::Mat(newX, newY, CV_8UC1, cv::Scalar::all(0));
    else
        applied = cv::Mat(newX, newY, CV_8UC3, cv::Scalar::all(0));

    for (int i = 0; i < newX; i++) {
        for (int j = 0; j < newY; j++) {
             double x = (i - newX / 2) * std::cos(rads) + (j - newY / 2) * std::sin(rads) + image.cols / 2;
            double y = -(i - newX / 2) * std::sin(rads) + (j - newY / 2) * std::cos(rads) + image.rows / 2;

            if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                if (cn > 1) {
                    for (int ch = 0; ch < 3; ch++) {
                         applied.at<cv::Vec3b>(j, i)[ch] = image.at<cv::Vec3b>(y, x)[ch];
                    }
                } else {
                    applied.at<uint8_t>(j, i) =  image.at<uint8_t>(y, x);
                }
            }
        }
    }


    cv::String windowName = "Imagem rotacionada por Interpolação por proximidade";
    cv::namedWindow(windowName);
    cv::imshow(windowName, applied);
    cv::waitKey(0);
    cv::destroyWindow(windowName);
}


void rotateBilinear(cv::Mat image, double degrees) {
    double rads = (-Pi / 180) * degrees;
    double diag = sqrt(image.rows * image.rows + image.cols * image.cols);
    int newX = static_cast<int>(ceil(diag));
    int newY = newX;
    int cn = image.channels();

    cv::Mat applied;
    if (cn == 1)
        applied = cv::Mat(newX, newY, CV_8UC1, cv::Scalar::all(0));
    else
        applied = cv::Mat(newX, newY, CV_8UC3, cv::Scalar::all(0));

    for (int i = 0; i < newX; i++) {
        for (int j = 0; j < newY; j++) {
             double x = (i - newX / 2) * std::cos(rads) + (j - newY / 2) * std::sin(rads) + image.cols / 2;
            double y = -(i - newX / 2) * std::sin(rads) + (j - newY / 2) * std::cos(rads) + image.rows / 2;

            if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                if (cn > 1) {
                    for (int ch = 0; ch < 3; ch++) {
                        cv::Mat A(4, 4, CV_64F);
                        cv::Mat B(4, 1, CV_64F);

                        int x1 = static_cast<int>(x);
                        int x2 = x1 + 1;
                        int y1 = static_cast<int>(y);
                        int y2 = y1 + 1;

                        A.at<double>(0, 0) = x1;
                        A.at<double>(0, 1) = y1;
                        A.at<double>(0, 2) = x1 * y1;
                        A.at<double>(0, 3) = 1;

                        A.at<double>(1, 0) = x1;
                        A.at<double>(1, 1) = y2;
                        A.at<double>(1, 2) = x1 * y2;
                        A.at<double>(1, 3) = 1;

                        A.at<double>(2, 0) = x2;
                        A.at<double>(2, 1) = y1;
                        A.at<double>(2, 2) = x2 * y1;
                        A.at<double>(2, 3) = 1;

                        A.at<double>(3, 0) = x2;
                        A.at<double>(3, 1) = y2;
                        A.at<double>(3, 2) = x2 * y2;
                        A.at<double>(3, 3) = 1;

                        B.at<double>(0, 0) = image.at<cv::Vec3b>(y1, x1)[ch];
                        B.at<double>(1, 0) = image.at<cv::Vec3b>(y2, x1)[ch];
                        B.at<double>(2, 0) = image.at<cv::Vec3b>(y1, x2)[ch];
                        B.at<double>(3, 0) = image.at<cv::Vec3b>(y2, x2)[ch];

                        cv::Mat coefficients = A.inv() * B;
                        double a = coefficients.at<double>(0, 0);
                        double b = coefficients.at<double>(1, 0);
                        double c = coefficients.at<double>(2, 0);
                        double d = coefficients.at<double>(3, 0);

                        applied.at<cv::Vec3b>(j, i)[ch] = static_cast<uint8_t>(a * x + b * y + c * x * y + d);
                    }
                } else {
                    cv::Mat A(4, 4, CV_64F);
                    cv::Mat B(4, 1, CV_64F);

                    int x1 = static_cast<int>(x);
                    int x2 = x1 + 1;
                    int y1 = static_cast<int>(y);
                    int y2 = y1 + 1;

                    A.at<double>(0, 0) = x1;
                    A.at<double>(0, 1) = y1;
                    A.at<double>(0, 2) = x1 * y1;
                    A.at<double>(0, 3) = 1;

                    A.at<double>(1, 0) = x1;
                    A.at<double>(1, 1) = y2;
                    A.at<double>(1, 2) = x1 * y2;
                    A.at<double>(1, 3) = 1;

                    A.at<double>(2, 0) = x2;
                    A.at<double>(2, 1) = y1;
                    A.at<double>(2, 2) = x2 * y1;
                    A.at<double>(2, 3) = 1;

                    A.at<double>(3, 0) = x2;
                    A.at<double>(3, 1) = y2;
                    A.at<double>(3, 2) = x2 * y2;
                    A.at<double>(3, 3) = 1;

                    B.at<double>(0, 0) = image.at<uint8_t>(y1, x1);
                    B.at<double>(1, 0) = image.at<uint8_t>(y2, x1);
                    B.at<double>(2, 0) = image.at<uint8_t>(y1, x2);
                    B.at<double>(3, 0) = image.at<uint8_t>(y2, x2);

                    cv::Mat coefficients = A.inv() * B;
                    double a = coefficients.at<double>(0, 0);
                    double b = coefficients.at<double>(1, 0);
                    double c = coefficients.at<double>(2, 0);
                    double d = coefficients.at<double>(3, 0);

                    applied.at<uint8_t>(j, i) = static_cast<uint8_t>(a * x + b * y + c * x * y + d);
                }
            }
        }
    }


    cv::String windowName = "Imagem rotacionada por Interpolação Bilinear";
    cv::namedWindow(windowName);
    cv::imshow(windowName, applied);
    cv::waitKey(0);
    cv::destroyWindow(windowName);
}


//------------------------------------------------------------------------------------------------

void Vnegative(Mat image){
    MatHsv a(image);
    for(int i = 0; i < a.data.size(); i++){
        for(int j = 0; j < a.data[0].size(); j++){
            if(a.isGray){
                a.data[i][j].h = Pi + a.data[i][j].h;
                if(a.data[i][j].h < 0){
                   a.data[i][j].h += 2*Pi;
                }
                else if (a.data[i][j].h >= 2*Pi){
                   a.data[i][j].h -= 2*Pi;
                }

                    a.data[i][j].s = 1 - a.data[i][j].s;
            }
            a.data[i][j].v = 1 - a.data[i][j].v;
        }
    }
    cv::Mat result(a.toRGB());
    imshow("Negativo em HSV", result);
    waitKey();
}

void grayscale(Mat image){
    MatHsv a(image);
    for(int i = 0; i < a.data.size(); i++){
        for(int j = 0; j < a.data[0].size(); j++){
            a.data[i][j].s = 0;
        }
    }
    cv::Mat result(a.toRGB());
    imshow("Escala de Cinza", result);
    waitKey();
}

void colorFilter(Mat image, HSVcell color){
    MatHsv a(image);
    float huep = color.h/(2*Pi);
    for(int i = 0; i < a.data.size(); i++){
        for(int j = 0; j < a.data[0].size(); j++){
            a.data[i][j].h *= huep;
            a.data[i][j].s *= color.s;
            a.data[i][j].v *= color.v;
            }
    }
    cv::Mat result(a.toRGB());
    imshow("Imagem com filtro de cor", result);
}
