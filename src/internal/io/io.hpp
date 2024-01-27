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
    long METADATA_FLAG = -9876543210;

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
        NONE
    };

    /**
     * 
    */
    static bool isRGBConversion(IO::CONVERSION_TYPES value);

    /**
     * 
    */
    static bool isBWConversion(IO::CONVERSION_TYPES value);

    /**
     * Represents general CGU file oriented metadata configuration.
    */
    class FileMetadata {
    private:
        bool compatible = 1;

        size_t defaultSize = 0;

        /**
         * 
        */
        bool optimal;

        IO::CONVERSION_TYPES convertion;

        std::vector<int> indeces;

        std::vector<Uint8> compounds;
    public:
        FileMetadata() {};

        FileMetadata(IO::CONVERSION_TYPES convertion, bool optimal) : convertion(convertion), optimal(optimal) {};
        
        FileMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<int> indeces) : convertion(convertion), optimal(optimal), indeces(indeces) {};

        FileMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<Uint8> compounds) : convertion(convertion), optimal(optimal), compounds(compounds) {};

        /**
         * Retrieves compatibility flag.
         * @return compatibility flag.
        */
        bool getCompatible();

        /**
         * Sets given compatible type.
         * @param value - compatible flag value.
        */
        void setCompatible(bool value);

        /**
         * Retrieves default size flag.
         * @return default size.
        */
        size_t getDefaultSize();

        /**
         * Retrieves optimal flag.
         * @return optimal flag.
        */
        bool getOptimal();

        /**
         * Sets given optimal type.
         * @param value - optimal flag value.
        */
        void setOptimal(bool value);

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
            bool compatible = value.getCompatible();

            ofs << (char*)&compatible;

            // ofs.write((char*)&(compatible), sizeof(bool));

            // ofs << std::endl;
            // ofs << value.getCompatible();
            // ofs << std::endl;
            // ofs << static_cast<int>(value.getConvertion());
            // ofs << std::endl;
            // ofs << value.getOptimal();

            // if (value.getIndeces().size() > 0) {
            //     ofs << std::endl;
            //     ofs << value.getIndeces().size();
            //     ofs << std::endl;
            // } else if (value.getCompounds().size() > 0) {
            //     ofs << std::endl;
            //     ofs << value.getCompounds().size();
            //     ofs << std::endl;
            //     // std::cout << IO::combineCompounds(value.getCompounds()) << std::endl;
            // } else {
            //     ofs << std::endl;
            //     ofs << value.getDefaultSize();
            // }

            

            // for (int index : value.getIndeces()) {
            //     ofs << index;
            //     ofs << std::endl;
            // }

            // for (Uint8 compound : value.getCompounds()) {
            //     ofs << static_cast<int>(compound);
            //     ofs << std::endl;
            // }

            return ofs;
        };

        friend std::ifstream & operator >> (std::ifstream & ifs, IO::FileMetadata & value) {
            bool compatible;
            // int convertion;
            // bool optimal;
            // size_t data_size;
            // int index;
            // std::vector<int> indeces;
            // Uint8 compound;
            // std::vector<Uint8> compounds;
            ifs.read((char*)&compatible, sizeof(bool));

            // ifs >> (char *)&compatible;

            // ifs >> compatible;
            // ifs >> convertion;
            // ifs >> optimal;
            // ifs >> data_size;

            // if (data_size > 0) {
            //     std::cout << compatible << std::endl;
            //     std::cout << convertion << std::endl;
            //     std::cout << data_size << std::endl;
            // }

            std::cout << compatible << std::endl;
            // std::cout << convertion << std::endl;
            // std::cout << optimal << std::endl;
            // std::cout << data_size << std::endl;

            value.setCompatible(compatible);
            // value.setConvertion((IO::CONVERSION_TYPES)convertion);

            return ifs;
        };
    };

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param optimal - given CGU file writer mode.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata* composeNativeMetadata(IO::CONVERSION_TYPES convertion, bool optimal);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param optimal - given CGU file writer mode.
     * @param indeces - given CGU file indeces.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata* composeIndecesMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<int> indeces);

    /**
     * Composes CGU file metadata struct with the given arguments.
     * @param convertion - given CGU file convertion type.
     * @param optimal - given CGU file writer mode.
     * @param compounds - given CGU file compounds.
     * @return composed CGU file metadata.
    */
    static IO::FileMetadata* composeCompoundsMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<Uint8> compounds);

    /**
     * 
    */
    static std::string combineIndeces(std::vector<int> indeces);

    /**
     * 
    */
    static std::string combineCompounds(std::vector<Uint8> compounds);

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
     * Reads media CGU file with the given path in the default way.
     * into managable surface canvas.
     * @param path - a location of the file to be read.
     * @return managable surface canvas.
    */
    static SDL_Surface* readFileCGUDefault(std::string path);

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

    /**
     * Writes media CGU file to the given path in the default way.
     * from the managable surface canvas.
     * @param path - a location of the file to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileCGUDefault(std::string path, FileMetadata* metadata, SDL_Surface* surface);

    /**
     * 
    */
    static int writeFileCGUOptimalRGB(std::string path, SDL_Surface* surface);

    /**
     * 
    */
    static int writeFileCGUOptimalBW(std::string path, SDL_Surface* surface);

    /**
     * Writes media CGU file to the given path in the optimal way.
     * from the managable surface canvas.
     * @param path - a location of the file to be written to.
     * @param surface - a modified managable surface canvas.
     * @return operation result code.
    */
    static int writeFileCGUOptimal(std::string path, FileMetadata* metadata, SDL_Surface* surface);

    /**
     * Reads metadata from the given CGU file.
     * @param path - a location of the file to be read.
     * @return CGU file metadata.
    */
    static IO::FileMetadata* readMetadataFromFileCGU(std::string path);
private:
    /**
     * Writes given metadata to the CGU file at the given location.
     * @param path - a location of the file, where metadata is intended to be set.
     * @param metadata - CGU file metadata.
     * @return status of the operation.
    */
    static int writeMetadataToFileCGU(std::string path, IO::FileMetadata* metadata);
};