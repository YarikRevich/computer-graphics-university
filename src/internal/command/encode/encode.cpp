#include "encode.hpp"

Encode::Encode(args::ArgumentParser* argumentParser) {
    this->command = new args::Command(*argumentParser, "encode", "Encodes given media");
    args::Group* group = new args::Group(*command, "");
    this->from = new args::ValueFlag<std::string>(*group, "path", "Path to the source media", {"from"});
    this->type = new args::ValueFlag<std::string>(*group, "jpg|jpeg|png", "Type of the source media", {"type"});
    this->to = new args::ValueFlag<std::string>(*group, "path", "Path to the output media", {"to"});
}

bool Encode::isCalled() {
    return command->Matched();
}

int Encode::handle() {
    if (!from->Matched()){
        Validator::throwValueFlagException("from");
        return EXIT_FAILURE;
    }

    if (!type->Matched()){
        Validator::throwValueFlagException("type");
        return EXIT_FAILURE;
    }

    if (!to->Matched()){
        Validator::throwValueFlagException("to");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}