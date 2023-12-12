#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <SDL2/SDL.h>

/**
 * Сontains toolset to perform codec operations.
*/
class Processor {
private:
    /**
     * Composes bit color map of the given surface.
     * @param
     * @return
    */
    static std::vector<SDL_Color> getBitColorMap(SDL_Surface* surface);

    /**
     * Checks if the given colors are equal.
     * @param
     * @param
     * @return
    */
    static bool isColorEqual(SDL_Color color1, SDL_Color color2);

    /**
     * Checks if the given color is present in the given color map.
     * @param colors - given map of colors.
     * @param color - given color for a condition check.
     * @return result if given color is present in the given map of colors.
    */
    static bool isColorPresent(std::vector<SDL_Color> colors, const SDL_Color& color);

    /**
     * Generates color buckets from the given color bit map using MedianCut algorithm.
     * @param colors - given colors bit map.
    */
    static void generateColorBuckets(std::vector<SDL_Color>& colors);

    /**
     * Sorts given color map in the incrementing way.
     * @param colors - given map of colors.
    */
    static void sortColorMap(std::vector<SDL_Color>& colors);

    /**
     * Retrieves pixel of the surface in the given position.
     * @param
     * @param
     * @param
     * @return
    */
    static SDL_Color getPixel(SDL_Surface* surface, int x, int y);

    /**
     * Sets pixel of the surface in the given position.
     * @param
     * @param
     * @param
     * @param
     * @return
    */
    static void setPixel(SDL_Surface* surface, int x, int y, SDL_Color color);
public:
    /**
     * Converts given surface canvas to CGU media type.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertToCGU(SDL_Surface* surface);

    /**
     * Converts given surface canvas from CGU media type
     * back to the original, but with the some loss of quality.
     * @param surface - given surface to be converted.
     * @return result operation status code.
    */
    static int convertFromCGU(SDL_Surface* surface);

    /**
     * Converts given 24 bit RGB color to 7 bit RGB single.
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit RGB single.
    */
    static Uint8 convert24BitRGBTo7BitRGB(SDL_Color color);

    /**
     * Converts given 7 bit single RGB to 24 bit RGB color.
     * @param color - given 7 bit single color to be converted.
     * @return converted 24 bit RGB color.
    */
    static SDL_Color convert7BitRGBTo24BitRGB(Uint8 color);

    /**
     * Converts given 24 bit RGB color to 7 bit grey single. 
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit grey single.
    */
    static Uint8 convert24BitRGBTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit RGB color.
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit RGB color.
    */
    static SDL_Color convert7BitGreyTo24BitRGB(Uint8 grey);

    /**
     * Normilizes given value according to the given min and max limiters.
     * @param value - value to be normilized.
     * @param min - min value in the range.
     * @param max - max value in the range.
     * @return normilized value.
    */
    static int normalizeValue(int value, int min, int max);
};