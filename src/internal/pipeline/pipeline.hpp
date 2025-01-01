#pragma once

#include "../service/service.hpp"
#include "../io/io.hpp"
#include <iostream>

/**
 * Сontains convertion flow for codec operations.
 */
class Pipeline
{
public:
    /**
     * Handles CGU view operation.
     * 
     * @param inputStream - given CGU file input stream.
     * @param debug - given CGU file debug option.
     * @return retrieved surface.
     */
    static SDL_Surface* handleView(std::ifstream &inputStream, bool debug);

    /**
     * Handles CGU decode operation.
     * 
     * @param inputStream - given CGU file input stream.
     * @param debug - given CGU file debug option.
     * @param fileType - given CGU file output type.
     * @param to - given CGU file output destination.
     * @return result of decode operation execution.
    */
    static int handleDecode(
        std::ifstream &inputStream, bool debug, IO::FILE_TYPES fileType, std::string to);

    /**
     * Handles CGU encode operation.
     * 
     * @param input - given CGU file surface.
     * @param conversionType - given CGU file conversion type.
     * @param bitType - given CGU file bit type.
     * @param modelType - given CGU file model type.
     * @param losslessCompressionType - given CGU file lossless compression type.
     * @param lossyCompressionType - given CGU file lossy compression type.
     * @param samplingType - given CGU file sampling type.
     * @param filterType - given CGU file filter type.
     * @param dithering - given CGU file dithering option.
     * @param outputStream - given CGU file output stream.
     * @return result of encode command.
    */
    static int handleEncode(
        SDL_Surface* input, 
        IO::CONVERSION_TYPES conversionType,
        IO::BIT_TYPES bitType,
        IO::MODEL_TYPES modelType,
        IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType,
        IO::LOSSY_COMPRESSION_TYPES lossyCompressionType,
        IO::SAMPLING_TYPES samplingType,
        IO::FILTER_TYPES filterType,
        bool dithering,
        std::ofstream &outputStream);
};