#include "service.hpp"

#include <iostream>

void Service::convertToYCbCr(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = Processor::convertRGBToYCbCr(colors[i]);
    }
}

void Service::convertFromYCbCr(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors[i];

        colors[i] = Processor::convertYCbCrToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToYIQ(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = Processor::convertRGBToYIQ(colors[i]);
    }
}

void Service::convertFromYIQ(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors[i];

        colors[i] = Processor::convertYIQToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToYUV(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = Processor::convertRGBToYUV(colors[i]);
    }
}

void Service::convertFromYUV(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors[i];

        colors[i] = Processor::convertYUVToRGB(color.r, color.g, color.b);
    }
}

void Service::convertToHSL(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i] = Processor::convertRGBToHSL(colors[i]);
    }
}

void Service::convertFromHSL(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors[i];

        colors[i] = Processor::convertHSLToRGB(color.r, color.g, color.b);
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

        color.r = value[0];
        color.g = value[1];
        color.b = value[2];

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

// std::vector<std::vector<Uint8>> Service::convertTo7BitBW(std::vector<SDL_Color> &colors)
// {
//     std::vector<std::vector<Uint8>> buff;
//     std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL);

//     for (int i = 0; i < colors.size(); i += ORIGINAL_BIT_NUM_PER_PIXEL)
//     {
//         assemble.clear();

//         for (int j = 0; j < ORIGINAL_BIT_NUM_PER_PIXEL; j++)
//         {
//             assemble.push_back(Processor::convert24BitRGBTo7BitGrey(colors[i + j]));
//         }

//         buff.push_back(Processor::convert8BitTo7Bit(assemble));
//     }

//     return buff;
// };



// std::vector<SDL_Color> Service::convertFrom7BitBW(std::vector<std::vector<Uint8>> &colors)
// {
//     std::vector<SDL_Color> buff;

//     std::vector<Uint8> assemble(ORIGINAL_BIT_NUM_PER_PIXEL, 0);
//     for (auto &value : colors)
//     {
//         assemble = Processor::convert7BitTo8Bit(value);

//         for (auto &compound : assemble)
//         {
//             buff.push_back(Processor::convert7BitGreyTo24BitRGB(compound));
//         }
//     }

//     return buff;
// };

void Service::convertToRGBBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        SDL_Color color = colors[i];

        colors[i] = Processor::convertRGBToGrey(color);
    }
};

void Service::convertToYUVBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i].g = 0;
        colors[i].b = 0;
    }
};

void Service::convertToYIQBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i].g = 0;
        colors[i].b = 0;
    }
};

void Service::convertToYCbCrBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i].g = 128;
        colors[i].b = 128;
    }
};

void Service::convertToHSLBW(std::vector<SDL_Color> &colors)
{
    for (int i = 0; i < colors.size(); i++)
    {
        colors[i].g = 0;
        colors[i].r = 0;
    }
};

std::vector<SDL_Color> Service::sampleFourToOne(std::vector<SDL_Color> &colors, SDL_Surface *surface)
{
    std::vector<SDL_Color> result = colors;

    for (int x = 0; x <= surface->w; x += 2)
    {
        for (int y = 0; y <= surface->h; y += 2)
        {
            int index1 = y + (x * surface->h);
            int index2 = y + ((x + 1) * surface->h);
            int index3 = (y + 1) + (x * surface->h);
            int index4 = (y + 1) + ((x + 1) * surface->h);

            if (index1 >= colors.size() || index2 >= colors.size() ||
                index3 >= colors.size() || index4 >= colors.size())
            {
                continue;
            }

            SDL_Color first = colors[index1];
            SDL_Color second = colors[index2];
            SDL_Color third = colors[index3];
            SDL_Color forth = colors[index4];

            SDL_Color averaged;
            averaged.r = (first.r + second.r + third.r + forth.r) / 4;
            averaged.g = (first.g + second.g + third.g + forth.g) / 4;
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;
            averaged.a = (first.a + second.a + third.a + forth.a) / 4;

            result[index1] = averaged;
            result[index2] = averaged;
            result[index3] = averaged;
            result[index4] = averaged;
        }
    }

    return result;
}

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
        dithering ? IO::FileMetadata::DITHERING_FLAG : 0,
        width,
        height,
        indecesSize);

    metadata->writeTo(outputStream);
}