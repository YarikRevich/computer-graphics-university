#include "pipeline.hpp"

int Pipeline::handleDecode(std::ifstream &inputStream, bool debug, IO::FILE_TYPES fileType)
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
        input = Converter::convertFromCGUNativeRGB(inputStream, metadata);
        break;
    case IO::CONVERSION_TYPES::NATIVE_BW:
        input = Converter::convertFromCGUNativeBW(inputStream, metadata);
        break;
    case IO::CONVERSION_TYPES::PALETTE_COLORFUL:
        input = Converter::convertFromCGUPaletteRGB(inputStream, metadata);
        break;
    case IO::CONVERSION_TYPES::PALETTE_BW:
        input = Converter::convertFromCGUPaletteBW(inputStream, metadata);
        break;
    default:
        inputStream.close();

        Validator::throwValueFlagInvalidException("conversion");

        return EXIT_FAILURE;
    }

    inputStream.close();

    if (input == NULL)
    {
        return EXIT_FAILURE;
    }

    if (debug->Get())
    {
        if (Converter::convertToCGUPaletteDetected(input) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        };
    }

    switch (IO::getFileType(type->Get()))
    {
    case IO::FILE_TYPES::JPG:
        if (IO::writeFileJPEG(to->Get(), input) != EXIT_SUCCESS)
        {
            return EXIT_SUCCESS;
        };
        break;
    case IO::FILE_TYPES::PNG:
        if (IO::writeFilePNG(to->Get(), input) != EXIT_SUCCESS)
        {
            return EXIT_SUCCESS;
        };
        break;
    case IO::FILE_TYPES::BMP:
        if (IO::writeFileBMP(to->Get(), input) != EXIT_SUCCESS)
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
    IO::COMPRESSION_TYPES compressionType,
    IO::FILTER_TYPES filterType,
    bool dithering,
    std::ofstream &outputStream)
{
}

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

//     Converter::composeMetadata(
//         conversion->Get(),
//         bit->Get(),
//         model->Get(),
//         compression->Get(),
//         dithering->Get(),
//         input->w,
//         input->h,
//         nullptr,
//         outputStream);