
#include <iostream>
#include <cmath>


#include "include/bitmap.hpp"

using namespace std;


struct Point{
    int x;
    int y;
};

struct Box{
    Point corner;
    int width;
    int height;
};

struct MandelBrotFractal{
    const int width;
    const int height;
    const unsigned int max_iteration;
    int* const historgram;
    int* const image;
    const double scale;
    const Point center;
};

void DrawMandelBrot(MandelBrotFractal fractal, Box box){

    for (int x = box.corner.x; x < box.corner.x + box.width; x++){
        for (int y = box.corner.y; y < box.corner.y + box.height; y++){
            double x_bias = ( (2*x - static_cast<double>(fractal.width))/2.0 + fractal.center.x ) 
                                / fractal.scale * 2.0/static_cast<double>(fractal.width);
            double y_bias = ( (2*y - static_cast<double>(fractal.height))/2.0 + fractal.center.y ) 
                                / fractal.scale * 2.0/static_cast<double>(fractal.height);
            double zx = 0;
            double zy = 0;
            int itr = 0;
            while(itr!=fractal.max_iteration){
                double temp_zx = zx*zx - zy*zy + x_bias;
                double temp_zy = 2.0*zx*zy + y_bias;
                if(sqrt(temp_zx*temp_zx + temp_zy*temp_zy) > 2){
                    break;
                }
                zx = temp_zx;
                zy = temp_zy;
                itr++;
            }
            fractal.image[y*fractal.width+x] = itr;
            fractal.historgram[itr]++;
        }
    }

}

enum class Color{red, green, blue};

void saveToImage(MandelBrotFractal m_b_fractal, char* file_name, Color color, int color_option = 1){

    Bitmap* bmp = new Bitmap(m_b_fractal.width,m_b_fractal.height);

    int total = 0;
    for(int i=0; i<m_b_fractal.max_iteration; i++){
        total = total + m_b_fractal.historgram[i];
    }
    for (int x = 0; x<m_b_fractal.width; x++){
        for (int y = 0; y<m_b_fractal.height; y++){

            uint8_t shade = 0;
            double alpha = 0;
            for(int i = 0; i<= m_b_fractal.image[y*m_b_fractal.width+x]; i++){
                alpha += static_cast<double>(m_b_fractal.historgram[i])/total;
            }
            switch (color_option) {
                case 1:
                    shade = alpha * 255;
                    break;
                case 2:
                    shade = pow(255, alpha);
                    break;
            }
            switch (color)
            {
                case Color::red:
                    bmp->setPixel(x, y, shade, 0, 0 );
                    break;
                case Color::green:
                    bmp->setPixel(x, y, 0, shade, 0 );
                    break;
                case Color::blue:
                    bmp->setPixel(x, y, 0, 0, shade );
                    break;
                default:
                    break;
            }
            
        }
    }
    bmp->save(file_name);
}

int main(int argc, const char * argv[]) {

    const size_t width = 3840/5;
    const size_t height = 2160/5;
    const unsigned int max_iteration = 1000;
    int* historgram = new int[max_iteration + 1]{0};
    int* fractal = new int[width * height]{0};
    const double scale = 1.0;

    Point center{0, 0};
    

    MandelBrotFractal mandel_brot_fractal{width,height,max_iteration,
                                            historgram, fractal, 
                                            scale, center};
    
    DrawMandelBrot(mandel_brot_fractal, Box{Point{0,0}, 384, 216});
    DrawMandelBrot(mandel_brot_fractal, Box{Point{1*384,0*216},384,216});
    DrawMandelBrot(mandel_brot_fractal, Box{Point{0*384,1*216},384,216});
    DrawMandelBrot(mandel_brot_fractal, Box{Point{1*384,1*216},384,216});
    
    char file_name[]{"MandelBrot.bmp"};
    saveToImage(mandel_brot_fractal, file_name, Color::red);

    delete[] historgram;
    delete[] fractal;
    return 0;
    
}