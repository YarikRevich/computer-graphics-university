#include "processor.hpp"

std::vector<SDL_Color> Processor::getReducedBitColorMap(SDL_Surface* surface){
    std::vector<SDL_Color> result;

    SDL_Color color;

    for (int x = 0; x < surface->w; x++){
        for (int y = 0; y < surface->h; y++){
            color = getPixel(surface, x, y);

            if (!isColorPresent(result, color)){
                result.push_back(color);
            }
        }   
    }

    return result;
}

std::vector<SDL_Color> Processor::getCompleteBitColorMap(SDL_Surface* surface) {
    std::vector<SDL_Color> result;

    for (int x = 0; x < surface->w; x++){
        for (int y = 0; y < surface->h; y++){
            result.push_back(getPixel(surface, x, y));
        }
    }

    return result;
};

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

SDL_Color Processor::getNearestColor(std::vector<SDL_Color> colors, Uint8 compound) {
    SDL_Color result;

    int min = -1;
    int distance;

    for (SDL_Color color : colors) {
        distance = abs(compound - color.r);

        if (distance < min || min == -1) {
            min = distance;
            result = color;
        }
    }

    return result;
};

void Processor::sortColorMap(std::vector<SDL_Color>& colors, int begin, int end) {
    std::vector<SDL_Color> src;

// std::cout << src.size() << std::endl;

    // std::cout << colors.size() << std::endl;
    // std::cout << begin << std::endl;
    // std::cout << end << std::endl;
    for (int i = begin; i < end; i++) {
        src.push_back(colors.at(i));
    }

    // std::vector<SDL_Color> src = 
    //     std::vector<SDL_Color>(colors.begin() + begin, colors.end() - (colors.size() - end));

    // std::cout << unsigned(colors.begin() + begin) << " " << colors.end() - (colors.size() - end) << std::endl;
    // std::cout << src.size() << std::endl;

    qsort(
        &src[0],
        src.size(), 
        sizeof(SDL_Color), 
        [](const void* element1, const void* element2) -> int { 
            if (((SDL_Color*)element1)->r < ((SDL_Color*)element2)->r) {
                return -1;
            }
            return 1;
        });

    for (int i = 0; i < src.size(); i++) {
        // std::cout << unsigned(src.at(i).r) << std::endl;
        colors.insert(colors.begin() + begin + i, src.at(i));
    }

    // std::cout << std::endl;
};

void Processor::generateMedianCutBWSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration){
    if (iteration > 0){
        sortColorMap(image, begin, end);

        int mid = (begin + end + 1) / 2;

        generateMedianCutBWSelectionRaw(image, colors, begin, mid - 1, bucket, iteration - 1);
        generateMedianCutBWSelectionRaw(image, colors, mid, end, bucket, iteration - 1);
    } else {
        int sum = 0;
 
        std::for_each(image.begin() + begin, image.end() - (image.size() - end), [&] (SDL_Color color) {
            sum += 0.299*color.r+0.587*color.g+0.114*color.b;
        });

        Uint8 compound = sum / (begin + 1 - end);

        std::cout << unsigned(compound) << std::endl;
        colors.at(*bucket) = {compound, compound, compound};

        // std::cout<<"Kubelek "<<*bucket<<": (s:"<<begin<<", k:"<<end<<", e:"<<(end+1 - begin)<<")"<<std::endl;
        // cout<<"Kolor "<<*bucket<<": ("<<(int)nowyKolor.r<<","<<(int)nowyKolor.g<<","<<(int)nowyKolor.b<<")"<<endl;
        (*bucket)++;
    }
};

std::vector<SDL_Color> Processor::generateMedianCutBWSelection(std::vector<SDL_Color>& image, int pixels) {
    std::vector<SDL_Color> result(BIT_NUM_MAX);
    
    generateMedianCutBWSelectionRaw(image, result, 0, pixels, new int(0), MEDIAN_CUT_BATCH);

    return result;
};

std::vector<SDL_Color> Processor::generateColorBuckets(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<SDL_Color> result;

    std::vector<SDL_Color> colors = generateMedianCutBWSelection(image, getPixelAmount(surface));

    SDL_Color color;

    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            color = getPixel(surface, x, y);

            SDL_Color nearest = getNearestColor(colors, 0.299*color.r+0.587*color.g+0.114*color.b);
            // std::cout << "element: " << "r: " << (int)nearest.r << " g: " << (int)nearest.g << " b: " << (int)nearest.b << std::endl;
            setPixel(surface, x, y, nearest);
            // result.push_back(getNearestColor(colors, 0.299*color.r+0.587*color.g+0.114*color.b));
        }
    }

    // std::cout << colors.size() << std::endl;

    // kolor=getPixel(x, y);
    //         szary=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
    //         indeks=znajdzSasiadaBW(szary);
    //         setPixel(x+szerokosc/2,y+wysokosc/2, paleta[indeks].r, paleta[indeks].g, paleta[indeks].b);
    // for (SDL_Color color : colors) {
    //     result.push_back(getNearestColor(colors, color.r));
    // }

    return result;
};

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

SDL_Color Processor::getPixel(SDL_Surface* surface, int x, int y) {
    SDL_Color result;
    Uint32 col = 0;
    
    if ((x >= 0) && (x < surface->w) && (y >= 0) && (y < surface->h)) {
        char* pPosition = (char*)surface->pixels;

        pPosition += (surface->pitch * y);
        pPosition += (surface->format->BytesPerPixel * x);

        memcpy(&col, pPosition, surface->format->BytesPerPixel);
        SDL_GetRGB(col, surface->format, &result.r, &result.g, &result.b);
    }

    return result;
}

int Processor::getPixelAmount(SDL_Surface* surface) {
    return surface->w * surface->h;
}

void Processor::setPixel(SDL_Surface* surface, int x, int y, SDL_Color color) {
    if ((x >= 0) && (x < surface->w) && (y >= 0) && (y < surface->h)){
        Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

        int bpp = surface->format->BytesPerPixel;

        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp)
        {
            case 1:
                *p = pixel;
                break;

            case 2:
                *(Uint16 *)p = pixel;
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;

            case 4:
                *(Uint32 *)p = pixel;
                break;
        }
    }
}

void Processor::setColors(SDL_Surface* surface, std::vector<SDL_Color> colors) {
    int x = 0;
    int y = 0;

    for (SDL_Color color : colors) {
        setPixel(surface, x, y, color);

        if (x < surface->w) {
            x++;
        } else{
            x = 0;
            y++;
        }
    }
}