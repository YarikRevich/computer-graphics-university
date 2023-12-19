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
     * Converts given surface canvas to CGU media type.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGU(SDL_Surface* surface, IO::CONVERSION_TYPES convertion);

    /**
     * Converts given surface canvas from CGU media type
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGU(SDL_Surface* surface, IO::CONVERSION_TYPES convertion);
};