#pragma once

#include "../logger/logger.hpp"
#include "../io/io.hpp"
#include "../tools/tools.hpp"
#include "../processor/processor.hpp"
#include "../io/io.hpp"

#include <vector>
#include <SDL2/SDL.h>

/**
 * Сontains toolset to perform codec operations.
 */
class Service
{
public:
    /**
     * Converts given colors to YCbCr mode.
     *
     * @param colors - given colors to be converted.
     */
    static void convertToYCbCr(std::vector<SDL_Color> &colors);

    /**
     * Converts given YCbCr colors to RGB.
     *
     * @param colors - given colors to be converted.
     */
    static void convertFromYCbCr(std::vector<SDL_Color> &colors);

    /**
     * Converts given colors to YIQ mode.
     *
     * @param colors - given colors to be converted.
     */
    static void convertToYIQ(std::vector<SDL_Color> &colors);

    /**
     * Converts given YIQ colors to RGB.
     *
     * @param colors - given colors to be converted.
     */
    static void convertFromYIQ(std::vector<SDL_Color> &colors);

    /**
     * Converts given colors to YIQ mode.
     *
     * @param colors - given colors to be converted.
     */
    static void convertToYUV(std::vector<SDL_Color> &colors);

    /**
     * Converts given YUV colors to RGB.
     *
     * @param colors - given colors to be converted.
     */
    static void convertFromYUV(std::vector<SDL_Color> &colors);

    /**
     * Converts given colors to HSL mode.
     *
     * @param colors - given colors to be converted.
     */
    static void convertToHSL(std::vector<SDL_Color> &colors);

    /**
     * Converts given HSL colors to RGB.
     *
     * @param colors - given colors to be converted.
     */
    static void convertFromHSL(std::vector<SDL_Color> &colors);

    /**
     * Converts given colors to 24 bit colorful representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<std::vector<Uint8>> convertTo24Bit(std::vector<SDL_Color> &colors);

    /**
     * Converts given 24 bit to color representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<SDL_Color> convertFrom24Bit(std::vector<std::vector<Uint8>> &colors);

    /**
     * Converts given colors to 16 bit representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<Uint16> convertTo16Bit(std::vector<SDL_Color> &colors);

    /**
     * Converts given 16 bit to color representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<SDL_Color> convertFrom16Bit(std::vector<Uint16> &colors);

    /**
     * Converts given colors to 15 bit representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<Uint16> convertTo15Bit(std::vector<SDL_Color> &colors);

    /**
     * Converts given 16 bit to color representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<SDL_Color> convertFrom15Bit(std::vector<Uint16> &colors);

    /**
     * Converts given colors to 7 bit colorful representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<std::vector<Uint8>> convertTo7BitColorful(std::vector<SDL_Color> &colors);

    /**
     * Converts given 7 bit to color representation.
     *
     * @param colors - given colors to be converted.
     * @param surface - given surface.
     * @return converted colors.
     */
    static std::vector<SDL_Color> convertFrom7BitColorful(std::vector<std::vector<Uint8>> &colors);

    /**
     * Converts given colors to 7 bit bw representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<std::vector<Uint8>> convertTo7BitBW(std::vector<SDL_Color> &colors);

    /**
     * Converts given 7 bit to bw representation.
     *
     * @param colors - given colors to be converted.
     * @return converted colors.
     */
    static std::vector<SDL_Color> convertFrom7BitBW(std::vector<std::vector<Uint8>> &colors);

    /**
     * Converts given colors to bw representation.
     *
     * @param colors - given colors to be converted.
     */
    static void convertToBW(std::vector<SDL_Color> &colors);

    /**
     * Performs image sampling using four to one one approach with rgb representation.
     * 
     * @param colors - given colors to be sampled.
     * @param surface - given surface to be modified.
     * @return result of sampling operation.
     */
    static std::vector<SDL_Color> sampleFourToOneRGB(std::vector<SDL_Color>& colors, SDL_Surface *surface);

    /**
     * Performs image sampling using four to one one approach with yuv representation.
     * 
     * @param colors - given colors to be sampled.
     * @param surface - given surface to be modified.
     * @return result of sampling operation.
     */
    static std::vector<SDL_Color> sampleFourToOneYUV(std::vector<SDL_Color>& colors, SDL_Surface *surface);

    /**
     * Performs image sampling using four to one one approach with yiq representation.
     * 
     * @param colors - given colors to be sampled.
     * @param surface - given surface to be modified.
     * @return result of sampling operation.
     */
    static std::vector<SDL_Color> sampleFourToOneYIQ(std::vector<SDL_Color>& colors, SDL_Surface *surface);

    /**
     * Performs image sampling using four to one one approach with ycbcr representation.
     * 
     * @param colors - given colors to be sampled.
     * @param surface - given surface to be modified.
     * @return result of sampling operation.
     */
    static std::vector<SDL_Color> sampleFourToOneYCbCr(std::vector<SDL_Color>& colors, SDL_Surface *surface);

    /**
     * Performs image sampling using four to one one approach with hsl representation.
     * 
     * @param colors - given colors to be sampled.
     * @param surface - given surface to be modified.
     * @return result of sampling operation.
     */
    static std::vector<SDL_Color> sampleFourToOneHSL(std::vector<SDL_Color>& colors, SDL_Surface *surface);

    /**
     * Applies colorful dithering to the surface.
     *
     * @param surface - given surface to be modified.
     * @return result operation status code.
     */
    static int applyColorfulDithering(SDL_Surface *surface);

    /**
     * Applies bw dithering to the surface.
     *
     * @param surface - given surface to be modified.
     * @return result opereation status code.
     */
    static int applyBWDithering(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette Detected mode.
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUPaletteDetected(SDL_Surface *surface);

    /**
     *
     */
    static void compressToByteRun();

    /**
     *
     */
    static void decompressFromByteRun();

    /**
     *
     */
    static void compressToRLE();

    /**
     *
     */
    static void decompressFromRLE();

    /**
     *
     */
    static void compressToLZW();

    /**
     *
     */
    static void decompressFromLZW();

    /**
     *
     */
    static void compressToLZ77();

    /**
     *
     */
    static void decompressFromLZ77();

    /**
     *
     */
    static void compressToDCT();

    /**
     *
     */
    static void decompressFromDCT();

    /**
     * Saves CGU file metadata struct with the given arguments.
     *
     * @param convertion - given CGU file raw convertion type.
     * @param bit - given CGU file raw bit type.
     * @param model - given CGU file raw model type.
     * @param losslessCompression - given CGU file raw lossless compression type.
     * @param lossyCompression - given CGU file raw lossy compression type.
     * @param sampling - given CGU file raw sampling type.
     * @param filter - given CGU file raw filter type.
     * @param dithering - CGU file dithering mode switch.
     * @param width - given CGU file width.
     * @param height - given CGU file height.
     * @param indecesSize - given CGU file color palette size.
     * @param outputStream - given output stream.
     */
    static void saveMetadata(
        IO::CONVERSION_TYPES conversionType,
        IO::BIT_TYPES bitType,
        IO::MODEL_TYPES modelType,
        IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType,
        IO::LOSSY_COMPRESSION_TYPES lossyCompressionType,
        IO::SAMPLING_TYPES samplingType,
        IO::FILTER_TYPES filterType,
        bool dithering,
        int width,
        int height,
        int indecesSize,
        std::ofstream &outputStream);
};