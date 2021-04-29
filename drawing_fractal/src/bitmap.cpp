
#include "bitmap.hpp"


void Bitmap::setPixel(size_t x, size_t y, uint8_t red, uint8_t green, uint8_t blue){
    
    if (x<width_ && y<height_){
        uint8_t* cur_pixel = bitmap_pixels_ + (x + width_ * y)*3;
        cur_pixel[0] = blue;
        cur_pixel[1] = green;
        cur_pixel[2] = red;
    }else{
        throw "Out of range!";
    }

}

bool Bitmap::save(const char* file_name){
    
    bool saved = false;
    ofstream file;
    file.open(file_name, ios::binary);
    
    if(!file){
        return false;
    }
    
    file.write((char*)(&file_header_), sizeof(file_header_));
    file.write((char*)(&info_header_), sizeof(info_header_));
    file.write((char*)(bitmap_pixels_), width_*height_*3);
    
    saved = true;
    
    file.close();
    if (saved && !file.is_open()){
        return true;
    }else{
        return false;
    }
    
}