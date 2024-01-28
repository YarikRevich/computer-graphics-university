#include "encode.hpp"
#include <iostream>

Encode::Encode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the source media", {"type"});
    this->conversion = new args::ValueFlag<std::string>(*group, "native_rgb|native_bw|palette_rgb|palette_bw", "Type of the media conversion", {"conversion"});
    this->dithering = new args::Flag(*group, "true|false(default)", "Enables dithering for the output stream", {"dithering"});
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Encode::isCalled() {
    return command->Matched();
}

int Encode::handle() {
    if (!from->Matched()){
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched()){
        Validator::throwValueFlagRequiredException("type");
        return EXIT_FAILURE;
    }

    if (!conversion->Matched()) {
        Validator::throwValueFlagRequiredException("conversion");
        return EXIT_FAILURE;
    }

    if (!to->Matched()){
        Validator::throwValueFlagRequiredException("to");
        return EXIT_FAILURE;
    }

    SDL_Surface* input;

    switch (IO::getFileType(type->Get())) {
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

    if (input == NULL) {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    std::ofstream outputStream(to->Get(), std::ios_base::out | std::ios_base::binary);
    if (!outputStream.is_open()) {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    IO::FileMetadata* metadata;

    int result;

    switch (IO::getConversionType(conversion->Get())) {
        case IO::CONVERSION_TYPES::NATIVE_RGB:
            metadata = 
                IO::composeNativeMetadata(
                    IO::CONVERSION_TYPES::NATIVE_RGB, dithering->Get(), input->w, input->h);

            metadata->writeTo(outputStream);

            if (dithering->Get()) {
                result = Converter::convertToCGUNativeRGBDithering(input, outputStream);
            } else {
                result = Converter::convertToCGUNativeRGB(input, outputStream);
            }

            break;
        case IO::CONVERSION_TYPES::NATIVE_BW:
            metadata = 
                IO::composeNativeMetadata(
                    IO::CONVERSION_TYPES::NATIVE_BW, dithering->Get(), input->w, input->h);

            metadata->writeTo(outputStream);

            if (dithering->Get()) {
                result = Converter::convertToCGUNativeBWDithering(input, outputStream);
            } else {
                result = Converter::convertToCGUNativeBW(input, outputStream);
            }
            break;
        case IO::CONVERSION_TYPES::PALETTE_RGB:
            metadata = 
                IO::composeIndecesMetadata(
                    IO::CONVERSION_TYPES::PALETTE_RGB, dithering->Get(), input->w, input->h, State::getPaletteIndeces());

            metadata->writeTo(outputStream);

            if (dithering->Get()) {
                result = Converter::convertToCGUPaletteRGBDithering(input, outputStream);
            } else {
                result = Converter::convertToCGUPaletteRGB(input, outputStream);
            }
            break;
        case IO::CONVERSION_TYPES::PALETTE_BW:
            metadata = 
                IO::composeIndecesMetadata(
                    IO::CONVERSION_TYPES::PALETTE_BW, dithering->Get(), input->w, input->h, State::getPaletteIndeces());

            metadata->writeTo(outputStream);

            if (dithering->Get()) {
                result = Converter::convertToCGUPaletteBWDithering(input, outputStream);
            } else {
                result = Converter::convertToCGUPaletteBW(input, outputStream);
            }
            break;
        default:
            Validator::throwValueFlagInvalidException("conversion");
            return EXIT_FAILURE;
    }

    if (result != EXIT_SUCCESS){
        return EXIT_FAILURE;
    };

    outputStream.close();

    return EXIT_SUCCESS;
}