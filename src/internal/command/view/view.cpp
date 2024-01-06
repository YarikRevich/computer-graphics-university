#include "view.hpp"

View::View(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "view", "Opens viewport for a specified media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
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

    if (!IO::isFileCGUCompatible(from->Get())) {
        Logger::SetError(FILE_NOT_COMPATIBLE_EXCEPTION);
        return EXIT_FAILURE;
    }

    SDL_Surface* surface = IO::readFileCGU(from->Get());
    if (surface == NULL){
        return EXIT_FAILURE;
    }

    if (window->handle(surface) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}