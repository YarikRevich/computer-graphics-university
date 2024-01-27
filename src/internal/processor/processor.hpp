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
public:
    /**
     * Represents pixel unit to be used for batch surface update.
    */
    class PixelPoint {
        public:
            int x, y;

            SDL_Color color;

            PixelPoint(int x, int y, SDL_Color color) : x(x), y(y), color(color) {}
    };

    /**
     * Represents all available color compounds.
    */
    enum class COLOR_COMPOUNDS {
        RED,
        GREEN,
        BLUE
    };

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
     * Generates color buckets from the given color bit map using MedianCut algorithm in BW mode.
     * @param image - given image colors bit map.
     * @return palet of the color buckets.
    */
    static std::vector<Processor::PixelPoint> generateColorBucketsBW(SDL_Surface* surface, std::vector<SDL_Color>& image);

    /**
     * Generates color buckets from the given color bit map using MedianCut algorithm in RGB mode.
     * @param surface - surface intended to be scaned.
     * @param image - given image colors bit map.
     * @return palette of the color buckets.
    */
    static std::vector<Processor::PixelPoint> generateColorBucketsRGB(SDL_Surface* surface, std::vector<SDL_Color>& image);

    /**
     * Generates dedicated palette using the processed reduced color map.
     * @param surface - surface intended to be scaned.
     * @param image - given reduced image colors bit map.
     * @return dedicated palette of the color bit map.
    */
    static std::vector<Processor::PixelPoint> generateDedicatedPalette(SDL_Surface* surface, std::vector<SDL_Color>& image);

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
     * Converts 8 bit color to 7 bit color.
     * @param color - given 8 bit color to be converted.
     * @return converted 7 bit color.
    */
    static Uint8 convert8BitTo7Bit(Uint8 color);

    /**
     * Converts 7 bit RGB color to 8 bit RGB color.
     * @param color - given 7 bit color to be converted.
     * @return converted 8 bit color.
    */
    static Uint8 convert7BitTo8BitRGB(Uint8 color);

    /**
     * Converts given RGB color to its Grey representation.
     * @param color - given RGB color to be converted to Grey.
     * @return result of the conversion.
    */
    static Uint8 convertRGBToGreyUint8(SDL_Color color);

    /**
     * Converts given RGB color to its Grey representation.
     * @param color - given RGB color to be converted to Grey.
     * @return result of the conversion.
    */
    static SDL_Color convertRGBToGrey(SDL_Color color);

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
     * @param surface - surface where pixels are intended to be retrieved.
     * @param x - pixel location in the x-axis.
     * @param y - pixel location in the y-axis.
     * @return pixel at the given location.
    */
    static SDL_Color getPixel(SDL_Surface* surface, int x, int y);

    /**
     * Sets pixel for the surface in the given position.
     * @param surface - surface where pixel is intended to be set.
     * @param x - pixel location in the x-axis.
     * @param y - pixel location in the y-axis.
     * @param color - color to be set at the given location.
    */
    static void setPixel(SDL_Surface* surface, int x, int y, SDL_Color color);

    /**
     * Sets given pixels for the surface.
     * @param surface - surface where pixels are intended to be set.
     * @param pixels - pixels intended to be set in the given surface.
    */
    static void setPixels(SDL_Surface* surface, std::vector<PixelPoint> pixels);

    /**
     * Cleans the given surface with the empty rectangle.
     * @param surface - given surface to be cleaned.
    */
    static void cleanSurface(SDL_Surface* surface);
private:
    /**
     * Checks if the given colors are equal.
     * @param color1 - first color to be used in comparison
     * @param color2 - second color to be used in comparison
     * @return result of the check.
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
     * Retrieves position of the nearest color to the given BW color compound.
     * @param colors - given map of colors.
     * @param compund - one color compund.
     * @return nearest color to the given color compound.
    */
    static SDL_Color getNearestColorBW(std::vector<SDL_Color> colors, Uint8 compound);

    /**
     * Retrieves position of the nearest color to the given RGB color.
     * @param colors - given map of colors.
     * @param compund - one color compund.
     * @return nearest color to the given color compound.
    */
    static SDL_Color getNearestColorRGB(std::vector<SDL_Color> colors, SDL_Color src);

    /**
     * Sorts given color map in BW mode, in the incrementing way.
     * @param colors - given map of colors.
     * @param begin - index of the beginning of the the color map.
     * @param end - index of the end of the color bit map.
    */
    static void sortColorMapBW(std::vector<SDL_Color>& colors, int begin, int end);

    /**
     * Sorts given color map in RGB mode, in the incrementing way.
     * @param colors - given map of colors.
     * @param begin - index of the beginning of the the color map.
     * @param end - index of the end of the color bit map.
     * @param compound - selective color compound. 
    */
    static void sortColorMapRGB(std::vector<SDL_Color>& colors, int begin, int end, COLOR_COMPOUNDS compound);

    /**
     * Generates MedianCut selection in BW mode, using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param colors - reference for a result color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @param bucket - amount of generated bucket.
     * @param iteration - current iteration number. 
    */
    static void generateMedianCutBWSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration);

    /**
     * Generates MedianCut selection in RGB mode, using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param colors - reference for a result color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @param bucket - amount of generated bucket.
     * @param iteration - current iteration number. 
    */
    static void generateMedianCutRGBSelectionRaw(std::vector<SDL_Color>& image, std::vector<SDL_Color>& colors, int begin, int end, int* bucket, int iteration);

    /**
     * Executes MedianCut selection for BW, using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param pixels - amount of pixels of the given raw image.
     * @return result bit color map.
    */
    static std::vector<SDL_Color> generateMedianCutBWSelection(std::vector<SDL_Color>& image, int pixels);

    /**
     * Executes MedianCut selection for RGB, using proper algorithm implementation.
     * @param image - given raw image color bits.
     * @param pixels - amount of pixels of the given raw image.
     * @return result bit color map.
    */
    static std::vector<SDL_Color> generateMedianCutRGBSelection(std::vector<SDL_Color>& image, int pixels);

    /**
     * Retrieves color with the biggest difference in case of compounds.
     * @param image - given raw image color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @return color compound with the biggest global difference.
    */
    static COLOR_COMPOUNDS getCompoundDifference(std::vector<SDL_Color>& image, int begin, int end);
};