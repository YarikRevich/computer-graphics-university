#include "converter.hpp"
#include <iostream>

int Converter::convertToCGUNativeColorful(SDL_Surface *surface)
{
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(image[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeNativeMetadata(
    //         IO::CONVERSION_TYPES::NATIVE_COLORFUL, 0, surface->w, surface->h);

    // metadata->writeTo(outputStream);

    for (std::vector<Uint8> &value : buff)
    {
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUNativeColorfulDithering(SDL_Surface *surface)
{
    std::vector<Processor::PixelPoint> result =
        Processor::generateFloydSteinbergDitheringRGB(surface);

    Processor::setPixels(surface, result);

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(image[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeNativeMetadata(
    //         IO::CONVERSION_TYPES::NATIVE_COLORFUL, IO::FileMetadata::DITHERING_FLAG, surface->w, surface->h);

    // metadata->writeTo(outputStream);

    for (std::vector<Uint8> &value : buff)
    {
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUNativeBW(SDL_Surface *surface)
{
    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(image[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeNativeMetadata(
    //         IO::CONVERSION_TYPES::NATIVE_BW, 0, surface->w, surface->h);

    // metadata->writeTo(outputStream);

    for (std::vector<Uint8> &value : buff)
    {
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUNativeBWDithering(SDL_Surface *surface)
{
    std::vector<Processor::PixelPoint> result =
        Processor::generateFloydSteinbergDitheringBW(surface);

    Processor::setPixels(surface, result);

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < Processor::getPixelAmount(surface); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(image[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeNativeMetadata(
    //         IO::CONVERSION_TYPES::NATIVE_BW, IO::FileMetadata::DITHERING_FLAG, surface->w, surface->h);

    // metadata->writeTo(outputStream);

    for (std::vector<Uint8> &value : buff)
    {
        outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    }

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteColorful(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    Processor::BucketResult *result =
        Processor::generateColorBucketsRGB(surface, image);

    std::vector<Uint32> indeces;

    for (auto &value : result->getColors())
    {
        indeces.push_back(Processor::convertColorToUint32(value));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeIndecesMetadata(
    //         IO::CONVERSION_TYPES::PALETTE_COLORFUL, 0, surface->w, surface->h, indeces);

    // metadata->writeTo(outputStream);

    outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteColorfulDithering(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    Processor::BucketResult *result =
        Processor::generateColorBucketsRGB(surface, image);

    std::vector<Uint32> indeces;

    for (auto &value : result->getColors())
    {
        indeces.push_back(Processor::convertColorToUint32(value));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeIndecesMetadata(
    //         IO::CONVERSION_TYPES::PALETTE_COLORFUL, IO::FileMetadata::DITHERING_FLAG, surface->w, surface->h, indeces);

    // metadata->writeTo(outputStream);

    outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteBW(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    Processor::BucketResult *result =
        Processor::generateColorBucketsBW(surface, image);

    std::vector<Uint32> indeces;

    for (auto &value : result->getColors())
    {
        indeces.push_back(Processor::convertColorToUint32(value));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeIndecesMetadata(
    //         IO::CONVERSION_TYPES::PALETTE_BW, 0, surface->w, surface->h, indeces);

    // metadata->writeTo(outputStream);

    outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteBWDithering(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);
        return EXIT_FAILURE;
    }

    std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

    Processor::BucketResult *result =
        Processor::generateColorBucketsBW(surface, image);

    std::vector<Uint32> indeces;

    for (auto &value : result->getColors())
    {
        indeces.push_back(Processor::convertColorToUint32(value));
    }

    // IO::FileMetadata *metadata =
    //     IO::composeIndecesMetadata(
    //         IO::CONVERSION_TYPES::PALETTE_BW, IO::FileMetadata::DITHERING_FLAG, surface->w, surface->h, indeces);

    // metadata->writeTo(outputStream);

    outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    return EXIT_SUCCESS;
}

int Converter::convertToCGUPaletteDetected(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateDedicatedPalette(surface, colors);

    Processor::cleanSurface(surface);
    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

SDL_Surface *Converter::convertFromCGUNativeColorful(std::ifstream &inputStream, IO::FileMetadata *metadata)
{
    inputStream.seekg(metadata->getSize());

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> input(PREFERRED_BIT_NUM_PER_PIXEL, 0);
    for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
    {
        inputStream.read((char *)(input.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

        buff.push_back(input);
    }

    std::vector<SDL_Color> image;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : buff)
    {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble)
        {
            image.push_back(Processor::convert7BitRGBTo24BitRGB(compound));
        }
    }

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++)
    {
        if (y == surface->h)
        {
            x += 1;
            y = 0;
        }

        Processor::setPixel(surface, x, y, image[k]);

        if (x == surface->w)
        {
            y += 1;
            x = 0;
        }
        else
        {
            y += 1;
        }
    }

    return surface;
};

SDL_Surface *Converter::convertFromCGUNativeBW(std::ifstream &inputStream, IO::FileMetadata *metadata)
{
    inputStream.seekg(metadata->getSize());

    std::vector<std::vector<Uint8>> buff;

    std::vector<Uint8> input(PREFERRED_BIT_NUM_PER_PIXEL, 0);
    for (int i = 0; i < ((metadata->getWidth() * metadata->getHeight()) / ORIGINAL_BIT_NUM_PER_PIXEL); i++)
    {
        inputStream.read((char *)(input.data()), PREFERRED_BIT_NUM_PER_PIXEL * sizeof(Uint8));

        buff.push_back(input);
    }

    std::vector<SDL_Color> image;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : buff)
    {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble)
        {
            image.push_back(Processor::convert7BitGreyTo24BitRGB(compound));
        }
    }

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++)
    {
        if (y == surface->h)
        {
            x += 1;
            y = 0;
        }

        Processor::setPixel(surface, x, y, image[k]);

        if (x == surface->w)
        {
            y += 1;
            x = 0;
        }
        else
        {
            y += 1;
        }
    }

    return surface;
}

SDL_Surface *Converter::convertFromCGUPaletteColorful(std::ifstream &inputStream, IO::FileMetadata *metadata)
{
    inputStream.seekg(metadata->getSize());

    std::vector<SDL_Color> colors;

    for (auto &value : metadata->getIndeces())
    {
        colors.push_back(Processor::convertUint32ToColor(value));
    }

    std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);
    inputStream.read((char *)(input.data()), input.size() * sizeof(int));

    std::vector<SDL_Color> image;
    for (auto &value : input)
    {
        image.push_back(colors[value]);
    }

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++)
    {
        if (y == surface->h)
        {
            x += 1;
            y = 0;
        }

        Processor::setPixel(surface, x, y, image[k]);

        if (x == surface->w)
        {
            y += 1;
            x = 0;
        }
        else
        {
            y += 1;
        }
    }

    if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
    {
        std::vector<Processor::PixelPoint> dithered =
            Processor::generateFloydSteinbergDitheringRGB(surface);

        Processor::setPixels(surface, dithered);
    }

    return surface;
};

SDL_Surface *Converter::convertFromCGUPaletteBW(std::ifstream &inputStream, IO::FileMetadata *metadata)
{
    inputStream.seekg(metadata->getSize());

    std::vector<SDL_Color> colors;

    for (auto &value : metadata->getIndeces())
    {
        colors.push_back(Processor::convertUint32ToColor(value));
    }

    std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);
    inputStream.read((char *)(input.data()), input.size() * sizeof(int));

    std::vector<SDL_Color> image;
    for (auto &value : input)
    {
        image.push_back(colors[value]);
    }

    SDL_Surface *surface =
        SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

    int x = 0;
    int y = 0;

    for (int k = 0; k < image.size(); k++)
    {
        if (y == surface->h)
        {
            x += 1;
            y = 0;
        }

        Processor::setPixel(surface, x, y, image[k]);

        if (x == surface->w)
        {
            y += 1;
            x = 0;
        }
        else
        {
            y += 1;
        }
    }

    if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
    {
        std::vector<Processor::PixelPoint> dithered =
            Processor::generateFloydSteinbergDitheringBW(surface);

        Processor::setPixels(surface, dithered);
    }

    return surface;
};

void Converter::composeMetadata(
    std::string conversion,
    std::string bit,
    std::string model,
    std::string compression,
    bool dithering,
    int width,
    int height,
    std::vector<Uint32> indeces,
    std::ofstream &outputStream)
{
    IO::FileMetadata* metadata = 
        IO::composeNativeMetadata(
            conversionType, 0, 0, 0, IO::FileMetadata::DITHERING_FLAG, input->w, input->h);

    metadata->writeTo(outputStream);
}