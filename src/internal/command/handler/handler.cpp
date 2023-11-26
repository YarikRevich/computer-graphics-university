#include "handler.hpp"
#include <iostream>

Handler::Handler(int argc, char **argv) : argc(argc), argv(argv) {
    this->argumentParser = new args::ArgumentParser("A codec for CGU media format.\nStand with Ukraine!");

    this->decode = new Decode(argumentParser);
    this->encode = new Encode(argumentParser);
    this->view = new View(argumentParser);
    this->help = new Help(argumentParser);
    this->validator = new Validator(argumentParser);
}

int Handler::run() {
    try
    {
        argumentParser->ParseCLI(argc, argv);
    }
    catch (...)
    {
        if (validator->handleArgumentParser(std::current_exception()) != EXIT_SUCCESS){
            return EXIT_FAILURE;
        };
    }

    if (decode->isCalled()) {
        return decode->handle();
    }

    if (encode->isCalled()) {
        return encode->handle();
    }

    if (view->isCalled()) {
        return view->handle();
    }

    return EXIT_SUCCESS;
}