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
    if (src == "native_colorful") {
        return IO::CONVERSION_TYPES::NATIVE_COLORFUL;
    } else if (src == "native_bw") {
        return IO::CONVERSION_TYPES::NATIVE_BW;
    } else if (src == "palette_colorful") {
        return IO::CONVERSION_TYPES::PALETTE_COLORFUL;
    } else if (src == "palette_bw") {
        return IO::CONVERSION_TYPES::PALETTE_BW;
    }

    return IO::CONVERSION_TYPES::NONE;
}

IO::BIT_TYPES IO::getBitType(std::string src){
    if (src == "24") {
        return IO::BIT_TYPES::TWENTY_FOUR;
    } else if (src == "16") {
        return IO::BIT_TYPES::SIXTEEN;
    } else if (src == "15") {
        return IO::BIT_TYPES::FIFTEEN;
    } else if (src == "7") {
        return IO::BIT_TYPES::SEVEN;
    }

    return IO::BIT_TYPES::NONE;
}

IO::MODEL_TYPES IO::getModelType(std::string src){
    if (src == "rgb") {
        return IO::MODEL_TYPES::RGB;
    } else if (src == "yuv") {
        return IO::MODEL_TYPES::YUV;
    } else if (src == "yiq") {
        return IO::MODEL_TYPES::YIQ;
    } else if (src == "ycbcr") {
        return IO::MODEL_TYPES::YCBCR;
    } else if (src == "hsl") {
        return IO::MODEL_TYPES::HSL;
    }

    return IO::MODEL_TYPES::NONE;
}

IO::COMPRESSION_TYPES IO::getCompressionType(std::string src){
    if (src == "dct") {
        return IO::COMPRESSION_TYPES::DCT;
    } else if (src == "byterun") {
        return IO::COMPRESSION_TYPES::BYTE_RUN;
    } else if (src == "rle") {
        return IO::COMPRESSION_TYPES::RLE;
    } else if (src == "lzw") {
        return IO::COMPRESSION_TYPES::LZW;
    } else if (src == "lz77") {
        return IO::COMPRESSION_TYPES::LZ77;
    }

    return IO::COMPRESSION_TYPES::NONE;
}

IO::FILTER_TYPES IO::getFilterType(std::string src){
    if (src == "differential") {
        return IO::FILTER_TYPES::DIFFERENTIAL;
    } else if (src == "line_difference") {
        return IO::FILTER_TYPES::LINE_DIFFERENCE;
    } else if (src == "average") {
        return IO::FILTER_TYPES::AVERAGE;
    } else if (src == "paeth") {
        return IO::FILTER_TYPES::PAETH;
    }

    return IO::FILTER_TYPES::NONE;
}

IO::FileMetadata::FileMetadata(std::ifstream& inputStream) {
    uint8_t compatibleTemp;
    uint8_t conversion;
    uint8_t bit;
    uint8_t model;
    uint8_t compression;
    uint8_t filter;
    uint16_t width;
    uint16_t height;
    uint8_t dithering;
    int indecesSize;

    inputStream.read((char*)&compatibleTemp, sizeof(uint8_t));
    inputStream.read((char*)&conversion, sizeof(uint8_t));
    inputStream.read((char*)&bit, sizeof(uint8_t));
    inputStream.read((char*)&model, sizeof(uint8_t));
    inputStream.read((char*)&compression, sizeof(uint8_t));
    inputStream.read((char*)&filter, sizeof(uint8_t));
    inputStream.read((char*)&width, sizeof(uint16_t));
    inputStream.read((char*)&height, sizeof(uint16_t));
    inputStream.read((char*)&dithering, sizeof(uint8_t));
    inputStream.read((char*)&indecesSize, sizeof(int));

    std::vector<Uint32> indeces(indecesSize, 0);
    inputStream.read((char*)(indeces.data()), indecesSize * sizeof(Uint32));

    setCompatible(compatibleTemp);
    setConvertion((IO::CONVERSION_TYPES)conversion);
    setBit((IO::BIT_TYPES)bit);
    setModel((IO::MODEL_TYPES)model);
    setCompression((IO::COMPRESSION_TYPES)compression);
    setFilter((IO::FILTER_TYPES)filter);
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

IO::BIT_TYPES IO::FileMetadata::getBit() {
    return bit;
}

void IO::FileMetadata::setBit(IO::BIT_TYPES value) {
    this->bit = value;
};

IO::MODEL_TYPES IO::FileMetadata::getModel() {
    return model;
}

void IO::FileMetadata::setModel(IO::MODEL_TYPES value) {
    this->model = value;
};

IO::COMPRESSION_TYPES IO::FileMetadata::getCompression() {
    return compression;
}

void IO::FileMetadata::setCompression(IO::COMPRESSION_TYPES value) {
    this->compression = value;
};

IO::FILTER_TYPES IO::FileMetadata::getFilter() {
    return filter;
}

void IO::FileMetadata::setFilter(IO::FILTER_TYPES value) {
    this->filter = value;
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

std::vector<Uint32> IO::FileMetadata::getIndeces() {
    return indeces;
};

void IO::FileMetadata::setIndeces(std::vector<Uint32> value) {
    this->indeces = value;
};

void IO::FileMetadata::writeTo(std::ofstream& ofs) {
    uint8_t compatibleTemp = getCompatible();
    uint8_t conversion = (uint8_t)getConvertion();
    uint8_t bit = (uint8_t)getBit();
    uint8_t model = (uint8_t)getModel();
    uint8_t compression = (uint8_t)getCompression();
    uint8_t filter = (uint8_t)getFilter();
    uint16_t width = getWidth();
    uint16_t height = getHeight();
    uint8_t dithering = getDithering();
    int indecesSize = getIndecesSize();
    std::vector<Uint32> indeces = getIndeces();

    ofs.write((char*)&compatibleTemp, sizeof(uint8_t));
    ofs.write((char*)&conversion, sizeof(uint8_t));
    ofs.write((char*)&bit, sizeof(uint8_t));
    ofs.write((char*)&model, sizeof(uint8_t));
    ofs.write((char*)&compression, sizeof(uint8_t));
    ofs.write((char*)&filter, sizeof(uint8_t));
    ofs.write((char*)&width, sizeof(uint16_t));
    ofs.write((char*)&height, sizeof(uint16_t));
    ofs.write((char*)&dithering, sizeof(uint8_t));
    ofs.write((char*)&indecesSize, sizeof(int));
    ofs.write((char*)(indeces.data()), indecesSize * sizeof(Uint32));
};

int IO::FileMetadata::getSize() {
    return sizeof(int) + (sizeof(uint16_t) * 2) + (sizeof(uint8_t) * 7) + (indeces.size() * sizeof(Uint32));
};

IO::FileMetadata* IO::composeMetadata(
    IO::CONVERSION_TYPES convertion,
    IO::BIT_TYPES bit,
    IO::MODEL_TYPES model,
    IO::COMPRESSION_TYPES compression,
    uint8_t dithering, 
    uint16_t width, 
    uint16_t height,
    std::optional<std::vector<Uint32>> indeces) {
    return new IO::FileMetadata(convertion, bit, model, compression, dithering, width, height);
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
