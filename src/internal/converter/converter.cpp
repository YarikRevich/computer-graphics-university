#include "converter.hpp"
#include <iostream>

int Converter::convertToCGUNativeRGB(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for(int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL) {
        assemble.clear();

        for(int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    for (std::vector<Uint8> &value : buff) {        
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}





// kolor = getPixel(x,y);
//             tymR = kolor.r + bledyR[x+przesuniecie][y];
//             tymG = kolor.g + bledyG[x+przesuniecie][y];
//             tymB = kolor.b + bledyB[x+przesuniecie][y];


//             if(tymR > 255) tymR = 255;
//             if(tymR < 0) tymR = 0;

//             if(tymG > 255) tymG = 255;
//             if(tymG < 0) tymG = 0;

//             if(tymB > 255) tymB = 255;
//             if(tymB < 0) tymB = 0;

//             tymKolor.r = tymR;
//             tymKolor.g = tymG;
//             tymKolor.b = tymB;

//             kolor_new = paleta[znajdzSasiada(tymKolor)];

//             nowyR = kolor_new.r;
//             blad = tymR - nowyR;

//             bledyR[x+1+przesuniecie][y] += (blad*7.0/16.0);
//             bledyR[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
//             bledyR[x+przesuniecie][y+1] += (blad*5.0/16.0);
//             bledyR[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

//             nowyG = kolor_new.g;
//             blad = tymG - nowyG;

//             bledyG[x+1+przesuniecie][y] += (blad*7.0/16.0);
//             bledyG[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
//             bledyG[x+przesuniecie][y+1] += (blad*5.0/16.0);
//             bledyG[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

//             nowyB = kolor_new.b;
//             blad = tymB - nowyB;

//             bledyB[x+1+przesuniecie][y] += (blad*7.0/16.0);
//             bledyB[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
//             bledyB[x+przesuniecie][y+1] += (blad*5.0/16.0);
//             bledyB[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

//             setPixel(x + szerokosc/2, y, kolor_new.r, kolor_new.g, kolor_new.b);








int Converter::convertToCGUNativeRGBDithering(SDL_Surface* surface, std::ofstream& outputStream) {
    // SDL_Color color, newColor, tempColor;

    // std::vector<std::vector<float>> colorShiftsR((surface->w) + 2, std::vector<float>((surface->h) + 2));
    // std::vector<std::vector<float>> colorShiftsG((surface->w) + 2, std::vector<float>((surface->h) + 2));
    // std::vector<std::vector<float>> colorShiftsB((surface->w) + 2, std::vector<float>((surface->h) + 2));

    // int colorShiftR = 0;
    // int colorShiftG = 0;
    // int colorShiftB = 0;

    // for (int y = 0; y < surface->h; y++){
    //     for(int x = 0; x < surface->w; x++){
    //         color = Processor::getPixel(surface, x, y);

    //         tempColor.r = Processor::normalizeValue(
    //             color.r + colorShiftsR[x+NATIVE_SHIFT][y], 0, 255);

    //         tempColor.g = Processor::normalizeValue(
    //             color.g + colorShiftsG[x+NATIVE_SHIFT][y], 0, 255);

    //         tempColor.b = Processor::normalizeValue(
    //             color.b + colorShiftsB[x+NATIVE_SHIFT][y], 0, 255);

    //         newColor = Processor::convert7BitRGBTo24BitRGB(Processor::convert24BitRGBTo7BitRGB(tempColor));

    //         colorShiftR = tempColor.r - newColor.r;
    //         colorShiftG = tempColor.g - newColor.g;
    //         colorShiftB = tempColor.b - newColor.b;

    //         Processor::setPixel(surface, x, y, newColor);

    //         colorShiftsR[x + 1 + NATIVE_SHIFT][y] += (colorShiftR * 7.0 / 16.0);
    //         colorShiftsR[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftR * 3.0 / 16.0);
    //         colorShiftsR[x + NATIVE_SHIFT][y + 1] += (colorShiftR * 5.0 / 16.0);
    //         colorShiftsR[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftR * 1.0 / 16.0);

    //         colorShiftsG[x + 1 + NATIVE_SHIFT][y] += (colorShiftG * 7.0 / 16.0);
    //         colorShiftsG[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftG * 3.0 / 16.0);
    //         colorShiftsG[x + NATIVE_SHIFT][y + 1] += (colorShiftG * 5.0 / 16.0);
    //         colorShiftsG[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftG * 1.0 / 16.0);

    //         colorShiftsB[x + 1 + NATIVE_SHIFT][y] += (colorShiftB * 7.0 / 16.0);
    //         colorShiftsB[x - 1 + NATIVE_SHIFT][y + 1] += (colorShiftB * 3.0 / 16.0);
    //         colorShiftsB[x + NATIVE_SHIFT][y + 1] += (colorShiftB * 5.0 / 16.0);
    //         colorShiftsB[x + 1 + NATIVE_SHIFT][y + 1] += (colorShiftB * 1.0 / 16.0);
    //     }
    // }
    std::vector<Processor::PixelPoint> result = 
        Processor::generateFloydSteinbergDitheringRGB(surface);

    Processor::setPixels(surface, result);

    return convertToCGUNativeRGB(surface, outputStream);
}

int Converter::convertToCGUNativeBW(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for(int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL) {
        assemble.clear();

        for(int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    for (std::vector<Uint8> &value : buff) {        
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUNativeBWDithering(SDL_Surface* surface, std::ofstream& outputStream) {

    std::vector<Processor::PixelPoint> result = 
        Processor::generateFloydSteinbergDitheringBW(surface);

    Processor::setPixels(surface, result);

    // std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);

    // SDL_Color color, newColor, tempColor;
    // Uint8 grey, newGrey, tempGrey;

    // std::vector<std::vector<float>> colorShifts((surface->w)+2, std::vector<float>((surface->h)+2));

    // int colorShift = 0;

    // for (int y = 0; y < surface->h; y++){
    //     for(int x = 0; x < surface->w; x++){
    //         color = Processor::getPixel(surface, x, y);

    //         grey = Processor::convertRGBToGreyUint8(color);

    //         tempGrey = Processor::normalizeValue(
    //             grey + colorShifts[x+NATIVE_SHIFT][y], 0, 255);

    //         tempColor.r = tempGrey;
    //         tempColor.g = tempGrey;
    //         tempColor.b = tempGrey;

    //         newColor = Processor::convert7BitGreyTo24BitRGB(Processor::convert24BitRGBTo7BitGrey(tempColor));

    //         newGrey = newColor.r;

    //         colorShift = tempGrey - newGrey;

    //         Processor::setPixel(surface, x, y, newColor);

    //         colorShifts[x+1+NATIVE_SHIFT][y] += (colorShift * 7.0 / 16.0);
    //         colorShifts[x-1+NATIVE_SHIFT][y+1] += (colorShift * 3.0 / 16.0);
    //         colorShifts[x+NATIVE_SHIFT][y+1] += (colorShift * 5.0 / 16.0);
    //         colorShifts[x+1+NATIVE_SHIFT][y+1] += (colorShift * 1.0 / 16.0);
    //     }
    // }

    return convertToCGUNativeBW(surface, outputStream);
}

int Converter::convertToCGUPaletteRGB(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsRGB(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteRGBDithering(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsRGB(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteBW(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsBW(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteBWDithering(SDL_Surface* surface, std::ofstream& outputStream) {
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() > BIT_NUM_MAX) {
        Logger::SetError(BIT_SIZE_MAX_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateColorBucketsBW(surface, image);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteDetected(SDL_Surface* surface) {
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateDedicatedPalette(surface, colors);

    Processor::cleanSurface(surface);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

SDL_Surface* Converter::convertFromCGUNativeRGB(std::ifstream& inputStream, IO::FileMetadata* metadata) {
    inputStream.seekg(metadata->getSize());

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> input(PREFERRED_BIT_NUM_PER_PIXEL, 0);
    for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++) {
        inputStream.read((char *)(input.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

        buff.push_back(input);
    }

    std::vector<SDL_Color> image;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : buff) {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble) {
            image.push_back(Processor::convert7BitRGBTo24BitRGB(compound));
        }
    }

    SDL_Surface *surface = 
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++) {
        if (y == surface->h) { 
            x += 1;
            y = 0;
        } 

        Processor::setPixel(surface, x, y, image[k]);
        
        if (x == surface->w) {
            y += 1;
            x = 0;            
        } else {
            y += 1;
        }
    }

    return surface;
};

SDL_Surface* Converter::convertFromCGUNativeBW(std::ifstream& inputStream, IO::FileMetadata* metadata) {
    inputStream.seekg(metadata->getSize());

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> input(PREFERRED_BIT_NUM_PER_PIXEL, 0);
    for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++) {
        inputStream.read((char *)(input.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

        buff.push_back(input);
    }

    std::vector<SDL_Color> image;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : buff) {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble) {
            image.push_back(Processor::convert7BitRGBTo24BitRGB(compound));
        }
    }

    SDL_Surface *surface = 
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++) {
        if (y == surface->h) { 
            x += 1;
            y = 0;
        } 

        Processor::setPixel(surface, x, y, image[k]);
        
        if (x == surface->w) {
            y += 1;
            x = 0;            
        } else {
            y += 1;
        }
    }

    return surface;
}