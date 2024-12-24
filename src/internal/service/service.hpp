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
     * Converts given surface canvas to CGU media type in native colorful mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUNativeColorful(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in native BW mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUNativeBW(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in native colorful mode including dithering.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUNativeColorfulDithering(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in native BW mode including dithering.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUNativeBWDithering(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette colorful mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUPaletteColorful(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette colorful mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUPaletteColorfulDithering(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette BW mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUPaletteBW(SDL_Surface *surface);

    /**
     * Converts given surface canvas to CGU media type in palette BW mode.
     *
     * @param surface - given surface to be converted.
     * @return result operation status code.
     */
    static int convertToCGUPaletteBWDithering(SDL_Surface *surface);

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

    static SDL_Surface *convertPaletteBWDithering











    static void compressToDCT();

    static void decompressFromDCT();

    /**
     * Composes CGU file metadata struct with the given arguments.
     *
     * @param convertion - given CGU file raw convertion type.
     * @param bit - given CGU file raw bit type.
     * @param model - given CGU file raw model type.
     * @param compression - given CGU file raw compression type.
     * @param dithering - CGU file dithering mode switch.
     * @param width - given CGU file width.
     * @param height - given CGU file height.
     * @param indeces - given CGU file color palette.
     * @param outputStream - given output stream.
     */
    static void composeMetadata(
        std::string conversion,
        std::string bit,
        std::string model,
        std::string compression,
        bool dithering,
        int width,
        int height,
        std::vector<Uint32> indeces,
        std::ofstream &outputStream);
private:
    /**
     * Represents a result 
     */
    class PaletteConversionResult {

    };
};