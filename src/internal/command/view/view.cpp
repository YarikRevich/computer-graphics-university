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

    SDL_Surface* input = Pipeline::handleView(inputStream, debug->Get());

    inputStream.close();

    if (input == NULL) {
        return EXIT_FAILURE;
    }

    if (window->handle(input) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}