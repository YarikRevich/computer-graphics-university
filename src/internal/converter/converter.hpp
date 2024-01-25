#pragma once

#include "../logger/logger.hpp"
#include "../io/io.hpp"
#include "../tools/tools.hpp"
#include "../processor/processor.hpp"

#include <vector>
#include <SDL2/SDL.h>

/**
 * Сontains toolset to perform codec operations.
*/
class Converter {
public:
    /**
     * Converts given surface canvas to CGU media type in native RGB mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUNativeRGB(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in native BW mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUNativeBW(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in native RGB mode including dithering.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUNativeRGBDithering(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in native BW mode including dithering.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUNativeBWDithering(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in palette RGB mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUPaletteRGB(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in palette BW mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUPaletteBW(SDL_Surface* surface);

    /**
     * Converts given surface canvas to CGU media type in palette Detected mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGUPaletteDetected(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type in native BW mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGUNativeBW(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type in native RGB mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGUNativeRGB(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type in palette BW mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @param indeces - retrieved metadata indeces.
     * @return CGU palette indexes.
    */
    static int convertFromCGUPaletteBW(SDL_Surface* surface, std::vector<int>& indeces);

    /**
     * Converts given surface canvas from CGU media type in palette RGB mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @param indeces - retrieved metadata indeces.
     * @return CGU palette indexes.
    */
    static int convertFromCGUPaletteRGB(SDL_Surface* surface, std::vector<int>& indeces);

    /**
     * Converts given surface canvas from CGU media type in palette Detected mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return CGU palette indexes.
    */
    static std::vector<int> convertFromCGUPaletteDetected(SDL_Surface* surface);
};