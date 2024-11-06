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
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
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
};

int Processor::getColorIndex(std::vector<SDL_Color> colors, SDL_Color src) {
    for (int i = 0; i < colors.size(); i++) {
        if (isColorEqual(colors[i], src)) {
            return i;
        }
    }

    return -1;
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

std::vector<SDL_Color> Processor::BucketResult::getColors() {
    return colors;
}

std::vector<int> Processor::BucketResult::getIndeces() {
    return indeces;
}

Processor::BucketResult* Processor::generateColorBucketsBW(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<int> result;

    std::vector<SDL_Color> colors = generateMedianCutBWSelection(image, getPixelAmount(surface));

    SDL_Color color, nearestColor;
    for (int x = 0; x < surface->w; x++) {
        for (int y = 0; y < surface->h; y++) {
            color = getPixel(surface, x, y);

            nearestColor = getNearestColorBW(colors, 0.299*color.r+0.587*color.g+0.114*color.b);

            result.push_back(getColorIndex(colors, nearestColor));
        }
    }

    return new BucketResult(colors, result);
};

Processor::BucketResult* Processor::generateColorBucketsRGB(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<int> result;

    std::vector<SDL_Color> colors = generateMedianCutRGBSelection(image, getPixelAmount(surface));

    SDL_Color color, nearestColor;
    for (int x = 0; x < surface->w; x++) {
        for (int y = 0; y < surface->h; y++) {
            color = getPixel(surface, x, y);

            nearestColor = getNearestColorRGB(colors, color);

            result.push_back(getColorIndex(colors, nearestColor));
        }
    }

    return new BucketResult(colors, result);
};

std::vector<Processor::PixelPoint> Processor::generateDedicatedPalette(SDL_Surface* surface, std::vector<SDL_Color>& image) {
    std::vector<Processor::PixelPoint> result;

    int y = 0;
    int x = 0;

    int ySize = sqrt(surface->h);
    int xSize = sqrt(surface->w);

    for (int k = 0; k < image.size(); k++) {
        if ((x + xSize) > surface->w) {
            x = 0;
            y += ySize;
        }   

        for (int xx = 0; xx < xSize; xx++)  {
            for (int yy = 0; yy < ySize; yy++) {
                result.push_back(PixelPoint(x + xx, y + yy, image[k]));
            }
        }

        if (x >= surface->w) { 
            x = 0;
            y += ySize;
        } else {
            x += xSize;
        }
    }

    return result;
}

std::vector<Processor::PixelPoint> Processor::generateFloydSteinbergDitheringRGB(SDL_Surface* surface) {
    std::vector<Processor::PixelPoint> result;

    SDL_Color color, newColor, tempColor;

    std::vector<std::vector<float>> colorShiftsR((surface->w) + 2, std::vector<float>((surface->h) + 2));
    std::vector<std::vector<float>> colorShiftsG((surface->w) + 2, std::vector<float>((surface->h) + 2));
    std::vector<std::vector<float>> colorShiftsB((surface->w) + 2, std::vector<float>((surface->h) + 2));

    int colorShiftR = 0;
    int colorShiftG = 0;
    int colorShiftB = 0;

    for (int y = 0; y < surface->h; y++){
        for(int x = 0; x < surface->w; x++){
            color = Processor::getPixel(surface, x, y);

            tempColor.r = Processor::normalizeValue(
                color.r + colorShiftsR[x+NATIVE_SHIFT][y], 0, 255);

            tempColor.g = Processor::normalizeValue(
                color.g + colorShiftsG[x+NATIVE_SHIFT][y], 0, 255);

            tempColor.b = Processor::normalizeValue(
                color.b + colorShiftsB[x+NATIVE_SHIFT][y], 0, 255);

            newColor = Processor::convert7BitRGBTo24BitRGB(Processor::convert24BitRGBTo7BitRGB(tempColor));

            colorShiftR = tempColor.r - newColor.r;
            colorShiftG = tempColor.g - newColor.g;
            colorShiftB = tempColor.b - newColor.b;

            result.push_back(PixelPoint(x, y, newColor));

            colorShiftsR[x + 1 + NATIVE_SHIFT][y] += (colorShiftR * 7.0 / 16.0);
            colorShiftsR[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftR * 3.0 / 16.0);
            colorShiftsR[x + NATIVE_SHIFT][y + 1] += (colorShiftR * 5.0 / 16.0);
            colorShiftsR[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftR * 1.0 / 16.0);

            colorShiftsG[x + 1 + NATIVE_SHIFT][y] += (colorShiftG * 7.0 / 16.0);
            colorShiftsG[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftG * 3.0 / 16.0);
            colorShiftsG[x + NATIVE_SHIFT][y + 1] += (colorShiftG * 5.0 / 16.0);
            colorShiftsG[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftG * 1.0 / 16.0);

            colorShiftsB[x + 1 + NATIVE_SHIFT][y] += (colorShiftB * 7.0 / 16.0);
            colorShiftsB[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftB * 3.0 / 16.0);
            colorShiftsB[x + NATIVE_SHIFT][y + 1] += (colorShiftB * 5.0 / 16.0);
            colorShiftsB[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftB * 1.0 / 16.0);
        }
    }

    return result;
};

std::vector<Processor::PixelPoint> Processor::generateFloydSteinbergDitheringBW(SDL_Surface* surface) {
    std::vector<Processor::PixelPoint> result;

    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);

    SDL_Color color, newColor, tempColor;
    Uint8 grey, newGrey, tempGrey;

    std::vector<std::vector<float>> colorShifts((surface->w)+2, std::vector<float>((surface->h)+2));

    int colorShift = 0;

    for (int y = 0; y < surface->h; y++){
        for(int x = 0; x < surface->w; x++){
            color = Processor::getPixel(surface, x, y);

            grey = Processor::convertRGBToGreyUint8(color);

            tempGrey = Processor::normalizeValue(
                grey + colorShifts[x+NATIVE_SHIFT][y], 0, 255);

            tempColor.r = tempGrey;
            tempColor.g = tempGrey;
            tempColor.b = tempGrey;

            newColor = Processor::convert7BitGreyTo24BitRGB(Processor::convert24BitRGBTo7BitGrey(tempColor));

            newGrey = newColor.r;

            colorShift = tempGrey - newGrey;

            result.push_back(PixelPoint(x, y, newColor));

            colorShifts[x+1+NATIVE_SHIFT][y] += (colorShift * 7.0 / 16.0);
            colorShifts[x-1+NATIVE_SHIFT][y+1] += (colorShift * 3.0 / 16.0);
            colorShifts[x+NATIVE_SHIFT][y+1] += (colorShift * 5.0 / 16.0);
            colorShifts[x+1+NATIVE_SHIFT][y+1] += (colorShift * 1.0 / 16.0);
        }
    }

    return result;
};

Uint8 Processor::convert24BitRGBTo7BitRGB(SDL_Color color) {
    int newColorR = round(color.r * 3.0 / 255.0);
    int newColorG = round(color.g * 7.0 / 255.0);
    int newColorB = round(color.b * 3.0 / 255.0);

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
    return round(Processor::convertRGBToGreyUint8(color) * 127.0 / 255.0);
}

SDL_Color Processor::convert7BitGreyTo24BitRGB(Uint8 grey) {
    return {
        .r = static_cast<Uint8>(grey * 255.0 / 127.0),
        .g = static_cast<Uint8>(grey * 255.0 / 127.0),
        .b = static_cast<Uint8>(grey * 255.0 / 127.0)
    };
}

Uint16 Processor::convert24BitRGBTo16BitRGB(SDL_Color color) {
    Uint8 r = color.r >> 3;
    Uint8 g = color.g >> 2;
    Uint8 b = color.b >> 3;

    return (Uint16)((r << 11) | (g << 5) | b);
}

SDL_Color Processor::convert16BitRGBTo24BitRGB(Uint16 color) {
    return {
        .r = static_cast<Uint8>((color >> 11) << 3), 
        .g = static_cast<Uint8>(((color << 5) >> 10) << 2),
        .b = static_cast<Uint8>(color << 3)
    };
}

Uint16 Processor::convert24BitRGBTo15BitRGB(SDL_Color color) {
    Uint8 r = color.r >> 3;
    Uint8 g = color.g >> 3;
    Uint8 b = color.b >> 3;

    return (Uint16)((r << 10) | (g << 5) | b);
}

SDL_Color Processor::convert15BitRGBTo24BitRGB(Uint16 color) {
    return {
        .r = static_cast<Uint8>(((color << 1) >> 11) << 3), 
        .g = static_cast<Uint8>(((color << 6) >> 11) << 3),
        .b = static_cast<Uint8>(color << 3)
    };
}

std::vector<Uint8> Processor::convert8BitTo7Bit(std::vector<Uint8> input) {
    std::vector<Uint8> output(PREFERRED_BIT_NUM_PER_PIXEL, 0);

    Uint8 tmp;

    input[0] = input[0] << 1;
    tmp = input[1];
    input[1] = input[1] >> 6;
    output[0] = input[0] | input[1];

    tmp = tmp << 2;
    output[1] = tmp;
    tmp = input[2];
    input[2] = input[2] >> 5;
    output[1] = output[1] | input[2];

    tmp = tmp << 3;
    output[2] = tmp;
    tmp = input[3];
    input[3] = input[3] >> 4;
    output[2] = output[2] | input[3];

    tmp = tmp << 4;
    output[3] = tmp;
    tmp = input[4];
    input[4] = input[4] >> 3;
    output[3] = output[3] | input[4];

    tmp = tmp << 5;
    output[4] = tmp;
    tmp = input[5];
    input[5] = input[5] >> 2;
    output[4] = output[4] | input[5];

    tmp = tmp << 6;
    output[5] = tmp;
    tmp = input[6];
    input[6] = input[6] >> 1;
    output[5] = output[5] | input[6];

    tmp = tmp << 7;
    output[6] = tmp;
    tmp = input[7];
    input[7] = input[7] >> 0;
    output[6] = output[6] | input[7];

    return output;
};

std::vector<Uint8> Processor::convert7BitTo8Bit(std::vector<Uint8> input) {
    std::vector<Uint8> output(ORIGINAL_BIT_NUM_PER_PIXEL, 0);

    Uint8 tmp;

    tmp = input[0];
    tmp = tmp >> 1;
    output[0] = tmp;
    tmp = input[0];
    tmp = tmp << 7;
    tmp = tmp >> 1;
    output[1] = output[1] | tmp;

    tmp = input[1];
    tmp = tmp >> 2;
    output[1] = output[1] | tmp;
    tmp = input[1];
    tmp = tmp << 6;
    tmp = tmp >> 1;
    output[2] = output[2] | tmp;

    tmp = input[2];
    tmp = tmp >> 3;
    output[2] = output[2] | tmp;
    tmp = input[2];
    tmp = tmp << 5;
    tmp = tmp >> 1;
    output[3] = output[3] | tmp;

    tmp = input[3];
    tmp = tmp >> 4;
    output[3] = output[3] | tmp;
    tmp = input[3];
    tmp = tmp << 4;
    tmp = tmp >> 1;
    output[4] = output[4] | tmp;

    tmp = input[4];
    tmp = tmp >> 5;
    output[4] = output[4] | tmp;
    tmp = input[4];
    tmp = tmp << 3;
    tmp = tmp >> 1;
    output[5] = output[5] | tmp;

    tmp = input[5];
    tmp = tmp >> 6;
    output[5] = output[5] | tmp;
    tmp = input[5];
    tmp = tmp << 2;
    tmp = tmp >> 1;
    output[6] = output[6] | tmp;

    tmp = input[6];
    tmp = tmp >> 7;
    output[6] = output[6] | tmp;
    tmp = input[6];
    tmp = tmp << 1;
    tmp = tmp >> 1;
    output[7] = output[7] | tmp;

    return output;
};

Uint32 Processor::convertColorToUint32(SDL_Color color) {
    return (Uint32)((color.r << 16) + (color.g << 8) + (color.b << 0));
};

SDL_Color Processor::convertUint32ToColor(Uint32 color) {
    SDL_Color result;

	result.a = 255;
	result.r = (color >> 16) & 0xFF;
	result.g = (color >> 8) & 0xFF;
	result.b = color & 0xFF;

    return result;
};

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

void Processor::cleanSurface(SDL_Surface* surface) {
    SDL_FillRect(surface, NULL, 0x000000);
}