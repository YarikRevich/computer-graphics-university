#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

/**
 * Сontains toolset to perform codec operations.
 */
class Processor
{
public:
    /**
     * Represents pixel unit to be used for batch surface update.
     */
    class PixelPoint
    {
    public:
        /**
         * Represents a position of the pixel point.
         */
        int x, y;

        /**
         * Represents a color of the pixel point.
         */
        SDL_Color color;

        PixelPoint(int x, int y, SDL_Color color) : x(x), y(y), color(color) {}
    };

    /**
     * Represents all available color compounds.
     */
    enum class COLOR_COMPOUNDS
    {
        RED,
        GREEN,
        BLUE
    };

    /**
     * Composes reduced bit color map of the given surface.
     *
     * @param surface - surface intended to be scaned.
     * @return reduced bit color map.
     */
    static std::vector<SDL_Color> getReducedBitColorMap(SDL_Surface *surface);

    /**
     * Composes complete bit color map of the given surface.
     *
     * @param surface - surface intended to be scaned.
     * @return complete bit color map.
     */
    static std::vector<SDL_Color> getCompleteBitColorMap(SDL_Surface *surface);

    /**
     * Represents result struct for the MedianCut related algorithms.
     */
    class BucketResult
    {
    public:
        BucketResult(std::vector<SDL_Color> colors, std::vector<int> indeces) : colors(colors), indeces(indeces) {};

        /**
         * Retrieves color bit map.
         *
         * @return retrieved color bit map.
         */
        std::vector<SDL_Color> getColors();

        /**
         * Retrieves color bit map indeces.
         *
         * @return retrieved color bit map indeces.
         */
        std::vector<int> getIndeces();

    private:
        /**
         * Represents bucket colors.
         */
        std::vector<SDL_Color> colors;

        /**
         * Represents bucket indeces.
         */
        std::vector<int> indeces;
    };

    /**
     * Generates color buckets from the given color bit map using MedianCut algorithm in BW mode.
     *
     * @param image - given image colors bit map.
     * @return result bucket.
     */
    static BucketResult *generateColorBucketsBW(SDL_Surface *surface, std::vector<SDL_Color> &image);

    /**
     * Generates color buckets from the given color bit map using MedianCut algorithm in RGB mode.
     *
     * @param surface - surface intended to be scanned.
     * @param image - given image colors bit map.
     * @return result bucket.
     */
    static BucketResult *generateColorBucketsRGB(SDL_Surface *surface, std::vector<SDL_Color> &image);

    /**
     * Generates dedicated palette using the processed reduced color map.
     *
     * @param surface - surface intended to be scanned.
     * @param image - given reduced image colors bit map.
     * @return dedicated palette of the color bit map.
     */
    static std::vector<Processor::PixelPoint> generateDedicatedPalette(SDL_Surface *surface, std::vector<SDL_Color> &image);

    /**
     * Generates palette with the applied dithering Floyd-Steinberg algorithm for RGB mode.
     *
     * @param surface - surface intended to be scanned.
     * @return palette with the applied dithering algorithm.
     */
    static std::vector<Processor::PixelPoint> generateFloydSteinbergDitheringRGB(SDL_Surface *surface);

    /**
     * Generates palette with the applied dithering Floyd-Steinberg algorithm for BW mode.
     *
     * @param surface - surface intended to be scanned.
     * @return palette with the applied dithering algorithm.
     */
    static std::vector<Processor::PixelPoint> generateFloydSteinbergDitheringBW(SDL_Surface *surface);

    /**
     * Converts given 24 bit RGB color to 7 bit RGB single.
     *
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit RGB single.
     */
    static Uint8 convert24BitRGBTo7BitRGB(SDL_Color color);

    /**
     * Converts given 7 bit single RGB to 24 bit RGB color.
     *
     * @param color - given 7 bit single color to be converted.
     * @return converted 24 bit RGB color.
     */
    static SDL_Color convert7BitRGBTo24BitRGB(Uint8 color);

    /**
     * Converts given 24 bit RGB color to 7 bit grey single.
     *
     * @param color - given 24 bit RGB color to be converted.
     * @return converted 7 bit grey single.
     */
    static Uint8 convert24BitRGBTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit RGB color.
     *
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit RGB color.
     */
    static SDL_Color convert7BitGreyTo24BitRGB(Uint8 grey);

    /**
     * Converts given 24 bit YCbCr color to 7 bit grey single.
     *
     * @param color - given 24 bit YCbCr color to be converted.
     * @return converted 7 bit grey single.
     */
    static Uint8 convert24BitYCbCrTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit YCbCr color.
     *
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit YCbCr color.
     */
    static SDL_Color convert7BitGreyTo24BitYCbCr(Uint8 grey);

    /**
     * Converts given 24 bit YUV color to 7 bit grey single.
     *
     * @param color - given 24 bit YUV color to be converted.
     * @return converted 7 bit grey single.
     */
    static Uint8 convert24BitYUVTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit YUV color.
     *
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit YUV color.
     */
    static SDL_Color convert7BitGreyTo24BitYUV(Uint8 grey);

    /**
     * Converts given 24 bit YIQ color to 7 bit grey single.
     *
     * @param color - given 24 bit YIQ color to be converted.
     * @return converted 7 bit grey single.
     */
    static Uint8 convert24BitYIQTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit YIQ color.
     *
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit YIQ color.
     */
    static SDL_Color convert7BitGreyTo24BitYIQ(Uint8 grey);

    /**
     * Converts given 24 bit HSL color to 7 bit grey single.
     *
     * @param color - given 24 bit HSL color to be converted.
     * @return converted 7 bit grey single.
     */
    static Uint8 convert24BitHSLTo7BitGrey(SDL_Color color);

    /**
     * Converts 7 bit grey single to 24 bit HSL color.
     *
     * @param color - given 7 bit grey single to be converted.
     * @return converted 24 bit HSL color.
     */
    static SDL_Color convert7BitGreyTo24BitHSL(Uint8 grey);

    /**
     * Converts given 24 bit color to 16 bit single.
     *
     * @param color - given 24 bit color to be converted.
     * @return converted 16 bit single.
     */
    static Uint16 convert24BitColorTo16BitColor(SDL_Color color);

    /**
     * Converts given 16 bit single to 24 bit color.
     *
     * @param color - given 16 bit single color to be converted.
     * @return converted 24 bit color.
     */
    static SDL_Color convert16BitColorTo24BitColor(Uint16 color);

    /**
     * Converts given 24 bit color to 15 bit single.
     *
     * @param color - given 24 bit color to be converted.
     * @return converted 15 bit single.
     */
    static Uint16 convert24BitColorTo15BitColor(SDL_Color color);

    /**
     * Converts given 15 bit single to 24 bit color.
     *
     * @param color - given 15 bit single color to be converted.
     * @return converted 24 bit color.
     */
    static SDL_Color convert15BitColorTo24BitColor(Uint16 color);
    /**
     * Converts 8 bit color to 7 bit color.
     *
     * @param input - given 8 bit color to be converted.
     * @return converted 7 bit color.
     */
    static std::vector<Uint8> convert8BitTo7Bit(std::vector<Uint8> input);

    /**
     * Converts 7 bit RGB color to 8 bit RGB color.
     *
     * @param input - given 7 bit color to be converted.
     * @return converted 8 bit color.
     */
    static std::vector<Uint8> convert7BitTo8Bit(std::vector<Uint8> input);

    /**
     * Converts given SDL_Color color to its Uint32 representation.
     *
     * @param color - given SDL_Color color to be converted.
     * @return converted Uint32 representation.
     */
    static Uint32 convertColorToUint32(SDL_Color color);

    /**
     * Converts given Uint32 color to its SDL_Color representation.
     *
     * @param color - given Uint32 color to be converted.
     * @return converted SDL_Color representation.
     */
    static SDL_Color convertUint32ToColor(Uint32 color);

    /**
     * Converts given color to its Grey representation.
     *
     * @param color - given color to be converted to Grey.
     * @return result of the conversion.
     */
    static Uint8 convertColorToGreyUint8(SDL_Color color);

    /**
     * Converts given RGB to its Grey representation.
     *
     * @param color - given color to be converted to Grey.
     * @return result of the conversion.
     */
    static SDL_Color convertRGBToGrey(SDL_Color color);

    /**
     * Converts given YCbCr color to RGB.
     *
     * @param y - given 'y' compound.
     * @param cb - given 'cb' compound.
     * @param cr - given 'cr' compound.
     * @return result of conversion.
     */
    static SDL_Color convertYCbCrToRGB(int y, int cb, int cr);

    /**
     * Converts given RGB color to YCbCr.
     *
     * @param color - given RGB color.
     * @return result of conversion.
     */
    static SDL_Color convertRGBToYCbCr(SDL_Color color);

    /**
     * Converts given YUV color to RGB.
     *
     * @param y - given 'y' compound.
     * @param u - given 'u' compound.
     * @param v - given 'v' compound.
     * @return result of conversion.
     */
    static SDL_Color convertYUVToRGB(int y, int u, int v);

    /**
     * Converts given RGB color to YUV.
     *
     * @param color - given RGB color.
     * @return result of conversion.
     */
    static SDL_Color convertRGBToYUV(SDL_Color color);

    /**
     * Converts given YIQ color to RGB.
     *
     * @param y - given 'y' compound.
     * @param i - given 'i' compound.
     * @param q - given 'q' compound.
     * @return result of conversion.
     */
    static SDL_Color convertYIQToRGB(int y, int i, int q);

    /**
     * Converts given RGB color to YIQ.
     *
     * @param color - given RGB color.
     * @return result of conversion.
     */
    static SDL_Color convertRGBToYIQ(SDL_Color color);

    /**
     * Converts given HSL color to RGB.
     *
     * @param h - given 'h' compound.
     * @param s - given 's' compound.
     * @param l - given 'l' compound.
     * @return result of conversion.
     */
    static SDL_Color convertHSLToRGB(int h, int s, int l);

    /**
     * Converts given RGB color to HSL.
     *
     * @param color - given RGB color.
     * @return result of conversion.
     */
    static SDL_Color convertRGBToHSL(SDL_Color color);

    /**
     * Retrieves amount of pixels in the given surface.
     *
     * @param surface - surface where pixels are intended to be counted.
     * @return amount of pixels in the given surface.
     */
    static int getPixelAmount(SDL_Surface *surface);

    /**
     * Normilizes given value according to the given min and max limiters.
     *
     * @param value - value to be normilized.
     * @param min - min value in the range.
     * @param max - max value in the range.
     * @return normilized value.
     */
    static int normalizeValue(int value, int min, int max);

    /**
     * Retrieves pixel of the surface in the given position.
     *
     * @param surface - surface where pixels are intended to be retrieved.
     * @param x - pixel location in the x-axis.
     * @param y - pixel location in the y-axis.
     * @return pixel at the given location.
     */
    static SDL_Color getPixel(SDL_Surface *surface, int x, int y);

    /**
     * Sets pixel for the surface in the given position.
     *
     * @param surface - surface where pixel is intended to be set.
     * @param x - pixel location in the x-axis.
     * @param y - pixel location in the y-axis.
     * @param color - color to be set at the given location.
     */
    static void setPixel(SDL_Surface *surface, int x, int y, SDL_Color color);

    /**
     * Creates surface filled with the given colors.
     *
     * @param width - given surface width.
     * @param height - given surface height.
     * @param image - given colors.
     * @return created surface.
     */
    static SDL_Surface *createFilledSurface(int width, int height, std::vector<SDL_Color> &image);

    /**
     * Sets given pixels for the surface.
     *
     * @param surface - surface where pixels are intended to be set.
     * @param pixels - pixels intended to be set in the given surface.
     */
    static void setPixels(SDL_Surface *surface, std::vector<PixelPoint> pixels);

    /**
     * Cleans the given surface with the empty rectangle.
     *
     * @param surface - given surface to be cleaned.
     */
    static void cleanSurface(SDL_Surface *surface);

    /**
     * Represents LZ77 compression result holder.
     */
    template<typename T>
    class LZ77Result
    {
    public:
        LZ77Result(int distance, int length, T symbol) : distance{distance}, length{length}, symbol{symbol} {}

        /**
         * Retrieves distance value.
         *
         * @return retrieved distance value.
         */
        int getDistance();

        /**
         * Retrieves length value.
         *
         * @return retrieved length value.
         */
        int getLength();

        /**
         * Retrieves symbol value.
         *
         * @return retrieved symbol value.
         */
        T getSymbol();

    private:
        /**
         * Represents distance value.
         */
        int distance;

        /**
         * Represents length value.
         */
        int length;

        /**
         * Represents symbol value.
         */
        T symbol;
    };

private:
    /**
     * Checks if the given colors are equal.
     *
     * @param color1 - first color to be used in comparison
     * @param color2 - second color to be used in comparison
     * @return result of the check.
     */
    static bool isColorEqual(SDL_Color color1, SDL_Color color2);

    /**
     * Checks if the given color is present in the given color map.
     *
     * @param colors - given map of colors.
     * @param color - given color for a condition check.
     * @return result if given color is present in the given map of colors.
     */
    static bool isColorPresent(std::vector<SDL_Color> colors, const SDL_Color &color);

    /**
     * Retrieves position of the nearest color to the given BW color compound.
     *
     * @param colors - given map of colors.
     * @param compund - one color compund.
     * @return nearest color to the given color compound.
     */
    static SDL_Color getNearestColorBW(std::vector<SDL_Color> colors, Uint8 compound);

    /**
     * Retrieves position of the nearest color to the given RGB color.
     *
     * @param colors - given map of colors.
     * @param compund - one color compund.
     * @return nearest color to the given color compound.
     */
    static SDL_Color getNearestColorRGB(std::vector<SDL_Color> colors, SDL_Color src);

    /**
     * Retrieves given color index in the given palette.
     *
     * @param colors - given bit color map to be scanned.
     * @param src - given color.
     * @return retrieved given color's index in the given color bit map(returns -1 if not found).
     */
    static int getColorIndex(std::vector<SDL_Color> colors, SDL_Color src);

    /**
     * Sorts given color map in BW mode, in the incrementing way.
     *
     * @param colors - given map of colors.
     * @param begin - index of the beginning of the the color map.
     * @param end - index of the end of the color bit map.
     */
    static void sortColorMapBW(std::vector<SDL_Color> &colors, int begin, int end);

    /**
     * Sorts given color map in RGB mode, in the incrementing way.
     *
     * @param colors - given map of colors.
     * @param begin - index of the beginning of the the color map.
     * @param end - index of the end of the color bit map.
     * @param compound - selective color compound.
     */
    static void sortColorMapRGB(std::vector<SDL_Color> &colors, int begin, int end, COLOR_COMPOUNDS compound);

    /**
     * Generates MedianCut selection in BW mode, using proper algorithm implementation.
     *
     * @param image - given raw image color bits.
     * @param colors - reference for a result color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @param bucket - amount of generated bucket.
     * @param iteration - current iteration number.
     */
    static void generateMedianCutBWSelectionRaw(std::vector<SDL_Color> &image, std::vector<SDL_Color> &colors, int begin, int end, int *bucket, int iteration);

    /**
     * Generates MedianCut selection in RGB mode, using proper algorithm implementation.
     *
     * @param image - given raw image color bits.
     * @param colors - reference for a result color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @param bucket - amount of generated bucket.
     * @param iteration - current iteration number.
     */
    static void generateMedianCutRGBSelectionRaw(std::vector<SDL_Color> &image, std::vector<SDL_Color> &colors, int begin, int end, int *bucket, int iteration);

    /**
     * Executes MedianCut selection for BW, using proper algorithm implementation.
     *
     * @param image - given raw image color bits.
     * @param pixels - amount of pixels of the given raw image.
     * @return result bit color map.
     */
    static std::vector<SDL_Color> generateMedianCutBWSelection(std::vector<SDL_Color> &image, int pixels);

    /**
     * Executes MedianCut selection for RGB, using proper algorithm implementation.
     *
     * @param image - given raw image color bits.
     * @param pixels - amount of pixels of the given raw image.
     * @return result bit color map.
     */
    static std::vector<SDL_Color> generateMedianCutRGBSelection(std::vector<SDL_Color> &image, int pixels);

    /**
     * Retrieves color with the biggest difference in case of compounds.
     *
     * @param image - given raw image color bits.
     * @param begin - beggining of scanning process.
     * @param end - ending of scanning process.
     * @return color compound with the biggest global difference.
     */
    static COLOR_COMPOUNDS getCompoundDifference(std::vector<SDL_Color> &image, int begin, int end);
};