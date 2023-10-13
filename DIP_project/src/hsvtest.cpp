#include <cstdint>
#include <iostream>
#include <cmath>
#include <inttypes.h>
#define pi 3.14159
#define max2(x, y) ((x>y)?x:y)
#define min2(x, y) ((x<y)?x:y)

struct RGBcell{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGBcell(int red, int green, int blue){
        r = red;
        g = green;
        b = blue;
    }
};

struct HSVcell{
    float h;
    float s;
    float v;
    HSVcell(RGBcell c){
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
        h = ((2*pi/6)*((g-b)/maxdiff) + (2*pi));
        if(h > 2*pi){
            h -= 2*pi;
        }
    } else if(cmax == g){
        h = ((2*pi/6)*((b-r)/maxdiff) + (2*pi/3));
        if(h > 2*pi){
            h -= 2*pi;
        }
    } else if(cmax == b){
        h = ((2*pi/6)*((r-g)/maxdiff) + (4*pi/3));
        if(h > 2*pi){
            h -= 2*pi;
        }
    } else {h = 0; s = 0; v = 0;}

    if(cmax == 0){
        s = 0;
    } else{
        s = (maxdiff/cmax);
    }
    v = cmax;
    }

    RGBcell toRGB(){
        float red = 0, green = 0, blue = 0;
    if(h >= 0 && h < pi/3){
        red = 255;
        green = h*255/(pi/3);
    } else if(h >= pi/3 && h < 2*pi/3){
        green = 255;
        red = 255 - ((h - pi/3)*255/(pi/3));
    }
    else if(h >= 2*pi/3 && h < pi){
        green = 255;
        blue = (h - 2*pi/3)*255/(pi/3);
    }
    else if(h >= pi && h < 4*pi/3){
        blue = 255;
        green = 255 - ((h - pi)*255/(pi/3));
    }
    else if(h >= 4*pi/3 && h < 5*pi/3){
        blue = 255;
        red = (h - 4*pi/3)*255/(pi/3);
    }
    else if(h >= 5*pi/3 && h <= 2*pi){
        red = 255;
        blue = 255 - ((h - 5*pi/3)*255/(pi/3));
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
};

int main(){
    RGBcell random(255, 255, 255);
    for(int i = 0; i < 1000; i++){
        HSVcell test(random);
        test.v = 0.001 * i;
        RGBcell aux = test.toRGB();
        float diff = (((float)(random.r))/1000)*i - aux.b;
        if(std::abs(diff) > 0.45){
            std::cout << diff << std::endl;
        }
    }
    
}