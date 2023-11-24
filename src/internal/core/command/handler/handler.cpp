#include "handler.hpp"

Handler::Handler(int argc, char **argv) : argc(argc), argv(argv) {
    this->argumentParser = new args::ArgumentParser("This is a test program.", "This goes after the options.");

    this->decode = new Decode(argumentParser);
    this->encode = new Encode(argumentParser);
    this->help = new Help(argumentParser);
}

void Handler::run() {
    try
    {
        argumentParser->ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
    }
    catch (const args::Help&)
    {
        std::cout << *argumentParser;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << *argumentParser;
    }
}