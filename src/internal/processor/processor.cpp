#include "processor.hpp"

int Processor::convertToCGU(SDL_Surface* surface) {
    return EXIT_SUCCESS;
}

int Processor::convertFromCGU(SDL_Surface* surface) {
    return EXIT_SUCCESS;
}

Uint8 Processor::convert24BitRGBTo7BitRGB(SDL_Color color) {
    int newR=round(color.r*3.0/255.0);
    int newG=round(color.g*7.0/255.0);
    int newB=round(color.b*3.0/255.0);

    return (newR<<5)|(newG<<2)|newB;
}

SDL_Color Processor::convert7BitRGBTo24BitRGB(Uint8 color) {
    int newR=(color&(0b01100000))>>5;
    int newG=(color&(0b00011100))>>2;
    int newB=(color&(0b00000011));

    
    return {
        .r = static_cast<Uint8>(newR*255.0/3.0), 
        .g = static_cast<Uint8>(newG*255.0/7.0),
        .b = static_cast<Uint8>(newB*255.0/3.0)
    };
}

Uint8 Processor::convert24BitRGBTo7BitGrey(SDL_Color color) {
    return round((0.299*color.r+0.587*color.g+0.114*color.b)*127.0/255.0);
}

SDL_Color Processor::convert7BitGreyTo24BitRGB(Uint8 grey) {
    return {
        .r = static_cast<Uint8>(grey*255.0/127.0),
        .g = static_cast<Uint8>(grey*255.0/127.0),
        .b = static_cast<Uint8>(grey*255.0/127.0)
    };
}

int Processor::normalizeValue(int value, int min, int max) {
    if (value < min) {
        return min;
    } 

    if (value > max) {
        return max;
    }

    return value;
}