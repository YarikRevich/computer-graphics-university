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

            std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
            {
                inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                buff.push_back(internal);
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
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
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
            }
            else if (metadata->getLosslessCompression() == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
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

            std::vector<Uint8> internal(3, 0);
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight())); i++)
            {
                inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                buff.push_back(internal);
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
        inputStream.read((char *)(input.data()), input.size() * sizeof(int));

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

            std::vector<Uint8> internal(PREFERRED_BIT_NUM_PER_PIXEL, 0);
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
            {
                inputStream.read((char *)(internal.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

                buff.push_back(internal);
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

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            colors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            colors = Service::convertFrom16Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> buff;

            std::vector<Uint8> internal(3, 0);
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight())); i++)
            {
                inputStream.read((char *)(internal.data()), 3 * sizeof(Uint8));

                buff.push_back(internal);
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
        inputStream.read((char *)(input.data()), input.size() * sizeof(int));

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
            if (modelType == IO::MODEL_TYPES::RGB) {
                colors = Service::sampleFourToOneRGB(colors, input);
            } else if (modelType == IO::MODEL_TYPES::YUV) {
                colors = Service::sampleFourToOneYUV(colors, input);
            } else if (modelType == IO::MODEL_TYPES::YIQ) {
                colors = Service::sampleFourToOneYIQ(colors, input);
            } else if (modelType == IO::MODEL_TYPES::YCBCR) {
                colors = Service::sampleFourToOneYCbCr(colors, input);
            } else if (modelType == IO::MODEL_TYPES::HSL) {
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
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
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
            }
            else if (losslessCompressionType == IO::LOSSLESS_COMPRESSION_TYPES::LZ77)
            {
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
            if (modelType == IO::MODEL_TYPES::RGB) {
                internal = Service::sampleFourToOneRGB(internal, input);
            } else if (modelType == IO::MODEL_TYPES::YUV) {
                internal = Service::sampleFourToOneYUV(internal, input);
            } else if (modelType == IO::MODEL_TYPES::YIQ) {
                internal = Service::sampleFourToOneYIQ(internal, input);
            } else if (modelType == IO::MODEL_TYPES::YCBCR) {
                internal = Service::sampleFourToOneYCbCr(internal, input);
            } else if (modelType == IO::MODEL_TYPES::HSL) {
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

        outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

        return EXIT_SUCCESS;
    }

    Logger::SetError(NO_CONVERSION_TYPE_EXCEPTION);

    return EXIT_FAILURE;
}