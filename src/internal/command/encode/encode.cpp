#include "encode.hpp"
#include <iostream>

Encode::Encode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the source media", {"type"});
    this->conversion = new args::ValueFlag<std::string>(*group, "native_rgb|native_bw|palette_rgb|palette_bw", "Type of the media conversion", {"conversion"});
    this->compression = new args::ValueFlag<bool>(*group, "true|false", "Enable compression", {"conversion"});
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

    int result;

    switch (IO::getConversionType(conversion->Get())) {
        case IO::CONVERSION_TYPES::NATIVE_RGB:
            result = Converter::convertToCGUNativeRGB(input);
            break;
        case IO::CONVERSION_TYPES::NATIVE_BW:
            result = Converter::convertToCGUNativeBW(input);
            break;
        case IO::CONVERSION_TYPES::PALETTE_RGB:
            result = Converter::convertToCGUPaletteRGB(input);
            break;
        case IO::CONVERSION_TYPES::PALETTE_BW:
            result = Converter::convertToCGUPaletteBW(input);
            break;
        default:
            Validator::throwValueFlagInvalidException("conversion");
            return EXIT_FAILURE;
    }

    if (result != EXIT_SUCCESS){
        return EXIT_FAILURE;
    };

    return IO::writeFileCGU(
        to->Get(), 
        IO::composeMetadata(
            IO::getConversionType(conversion->Get())), 
        input);
}