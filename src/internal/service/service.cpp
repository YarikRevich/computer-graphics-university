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

std::vector<SDL_Color> Service::sampleFourToOneRGB(std::vector<SDL_Color> &colors, SDL_Surface *surface)
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

            result[index1] = averaged;
            result[index2] = averaged;
            result[index3] = averaged;
            result[index4] = averaged;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYUV(std::vector<SDL_Color> &colors, SDL_Surface *surface)
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
            averaged.g = (first.g + second.g + third.g + forth.g) / 4;
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;

            result[index1] = averaged;
            result[index2] = averaged;
            result[index3] = averaged;
            result[index4] = averaged;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYIQ(std::vector<SDL_Color> &colors, SDL_Surface *surface)
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
            averaged.g = (first.g + second.g + third.g + forth.g) / 4;
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;

            result[index1] = averaged;
            result[index2] = averaged;
            result[index3] = averaged;
            result[index4] = averaged;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneYCbCr(std::vector<SDL_Color> &colors, SDL_Surface *surface)
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
            averaged.g = (first.g + second.g + third.g + forth.g) / 4;
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;

            result[index1] = averaged;
            result[index2] = averaged;
            result[index3] = averaged;
            result[index4] = averaged;
        }
    }

    return result;
}

std::vector<SDL_Color> Service::sampleFourToOneHSL(std::vector<SDL_Color> &colors, SDL_Surface *surface)
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
            averaged.b = (first.b + second.b + third.b + forth.b) / 4;

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

std::vector<Sint16> Service::compressByteRunImageUint16(std::vector<Uint16> image)
{
    std::vector<Sint16> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 127)
            {
                j++;
            }

            result.push_back(-j);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 128)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 128))
            {
                j++;
            }

            result.push_back(j - 1);
            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            i += j;
        }
    }

    return result;
}

std::vector<Uint16> Service::decompressByteRunImageUint16(std::vector<Sint16> image)
{
    std::vector<Uint16> result;

    for (int i = 0; i < image.size();)
    {
        if (i + 1 >= image.size() - 1)
        {
            break;
        }

        if (image.at(i) < 0)
        {
            int repeatCount = -image.at(i);

            Uint16 value = static_cast<Uint16>(image.at(i + 1));

            for (int j = 0; j <= repeatCount; j++)
            {
                result.push_back(value);
            }

            i += 2;
        }
        else
        {
            int copyCount = image.at(i) + 1;

            for (int j = 0; j < copyCount; j++)
            {
                if (i + 1 + j >= image.size() - 1)
                {
                    continue;
                }

                result.push_back(static_cast<Uint16>(image.at(i + 1 + j)));
            }

            i += (1 + copyCount);
        }
    }

    return result;
}

std::vector<Sint8> Service::compressByteRunImageUint8(std::vector<Uint8> image)
{
    std::vector<Sint8> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 127)
            {
                j++;
            }

            result.push_back(-j);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 128)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 128))
            {
                j++;
            }

            result.push_back(j - 1);
            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            i += j;
        }
    }

    return result;
}

std::vector<Uint8> Service::decompressByteRunImageUint8(std::vector<Sint8> image)
{
    std::vector<Uint8> result;

    for (int i = 0; i < image.size();)
    {
        if (i + 1 >= image.size() - 1)
        {
            break;
        }

        if (image.at(i) < 0)
        {
            int repeatCount = -image.at(i);

            Uint8 value = static_cast<Uint8>(image.at(i + 1));

            for (int j = 0; j <= repeatCount; j++)
            {
                result.push_back(value);
            }

            i += 2;
        }
        else
        {
            int copyCount = image.at(i) + 1;

            for (int j = 0; j < copyCount; j++)
            {
                if (i + 1 + j >= image.size() - 1)
                {
                    continue;
                }

                result.push_back(static_cast<Uint8>(image.at(i + 1 + j)));
            }

            i += (1 + copyCount);
        }
    }

    return result;
}

std::vector<int> Service::compressByteRunImageInt(std::vector<int> image)
{
    std::vector<int> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 127)
            {
                j++;
            }

            result.push_back(-j);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 128)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 128))
            {
                j++;
            }

            result.push_back(j - 1);
            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            i += j;
        }
    }

    return result;
}

std::vector<int> Service::decompressByteRunImageInt(std::vector<int> image)
{
    std::vector<int> result;

    for (int i = 0; i < image.size();)
    {
        if (i + 1 >= image.size() - 1)
        {
            break;
        }

        if (image.at(i) < 0)
        {
            int repeatCount = -image.at(i);

            int value = static_cast<int>(image.at(i + 1));

            for (int j = 0; j <= repeatCount; j++)
            {
                result.push_back(value);
            }

            i += 2;
        }
        else
        {
            int copyCount = image.at(i) + 1;

            for (int j = 0; j < copyCount; j++)
            {
                if (i + 1 + j >= image.size() - 1)
                {
                    continue;
                }

                result.push_back(static_cast<int>(image.at(i + 1 + j)));
            }

            i += (1 + copyCount);
        }
    }

    return result;
}

std::vector<Uint16> Service::compressRLEImageUint16(std::vector<Uint16> image)
{
    std::vector<Uint16> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 254)
            {
                j++;
            }

            result.push_back(j + 1);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 254)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 254))
            {
                j++;
            }

            result.push_back(0);
            result.push_back(j);

            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            if (j % 2 != 0)
            {
                result.push_back(0);
            }

            i += j;
        }
    }

    return result;
}

std::vector<Uint16> Service::decompressRLEImageUint16(std::vector<Uint16> image)
{
    std::vector<Uint16> result;

    int i = 0;
    while (i < image.size())
    {
        Uint16 byte1 = image[i];
        Uint16 byte2 = image[i + 1];

        if (byte1 == 0)
        {
            i += 2;

            int repetitions = byte2;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(image[i]);
                i++;
            }

            if (i % 2 == 1)
            {
                i++;
            }
        }
        else
        {
            int repetitions = byte1;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(byte2);
            }

            i += 2;
        }
    }

    return result;
}

std::vector<Uint8> Service::compressRLEImageUint8(std::vector<Uint8> image)
{
    std::vector<Uint8> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 254)
            {
                j++;
            }

            result.push_back(j + 1);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 254)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 254))
            {
                j++;
            }

            result.push_back(0);
            result.push_back(j);

            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            if (j % 2 != 0)
            {
                result.push_back(0);
            }

            i += j;
        }
    }

    return result;
}

std::vector<Uint8> Service::decompressRLEImageUint8(std::vector<Uint8> image)
{
    std::vector<Uint8> result;

    int i = 0;
    while (i < image.size())
    {
        Uint8 byte1 = image[i];
        Uint8 byte2 = image[i + 1];

        if (byte1 == 0)
        {
            i += 2;

            int repetitions = byte2;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(image[i]);
                i++;
            }

            if (i % 2 == 1)
            {
                i++;
            }
        }
        else
        {
            int repetitions = byte1;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(byte2);
            }

            i += 2;
        }
    }

    return result;
}

std::vector<int> Service::compressRLEImageInt(std::vector<int> image)
{
    std::vector<int> result;

    int i = 0;

    while (i < image.size())
    {
        if ((i < image.size() - 1) && (image.at(i) == image.at(i + 1)))
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) == image.at(i + j + 1)) && j < 254)
            {
                j++;
            }

            result.push_back(j + 1);
            result.push_back(image.at(i + j));

            i += (j + 1);
        }
        else
        {
            int j = 0;

            while ((i + j < image.size() - 1) && (image.at(i + j) != image.at(i + j + 1) && (j < 254)))
            {
                j++;
            }

            if ((i + j == image.size() - 1) && (j < 254))
            {
                j++;
            }

            result.push_back(0);
            result.push_back(j);

            for (int k = 0; k < j; k++)
            {
                result.push_back(image.at(i + k));
            }

            if (j % 2 != 0)
            {
                result.push_back(0);
            }

            i += j;
        }
    }

    return result;
}

std::vector<int> Service::decompressRLEImageInt(std::vector<int> image)
{
    std::vector<int> result;

    int i = 0;
    while (i < image.size())
    {
        int byte1 = image[i];
        int byte2 = image[i + 1];

        if (byte1 == 0)
        {
            i += 2;

            int repetitions = byte2;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(image[i]);
                i++;
            }

            if (i % 2 == 1)
            {
                i++;
            }
        }
        else
        {
            int repetitions = byte1;

            for (int j = 0; j < repetitions; j++)
            {
                result.push_back(byte2);
            }

            i += 2;
        }
    }

    return result;
}

std::vector<Processor::LZ77Result<Uint16> *> Service::compressLZ77ImageUint16(std::vector<Uint16> image)
{
    std::vector<Processor::LZ77Result<Uint16> *> result;

    int match_distance = 0;
    int match_length = 0;
    int start_window = 0;

    for (int i = 0; i < image.size();)
    {
        match_distance = 0;
        match_length = 0;
        start_window = 0;

        Uint16 value = image[i];

        if (i - LZ77_WINDOW_SIZE > 0)
        {
            start_window = i - LZ77_WINDOW_SIZE;
        }

        for (int j = start_window; j < i; j++)
        {
            int length = 0;

            while (length < LZ77_BUFFER_SIZE &&
                   (i + length) < image.size() &&
                   image[j + length] == image[i + length])
            {
                length++;
            }

            if (length > match_length)
            {
                match_distance = i - j;
                match_length = length;
            }
        }

        if (match_length > 0)
        {
            value = image[i + match_length];
        }

        result.push_back(new Processor::LZ77Result(match_distance, match_length, value));

        i += match_length + 1;
    }

    return result;
}

std::vector<Uint16> Service::decompressLZ77ImageUint16(std::vector<Processor::LZ77Result<Uint16> *> src)
{
    std::vector<Uint16> result;

    for (const auto &token : src)
    {
        if (token->getDistance() > 0)
        {
            int start = result.size() - token->getDistance();

            for (int i = 0; i < token->getLength(); i++)
            {
                result.push_back(result[start + i]);
            }
        }

        result.push_back(token->getSymbol());
    }

    return result;
}

std::vector<Processor::LZ77Result<Uint8> *> Service::compressLZ77ImageUint8(std::vector<Uint8> image)
{
    std::vector<Processor::LZ77Result<Uint8> *> result;

    int match_distance = 0;
    int match_length = 0;
    int start_window = 0;

    for (int i = 0; i < image.size();)
    {
        match_distance = 0;
        match_length = 0;
        start_window = 0;

        Uint8 value = image[i];

        if (i - LZ77_WINDOW_SIZE > 0)
        {
            start_window = i - LZ77_WINDOW_SIZE;
        }

        for (int j = start_window; j < i; j++)
        {
            int length = 0;

            while (length < LZ77_BUFFER_SIZE &&
                   (i + length) < image.size() &&
                   image[j + length] == image[i + length])
            {
                length++;
            }

            if (length > match_length)
            {
                match_distance = i - j;
                match_length = length;
            }
        }

        if (match_length > 0)
        {
            value = image[i + match_length];
        }

        result.push_back(new Processor::LZ77Result(match_distance, match_length, value));

        i += match_length + 1;
    }

    return result;
}

std::vector<Uint8> Service::decompressLZ77ImageUint8(std::vector<Processor::LZ77Result<Uint8> *> src)
{
    std::vector<Uint8> result;

    for (const auto &token : src)
    {
        if (token->getDistance() > 0)
        {
            int start = result.size() - token->getDistance();

            for (int i = 0; i < token->getLength(); i++)
            {
                result.push_back(result[start + i]);
            }
        }

        result.push_back(token->getSymbol());
    }

    return result;
}

std::vector<Processor::LZ77Result<int> *> Service::compressLZ77ImageInt(std::vector<int> image)
{
    std::vector<Processor::LZ77Result<int> *> result;

    int match_distance = 0;
    int match_length = 0;
    int start_window = 0;

    for (int i = 0; i < image.size();)
    {
        match_distance = 0;
        match_length = 0;
        start_window = 0;

        int value = image[i];

        if (i - LZ77_WINDOW_SIZE > 0)
        {
            start_window = i - LZ77_WINDOW_SIZE;
        }

        for (int j = start_window; j < i; j++)
        {
            int length = 0;

            while (length < LZ77_BUFFER_SIZE &&
                   (i + length) < image.size() &&
                   image[j + length] == image[i + length])
            {
                length++;
            }

            if (length > match_length)
            {
                match_distance = i - j;
                match_length = length;
            }
        }

        if (match_length > 0)
        {
            value = image[i + match_length];
        }

        result.push_back(new Processor::LZ77Result(match_distance, match_length, value));

        i += match_length + 1;
    }

    return result;
}

std::vector<int> Service::decompressLZ77ImageInt(std::vector<Processor::LZ77Result<int> *> src)
{
    std::vector<int> result;

    for (const auto &token : src)
    {
        if (token->getDistance() > 0)
        {
            int start = result.size() - token->getDistance();

            for (int i = 0; i < token->getLength(); i++)
            {
                result.push_back(result[start + i]);
            }
        }

        result.push_back(token->getSymbol());
    }

    return result;
}

Processor::LZWResult<Uint16> *Service::compressLZWImageUint16(std::vector<Uint16> image)
{
    std::vector<int> result;

    int counter = 65535;

    std::map<int, std::vector<Uint16>> compounds;

    for (int i = 0; i < 65535; ++i)
    {
        compounds[i] = {static_cast<Uint16>(i)};
    }

    std::vector<Uint16> current;

    int i = 0;

    for (Uint16 value : image)
    {
        std::vector<Uint16> component = current;

        component.push_back(value);

        if (Processor::isLZWCompoundPresent(compounds, component))
        {
            current = component;
        }
        else
        {
            int key = Processor::getLZWCompoundCounter(compounds, current);
            if (key || !current.empty())
            {
                result.push_back(key);
            }

            compounds[counter] = component;

            counter++;

            current = {value};
        }
    }

    if (!current.empty())
    {
        int key = Processor::getLZWCompoundCounter(compounds, current);
        if (key)
        {
            result.push_back(key);
        }
    }

    return new Processor::LZWResult(compounds, result);
}

std::vector<Uint16> Service::decompressLZWImageUint16(Processor::LZWResult<Uint16> *src)
{
    std::vector<Uint16> result;

    for (int key : src->getResult())
    {
        std::vector<Uint16> compound = src->getCompounds().at(key);

        for (Uint16 value : compound)
        {
            result.push_back(value);
        }
    }

    return result;
}

Processor::LZWResult<Uint8> *Service::compressLZWImageUint8(std::vector<Uint8> image)
{
    std::vector<int> result;

    int counter = 256;

    std::map<int, std::vector<Uint8>> compounds;

    for (int i = 0; i < 256; ++i)
    {
        compounds[i] = {static_cast<Uint8>(i)};
    }

    std::vector<Uint8> current;

    int i = 0;

    for (Uint8 value : image)
    {
        std::vector<Uint8> component = current;

        component.push_back(value);

        if (Processor::isLZWCompoundPresent(compounds, component))
        {
            current = component;
        }
        else
        {
            int key = Processor::getLZWCompoundCounter(compounds, current);
            if (key || !current.empty())
            {
                result.push_back(key);
            }

            compounds[counter] = component;

            counter++;

            current = {value};
        }
    }

    if (!current.empty())
    {
        int key = Processor::getLZWCompoundCounter(compounds, current);
        if (key)
        {
            result.push_back(key);
        }
    }

    return new Processor::LZWResult(compounds, result);
}

std::vector<Uint8> Service::decompressLZWImageUint8(Processor::LZWResult<Uint8> *src)
{
    std::vector<Uint8> result;

    for (int key : src->getResult())
    {
        std::vector<Uint8> compound = src->getCompounds().at(key);

        for (Uint8 value : compound)
        {
            result.push_back(value);
        }
    }

    return result;
}

Processor::LZWResult<int> *Service::compressLZWImageInt(std::vector<int> image)
{
    std::vector<int> result;

    int counter = 65535;

    std::map<int, std::vector<int>> compounds;

    for (int i = 0; i < 65535; ++i)
    {
        compounds[i] = {static_cast<int>(i)};
    }

    std::vector<int> current;

    int i = 0;

    for (int value : image)
    {
        std::vector<int> component = current;

        component.push_back(value);

        if (Processor::isLZWCompoundPresent(compounds, component))
        {
            current = component;
        }
        else
        {
            int key = Processor::getLZWCompoundCounter(compounds, current);
            if (key || !current.empty())
            {
                result.push_back(key);
            }

            compounds[counter] = component;

            counter++;

            current = {value};
        }
    }

    if (!current.empty())
    {
        int key = Processor::getLZWCompoundCounter(compounds, current);
        if (key)
        {
            result.push_back(key);
        }
    }

    return new Processor::LZWResult(compounds, result);
}

std::vector<int> Service::decompressLZWImageInt(Processor::LZWResult<int> *src)
{
    std::vector<int> result;

    for (int key : src->getResult())
    {
        std::vector<int> compound = src->getCompounds().at(key);

        for (int value : compound)
        {
            result.push_back(value);
        }
    }

    return result;
}

std::vector<Uint8> Service::compressDCTImageUint8(std::vector<Uint8> image, SDL_Surface *input)
{
    std::vector<Uint8> result(image.size(), 0);

    Uint8 block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    float dctMatrix[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    for (int x = 0; x <= input->h; x += DCT_BLOCK_SIZE)
    {
        for (int y = 0; y <= input->w; y += DCT_BLOCK_SIZE)
        {
            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < input->h; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < input->w; yy++)
                {
                    block[xx][yy] = image[(x + xx) * input->w + (y + yy)];
                }
            }

            Processor::generateDCTMatrix(block, dctMatrix);

            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < input->h; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < input->w; yy++)
                {
                    result[(x + xx) * input->w + (y + yy)] = Processor::normalizeValue(dctMatrix[xx][yy], 0, 255);
                }
            }
        }
    }

    return result;
}

std::vector<Uint8> Service::decompressDCTImageUint8(std::vector<Uint8> src, int height, int width)
{
    std::vector<Uint8> result(src.size(), 0);

    Uint8 block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    float dctMatrix[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    for (int x = 0; x <= height; x += DCT_BLOCK_SIZE)
    {
        for (int y = 0; y <= width; y += DCT_BLOCK_SIZE)
        {
            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < height; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < width; yy++)
                {
                    dctMatrix[xx][yy] = src[(x + xx) * width + (y + yy)];
                }
            }

            Processor::generateInversedDCTMatrix(dctMatrix, block);

            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < height; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < width; yy++)
                {
                    result[(x + xx) * width + (y + yy)] = block[xx][yy];
                }
            }
        }
    }

    return result;
}

std::vector<int> Service::compressDCTImageInt(std::vector<int> image, SDL_Surface *input)
{
    std::vector<int> result(image.size(), 0);

    int block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    float dctMatrix[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    for (int x = 0; x <= input->h; x += DCT_BLOCK_SIZE)
    {
        for (int y = 0; y <= input->w; y += DCT_BLOCK_SIZE)
        {
            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < input->h; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < input->w; yy++)
                {
                    block[xx][yy] = image[(x + xx) * input->w + (y + yy)];
                }
            }

            Processor::generateDCTMatrix(block, dctMatrix);

            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < input->h; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < input->w; yy++)
                {
                    result[(x + xx) * input->w + (y + yy)] = dctMatrix[xx][yy] * 1000.0;
                }
            }
        }
    }

    return result;
}

std::vector<int> Service::decompressDCTImageInt(std::vector<int> src, int height, int width)
{
    std::vector<int> result(src.size(), 0);

    int block[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    float dctMatrix[DCT_BLOCK_SIZE][DCT_BLOCK_SIZE];

    for (int x = 0; x <= height; x += DCT_BLOCK_SIZE)
    {
        for (int y = 0; y <= width; y += DCT_BLOCK_SIZE)
        {
            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < height; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < width; yy++)
                {
                    dctMatrix[xx][yy] = src[(x + xx) * width + (y + yy)] / 1000.0;
                }
            }

            Processor::generateInversedDCTMatrix(dctMatrix, block);

            for (int xx = 0; xx < DCT_BLOCK_SIZE && x + xx < height; xx++)
            {
                for (int yy = 0; yy < DCT_BLOCK_SIZE && y + yy < width; yy++)
                {
                    result[(x + xx) * width + (y + yy)] = block[xx][yy];
                }
            }
        }
    }

    return result;
}

std::vector<int> Service::applyDifferentialFilterInt(std::vector<int> image, SDL_Surface *input)
{
    std::vector<int> result(image.size());

    for (int y = 0; y < input->h; ++y)
    {
        for (int x = 0; x < input->w; ++x)
        {
            if (x == 0)
            {
                result[y * input->w + x] = image[y * input->w + x];
            }
            else
            {
                result[y * input->w + x] = image[y * input->w + x] - image[y * input->w + (x - 1)];
            }
        }
    }

    return result;
}

std::vector<int> Service::revertDifferentialFilterInt(std::vector<int> image, int height, int width)
{
    std::vector<int> result(image.size());

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (x == 0)
            {
                result[y * width + x] = image[y * width + x];
            }
            else
            {
                result[y * width + x] = image[y * width + x] + result[y * width + (x - 1)];
            }
        }
    }

    return result;
}

std::vector<int> Service::applyLineDifferenceFilterInt(std::vector<int> image, SDL_Surface *input)
{
    std::vector<int> result(image.size());

    for (int y = 0; y < input->w; ++y)
    {
        for (int x = 0; x < input->h; ++x)
        {
            if (x == 0)
            {
                result[y * input->h + x] = image[y * input->h + x];
            }
            else
            {
                result[y * input->h + x] = image[y * input->h + x] - image[y * input->h + (x - 1)];
            }
        }
    }

    return result;
}

std::vector<int> Service::revertLineDifferenceFilterInt(std::vector<int> image, int height, int width)
{
    std::vector<int> result(image.size());

    for (int y = 0; y < width; ++y)
    {
        for (int x = 0; x < height; ++x)
        {
            if (x == 0)
            {
                result[y * height + x] = image[y * height + x];
            }
            else
            {
                result[y * height + x] = image[y * height + x] + result[y * height + (x - 1)];
            }
        }
    }

    return result;
}

std::vector<int> Service::applyAverageFilterInt(std::vector<int> image, SDL_Surface *input)
{
    std::vector<int> result(image.size());

    int left = 0;
    int up = 0;

    for (int y = 0; y < input->h; ++y)
    {
        for (int x = 0; x < input->w; ++x)
        {
            if (x > 0)
            {
                left = image[y * input->w + (x - 1)];
            }
            else
            {
                left = 0;
            }

            if (y > 0)
            {
                up = image[(y - 1) * input->w + x];
            }
            else
            {
                up = 0;
            }

            result[y * input->w + x] = image[y * input->w + x] - ((left + up) / 2);
        }
    }

    return result;
}

std::vector<int> Service::revertAverageFilterInt(std::vector<int> image, int height, int width)
{
    std::vector<int> result(image.size());

    int left = 0;
    int up = 0;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (x > 0)
            {
                left = result[y * width + (x - 1)];
            }
            else
            {
                left = 0;
            }

            if (y > 0)
            {
                up = result[(y - 1) * width + x];
            }
            else
            {
                up = 0;
            }

            result[y * width + x] = image[y * width + x] + ((left + up) / 2);
        }
    }

    return result;
}

std::vector<int> Service::applyPaethFilterInt(std::vector<int> image, SDL_Surface *input)
{
    std::vector<int> result(image.size());

    int a = 0;
    int b = 0;
    int c = 0;

    for (int y = 0; y < input->h; ++y)
    {
        for (int x = 0; x < input->w; ++x)
        {
            if (x > 0)
            {
                a = image[y * input->w + (x - 1)];
            }
            else
            {
                a = 0;
            }

            if (y > 0)
            {
                b = image[(y - 1) * input->w + x];
            }
            else
            {
                b = 0;
            }

            if (x > 0 && y > 0)
            {
                c = image[(y - 1) * input->w + (x - 1)];
            }
            else
            {
                c = 0;
            }

            result[y * input->w + x] = (image[y * input->w + x] - Processor::getPaethPredictor(a, b, c));
        }
    }

    return result;
}

std::vector<int> Service::revertPaethFilterInt(std::vector<int> image, int height, int width)
{
    std::vector<int> result(image.size());

    int a = 0;
    int b = 0;
    int c = 0;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (x > 0)
            {
                a = result[y * width + (x - 1)];
            }
            else
            {
                a = 0;
            }

            if (y > 0)
            {
                b = result[(y - 1) * width + x];
            }
            else
            {
                b = 0;
            }

            if (x > 0 && y > 0)
            {
                c = result[(y - 1) * width + (x - 1)];
            }
            else
            {
                c = 0;
            }

            result[y * width + x] = (image[y * width + x] + Processor::getPaethPredictor(a, b, c));
        }
    }

    return result;
}

void Service::saveMetadata(
    IO::CONVERSION_TYPES conversionType,
    IO::BIT_TYPES bitType,
    IO::MODEL_TYPES modelType,
    IO::LOSSLESS_COMPRESSION_TYPES losslessCompressionType,
    int losslessCompressionSize,
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
        losslessCompressionSize,
        lossyCompressionType,
        samplingType,
        filterType,
        dithering ? IO::FileMetadata::DITHERING_FLAG : 0,
        width,
        height,
        indecesSize);

    metadata->writeTo(outputStream);
}
