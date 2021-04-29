#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <cstdint>
#include <SDL.h>

using namespace std;

struct Point{
    uint32_t x;
    uint32_t y;
};


struct Box{
    Point corner;
    uint32_t width;
    uint32_t height;
};


class Color{
private:
    uint8_t red_ = 0x00;
    uint8_t green_ = 0x00;
    uint8_t blue_ = 0x00;
    uint8_t alpha_ = 0xFF;
    uint32_t color_ = 0x00000000;
    
protected:
    
public:
    
    Color(uint32_t RGBDA = 0x000000FF): color_(RGBDA)
        {
            
            red_ = (color_ & 0xFF000000) >> 24;
            green_ = (color_ & 0x00FF0000) >> 16;
            blue_ = (color_ & 0x0000FF00) >> 8 ;
            alpha_ = (color_ & 0x000000FF) >> 0;
            
        }
    
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF)
        : red_(red), green_(green), blue_(blue), alpha_(alpha)
        {
            color_+=red;
            color_<<=8;
            color_+=green;
            color_<<=8;
            color_+=blue;
            color_<<=8;
            color_+=alpha;
        }
    
    Color(Color& color)
        :red_(color.getRed()), green_(color.getGreen()), blue_(color.getBlue()), alpha_(color.getAlpha()), color_(color.getRGBA())
        {}

    Color(Color&& color)
        :red_(color.getRed()), green_(color.getGreen()), blue_(color.getBlue()), alpha_(color.getAlpha()), color_(color.getRGBA())
        {}

    Color& operator =(const Color& rhs)
        {
            if (&rhs == this) return *this;
            this->setRed(rhs.getRed());
            this->setGreen(rhs.getGreen());
            this->setBlue(rhs.getBlue());
            this->setAlpha(rhs.getAlpha());
            return *this;
        }

    Color& operator =(const Color&& rhs)
        {
            if (&rhs == this) return *this;
            this->setRed(rhs.getRed());
            this->setGreen(rhs.getGreen());
            this->setBlue(rhs.getBlue());
            this->setAlpha(rhs.getAlpha());
            return *this;
        }

    ~Color() = default;
    
    void setRed(const uint8_t red);
    void setGreen(const uint8_t green);
    void setBlue(const uint8_t blue);
    void setAlpha(const uint8_t alpha);
    void setRGBA(const uint32_t RGBA);
        
    uint8_t getRed() const;
    uint8_t getGreen() const;
    uint8_t getBlue() const;
    uint8_t getAlpha() const;
    uint32_t getRGBA() const;
    
    static uint8_t getRed(const uint32_t RGBA);
    static uint8_t getGreen(const uint32_t RGBA);
    static uint8_t getBlue(const uint32_t RGBA);
    static uint8_t getAlpha(const uint32_t RGBA);
    static uint32_t getRGBA(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha = 0xFF);
    
};


class Canvas{
    
private:
    const uint32_t poseX_ = 0;
    const uint32_t poseY_ = 0;
    const uint32_t width_ = 0;
    const uint32_t height_ = 0;
    char title_[30]{0};

    SDL_Window* main_window_ = nullptr;
    SDL_Renderer* main_renderer_ = nullptr;
    uint32_t* main_buffer_ = nullptr;
    const uint32_t rendered_driver_index = -1;
    
    SDL_Texture* main_texture_ = nullptr;
    uint32_t format = SDL_PIXELFORMAT_RGBA8888;
    uint32_t access = SDL_TEXTUREACCESS_STATIC;
    
protected:
    Canvas() = delete;
    Canvas(Canvas&) = delete;
    Canvas(Canvas&&) = delete;
    Canvas& operator = (const Canvas&) = delete;

public:

    Canvas(const char* title, const uint32_t width, const uint32_t height, const uint32_t poseX = SDL_WINDOWPOS_UNDEFINED, const uint32_t poseY = SDL_WINDOWPOS_UNDEFINED)
        : width_(width), height_(height), poseX_(poseX), poseY_(poseY)
        {
            main_buffer_ = new uint32_t[width_ * height_];
            memset(main_buffer_, 0x00 , width_ * height_ * sizeof(uint32_t) );
            
            strncpy(title_, title, 29);
            
            if(SDL_Init(SDL_INIT_VIDEO) != 0){
                throw "Error: SDL is NOT Initialized!";
            }
        
            main_window_ = SDL_CreateWindow(title_, poseX_, poseY_, width_, height_, SDL_WINDOW_SHOWN);
            if(main_window_ == nullptr){
                const char* err_msg = SDL_GetError();
                SDL_Quit();
                throw err_msg;
            }
        
            main_renderer_ = SDL_CreateRenderer(main_window_, rendered_driver_index, SDL_RENDERER_PRESENTVSYNC);
            if(main_renderer_ == nullptr){
                SDL_DestroyWindow(main_window_);
                const char* err_msg = SDL_GetError();
                SDL_Quit();
                throw err_msg;
            }
        
            main_texture_ = SDL_CreateTexture(main_renderer_, format, access, width_, height_);
            if(main_texture_ == nullptr){
                SDL_DestroyRenderer(main_renderer_);
                SDL_DestroyWindow(main_window_);
                const char* err_msg = SDL_GetError();
                SDL_Quit();
                throw err_msg;
            }

    }
    
    ~Canvas(){
        delete[] main_buffer_;
        SDL_ClearError();
        SDL_DestroyTexture(main_texture_);
        SDL_DestroyRenderer(main_renderer_);
        SDL_DestroyWindow(main_window_);
        SDL_Quit();
    }
    
    uint32_t getPoseX();
    uint32_t getPoseY();
    uint32_t getWidth();
    uint32_t getHeight();
    const char * getTitle();
    
    uint32_t* const getBuffer();
    
    void Update();
    void setPixel(const uint32_t pX, const uint32_t pY, const Color color );
    void setBackgroundColor(Color color);

    void Blur(Box box, int k = 0);

    void saveCurrentFrame(char* file_name);
    
};


#endif