
#include "canvas.hpp"


void Color::setRed(const uint8_t red)
    {
        red_ = red;
        uint32_t color_mask = 0x00FFFFFF;
        color_ = color_ & color_mask;
        color_mask = red_;
        color_mask<<=24;
        color_ = color_ | color_mask;
    }
        
void Color::setGreen(const uint8_t green)
    {
        green_ = green;
        uint32_t color_mask = 0xFF00FFFF;
        color_ = color_ & color_mask;
        color_mask = green_;
        color_mask<<=16;
        color_ = color_ | color_mask;
        
    }
        
void Color::setBlue(const uint8_t blue)
    {
        blue_ = blue;
        uint32_t color_mask = 0xFFFF00FF;
        color_ = color_ & color_mask;
        color_mask = blue_;
        color_mask<<=8;
        color_ = color_ | color_mask;
    }
        
void Color::setAlpha(const uint8_t alpha)
    {
        alpha_ = alpha;
        uint32_t color_mask = 0xFFFF00FF;
        color_ = color_ & color_mask;
        color_mask = alpha_;
        color_ = color_ | color_mask;
    }
     
void Color::setRGBA(const uint32_t RGBA)
    {
        color_ = RGBA;
        red_ = (color_ & 0xFF000000) >> 24;
        green_ = (color_ & 0x00FF0000) >> 16;
        blue_ = (color_ & 0x0000FF00) >> 8 ;
        alpha_ = (color_ & 0x000000FF) >> 0;
    }
        
uint8_t Color::getRed() const
    {
        return red_;
    }
    
uint8_t Color::getGreen() const
    {
        return green_;
    }
        
uint8_t Color::getBlue() const
    {
        return blue_;
    }
    
uint8_t Color::getAlpha() const
    {
        return alpha_;
    }

uint32_t Color::getRGBA() const
    {
        return color_;
    }
    
uint8_t Color::getRed(const uint32_t RGBA)
    {
        uint8_t red = (RGBA & 0xFF000000) >> 24;
        return red;
    }

uint8_t Color::getGreen(const uint32_t RGBA)
    {
        uint8_t green = (RGBA & 0x00FF0000) >> 16;
        return green;
    }

uint8_t Color::getBlue(const uint32_t RGBA)
    {
        uint8_t blue = (RGBA & 0x0000FF00) >> 8;
        return blue;
    }

uint8_t Color::getAlpha(const uint32_t RGBA)
    {
        uint8_t alpha = (RGBA & 0x000000FF);
        return alpha;
    }

uint32_t Color::getRGBA(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
    {
            uint32_t color = 0x00000000;
            color+=red;
            color<<=8;
            color+=green;
            color<<=8;
            color+=blue;
            color<<=8;
            color+=alpha;
            
            return color;
    }




uint32_t Canvas::getPoseX()
    {
        return poseX_;
    }
uint32_t Canvas::getPoseY()
    {
        return poseY_;
    }
uint32_t Canvas::getWidth()
    {
        return width_;
    }
uint32_t Canvas::getHeight()
    {
        return height_;
    }

const char* Canvas::getTitle()
    {
        char* ret_value = new char[30];
        strncpy(ret_value,title_,29);
        return ret_value;
    }

uint32_t* const Canvas::getBuffer()
    {
        return main_buffer_;
    }

void Canvas::Update()
    {
        const SDL_Rect* rect = nullptr;
        int pitch = width_ * sizeof(uint32_t);
        SDL_UpdateTexture(main_texture_, rect, main_buffer_, pitch);
        SDL_RenderClear(main_renderer_);
        const SDL_Rect* dstrect = nullptr;
        SDL_RenderCopy(main_renderer_, main_texture_, rect, dstrect);
        SDL_RenderPresent(main_renderer_);
    }

void Canvas::setPixel(const uint32_t pX, const uint32_t pY, const Color color )
    {
        // Down-Left Corner as Origin: x -> right, Y ^ top
        int Y = height_ - pY - 1;
        main_buffer_[Y * width_ + pX] = color.getRGBA();
    }

void Canvas::setBackgroundColor(Color color)
    {
        for(int i=0; i<width_; i++){
            for(int j=0; j<height_; j++){
                Canvas::setPixel(i, j, color);
            }
        }
    }

void Canvas::Blur(Box box, int k){
    
    uint32_t* blured_buffer = new uint32_t[width_ * height_]{0};

    int total_red = 0;
    int total_green = 0;
    int total_blue = 0;
    
    for (int i = box.width / 2; i < width_ - box.width / 2; i++){
        for (int j = box.height /2 ; j < height_ - box.height / 2; j++){
            total_red = k * Color::getRed(main_buffer_[j*width_ + i]);
            total_green = k * Color::getGreen(main_buffer_[j*width_ + i]);
            total_blue = k * Color::getBlue(main_buffer_[j*width_ + i]);
            
            for(int m = -box.width / 2; m < box.width / 2; m++){
                for(int n = -box.height / 2; n < box.height / 2; n++){
                    int I = i+m;
                    int J = j+n;
                    total_red += Color::getRed(main_buffer_[J*width_+I]);
                    total_green += Color::getGreen(main_buffer_[J*width_+I]);
                    total_blue += Color::getBlue(main_buffer_[J*width_+I]);
                }
            }
            
            blured_buffer[j*width_+i] = Color::getRGBA(total_red/(9+k), total_green/(9+k), total_blue/(9+k));
        }
    }

    delete[] main_buffer_;
    main_buffer_ = blured_buffer;
}


void Canvas::saveCurrentFrame(char* file_name){
        
        SDL_Surface *surface;

        const uint32_t rmask = 0xff000000;
        const uint32_t gmask = 0x00ff0000;
        const uint32_t bmask = 0x0000ff00;
        const uint32_t amask = 0x000000ff;

        surface = SDL_CreateRGBSurface(0, width_, height_, 32,
                                        rmask, gmask, bmask, amask);
        if (surface == NULL) {
            SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
            exit(1);
        }

        for(uint32_t x = 0; x<width_; x++){
            for(uint32_t y = 0; y<height_; y++){
                uint32_t * const target_pixel = reinterpret_cast<uint32_t *> (reinterpret_cast<uint8_t *>(surface->pixels)
                                                                            + y * surface->pitch
                                                                            + x * surface->format->BytesPerPixel);
                (*target_pixel) = main_buffer_[(height_ - y - 1) * width_ + x];
            }
        }
        SDL_SaveBMP(surface, file_name);
}




