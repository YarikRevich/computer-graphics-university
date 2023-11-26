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

    switch (IO::getType(type->Get())) {
        case IO::TYPES::JPG:
            break;
        case IO::TYPES::PNG:
            break;
        default:
            Validator::throwValueFlagInvalidException("type");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
};