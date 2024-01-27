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

uint16_t IO::FileMetadata::getCompatible() {
    return compatible;
}

void IO::FileMetadata::setCompatible(uint16_t value) {
    this->compatible = value;
};

size_t IO::FileMetadata::getDefaultSize() {
    return defaultSize;
}

IO::CONVERSION_TYPES IO::FileMetadata::getConvertion() {
    return convertion;
}

void IO::FileMetadata::setConvertion(IO::CONVERSION_TYPES value) {
    this->convertion = value;
};

int IO::FileMetadata::getWidth() {
    return width;
};

void IO::FileMetadata::setWidth(int value) {
    this->width = value;
};

int IO::FileMetadata::getHeight() {
    return height;
};

void IO::FileMetadata::setHeight(int value) {
    this->height = value;
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

void IO::FileMetadata::writeToDefault(std::ofstream& ofs) {
    uint16_t compatibleTemp = getCompatible();

    ofs << (char*)&compatibleTemp;
};

void IO::FileMetadata::writeToOptimal(std::ofstream& ofs) {
    uint16_t compatibleTemp = getCompatible();
    int conversion = (int)getConvertion();
    int width = getWidth();
    int height = getHeight();

    ofs.write((char*)&compatibleTemp, sizeof(uint16_t));
    ofs.write((char*)&conversion, sizeof(int));
    ofs.write((char*)&width, sizeof(int));
    ofs.write((char*)&height, sizeof(int));
};

void IO::FileMetadata::readFromDefault(std::ifstream& ifs) {
    // uint16_t compatibleTemp;
    // int conversion;
    // int width;
    // int height;

    // ifs.read((char*)&compatibleTemp, sizeof(uint16_t));
    // ifs.read((char*)&conversion, sizeof(int));
    // ifs.read((char*)&width, sizeof(int));
    // ifs.read((char*)&height, sizeof(int));

    // setCompatible(compatibleTemp);
    // setConvertion((IO::CONVERSION_TYPES)conversion);
    // setWidth(width);
    // setHeight(height);
};

void IO::FileMetadata::readFromOptimal(std::ifstream& ifs) {
    uint16_t compatibleTemp;
    int conversion;
    int width;
    int height;

    ifs.read((char*)&compatibleTemp, sizeof(uint16_t));
    ifs.read((char*)&conversion, sizeof(int));
    ifs.read((char*)&width, sizeof(int));
    ifs.read((char*)&height, sizeof(int));

    setCompatible(compatibleTemp);
    setConvertion((IO::CONVERSION_TYPES)conversion);
    setWidth(width);
    setHeight(height);
};

int IO::FileMetadata::getSize() {
    return (sizeof(int) * 3) + sizeof(uint16_t) + (indeces.size() * sizeof(int)) + (compounds.size() * sizeof(Uint8));
};

IO::FileMetadata* IO::composeNativeMetadata(IO::CONVERSION_TYPES convertion, int width, int height) {
    return new IO::FileMetadata(convertion, width, height);
}

IO::FileMetadata* IO::composeIndecesMetadata(IO::CONVERSION_TYPES convertion, int width, int height, std::vector<int> indeces) {
    return new IO::FileMetadata(convertion, width, height, indeces);
}

IO::FileMetadata* IO::composeCompoundsMetadata(IO::CONVERSION_TYPES convertion, int width, int height, std::vector<Uint8> compounds) {
    return new IO::FileMetadata(convertion, width, height, compounds);
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

SDL_Surface* IO::readFileCGUOptimalRGB(std::string path, IO::FileMetadata* metadata) {
    std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        return NULL;
    }

    file.seekg(metadata->getSize());

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> input(7, 0);
    for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / 8); i++) {
        file.read((char *)(input.data()), 7 * sizeof(Uint8));

        buff.push_back(input);
    }

    file.close();

    std::vector<SDL_Color> image;

    std::vector<Uint8> assemble(8, 0);
    for (auto &value : buff) {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble) {
            image.push_back(Processor::convert7BitRGBTo24BitRGB(compound));
        }
    }

    SDL_Surface *surface = 
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++) {
        if (y == surface->h) { 
            x += 1;
            y = 0;
        } 

        Processor::setPixel(surface, x, y, image[k]);
        
        if (x == surface->w) {
            y += 1;
            x = 0;            
        } else {
            y += 1;
        }
    }

    return surface;
};

SDL_Surface* IO::readFileCGUOptimalBW(std::string path, IO::FileMetadata* metadata) {
    return NULL;
};

SDL_Surface* IO::readFileCGUOptimal(std::string path, IO::FileMetadata* metadata) {
    if (isRGBConversion(metadata->getConvertion())) {
        return readFileCGUOptimalRGB(path, metadata);

    } else if (isBWConversion(metadata->getConvertion())) {
        return readFileCGUOptimalBW(path, metadata);

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

    if (writeMetadataToFileCGUDefault(path, metadata) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    };

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimalRGB(std::string path, SDL_Surface* surface) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> assemble(8);

    for(int i = 0; i < Processor::getPixelAmount(surface); i+=8) {
        assemble.clear();

        for(int j = 0; j < 8; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    std::ofstream file(path, std::ios_base::app | std::ios_base::binary);

    for (std::vector<Uint8> &value : buff) {        
        file.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    file.close();

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimalBW(std::string path, SDL_Surface* surface) {
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> assemble(8);

    for(int i = 0; i < Processor::getPixelAmount(surface); i+=8) {
        assemble.clear();

        for(int j = 0; j < 8; j++) {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(image[i+j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    std::ofstream file(path, std::ios_base::app | std::ios_base::binary);

    for (std::vector<Uint8> &value : buff) {
        file.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    file.close();

    return EXIT_SUCCESS;
};

int IO::writeFileCGUOptimal(std::string path, IO::FileMetadata* metadata, SDL_Surface* surface){
    if (writeMetadataToFileCGUOptimal(path, metadata) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    };

    if (isRGBConversion(metadata->getConvertion())) {
        return writeFileCGUOptimalRGB(path, surface);

    } else if (isBWConversion(metadata->getConvertion())) {
        return writeFileCGUOptimalBW(path, surface);
    }
    
    return EXIT_FAILURE;
};

int IO::writeMetadataToFileCGUDefault(std::string path, IO::FileMetadata* metadata) {
    std::ofstream file(path, std::ios_base::app | std::ios_base::binary);
    if (!file.is_open()) {
        return EXIT_FAILURE;
    }

    file.seekp(0, std::ios::end);

    metadata->writeToDefault(file);

    file.close();

    return EXIT_SUCCESS;
}

int IO::writeMetadataToFileCGUOptimal(std::string path, IO::FileMetadata* metadata) {
    std::ofstream file(path, std::ios_base::out | std::ios_base::binary);
    if (!file.is_open()) {
        return EXIT_FAILURE;
    }

    metadata->writeToOptimal(file);

    file.close();

    return EXIT_SUCCESS;
}

IO::FileMetadata* IO::readMetadataFromFileCGUDefault(std::string path) {
    IO::FileMetadata* result = new FileMetadata();

    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open()) {
        return NULL;
    }

    file.seekg(((int)file.tellg()) - (METADATA_FIELDS_NUM + 1), std::ios::end);
    
    result->readFromDefault(file);

    file.close();

    return result;
}

IO::FileMetadata* IO::readMetadataFromFileCGUOptimal(std::string path) {
    IO::FileMetadata* result = new FileMetadata();

    std::ifstream file(path, std::ios_base::binary);
    if (!file.is_open()) {
        return NULL;
    }

    result->readFromOptimal(file);

    file.close();

    return result;
}