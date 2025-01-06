#include "pipeline.hpp"

SDL_Surface *Pipeline::handleView(std::ifstream &inputStream, bool debug)
{
    IO::FileMetadata *metadata = IO::readMetadata(inputStream);
    if (!metadata->getCompatible())
    {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);

        return NULL;
    }

    inputStream.seekg(metadata->getSize());

    std::vector<SDL_Color> colors;

    if (metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_COLORFUL ||
        metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_BW)
    {
        if (metadata->getBit() == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> buff;

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Sint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressByteRunImageUint8(compressionBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Uint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressRLEImageUint8(compressionBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint8>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint8> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint8));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                auto decompressedBuffer = Service::decompressLZWImageUint8(new Processor::LZWResult<Uint8>(compounds, compressionBuffer));

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint8> *> serializedBuffer;

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint8 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint8));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint8>(distance, length, symbol));
                }

                auto decompressedBuffer = Service::decompressLZ77ImageUint8(serializedBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else
            {
                std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
                for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
                {
                    inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                    buff.push_back(internal);
                }
            }

            if (metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_COLORFUL)
            {
                colors = Service::convertFrom7BitColorful(buff);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
                for (auto &value : buff)
                {
                    assemble = Processor::convert7BitTo8Bit(value);

                    for (auto &compound : assemble)
                    {
                        switch (metadata->getModel())
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            colors.push_back(Processor::convert7BitGreyTo24BitYCbCr(compound));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            colors.push_back(Processor::convert7BitGreyTo24BitYUV(compound));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            colors.push_back(Processor::convert7BitGreyTo24BitYIQ(compound));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            colors.push_back(Processor::convert7BitGreyTo24BitHSL(compound));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            colors.push_back(Processor::convert7BitGreyTo24BitRGB(compound));

                            break;
                        }
                    }
                }
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Sint16));

                buff = Service::decompressByteRunImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Uint16));

                buff = Service::decompressRLEImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint16>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint16> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint16));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                buff = Service::decompressLZWImageUint16(new Processor::LZWResult<Uint16>(compounds, compressionBuffer));
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint16> *> serializedBuffer;

                std::vector<Uint16> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint16 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint16));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint16>(distance, length, symbol));
                }

                buff = Service::decompressLZ77ImageUint16(serializedBuffer);
            }
            else
            {
                inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));
            }

            colors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Sint16));

                buff = Service::decompressByteRunImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Uint16));

                buff = Service::decompressRLEImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint16>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint16> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint16));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                buff = Service::decompressLZWImageUint16(new Processor::LZWResult<Uint16>(compounds, compressionBuffer));
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint16> *> serializedBuffer;

                std::vector<Uint16> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint16 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint16));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint16>(distance, length, symbol));
                }

                buff = Service::decompressLZ77ImageUint16(serializedBuffer);
            }
            else
            {
                inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));
            }

            colors = Service::convertFrom16Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> buff;

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Sint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressByteRunImageUint8(compressionBuffer);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Uint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressRLEImageUint8(compressionBuffer);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint8>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint8> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint8));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                auto decompressedBuffer = Service::decompressLZWImageUint8(new Processor::LZWResult<Uint8>(compounds, compressionBuffer));

                for (int i = 0; i < decompressedBuffer.size(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint8> *> serializedBuffer;

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint8 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint8));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint8>(distance, length, symbol));
                }

                auto decompressedBuffer = Service::decompressLZ77ImageUint8(serializedBuffer);

                for (int i = 0; i < decompressedBuffer.size(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else
            {
                std::vector<Uint8> internal(3, 0);
                for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight())); i++)
                {
                    inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                    buff.push_back(internal);
                }
            }

            colors = Service::convertFrom24Bit(buff);
        }
    }
    else if (metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_COLORFUL ||
             metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_BW)
    {
        std::vector<SDL_Color> encodedColors;

        if (metadata->getBit() == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> buff;

            std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
            for (int i = 0; i < (metadata->getIndecesSize() / PREFERRED_BIT_NUM_PER_PIXEL) - 2; i++)
            {
                inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                buff.push_back(internal);
            }

            if (metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_COLORFUL)
            {
                encodedColors = Service::convertFrom7BitColorful(buff);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
                for (auto &value : buff)
                {
                    assemble = Processor::convert7BitTo8Bit(value);

                    for (auto &compound : assemble)
                    {
                        switch (metadata->getModel())
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYCbCr(compound));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYUV(compound));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYIQ(compound));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitHSL(compound));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitRGB(compound));

                            break;
                        }
                    }
                }
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getIndecesSize(), 0);

            inputStream.read((char *)(buff.data()), metadata->getIndecesSize() * sizeof(Uint16));

            encodedColors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getIndecesSize(), 0);

            inputStream.read((char *)(buff.data()), metadata->getIndecesSize() * sizeof(Uint16));

            encodedColors = Service::convertFrom16Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> buff;

            std::vector<Uint8> internal(3, 0);
            for (int i = 0; i < metadata->getIndecesSize(); i++)
            {
                inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                buff.push_back(internal);
            }

            encodedColors = Service::convertFrom24Bit(buff);
        }

        std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);

        if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
        {
            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressByteRunImageInt(compressionBuffer);
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
        {
            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressRLEImageInt(compressionBuffer);
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
        {
            std::map<int, std::vector<int>> compounds;

            int valuesSize;

            inputStream.read((char *)&valuesSize, sizeof(int));

            for (int i = 0; i < valuesSize; i++)
            {
                int valueFirst;

                inputStream.read((char *)&valueFirst, sizeof(int));

                int valueSecondSize;

                inputStream.read((char *)&valueSecondSize, sizeof(int));

                std::vector<int> valueSecond(valueSecondSize, 0);

                inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(int));

                compounds[valueFirst] = valueSecond;
            }

            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressLZWImageInt(new Processor::LZWResult<int>(compounds, compressionBuffer));
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
        {
            std::vector<Processor::LZ77Result<int> *> serializedBuffer;

            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
            {
                serializedBuffer.push_back(
                    new Processor::LZ77Result<int>(
                        compressionBuffer.at(i), compressionBuffer.at(i + 1), compressionBuffer.at(i + 2)));
            }

            input = Service::decompressLZ77ImageInt(serializedBuffer);
        }
        else
        {
            inputStream.read((char *)(input.data()), input.size() * sizeof(int));
        }

        if (metadata->getLossyCompression() == IO::LOSSY_COMPRESSION_TYPES::DCT) {
            input = Service::decompressDCTImageInt(input, metadata->getHeight(), metadata->getWidth());
        }

        for (auto &value : input)
        {
            colors.push_back(encodedColors[value]);
        }
    }
    else
    {
        return NULL;
    }

    switch (metadata->getModel())
    {
    case IO::MODEL_TYPES::YCBCR:
        Service::convertFromYCbCr(colors);

        break;
    case IO::MODEL_TYPES::YIQ:
        Service::convertFromYIQ(colors);

        break;
    case IO::MODEL_TYPES::YUV:
        Service::convertFromYUV(colors);

        break;
    case IO::MODEL_TYPES::HSL:
        Service::convertFromHSL(colors);

        break;
    }

    SDL_Surface *input = Processor::createFilledSurface(metadata->getWidth(), metadata->getHeight(), colors);
    if (input == NULL)
    {
        return NULL;
    }

    if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
    {
        switch (metadata->getConvertion())
        {
        case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
            if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }

            break;
        case IO::CONVERSION_TYPES::PALETTE_BW:
            if (Service::applyBWDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }

            break;
        }
    }

    if (debug)
    {
        if (Service::convertToCGUPaletteDetected(input) != EXIT_SUCCESS)
        {
            return NULL;
        };
    }

    return input;
};

int Pipeline::handleDecode(std::ifstream &inputStream, bool debug, IO::FILE_TYPES fileType, std::string to)
{
    IO::FileMetadata *metadata = IO::readMetadata(inputStream);
    if (!metadata->getCompatible())
    {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);

        return EXIT_FAILURE;
    }

    inputStream.seekg(metadata->getSize());

    std::vector<SDL_Color> colors;

    if (metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_COLORFUL ||
        metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_BW)
    {
        if (metadata->getBit() == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> buff;

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Sint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressByteRunImageUint8(compressionBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Uint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressRLEImageUint8(compressionBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint8>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint8> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint8));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                auto decompressedBuffer = Service::decompressLZWImageUint8(new Processor::LZWResult<Uint8>(compounds, compressionBuffer));

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint8> *> serializedBuffer;

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint8 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint8));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint8>(distance, length, symbol));
                }

                auto decompressedBuffer = Service::decompressLZ77ImageUint8(serializedBuffer);

                for (int i = 0; i < (metadata->getWidth() * metadata->getHeight()) - ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i += PREFERRED_BIT_NUM_PER_PIXEL)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < PREFERRED_BIT_NUM_PER_PIXEL; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else
            {
                std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
                for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
                {
                    inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                    buff.push_back(internal);
                }
            }

            if (metadata->getConvertion() == IO::CONVERSION_TYPES::NATIVE_COLORFUL)
            {
                colors = Service::convertFrom7BitColorful(buff);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
                for (auto &value : buff)
                {
                    assemble = Processor::convert7BitTo8Bit(value);

                    for (auto &compound : assemble)
                    {
                        switch (metadata->getModel())
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            colors.push_back(Processor::convert7BitGreyTo24BitYCbCr(compound));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            colors.push_back(Processor::convert7BitGreyTo24BitYUV(compound));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            colors.push_back(Processor::convert7BitGreyTo24BitYIQ(compound));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            colors.push_back(Processor::convert7BitGreyTo24BitHSL(compound));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            colors.push_back(Processor::convert7BitGreyTo24BitRGB(compound));

                            break;
                        }
                    }
                }
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Sint16));

                buff = Service::decompressByteRunImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Uint16));

                buff = Service::decompressRLEImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint16>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint16> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint16));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                buff = Service::decompressLZWImageUint16(new Processor::LZWResult<Uint16>(compounds, compressionBuffer));
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint16> *> serializedBuffer;

                std::vector<Uint16> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint16 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint16));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint16>(distance, length, symbol));
                }

                buff = Service::decompressLZ77ImageUint16(serializedBuffer);
            }
            else
            {
                inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));
            }

            colors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Sint16));

                buff = Service::decompressByteRunImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> compressed(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressed.data()), metadata->getLosslessCompressionSize() * sizeof(Uint16));

                buff = Service::decompressRLEImageUint16(compressed);
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint16>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint16> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint16));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                buff = Service::decompressLZWImageUint16(new Processor::LZWResult<Uint16>(compounds, compressionBuffer));
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint16> *> serializedBuffer;

                std::vector<Uint16> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint16 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint16));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint16>(distance, length, symbol));
                }

                buff = Service::decompressLZ77ImageUint16(serializedBuffer);
            }
            else
            {
                inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));
            }

            colors = Service::convertFrom16Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> buff;

            if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Sint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressByteRunImageUint8(compressionBuffer);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(Uint8));

                std::vector<Uint8> decompressedBuffer = Service::decompressRLEImageUint8(compressionBuffer);

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::map<int, std::vector<Uint8>> compounds;

                int valuesSize;

                inputStream.read((char *)&valuesSize, sizeof(int));

                for (int i = 0; i < valuesSize; i++)
                {
                    int valueFirst;

                    inputStream.read((char *)&valueFirst, sizeof(int));

                    int valueSecondSize;

                    inputStream.read((char *)&valueSecondSize, sizeof(int));

                    std::vector<Uint8> valueSecond(valueSecondSize, 0);

                    inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(Uint8));

                    compounds[valueFirst] = valueSecond;
                }

                std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

                inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

                auto decompressedBuffer = Service::decompressLZWImageUint8(new Processor::LZWResult<Uint8>(compounds, compressionBuffer));

                for (int i = 0; i < decompressedBuffer.size(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Processor::LZ77Result<Uint8> *> serializedBuffer;

                for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
                {
                    int distance;

                    inputStream.read((char *)&distance, sizeof(int));

                    int length;

                    inputStream.read((char *)&length, sizeof(int));

                    Uint8 symbol;

                    inputStream.read((char *)&symbol, sizeof(Uint8));

                    serializedBuffer.push_back(new Processor::LZ77Result<Uint8>(distance, length, symbol));
                }

                auto decompressedBuffer = Service::decompressLZ77ImageUint8(serializedBuffer);

                for (int i = 0; i < decompressedBuffer.size(); i += 3)
                {
                    std::vector<Uint8> internal;

                    for (int k = 0; k < 3; k++)
                    {
                        internal.push_back(decompressedBuffer[i + k]);
                    }

                    buff.push_back(internal);
                }
            }
            else
            {
                std::vector<Uint8> internal(3, 0);
                for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight())); i++)
                {
                    inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                    buff.push_back(internal);
                }
            }

            colors = Service::convertFrom24Bit(buff);
        }
    }
    else if (metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_COLORFUL ||
             metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_BW)
    {
        std::vector<SDL_Color> encodedColors;

        if (metadata->getBit() == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> buff;

            std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
            for (int i = 0; i < (metadata->getIndecesSize() / PREFERRED_BIT_NUM_PER_PIXEL) - 2; i++)
            {
                inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                buff.push_back(internal);
            }

            if (metadata->getConvertion() == IO::CONVERSION_TYPES::PALETTE_COLORFUL)
            {
                encodedColors = Service::convertFrom7BitColorful(buff);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
                for (auto &value : buff)
                {
                    assemble = Processor::convert7BitTo8Bit(value);

                    for (auto &compound : assemble)
                    {
                        switch (metadata->getModel())
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYCbCr(compound));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYUV(compound));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitYIQ(compound));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitHSL(compound));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            encodedColors.push_back(Processor::convert7BitGreyTo24BitRGB(compound));

                            break;
                        }
                    }
                }
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getIndecesSize(), 0);

            inputStream.read((char *)(buff.data()), metadata->getIndecesSize() * sizeof(Uint16));

            encodedColors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getIndecesSize(), 0);

            inputStream.read((char *)(buff.data()), metadata->getIndecesSize() * sizeof(Uint16));

            encodedColors = Service::convertFrom16Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> buff;

            std::vector<Uint8> internal(3, 0);
            for (int i = 0; i < metadata->getIndecesSize(); i++)
            {
                inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                buff.push_back(internal);
            }

            encodedColors = Service::convertFrom24Bit(buff);
        }

        std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);

        if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
        {
            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressByteRunImageInt(compressionBuffer);
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
        {
            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressRLEImageInt(compressionBuffer);
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
        {
            std::map<int, std::vector<int>> compounds;

            int valuesSize;

            inputStream.read((char *)&valuesSize, sizeof(int));

            for (int i = 0; i < valuesSize; i++)
            {
                int valueFirst;

                inputStream.read((char *)&valueFirst, sizeof(int));

                int valueSecondSize;

                inputStream.read((char *)&valueSecondSize, sizeof(int));

                std::vector<int> valueSecond(valueSecondSize, 0);

                inputStream.read((char *)(valueSecond.data()), valueSecondSize * sizeof(int));

                compounds[valueFirst] = valueSecond;
            }

            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            input = Service::decompressLZWImageInt(new Processor::LZWResult<int>(compounds, compressionBuffer));
        }
        else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
        {
            std::vector<Processor::LZ77Result<int> *> serializedBuffer;

            std::vector<int> compressionBuffer(metadata->getLosslessCompressionSize(), 0);

            inputStream.read((char *)(compressionBuffer.data()), metadata->getLosslessCompressionSize() * sizeof(int));

            for (int i = 0; i < metadata->getLosslessCompressionSize(); i += 3)
            {
                serializedBuffer.push_back(
                    new Processor::LZ77Result<int>(
                        compressionBuffer.at(i), compressionBuffer.at(i + 1), compressionBuffer.at(i + 2)));
            }

            input = Service::decompressLZ77ImageInt(serializedBuffer);
        }
        else
        {
            inputStream.read((char *)(input.data()), input.size() * sizeof(int));
        }

        if (metadata->getLossyCompression() == IO::LOSSY_COMPRESSION_TYPES::DCT) {
            input = Service::decompressDCTImageInt(input, metadata->getHeight(), metadata->getWidth());
        }

        for (auto &value : input)
        {
            colors.push_back(encodedColors[value]);
        }
    }
    else
    {
        return NULL;
    }

    switch (metadata->getModel())
    {
    case IO::MODEL_TYPES::YCBCR:
        Service::convertFromYCbCr(colors);

        break;
    case IO::MODEL_TYPES::YIQ:
        Service::convertFromYIQ(colors);

        break;
    case IO::MODEL_TYPES::YUV:
        Service::convertFromYUV(colors);

        break;
    case IO::MODEL_TYPES::HSL:
        Service::convertFromHSL(colors);

        break;
    }

    SDL_Surface *input = Processor::createFilledSurface(metadata->getWidth(), metadata->getHeight(), colors);
    if (input == NULL)
    {
        return EXIT_FAILURE;
    }

    if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
    {
        switch (metadata->getConvertion())
        {
        case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
            if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

            break;
        case IO::CONVERSION_TYPES::PALETTE_BW:
            if (Service::applyBWDithering(input) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

            break;
        }
    }

    if (debug)
    {
        if (Service::convertToCGUPaletteDetected(input) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        };
    }

    switch (fileType)
    {
    case IO::FILE_TYPES::JPG:
        if (IO::writeFileJPEG(to, input) != EXIT_SUCCESS)
        {
            return EXIT_SUCCESS;
        };
        break;
    case IO::FILE_TYPES::PNG:
        if (IO::writeFilePNG(to, input) != EXIT_SUCCESS)
        {
            return EXIT_SUCCESS;
        };
        break;
    case IO::FILE_TYPES::BMP:
        if (IO::writeFileBMP(to, input) != EXIT_SUCCESS)
        {
            return EXIT_SUCCESS;
        };
        break;
    }
}

int Pipeline::handleEncode(
    SDL_Surface *input,
    IO::CONVERSION_TYPES conversionType,
    IO::BIT_TYPES bitType,
    IO::MODEL_TYPES modelType,
    IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType,
    IO::LOSSY_COMPRESSION_TYPES lossyCompressionType,
    IO::SAMPLING_TYPES samplingType,
    IO::FILTER_TYPES filterType,
    bool dithering,
    std::ofstream &outputStream)
{
    if (conversionType == IO::CONVERSION_TYPES::NATIVE_COLORFUL ||
        conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
    {
        if (dithering)
        {
            switch (conversionType)
            {
            case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
                if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
                {
                    return EXIT_FAILURE;
                }

                break;
            case IO::CONVERSION_TYPES::NATIVE_BW:
                if (Service::applyBWDithering(input) != EXIT_SUCCESS)
                {
                    return EXIT_FAILURE;
                }

                break;
            }
        }

        std::vector<SDL_Color> colors = Processor::getCompleteBitColorMap(input);

        switch (modelType)
        {
        case IO::MODEL_TYPES::YCBCR:
            Service::convertToYCbCr(colors);

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
            {
                Service::convertToYCbCrBW(colors);
            }

            break;
        case IO::MODEL_TYPES::YIQ:
            Service::convertToYIQ(colors);

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
            {
                Service::convertToYIQBW(colors);
            }

            break;
        case IO::MODEL_TYPES::YUV:
            Service::convertToYUV(colors);

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
            {
                Service::convertToYUVBW(colors);
            }

            break;
        case IO::MODEL_TYPES::HSL:
            Service::convertToHSL(colors);

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
            {
                Service::convertToHSLBW(colors);
            }

            break;

        case IO::MODEL_TYPES::RGB:
            if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
            {
                Service::convertToRGBBW(colors);
            }
        }

        if (samplingType == IO::SAMPLING_TYPES::FOUR_TWO_ONE)
        {
            if (modelType == IO::MODEL_TYPES::RGB)
            {
                colors = Service::sampleFourToOneRGB(colors, input);
            }
            else if (modelType == IO::MODEL_TYPES::YUV)
            {
                colors = Service::sampleFourToOneYUV(colors, input);
            }
            else if (modelType == IO::MODEL_TYPES::YIQ)
            {
                colors = Service::sampleFourToOneYIQ(colors, input);
            }
            else if (modelType == IO::MODEL_TYPES::YCBCR)
            {
                colors = Service::sampleFourToOneYCbCr(colors, input);
            }
            else if (modelType == IO::MODEL_TYPES::HSL)
            {
                colors = Service::sampleFourToOneHSL(colors, input);
            }
        }

        if (bitType == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> sevenBitColors;

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_COLORFUL)
            {
                sevenBitColors = Service::convertTo7BitColorful(colors);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

                for (int i = 0; i < colors.size(); i += ORIGINAL_BIT_NUM_PER_PIXEL)
                {
                    assemble.clear();

                    for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
                    {
                        switch (modelType)
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            assemble.push_back(Processor::convert24BitYCbCrTo7BitGrey(colors[i + j]));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            assemble.push_back(Processor::convert24BitYUVTo7BitGrey(colors[i + j]));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            assemble.push_back(Processor::convert24BitYIQTo7BitGrey(colors[i + j]));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            assemble.push_back(Processor::convert24BitHSLTo7BitGrey(colors[i + j]));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(colors[i + j]));

                            break;
                        }
                    }

                    sevenBitColors.push_back(Processor::convert8BitTo7Bit(assemble));
                }
            }

            if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : sevenBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                std::vector<Sint8> sevenBitColorsCompressed = Service::compressByteRunImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sevenBitColorsCompressed.data()), sevenBitColorsCompressed.size() * sizeof(Sint8));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : sevenBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                std::vector<Uint8> sevenBitColorsCompressed = Service::compressRLEImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sevenBitColorsCompressed.data()), sevenBitColorsCompressed.size() * sizeof(Uint8));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::vector<Uint8> serializedBuffer;

                for (std::vector<Uint8> value : sevenBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        serializedBuffer.push_back(compound);
                    }
                }

                auto sevenBitColorsCompressed = Service::compressLZWImageUint8(serializedBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed->getResult().size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                int valuesSize = sevenBitColorsCompressed->getCompounds().size();

                outputStream.write((char *)(&valuesSize), sizeof(int));

                for (auto value : sevenBitColorsCompressed->getCompounds())
                {
                    int valueFirst = value.first;

                    outputStream.write((char *)(&valueFirst), sizeof(int));

                    int valueSecondSize = value.second.size();

                    outputStream.write((char *)(&valueSecondSize), sizeof(int));

                    outputStream.write((char *)(value.second.data()), value.second.size() * sizeof(Uint8));
                }

                outputStream.write((char *)(sevenBitColorsCompressed->getResult().data()), sevenBitColorsCompressed->getResult().size() * sizeof(int));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : sevenBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                auto sevenBitColorsCompressed = Service::compressLZ77ImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed.size() * 3,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (auto value : sevenBitColorsCompressed)
                {
                    int distance = value->getDistance();

                    outputStream.write((char *)&distance, sizeof(int));

                    int length = value->getLength();

                    outputStream.write((char *)&length, sizeof(int));

                    Uint8 symbol = value->getSymbol();

                    outputStream.write((char *)&symbol, sizeof(Uint8));
                }
            }
            else
            {
                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    0,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (std::vector<Uint8> &value : sevenBitColors)
                {
                    outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
                }
            }
        }
        else if (bitType == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> fifteenBitColors = Service::convertTo15Bit(colors);

            if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> fifteenBitColorsCompressed = Service::compressByteRunImageUint16(fifteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    fifteenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(fifteenBitColorsCompressed.data()), fifteenBitColorsCompressed.size() * sizeof(Sint16));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> fifteenBitColorsCompressed = Service::compressRLEImageUint16(fifteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    fifteenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(fifteenBitColorsCompressed.data()), fifteenBitColorsCompressed.size() * sizeof(Uint16));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                auto fifteenBitColorsCompressed = Service::compressLZWImageUint16(fifteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    fifteenBitColorsCompressed->getResult().size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                int valuesSize = fifteenBitColorsCompressed->getCompounds().size();

                outputStream.write((char *)(&valuesSize), sizeof(int));

                for (auto value : fifteenBitColorsCompressed->getCompounds())
                {
                    int valueFirst = value.first;

                    outputStream.write((char *)(&valueFirst), sizeof(int));

                    int valueSecondSize = value.second.size();

                    outputStream.write((char *)(&valueSecondSize), sizeof(int));

                    outputStream.write((char *)(value.second.data()), value.second.size() * sizeof(Uint16));
                }

                outputStream.write((char *)(fifteenBitColorsCompressed->getResult().data()), fifteenBitColorsCompressed->getResult().size() * sizeof(int));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                auto compressionBuffer = Service::compressLZ77ImageUint16(fifteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    compressionBuffer.size() * 3,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (auto value : compressionBuffer)
                {
                    int distance = value->getDistance();

                    outputStream.write((char *)&distance, sizeof(int));

                    int length = value->getLength();

                    outputStream.write((char *)&length, sizeof(int));

                    Uint16 symbol = value->getSymbol();

                    outputStream.write((char *)&symbol, sizeof(Uint16));
                }
            }
            else
            {
                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    0,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(fifteenBitColors.data()), fifteenBitColors.size() * sizeof(Uint16));
            }
        }
        else if (bitType == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> sixteenBitColors = Service::convertTo16Bit(colors);

            if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Sint16> sixteenBitColorsCompressed = Service::compressByteRunImageUint16(sixteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sixteenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sixteenBitColorsCompressed.data()), sixteenBitColorsCompressed.size() * sizeof(Sint16));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint16> sixteenBitColorsCompressed = Service::compressRLEImageUint16(sixteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sixteenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sixteenBitColorsCompressed.data()), sixteenBitColorsCompressed.size() * sizeof(Uint16));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                auto sixteenBitColorsCompressed = Service::compressLZWImageUint16(sixteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sixteenBitColorsCompressed->getResult().size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                int valuesSize = sixteenBitColorsCompressed->getCompounds().size();

                outputStream.write((char *)(&valuesSize), sizeof(int));

                for (auto value : sixteenBitColorsCompressed->getCompounds())
                {
                    int valueFirst = value.first;

                    outputStream.write((char *)(&valueFirst), sizeof(int));

                    int valueSecondSize = value.second.size();

                    outputStream.write((char *)(&valueSecondSize), sizeof(int));

                    outputStream.write((char *)(value.second.data()), value.second.size() * sizeof(Uint16));
                }

                outputStream.write((char *)(sixteenBitColorsCompressed->getResult().data()), sixteenBitColorsCompressed->getResult().size() * sizeof(int));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                auto compressionBuffer = Service::compressLZ77ImageUint16(sixteenBitColors);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    compressionBuffer.size() * 3,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (auto value : compressionBuffer)
                {
                    int distance = value->getDistance();

                    outputStream.write((char *)&distance, sizeof(int));

                    int length = value->getLength();

                    outputStream.write((char *)&length, sizeof(int));

                    Uint16 symbol = value->getSymbol();

                    outputStream.write((char *)&symbol, sizeof(Uint16));
                }
            }
            else
            {
                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    0,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sixteenBitColors.data()), sixteenBitColors.size() * sizeof(Uint16));
            }
        }
        else if (bitType == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> twentyFourBitColors = Service::convertTo24Bit(colors);

            if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : twentyFourBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                std::vector<Sint8> sevenBitColorsCompressed = Service::compressByteRunImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sevenBitColorsCompressed.data()), sevenBitColorsCompressed.size() * sizeof(Sint8));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : twentyFourBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                std::vector<Uint8> sevenBitColorsCompressed = Service::compressRLEImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    sevenBitColorsCompressed.size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                outputStream.write((char *)(sevenBitColorsCompressed.data()), sevenBitColorsCompressed.size() * sizeof(Uint8));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
            {
                std::vector<Uint8> serializedBuffer;

                for (std::vector<Uint8> value : twentyFourBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        serializedBuffer.push_back(compound);
                    }
                }

                auto twentyFourBitColorsCompressed = Service::compressLZWImageUint8(serializedBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    twentyFourBitColorsCompressed->getResult().size(),
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                int valuesSize = twentyFourBitColorsCompressed->getCompounds().size();

                outputStream.write((char *)(&valuesSize), sizeof(int));

                for (auto value : twentyFourBitColorsCompressed->getCompounds())
                {
                    int valueFirst = value.first;

                    outputStream.write((char *)(&valueFirst), sizeof(int));

                    int valueSecondSize = value.second.size();

                    outputStream.write((char *)(&valueSecondSize), sizeof(int));

                    outputStream.write((char *)(value.second.data()), value.second.size() * sizeof(Uint8));
                }

                outputStream.write((char *)(twentyFourBitColorsCompressed->getResult().data()), twentyFourBitColorsCompressed->getResult().size() * sizeof(int));
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
                std::vector<Uint8> compressionBuffer;

                for (std::vector<Uint8> value : twentyFourBitColors)
                {
                    for (Uint8 compound : value)
                    {
                        compressionBuffer.push_back(compound);
                    }
                }

                auto twentyFourBitColorsCompressed = Service::compressLZ77ImageUint8(compressionBuffer);

                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    twentyFourBitColorsCompressed.size() * 3,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (auto value : twentyFourBitColorsCompressed)
                {
                    int distance = value->getDistance();

                    outputStream.write((char *)&distance, sizeof(int));

                    int length = value->getLength();

                    outputStream.write((char *)&length, sizeof(int));

                    Uint8 symbol = value->getSymbol();

                    outputStream.write((char *)&symbol, sizeof(Uint8));
                }
            }
            else
            {
                Service::saveMetadata(
                    conversionType,
                    bitType,
                    modelType,
                    losslessCompressionType,
                    0,
                    lossyCompressionType,
                    samplingType,
                    filterType,
                    dithering,
                    input->w,
                    input->h,
                    0,
                    outputStream);

                for (std::vector<Uint8> &value : twentyFourBitColors)
                {
                    outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
                }
            }
        }

        return EXIT_SUCCESS;
    }
    else if (conversionType == IO::CONVERSION_TYPES::PALETTE_COLORFUL ||
             conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
    {
        std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(input);
        if (colors.size() < BIT_NUM_MAX)
        {
            Logger::SetError(BIT_SIZE_MIN_EXCEPTION);

            return NULL;
        }

        std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(input);

        Processor::BucketResult *result;

        if (conversionType == IO::CONVERSION_TYPES::PALETTE_COLORFUL)
        {
            result = Processor::generateColorBucketsRGB(input, image);
        }
        else if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
        {
            result =
                Processor::generateColorBucketsBW(input, image);
        }

        std::optional<std::map<int, std::vector<int>>> compounds;

        std::vector<int> indeces = result->getIndeces();

        if (lossyCompressionType == IO::LOSSY_COMPRESSION_TYPES::DCT) {
            indeces = Service::compressDCTImageInt(indeces, input);
        }

        if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::BYTE_RUN)
        {
            indeces = Service::compressByteRunImageInt(indeces);
        }
        else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::RLE)
        {
            indeces = Service::compressRLEImageInt(indeces);
        }
        else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZW)
        {
            auto compressionBuffer = Service::compressLZWImageInt(indeces);

            compounds = std::optional<std::map<int, std::vector<int>>>{compressionBuffer->getCompounds()};

            indeces = compressionBuffer->getResult();
        }
        else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
        {
            auto compressionBuffer = Service::compressLZ77ImageInt(indeces);

            for (auto value : compressionBuffer)
            {
                indeces.push_back(value->getDistance());

                indeces.push_back(value->getLength());

                indeces.push_back(value->getSymbol());
            }
        }

        Service::saveMetadata(
            conversionType,
            bitType,
            modelType,
            losslessCompressionType,
            losslessCompressionType != IO::LOSSLESS_COMPRESSION_TYPES::NONE ? indeces.size() : 0,
            lossyCompressionType,
            samplingType,
            filterType,
            dithering,
            input->w,
            input->h,
            result->getColors().size(),
            outputStream);

        std::vector<SDL_Color> internal = result->getColors();

        switch (modelType)
        {
        case IO::MODEL_TYPES::YCBCR:
            Service::convertToYCbCr(internal);

            if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
            {
                Service::convertToYCbCrBW(internal);
            }

            break;
        case IO::MODEL_TYPES::YIQ:
            Service::convertToYIQ(internal);

            if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
            {
                Service::convertToYIQBW(internal);
            }

            break;
        case IO::MODEL_TYPES::YUV:
            Service::convertToYUV(internal);

            if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
            {
                Service::convertToYUVBW(internal);
            }

            break;
        case IO::MODEL_TYPES::HSL:
            Service::convertToHSL(internal);

            if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
            {
                Service::convertToHSLBW(internal);
            }

            break;
        case IO::MODEL_TYPES::RGB:
            if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
            {
                Service::convertToRGBBW(internal);
            }
        }

        if (samplingType == IO::SAMPLING_TYPES::FOUR_TWO_ONE)
        {
            if (modelType == IO::MODEL_TYPES::RGB)
            {
                internal = Service::sampleFourToOneRGB(internal, input);
            }
            else if (modelType == IO::MODEL_TYPES::YUV)
            {
                internal = Service::sampleFourToOneYUV(internal, input);
            }
            else if (modelType == IO::MODEL_TYPES::YIQ)
            {
                internal = Service::sampleFourToOneYIQ(internal, input);
            }
            else if (modelType == IO::MODEL_TYPES::YCBCR)
            {
                internal = Service::sampleFourToOneYCbCr(internal, input);
            }
            else if (modelType == IO::MODEL_TYPES::HSL)
            {
                internal = Service::sampleFourToOneHSL(internal, input);
            }
        }

        if (bitType == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> sevenBitColors;

            if (conversionType == IO::CONVERSION_TYPES::PALETTE_COLORFUL)
            {
                sevenBitColors = Service::convertTo7BitColorful(internal);
            }
            else
            {
                std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

                for (int i = 0; i < internal.size(); i += ORIGINAL_BIT_NUM_PER_PIXEL)
                {
                    assemble.clear();

                    for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
                    {
                        switch (modelType)
                        {
                        case IO::MODEL_TYPES::YCBCR:
                            assemble.push_back(Processor::convert24BitYCbCrTo7BitGrey(internal[i + j]));

                            break;
                        case IO::MODEL_TYPES::YUV:
                            assemble.push_back(Processor::convert24BitYUVTo7BitGrey(internal[i + j]));

                            break;
                        case IO::MODEL_TYPES::YIQ:
                            assemble.push_back(Processor::convert24BitYIQTo7BitGrey(internal[i + j]));

                            break;
                        case IO::MODEL_TYPES::HSL:
                            assemble.push_back(Processor::convert24BitHSLTo7BitGrey(internal[i + j]));

                            break;
                        case IO::MODEL_TYPES::RGB:
                            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(internal[i + j]));

                            break;
                        }
                    }

                    sevenBitColors.push_back(Processor::convert8BitTo7Bit(assemble));
                }
            }

            for (std::vector<Uint8> &value : sevenBitColors)
            {
                outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
            }
        }
        else if (bitType == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> fifteenBitColors = Service::convertTo15Bit(internal);

            outputStream.write((char *)(fifteenBitColors.data()), fifteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> sixteenBitColors = Service::convertTo16Bit(internal);

            outputStream.write((char *)(sixteenBitColors.data()), sixteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> twentyFourBitColors = Service::convertTo24Bit(internal);

            for (std::vector<Uint8> &value : twentyFourBitColors)
            {
                outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
            }
        }

        if (compounds.has_value())
        {
            std::map<int, std::vector<int>> values = compounds.value();

            int valuesSize = values.size();

            outputStream.write((char *)(&valuesSize), sizeof(int));

            for (auto value : values)
            {
                int valueFirst = value.first;

                outputStream.write((char *)(&valueFirst), sizeof(int));

                int valueSecondSize = value.second.size();

                outputStream.write((char *)(&valueSecondSize), sizeof(int));

                outputStream.write((char *)(value.second.data()), value.second.size() * sizeof(int));
            }
        }

        outputStream.write((char *)(indeces.data()), indeces.size() * sizeof(int));

        return EXIT_SUCCESS;
    }

    Logger::SetError(NO_CONVERSION_TYPE_EXCEPTION);

    return EXIT_FAILURE;
}