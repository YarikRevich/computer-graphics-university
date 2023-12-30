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
     * Retrieves position of the nearest color to the given color compound.
     * @param colors - given map of colors.
     * @param compund - one color compund.
     * @return nearest color to the given color compound.
    */
    static SDL_Color getNearestColor(std::vector<SDL_Color> colors, Uint8 compound);

    /**
     * Sorts given color map in the incrementing way.
     * @param colors - given map of colors.
     * @param begin - index of the beginning of the the color map.
     * @param end - index of the end of the color bit map.
    */
    static void sortColorMap(std::vector<SDL_Color>& colors, int begin, int end);

    /**
     * Generates median cut selection using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param colors - reference for a result color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @param bucket - amount of generated bucket.
     * @param iteration - current iteration number. 
    */
    static void generateMedianCutBWSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration);

    static void generateMedianCutRGBSelectionRaw();

    /**
     * Generates median cut selection using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param pixels - amount of pixels of the given raw image.
     * @return result bit color map.
    */
    static std::vector<SDL_Color> generateMedianCutBWSelection(std::vector<SDL_Color>& image, int pixels);

    static std::vector<SDL_Color> generateMedianCutRGBSelection();

    /**
     * Represents all available color compounds.
    */
    enum class COLOR_COMPOUNDS {
        RED,
        GREEN,
        BLUE
    };

    /**
     * Retrieves color with the biggest difference in case of compounds.
     * @param image - given raw image color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @return color compound with the biggest global difference.
    */
    static COLOR_COMPOUNDS getCompoundDifference(std::vector<SDL_Color>& image, int begin, int end);
public:
    /**
     * Composes reduced bit color map of the given surface. 
     * @param surface - surface intended to be scaned.
     * @return reduced bit color map.
    */
    static std::vector<SDL_Color> getReducedBitColorMap(SDL_Surface* surface);

    /**
     * Composes complete bit color map of the given surface. 
     * @param surface - surface intended to be scaned.
     * @return complete bit color map.
    */
    static std::vector<SDL_Color> getCompleteBitColorMap(SDL_Surface* surface);
    
    /**
     * Generates color buckets from the given color bit map using MedianCut algorithm.
     * @param image - given image colors bit map.
     * @return palet of the color buckets.
    */
    static std::vector<SDL_Color> generateColorBuckets(SDL_Surface* surface, std::vector<SDL_Color>& image);

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
     * Retrieves amount of pixels in the given surface.
     * @param surface - surface where pixels are intended to be counted.
     * @return amount of pixels in the given surface.
    */
    static int getPixelAmount(SDL_Surface* surface);

    /**
     * Normilizes given value according to the given min and max limiters.
     * @param value - value to be normilized.
     * @param min - min value in the range.
     * @param max - max value in the range.
     * @return normilized value.
    */
    static int normalizeValue(int value, int min, int max);

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

    /**
     * 
    */
    static void setColors(SDL_Surface* surface, std::vector<SDL_Color> colors);
};