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
    uint16_t compatibleTemp;
    int conversion;
    int width;
    int height;
    int dithering;
    int indecesSize;

    inputStream.read((char*)&compatibleTemp, sizeof(uint16_t));
    inputStream.read((char*)&conversion, sizeof(int));
    inputStream.read((char*)&width, sizeof(int));
    inputStream.read((char*)&height, sizeof(int));
    inputStream.read((char*)&dithering, sizeof(int));
    inputStream.read((char*)&indecesSize, sizeof(int));

    std::vector<Uint32> indeces(indecesSize, 0);
    inputStream.read((char*)(indeces.data()), indecesSize * sizeof(Uint32));

    setCompatible(compatibleTemp);
    setConvertion((IO::CONVERSION_TYPES)conversion);
    setWidth(width);
    setHeight(height);
    setDithering(dithering);
    setIndecesSize(indecesSize);
    setIndeces(indeces);
};

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


int IO::FileMetadata::getDithering() {
    return dithering;
}

void IO::FileMetadata::setDithering(int value) {
    this->dithering = value;
};

int IO::FileMetadata::getIndecesSize() {
    return indecesSize;
};

void IO::FileMetadata::setIndecesSize(int value) {
    this->indecesSize = value;
};

std::vector<Uint32> IO::FileMetadata::getIndeces() {
    return indeces;
};

void IO::FileMetadata::setIndeces(std::vector<Uint32> value) {
    this->indeces = value;
};

void IO::FileMetadata::writeTo(std::ofstream& ofs) {
    uint16_t compatibleTemp = getCompatible();
    int conversion = (int)getConvertion();
    int width = getWidth();
    int height = getHeight();
    int dithering = getDithering();
    int indecesSize = getIndecesSize();
    std::vector<Uint32> indeces = getIndeces();

    ofs.write((char*)&compatibleTemp, sizeof(uint16_t));
    ofs.write((char*)&conversion, sizeof(int));
    ofs.write((char*)&width, sizeof(int));
    ofs.write((char*)&height, sizeof(int));
    ofs.write((char*)&dithering, sizeof(int));
    ofs.write((char*)&indecesSize, sizeof(int));
    ofs.write((char*)(indeces.data()), indecesSize * sizeof(Uint32));
};

int IO::FileMetadata::getSize() {
    return (sizeof(int) * 5) + sizeof(uint16_t) + (indeces.size() * sizeof(Uint32));
};

IO::FileMetadata* IO::composeNativeMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height) {
    return new IO::FileMetadata(convertion, dithering, width, height);
}

IO::FileMetadata* IO::composeIndecesMetadata(IO::CONVERSION_TYPES convertion, int dithering, int width, int height, std::vector<Uint32> indeces) {
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
