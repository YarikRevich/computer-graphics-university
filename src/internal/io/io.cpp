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
    }

    return IO::CONVERSION_TYPES::NONE;
}

IO::FileMetadata::FileMetadata(std::ifstream& inputStream) {
    uint8_t compatibleTemp;
    uint8_t conversion;
    uint16_t width;
    uint16_t height;
    uint8_t dithering;
    int indecesSize;

    inputStream.read((char*)&compatibleTemp, sizeof(uint8_t));
    inputStream.read((char*)&conversion, sizeof(uint8_t));
    inputStream.read((char*)&width, sizeof(uint16_t));
    inputStream.read((char*)&height, sizeof(uint16_t));
    inputStream.read((char*)&dithering, sizeof(uint8_t));
    inputStream.read((char*)&indecesSize, sizeof(int));

    std::vector<Uint8> indeces(indecesSize, 0);
    inputStream.read((char*)(indeces.data()), indecesSize * sizeof(Uint8));

    setCompatible(compatibleTemp);
    setConvertion((IO::CONVERSION_TYPES)conversion);
    setWidth(width);
    setHeight(height);
    setDithering(dithering);
    setIndecesSize(indecesSize);
    setIndeces(indeces);
};

uint8_t IO::FileMetadata::getCompatible() {
    return compatible;
}

void IO::FileMetadata::setCompatible(uint8_t value) {
    this->compatible = value;
};

IO::CONVERSION_TYPES IO::FileMetadata::getConvertion() {
    return convertion;
}

void IO::FileMetadata::setConvertion(IO::CONVERSION_TYPES value) {
    this->convertion = value;
};

uint16_t IO::FileMetadata::getWidth() {
    return width;
};

void IO::FileMetadata::setWidth(uint16_t value) {
    this->width = value;
};

uint16_t IO::FileMetadata::getHeight() {
    return height;
};

void IO::FileMetadata::setHeight(uint16_t value) {
    this->height = value;
};


uint8_t IO::FileMetadata::getDithering() {
    return dithering;
}

void IO::FileMetadata::setDithering(uint8_t value) {
    this->dithering = value;
};

int IO::FileMetadata::getIndecesSize() {
    return indecesSize;
};

void IO::FileMetadata::setIndecesSize(int value) {
    this->indecesSize = value;
};

std::vector<Uint8> IO::FileMetadata::getIndeces() {
    return indeces;
};

void IO::FileMetadata::setIndeces(std::vector<Uint8> value) {
    this->indeces = value;
};

void IO::FileMetadata::writeTo(std::ofstream& ofs) {
    uint8_t compatibleTemp = getCompatible();
    uint8_t conversion = (uint8_t)getConvertion();
    uint16_t width = getWidth();
    uint16_t height = getHeight();
    uint8_t dithering = getDithering();
    int indecesSize = getIndecesSize();
    std::vector<Uint8> indeces = getIndeces();

    ofs.write((char*)&compatibleTemp, sizeof(uint8_t));
    ofs.write((char*)&conversion, sizeof(uint8_t));
    ofs.write((char*)&width, sizeof(uint16_t));
    ofs.write((char*)&height, sizeof(uint16_t));
    ofs.write((char*)&dithering, sizeof(uint8_t));
    ofs.write((char*)&indecesSize, sizeof(int));
    ofs.write((char*)(indeces.data()), indecesSize * sizeof(Uint8));
};

int IO::FileMetadata::getSize() {
    return sizeof(int) + (sizeof(uint16_t) * 2) + (sizeof(uint8_t) * 3) + (indeces.size() * sizeof(Uint8));
};

IO::FileMetadata* IO::composeNativeMetadata(IO::CONVERSION_TYPES convertion, uint8_t dithering, uint16_t width, uint16_t height) {
    return new IO::FileMetadata(convertion, dithering, width, height);
}

IO::FileMetadata* IO::composeIndecesMetadata(IO::CONVERSION_TYPES convertion, uint8_t dithering, uint16_t width, uint16_t height, std::vector<Uint8> indeces) {
    return new IO::FileMetadata(convertion, dithering, width, height, indeces);
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

int IO::writeFileJPEG(std::string path, SDL_Surface* surface){
    return IMG_SaveJPG(surface, path.c_str(), 66);  
};

int IO::writeFilePNG(std::string path, SDL_Surface* surface){
    return IMG_SavePNG(surface, path.c_str());  
};

int IO::writeFileBMP(std::string path, SDL_Surface* surface) {
    return SDL_SaveBMP(surface, path.c_str());
};
