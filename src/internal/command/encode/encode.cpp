#include "encode.hpp"
#include <iostream>

Encode::Encode(args::ArgumentParser *argumentParser)
{
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    args::Group *group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the source media", {"type"});
    this->conversion = new args::ValueFlag<std::string>(*group, "native_colorful|native_bw|palette_colorful|palette_bw", "Type of the media conversion", {"conversion"});
    this->bit = new args::ValueFlag<std::string>(*group, "24|16|15|7", "Amount of bits used for the media conversion", {"bit"});
    this->model = new args::ValueFlag<std::string>(*group, "rgb|yuv|yiq|ycbcr|hsl", "Type of color model to be used for media conversion", {"model"});
    this->compression = new args::ValueFlag<std::string>(*group, "dct(sampling)|byterun|rle|lzw|lz77", "Type of compression to be used for media conversion", {"compression"});
    this->dithering = new args::Flag(*group, "true|false(default)", "Enables dithering for the output stream", {"dithering"});
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Encode::isCalled()
{
    return command->Matched();
}

int Encode::handle()
{
    if (!from->Matched())
    {
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched())
    {
        Validator::throwValueFlagRequiredException("type");
        return EXIT_FAILURE;
    }

    if (!conversion->Matched())
    {
        Validator::throwValueFlagRequiredException("conversion");
        return EXIT_FAILURE;
    }

    if (!to->Matched())
    {
        Validator::throwValueFlagRequiredException("to");
        return EXIT_FAILURE;
    }

    SDL_Surface *input;

    switch (IO::getFileType(type->Get()))
    {
    case IO::FILE_TYPES::JPG:
        input = IO::readFileJPEG(from->Get());
        break;
    case IO::FILE_TYPES::PNG:
        input = IO::readFilePNG(from->Get());
        break;
    case IO::FILE_TYPES::BMP:
        input = IO::readFileBMP(from->Get());
        break;
    default:
        Validator::throwValueFlagInvalidException("type");
        return EXIT_FAILURE;
    }

    if (input == NULL)
    {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    std::ofstream outputStream(to->Get(), std::ios_base::out | std::ios_base::binary);
    if (!outputStream.is_open())
    {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    int result;

    switch (IO::getConversionType(conversion->Get()))
    {
    case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
        if (dithering->Get())
        {
            result = Converter::convertToCGUNativeRGBDithering(input);
        }
        else
        {
            result = Converter::convertToCGUNativeRGB(input);
        }

        break;
    case IO::CONVERSION_TYPES::NATIVE_BW:
        if (dithering->Get())
        {
            result = Converter::convertToCGUNativeBWDithering(input);
        }
        else
        {
            result = Converter::convertToCGUNativeBW(input);
        }

        break;
    case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
        if (dithering->Get())
        {
            result = Converter::convertToCGUPaletteRGBDithering(input);
        }
        else
        {
            result = Converter::convertToCGUPaletteRGB(input);
        }

        break;
    case IO::CONVERSION_TYPES::PALETTE_BW:
        if (dithering->Get())
        {
            result = Converter::convertToCGUPaletteBWDithering(input);
        }
        else
        {
            result = Converter::convertToCGUPaletteBW(input);
        }

        break;
    default:
        Validator::throwValueFlagInvalidException("conversion");
        return EXIT_FAILURE;
    }

    if (result != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    };

    Converter::composeMetadata(
        conversion->Get(),
        bit->Get(),
        model->Get(),
        compression->Get(),
        dithering->Get(),
        input->w,
        input->h,
        nullptr,
        outputStream);

    outputStream.close();

    return EXIT_SUCCESS;
}