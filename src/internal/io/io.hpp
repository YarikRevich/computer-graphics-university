#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "../processor/processor.hpp"

/**
 * Contains operations used for IO management.
 */
class IO
{
public:
    /**
     * Represents all image formats available to be processed.
     */
    enum class FILE_TYPES
    {
        JPG,
        PNG,
        BMP,
        NONE
    };

    /**
     * Represents all convertion approach types available to be chosen.
     */
    enum class CONVERSION_TYPES
    {
        NATIVE_COLORFUL,
        NATIVE_BW,
        PALETTE_COLORFUL,
        PALETTE_BW,
        NONE
    };

    /**
     * Represents all bit types available to be chosen.
     */
    enum class BIT_TYPES
    {
        TWENTY_FOUR,
        SIXTEEN,
        FIFTEEN,
        SEVEN,
        NONE
    };

    /**
     * Represents all color model types available to be chosen.
     */
    enum class MODEL_TYPES
    {
        RGB,
        YUV,
        YIQ,
        YCBCR,
        HSL,
        NONE
    };

    /**
     * Represents all lossless compression types available to be chosen.
     */
    enum class LOSSLESS_COMPRESSION_TYPES
    {
        BYTE_RUN,
        RLE,
        LZW,
        LZ77,
        NONE
    };

    /**
     * Represents all lossy compression types available to be chosen.
     */
    enum class LOSSY_COMPRESSION_TYPES
    {
        DCT,
        NONE
    };

    /**
     * Represents all sampling types available to be chosen.
     */
    enum class SAMPLING_TYPES
    {
        FOUR_TWO_ONE,
        NONE
    };

    /**
     * Represents all filter types available to be chosen.
     */
    enum class FILTER_TYPES
    {
        DIFFERENTIAL,
        LINE_DIFFERENCE,
        AVERAGE,
        PAETH,
        NONE
    };

    /**
     * Represents general CGU file oriented metadata configuration.
     */
    class FileMetadata
    {
    private:
        uint8_t compatible;

        IO::CONVERSION_TYPES convertion;

        IO::BIT_TYPES bit;

        IO::MODEL_TYPES model;

        IO::LOSSLESS_COMPRESSION_TYPES losslessCompression;

        int losslessCompressionSize;

        IO::LOSSY_COMPRESSION_TYPES lossyCompression;

        IO::SAMPLING_TYPES sampling;

        IO::FILTER_TYPES filter;

        uint8_t dithering;

        uint16_t height;

        uint16_t width;

        int indecesSize = 0;

    public:
        static const uint8_t COMPATIBLE_FLAG = 15;

        static const uint8_t DITHERING_FLAG = 14;

        FileMetadata(
            uint8_t compatible,
            IO::CONVERSION_TYPES convertion, 
            IO::BIT_TYPES bit, 
            IO::MODEL_TYPES model, 
            IO::LOSSLESS_COMPRESSION_TYPES losslessCompression,
            int losslessCompressionSize,
            IO::LOSSY_COMPRESSION_TYPES lossyCompression,
            IO::SAMPLING_TYPES sampling,
            IO::FILTER_TYPES filter,
            uint16_t width, 
            uint16_t height, 
            uint8_t dithering, 
            int indecesSize) : 
            compatible(compatible), 
            convertion(convertion), 
            bit(bit),
            model(model),
            losslessCompression(losslessCompression),
            losslessCompressionSize(losslessCompressionSize),
            lossyCompression(lossyCompression),
            sampling(sampling),
            filter(filter),
            width(width), 
            height(height), 
            dithering(dithering), 
            indecesSize(indecesSize) {};

        /**
         * Retrieves compatibility flag.
         *
         * @return compatibility flag.
         */
        uint8_t getCompatible();

        /**
         * Retrieves CGU file convertion type.
         *
         * @return CGU file convertion type.
         */
        IO::CONVERSION_TYPES getConvertion();

        /**
         * Retrieves CGU file bit type.
         *
         * @return CGU file bit type.
         */
        IO::BIT_TYPES getBit();

        /**
         * Retrieves CGU file model type.
         *
         * @return CGU file model type.
         */
        IO::MODEL_TYPES getModel();

        /**
         * Retrieves CGU file lossless compression type.
         *
         * @return CGU file lossless compression type.
         */
        IO::LOSSLESS_COMPRESSION_TYPES getLosslessCompression();

         /**
         * Retrieves CGU file lossless compression size.
         *
         * @return CGU file lossless compression size.
         */
        int getLosslessCompressionSize();

        /**
         * Retrieves CGU file lossy compression type.
         *
         * @return CGU file lossy compression type.
         */
        IO::LOSSY_COMPRESSION_TYPES getLossyCompression();

        /**
         * Retrieves CGU file sampling type.
         *
         * @return CGU file sampling type.
         */
        IO::SAMPLING_TYPES getSampling();

        /**
         * Retrieves CGU file filter type.
         *
         * @return CGU file filter type.
         */
        IO::FILTER_TYPES getFilter();

        /**
         * Retrives status of the dithering mode.
         * 
         * @return retrieved status of the dithering mode.
         */
        uint8_t getDithering();

        /**
         * Retrieves CGU file width.
         * 
         * @return CGU file width.
         */
        uint16_t getWidth();

        /**
         * Retrieves CGU file height.
         * 
         * @return CGU file height.
         */
        uint16_t getHeight();

        /**
         * Retrieves indeces array size.
         * 
         * @return retrieved indeces array size.
         */
        int getIndecesSize();

        /**
         * Writes metadata content to the given stream.
         * 
         * @param ofs - given output stream.
         */
        void writeTo(std::ofstream &ofs);

        /**
         * Retrieves metadata size.
         * 
         * @return retrieved metadata size.
         */
        int getSize();
    };

    /**
     * Reads CGU metadata file.
     * 
     * @param inputStream - given CGU metadata input stream.
     * @return read CGU common metadata file.
     */
    static IO::FileMetadata *readMetadata(std::ifstream &inputStream);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * 
     * @param convertion - given CGU file convertion type.
     * @param bit - given CGU file bit type.
     * @param model - given CGU file model type.
     * @param losslessCompression - given CGU file lossless compression type.
     * @param losslessCompressionSize - given CGU file lossless compression size.
     * @param lossyCompression - given CGU file lossy compression type.
     * @param filter - given CGU file filter type.
     * @param dithering - CGU file dithering mode switch.
     * @param width - given CGU file width.
     * @param height - given CGU file height.
     * @return composed CGU file metadata.
     */
    static IO::FileMetadata *composeMetadata(
        IO::CONVERSION_TYPES convertion, 
        IO::BIT_TYPES bit, 
        IO::MODEL_TYPES model, 
        IO::LOSSLESS_COMPRESSION_TYPES losslessCompression, 
        int losslessCompressionSize,
        IO::LOSSY_COMPRESSION_TYPES lossyCompression, 
        IO::SAMPLING_TYPES sampling,
        IO::FILTER_TYPES filter,
        uint8_t dithering, 
        uint16_t width, 
        uint16_t height,
        int indecesSize);

    /**
     * Converts given file type to enum representation.
     * 
     * @param src - given file type.
     * @return enum representation of a file type.
     */
    static IO::FILE_TYPES getFileType(std::string src);

    /**
     * Converts given conversion type to enum representation.
     * 
     * @param src - given conversion type.
     * @return enum representation of a conversion type.
     */
    static IO::CONVERSION_TYPES getConversionType(std::string src);

    /**
     * Converts given bit type to enum representation.
     * 
     * @param src - given bit type.
     * @return enum representation of a bit type.
     */
    static IO::BIT_TYPES getBitType(std::string src);

    /**
     * Converts given model type to enum representation.
     * 
     * @param src - given model type.
     * @return enum representation of a model type.
     */
    static IO::MODEL_TYPES getModelType(std::string src);

    /**
     * Converts given lossless compression type to enum representation.
     * 
     * @param src - given lossless compression type.
     * @return enum representation of a lossless compression type.
     */
    static IO::LOSSLESS_COMPRESSION_TYPES getLosslessCompressionType(std::string src);

    /**
     * Converts given lossy compression type to enum representation.
     * 
     * @param src - given lossy compression type.
     * @return enum representation of a lossy compression type.
     */
    static IO::LOSSY_COMPRESSION_TYPES getLossyCompressionType(std::string src);

    /**
     * Converts given sampling type to enum representation.
     * 
     * @param src - given sampling type.
     * @return enum representation of a sampling type.
     */
    static IO::SAMPLING_TYPES getSamplingType(std::string src);

    /**
     * Converts given filter type to enum representation.
     * 
     * @param src - given filter type.
     * @return enum representation of a filter type.
     */
    static IO::FILTER_TYPES getFilterType(std::string src);

    /**
     * Reads media JPEG file with the given path into managable surface canvas.
     * 
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
     */
    static SDL_Surface *readFileJPEG(std::string path);

    /**
     * Reads media PNG file with the given path into managable surface canvas.
     * 
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
     */
    static SDL_Surface *readFilePNG(std::string path);

    /**
     * Reads media BMP file with the given path into managable surface canvas.
     * 
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
     */
    static SDL_Surface *readFileBMP(std::string path);

    /**
     * Writes media JPEG file to the given path from the managable surface canvas.
     * 
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
     */
    static int writeFileJPEG(std::string path, SDL_Surface *surface);

    /**
     * Writes media PNG file to the given path from the managable surface canvas.
     * 
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
     */
    static int writeFilePNG(std::string path, SDL_Surface *surface);

    /**
     * Writes media BMP file to the given path from the managable surface canvas.
     * 
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
     */
    static int writeFileBMP(std::string path, SDL_Surface *surface);
};