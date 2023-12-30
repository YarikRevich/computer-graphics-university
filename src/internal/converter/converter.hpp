#pragma once

#include "../io/io.hpp"
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
     * Converts given surface canvas from CGU media type in native mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGUNative(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type in palette mode.
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGUPalette(SDL_Surface* surface);
};