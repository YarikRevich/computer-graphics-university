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
     * Represents a result of palette based conversion.
     */
    class PaletteConversionResult
    {
    public:
        PaletteConversionResult(
            std::vector<int> data, std::vector<Uint32> indeces) : data(data), indeces(indeces) {};

    private:
        /**
         * Represents palette conversion data.
         */
        std::vector<int> data;

        /**
         * Represents palette conversion indeces.
         */
        std::vector<Uint32> indeces;
    };

    /**
     * Represents a result of native based conversion.
     */
    class NativeConversionResult
    {
    public:
        NativeConversionResult(std::vector<std::vector<Uint8>> data) : data(data) {};

    private:
        /**
         * represents native conversion data.
         */
        std::vector<std::vector<Uint8>> data;
    };

    /**
     * Converts given surface canvas to CGU media type in native colorful mode.
     *
     * @param surface - given surface to be converted.
     * @return native conversion result.
     */
    static Service::NativeConversionResult *convertToCGUNativeColorful(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in native BW mode.
     *
     * @param surface - given surface to be converted.
     * @return native conversion result.
     */
    static Service::NativeConversionResult *convertToCGUNativeBW(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette colorful mode.
     *
     * @param surface - given surface to be converted.
     * @return palette conversion result.
     */
    static Service::PaletteConversionResult *convertToCGUPaletteColorful(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette BW mode.
     *
     * @param surface - given surface to be converted.
     * @return palette conversion result.
     */
    static Service::PaletteConversionResult *convertToCGUPaletteBW(SDL_Surface *surface);

    /**
     * Applies colorful dithering to the surface.
     *
     * @param surface - given surface to be modified.
     * @return result opereation status code.
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
     * Converts given surface canvas from CGU media type in native colorful mode.
     * back to the original, but with the some loss of quality.
     *
     * @param inputStream - given input stream.
     * @param metadata - given file metadata.
     * @return result operation status code.
     */
    static SDL_Surface *convertFromCGUNativeColorful(std::ifstream &inputStream, IO::FileMetadata *metadata);

    /**
     * Converts given surface canvas from CGU media type in native BW mode.
     * back to the original, but with the some loss of quality.
     *
     * @param inputStream - given input stream.
     * @param metadata - given file metadata.
     * @return result operation status code.
     */
    static SDL_Surface *convertFromCGUNativeBW(std::ifstream &inputStream, IO::FileMetadata *metadata);

    /**
     * Converts given surface canvas from CGU media type in palette colorful mode.
     * back to the original, but with the some loss of quality.
     *
     * @param inputStream - given input stream.
     * @param metadata - given file metadata.
     * @return result operation status code.
     */
    static SDL_Surface *convertFromCGUPaletteColorful(std::ifstream &inputStream, IO::FileMetadata *metadata);

    /**
     * Converts given surface canvas from CGU media type in palette BW mode.
     * back to the original, but with the some loss of quality.
     *
     * @param inputStream - given input stream.
     * @param metadata - given file metadata.
     * @return result operation status code.
     */
    static SDL_Surface *convertFromCGUPaletteBW(std::ifstream &inputStream, IO::FileMetadata *metadata);

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
     * @param indeces - given CGU file color palette.
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
        std::optional<std::vector<Uint32>> indeces,
        std::ofstream &outputStream);
};