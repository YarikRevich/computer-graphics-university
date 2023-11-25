#include "view.hpp"

View::View(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "view", "Opens viewport for a specified media");
}

bool View::isCalled() {
    return command->Matched();
}

int View::handle() {
    Window* window = new Window();
    if (window->init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (window->handle() != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    return window->close();
}