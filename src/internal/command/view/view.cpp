#include "view.hpp"

View::View(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "view", "Opens viewport for a specified media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->optimal = new args::Flag(*group, "true|false(default)", "Enables optional mode of metadata parsing", {"optimal"});;
    this->debug = new args::Flag(*group, "true|false(default)", "Enables debug view mode", {"debug"});;
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

    IO::FileMetadata* metadata;

    if (optimal->Get()) {
        metadata = IO::readMetadataFromFileCGUOptimal(from->Get());
    } else {
        metadata = IO::readMetadataFromFileCGUDefault(from->Get());
    }
    
    if (metadata == NULL) {
        Logger::SetError(METADATA_RETRIEVAL_EXCEPTION);        
        return EXIT_FAILURE;
    }

    if (metadata->getCompatible() != IO::FileMetadata::COMPATIBLE_FLAG) {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);
        return EXIT_FAILURE;
    }

    SDL_Surface* surface;
    
    if (optimal->Get()) {
        surface = IO::readFileCGUOptimal(from->Get(), metadata);
    } else {

        surface = IO::readFileCGUDefault(from->Get());
    }
     
    if (surface == NULL){
        return EXIT_FAILURE;
    }

    if (debug->Get()) {
        if (Converter::convertToCGUPaletteDetected(surface) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        };
    }

    if (window->handle(surface) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}