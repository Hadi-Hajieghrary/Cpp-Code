
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

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

void writeToConvas(MandelBrotFractal m_b_fractal, Bitmap* canvas, Color color, int color_option = 1, Box box = Box{Point{0,0}, 0, 0}){
    int total = 0;
    if(box.width == 0 || box.height == 0){
        box.width = m_b_fractal.width;
        box.height = m_b_fractal.height;
    }
    for(int i = 0; i<m_b_fractal.max_iteration; i++){
        total = total + m_b_fractal.historgram[i];
    }
    for (int x = box.corner.x; x < box.corner.x + box.width; x++){
        for (int y = box.corner.y; y< box.corner.y + box.height; y++){
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
                    canvas->setPixel(x, y, shade, 0, 0 );
                    break;
                case Color::green:
                    canvas->setPixel(x, y, 0, shade, 0 );
                    break;
                case Color::blue:
                    canvas->setPixel(x, y, 0, 0, shade );
                    break;
                default:
                    break;
            }
            
        }
    }
}

int main(int argc, const char * argv[]) {

    const size_t width = 7680;
    const size_t height = 4520;
    // Resolution of the Fractal
    const unsigned int max_iteration = 10000;
    int* historgram = new int[max_iteration + 1]{0};
    int* fractal = new int[width * height]{0};
    // Scale of the fractal.
    const double scale = 1.0;

    Point center{0, 0};

    MandelBrotFractal mandel_brot_fractal{width,height,max_iteration,
                                            historgram, fractal, 
                                            scale, center};
    
    std::chrono::duration<long,std::nano> exec_duration;
    std::chrono::steady_clock::time_point exec_start;
    std::chrono::steady_clock::time_point exec_end;

    Box quarters[4];
    quarters[0] = Box{Point{0*width/2,0*height/2}, width/2, height/2};
    quarters[1] = Box{Point{1*width/2,0*height/2}, width/2, height/2};
    quarters[2] = Box{Point{0*width/2,1*height/2}, width/2, height/2};
    quarters[3] = Box{Point{1*width/2,1*height/2}, width/2, height/2};

    // Creating Image
 
        /*
        // Concurrent
        // 1000 iteration 3840x2260 @ Sec: 23, MiliSec: 984, MicroSec: 879, NanoSec: 61
        // Processor Type Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz
        exec_start = std::chrono::steady_clock::now();
        DrawMandelBrot(mandel_brot_fractal, quarters[0]);
        DrawMandelBrot(mandel_brot_fractal, quarters[1]);
        DrawMandelBrot(mandel_brot_fractal, quarters[2]);
        DrawMandelBrot(mandel_brot_fractal, quarters[3]);
        exec_end = std::chrono::steady_clock::now();
        */
        
        ///*
        // Parallel
        // 1000 iteration 3840x2260 @ Sec: 9, MiliSec: 641, MicroSec: 461, NanoSec: 544
        // Processor Type Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz
        exec_start = std::chrono::steady_clock::now();
        thread t0(DrawMandelBrot, mandel_brot_fractal, quarters[0]);
        thread t1(DrawMandelBrot, mandel_brot_fractal, quarters[1]);
        thread t2(DrawMandelBrot, mandel_brot_fractal, quarters[2]);
        thread t3(DrawMandelBrot, mandel_brot_fractal, quarters[3]);
        t0.join();
        t1.join();
        t2.join();
        t3.join();
        exec_end = std::chrono::steady_clock::now();
        //*/

    exec_duration = exec_end - exec_start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(exec_duration);
    exec_duration -= secs;
    auto mili_secs = std::chrono::duration_cast<std::chrono::milliseconds>(exec_duration);
    exec_duration -= mili_secs;
    auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(exec_duration);
    exec_duration -= micro_secs;
    auto nano_secs = exec_duration;

    std::cout<< "Creating Image took:-> "<< "Sec: "<<secs.count()<<", MiliSec: "<<mili_secs.count()<<", MicroSec: "<<micro_secs.count()<<", NanoSec: "<<nano_secs.count()<<std::endl;
    char file_name[]{"MandelBrot.bmp"};
    
    
    // Writing to Bitmap Image
        
        Bitmap* canvas = new Bitmap(mandel_brot_fractal.width,mandel_brot_fractal.height);

        /*
        // Concurrent
        // 1000 iteration 3840x2260 @ Sec: 19, MiliSec: 332, MicroSec: 109, NanoSec: 881
        // Processor Type Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz
        exec_start = std::chrono::steady_clock::now();
        writeToConvas(mandel_brot_fractal, canvas, Color::red, 1);
        exec_end = std::chrono::steady_clock::now();
        */

        ///*
        // Parallel
        // 1000 iteration 3840x2260 @ Sec: 6, MiliSec: 834, MicroSec: 291, NanoSec: 320
        // Processor Type Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz
        exec_start = std::chrono::steady_clock::now();
        thread tp0(writeToConvas, mandel_brot_fractal, canvas, Color::red, 2, quarters[0]);
        thread tp1(writeToConvas, mandel_brot_fractal, canvas, Color::red, 2, quarters[1]);
        thread tp2(writeToConvas, mandel_brot_fractal, canvas, Color::red, 2, quarters[2]);
        thread tp3(writeToConvas, mandel_brot_fractal, canvas, Color::red, 2, quarters[3]);
        tp0.join();
        tp1.join();
        tp2.join();
        tp3.join();
        exec_end = std::chrono::steady_clock::now();
        //*/

    exec_duration = exec_end - exec_start;
    secs = std::chrono::duration_cast<std::chrono::seconds>(exec_duration);
    exec_duration -= secs;
    mili_secs = std::chrono::duration_cast<std::chrono::milliseconds>(exec_duration);
    exec_duration -= mili_secs;
    micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(exec_duration);
    exec_duration -= micro_secs;
    nano_secs = exec_duration;

    std::cout<< "Saving Image took:-> "<<"Sec: "<<secs.count()<<", MiliSec: "<<mili_secs.count()<<", MicroSec: "<<micro_secs.count()<<", NanoSec: "<<nano_secs.count()<<std::endl;

    canvas->save(file_name);
    
    delete[] historgram;
    delete[] fractal;
    delete canvas;
    return 0;
    
}