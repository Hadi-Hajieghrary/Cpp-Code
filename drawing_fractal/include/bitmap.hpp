#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdint>


#pragma pack(push)
#pragma pack(2)

struct BitmapFileHeader{
    const char header[2]{'B','M'};
    int32_t file_size = 0;
    const int32_t reserve{0};
    int32_t offset = 0;
};

struct BitmapInfoHeader{
    
    const int32_t header_size{40};
    
    int32_t width{0};
    int32_t height{0};
    
    int16_t planes{1};
    int16_t bits_per_pixel{24};
    int32_t compression{0};
    
    int32_t data_size{0};
    int32_t horizental_resolution{2400};
    int32_t vertical_resolution{2400};
    int32_t colors{0};
    int32_t important_colors{0};

    
};

#pragma pack(pop)



using namespace std;

class Bitmap{
    
private:
    
    const int32_t width_;
    const int32_t height_;
    
    uint8_t* bitmap_pixels_ = nullptr;
    
    BitmapFileHeader file_header_;
    BitmapInfoHeader info_header_;

protected:
    
    
public:
    
    Bitmap() = delete;
    Bitmap(int32_t width, int32_t height)
        : width_(width), height_(height), bitmap_pixels_(new uint8_t[width_*height_*3]{0}) 
        {
            file_header_.file_size = sizeof(file_header_) + sizeof(info_header_) + width_*height_*3;
            file_header_.offset = sizeof(file_header_) + sizeof(info_header_);
            
            info_header_.width = width_;
            info_header_.height =  height_;
        
        }

    virtual ~Bitmap(){
        if (bitmap_pixels_ != nullptr){
            delete[] bitmap_pixels_;
        }
    }
    
    bool save(const char* file_name);
    void setPixel(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue);
};

#endif /* BITMAP_HPP */