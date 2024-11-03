#include "decode.hpp"
#include <iostream>

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media from a certain type");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the output media", {"type"});
    this->debug = new args::Flag(*group, "true|false(default)", "Enables debug mode", {"debug"});;
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Decode::isCalled() {
    return command->Matched();
}

int Decode::handle() {
    if (!from->Matched()){
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched()){
        Validator::throwValueFlagRequiredException("type");
        return EXIT_FAILURE;
    }

    if (!to->Matched()){
        Validator::throwValueFlagRequiredException("to");
        return EXIT_FAILURE;
    }

    std::ifstream inputStream(from->Get(), std::ios_base::binary);
    if (!inputStream.is_open()) {
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    IO::FileMetadata* metadata = new IO::FileMetadata(inputStream);
    if (!metadata->getCompatible()) {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);
        return EXIT_FAILURE;
    }

    SDL_Surface* input;

    switch (metadata->getConvertion()) {
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
            Validator::throwValueFlagInvalidException("conversion");
            return EXIT_FAILURE;
    }

    if (input == NULL) {
        return EXIT_FAILURE;
    }

    if (debug->Get()) {
        if (Converter::convertToCGUPaletteDetected(input) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        };
    }

    switch (IO::getFileType(type->Get())) {
        case IO::FILE_TYPES::JPG:
            if (IO::writeFileJPEG(to->Get(), input) != EXIT_SUCCESS){
                return EXIT_SUCCESS;
            };
            break;
        case IO::FILE_TYPES::PNG:
            if (IO::writeFilePNG(to->Get(), input) != EXIT_SUCCESS){
                return EXIT_SUCCESS;
            };
            break;
        case IO::FILE_TYPES::BMP:
            if (IO::writeFileBMP(to->Get(), input) != EXIT_SUCCESS){
                return EXIT_SUCCESS;
            };
            break;
        default:
            Validator::throwValueFlagInvalidException("type");
            return EXIT_FAILURE;
    }

    inputStream.close();

    return EXIT_SUCCESS;
};