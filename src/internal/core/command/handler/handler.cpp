#include "handler.hpp"
#include <iostream>

Handler::Handler(int argc, char **argv) : argc(argc), argv(argv) {
    this->argumentParser = new args::ArgumentParser("This is a test program.", "This goes after the options.");

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
        validator->handle(std::current_exception());
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

    return -1;
}