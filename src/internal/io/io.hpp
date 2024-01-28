#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "../state/state.hpp"
#include "../processor/processor.hpp"

/**
 * Contains operations used for IO management.
*/
class IO {
public:
    static const long METADATA_FLAG = -9876543210;

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

    // /**
    //  * 
    // */
    // static bool isRGBConversion(IO::CONVERSION_TYPES value);

    // /**
    //  * 
    // */
    // static bool isBWConversion(IO::CONVERSION_TYPES value);

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

        std::vector<Uint8> indeces;
    public:
        static const int COMPATIBLE_FLAG = 13;

        FileMetadata(std::ifstream& inputStream);

        FileMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height) : compatible(COMPATIBLE_FLAG), convertion(convertion), dithering(dithering), width(width), height(height) {};
        
        FileMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height, std::vector<Uint8> indeces) : compatible(COMPATIBLE_FLAG), convertion(convertion), dithering(dithering), width(width), height(height), indecesSize(indeces.size()), indeces(indeces) {};

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
         * 
        */
        int getDithering();

        /**
         * 
        */
        void setDithering(int value);

        /**
         * Retrieves CGU file width.
         * @return CGU file width.
        */
        int getWidth();

        /**
         * 
        */
        void setWidth(int value);

        /**
         * Retrieves CGU file height.
         * @return CGU file height.
        */
        int getHeight();

        /**
         * 
        */
        void setHeight(int value);

        /** 
         * 
        */
        void setIndecesSize(int value);

        /** 
         * 
        */
        int getIndecesSize();

        /**
         * Retrieves indeces for the image convertion.
         * @return CGU file indeces.
        */
        std::vector<Uint8> getIndeces();

        /**
         * Sets indeces for the image convertion.
         * @param indexes - given CGU file convertion indeces.
        */
        void setIndeces(std::vector<Uint8> value);

        /**
         * 
        */
        void writeTo(std::ofstream& ofs);

        /**
         * 
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
    static IO::FileMetadata* composeIndecesMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height, std::vector<Uint8> indeces);

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
     * 
    */
    static SDL_Surface* readFileCGUOptimalRGB(std::string path, IO::FileMetadata* metadata);

    /**
     * 
    */
    static SDL_Surface* readFileCGUOptimalBW(std::string path, IO::FileMetadata* metadata);

    /**
     * Reads media CGU file with the given path in the optimal way.
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @param metadata - CGU file metadata.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileCGUOptimal(std::string path, IO::FileMetadata* metadata);

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

    // /**
    //  * Writes media CGU file to the given path in the default way.
    //  * from the managable surface canvas.
    //  * @param path - a location of the file to be written to.
    //  * @param surface - a modified managable surface canvas.
    //  * @return operation result code.
    // */
    // static int writeFileCGU(std::string path, FileMetadata* metadata, SDL_Surface* surface);

    /**
     * 
    */
    static int writeFileCGUOptimalRGB(std::string path, SDL_Surface* surface);

    /**
     * 
    */
    static int writeFileCGUOptimalBW(std::string path, SDL_Surface* surface);

    // /**
    //  * Writes media CGU file to the given path in the optimal way.
    //  * from the managable surface canvas.
    //  * @param path - a location of the file to be written to.
    //  * @param surface - a modified managable surface canvas.
    //  * @return operation result code.
    // */
    // static int writeFileCGU(std::string path, FileMetadata* metadata, SDL_Surface* surface);

    // /**
    //  * Reads metadata from the given CGU file with optimal mode.
    //  * @param path - a location of the file to be read.
    //  * @return CGU file metadata.
    // */
    // static IO::FileMetadata* readMetadataFromFileCGU(std::string path);


    // /**
    //  * 
    // */
    // static std::ofstream& openWriteFileCGU(std::string path);


    // /**
    //  * 
    // */
    // static std::ifstream& openReadFileCGU(std::string path);


    // static int saveWriteFileCGU(std::ofstream& stream);

    // static int closeReadFileCGU(std::ifstream& stream);
private:
    // /**
    //  * Writes given metadata to the CGU file at the given location with optimal mode.
    //  * @param path - a location of the file, where metadata is intended to be set.
    //  * @param metadata - CGU file metadata.
    //  * @return status of the operation.
    // */
    // static int writeMetadataToFileCGU(std::string path, IO::FileMetadata* metadata);
};