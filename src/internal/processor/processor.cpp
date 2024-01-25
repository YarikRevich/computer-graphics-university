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

SDL_Color Processor::getNearestColorBW(std::vector<SDL_Color> colors, Uint8 compound) {
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

SDL_Color Processor::getNearestColorRGB(std::vector<SDL_Color> colors, SDL_Color src) {
    SDL_Color result;

    int min = -1;
    int distance;

    for (SDL_Color color : colors) {
        distance = abs(src.r - color.r) + abs(src.g - color.g) + abs(src.b - color.b);

        if (distance < min || min == -1) {
            min = distance;
            result = color;
        }
    }

    return result;



    // int minimum=999;
    // int indexMinimum;
    // SDL_Color kolorPaleta;
    // int odleglosc;
    // for(int i=0; i<128; i++){
    //     kolorPaleta=paleta[i];
    //     odleglosc=abs(kolor.r-kolorPaleta.r)+abs(kolor.g-kolorPaleta.g)+abs(kolor.b-kolorPaleta.b);
    //     if(odleglosc < minimum){
    //         indexMinimum=i;
    //         minimum=odleglosc;
    //     }
    // }
    // return indexMinimum;
};

void Processor::sortColorMapBW(std::vector<SDL_Color>& colors, int begin, int end) {
    std::vector<SDL_Color> src;

    for (int i = begin; i < end; i++) {
        src.push_back(colors[i]);
    }

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
        colors.at(begin + i) = src[i];
    }
};

void Processor::sortColorMapRGB(std::vector<SDL_Color>& colors, int begin, int end, COLOR_COMPOUNDS compound) {
    std::vector<SDL_Color> src;

    for (int i = begin; i < end; i++) {
        src.push_back(colors[i]);
    }

    switch (compound) {
        case Processor::COLOR_COMPOUNDS::RED:
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
            break;
        case Processor::COLOR_COMPOUNDS::GREEN:
            qsort(
                &src[0],
                src.size(), 
                sizeof(SDL_Color), 
                [](const void* element1, const void* element2) -> int { 
                    if (((SDL_Color*)element1)->g < ((SDL_Color*)element2)->g) {
                        return -1;
                    }
                    return 1;
            });
            break;
        case Processor::COLOR_COMPOUNDS::BLUE:
            qsort(
                &src[0],
                src.size(), 
                sizeof(SDL_Color), 
                [](const void* element1, const void* element2) -> int { 
                    if (((SDL_Color*)element1)->b < ((SDL_Color*)element2)->b) {
                        return -1;
                    }
                    return 1;
            });
    }

    for (int i = 0; i < src.size(); i++) {
        colors.at(begin + i) = src[i];
    }
};

void Processor::generateMedianCutRGBSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration){
    if (iteration > 0){
        Processor::COLOR_COMPOUNDS compound = getCompoundDifference(image, begin, end);
        sortColorMapRGB(image, begin, end, compound);

        int mid = (begin + end + 1) / 2;

        generateMedianCutRGBSelectionRaw(image, colors, begin, mid - 1, bucket, iteration - 1);
        generateMedianCutRGBSelectionRaw(image, colors, mid + 1, end, bucket, iteration - 1);
    } else {
        int sumColorR = 0, sumColorG = 0, sumColorB = 0;
 
        std::for_each(image.begin() + begin, image.end() - (image.size() - end), [&] (SDL_Color color) {
            sumColorR += color.r;
            sumColorG += color.g;
            sumColorB += color.b;
        });

        colors[*bucket] = {
            (Uint8)(sumColorR / (end + 1 - begin)), 
            (Uint8)(sumColorG / (end + 1 - begin)),
            (Uint8)(sumColorB / (end + 1 - begin))
        };

        (*bucket)++;
    }
}

std::vector<SDL_Color> Processor::generateMedianCutRGBSelection(std::vector<SDL_Color>& image, int pixels) {
    std::vector<SDL_Color> result(BIT_NUM_MAX);
    
    generateMedianCutRGBSelectionRaw(image, result, 0, pixels, new int(0), MEDIAN_CUT_BATCH);

    return result;
}

Processor::COLOR_COMPOUNDS Processor::getCompoundDifference(std::vector<SDL_Color>& image, int begin, int end) {
    Uint8 minColorR = 0, minColorG = 0, minColorB = 0;
    Uint8 maxColorR = 0, maxColorG = 0, maxColorB = 0;

    std::for_each(image.begin() + begin, image.end() - (image.size() - end), [&] (SDL_Color color) {
        if(color.b < minColorB)
            minColorB = color.b;
        if(color.g < minColorG)
            minColorG = color.g;
        if(color.r < minColorR)
            minColorR = color.r;
        if(color.b > maxColorB)
            maxColorB = color.b;
        if(color.g > maxColorG)
            maxColorG = color.g;
        if(color.r > maxColorR)
            maxColorR = color.r;
    });

    switch (std::max(std::max(maxColorR - minColorR, maxColorG - minColorG), maxColorB - minColorB)) {
        case 1:
            return Processor::COLOR_COMPOUNDS::RED;
        case 2:
            return Processor::COLOR_COMPOUNDS::GREEN;
        default:
            return Processor::COLOR_COMPOUNDS::BLUE;
    };
}

void Processor::generateMedianCutBWSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration){
    if (iteration > 0){
        sortColorMapBW(image, begin, end);

        int mid = (begin + end + 1) / 2;

        generateMedianCutBWSelectionRaw(image, colors, begin, mid - 1, bucket, iteration - 1);
        generateMedianCutBWSelectionRaw(image, colors, mid, end, bucket, iteration - 1);
    } else {
        int sum = 0;
 
        std::for_each(image.begin() + begin, image.end() - (image.size() - end), [&] (SDL_Color color) {
            sum += 0.299*color.r+0.587*color.g+0.114*color.b;
        });

        Uint8 compound = sum / (begin + 1 - end);
        colors[*bucket] = {compound, compound, compound};

        (*bucket)++;
    }
};

std::vector<SDL_Color> Processor::generateMedianCutBWSelection(std::vector<SDL_Color>& image, int pixels) {
    std::vector<SDL_Color> result(BIT_NUM_MAX);
    
    generateMedianCutBWSelectionRaw(image, result, 0, pixels, new int(0), MEDIAN_CUT_BATCH);

    return result;
};

std::vector<Processor::PixelPoint> Processor::generateColorBucketsBW(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<Processor::PixelPoint> result;

    std::vector<SDL_Color> colors = generateMedianCutBWSelection(image, getPixelAmount(surface));

    SDL_Color color;
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            color = getPixel(surface, x, y);

            result.push_back(PixelPoint(x, y, getNearestColorBW(colors, 0.299*color.r+0.587*color.g+0.114*color.b)));
        }
    }

    return result;
};

std::vector<Processor::PixelPoint> Processor::generateColorBucketsRGB(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<Processor::PixelPoint> result;

    std::vector<SDL_Color> colors = generateMedianCutRGBSelection(image, getPixelAmount(surface));

    SDL_Color color;
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {
            color = getPixel(surface, x, y);

            result.push_back(PixelPoint(x, y, getNearestColorRGB(colors, color)));
        }
    }

    return result;
};

std::vector<Processor::PixelPoint> Processor::generateDedicatedPalette(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<Processor::PixelPoint> result;

    int x, y;

    for (int k = 0; k < BIT_NUM_MAX; k++) {
        y = k / 8;
        x = k - y / 8;
        for (int xx = 0; xx < 40; xx++)  {
            for (int yy = 0; yy < 50; yy++) {
                result.push_back(PixelPoint(x * 40 + xx, y * 50 + yy, image[k]));
            }
        }
    }

// void narysujPalete(SDL_Color paleta5[]){
//     int x, y;
//     for(int k=0; k<32; k++) {
//         y = k / 8;
//         x = k - y * 8;
//         for (int xx=0; xx < 40; xx++) {
//             for(int yy=0; yy < 50; yy++) {
//                 setPixel(x * 40 + xx, y * 50 + yy, paleta5[k].r, paleta5[k].g, paleta5[k].b);
//             }
//         }
//     }

// }

    return result;
}

Uint8 Processor::convert24BitRGBTo7BitRGB(SDL_Color color) {
    int newColorR = round(color.r*3.0/255.0);
    int newColorG = round(color.g*7.0/255.0);
    int newColorB = round(color.b*3.0/255.0);

    return (newColorR << 5) | (newColorG << 2) | newColorB;
}

SDL_Color Processor::convert7BitRGBTo24BitRGB(Uint8 color) {
    int newColorR = (color & (0b01100000)) >> 5;
    int newColorG = (color & (0b00011100)) >> 2;
    int newColorB = (color & (0b00000011));

    return {
        .r = static_cast<Uint8>(newColorR * 255.0 / 3.0), 
        .g = static_cast<Uint8>(newColorG * 255.0 / 7.0),
        .b = static_cast<Uint8>(newColorB * 255.0 / 3.0)
    };
}

Uint8 Processor::convert24BitRGBTo7BitGrey(SDL_Color color) {
    return round((0.299 * color.r + 0.587 * color.g + 0.114 * color.b) * 127.0 / 255.0);
}

SDL_Color Processor::convert7BitGreyTo24BitRGB(Uint8 grey) {
    return {
        .r = static_cast<Uint8>(grey * 255.0 / 127.0),
        .g = static_cast<Uint8>(grey * 255.0 / 127.0),
        .b = static_cast<Uint8>(grey * 255.0 / 127.0)
    };
}

Uint8 Processor::convertRGBToGreyUint8(SDL_Color color) {
    return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

SDL_Color Processor::convertRGBToGrey(SDL_Color color) {
    Uint8 grey = convertRGBToGreyUint8(color);

    return {
        .r = grey,
        .g = grey,
        .b = grey
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

void Processor::setPixels(SDL_Surface* surface, std::vector<Processor::PixelPoint> pixels) {
    for (Processor::PixelPoint pixel : pixels) {
        setPixel(surface, pixel.x, pixel.y, pixel.color);
    }
}