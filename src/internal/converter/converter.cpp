#include "converter.hpp"
#include <iostream>

int Converter::convertToCGU(SDL_Surface* surface, IO::CONVERSION_TYPES conversion) {
    switch (conversion) {
        case IO::CONVERSION_TYPES::NATIVE:
            // [[fallthrough]];
            break;
        case IO::CONVERSION_TYPES::PALETTE:
            std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
            if (colors.size() < BIT_NUM_MAX) {
                return EXIT_SUCCESS;
            }

            Processor::generateColorBuckets(colors, Processor::getPixelAmount(surface));
            // break;
    };

    

    

    // for (auto element : colors) {
    //     std::cout << "element: " << "r: " << (int)element.r << " g: " << (int)element.g << " b: " << (int)element.b << std::endl;
    // };

    return EXIT_SUCCESS;
}

int Converter::convertFromCGU(SDL_Surface* surface, IO::CONVERSION_TYPES conversion) {
    //
    return EXIT_SUCCESS;
}