#include "decode.hpp"
#include <iostream>

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media from a certain type");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "bmp|jpg|jpeg|png", "Type of the output media", {"type"});
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

    if (!IO::isFileCGUCompatible(from->Get())) {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);
        return EXIT_FAILURE;
    }

    SDL_Surface* input = IO::readFileCGU(from->Get());
    if (input == NULL){
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    IO::FileMetadata metadata = IO::readMetadataFromFileCGU(from->Get());

    int result;

    switch (metadata.getConvertion()) {
        case IO::CONVERSION_TYPES::NATIVE_RGB:
            result = Converter::convertFromCGUNativeRGB(input);
            break;
        case IO::CONVERSION_TYPES::NATIVE_BW:
            result = Converter::convertFromCGUNativeBW(input);
            break;
        case IO::CONVERSION_TYPES::PALETTE_RGB:
            result = Converter::convertFromCGUPaletteRGB(input);
            break;
        case IO::CONVERSION_TYPES::PALETTE_BW:
            result = Converter::convertFromCGUPaletteBW(input);
            break;
        default:
            Validator::throwValueFlagInvalidException("conversion");
            return EXIT_FAILURE;
    }

    if (result != EXIT_SUCCESS){
        return EXIT_FAILURE;
    };

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

    return EXIT_SUCCESS;
};