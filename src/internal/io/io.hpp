#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

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
     * Reads a media JPEG file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileJPEG(std::string path);

    /**
     * Reads a media PNG file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFilePNG(std::string path);

    /**
     * Reads a media BMP file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileBMP(std::string path);

    /**
     * Reads a media CGU file with the given path
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileCGU(std::string path);

    /**
     * Writes a media JPEG file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileJPEG(std::string path, SDL_Surface* surface);

    /**
     * Writes a media PNG file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFilePNG(std::string path, SDL_Surface* surface);

    /**
     * Writes a media BMP file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileBMP(std::string path, SDL_Surface* surface);

    /**
     * Writes a media CGU file to the given path
     * from the managable surface canvas.
     * @param path - a location of the fle to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileCGU(std::string path, SDL_Surface* surface);
};