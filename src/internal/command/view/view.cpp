#include "view.hpp"

View::View(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "view", "Opens viewport for a specified media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->debug = new args::ValueFlag<bool>(*group, "true|false(default)", "Enables debug view mode", {"debug"});;
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

    IO::FileMetadata* metadata = IO::readMetadataFromFileCGU(from->Get());
    if (metadata == NULL) {
        return EXIT_FAILURE;
    }

    if (!metadata->getCompatible()) {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);
        return EXIT_FAILURE;
    }

    SDL_Surface* surface;
    
    if (metadata->getOptimal()) {
        surface = IO::readFileCGUOptimal(from->Get());
    } else {
        surface = IO::readFileCGUDefault(from->Get());
    }
     
    if (surface == NULL){
        return EXIT_FAILURE;
    }

    if (window->handle(surface) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}