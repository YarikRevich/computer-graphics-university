#include "decode.hpp"
#include <iostream>

Decode::Decode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "decode", "Decodes given media from a certain type");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "jpg|jpeg|png", "Type of the output media", {"type"});
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

    SDL_Surface* input = IO::readFileCGU(from->Get());
    if (input == NULL){
        Validator::throwValueFlagInvalidException("from");
        return EXIT_FAILURE;
    }

    if (Processor::convertFromCGU(input) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    };

    switch (IO::getType(type->Get())) {
        case IO::TYPES::JPG:
            if (IO::writeFileJPEG(to->Get(), input) != EXIT_SUCCESS){
                return EXIT_SUCCESS;
            };
            break;
        case IO::TYPES::PNG:
            if (IO::writeFilePNG(to->Get(), input) != EXIT_SUCCESS){
                return EXIT_SUCCESS;
            };
            break;
        default:
            Validator::throwValueFlagInvalidException("type");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};