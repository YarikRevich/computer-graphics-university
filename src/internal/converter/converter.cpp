#include "converter.hpp"
#include <iostream>

int Converter::convertToCGUNativeRGB(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

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

            Processor::setPixel(surface, x, y, newColor);

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

    return EXIT_SUCCESS;
}

int Converter::convertToCGUNativeBW(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    SDL_Color color, newColor, tempColor;

    std::vector<std::vector<float>> colorShiftsR((surface->w)+2, std::vector<float>((surface->h)+2));
    std::vector<std::vector<float>> colorShiftsG((surface->w)+2, std::vector<float>((surface->h)+2));
    std::vector<std::vector<float>> colorShiftsB((surface->w)+2, std::vector<float>((surface->h)+2));

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

            newColor = Processor::convert7BitGreyTo24BitRGB(Processor::convert24BitRGBTo7BitGrey(tempColor));

            colorShiftR = tempColor.r - newColor.r;
            colorShiftG = tempColor.g - newColor.g;
            colorShiftB = tempColor.b - newColor.b;

            Processor::setPixel(surface, x, y, newColor);

            colorShiftsR[x+1+NATIVE_SHIFT][y] += (colorShiftR * 7.0 / 16.0);
            colorShiftsR[x-1+NATIVE_SHIFT][y+1] += (colorShiftR * 3.0 / 16.0);
            colorShiftsR[x+NATIVE_SHIFT][y+1] += (colorShiftR * 5.0 / 16.0);
            colorShiftsR[x+1+NATIVE_SHIFT][y+1] += (colorShiftR * 1.0 / 16.0);

            colorShiftsG[x+1+NATIVE_SHIFT][y] += (colorShiftG * 7.0 / 16.0);
            colorShiftsG[x-1+NATIVE_SHIFT][y+1] += (colorShiftG * 3.0 / 16.0);
            colorShiftsG[x+NATIVE_SHIFT][y+1] += (colorShiftG * 5.0 / 16.0);
            colorShiftsG[x+1+NATIVE_SHIFT][y+1] += (colorShiftG * 1.0 / 16.0);

            colorShiftsB[x+1+NATIVE_SHIFT][y] += (colorShiftB * 7.0 / 16.0);
            colorShiftsB[x-1+NATIVE_SHIFT][y+1] += (colorShiftB * 3.0 / 16.0);
            colorShiftsB[x+NATIVE_SHIFT][y+1] += (colorShiftB * 5.0 / 16.0);
            colorShiftsB[x+1+NATIVE_SHIFT][y+1] += (colorShiftB * 1.0 / 16.0);
        }
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteRGB(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsRGB(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteBW(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsBW(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertFromCGUNativeBW(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Converter::convertFromCGUNativeRGB(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Converter::convertFromCGUPaletteBW(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Converter::convertFromCGUPaletteRGB(SDL_Surface* surface) {
    Tools::startIndefiniteSpinner();

    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}