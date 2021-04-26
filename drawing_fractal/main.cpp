
#include <iostream>
#include <cmath>

#include "include/bitmap.hpp"

using namespace std;

void DrawMandelBrot(const char* file_name, int width, int height, int cX = 0, int cY = 0, double scale = 1, int colorOpt = 1, int max_iteration = 1000){
    
    Bitmap canvas(width,height);
    
    int* historgram = new int[max_iteration+1]{0};
    int* fractal = new int[width*height]{0};
    
    
    for (int X = 0; X < width; X++){
        for (int Y = 0; Y < height; Y++){
            //double x = (2*X-width)/2 * 2.0 /width;
            //double y = (2*Y-height)/2 * 2.0 /height;
            
            double x = ( (2*X-width)/2 + cX ) / scale * 2.0/width;
            double y = ( (2*Y-height)/2 + cY) / scale * 2.0/height;
            
            
            double zx = 0;
            double zy = 0;
            
            int itr = 0;
            while(itr!=max_iteration){
                double temp_zx = zx*zx - zy*zy + x;
                double temp_zy = 2.0*zx*zy + y;
                if(sqrt(temp_zx*temp_zx + temp_zy*temp_zy) > 2){
                    break;
                }
                zx = temp_zx;
                zy = temp_zy;
                itr++;
            }
            fractal[Y*width+X] = itr;
            historgram[itr]++;
        }
    }
    
    int total = 0;
    for(int i=0; i<max_iteration; i++){
        total = total + historgram[i];
    }
    
    for (int X = 0; X<width; X++){
        for (int Y = 0; Y<height; Y++){
            
            uint8_t red = 0;
            uint8_t green = 0;
            uint8_t blue = 0;
            double alpha = 0;
            for(int i=0; i<= fractal[Y*width+X]; i++){
                alpha += static_cast<double>(historgram[i])/total;
            }
            switch (colorOpt) {
                case 1:
                    green = alpha * 255;
                    break;
                case 2:
                    green = pow(255, alpha);
                    break;
            }
            canvas.setPixel(X, Y, red, green, blue );
        }
    }
    
    delete[] historgram;
    delete[] fractal;
    canvas.save(file_name);

}



int main(int argc, const char * argv[]) {
    
    DrawMandelBrot("MandelBrot.bmp", 3840 , 2160, 0, 0, 1, 1);

    return 0;
}