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
class IO {
public:
    /**
     * Represents all image formats available
     * to be processed.
    */
    enum class FILE_TYPES {
        JPG,
        PNG,
        BMP,
        NONE
    };

    /**
     * Represents all convertion approach types 
     * available to be chosen.
    */
    enum class CONVERSION_TYPES {
        NATIVE_RGB,
        NATIVE_BW,
        PALETTE_RGB,
        PALETTE_BW,
        NONE
    };

    /**
     * Represents general CGU file oriented metadata configuration.
    */
    class FileMetadata {
    private:
        uint16_t compatible;

        IO::CONVERSION_TYPES convertion;

        int dithering;

        int height;

        int width;

        int indecesSize = 0;

        std::vector<Uint32> indeces;
    public:
        static const int COMPATIBLE_FLAG = 13;

        static const int DITHERING_FLAG = 14;

        FileMetadata(std::ifstream& inputStream);

        FileMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height) : compatible(COMPATIBLE_FLAG), convertion(convertion), dithering(dithering), width(width), height(height) {};
        
        FileMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height, std::vector<Uint32> indeces) : compatible(COMPATIBLE_FLAG), convertion(convertion), dithering(dithering), width(width), height(height), indecesSize(indeces.size()), indeces(indeces) {};

        /**
         * Retrieves compatibility flag.
         * @return compatibility flag.
        */
        uint16_t getCompatible();

        /**
         * Sets given compatible type.
         * @param value - compatible flag value.
        */
        void setCompatible(uint16_t value);

        /**
         * Retrieves CGU file convertion type.
         * @return CGU file convertion type.
        */
        IO::CONVERSION_TYPES getConvertion();

        /**
         * Sets given convertion type.
         * @param value - given CGU file convertion type.
        */
        void setConvertion(IO::CONVERSION_TYPES value);

        /**
         * Retrives status of the dithering mode.
         * @return retrieved status of the dithering mode.
        */
        int getDithering();

        /**
         * Sets dithering mode.
         * @param value - dithering mode to be set.
        */
        void setDithering(int value);

        /**
         * Retrieves CGU file width.
         * @return CGU file width.
        */
        int getWidth();

        /**
         * Sets width of the image.
         * @param value - given image width.
        */
        void setWidth(int value);

        /**
         * Retrieves CGU file height.
         * @return CGU file height.
        */
        int getHeight();

        /**
         * Sets height of the image.
         * @param value - given image height.
        */
        void setHeight(int value);

        /** 
         * Retrieves indeces array size.
         * @return retrieved indeces array size.
        */
        int getIndecesSize();

        /** 
         * Sets indeces array size.
         * @param value - given indeces array size.
        */
        void setIndecesSize(int value);

        /**
         * Retrieves indeces for the image convertion.
         * @return CGU file indeces.
        */
        std::vector<Uint32> getIndeces();

        /**
         * Sets indeces for the image convertion.
         * @param indexes - given CGU file convertion indeces.
        */
        void setIndeces(std::vector<Uint32> value);

        /**
         * Writes metadata content to the given stream.
         * @param ofs - given output stream.
        */
        void writeTo(std::ofstream& ofs);

        /**
         * Retrieves metadata size.
         * @return retrieved metadata size.
        */
        int getSize();
    };

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param optimal - given CGU file writer mode.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata* composeNativeMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param optimal - given CGU file writer mode.
     * @param indeces - given CGU file indeces.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata* composeIndecesMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height, std::vector<Uint32> indeces);

    /**
     * Converts given file type to enum representation.
     * @param src - given file type.
     * @return enum representation of a file type.
    */
    static IO::FILE_TYPES getFileType(std::string src);

    /**
     * Converts given conversion type to enum representation.
     * @param src - given conversion type.
     * @return enum representation of a conversion type.
    */
    static IO::CONVERSION_TYPES getConversionType(std::string src);

    /**
     * Reads media JPEG file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileJPEG(std::string path);

    /**
     * Reads media PNG file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFilePNG(std::string path);

    /**
     * Reads media BMP file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileBMP(std::string path);

    /**
     * Writes media JPEG file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileJPEG(std::string path, SDL_Surface* surface);

    /**
     * Writes media PNG file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFilePNG(std::string path, SDL_Surface* surface);

    /**
     * Writes media BMP file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileBMP(std::string path, SDL_Surface* surface);
};