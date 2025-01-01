#include "service.hpp"

#include <iostream>

void Service::convertToYCbCr(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors.at(i) = Processor::convertRGBToYCbCr(colors.at(i));
    }
}

void Service::convertFromYCbCr(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors.at(i);

        colors.at(i) = Processor::convertYCbCrToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToYIQ(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors.at(i) = Processor::convertRGBToYIQ(colors.at(i));
    }
}

void Service::convertFromYIQ(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors.at(i);

        colors.at(i) = Processor::convertYIQToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToYUV(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors.at(i) = Processor::convertRGBToYUV(colors.at(i));
    }
}

void Service::convertFromYUV(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors.at(i);

        colors.at(i) = Processor::convertYUVToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToHSL(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors.at(i) = Processor::convertRGBToHSL(colors.at(i));
    }
}

void Service::convertFromHSL(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors.at(i);

        colors.at(i) = Processor::convertHSLToRGB(color.r, color.g, color.b);
    }
}

std::vector<std::vector<Uint8>> Service::convertTo24Bit(std::vector<SDL_Color> &colors)
{
    std::vector<std::vector<Uint8>> buff;

    for (int i = 0; i < colors.size(); i++)
    {
        std::vector<Uint8> assemble;

        assemble.push_back(colors[i].r);
        assemble.push_back(colors[i].g);
        assemble.push_back(colors[i].b);

        buff.push_back(assemble);
    }

    return buff;
};

std::vector<SDL_Color> Service::convertFrom24Bit(std::vector<std::vector<Uint8>> &colors)
{
    std::vector<SDL_Color> buff;

    for (std::vector<Uint8> &value : colors)
    {
        SDL_Color color;

        color.r = value.at(0);
        color.g = value.at(1);
        color.b = value.at(2);

        buff.push_back(color);
    }

    return buff;
}

std::vector<Uint16> Service::convertTo16Bit(std::vector<SDL_Color> &colors)
{
    std::vector<Uint16> buff;

    for (int i = 0; i < colors.size(); i++)
    {
        buff.push_back(Processor::convert24BitColorTo16BitColor(colors[i]));
    }

    return buff;
};

std::vector<SDL_Color> Service::convertFrom16Bit(std::vector<Uint16> &colors)
{
    std::vector<SDL_Color> buff;

    for (Uint16 value : colors)
    {
        buff.push_back(Processor::convert16BitColorTo24BitColor(value));
    }

    return buff;
}

std::vector<Uint16> Service::convertTo15Bit(std::vector<SDL_Color> &colors)
{
    std::vector<Uint16> buff;

    for (int i = 0; i < colors.size(); i++)
    {
        buff.push_back(Processor::convert24BitColorTo15BitColor(colors[i]));
    }

    return buff;
};

std::vector<SDL_Color> Service::convertFrom15Bit(std::vector<Uint16> &colors)
{
    std::vector<SDL_Color> buff;

    for (Uint16 value : colors)
    {
        buff.push_back(Processor::convert15BitColorTo24BitColor(value));
    }

    return buff;
}

std::vector<std::vector<Uint8>> Service::convertTo7BitColorful(std::vector<SDL_Color> &colors)
{
    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < colors.size(); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitRGB(colors[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    return buff;
};

std::vector<SDL_Color> Service::convertFrom7BitColorful(std::vector<std::vector<Uint8>> &colors)
{
    std::vector<SDL_Color> buff;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : colors)
    {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble)
        {
            buff.push_back(Processor::convert7BitRGBTo24BitRGB(compound));
        }
    }

    return buff;
};

std::vector<std::vector<Uint8>> Service::convertTo7BitBW(std::vector<SDL_Color> &colors)
{
    std::vector<std::vector<Uint8>> buff;
    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

    for (int i = 0; i < colors.size(); i += ORIGINAL_BIT_NUM_PER_PIXEL)
    {
        assemble.clear();

        for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
        {
            assemble.push_back(Processor::convert24BitRGBTo7BitGrey(colors[i + j]));
        }

        buff.push_back(Processor::convert8BitTo7Bit(assemble));
    }

    return buff;
};

std::vector<SDL_Color> Service::convertFrom7BitBW(std::vector<std::vector<Uint8>> &colors)
{
    std::vector<SDL_Color> buff;

    std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
    for (auto &value : colors)
    {
        assemble = Processor::convert7BitTo8Bit(value);

        for (auto &compound : assemble)
        {
            buff.push_back(Processor::convert7BitGreyTo24BitRGB(compound));
        }
    }

    return buff;
};

void Service::convertToBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors.at(i);

        colors.at(i) = Processor::convertColorToGrey(color);
        ;
    }
};

std::vector<SDL_Color> Service::sampleFourToOneRGB(std::vector<SDL_Color>& colors, SDL_Surface *surface)
{

    std::vector<SDL_Color> result = colors;

    for (int x = 0; x < surface->w; x += 2)
    {
        for (int y = 0; y < surface->h; y += 2)
        {
            // Calculate indices safely
            int idx1 = (y * surface->w) + x;
            int idx2 = (y * surface->w) + (x + 1);
            int idx3 = ((y + 1) * surface->w) + x;
            int idx4 = ((y + 1) * surface->w) + (x + 1);

            if (idx1 >= colors.size() || idx2 >= colors.size() || 
                idx3 >= colors.size() || idx4 >= colors.size()) {
                continue;
            }

            SDL_Color first = colors[idx1];
            SDL_Color second = colors[idx2];
            SDL_Color third = colors[idx3];
            SDL_Color forth = colors[idx4];

            // Average the four pixels
            SDL_Color averaged;
            averaged.r = (first.r + second.r + third.r + forth.r) / 4;
            averaged.g = (first.g + second.g + third.g + forth.g) / 4;
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;
            averaged.a = (first.a + second.a + third.a + forth.a) / 4;

            result[idx1] = averaged;
            result[idx2] = averaged;
            result[idx3] = averaged;
            result[idx4] = averaged;
        }
    }

    return result;

    // std::vector<SDL_Color> result = colors;

    // for (int x = 0; x < surface->w; x += 2)
    // {
    //     for (int y = 0; y < surface->h; y += 2)
    //     {
    //         int idx1 = (y * surface->w) + x;
    //         int idx2 = (y * surface->w) + (x + 1);
    //         int idx3 = ((y + 1) * surface->w) + x;
    //         int idx4 = ((y + 1) * surface->w) + (x + 1);

    //         if (idx1 >= colors.size() || idx2 >= colors.size() || 
    //             idx3 >= colors.size() || idx4 >= colors.size()) {
    //             continue; // Safety check
    //         }

    //         SDL_Color first = colors[idx1];
    //         SDL_Color second = colors[idx2];
    //         SDL_Color third = colors[idx3];
    //         SDL_Color forth = colors[idx4];

            // SDL_Color first;

            // if (y + (surface->w * x) <= colors.size() - 1) {
            //     first = colors.at(y + (surface->w * x));
            // } else {
            //     continue;
            // }

            // SDL_Color second;

            // if (y + (surface->w * (x + 1)) <= colors.size() - 1) {
            //     second = colors.at(y + (surface->w * (x + 1)));
            // } else {
            //     continue;
            // }

            // SDL_Color third;

            // if ((y + 1) + (surface->w * x) <= colors.size() - 1) {
            //     third = colors.at((y + 1) + (surface->w * x));
            // } else {
            //     continue;
            // }

            // SDL_Color forth;

            // if ((y + 1) + (surface->w * (x + 1)) <= colors.size() - 1) {
            //     forth = colors.at((y + 1) + (surface->w * (x + 1)));
            // } else {
            //     continue;
            // }

    //         float r = (first.r + second.r + third.r + forth.r) / 4.0;

    //         first.r = r;
    //         second.r = r;
    //         third.r = r;
    //         forth.r = r;

    //         result.at(idx1) = first;
    //         result.at(idx2) = second;
    //         result.at(idx3) = third;
    //         result.at(idx4) = forth;
    //     }
    // }

    // return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYUV(std::vector<SDL_Color> &colors, SDL_Surface *surface)
{
    std::vector<SDL_Color> result(colors.size());

    for (int x = 0; x < surface->w; x += 2)
    {
        for (int y = 0; y < surface->h; y += 2)
        {
            SDL_Color first;

            if (y + (surface->w * x) <= colors.size() - 1) {
                first = colors.at(y + (surface->w * x));
            }

            SDL_Color second;

            if (y + (surface->w * (x + 1)) <= colors.size() - 1) {
                second = colors.at(y + (surface->w * (x + 1)));
            }

            SDL_Color third;

            if ((y + 1) + (surface->w * x) <= colors.size() - 1) {
                third = colors.at((y + 1) + (surface->w * x));
            }

            SDL_Color forth;

            if ((y + 1) + (surface->w * (x + 1)) <= colors.size() - 1) {
                forth = colors.at((y + 1) + (surface->w * (x + 1)));
            }

            float u = (first.g + second.g + third.g + forth.g) / 4.0;

            first.g = u;
            second.g = u;
            third.g = u;
            forth.g = u;

            float v = (first.b + second.b + third.b + forth.b) / 4.0;

            first.b = v;
            second.b = v;
            third.b = v;
            forth.b = v;

            result.at(y + (surface->w * x)) = first;
            result.at(y + (surface->w * (x + 1))) = second;
            result.at((y + 1) + (surface->w * x)) = third;
            result.at((y + 1) + (surface->w * (x + 1))) = forth;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYIQ(std::vector<SDL_Color> &colors, SDL_Surface *surface)
{
    std::vector<SDL_Color> result(colors.size());

    for (int x = 0; x < surface->w; x += 2)
    {
        for (int y = 0; y < surface->h; y += 2)
        {
            SDL_Color first;

            if (y + (surface->w * x) <= colors.size() - 1) {
                first = colors.at(y + (surface->w * x));
            }

            SDL_Color second;

            if (y + (surface->w * (x + 1)) <= colors.size() - 1) {
                second = colors.at(y + (surface->w * (x + 1)));
            }

            SDL_Color third;

            if ((y + 1) + (surface->w * x) <= colors.size() - 1) {
                third = colors.at((y + 1) + (surface->w * x));
            }

            SDL_Color forth;

            if ((y + 1) + (surface->w * (x + 1)) <= colors.size() - 1) {
                forth = colors.at((y + 1) + (surface->w * (x + 1)));
            }

            float i = (first.g + second.g + third.g + forth.g) / 4.0;

            first.g = i;
            second.g = i;
            third.g = i;
            forth.g = i;

            float q = (first.b + second.b + third.b + forth.b) / 4.0;

            first.b = q;
            second.b = q;
            third.b = q;
            forth.b = q;

            result.at(y + (surface->w * x)) = first;
            result.at(y + (surface->w * (x + 1))) = second;
            result.at((y + 1) + (surface->w * x)) = third;
            result.at((y + 1) + (surface->w * (x + 1))) = forth;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYCbCr(std::vector<SDL_Color> &colors, SDL_Surface *surface)
{
    std::vector<SDL_Color> result(colors.size());

    for (int x = 0; x < surface->w; x += 2)
    {
        for (int y = 0; y < surface->h; y += 2)
        {
            SDL_Color first;

            if (y + (surface->w * x) <= colors.size() - 1) {
                first = colors.at(y + (surface->w * x));
            }

            SDL_Color second;

            if (y + (surface->w * (x + 1)) <= colors.size() - 1) {
                second = colors.at(y + (surface->w * (x + 1)));
            }

            SDL_Color third;

            if ((y + 1) + (surface->w * x) <= colors.size() - 1) {
                third = colors.at((y + 1) + (surface->w * x));
            }

            SDL_Color forth;

            if ((y + 1) + (surface->w * (x + 1)) <= colors.size() - 1) {
                forth = colors.at((y + 1) + (surface->w * (x + 1)));
            }

            float cb = (first.g + second.g + third.g + forth.g) / 4.0;

            first.g = cb;
            second.g = cb;
            third.g = cb;
            forth.g = cb;

            float cr = (first.b + second.b + third.b + forth.b) / 4.0;

            first.b = cr;
            second.b = cr;
            third.b = cr;
            forth.b = cr;

            result.at(y + (surface->w * x)) = first;
            result.at(y + (surface->w * (x + 1))) = second;
            result.at((y + 1) + (surface->w * x)) = third;
            result.at((y + 1) + (surface->w * (x + 1))) = forth;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneHSL(std::vector<SDL_Color> &colors, SDL_Surface *surface)
{
    std::vector<SDL_Color> result(colors.size());

    for (int x = 0; x < surface->w; x += 2)
    {
        for (int y = 0; y < surface->h; y += 2)
        {
            SDL_Color first;

            if (y + (surface->w * x) <= colors.size() - 1) {
                first = colors.at(y + (surface->w * x));
            }

            SDL_Color second;

            if (y + (surface->w * (x + 1)) <= colors.size() - 1) {
                second = colors.at(y + (surface->w * (x + 1)));
            }

            SDL_Color third;

            if ((y + 1) + (surface->w * x) <= colors.size() - 1) {
                third = colors.at((y + 1) + (surface->w * x));
            }

            SDL_Color forth;

            if ((y + 1) + (surface->w * (x + 1)) <= colors.size() - 1) {
                forth = colors.at((y + 1) + (surface->w * (x + 1)));
            }

            float s = (first.g + second.g + third.g + forth.g) / 4.0;

            first.g = s;
            second.g = s;
            third.g = s;
            forth.g = s;

            result.at(y + (surface->w * x)) = first;
            result.at(y + (surface->w * (x + 1))) = second;
            result.at((y + 1) + (surface->w * x)) = third;
            result.at((y + 1) + (surface->w * (x + 1))) = forth;
        }
    }

    return result;
}

// Service::PaletteConversionResult *Service::convertToCGUPaletteColorful(SDL_Surface *surface)
// {
//     std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
//     if (colors.size() < BIT_NUM_MAX)
//     {
//         Logger::SetError(BIT_SIZE_MIN_EXCEPTION);

//         return NULL;
//     }

//     std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

//     Processor::BucketResult *result =
//         Processor::generateColorBucketsRGB(surface, image);

//     std::vector<Uint32> indeces;

//     for (auto &value : result->getColors())
//     {
//         indeces.push_back(Processor::convertColorToUint32(value));
//     }

//     return new Service::PaletteConversionResult(result->getIndeces(), indeces);
// }

// Service::PaletteConversionResult *Service::convertToCGUPaletteBW(SDL_Surface *surface)
// {
//     std::vector<SDL_Color> colors = Processor::getReducedBitColorMap(surface);
//     if (colors.size() < BIT_NUM_MAX)
//     {
//         Logger::SetError(BIT_SIZE_MIN_EXCEPTION);

//         return NULL;
//     }

//     std::vector<SDL_Color> image = Processor::getCompleteBitColorMap(surface);

//     Processor::BucketResult *result =
//         Processor::generateColorBucketsBW(surface, image);

//     std::vector<Uint32> indeces;

//     for (auto &value : result->getColors())
//     {
//         indeces.push_back(Processor::convertColorToUint32(value));
//     }

//     return new Service::PaletteConversionResult(result->getIndeces(), indeces);
//

int Service::applyColorfulDithering(SDL_Surface *surface)
{
    std::vector<Processor::PixelPoint> result =
        Processor::generateFloydSteinbergDitheringRGB(surface);

    Processor::setPixels(surface, result);

    return EXIT_SUCCESS;
}

int Service::applyBWDithering(SDL_Surface *surface)
{
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

// SDL_Surface *Service::convertFromCGUPaletteColorful(std::ifstream &inputStream, IO::FileMetadata *metadata)
// {
//     inputStream.seekg(metadata->getSize());

//     std::vector<SDL_Color> colors;

//     for (auto &value : metadata->getIndeces())
//     {
//         colors.push_back(Processor::convertUint32ToColor(value));
//     }

//     std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);
//     inputStream.read((char *)(input.data()), input.size() * sizeof(int));

//     std::vector<SDL_Color> image;
//     for (auto &value : input)
//     {
//         image.push_back(colors[value]);
//     }

//     SDL_Surface *surface =
//         SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

//     int x = 0;
//     int y = 0;

//     for (int k = 0; k < image.size(); k++)
//     {
//         if (y == surface->h)
//         {
//             x += 1;
//             y = 0;
//         }

//         Processor::setPixel(surface, x, y, image[k]);

//         if (x == surface->w)
//         {
//             y += 1;
//             x = 0;
//         }
//         else
//         {
//             y += 1;
//         }
//     }

//     return surface;
// };

// SDL_Surface *Service::convertFromCGUPaletteBW(std::ifstream &inputStream, IO::FileMetadata *metadata)
// {
//     inputStream.seekg(metadata->getSize());

//     std::vector<SDL_Color> colors;

//     for (auto &value : metadata->getIndeces())
//     {
//         colors.push_back(Processor::convertUint32ToColor(value));
//     }

//     std::vector<int> input(metadata->getWidth() * metadata->getHeight(), 0);
//     inputStream.read((char *)(input.data()), input.size() * sizeof(int));

//     std::vector<SDL_Color> image;
//     for (auto &value : input)
//     {
//         image.push_back(colors[value]);
//     }

//     SDL_Surface *surface =
//         SDL_CreateRGBSurface(0, metadata->getWidth(), metadata->getHeight(), 32, 0, 0, 0, 0);

//     int x = 0;
//     int y = 0;

//     for (int k = 0; k < image.size(); k++)
//     {
//         if (y == surface->h)
//         {
//             x += 1;
//             y = 0;
//         }

//         Processor::setPixel(surface, x, y, image[k]);

//         if (x == surface->w)
//         {
//             y += 1;
//             x = 0;
//         }
//         else
//         {
//             y += 1;
//         }
//     }

//     return surface;
// };

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
    int indecesSize,
    std::ofstream &outputStream)
{
    IO::FileMetadata *metadata = IO::composeMetadata(
        conversionType,
        bitType,
        modelType,
        losslessCompressionType,
        lossyCompressionType,
        samplingType,
        filterType,
        IO::FileMetadata::DITHERING_FLAG ? dithering : 0,
        width,
        height,
        indecesSize);

    metadata->writeTo(outputStream);
}