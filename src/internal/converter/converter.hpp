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
     * @return result operation status code.
    */
    static int convertFromCGUPaletteBW(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type in palette RGB mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGUPaletteRGB(SDL_Surface* surface);
};