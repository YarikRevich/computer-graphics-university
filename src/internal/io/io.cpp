#include "io.hpp"
#include <iostream>

IO::FILE_TYPES IO::getFileType(std::string src){
    if (src == "jpg" || src == "jpeg") {
        return IO::FILE_TYPES::JPG;
    } else if (src == "png") {
        return IO::FILE_TYPES::PNG;
    } else if (src == "bmp") {
        return IO::FILE_TYPES::BMP;
    }

    return IO::FILE_TYPES::NONE;
}

IO::CONVERSION_TYPES IO::getConversionType(std::string src){
    if (src == "native_rgb") {
        return IO::CONVERSION_TYPES::NATIVE_RGB;
    } else if (src == "native_bw") {
        return IO::CONVERSION_TYPES::NATIVE_BW;
    } else if (src == "palette_rgb") {
        return IO::CONVERSION_TYPES::PALETTE_RGB;
    } else if (src == "palette_bw") {
        return IO::CONVERSION_TYPES::PALETTE_BW;
    } else if (src == "palette_detected") {
        return IO::CONVERSION_TYPES::PALETTE_DETECTED;
    }

    return IO::CONVERSION_TYPES::NONE;
}

uint16_t IO::FileMetadata::getCompatible() {
    return compatible;
}

void IO::FileMetadata::setCompatible(uint16_t value) {
    this->compatible = value;
};

IO::CONVERSION_TYPES IO::FileMetadata::getConvertion() {
    return convertion;
}

void IO::FileMetadata::setConvertion(IO::CONVERSION_TYPES value) {
    this->convertion = value;
};

std::vector<int> IO::FileMetadata::getIndexes() {
    return indexes;
};

void IO::FileMetadata::setIndexes(std::vector<int> indexes) {
    this->indexes = indexes;
};

IO::FileMetadata composeNativeMetadata(IO::CONVERSION_TYPES convertion) {
    IO::FileMetadata result(convertion);

    return result;
}

IO::FileMetadata composePaletteMetadata(IO::CONVERSION_TYPES convertion, std::vector<int> indexes) {
    IO::FileMetadata result(convertion, indexes);

    return result;
}

SDL_Surface* IO::readFileJPEG(std::string path) {
    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG){
        return NULL;
    }

    return IMG_Load(path.c_str());
}

SDL_Surface* IO::readFilePNG(std::string path) {
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        return NULL;
    }

    return IMG_Load(path.c_str());
};

SDL_Surface* IO::readFileBMP(std::string path) {
    return SDL_LoadBMP(path.c_str());
}

SDL_Surface* IO::readFileCGU(std::string path) {
    return IMG_Load(path.c_str());
};

int IO::writeFileJPEG(std::string path, SDL_Surface* surface){
    return IMG_SaveJPG(surface, path.c_str(), 66);  
};

int IO::writeFilePNG(std::string path, SDL_Surface* surface){
    return IMG_SavePNG(surface, path.c_str());  
};

int IO::writeFileBMP(std::string path, SDL_Surface* surface) {
    return SDL_SaveBMP(surface, path.c_str());
};

int IO::writeFileCGU(std::string path, IO::FileMetadata metadata, SDL_Surface* surface){
    if (SDL_SaveBMP(surface, path.c_str()) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    writeMetadataToFileCGU(path, metadata);

    return EXIT_SUCCESS;
};

bool IO::isFileCGUCompatible(std::string path) {
    IO::FileMetadata metadata = readMetadataFromFileCGU(path);

    return metadata.getCompatible() == 1;
}

void IO::writeMetadataToFileCGU(std::string path, IO::FileMetadata metadata) {
    std::ofstream file(path, std::ios_base::app);

    file.seekp(0, std::ios::end);
    file << metadata;

    file.close();
}

IO::FileMetadata IO::readMetadataFromFileCGU(std::string path) {
    IO::FileMetadata result;

    std::ifstream file(path);

    file.seekg(((int)file.tellg()) - (METADATA_FIELDS_NUM + 1), std::ios::end);
    file >> result;

    file.close();

    return result;
}