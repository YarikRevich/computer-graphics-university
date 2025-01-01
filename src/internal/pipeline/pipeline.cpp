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
                colors = Service::convertFrom7BitBW(buff);
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
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
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
                encodedColors = Service::convertFrom7BitBW(buff);
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            encodedColors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            encodedColors = Service::convertFrom16Bit(buff);
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
                colors = Service::convertFrom7BitBW(buff);
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
            for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
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
                encodedColors = Service::convertFrom7BitBW(buff);
            }
        }
        else if (metadata->getBit() == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            encodedColors = Service::convertFrom15Bit(buff);
        }
        else if (metadata->getBit() == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> buff(metadata->getWidth() * metadata->getHeight(), 0);

            inputStream.read((char *)(buff.data()), metadata->getWidth() * metadata->getHeight() * sizeof(Uint16));

            encodedColors = Service::convertFrom16Bit(buff);
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
        return EXIT_FAILURE;
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
        switch (conversionType)
        {
        case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
            if (dithering)
            {
                if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
                {
                    return EXIT_FAILURE;
                }
            }

            break;
        case IO::CONVERSION_TYPES::NATIVE_BW:
            if (dithering)
            {
                if (Service::applyBWDithering(input) != EXIT_SUCCESS)
                {
                    return EXIT_FAILURE;
                }
            }

            break;
        }

        std::vector<SDL_Color> colors = Processor::getCompleteBitColorMap(input);

        if (conversionType == IO::CONVERSION_TYPES::NATIVE_BW)
        {
            Service::convertToBW(colors);
        }

        switch (modelType)
        {
        case IO::MODEL_TYPES::YCBCR:
            Service::convertToYCbCr(colors);

            break;
        case IO::MODEL_TYPES::YIQ:
            Service::convertToYIQ(colors);

            break;
        case IO::MODEL_TYPES::YUV:
            Service::convertToYUV(colors);

            break;
        case IO::MODEL_TYPES::HSL:
            Service::convertToHSL(colors);

            break;
        }

        if (samplingType == IO::SAMPLING_TYPES::TWO_TWO_ONE)
        {
            colors = Service::sampleFourToOne(colors, input);
        }

        Service::saveMetadata(
            conversionType,
            bitType,
            modelType,
            losslessCompressionType,
            lossyCompressionType,
            samplingType,
            filterType,
            dithering,
            input->w,
            input->h,
            0,
            outputStream);

        if (bitType == IO::BIT_TYPES::SEVEN)
        {
            std::vector<std::vector<Uint8>> sevenBitColors;

            if (conversionType == IO::CONVERSION_TYPES::NATIVE_COLORFUL)
            {
                sevenBitColors = Service::convertTo7BitColorful(colors);
            }
            else
            {
                sevenBitColors = Service::convertTo7BitBW(colors);
            }

            for (std::vector<Uint8> &value : sevenBitColors)
            {
                outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
            }
        }
        else if (bitType == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> fifteenBitColors = Service::convertTo15Bit(colors);

            outputStream.write((char *)(fifteenBitColors.data()), fifteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> sixteenBitColors = Service::convertTo16Bit(colors);

            outputStream.write((char *)(sixteenBitColors.data()), sixteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> twentyFourBitColors = Service::convertTo24Bit(colors);

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
            lossyCompressionType,
            samplingType,
            filterType,
            dithering,
            input->w,
            input->h,
            result->getIndeces().size(),
            outputStream);

        std::vector<SDL_Color> internal = result->getColors();

        if (conversionType == IO::CONVERSION_TYPES::PALETTE_BW)
        {
            Service::convertToBW(colors);
        }

        switch (modelType)
        {
        case IO::MODEL_TYPES::YCBCR:
            Service::convertToYCbCr(internal);

            break;
        case IO::MODEL_TYPES::YIQ:
            Service::convertToYIQ(internal);

            break;
        case IO::MODEL_TYPES::YUV:
            Service::convertToYUV(internal);

            break;
        case IO::MODEL_TYPES::HSL:
            Service::convertToHSL(internal);

            break;
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
                sevenBitColors = Service::convertTo7BitBW(colors);
            }

            for (std::vector<Uint8> &value : sevenBitColors)
            {
                outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
            }
        }
        else if (bitType == IO::BIT_TYPES::FIFTEEN)
        {
            std::vector<Uint16> fifteenBitColors = Service::convertTo15Bit(colors);

            outputStream.write((char *)(fifteenBitColors.data()), fifteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::SIXTEEN)
        {
            std::vector<Uint16> sixteenBitColors = Service::convertTo16Bit(colors);

            outputStream.write((char *)(sixteenBitColors.data()), sixteenBitColors.size() * sizeof(Uint16));
        }
        else if (bitType == IO::BIT_TYPES::TWENTY_FOUR)
        {
            std::vector<std::vector<Uint8>> twentyFourBitColors = Service::convertTo24Bit(colors);

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