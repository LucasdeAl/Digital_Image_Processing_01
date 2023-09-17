#include "../includes/Algorithms.hpp"
#include <iterator>
//g++ opencv.cpp -o opencv -lopencv_core -lopencv_highgui -lopencv_imgcodecs
void HistogramEqualization(Mat image)
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
            cout << equalizedHistogram[k][i];       
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
    
    String windowName = "Equlização de Histograma"; 

    namedWindow(windowName); // Create a window

    imshow(windowName, newImage); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window
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
