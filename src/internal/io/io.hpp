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
     * Represents all compression types available to be chosen.
     */
    enum class COMPRESSION_TYPES
    {
        DCT,
        BYTE_RUN,
        RLE,
        LZW,
        LZ77,
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

        IO::COMPRESSION_TYPES compression;

        IO::FILTER_TYPES filter;

        uint8_t dithering;

        uint16_t height;

        uint16_t width;

        int indecesSize = 0;

        std::vector<Uint32> indeces;

    public:
        static const uint8_t COMPATIBLE_FLAG = 13;

        static const uint8_t DITHERING_FLAG = 14;

        FileMetadata(std::ifstream &inputStream);

        FileMetadata(
            IO::CONVERSION_TYPES convertion, 
            IO::BIT_TYPES bit, 
            IO::MODEL_TYPES model, 
            IO::COMPRESSION_TYPES compression, 
            uint8_t dithering, 
            uint16_t width, 
            uint16_t height) : 
            compatible(COMPATIBLE_FLAG), 
            convertion(convertion), 
            bit(bit),
            model(model),
            compression(compression),
            dithering(dithering), 
            width(width), 
            height(height) {};

        FileMetadata(
            IO::CONVERSION_TYPES convertion, 
            IO::BIT_TYPES bit, 
            IO::MODEL_TYPES model, 
            IO::COMPRESSION_TYPES compression,
            IO::FILTER_TYPES filter,
            uint8_t dithering, 
            uint16_t width, 
            uint16_t height, 
            std::vector<Uint32> indeces) : 
            compatible(COMPATIBLE_FLAG), 
            convertion(convertion), 
            bit(bit),
            model(model),
            compression(compression),
            filter(filter),
            dithering(dithering), 
            width(width), 
            height(height), 
            indecesSize(indeces.size()), 
            indeces(indeces) {};

        /**
         * Retrieves compatibility flag.
         *
         * @return compatibility flag.
         */
        uint8_t getCompatible();

        /**
         * Sets given compatible type.
         *
         * @param value - compatible flag value.
         */
        void setCompatible(uint8_t value);

        /**
         * Retrieves CGU file convertion type.
         *
         * @return CGU file convertion type.
         */
        IO::CONVERSION_TYPES getConvertion();

        /**
         * Sets given convertion type.
         *
         * @param value - given CGU file convertion type.
         */
        void setConvertion(IO::CONVERSION_TYPES value);

        /**
         * Retrieves CGU file bit type.
         *
         * @return CGU file bit type.
         */
        IO::BIT_TYPES getBit();

        /**
         * Sets given bit type.
         *
         * @param value - given CGU file bit type.
         */
        void setBit(IO::BIT_TYPES value);

        /**
         * Retrieves CGU file model type.
         *
         * @return CGU file model type.
         */
        IO::MODEL_TYPES getModel();

        /**
         * Sets given model type.
         *
         * @param value - given CGU file model type.
         */
        void setModel(IO::MODEL_TYPES value);

        /**
         * Retrieves CGU file compression type.
         *
         * @return CGU file compression type.
         */
        IO::COMPRESSION_TYPES getCompression();

        /**
         * Sets given filter type.
         *
         * @param value - given CGU file filter type.
         */
        void setFilter(IO::FILTER_TYPES value);

        /**
         * Retrieves CGU file filter type.
         *
         * @return CGU file filter type.
         */
        IO::FILTER_TYPES getFilter();

        /**
         * Sets given compression type.
         *
         * @param value - given CGU file compression type.
         */
        void setCompression(IO::COMPRESSION_TYPES value);

        /**
         * Retrives status of the dithering mode.
         * 
         * @return retrieved status of the dithering mode.
         */
        uint8_t getDithering();

        /**
         * Sets dithering mode.
         * 
         * @param value - dithering mode to be set.
         */
        void setDithering(uint8_t value);

        /**
         * Retrieves CGU file width.
         * 
         * @return CGU file width.
         */
        uint16_t getWidth();

        /**
         * Sets width of the image.
         * 
         * @param value - given image width.
         */
        void setWidth(uint16_t value);

        /**
         * Retrieves CGU file height.
         * 
         * @return CGU file height.
         */
        uint16_t getHeight();

        /**
         * Sets height of the image.
         * 
         * @param value - given image height.
         */
        void setHeight(uint16_t value);

        /**
         * Retrieves indeces array size.
         * 
         * @return retrieved indeces array size.
         */
        int getIndecesSize();

        /**
         * Sets indeces array size.
         * 
         * @param value - given indeces array size.
         */
        void setIndecesSize(int value);

        /**
         * Retrieves indeces for the image convertion.
         * 
         * @return CGU file indeces.
         */
        std::vector<Uint32> getIndeces();

        /**
         * Sets indeces for the image convertion.
         * 
         * @param indexes - given CGU file convertion indeces.
         */
        void setIndeces(std::vector<Uint32> value);

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
     * Composes CGU file metadata struct with the given arguments.
     * 
     * @param convertion - given CGU file convertion type.
     * @param bit - given CGU file bit type.
     * @param model - given CGU file model type.
     * @param compression - given CGU file compression type.
     * @param dithering - CGU file dithering mode switch.
     * @param width - given CGU file width.
     * @param height - given CGU file height.
     * @param indeces - given CGU file optional color palette.
     * @return composed CGU file metadata.
     */
    static IO::FileMetadata *composeMetadata(
        IO::CONVERSION_TYPES convertion, 
        IO::BIT_TYPES bit, 
        IO::MODEL_TYPES model, 
        IO::COMPRESSION_TYPES compression, 
        uint8_t dithering, 
        uint16_t width, 
        uint16_t height,
        std::optional<std::vector<Uint32>> indeces);

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
     * Converts given compression type to enum representation.
     * 
     * @param src - given compression type.
     * @return enum representation of a compression type.
     */
    static IO::COMPRESSION_TYPES getCompressionType(std::string src);

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