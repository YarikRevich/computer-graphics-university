#include "io.hpp"
#include <iostream>

IO::FILE_TYPES IO::getFileType(std::string src)
{
    if (src == "jpg" || src == "jpeg")
    {
        return IO::FILE_TYPES::JPG;
    }
    else if (src == "png")
    {
        return IO::FILE_TYPES::PNG;
    }
    else if (src == "bmp")
    {
        return IO::FILE_TYPES::BMP;
    }

    return IO::FILE_TYPES::NONE;
}

IO::CONVERSION_TYPES IO::getConversionType(std::string src)
{
    if (src == "native_colorful")
    {
        return IO::CONVERSION_TYPES::NATIVE_COLORFUL;
    }
    else if (src == "native_bw")
    {
        return IO::CONVERSION_TYPES::NATIVE_BW;
    }
    else if (src == "palette_colorful")
    {
        return IO::CONVERSION_TYPES::PALETTE_COLORFUL;
    }
    else if (src == "palette_bw")
    {
        return IO::CONVERSION_TYPES::PALETTE_BW;
    }

    return IO::CONVERSION_TYPES::NONE;
}

IO::BIT_TYPES IO::getBitType(std::string src)
{
    if (src == "24")
    {
        return IO::BIT_TYPES::TWENTY_FOUR;
    }
    else if (src == "16")
    {
        return IO::BIT_TYPES::SIXTEEN;
    }
    else if (src == "15")
    {
        return IO::BIT_TYPES::FIFTEEN;
    }
    else if (src == "7")
    {
        return IO::BIT_TYPES::SEVEN;
    }

    return IO::BIT_TYPES::NONE;
}

IO::MODEL_TYPES IO::getModelType(std::string src)
{
    if (src == "rgb")
    {
        return IO::MODEL_TYPES::RGB;
    }
    else if (src == "yuv")
    {
        return IO::MODEL_TYPES::YUV;
    }
    else if (src == "yiq")
    {
        return IO::MODEL_TYPES::YIQ;
    }
    else if (src == "ycbcr")
    {
        return IO::MODEL_TYPES::YCBCR;
    }
    else if (src == "hsl")
    {
        return IO::MODEL_TYPES::HSL;
    }

    return IO::MODEL_TYPES::NONE;
}

IO::LOSSLESS_COMPRESSION_TYPES IO::getLosslessCompressionType(std::string src)
{
    if (src == "byterun")
    {
        return IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN;
    }
    else if (src == "rle")
    {
        return IO::LOSSLESS_COMPRESSION_TYPES::RLE;
    }
    else if (src == "lzw")
    {
        return IO::LOSSLESS_COMPRESSION_TYPES::LZW;
    }
    else if (src == "lz77")
    {
        return IO::LOSSLESS_COMPRESSION_TYPES::LZ77;
    }

    return IO::LOSSLESS_COMPRESSION_TYPES::NONE;
}

IO::LOSSY_COMPRESSION_TYPES IO::getLossyCompressionType(std::string src)
{
    if (src == "dct")
    {
        return IO::LOSSY_COMPRESSION_TYPES::DCT;
    }

    return IO::LOSSY_COMPRESSION_TYPES::NONE;
}

IO::SAMPLING_TYPES IO::getSamplingType(std::string src)
{
    if (src == "four_two_one")
    {
        return IO::SAMPLING_TYPES::FOUR_TWO_ONE;
    }

    return IO::SAMPLING_TYPES::NONE;
}

IO::FILTER_TYPES IO::getFilterType(std::string src)
{
    if (src == "differential")
    {
        return IO::FILTER_TYPES::DIFFERENTIAL;
    }
    else if (src == "line_difference")
    {
        return IO::FILTER_TYPES::LINE_DIFFERENCE;
    }
    else if (src == "average")
    {
        return IO::FILTER_TYPES::AVERAGE;
    }
    else if (src == "paeth")
    {
        return IO::FILTER_TYPES::PAETH;
    }

    return IO::FILTER_TYPES::NONE;
}

uint8_t IO::FileMetadata::getCompatible()
{
    return compatible;
}

IO::CONVERSION_TYPES IO::FileMetadata::getConvertion()
{
    return convertion;
}

IO::BIT_TYPES IO::FileMetadata::getBit()
{
    return bit;
}

IO::MODEL_TYPES IO::FileMetadata::getModel()
{
    return model;
}

IO::LOSSLESS_COMPRESSION_TYPES IO::FileMetadata::getLosslessCompression()
{
    return losslessCompression;
}

int IO::FileMetadata::getLosslessCompressionSize()
{
    return losslessCompressionSize;
}

IO::LOSSY_COMPRESSION_TYPES IO::FileMetadata::getLossyCompression()
{
    return lossyCompression;
}

IO::SAMPLING_TYPES IO::FileMetadata::getSampling()
{
    return sampling;
}

IO::FILTER_TYPES IO::FileMetadata::getFilter()
{
    return filter;
}

uint16_t IO::FileMetadata::getWidth()
{
    return width;
};

uint16_t IO::FileMetadata::getHeight()
{
    return height;
};

uint8_t IO::FileMetadata::getDithering()
{
    return dithering;
}

int IO::FileMetadata::getIndecesSize()
{
    return indecesSize;
};

void IO::FileMetadata::writeTo(std::ofstream &ofs)
{
    uint8_t compatibleTemp = getCompatible();
    uint8_t conversion = (uint8_t)getConvertion();
    uint8_t bit = (uint8_t)getBit();
    uint8_t model = (uint8_t)getModel();
    uint8_t losslessCompression = (uint8_t)getLosslessCompression();
    int losslessCompressionSize = getLosslessCompressionSize();
    uint8_t lossyCompression = (uint8_t)getLossyCompression();
    uint8_t sampling = (uint8_t)getSampling();
    uint8_t filter = (uint8_t)getFilter();
    uint16_t width = getWidth();
    uint16_t height = getHeight();
    uint8_t dithering = getDithering();
    int indecesSize = getIndecesSize();

    ofs.write((char *)&compatibleTemp, sizeof(uint8_t));
    ofs.write((char *)&conversion, sizeof(uint8_t));
    ofs.write((char *)&bit, sizeof(uint8_t));
    ofs.write((char *)&model, sizeof(uint8_t));
    ofs.write((char *)&losslessCompression, sizeof(uint8_t));
    ofs.write((char *)&losslessCompressionSize, sizeof(int));
    ofs.write((char *)&lossyCompression, sizeof(uint8_t));
    ofs.write((char *)&sampling, sizeof(uint8_t));
    ofs.write((char *)&filter, sizeof(uint8_t));
    ofs.write((char *)&width, sizeof(uint16_t));
    ofs.write((char *)&height, sizeof(uint16_t));
    ofs.write((char *)&dithering, sizeof(uint8_t));
    ofs.write((char *)&indecesSize, sizeof(int));
};

IO::FileMetadata *IO::readMetadata(std::ifstream &inputStream)
{
    uint8_t compatibleTemp;
    uint8_t conversion;
    uint8_t bit;
    uint8_t model;
    uint8_t losslessCompression;
    int losslessCompressionSize;
    uint8_t lossyCompression;
    uint8_t sampling;
    uint8_t filter;
    uint16_t width;
    uint16_t height;
    uint8_t dithering;
    int indecesSize;

    inputStream.read((char *)&compatibleTemp, sizeof(uint8_t));
    inputStream.read((char *)&conversion, sizeof(uint8_t));
    inputStream.read((char *)&bit, sizeof(uint8_t));
    inputStream.read((char *)&model, sizeof(uint8_t));
    inputStream.read((char *)&losslessCompression, sizeof(uint8_t));
    inputStream.read((char *)&losslessCompressionSize, sizeof(int));
    inputStream.read((char *)&lossyCompression, sizeof(uint8_t));
    inputStream.read((char *)&sampling, sizeof(uint8_t));
    inputStream.read((char *)&filter, sizeof(uint8_t));
    inputStream.read((char *)&width, sizeof(uint16_t));
    inputStream.read((char *)&height, sizeof(uint16_t));
    inputStream.read((char *)&dithering, sizeof(uint8_t));
    inputStream.read((char *)&indecesSize, sizeof(int));

    IO::CONVERSION_TYPES conversionType = (IO::CONVERSION_TYPES)conversion;
    IO::BIT_TYPES bitType = (IO::BIT_TYPES)bit;
    IO::MODEL_TYPES modelType = (IO::MODEL_TYPES)model;
    IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType = (IO::LOSSLESS_COMPRESSION_TYPES)losslessCompression;
    IO::LOSSY_COMPRESSION_TYPES lossyCompressionType = (IO::LOSSY_COMPRESSION_TYPES)lossyCompression;
    IO::SAMPLING_TYPES samplingType = (IO::SAMPLING_TYPES)sampling;
    IO::FILTER_TYPES filterType = (IO::FILTER_TYPES)filter;

    return new IO::FileMetadata(
        compatibleTemp,
        conversionType,
        bitType,
        modelType,
        losslessCompressionType,
        losslessCompressionSize,
        lossyCompressionType,
        samplingType,
        filterType,
        width,
        height,
        dithering,
        indecesSize);
}

int IO::FileMetadata::getSize() {
    return sizeof(int) + (sizeof(uint16_t) * 2) + (sizeof(uint8_t) * 9);
};

IO::FileMetadata *IO::composeMetadata(
    IO::CONVERSION_TYPES convertion,
    IO::BIT_TYPES bit,
    IO::MODEL_TYPES model,
    IO::LOSSLESS_COMPRESSION_TYPES losslessCompression,
    int losslessCompressionSize,
    IO::LOSSY_COMPRESSION_TYPES lossyCompression,
    IO::SAMPLING_TYPES sampling,
    IO::FILTER_TYPES filter,
    uint8_t dithering,
    uint16_t width,
    uint16_t height,
    int indecesSize)
{
    return new IO::FileMetadata(
        IO::FileMetadata::COMPATIBLE_FLAG, 
        convertion, 
        bit, 
        model, 
        losslessCompression, 
        losslessCompressionSize,
        lossyCompression, 
        sampling, 
        filter, 
        width, 
        height, 
        dithering, 
        indecesSize);
}

SDL_Surface *IO::readFileJPEG(std::string path)
{
    if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG)
    {
        return NULL;
    }

    return IMG_Load(path.c_str());
}

SDL_Surface *IO::readFilePNG(std::string path)
{
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        return NULL;
    }

    return IMG_Load(path.c_str());
};

SDL_Surface *IO::readFileBMP(std::string path)
{
    return SDL_LoadBMP(path.c_str());
}

int IO::writeFileJPEG(std::string path, SDL_Surface *surface)
{
    return IMG_SaveJPG(surface, path.c_str(), 66);
};

int IO::writeFilePNG(std::string path, SDL_Surface *surface)
{
    return IMG_SavePNG(surface, path.c_str());
};

int IO::writeFileBMP(std::string path, SDL_Surface *surface)
{
    return SDL_SaveBMP(surface, path.c_str());
};
