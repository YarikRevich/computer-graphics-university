#include "pipeline.hpp"

SDL_Surface *Pipeline::handleView(std::ifstream &inputStream, bool debug)
{
    IO::FileMetadata *metadata = new IO::FileMetadata(inputStream);
    if (!metadata->getCompatible())
    {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);

        return NULL;
    }

    SDL_Surface *input;

    switch (metadata->getConvertion())
    {
    case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
        input = Service::convertFromCGUNativeColorful(inputStream, metadata);

        break;
    case IO::CONVERSION_TYPES::NATIVE_BW:
        input = Service::convertFromCGUNativeBW(inputStream, metadata);

        break;
    case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
        input = Service::convertFromCGUPaletteColorful(inputStream, metadata);
        if (input == NULL)
        {
            return NULL;
        }

        if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
        {
            if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }
        }

        break;
    case IO::CONVERSION_TYPES::PALETTE_BW:
        input = Service::convertFromCGUPaletteBW(inputStream, metadata);
        if (input == NULL)
        {
            return NULL;
        }

        if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
        {
            if (Service::applyBWDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }
        }

        break;
    }

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
    IO::FileMetadata *metadata = new IO::FileMetadata(inputStream);
    if (!metadata->getCompatible())
    {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);

        return EXIT_FAILURE;
    }

    SDL_Surface *input;

    switch (metadata->getConvertion())
    {
    case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
        input = Service::convertFromCGUNativeColorful(inputStream, metadata);

        break;
    case IO::CONVERSION_TYPES::NATIVE_BW:
        input = Service::convertFromCGUNativeBW(inputStream, metadata);

        break;
    case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
        input = Service::convertFromCGUPaletteColorful(inputStream, metadata);
        if (input == NULL)
        {
            return NULL;
        }

        if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
        {
            if (Service::applyColorfulDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }
        }

        break;
    case IO::CONVERSION_TYPES::PALETTE_BW:
        input = Service::convertFromCGUPaletteBW(inputStream, metadata);
        if (input == NULL)
        {
            return NULL;
        }

        if (metadata->getDithering() == IO::FileMetadata::DITHERING_FLAG)
        {
            if (Service::applyBWDithering(input) != EXIT_SUCCESS)
            {
                return NULL;
            }
        }

        break;
    }

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
    // switch (metadata->getConvertion()) {
    //         case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
    //             input = Converter::convertFromCGUNativeRGB(inputStream, metadata);
    //             break;
    //         case IO::CONVERSION_TYPES::NATIVE_BW:
    //             input = Converter::convertFromCGUNativeBW(inputStream, metadata);
    //             break;
    //         case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
    //             input = Converter::convertFromCGUPaletteRGB(inputStream, metadata);
    //             break;
    //         case IO::CONVERSION_TYPES::PALETTE_BW:
    //             input = Converter::convertFromCGUPaletteBW(inputStream, metadata);
    //             break;
    //         default:
    //             inputStream.close();

    //             Validator::throwValueFlagInvalidException("conversion");

    //             return EXIT_FAILURE;
    //     }

    // if (debug->Get()) {
    //         if (Converter::convertToCGUPaletteDetected(input) != EXIT_SUCCESS) {
    //             return EXIT_FAILURE;
    //         };
    //     }

    //     switch (IO::getFileType(type->Get())) {
    //         case IO::FILE_TYPES::JPG:
    //             if (IO::writeFileJPEG(to->Get(), input) != EXIT_SUCCESS){
    //                 return EXIT_SUCCESS;
    //             };
    //             break;
    //         case IO::FILE_TYPES::PNG:
    //             if (IO::writeFilePNG(to->Get(), input) != EXIT_SUCCESS){
    //                 return EXIT_SUCCESS;
    //             };
    //             break;
    //         case IO::FILE_TYPES::BMP:
    //             if (IO::writeFileBMP(to->Get(), input) != EXIT_SUCCESS){
    //                 return EXIT_SUCCESS;
    //             };
    //             break;
    //         default:
    //             Validator::throwValueFlagInvalidException("type");
    //             return EXIT_FAILURE;
    //     }

    // switch (IO::getConversionType(conversion->Get()))
    //     {
    //     case IO::CONVERSION_TYPES::NATIVE_COLORFUL:
    //         if (dithering->Get())
    //         {
    //             result = Converter::convertToCGUNativeRGBDithering(input);
    //         }
    //         else
    //         {
    //             result = Converter::convertToCGUNativeRGB(input);
    //         }

    //         break;
    //     case IO::CONVERSION_TYPES::NATIVE_BW:
    //         if (dithering->Get())
    //         {
    //             result = Converter::convertToCGUNativeBWDithering(input);
    //         }
    //         else
    //         {
    //             result = Converter::convertToCGUNativeBW(input);
    //         }

    //         break;
    //     case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
    //         if (dithering->Get())
    //         {
    //             result = Converter::convertToCGUPaletteRGBDithering(input);
    //         }
    //         else
    //         {
    //             result = Converter::convertToCGUPaletteRGB(input);
    //         }

    //         break;
    //     case IO::CONVERSION_TYPES::PALETTE_BW:
    //         if (dithering->Get())
    //         {
    //             result = Converter::convertToCGUPaletteBWDithering(input);
    //         }
    //         else
    //         {
    //             result = Converter::convertToCGUPaletteBW(input);
    //         }

    //         break;
    //     default:
    //         Validator::throwValueFlagInvalidException("conversion");
    //         return EXIT_FAILURE;
    //     }

    //     if (result != EXIT_SUCCESS)
    //     {
    //         return EXIT_FAILURE;
    //     };

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
        std::optional<std::vector<Uint32>>{},
        outputStream);
}