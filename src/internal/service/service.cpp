#include "service.hpp"

#include <iostream>

Service::NativeConversionResult* Service::convertToCGUNativeColorful(SDL_Surface *surface)
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

    // for (std::vector<Uint8> &value : buff)
    // {
    //     outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    // }

    return new Service::NativeConversionResult(buff);
}

Service::NativeConversionResult* Service::convertToCGUNativeBW(SDL_Surface *surface)
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

    // for (std::vector<Uint8> &value : buff)
    // {
    //     outputStream.write((char *)(value.data()), value.size() * sizeof(Uint8));
    // }

    return new Service::NativeConversionResult(buff);
}



Service::PaletteConversionResult* Service::convertToCGUPaletteColorful(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);

        return NULL;
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

    // outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    // return EXIT_SUCCESS;

    return new Service::PaletteConversionResult(result->getIndeces(), indeces);
}

Service::PaletteConversionResult* Service::convertToCGUPaletteBW(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
    if (colors.size() < BIT_NUM_MAX)
    {
        Logger::SetError(BIT_SIZE_MIN_EXCEPTION);

        return NULL;
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

    // outputStream.write((char *)(result->getIndeces().data()), result->getIndeces().size() * sizeof(int));

    return new Service::PaletteConversionResult(result->getIndeces(), indeces);
}

int Service::applyColorfulDithering(SDL_Surface* surface) {
    std::vector<Processor::PixelPoint> result =
        Processor::generateFloydSteinbergDitheringRGB(surface);

    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Service::applyBWDithering(SDL_Surface* surface) {
    std::vector<Processor::PixelPoint> result =
        Processor::generateFloydSteinbergDitheringBW(surface);

    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Service::convertToCGUPaletteDetected(SDL_Surface *surface)
{
    std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);

    std::vector<Processor::PixelPoint> result =
        Processor::generateDedicatedPalette(surface, colors);

    Processor::cleanSurface(surface);

    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

SDL_Surface *Service::convertFromCGUNativeColorful(std::ifstream &inputStream, IO::FileMetadata *metadata)
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

SDL_Surface *Service::convertFromCGUNativeBW(std::ifstream &inputStream, IO::FileMetadata *metadata)
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

SDL_Surface *Service::convertFromCGUPaletteColorful(std::ifstream &inputStream, IO::FileMetadata *metadata)
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

SDL_Surface *Service::convertFromCGUPaletteBW(std::ifstream &inputStream, IO::FileMetadata *metadata)
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

// void getDCTMatrix(Uint8 intput[BLOCK_SIZE][BLOCK_SIZE], float output[BLOCK_SIZE][BLOCK_SIZE])
// {
//     float result[BLOCK_SIZE][BLOCK_SIZE];

//     double compound;

//     double cu;
//     double cv;

//     for (int v = 0; v < BLOCK_SIZE; ++v)
//     {
//         for (int u = 0; u < BLOCK_SIZE; ++u)
//         {
//             if (u == 0)
//             {
//                 cu = 1.0 / sqrt(2);
//             }
//             else
//             {
//                 cu = 1.0;
//             }

//             if (v == 0)
//             {
//                 cv = 1.0 / sqrt(2);
//             }
//             else
//             {
//                 cv = 1.0;
//             }

//             compound = 0;

//             for (int y = 0; y < BLOCK_SIZE; ++y)
//             {
//                 for (int x = 0; x < BLOCK_SIZE; ++x)
//                 {
//                     compound += (double)intput[x][y] *
//                                 cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)BLOCK_SIZE)) *
//                                 cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)BLOCK_SIZE));
//                 }
//             }

//             compound *= (2.0 / (double)BLOCK_SIZE) * cu * cv;

//             result[u][v] = compound;
//         }
//     }

//     for (int j = 0; j < BLOCK_SIZE; j++)
//     {
//         for (int i = 0; i < BLOCK_SIZE; i++)
//         {
//             output[i][j] = result[i][j];
//         }
//     }
// }

// void getInversedDCTMatrix(float input[BLOCK_SIZE][BLOCK_SIZE], Uint8 output[BLOCK_SIZE][BLOCK_SIZE])
// {
//     int result[BLOCK_SIZE][BLOCK_SIZE];

//     double pixel;

//     double cu;
//     double cv;

//     for (int x = 0; x < BLOCK_SIZE; ++x)
//     {
//         for (int y = 0; y < BLOCK_SIZE; ++y)
//         {
//             pixel = 0;

//             for (int u = 0; u < BLOCK_SIZE; ++u)
//             {
//                 for (int v = 0; v < BLOCK_SIZE; ++v)
//                 {
//                     if (u == 0)
//                     {
//                         cu = 1.0 / sqrt(2);
//                     }
//                     else
//                     {
//                         cu = 1.0;
//                     }

//                     if (v == 0)
//                     {
//                         cv = 1.0 / sqrt(2);
//                     }
//                     else
//                     {
//                         cv = 1.0;
//                     }

//                     pixel += input[u][v] *
//                              cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)BLOCK_SIZE)) *
//                              cu *
//                              cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)BLOCK_SIZE)) *
//                              cv;
//                 }
//             }
//             pixel *= (2.0 / (double)BLOCK_SIZE);

//             result[x][y] = round(pixel);
//         }
//     }

//     for (int j = 0; j < BLOCK_SIZE; j++)
//     {
//         for (int i = 0; i < BLOCK_SIZE; i++)
//         {
//             if (result[i][j] > 255)
//             {
//                 result[i][j] = 255;
//             }

//             if (result[i][j] < 0)
//             {
//                 result[i][j] = 0;
//             }

//             output[i][j] = result[i][j];
//         }
//     }
// }

void Service::saveMetadata(
    IO::CONVERSION_TYPES conversionType,
    IO::BIT_TYPES bitType,
    IO::MODEL_TYPES modelType,
    IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType,
    IO::LOSSY_COMPRESSION_TYPES lossyCompressionType,
    IO::SAMPLING_TYPES samplingType,
    IO::FILTER_TYPES filterType,
    bool dithering,
    int width,
    int height,
    std::optional<std::vector<Uint32>> indeces,
    std::ofstream &outputStream)
{
    IO::FileMetadata* metadata = 
        IO::composeMetadata(
            conversionType, 
            bitType, 
            modelType, 
            losslessCompressionType, 
            lossyCompressionType,
            samplingType,
            filterType,
            IO::FileMetadata::DITHERING_FLAG, 
            width, 
            height,
            indeces);

    metadata->writeTo(outputStream);
}