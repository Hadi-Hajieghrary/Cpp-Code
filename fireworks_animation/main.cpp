
//
//  main.cpp
//  Particle Explosion
//
//  Created by Hadi Hajieghrary on 2/20/20.
//  Copyright © 2020 Hadi Hajieghrary. All rights reserved.
//
#include <chrono>
#include <cstdio>

#include "canvas.hpp"


using namespace std;

int main(int argc, const char * argv[]) {
    
    
    srand(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));
    Color background_color_(0x00000000);
    
    const int winWidth = 1600;
    const int winHeight = 1200;
    Canvas main_window("Main Canvas", winWidth, winHeight);
    
    Color backgroung_color(0x00,0X00,0x00);
    main_window.setBackgroundColor(backgroung_color);
    main_window.Update();

    const int paericles_number = 5000;
    // Initialize the particles at the center of the screen
    double particles[paericles_number][2]{0};
    double particles_speed[paericles_number]{0};
    double particles_direction[paericles_number]{0};
    const double max_speed = 0.1;
    for (int i=0;i<paericles_number; i++) {
        particles_speed[i] = max_speed * (static_cast<double>(rand())/RAND_MAX);
        particles_speed[i] *= particles_speed[i];
        particles_direction[i] = M_PI * (static_cast<double>(rand())/RAND_MAX * 2.0 - 1.0);
    }
    Color particle_color(0xFF,0XFF,0x00);
    
    bool quit_window = false;
    SDL_Event event;
    int im = 0;
    while(!quit_window){
        for (int i=0;i<paericles_number; i++){
            int x = winWidth/2*particles[i][0]+winWidth/2-1;
            int y = winHeight/2*particles[i][1]+winHeight/2-1;
            //Canvas::setPixel(buffer, winWidth, winHeight, x, y, backgroung_color);
            // Curl the particles path
            particles_direction[i] = particles_direction[i] + 1e-1 * (1.0 - 2.0 * static_cast<double>(rand())/RAND_MAX);
            particles[i][0] = particles[i][0] + particles_speed[i] * cos(particles_direction[i]);
            particles[i][1] = particles[i][1] + particles_speed[i] * sin(particles_direction[i]);
            if (particles[i][0]>1.0){
                particles_speed[i] = 0.0;
                particles_direction[i] = 0.0;
                continue;
            }
            if (particles[i][1]>1.0){
                particles_speed[i] = 0.0;
                particles_direction[i] = 0.0;
                continue;
            }
            if (particles[i][0]<-1.0){
                particles_speed[i] = 0.0;
                particles_direction[i] = 0.0;
                continue;
            }
            if (particles[i][1]<-1.0) {
                particles_speed[i] = 0.0;
                particles_direction[i] = 0.0;
                continue;
            }
            x = winWidth/2*particles[i][0]+winWidth/2-1;
            y = winHeight/2*particles[i][1]+winHeight/2-1;
            main_window.setPixel(x, y, particle_color);

        }
        Box blur_box{Point{0,0}, 2, 2};
        main_window.Blur(blur_box,100);
        // Save the frame
        char bmp_name[20]{0};
        std::sprintf(bmp_name,"img%03d.bmp",im);
        im++;
        main_window.Update();
        main_window.saveCurrentFrame(bmp_name);
        //SDL_Delay(10);
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) quit_window = true;
        }
    }

    return 0;
}
