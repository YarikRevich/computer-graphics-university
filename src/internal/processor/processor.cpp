#include "processor.hpp"
#include <iostream>

std::vector<SDL_Color> Processor::getBitColorMap(SDL_Surface* surface){
    std::vector<SDL_Color> result;

    SDL_Color color;

    for (int x = 0; x < surface->w; x++){
        for (int y = 0; y < surface->h; y++){
            if (result.size() >= BIT_NUM_MAX) {
                return result;
            }

            color = getPixel(surface, x, y);

            if (!isColorPresent(result, color)){
                result.push_back(color);
            }
        }   
    }

    return result;
}

bool Processor::isColorEqual(SDL_Color color1, SDL_Color color2) {
    return color1.r == color2.r || color1.g == color2.g || color1.b == color2.b;
};

bool Processor::isColorPresent(std::vector<SDL_Color> colors, const SDL_Color& color){
    if(auto iter = std::find_if(
        colors.begin(), 
        colors.end(), 
        [&](const SDL_Color& element) { return Processor::isColorEqual(element, color);}); iter != std::end(colors)) {
        return true;
    } else {    
        return false;
    }
};

void Processor::generateColorBuckets(std::vector<SDL_Color>& colors) {
    // MEDIAN_CUT_BATCH  


};

void Processor::sortColorMap(std::vector<SDL_Color>& colors) {
    qsort(
        &colors[0],
        colors.size(), 
        sizeof(SDL_Color), 
        [](const void* element1, const void* element2) -> int { 
            if (((SDL_Color*)element1)->r < ((SDL_Color*)element2)->r) {
                return -1;
            }
            return 1;
        });
};

SDL_Color Processor::getPixel(SDL_Surface* surface, int x, int y) {
    SDL_Color result;
    Uint32 col = 0;
    
    if ((x >= 0) && (x < surface->w) && (y >= 0) && (y < surface->h)) {
        char* pPosition = (char*)surface->pixels;

        pPosition += (surface->pitch * y * 2);
        pPosition += (surface->format->BytesPerPixel * x * 2);

        memcpy(&col, pPosition, surface->format->BytesPerPixel);
        SDL_GetRGB(col, surface->format, &result.r, &result.g, &result.b);
    }

    return result;
}

void Processor::setPixel(SDL_Surface* surface, int x, int y, SDL_Color color) {
    if ((x >= 0) && (x < surface->w) && (y >= 0) && (y < surface->h)){
        Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

        int bpp = surface->format->BytesPerPixel;

        Uint8 *p1 = (Uint8 *)surface->pixels + (y * 2) * surface->pitch + (x*2) * bpp;
        Uint8 *p2 = (Uint8 *)surface->pixels + (y * 2 + 1) * surface->pitch + (x*2) * bpp;
        Uint8 *p3 = (Uint8 *)surface->pixels + (y*2) * surface->pitch + (x*2+1) * bpp;
        Uint8 *p4 = (Uint8 *)surface->pixels + (y*2+1) * surface->pitch + (x*2+1) * bpp;

        *p1 = pixel;
        *p2 = pixel;
        *p3 = pixel;
        *p4 = pixel;
  }
}

int Processor::convertToCGU(SDL_Surface* surface) {
    std::vector<SDL_Color> colors = getBitColorMap(surface);

    sortColorMap(colors);
    generateColorBuckets(colors);

    for (auto element : colors) {
        std::cout << "element: " << "r: " << (int)element.r << " g: " << (int)element.g << " b: " << (int)element.b << std::endl;
    };

    return EXIT_SUCCESS;
}

int Processor::convertFromCGU(SDL_Surface* surface) {
    //
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