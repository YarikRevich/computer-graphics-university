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
    } else if (src == "native_rgb_dithering") {
        return IO::CONVERSION_TYPES::NATIVE_RGB_DITHERING;
    } else if (src == "native_bw_dithering") {
        return IO::CONVERSION_TYPES::NATIVE_BW_DITHERING;
    } else if (src == "palette_rgb") {
        return IO::CONVERSION_TYPES::PALETTE_RGB;
    } else if (src == "palette_bw") {
        return IO::CONVERSION_TYPES::PALETTE_BW;
    }

    return IO::CONVERSION_TYPES::NONE;
}

bool IO::isRGBConversion(IO::CONVERSION_TYPES value) {
    return value == IO::CONVERSION_TYPES::NATIVE_RGB ||
        value == IO::CONVERSION_TYPES::NATIVE_RGB_DITHERING ||
        value == IO::CONVERSION_TYPES::PALETTE_RGB;
}

bool IO::isBWConversion(IO::CONVERSION_TYPES value) {
    return value == IO::CONVERSION_TYPES::NATIVE_BW ||
        value == IO::CONVERSION_TYPES::NATIVE_BW_DITHERING ||
        value == IO::CONVERSION_TYPES::PALETTE_BW;
}

bool IO::FileMetadata::getCompatible() {
    return compatible;
}

void IO::FileMetadata::setCompatible(bool value) {
    this->compatible = value;
};

size_t IO::FileMetadata::getDefaultSize() {
    return defaultSize;
}

bool IO::FileMetadata::getOptimal() {
    return compatible;
}

void IO::FileMetadata::setOptimal(bool value) {
    this->compatible = value;
};

IO::CONVERSION_TYPES IO::FileMetadata::getConvertion() {
    return convertion;
}

void IO::FileMetadata::setConvertion(IO::CONVERSION_TYPES value) {
    this->convertion = value;
};

std::vector<int> IO::FileMetadata::getIndeces() {
    return indeces;
};

void IO::FileMetadata::setIndeces(std::vector<int> indeces) {
    this->indeces = indeces;
};

std::vector<Uint8> IO::FileMetadata::getCompounds() {
    return compounds;
};

void IO::FileMetadata::setCompounds(std::vector<Uint8> compounds) {
    this->compounds = compounds;
};

IO::FileMetadata* IO::composeNativeMetadata(IO::CONVERSION_TYPES convertion, bool optimal) {
    return new IO::FileMetadata(convertion, optimal);
}

IO::FileMetadata* IO::composeIndecesMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<int> indeces) {
    return new IO::FileMetadata(convertion, optimal, indeces);
}

IO::FileMetadata* IO::composeCompoundsMetadata(IO::CONVERSION_TYPES convertion, bool optimal, std::vector<Uint8> compounds) {
    return new IO::FileMetadata(convertion, optimal, compounds);
}

std::string IO::combineIndeces(std::vector<int> indeces) {
    std::ostringstream imploded;

    std::copy(indeces.begin(), indeces.end(),
           std::ostream_iterator<int>(imploded, " "));

    return imploded.str();
};

std::string IO::combineCompounds(std::vector<Uint8> compounds) {
    std::ostringstream imploded;




    std::copy(compounds.begin(), compounds.end(),
           std::ostream_iterator<Uint8>(imploded, " "));

    return imploded.str();
};

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

SDL_Surface* IO::readFileCGUDefault(std::string path) {
    return IMG_Load(path.c_str());
};

SDL_Surface* IO::readFileCGUOptimalRGB(std::string path) {
    return NULL;
};

SDL_Surface* IO::readFileCGUOptimalBW(std::string path) {
    return NULL;
};

SDL_Surface* IO::readFileCGUOptimal(std::string path, IO::FileMetadata* metadata) {
    if (isRGBConversion(metadata->getConvertion())) {
        return readFileCGUOptimalRGB(path);

    } else if (isBWConversion(metadata->getConvertion())) {
        return readFileCGUOptimalBW(path);

    }

    return NULL;
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

int IO::writeFileCGUDefault(std::string path, IO::FileMetadata* metadata, SDL_Surface* surface){
    if (SDL_SaveBMP(surface, path.c_str()) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (writeMetadataToFileCGU(path, metadata) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimalRGB(std::string path, SDL_Surface* surface) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> assemble(8);

    for(int i = 0; i < (surface->w*surface->h); i+=8) {
        assemble.clear();

        for(int j = 0; j < 8; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    std::ofstream file(path, std::ios_base::out | std::ios_base::binary);

    for (std::vector<Uint8> &value : buff) {
        file.write((char *)&value[0], value.size() * sizeof(Uint8));
    }

    file.close();

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimalBW(std::string path, SDL_Surface* surface) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> assemble(8);

    for(int i = 0; i < (surface->w*surface->h); i+=8) {
        assemble.clear();

        for(int j = 0; j < 8; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    std::ofstream file(path, std::ios_base::out | std::ios_base::binary);

    for (std::vector<Uint8> &value : buff) {
        file.write((char *)&value[0], value.size() * sizeof(Uint8));
    }

    file.close();

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimal(std::string path, IO::FileMetadata* metadata, SDL_Surface* surface){
    int result;

    if (isRGBConversion(metadata->getConvertion())) {
        result = writeFileCGUOptimalRGB(path, surface);

    } else if (isBWConversion(metadata->getConvertion())) {
        result = writeFileCGUOptimalBW(path, surface);
        
    } else {
        return EXIT_FAILURE;
    }

    if (result != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (writeMetadataToFileCGU(path, metadata) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
};

int IO::writeMetadataToFileCGU(std::string path, IO::FileMetadata* metadata) {
    std::ofstream file(path, std::ios_base::app | std::ios_base::binary);
    if (!file.is_open()) {
        return EXIT_FAILURE;
    }

    file.seekp(0, std::ios::end);
    file << *metadata;

    file.close();

    return EXIT_SUCCESS;
}

IO::FileMetadata* IO::readMetadataFromFileCGU(std::string path) {
    IO::FileMetadata* result = new FileMetadata();

    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open()) {
        return NULL;
    }

    file.seekg(((int)file.tellg()) - (METADATA_FIELDS_NUM + 1), std::ios::end);
    file >> *result;

    file.close();

    return result;
}