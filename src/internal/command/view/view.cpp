#include "view.hpp"

View::View(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "view", "Opens viewport for a specified media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->debug = new args::Flag(*group, "true|false(default)", "Enables debug mode", {"debug"});;
}

bool View::isCalled() {
    return command->Matched();
}

int View::handle() {
    if (!from->Matched()){
        Validator::throwValueFlagRequiredException("from");
        return EXIT_FAILURE;
    }

    Window* window = new Window();
    if (window->init() != EXIT_SUCCESS) {
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

    inputStream.close();

    if (window->handle(input) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}