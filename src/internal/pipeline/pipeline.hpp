#pragma once

#include "../converter/converter.hpp"
#include "../io/io.hpp"
#include <iostream>

/**
 * Сontains convertion flow for codec operations.
 */
class Pipeline
{
public:
    /**
     * Handles CGU decode operation.
     * 
     * @param inputStream - given CGU file input stream.
     * @param debug - given CGU file debug option.
     * @param fileType - given CGU file output type.
     * @return result of decode operation execution.
    */
    static int handleDecode(std::ifstream &inputStream, bool debug, IO::FILE_TYPES fileType);

    /**
     * Handles CGU encode operation.
     * 
     * @param input - given CGU file surface.
     * @param conversionType - given CGU file conversion type.
     * @param bitType - given CGU file bit type.
     * @param modelType - given CGU file model type.
     * @param compressionType - given CGU file compression type.
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
        IO::COMPRESSION_TYPES compressionType,
        IO::FILTER_TYPES filterType,
        bool dithering,
        std::ofstream &outputStream);
};