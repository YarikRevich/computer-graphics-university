#pragma once

#include <string>
#include <iostream>
#include <fstream>
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
        NATIVE_RGB_DITHERING,
        NATIVE_BW_DITHERING,
        PALETTE_RGB,
        PALETTE_BW,
        PALETTE_DETECTED,
        NONE
    };

    /**
     * Represents general CGU file oriented metadata configuration.
    */
    class FileMetadata {
    private:
        uint16_t compatible;

        IO::CONVERSION_TYPES convertion;

        std::vector<int> indeces;

        std::vector<Uint8> compounds;
    public:
        FileMetadata() {};

        FileMetadata(IO::CONVERSION_TYPES convertion) : compatible(1), convertion(convertion) {};
        
        FileMetadata(IO::CONVERSION_TYPES convertion, std::vector<int> indeces) : compatible(1), convertion(convertion), indeces(indeces) {};

        FileMetadata(IO::CONVERSION_TYPES convertion, std::vector<Uint8> compounds) : compatible(1), convertion(convertion), compounds(compounds) {};

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
         * Retrieves indeces for the image convertion.
         * @return CGU file indeces.
        */
        std::vector<int> getIndeces();

        /**
         * Sets indeces for the image convertion.
         * @param indexes - given CGU file convertion indeces.
        */
        void setIndeces(std::vector<int> indeces);

        /**
         * Retrieves compounds for the image convertion.
         * @return CGU file compounds.
        */
        std::vector<Uint8> getCompounds();

        /**
         * Sets compounds for the image convertion.
         * @param indexes - given CGU file convertion compounds.
        */
        void setCompounds(std::vector<Uint8> compounds);

        friend std::ofstream & operator << (std::ofstream & ofs, IO::FileMetadata value) {
            ofs << std::endl;
            ofs << value.getCompatible();
            ofs << std::endl;
            ofs << static_cast<int>(value.getConvertion());

            if (value.getIndeces().size() > 0) {
                ofs << std::endl;
                ofs << value.getIndeces().size();
                ofs << std::endl;
            } else if (value.getCompounds().size() > 0) {
                ofs << std::endl;
                ofs << value.getCompounds().size();
                ofs << std::endl;
            }

            for (int index : value.getIndeces()) {
                ofs << index;
                ofs << std::endl;
            }

            for (Uint8 compound : value.getCompounds()) {
                ofs << static_cast<int>(compound);
                ofs << std::endl;
            }

            return ofs;
        };

        friend std::ifstream & operator >> (std::ifstream & ifs, IO::FileMetadata & value) {
            uint16_t compatible;
            int convertion;
            size_t data_size;
            int index;
            std::vector<int> indeces;
            Uint8 compound;
            std::vector<Uint8> compounds;

            ifs >> compatible;
            ifs >> convertion;
            ifs >> data_size;

            if (data_size > 0) {
                std::cout << data_size << std::endl;
            }

            value.setCompatible(compatible);
            value.setConvertion((IO::CONVERSION_TYPES)convertion);

            return ifs;
        };
    };

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata composeNativeMetadata(IO::CONVERSION_TYPES convertion);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param indeces - given CGU file indeces.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata composeIndecesMetadata(IO::CONVERSION_TYPES convertion, std::vector<int> indeces);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param compounds - given CGU file compounds.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata composeCompoundsMetadata(IO::CONVERSION_TYPES convertion, std::vector<Uint8> compounds);

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
     * @param path - a location of the file to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileCGU(std::string path, FileMetadata metadata, SDL_Surface* surface);

    /**
     * Checks if the file at the given location is CGU compatible.
     * @param path - a location of the file to be scanned.
     * @return CGU compatibility check.
    */
    static bool isFileCGUCompatible(std::string path);

    /**
     * Reads metadata from the given CGU file.
     * @param path - a location of the file to be read.
     * @return CGU file metadata.
    */
    static IO::FileMetadata readMetadataFromFileCGU(std::string path);
private:
    /**
     * Writes given metadata to the CGU file at the given location.
     * @param path - a location of the file, where metadata is intended to be set.
     * @param metadata - CGU file metadata.
    */
    static void writeMetadataToFileCGU(std::string path, IO::FileMetadata metadata);
};